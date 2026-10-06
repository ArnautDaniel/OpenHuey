/* libpad2 (PS2 0x001EFA38..0x001F0250) on PC: input will come from SDL. */

/* scePad2Init */
int scePad2Init(int mode) { (void)mode; return 1; }

/* scePad2CreateSocket(port, dma buffer): socket id */
int scePad2CreateSocket(int port, void *buffer) { (void)buffer; return port; }

#include <string.h>

/* scePad2GetState: 1 = ready */
int scePad2GetState(int socket) { (void)socket; return 1; }

/* scePad2 button profile: every button and analog byte present */
int scePad2GetButtonProfile(int socket, void *profile) {
    unsigned char *p = profile;

    (void)socket;
    memset(p, 0, 0x100);
    p[0] = p[1] = p[2] = p[3] = 0xFF;
    return 1;
}

/* scePad2Read: DualShock 2 data from the keyboard / an SDL gamepad (input.c) */
extern void hg_input_read(unsigned char *data);

int scePad2Read(int socket, void *data) {
    (void)socket;
    memset(data, 0, 0x100);
    hg_input_read(data);
    return 1;
}

/* libdbc actuator wrappers (PS2 0x002D25D8 / 0x002D2658): no motors yet */
int Dbc_ActuatorInfo(int socket, void *mask) { (void)socket; (void)mask; return 0; }
void func_002D2658(int socket, int count, void *mask, int bytes, void *data) {
    (void)socket; (void)count; (void)mask; (void)bytes; (void)data;
}


/* scePad2DeleteSocket / scePad2End: nothing to release on the PC */
int scePad2DeleteSocket(int socket) { (void)socket; return 1; }
int scePad2End(void) { return 1; }
