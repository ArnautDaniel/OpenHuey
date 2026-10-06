/* Small vector and matrix maths. Matrices are 4x4, column-major (m[col*4 + row]) as OpenGL
 * takes them - the same layout the PS2's VU0 used, so the game's matrices load as they are. */
#ifndef MATHX_H
#define MATHX_H

#include <math.h>

typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Mat4 {
    float m[16];
} Mat4;

static inline Vec3 vec3(float x, float y, float z) { return (Vec3){x, y, z}; }
static inline Vec3 vec3_add(Vec3 a, Vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline Vec3 vec3_sub(Vec3 a, Vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline Vec3 vec3_scale(Vec3 a, float s) { return vec3(a.x * s, a.y * s, a.z * s); }
static inline float vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float vec3_len(Vec3 a) { return sqrtf(vec3_dot(a, a)); }
static inline Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
static inline Vec3 vec3_norm(Vec3 a) {
    float l = vec3_len(a);
    return l > 0.0f ? vec3_scale(a, 1.0f / l) : a;
}
static inline Vec3 vec3_min(Vec3 a, Vec3 b) { return vec3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z)); }
static inline Vec3 vec3_max(Vec3 a, Vec3 b) { return vec3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)); }

static inline Mat4 mat4_identity(void) {
    Mat4 r = {{0}};
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
    return r;
}

static inline Mat4 mat4_mul(Mat4 a, Mat4 b) {   /* a * b: b applies first */
    Mat4 r;
    int c, k;

    for (c = 0; c < 4; c++) {
        for (k = 0; k < 4; k++) {
            r.m[c * 4 + k] = a.m[0 * 4 + k] * b.m[c * 4 + 0] + a.m[1 * 4 + k] * b.m[c * 4 + 1] +
                             a.m[2 * 4 + k] * b.m[c * 4 + 2] + a.m[3 * 4 + k] * b.m[c * 4 + 3];
        }
    }
    return r;
}

static inline Vec3 mat4_point(const Mat4 *a, Vec3 p) {   /* a * (p, 1) */
    return vec3(a->m[0] * p.x + a->m[4] * p.y + a->m[8] * p.z + a->m[12],
                a->m[1] * p.x + a->m[5] * p.y + a->m[9] * p.z + a->m[13],
                a->m[2] * p.x + a->m[6] * p.y + a->m[10] * p.z + a->m[14]);
}

/* perspective projection: vertical field of view in radians, aspect w/h */
static inline Mat4 mat4_perspective(float fovy, float aspect, float znear, float zfar) {
    Mat4 r = {{0}};
    float t = 1.0f / tanf(fovy * 0.5f);

    r.m[0] = t / aspect;
    r.m[5] = t;
    r.m[10] = (zfar + znear) / (znear - zfar);
    r.m[11] = -1.0f;
    r.m[14] = 2.0f * zfar * znear / (znear - zfar);
    return r;
}

/* a view matrix looking from eye along forward (unit), with up roughly up */
static inline Mat4 mat4_look(Vec3 eye, Vec3 forward, Vec3 up) {
    Vec3 s = vec3_norm(vec3_cross(forward, up)), u = vec3_cross(s, forward);
    Mat4 r = mat4_identity();

    r.m[0] = s.x;  r.m[4] = s.y;  r.m[8] = s.z;
    r.m[1] = u.x;  r.m[5] = u.y;  r.m[9] = u.z;
    r.m[2] = -forward.x;  r.m[6] = -forward.y;  r.m[10] = -forward.z;
    r.m[12] = -vec3_dot(s, eye);
    r.m[13] = -vec3_dot(u, eye);
    r.m[14] = vec3_dot(forward, eye);
    return r;
}

/* screen-space projection for 2D: pixels, (0, 0) at the top left */
static inline Mat4 mat4_ortho2d(float w, float h) {
    Mat4 r = mat4_identity();

    r.m[0] = 2.0f / w;
    r.m[5] = -2.0f / h;
    r.m[12] = -1.0f;
    r.m[13] = 1.0f;
    return r;
}

#endif
