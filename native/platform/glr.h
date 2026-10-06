/* The PC renderer (OpenGL 3.3 core): the game's draw paths, 3D and 2D, hand it their data here
 * instead of building PS2 VU1/GS packets; it draws the whole frame. */
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
/* PC-only bit: no depth test (2D: drawn over whatever is there, in layer order) */
#define GLR_PRIM_NOZT 0x800000u
/* PC-only bit: with GLR_PRIM_FIX or _ADD, subtracted instead: Cd - Cs * FIX / 128 or Cd - Cs * As
 * (GS ALPHA (0 - Cs) * FIX + Cd, (0 - Cs) * As + Cd) */
#define GLR_PRIM_SUB 0x8000u
/* PC-only bit: with GLR_PRIM_FIX, a mix instead: (Cs - Cd) * FIX / 128 + Cd (GS ALPHA 0x64) */
#define GLR_PRIM_LERP 0x4000u

/* 2D primitives in renderer layer `layer` (-1: the one being drawn), in the game's 512 x 448 screen pixels at the GS's
 * pixel centres (as its XYZ2 / 16): `n` vertices `xy` as a triangle strip (GLR_2D_STRIP) or fan
 * (GLR_2D_FAN), texture coordinates `st` (0..1 over the texture; ignored untextured), colours
 * `rgba` (n x RGBA, 0x80 = 1.0); `tex` a .TEX entry (NULL: untextured) with palette `csa`,
 * modulated, its alpha used. `prim`: 0x40 blended ((Cs - Cd) * As + Cd) plus GLR_PRIM_ bits
 * (ADD, FIX, SUB). No depth test or writes. */
enum { GLR_2D_STRIP, GLR_2D_FAN };
void glr_prim2d(int layer, int kind, int n, const float *xy, const float *st, const uint8_t *rgba, const void *tex,
                int csa, uint32_t prim);

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
/* room 0x61's haze over the screen (func_00374E50) */
void glr_haze(float phase, float sway);
void glr_haze_fix(float phase, float sway, int fix);
void glr_haze2(float phase, float size);
void glr_marker(float x, float y, float z, float scale, float jitter, int fix);
void glr_negative(void);
void glr_zoom_blur(void);
void glr_panic(int limit, int amount);

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

/* an image the game sends to VRAM at word address `addr` (renderer +0x40; RGBA, alpha 0x80 =
 * 1; copied), and its being shown over the whole screen by its alpha in renderer layer `layer` */
void glr_vram_upload(uint32_t addr, const void *rgba, int w, int h);
void glr_vram_draw(uint32_t addr, int layer);
void glr_vram_blit(int layer);   /* the last one sent straight into the screen (0x88000) */

/* layer 0x23 is drawn this frame with the room's two-colour effect (effect 0x1F: colour `add`
 * added, `contrast` pushed from, blurred or not): they and the fog run again on what it draws */
void glr_late_layer(uint32_t add, uint32_t contrast, int blur);

/* layer 0x14 is drawn this frame: the bright parts of what it draws glow in colour `rgba` */
void glr_shine_layer(uint32_t rgba);

/* layer 0x1C is drawn this frame: what it draws is softened (blended back at 256 x 256) */
void glr_soft_layer(void);

/* the game finished building a frame (renderer flip): it becomes the one shown */
void glr_end_frame(void);

/* the render scale: the scene drawn at 640 x 448 times `scale` (1..4; 0 fits the window) */
void glr_set_scale(int scale);
int glr_scale(void);

/* the PC options menu this frame: `n` lines, line `sel` picked (drawn over the window) */
void glr_menu(const char *const *lines, int n, int sel);

/* platform side (video.c) */
int glr_init(void);                                    /* after the GL context exists */
void glr_present(int outW, int outH);                  /* the frame drawn, shown at outW x outH */
void glr_read_pixels(uint32_t *out, int w, int h);     /* the last presented frame, top-down RGBA */

#endif
