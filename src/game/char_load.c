/* Loading the character of slot 2 (the stalker in play, or an event character) by kind: its
 * object (0x1800 bytes, 0x1840 for kinds 4 / 12 / 23 / 37) from the scene heap, constructed and
 * registered, then its model (model.c). Kinds 0, 1 and 5 have no character. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "actor.h"
#include "ptmf.h"

void *func_001727C0(void *p, s32 arg);

extern void *func_00124E50(u32 size, void *mem);   /* placement new */
extern void func_0016D180(Progress *p, s32 slot);

extern void *func_00172910(void *obj, s32 slot);
extern void *func_00172960(void *obj, s32 slot);
extern void *func_001729B0(void *obj, s32 slot);
extern void *func_00172A00(void *obj, s32 slot);
extern void *func_00172A50(void *obj, s32 slot);
extern void *func_00172AA0(void *obj, s32 slot);
extern void *func_00172AF0(void *obj, s32 slot);
extern void *func_00172B40(void *obj, s32 slot);
extern void *func_00172B90(void *obj, s32 slot);
extern void *func_00172BE0(void *obj, s32 slot);
extern void *func_00172C30(void *obj, s32 slot);
extern void *func_00172C80(void *obj, s32 slot);
extern void *func_00172DE0(void *obj, s32 slot);
extern void *func_00172E20(void *obj, s32 slot);
extern void *func_00172E70(void *obj, s32 slot);
extern void *func_00172EC0(void *obj, s32 slot);
extern void *func_00172F10(void *obj, s32 slot);
extern void *func_00172F60(void *obj, s32 slot);
extern void *func_00172FB0(void *obj, s32 slot);
extern void *func_00173110(void *obj, s32 slot);
extern void *func_00173150(void *obj, s32 slot);
extern void *func_001731A0(void *obj, s32 slot);
extern void *func_001731F0(void *obj, s32 slot);
extern void *func_00173240(void *obj, s32 slot);
extern void *func_00173290(void *obj, s32 slot);
extern void *func_001732E0(void *obj, s32 slot);
extern void *func_00173330(void *obj, s32 slot);
extern void *func_00173380(void *obj, s32 slot);
extern void *func_001733D0(void *obj, s32 slot);
extern void *func_00173420(void *obj, s32 slot);
extern void *func_00173470(void *obj, s32 slot);
extern void *func_001734C0(void *obj, s32 slot);
extern void *func_00173510(void *obj, s32 slot);
extern void *func_00173560(void *obj, s32 slot);
extern void *func_001735B0(void *obj, s32 slot);
extern void *func_00173600(void *obj, s32 slot);
extern void func_0016F420(Progress *p, u32 slot);
extern void func_0016F860(Progress *p, u32 slot);
extern void func_0016FEC0(Progress *p, u32 slot);
extern void func_00170480(Progress *p, u32 slot);
extern void func_00170510(Progress *p, u32 slot);
extern void func_00170710(Progress *p, u32 slot);
extern void func_001707A0(Progress *p, u32 slot);
extern void func_00170830(Progress *p, u32 slot);
extern void func_00170910(Progress *p, u32 slot);
extern void func_00170A50(Progress *p, u32 slot);
extern void func_00170B60(Progress *p, u32 slot);
extern void func_00170D30(Progress *p, u32 slot);
extern void func_00170FB0(Progress *p, u32 slot);

typedef struct {
    u32 size;
    void *(*ctor)(void *obj, s32 slot);
    void (*model)(Progress *p, u32 slot);
    u8 withKind;   /* a class shared by several kinds: its constructor also takes the kind */
} CharKind;

static const CharKind sKinds[0x28] = {
    [2] = { 0x1800, func_00173600, func_00170FB0 },   /* Debilitas */
    [3] = { 0x1800, func_001734C0, func_00170D30 },   /* Daniella */
    [4] = { 0x1840, func_00173380, func_00170B60 },   /* Riccardo */
    [6] = { 0x1800, func_001735B0, func_00170FB0 },   /* the second Debilitas class */
    [7] = { 0x1800, func_00173560, func_00170FB0 },
    [8] = { 0x1800, func_001731A0, func_00170710 },
    [9] = { 0x1800, func_00173330, func_00170A50 },
    [10] = { 0x1800, func_001732E0, func_00170910 },   /* the second Lorenzo class */
    [11] = { 0x1800, func_00173240, func_00170830 },   /* Lorenzo */
    [12] = { 0x1840, func_001731F0, func_001707A0 },
    [13] = { 0x1800, func_00173150, func_00170510 },
    [14] = { 0x1800, func_00173110, func_00170480, 1 },
    [15] = { 0x1800, func_00172FB0, func_00170480, 1 },
    [16] = { 0x1800, func_00172F60, func_00170710 },
    [17] = { 0x1800, func_00172F10, func_00170710 },
    [18] = { 0x1800, func_00172EC0, func_0016FEC0 },
    [19] = { 0x1800, func_00172E70, func_00170710 },
    [20] = { 0x1800, func_00172E20, func_00170710 },
    [21] = { 0x1800, func_00172DE0, func_00170710, 1 },
    [22] = { 0x1800, func_00172C80, func_00170710, 1 },
    [23] = { 0x1840, func_00172C30, func_0016F860 },
    [24] = { 0x1800, func_00172B90, func_00170710 },
    [25] = { 0x1800, func_00172B40, func_00170710 },
    [26] = { 0x1800, func_00172AF0, func_00170710 },
    [27] = { 0x1800, func_00173510, func_00170FB0 },
    [28] = { 0x1800, func_00172AA0, func_00170710 },
    [29] = { 0x1800, func_00172A50, func_00170710 },
    [30] = { 0x1800, func_00172A00, func_00170710 },
    [31] = { 0x1800, func_001729B0, func_00170710 },
    [32] = { 0x1800, func_00172960, func_00170710 },
    [33] = { 0x1800, func_00172910, func_0016F420 },
    [34] = { 0x1800, func_00173470, func_00170D30 },
    [35] = { 0x1800, func_00173420, func_00170D30 },
    [36] = { 0x1800, func_001733D0, func_00170D30 },
    [37] = { 0x1840, func_00172BE0, func_0016F860 },
    [38] = { 0x1800, func_001727C0, func_00170710 },
    [39] = { 0x1800, func_00173290, func_00170910 },
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

s32 Characters_Register(void *self, u32 kind, void *obj);
void *func_00171090(u8 *p, u32 id, s32 arg);

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
u8 func_00171160(Progress *p, u32 id) {
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
        hg_skipped("func_00171160: a character kind whose class is not in C yet");
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
    obj = func_00124E50(k->size, mem);
    if (obj != NULL) {
        if (k->withKind) {
            obj = ((void *(*)(void *, s32, s32))k->ctor)(obj, 2, id);
        } else {
            obj = k->ctor(obj, 2);
        }
    }
    Characters_Register(p, 2, obj);
    k->model(p, 2);
    func_0016D180(p, 2);
    return 1;
}

void *func_001727C0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x26, arg, D_00478FF0);
}

/* the character in `slot` gets its data buffers: their sizes (vtable +0xFC, seven) as one
 * block from the scene heap (+0x166C; +0x1668 set), split in order to +0x1670, +0x1678,
 * +0x1674, (none for the fourth, though its size is counted), +0x167C, +0x1680, +0x1684; an
 * empty one gets no pointer (0) */
void func_0016D180(Progress *p, s32 slot) {
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
s32 func_0016D6D0(Progress *p, u32 id, u32 slot) {
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
    obj = func_00124E50(0x17C0, mem);
    if (obj != NULL) {
        obj = func_00171090(obj, id, slot);
    }
    Characters_Register(p, slot, obj);
    k->model(p, slot);
    AT(obj, 0x1668, u8) = 0;
    return 1;
}

/* Room object constructor: base 0x469C20 -> 0x469C60 -> 0x46D810; id at +0x153C. */
void *func_00171090(u8 *p, u32 id, s32 arg) {
    FLD(p, 0x0, void **) = D_00469C20;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = D_00469C60;
    FLD(p, 0x1380, s32) = 0;
    p[0x153C] = (u8)id;
    FLD(p, 0x0, void **) = D_0046D810;
    return p;
}
