/* Class destructors still left in the PS2 disassembly, matched to the plain shape (vtable
 * stores, members / the class's global pointer cleared, then operator delete when flags > 0)
 * and generated from it (2026-10-05). */
#include "common.h"
#include "globals.h"
#include "memcard.h"
#include "navmesh.h"

extern void *D_0046BA68[];
extern void *D_0046F580[];
extern void *D_00471060[];
extern void *D_004795C0[];
extern void *D_00479870[];
extern void *D_004798B0[];
extern void *D_004799D0[];
extern void *D_00479AC0[];
extern void *D_00479B00[];
extern void *D_0047A2F0[];
extern void *D_0047A3D0[];
extern void *D_0047A3F0[];
extern void *D_0047A410[];
extern void *D_0047A430[];
extern void func_00100490(void *p);
extern void func_002D63B0(void *p);

/* destructor (vtable D_0046BA68) */
void *func_001FB400(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046BA68;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_00471060) */
void *func_00306290(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00471060;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_004795C0) */
void *func_00358C20(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004795C0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_00479870) */
void *func_0035BBD0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479870;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_004798B0) */
void *func_0035CE40(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004798B0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_004799D0) */
void *func_0035D7E0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004799D0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_00479AC0) */
void *func_00360B60(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479AC0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_00479B00) */
void *func_00361940(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479B00;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0047A2F0) */
void *func_00370030(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A2F0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0047A3D0) */
void *func_00377CC0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A3D0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0047A3F0) */
void *func_00377FF0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A3F0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0047A410) */
void *func_00378310(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A410;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0047A430) */
void *func_003784F0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A430;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}
