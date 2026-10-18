/* The view: where the eye is and where it looks. Scripts move it (camera fields in Forth). */
#ifndef CAMERA_H
#define CAMERA_H

#include "../core/mathx.h"

typedef struct Camera {
    Vec3 pos;
    float yaw;     /* radians: 0 looks along -z, positive turns right */
    float pitch;   /* radians: positive looks up */
    float fov;     /* vertical field of view, radians (the picture gets wider, not shorter, on a wide
                    * screen) */
    float znear, zfar;
    float up;      /* +1, or -1 for a world whose y points down */
    float roll;    /* radians, a cutscene's: the original's Camera +0x30 - its up (0, -1, 0) on the
                    * PS2's downward screen turned about the view by it, through a transposed
                    * matrix (Camera_ViewMatrix); ours, +1 on an upward one, turned by -roll */
} Camera;

static inline Vec3 camera_forward(const Camera *c) {
    return vec3(sinf(c->yaw) * cosf(c->pitch), sinf(c->pitch) * c->up, -cosf(c->yaw) * cosf(c->pitch));
}

static inline Mat4 camera_proj(const Camera *c, float aspect) {
    return mat4_perspective(c->fov, aspect, c->znear, c->zfar);
}

static inline Mat4 camera_view(const Camera *c) {
    Vec3 f = camera_forward(c), up = vec3(0.0f, c->up, 0.0f);

    if (c->roll != 0.0f) {   /* (Rodrigues: about the unit view direction) */
        float co = cosf(-c->roll), si = sinf(-c->roll);

        up = vec3_add(vec3_add(vec3_scale(up, co), vec3_scale(vec3_cross(f, up), si)),
                      vec3_scale(f, vec3_dot(f, up) * (1.0f - co)));
    }
    return mat4_look(c->pos, f, up);
}

#endif
