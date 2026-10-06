#ifndef LIBC_H
#define LIBC_H

/* Sony's libc (string, memory, printf): native/platform/libc.c on PC. */
#include "common.h"
#include <stdarg.h>

extern void func_001136E8(s32 status);   /* exit() */
extern void *func_00114DA8(s32 align, s32 size);   /* memalign */
extern void *func_00114FA8(u32 size);   /* malloc */
extern void func_00114FD0(void *p);   /* free */
extern void *func_00115B68(void *d, const void *s, u32 n);   /* memcpy */
extern void func_00115D20(void *p, s32 c, u32 n);   /* memset */
extern char *func_001180E8(const char *s, s32 c);   /* strchr */
extern s32 func_00118278(const char *a, const char *b);   /* strcmp */
extern char *func_001183C0(char *d, const char *s);   /* strcpy */
extern char *func_00118978(char *d, const char *s, s32 n);   /* strncpy */
extern s64 func_0011CE88(s64 a, s64 b);   /* __divdi3 */
extern s32 func_0026ED98(char *buf, s32 n, const char *fmt, va_list ap);   /* vsnprintf */
extern s32 func_0026EDD0(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
extern s32 func_0026EE88(const char *fmt, ...);   /* printf */

#endif /* LIBC_H */
