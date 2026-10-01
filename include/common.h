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

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif /* COMMON_H */
