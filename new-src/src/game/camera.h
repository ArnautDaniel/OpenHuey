/* The view: where the eye is and where it looks. Scripts move it (camera fields in Forth). */
#ifndef CAMERA_H
#define CAMERA_H

#include "../core/mathx.h"

typedef struct Camera {
    Vec3 pos;
    float yaw;     /* radians: 0 looks along -z, positive turns right */
    float pitch;   /* radians: positive looks up */
    float fov;     /* vertical field of view, radians */
    float znear, zfar;
    float up;      /* +1, or -1 for a world whose y points down */
} Camera;

static inline Vec3 camera_forward(const Camera *c) {
    return vec3(sinf(c->yaw) * cosf(c->pitch), sinf(c->pitch) * c->up, -cosf(c->yaw) * cosf(c->pitch));
}

static inline Mat4 camera_view_proj(const Camera *c, float aspect) {
    Mat4 proj = mat4_perspective(c->fov, aspect, c->znear, c->zfar);
    Mat4 view = mat4_look(c->pos, camera_forward(c), vec3(0.0f, c->up, 0.0f));

    return mat4_mul(proj, view);
}

#endif
