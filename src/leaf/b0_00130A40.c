/* Leaf functions (batch 0), 0x00130A40 - 0x00171090. */
#include "common.h"
#include "ptmf.h"
#include "actor.h"

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

/* Memory block list: +0x4 base, +0x8 size, +0xC block table (16 bytes each), +0x10 count. */
typedef struct B0_Block {
    s32 used;
    s32 index;
    u32 addr;
    u32 size;
} B0_Block;

typedef struct B0_Heap {
    void **vtbl;
    u32 base;
    u32 size;
    B0_Block *blocks;
    u32 count;
} B0_Heap;

/* Tail call of the sub-object's virtual +0x14 with the value at +0x73EDC0. */
void func_0016CD30(u8 *p, s32 a1, s32 a2, s32 a3) {
    void *obj = p + 0x6FBF00;

    VCALL(obj, 0x14, void (*)(void *, s32, s32, s32))(obj, FLD(p, 0x73EDC0, s32), a2, a3);
}
