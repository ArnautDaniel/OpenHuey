/* Class destructors still left in the PS2 disassembly, matched to the plain shape (vtable
 * stores, members / the class's global pointer cleared, then operator delete when flags > 0)
 * and generated from it (2026-10-05). */
#include "common.h"

extern void *D_0044E4B8;
extern void *D_0044E4C8;
extern void *D_0044E4E8;
extern void *D_0044E4F0;
extern void *D_0044E4F8;
extern void *D_0044E560;
extern void *D_0044E570;
extern void *D_0044E7A8;
extern void *D_0044E978;
extern void *D_0044E980;
extern void *D_0044E9A0;
extern void *D_0044F7F8;
extern void *D_0044FE10;
extern void *D_0044FEB0;
extern void *D_0044FEF8;
extern void *D_0044FF00;
extern void *D_00456DE8;
extern void *D_004699E0[];
extern void *D_00469B40[];
extern void *D_00469D00[];
extern void *D_0046A0D0[];
extern void *D_0046AA40[];
extern void *D_0046AC00[];
extern void *D_0046AC50[];
extern void *D_0046ACF0[];
extern void *D_0046AD88[];
extern void *D_0046ADD0[];
extern void *D_0046AE10[];
extern void *D_0046AE30[];
extern void *D_0046AE60[];
extern void *D_0046AEC0[];
extern void *D_0046AED0[];
extern void *D_0046AF20[];
extern void *D_0046AF90[];
extern void *D_0046B050[];
extern void *D_0046B1D0[];
extern void *D_0046B1F0[];
extern void *D_0046B350[];
extern void *D_0046BA68[];
extern void *D_0046BB20[];
extern void *D_0046BEE0[];
extern void *D_0046C6F0[];
extern void *D_0046C770[];
extern void *D_0046C780[];
extern void *D_0046D730[];
extern void *D_0046D750[];
extern void *D_0046D770[];
extern void *D_0046D780[];
extern void *D_0046D790[];
extern void *D_0046D7A0[];
extern void *D_0046D7B0[];
extern void *D_0046D7D0[];
extern void *D_0046EB40[];
extern void *D_0046EB60[];
extern void *D_0046EC60[];
extern void *D_0046EC80[];
extern void *D_0046ED30[];
extern void *D_0046F3D0[];
extern void *D_0046F580[];
extern void *D_00470F90[];
extern void *D_00471060[];
extern void *D_00472F60[];
extern void *D_00476B50[];
extern void *D_00476F40[];
extern void *D_00478B70[];
extern void *D_004795C0[];
extern void *D_00479600[];
extern void *D_00479870[];
extern void *D_004798B0[];
extern void *D_004799D0[];
extern void *D_00479AC0[];
extern void *D_00479B00[];
extern void *D_00479FF0[];
extern void *D_0047A2F0[];
extern void *D_0047A3D0[];
extern void *D_0047A3F0[];
extern void *D_0047A410[];
extern void *D_0047A430[];
extern void *D_0047A6F0[];
extern void *gBootMessage;
extern void *gSceneGameF29740;
extern void func_00100490(void *p);
extern void func_002672E0(void *p);
extern void func_002D63B0(void *p);

/* destructor (vtable D_004699E0) */
void *func_00120EF0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004699E0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_00469B40) */
void *func_00122AD0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00469B40;
        D_0044E4B8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AA40) */
void *func_0017CDD0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AA40;
        D_0044E570 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AC00) */
void *func_001AABD0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AC00;
        gSceneGameF29740 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AC50) */
void *func_001AAE10(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AC50;
        AT(o, 0x0, void **) = D_0046ACF0;
        D_0044E4F0 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046ACF0) */
void *func_001BC090(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ACF0;
        D_0044E4F0 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AD88) */
void *func_001BC320(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AD88;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046ADD0) */
void *func_001BE730(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ADD0;
        D_0044FEB0 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_001BECA0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x4, s32) = 0;
        AT(o, 0x0, s32) = 0;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x14, s32) = 0;
        AT(o, 0x10, u8) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AE10) */
void *func_001BF220(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE10;
        D_0044F7F8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AE30) */
void *func_001BF280(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE30;
        D_0044E7A8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AE60) */
void *func_001BF2E0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE60;
        D_0044FF00 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AEC0) */
void *func_001BF5F0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AEC0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        AT(o, 0xC, s32) = 0;
        D_0044E980 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AED0) */
void *func_001BF660(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AED0;
        AT(o, 0x4, u8) = 0;
        D_0044FEF8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AF20) */
void *func_001BF7A0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF20;
        D_0044E9A0 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AF90) */
void *func_001BF800(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF90;
        D_0044E560 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046B050) */
void *func_001BF880(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046B050;
        AT(o, 0x0, void **) = D_0046AF20;
        D_0044E9A0 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046B1D0) */
void *func_001F4590(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046B1D0;
        AT(o, 0x0, void **) = D_0046B1F0;
        D_0044E4E8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046B350) */
void *func_001FB0F0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046B350;
        D_0044E4C8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

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

/* destructor (vtable ?) */
void *func_0020D8D0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, s32) = 0;
        AT(o, 0x4, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_0020D920(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x44, s32) = 0;
        AT(o, 0x48, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_0020D970(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, s32) = 0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_0020D9C0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x10, s32) = 0;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046BEE0) */
void *func_0020DB40(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046BEE0;
        D_0044E978 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046C6F0) */
void *func_00225620(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C6F0;
        D_0044E4F8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046C770) */
void *func_0025C850(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C770;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046C780) */
void *func_0025E950(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C780;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D750) */
void *func_00267310(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D750;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D790) */
void *func_00267480(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D790;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D770) */
void *func_00267500(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D770;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D780) */
void *func_00268110(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D780;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D7A0) */
void *func_00269970(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D7A0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D7B0) */
void *func_0026B1E0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D7B0;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046EB40) */
void *func_002BAFD0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EB40;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046EB60) */
void *func_002BB220(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EB60;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046EC60) */
void *func_002C64E0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EC60;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046EC80) */
void *func_002C65C0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EC80;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D7D0) */
void *func_002D00A0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D7D0;
        AT(o, 0x0, void **) = D_0046A0D0;
        gBootMessage = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046F3D0) */
void *func_002D0C10(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046F3D0;
        D_00456DE8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046ED30) */
void *func_002D0C70(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ED30;
        AT(o, 0x0, void **) = D_0046BB20;
        D_0044FE10 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_00470F90) */
void *func_00301250(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00470F90;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
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

/* destructor (vtable D_00476B50) */
void *func_003156A0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476B50;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_00472F60) */
void *func_00316D80(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00472F60;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* destructor (vtable D_00476F40) */
void *func_0033D990(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476F40;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_00478B70) */
void *func_00347640(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00478B70;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
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

/* destructor (vtable D_00479600) */
void *func_0035A230(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479600;
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

/* destructor (vtable D_00479FF0) */
void *func_0036A4D0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479FF0;
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

/* destructor (vtable D_0047A6F0) */
void *func_0037B9A0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A6F0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}
