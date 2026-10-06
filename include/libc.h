#ifndef LIBC_H
#define LIBC_H

/* Sony's libc (string, memory, printf): native/platform/libc.c on PC. */
#include "common.h"
#include <stdarg.h>

extern void msl_exit(s32 status);   /* exit() */
extern void *msl_memalign(s32 align, s32 size);   /* memalign */
extern void *msl_malloc(u32 size);   /* malloc */
extern void msl_free(void *p);   /* free */
extern void *msl_memcpy(void *d, const void *s, u32 n);   /* memcpy */
extern void msl_memset(void *p, s32 c, u32 n);   /* memset */
extern char *msl_strchr(const char *s, s32 c);   /* strchr */
extern s32 msl_strcmp(const char *a, const char *b);   /* strcmp */
extern char *msl_strcpy(char *d, const char *s);   /* strcpy */
extern char *msl_strncpy(char *d, const char *s, s32 n);   /* strncpy */
extern s64 sf_divdi3(s64 a, s64 b);   /* __divdi3 */
extern s32 msl_vsnprintf(char *buf, s32 n, const char *fmt, va_list ap);   /* vsnprintf */
extern s32 msl_snprintf(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
extern s32 msl_printf(const char *fmt, ...);   /* printf */

#endif /* LIBC_H */
