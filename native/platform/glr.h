/* The PC renderer (OpenGL 3.3 core): the game's draw paths hand it their data here instead of
 * building PS2 VU1/GS packets. The software GS frame (still used by 2D paths not ported yet) is
 * shown underneath. */
#ifndef GLR_H
#define GLR_H

#include <stdint.h>

/* a triangle strip of `n` vertices: positions (x, y, z floats and a GS flags word: bit 15 set,
 * the triangle ending there is skipped; 16-byte stride), texture coordinates (s, t floats) and
 * colours (RGBA bytes, 0x80 = 1.0), drawn with `mvp` (column major, world/local to PS2 clip
 * space); `tex`: its texture's entry in a loaded .TEX file (NULL: none), `tex0` the GS TEX0 the
 * game made for it (its TCC / TFX bits are used); `prim`: the GS PRIM bits (TME, ABE) */
void glr_strip(const float mvp[16], int n, const float *xyzw, const float *st, const uint8_t *rgba,
               const void *tex, uint64_t tex0, uint32_t prim);

/* a draw path not ported to OpenGL yet: reported once (nothing is drawn) */
void glr_todo(const char *what);

/* a full-screen tint over this frame (RGBA, alpha 0x80 = opaque): the screen fades */
void glr_overlay(uint32_t rgba);

/* the game finished building a frame (renderer flip): it becomes the one shown */
void glr_end_frame(void);

/* platform side (video.c) */
int glr_init(void);                                    /* after the GL context exists */
void glr_present(const uint32_t *gsPixels, int pitch, int w, int h, int outW, int outH);
void glr_read_pixels(uint32_t *out, int w, int h);     /* the last presented frame, top-down RGBA */

#endif
