/* The library's glTF files' shared parts: see gltfio.h. */
#include "gltfio.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gltf_add(GltfText *o, const char *fmt, ...) {
    va_list ap;
    int k;

    for (;;) {
        va_start(ap, fmt);
        k = vsnprintf(o->s != NULL ? o->s + o->n : NULL, o->s != NULL ? o->cap - o->n : 0, fmt, ap);
        va_end(ap);
        if (o->s != NULL && (size_t)k < o->cap - o->n) {
            o->n += (size_t)k;
            return;
        }
        o->cap = o->cap ? o->cap * 2 + (size_t)k : 1 << 16;
        o->s = realloc(o->s, o->cap);
    }
}

size_t gltf_put(GltfBin *b, const void *p, size_t n) {
    size_t at = b->n;

    b->b = realloc(b->b, b->n + n + 4);
    if (n > 0) {
        memcpy(b->b + b->n, p, n);
    }
    b->n += n;
    while (b->n & 3) {
        b->b[b->n++] = 0;
    }
    return at;
}

void gltf_distinct(const void *v, int count, size_t size, void *u, int *nu, uint32_t *idx) {
    int cap = 1, *slot, i, start = *nu;
    const uint8_t *src = v;
    uint8_t *dst = u;

    while (cap < count * 2) {
        cap <<= 1;
    }
    slot = malloc(sizeof(int) * (size_t)cap);
    memset(slot, 0xFF, sizeof(int) * (size_t)cap);
    for (i = 0; i < count; i++) {
        const uint8_t *k = src + (size_t)i * size;
        uint32_t h = 2166136261u;
        size_t j;

        for (j = 0; j < size; j++) {
            h = (h ^ k[j]) * 16777619u;
        }
        for (j = h & (uint32_t)(cap - 1); slot[j] >= 0 && memcmp(dst + (size_t)slot[j] * size, k, size) != 0;
             j = (j + 1) & (size_t)(cap - 1)) {
        }
        if (slot[j] < 0) {
            slot[j] = (*nu)++;
            memcpy(dst + (size_t)slot[j] * size, k, size);
        }
        idx[i] = (uint32_t)(slot[j] - start);
    }
    free(slot);
}

const char *gltf_hga(const cgltf_extras *e, const char *kind) {
    const char *s = e->data != NULL ? strstr(e->data, "\"hga\"") : NULL;
    size_t k = strlen(kind);

    if (s == NULL || (s = strchr(s + 5, '"')) == NULL || strncmp(s + 1, kind, k) != 0) {
        return NULL;
    }
    return s + 1 + k;
}

const uint8_t *gltf_bytes(const cgltf_accessor *a, size_t elem) {
    const cgltf_buffer_view *v = a->buffer_view;

    if (v == NULL || v->buffer->data == NULL || a->offset + a->count * elem > v->size ||
        v->offset + v->size > v->buffer->size) {
        return NULL;
    }
    return (const uint8_t *)v->buffer->data + v->offset + a->offset;
}

const cgltf_accessor *gltf_attribute(const cgltf_primitive *p, const char *name) {
    size_t i;

    for (i = 0; i < p->attributes_count; i++) {
        if (strcmp(p->attributes[i].name, name) == 0) {
            return p->attributes[i].data;
        }
    }
    return NULL;
}
