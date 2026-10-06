#ifndef RICCARDO_H
#define RICCARDO_H

/* riccardo.c: what other files call. */
#include "common.h"

typedef struct Character Character;
typedef struct Pursuer Pursuer;

/* riccardo.c */
extern s32 func_002D7A60(void);
extern Pursuer *func_002D7A70(Pursuer *p, s32 flags);
extern void func_002D7CE0(Pursuer *p);
extern void func_002D7D20(Pursuer *p, s32 exit);
extern void func_002D7E10(Pursuer *p, s32 exit);
extern void func_002D8120(Pursuer *p, s32 *e, f32 *a, f32 *b);
extern void func_002D8210(Pursuer *p, s8 situation);
extern void func_002DB7F0(Pursuer *p);
extern void func_002DB900(Pursuer *p, s32 on);
extern s32 func_002DB950(Pursuer *p);
extern s32 func_002DB960(Pursuer *p);
extern s32 func_002DB990(Pursuer *p);
extern s32 func_002DB9E0(Pursuer *p);
extern void func_002DC070(Pursuer *p);
extern void func_002DC4E0(Pursuer *p);
extern void func_002D85D0(Pursuer *p);
extern void func_002D8690(Pursuer *p);
extern void func_002D8AC0(Pursuer *p);
extern void func_002D8CB0(Pursuer *p);
extern void func_002DA4C0(Pursuer *p);
extern void func_002D7E20(Pursuer *p, Character *who);
extern s32 func_002D8840(Pursuer *p, Character *who);
extern void func_002DA120(Pursuer *p);
extern void func_002DB480(Pursuer *p);
extern s32 func_002DBA90(Pursuer *p, f32 *out);
extern void func_002DBD70(Pursuer *p);
extern void func_002D8DF0(Pursuer *p);
extern void func_002D9500(Pursuer *p);
extern void func_002DA6B0(Pursuer *p);

#endif /* RICCARDO_H */
