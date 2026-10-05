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

/* PC-only bits in glr_strip's prim: additive blending (the GS ALPHA (Cs - 0) * As + Cd), and
 * no depth writes (translucent sprites) */
#define GLR_PRIM_ADD 0x10000u
#define GLR_PRIM_NOZW 0x20000u
/* PC-only bit: also drawn into the glow buffer (the renderer's 128 x 112 work buffer, tested
 * against the scene's depth at that size) */
#define GLR_PRIM_GLOW 0x40000u

/* a draw path not ported to OpenGL yet: reported once (nothing is drawn) */
void glr_todo(const char *what);

/* this frame's fog: from view depth `nearZ` (colour c0) to `farZ` (c1); RGBA, alpha 0x80 = full */
void glr_fog(uint32_t c0, uint32_t c1, float nearZ, float farZ);

/* a full-screen tint over this frame (RGBA, alpha 0x80 = opaque): the screen fades */
void glr_overlay(uint32_t rgba);

/* renderer +0x58 this frame: the glow buffer fades to 3/4 and is added over the screen at half
 * strength, stretched (so blurred); it is kept between frames, a trail */
void glr_glow(void);

/* renderer +0x5C: the glow buffer is cleared to black */
void glr_glow_clear(void);

/* func_002699D0 this frame: the screen bloom - a blurred half-size copy of the screen goes into
 * the glow buffer and over the screen, tinted by `rgba` (0x80 = 1.0) at alpha / 2; added, or
 * subtracted when `subtract` */
void glr_bloom(uint32_t rgba, int subtract);

/* the game finished building a frame (renderer flip): it becomes the one shown */
void glr_end_frame(void);

/* platform side (video.c) */
int glr_init(void);                                    /* after the GL context exists */
void glr_present(const uint32_t *gsPixels, int pitch, int w, int h, int outW, int outH);
void glr_read_pixels(uint32_t *out, int w, int h);     /* the last presented frame, top-down RGBA */

#endif
