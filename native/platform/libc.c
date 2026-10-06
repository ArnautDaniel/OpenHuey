/* Sony libc (PS2 0x00117F00..0x0011F970 and neighbours) mapped to the host C library. */
#include <string.h>

void *msl_memset(void *p, int c, unsigned n) { return memset(p, c, n); }
long long sf_divdi3(long long a, long long b) { return a / b; }   /* __divdi3 */
void *msl_memcpy(void *d, const void *s, unsigned n) { return memcpy(d, s, n); }   /* memcpy */

#include <stdarg.h>
#include <stdio.h>

int msl_strcmp(const char *a, const char *b) { return strcmp(a, b); }               /* strcmp */
char *msl_strcpy(char *d, const char *s) { return strcpy(d, s); }                   /* strcpy */
char *msl_strncpy(char *d, const char *s, unsigned n) { return strncpy(d, s, n); }   /* strncpy */
char *msl_strchr(const char *s, int c) { return strchr(s, c); }                      /* strchr */
char *func_00117FB8(char *d, const char *s) { return strcat(d, s); }                   /* strcat */

/* vsnprintf */
int msl_vsnprintf(char *buf, int size, const char *fmt, va_list ap) {
    return vsnprintf(buf, size, fmt, ap);
}

/* snprintf */
int msl_snprintf(char *buf, int size, const char *fmt, ...) {
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = vsnprintf(buf, size, fmt, ap);
    va_end(ap);
    return n;
}

#include <stdlib.h>

/* memalign / free (Sony libc) */
void *msl_memalign(unsigned align, unsigned size) {
    void *p = NULL;

    if (align < sizeof(void *)) {
        align = sizeof(void *);
    }
    return posix_memalign(&p, align, size ? size : 1) == 0 ? p : NULL;
}

void msl_free(void *p) { free(p); }

/* malloc (Sony libc: _malloc_r on the global reent) */
void *msl_malloc(unsigned size) { return malloc(size ? size : 1); }

/* exit (Sony libc) */
void msl_exit(int status) {
    exit(status);
}

/* the game's debug printf (MSL printf through its own console writer): shown with HG_GAMELOG */
int msl_printf(const char *fmt, ...) {
    static int on = -1;
    va_list ap;
    int n = 0;

    if (on < 0) {
        on = getenv("HG_GAMELOG") != NULL;
    }
    if (on) {
        va_start(ap, fmt);
        n = vfprintf(stderr, fmt, ap);
        va_end(ap);
        fputc('\n', stderr);
    }
    return n;
}
