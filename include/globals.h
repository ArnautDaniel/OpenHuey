#ifndef GLOBALS_H
#define GLOBALS_H

/* The engine's singletons: objects the game creates once (mostly as members of the game
 * scene) and reaches through these pointers. Their classes are called through their vtables
 * (VCALL); the address of each pointer in the original is noted. */
#include "common.h"
#include "game.h"

extern VObject *gRandom;        /* 0x0044E550 the random number generator: +0x10 an integer, +0x18 / +0x1C a float 0..1 */
extern VObject *gFileLoader;    /* 0x0044E4E0 file loading (CRI file system) */
extern VObject *gBootMessage;   /* 0x0044E998 the message object (texts, and the characters' texture sets) */
extern VObject *gRenderer;      /* 0x0044E4F0 */
extern VObject *gTexCache;      /* 0x0044E4E8 the texture cache */
extern VObject *gVram;          /* 0x0044E9A0 the VRAM manager */
extern VObject *gSound;         /* 0x0044E560 the sound driver */
extern VObject *gCamera;        /* 0x0044E4B8 */
extern VObject *gCamDirector;   /* 0x0044E4F8 the camera director */
extern VObject *gCutscene;      /* 0x0044FE10 the cutscene director */
extern VObject *gRumble;        /* 0x0044E7A8 controller rumble (Rumble, rumble.c) */
extern VObject *gLights;        /* 0x0044E4C8 the scene's lights */
extern VObject *gEvents;        /* 0x0044E4D0 the event system (scripts and their variables) */
extern VObject *gRooms;         /* 0x0044E568 the room map and the doors between rooms */
extern VObject *gDoors;         /* 0x0044E558 */
extern VObject *gObstacles;     /* 0x0044FE08 */
extern VObject *gPlacedThings;  /* 0x0044F260 */
extern VObject *gSubScreen;     /* 0x0044E988 the sub screen (menus; its item list at +8) */
extern VObject *gRoutePlanner;  /* 0x0044E580 */
extern VObject *gPad;           /* 0x0044FEB0 the pad manager */
extern VObject *gSystem;        /* 0x0044F7F8 the system object */
extern VObject *gMusic;         /* 0x0044E970 the music controller (BgmCtl, bgm.h) */

/* structures reached by offset (AT) */
extern u8 *gEffects;            /* 0x0044E578 the effect manager (effectmgr.h) */
extern u8 *gRoomEffects;        /* 0x0044E4C0 */
extern u8 *gCreatures;          /* 0x0044F258 the room creatures (10 slots) */
extern u8 *gGamePtr;          /* 0x0044E978 the Game object (gGame): its vtable gives the resident buffers; +0x4 / +0x10 the next scene and its start, +0x20 the system data (options, unlocks, the clear record) */
extern void *gSceneTable;       /* 0x0044E960 the scene table: scenes at +4, the scene heap at +0x10D9040 */
extern void *gSceneTitle;       /* 0x0044E968 the title scene */
extern void *gSubPool;          /* 0x0044E990 the sub screen's entry pool */
extern void *gMovie;            /* 0x0044E958 the movie playing (Movie, movie.c) */
extern void *gMovieLib;         /* 0x0044FEF8 the CRI movie library's state (MovieLib, movie.c) */
extern void *gAdx;              /* 0x0044E980 the ADX streaming sound system (Bgm, bgm.c) */

#endif /* GLOBALS_H */
