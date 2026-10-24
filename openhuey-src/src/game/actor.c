#include "actor.h"

#include <math.h>

#include "../core/files.h"
#include "../data/exetables.h"
#include "../data/texbank.h"
#include "../data/modelgltf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void load_textures(Actor *a, const char *name) {
    char path[96];
    TexBank *bank;
    int n, i;

    snprintf(path, sizeof(path), "%s.TEX", name);
    bank = texbank_load(path);
    n = texbank_count(bank);
    for (i = 0; i < n && i < ACTOR_MAX_TEXTURES; i++) {
        int w, h;
        uint8_t *rgba = texbank_image(bank, i, -1, &w, &h);   /* (at its own size: a texture pack's bigger) */

        if (rgba != NULL) {
            a->textures[i] = render_texture(rgba, w, h);
        }
        free(rgba);
    }
    a->ntextures = n < ACTOR_MAX_TEXTURES ? n : ACTOR_MAX_TEXTURES;
    texbank_free(bank);
}

/* the library's model: NAME.model/model.gltf, model.bin and motions.bin (data/modelgltf.h); 0 if
 * there is none */
static int load_model_gltf(Model *m, const char *name) {
    char path[160];
    uint8_t *json, *bin, *motions;
    size_t len, bs, ms = 0;
    int ok;

    snprintf(path, sizeof(path), "%s.model/model.gltf", name);
    if ((json = files_read(path, &len)) == NULL) {
        return 0;
    }
    snprintf(path, sizeof(path), "%s.model/model.bin", name);
    bin = files_read(path, &bs);
    snprintf(path, sizeof(path), "%s.model/motions.bin", name);
    motions = files_read(path, &ms);
    ok = bin != NULL && modelgltf_read(m, (const char *)json, len, bin, bs, motions, motions != NULL ? ms : 0);
    if (!ok) {
        fprintf(stderr, "actor: %s.model/model.gltf isn't one OpenHuey reads\n", name);
    }
    free(json);
    free(bin);
    return ok;
}

int actor_load(Actor *actors, const char *name) { return actor_load_textured(actors, name, name); }

void actor_texture_restore(Actor *a) {
    if (a->swap_sel < 0) {
        return;
    }
    render_texture_free(a->textures[a->swap_sel]);
    a->textures[a->swap_sel] = a->swap_orig;
    a->swap_sel = -1;
}

int actor_texture_swap(Actor *a, int sel, const char *file) {
    TexBank *bank = texbank_load(file);
    uint8_t *rgba;
    int ok = 0, w, h;

    if (bank == NULL || sel < 0 || sel >= ACTOR_MAX_TEXTURES) {
        texbank_free(bank);
        return 0;
    }
    rgba = texbank_image(bank, 0, -1, &w, &h);
    if (rgba != NULL) {
        actor_texture_restore(a);
        a->swap_orig = a->textures[sel];
        a->swap_sel = sel;
        a->textures[sel] = render_texture(rgba, w, h);
        if (sel >= a->ntextures) {
            a->ntextures = sel + 1;
        }
        ok = 1;
    }
    free(rgba);
    texbank_free(bank);
    return ok;
}

int actor_load_textured(Actor *actors, const char *name, const char *tex) {
    char path[96];
    size_t size;
    uint8_t *pck;
    int i;
    Actor *a;

    for (i = 0; i < MAX_ACTORS && actors[i].used; i++) {
    }
    if (i == MAX_ACTORS) {
        return -1;
    }
    a = &actors[i];
    memset(a, 0, sizeof(*a));
    if (!load_model_gltf(&a->model, name)) {   /* (else the .PCK) */
        snprintf(path, sizeof(path), "%s.PCK", name);
        pck = files_read(path, &size);
        if (pck == NULL || !model_build(&a->model, pck, size)) {
            return -1;
        }
    }
    a->used = 1;
    snprintf(a->name, sizeof(a->name), "%s", name);
    load_textures(a, tex);
    a->cloth = cloth_for(name);
    a->posed = malloc((size_t)a->model.nv * sizeof(MeshVertex));
    if (a->model.nmorphs > 0) {   /* (its face and hands moved from their rest each frame) */
        a->shaped = malloc((size_t)a->model.nv * sizeof(SkinVertex));
        memcpy(a->shaped, a->model.v, (size_t)a->model.nv * sizeof(SkinVertex));
    }
    if (strncmp(name, "O_FI", 4) == 0) {   /* (CharModel_Loaded: Fiona's models' hands; RiccardoModel_Loaded) */
        a->hand_default[0] = a->hand_default[1] = 1;
    } else if (strncmp(name, "O_RCG", 5) == 0) {
        a->hand_default[1] = 8;
    }
    a->scale = 1.0f;
    a->rate = 1.0f;   /* (a motion frame a tick: the game ran its motions at its own 30 fps) */
    a->motion = -1;
    a->prev_motion = -1;
    a->variant = a->prev_variant = -1;
    a->swap_sel = -1;
    a->vweight = a->prev_vweight = 1.0f;
    for (i = 0; i < 3; i++) {
        a->parts[i].motion = a->parts[i].own = a->parts[i].prev = a->parts[i].pending = -1;
    }
    a->loop = 1;
    a->visible = 1;
    a->shadow_size = 7.0f;
    a->alpha = 1.0f;
    a->tint = 0x80808080u;
    a->doorway_fade = 1;
    memset(a->parts_shown, 0xFF, sizeof(a->parts_shown));
    snprintf(path, sizeof(path), "%s.MRK", name);
    a->events = files_exist(path) ? files_read(path, &a->events_size) : NULL;
    a->lo = vec3(1e30f, 1e30f, 1e30f);
    a->hi = vec3(-1e30f, -1e30f, -1e30f);
    for (i = 0; i < a->model.nv; i++) {
        a->lo = vec3_min(a->lo, a->model.v[i].pos);
        a->hi = vec3_max(a->hi, a->model.v[i].pos);
    }
    return (int)(a - actors);
}

int actor_load_motions(Actor *a, const char *path) {
    size_t size;
    uint8_t *b = files_read(path, &size);
    int in_use = a->model.motions2 != NULL && a->model.motions2 == a->loaded_motions;

    if (b == NULL) {
        return 0;
    }
    free(a->loaded_motions);
    a->loaded_motions = b;
    a->loaded_motions_size = size;
    if (in_use) {
        actor_use_motions(a, 1);
    }
    return 1;
}

void actor_use_motions(Actor *a, int on) {
    a->model.motions2 = on ? a->loaded_motions : NULL;
    a->model.motions2_size = on ? a->loaded_motions_size : 0;
}

void actor_free(Actor *a) {
    int i;

    if (a->swap_sel >= 0) {
        render_texture_free(a->swap_orig);
        a->swap_sel = -1;
    }
    for (i = 0; i < a->ntextures; i++) {
        render_texture_free(a->textures[i]);
    }
    render_mesh_free(&a->gpu);
    render_mesh_free(&a->shadow);
    model_free(&a->model);
    free(a->posed);
    free(a->shaped);
    free(a->events);
    free(a->cloth);
    free(a->loaded_motions);
    memset(a, 0, sizeof(*a));
}

int actor_motion_done(const Actor *a) {
    return a->motion >= 0 && !a->loop && a->frame >= (float)(model_motion_frames(&a->model, a->motion) - 1);
}

/* a play time on by the speed: looping wraps it, else it stops at the last frame; 1 when it
 * went past the end (anim_step) */
static int step_time(float *t, float speed, int frames, int loop) {
    int wrapped = 0;

    *t += speed;
    if (loop) {
        while (*t < 0.0f) {
            *t += (float)frames;
        }
        while (*t >= (float)frames) {
            *t -= (float)frames;
            wrapped = 1;
        }
        return wrapped;
    }
    if (*t < 0.0f) {
        *t = 0.0f;
    }
    if (*t > (float)(frames - 1)) {
        *t = (float)(frames - 1);
        wrapped = 1;
    }
    return wrapped;
}

static void part_start(struct ActorPart *p, int index, int flags, float blend);
static int model_motion_entry_flags(const Actor *a, int index);
static void legs_contact(Actor *a);

/* a motion and its variant kept in step (Motion_KeepInStep): the variant at the motion's phase,
 * their speeds meeting by the weight `w` (the motion's share) - the motion at w + (l0 / l1) (1 - w)
 * of its own, the variant at (1 - w) + (l1 / l0) w - so the two run as one cycle of the blended
 * length; their root movements are scaled by the same speeds (motion_root). No variant: 1, 1 */
static void step_speeds(const Actor *a, int motion, int var, float w, float *s0, float *s1) {
    float l0, l1;

    *s0 = *s1 = 1.0f;
    if (motion < 0 || var < 0) {
        return;
    }
    l0 = (float)model_motion_frames(&a->model, motion);
    l1 = (float)model_motion_frames(&a->model, var);
    if (l0 <= 0.0f || l1 <= 0.0f) {
        return;
    }
    *s0 = w + (l0 / l1) * (1.0f - w);
    *s1 = (1.0f - w) + (l1 / l0) * w;
}
static void keep_in_step(const Actor *a, int motion, float frame, int var, float *vframe) {
    if (motion >= 0 && var >= 0 && model_motion_frames(&a->model, motion) > 0) {
        *vframe = (float)model_motion_frames(&a->model, var) * (frame / (float)model_motion_frames(&a->model, motion));
    }
}

static void part_tick(Actor *a, struct ActorPart *p) {
    if (p->motion >= 0 && !(p->flags & 0x10)) {
        step_time(&p->frame, 1.0f, model_motion_frames(&a->model, p->motion), p->flags & 1);
    }
    if (p->fade > 0.0f) {
        if (p->prev >= 0) {
            step_time(&p->prev_frame, 1.0f, model_motion_frames(&a->model, p->prev), 1);
        }
        p->fade -= 1.0f;
    }
    if (p->fade <= 0.0f && p->pending >= 0) {   /* one waiting for the fade */
        int blend, pose, flags, i = p->pending;

        p->pending = -1;
        actor_motion_entry(a, i, &blend, &pose, &flags);
        part_start(p, i, flags, (float)blend);
    }
}

/* ---- the face and hands ---- */

/* 0 <= r < 1 (the blinks' odds: the original's gRandom) */
static float face_random(void) {
    static uint32_t seed = 0x2545F491u;

    seed = seed * 1103515245u + 12345u;
    return (float)(seed >> 8 & 0xFFFF) / 65536.0f;
}

/* a blink: the eyelids (shapes 7 and 8) down and up again over 14 frames by the game's curve;
 * from 30 frames on another starts at random, surely by 150 (Motion_Eyes) */
static void blink(Actor *a) {
    size_t n = 0;
    const uint8_t *curve = exetable("blink-curve", &n);

    if (a->blink < 14 && curve != NULL && n >= 14 * 4) {
        memcpy(&a->face.w[7], curve + a->blink * 4, 4);
        a->face.w[8] = a->face.w[7];
    }
    if (a->blink >= 30 && (int)((float)(150 - a->blink) * face_random()) == 0) {
        a->blink = 0;
    }
    a->blink++;
}

/* the hands posed by the motion (Motion_Hands): its table's pose (left << 4 | right), each hand
 * shaped over five frames to come as its events say (bit 4 the left, 8 the right) - its pose's
 * keys forward, or back the way it came; a pose of 7 and up with no shape there has the hand's
 * own. A motion started again (its time 0) has the hands as they were made */
static void pose_hands(Actor *a) {
    size_t n = 0;
    const uint8_t *keys = exetable("hand-poses", &n);
    int blend, pose, flags, hand, i;
    int ev[5];

    if (a->motion < 0 || a->frame == a->hand_frame) {
        return;
    }
    if (a->frame == 0.0f) {
        a->hand_on[0] = a->hand_on[1] = 0;
        for (hand = 0; hand < 2; hand++) {
            a->face.hand[hand].a = a->face.hand[hand].b = a->hand_default[hand];
            a->face.hand[hand].w = 0.0f;
        }
    }
    if (a->table == NULL || !actor_motion_entry(a, a->motion, &blend, &pose, &flags) || pose == 0 || keys == NULL ||
        n < 16 * 60) {
        return;
    }
    for (i = 0; i < 5; i++) {
        ev[i] = actor_event_flags(a, 0, i, -2);
    }
    for (hand = 0; hand < 2; hand++) {
        int on = a->hand_on[hand], bit = hand == 0 ? 4 : 8, which = hand == 0 ? pose >> 4 & 0xF : pose & 0xF;

        for (i = 0; i < 5; i++) {
            const uint8_t *e = keys + which * 60 + (on ? i : 4 - i) * 12;
            int32_t sa, sb;

            if (!(ev[i] & bit)) {
                continue;
            }
            memcpy(&sa, e, 4);
            memcpy(&sb, e + 4, 4);
            a->face.hand[hand].a = which < 7 || sa != 0 ? sa : a->hand_default[hand];
            a->face.hand[hand].b = which < 7 || sb != 0 ? sb : a->hand_default[hand];
            memcpy(&a->face.hand[hand].w, e + 8, 4);
            if (i == 0) {
                on = !on;
            }
        }
        a->hand_on[hand] = (uint8_t)on;
    }
}

/* the motion's channels for the face and hands at its time (Motion_ExtraChannels): tracks -7..-10
 * into the face's weights from shape 0, 1, 2, 3 on - each as many as it has, so a later one
 * overwrites what an earlier set beyond its own (the scenes': four values, the mouth the second
 * one's first), -11 / -12 the hands' shapes and way between */
static void face_channels(Actor *a, const Model *m, int index, float frame) {
    int i, j;

    for (i = 0; i < 6; i++) {
        float out[8];
        int set = model_track_sample(m, index, -7 - i, frame, out);
        float *to = i < 4 ? &a->face.w[i] : (float *)&a->face.hand[i - 4];
        int room = i < 4 ? 16 - i : 3;

        for (j = 0; j < 8 && j < room; j++) {
            if (set & (1 << j)) {
                memcpy(&to[j], &out[j], 4);
            }
        }
    }
}

static void advance(Actor *a);

void actor_tick(Actor *a) {
    int i;

    if (!a->used) {
        return;
    }
    if (a->alive && a->drive == NULL) {   /* (Fiona_AnimUpdate / Pursuer_ModelUpdate; a scene's own channels) */
        pose_hands(a);
        blink(a);
    }
    a->hand_frame = a->frame;
    for (i = 0; i < 3; i++) {
        part_tick(a, &a->parts[i]);
    }
    if (a->cloth != NULL) {
        cloth_watch(a->cloth, a->motion >= 0 ? model_motion_id(&a->model, a->motion) : -1, a->yaw);
    }
    cloth_tick(a->cloth);
    if (a->legs.on) {
        legs_contact(a);
    }
    advance(a);
    if (a->hlegs.on) {   /* (HumanModel_PlantFeet's part before the pose: after Motion_Update moved on) */
        humanlegs_tick(a);
    }
}

/* the motion moves on (Motion_Advance) */
static void advance(Actor *a) {
    int frames, loop;
    float s0, s1;

    if (a->motion < 0 || (a->mflags & 0x40)) {
        return;
    }
    frames = model_motion_frames(&a->model, a->motion);
    loop = a->loop || (a->mflags & 1);
    keep_in_step(a, a->motion, a->frame, a->variant, &a->vframe);
    step_speeds(a, a->motion, a->variant, a->vweight, &s0, &s1);
    if (!(a->mflags & 0x10)) {
        if (step_time(&a->frame, a->rate * s0, frames, loop)) {
            a->mflags |= 0x20;
        } else {
            a->mflags &= ~0x20;
        }
        if (a->frame >= (float)(frames - 1)) {
            a->mflags |= 0x400;
        } else {
            a->mflags &= ~0x400;
        }
    }
    if (a->variant >= 0 && !(a->mflags & 0x10)) {
        step_time(&a->vframe, a->rate * s1, model_motion_frames(&a->model, a->variant), loop);
    }
    if (a->fade > 0.0f) {   /* the motion before goes on while it fades out (not with flag 8) */
        keep_in_step(a, a->prev_motion, a->prev_frame, a->prev_variant, &a->prev_vframe);
        step_speeds(a, a->prev_motion, a->prev_variant, a->prev_vweight, &s0, &s1);
        if (a->prev_motion >= 0 && !a->prev_frozen) {
            step_time(&a->prev_frame, a->rate * s0, model_motion_frames(&a->model, a->prev_motion), 1);
        }
        if (a->prev_variant >= 0 && !a->prev_frozen) {
            step_time(&a->prev_vframe, a->rate * s1, model_motion_frames(&a->model, a->prev_variant), 1);
        }
        a->fade -= 1.0f;
    }
    if (a->fade <= 0.0f && (a->mflags & 8)) {   /* (Motion_Update: flag 8 holds it only while it fades in) */
        a->mflags &= ~0x10;
    }
}

static void part_start(struct ActorPart *p, int index, int flags, float blend) {
    if (p->motion >= 0 && blend > 0.0f) {
        p->prev = p->motion;
        p->prev_frame = p->frame;
        p->fade = p->fade_len = blend;
    } else {
        p->fade = p->fade_len = 0.0f;
    }
    p->motion = index;
    p->frame = 0.0f;
    p->flags = (flags & 0xFFFF) | (flags & 8 ? 0x10 : 0);
}

/* the original's Motion_Start: the body (part 1) as its motion, each part 2..4 its share of it,
 * or its own */
void actor_motion_start(Actor *a, int index, int flags, float blend) {
    int parts = model_motion_parts(&a->model, index), k;

    for (k = 0; k < 3; k++) {
        struct ActorPart *p = &a->parts[k];

        if (parts & (4 << k)) {
            if (!(parts & 2)) {
                p->own = index;   /* (a motion of the part's own) */
            }
            part_start(p, index, flags, blend);
        } else if (p->own >= 0 && p->own != p->motion) {
            part_start(p, p->own, model_motion_entry_flags(a, p->own), blend);
        }
    }
    if ((parts & 2) || !(parts & 0x1C)) {
        actor_body_start(a, index, flags, blend);
    }
}

void actor_motion_when_free(Actor *a, int index) {
    int parts = model_motion_parts(&a->model, index), whole, k, blend, pose, flags;

    if (index < 0) {
        return;
    }
    actor_motion_entry(a, index, &blend, &pose, &flags);
    if (parts == 0x1F || (parts & 2)) {
        if (a->motion != index) {
            actor_motion_start(a, index, flags, (float)blend);
        }
        return;
    }
    whole = a->motion >= 0 && model_motion_parts(&a->model, a->motion) == 0x1F;
    for (k = 0; k < 3; k++) {
        struct ActorPart *p = &a->parts[k];

        if (!(parts & (4 << k))) {
            continue;
        }
        if (whole) {   /* the body motion has this part: its own for later */
            p->own = index;
            continue;
        }
        if (p->motion == index) {
            continue;
        }
        p->own = index;
        if (p->fade <= 0.0f) {
            part_start(p, index, flags, (float)blend);
        } else {
            p->pending = index;
        }
    }
}

static int model_motion_entry_flags(const Actor *a, int index) {
    int blend, pose, flags;

    actor_motion_entry(a, index, &blend, &pose, &flags);
    return flags;
}

void actor_body_start(Actor *a, int index, int flags, float blend) {
    int frames = model_motion_frames(&a->model, index);
    /* the motion before had come to its end, not looping: it fades out posed at its last frame
     * (stepped on, it would wrap to its first - a crouch's end back to standing); only its pose:
     * its root goes on as it did (Hewie's turns while fading rest on it) */
    int ended = a->motion >= 0 && !(a->loop || (a->mflags & 1)) && (a->mflags & 0x400);

    if (a->motion >= 0 && blend > 0.0f) {
        a->prev_motion = a->motion;
        a->prev_frame = a->frame;
        a->prev_variant = a->variant;
        a->prev_vframe = a->vframe;
        a->prev_vweight = a->vweight;
        a->fade = a->fade_len = blend;
    } else {
        a->fade = a->fade_len = 0.0f;
    }
    a->variant = -1;
    a->vweight = 1.0f;
    a->prev_old_flags = a->mflags;
    a->prev_old_frames = a->motion >= 0 ? model_motion_frames(&a->model, a->motion) : 0;
    a->prev_old_frame = a->frame;
    if ((flags & a->mflags & 2) && a->motion >= 0) {   /* in step: the same phase */
        a->frame = (float)frames * (a->frame / (float)model_motion_frames(&a->model, a->motion));
    } else {
        a->frame = 0.0f;
    }
    a->motion = index;
    a->mflags = (flags & 0xFFFF) | (flags & 8 ? 0x10 : 0);
    a->prev_frozen = (flags & 8) != 0;
    a->prev_held = ended;
    a->loop = flags & 1;
}

void actor_body_variant(Actor *a, int index) {
    int frames;

    a->variant = index;
    a->vframe = 0.0f;
    if (index < 0) {
        return;
    }
    frames = model_motion_frames(&a->model, index);
    if ((a->mflags & a->prev_old_flags & 2) && a->prev_old_frames > 0) {   /* (Motion_StartBody) */
        a->vframe = (float)frames * (a->prev_old_frame / (float)a->prev_old_frames);
    }
}

/* one slot's root step: the motion and its variant by the weight (motion_root, layer 1) */
/* the root's step (Motion_RootMovement's motion_root: each animation's at its speed, the variant
 * blended by the weight) and turn (Motion_RootRotation: the motion's own alone, at its speed) */
static void root_of(const Actor *a, int index, float frame, int var, float vframe, float w, float *turn, Vec3 *step) {
    float t2, s0, s1;
    Vec3 s2;

    step_speeds(a, index, var, w, &s0, &s1);
    model_root_delta(&a->model, index, frame, turn, step);
    *turn *= s0;
    *step = vec3_scale(*step, s0);
    if (var >= 0 && model_root_delta(&a->model, var, vframe, &t2, &s2)) {
        *step = vec3_add(vec3_scale(*step, w), vec3_scale(s2, s1 * (1.0f - w)));
    }
}

void actor_root_delta(const Actor *a, float *turn, Vec3 *step) {
    if (a->mflags & 0x10) {   /* (no time of its own: no root movement - motion_root) */
        *turn = 0.0f;
        *step = vec3(0.0f, 0.0f, 0.0f);
    } else {
        root_of(a, a->motion, a->frame, a->variant, a->vframe, a->vweight, turn, step);
    }
    if (a->fade > 0.0f && a->fade_len > 0.0f && a->prev_motion >= 0) {   /* prev x w + this x (1 - w) */
        float x = a->fade / a->fade_len, w = x * x * (3.0f - 2.0f * x), t2 = 0.0f;
        Vec3 s2 = vec3(0.0f, 0.0f, 0.0f);

        if (!a->prev_frozen) {   /* (held still: none of its own) */
            root_of(a, a->prev_motion, a->prev_frame, a->prev_variant, a->prev_vframe, a->prev_vweight, &t2, &s2);
        }
        (void)t2;   /* (the turn: the current motion's alone - Motion_RootRotation) */
        *step = vec3_add(vec3_scale(s2, w), vec3_scale(*step, 1.0f - w));
    }
}

static uint32_t rd32le(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }

int actor_event_flags(const Actor *a, int layer, int dt, int loop) {
    int index = layer == 0 ? a->motion : a->variant, idx, t, len;
    uint32_t n, off;

    if (a->events == NULL || a->events_size < 4 || index < 0) {
        return 0;
    }
    idx = model_motion_pos(&a->model, index);
    if (idx < 0) {
        return 0;
    }
    t = (int)((float)dt + (layer == 0 ? a->frame : a->vframe));
    len = model_motion_frames(&a->model, index);
    n = rd32le(a->events);
    if ((size_t)(1 + n + (uint32_t)idx + 1) * 4 > a->events_size) {
        return 0;
    }
    off = rd32le(a->events + 4 + n * 4 + (uint32_t)idx * 4);
    if (((a->mflags | (a->loop ? 1 : 0)) & (loop & 1)) && len > 0) {
        float ft = (float)t;

        while (ft < 0.0f) {
            ft += (float)len;
        }
        while (!(ft < (float)len)) {
            ft -= (float)len;
        }
        t = (int)ft;
    } else if (t < 0 || len - 1 < t) {
        return 0;
    }
    return off + (size_t)t < a->events_size ? a->events[off + (size_t)t] : 0;
}

Vec3 actor_bone(const Actor *a, int b) {
    return b >= 0 && b < a->model.nbones ? a->bones[b] : a->pos;
}

Vec3 actor_bone_point(const Actor *a, int b, Vec3 off) {
    const Vec3 *ax;

    if (b < 0 || b >= a->model.nbones) {
        return a->pos;
    }
    ax = a->bone_axes[b];
    return vec3_add(a->bones[b], vec3_add(vec3_add(vec3_scale(ax[0], off.x), vec3_scale(ax[1], off.y)), vec3_scale(ax[2], off.z)));
}

int actor_motion_entry(const Actor *a, int index, int *blend, int *pose, int *flags) {
    const uint8_t *e;

    index = model_motion_pos(&a->model, index);   /* (the table goes by the id list) */
    if (a->table == NULL || index < 0 || index >= a->ntable) {
        *blend = *pose = *flags = 0;
        return 0;
    }
    e = a->table + index * 6;
    *blend = (int16_t)(e[0] | e[1] << 8);
    *pose = e[2];
    *flags = e[4] | e[5] << 8;
    return 1;
}

/* ---- through the pose hook: a dog's legs (doglegs.c), the hanging parts (cloth.c) ---- */
static Actor *sHookActor;
static Mat4 sHookPlace, sHookUnplace;

static void pose_hook(Mat4 *world, int n, void *user) {
    (void)user;
    if (sHookActor->legs.on) {
        doglegs_pose(&sHookActor->legs, &sHookActor->model, world, &sHookPlace, &sHookUnplace);
    }
    if (sHookActor->hlegs.on) {
        humanlegs_pose(&sHookActor->hlegs, &sHookActor->model, world, &sHookPlace, &sHookUnplace);
    }
    if (sHookActor->cloth != NULL) {
        cloth_pose(sHookActor->cloth, world, n, &sHookPlace, &sHookUnplace);
    }
}

/* room -> model space: the place undone (turn back, unscale; its origin at height y) */
static Mat4 unplace_of(const Actor *a, float y) {
    Mat4 u = mat4_identity();
    float c = cosf(a->yaw), s = sinf(a->yaw), k = a->scale != 0.0f ? 1.0f / a->scale : 1.0f;

    u.m[0] = c * k;  u.m[2] = s * k;
    u.m[5] = k;
    u.m[8] = -s * k; u.m[10] = c * k;
    u.m[12] = -(u.m[0] * a->pos.x + u.m[8] * a->pos.z);
    u.m[13] = -k * y;
    u.m[14] = -(u.m[2] * a->pos.x + u.m[10] * a->pos.z);
    return u;
}

/* the feet's contact this tick (the contact tracks -5 / -6, as foot-down?) */
static void legs_contact(Actor *a) {
    int i;

    for (i = 0; i < 4; i++) {
        float c0[3], c1[3];
        int code = i < 2 ? -5 : -6, k = i & 1;
        int has0 = model_track_raw(&a->model, a->motion, code, a->frame, c0);
        int has1 = a->fade > 0.0f && a->prev_motion >= 0 && model_track_raw(&a->model, a->prev_motion, code, a->prev_frame, c1);

        a->legs.down_was[i] = a->legs.down[i];
        a->legs.down[i] = has0 && has1 ? (c0[k] > 0.0f && c1[k] > 0.0f) : (has0 && c0[k] > 0.0f);
    }
}

int actor_drive_place(Actor *a, const uint8_t *drive, size_t size, float frame, Vec3 *pos, float *yaw) {
    static Mat4 skin_m[MODEL_MAX_BONES];
    Model *m = &a->model;
    const uint8_t *own = m->motions, *second = m->motions2;
    size_t own_size = m->motions_size;
    Vec3 root = a->pos, end, bind;
    const Mat4 *inv;
    int r, k, best = 0, index;

    if (drive == NULL || m->nbones <= 0) {
        return 0;
    }
    m->motions = drive;
    m->motions_size = size;
    m->motions2 = NULL;
    index = model_motion_find(m, 0x8000);
    if (index >= 0) {
        model_pose_root(m, index, frame, skin_m, &root);
    }
    m->motions = own;
    m->motions_size = own_size;
    m->motions2 = second;
    if (index < 0) {
        return 0;
    }
    for (r = 0; r < m->nbones && m->bones[r].parent >= 0; r++) {
    }
    if (r == m->nbones) {
        return 0;
    }
    *pos = vec3(root.x, root.y - m->bones[r].rest_pos[1], root.z);
    inv = &m->bones[r].inv_bind;   /* (its rotation's rows: the bind pose's axes) */
    for (k = 1; k < 3; k++) {   /* the root's most level axis */
        if (fabsf(inv->m[4 + k]) < fabsf(inv->m[4 + best])) {
            best = k;
        }
    }
    bind = vec3(inv->m[best], inv->m[4 + best], inv->m[8 + best]);
    end = model_pose_axes()[r][best];
    *yaw = atan2f(end.x, end.z) - atan2f(bind.x, bind.z);
    *yaw = atan2f(sinf(*yaw), cosf(*yaw));
    return 1;
}

/* the posed vertices and normals, in room space (the shader lights them) */
static void skin(Actor *a) {
    static Mat4 skin_m[MODEL_MAX_BONES];
    Model *m = &a->model;
    Mat4 place = mat4_identity();
    float c = cosf(a->yaw), s = sinf(a->yaw);
    int i, j;

    if (a->drive != NULL) {   /* a cutscene's motion: placed in the room by its root bone */
        const uint8_t *own = m->motions, *second = m->motions2;
        size_t own_size = m->motions_size;
        Vec3 root = a->pos;
        int root_bone;

        m->motions = a->drive;
        m->motions_size = a->drive_size;
        m->motions2 = NULL;   /* (the scene's motion alone) */
        if (a->cloth != NULL) {   /* (its root in the room: the bones are there but for the scale) */
            float k = a->scale != 0.0f ? 1.0f / a->scale : 1.0f;

            sHookActor = a;
            sHookPlace = sHookUnplace = mat4_identity();
            sHookPlace.m[0] = sHookPlace.m[5] = sHookPlace.m[10] = a->scale;
            sHookUnplace.m[0] = sHookUnplace.m[5] = sHookUnplace.m[10] = k;
            model_pose_hook(pose_hook, NULL);
        }
        model_pose_root(m, model_motion_find(m, 0x8000), a->drive_frame, skin_m, &root);
        model_pose_hook(NULL, NULL);
        face_channels(a, m, model_motion_find(m, 0x8000), a->drive_frame);
        m->motions = own;
        m->motions_size = own_size;
        m->motions2 = second;
        for (root_bone = 0; root_bone < m->nbones && m->bones[root_bone].parent >= 0; root_bone++) {
        }
        a->pos = vec3(root.x, root.y - (root_bone < m->nbones ? m->bones[root_bone].rest_pos[1] : 0.0f), root.z);
        c = 1.0f;
        s = 0.0f;
        place.m[0] = place.m[5] = place.m[10] = a->scale;
    } else {
        float x = a->fade_len > 0.0f && a->fade > 0.0f ? a->fade / a->fade_len : 0.0f;   /* (fade_weight) */
        /* (a motion come to its end fades out from its last frame: prev_held) */
        float prev_at = a->prev_held ? (float)(model_motion_frames(&a->model, a->prev_motion) - 1) : a->prev_frame;
        ModelLayer l[4];
        int nl = 0, k, body = 0x03;

        for (k = 0; k < 3; k++) {   /* the parts with no motion of their own go with the body */
            if (a->parts[k].motion < 0) {
                body |= 4 << k;
            }
        }
        l[nl++] = (ModelLayer){a->motion, a->frame, a->prev_motion, prev_at, x * x * (3.0f - 2.0f * x), body,
                               a->variant, a->vframe, 1.0f - a->vweight, a->prev_variant, a->prev_vframe,
                               1.0f - a->prev_vweight};
        for (k = 0; k < 3; k++) {
            const struct ActorPart *p = &a->parts[k];
            float y = p->fade_len > 0.0f && p->fade > 0.0f ? p->fade / p->fade_len : 0.0f;

            if (p->motion >= 0) {
                l[nl++] = (ModelLayer){p->motion, p->frame, p->prev, p->prev_frame, y * y * (3.0f - 2.0f * y), 4 << k, -1, 0.0f, 0.0f, -1, 0.0f, 0.0f};
            }
        }
        place.m[0] = c * a->scale;  place.m[2] = -s * a->scale;
        place.m[5] = a->scale;
        place.m[8] = s * a->scale;  place.m[10] = c * a->scale;
        place.m[12] = a->pos.x;
        place.m[13] = a->hlegs.on && a->hlegs.fitted ? a->hlegs.height : a->pos.y;   /* (BodyFrames' +0x804) */
        place.m[14] = a->pos.z;
        model_pose_turns(a->turns, a->nturns);
        if (a->legs.on || a->hlegs.on || a->cloth != NULL) {
            sHookActor = a;
            sHookPlace = place;
            sHookUnplace = unplace_of(a, place.m[13]);
            model_pose_hook(pose_hook, NULL);
        }
        model_pose_layers(m, l, nl, skin_m, NULL);
        model_pose_hook(NULL, NULL);
        model_pose_turns(NULL, 0);
        if (a->motion >= 0) {
            face_channels(a, m, a->motion, a->frame);
        }
    }
    if (a->shaped != NULL) {
        model_morph(m, &a->face, a->shaped);
    }
    for (i = 0; i < m->nbones; i++) {   /* the bones' origins and axes, placed */
        Vec3 o = mat4_point(&place, vec3(0.0f, 0.0f, 0.0f));
        int k;

        a->bones[i] = mat4_point(&place, model_pose_origins()[i]);
        for (k = 0; k < 3; k++) {
            a->bone_axes[i][k] = vec3_sub(mat4_point(&place, model_pose_axes()[i][k]), o);
        }
    }
    for (i = 0; i < m->nv; i++) {
        const SkinVertex *v = a->shaped != NULL ? &a->shaped[i] : &m->v[i];
        MeshVertex *o = &a->posed[i];
        Vec3 p = vec3(0, 0, 0), n = vec3(0, 0, 0);

        for (j = 0; j < 4; j++) {
            const Mat4 *b = &skin_m[v->bones[j] < m->nbones ? v->bones[j] : 0];
            float w = v->weights[j];

            if (w == 0.0f) {
                continue;
            }
            p = vec3_add(p, vec3_scale(mat4_point(b, v->pos), w));
            n = vec3_add(n, vec3_scale(vec3(b->m[0] * v->normal.x + b->m[4] * v->normal.y + b->m[8] * v->normal.z,
                                            b->m[1] * v->normal.x + b->m[5] * v->normal.y + b->m[9] * v->normal.z,
                                            b->m[2] * v->normal.x + b->m[6] * v->normal.y + b->m[10] * v->normal.z),
                                       w));
        }
        p = mat4_point(&place, p);
        n = vec3_norm(vec3(c * n.x + s * n.z, n.y, -s * n.x + c * n.z));
        o->x = p.x;
        o->y = p.y;
        o->z = p.z;
        o->s = v->s;
        o->t = v->t;
        o->rgba[0] = o->rgba[1] = o->rgba[2] = o->rgba[3] = 0x80;
        o->nx = n.x;
        o->ny = n.y;
        o->nz = n.z;
    }
}

/* a soft dark disc on the floor under the actor, a little above it to stay in front */
static void draw_shadow(Actor *a, const Mat4 *view_proj) {
    static const float kCorner[6][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, -1}, {1, 1}, {-1, 1}};
    static const uint32_t kAllGroups[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};
    MeshVertex q[6];
    MeshDraw d;
    GpuTexture blob = render_blob_texture();
    float r = a->shadow_size * a->scale;
    int i;

    memset(q, 0, sizeof(q));
    for (i = 0; i < 6; i++) {
        q[i].x = a->pos.x + kCorner[i][0] * r;
        q[i].y = a->pos.y + 0.3f;
        q[i].z = a->pos.z + kCorner[i][1] * r;
        q[i].s = kCorner[i][0] * 0.5f + 0.5f;
        q[i].t = kCorner[i][1] * 0.5f + 0.5f;
        q[i].rgba[3] = 0x50;   /* black, at about 60% */
    }
    memset(&d, 0, sizeof(d));
    d.first = 0;
    d.count = 6;
    d.texture = 0;
    d.blend = 1;
    d.no_zwrite = 1;
    render_mesh_update(&a->shadow, q, 6);
    render_mesh(&a->shadow, view_proj, &d, 1, &blob, 1, kAllGroups);
}

void actor_prepare(Actor *a) {
    if (!a->used || !a->visible || a->model.nv == 0) {
        return;
    }
    skin(a);
    render_mesh_update(&a->gpu, a->posed, a->model.nv);
}

Vec3 actor_center(const Actor *a) {
    return vec3(a->pos.x, a->pos.y + (a->lo.y + a->hi.y) * 0.5f * a->scale, a->pos.z);
}

float actor_radius(const Actor *a) {
    Vec3 size = vec3_sub(a->hi, a->lo);

    return 0.5f * a->scale * fmaxf(size.y, fmaxf(size.x, size.z));
}

void actor_draw(Actor *a, const Mat4 *view_proj) {

    if (!a->used || !a->visible || a->model.nv == 0) {
        return;
    }
    {
        float k = 1.0f / 128.0f, alpha = a->alpha * (float)(a->tint & 0xFF) * k;

        if (alpha <= 0.0f) {
            return;
        }
        if (gRender.shadows && a->shadow_size > 0.0f && !a->shadow_off && alpha >= 1.0f) {
            draw_shadow(a, view_proj);
        }
        render_mesh_tint((float)(a->tint >> 24) * k, (float)(a->tint >> 16 & 0xFF) * k, (float)(a->tint >> 8 & 0xFF) * k, alpha);
        render_mesh_ps2_clip(1);
        render_mesh(&a->gpu, view_proj, a->model.d, a->model.nd, a->textures, a->ntextures, a->parts_shown);
        render_mesh_ps2_clip(0);
        render_mesh_tint(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
