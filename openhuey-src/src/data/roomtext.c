/* A room's sections as text: see roomtext.h. */
#include "roomtext.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gltfio.h"
#include "pac.h"

const char *roomtext_file(int section) {
    return section == PAC_LIGHTS ? "lights.txt" : section == PAC_CAMERAS ? "cameras.txt" : NULL;
}

/* ---- writing ---- */

static void word(GltfText *o, const uint8_t *p) {   /* a float, or its bits when %.9g can't give them back */
    float f;
    uint32_t bits, back_bits;
    char s[48];

    memcpy(&f, p, 4);
    memcpy(&bits, p, 4);
    snprintf(s, sizeof(s), "%.9g", f);
    {
        float back = strtof(s, NULL);

        memcpy(&back_bits, &back, 4);
    }
    if (isfinite(f) && back_bits == bits && strchr(s, '#') == NULL) {
        gltf_add(o, " %s", s);
    } else {
        gltf_add(o, " #%08x", bits);
    }
}

static void words(GltfText *o, const uint8_t *p, int n) {
    int i;

    for (i = 0; i < n; i++) {
        word(o, p + i * 4);
    }
}

static void tail(GltfText *o, const uint8_t *p, size_t n) {
    size_t i;

    if (n == 0) {
        return;
    }
    gltf_add(o, "tail ");
    for (i = 0; i < n; i++) {
        gltf_add(o, "%02x", p[i]);
    }
    gltf_add(o, "\n");
}

char *roomtext_write(int section, const uint8_t *sec, size_t size, const char *about, size_t *len) {
    GltfText o = {0};
    size_t at = 0;
    int32_t n;
    int i;

    gltf_add(&o, "\\ %s\n\\ hga-room-text %d (docs/library-and-mods.md, src/data/roomtext.h)\n", about, ROOMTEXT_FORMAT);
    if (section == PAC_LIGHTS) {
        if (size < 16) {
            free(o.s);
            return NULL;
        }
        memcpy(&n, sec, 4);
        gltf_add(&o, "\\ a light: position x y z, w; colour r g b; intensity; range; two unknown; shadow\nlights %d\nambient", n);
        words(&o, sec + 4, 3);
        gltf_add(&o, "\n");
        at = 16;
        for (i = 0; i < n && at + 48 <= size; i++, at += 48) {
            gltf_add(&o, "light");
            words(&o, sec + at, 4);
            gltf_add(&o, " ");
            words(&o, sec + at + 16, 3);
            gltf_add(&o, " ");
            words(&o, sec + at + 28, 1);
            words(&o, sec + at + 32, 1);
            gltf_add(&o, " ");
            words(&o, sec + at + 36, 2);
            gltf_add(&o, " ");
            words(&o, sec + at + 44, 1);
            gltf_add(&o, "\n");
        }
    } else if (section == PAC_CAMERAS) {
        gltf_add(&o, "\\ a camera: eye x y z; field of view (degrees across a 4:3 picture, 0: 60); target x y z; w\n");
        for (; at + 32 <= size; at += 32) {
            memcpy(&n, sec + at, 4);
            if (n == -1) {
                break;
            }
            gltf_add(&o, "camera");
            words(&o, sec + at, 3);
            gltf_add(&o, " ");
            words(&o, sec + at + 12, 1);
            gltf_add(&o, " ");
            words(&o, sec + at + 16, 3);
            gltf_add(&o, " ");
            words(&o, sec + at + 28, 1);
            gltf_add(&o, "\n");
        }
    } else {
        free(o.s);
        return NULL;
    }
    tail(&o, sec + at, size - at);
    *len = o.n;
    return o.s;
}

/* ---- reading ---- */

typedef struct Bytes {
    uint8_t *b;
    size_t n, cap;
} Bytes;

static void put(Bytes *t, const void *p, size_t n) {
    if (t->n + n > t->cap) {
        t->cap = (t->n + n) * 2 + 256;
        t->b = realloc(t->b, t->cap);
    }
    memcpy(t->b + t->n, p, n);
    t->n += n;
}

/* `n` words of a line (after its first word) into t: 0 if there aren't n of them as numbers */
static int read_words(Bytes *t, const char **s, int n) {
    int i;

    for (i = 0; i < n; i++) {
        char *end;

        while (**s == ' ' || **s == '\t') {
            (*s)++;
        }
        if (**s == '#') {
            uint32_t bits = (uint32_t)strtoul(*s + 1, &end, 16);

            if (end == *s + 1) {
                return 0;
            }
            put(t, &bits, 4);
        } else {
            float f = strtof(*s, &end);

            if (end == *s) {
                return 0;
            }
            put(t, &f, 4);
        }
        *s = end;
    }
    return 1;
}

uint8_t *roomtext_read(int section, const char *text, size_t len, const char *name, size_t *size) {
    Bytes t = {0};
    const char *s = text, *end = text + len;
    int line = 0, ok = 1;
    int32_t count = -1;

    if (section != PAC_LIGHTS && section != PAC_CAMERAS) {
        return NULL;
    }
    if (section == PAC_LIGHTS) {   /* (the header: written when "lights" and "ambient" are read) */
        uint8_t zero[16] = {0};

        put(&t, zero, 16);
    }
    while (s < end && ok) {
        const char *eol = memchr(s, '\n', (size_t)(end - s)), *w = s;
        char *ln;
        size_t n;

        eol = eol != NULL ? eol : end;
        n = (size_t)(eol - s);
        if ((ln = malloc(n + 1)) == NULL) {
            ok = 0;
            break;
        }
        memcpy(ln, s, n);
        ln[n] = 0;
        line++;
        s = eol + 1;
        w = ln;
        while (*w == ' ' || *w == '\t' || *w == '\r') {
            w++;
        }
        if (*w == 0 || *w == '\\') {
            free(ln);
            continue;
        }
        if (section == PAC_LIGHTS && strncmp(w, "lights ", 7) == 0) {
            count = (int32_t)strtol(w + 7, NULL, 10);
            memcpy(t.b, &count, 4);
        } else if (section == PAC_LIGHTS && strncmp(w, "ambient", 7) == 0) {
            Bytes a = {0};
            const char *p = w + 7;

            ok = read_words(&a, &p, 3);
            if (ok) {
                memcpy(t.b + 4, a.b, 12);
            }
            free(a.b);
        } else if (section == PAC_LIGHTS && strncmp(w, "light ", 6) == 0) {
            const char *p = w + 5;

            ok = read_words(&t, &p, 12);
        } else if (section == PAC_CAMERAS && strncmp(w, "camera ", 7) == 0) {
            const char *p = w + 6;

            ok = read_words(&t, &p, 8);
        } else if (strncmp(w, "tail ", 5) == 0) {
            const char *p = w + 5;

            while (p[0] != 0 && p[1] != 0 && p[0] != '\r') {
                unsigned x;

                if (sscanf(p, "%2x", &x) != 1) {
                    ok = 0;
                    break;
                }
                put(&t, &(uint8_t){(uint8_t)x}, 1);
                p += 2;
            }
        } else {
            ok = 0;
        }
        if (!ok) {
            fprintf(stderr, "room: %s:%d: not as the room's text has it\n", name, line);
        }
        free(ln);
    }
    if (!ok || (section == PAC_LIGHTS && count < 0)) {
        if (ok) {
            fprintf(stderr, "room: %s: no \"lights\" line\n", name);
        }
        free(t.b);
        return NULL;
    }
    *size = t.n;
    return t.b;
}
