/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"



void func_002C9620(u8 *self) {
    self[0x205] = 1;
}

/* Entries of 12 bytes at +0x6C, current index at +0x64. */
s32 func_002C9630(u8 *self) {
    s32 i = *(s32 *)(self + 0x64);

    return *(s32 *)(self + 0x6C + i * 12) == 3;
}

/* Returns an s8. */
s32 func_002C9660(u8 *self, s32 i) {
    return (s8)(((s8 *)self)[0x206 + i] - 1);
}

/* Returns an s16. */
s32 func_002C9680(u8 *self, s32 i) {
    return *(s16 *)(self + 0x216 + i * 2);
}

void func_002C96F0(u8 *self, s32 i) {
    self[0x82 + i * 12] = 1;
}

void func_002C9710(u8 *self, s32 i) {
    self[0x83 + i * 12] = 1;
}

/* Resets the slots whose bits are set in the mask record `idx` (12-byte records at self->0x18):
 * bits of +0x24 select one of 32 12-byte slots at +0x80 (slot 0 is self+0x20 itself),
 * bits of bytes +0x28/+0x29 clear the words at +0x24/+0x44. */
void func_002CBFF0(u8 *self, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        u8 *slot = self + 0x80 + i * 12;

        if (slot[0] == 0 && i != 0) {
            continue;
        }
        if ((*(u32 *)(*(u8 **)(self + 0x18) + idx * 12 + 0x24) & (1 << i)) && (u32)i < 26) {
            if (i == 0) {
                *(u32 *)(self + 0x20) = 0;
            } else {
                u8 *obj = *(u8 **)(slot + 8);

                *(u32 *)(*(u8 **)(obj + 0xF0) + 0x4C8) = 0;
                (*(u8 **)(slot + 8))[0xE0] = 0;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if ((*(u8 **)(self + 0x18))[idx * 12 + 0x28] & (1 << i)) {
            *(u32 *)(self + 0x24 + i * 4) = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        if ((*(u8 **)(self + 0x18))[idx * 12 + 0x29] & (1 << i)) {
            *(u32 *)(self + 0x44 + i * 4) = 0;
        }
    }
}

static const s8 b3_CC5A0_map[26] = {
    -1, 0, 1, 2, 3, 4, 9, 10, 11, 12, 8, 13, 18, 14, 15, 16, 17, 19, 23, 24, 25, 29, 30, 31, 32, 38,
};

/* Maps an id (0..25) to another id; -1 when out of range. */
s32 func_002CC5A0(void *self, u32 id) {
    if (id >= 26) {
        return -1;
    }
    return b3_CC5A0_map[id];
}

void func_002CC830(u8 *self, u32 a, u32 b) {
    *(u32 *)(self + 0x18) = a;
    *(u32 *)(self + 0x1C) = b;
}
