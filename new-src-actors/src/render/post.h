/* The render targets and the passes after the scene (internal to the renderer). */
#ifndef POST_H
#define POST_H

#include "../core/mathx.h"
#include "render.h"

typedef struct PostCamera {
    Mat4 proj;
    float znear, zfar;
} PostCamera;

void post_init(void);
/* make the scene target current (sized w x h, `msaa` samples) and clear it */
void post_begin(int w, int h, int msaa, Vec3 clear_linear);
/* resolve the scene, run the effects and draw the result into the window's rectangle */
void post_finish(const PostCamera *cam, const RenderSettings *s, int x, int y, int w, int h, float time);

#endif
