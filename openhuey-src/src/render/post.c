/* After the scene: the picture is rendered into an HDR target (multisampled when MSAA is on),
 * resolved, and finished in a few full-screen passes:
 *
 *   ambient occlusion  (half resolution) from the depth alone: a hemisphere of samples around each
 *                      point, blurred - corners and contacts darken
 *   the room's look    (the game's own, per room: render.h RoomLook) - a blurred half-size copy
 *                      of the screen for the tint's glow and contrast, and the areas the room's
 *                      bloom mask marks (the second colour target) blurred for its bloom
 *   the glow pass      (the original's renderer +0x58) the glowing sprites drawn again into a
 *                      quarter-size buffer kept between frames (against the scene's depth at
 *                      that size), which fades to 3/4 each glowing frame and is added over the
 *                      screen at half: moving glows leave trails
 *   the shine          (layer $14's) the characters in it drawn again at half size, their
 *                      bright parts blurred and added in the colour asked for
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
    GLuint dof_fbo, dof;                 /* ... and blurred more: the depth of field */
    GLuint glow_fbo[2], glow[2];         /* the bloom mask's areas at half size, then blurred */
    int ao_w, ao_h;
    GLuint ao_fbo[2], ao[2];             /* raw, blurred */
    int bloom_w[BLOOM_LEVELS], bloom_h[BLOOM_LEVELS];
    GLuint bloom_fbo[BLOOM_LEVELS], bloom[BLOOM_LEVELS];
} Targets;

static Targets T;
static GLuint sEmptyVao, sLinear, sAoProg, sBlurProg, sDownProg, sUpProg, sCompositeProg;
static GLuint sTrailProg;
static GLuint sPanicProg;   /* (the panic's screens: below) */

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

static const char *kTrailFs =   /* the last frame over this one, at u_a (1 - the original's frame blend) */
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "layout(location = 0) uniform float u_a;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = vec4(texture(u_src, v_uv).rgb, u_a);\n"
    "}\n";

/* the panic's screens (post.h), on the frame's display values (the PS2's 0..255: x 255) */
static const char *kPanicFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "layout(location = 0) uniform int u_mode;\n"     /* 1 negative, 2 zoom blur, 3 tint */
    "layout(location = 1) uniform float u_limit;\n"
    "layout(location = 2) uniform float u_amount;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    vec3 d = floor(texture(u_src, v_uv).rgb * 255.0 + 0.5);\n"
    "    vec3 c;\n"
    "    if (u_mode == 1) {\n"
    "        c = max(vec3(0.0), 128.0 - d);\n"
    "    } else if (u_mode == 2) {\n"   /* (-16 .. 528 x -16 .. 464 from the whole of it) */
    "        vec2 uv = 0.5 + (v_uv - 0.5) * vec2(512.0 / 544.0, 448.0 / 480.0);\n"
    "        vec3 z = floor(texture(u_src, uv).rgb * 255.0 + 0.5);\n"
    "        c = d + floor((z - d) * 64.0 / 128.0);\n"
    "    } else {\n"   /* (the frame's alpha 0x80 where drawn: its low six bits none) */
    "        float idx = floor(d.g / 64.0) * 64.0;\n"
    "        vec3 cs = idx < u_limit ? vec3(0.0) : vec3(192.0);\n"
    "        c = d + floor((cs - d) * u_amount / 256.0);\n"
    "    }\n"
    "    o_color = vec4(c / 255.0, 1.0);\n"
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
    "layout(location = 20) uniform vec4 u_dof;\n"   /* the room's depth of field (x > y: off) */
    "layout(binding = 7) uniform sampler2D u_dof_blur;\n"
    "layout(binding = 8) uniform sampler2D u_glow_trail;\n"   /* the glow pass's buffer */
    "layout(location = 21) uniform float u_glow_k;\n"          /* its strength this frame (0: none) */
    "layout(binding = 9) uniform sampler2D u_shine;\n"         /* layer $14's bright parts, blurred */
    "layout(location = 22) uniform vec3 u_shine_col;\n"        /* in this colour (0: none) */
    "out vec4 o_color;\n"
    "uint mixu(uint x) {\n"   /* an integer hash (lowbias32): no patterns, on any GPU */
    "    x ^= x >> 16; x *= 0x7feb352du; x ^= x >> 15; x *= 0x846ca68bu; x ^= x >> 16;\n"
    "    return x;\n"
    "}\n"
    "float noise(uvec2 p, uint k) { return float(mixu(p.x + mixu(p.y + mixu(k)))) / 4294967295.0; }\n"
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
    /* the room's depth of field: blurred near (to dof.y), sharp, blurred far (from dof.z to dof.w) */
    "    if (u_dof.x <= u_dof.y) {\n"
    "        float zz = d < 1.0 ? z : 1e9;\n"
    "        float near = 1.0 - smoothstep(u_dof.x, u_dof.y, zz);\n"
    "        float far = u_dof.w > u_dof.z ? smoothstep(u_dof.z, u_dof.w, zz) : step(u_dof.z, zz);\n"
    "        c = mix(c, texture(u_dof_blur, v_uv).rgb, clamp(max(near, far), 0.0, 1.0));\n"
    "    }\n"
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
    "    s += u_glow_k * pow(max(texture(u_glow_trail, v_uv).rgb, 0.0), vec3(1.0 / 2.2));\n"

    "    s += 0.5 * texture(u_shine, v_uv).rgb * u_shine_col;\n"
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
    /* film grain (a setting: none by default) - a new pattern each frame, kept out of the
     * shadows, where a dark game would show it as crawling dither */
    "    uvec2 p = uvec2(gl_FragCoord.xy);\n"
    "    if (u_grain > 0.0) {\n"
    "        float n = noise(p, uint(u_time * 30.0) + 1u);\n"
    "        s += (n - 0.5) * u_grain * smoothstep(0.02, 0.35, dot(s, vec3(0.299, 0.587, 0.114)));\n"
    "    }\n"
    /* against banding in the dark gradients going to 8 bits: a fixed (not moving) triangular
     * dither of one step - below what the eye picks out */
    "    float d1 = noise(p, 0u) , d2 = noise(p, 0x9E3779B9u);\n"
    "    s += (d1 + d2 - 1.0) / 255.0;\n"
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
        glNamedFramebufferTexture(f, depth == T.depth ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, depth, 0);
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
    glDeleteFramebuffers(1, &T.dof_fbo);
    glDeleteTextures(1, &T.dof);
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
    T.depth = texture2d(GL_DEPTH32F_STENCIL8, w, h);   /* (the stencil: the reflections' quads) */
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
        glNamedRenderbufferStorageMultisample(T.ms_depth, msaa, GL_DEPTH32F_STENCIL8, w, h);
        glCreateFramebuffers(1, &T.ms_fbo);
        glNamedFramebufferRenderbuffer(T.ms_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, T.ms_color);
        glNamedFramebufferRenderbuffer(T.ms_fbo, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, T.ms_mask);
        glNamedFramebufferRenderbuffer(T.ms_fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, T.ms_depth);
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
        if (i == 0) {
            T.dof = texture2d(GL_RGBA16F, T.half_w, T.half_h);
            T.dof_fbo = framebuffer(T.dof, 0);
        }
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

/* ---- the screen as a TV shows it (TvScreenA / B: the original copies the screen so far at half
 * size through a grey palette): last frame's picture, grey, at half size - made only while asked
 * for (post_tv_want, each frame) ---- */
static const char *kGreyFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    vec3 c = texture(u_src, v_uv).rgb;\n"
    "    float l = sqrt(clamp(dot(c, vec3(0.299, 0.587, 0.114)), 0.0, 1.0));\n"   /* (as the screen shows it) */
    "    o_color = vec4(vec3(l), 1.0);\n"
    "}\n";
/* the haze (LightShaft / Room49Effect: the screen at half size blended half with itself drawn in
 * 33 columns 8 apart, each moved down sin(a) (2 + (1 + cos a) / 2) - a the phase + 90 degrees a
 * column - and all of it 2 sin(phase 2) right; that blended back over the screen at its strength,
 * 0x48 / 128 the light shaft's, 0x60 Room49Effect's) */
static const char *kHazeFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "layout(location = 0) uniform vec2 u_phase;\n"      /* the columns' phase; the sideways shift (texels of 256) */
    "layout(location = 1) uniform vec3 u_k;\n"          /* the wave's height; the strength in the middle, at the edges */
    "out vec4 o_color;\n"
    "void main() {\n"
    "    float col = floor(v_uv.x * 256.0 / 8.0);\n"
    "    float a = u_phase.x + 1.5707963 * col;\n"
    "    float dy = sin(a) * (u_k.x + 0.5 * (1.0 + cos(a))) / 224.0;\n"
    "    float dx = u_phase.y / 256.0;\n"
    "    float k = mix(u_k.z, u_k.y, 1.0 - abs(2.0 * v_uv.x - 1.0));\n"
    "    vec3 c = texture(u_src, v_uv).rgb;\n"
    "    vec3 m = texture(u_src, v_uv + vec2(-dx, dy)).rgb;\n"
    "    o_color = vec4(mix(c, 0.5 * (c + m), k), 1.0);\n"
    "}\n";
static GLuint sHazeProg, sHazeTex, sHazeFbo;
static int sHazeW, sHazeH, sHazeWanted;
static float sHazePhase[2], sHazeK[3];

/* the glow pass's step: the buffer faded to 3/4 (the original: B + (B / 2 - B) / 2), kept to
 * what its 8 bits hold (0xFF: 2 x 0x80) */
static const char *kGlowStepFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = vec4(0.75 * clamp(texture(u_src, v_uv).rgb, 0.0, 2.0), 1.0);\n"
    "}\n";
static GLuint sGlowStepProg;
static GLuint sGlowFbo[2], sGlowTrail[2], sGlowDepth;   /* the buffer (two to step between), its depth */
static GLuint sGlowOf;                                  /* the scene depth it was made for */
static int sGlowW, sGlowH, sGlowCur, sGlowUsed, sGlowClear = 1;
static float sBloomAdd[3];   /* the effects' blooms this frame (Bloom_Start): r g b x strength, signed */

/* the shine (layer $14's end): its pixels drawn and bright enough (green 0x40 and up, of the
 * frame's 0xFF) halved, then each with half of eight round it added */
static const char *kShineFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out vec4 o_color;\n"
    "vec3 bright(vec2 uv) {\n"
    "    vec4 c = texture(u_src, uv);\n"
    "    vec3 d = pow(max(c.rgb, 0.0), vec3(1.0 / 2.2));\n"
    "    return c.a > 0.01 && d.g >= 0.25 ? d : vec3(0.0);\n"
    "}\n"
    "void main() {\n"
    "    vec2 t = 1.0 / vec2(textureSize(u_src, 0));\n"
    "    vec3 c = bright(v_uv);\n"
    "    c += 0.5 * (bright(v_uv + t * vec2(-1, -1)) + bright(v_uv + t * vec2(1, -1)) + bright(v_uv + t * vec2(-1, 1)) +\n"
    "                bright(v_uv + t * vec2(1, 1)) + bright(v_uv + t * vec2(-2, 0)) + bright(v_uv + t * vec2(2, 0)) +\n"
    "                bright(v_uv + t * vec2(0, -2)) + bright(v_uv + t * vec2(0, 2)));\n"
    "    o_color = vec4(c, 1.0);\n"
    "}\n";
/* the softened layer ($1C): its pixels drawn, premultiplied by a coverage of 0 or 1 (blended
 * back stretched: its edges soft) */
static const char *kSoftPrepFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    vec4 c = texelFetch(u_src, ivec2(gl_FragCoord.xy), 0);\n"
    "    float k = c.a > 0.01 ? 1.0 : 0.0;\n"
    "    o_color = vec4(c.rgb * k, k);\n"
    "}\n";
/* ... laid over the scene, stretched (premultiplied: blended ONE, ONE_MINUS_SRC_ALPHA) */
static const char *kSoftOverFs =
    "#version 460 core\n"
    "in vec2 v_uv;\n"
    "layout(binding = 0) uniform sampler2D u_src;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = texture(u_src, v_uv);\n"
    "}\n";
static GLuint sSoftOverProg;
static GLuint sShineProg, sSoftPrepProg;
static int sShineWanted;
static float sShineCol[3];

static GLuint sGreyProg, sTvTex, sTvFbo;
static int sTvW, sTvH, sTvWanted;

void post_init(void) {
    glCreateVertexArrays(1, &sEmptyVao);
    sAoProg = gl_program(kFullscreenVs, kAoFs, "ssao");
    sBlurProg = gl_program(kFullscreenVs, kBlurFs, "ssao blur");
    sDownProg = gl_program(kFullscreenVs, kDownFs, "bloom down");
    sUpProg = gl_program(kFullscreenVs, kUpFs, "bloom up");
    sCompositeProg = gl_program(kFullscreenVs, kCompositeFs, "composite");
    sTrailProg = gl_program(kFullscreenVs, kTrailFs, "frame blend");
    sPanicProg = gl_program(kFullscreenVs, kPanicFs, "panic screens");
    sGreyProg = gl_program(kFullscreenVs, kGreyFs, "tv grey");
    sHazeProg = gl_program(kFullscreenVs, kHazeFs, "haze");
    sGlowStepProg = gl_program(kFullscreenVs, kGlowStepFs, "glow step");
    sShineProg = gl_program(kFullscreenVs, kShineFs, "shine");
    sSoftPrepProg = gl_program(kFullscreenVs, kSoftPrepFs, "soft layer");
    sSoftOverProg = gl_program(kFullscreenVs, kSoftOverFs, "soft layer over");
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
    glClearNamedFramebufferfi(target, GL_DEPTH_STENCIL, 0, depth, 0);
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

/* the frame blend (Renderer_Additive: the scripts' 0xCD): this frame's picture with the last at
 * sTrail, for one frame; the last frame kept while it is wanted */
static GLuint sTrailTex;
static int sTrailW, sTrailH, sTrailValid, sTrailOn;
static float sTrail;   /* the last frame's weight */

void post_trail(float a) {
    a = a < 0.0f ? 0.0f : a > 1.0f ? 1.0f : a;
    sTrail = 1.0f - a;
    sTrailOn = 1;
}
void post_trail_off(void) { sTrailOn = 0; }

static void trail(int x, int y, int w, int h) {
    if (!sTrailOn) {
        sTrailValid = 0;
        return;
    }
    if (sTrailTex == 0 || sTrailW != w || sTrailH != h) {
        if (sTrailTex != 0) {
            glDeleteTextures(1, &sTrailTex);
        }
        glCreateTextures(GL_TEXTURE_2D, 1, &sTrailTex);
        glTextureStorage2D(sTrailTex, 1, GL_RGBA8, w, h);
        sTrailW = w;
        sTrailH = h;
        sTrailValid = 0;
    }
    if (sTrailValid) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glProgramUniform1f(sTrailProg, 0, sTrail);
        bind(0, sTrailTex, sLinear);
        glUseProgram(sTrailProg);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisable(GL_BLEND);
    }
    glCopyTextureSubImage2D(sTrailTex, 0, 0, 0, x, y, w, h);
    sTrailValid = 1;
}

/* ---- the panic's screens (post.h): each the screen so far copied, drawn back through kPanicFs ---- */
static GLuint sPanicTex;
static int sPanicW, sPanicH, sPanicNeg, sPanicZoom, sPanicLimit, sPanicAmount;

void post_negative(void) { sPanicNeg = 1; }
void post_zoom_blur(void) { sPanicZoom = 1; }
void post_panic_tint(int limit, int amount) {
    sPanicLimit = limit;
    sPanicAmount = amount;
}

static void panic_pass(int mode, int x, int y, int w, int h) {
    if (sPanicTex == 0 || sPanicW != w || sPanicH != h) {
        if (sPanicTex != 0) {
            glDeleteTextures(1, &sPanicTex);
        }
        glCreateTextures(GL_TEXTURE_2D, 1, &sPanicTex);
        glTextureStorage2D(sPanicTex, 1, GL_RGBA8, w, h);
        sPanicW = w;
        sPanicH = h;
    }
    glCopyTextureSubImage2D(sPanicTex, 0, 0, 0, x, y, w, h);
    glProgramUniform1i(sPanicProg, 0, mode);
    glProgramUniform1f(sPanicProg, 1, (float)sPanicLimit);
    glProgramUniform1f(sPanicProg, 2, (float)sPanicAmount);
    bind(0, sPanicTex, sLinear);
    glUseProgram(sPanicProg);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void panic_screens(int x, int y, int w, int h) {   /* (in ScreenFade_Frame's order) */
    if (sPanicNeg) {
        panic_pass(1, x, y, w, h);
    }
    if (sPanicZoom) {
        panic_pass(2, x, y, w, h);
    }
    if (sPanicAmount > 0) {
        panic_pass(3, x, y, w, h);
    }
    sPanicNeg = sPanicZoom = sPanicAmount = 0;
}

void post_tv_want(void) { sTvWanted = 1; }
void post_haze(float phase, float shift, float height, int middle, int edge) {
    sHazeWanted = 1;
    sHazePhase[0] = phase;
    sHazePhase[1] = shift;
    sHazeK[0] = height;
    sHazeK[1] = (float)middle / 128.0f;
    sHazeK[2] = (float)edge / 128.0f;
}

void post_glow_clear(void) { sGlowClear = 1; }

void post_bloom(uint32_t rgba, int subtract) {
    float a = (float)(rgba >> 24) / 256.0f * (subtract ? -1.0f : 1.0f);

    sBloomAdd[0] += (float)(rgba & 0xFF) / 128.0f * a;
    sBloomAdd[1] += (float)(rgba >> 8 & 0xFF) / 128.0f * a;
    sBloomAdd[2] += (float)(rgba >> 16 & 0xFF) / 128.0f * a;
    sGlowUsed = 1;   /* (Bloom_Start runs the glow pass too) */
}

int post_glow_begin(void) {
    int i;

    if (T.fbo == 0) {
        return 0;
    }
    if (sGlowTrail[0] == 0 || sGlowOf != T.depth) {
        if (sGlowTrail[0] != 0) {
            glDeleteFramebuffers(2, sGlowFbo);
            glDeleteTextures(2, sGlowTrail);
            glDeleteTextures(1, &sGlowDepth);
        }
        sGlowW = T.w / 4 > 0 ? T.w / 4 : 1;
        sGlowH = T.h / 4 > 0 ? T.h / 4 : 1;
        sGlowDepth = texture2d(GL_DEPTH32F_STENCIL8, sGlowW, sGlowH);
        for (i = 0; i < 2; i++) {
            sGlowTrail[i] = texture2d(GL_RGBA16F, sGlowW, sGlowH);
            glCreateFramebuffers(1, &sGlowFbo[i]);
            glNamedFramebufferTexture(sGlowFbo[i], GL_COLOR_ATTACHMENT0, sGlowTrail[i], 0);
            glNamedFramebufferTexture(sGlowFbo[i], GL_DEPTH_STENCIL_ATTACHMENT, sGlowDepth, 0);
        }
        sGlowOf = T.depth;
        sGlowClear = 1;
    }
    if (sGlowClear) {
        static const float kNone[4] = {0, 0, 0, 0};

        glClearNamedFramebufferfv(sGlowFbo[0], GL_COLOR, 0, kNone);
        glClearNamedFramebufferfv(sGlowFbo[1], GL_COLOR, 0, kNone);
        sGlowClear = 0;
    }
    /* the scene's depth so far, at the buffer's size (the original copies its Z buffer down) */
    if (T.ms_fbo != 0) {   /* (resolved first: a multisampled one can't be scaled) */
        glBlitNamedFramebuffer(T.ms_fbo, T.fbo, 0, 0, T.w, T.h, 0, 0, T.w, T.h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
    glBlitNamedFramebuffer(T.fbo, sGlowFbo[sGlowCur], 0, 0, T.w, T.h, 0, 0, sGlowW, sGlowH, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, sGlowFbo[sGlowCur]);
    glViewport(0, 0, sGlowW, sGlowH);
    sGlowUsed = 1;
    return 1;
}

void post_glow_end(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, T.ms_fbo != 0 ? T.ms_fbo : T.fbo);
    glViewport(0, 0, T.w, T.h);
}

void post_shine(uint32_t rgba) {
    sShineWanted = 1;
    sShineCol[0] = (float)(rgba >> 24) / 128.0f;
    sShineCol[1] = (float)(rgba >> 16 & 0xFF) / 128.0f;
    sShineCol[2] = (float)(rgba >> 8 & 0xFF) / 128.0f;
}

/* a half-size target that characters are drawn again into, against the scene's depth (the shine,
 * the softened layer), and the pass made from it */
typedef struct Redraw {
    GLuint fbo, tex, depth, out_fbo, out, of;
    int w, h, drawn;
} Redraw;

static void redraw_make(Redraw *r) {
    if (r->tex != 0 && r->of == T.depth) {
        return;
    }
    if (r->tex != 0) {
        glDeleteFramebuffers(1, &r->fbo);
        glDeleteFramebuffers(1, &r->out_fbo);
        glDeleteTextures(1, &r->tex);
        glDeleteTextures(1, &r->out);
        glDeleteTextures(1, &r->depth);
    }
    r->w = T.half_w;
    r->h = T.half_h;
    r->tex = texture2d(GL_RGBA16F, r->w, r->h);
    r->out = texture2d(GL_RGBA16F, r->w, r->h);
    r->depth = texture2d(GL_DEPTH32F_STENCIL8, r->w, r->h);
    glCreateFramebuffers(1, &r->fbo);
    glNamedFramebufferTexture(r->fbo, GL_COLOR_ATTACHMENT0, r->tex, 0);
    glNamedFramebufferTexture(r->fbo, GL_DEPTH_STENCIL_ATTACHMENT, r->depth, 0);
    r->out_fbo = framebuffer(r->out, 0);
    r->of = T.depth;
}

static int redraw_begin(Redraw *r) {
    static const float kNone[4] = {0, 0, 0, 0};

    if (T.fbo == 0) {
        return 0;
    }
    redraw_make(r);
    if (T.ms_fbo != 0) {
        glBlitNamedFramebuffer(T.ms_fbo, T.fbo, 0, 0, T.w, T.h, 0, 0, T.w, T.h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
    glBlitNamedFramebuffer(T.fbo, r->fbo, 0, 0, T.w, T.h, 0, 0, r->w, r->h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glClearNamedFramebufferfv(r->fbo, GL_COLOR, 0, kNone);
    glBindFramebuffer(GL_FRAMEBUFFER, r->fbo);
    glViewport(0, 0, r->w, r->h);
    glDepthMask(GL_TRUE);
    glEnable(GL_POLYGON_OFFSET_FILL);   /* (the scene's depth, halved, may have these characters in it already) */
    glPolygonOffset(-2.0f, -16.0f);
    r->drawn = 1;
    return 1;
}

static void redraw_end(void) {
    glDisable(GL_POLYGON_OFFSET_FILL);
    post_glow_end();
}

static Redraw sShine, sSoftLayer;

int post_shine_begin(void) {
    return sShineWanted && redraw_begin(&sShine);
}

void post_shine_end(void) {
    redraw_end();
}

int post_soft_begin(void) {
    return redraw_begin(&sSoftLayer);
}

void post_soft_end(void) {
    redraw_end();
}

static void shine(void) {
    if (sShine.drawn) {
        bind(0, sShine.tex, sLinear);
        fullscreen(sShine.out_fbo, sShine.w, sShine.h, sShineProg);
    }
    if (sSoftLayer.drawn) {   /* (layer $1C: what it drew, premultiplied by where it drew, laid over
                               * the scene before the rest - the room's look sees it as any) */
        bind(0, sSoftLayer.tex, 0);
        fullscreen(sSoftLayer.out_fbo, sSoftLayer.w, sSoftLayer.h, sSoftPrepProg);
        bind(0, sSoftLayer.out, sLinear);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);   /* (the bloom mask left be) */
        fullscreen(T.fbo, T.w, T.h, sSoftOverProg);
        glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDisable(GL_BLEND);
    }
}

static void glow_step(void) {
    if (sGlowTrail[0] == 0 || !sGlowUsed) {
        return;
    }
    bind(0, sGlowTrail[sGlowCur], 0);
    fullscreen(sGlowFbo[sGlowCur ^ 1], sGlowW, sGlowH, sGlowStepProg);
    sGlowCur ^= 1;
}

static void haze(void) {   /* (the scene's colour wavered in place) */
    if (!sHazeWanted) {
        return;
    }
    sHazeWanted = 0;
    if (sHazeTex == 0 || sHazeW != T.w || sHazeH != T.h) {
        if (sHazeTex != 0) {
            glDeleteFramebuffers(1, &sHazeFbo);
            glDeleteTextures(1, &sHazeTex);
        }
        sHazeTex = texture2d(GL_RGBA16F, T.w, T.h);
        sHazeFbo = framebuffer(sHazeTex, 0);
        sHazeW = T.w;
        sHazeH = T.h;
    }
    glProgramUniform2f(sHazeProg, 0, sHazePhase[0], sHazePhase[1]);
    glProgramUniform3f(sHazeProg, 1, sHazeK[0], sHazeK[1], sHazeK[2]);
    bind(0, T.color, sLinear);
    fullscreen(sHazeFbo, T.w, T.h, sHazeProg);
    glCopyImageSubData(sHazeTex, GL_TEXTURE_2D, 0, 0, 0, 0, T.color, GL_TEXTURE_2D, 0, 0, 0, 0, T.w, T.h, 1);
}

unsigned int post_tv_texture(void) { return sTvTex; }

static void tv_copy(void) {
    int w = T.w / 2 > 0 ? T.w / 2 : 1, h = T.h / 2 > 0 ? T.h / 2 : 1;

    if (!sTvWanted) {
        return;
    }
    sTvWanted = 0;
    if (sTvTex == 0 || sTvW != w || sTvH != h) {
        if (sTvTex != 0) {
            glDeleteFramebuffers(1, &sTvFbo);
            glDeleteTextures(1, &sTvTex);
        }
        sTvTex = texture2d(GL_RGBA8, w, h);
        sTvFbo = framebuffer(sTvTex, 0);
        sTvW = w;
        sTvH = h;
    }
    bind(0, T.color, sLinear);
    fullscreen(sTvFbo, w, h, sGreyProg);
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
    haze();
    tv_copy();
    glow_step();
    shine();

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
    if (s->room_dof && gRoomLook.has_dof) {   /* two more tent passes: about the game's 8 */
        bind(0, T.soft[1], sLinear);
        fullscreen(T.dof_fbo, T.half_w, T.half_h, sUpProg);
        bind(0, T.dof, sLinear);
        fullscreen(T.soft_fbo[0], T.half_w, T.half_h, sUpProg);
        bind(0, T.soft[0], sLinear);
        fullscreen(T.dof_fbo, T.half_w, T.half_h, sUpProg);
    }
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
    bind(7, T.dof, sLinear);
    bind(8, sGlowTrail[sGlowCur], sLinear);
    glProgramUniform1f(sCompositeProg, 21, sGlowUsed && sGlowTrail[0] != 0 ? 0.5f : 0.0f);
    bind(9, sShine.out, sLinear);
    if (sShine.drawn) {
        glProgramUniform3f(sCompositeProg, 22, sShineCol[0], sShineCol[1], sShineCol[2]);
    } else {
        glProgramUniform3f(sCompositeProg, 22, 0.0f, 0.0f, 0.0f);
    }
    if (s->room_dof && gRoomLook.has_dof) {
        glProgramUniform4f(sCompositeProg, 20, gRoomLook.dof[0], gRoomLook.dof[1], gRoomLook.dof[2], gRoomLook.dof[3]);
    } else {
        glProgramUniform4f(sCompositeProg, 20, 1.0f, 0.0f, 0.0f, 0.0f);
    }
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
        float k = bloom ? (l->bloom_subtract ? -l->bloom[3] : l->bloom[3]) : 0.0f;

        glProgramUniform4f(sCompositeProg, 19, l->bloom[0] * k + sBloomAdd[0], l->bloom[1] * k + sBloomAdd[1],
                           l->bloom[2] * k + sBloomAdd[2], 1.0f);   /* (the room's and the effects') */
    }
    glUseProgram(sCompositeProg);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    panic_screens(x, y, w, h);
    trail(x, y, w, h);
    for (i = 0; i < 10; i++) {
        bind((GLuint)i, 0, 0);
    }
    sGlowUsed = 0;
    sShineWanted = sShine.drawn = sSoftLayer.drawn = 0;
    sBloomAdd[0] = sBloomAdd[1] = sBloomAdd[2] = 0.0f;
    glBindVertexArray(0);
    (void)cam->znear;
}
