/* The rooms' event handler classes (the 4-byte objects of the events object, +0x120: one per
 * room, see sRooms in event.c; base D_0046DB80): their destructors and small table getters.
 * Each room's class gives the event system the room's data tables (by slot, see the vtables). */
#include "common.h"

extern void func_00100490(void *p);   /* operator delete */

extern void *D_0046B4B0[], *D_0046B4F0[], *D_0046B530[], *D_0046B570[], *D_0046B5B0[], *D_0046B5F0[];
extern void *D_0046B630[], *D_0046B670[], *D_0046B6B0[], *D_0046B6F0[], *D_0046B730[], *D_0046B770[];
extern void *D_0046B7B0[], *D_0046B7F0[], *D_0046B830[], *D_0046B870[], *D_0046B8B0[], *D_0046DB80[];
extern void *D_0046DBC0[], *D_0046DC00[], *D_0046DC40[], *D_0046DC80[], *D_0046DCC0[], *D_0046DD00[];
extern void *D_0046DD40[], *D_0046DD80[], *D_0046DDC0[], *D_0046DE00[], *D_0046DE40[], *D_0046DE80[];
extern void *D_0046DEC0[], *D_0046DF00[], *D_0046DF40[], *D_0046DF80[], *D_0046DFC0[], *D_0046E000[];
extern void *D_0046E040[], *D_0046E080[], *D_0046E0C0[], *D_0046E100[], *D_0046E140[], *D_0046E180[];
extern void *D_0046E1C0[], *D_0046E200[], *D_0046E240[], *D_0046E280[], *D_0046E2C0[], *D_0046E300[];
extern void *D_0046E340[], *D_0046E380[], *D_0046E3C0[], *D_0046E400[], *D_0046E440[], *D_0046E480[];
extern void *D_0046E4C0[], *D_0046E500[], *D_0046E540[], *D_0046E580[], *D_0046E5C0[], *D_0046E600[];
extern void *D_0046E640[], *D_0046E680[], *D_0046E6C0[], *D_0046E700[], *D_0046E740[], *D_0046E780[];
extern void *D_0046E7C0[], *D_0046E800[], *D_0046E840[], *D_0046E880[], *D_0046E8C0[], *D_0046E900[];
extern void *D_0046E940[], *D_0046E980[], *D_0046E9C0[], *D_0046EA00[], *D_0046EDC0[], *D_0046EE00[];
extern void *D_0046F3F0[], *D_0046FC40[], *D_0046FC80[], *D_0046FCC0[], *D_0046FD00[], *D_0046FD40[];
extern void *D_0046FD80[], *D_0046FDC0[], *D_0046FE00[], *D_0046FE40[], *D_0046FE80[], *D_0046FEC0[];
extern void *D_00470DC0[], *D_00470EB0[], *D_00470EF0[], *D_00470F50[], *D_00471020[], *D_00471210[];
extern void *D_00471250[], *D_00471E60[], *D_00471EA0[], *D_00471EE0[], *D_00471F20[], *D_00471F60[];
extern void *D_00471FA0[], *D_00471FE0[], *D_00473460[], *D_00473C90[], *D_00474520[], *D_00474F40[];
extern void *D_004760E0[], *D_004771C0[], *D_00477200[], *D_00477240[], *D_00477280[], *D_004772C0[];
extern void *D_00477300[], *D_00477340[], *D_00477380[], *D_004773C0[], *D_00477400[], *D_00477440[];
extern void *D_00477480[], *D_004774C0[], *D_00477500[], *D_00477540[], *D_00477580[], *D_00477610[];
extern void *D_00477650[], *D_00477690[], *D_004776D0[], *D_00477710[], *D_00477750[], *D_00478570[];
extern void *D_004785B0[], *D_004785F0[], *D_00478630[], *D_00478670[], *D_004786B0[], *D_004786F0[];
extern void *D_00478730[], *D_00478AA0[], *D_00478B80[], *D_00478C00[], *D_00478C40[], *D_00479340[];
extern void *D_00479380[], *D_004793C0[], *D_00479620[], *D_004798D0[], *D_00479910[], *D_00479950[];
extern void *D_00479990[], *D_00479FB0[], *D_0047A070[], *D_0047A0B0[], *D_0047A0F0[], *D_0047A130[];
extern void *D_0047A170[], *D_0047A1B0[], *D_0047A1F0[], *D_0047A230[], *D_0047A270[], *D_0047A2B0[];
extern void *D_0047A450[], *D_0047A490[], *D_0047A4D0[], *D_0047A510[], *D_0047A550[], *D_0047A590[];
extern void *D_0047A5D0[], *D_0047A610[], *D_0047A650[], *D_0047A690[];
extern u8 D_0047A9C8[], D_0047A9D0[], D_0047A9DC[], D_0047A9E0[], D_0047A9E8[], D_0047A9F0[];
extern u8 D_0047A9F8[], D_0047AA40[], D_0047AA80[], D_0047AB10[], D_0047AB3C[], D_0047AB50[];
extern u8 D_0047AB58[], D_0047AB70[], D_0047AB98[], D_0047ABC0[], D_0047ABD0[], D_0047ABE0[];
extern u8 D_0047AC10[], D_0047AC40[], D_0047AC60[], D_0047AC68[], D_0047AC78[], D_0047AC80[];
extern u8 D_0047ACB0[], D_0047ACB8[], D_0047ACC0[], D_0047ACC4[], D_0047ACC8[], D_0047ACE0[];
extern u8 D_0047ADF8[], D_0047AE18[], D_0047AE30[], D_0047AE38[], D_0047AE50[], D_0047AE74[];
extern u8 D_0047AE7C[], D_0047AE84[], D_0047AE98[], D_0047AEBC[], D_0047AF04[], D_0047AF08[];
extern u8 D_0047AF0C[], D_0047AF1C[], D_0047AF20[], D_0047AF28[], D_0047AF30[], D_0047AF40[];
extern u8 D_0047AF58[], D_0047AF5C[], D_0047AF70[], D_0047AF78[], D_0047AF88[], D_0047AF90[];
extern u8 D_0047AF98[], D_0047AFA0[], D_0047AFA8[], D_0047AFB0[], D_0047AFB8[], D_0047AFC0[];
extern u8 D_0047AFD0[], D_0047AFD8[], D_0047AFE0[], D_0047AFE8[], D_0047AFF0[], D_0047B028[];
extern u8 D_0047B030[], D_0047B040[], D_0047B048[], D_0047B050[], D_0047B054[], D_0047B058[];
extern u8 D_0047B060[], D_0047B068[], D_0047B06C[], D_0047B070[], D_0047B078[], D_0047B07C[];
extern u8 D_0047B080[], D_0047B088[], D_0047B090[], D_0047B098[], D_0047B0A0[], D_0047B0A8[];
extern u8 D_0047B0B0[], D_0047B0C0[], D_0047B0C8[], D_0047B0D0[], D_0047B0D4[], D_0047B0DC[];
extern u8 D_0047B0E0[], D_0047B0E4[], D_0047B0E8[], D_0047B0F0[], D_0047B0F4[], D_0047B0F8[];
extern u8 D_0047B100[], D_0047B108[], D_0047B110[], D_0047B118[], D_0047B11C[], D_0047B120[];
extern u8 D_0047B128[], D_0047B130[], D_0047B134[];

/* the base (D_0046DB80): destructor */
void *func_001FB3B0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046DB80;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* a room's class: its vtable, then the base's */
static inline void *room_dtor(void *o, s32 flags, void **own, void **base) {
    if (o != NULL) {
        AT(o, 0x0, void **) = own;
        if (o != NULL) {
            AT(o, 0x0, void **) = base;
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the base's defaults: nothing (0) */
s32 func_00209200(void *o) {   /* +0x30 */
    return 0;
}

s32 func_002097E0(void *o) {   /* +0x20 */
    return 0;
}

s32 func_002097F0(void *o) {   /* +0x1C */
    return 0;
}

s32 func_00209820(void *o) {   /* +0x10 */
    return 0;
}

s32 func_00209830(void *o) {   /* +0xC */
    return 0;
}

s32 func_0020BCE0(void *o) {   /* +0x38 */
    return 0;
}

s32 func_002A8930(void *o) {   /* +0x34 */
    return 0;
}

s32 func_002A8940(void *o) {   /* +0x2C */
    return 0;
}

s32 func_002A8950(void *o) {   /* +0x28 */
    return 0;
}

s32 func_002A8960(void *o) {   /* +0x24 */
    return 0;
}

/* destructors */
void *func_0020B090(void *o, s32 flags) { return room_dtor(o, flags, D_0046B4B0, D_0046DB80); }
void *func_0020B130(void *o, s32 flags) { return room_dtor(o, flags, D_0046B4F0, D_0046DB80); }
void *func_0020B1D0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B530, D_0046DB80); }
void *func_0020B2A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B570, D_0046DB80); }
void *func_0020B380(void *o, s32 flags) { return room_dtor(o, flags, D_0046B5B0, D_0046DB80); }
void *func_0020B420(void *o, s32 flags) { return room_dtor(o, flags, D_0046B5F0, D_0046DB80); }
void *func_0020B4C0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B630, D_0046DB80); }
void *func_0020B5A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B670, D_0046DB80); }
void *func_0020B670(void *o, s32 flags) { return room_dtor(o, flags, D_0046B6B0, D_0046DB80); }
void *func_0020B710(void *o, s32 flags) { return room_dtor(o, flags, D_0046B6F0, D_0046DB80); }
void *func_0020B7B0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B730, D_0046DB80); }
void *func_0020B8A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B770, D_0046DB80); }
void *func_0020B970(void *o, s32 flags) { return room_dtor(o, flags, D_0046B7B0, D_0046DB80); }
void *func_0020BA60(void *o, s32 flags) { return room_dtor(o, flags, D_0046B7F0, D_0046DB80); }
void *func_0020BB40(void *o, s32 flags) { return room_dtor(o, flags, D_0046B830, D_0046DB80); }
void *func_0020BC10(void *o, s32 flags) { return room_dtor(o, flags, D_0046B870, D_0046DB80); }
void *func_0020BCF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B8B0, D_0046DB80); }
void *func_002A8980(void *o, s32 flags) { return room_dtor(o, flags, D_0046DBC0, D_0046DB80); }
void *func_002A8E00(void *o, s32 flags) { return room_dtor(o, flags, D_0046DC00, D_0046DB80); }
void *func_002A9600(void *o, s32 flags) { return room_dtor(o, flags, D_0046DC40, D_0046DB80); }
void *func_002A9A50(void *o, s32 flags) { return room_dtor(o, flags, D_0046DC80, D_0046DB80); }
void *func_002AA250(void *o, s32 flags) { return room_dtor(o, flags, D_0046DCC0, D_0046DB80); }
void *func_002AA440(void *o, s32 flags) { return room_dtor(o, flags, D_0046DD00, D_0046DB80); }
void *func_002AA920(void *o, s32 flags) { return room_dtor(o, flags, D_0046DD40, D_0046DB80); }
void *func_002AAB00(void *o, s32 flags) { return room_dtor(o, flags, D_0046DD80, D_0046DB80); }
void *func_002AB030(void *o, s32 flags) { return room_dtor(o, flags, D_0046DDC0, D_0046DB80); }
void *func_002AB1F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE00, D_0046DB80); }
void *func_002AB820(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE40, D_0046DB80); }
void *func_002AB8F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE80, D_0046DB80); }
void *func_002AB9F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DEC0, D_0046DB80); }
void *func_002ABF00(void *o, s32 flags) { return room_dtor(o, flags, D_0046DF00, D_0046DB80); }
void *func_002AC0A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DF40, D_0046DB80); }
void *func_002AC190(void *o, s32 flags) { return room_dtor(o, flags, D_0046DF80, D_0046DB80); }
void *func_002AC4D0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DFC0, D_0046DB80); }
void *func_002AC670(void *o, s32 flags) { return room_dtor(o, flags, D_0046E000, D_0046DB80); }
void *func_002ACB60(void *o, s32 flags) { return room_dtor(o, flags, D_0046E040, D_0046DB80); }
void *func_002AD020(void *o, s32 flags) { return room_dtor(o, flags, D_0046E080, D_0046DB80); }
void *func_002AD1C0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E0C0, D_0046DB80); }
void *func_002AD420(void *o, s32 flags) { return room_dtor(o, flags, D_0046E100, D_0046DB80); }
void *func_002AD690(void *o, s32 flags) { return room_dtor(o, flags, D_0046E140, D_0046DB80); }
void *func_002AD9F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E180, D_0046DB80); }
void *func_002ADD10(void *o, s32 flags) { return room_dtor(o, flags, D_0046E1C0, D_0046DB80); }
void *func_002ADF90(void *o, s32 flags) { return room_dtor(o, flags, D_0046E200, D_0046DB80); }
void *func_002AE090(void *o, s32 flags) { return room_dtor(o, flags, D_0046E240, D_0046DB80); }
void *func_002AE390(void *o, s32 flags) { return room_dtor(o, flags, D_0046E280, D_0046DB80); }
void *func_002AEFC0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E2C0, D_0046DB80); }
void *func_002AF7E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E300, D_0046DB80); }
void *func_002AF8E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E340, D_0046DB80); }
void *func_002AFC30(void *o, s32 flags) { return room_dtor(o, flags, D_0046E380, D_0046DB80); }
void *func_002B00F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E3C0, D_0046DB80); }
void *func_002B03C0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E400, D_0046DB80); }
void *func_002B0CF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E440, D_0046DB80); }
void *func_002B0E30(void *o, s32 flags) { return room_dtor(o, flags, D_0046E480, D_0046DB80); }
void *func_002B10A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E4C0, D_0046DB80); }
void *func_002B1600(void *o, s32 flags) { return room_dtor(o, flags, D_0046E500, D_0046DB80); }
void *func_002B1790(void *o, s32 flags) { return room_dtor(o, flags, D_0046E540, D_0046DB80); }
void *func_002B2330(void *o, s32 flags) { return room_dtor(o, flags, D_0046E580, D_0046DB80); }
void *func_002B25E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E5C0, D_0046DB80); }
void *func_002B2820(void *o, s32 flags) { return room_dtor(o, flags, D_0046E600, D_0046DB80); }
void *func_002B2960(void *o, s32 flags) { return room_dtor(o, flags, D_0046E640, D_0046DB80); }
void *func_002B2EF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E680, D_0046DB80); }
void *func_002B2FF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E6C0, D_0046DB80); }
void *func_002B3540(void *o, s32 flags) { return room_dtor(o, flags, D_0046E700, D_0046DB80); }
void *func_002B3970(void *o, s32 flags) { return room_dtor(o, flags, D_0046E740, D_0046DB80); }
void *func_002B3F10(void *o, s32 flags) { return room_dtor(o, flags, D_0046E780, D_0046DB80); }
void *func_002B4690(void *o, s32 flags) { return room_dtor(o, flags, D_0046E7C0, D_0046DB80); }
void *func_002B4840(void *o, s32 flags) { return room_dtor(o, flags, D_0046E800, D_0046DB80); }
void *func_002B4A70(void *o, s32 flags) { return room_dtor(o, flags, D_0046E840, D_0046DB80); }
void *func_002B4CD0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E880, D_0046DB80); }
void *func_002B4EF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E8C0, D_0046DB80); }
void *func_002B4FF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E900, D_0046DB80); }
void *func_002B50F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E940, D_0046DB80); }
void *func_002B5280(void *o, s32 flags) { return room_dtor(o, flags, D_0046E980, D_0046DB80); }
void *func_002B58B0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E9C0, D_0046DB80); }
void *func_002B5E80(void *o, s32 flags) { return room_dtor(o, flags, D_0046EA00, D_0046DB80); }
void *func_002CC950(void *o, s32 flags) { return room_dtor(o, flags, D_0046EDC0, D_0046DB80); }
void *func_002CCA90(void *o, s32 flags) { return room_dtor(o, flags, D_0046EE00, D_0046DB80); }
void *func_002D2500(void *o, s32 flags) { return room_dtor(o, flags, D_0046F3F0, D_0046DB80); }
void *func_002E5740(void *o, s32 flags) { return room_dtor(o, flags, D_0046FC40, D_0046DB80); }
void *func_002E5B40(void *o, s32 flags) { return room_dtor(o, flags, D_0046FC80, D_0046DB80); }
void *func_002E5FB0(void *o, s32 flags) { return room_dtor(o, flags, D_0046FCC0, D_0046DB80); }
void *func_002E6420(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD00, D_0046DB80); }
void *func_002E6890(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD40, D_0046DB80); }
void *func_002E6D00(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD80, D_0046DB80); }
void *func_002E6F10(void *o, s32 flags) { return room_dtor(o, flags, D_0046FDC0, D_0046DB80); }
void *func_002E7220(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE00, D_0046DB80); }
void *func_002E7460(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE40, D_0046DB80); }
void *func_002E76A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE80, D_0046DB80); }
void *func_002E78E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046FEC0, D_0046DB80); }
void *func_002FCB40(void *o, s32 flags) { return room_dtor(o, flags, D_00470DC0, D_0046DB80); }
void *func_002FEEB0(void *o, s32 flags) { return room_dtor(o, flags, D_00470EB0, D_0046DB80); }
void *func_002FEF80(void *o, s32 flags) { return room_dtor(o, flags, D_00470EF0, D_0046DB80); }
void *func_003001B0(void *o, s32 flags) { return room_dtor(o, flags, D_00470F50, D_0046DB80); }
void *func_00305E90(void *o, s32 flags) { return room_dtor(o, flags, D_00471020, D_0046DB80); }
void *func_00308990(void *o, s32 flags) { return room_dtor(o, flags, D_00471210, D_0046DB80); }
void *func_00308AF0(void *o, s32 flags) { return room_dtor(o, flags, D_00471250, D_0046DB80); }
void *func_0030EE90(void *o, s32 flags) { return room_dtor(o, flags, D_00471E60, D_0046DB80); }
void *func_0030EF80(void *o, s32 flags) { return room_dtor(o, flags, D_00471EA0, D_0046DB80); }
void *func_0030F070(void *o, s32 flags) { return room_dtor(o, flags, D_00471EE0, D_0046DB80); }
void *func_0030F850(void *o, s32 flags) { return room_dtor(o, flags, D_00471F20, D_0046DB80); }
void *func_0030F940(void *o, s32 flags) { return room_dtor(o, flags, D_00471F60, D_0046DB80); }
void *func_00310170(void *o, s32 flags) { return room_dtor(o, flags, D_00471FA0, D_0046DB80); }
void *func_00310A30(void *o, s32 flags) { return room_dtor(o, flags, D_00471FE0, D_0046DB80); }
void *func_0031E1C0(void *o, s32 flags) { return room_dtor(o, flags, D_00473460, D_0046DB80); }
void *func_00320E50(void *o, s32 flags) { return room_dtor(o, flags, D_00473C90, D_0046DB80); }
void *func_0032C690(void *o, s32 flags) { return room_dtor(o, flags, D_00474520, D_0046DB80); }
void *func_0032DBF0(void *o, s32 flags) { return room_dtor(o, flags, D_00474F40, D_0046DB80); }
void *func_00339CE0(void *o, s32 flags) { return room_dtor(o, flags, D_004760E0, D_0046DB80); }
void *func_0033EF80(void *o, s32 flags) { return room_dtor(o, flags, D_004771C0, D_0046DB80); }
void *func_0033F090(void *o, s32 flags) { return room_dtor(o, flags, D_00477200, D_0046DB80); }
void *func_0033F190(void *o, s32 flags) { return room_dtor(o, flags, D_00477240, D_0046DB80); }
void *func_0033F340(void *o, s32 flags) { return room_dtor(o, flags, D_00477280, D_0046DB80); }
void *func_0033F4F0(void *o, s32 flags) { return room_dtor(o, flags, D_004772C0, D_0046DB80); }
void *func_0033F680(void *o, s32 flags) { return room_dtor(o, flags, D_00477300, D_0046DB80); }
void *func_0033F840(void *o, s32 flags) { return room_dtor(o, flags, D_00477340, D_0046DB80); }
void *func_0033F930(void *o, s32 flags) { return room_dtor(o, flags, D_00477380, D_0046DB80); }
void *func_00340060(void *o, s32 flags) { return room_dtor(o, flags, D_004773C0, D_0046DB80); }
void *func_003409C0(void *o, s32 flags) { return room_dtor(o, flags, D_00477400, D_0046DB80); }
void *func_00340D60(void *o, s32 flags) { return room_dtor(o, flags, D_00477440, D_0046DB80); }
void *func_00341580(void *o, s32 flags) { return room_dtor(o, flags, D_00477480, D_0046DB80); }
void *func_00341B10(void *o, s32 flags) { return room_dtor(o, flags, D_004774C0, D_0046DB80); }
void *func_00343730(void *o, s32 flags) { return room_dtor(o, flags, D_00477500, D_0046DB80); }
void *func_003438C0(void *o, s32 flags) { return room_dtor(o, flags, D_00477540, D_0046DB80); }
void *func_00344050(void *o, s32 flags) { return room_dtor(o, flags, D_00477580, D_0046DB80); }
void *func_00344870(void *o, s32 flags) { return room_dtor(o, flags, D_00477610, D_0046DB80); }
void *func_00344970(void *o, s32 flags) { return room_dtor(o, flags, D_00477650, D_0046DB80); }
void *func_00344B90(void *o, s32 flags) { return room_dtor(o, flags, D_00477690, D_0046DB80); }
void *func_00344D80(void *o, s32 flags) { return room_dtor(o, flags, D_004776D0, D_0046DB80); }
void *func_00344E80(void *o, s32 flags) { return room_dtor(o, flags, D_00477710, D_0046DB80); }
void *func_00345000(void *o, s32 flags) { return room_dtor(o, flags, D_00477750, D_0046DB80); }
void *func_0034A140(void *o, s32 flags) { return room_dtor(o, flags, D_00478570, D_0046DB80); }
void *func_0034A360(void *o, s32 flags) { return room_dtor(o, flags, D_004785B0, D_0046DB80); }
void *func_0034A4C0(void *o, s32 flags) { return room_dtor(o, flags, D_004785F0, D_0046DB80); }
void *func_0034A9B0(void *o, s32 flags) { return room_dtor(o, flags, D_00478630, D_0046DB80); }
void *func_0034B000(void *o, s32 flags) { return room_dtor(o, flags, D_00478670, D_0046DB80); }
void *func_0034B0E0(void *o, s32 flags) { return room_dtor(o, flags, D_004786B0, D_0046DB80); }
void *func_0034B4C0(void *o, s32 flags) { return room_dtor(o, flags, D_004786F0, D_0046DB80); }
void *func_0034B5C0(void *o, s32 flags) { return room_dtor(o, flags, D_00478730, D_0046DB80); }
void *func_0034DBF0(void *o, s32 flags) { return room_dtor(o, flags, D_00478AA0, D_0046DB80); }
void *func_003506E0(void *o, s32 flags) { return room_dtor(o, flags, D_00478B80, D_0046DB80); }
void *func_00350E60(void *o, s32 flags) { return room_dtor(o, flags, D_00478C00, D_0046DB80); }
void *func_00350FF0(void *o, s32 flags) { return room_dtor(o, flags, D_00478C40, D_0046DB80); }
void *func_00352AF0(void *o, s32 flags) { return room_dtor(o, flags, D_00479340, D_0046DB80); }
void *func_00352C90(void *o, s32 flags) { return room_dtor(o, flags, D_00479380, D_0046DB80); }
void *func_00352E20(void *o, s32 flags) { return room_dtor(o, flags, D_004793C0, D_0046DB80); }
void *func_0035AEB0(void *o, s32 flags) { return room_dtor(o, flags, D_00479620, D_0046DB80); }
void *func_0035D1A0(void *o, s32 flags) { return room_dtor(o, flags, D_004798D0, D_0046DB80); }
void *func_0035D330(void *o, s32 flags) { return room_dtor(o, flags, D_00479910, D_0046DB80); }
void *func_0035D4C0(void *o, s32 flags) { return room_dtor(o, flags, D_00479950, D_0046DB80); }
void *func_0035D650(void *o, s32 flags) { return room_dtor(o, flags, D_00479990, D_0046DB80); }
void *func_0036A300(void *o, s32 flags) { return room_dtor(o, flags, D_00479FB0, D_0046DB80); }
void *func_0036DA30(void *o, s32 flags) { return room_dtor(o, flags, D_0047A070, D_0046DB80); }
void *func_0036DF30(void *o, s32 flags) { return room_dtor(o, flags, D_0047A0B0, D_0046DB80); }
void *func_0036E2D0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A0F0, D_0046DB80); }
void *func_0036E550(void *o, s32 flags) { return room_dtor(o, flags, D_0047A130, D_0046DB80); }
void *func_0036E800(void *o, s32 flags) { return room_dtor(o, flags, D_0047A170, D_0046DB80); }
void *func_0036EA80(void *o, s32 flags) { return room_dtor(o, flags, D_0047A1B0, D_0046DB80); }
void *func_0036EE70(void *o, s32 flags) { return room_dtor(o, flags, D_0047A1F0, D_0046DB80); }
void *func_0036F4A0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A230, D_0046DB80); }
void *func_0036F930(void *o, s32 flags) { return room_dtor(o, flags, D_0047A270, D_0046DB80); }
void *func_0036FCF0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A2B0, D_0046DB80); }
void *func_00378870(void *o, s32 flags) { return room_dtor(o, flags, D_0047A450, D_0046DB80); }
void *func_00378FE0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A490, D_0046DB80); }
void *func_00379290(void *o, s32 flags) { return room_dtor(o, flags, D_0047A4D0, D_0046DB80); }
void *func_00379540(void *o, s32 flags) { return room_dtor(o, flags, D_0047A510, D_0046DB80); }
void *func_00379820(void *o, s32 flags) { return room_dtor(o, flags, D_0047A550, D_0046DB80); }
void *func_00379C20(void *o, s32 flags) { return room_dtor(o, flags, D_0047A590, D_0046DB80); }
void *func_0037A2D0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A5D0, D_0046DB80); }
void *func_0037A5F0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A610, D_0046DB80); }
void *func_0037A8D0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A650, D_0046DB80); }
void *func_0037AB80(void *o, s32 flags) { return room_dtor(o, flags, D_0047A690, D_0046DB80); }

/* a table of the room's (by vtable slot) */
void *func_002AB880(void *o) { return D_0047A9D0; }   /* D_0046DE40 +0xC */
void *func_002AD0D0(void *o) { return D_0047AA40; }   /* D_0046E080 +0x20 */
void *func_002ADDE0(void *o) { return D_0047AA80; }   /* D_0046E1C0 +0x20 */
void *func_002AFCD0(void *o) { return D_0047AB10; }   /* D_0046E380 +0x20 */
void *func_002B2680(void *o) { return D_0047AB3C; }   /* D_0046E5C0 +0x20 */
void *func_002B28A0(void *o) { return D_0047AB50; }   /* D_0046E600 +0x10 */
void *func_002B28C0(void *o) { return D_0047AB58; }   /* D_0046E600 +0x20 */
void *func_002B28F0(void *o) { return D_0047AB70; }   /* D_0046E600 +0x38 */
void *func_002B48E0(void *o) { return D_0047AB98; }   /* D_0046E800 +0x20 */
void *func_002B4F90(void *o) { return D_0047ABC0; }   /* D_0046E8C0 +0x20 */
void *func_002B5080(void *o) { return D_0047ABD0; }   /* D_0046E900 +0x20 */
void *func_002B52E0(void *o) { return D_0047ABE0; }   /* D_0046E980 +0xC */
void *func_002D2590(void *o) { return D_0047AC10; }   /* D_0046F3F0 +0x14 */
void *func_002E57F0(void *o) { return D_0047AC40; }   /* D_0046FC40 +0x20 */
void *func_002E6480(void *o) { return D_0047AC60; }   /* D_0046FD00 +0xC */
void *func_002E64E0(void *o) { return D_0047AC68; }   /* D_0046FD00 +0x38 */
void *func_002E6FB0(void *o) { return D_0047AC78; }   /* D_0046FDC0 +0x20 */
void *func_002E7740(void *o) { return D_0047AC80; }   /* D_0046FE80 +0x20 */
void *func_002FCBB0(void *o) { return D_0047ACB0; }   /* D_00470DC0 +0x30 */
void *func_002FF010(void *o) { return D_0047ACC4; }   /* D_00470EF0 +0x14 */
void *func_00300250(void *o) { return D_0047ACC8; }   /* D_00470F50 +0x20 */
void *func_00339DC0(void *o) { return D_0047ADF8; }   /* D_004760E0 +0x38 */
void *func_0033F220(void *o) { return D_0047AE18; }   /* D_00477240 +0x14 */
void *func_0033F400(void *o) { return D_0047AE30; }   /* D_00477280 +0x38 */
void *func_0033F590(void *o) { return D_0047AE38; }   /* D_004772C0 +0x20 */
void *func_0033F9D0(void *o) { return D_0047AE50; }   /* D_00477380 +0x20 */
void *func_003437C0(void *o) { return D_0047AE74; }   /* D_00477500 +0x14 */
void *func_00343960(void *o) { return D_0047AE7C; }   /* D_00477540 +0x20 */
void *func_00344910(void *o) { return D_0047AE84; }   /* D_00477610 +0x20 */
void *func_00344A10(void *o) { return D_0047AE98; }   /* D_00477650 +0x20 */
void *func_00344F20(void *o) { return D_0047AEBC; }   /* D_00477710 +0x20 */
void *func_0034A590(void *o) { return D_0047AF04; }   /* D_004785F0 +0x38 */
void *func_0034AA80(void *o) { return D_0047AF08; }   /* D_00478630 +0x38 */
void *func_0034B060(void *o) { return D_0047AF0C; }   /* D_00478670 +0xC */
void *func_0034B0B0(void *o) { return D_0047AF1C; }   /* D_00478670 +0x38 */
void *func_0034B180(void *o) { return D_0047AF20; }   /* D_004786B0 +0x20 */
void *func_0034B1B0(void *o) { return D_0047AF28; }   /* D_004786B0 +0x38 */
void *func_0034B560(void *o) { return D_0047AF30; }   /* D_004786F0 +0x20 */
void *func_0034B680(void *o) { return D_0047AF40; }   /* D_00478730 +0x38 */
void *func_0034DC50(void *o) { return D_0047AF58; }   /* D_00478AA0 +0xC */
void *func_0034DC70(void *o) { return D_0047AF5C; }   /* D_00478AA0 +0x10 */
void *func_00350740(void *o) { return D_0047AF70; }   /* D_00478B80 +0xC */
void *func_00350760(void *o) { return D_0047AF78; }   /* D_00478B80 +0x10 */
void *func_00350EC0(void *o) { return D_0047AF88; }   /* D_00478C00 +0xC */
void *func_00350EF0(void *o) { return D_0047AF90; }   /* D_00478C00 +0x14 */
void *func_00351050(void *o) { return D_0047AF98; }   /* D_00478C40 +0xC */
void *func_00352B50(void *o) { return D_0047AFA0; }   /* D_00479340 +0xC */
void *func_00352B80(void *o) { return D_0047AFA8; }   /* D_00479340 +0x14 */
void *func_00352CF0(void *o) { return D_0047AFB0; }   /* D_00479380 +0xC */
void *func_00352D20(void *o) { return D_0047AFB8; }   /* D_00479380 +0x14 */
void *func_0035AF10(void *o) { return D_0047AFC0; }   /* D_00479620 +0xC */
void *func_0035D240(void *o) { return D_0047AFD8; }   /* D_004798D0 +0x20 */
void *func_0035D3D0(void *o) { return D_0047AFE0; }   /* D_00479910 +0x20 */
void *func_0035D560(void *o) { return D_0047AFE8; }   /* D_00479950 +0x20 */
void *func_0035D6F0(void *o) { return D_0047AFF0; }   /* D_00479990 +0x20 */
void *func_0036DAC0(void *o) { return D_0047B030; }   /* D_0047A070 +0x14 */
void *func_0036DFC0(void *o) { return D_0047B048; }   /* D_0047A0B0 +0x14 */
void *func_0036E330(void *o) { return D_0047B054; }   /* D_0047A0F0 +0xC */
void *func_0036E5B0(void *o) { return D_0047B058; }   /* D_0047A130 +0xC */
void *func_0036E5E0(void *o) { return D_0047B060; }   /* D_0047A130 +0x14 */
void *func_0036E860(void *o) { return D_0047B06C; }   /* D_0047A170 +0xC */
void *func_0036EED0(void *o) { return D_0047B070; }   /* D_0047A1F0 +0xC */
void *func_0036EF00(void *o) { return D_0047B078; }   /* D_0047A1F0 +0x14 */
void *func_0036F530(void *o) { return D_0047B07C; }   /* D_0047A230 +0x14 */
void *func_0036F550(void *o) { return D_0047B080; }   /* D_0047A230 +0x20 */
void *func_0036F9C0(void *o) { return D_0047B090; }   /* D_0047A270 +0x14 */
void *func_0036F9E0(void *o) { return D_0047B098; }   /* D_0047A270 +0x20 */
void *func_00378900(void *o) { return D_0047B0B0; }   /* D_0047A450 +0x14 */
void *func_00379070(void *o) { return D_0047B0C8; }   /* D_0047A490 +0x14 */
void *func_00379320(void *o) { return D_0047B0D4; }   /* D_0047A4D0 +0x14 */
void *func_003795D0(void *o) { return D_0047B0E0; }   /* D_0047A510 +0x14 */
void *func_003798B0(void *o) { return D_0047B0E8; }   /* D_0047A550 +0x14 */
void *func_00379CB0(void *o) { return D_0047B0F4; }   /* D_0047A590 +0x14 */
void *func_0037A390(void *o) { return D_0047B100; }   /* D_0047A5D0 +0x14 */
void *func_0037A680(void *o) { return D_0047B118; }   /* D_0047A610 +0x14 */
void *func_0037A930(void *o) { return D_0047B120; }   /* D_0047A650 +0xC */
void *func_0037A960(void *o) { return D_0047B128; }   /* D_0047A650 +0x14 */
void *func_0037AC10(void *o) { return D_0047B134; }   /* D_0047A690 +0x14 */

/* entry `i` of a table of the room's */
u32 func_002AB0E0(void *o, s32 i) { return ((u32 *)D_0047A9C8)[i]; }   /* D_0046DDC0 +0x34 */
u32 func_002AB8B0(void *o, s32 i) { return ((u32 *)D_0047A9DC)[i]; }   /* D_0046DE40 +0x24 */
u32 func_002AB8D0(void *o, s32 i) { return ((u32 *)D_0047A9E0)[i]; }   /* D_0046DE40 +0x34 */
u32 func_002AB9D0(void *o, s32 i) { return ((u32 *)D_0047A9E8)[i]; }   /* D_0046DE80 +0x34 */
u32 func_002ABFF0(void *o, s32 i) { return ((u32 *)D_0047A9F0)[i]; }   /* D_0046DF00 +0x34 */
u32 func_002AC170(void *o, s32 i) { return ((u32 *)D_0047A9F8)[i]; }   /* D_0046DF40 +0x34 */
u32 func_002FEF40(void *o, s32 i) { return ((u32 *)D_0047ACB8)[i]; }   /* D_00470EB0 +0x24 */
u32 func_002FEF60(void *o, s32 i) { return ((u32 *)D_0047ACC0)[i]; }   /* D_00470EB0 +0x34 */
u32 func_00300290(void *o, s32 i) { return ((u32 *)D_0047ACE0)[i]; }   /* D_00470F50 +0x34 */
u32 func_0035AF50(void *o, s32 i) { return ((u32 *)D_0047AFD0)[i]; }   /* D_00479620 +0x34 */
u32 func_0036A4B0(void *o, s32 i) { return ((u32 *)D_0047B028)[i]; }   /* D_00479FB0 +0x34 */
u32 func_0036DAE0(void *o, s32 i) { return ((u32 *)D_0047B040)[i]; }   /* D_0047A070 +0x24 */
u32 func_0036DFE0(void *o, s32 i) { return ((u32 *)D_0047B050)[i]; }   /* D_0047A0B0 +0x24 */
u32 func_0036E600(void *o, s32 i) { return ((u32 *)D_0047B068)[i]; }   /* D_0047A130 +0x24 */
u32 func_0036F560(void *o, s32 i) { return ((u32 *)D_0047B088)[i]; }   /* D_0047A230 +0x24 */
u32 func_0036F9F0(void *o, s32 i) { return ((u32 *)D_0047B0A0)[i]; }   /* D_0047A270 +0x24 */
u32 func_0036FD90(void *o, s32 i) { return ((u32 *)D_0047B0A8)[i]; }   /* D_0047A2B0 +0x24 */
u32 func_00378960(void *o, s32 i) { return ((u32 *)D_0047B0C0)[i]; }   /* D_0047A450 +0x34 */
u32 func_00379090(void *o, s32 i) { return ((u32 *)D_0047B0D0)[i]; }   /* D_0047A490 +0x24 */
u32 func_00379340(void *o, s32 i) { return ((u32 *)D_0047B0DC)[i]; }   /* D_0047A4D0 +0x24 */
u32 func_00379630(void *o, s32 i) { return ((u32 *)D_0047B0E4)[i]; }   /* D_0047A510 +0x34 */
u32 func_003798D0(void *o, s32 i) { return ((u32 *)D_0047B0F0)[i]; }   /* D_0047A550 +0x24 */
u32 func_00379D10(void *o, s32 i) { return ((u32 *)D_0047B0F8)[i]; }   /* D_0047A590 +0x34 */
u32 func_0037A370(void *o, s32 i) { return ((u32 *)D_0047B108)[i]; }   /* D_0047A5D0 +0x24 */
u32 func_0037A3B0(void *o, s32 i) { return ((u32 *)D_0047B110)[i]; }   /* D_0047A5D0 +0x34 */
u32 func_0037A6E0(void *o, s32 i) { return ((u32 *)D_0047B11C)[i]; }   /* D_0047A610 +0x34 */
u32 func_0037A980(void *o, s32 i) { return ((u32 *)D_0047B130)[i]; }   /* D_0047A650 +0x24 */

/* ---- the rooms' script hooks (pointers to members the event commands call) ---- */

#include "game.h"
#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"

extern Character *gCharacters[];
extern Character *gCharPursuer;
extern VObject *D_0044E4B8;   /* the camera: +0xD4 (pos) on screen */
extern VObject *D_0044E4D0;   /* the events: +0x30 (var, value), +0x34 (var), +0x5C (n) */
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */
extern void func_00125960(Character *c);
extern f32 func_002E2D00(f32 a);

/* the pursuer is about and up to something (+0xE8 0 / 1) or out of sight */
s32 func_002E5880(void) {
    Character *p = gCharPursuer;

    if (p != NULL && p->a.active != 0 &&
        (p->unkE8 == 0 || p->unkE8 == 1 || !(VCALL(D_0044E4B8, 0xD4, u32 (*)(VObject *, f32 *))(D_0044E4B8, p->a.pos) & 0xFF))) {
        return 1;
    }
    return 0;
}

/* a character `c` hook by the command's byte 3: 0 its +0xE1 / +0xF4 cleared, 1 it turns
 * (pi - 0.1 x event var 2, at least 1.57; at the limit event +0x5C(3)) and var 2 goes up by 2,
 * 2 func_00125960; then while character kind 0x21 is right of x -84, event var 1 = 1 */
s32 func_002E5950(void *self, Character *c, u8 *cmd) {
    Character *who = gCharacters[func_001770D0(gProgress, 0x21) & 0xFF];
    VObject *ev;
    f32 a;

    switch (cmd[3]) {
    case 0:
        c->unkE1 = 0;
        c->unkF4 = 0;
        break;
    case 1:
        ev = D_0044E4D0;
        a = 0x1.921fb6p+1f /* pi */ - 0x1.99999ap-4f /* 0.1 */ * (f32)(u32)VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2);
        if (a <= 0x1.91eb86p+0f /* 1.57 */) {
            a = 0x1.91eb86p+0f;
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
        }
        a = func_002E2D00(a);
        c->a.angle[1] = a;
        sceVu0UnitMatrix(c->a.rot);
        sceVu0RotMatrixY(c->a.rot, c->a.rot, a);
        ev = D_0044E4D0;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 2, VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2) + 2);
        break;
    case 2:
        func_00125960(c);
        break;
    }
    if (!(who->a.pos[0] <= -84.0f)) {
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 1, 1);
    }
    return 1;
}
