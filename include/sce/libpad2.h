#ifndef SCE_LIBPAD2_H
#define SCE_LIBPAD2_H

/* libpad2 (the DualShock 2): native/platform/pad.c on PC. */
#include "common.h"

extern s32 func_001EF990(s32 mode);   /* libpad2: init */
extern void func_001EF9D0(void);   /* scePad2End */
extern s32 func_001EFA38(s32 port, void *buffer);   /* libpad2: create a socket for port, DMA buffer */
extern void func_001EFB40(s32 socket);   /* scePad2DeleteSocket */
extern s32 func_001EFB98(s32 socket, void *data);   /* libpad2: read */
extern s32 func_001EFC70(s32 socket, void *profile);   /* libpad2: button profile */
extern s32 func_001EFD40(s32 socket);   /* libpad2: state (1 = ready) */
extern s32 func_002D25D8(s32 socket, void *mask);   /* actuators (libdbc): count, mask */
extern void func_002D2658(s32 socket, s32 count, void *mask, s32 bytes, void *data);

#endif /* SCE_LIBPAD2_H */
