#ifndef SCRIPT_H
#define SCRIPT_H

/* script.c: what other files call. */
#include "common.h"

/* script.c */
extern void Script_Start(u8 *s, u8 *script);   /* start a script */
extern void Script_RunControl(u8 *s);   /* a control op (0xF0..) */

#endif /* SCRIPT_H */
