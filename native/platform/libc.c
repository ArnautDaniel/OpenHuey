/* Sony libc (PS2 0x00117F00..0x0011F970 and neighbours) mapped to the host C library. */
#include <string.h>

void *func_00115D20(void *p, int c, unsigned n) { return memset(p, c, n); }

#include <stdarg.h>
#include <stdio.h>

int func_00118278(const char *a, const char *b) { return strcmp(a, b); }               /* strcmp */
char *func_001183C0(char *d, const char *s) { return strcpy(d, s); }                   /* strcpy */
char *func_00118978(char *d, const char *s, unsigned n) { return strncpy(d, s, n); }   /* strncpy */
char *func_001180E8(const char *s, int c) { return strchr(s, c); }                      /* strchr */
char *func_00117FB8(char *d, const char *s) { return strcat(d, s); }                   /* strcat */

/* vsnprintf */
int func_0026ED98(char *buf, int size, const char *fmt, va_list ap) {
    return vsnprintf(buf, size, fmt, ap);
}

/* snprintf */
int func_0026EDD0(char *buf, int size, const char *fmt, ...) {
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = vsnprintf(buf, size, fmt, ap);
    va_end(ap);
    return n;
}

#include <stdlib.h>

/* memalign / free (Sony libc) */
void *func_00114DA8(unsigned align, unsigned size) {
    void *p = NULL;

    if (align < sizeof(void *)) {
        align = sizeof(void *);
    }
    return posix_memalign(&p, align, size ? size : 1) == 0 ? p : NULL;
}

void func_00114FD0(void *p) { free(p); }

/* malloc (Sony libc: _malloc_r on the global reent) */
void *func_00114FA8(unsigned size) { return malloc(size ? size : 1); }
