/* The models' conversion checked (docs/library-and-mods.md): every .PCK of the game data built as
 * the engine builds it, written as the import writes it (model.gltf + model.bin, the motion bank as
 * it is), read back as the engine reads the library: the same Model - its bones, the bone table,
 * every vertex and draw, the motion bank's bytes. And valid glTF (cgltf's checks). And a mod's
 * model with a bone of its own (Fiona's, as Blender might write it back: the joints in another
 * order, the new bone without OpenHuey's extras): the game's bones at their places, the new one
 * after them (docs/modding.md "A model's bones").
 *
 *   model-check <data folder> */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/model.h"
#include "../src/data/modelgltf.h"
#include "../src/third_party/cgltf.h"

static int sModels, sBad, sUnbuilt, sMorphs, sModded;
static long long sPck, sGltf;

/* Fiona with a bone a mod added ("finger", under her bone `under`, a vertex on it), written,
 * then made as Blender might: the skin's joints reversed (their inverse binds and the vertices'
 * joints with them), the new bone's extras gone - read back, the game's bones as they were */
static void check_mod(const char *name, const Model *a) {
    Model c = *a, b;
    SkinVertex *v = malloc(sizeof(SkinVertex) * (size_t)a->nv);
    int n = a->nbones, under = n - 1, i, ok;
    const float rot[3] = {0.1f, -0.2f, 0.3f}, pos[3] = {1.0f, 2.0f, 3.0f};
    uint8_t *bin;
    size_t len, bs;
    char *json, *at, *end, *joints;
    cgltf_options opts;
    cgltf_data *g = NULL;

    memcpy(v, a->v, sizeof(SkinVertex) * (size_t)a->nv);
    c.v = v;
    c.nbones = n + 1;
    memset(&c.bones[n], 0, sizeof(ModelBone));
    snprintf(c.bones[n].name, sizeof(c.bones[n].name), "finger");
    c.bones[n].parent = under;
    memcpy(c.bones[n].rest_rot, rot, sizeof(rot));
    memcpy(c.bones[n].rest_pos, pos, sizeof(pos));
    c.bones[n].inv_bind = mat4_identity();
    v[0].bones[0] = (uint8_t)n;
    json = modelgltf_write(&c, "model.bin", "../X.texbank/", &bin, &bs, &len);
    /* the new bone's extras gone */
    at = strstr(json, "{\"name\":\"finger\"");
    at = at != NULL ? strstr(at, ",\"extras\":") : NULL;
    end = at != NULL ? strstr(at, "\"}") : NULL;
    if (end == NULL) {
        fprintf(stderr, "FAIL %s (a mod's bone): no extras to take off\n", name);
        sBad++;
        goto out;
    }
    memmove(at, end + 2, strlen(end + 2) + 1);
    len = strlen(json);
    /* the joints reversed: the list, the inverse binds, each vertex's joints */
    memset(&opts, 0, sizeof(opts));
    if (cgltf_parse(&opts, json, len, &g) != cgltf_result_success) {
        fprintf(stderr, "FAIL %s (a mod's bone): not glTF\n", name);
        sBad++;
        goto out;
    }
    {
        const cgltf_accessor *ib = g->skins[0].inverse_bind_matrices;
        uint8_t *m = bin + ib->offset + ib->buffer_view->offset, tmp[64];
        const cgltf_accessor *done[256];
        int ndone = 0, k;
        size_t p, q;

        for (i = 0; i < (n + 1) / 2; i++) {
            memcpy(tmp, m + i * 64, 64);
            memcpy(m + i * 64, m + (n - i) * 64, 64);
            memcpy(m + (n - i) * 64, tmp, 64);
        }
        for (p = 0; p < g->meshes_count; p++) {
            for (q = 0; q < g->meshes[p].primitives_count; q++) {
                const cgltf_primitive *pr = &g->meshes[p].primitives[q];
                size_t t, e;

                for (t = 0; t < pr->attributes_count; t++) {
                    const cgltf_accessor *aj = pr->attributes[t].data;
                    uint8_t *j;

                    if (strcmp(pr->attributes[t].name, "JOINTS_0") != 0) {
                        continue;
                    }
                    for (k = 0; k < ndone && done[k] != aj; k++) {
                    }
                    if (k < ndone || ndone == 256) {
                        continue;
                    }
                    done[ndone++] = aj;
                    j = bin + aj->offset + aj->buffer_view->offset;
                    for (e = 0; e < aj->count * 4; e++) {
                        j[e] = (uint8_t)(n - j[e]);
                    }
                }
            }
        }
    }
    cgltf_free(g);
    joints = strstr(json, "\"joints\":[");
    end = joints != NULL ? strchr(joints, ']') : NULL;
    if (end != NULL) {
        char *list = malloc((size_t)(n + 1) * 5 + 2), *rest = strdup(end), *out;
        size_t ln = 0;

        for (i = n; i >= 0; i--) {
            ln += (size_t)sprintf(list + ln, "%s%d", i < n ? "," : "", i);
        }
        out = malloc(strlen(json) + ln + 16);
        memcpy(out, json, (size_t)(joints - json) + 10);
        sprintf(out + (joints - json) + 10, "%s%s", list, rest);
        free(json);
        free(list);
        free(rest);
        json = out;
        len = strlen(json);
    }
    {   /* (a broken file: an index list longer than its buffer - not read, not read past) */
        const char *at2 = strstr(json, "\"componentType\":5125,\"count\":");
        char *broken = malloc(strlen(json) + 8);
        Model x;

        if (at2 != NULL && broken != NULL) {   /* (its count with 999 before it: far more than there are, still JSON) */
            sprintf(broken, "%.*s999%s", (int)(at2 + 29 - json), json, at2 + 29);
        }
        if (at2 == NULL || broken == NULL || modelgltf_read(&x, broken, strlen(broken), bin, bs, NULL, 0)) {
            fprintf(stderr, "FAIL %s: a model with an index list past its buffer read\n", name);
            sBad++;
        }
        free(broken);
    }
    if (!modelgltf_read(&b, json, len, bin, bs, NULL, 0)) {
        fprintf(stderr, "FAIL %s (a mod's bone): not read\n", name);
        sBad++;
        goto out;
    }
    ok = b.nbones == n + 1 && b.ngame == n && memcmp(b.v, v, sizeof(SkinVertex) * (size_t)a->nv) == 0 &&
         model_bone_named(&b, "finger") == n && model_bone_named(&b, "bone 3") == 3 && b.bones[n].parent == under;
    for (i = 0; ok && i < n; i++) {
        ok = memcmp(&b.bones[i], &a->bones[i], sizeof(ModelBone)) == 0;
    }
    for (i = 0; ok && i < 3; i++) {   /* (its rest from its node: a quaternion's round trip) */
        ok = fabsf(b.bones[n].rest_rot[i] - rot[i]) < 1e-5f && fabsf(b.bones[n].rest_pos[i] - pos[i]) < 1e-5f;
    }
    if (!ok) {
        fprintf(stderr, "FAIL %s (a mod's bone): read back differs (%d bones, %d the game's, parent %d)\n", name,
                b.nbones, b.ngame, b.bones[n < b.nbones ? n : 0].parent);
        sBad++;
    } else {
        sModded++;
    }
    model_free(&b);
out:
    free(json);
    free(bin);
    free(v);
}

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
    if (strcasecmp(name, "DNL_000.PCK") == 0) {   /* (parts are numbered on through the rigid and morphing ones,
                                                    * as the original's draw flags: Daniella's part 31 is rigid) */
        int found = 0;

        for (i = 0; i < a.nd; i++) {
            found |= a.d[i].group == 32;
        }
        if (!found) {
            fprintf(stderr, "FAIL %s: no draw is its part 31 (group 32)\n", name);
            sBad++;
        }
    }
    if (strcasecmp(name, "FIN_000.PCK") == 0) {
        check_mod(name, &a);
    }
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
        same = same && a.nmorphs == b.nmorphs;
        for (i = 0; same && i < a.nmorphs; i++) {   /* (the faces and hands: each shape's every vertex) */
            const ModelMorph *x = &a.morphs[i], *y = &b.morphs[i];
            size_t n = (size_t)x->nshapes * (size_t)x->count * sizeof(Vec3);

            same = x->first == y->first && x->count == y->count && x->face == y->face && x->hand == y->hand &&
                   x->nshapes == y->nshapes && memcmp(x->dpos, y->dpos, n) == 0 && memcmp(x->dnrm, y->dnrm, n) == 0;
            if (!same) {
                fprintf(stderr, "FAIL %s: morph %d read back differs\n", name, i);
            }
        }
        if (a.nmorphs > 0) {
            sMorphs += a.nmorphs;
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
    printf("model-check: %d models (%lld MB of PCK; %d not models the engine builds; %d faces and hands) as %lld MB of glTF "
           "and motions, %d failures; %d mod's model with a bone of its own read\n", sModels, sPck >> 20, sUnbuilt, sMorphs,
           sGltf >> 20, sBad, sModded);
    return sBad != 0 || sModels == 0 || sModded == 0;
}
