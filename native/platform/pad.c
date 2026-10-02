/* libpad2 (PS2 0x001EFA38..0x001F0250) on PC: input will come from SDL. */

/* scePad2Init */
int func_001EF990(int mode) { (void)mode; return 1; }

/* scePad2CreateSocket(port, dma buffer): socket id */
int func_001EFA38(int port, void *buffer) { (void)buffer; return port; }

#include <string.h>

/* scePad2GetState: 1 = ready */
int func_001EFD40(int socket) { (void)socket; return 1; }

/* scePad2 button profile / read: no input yet (SDL to come) */
int func_001EFC70(int socket, void *profile) { (void)socket; memset(profile, 0, 0x100); return 1; }
int func_001EFB98(int socket, void *data) { (void)socket; memset(data, 0, 0x100); return 1; }

/* libdbc actuator wrappers (PS2 0x002D25D8 / 0x002D2658): no motors yet */
int func_002D25D8(int socket, void *mask) { (void)socket; (void)mask; return 0; }
void func_002D2658(int socket, int count, void *mask, int bytes, void *data) {
    (void)socket; (void)count; (void)mask; (void)bytes; (void)data;
}

/* TEMPORARY stand-in for the game's input state builder (PS2 0x002D4780, 620 instructions; to be
 * decompiled next): the input state stays all zero, i.e. no buttons. */
void func_002D4780(void *pads) { (void)pads; }
