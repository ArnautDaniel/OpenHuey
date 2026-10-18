/* The rooms' conversion checked (docs/library-and-mods.md): every room's PAC as the library holds
 * it - less the sections converted (the texture banks 9 and 11, the text 10) - has every other
 * section the same bytes, the same size, as the disc's; the dropped ones none. (The converted
 * sections themselves: texbank-check, messages-check.)
 *
 *   room-check <data folder> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/pac.h"
#include "../src/data/roomgltf.h"
#include "../src/data/roommesh.h"
#include "../src/data/roomtext.h"
#include "../src/third_party/cgltf.h"

static int sRooms, sBad, sMeshes, sTexts;
static long long sBefore, sAfter, sGltf;

/* the room's mesh built from its section, written as glTF and read back: the same RoomMesh */
static void check_mesh(const char *name, const uint8_t *sec, size_t size) {
    RoomMesh a, b;
    char *json;
    uint8_t *bin;
    size_t len, bs;
    int i, same;

    if (!roommesh_build(&a, sec, size)) {
        return;   /* (malformed: the PAC keeps it) */
    }
    sMeshes++;
    json = roomgltf_write(&a, "mesh.bin", &bin, &bs, &len);
    sGltf += (long long)(len + bs);
    {   /* (glTF as the spec has it: cgltf's own checks) */
        cgltf_options opts;
        cgltf_data *g = NULL;

        memset(&opts, 0, sizeof(opts));
        if (cgltf_parse(&opts, json, len, &g) != cgltf_result_success) {
            fprintf(stderr, "FAIL %s: its mesh.gltf isn't glTF\n", name);
            sBad++;
        } else {
            g->buffers[0].data = bin;
            g->buffers[0].data_free_method = cgltf_data_free_method_none;
            cgltf_result r = cgltf_validate(g);

            if (r != cgltf_result_success) {
                fprintf(stderr, "FAIL %s: its mesh.gltf isn't valid glTF (cgltf: %d)\n", name, (int)r);
                sBad++;
            }
        }
        cgltf_free(g);
    }
    if (!roomgltf_read(&b, json, len, bin, bs)) {
        fprintf(stderr, "FAIL %s: its mesh.gltf isn't read back\n", name);
        sBad++;
        roommesh_free(&a);
        free(json);
        free(bin);
        return;
    }
    same = a.nv == b.nv && a.nd == b.nd && a.ndv == b.ndv && a.ndyn == b.ndyn &&
           memcmp(&a.lo, &b.lo, sizeof(a.lo)) == 0 && memcmp(&a.hi, &b.hi, sizeof(a.hi)) == 0 &&
           memcmp(a.v, b.v, sizeof(MeshVertex) * (size_t)a.nv) == 0 && memcmp(a.d, b.d, sizeof(MeshDraw) * (size_t)a.nd) == 0 &&
           memcmp(a.dv, b.dv, sizeof(MeshVertex) * (size_t)a.ndv) == 0;
    for (i = 0; same && i < a.ndyn; i++) {
        const DynBatch *x = &a.dyn[i], *y = &b.dyn[i];

        same = memcmp(&x->draw, &y->draw, sizeof(MeshDraw)) == 0 && memcmp(&x->local, &y->local, sizeof(Mat4)) == 0 &&
               memcmp(x->no_tri, y->no_tri, (size_t)x->draw.count) == 0 && x->parallax == y->parallax &&
               x->billboard == y->billboard && x->flip_type == y->flip_type && x->frames == y->frames &&
               x->frame_ticks == y->frame_ticks && x->frame == y->frame && x->timer == y->timer &&
               x->frame_w == y->frame_w && x->frame_h == y->frame_h;
    }
    if (!same) {
        fprintf(stderr, "FAIL %s: its mesh read back differs (%d/%d vertices, %d/%d draws, %d/%d batches)\n", name, b.nv,
                a.nv, b.nd, a.nd, b.ndyn, a.ndyn);
        sBad++;
    }
    roommesh_free(&a);
    roommesh_free(&b);
    free(json);
    free(bin);
}

static void check(const char *name, uint8_t *data, size_t size) {
    Pac a = {data, size}, b;
    unsigned drop = 1u << PAC_MESH | 1u << PAC_LIGHTS | 1u << PAC_CAMERAS | 1u << PAC_TEXTURES | 1u << PAC_TEXT |
                    1u << PAC_TEXTURES2;   /* (as the import drops them) */
    int i;

    {
        size_t ms;
        const uint8_t *mesh = pac_section(&a, PAC_MESH, &ms);

        if (mesh != NULL) {
            check_mesh(name, mesh, ms);
        }
    }
    for (i = 0; i < PAC_SECTIONS; i++) {   /* the sections as text: read back, the same bytes */
        size_t n = 0, tl, bn;
        const uint8_t *sec = pac_section(&a, i, &n);
        char *text;
        uint8_t *back;

        if (roomtext_file(i) == NULL || sec == NULL) {
            continue;
        }
        if ((text = roomtext_write(i, sec, n, name, &tl)) == NULL) {
            fprintf(stderr, "FAIL %s: section %d not written as text\n", name, i);
            sBad++;
            continue;
        }
        sTexts++;
        back = roomtext_read(i, text, tl, name, &bn);
        if (back == NULL || bn != n || memcmp(back, sec, n) != 0) {
            fprintf(stderr, "FAIL %s: %s read back differs (%zu bytes, %zu before)\n", name, roomtext_file(i), back ? bn : 0, n);
            sBad++;
        }
        free(text);
        free(back);
    }
    b.data = pac_strip(&a, drop, &b.size);
    sRooms++;
    if (b.data == NULL) {
        fprintf(stderr, "FAIL %s: not stripped\n", name);
        sBad++;
        return;
    }
    sBefore += (long long)size;
    sAfter += (long long)b.size;
    for (i = 0; i < PAC_SECTIONS; i++) {
        size_t na = 0, nb = 0;
        const uint8_t *sa = pac_section(&a, i, &na), *sb = pac_section(&b, i, &nb);

        if (drop >> i & 1) {
            if (sb != NULL) {
                fprintf(stderr, "FAIL %s: section %d still there\n", name, i);
                sBad++;
            }
        } else if ((sa == NULL) != (sb == NULL) || na != nb || (sa != NULL && memcmp(sa, sb, na) != 0)) {
            fprintf(stderr, "FAIL %s: section %d differs (%zu bytes, %zu before)\n", name, i, nb, na);
            sBad++;
        } else if (sb != NULL && ((size_t)(sb - b.data) & 15) != ((size_t)(sa - a.data) & 15)) {
            fprintf(stderr, "FAIL %s: section %d moved off its alignment\n", name, i);
            sBad++;
        }
    }
    free(b.data);
}

static SDL_EnumerationResult one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;
    size_t len = strlen(name), size;
    uint8_t *data;

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, one, user);
    } else if (len > 4 && strcasecmp(name + len - 4, ".PAC") == 0 && (data = SDL_LoadFile(path, &size)) != NULL) {
        check(name, data, size);
        SDL_free(data);
    }
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];

    if (argc != 2) {
        fprintf(stderr, "usage: room-check <data folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("room-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("room-check: %d rooms (%lld MB) as %lld MB of PAC less what is converted; %d meshes as %lld MB of glTF; %d "
           "sections as text; %d failures\n", sRooms, sBefore >> 20, sAfter >> 20, sMeshes, sGltf >> 20, sTexts, sBad);
    return sBad != 0 || sRooms == 0;
}
