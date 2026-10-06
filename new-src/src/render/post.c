/* After the scene: the picture is rendered into an HDR target (multisampled when MSAA is on),
 * resolved, and finished in a few full-screen passes:
 *
 *   ambient occlusion  (half resolution) from the depth alone: a hemisphere of samples around each
 *                      point, blurred - corners and contacts darken
 *   the room's look    (the game's own, per room: render.h RoomLook) - a blurred half-size copy
 *                      of the screen for the tint's glow and contrast, and the areas the room's
 *                      bloom mask marks (the second colour target) blurred for its bloom
 *   bloom              bright parts downsampled into a chain of smaller targets and added back up
 *                      (each step a small blur): glows spread wider the brighter they are
 *   composite          occlusion, fog by distance, bloom, exposure, tone mapping, saturation and
 *                      contrast, vignette, film grain, and the display encoding
 *
 * Every pass is a single triangle covering the target (its corners made from gl_VertexID). */
#include "post.h"

#include "../platform/gl.h"

#include <stdio.h>
#include <string.h>

#define BLOOM_LEVELS 6

typedef struct Targets {
    int w, h, msaa;
    GLuint ms_fbo, ms_color, ms_mask, ms_depth;   /* renderbuffers, when msaa > 1 */
    GLuint fbo, color, mask, depth;      /* the resolved scene: textures (mask: the bloom mask) */
    int half_w, half_h;
    GLuint soft_fbo[2], soft[2];         /* the screen at half size, then blurred */
    GLuint glow_fbo[2], glow[2];         /* the bloom mask's areas at half size, then blurred */
    int ao_w, ao_h;
    GLuint ao_fbo[2], ao[2];             /* raw, blurred */
    int bloom_w[BLOOM_LEVELS], bloom_h[BLOOM_LEVELS];
    GLuint bloom_fbo[BLOOM_LEVELS], bloom[BLOOM_LEVELS];
} Targets;

static Targets T;
static GLuint sEmptyVao, sLinear, sAoProg, sBlurProg, sDownProg, sUpProg, sCompositeProg;

/* ---- shaders ---- */

static const char *kFullscreenVs =
    "#version 460 core\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "    vec2 p = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);\n"
    "    v_uv = p;\n"
    "    gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);\n"
    "}\n";

/* the view-space position of a depth sample, from the projection's terms (a, b, P10, P14) */
#define VIEW_POS_GLSL                                                                       \
    "vec3 view_pos(vec2 uv, float d) {\n"                                                   \
    "    float z = -u_proj.w / (d * 2.0 - 1.0 + u_proj.z);\n"                               \
    "    vec2 ndc = uv * 2.0 - 1.0;\n"                                                      \
    "    return vec3(ndc.x * -z / u_proj.x, ndc.y * -z / u_proj.y, z);\n"                   \
    "}\n"

static const char *kAoFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_depth;\n"
    "layout(location = 0) uniform vec4 u_proj;\n"     /* P[0], P[5], P[10], P[14] */
    "layout(location = 1) uniform float u_radius;\n"
    "out float o_ao;\n"
    VIEW_POS_GLSL
    "vec3 at(vec2 uv) { return view_pos(uv, textureLod(u_depth, uv, 0.0).r); }\n"
    "void main() {\n"
    "    float d = textureLod(u_depth, v_uv, 0.0).r;\n"
    "    if (d >= 1.0) { o_ao = 1.0; return; }\n"
    "    vec3 p = view_pos(v_uv, d);\n"
    /* the normal from the neighbours: the nearer side each way (no smearing across edges) */
    "    vec2 t = 1.0 / vec2(textureSize(u_depth, 0));\n"
    "    vec3 l = p - at(v_uv - vec2(t.x, 0.0)), r = at(v_uv + vec2(t.x, 0.0)) - p;\n"
    "    vec3 b = p - at(v_uv - vec2(0.0, t.y)), u = at(v_uv + vec2(0.0, t.y)) - p;\n"
    "    vec3 n = normalize(cross(abs(l.z) < abs(r.z) ? l : r, abs(b.z) < abs(u.z) ? b : u));\n"
    "    if (dot(n, p) > 0.0) n = -n;\n"
    /* a hemisphere of 16 samples (a spiral), turned by a per-pixel angle */
    "    float spin = 6.2831853 * fract(52.9829189 * fract(dot(gl_FragCoord.xy, vec2(0.06711056, 0.00583715))));\n"
    "    vec3 side = normalize(abs(n.x) < 0.9 ? cross(n, vec3(1, 0, 0)) : cross(n, vec3(0, 1, 0)));\n"
    "    vec3 other = cross(n, side);\n"
    "    float hidden = 0.0;\n"
    "    for (int i = 0; i < 16; i++) {\n"
    "        float k = (float(i) + 0.5) / 16.0;\n"
    "        float h = sqrt(1.0 - k), a = float(i) * 2.3999632 + spin;\n"
    "        vec3 dir = side * (cos(a) * sqrt(k)) + other * (sin(a) * sqrt(k)) + n * h;\n"
    "        vec3 q = p + dir * u_radius * mix(0.15, 1.0, k * k);\n"
    "        vec2 quv = vec2(u_proj.x * q.x, u_proj.y * q.y) / -q.z * 0.5 + 0.5;\n"
    "        float sz = at(quv).z;\n"
    "        float near = smoothstep(0.0, 1.0, u_radius / max(abs(p.z - sz), 1e-3));\n"
    "        hidden += (sz >= q.z + 0.02 * u_radius ? 1.0 : 0.0) * near;\n"
    "    }\n"
    "    o_ao = 1.0 - hidden / 16.0;\n"
    "}\n";

static const char *kBlurFs =   /* a 4 x 4 box, which also undoes the per-pixel spin */
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out float o_ao;\n"
    "void main() {\n"
    "    vec2 t = 1.0 / vec2(textureSize(u_src, 0));\n"
    "    float s = 0.0;\n"
    "    for (int y = -2; y < 2; y++)\n"
    "        for (int x = -2; x < 2; x++)\n"
    "            s += texture(u_src, v_uv + (vec2(x, y) + 0.5) * t).r;\n"
    "    o_ao = s / 16.0;\n"
    "}\n";

static const char *kDownFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "layout(location = 0) uniform int u_prefilter;\n"
    "layout(location = 1) uniform float u_threshold;\n"
    "layout(location = 2) uniform int u_masked;\n"   /* only where the bloom mask is set */
    "layout(binding = 1) uniform sampler2D u_mask;\n"
    "out vec4 o_color;\n"
    "vec3 at(vec2 uv) { vec3 c = texture(u_src, uv).rgb; return u_masked != 0 ? c * texture(u_mask, uv).r : c; }\n"
    "void main() {\n"
    "    vec2 t = 1.0 / vec2(textureSize(u_src, 0));\n"
    "    vec3 c = 0.25 * (at(v_uv + t * vec2(-1, -1)) + at(v_uv + t * vec2(1, -1)) +\n"
    "                     at(v_uv + t * vec2(-1, 1)) + at(v_uv + t * vec2(1, 1)));\n"
    "    if (u_prefilter != 0) {\n"   /* only what is brighter than the threshold, with a soft knee */
    "        c = min(c, vec3(64.0));\n"
    "        float br = max(c.r, max(c.g, c.b)), knee = u_threshold * 0.5;\n"
    "        float soft = clamp(br - u_threshold + knee, 0.0, 2.0 * knee);\n"
    "        soft = soft * soft / (4.0 * knee + 1e-4);\n"
    "        c *= max(soft, br - u_threshold) / max(br, 1e-4);\n"
    "    }\n"
    "    o_color = vec4(c, 1.0);\n"
    "}\n";

static const char *kUpFs =   /* a 3 x 3 tent of the smaller level, added onto the larger */
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    vec2 t = 1.0 / vec2(textureSize(u_src, 0));\n"
    "    vec3 c = 4.0 * texture(u_src, v_uv).rgb;\n"
    "    c += 2.0 * (texture(u_src, v_uv + vec2(t.x, 0)).rgb + texture(u_src, v_uv - vec2(t.x, 0)).rgb +\n"
    "                texture(u_src, v_uv + vec2(0, t.y)).rgb + texture(u_src, v_uv - vec2(0, t.y)).rgb);\n"
    "    c += texture(u_src, v_uv + t).rgb + texture(u_src, v_uv - t).rgb +\n"
    "         texture(u_src, v_uv + vec2(t.x, -t.y)).rgb + texture(u_src, v_uv + vec2(-t.x, t.y)).rgb;\n"
    "    o_color = vec4(c / 16.0, 1.0);\n"
    "}\n";

static const char *kCompositeFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_scene;\n"
    "layout(binding = 1) uniform sampler2D u_depth;\n"
    "layout(binding = 2) uniform sampler2D u_ao;\n"
    "layout(binding = 3) uniform sampler2D u_bloom;\n"
    "layout(binding = 4) uniform sampler2D u_soft;\n"
    "layout(binding = 5) uniform sampler2D u_glow;\n"
    "layout(binding = 6) uniform sampler2D u_mask;\n"
    "layout(location = 0) uniform vec4 u_proj;\n"
    "layout(location = 1) uniform float u_ao_strength;\n"
    "layout(location = 2) uniform vec3 u_fog_color;\n"   /* linear */
    "layout(location = 3) uniform float u_fog_density;\n"
    "layout(location = 4) uniform float u_fog_start;\n"
    "layout(location = 5) uniform float u_bloom_strength;\n"
    "layout(location = 6) uniform float u_exposure;\n"
    "layout(location = 7) uniform int u_tonemap;\n"
    "layout(location = 8) uniform float u_saturation;\n"
    "layout(location = 9) uniform float u_contrast;\n"
    "layout(location = 10) uniform float u_vignette;\n"
    "layout(location = 11) uniform float u_grain;\n"
    "layout(location = 12) uniform float u_time;\n"
    "layout(location = 13) uniform int u_debug;\n"   /* 1 occlusion, 2 bloom, 3 depth (black near, white 1500 away), 4 mask */
    "layout(location = 14) uniform vec4 u_fog_near_color;\n"   /* the room's fog (a: 0 off) */
    "layout(location = 15) uniform vec4 u_fog_far_color;\n"
    "layout(location = 16) uniform vec2 u_fog_range;\n"
    "layout(location = 17) uniform vec4 u_tint_glow;\n"   /* the room's tint (a: its strength) */
    "layout(location = 18) uniform vec4 u_tint_contrast;\n"
    "layout(location = 19) uniform vec4 u_room_bloom;\n"   /* the room's bloom (a: signed strength) */
    "out vec4 o_color;\n"
    "vec3 soft_shoulder(vec3 x) {\n"   /* the identity up to 0.8, then easing into 1 */
    "    vec3 over = max(x - 0.8, 0.0);\n"
    "    return min(x, 0.8) + 0.2 * (1.0 - exp(-over / 0.2));\n"
    "}\n"
    "vec3 aces(vec3 x) {\n"   /* Narkowicz's fit of the ACES curve */
    "    return clamp((x * (2.51 * x + 0.03)) / (x * (2.43 * x + 0.59) + 0.14), 0.0, 1.0);\n"
    "}\n"
    "void main() {\n"
    "    if (u_debug == 1) { o_color = vec4(vec3(texture(u_ao, v_uv).r), 1.0); return; }\n"
    "    if (u_debug == 2) { o_color = vec4(pow(texture(u_bloom, v_uv).rgb, vec3(1.0 / 2.2)), 1.0); return; }\n"
    "    if (u_debug == 4) { o_color = vec4(vec3(texture(u_mask, v_uv).r), 1.0); return; }\n"
    "    if (u_debug == 3) { float z = u_proj.w / (texture(u_depth, v_uv).r * 2.0 - 1.0 + u_proj.z);\n"
    "                        o_color = vec4(vec3(clamp(z / 1500.0, 0.0, 1.0)), 1.0); return; }\n"
    "    vec3 c = texture(u_scene, v_uv).rgb;\n"
    "    float ao = texture(u_ao, v_uv).r;\n"
    "    c *= mix(1.0, ao * ao, u_ao_strength);\n"
    "    float d = texture(u_depth, v_uv).r;\n"
    "    float z = u_proj.w / (d * 2.0 - 1.0 + u_proj.z);\n"   /* the distance along the view */
    /* the room's own look, on display values as the PS2 had them: tint, fog, bloom */
    "    vec3 s = pow(max(c, 0.0), vec3(1.0 / 2.2));\n"
    "    vec3 soft = 3.0 * pow(max(texture(u_soft, v_uv).rgb, 0.0), vec3(1.0 / 2.2));\n"
    "    s += soft * u_tint_glow.rgb * u_tint_glow.a;\n"
    /* (the second colour sees the screen after the first: its blurred copy has the glow too) */
    "    s += (s - soft * (1.0 + 3.0 * u_tint_glow.rgb * u_tint_glow.a) * u_tint_contrast.rgb) * u_tint_contrast.a;\n"
    /* (the game starts the ramp sharply at its near distance - its fixed cameras keep things
     * beyond it; ours roam, so it fades in over the last 40% before) */
    "    if (u_fog_near_color.a + u_fog_far_color.a > 0.0 && d < 1.0 && z > u_fog_range.x * 0.6) {\n"
    "        vec4 fc = mix(u_fog_near_color, u_fog_far_color, clamp((z - u_fog_range.x) / max(u_fog_range.y - u_fog_range.x, 1e-3), 0.0, 1.0));\n"
    "        fc.a *= smoothstep(u_fog_range.x * 0.6, u_fog_range.x, z);\n"
    "        s = mix(s, fc.rgb, clamp(fc.a, 0.0, 1.0));\n"
    "    }\n"
    "    s += 5.0 * pow(max(texture(u_glow, v_uv).rgb, 0.0), vec3(1.0 / 2.2)) * u_room_bloom.rgb * u_room_bloom.a;\n"
    "    c = pow(max(s, 0.0), vec3(2.2));\n"
    /* the modern additions: haze, bloom, grading */
    "    if (u_fog_density > 0.0 && d < 1.0) {\n"
    "        float f = 1.0 - exp(-u_fog_density * max(z - u_fog_start, 0.0));\n"
    "        c = mix(c, u_fog_color, f);\n"
    "    }\n"
    "    c += texture(u_bloom, v_uv).rgb * u_bloom_strength;\n"
    "    c *= u_exposure;\n"
    "    c = u_tonemap == 2 ? aces(c) : u_tonemap == 1 ? soft_shoulder(c) : clamp(c, 0.0, 1.0);\n"
    "    float l = dot(c, vec3(0.2126, 0.7152, 0.0722));\n"
    "    c = max(mix(vec3(l), c, u_saturation), 0.0);\n"
    "    s = pow(c, vec3(1.0 / 2.2));\n"   /* to display values */
    "    s = clamp((s - 0.5) * u_contrast + 0.5, 0.0, 1.0);\n"
    "    s *= 1.0 - u_vignette * smoothstep(0.35, 0.95, length((v_uv - 0.5) * vec2(1.4, 1.0)));\n"
    "    float n = fract(sin(dot(gl_FragCoord.xy + fract(u_time) * 97.0, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "    s += (n - 0.5) * u_grain;\n"
    "    o_color = vec4(s, 1.0);\n"
    "}\n";

/* ---- targets ---- */

static GLuint texture2d(GLenum format, int w, int h) {
    GLuint t;

    glCreateTextures(GL_TEXTURE_2D, 1, &t);
    glTextureStorage2D(t, 1, format, w, h);
    glTextureParameteri(t, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(t, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(t, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(t, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return t;
}

static GLuint framebuffer2(GLuint color, GLuint color2, GLuint depth) {
    static const GLenum kBoth[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    GLuint f;

    glCreateFramebuffers(1, &f);
    glNamedFramebufferTexture(f, GL_COLOR_ATTACHMENT0, color, 0);
    if (color2 != 0) {
        glNamedFramebufferTexture(f, GL_COLOR_ATTACHMENT1, color2, 0);
        glNamedFramebufferDrawBuffers(f, 2, kBoth);
    }
    if (depth != 0) {
        glNamedFramebufferTexture(f, GL_DEPTH_ATTACHMENT, depth, 0);
    }
    if (glCheckNamedFramebufferStatus(f, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "post: a framebuffer is incomplete\n");
    }
    return f;
}

static GLuint framebuffer(GLuint color, GLuint depth) {
    return framebuffer2(color, 0, depth);
}

static void free_targets(void) {
    if (T.fbo == 0) {
        return;
    }
    if (T.ms_fbo != 0) {
        glDeleteFramebuffers(1, &T.ms_fbo);
        glDeleteRenderbuffers(1, &T.ms_color);
        glDeleteRenderbuffers(1, &T.ms_mask);
        glDeleteRenderbuffers(1, &T.ms_depth);
    }
    glDeleteFramebuffers(1, &T.fbo);
    glDeleteTextures(1, &T.color);
    glDeleteTextures(1, &T.mask);
    glDeleteFramebuffers(2, T.soft_fbo);
    glDeleteTextures(2, T.soft);
    glDeleteFramebuffers(2, T.glow_fbo);
    glDeleteTextures(2, T.glow);
    glDeleteTextures(1, &T.depth);
    glDeleteFramebuffers(2, T.ao_fbo);
    glDeleteTextures(2, T.ao);
    glDeleteFramebuffers(BLOOM_LEVELS, T.bloom_fbo);
    glDeleteTextures(BLOOM_LEVELS, T.bloom);
    memset(&T, 0, sizeof(T));
}

static void make_targets(int w, int h, int msaa) {
    int i;

    free_targets();
    T.w = w;
    T.h = h;
    T.msaa = msaa;
    T.color = texture2d(GL_RGBA16F, w, h);
    T.mask = texture2d(GL_R8, w, h);
    T.depth = texture2d(GL_DEPTH_COMPONENT32F, w, h);
    glTextureParameteri(T.depth, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(T.depth, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    T.fbo = framebuffer2(T.color, T.mask, T.depth);
    if (msaa > 1) {
        static const GLenum kBoth[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

        glCreateRenderbuffers(1, &T.ms_color);
        glNamedRenderbufferStorageMultisample(T.ms_color, msaa, GL_RGBA16F, w, h);
        glCreateRenderbuffers(1, &T.ms_mask);
        glNamedRenderbufferStorageMultisample(T.ms_mask, msaa, GL_R8, w, h);
        glCreateRenderbuffers(1, &T.ms_depth);
        glNamedRenderbufferStorageMultisample(T.ms_depth, msaa, GL_DEPTH_COMPONENT32F, w, h);
        glCreateFramebuffers(1, &T.ms_fbo);
        glNamedFramebufferRenderbuffer(T.ms_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, T.ms_color);
        glNamedFramebufferRenderbuffer(T.ms_fbo, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, T.ms_mask);
        glNamedFramebufferRenderbuffer(T.ms_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, T.ms_depth);
        glNamedFramebufferDrawBuffers(T.ms_fbo, 2, kBoth);
        if (glCheckNamedFramebufferStatus(T.ms_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "post: the multisampled framebuffer is incomplete\n");
        }
    }
    T.ao_w = w / 2 > 1 ? w / 2 : 1;
    T.ao_h = h / 2 > 1 ? h / 2 : 1;
    for (i = 0; i < 2; i++) {
        T.ao[i] = texture2d(GL_R8, T.ao_w, T.ao_h);
        T.ao_fbo[i] = framebuffer(T.ao[i], 0);
    }
    T.half_w = T.ao_w;
    T.half_h = T.ao_h;
    for (i = 0; i < 2; i++) {
        T.soft[i] = texture2d(GL_RGBA16F, T.half_w, T.half_h);
        T.soft_fbo[i] = framebuffer(T.soft[i], 0);
        T.glow[i] = texture2d(GL_RGBA16F, T.half_w, T.half_h);
        T.glow_fbo[i] = framebuffer(T.glow[i], 0);
    }
    for (i = 0; i < BLOOM_LEVELS; i++) {
        T.bloom_w[i] = (w >> (i + 1)) > 1 ? w >> (i + 1) : 1;
        T.bloom_h[i] = (h >> (i + 1)) > 1 ? h >> (i + 1) : 1;
        T.bloom[i] = texture2d(GL_R11F_G11F_B10F, T.bloom_w[i], T.bloom_h[i]);
        T.bloom_fbo[i] = framebuffer(T.bloom[i], 0);
    }
}

/* ---- passes ---- */

void post_init(void) {
    glCreateVertexArrays(1, &sEmptyVao);
    sAoProg = gl_program(kFullscreenVs, kAoFs, "ssao");
    sBlurProg = gl_program(kFullscreenVs, kBlurFs, "ssao blur");
    sDownProg = gl_program(kFullscreenVs, kDownFs, "bloom down");
    sUpProg = gl_program(kFullscreenVs, kUpFs, "bloom up");
    sCompositeProg = gl_program(kFullscreenVs, kCompositeFs, "composite");
    glCreateSamplers(1, &sLinear);   /* for the passes' inputs: bilinear, clamped */
    glSamplerParameteri(sLinear, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sLinear, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sLinear, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sLinear, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void post_begin(int w, int h, int msaa, Vec3 clear) {
    static const float kNoMask[4] = {0, 0, 0, 0};
    const float colour[4] = {clear.x, clear.y, clear.z, 1.0f}, depth = 1.0f;
    GLuint target;

    if (w != T.w || h != T.h || msaa != T.msaa) {
        make_targets(w, h, msaa);
    }
    target = T.ms_fbo != 0 ? T.ms_fbo : T.fbo;
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glViewport(0, 0, w, h);
    glDepthMask(GL_TRUE);
    glClearNamedFramebufferfv(target, GL_COLOR, 0, colour);
    glClearNamedFramebufferfv(target, GL_COLOR, 1, kNoMask);
    glClearNamedFramebufferfv(target, GL_DEPTH, 0, &depth);
    if (T.ms_fbo != 0) {
        glEnable(GL_MULTISAMPLE);
    }
}

static void fullscreen(GLuint fbo, int w, int h, GLuint prog) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, w, h);
    glUseProgram(prog);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void bind(GLuint unit, GLuint texture, GLuint sampler) {
    glBindTextureUnit(unit, texture);
    glBindSampler(unit, sampler);
}

void post_finish(const PostCamera *cam, const RenderSettings *s, int x, int y, int w, int h, float time) {
    const float *P = cam->proj.m;
    int i;

    if (T.ms_fbo != 0) {   /* the samples averaged into the textures: colour, mask, depth */
        glNamedFramebufferReadBuffer(T.ms_fbo, GL_COLOR_ATTACHMENT0);
        glNamedFramebufferDrawBuffer(T.fbo, GL_COLOR_ATTACHMENT0);
        glBlitNamedFramebuffer(T.ms_fbo, T.fbo, 0, 0, T.w, T.h, 0, 0, T.w, T.h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glNamedFramebufferReadBuffer(T.ms_fbo, GL_COLOR_ATTACHMENT1);
        glNamedFramebufferDrawBuffer(T.fbo, GL_COLOR_ATTACHMENT1);
        glBlitNamedFramebuffer(T.ms_fbo, T.fbo, 0, 0, T.w, T.h, 0, 0, T.w, T.h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBlitNamedFramebuffer(T.ms_fbo, T.fbo, 0, 0, T.w, T.h, 0, 0, T.w, T.h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glNamedFramebufferReadBuffer(T.ms_fbo, GL_COLOR_ATTACHMENT0);
        {
            static const GLenum kBoth[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

            glNamedFramebufferDrawBuffers(T.fbo, 2, kBoth);
        }
    }
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDepthMask(GL_FALSE);
    glBindVertexArray(sEmptyVao);

    if (s->ssao) {
        glProgramUniform4f(sAoProg, 0, P[0], P[5], P[10], P[14]);
        glProgramUniform1f(sAoProg, 1, s->ssao_radius);
        bind(0, T.depth, 0);
        fullscreen(T.ao_fbo[0], T.ao_w, T.ao_h, sAoProg);
        bind(0, T.ao[0], sLinear);
        fullscreen(T.ao_fbo[1], T.ao_w, T.ao_h, sBlurProg);
    }

    /* the room's look: the screen at half size blurred (the tint), the masked areas (the bloom) */
    glProgramUniform1i(sDownProg, 0, 0);
    glProgramUniform1i(sDownProg, 2, 0);
    bind(0, T.color, sLinear);
    fullscreen(T.soft_fbo[0], T.half_w, T.half_h, sDownProg);
    bind(0, T.soft[0], sLinear);
    fullscreen(T.soft_fbo[1], T.half_w, T.half_h, sUpProg);
    glProgramUniform1i(sDownProg, 2, 1);
    bind(0, T.color, sLinear);
    bind(1, T.mask, sLinear);
    fullscreen(T.glow_fbo[0], T.half_w, T.half_h, sDownProg);
    bind(0, T.glow[0], sLinear);
    fullscreen(T.glow_fbo[1], T.half_w, T.half_h, sUpProg);
    bind(0, T.glow[1], sLinear);
    fullscreen(T.glow_fbo[0], T.half_w, T.half_h, sUpProg);
    glProgramUniform1i(sDownProg, 2, 0);

    if (s->bloom) {
        glProgramUniform1f(sDownProg, 1, s->bloom_threshold);
        for (i = 0; i < BLOOM_LEVELS; i++) {
            glProgramUniform1i(sDownProg, 0, i == 0);
            bind(0, i == 0 ? T.color : T.bloom[i - 1], sLinear);
            fullscreen(T.bloom_fbo[i], T.bloom_w[i], T.bloom_h[i], sDownProg);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        for (i = BLOOM_LEVELS - 1; i > 0; i--) {
            bind(0, T.bloom[i], sLinear);
            fullscreen(T.bloom_fbo[i - 1], T.bloom_w[i - 1], T.bloom_h[i - 1], sUpProg);
        }
        glDisable(GL_BLEND);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(x, y, w, h);
    glProgramUniform4f(sCompositeProg, 0, P[0], P[5], P[10], P[14]);
    glProgramUniform1f(sCompositeProg, 1, s->ssao ? s->ssao_strength : 0.0f);
    glProgramUniform3f(sCompositeProg, 2, powf(s->fog_color.x, 2.2f), powf(s->fog_color.y, 2.2f),
                       powf(s->fog_color.z, 2.2f));
    glProgramUniform1f(sCompositeProg, 3, s->fog ? s->fog_density : 0.0f);
    glProgramUniform1f(sCompositeProg, 4, s->fog_start);
    glProgramUniform1f(sCompositeProg, 5, s->bloom ? s->bloom_strength : 0.0f);
    glProgramUniform1f(sCompositeProg, 6, s->exposure);
    glProgramUniform1i(sCompositeProg, 7, s->tonemap);
    glProgramUniform1f(sCompositeProg, 8, s->saturation);
    glProgramUniform1f(sCompositeProg, 9, s->contrast);
    glProgramUniform1f(sCompositeProg, 10, s->vignette);
    glProgramUniform1f(sCompositeProg, 11, s->grain);
    glProgramUniform1f(sCompositeProg, 12, time);
    glProgramUniform1i(sCompositeProg, 13, s->debug);
    bind(0, T.color, sLinear);
    bind(1, T.depth, 0);
    bind(2, T.ao[1], sLinear);
    bind(3, T.bloom[0], sLinear);
    bind(4, T.soft[1], sLinear);
    bind(5, T.glow[0], sLinear);
    bind(6, T.mask, sLinear);
    {
        const RoomLook *l = &gRoomLook;
        int fog = s->room_fog && l->has_fog, tint = s->room_tint && l->has_tint, bloom = s->room_bloom && l->has_bloom;

        glProgramUniform4f(sCompositeProg, 14, l->fog_near_color[0], l->fog_near_color[1], l->fog_near_color[2],
                           fog ? l->fog_near_color[3] : 0.0f);
        glProgramUniform4f(sCompositeProg, 15, l->fog_far_color[0], l->fog_far_color[1], l->fog_far_color[2],
                           fog ? l->fog_far_color[3] : 0.0f);
        glProgramUniform2f(sCompositeProg, 16, l->fog_near, l->fog_far);
        glProgramUniform4f(sCompositeProg, 17, l->tint_glow[0], l->tint_glow[1], l->tint_glow[2], tint ? l->tint_glow[3] : 0.0f);
        glProgramUniform4f(sCompositeProg, 18, l->tint_contrast[0], l->tint_contrast[1], l->tint_contrast[2],
                           tint ? l->tint_contrast[3] : 0.0f);
        glProgramUniform4f(sCompositeProg, 19, l->bloom[0], l->bloom[1], l->bloom[2],
                           bloom ? (l->bloom_subtract ? -l->bloom[3] : l->bloom[3]) : 0.0f);
    }
    glUseProgram(sCompositeProg);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    for (i = 0; i < 7; i++) {
        bind((GLuint)i, 0, 0);
    }
    glBindVertexArray(0);
    (void)cam->znear;
}
