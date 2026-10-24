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
/* the frames blended with the last (the original's final packet, Renderer_Additive): the new
 * frame's weight `a` (0..1; 1 the frame as it is, 0 the last one held) - from now on, until
 * post_trail_off (a fresh game) */
void post_trail(float a);
void post_trail_off(void);
/* the panic's screens this frame (ScreenFade_Frame, layer 0x2A; hg's glr_negative / glr_zoom_blur /
 * glr_panic): the negative (0x80 less each channel, as the frame holds them), the frame drawn back
 * 16 pixels bigger each side over itself at half, and the threshold tint - each pixel black whose
 * green (with the frame's alpha bits) is below `limit`, else grey 0xC0, over it at amount / 256 */
void post_negative(void);
void post_zoom_blur(void);
void post_panic_tint(int limit, int amount);
/* a TV's picture (TvScreen): the frame, grey, at half size - made at this frame's end when wanted
 * (each frame); its texture for the next (0 none yet) */
void post_tv_want(void);
/* the screen wavering this frame (the haze of LightShaft / Room49Effect / Room92Effect): its columns
 * moved down by sin(a) (height + (1 + cos a) / 2) - a the phase + 90 degrees a column - and all of it
 * `shift` right (texels of 256 across), blended back at a strength (of 128) from `middle` in the
 * middle to `edge` at the sides */
void post_haze(float phase, float shift, float height, int middle, int edge);
unsigned int post_tv_texture(void);
/* the glow pass (the original's renderer +0x58): the glowing sprites drawn again between begin (0:
 * no target yet) and end, into a buffer kept between frames that fades and is added over the
 * screen on the frames that glow; cleared at the next frame after post_glow_clear (a new room) */
int post_glow_begin(void);
void post_glow_end(void);
void post_glow_clear(void);
/* a screen bloom this frame (Bloom_Start): the bloom mask's areas, blurred, added (or taken away)
 * in this colour - 0x80 = 1, the alpha its strength (of 256) - with the room's own; the glow pass
 * runs too */
void post_bloom(uint32_t rgba, int subtract);
/* the shine of layer $14 this frame (the original's renderer +0x78; the marker's): its bright parts
 * blurred and added in this colour ($RRGGBBAA in bytes r g b a, 0x80 = 1) - drawn again between
 * begin (0: none asked for this frame) and end */
void post_shine(uint32_t rgba);
int post_shine_begin(void);
void post_shine_end(void);
/* the softened layer ($1C, Fiona unseen in her camouflage): characters in it drawn there instead of
 * in the scene - at half size, blended back over it stretched (soft-edged) */
int post_soft_begin(void);
void post_soft_end(void);
/* layer 6, the original's shadow buffer: its draws go to a half-size target against the scene's
 * depth so far (begin: 0 no target yet); at the end it is blurred and taken off the screen at half,
 * on its display values, and the scene's target is current again */
int post_shade_begin(void);
void post_shade_end(void);
void post_finish(const PostCamera *cam, const RenderSettings *s, int x, int y, int w, int h, float time);

#endif
