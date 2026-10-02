/* IOP side of the PS2 (I/O processor: modules for pads, memory cards, sound, disc). The PC has
 * none: the game's IOP plumbing (game code, PS2 0x001BC0F0..) is replaced here as a whole. */
#include <stdio.h>

/* reset the IOP and set up module loading */
void func_001BC220(void *iop) { (void)iop; }

/* load IOP module `name` (SIO2MAN.IRX, LIBSD.IRX...): nothing to load; report a module id */
int func_001BC0F0(void *iop, const char *name, int a, int b, int c) {
    (void)iop;
    (void)a;
    (void)b;
    (void)c;
    fprintf(stderr, "iop: module %s (skipped)\n", name);
    return 1;
}
