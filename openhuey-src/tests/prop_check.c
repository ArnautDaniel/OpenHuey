/* Props' glTF read (src/data/propgltf.h, docs/modding.md "Props"): a model as Blender writes it -
 * a .gltf with its .bin and PNG beside it, one with its buffer and image as data: URIs, a .glb -
 * its nodes' transforms applied, its normals turned with them (or made from its triangles), its
 * base colour from the material's factor and the vertex colours (0x80 = 1.0), its texture read,
 * OPAQUE / MASK / BLEND as the material says, its bounds. And what isn't one: a missing buffer,
 * an index past its vertices, no triangles.
 *
 *   prop-check (needs nothing of the game's) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/data/propgltf.h"
#include "../src/third_party/stb_image_write.h"

static int sBad, sChecks;

#define CHECK(cond, ...)                         \
    do {                                         \
        sChecks++;                               \
        if (!(cond)) {                           \
            sBad++;                              \
            fprintf(stderr, "FAIL: " __VA_ARGS__); \
            fprintf(stderr, "\n");               \
        }                                        \
    } while (0)

/* ---- the files beside the glTF: kept in memory ---- */
typedef struct File {
    const char *name;
    uint8_t *b;
    size_t n;
} File;
static File sFiles[8];
static int sNFiles;

static void file_put(const char *name, const void *b, size_t n) {
    sFiles[sNFiles].name = name;
    sFiles[sNFiles].b = malloc(n);
    memcpy(sFiles[sNFiles].b, b, n);
    sFiles[sNFiles].n = n;
    sNFiles++;
}
static void files_drop(void) {
    while (sNFiles > 0) {
        free(sFiles[--sNFiles].b);
    }
}
static uint8_t *read_file(void *user, const char *rel, size_t *size) {
    int i;

    (void)user;
    for (i = 0; i < sNFiles; i++) {
        if (strcmp(sFiles[i].name, rel) == 0) {
            uint8_t *b = malloc(sFiles[i].n);

            memcpy(b, sFiles[i].b, sFiles[i].n);
            *size = sFiles[i].n;
            return b;
        }
    }
    return NULL;
}

/* ---- the model: a 4 x 4 square in the XY plane facing +Z, two triangles, indexed ---- */
static const float kPos[12] = {0, 0, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0};
static const float kNrm[12] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
static const float kUv[8] = {0, 1, 1, 1, 1, 0, 0, 0};
static const uint16_t kIdx[6] = {0, 1, 2, 0, 2, 3};
#define BIN_SIZE (sizeof(kPos) + sizeof(kNrm) + sizeof(kUv) + sizeof(kIdx))

static void bin(uint8_t *b) {
    memcpy(b, kPos, sizeof(kPos));
    memcpy(b + 48, kNrm, sizeof(kNrm));
    memcpy(b + 96, kUv, sizeof(kUv));
    memcpy(b + 128, kIdx, sizeof(kIdx));
}

/* its JSON: `buffer` the buffer's uri part ("uri":"..." or nothing for a .glb), `normals` whether
 * it has them, `image` the image ("uri":"..." / "bufferView":4 / NULL none), `alpha` its mode,
 * the node translated by (10, 0, 0), turned 90 degrees about Y, scaled 2 */
static int json(char *out, size_t cap, const char *buffer, int normals, const char *image, const char *alpha, int extra_view) {
    char view[80] = "";

    if (extra_view > 0) {   /* (a .glb's image: in the buffer after the model) */
        snprintf(view, sizeof(view), ",{\"buffer\":0,\"byteOffset\":140,\"byteLength\":%d}", extra_view);
    }
    return snprintf(out, cap,
        "{\"asset\":{\"version\":\"2.0\"},\"scene\":0,\"scenes\":[{\"nodes\":[0]}],"
        "\"nodes\":[{\"mesh\":0,\"translation\":[10,0,0],\"rotation\":[0,0.70710678,0,0.70710678],\"scale\":[2,2,2]}],"
        "\"buffers\":[{%s\"byteLength\":%d}],"
        "\"bufferViews\":[{\"buffer\":0,\"byteOffset\":0,\"byteLength\":48},{\"buffer\":0,\"byteOffset\":48,\"byteLength\":48},"
        "{\"buffer\":0,\"byteOffset\":96,\"byteLength\":32},{\"buffer\":0,\"byteOffset\":128,\"byteLength\":12}%s],"
        "\"accessors\":[{\"bufferView\":0,\"componentType\":5126,\"count\":4,\"type\":\"VEC3\",\"min\":[0,0,0],\"max\":[4,4,0]},"
        "{\"bufferView\":1,\"componentType\":5126,\"count\":4,\"type\":\"VEC3\"},"
        "{\"bufferView\":2,\"componentType\":5126,\"count\":4,\"type\":\"VEC2\"},"
        "{\"bufferView\":3,\"componentType\":5123,\"count\":6,\"type\":\"SCALAR\"}],"
        "\"meshes\":[{\"primitives\":[{\"attributes\":{\"POSITION\":0%s,\"TEXCOORD_0\":2},\"indices\":3,\"material\":0}]}],"
        "\"materials\":[{\"pbrMetallicRoughness\":{\"baseColorFactor\":[0.5,1,1,1]%s},\"alphaMode\":\"%s\"}]"
        "%s%s%s}",
        buffer, extra_view > 0 ? (int)BIN_SIZE + extra_view : (int)BIN_SIZE, view,
        normals ? ",\"NORMAL\":1" : "", image != NULL ? ",\"baseColorTexture\":{\"index\":0}" : "", alpha,
        image != NULL ? ",\"textures\":[{\"source\":0}],\"images\":[{" : "", image != NULL ? image : "",
        image != NULL ? "}]" : "");
}

static uint8_t sPng[4096];
static int sPngN;
static void png_out(void *ctx, void *data, int size) {
    (void)ctx;
    if (sPngN + size <= (int)sizeof(sPng)) {
        memcpy(sPng + sPngN, data, (size_t)size);
        sPngN += size;
    }
}
static void make_png(void) {   /* 2 x 2: red, green / blue, white */
    static const uint8_t px[16] = {255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 255};

    sPngN = 0;
    stbi_write_png_to_func(png_out, NULL, 2, 2, 4, px, 8);
}

static const char kB64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static void base64(const uint8_t *in, size_t n, char *out) {
    size_t i, o = 0;

    for (i = 0; i < n; i += 3) {
        uint32_t v = (uint32_t)in[i] << 16 | (i + 1 < n ? (uint32_t)in[i + 1] << 8 : 0) | (i + 2 < n ? in[i + 2] : 0);

        out[o++] = kB64[v >> 18 & 63];
        out[o++] = kB64[v >> 12 & 63];
        out[o++] = i + 1 < n ? kB64[v >> 6 & 63] : '=';
        out[o++] = i + 2 < n ? kB64[v & 63] : '=';
    }
    out[o] = 0;
}

static int near(float a, float b) {
    return fabsf(a - b) < 1e-4f;
}

/* the square as placed: turned 90 about Y (x -> -z), scaled 2, moved 10 along x - its corner
 * (4, 4, 0) at (10, 8, -8); its normal +Z turned to +X */
static void check_square(const PropMesh *m, const char *what, int textured) {
    const MeshVertex *v = m->v;

    CHECK(m->nv == 6 && m->nd == 1, "%s: 6 vertices, a draw (%d, %d)", what, m->nv, m->nd);
    if (m->nv != 6 || m->nd != 1) {
        return;
    }
    CHECK(near(v[2].x, 10) && near(v[2].y, 8) && near(v[2].z, -8), "%s: corner at (10 8 -8) (%g %g %g)", what, v[2].x, v[2].y, v[2].z);
    CHECK(near(v[0].x, 10) && near(v[0].y, 0) && near(v[0].z, 0), "%s: origin at (10 0 0)", what);
    CHECK(near(v[1].nx, 1) && near(v[1].ny, 0) && near(v[1].nz, 0), "%s: normal +X (%g %g %g)", what, v[1].nx, v[1].ny, v[1].nz);
    CHECK(near(v[2].s, 1) && near(v[2].t, 0), "%s: its UVs", what);
    CHECK(v[0].rgba[0] == 0x40 && v[0].rgba[1] == 0x80 && v[0].rgba[3] == 0x80, "%s: base colour 0.5 1 1 1 as 0x40 0x80 (%d %d %d)",
          what, v[0].rgba[0], v[0].rgba[1], v[0].rgba[3]);
    CHECK(m->d[0].first == 0 && m->d[0].count == 6 && m->d[0].lit, "%s: the draw lit, all six", what);
    CHECK(near(m->lo[0], 10) && near(m->hi[0], 10) && near(m->lo[2], -8) && near(m->hi[1], 8), "%s: bounds", what);
    if (textured) {
        CHECK(m->d[0].texture == 0 && m->nimages == 1 && m->images[0].w == 2 && m->images[0].h == 2 &&
                  m->images[0].rgba != NULL && m->images[0].rgba[0] == 255 && m->images[0].rgba[5] == 255,
              "%s: its 2 x 2 PNG read", what);
    } else {
        CHECK(m->d[0].texture == -1, "%s: untextured", what);
    }
}

int main(void) {
    char js[8192], uri[4096];
    static char b64[2048], img64[8192], json_img[9000];
    uint8_t b[BIN_SIZE], glb[16384];
    PropMesh m;
    int n, k;

    bin(b);
    make_png();

    /* a .gltf, its .bin and PNG beside it; opaque */
    file_put("square.bin", b, sizeof(b));
    file_put("tex/red.png", sPng, (size_t)sPngN);
    n = json(js, sizeof(js), "\"uri\":\"square.bin\",", 1, "\"uri\":\"tex/red.png\"", "OPAQUE", 0);
    k = propgltf_read(&m, (const uint8_t *)js, (size_t)n, read_file, NULL);
    CHECK(k, "the .gltf read");
    if (k) {
        check_square(&m, ".gltf", 1);
        CHECK(m.d[0].solid_tex && !m.d[0].blend, ".gltf: opaque");
        propgltf_free(&m);
    }

    /* the same, its buffer and image as data: URIs, no normals (made from the triangle), BLEND */
    base64(b, sizeof(b), b64);
    base64(sPng, (size_t)sPngN, img64);
    snprintf(uri, sizeof(uri), "\"uri\":\"data:application/octet-stream;base64,%s\",", b64);
    snprintf(json_img, sizeof(json_img), "\"uri\":\"data:image/png;base64,%s\"", img64);
    n = json(js, sizeof(js), uri, 0, json_img, "BLEND", 0);
    k = propgltf_read(&m, (const uint8_t *)js, (size_t)n, read_file, NULL);
    CHECK(k, "the embedded .gltf read");
    if (k) {
        check_square(&m, "embedded", 1);
        CHECK(m.d[0].blend && !m.d[0].solid_tex, "embedded: blended");
        propgltf_free(&m);
    }

    /* a .glb: the JSON chunk, then the BIN chunk (the buffer, then the PNG as buffer view 4); MASK */
    {
        int pad = (4 - sPngN % 4) % 4, jn, bl, total;
        uint32_t w;

        n = json(js, sizeof(js), "", 1, "\"bufferView\":4,\"mimeType\":\"image/png\"", "MASK", sPngN);
        (void)n;
        jn = (int)strlen(js);
        while (jn % 4 != 0) {
            js[jn++] = ' ';
        }
        js[jn] = 0;
        bl = (int)BIN_SIZE + sPngN + pad;
        total = 12 + 8 + jn + 8 + bl;
        memcpy(glb, "glTF", 4);
        w = 2;  memcpy(glb + 4, &w, 4);
        w = (uint32_t)total;  memcpy(glb + 8, &w, 4);
        w = (uint32_t)jn;  memcpy(glb + 12, &w, 4);
        memcpy(glb + 16, "JSON", 4);
        memcpy(glb + 20, js, (size_t)jn);
        w = (uint32_t)bl;  memcpy(glb + 20 + jn, &w, 4);
        memcpy(glb + 24 + jn, "BIN\0", 4);
        memcpy(glb + 28 + jn, b, BIN_SIZE);
        memcpy(glb + 28 + jn + BIN_SIZE, sPng, (size_t)sPngN);
        memset(glb + 28 + jn + BIN_SIZE + sPngN, 0, (size_t)pad);
        k = propgltf_read(&m, glb, (size_t)total, read_file, NULL);
        CHECK(k, "the .glb read");
        if (k) {
            check_square(&m, ".glb", 1);
            CHECK(!m.d[0].solid_tex && !m.d[0].blend, ".glb: masked");
            propgltf_free(&m);
        }
    }

    /* not one: its buffer missing */
    n = json(js, sizeof(js), "\"uri\":\"gone.bin\",", 1, NULL, "OPAQUE", 0);
    CHECK(!propgltf_read(&m, (const uint8_t *)js, (size_t)n, read_file, NULL), "a missing buffer: not read");
    CHECK(m.nv == 0 && m.v == NULL, "... and left empty");
    /* not one: an index past its four vertices */
    {
        uint8_t bad[BIN_SIZE];

        memcpy(bad, b, sizeof(bad));
        bad[128 + 10] = 9;
        file_put("bad.bin", bad, sizeof(bad));
        n = json(js, sizeof(js), "\"uri\":\"bad.bin\",", 1, NULL, "OPAQUE", 0);
        CHECK(!propgltf_read(&m, (const uint8_t *)js, (size_t)n, read_file, NULL), "an index past the vertices: not read");
    }
    /* not one: not glTF at all */
    CHECK(!propgltf_read(&m, (const uint8_t *)"hello", 5, read_file, NULL), "not glTF: not read");

    files_drop();
    printf("prop-check: %d checks, %d failed\n", sChecks, sBad);
    return sBad != 0;
}
