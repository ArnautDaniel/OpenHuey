/* Loading the character of slot 2 (the stalker in play, or an event character) by kind: its
 * object (0x1800 bytes, 0x1840 for kinds 4 / 12 / 23 / 37) from the scene heap, constructed and
 * registered, then its model (model.c). Kinds 0, 1 and 5 have no character. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "actor.h"
#include "ptmf.h"
#include "char_load.h"
#include "model.h"
#include "pursuer.h"

void *Kind38_ctor(void *p, s32 arg);

typedef struct {
    u32 size;
    void *(*ctor)(void *obj, s32 slot);
    void (*model)(Progress *p, u32 slot);
    u8 withKind;   /* a class shared by several kinds: its constructor also takes the kind */
} CharKind;

static const CharKind sKinds[0x28] = {
    [2] = { 0x1800, Debilitas_ctor, CharLoad_DebilitasModel },   /* Debilitas */
    [3] = { 0x1800, Daniella_ctor, CharLoad_DaniellaModel },   /* Daniella */
    [4] = { 0x1840, Riccardo_ctor, CharLoad_RiccardoModel },   /* Riccardo */
    [6] = { 0x1800, Debilitas2_ctor, CharLoad_DebilitasModel },   /* the second Debilitas class */
    [7] = { 0x1800, Debilitas3_ctor, CharLoad_DebilitasModel },
    [8] = { 0x1800, Kind08_ctor, CharLoad_PlainModel },
    [9] = { 0x1800, Kind09_ctor, CharLoad_Kind09Model },
    [10] = { 0x1800, Lorenzo2_ctor, CharLoad_Lorenzo2Model },   /* the second Lorenzo class */
    [11] = { 0x1800, Lorenzo_ctor, CharLoad_LorenzoModel },   /* Lorenzo */
    [12] = { 0x1840, Kind12_ctor, CharLoad_Kind12Model },
    [13] = { 0x1800, Kind13_ctor, CharLoad_Kind13Model },
    [14] = { 0x1800, (void * (*)(void *, s32))Kind14_ctor, CharLoad_Kind14Model, 1 },
    [15] = { 0x1800, (void * (*)(void *, s32))Kind15_ctor, CharLoad_Kind14Model, 1 },
    [16] = { 0x1800, Kind16_ctor, CharLoad_PlainModel },
    [17] = { 0x1800, Kind17_ctor, CharLoad_PlainModel },
    [18] = { 0x1800, Kind18_ctor, CharLoad_Kind18Model },
    [19] = { 0x1800, Kind19_ctor, CharLoad_PlainModel },
    [20] = { 0x1800, Kind20_ctor, CharLoad_PlainModel },
    [21] = { 0x1800, (void * (*)(void *, s32))Kind21_ctor, CharLoad_PlainModel, 1 },
    [22] = { 0x1800, (void * (*)(void *, s32))Kind22_ctor, CharLoad_PlainModel, 1 },
    [23] = { 0x1840, TintStalker_ctor, CharLoad_Kind23Model },
    [24] = { 0x1800, Kind24_ctor, CharLoad_PlainModel },
    [25] = { 0x1800, Kind25_ctor, CharLoad_PlainModel },
    [26] = { 0x1800, Kind26_ctor, CharLoad_PlainModel },
    [27] = { 0x1800, Kind27_ctor, CharLoad_DebilitasModel },
    [28] = { 0x1800, Kind28_ctor, CharLoad_PlainModel },
    [29] = { 0x1800, Kind29_ctor, CharLoad_PlainModel },
    [30] = { 0x1800, Kind30_ctor, CharLoad_PlainModel },
    [31] = { 0x1800, Kind31_ctor, CharLoad_PlainModel },
    [32] = { 0x1800, Kind32_ctor, CharLoad_PlainModel },
    [33] = { 0x1800, Kind33_ctor, CharLoad_Kind33Model },
    [34] = { 0x1800, Kind34_ctor, CharLoad_DaniellaModel },
    [35] = { 0x1800, Kind35_ctor, CharLoad_DaniellaModel },
    [36] = { 0x1800, Kind36_ctor, CharLoad_DaniellaModel },
    [37] = { 0x1840, Kind37_ctor, CharLoad_Kind23Model },
    [38] = { 0x1800, Kind38_ctor, CharLoad_PlainModel },
    [39] = { 0x1800, Kind39_ctor, CharLoad_Lorenzo2Model },
};

#ifdef HG_NATIVE
/* natively, the kinds whose classes are all in C (the rest stay "not loaded") */
static const u8 sReady[0x28] = {
    [2] = 1, [3] = 1, [4] = 1, [6] = 1, [10] = 1, [11] = 1,   /* the stalkers */
};
extern void hg_skipped(const char *what);   /* native/platform/skip.c */
#endif

extern void *D_00469C20[];
extern void *D_00469C60[];
extern void *D_0046D810[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *Pursuer_ctor(u8 *p, u32 id, s32 arg);

extern void *D_00478FF0[];
static inline void *b0_RoomCtor(void *p, u32 id, s32 arg, void **vtbl) {
    FLD(p, 0x0, void **) = D_00469C20;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = D_00469C60;
    FLD(p, 0x1380, s32) = 0;
    FLD(p, 0x153C, u8) = (u8)id;
    FLD(p, 0x0, void **) = vtbl;
    return p;
}

/* load character kind `id` into slot 2: 1 when it's there; 0 when slot 2 is taken, the kind has
   no character or the heap is full */
/* 0x00171160 */
s32 CharLoad_Partner(Progress *p, u32 id) {   /* (a u8) */
    const CharKind *k;
    VObject *heap;
    void *mem;
    void *obj;

    id &= 0xFF;
    if (id >= 0x28 || sKinds[id].ctor == NULL) {
        return 0;
    }
#ifdef HG_NATIVE
    if (!sReady[id]) {
        hg_skipped("CharLoad_Partner: a character kind whose class is not in C yet");
        return 0;
    }
#endif
    k = &sKinds[id];
    if (gCharacters[2] != NULL) {
        return 0;
    }
    heap = (VObject *)((u8 *)p + 0x6FBF00);
    mem = VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, k->size);
    if (mem == NULL) {
        return 0;
    }
    obj = func_00124E50((void *)k->size, mem);
    if (obj != NULL) {
        if (k->withKind) {
            obj = ((void *(*)(void *, s32, s32))k->ctor)(obj, 2, id);
        } else {
            obj = k->ctor(obj, 2);
        }
    }
    Characters_Register(p, 2, obj);
    k->model(p, 2);
    CharLoad_Buffers(p, 2);
    return 1;
}

/* 0x001727C0 */
void *Kind38_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x26, arg, D_00478FF0);
}

/* the character in `slot` gets its data buffers: their sizes (vtable +0xFC, seven) as one
 * block from the scene heap (+0x166C; +0x1668 set), split in order to +0x1670, +0x1678,
 * +0x1674, (none for the fourth, though its size is counted), +0x167C, +0x1680, +0x1684; an
 * empty one gets no pointer (0) */
/* 0x0016D180 */
void CharLoad_Buffers(Progress *p, s32 slot) {
    static const u16 sAt[7] = { 0x1670, 0x1678, 0x1674, 0, 0x167C, 0x1680, 0x1684 };
    u8 *c = (u8 *)gCharacters[slot];
    const u32 *size = VCALL(c, 0xFC, const u32 *(*)(void *))(c);
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u32 total = 0;
    u8 *at;
    s32 i;

    for (i = 0; i < 7; i++) {
        total += size[i];
    }
    AT(c, 0x166C, u8 *) = VCALL(heap, 0x10, u8 *(*)(VObject *, u32))(heap, total);
    AT(c, 0x1668, u8) = 1;
    for (i = 0; i < 7; i++) {
        if (sAt[i] != 0) {
            AT(c, sAt[i], u8 *) = NULL;
        }
    }
    AT(c, 0x1688, u8 *) = NULL;
    at = AT(c, 0x166C, u8 *);
    for (i = 0; i < 7; i++) {
        if (sAt[i] != 0 && size[i] != 0) {
            AT(c, sAt[i], u8 *) = at;
            at += size[i];
        }
    }
}

s32 Characters_Register(void *self, u32 kind, void *obj) {
    if (kind < 6 && gCharacters[kind] == NULL) {
        gCharacters[kind] = obj;
        FLD(gCharacters[kind], 0x20, u32) = kind;
        switch (kind) {
        case 0:
            gCharPlayer = obj;
            break;
        case 1:
            gCharPartner = obj;
            break;
        case 2:
            gCharPursuer = obj;
            break;
        }
        return 1;
    }
    return 0;
}

/* load character kind `id` as an event character into `slot` (3..5, free): 0x17C0 bytes from
   the scene heap, its kind's model; 1 when it's there, 0 when not (no character for that kind:
   none either) */
/* 0x0016D6D0 */
s32 CharLoad_EventChar(Progress *p, u32 id, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    const CharKind *k;
    void *mem;
    u8 *obj;

    id &= 0xFF;
    slot &= 0xFF;
    if (id >= 0x28 || sKinds[id].ctor == NULL) {
        return 0;
    }
    k = &sKinds[id];
    if (slot < 3 || slot >= 6 || gCharacters[slot] != NULL) {
        return 0;
    }
    mem = VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x17C0);
    if (mem == NULL) {
        return 0;
    }
    obj = func_00124E50((void *)0x17C0, mem);
    if (obj != NULL) {
        obj = Pursuer_ctor(obj, id, slot);
    }
    Characters_Register(p, slot, obj);
    k->model(p, slot);
    AT(obj, 0x1668, u8) = 0;
    return 1;
}

/* Room object constructor: base 0x469C20 -> 0x469C60 -> 0x46D810; id at +0x153C. */
/* 0x00171090 */
void *Pursuer_ctor(u8 *p, u32 id, s32 arg) {
    FLD(p, 0x0, void **) = D_00469C20;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = D_00469C60;
    FLD(p, 0x1380, s32) = 0;
    p[0x153C] = (u8)id;
    FLD(p, 0x0, void **) = D_0046D810;
    return p;
}
