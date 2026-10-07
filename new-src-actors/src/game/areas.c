/* The room's event areas: see areas.h. */
#include "areas.h"

#include "../data/pac.h"

#include <stdint.h>
#include <string.h>

/* area `area`'s record (NULL: no such area in the section) */
static const uint8_t *area_at(const Room *r, int area, size_t *left) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_EVENTS, &size);
    uint32_t first, off;

    if (sec == NULL || area < 0 || size < 4) {
        return NULL;
    }
    memcpy(&first, sec, 4);
    if ((uint32_t)area >= first || (size_t)(area + 1) * 4 > size) {   /* (the table's size) */
        return NULL;
    }
    memcpy(&off, sec + area * 4, 4);
    if ((size_t)off * 4 + 0x30 > size) {
        return NULL;
    }
    *left = size - off * 4;
    return sec + off * 4;
}

static float f32_at(const uint8_t *p, int off) {
    float v;

    memcpy(&v, p + off, 4);
    return v;
}

static int32_t s32_at(const uint8_t *p, int off) {
    int32_t v;

    memcpy(&v, p + off, 4);
    return v;
}

int area_inside(const Room *r, int area, Vec3 p) {
    size_t left;
    const uint8_t *a = area_at(r, area, &left);
    int inside = 0, k;
    float y;

    if (a == NULL || left < 0x50 || s32_at(a, 0) != 1) {
        return 0;
    }
    for (k = 0; k < 4; k++) {
        float x0 = f32_at(a, 0x10 + k * 0x10), z0 = f32_at(a, 0x18 + k * 0x10);
        float x1 = f32_at(a, 0x10 + ((k + 1) & 3) * 0x10), z1 = f32_at(a, 0x18 + ((k + 1) & 3) * 0x10);

        if ((p.x - x0) * (z1 - z0) - (p.z - z0) * (x1 - x0) <= 0.0f) {
            inside++;
        }
    }
    if (inside != 4) {
        return 0;
    }
    y = 1.0f + p.y;
    return !(y < f32_at(a, 0x24) || !(y <= f32_at(a, 0x8)));
}

int area_middle(const Room *r, int area, Vec3 *out) {
    size_t left;
    const uint8_t *a = area_at(r, area, &left);

    if (a == NULL || left < 0x40) {
        return 0;
    }
    *out = vec3((f32_at(a, 0x10) + f32_at(a, 0x30)) / 2.0f, f32_at(a, 0x14),
                (f32_at(a, 0x18) + f32_at(a, 0x38)) / 2.0f);
    return 1;
}

static int line_side(Vec3 p, Vec3 a, Vec3 b) {
    return (p.x - a.x) * (b.z - a.z) - (p.z - a.z) * (b.x - a.x) < 0.0f ? -1 : 1;
}

int area_cross(const Room *r, int area, Vec3 prev, Vec3 cur) {
    size_t left;
    const uint8_t *e = area_at(r, area, &left);
    Vec3 a, b;
    int n0, n1;

    if (e == NULL) {
        return 0;
    }
    if (s32_at(e, 0) != 0) {
        if (area_inside(r, area, cur)) {
            return area_inside(r, area, prev) ? 0 : 1;
        }
        return area_inside(r, area, prev) ? -1 : 0;
    }
    a = vec3(f32_at(e, 0x10), f32_at(e, 0x14), f32_at(e, 0x18));
    b = vec3(f32_at(e, 0x20), f32_at(e, 0x24), f32_at(e, 0x28));
    n0 = line_side(cur, a, b);
    n1 = line_side(prev, a, b);
    if (n0 + n1 != 0) {
        return 0;   /* both on one side */
    }
    if (line_side(a, cur, prev) + line_side(b, cur, prev) != 0) {
        return 0;   /* the step passes the gate's end */
    }
    cur.y = cur.y + 1.0f;
    prev.y = prev.y + 1.0f;
    if ((!(cur.y < a.y) && cur.y <= f32_at(e, 0x8)) || (!(prev.y < a.y) && prev.y <= f32_at(e, 0x8))) {
        return n0;
    }
    return 0;
}

int area_count(const Room *r) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_EVENTS, &size);
    uint32_t first;

    if (sec == NULL || size < 4) {
        return 0;
    }
    memcpy(&first, sec, 4);
    return (int)(first / 4 < size / 4 ? first / 4 : size / 4);
}

int area_corner(const Room *r, int area, int k, Vec3 *out) {
    size_t left;
    const uint8_t *a = area_at(r, area, &left);

    if (a == NULL || left < 0x50 || k < 0 || k > 3) {
        return 0;
    }
    *out = vec3(f32_at(a, 0x10 + k * 0x10), f32_at(a, 0x14 + k * 0x10), f32_at(a, 0x18 + k * 0x10));
    return 1;
}

int area_kind(const Room *r, int area) {
    size_t left;
    const uint8_t *a = area_at(r, area, &left);

    return a == NULL ? -1 : s32_at(a, 0);
}
