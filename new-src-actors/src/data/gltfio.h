/* What the library's glTF files (data/roomgltf.h, data/modelgltf.h) share: writing JSON text and
 * the binary buffer by hand, a run of vertices less its repeats (indexed), and reading back with
 * cgltf - an accessor's bytes, an attribute, the "hga" extra's words. */
#ifndef GLTFIO_H
#define GLTFIO_H

#include <stddef.h>
#include <stdint.h>

#include "../third_party/cgltf.h"

typedef struct GltfText {
    char *s;
    size_t n, cap;
} GltfText;
void gltf_add(GltfText *o, const char *fmt, ...);

typedef struct GltfBin {
    uint8_t *b;
    size_t n;
} GltfBin;
size_t gltf_put(GltfBin *b, const void *p, size_t n);   /* where it went (4-byte aligned) */

/* `count` elements of `size` bytes (from `v`) less the repeated ones: the distinct ones onto u
 * (*nu so far; room for count more), each element's index into them (from where this run
 * started in u) into idx */
void gltf_distinct(const void *v, int count, size_t size, void *u, int *nu, uint32_t *idx);

/* the "hga" extra's text after its first word `kind` ("draw ...": "draw"), NULL if it hasn't one */
const char *gltf_hga(const cgltf_extras *e, const char *kind);
/* an accessor's bytes (elements of `elem` bytes, packed), NULL if they aren't all in its buffer */
const uint8_t *gltf_bytes(const cgltf_accessor *a, size_t elem);
const cgltf_accessor *gltf_attribute(const cgltf_primitive *p, const char *name);

#endif
