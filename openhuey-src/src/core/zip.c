/* Reading .zip files (zip.h): the central directory read once; a file's data read and
 * inflated (stb_image's zlib) when asked for. Writing one: deflated by stb_image_write's zlib. */
#define _FILE_OFFSET_BITS 64
#include "zip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "../third_party/stb_image.h"

unsigned char *stbi_zlib_compress(unsigned char *data, int data_len, int *out_len, int quality);   /* (stb.c) */

typedef struct ZipEntry {
    char *name;
    uint32_t at;      /* its local header */
    uint32_t packed, size;
    int method;       /* 0 stored, 8 deflated */
} ZipEntry;

struct Zip {
    char path[1024];
    ZipEntry *e;
    int n;
};

static uint16_t le16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }
static uint32_t le32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }

static Zip *refuse(Zip *z, FILE *f, uint8_t *buf, const char *path, const char *why) {
    fprintf(stderr, "zip: %s: %s\n", path, why);
    free(buf);
    if (f != NULL) {
        fclose(f);
    }
    zip_close(z);
    return NULL;
}

Zip *zip_open(const char *path) {
    FILE *f = fopen(path, "rb");
    uint8_t *buf = NULL, *p, *end;
    long size, tail, i;
    uint32_t cd_at, cd_size;
    int count, k;
    Zip *z;

    if (f == NULL) {
        return NULL;
    }
    if ((z = calloc(1, sizeof(*z))) == NULL) {
        fclose(f);
        return NULL;
    }
    snprintf(z->path, sizeof(z->path), "%s", path);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    tail = size < 65557 ? size : 65557;   /* (the end record: 22 bytes and a comment up to 64K) */
    buf = malloc((size_t)tail);
    if (buf == NULL || fseek(f, size - tail, SEEK_SET) != 0 || fread(buf, 1, (size_t)tail, f) != (size_t)tail) {
        return refuse(z, f, buf, path, "can't be read");
    }
    for (i = tail - 22; i >= 0 && le32(buf + i) != 0x06054b50; i--) {
    }
    if (i < 0) {
        return refuse(z, f, buf, path, "isn't a zip");
    }
    count = le16(buf + i + 10);
    cd_size = le32(buf + i + 12);
    cd_at = le32(buf + i + 16);
    if (count == 0xFFFF || cd_at == 0xFFFFFFFFu || (long)cd_at + (long)cd_size > size) {
        return refuse(z, f, buf, path, "is a zip64 or damaged (not read)");
    }
    free(buf);
    buf = malloc(cd_size + 1u);
    if (buf == NULL || fseek(f, (long)cd_at, SEEK_SET) != 0 || fread(buf, 1, cd_size, f) != cd_size) {
        return refuse(z, f, buf, path, "its directory can't be read");
    }
    fclose(f);
    f = NULL;
    z->e = calloc((size_t)count + 1, sizeof(ZipEntry));
    p = buf;
    end = buf + cd_size;
    for (k = 0; k < count && z->e != NULL; k++) {
        int nlen, flags, j;
        ZipEntry *e;

        if (p + 46 > end || le32(p) != 0x02014b50) {
            return refuse(z, f, buf, path, "its directory is damaged");
        }
        nlen = le16(p + 28);
        flags = le16(p + 8);
        if (p + 46 + nlen > end) {
            return refuse(z, f, buf, path, "its directory is damaged");
        }
        e = &z->e[z->n];
        e->method = le16(p + 10);
        e->packed = le32(p + 20);
        e->size = le32(p + 24);
        e->at = le32(p + 42);
        if ((flags & 1) != 0) {
            return refuse(z, f, buf, path, "is encrypted");
        }
        if (nlen > 0 && p[46 + nlen - 1] != '/') {   /* (a folder's own entry: left out) */
            if ((e->name = malloc((size_t)nlen + 1)) == NULL) {
                return refuse(z, f, buf, path, "is too big to read");
            }
            memcpy(e->name, p + 46, (size_t)nlen);
            e->name[nlen] = 0;
            for (j = 0; j < nlen; j++) {
                e->name[j] = e->name[j] == '\\' ? '/' : e->name[j];
            }
            z->n++;
        }
        p += 46 + nlen + le16(p + 30) + le16(p + 32);
    }
    free(buf);
    return z;
}

void zip_close(Zip *z) {
    int i;

    if (z == NULL) {
        return;
    }
    for (i = 0; i < z->n; i++) {
        free(z->e[i].name);
    }
    free(z->e);
    free(z);
}

int zip_count(const Zip *z) { return z != NULL ? z->n : 0; }
const char *zip_name(const Zip *z, int i) { return z != NULL && i >= 0 && i < z->n ? z->e[i].name : NULL; }

int zip_find(const Zip *z, const char *name) {
    char want[1024];
    int i;

    if (z == NULL) {
        return -1;
    }
    snprintf(want, sizeof(want), "%s", name);
    for (i = 0; want[i] != 0; i++) {
        want[i] = want[i] == '\\' ? '/' : want[i];
    }
    for (i = 0; i < z->n; i++) {
        if (strcasecmp(z->e[i].name, want) == 0) {
            return i;
        }
    }
    return -1;
}

uint8_t *zip_read(const Zip *z, int i, size_t *size) {
    const ZipEntry *e;
    uint8_t head[30], *packed = NULL, *out = NULL;
    FILE *f;

    if (z == NULL || i < 0 || i >= z->n || (f = fopen(z->path, "rb")) == NULL) {
        return NULL;
    }
    e = &z->e[i];
    if (fseek(f, (long)e->at, SEEK_SET) != 0 || fread(head, 1, 30, f) != 30 || le32(head) != 0x04034b50 ||
        fseek(f, (long)e->at + 30 + le16(head + 26) + le16(head + 28), SEEK_SET) != 0 ||
        (packed = malloc(e->packed + 1u)) == NULL || fread(packed, 1, e->packed, f) != e->packed) {
        fprintf(stderr, "zip: %s: %s can't be read\n", z->path, e->name);
        free(packed);
        fclose(f);
        return NULL;
    }
    fclose(f);
    if (e->method == 0 && e->packed == e->size) {
        out = packed;
        packed = NULL;
    } else if (e->method == 8 && (out = malloc(e->size + 1u)) != NULL &&
               stbi_zlib_decode_noheader_buffer((char *)out, (int)e->size, (const char *)packed, (int)e->packed) !=
                   (int)e->size) {
        free(out);
        out = NULL;
    }
    free(packed);
    if (out == NULL) {
        fprintf(stderr, "zip: %s: %s isn't stored or deflated as OpenHuey reads (method %d)\n", z->path, e->name, e->method);
        return NULL;
    }
    out[e->size] = 0;
    if (size != NULL) {
        *size = e->size;
    }
    return out;
}

/* ---- writing ---- */

static uint32_t crc32_of(const uint8_t *p, size_t n) {
    uint32_t c = 0xFFFFFFFFu;
    size_t i;
    int k;

    for (i = 0; i < n; i++) {
        c ^= p[i];
        for (k = 0; k < 8; k++) {
            c = c & 1 ? c >> 1 ^ 0xEDB88320u : c >> 1;
        }
    }
    return ~c;
}

static void put16(uint8_t *p, unsigned v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }
static void put32(uint8_t *p, uint32_t v) { put16(p, v & 0xFFFF); put16(p + 2, v >> 16); }

static uint8_t *file_whole(const char *path, size_t *size) {
    FILE *f = fopen(path, "rb");
    uint8_t *buf = NULL;
    long n;

    if (f == NULL) {
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) == 0 && (n = ftell(f)) >= 0 && fseek(f, 0, SEEK_SET) == 0 &&
        (buf = malloc((size_t)n + 1)) != NULL && fread(buf, 1, (size_t)n, f) == (size_t)n) {
        *size = (size_t)n;
    } else {
        free(buf);
        buf = NULL;
    }
    fclose(f);
    return buf;
}

int zip_write(const char *path, const char *dir, const char *folder, const char *const *names, int n) {
    enum { MOST = 16 };
    struct { char name[256]; uint32_t at, crc, packed, size; int method; } e[MOST];
    uint8_t head[46];
    uint32_t cd_at, cd_size = 0;
    unsigned when, day;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    FILE *f;
    int i, count = 0, ok = 1;

    if (t == NULL) {
        return 0;
    }
    when = (unsigned)(t->tm_hour << 11 | t->tm_min << 5 | t->tm_sec / 2);   /* (MS-DOS time and date) */
    day = (unsigned)((t->tm_year - 80) << 9 | (t->tm_mon + 1) << 5 | t->tm_mday);
    if ((f = fopen(path, "wb")) == NULL) {
        fprintf(stderr, "zip: %s can't be written\n", path);
        return 0;
    }
    for (i = 0; i < n && count < MOST && ok; i++) {
        char from[1024];
        uint8_t *data, *z = NULL, *out;
        size_t size;
        int zlen = 0, len;

        snprintf(from, sizeof(from), "%s/%s", dir, names[i]);
        if ((data = file_whole(from, &size)) == NULL) {
            continue;
        }
        if (size > 0 && size < 0x7FFFFFFF) {
            z = stbi_zlib_compress(data, (int)size, &zlen, 8);
        }
        /* (stb's is zlib's: a 2-byte header and the Adler sum at the end round the deflated data) */
        if (z != NULL && zlen > 6 && (size_t)(zlen - 6) < size) {
            out = z + 2;
            e[count].packed = (uint32_t)(zlen - 6);
            e[count].method = 8;
        } else {
            out = data;
            e[count].packed = (uint32_t)size;
            e[count].method = 0;
        }
        snprintf(e[count].name, sizeof(e[count].name), "%s/%s", folder, names[i]);
        len = (int)strlen(e[count].name);
        e[count].at = (uint32_t)ftell(f);
        e[count].crc = crc32_of(data, size);
        e[count].size = (uint32_t)size;
        memset(head, 0, 30);
        put32(head, 0x04034b50);
        put16(head + 4, 20);
        put16(head + 8, (unsigned)e[count].method);
        put16(head + 10, when);
        put16(head + 12, day);
        put32(head + 14, e[count].crc);
        put32(head + 18, e[count].packed);
        put32(head + 22, e[count].size);
        put16(head + 26, (unsigned)len);
        ok = fwrite(head, 1, 30, f) == 30 && fwrite(e[count].name, 1, (size_t)len, f) == (size_t)len &&
             fwrite(out, 1, e[count].packed, f) == e[count].packed;
        free(z);
        free(data);
        count++;
    }
    cd_at = (uint32_t)ftell(f);
    for (i = 0; i < count && ok; i++) {
        int len = (int)strlen(e[i].name);

        memset(head, 0, 46);
        put32(head, 0x02014b50);
        put16(head + 4, 20);
        put16(head + 6, 20);
        put16(head + 10, (unsigned)e[i].method);
        put16(head + 12, when);
        put16(head + 14, day);
        put32(head + 16, e[i].crc);
        put32(head + 20, e[i].packed);
        put32(head + 24, e[i].size);
        put16(head + 28, (unsigned)len);
        put32(head + 42, e[i].at);
        ok = fwrite(head, 1, 46, f) == 46 && fwrite(e[i].name, 1, (size_t)len, f) == (size_t)len;
        cd_size += 46u + (uint32_t)len;
    }
    memset(head, 0, 22);
    put32(head, 0x06054b50);
    put16(head + 8, (unsigned)count);
    put16(head + 10, (unsigned)count);
    put32(head + 12, cd_size);
    put32(head + 16, cd_at);
    ok = ok && fwrite(head, 1, 22, f) == 22;
    if (fclose(f) != 0 || !ok) {
        fprintf(stderr, "zip: %s can't be written\n", path);
        remove(path);
        return 0;
    }
    return 1;
}
