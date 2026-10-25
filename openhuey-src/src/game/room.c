#include "room.h"

#include "../core/files.h"
#include "../data/tex.h"
#include "../data/texbank.h"
#include "../data/roomgltf.h"
#include "../data/roomtext.h"
#include "sprites.h"
#include "../render/post.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int room_exists(int id) {
    char path[64];

    if (id < 0 || id > 0xFFF) {
        return 0;
    }
    pac_room_path(id, path, sizeof(path));
    return files_exist(path);
}

/* a room's texture bank: the library's PNGs of it (ST_003.room/<name>.texbank/: data/texbank.h),
 * else the PAC's own section */
static TexBank *room_bank(Room *r, int section, const char *name) {
    char path[96];
    size_t size;
    const uint8_t *bank = pac_section(&r->pac, section, &size);

    pac_room_part(r->id, name, path, sizeof(path));
    return texbank_load_or_wrap(path, bank, bank != NULL ? size : 0);
}

/* the library's sections as text (ST_003.room/lights.txt ...: data/roomtext.h), read back into
 * the PAC (a mod's over the library's, or over the PAC's own) */
static void attach_texts(Room *r) {
    char path[96];
    int i;

    for (i = 0; i < PAC_SECTIONS; i++) {
        uint8_t *text, *sec;
        size_t len, n;

        if (roomtext_file(i) == NULL) {
            continue;
        }
        pac_room_part(r->id, roomtext_file(i), path, sizeof(path));
        if ((text = files_read(path, &len)) == NULL) {
            continue;
        }
        if ((sec = roomtext_read(i, (const char *)text, len, path, &n)) != NULL) {
            pac_attach(&r->pac, i, sec, n);
        }
        free(text);
        free(sec);
    }
}

/* the library's mesh: ST_003.room/mesh.gltf + mesh.bin (data/roomgltf.h); 0 if there is none */
static int load_mesh_gltf(Room *r) {
    char path[96];
    uint8_t *json, *bin;
    size_t len, bs;
    int ok;

    pac_room_part(r->id, "mesh.gltf", path, sizeof(path));
    if ((json = files_read(path, &len)) == NULL) {
        return 0;
    }
    pac_room_part(r->id, "mesh.bin", path, sizeof(path));
    bin = files_read(path, &bs);
    ok = bin != NULL && roomgltf_read(&r->mesh, (const char *)json, len, bin, bs);
    if (!ok) {
        fprintf(stderr, "room %03X: its mesh.gltf isn't one OpenHuey reads\n", r->id);
    }
    free(json);
    free(bin);
    return ok;
}

static void load_textures(Room *r) {
    TexBank *bank = room_bank(r, PAC_TEXTURES, "textures.TEX");
    int n = texbank_count(bank), i;

    for (i = 0; i < n && i < ROOM_MAX_TEXTURES; i++) {
        int w = 0, h = 0, psm = 0;
        uint8_t *rgba = texbank_image(bank, i, -1, &w, &h);   /* (at its own size: a texture pack's bigger) */

        if (rgba != NULL) {
            r->textures[i] = render_texture(rgba, w, h);
        } else {
            texbank_info(bank, i, &w, &h, &psm);
            fprintf(stderr, "room %03X: texture %d: format %02X not decoded\n", r->id, i, psm);
        }
        free(rgba);
    }
    r->ntextures = n < ROOM_MAX_TEXTURES ? n : ROOM_MAX_TEXTURES;
    texbank_free(bank);
}

/* a PS2 colour word (little-endian R, G, B, A) */
static void colour(uint32_t c, float scale, float alpha_scale, float *out) {
    out[0] = (float)(c & 0xFF) / scale;
    out[1] = (float)((c >> 8) & 0xFF) / scale;
    out[2] = (float)((c >> 16) & 0xFF) / scale;
    out[3] = (float)(c >> 24) / alpha_scale;
}

static uint32_t word(const uint8_t *p) {
    uint32_t x;

    memcpy(&x, p, 4);
    return x;
}

/* the room's look: PAC section 13 - offsets (from the section) at +0x14 the depth of field
 * (four view distances), +0x8 the tint (two colours,
 * a mode byte: not 0 = not blurred), +0xC the screen blend (a colour, a mode byte: 1 / 4
 * subtract, 2 / 3 / 4 fixed colours), +0x10 the fog (near and far colours, near and far
 * distances) (src/game/effects.c Tint_SetParams, ScreenBlend_SetParams, Fog_SetParams) */
static void load_look(Room *r);

void room_reset_look(Room *r) {
    if (r->id >= 0) {
        load_look(r);
    }
}

/* one of the look's effects from its parameters (the original's room effect slots 0x1C depth of
 * field, 0x1D fog, 0x1E screen blend, 0x1F tint: Tint_SetParams, ScreenBlend_SetParams,
 * Fog_SetParams, DepthRange_SetParams); NULL: that effect removed */
#define LOOK_RAW 26   /* (the longest: the fog's - Fog_SetParams reads 26 bytes) */
static uint8_t sLookRaw[4][LOOK_RAW];   /* each effect's parameters as given (slots 0x1C..0x1F) */
static size_t sLookN[4];
size_t room_look_get(int slot, uint8_t *out) {
    if (slot < 0x1C || slot > 0x1F) {
        return 0;
    }
    memcpy(out, sLookRaw[slot - 0x1C], sLookN[slot - 0x1C]);
    return sLookN[slot - 0x1C];
}
void room_look_set(int slot, const uint8_t *d, size_t n) {
    RoomLook *l = &gRoomLook;

    if (slot >= 0x1C && slot <= 0x1F) {
        sLookN[slot - 0x1C] = d != NULL ? (n < LOOK_RAW ? n : LOOK_RAW) : 0;
        if (d != NULL) {
            memcpy(sLookRaw[slot - 0x1C], d, sLookN[slot - 0x1C]);
        }
    }

    switch (slot) {
    case 0x1F:
        l->has_tint = d != NULL && n >= 9;
        if (l->has_tint) {
            colour(word(d), 128.0f, 256.0f, l->tint_glow);   /* (strength: alpha / 2, of 0x80) */
            colour(word(d + 4), 128.0f, 256.0f, l->tint_contrast);
            l->tint_sharp = d[8] != 0;
        }
        break;
    case 0x1E:
        l->has_bloom = d != NULL && n >= 5;
        if (l->has_bloom) {
            uint32_t c = word(d);
            int mode = d[4];

            c = mode == 2 ? 0x80004080u : (mode == 3 || mode == 4) ? 0x40404040u : c;
            colour(c, 128.0f, 256.0f, l->bloom);
            l->bloom_subtract = mode == 1 || mode == 4;
            l->bloom_mode = mode;
        }
        break;
    case 0x1D:
        l->has_fog = d != NULL && n >= 16;
        if (l->has_fog) {
            float range[2];

            /* (alpha 0x100 = opaque: the fog's palette halves it, Palette_Colour) */
            colour(word(d), 255.0f, 256.0f, l->fog_near_color);
            colour(word(d + 4), 255.0f, 256.0f, l->fog_far_color);
            memcpy(range, d + 8, 8);
            l->fog_near = range[0];
            l->fog_far = range[1];
            /* +16: the fog drawn early (layer 9, not $21 - Fog_Draw), the see-through parts before
             * it (RoomMgr_Draw); +20..25 the characters' light groups (Fog_Update: layer $B, after
             * the fog as $A is - not drawn differently here). Shorter parameters: 0 */
            l->fog_early = n >= 20 && word(d + 16) != 0;
        }
        break;
    case 0x1C:
        l->has_dof = d != NULL && n >= 16;
        if (l->has_dof) {
            memcpy(l->dof, d, 16);
        }
        break;
    }
}

static void load_look(Room *r) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_EFFECTS, &size);

    memset(&gRoomLook, 0, sizeof(gRoomLook));
    post_glow_clear();   /* (a new room: the glow pass's buffer black) */
    if (sec == NULL || size < 0x18) {
        return;
    }
    if (word(sec + 0x8) != 0 && word(sec + 0x8) + 9 <= size) {
        room_look_set(0x1F, sec + word(sec + 0x8), 9);
    }
    if (word(sec + 0xC) != 0 && word(sec + 0xC) + 5 <= size) {
        room_look_set(0x1E, sec + word(sec + 0xC), 5);
    }
    if (word(sec + 0x10) != 0 && word(sec + 0x10) + 16 <= size) {
        /* (all 26 bytes, as Fog_SetParams reads them - past the section into what follows it in the
         * file, when the fog's are its last 16: the next section's first bytes) */
        size_t left = (size_t)(r->pac.data + r->pac_read - (sec + word(sec + 0x10)));

        room_look_set(0x1D, sec + word(sec + 0x10), left < LOOK_RAW ? left : LOOK_RAW);
    }
    if (word(sec + 0x14) != 0 && word(sec + 0x14) + 16 <= size) {   /* (DepthRange_SetParams) */
        room_look_set(0x1C, sec + word(sec + 0x14), 16);
    }
}

/* PAC section 4: a count, the ambient colour, then 0x30 bytes a light (src/game/lights.c
 * Lights_TakeRoom); none: no lights, the ambient 128 (Lights_Reset) */
static void load_lights(Room *r) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_LIGHTS, &size);
    int32_t n;
    int i;

    r->nlights = 0;
    r->ambient = vec3(128, 128, 128);
    if (sec == NULL || size < 16) {
        return;
    }
    memcpy(&n, sec, 4);
    memcpy(&r->ambient, sec + 4, 12);
    for (i = 0; i < n && i < ROOM_MAX_LIGHTS && 16 + (size_t)(i + 1) * 48 <= size; i++) {
        float v[12];

        memcpy(v, sec + 16 + i * 48, sizeof(v));
        r->lights[i].pos = vec3(v[0], v[1], v[2]);
        r->lights[i].color = vec3(v[4], v[5], v[6]);
        r->lights[i].intensity = v[7];
        r->lights[i].range = v[8];
        r->lights[i].shadow = v[11];
        r->lights[i].reach = v[9];
        r->lights[i].range2 = v[10];
        r->own_lights[i] = r->lights[i];
    }
    r->nlights = i;
}

int room_lights_at(const Room *r, Vec3 pos, int out[3]) {
    float h;

    return room_lights_on(r, navmesh_find(&r->nav, pos, 10.0f, &h), pos, out);
}

int room_lights_on(const Room *r, int tri, Vec3 pos, int out[3]) {
    float best[3] = {-1, -1, -1};
    int i, k, n = 0;
    uint32_t mask = tri >= 0 && tri < r->nav.ntris ? r->nav.tris[tri].lights : 0;

    out[0] = out[1] = out[2] = -1;
    if (tri < 0) {   /* (off the mesh: the room's light 0 alone, wherever it reaches - Lights_ForModel) */
        out[0] = r->nlights > 0 ? 0 : -1;
        return r->nlights > 0;
    }
    for (i = 0; i < r->nlights; i++) {
        const RoomLight *l = &r->lights[i];
        float score = (0.3f * l->color.x + 0.6f * l->color.y + 0.1f * l->color.z) * l->intensity;
        int idx = i;

        if (!(mask >> i & 1)) {
            continue;
        }
        if (l->range > 0.0f) {
            float d = vec3_len(vec3_sub(pos, l->pos));

            score = d < l->range ? score * (l->range - d) / l->range : 0.0f;
        }
        if (score <= 0.0f) {
            continue;
        }
        for (k = 0; k < 3; k++) {   /* kept in order, brightest first */
            if (score > best[k]) {
                float ts = best[k];
                int ti = out[k];

                best[k] = score;
                out[k] = idx;
                score = ts;
                idx = ti;
                if (idx < 0) {
                    break;
                }
            }
        }
    }
    for (k = 0; k < 3; k++) {
        n += out[k] >= 0;
    }
    return n;
}

/* the doors: section 7 (8 offsets by exit, 0 none: an entry is its id, a point +0x4, another
 * +0x10, its turn +0x1C) and section 8 (8 offsets; the nth present door's model at its offset
 * + n x 0x10 - Doors_TakeSection8) */
static float rdf(const uint8_t *p) {
    float f;

    memcpy(&f, p, 4);
    return f;
}

static void load_doors(Room *r) {
    size_t s7size, s8size;
    const uint8_t *s7 = pac_section(&r->pac, PAC_DOORS, &s7size), *s8 = pac_section(&r->pac, PAC_DOORS2, &s8size);
    int i, n = 0;

    if (s7 == NULL || s7size < 32) {
        return;
    }
    for (i = 0; i < ROOM_DOORS; i++) {
        RoomDoor *d = &r->doors[i];
        uint32_t o, m;

        memcpy(&o, s7 + i * 4, 4);
        if (o == 0 || o + 0x28 > s7size) {
            continue;
        }
        d->pos = vec3(rdf(s7 + o + 4), rdf(s7 + o + 8), rdf(s7 + o + 12));
        d->stand = vec3(rdf(s7 + o + 0x10), rdf(s7 + o + 0x14), rdf(s7 + o + 0x18));
        memcpy(&d->tri, s7 + o, 4);
        d->keys = NULL;
        d->sides = s7 + o + 0x28;
        d->rot = vec3(rdf(s7 + o + 0x1C), rdf(s7 + o + 0x20), rdf(s7 + o + 0x24));
        if (s8 != NULL && (size_t)(n + 1) * 4 <= s8size) {
            memcpy(&m, s8 + n * 4, 4);
            if (m != 0xFFFFFFFFu && m + (uint32_t)n * 0x10 < s8size &&
                roommesh_build_list(&d->mesh, s8 + m + n * 0x10, s8 + s8size, MESH_SOLID) && d->mesh.nv > 0) {
                render_mesh_upload(&d->gpu, d->mesh.v, d->mesh.nv);
                d->present = 1;
                d->shown = d->tri != -1;   /* (Doors_TakeRoom: +0x70 only with one) */
            }
        }
        n++;
    }
}

void room_door_swing(Room *r, int exit, float degrees, int at_once) {
    if (exit >= 0 && exit < ROOM_DOORS) {
        if (at_once) {
            r->doors[exit].swing = r->doors[exit].target = degrees;
            r->doors[exit].mode = 0;
        } else {
            room_door_move(r, exit, degrees < -45.0f, 0);
        }
    }
}

void room_door_move(Room *r, int exit, int open, int slam) {
    RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present) {
        return;
    }
    d = &r->doors[exit];
    d->mode = slam ? 3 : 2;
    d->closing = !open;
    d->sounded = 0;
    d->keys = NULL;
}

/* ---- the doors' animations for their users (FIN_D000.MTN) ---- */
static uint8_t *sDoorMtn;
static size_t sDoorMtnSize;

int room_door_anims(void) {
    if (sDoorMtn == NULL) {
        sDoorMtn = files_read("O_FIN/FIN_D000.MTN", &sDoorMtnSize);
    }
    return sDoorMtn != NULL;
}

/* animation `anim`'s record (x, z, turn, frames, then the swing a frame), NULL none */
static const uint8_t *door_anim(int anim, int *frames) {
    uint32_t n, off;
    int32_t k;

    if (!room_door_anims() || sDoorMtnSize < 4) {
        return NULL;
    }
    memcpy(&n, sDoorMtn, 4);
    if (anim < 0 || (uint32_t)anim >= n || (size_t)(anim + 2) * 4 > sDoorMtnSize) {
        return NULL;
    }
    memcpy(&off, sDoorMtn + 4 + anim * 4, 4);
    if ((size_t)off + 16 > sDoorMtnSize) {
        return NULL;
    }
    memcpy(&k, sDoorMtn + off + 12, 4);
    if (k < 0 || (size_t)off + 16 + (size_t)k * 4 > sDoorMtnSize) {
        return NULL;
    }
    *frames = k;
    return sDoorMtn + off;
}

/* (Mtx_AtHeading: x' = x cos + z sin, z' = z cos - x sin, placed at `at`) */
static Vec3 door_frame_point(Vec3 at, float yaw, float x, float z) {
    float c = cosf(yaw), s = sinf(yaw);

    return vec3(at.x + x * c + z * s, at.y, at.z + z * c - x * s);
}

int room_door_user_spot(const Room *r, int exit, int anim, Vec3 *at, float *yaw) {
    const RoomDoor *d;
    const uint8_t *rec;
    int frames, pass;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present || (rec = door_anim(anim, &frames)) == NULL) {
        return -1;
    }
    d = &r->doors[exit];
    for (pass = 0; pass < 2; pass++) {
        Vec3 to = door_frame_point(d->stand, d->rot.y, pass == 0 ? rdf(rec) : 0.0f, rdf(rec + 4));
        int tri = navmesh_walk(&r->nav, d->tri, d->stand, to, NULL);

        if (tri >= 0) {
            float y;

            *at = to;
            if (navmesh_find(&r->nav, to, 8.0f, &y) == tri) {
                at->y = y;
            }
            *yaw = atan2f(sinf(rdf(rec + 8) + d->rot.y), cosf(rdf(rec + 8) + d->rot.y));
            return tri;
        }
    }
    return -1;
}

int room_door_anim_start(Room *r, int exit, int anim) {
    RoomDoor *d;
    const uint8_t *rec;
    int frames;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present || (rec = door_anim(anim, &frames)) == NULL ||
        frames <= 0) {
        return 0;
    }
    d = &r->doors[exit];
    d->keys = rec + 16;
    d->nkeys = frames;
    d->key = 0;
    d->closing = d->swing < -45.0f;
    d->sounded = 0;
    d->mode = 1;
    return 1;
}

int room_door_side(const Room *r, int exit, Vec3 p) {
    const RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present) {
        return -1;
    }
    d = &r->doors[exit];
    return !(sinf(d->rot.y) * (p.x - d->stand.x) + cosf(d->rot.y) * (p.z - d->stand.z) < 0.0f);
}

static void door_side_flags(Room *r, const RoomDoor *d, int side, uint32_t flags, int set);

void room_door_side_flags(Room *r, int exit, int side, uint32_t flags, int set) {
    if (exit >= 0 && exit < ROOM_DOORS && r->doors[exit].present) {
        door_side_flags(r, &r->doors[exit], side, flags, set);
    }
}

int room_door_on_side(const Room *r, int exit, int side, int tri) {
    const RoomDoor *d;
    const uint8_t *t, *end;
    uint32_t n, k, v;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present || side < 0 || side > 1 || r->doors[exit].sides == NULL) {
        return 0;
    }
    d = &r->doors[exit];
    end = r->pac.data + r->pac.size;
    t = d->sides;
    if (t + 4 > end) {
        return 0;
    }
    memcpy(&n, t, 4);
    t += 4;
    if (side != 0) {
        t += (size_t)n * 4;
        if (t + 4 > end) {
            return 0;
        }
        memcpy(&n, t, 4);
        t += 4;
    }
    for (k = 0; k < n && t + 4 <= end; k++, t += 4) {
        memcpy(&v, t, 4);
        if ((int)v == tri) {
            return 1;
        }
    }
    return 0;
}

int room_door_near(const Room *r, int exit, Vec3 p) {
    const RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present) {
        return 0;
    }
    d = &r->doors[exit];
    return fabsf(d->stand.y - p.y) <= 5.0f &&
           sqrtf((d->stand.x - p.x) * (d->stand.x - p.x) + (d->stand.z - p.z) * (d->stand.z - p.z)) <= 20.0f;
}

int room_door_in_area(const Room *r, int exit, int kind, Vec3 p) {
    static const float kAreas[5][8] = {   /* (D_003E51A0: 0 its front, 1 by its open leaf - to slam it, 2 / 3
                                           * before it, 4 at its shut leaf - to burst it open) */
        {-6.0f, -12.0f, 6.0f, -12.0f, 6.0f, 12.0f, -6.0f, 12.0f},
        {-6.0f, 3.0f, -6.0f, 12.0f, -15.0f, 12.0f, -15.0f, 3.0f},
        {-8.0f, 0.0f, 8.0f, 0.0f, 8.0f, 14.0f, -8.0f, 14.0f},
        {-8.0f, 0.0f, 6.0f, 0.0f, 6.0f, 15.0f, -8.0f, 15.0f},
        {-3.0f, 0.0f, -3.0f, -9.0f, 6.0f, -9.0f, 6.0f, 0.0f}};
    const RoomDoor *d;
    Vec3 c[4];
    int k;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present || kind < 0 || kind > 4) {
        return 0;
    }
    d = &r->doors[exit];
    if (fabsf(d->stand.y - p.y) > 5.0f) {
        return 0;
    }
    for (k = 0; k < 4; k++) {
        c[k] = door_frame_point(d->stand, d->rot.y, kAreas[kind][k * 2], kAreas[kind][k * 2 + 1]);
    }
    for (k = 0; k < 4; k++) {
        Vec3 a = c[k], b = c[(k + 1) & 3];

        if ((b.x - a.x) * (p.z - a.z) - (b.z - a.z) * (p.x - a.x) < 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* flags set (or cleared) on one side's triangles (Doors_Passage) */
static void door_side_flags(Room *r, const RoomDoor *d, int side, uint32_t flags, int set) {
    const uint8_t *end = r->pac.data + r->pac.size, *t = d->sides;
    uint32_t n, k, tri;

    if (t == NULL || t + 4 > end) {
        return;
    }
    memcpy(&n, t, 4);
    t += 4;
    if (side != 0) {
        t += (size_t)n * 4;
        if (t + 4 > end) {
            return;
        }
        memcpy(&n, t, 4);
        t += 4;
    }
    for (k = 0; k < n && t + 4 <= end; k++, t += 4) {
        memcpy(&tri, t, 4);
        if (tri < (uint32_t)r->nav.ntris) {
            if (set) {
                r->nav.tris[tri].flags |= flags;
            } else {
                r->nav.tris[tri].flags &= ~flags;
            }
        }
    }
}

void room_door_passage(Room *r, int exit, int open, uint32_t locks) {
    const RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present) {
        return;
    }
    d = &r->doors[exit];
    door_side_flags(r, d, open ? 0 : 1, 0x60000, 0);
    door_side_flags(r, d, open ? 1 : 0, 0x60000, 1);
    door_side_flags(r, d, 0, 0x5000000, 0);
    if (locks != 0) {
        door_side_flags(r, d, 0, locks, 1);
    }
}

int room_door_set_opened(Room *r, int exit, int open) {
    RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present || r->doors[exit].mode == 1) {
        return 0;
    }
    d = &r->doors[exit];
    if (open) {
        d->sounded = 1;
        d->next_sound = 0x91;
        d->next_how = 3;
    }
    d->mode = 3;
    d->closing = !open;
    d->keys = NULL;
    door_side_flags(r, d, open ? 0 : 1, 0x60000, 0);
    door_side_flags(r, d, open ? 1 : 0, 0x60000, 1);
    return 1;
}

void room_door_angle(Room *r, int exit, float radians) {
    if (exit >= 0 && exit < ROOM_DOORS) {
        r->doors[exit].swing = r->doors[exit].target = (radians - r->doors[exit].rot.y) * 180.0f / 3.14159265f;
    }
}

int room_load(Room *r, int id) {
    char path[64];
    size_t size;
    const uint8_t *sec;

    room_free(r);
    pac_room_path(id, path, sizeof(path));
    r->pac.data = files_read(path, &r->pac.size);
    if (r->pac.data == NULL) {
        fprintf(stderr, "room: can't read %s\n", path);
        return 0;
    }
    r->id = id;
    r->pac_read = r->pac.size;
    attach_texts(r);
    if (!load_mesh_gltf(r)) {   /* (else the PAC's own section) */
        sec = pac_section(&r->pac, PAC_MESH, &size);
        if (sec != NULL && !roommesh_build(&r->mesh, sec, size)) {
            fprintf(stderr, "room %03X: the mesh section is malformed\n", id);
        }
    }
    if (r->mesh.nv > 0) {
        render_mesh_upload(&r->gpu, r->mesh.v, r->mesh.nv);
    }
    load_textures(r);
    /* the effects' textures: groups 0 and 0x15 of the cache (RoomMgr_MakeCurrent) */
    sprites_group_bank(0, room_bank(r, PAC_TEXTURES, "textures.TEX"));
    sprites_group_bank(0x15, room_bank(r, PAC_TEXTURES2, "effects.TEX"));
    sec = pac_section(&r->pac, PAC_NAV, &size);
    navmesh_build(&r->nav, sec, size);
    sec = pac_section(&r->pac, PAC_NAV3, &size);
    navmesh_take_flags(&r->nav, sec, sec != NULL ? size : 0);
    load_look(r);
    load_lights(r);
    load_doors(r);
    {
        size_t asize = 0;
        const uint8_t *anims = pac_section(&r->pac, PAC_PLACED2, &asize);

        sec = pac_section(&r->pac, PAC_PLACED, &size);
        placed_load(&r->placed, sec, sec != NULL ? size : 0, anims, asize);
    }
    if (r->mesh.ndyn > 0) {
        r->moving_v = malloc((size_t)r->mesh.ndv * 3 * sizeof(MeshVertex));
        r->moving_d = malloc((size_t)r->mesh.ndyn * sizeof(MeshDraw));
    }
    return 1;
}

void room_free(Room *r) {
    int i;

    for (i = 0; i < r->ntextures; i++) {
        render_texture_free(r->textures[i]);
    }
    render_mesh_free(&r->gpu);
    render_mesh_free(&r->moving);
    for (i = 0; i < ROOM_DOORS; i++) {
        render_mesh_free(&r->doors[i].gpu);
        roommesh_free(&r->doors[i].mesh);
    }
    placed_free(&r->placed);
    free(r->moving_v);
    free(r->moving_d);
    roommesh_free(&r->mesh);
    navmesh_free(&r->nav);
    sprites_group(0, NULL, 0);
    sprites_group(0x15, NULL, 0);
    free(r->pac.data);
    memset(r, 0, sizeof(*r));
    r->id = -1;
}

void room_tick(Room *r) {
    int i;

    roommesh_tick(&r->mesh);
    placed_tick(&r->placed);
    for (i = 0; i < ROOM_DOORS; i++) {   /* (Door_Swing) */
        RoomDoor *d = &r->doors[i];
        float step = d->mode == 3 ? 15.0f : 5.0f;

        d->sound = d->next_sound;
        d->sound_how = d->next_how;
        d->next_sound = d->next_how = 0;
        d->settled = -1;
        if (!d->shown) {   /* (Door_Swing: only a shown one) */
            continue;
        }
        switch (d->mode) {
        case 1:   /* along the user's animation, a frame a key; creak / latch once */
            d->swing = d->target = rdf(d->keys + (size_t)d->key * 4);
            if (++d->key >= d->nkeys) {   /* (no rest reported: its user lets it go) */
                d->keys = NULL;
                d->mode = 0;
            }
            if (!d->sounded) {
                if (d->closing) {
                    if (d->swing > -6.0f) {
                        d->sounded = 1;
                        d->sound = 0x28;
                        d->sound_how = 2;
                    }
                } else if (d->swing < 0.0f) {
                    d->sounded = 1;
                    d->sound = 0x27;
                    d->sound_how = 1;
                }
            }
            break;
        case 2:
        case 3:
            if (!d->closing) {
                d->swing -= step;
                if (d->swing < -90.0f) {
                    d->swing = -90.0f;
                    d->mode = 0;
                    d->settled = 1;
                }
            } else {
                d->swing += step;
                if (d->mode == 2 && !d->sounded && d->swing > -6.0f) {   /* (the latch, silent) */
                    d->sounded = 1;
                    d->sound = 0x28;
                }
                if (d->swing > 0.0f) {
                    if (d->mode == 3) {   /* slammed: the latch, loud */
                        d->sounded = 1;
                        d->sound = 0x28;
                        d->sound_how = 4;
                    }
                    d->swing = 0.0f;
                    d->mode = 0;
                    d->settled = 0;
                }
            }
            d->target = d->swing;
            break;
        }
    }
}

/* door i's model to room space: turned (its rest turn plus the swing about the vertical), then
 * placed (sceVu0RotMatrix, sceVu0TransMatrix) */
static Mat4 door_matrix(const RoomDoor *d) {
    float y = d->rot.y + d->swing * 3.14159265f / 180.0f;
    float cx = cosf(d->rot.x), sx = sinf(d->rot.x), cy = cosf(y), sy = sinf(y), cz = cosf(d->rot.z), sz = sinf(d->rot.z);
    Mat4 rx = mat4_identity(), ry = mat4_identity(), rz = mat4_identity(), t = mat4_identity();

    rx.m[5] = cx;  rx.m[6] = sx;  rx.m[9] = -sx;  rx.m[10] = cx;
    ry.m[0] = cy;  ry.m[2] = -sy; ry.m[8] = sy;   ry.m[10] = cy;
    rz.m[0] = cz;  rz.m[1] = sz;  rz.m[4] = -sz;  rz.m[5] = cz;
    t.m[12] = d->pos.x;
    t.m[13] = d->pos.y;
    t.m[14] = d->pos.z;
    return mat4_mul(t, mat4_mul(rz, mat4_mul(ry, rx)));
}

/* the renderer layer a mesh part draws in (RoomMgr_Draw): the solid part 1, the see-through and
 * flip-book parts $19 - 8 when the fog is drawn early (the fog effect's +0x1C: in layer 9, so
 * they are fogged and what follows isn't) - the glows $1F, the bloom mask $26; part 3 never */
static int part_layer(int part) {
    switch (part) {
    case MESH_SOLID: return 1;
    case MESH_SEE_THROUGH:
    case MESH_ANIMATED: return gRoomLook.has_fog && gRoomLook.fog_early ? 8 : 0x19;
    case MESH_GLOW: return 0x1F;
    case MESH_BLOOM_MASK: return 0x26;
    }
    return -1;
}

/* the draws of d[0..n) in renderer layer `layer`, each run of them at once */
static void draw_layer_runs(Room *r, const GpuMesh *g, const Mat4 *vp, const MeshDraw *d, int n, int layer) {
    int first = 0, end;

    while (first < n) {
        if (part_layer(d[first].part) != layer) {
            first++;
            continue;
        }
        for (end = first; end < n && part_layer(d[end].part) == layer; end++) {
        }
        render_mesh(g, vp, d + first, end - first, r->textures, r->ntextures, r->groups);
        first = end;
    }
}

void room_draw_begin(Room *r, Vec3 eye, Vec3 forward) {
    r->nmoving = 0;
    if (r->id >= 0 && r->mesh.ndyn > 0) {
        int n = roommesh_dynamic(&r->mesh, eye, forward, r->moving_v, r->moving_d), nv = 0, i;

        for (i = 0; i < n; i++) {
            nv = r->moving_d[i].first + r->moving_d[i].count > nv ? r->moving_d[i].first + r->moving_d[i].count : nv;
        }
        render_mesh_update(&r->moving, r->moving_v, nv > 0 ? nv : 1);
        r->nmoving = n;
    }
}

void room_draw_layer(Room *r, const Mat4 *view_proj, int layer) {
    int i;

    if (r->id < 0) {
        return;
    }
    render_mesh_ps2_clip(1);   /* (the room's microprograms reject as the characters' do) */
    draw_layer_runs(r, &r->gpu, view_proj, r->mesh.d, r->mesh.nd, layer);
    draw_layer_runs(r, &r->moving, view_proj, r->moving_d, r->nmoving, layer);
    if (layer == 1) {   /* (Doors_Draw: after the room's parts) */
        for (i = 0; i < ROOM_DOORS; i++) {
            if (r->doors[i].present && r->doors[i].shown) {
                Mat4 mvp = mat4_mul(*view_proj, door_matrix(&r->doors[i]));

                render_mesh(&r->doors[i].gpu, &mvp, r->doors[i].mesh.d, r->doors[i].mesh.nd, r->textures, r->ntextures,
                            r->doors[i].groups);
            }
        }
    }
    placed_draw(&r->placed, view_proj, r->textures, r->ntextures, layer);
    render_mesh_ps2_clip(0);
}

int room_floor_below(const Room *r, float x, float y, float z, float *out) {
    int i, k, found = 0;
    float best = -1e30f;

    for (i = 0; i < r->mesh.nd; i++) {
        const MeshDraw *d = &r->mesh.d[i];

        if (d->part != MESH_SOLID) {
            continue;
        }
        for (k = d->first; k + 2 < d->first + d->count; k += 3) {
            const MeshVertex *a = &r->mesh.v[k], *b = &r->mesh.v[k + 1], *c = &r->mesh.v[k + 2];
            /* (x, z) inside the triangle's shadow on the ground plane: barycentric coordinates */
            float det = (b->z - c->z) * (a->x - c->x) + (c->x - b->x) * (a->z - c->z);
            float l1, l2, l3, h;

            if (fabsf(det) < 1e-6f) {
                continue;   /* a wall */
            }
            l1 = ((b->z - c->z) * (x - c->x) + (c->x - b->x) * (z - c->z)) / det;
            l2 = ((c->z - a->z) * (x - c->x) + (a->x - c->x) * (z - c->z)) / det;
            l3 = 1.0f - l1 - l2;
            if (l1 < 0.0f || l2 < 0.0f || l3 < 0.0f) {
                continue;
            }
            h = l1 * a->y + l2 * b->y + l3 * c->y;
            if (h <= y && h > best) {
                best = h;
                found = 1;
            }
        }
    }
    if (found) {
        *out = best;
    }
    return found;
}

/* ---- the open doors' light (src/game/lights.c DoorShadow_*, Shadow_BlockerVolumes; doors.c
 * Door_Draw, door_portal) ---- */

/* the door's leaf in its own space (D_003EC080): 12 wide, 22 high, 1 thick - its two faces, each
 * four corners (top far, top near, bottom far, bottom near), the way it faces and its top middle */
static const float kLeaf[2][6][3] = {
    {{12, 22, 0}, {0, 22, 0}, {12, 0, 0}, {0, 0, 0}, {0, 0, 1}, {6, 22, 0}},
    {{0, 22, -1}, {12, 22, -1}, {0, 0, -1}, {12, 0, -1}, {0, 0, -1}, {6, 22, -1}},
};

static Vec3 leaf_point(const Mat4 *m, int face, int k) {
    return mat4_point(m, vec3(kLeaf[face][k][0], kLeaf[face][k][1], kLeaf[face][k][2]));
}

static Vec3 leaf_facing(const Mat4 *m, int face) {
    return vec3_norm(vec3_sub(leaf_point(m, face, 4), mat4_point(m, vec3(0, 0, 0))));
}

RoomView room_view(const Mat4 *proj, const Mat4 *view, Vec3 eye, float znear, float zfar) {
    RoomView v;
    float aspect = proj->m[0] != 0.0f ? proj->m[5] / proj->m[0] : 4.0f / 3.0f;

    v.vp = mat4_mul(*proj, *view);
    v.eye = eye;
    v.bx = (2047.0f / 256.0f) * (4.0f / 3.0f) / aspect;   /* (as render_mesh_ps2_clip) */
    v.by = 2047.0f / 224.0f;
    v.znear = znear;
    v.zfar = zfar;
    return v;
}

int room_in_view(const RoomView *v, Vec3 p) {
    const float *m = v->vp.m;
    float x = m[0] * p.x + m[4] * p.y + m[8] * p.z + m[12], y = m[1] * p.x + m[5] * p.y + m[9] * p.z + m[13];
    float w = m[3] * p.x + m[7] * p.y + m[11] * p.z + m[15];

    return w >= v->znear && w <= v->zfar && fabsf(x) <= v->bx * w && fabsf(y) <= v->by * w;
}

/* where a light shades from: 5 above it (DoorShadow_FromLight +0x60, Shadow_BlockerVolumes) */
static Vec3 shade_from(const RoomLight *l) {
    return vec3(l->pos.x, l->pos.y + 5.0f, l->pos.z);
}

/* DoorShadow_FromLight: the leaf's face away from the light, the strength - the light's, its
 * falloff at the face's top middle, the square of the angle it meets the face (times 4), the
 * room's ambient - and the box; 0 if out of the light's reach or nothing */
static int door_shade(const Room *r, const Mat4 *m, const RoomLight *l, RoomShade *s) {
    Vec3 lp = shade_from(l), d = vec3_sub(lp, leaf_point(m, 0, 5)), n;
    float fall = 1.0f, c, len = 0.75f * l->shadow;
    int face = 0, k, a;

    if (!(vec3_dot(d, leaf_facing(m, 0)) < 0.0f)) {
        face = 1;
        d = vec3_sub(lp, leaf_point(m, 1, 5));
    }
    if (l->range2 != 0.0f) {
        float f = l->range2 - vec3_dot(d, d);

        if (f < 0.0f) {
            return 0;
        }
        f = f * l->reach * l->reach;
        f = f * f;
        fall = f * f;
    }
    n = leaf_facing(m, face);
    c = -vec3_dot(vec3_norm(d), n);
    a = (int)(4.0f * c * c * fall * l->intensity * (64.0f + (r->ambient.x + r->ambient.y + r->ambient.z) / 3.0f));
    if (a <= 0) {
        return 0;
    }
    s->alpha = a > 0x80 ? 0x80 : (float)a;
    for (k = 0; k < 4; k++) {
        s->p[k] = leaf_point(m, face, k);
        s->p[k + 4] = vec3_add(s->p[k], vec3_scale(vec3_norm(vec3_sub(s->p[k], lp)), len));
    }
    return 1;
}

/* the door's leaf shown: its model's last part's group is 0 or on (PlacedMesh_LastShown) */
static int leaf_shown(const RoomDoor *d) {
    int g = d->mesh.nd > 0 ? d->mesh.d[d->mesh.nd - 1].group : 0;

    return g == 0 || (d->groups[g >> 5] >> (g & 31) & 1u);
}

int room_door_shades(const Room *r, const RoomView *v, RoomShade *out, int max) {
    int i, k, n = 0;

    if (r->id < 0) {
        return 0;
    }
    for (i = 0; i < ROOM_DOORS; i++) {
        const RoomDoor *d = &r->doors[i];
        int lights[3];
        Mat4 m;

        if (!d->present || !d->shown || !leaf_shown(d) || !(d->swing < 0.0f)) {
            continue;
        }
        /* a shadow may fall at it (lights +0x2C): the eye above its triangle's plane */
        if (!(vec3_dot(navmesh_normal(&r->nav, d->tri), vec3_sub(d->pos, v->eye)) < 0.0f)) {
            continue;
        }
        if (r->nlights > 0 && d->tri != -1) {   /* (Lights_ShadowLights) */
            room_lights_on(r, d->tri, d->pos, lights);
        } else {
            lights[0] = 0;
            lights[1] = lights[2] = -1;
        }
        m = door_matrix(d);
        for (k = 0; k < 3 && n < max; k++) {
            RoomShade *s = &out[n];
            int j, seen = 1;

            if (lights[k] < 0 || lights[k] >= r->nlights || !door_shade(r, &m, &r->lights[lights[k]], s)) {
                continue;
            }
            for (j = 0; j < 8; j++) {   /* (DoorShadow_Draw: none if a corner is out of view) */
                seen &= room_in_view(v, s->p[j]);
            }
            if (seen) {
                s->exit = i;
                s->light = lights[k];
                n++;
            }
        }
    }
    return n;
}

RoomBlocker room_blocker(const Vec3 q[4]) {
    RoomBlocker b;
    int k;

    for (k = 0; k < 4; k++) {
        b.q[k] = q[k];
    }
    b.facing = vec3_norm(vec3_cross(vec3_sub(q[1], q[0]), vec3_sub(q[2], q[0])));
    b.top = vec3(q[3].x + 0.5f * (q[0].x - q[3].x), q[2].y + 0.5f * (q[1].y - q[2].y), q[3].z + 0.5f * (q[0].z - q[3].z));
    return b;
}

int room_door_blockers(const Room *r, const RoomView *v, RoomBlocker *out, int n, int max) {
    int i, face, k;

    if (r->id < 0) {
        return n;
    }
    for (i = 0; i < ROOM_DOORS; i++) {
        const RoomDoor *d = &r->doors[i];
        Mat4 m;

        if (!d->present || !d->shown || !leaf_shown(d) || !(d->swing <= -78.75f)) {
            continue;
        }
        m = door_matrix(d);
        for (face = 0; face < 2 && n < max; face++) {   /* (door_portal: z 0, then -1) */
            RoomBlocker *b = &out[n];
            int seen = 1;

            for (k = 0; k < 4; k++) {
                b->q[k] = leaf_point(&m, face, k);
                seen &= room_in_view(v, b->q[k]);
            }
            if (!seen) {   /* (Lights_AddDoorway: only wholly in view) */
                continue;
            }
            b->facing = leaf_facing(&m, face);
            b->top = leaf_point(&m, face, 5);
            n++;
        }
    }
    return n;
}

/* inside the volume of blocker b from lp, `len` long: behind the quad (in the pyramid from the
 * light through it) and before the far end (its corners each `len` on) */
static int in_blocker_volume(const RoomBlocker *b, Vec3 lp, float len, Vec3 p) {
    static const int kRound[4] = {0, 1, 3, 2};   /* (the corners in turn round the quad) */
    Vec3 f[4], mid = vec3(0, 0, 0), fn;
    int k;

    if (!(vec3_dot(vec3_sub(p, b->q[0]), b->facing) > 0.0f)) {
        return 0;
    }
    for (k = 0; k < 4; k++) {
        mid = vec3_add(mid, vec3_scale(b->q[k], 0.25f));
    }
    for (k = 0; k < 4; k++) {
        Vec3 a = b->q[kRound[k]], c = b->q[kRound[(k + 1) & 3]];
        Vec3 sn = vec3_cross(vec3_sub(a, lp), vec3_sub(c, lp));

        if (vec3_dot(sn, vec3_sub(p, lp)) * vec3_dot(sn, vec3_sub(mid, lp)) < 0.0f) {
            return 0;
        }
        f[k] = vec3_add(b->q[k], vec3_scale(vec3_norm(vec3_sub(b->q[k], lp)), len));
    }
    fn = vec3_cross(vec3_sub(f[3], f[0]), vec3_sub(f[2], f[1]));   /* (the far end: across its diagonals) */
    mid = vec3_scale(vec3_add(vec3_add(f[0], f[1]), vec3_add(f[2], f[3])), 0.25f);
    return vec3_dot(fn, vec3_sub(p, mid)) * vec3_dot(fn, vec3_sub(lp, mid)) >= 0.0f;
}

int room_blocked(const RoomLight *l, const RoomBlocker *b, int n, Vec3 p) {
    Vec3 lp = shade_from(l);
    int i;

    if (!(l->shadow > 0.0f)) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        /* (only a quad facing away from the light shades it) */
        if (vec3_dot(vec3_sub(lp, b[i].top), b[i].facing) < 0.0f && in_blocker_volume(&b[i], lp, l->shadow, p)) {
            return 1;
        }
    }
    return 0;
}
