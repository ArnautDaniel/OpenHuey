#ifndef INPUT_H
#define INPUT_H

/* The controller state the game reads (written each frame by the pad code, src/game/pad.c). */
#include "common.h"

extern u32 gPadHeld;   /* pad buttons held */
extern u32 gPadPressed;   /* pad buttons pressed this frame */
extern u32 gMenuPressed;   /* menu buttons pressed this frame (MENU_*) */

/* pad buttons (PS2 order) */
#define PAD_SELECT   0x0001
#define PAD_L3       0x0002
#define PAD_R3       0x0004
#define PAD_START    0x0008
#define PAD_UP       0x0010
#define PAD_RIGHT    0x0020
#define PAD_DOWN     0x0040
#define PAD_LEFT     0x0080
#define PAD_L2       0x0100
#define PAD_R2       0x0200
#define PAD_L1       0x0400
#define PAD_R1       0x0800
#define PAD_TRIANGLE 0x1000
#define PAD_CIRCLE   0x2000
#define PAD_CROSS    0x4000
#define PAD_SQUARE   0x8000

/* menu buttons: the pad mapped by the controller layout */
#define MENU_UP      0x001
#define MENU_RIGHT   0x002
#define MENU_DOWN    0x004
#define MENU_LEFT    0x008
#define MENU_CONFIRM 0x010
#define MENU_CANCEL  0x020
#define MENU_PREV    0x040   /* the in-game menu's previous / next page */
#define MENU_NEXT    0x080
#define MENU_DEFAULT 0x200   /* the options' "restore defaults" */

#endif /* INPUT_H */
