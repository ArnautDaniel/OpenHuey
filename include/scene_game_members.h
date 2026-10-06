#ifndef SCENE_GAME_MEMBERS_H
#define SCENE_GAME_MEMBERS_H

/* scene_game_members.c: what other files call. */
#include "common.h"

/* scene_game_members.c */
extern void *func_002D15B0(void *p);
extern void func_002212D0(u8 *p);
extern void func_002A8410(u8 *p);   /* reset a state block */
extern void func_002ECB50(u8 *p);
extern void *func_002D10C0(u8 *p);
extern void *func_002D1160(u8 *p);
extern void *func_002D1200(u8 *p);
extern void func_002A7B40(u8 *p);
extern void func_002A8060(u8 *p);   /* a fresh save's progress */
extern void func_00120980(u8 *m);
extern void *func_002D13B0(u8 *p);
extern void func_00120C80(u8 *m);
extern void func_00176780(u8 *prog);
extern void func_001F40F0(u8 *p, s32 a, s32 b, s32 c);
extern void func_002A84C0(u8 *e);
extern void func_002A8500(u8 *e);
extern void func_001F9D90(u8 *o);
extern void func_002E2890(u8 *o, const void *unused);
extern void func_00120720(u8 *rm, u32 room, s32 slot);
extern void func_001AABC0(u8 *o);
extern void func_00267250(u8 *o);
extern void func_002672E0(void *p);   /* delete (effects' heap) */
extern void func_002D6330(u8 *o);
extern void func_002D63B0(void *p);   /* free from the scene heap? */
extern s32 func_00120660(u8 *rm, s32 slot);   /* room slot still loading */
extern void func_002E2820(u8 *o);
extern void func_0011FFB0(u8 *rm, s32 slot);
extern void func_0025F810(u8 *o);
extern void func_0025F9D0(u8 *o, s32 id);
extern void *func_002672F0(u32 size, void *place);   /* placement new */
extern void func_002D6100(u8 *o);
extern void func_002D6170(u8 *o, s32 slot);
extern void func_00305520(u8 *m, s32 room);
extern void func_00303E60(u8 *m, s8 page);
extern void func_00304F50(u8 *m);   /* the map, each frame */
extern void func_00305380(u8 *m);
extern void func_002A7C70(const u8 *s, u8 *d);   /* copies saved flags into Progress */
extern void func_002A76E0(u8 *p);   /* four bytes cleared */
extern void *func_002A8520(void *o, s32 flags);
extern void func_002670F0(u8 *fx, s32 n);   /* effect slot n gone */
extern s32 func_00266C70(u8 *o, s32 n, void *arg);
extern void *func_002D63C0(u32 size, void *place);   /* placement new */
extern s32 func_002D6090(u8 *mgr, s32 slot, void *params);
extern void func_002E2650(u8 *o);
extern void func_002A7720(u8 *o);
extern void func_002EC470(u8 *o, u8 kind);
extern void func_002EC450(u8 *o, s32 sec);
extern void func_002EC3C0(u8 *o, s32 sec);
extern void func_002EC940(u8 *o);
extern void func_002A84A0(u8 *r);
extern void func_002A84E0(u8 *r);
extern void func_0011FB20(u8 *rm, s32 slot);
extern void func_002E2A60(u8 *o);
extern void func_0011FF30(u8 *rm);
extern void func_0011FEB0(u8 *rm);
extern void func_002A7630(u8 *t);
extern void func_002D75C0(u8 *o);
extern void func_002671F0(u8 *o);
extern void func_002D6280(u8 *mgr);
extern void *func_00266C40(void *effects, s32 k);   /* effect slot n */
extern void func_002E29A0(u8 *o);
extern void func_002D74E0(u8 *o);
extern void func_00267160(u8 *o);
extern void func_002D61E0(u8 *mgr);
extern void func_002EF480(u8 *fade, f32 t);   /* the screen fade's level, 0..1 */
extern void func_002F0340(u8 *fade, s32 mode);
extern void func_002A8440(u8 *n, s32 loud, s32 room, s32 tri, s32 door);   /* make a noise */
extern void func_002EC4F0(u8 *o, u8 *n);
extern s32 func_00120540(u8 *o, s32 k);
extern void func_001F4100(u8 *o);   /* shut down Game.unk14E8C90 */
extern void func_001F9D20(u8 *o);
extern void *func_0020E820(void *o, s32 flags);
extern void func_00267140(void);
extern void func_001205A0(u8 *o, s32 k);
extern void func_0017D1B0(u8 *o);
extern void func_001AAC60(u8 *o);
extern void *func_001BF6C0(u8 *o, s32 flags);
extern s32 func_002D6020(u8 *o, s32 slot);   /* a slot's effect state (3: ended) */

#endif /* SCENE_GAME_MEMBERS_H */
