/* The models' conversion checked (docs/library-and-mods.md): every .PCK of the game data built as
 * the engine builds it, written as the import writes it (model.gltf + model.bin, the motion bank as
 * it is), read back as the engine reads the library: the same Model - its bones, the bone table,
 * every vertex and draw, the motion bank's bytes. And valid glTF (cgltf's checks).
 *
 *   model-check <data folder> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/model.h"
#include "../src/data/modelgltf.h"
#include "../src/third_party/cgltf.h"

static int sModels, sBad, sUnbuilt;
static long long sPck, sGltf;

static void check(const char *name, uint8_t *pck, size_t size) {
    Model a, b;
    char *json;
    uint8_t *bin, *motions = NULL;
    size_t len, bs;
    int i, same;

    sPck += (long long)size;
    if (!model_build(&a, pck, size)) {   /* (takes pck) */
        sUnbuilt++;
        return;
    }
    sModels++;
    json = modelgltf_write(&a, "model.bin", "../X.texbank/", &bin, &bs, &len);
    sGltf += (long long)(len + bs + a.motions_size);
    {
        cgltf_options opts;
        cgltf_data *g = NULL;

        memset(&opts, 0, sizeof(opts));
        if (cgltf_parse(&opts, json, len, &g) != cgltf_result_success) {
            fprintf(stderr, "FAIL %s: its model.gltf isn't glTF\n", name);
            sBad++;
        } else {
            g->buffers[0].data = bin;
            g->buffers[0].data_free_method = cgltf_data_free_method_none;
            if (cgltf_validate(g) != cgltf_result_success) {
                fprintf(stderr, "FAIL %s: its model.gltf isn't valid glTF\n", name);
                sBad++;
            }
        }
        cgltf_free(g);
    }
    if (a.motions != NULL) {
        motions = malloc(a.motions_size);
        memcpy(motions, a.motions, a.motions_size);
    }
    if (!modelgltf_read(&b, json, len, bin, bs, motions, a.motions != NULL ? a.motions_size : 0)) {
        fprintf(stderr, "FAIL %s: its model.gltf isn't read back\n", name);
        sBad++;
    } else {
        same = a.nbones == b.nbones && a.nv == b.nv && a.nd == b.nd && a.motions_size == b.motions_size &&
               (a.bone_table == NULL) == (b.bone_table == NULL) &&
               (a.bone_table == NULL || memcmp(a.bone_table, b.bone_table, 256) == 0) &&
               memcmp(a.v, b.v, sizeof(SkinVertex) * (size_t)a.nv) == 0 && memcmp(a.d, b.d, sizeof(MeshDraw) * (size_t)a.nd) == 0 &&
               (a.motions == NULL || memcmp(a.motions, b.motions, a.motions_size) == 0);
        for (i = 0; same && i < a.nbones; i++) {
            same = memcmp(&a.bones[i], &b.bones[i], sizeof(ModelBone)) == 0;
        }
        if (!same) {
            int v = -1, d = -1, bn = -1;

            for (i = 0; v < 0 && i < a.nv && i < b.nv; i++) {
                v = memcmp(&a.v[i], &b.v[i], sizeof(SkinVertex)) != 0 ? i : -1;
            }
            for (i = 0; d < 0 && i < a.nd && i < b.nd; i++) {
                d = memcmp(&a.d[i], &b.d[i], sizeof(MeshDraw)) != 0 ? i : -1;
            }
            for (i = 0; bn < 0 && i < a.nbones; i++) {
                bn = memcmp(&a.bones[i], &b.bones[i], sizeof(ModelBone)) != 0 ? i : -1;
            }
            fprintf(stderr, "FAIL %s: read back differs (%d/%d bones, %d/%d vertices, %d/%d draws; first vertex %d, draw %d, "
                    "bone %d differing)\n", name, b.nbones, a.nbones, b.nv, a.nv, b.nd, a.nd, v, d, bn);
            sBad++;
        }
        model_free(&b);
    }
    model_free(&a);
    free(json);
    free(bin);
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
    } else if (len > 4 && strcasecmp(name + len - 4, ".PCK") == 0 && (data = SDL_LoadFile(path, &size)) != NULL) {
        uint8_t *copy = malloc(size);   /* (malloc'd: model_build takes it) */

        memcpy(copy, data, size);
        SDL_free(data);
        check(name, copy, size);
    }
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];

    if (argc != 2) {
        fprintf(stderr, "usage: model-check <data folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("model-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("model-check: %d models (%lld MB of PCK; %d not models the engine builds) as %lld MB of glTF and motions, %d "
           "failures\n", sModels, sPck >> 20, sUnbuilt, sGltf >> 20, sBad);
    return sBad != 0 || sModels == 0;
}
