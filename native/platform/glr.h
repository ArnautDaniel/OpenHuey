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
/* PC-only bits: blended Cs * FIX / 128 + Cd, FIX in bits 24..31 (GS ALPHA (Cs - 0) * FIX + Cd) */
#define GLR_PRIM_FIXB 0x80000u
#define GLR_PRIM_FIX(f) (GLR_PRIM_FIXB | (uint32_t)(f) << 24)
/* PC-only bit: a quad marking the reflection's mask where it is in front of the scene */
#define GLR_PRIM_MASK 0x100000u
/* PC-only bits: a shadow volume quad counting in layer 6's stencil, +1 with _INC else -1 */
#define GLR_PRIM_STENCIL 0x200000u
#define GLR_PRIM_STENCIL_INC 0x400000u

/* the renderer layer (0..52, drawn in order) what follows is sent in; -1: none */
void glr_layer(int layer);

/* a draw path not ported to OpenGL yet: reported once (nothing is drawn) */
void glr_todo(const char *what);

/* the fog, in the current layer: from view depth `nearZ` (colour c0) to `farZ` (c1), blended
 * over what is drawn by then; RGBA, alpha 0x80 = full */
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

/* func_002685F0 this frame: the screen halved and brightened (`blur`: 8 offset copies at 1/4
 * added, else 8 in place), stretched and tinted by `rgba` at alpha / 2 - added, or with
 * `contrast` the screen pushed away from it (D + (D - it)) */
void glr_screen2(uint32_t rgba, int contrast, int blur);

/* func_0021C840 this frame (renderer layer 0x2A): the screen's corners darkened, by up to
 * strength / 128, fading to nothing at the middles of the edges (moved by `offset` pixels) */
void glr_vignette(int strength, int offset);

/* func_0034E9E0 (the light caustic): the frame's alpha cleared before its grids, then the
 * halved screen where they left alpha >= `aref` blurred and added back at 1/2 */
void glr_caustic_begin(void);
void glr_caustic_glow(int aref);

/* func_002C6650 (the depth of field): the screen blurred where its view depth is outside
 * from .. to, fully beyond a / b, in steps of a quarter between */
void glr_dof(float a, float from, float to, float b);

/* the reflecting floor (func_00317D40): layer 0x17's draws go into the reflection (from the
 * mirrored camera's half-size matrices; its background the screen mirrored, renderer +0x8C
 * `flip`); glr_mask_clear and GLR_PRIM_MASK quads mark where it shows; glr_refl blends it over
 * the screen at fix / 128 - `prep` prepares it first (blurred, sharp where drawn), `flip`
 * mirrors top / bottom rather than left / right, `masked` only within the marks, moved `dx`
 * pixels */
void glr_refl_flip(int flip);
void glr_mask_clear(void);
void glr_refl(int prep, int fix, int flip, int masked, float dx);

/* the shadows (layer 6; src/game/shadow.c): glr_shadow_begin restarts the count (0x7F),
 * glr_shadow_quad counts a volume quad (clip space, strip order), glr_shadow_fill writes the
 * colour (RGBA, alpha 0x80 = 1) where the count ended above 0x7F within the box (GS pixels
 * around 2048, half size); glr_shadow_cancel drops this shadow's entries (its draw failed).
 * Layer 6 is drawn at half size from the camera's half matrices, then blurred and taken off the
 * screen. */
void glr_shadow_begin(void);
void glr_shadow_quad(const float *xyz, int inc);
void glr_shadow_fill(float x0, float y0, float x1, float y1, uint32_t rgba);
void glr_shadow_cancel(void);

/* a fading layer (0x0F, 0x1A or 0x23) is drawn this frame with the renderer's tint `tint`
 * (+0x304D54): what it draws fades back to the screen behind it (alpha up to 0x80 = shown) or
 * turns towards the tint's colour (above) */
void glr_tint_layer(int layer, uint32_t tint);

/* the game finished building a frame (renderer flip): it becomes the one shown */
void glr_end_frame(void);

/* the render scale: the scene drawn at 640 x 448 times `scale` (1..4; 0 fits the window) */
void glr_set_scale(int scale);
int glr_scale(void);

/* the PC options menu this frame: `n` lines, line `sel` picked (drawn over the window) */
void glr_menu(const char *const *lines, int n, int sel);

/* platform side (video.c) */
int glr_init(void);                                    /* after the GL context exists */
void glr_present(const uint32_t *gsPixels, int pitch, int w, int h, int outW, int outH);
void glr_read_pixels(uint32_t *out, int w, int h);     /* the last presented frame, top-down RGBA */

#endif
