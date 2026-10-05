#ifndef COMMON_H
#define COMMON_H

/* Fixed-width types used throughout the decomp (portable: also valid on a 64-bit host
 * as long as game structures don't store raw pointers -- see README "Porting notes"). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

/* a field at a byte offset, for structures not (fully) declared yet */
#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

/* the EE's uncached view of RAM (address | 0x20000000); on PC a buffer is just itself */
#ifdef HG_NATIVE
#define UNCACHED_BIT 0
#else
#define UNCACHED_BIT 0x20000000
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

/* HG_ROOMLOG=1 (PC only): room changes and placements traced to stderr */
#ifdef HG_NATIVE
int hg_roomlog_on(void);
void hg_roomlog(const char *fmt, ...);
#define ROOMLOG(...) do { if (hg_roomlog_on()) hg_roomlog(__VA_ARGS__); } while (0)
#else
#define ROOMLOG(...) do { } while (0)
#endif

/* the EE's sqrt.s: the root of |x|, always the instruction (GCC may otherwise call sqrtf in a
 * block it thinks cold) */
static inline float ee_sqrtf(float x) {
#ifdef HG_NATIVE
    return __builtin_sqrtf(__builtin_fabsf(x));
#else
    float r;

    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
#endif
}

#endif /* COMMON_H */
