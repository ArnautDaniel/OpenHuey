/* Sony libc (PS2 0x00117F00..0x0011F970 and neighbours) mapped to the host C library. */
#include <string.h>

void *func_00115D20(void *p, int c, unsigned n) { return memset(p, c, n); }
