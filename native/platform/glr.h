/* The PC renderer (OpenGL 3.3 core): the game's draw paths hand it their data here instead of
 * building PS2 VU1/GS packets. The software GS frame (still used by 2D paths not ported yet) is
 * shown underneath. */
#ifndef GLR_H
#define GLR_H

#include <stdint.h>

/* a triangle strip of `n` vertices: positions (x, y, z, w floats, 16-byte stride), texture
 * coordinates (s, t floats) and colours (RGBA bytes, 0x80 = 1.0), drawn with `mvp` (column
 * major, world/local to PS2 clip space); `tex0`: the GS TEX0 register of its texture, 0 for
 * none; `prim`: the GS PRIM bits (ABE for blending) */
void glr_strip(const float mvp[16], int n, const float *xyzw, const float *st, const uint8_t *rgba,
               uint64_t tex0, uint32_t prim);

/* a draw path not ported to OpenGL yet: reported once (nothing is drawn) */
void glr_todo(const char *what);

/* the game finished building a frame (renderer flip): it becomes the one shown */
void glr_end_frame(void);

/* platform side (video.c) */
int glr_init(void);                                    /* after the GL context exists */
void glr_present(const uint32_t *gsPixels, int pitch, int w, int h, int outW, int outH);
void glr_read_pixels(uint32_t *out, int w, int h);     /* the last presented frame, top-down RGBA */

#endif
