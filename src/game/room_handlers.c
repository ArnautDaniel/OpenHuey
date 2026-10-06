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

extern VObject *D_00456DF8;   /* the room's objects: +0x18 (id) the object */
extern Character *gCharPlayer;
extern f32 func_0031C248(f32 x);   /* sinf */
extern void func_00122C20(Actor *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos);

/* three hanging things (the room's +0x34 (0..2) objects) swinging, by the command's byte 3:
 * 0 at rest (push +0x3C 0.9); 1 Fiona's movement pushes them (the squared step) - past 1 one
 * swings for 20 frames (+0x38) and the first toggles event flag 3 with a sound (Fiona's 1 /
 * 2); 2 a swing step: +0x30 on by 36 degrees, its tilt +0x10 = (1 + sin) degrees in radians */
static inline __attribute__((always_inline)) s32 swing_three_by(VObject *self, u8 *cmd, s32 byIndex) {
    VObject *objs = D_00456DF8, *ev = D_0044E4D0;
    s32 i;

    for (i = 0; i < 3; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, s32))(objs, VCALL(self, 0x34, s32 (*)(VObject *, s32))(self, i));

        if (o == NULL) {
            continue;
        }
        switch (cmd[3]) {
        case 0:
            AT(o, 0x30, s32) = 0;
            AT(o, 0x38, s32) = 0;
            AT(o, 0x3C, f32) = 0x1.ccccccp-1f /* 0.9 */;
            break;
        case 1:
            if (AT(o, 0x38, f32) <= 8.0f) {
                Character *ch = byIndex ? gCharacters[cmd[4]] : gCharPlayer;
                f32 d[4] __attribute__((aligned(16)));

                if (ch == NULL) {
                    break;
                }
                sceVu0CopyVector(d, ch->a.prevPos);
                sceVu0SubVector(d, ch->a.pos, d);
                AT(o, 0x3C, f32) = AT(o, 0x3C, f32) + (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
                if (!(AT(o, 0x3C, f32) <= 1.0f)) {
                    AT(o, 0x38, f32) = 20.0f;
                    AT(o, 0x3C, s32) = 0;
                    if (i == 0) {
                        if ((VCALL(ev, 0x58, u32 (*)(VObject *, s32))(ev, 3) & 0xFF) == 1) {
                            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 3);
                            func_00122C20(&ch->a, 1, 6, 0, 0, NULL);
                        } else {
                            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
                            func_00122C20(&ch->a, 2, 6, 0, 0, NULL);
                        }
                    }
                }
            }
            break;
        case 2:
            if (!(AT(o, 0x38, f32) <= 0.0f)) {
                f32 t;

                AT(o, 0x38, f32) = AT(o, 0x38, f32) - 1.0f;
                if (AT(o, 0x38, f32) < 0.0f) {
                    AT(o, 0x38, s32) = 0;
                }
                AT(o, 0x30, f32) = AT(o, 0x30, f32) + 36.0f;
                if (!(AT(o, 0x30, f32) < 360.0f)) {
                    AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
                }
                t = 0x1.921fb6p+1f * (1.0f + func_0031C248(0x1.921fb6p+1f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
                AT(o, 0x10, f32) = t;
                if (!(t <= 0x1.921fb6p+1f)) {
                    AT(o, 0x10, f32) = t - 0x1.921fb6p+2f;
                }
            }
            break;
        }
    }
    return 1;
}

static s32 swing_three(VObject *self, u8 *cmd) {
    return swing_three_by(self, cmd, 0);
}

s32 func_002E5C60(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
s32 func_002E60D0(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
s32 func_002E6540(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
s32 func_002E69B0(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }

/* ---- more hooks: effects spawned into the effect manager, Fiona nudged, a partner's line ---- */

#include "effectmgr.h"

extern void *D_00479A80[], *D_0047A3D0[], *D_0047A730[];
extern s32 D_0047B274, D_0047B278, D_0047B27C;   /* the nudge countdowns */
extern const char *D_004193A8, *D_004193AC;
extern VObject *gBootMessage;
extern Character *gCharPartner;
extern void func_0016CEC0(Progress *p, const char *name);
extern s32 func_0016CD60(Progress *p, s32 who, s32 arg);
extern void func_0016CD30(Progress *p);
extern s32 func_0032D150(Character *c);
extern void func_0032D270(Character *c, s32 a, f32 x, f32 y);
extern void func_002D6170(u8 *mgr, s32 slot);

static void effect_C0_init(void **obj) {
    obj[0] = D_00479A80;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

static void effect_10_init(void **obj) {
    obj[0] = D_0047A3D0;
}

static void effect_4480_init(void **obj) {
    obj[0] = D_0047A730;
    obj[0x3010 / 4] = D_00469D00;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = D_0046FC30;
}

s32 func_002E6E20(void) {
    Effect_New(D_0044E578, 0xC0, effect_C0_init);
    return 1;
}

s32 func_002E7020(void) {
    Effect_New(D_0044E578, 0x10, effect_10_init);
    return 1;
}

/* byte 3 0: a 90-frame countdown starts; else while it runs the character kind 0xE moves by
 * (dx, 3) on +0x14 / +0x18 (2) */
static s32 nudge(s32 *count, u8 *cmd, f32 dx) {
    Character *c;

    if (cmd[3] == 0) {
        *count = 90;
        return 1;
    }
    if (--*count == 0) {
        return 1;
    }
    c = gCharacters[func_001770D0(gProgress, 0xE) & 0xFF];
    AT(c, 0x14, f32) = AT(c, 0x14, f32) + dx;
    AT(c, 0x18, f32) = AT(c, 0x18, f32) + 3.0f;
    return 2;
}

s32 func_002E70F0(void *self, void *a1, u8 *cmd) { return nudge(&D_0047B274, cmd, 2.0f); }
s32 func_002E7560(void *self, void *a1, u8 *cmd) { return nudge(&D_0047B278, cmd, 1.0f); }
s32 func_002E77B0(void *self, void *a1, u8 *cmd) { return nudge(&D_0047B27C, cmd, 1.0f); }

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for func_0016CD60(1, 0), then the partner's
 * message slot shows progress +0x73EDC0; else func_0016CD30 and the slot is closed */
s32 func_002E7350(void *self, void *a1, u8 *cmd) {
    Progress *p;

    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_004193A8);
        break;
    case 1:
        func_0016CEC0(gProgress, D_004193AC);
        break;
    case 2:
        p = gProgress;
        if (func_0016CD60(p, 1, 0) == 0) {
            return 2;
        }
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, s32, s32))(gBootMessage, gCharPartner->msgSlot,
                                                                       AT(p, 0x73EDC0, s32), 1);
        break;
    default:
        func_0016CD30(gProgress);
        VCALL(gBootMessage, 0x14, void (*)(VObject *, u32))(gBootMessage, gCharPartner->msgSlot);
        break;
    }
    return 1;
}

/* character kind 0x1A: byte 3 0 starts func_0032D270(2, -6, 257); else waits (2) until
 * func_0032D150 says done */
s32 func_002E7600(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[func_001770D0(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        func_0032D270(c, 2, -6.0f, 257.0f);
        return 1;
    }
    return func_0032D150(c) == 0 ? 2 : 1;
}

s32 func_002E7A00(void) {
    return AT(gProgress, 0xFB6, s16) >= 100;
}

/* byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's
 * effect is removed */
s32 func_002E7A50(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(D_0044E578, 0x4480, effect_4480_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, slot);
    } else {
        func_002D6170(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0));
    }
    return 1;
}

extern VObject *D_0044E988;   /* the items: +0x8 the list */
extern u32 func_00260CF0(void *list, s32 item);   /* how many */
extern void func_00261090(void *list, s32 item, s32 n);   /* given */

/* an empty hook */
void func_002FCB30(void) {
}

/* the items 0x91 / 0x92, by byte 3: 0 which of them Fiona lacks (one each) kept in event var 0
 * (0x91 low byte, 0x92 the next), and event +0x5C(3) when any; 1 they are given back, event
 * +0x5C(0) / (1) */
s32 func_002FCC30(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    u32 got;

    switch (cmd[3]) {
    case 0: {
        u8 *items = (u8 *)D_0044E988 + 8;
        s32 a = 1 - (func_00260CF0(items, 0x91) & 0xFF);
        s32 b;

        if (a < 0) {
            a = 0;
        }
        b = 1 - (func_00260CF0(items, 0x92) & 0xFF);
        if (b < 0) {
            b = 0;
        }
        if ((a | b) != 0) {
            ev = D_0044E4D0;
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, (b << 8) | a);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
        }
        break;
    }
    case 1:
        ev = D_0044E4D0;
        got = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 0);
        if (got & 0xFF) {
            func_00261090((u8 *)D_0044E988 + 8, 0x91, got & 0xFF);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
        }
        if (got & 0xFF00) {
            func_00261090((u8 *)D_0044E988 + 8, 0x92, (got >> 8) & 0xFF);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 1);
        }
        break;
    }
    return 1;
}

extern u8 *D_0044E4C0;        /* the room's effects */
extern VObject *D_0044FE10;   /* the cutscene director: +0x34 its frame */
extern void *D_0046EC60[], *D_00472F60[];   /* a depth range, a lit quad */
extern void *func_002672F0(u32 size, void *place);
extern s32 func_00266C70(u8 *fx, s32 n, void *arg);
extern void func_002670F0(u8 *fx, s32 n);   /* effect slot n gone */

/* the room's effect n made anew as class vtbl (any old one dropped) */
static void room_effect_new(u8 *fx, s32 n, void **vtbl) {
    VObject *pool = (VObject *)(fx + 0x1400);
    VObject **slot = &AT(fx, 0x1438 + n * 4, VObject *);
    void *mem;

    if (*slot != NULL) {
        VCALL(pool, 0x14, void (*)(VObject *, void *))(pool, *slot);
        *slot = NULL;
    }
    mem = VCALL(pool, 0x10, void *(*)(VObject *, u32))(pool, 0xA0);
    if (mem != NULL) {
        VObject *e = func_002672F0(0xA0, mem);

        if (e != NULL) {
            e->vtbl = vtbl;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(VObject *))(*slot);
    }
}

/* the pursuer's model's +0x9E8 by byte 3: 0 0.15, 1 0, else 0.05 */
s32 func_002FF0A0(void *self, void *a1, u8 *cmd) {
    u8 *m = AT(gCharacters[func_001770D0(gProgress, 0xFE) & 0xFF], 0xF0, u8 *);

    switch (cmd[3]) {
    case 0:
        AT(m, 0x9E8, f32) = 0x1.333334p-3f /* 0.15 */;
        break;
    case 1:
        AT(m, 0x9E8, s32) = 0;
        break;
    default:
        AT(m, 0x9E8, f32) = 0x1.99999ap-5f /* 0.05 */;
        break;
    }
    return 1;
}

/* the depth range (effect 0x1C) opening out with the cutscene from its frame 1260: 1 / 21 / 40
 * / 80 on by 0.4 a frame, up to 41 / 61 / 80 / 120 */
s32 func_002FF130(void) {
    u8 *fx = D_0044E4C0;
    f32 t = (f32)(VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 1260);
    f32 r[4] __attribute__((aligned(16)));
    f32 d;

    room_effect_new(fx, 0x1C, D_0046EC60);
    d = 0x1.999998p-2f /* 0.4 */ * t;
    r[0] = 1.0f + d;
    if (!(r[0] <= 41.0f)) {
        r[0] = 41.0f;
    }
    r[1] = 21.0f + d;
    if (!(r[1] <= 61.0f)) {
        r[1] = 61.0f;
    }
    r[2] = 40.0f + d;
    if (!(r[2] <= 80.0f)) {
        r[2] = 80.0f;
    }
    r[3] = 80.0f + d;
    if (!(r[3] <= 120.0f)) {
        r[3] = 120.0f;
    }
    func_00266C70(fx, 0x1C, r);
    return 1;
}

s32 func_002FF300(void) {
    return 1;
}

s32 func_002FF310(void) {
    return 1;
}

/* a lit quad (room effect n) with corners q[0..15] and colour value `c`, by byte 3: 1 removed,
 * 2 on, else off */
static inline __attribute__((always_inline)) s32 lit_quad_in(s32 n, u8 *cmd, const u32 *corners, u32 c) {
    u8 *fx;
    u32 q[20] __attribute__((aligned(16)));
    s32 i;

    if (cmd[3] == 1) {
        func_002670F0(D_0044E4C0, n);
        return 1;
    }
    fx = D_0044E4C0;
    room_effect_new(fx, n, D_00472F60);
    for (i = 0; i < 16; i++) {
        q[i] = corners[i];
    }
    q[0x10] = cmd[3] == 2 ? 0x3F800000 : 0;
    q[0x11] = c;
    q[0x12] = 0;
    q[0x13] = c;
    func_00266C70(fx, n, q);
    return 1;
}

/* the usual one, room effect 0x1B */
static s32 lit_quad(u8 *cmd, const u32 *corners, u32 c) {
    return lit_quad_in(0x1B, cmd, corners, c);
}

/* a lit quad at x -15.96, z -2 .. 6, height 111 / 91 */
s32 func_002FF320(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC17F5810, 0x42DE0000, 0xC00001A3, 0x3F800000, 0xC17F5810, 0x42DE0000, 0x40BFFF2E, 0x3F800000,
        0xC17F5810, 0x42B60000, 0xC00001A3, 0x3F800000, 0xC17F5810, 0x42B60000, 0x40BFFF2E, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}

#include "effectmgr.h"

extern void *D_00474000[];   /* the room 0x2A effect (props.c) */

static void room2a_effect_init(void **obj) {
    obj[0] = D_00474000;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* room 0x2A: its effect (D_00474000) started at (220, 0, -100) */
s32 func_002B12D0(void) {
    u8 *mgr = D_0044E578;
    s32 slot = Effect_New(mgr, 0x900, room2a_effect_init);
    f32 at[4] __attribute__((aligned(16)));

    at[0] = 220.0f;
    at[2] = -100.0f;
    at[1] = 0.0f;
    at[3] = 1.0f;
    func_002D6090(mgr, slot, at);
    return 1;
}

extern void *D_00478BC0[];   /* a 0x14-byte effect (props.c) */
extern VObject *D_0044F260;   /* the placed things */
extern u8 *D_0044F808;        /* the stalker in play */
extern s32 func_0029A710(void *p);
extern s32 func_00177620(Progress *p);
extern const char *const D_00405618, *const D_0040561C;   /* "kibako" (the box), "a_koushi" (the grate) */

/* the pursuer's func_0029A710 */
s32 func_002B0E20(void) {
    return func_0029A710(gCharPursuer);
}

/* no thing of kind 3 lies about */
s32 func_002B0F60(void) {
    return VCALL(D_0044F260, 0x10, void *(*)(VObject *, s32, s32))(D_0044F260, 3, 0) == NULL;
}

/* the stalker in play is chasing (+0x153C 2, 6 or 7, not +0xC4 2) with the progress state 2:
 * in this room, whether the camera sees it; elsewhere 1 */
s32 func_002B0FA0(void) {
    u8 *s = D_0044F808;
    Progress *p;
    u8 k;

    if (s == NULL || AT(s, 0x28, u8) == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    p = gProgress;
    if ((func_00177620(p) & 0xFF) != 2) {
        return 0;
    }
    if (AT(s, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return VCALL(D_0044E4B8, 0xD4, s32 (*)(VObject *, f32 *))(D_0044E4B8, (f32 *)(s + 0x10));
    }
    return 1;
}

static void effect_14_init(void **obj) {
    obj[0] = D_00478BC0;
}

/* the 0x14-byte effect (D_00478BC0) started with the command's parameters (from byte 3) */
s32 func_002B11D0(void *self, void *a1, u8 *cmd) {
    u8 *mgr = D_0044E578;

    func_002D6090(mgr, Effect_New(mgr, 0x14, effect_14_init), cmd + 3);
    return 1;
}

/* the box and the grate, by byte 3: 0 the box's +0x28 on by 0.4; the grate's +0x14 (an angle)
 * 1 back 1.5 degrees, 2 on 0.5, 3 back 0.5 */
s32 func_002B14A0(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
    u8 *o;

    switch (cmd[3]) {
    case 0:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00405618);
        if (o != NULL) {
            AT(o, 0x28, f32) = AT(o, 0x28, f32) + 0x1.99999ap-2f /* 0.4 */;
        }
        break;
    case 1:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040561C);
        if (o != NULL) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - 0x1.aceeap-6f /* 1.5 degrees */;
        }
        break;
    case 2:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040561C);
        if (o != NULL) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) + 0x1.1df46ap-7f /* 0.5 degrees */;
        }
        break;
    case 3:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040561C);
        if (o != NULL) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - 0x1.1df46ap-7f;
        }
        break;
    }
    return 1;
}

extern void *D_00470A50[];   /* the rising motes */
extern const char *const D_00403948, *const D_0040394C;   /* "left", "right" */
extern const char *const D_0040395C, *const D_00403960;   /* "movechair_1", "movechair_2" */
extern s32 func_0032D2C0(Character *c);
extern void func_0032D3E0(Character *c, s32 a, f32 x, f32 y);

static void motes_init(void **obj) {
    obj[0] = D_00470A50;
    obj[0x3010 / 4] = D_00469D00;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = D_0046FC30;
}

/* the rising motes started */
s32 func_002B02D0(void) {
    Effect_New(D_0044E578, 0x3860, motes_init);
    return 1;
}

/* the partner's target (+0xF35E0 on, +0xF35F0) 3 above the rocking chair (movechair_2): its
 * seat 6 ahead, tipped by its rock (90 x +0x34 x sin +0x30 degrees) and turned with it */
s32 func_002B04E0(void) {
    u8 *chair = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00403960);
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));

    v[0] = 0.0f;
    v[1] = 6.0f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, 0x1.921fb6p+1f * (90.0f * AT(chair, 0x34, f32) * func_0031C248(0x1.921fb6p+1f * AT(chair, 0x30, f32) / 180.0f)) / 180.0f);
    sceVu0RotMatrixY(m, m, AT(chair, 0x14, f32));
    sceVu0ApplyMatrix(v, m, v);
    sceVu0AddVector(at, (f32 *)(chair + 0x20), v);
    at[1] = at[1] + 3.0f;
    AT(gCharPartner, 0xF35E0, u8) = 1;
    sceVu0CopyVector((f32 *)((u8 *)gCharPartner + 0xF35F0), at);
    return 1;
}

/* the two doors ("left", "right") opening: byte 3 0 at once (2.25), else a step (0.075) */
s32 func_002B0B60(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
    u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00403948);

    if (o != NULL) {
        AT(o, 0x14, f32) = AT(o, 0x14, f32) - (cmd[3] == 0 ? 2.25f : 0x1.333334p-4f /* 0.075 */);
    }
    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040394C);
    if (o != NULL) {
        AT(o, 0x14, f32) = AT(o, 0x14, f32) + (cmd[3] == 0 ? 2.25f : 0x1.333334p-4f);
    }
    return 1;
}

/* the first rocking chair still rocking (+0x34 over 0.3) */
s32 func_002B0C90(void) {
    u8 *chair = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0040395C);

    return !(AT(chair, 0x34, f32) <= 0x1.333334p-2f /* 0.3 */);
}

/* character kind 0x1A: byte 3 0 starts func_0032D3E0(2, -290, 42); else waits (2) until
 * func_0032D2C0 says done */
s32 func_002B1700(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[func_001770D0(gProgress, 0x1A) & 0xFF];

#ifdef HG_NATIVE
    if ((func_001770D0(gProgress, 0x1A) & 0xFF) >= 6 || c == NULL) {   /* (the PS2 writes through junk) */
        return 1;
    }
#endif
    if (cmd[3] != 0) {
        return func_0032D2C0(c) == 0 ? 2 : 1;
    }
    func_0032D3E0(c, 2, -290.0f, 42.0f);
    return 1;
}

s32 func_002B2950(void) {
    return 1;
}

extern const char *const D_004070C0, *const D_004070C4, *const D_004070C8;   /* "sara_l", "sara_r", "tenbin" */
extern void *D_00479580[], *D_0047A390[];

static f32 hook_sqrt(f32 x) {
    return __builtin_sqrtf(x);
}

/* Hewie (in state 0x7F, +0xF3564) within 5 of the spot by byte 3: 0 (-259.5, 190), 1 (-276,
 * 160) (others: what the caller left) */
s32 func_002B18B0(void *self, void *a1, u8 *cmd) {
    Character *h = gCharacters[func_001770D0(gProgress, 1) & 0xFF];
    f32 dx = 0.0f, dz = 0.0f;

    if (h == NULL || AT(h, 0x28, u8) == 0 || AT(h, 0xF3564, s32) != 0x7F) {
        return 0;
    }
    switch (cmd[3]) {
    case 0:
        dx = 259.5f + AT(h, 0x10, f32);
        dz = AT(h, 0x18, f32) - 190.0f;
        break;
    case 1:
        dx = 276.0f + AT(h, 0x10, f32);
        dz = AT(h, 0x18, f32) - 160.0f;
        break;
    }
    return hook_sqrt(dz * dz + dx * dx) < 5.0f;
}

/* the scales ("tenbin") and their pans ("sara_l", "sara_r"): level (byte 3 0) or tipped */
s32 func_002B2450(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
    u8 *o;

    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_004070C0);
    if (o != NULL) {
        AT(o, 0x10, u32) = 0;
        AT(o, 0x14, u32) = 0;
        AT(o, 0x18, u32) = 0;
        if (cmd[3] == 0) {
            AT(o, 0x20, u32) = 0x40C669AD;
            AT(o, 0x24, u32) = 0x419E6666;
        } else {
            AT(o, 0x20, u32) = 0x40CB367A;
            AT(o, 0x24, u32) = 0x41A4CCCD;
        }
        AT(o, 0x28, u32) = 0x4188CCCD;
    }
    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_004070C4);
    if (o != NULL) {
        AT(o, 0x10, u32) = 0;
        AT(o, 0x14, u32) = 0;
        AT(o, 0x18, u32) = 0;
        if (cmd[3] == 0) {
            AT(o, 0x20, u32) = 0x404322D1;
            AT(o, 0x24, u32) = 0x419E6666;
        } else {
            AT(o, 0x20, u32) = 0x40660419;
            AT(o, 0x24, u32) = 0x419A0000;
        }
        AT(o, 0x28, u32) = 0x4188CCCD;
    }
    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_004070C8);
    if (o != NULL) {
        AT(o, 0x10, u32) = 0;
        AT(o, 0x14, u32) = 0;
        AT(o, 0x18, u32) = cmd[3] == 0 ? 0 : 0x3EDF66F3;
        AT(o, 0x20, u32) = 0x4093D14E;
        AT(o, 0x24, u32) = 0x41A4CCCD;
        AT(o, 0x28, u32) = 0x4188A7F0;
    }
    return 1;
}

static void effect_6cf0_init(void **obj) {
    obj[0] = D_00479580;
    obj[0x6040 / 4] = D_00469D00;
    ((s32 *)obj)[0x6044 / 4] = -1;
    obj[0x6040 / 4] = D_0046FC30;
    obj[0x6078 / 4] = D_00469D00;
    ((s32 *)obj)[0x607C / 4] = -1;
    obj[0x6078 / 4] = D_0046FC30;
    obj[0x60B0 / 4] = D_00469D00;
    ((s32 *)obj)[0x60B4 / 4] = -1;
    obj[0x60B0 / 4] = D_0046FC30;
}

/* the effect D_00479580 (three quad drawers) started with parameter 0 */
s32 func_002B26F0(void) {
    u8 *mgr = D_0044E578;
    s32 slot = Effect_New(mgr, 0x6CF0, effect_6cf0_init);
    s32 arg = 0;

    func_002D6090(mgr, slot, &arg);
    return 1;
}

static void effect_1a60_init(void **obj) {
    obj[0] = D_0047A390;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

/* byte 3 0: the effect D_0047A390 spawned, its slot in event var 0; 1: removed */
s32 func_002B2A80(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0: {
        s32 slot = Effect_New(D_0044E578, 0x1A60, effect_1a60_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, slot);
        break;
    }
    case 1:
        func_002D6170(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0));
        break;
    }
    return 1;
}

/* ---- room 0x61's light shaft, class D_0047A370 (0x700 bytes): a beam of light (a scrolling
 * texture on a strip between six points, drawn twice) with 16 dust motes rising through it
 * (double-buffered quad records +0x10 + buffer +0x6EC * 0x300, drawn by the quad drawer at
 * +0x610), each with its rise (+0x660 + i * 4) and wobble angle (+0x6A0); +0x6E8 the
 * texture's scroll (0 .. 4096, 1/16 texels), +0x650 where the motes start, +0x6F0 stopped,
 * +0x6F1 the haze on: the whole screen wavers (phases +0x6E0 / +0x6E4) ---- */

#include "texcache.h"

extern void *D_0047A370[], *D_0046F580[];
extern VObject *D_0044E550;   /* random numbers */
extern void func_002D63B0(void *p);   /* free (the effect manager's heap) */
extern f32 func_002E2D00(f32 angle);   /* wrapped into -pi..pi */
extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */
extern void func_002E56C0(u8 *quad);

#define SHAFT_REC(o, i) ((o) + AT(o, 0x6EC, s32) * 0x300 + (i) * 0x30 + 0x10)

static f32 shaft_rnd(VObject *rnd) {
    return VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
}

/* +0x8 destructor (the quad drawer's inlined) */
u8 *func_00374B30(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A370;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* mote i (re)started at the bottom: rising 0.1 .. 0.3 a frame, grey, alpha 0x40 .. 0x7F, spread
 * 50 either way along z, sized 3/4 of its rise, a random turn */
void func_00374BC0(u8 *o, s32 i) {
    VObject *rnd;
    u8 *r;

    if (AT(o, 0x6F0, u8) == 1) {
        return;
    }
    rnd = D_0044E550;
    AT(o, 0x660 + i * 4, f32) = 0x1.99999ap-4f /* 0.1 */ + 0x1.99999ap-3f /* 0.2 */ * shaft_rnd(rnd);
    AT(o, 0x6A0 + i * 4, f32) = 0x1.921fb6p+2f /* 2 pi */ * (shaft_rnd(rnd) - 0.5f);
    r = SHAFT_REC(o, i);
    AT(r, 0x0, s32) = 0x80;
    AT(r, 0x4, s32) = 0x80;
    AT(r, 0x8, s32) = 0x80;
    AT(r, 0xC, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
    AT(r, 0x10, f32) = AT(o, 0x650, f32);
    AT(r, 0x14, f32) = AT(o, 0x654, f32);
    AT(r, 0x18, f32) = AT(o, 0x658, f32) + 100.0f * (shaft_rnd(rnd) - 0.5f);
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = AT(r, 0x24, f32) = 0.75f * AT(o, 0x660 + i * 4, f32);
    AT(r, 0x28, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
    AT(r, 0x2C, s32) = 0;
}

/* +0x18 start: NULL stops it; kind (+0x10) 0 the motes from the position (+0x0), 1 the haze
 * on, 2 off */
void func_00374DB0(u8 *o, u8 *arg) {
    s32 i;

    if (arg == NULL) {
        AT(o, 0x6F0, u8) = 1;
        return;
    }
    switch (AT(arg, 0x10, s32)) {
    case 0:
        sceVu0CopyVector((f32 *)(o + 0x650), (f32 *)arg);
        for (i = 0; i < 16; i++) {
            func_00374BC0(o, i);
        }
        break;
    case 1:
        AT(o, 0x6F1, u8) = 1;
        break;
    case 2:
        AT(o, 0x6F1, u8) = 0;
        break;
    }
}

/* +0x10 update: the buffers swapped (the motes copied over), the texture scrolled 3.2 / 16
 * texels; each mote turns, wobbles 0.1 about its angle and rises; on every other frame it
 * fades by 0 or 1 and starts over once gone. The haze's phases on by 3 .. 5 and 1 .. 3 degrees */
s32 func_00376200(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *rnd;
    f32 t;
    s32 i, k;

    if (AT(o, 0x6F0, u8) == 1) {
        return 0;
    }
    AT(o, 0x6EC, s32) ^= 1;
    AT(o, 0x6E8, s32) = (s32)((f32)AT(o, 0x6E8, s32) + 0x1.99999ap+1f /* 3.2 */);
    t = (f32)AT(o, 0x6E8, s32);
    if (!(t <= 4096.0f)) {
        AT(o, 0x6E8, s32) = (s32)(t - 4096.0f);
    }
    rnd = D_0044E550;
    for (i = 0; i < 16; i++) {
        s32 cur = AT(o, 0x6EC, s32);
        u32 *dst = (u32 *)(o + cur * 0x300 + i * 0x30 + 0x10);
        u32 *src = (u32 *)(o + (cur ^ 1) * 0x300 + i * 0x30 + 0x10);
        u8 *r;
        f32 a;

        for (k = 0; k < 12; k++) {
            *dst++ = *src++;
        }
        r = SHAFT_REC(o, i);
        AT(r, 0x28, f32) = AT(r, 0x28, f32) + 0.5f * (kPi.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) / 180.0f);
        a = AT(o, 0x6A0 + i * 4, f32) + kPi.f * (90.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x6A0 + i * 4, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x6A0 + i * 4, f32) = a - k2Pi.f;
        }
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + 0x1.99999ap-4f /* 0.1 */ * func_0031C248(AT(o, 0x6A0 + i * 4, f32));
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + AT(o, 0x660 + i * 4, f32);
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + 0x1.99999ap-4f /* 0.1 */ * func_0031C058(AT(o, 0x6A0 + i * 4, f32));
        if (AT(o, 0x6EC, s32) == 0) {
            AT(r, 0xC, s32) = AT(r, 0xC, s32) - (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 1);
            if (AT(r, 0xC, s32) < 0) {
                func_00374BC0(o, i);
            }
        }
    }
    if (AT(o, 0x6F1, u8) == 1) {
        rnd = D_0044E550;
        t = AT(o, 0x6E0, f32) + kPi.f * (3.0f + 2.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x6E0, f32) = t;
        AT(o, 0x6E0, f32) = func_002E2D00(t);
        t = AT(o, 0x6E4, f32) + kPi.f * (1.0f + 2.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x6E4, f32) = t;
        AT(o, 0x6E4, f32) = func_002E2D00(t);
    }
    return 1;
}

/* +0xC set up: a random scroll; the motes' drawer: layer 0x19, texture group 0x10 cell
 * (0x1A0, 0x40) 32 x 32 of 512 x 256, additive, 5 frames; the haze at random phases */
void func_003765B0(u8 *o) {
    VObject *rnd = D_0044E550;

    AT(o, 0x6EC, s32) = 0;
    AT(o, 0x6F0, u8) = 0;
    AT(o, 0x6F1, u8) = 0;
    AT(o, 0x6E8, s32) = (s32)(4096.0f * shaft_rnd(rnd));
    AT(o, 0x618, s64) = -1;
    AT(o, 0x624, s32) = 0;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 0x10;
    AT(o, 0x636, s16) = 0x1A0;
    AT(o, 0x638, s16) = 0x40;
    AT(o, 0x63A, s16) = 0x20;
    AT(o, 0x63C, s16) = 0x20;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, s8) = 0x40;
    AT(o, 0x643, s8) = 1;
    AT(o, 0x644, s8) = 1;
    AT(o, 0x645, s8) = 0x10;
    AT(o, 0x646, s8) = 5;
    AT(o, 0x6E0, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (shaft_rnd(rnd) - 0.5f)) / 180.0f;
    AT(o, 0x6E4, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (shaft_rnd(rnd) - 0.5f)) / 180.0f;
}

#ifdef HG_NATIVE
extern void glr_layer(s32 layer);
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern void glr_haze(f32 phase, f32 sway);

/* the beam's corners: the top edge (0, 60, 82 .. 57), the floor (10, 30, 100 .. 40), middles
 * at z 70 */
static const f32 kShaft[6][4] __attribute__((aligned(16))) = {
    {0.0f, 60.0f, 82.0f, 1.0f}, {0.0f, 60.0f, 57.0f, 1.0f}, {10.0f, 30.0f, 100.0f, 1.0f},
    {10.0f, 30.0f, 40.0f, 1.0f}, {0.0f, 60.0f, 70.0f, 1.0f}, {10.0f, 30.0f, 70.0f, 1.0f},
};

/* +0x14 draw (PC; the PS2 sends GS packets): when all of the beam is in view, the strip
 * top-front, floor-front, top-middle, floor-middle, top-back, floor-back (alpha 0x40 at the
 * edges, 0x80 in the middle, texture group 8 id 0, rows 224 .. 256) twice, scrolled +0x6E8 and
 * (mirrored) a quarter further, in layer 0x19 without depth writes; then the motes. With the
 * haze on: in layer 0x2A the screen at half size is copied, blended half with itself drawn in
 * 33 columns 8 apart each moved down 16 sin(a) (2 + (1 + cos a) / 2) / 16 (a = +0x6E0 + 90
 * degrees a column) and all of it 2 sin(+0x6E4) right; that blended back over the screen at
 * 0x48 / 128 */
void func_00374E50(u8 *o) {
    static const s8 kOrder[6] = {0, 2, 4, 5, 1, 3};
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 xyzw[6][4] __attribute__((aligned(16)));
    f32 st[6][2];
    u8 rgba[6][4];
    u8 *tex = NULL;
    s32 in = 1, i, pass;

    if (AT(o, 0x6F0, u8) == 1) {
        return;
    }
    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    if (TexCache_Resident(8, 0, 0x19, &tex) == -1) {
        in = 0;
    }
    VCALL(D_0044E4B8, 0x48, void (*)(VObject *, f32 (*)[4]))(D_0044E4B8, clip);
    for (i = 0; i < 6; i++) {
        f32 v[4] __attribute__((aligned(16)));

        sceVu0ApplyMatrix(v, clip, kShaft[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            in = 0;
        }
    }
    if (in && tex != NULL && AT(tex, 4, u16) != 0 && AT(tex, 6, u16) != 0) {
        f32 tw = (f32)AT(tex, 4, u16), th = (f32)AT(tex, 6, u16);

        glr_layer(0x19);
        for (pass = 0; pass < 2; pass++) {
            s32 s = AT(o, 0x6E8, s32);

            if (pass == 1 && (s += 0x400) >= 0x1001) {
                s -= 0x1000;
            }
            for (i = 0; i < 6; i++) {
                s32 col = i >> 1;   /* front, middle, back */

                sceVu0CopyVector(xyzw[i], kShaft[kOrder[i]]);
                AT(&xyzw[i][3], 0, u32) = 0;
                if (pass == 1) {
                    col = 2 - col;
                }
                st[i][0] = (f32)(s + 8 + (col == 0 ? 0 : col == 1 ? 0x800 : 0x1000)) / 16.0f / tw;
                st[i][1] = (i & 1 ? 256.5f : 224.5f) / th;
                rgba[i][0] = rgba[i][1] = rgba[i][2] = 0x80;
                rgba[i][3] = (i >> 1) == 1 ? 0x80 : 0x40;
            }
            glr_strip(&clip[0][0], 6, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, 1ull << 34,
                      0x10 | 0x40 | 0x20000 /* GLR_PRIM_NOZW */);
        }
        glr_layer(-1);
    }
    AT(o, 0x620, u8 *) = o + AT(o, 0x6EC, s32) * 0x300 + 0x10;
    func_002E56C0(o + 0x610);
    if (AT(o, 0x6F1, u8) == 1) {
        VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
        VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
        glr_haze(AT(o, 0x6E0, f32), 2.0f * func_0031C248(AT(o, 0x6E4, f32)));
    }
}
#endif

/* room 0x61: byte 3 0 the light shaft (D_0047A370) started with its motes from (30, 0, 70),
 * its slot in event var 3; 1 its haze on, 2 off */
static void shaft_init(void **obj) {
    obj[0] = D_0047A370;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

s32 func_003114C0(void *self, void *a1, u8 *cmd) {
    s32 arg[5] __attribute__((aligned(16)));
    s32 slot;

    switch (cmd[3]) {
    case 0:
        slot = Effect_New(D_0044E578, 0x700, shaft_init);
        AT(&arg[0], 0, f32) = 30.0f;
        AT(&arg[1], 0, f32) = 0.0f;
        AT(&arg[2], 0, f32) = 70.0f;
        AT(&arg[3], 0, f32) = 1.0f;
        arg[4] = 0;
        func_002D6090(D_0044E578, slot, arg);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 3, slot);
        break;
    case 1:
        arg[4] = 1;
        func_002D6090(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 3), arg);
        break;
    case 2:
        arg[4] = 2;
        func_002D6090(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 3), arg);
        break;
    default:
        return 1;
    }
    return 1;
}

/* ---- room 0x61's wanderers: characters 0x14 / 0x15 / 0x16 each follow a path (0x40 bytes:
 * +0x0 / +0xC the box it stays in, +0x18 points, +0x1C frames between two, +0x20 the frame,
 * +0x24 the point it left, +0x28 the points (s16 x 3 in 1/16), +0x30 where it was) - points one
 * to a cell of a grid of 10-unit cells, at random in the cell, shuffled; a cubic curve from
 * each to the next ---- */

extern u8 D_01991210[], D_01991250[], D_01991290[];   /* the three paths */
extern s16 D_019912D0[], D_019913D0[], D_01991490[];   /* their points */
extern f32 func_0031C5C0(f32 x, f32 z);   /* atan2 */

#define PATH_PT(st, i) (AT(st, 0x28, s16 *) + (i) * 3)

/* point i's way on: 20 along the line from the point before to the one after */
void func_00310B20(u8 *st, s32 i, f32 *out) {
    f32 b[4] __attribute__((aligned(16)));
    s32 prev = i - 1, next;

    if (prev < 0) {
        prev = AT(st, 0x18, s32) - 1;
    }
    next = i + 1;
    if (AT(st, 0x18, s32) - 1 < next) {
        next = 0;
    }
    out[0] = 0.0625f * (f32)PATH_PT(st, next)[0];
    out[1] = 0.0625f * (f32)PATH_PT(st, next)[1];
    out[2] = 0.0625f * (f32)PATH_PT(st, next)[2];
    out[3] = 1.0f;
    b[0] = 0.0625f * (f32)PATH_PT(st, prev)[0];
    b[1] = 0.0625f * (f32)PATH_PT(st, prev)[1];
    b[2] = 0.0625f * (f32)PATH_PT(st, prev)[2];
    b[3] = 1.0f;
    sceVu0SubVector(out, out, b);
    sceVu0Normalize(out, out);
    sceVu0ScaleVector(out, out, 20.0f);
}

static void path_point(f32 *out, u8 *st, s32 i) {
    out[0] = 0.0625f * (f32)PATH_PT(st, i)[0];
    out[1] = 0.0625f * (f32)PATH_PT(st, i)[1];
    out[2] = 0.0625f * (f32)PATH_PT(st, i)[2];
    out[3] = 1.0f;
}

/* one frame along the path: `at` moved on the curve (kept in the box); returns the heading
 * `yaw` turned towards the way it moved, by at most 3.6 degrees */
f32 func_00310C90(u8 *st, f32 *at, f32 yaw) {
    static const union { u32 u; f32 f; } kTurn = {0x3D80ADFD}, kTurnN = {0xBD80ADFD};
    f32 c[4][4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 w[4];
    f32 t, u, k3, a;
    s32 next = AT(st, 0x24, s32) + 1, i;

    if (!(next < AT(st, 0x18, s32))) {
        next = 0;
    }
    t = (f32)AT(st, 0x20, s32) / (f32)AT(st, 0x1C, s32);
    u = 1.0f - t;
    k3 = 3.0f * u;
    w[0] = u * (u * u);
    w[1] = t * (k3 * u);
    w[2] = t * (k3 * t);
    w[3] = t * (t * t);
    sceVu0CopyVector((f32 *)(st + 0x30), at);
    path_point(c[0], st, AT(st, 0x24, s32));
    path_point(c[1], st, AT(st, 0x24, s32));
    func_00310B20(st, AT(st, 0x24, s32), d);
    sceVu0AddVector(c[1], c[1], d);
    path_point(c[2], st, next);
    func_00310B20(st, next, d);
    sceVu0SubVector(c[2], c[2], d);
    path_point(c[3], st, next);
    for (i = 0; i < 3; i++) {
        at[i] = 0.0f;
        at[i] = 0.0f + at[i] + w[0] * c[0][i];
        at[i] = 0.0f + at[i] + w[1] * c[1][i];
        at[i] = 0.0f + at[i] + w[2] * c[2][i];
        at[i] = 0.0f + at[i] + w[3] * c[3][i];
    }
    for (i = 0; i < 3; i++) {
        f32 v = at[i], hi = AT(st, 0xC + i * 4, f32), m = v <= hi ? v : hi;

        at[i] = m < AT(st, i * 4, f32) ? AT(st, i * 4, f32) : m;
    }
    AT(st, 0x20, s32) += 1;
    if (!(AT(st, 0x20, s32) < AT(st, 0x1C, s32))) {
        AT(st, 0x20, s32) = 0;
        AT(st, 0x24, s32) += 1;
        if (!(AT(st, 0x24, s32) < AT(st, 0x18, s32))) {
            AT(st, 0x24, s32) = 0;
        }
    }
    sceVu0SubVector(d, at, (f32 *)(st + 0x30));
    a = func_002E2D00(func_0031C5C0(d[0], d[2]) - yaw);
    if (a < kTurnN.f) {
        a = kTurnN.f;
    }
    if (!(a <= kTurn.f)) {
        a = kTurn.f;
    }
    return func_002E2D00(yaw + a);
}

/* set path `st` up: a point at random in each cell of an nx x ny x nz grid of 10-unit cells
 * from (ox, oy, oz) into `pts`, then shuffled (30 random swaps among the first 30 per point),
 * `frames` between two */
void func_00311140(u8 *st, s32 nx, s32 ny, s32 nz, s32 frames, s16 *pts, f32 ox, f32 oy, f32 oz) {
    VObject *rnd;
    s16 *p = pts;
    s32 x, y, z, n;

    rnd = D_0044E550;
    for (x = 0; x < nx; x++) {
        for (y = 0; y < ny; y++) {
            for (z = 0; z < nz; z++) {
                p[0] = (s16)(s32)(16.0f * (0.0f + ox + 10.0f * ((f32)x + VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd))));
                p[1] = (s16)(s32)(16.0f * (0.0f + oy + 10.0f * ((f32)y + VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd))));
                p[2] = (s16)(s32)(16.0f * (0.0f + oz + 10.0f * ((f32)z + VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd))));
                p += 3;
            }
        }
    }
    AT(st, 0x18, s32) = nz * (nx * ny);
    AT(st, 0x28, s16 *) = pts;
    if (AT(st, 0x18, s32) > 0) {
        rnd = D_0044E550;
        for (n = 0; n < AT(st, 0x18, s32); n++) {
            s32 i = (s32)(30.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd));
            s16 *a = pts + i * 3, *b, t;

            b = pts + (s32)(30.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd)) * 3;
            t = a[0], a[0] = b[0], b[0] = t;
            t = a[1], a[1] = b[1], b[1] = t;
            t = a[2], a[2] = b[2], b[2] = t;
        }
    }
    AT(st, 0x20, s32) = 0;
    AT(st, 0x24, s32) = 0;
    AT(st, 0x1C, s32) = frames;
    AT(st, 0x0, f32) = ox;
    AT(st, 0x4, f32) = oy;
    AT(st, 0x8, f32) = oz;
    AT(st, 0xC, f32) = 0.0f + ox + 10.0f * (f32)nx;
    AT(st, 0x10, f32) = 0.0f + oy + 10.0f * (f32)ny;
    AT(st, 0x14, f32) = 0.0f + oz + 10.0f * (f32)nz;
}

/* character `kind` one frame along path `st`, turned to its heading */
static void swim_step(s32 kind, u8 *st) {
    u8 *c = (u8 *)gCharacters[func_001770D0(gProgress, kind) & 0xFF];
    f32 yaw;

#ifdef HG_NATIVE
    if (c == NULL) {   /* absent (the PS2 reads through junk) */
        return;
    }
#endif
    yaw = func_00310C90(st, (f32 *)(c + 0x10), AT(c, 0x54, f32));
    AT(c, 0x54, f32) = yaw;
    sceVu0UnitMatrix((f32 (*)[4])(c + 0x60));
    sceVu0RotMatrixY((f32 (*)[4])(c + 0x60), (f32 (*)[4])(c + 0x60), yaw);
}

extern void *D_0047A3B0[];

static void glint_init(void **obj) {
    obj[0] = D_0047A3B0;
}

/* room 0x61 (byte 3): 0 / 2 / 4 set up the paths of characters 0x14 (2 x 3 x 7 cells from (25,
 * 0, 40), 480 frames a stretch; with the glint D_0047A3B0) / 0x15 (2 x 3 x 5 from (30, 0, 50),
 * 240) / 0x16 (the same box, 280); 1 / 3 / 5 move them a frame (returning 2: again next frame) */
s32 func_003116B0(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_00311140(D_01991210, 2, 3, 7, 0x1E0, D_019912D0, 25.0f, 0.0f, 40.0f);
        Effect_New(D_0044E578, 4, glint_init);
        return 1;
    case 1:
        swim_step(0x14, D_01991210);
        return 2;
    case 2:
        func_00311140(D_01991250, 2, 3, 5, 0xF0, D_019913D0, 30.0f, 0.0f, 50.0f);
        return 1;
    case 3:
        swim_step(0x15, D_01991250);
        return 2;
    case 4:
        func_00311140(D_01991290, 2, 3, 5, 0x118, D_01991490, 30.0f, 0.0f, 50.0f);
        return 1;
    case 5:
        swim_step(0x16, D_01991290);
        return 2;
    }
    return 1;
}

/* ---- class D_0047A3B0 (4 bytes): a glint on character 0x14 (its bone 6) ---- */

extern u32 func_002D6010(u8 *mgr);   /* the effects paused */
extern f32 *func_0017CE80(u8 *skel, s32 bone);   /* a bone's matrix */

/* +0x8 destructor */
void **func_00377B10(void **o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    o[0] = D_0047A3B0;
    o[0] = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0x14 draw: unless the effects are paused, a white 1 x 1 sprite (group 0x10 cell (0x40,
 * 0x20) 32 x 32 of 512 x 256) at the bone, in layer 0x26 */
void func_00377B70(void) {
    struct {
        void **vtbl;
        s32 a;
        u64 tex;
        u8 *rec;
        s32 b, c, d;
        s32 layer;
        s16 s[7];
        s8 k[5];
    } q __attribute__((aligned(8)));
    s32 rec[12] __attribute__((aligned(16)));
    u8 *ch;

    if (func_002D6010(D_0044E578) != 0) {
        return;
    }
    ch = (u8 *)gCharacters[func_001770D0(gProgress, 0x14) & 0xFF];
#ifdef HG_NATIVE
    if (ch == NULL) {   /* absent (the PS2 reads through junk) */
        return;
    }
#endif
    rec[0] = rec[1] = rec[2] = rec[3] = 0x80;
    sceVu0CopyVector((f32 *)&rec[4], func_0017CE80(AT(AT(ch, 0xF0, u8 *), 0x810, u8 *), 6) + 12);
    AT(&rec[8], 0, f32) = 1.0f;
    AT(&rec[9], 0, f32) = 1.0f;
    q.a = -1;
    q.vtbl = D_0046FC30;
    q.k[4] = -1;
    q.tex = -1;
    q.c = 0;
    q.rec = (u8 *)rec;
    q.d = 0;
    q.layer = 0x26;
    q.b = 0;
    q.s[2] = 0x40;
    q.s[0] = 1;
    q.s[5] = 0x200;
    q.s[1] = 0x20;
    q.s[6] = 0x100;
    q.s[3] = 0x20;
    q.k[3] = 0x10;
    q.s[4] = 0x20;
    q.k[0] = 0;
    q.k[1] = 1;
    q.k[2] = 1;
    rec[10] = 0;
    rec[11] = 0;
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

/* +0x10 update */
s32 func_00377CA0(void) {
    return 1;
}

/* +0xC set up */
void func_00377CB0(void) {
}

/* the depth range (effect 0x1C) opening with the cutscene from its frame 1156: 1 / 1 / 40 / 100,
 * the far two on by 1 a frame up to 80 / 140 */
s32 func_002B2BF0(void) {
    u8 *fx = D_0044E4C0;
    f32 t = (f32)(VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 1156);
    f32 r[4] __attribute__((aligned(16)));

    room_effect_new(fx, 0x1C, D_0046EC60);
    r[0] = 1.0f;
    r[1] = 1.0f;
    r[2] = 40.0f + t;
    if (!(r[2] <= 80.0f)) {
        r[2] = 80.0f;
    }
    r[3] = 100.0f + t;
    if (!(r[3] <= 140.0f)) {
        r[3] = 140.0f;
    }
    func_00266C70(fx, 0x1C, r);
    return 1;
}

/* a lit quad at x -43, z -15.12 .. 5.07, height 30.05 / 10.05 */
s32 func_002B2D50(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC22C0000, 0x41F06D5D, 0xC171FD22, 0x3F800000, 0xC22C0000, 0x41F06D5D, 0x40A228F6, 0x3F800000,
        0xC22C0000, 0x4120DABA, 0xC171FD22, 0x3F800000, 0xC22C0000, 0x4120DABA, 0x40A228F6, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x40);
}

extern const char *const D_00403964;   /* "movechair_3" */
extern VObject *D_0044E560;   /* the sound driver */
extern f32 func_0031C4C0(f32 x);   /* asinf */
extern void func_002FF650(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* the creak of the chairs, at (2.09, 0.3, -2.09) */
static void chair_creak(VObject *snd, u32 id, f32 *pos, s32 vol) {
    AT(pos, 0x0, u32) = 0x40058ADB;
    AT(pos, 0x4, u32) = 0x3E99999A;
    AT(pos, 0x8, u32) = 0xC00582AA;
    func_002FF650(snd, id, 6, pos, vol, 0);
}

/* the three rocking chairs ("movechair_1..3"; +0x30 the rock's phase in degrees, +0x34 its
 * size, +0x38 how fast it dies down; +0x10 the tilt), by byte 3: 0 all still; 1 a rocking
 * step (6 degrees; each swing smaller, the first chair creaking at a volume by its size);
 * 2 / 3 set rocking at full size from their tilt now (swinging forward / back), with a creak */
s32 func_002B0640(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
    u8 *chairs[3];
    f32 pos[4] __attribute__((aligned(16)));
    VObject *snd;
    s32 i;

    chairs[0] = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040395C);
    chairs[1] = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00403960);
    chairs[2] = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00403964);
    switch (cmd[3]) {
    case 0:
        for (i = 0; i < 3; i++) {
            if (chairs[i] != NULL) {
                AT(chairs[i], 0x3C, s32) = 0;
                AT(chairs[i], 0x38, s32) = 0;
                AT(chairs[i], 0x34, s32) = 0;
                AT(chairs[i], 0x30, s32) = 0;
            }
        }
        break;
    case 1:
        snd = D_0044E560;
        for (i = 0; i < 3; i++) {
            u8 *c = chairs[i];

            if (c == NULL || AT(c, 0x34, f32) <= 0.0f) {
                continue;
            }
            AT(c, 0x30, f32) = AT(c, 0x30, f32) + 6.0f;
            if (!(AT(c, 0x30, f32) < 360.0f)) {
                AT(c, 0x30, f32) = AT(c, 0x30, f32) - 360.0f;
                AT(c, 0x34, f32) = AT(c, 0x34, f32) - AT(c, 0x38, f32);
                if (AT(c, 0x34, f32) < 0.0f) {
                    AT(c, 0x34, f32) = 0.0f;
                }
                if (i == 0) {
                    chair_creak(snd, 2, pos, (s8)(s32)(-100.0f * (1.0f - AT(c, 0x34, f32))));
                }
            }
            AT(c, 0x10, f32) = 0x1.921fb6p+1f * (10.0f * AT(c, 0x34, f32) * func_0031C248(0x1.921fb6p+1f * AT(c, 0x30, f32) / 180.0f)) / 180.0f;
            if (!(AT(c, 0x10, f32) <= 0x1.921fb6p+1f)) {
                AT(c, 0x10, f32) = AT(c, 0x10, f32) - 0x1.921fb6p+2f;
            }
        }
        break;
    case 2:
    case 3:
        snd = D_0044E560;
        for (i = 0; i < 3; i++) {
            u8 *c = chairs[i];
            f32 a;

            if (c == NULL) {
                continue;
            }
            a = 180.0f * func_0031C4C0(180.0f * AT(c, 0x10, f32) / 0x1.921fb6p+1f / 10.0f) / 0x1.921fb6p+1f;
            AT(c, 0x30, f32) = cmd[3] == 2 ? a : 180.0f - a;
            AT(c, 0x34, f32) = 1.0f;
            AT(c, 0x38, u32) = 0x3D4CCCCD;   /* 0.05 */
            AT(c, 0x3C, s32) = 0;
            chair_creak(snd, 5, pos, 0);
        }
        break;
    }
    return 1;
}

extern void *D_00472390[];   /* a 0xC0-byte effect */

/* two hanging things (the room's +0x34 (4, 5) objects, half a swing apart: +0x34 60 x i
 * degrees), by byte 3: 0 at rest; 1 Fiona's movement pushes them (her step's length) - past 5
 * they swing for 20 frames, and the first toggles event flag 3 with a sound (Fiona's 1 / 2) -
 * and a swing step (+0x30 on by 36 degrees, the tilt +0x10 (1 + sin) degrees in radians) */
static inline __attribute__((always_inline)) s32 hangers_swing(VObject *self, u8 *cmd, s32 base, s32 n, f32 push,
                                                              s32 root, s32 flag, s32 sndOn, s32 sndOff) {
    VObject *objs = D_00456DF8, *ev = D_0044E4D0;
    f32 d[4] __attribute__((aligned(16)));
    s32 i;

    for (i = 0; i < n; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, s32))(objs, VCALL(self, 0x34, s32 (*)(VObject *, s32))(self, i + base));

        if (o == NULL) {
            continue;
        }
        switch (cmd[3]) {
        case 0:
            AT(o, 0x30, f32) = 0.0f;
            AT(o, 0x34, f32) = 60.0f * (f32)i;
            AT(o, 0x38, f32) = 0.0f;
            AT(o, 0x3C, f32) = 0.0f;
            break;
        case 1:
            if (gCharPlayer != NULL) {
                sceVu0CopyVector(d, gCharPlayer->a.prevPos);
                sceVu0SubVector(d, gCharPlayer->a.pos, d);
                if (root) {
                    AT(o, 0x3C, f32) = AT(o, 0x3C, f32) + hook_sqrt(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
                } else {
                    AT(o, 0x3C, f32) = AT(o, 0x3C, f32) + (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
                }
                if (!(AT(o, 0x3C, f32) <= push)) {
                    AT(o, 0x38, f32) = 20.0f;
                    AT(o, 0x3C, f32) = 0.0f;
                    if (i == 0) {
                        if ((VCALL(ev, 0x58, u32 (*)(VObject *, s32))(ev, flag) & 0xFF) == 1) {
                            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, flag);
                            func_00122C20(&gCharPlayer->a, sndOn, 6, 0, 0, NULL);
                        } else {
                            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, flag);
                            func_00122C20(&gCharPlayer->a, sndOff, 6, 0, 0, NULL);
                        }
                    }
                }
            }
            if (!(AT(o, 0x38, f32) <= 0.0f)) {
                f32 t;

                AT(o, 0x38, f32) = AT(o, 0x38, f32) - 1.0f;
                if (AT(o, 0x38, f32) < 0.0f) {
                    AT(o, 0x38, f32) = 0.0f;
                }
                AT(o, 0x30, f32) = AT(o, 0x30, f32) + 36.0f;
                if (!(AT(o, 0x30, f32) + AT(o, 0x34, f32) < 360.0f)) {
                    AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
                }
                t = 0x1.921fb6p+1f * (1.0f + func_0031C248(0x1.921fb6p+1f * (AT(o, 0x30, f32) + AT(o, 0x34, f32)) / 180.0f)) / 180.0f;
                AT(o, 0x10, f32) = t;
                if (!(t <= 0x1.921fb6p+1f)) {
                    AT(o, 0x10, f32) = t - 0x1.921fb6p+2f;
                }
            }
            break;
        }
    }
    return 1;
}

s32 func_002B1A00(VObject *self, void *a1, u8 *cmd) {
    return hangers_swing(self, cmd, 4, 2, 5.0f, 1, 3, 1, 2);
}

static void effect_c0b_init(void **obj) {
    obj[0] = D_00472390;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

/* something dropped (effect D_00472390, its slot in event var 1) from (-276.5, 3, 160), by
 * byte 3: 0 started (event var 0 the frame count); 1 a frame (2 while falling): it drifts 0.5
 * a frame in x and falls 0.05 x n(n+1)/2, gone below -10 */
s32 func_002B1D50(void *self, void *a1, u8 *cmd) {
    VObject *ev = D_0044E4D0;
    f32 p[4] __attribute__((aligned(16)));

    switch (cmd[3]) {
    case 0: {
        u8 *mgr;
        s32 slot;

        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, 0);
        mgr = D_0044E578;
        slot = Effect_New(mgr, 0xC0, effect_c0b_init);
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, slot);
        p[1] = 3.0f;
        p[0] = -276.5f;
        p[2] = 160.0f;
        p[3] = 1.0f;
        func_002D6090(mgr, slot, p);
        break;
    }
    case 1: {
        u32 n = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 0) + 1;
        s32 slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);

        p[0] = -276.5f + 0.5f * (f32)n;
        p[2] = 160.0f;
        p[3] = 1.0f;
        p[1] = 3.0f - 0x1.99999ap-5f /* 0.05 */ * (f32)((n * (n + 1)) >> 1);
        if (!(p[1] <= -10.0f)) {
            func_002D6090(D_0044E578, slot, p);
            VCALL(ev, 0x30, void (*)(VObject *, s32, u32))(ev, 0, n);
            return 2;
        }
        func_002D6090(D_0044E578, slot, NULL);
        break;
    }
    }
    return 1;
}

/* a hanging thing (the room's +0x34 (byte 3 + 2) object) swinging, by byte 4: 0 / 2 set
 * going (12 degrees) away from the partner / Fiona; 1 a step (22.5 degrees of its swing,
 * shrinking to 0.4 at each end; under half a degree it stops) - 2 while it swings */
s32 func_002B2050(VObject *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, s32))(
        D_00456DF8, VCALL(self, 0x34, s32 (*)(VObject *, s32))(self, cmd[3] + 2));
    f32 d[4] __attribute__((aligned(16)));
    f32 a;

    if (o == NULL) {
        return 1;
    }
    switch (cmd[4]) {
    case 0:
    case 2:
        AT(o, 0x4, s32) = 0;
        AT(o, 0x30, f32) = 0.0f;
        AT(o, 0x34, f32) = 0x1.aceeap-3f /* 12 degrees */;
        sceVu0SubVector(d, (f32 *)(o + 0x20), cmd[4] == 0 ? gCharPartner->a.pos : gCharPlayer->a.pos);
        d[1] = 0.0f;
        sceVu0Normalize(d, d);
        AT(o, 0x38, f32) = d[2];
        AT(o, 0x3C, f32) = -d[0];
        break;
    case 1:
        if (AT(o, 0x4, s32) == 0) {
            AT(o, 0x30, f32) = a = AT(o, 0x30, f32) + 0x1.921fb6p-2f /* 22.5 degrees */;
            if (a <= 0.0f) {
                a = -a;
            }
            if (a < 0x1.1df46ap-6f /* 1 degree */) {
                AT(o, 0x34, f32) = AT(o, 0x34, f32) * 0x1.99999ap-2f /* 0.4 */;
            }
            if (!(AT(o, 0x30, f32) <= 0x1.921fb6p+0f)) {
                AT(o, 0x4, s32) = 1;
            }
        } else {
            AT(o, 0x30, f32) = a = AT(o, 0x30, f32) - 0x1.921fb6p-2f;
            if (a <= 0.0f) {
                a = -a;
            }
            if (a < 0x1.1df46ap-6f) {
                AT(o, 0x34, f32) = AT(o, 0x34, f32) * 0x1.99999ap-2f;
            }
            if (AT(o, 0x30, f32) < -0x1.921fb6p+0f) {
                AT(o, 0x4, s32) = 0;
            }
        }
        if (AT(o, 0x34, f32) <= 0x1.1df46ap-7f /* half a degree */) {
            AT(o, 0x10, f32) = 0.0f;
            AT(o, 0x18, f32) = 0.0f;
            break;
        }
        AT(o, 0x10, f32) = AT(o, 0x38, f32) * (AT(o, 0x34, f32) * func_0031C248(AT(o, 0x30, f32)));
        AT(o, 0x18, f32) = AT(o, 0x3C, f32) * (AT(o, 0x34, f32) * func_0031C248(AT(o, 0x30, f32)));
        return 2;
    }
    return 1;
}

/* the lattice ("kousi"): swung open (-90 degrees) while the hook's flag byte is set, shut otherwise */
extern const char *const D_003F99B8[];   /* { "kousi" } */
s32 func_002AC600(void *a0, void *a1, u8 *arg) {
    u8 *kousi = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F99B8[0]);

    if (kousi != NULL) {
        AT(kousi, 0x14, f32) = arg[3] == 0 ? 0.0f : -0x1.921fb6p+0f;
    }
    return 1;
}

extern VObject *D_0044E988;   /* the item manager */
extern VObject *D_0044E560;   /* the sound driver */
extern s32 func_001788F0(Progress *p, u32 door);

/* door be16 cmd[3..4]: func_001788F0 */
s32 func_002AA5B0(void *self, void *a1, u8 *cmd) {
    return func_001788F0(gProgress, (cmd[3] << 8 | cmd[4]) & 0xFFFF);
}

/* (not with progress flag 0xAF) with flag `flag` set and item 0x238 held, a sound (0xC, 5) */
static inline s32 item238_sound(u32 bit) {
    Progress *p = gProgress;

    if (!(AT(p, 0x30, u32) & 0x8000) && (AT(p, 0xE4, u32) & bit) &&
        VCALL(D_0044E988, 0xC, s32 (*)(VObject *, s32))(D_0044E988, 0x238) != 0) {
        VCALL(D_0044E560, 0x14, void (*)(VObject *, u32, u32))(D_0044E560, 0xC, 5);
    }
    return 1;
}

s32 func_002B47C0(void) {   /* progress flag 0x651 */
    return item238_sound(0x20000);
}

s32 func_002B4970(void) {   /* progress flag 0x650 */
    return item238_sound(0x10000);
}

extern void *D_0046F5F0[], *D_0046EA40[], *D_00476BF0[];

s32 func_002AD550(void) {   /* room effect 0 (D_0046F5F0) */
    room_effect_new(D_0044E4C0, 0, D_0046F5F0);
    return 1;
}

s32 func_002AF730(void) {   /* room effect 1 (D_0046EA40) */
    room_effect_new(D_0044E4C0, 1, D_0046EA40);
    return 1;
}

static inline void effect476bf0_init(void **o) {
    o[0] = D_00476BF0;
}

s32 func_002B4BE0(void) {   /* a scene effect (D_00476BF0, 0x6E0 bytes) */
    Effect_New(D_0044E578, 0x6E0, effect476bf0_init);
    return 1;
}

extern const char *D_003F0404, *D_003F6F64;   /* room object names */
extern void *D_0046F5A0[], *D_00469D00[], *D_0046FC30[];

/* the room object D_003F0404's +0x24 by byte 3: 0 set (37.978 with progress flag 0x12, else
 * 11), 1 up 0.25 to 37.978 (then event +0x5C (2)), 2 up 0.25, else down 0.25 */
s32 func_002A94C0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kTop = {0x4217E979};   /* 37.978 */
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F0404);

    if (o == NULL) {
        return 1;
    }
    switch (cmd[3]) {
    case 0:
        AT(o, 0x24, f32) = (AT(gProgress, 0x1C, u32) & 0x40000) ? kTop.f : 11.0f;
        break;
    case 1:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + 0.25f;
        if (!(AT(o, 0x24, f32) < kTop.f)) {
            VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 2);
            AT(o, 0x24, f32) = kTop.f;
        }
        break;
    case 2:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + 0.25f;
        break;
    default:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) - 0.25f;
        break;
    }
    return 1;
}

/* the room object D_003F6F64's +0x24 toward 1 (event variable 0 unset) or 0 (set): byte 3 0 at
 * once, else by 0.2 a step */
static inline s32 var_fade(const char *name, s32 var, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3E4CCCCD};   /* 0.2 */
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);

    if (o == NULL) {
        return 1;
    }
    if (cmd[3] == 0) {
        if (VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, var) == 0) {
            AT(o, 0x24, f32) = 1.0f;
        } else {
            AT(o, 0x24, f32) = 0.0f;
        }
    } else if (VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, var) == 0) {
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + kStep.f;
        if (!(AT(o, 0x24, f32) <= 1.0f)) {
            AT(o, 0x24, f32) = 1.0f;
        }
    } else {
        AT(o, 0x24, f32) = AT(o, 0x24, f32) - kStep.f;
        if (AT(o, 0x24, f32) < 0.0f) {
            AT(o, 0x24, f32) = 0.0f;
        }
    }
    return 1;
}

s32 func_002ABDD0(void *self, void *a1, u8 *cmd) {
    return var_fade(D_003F6F64, 0, cmd);
}

static inline void smoke_init(void **o) {
    o[0] = D_0046F5A0;
    o[0x1810 / 4] = D_00469D00;
    ((s32 *)o)[0x1814 / 4] = -1;
    o[0x1810 / 4] = D_0046FC30;
}

s32 func_002AFE90(void) {   /* the rising smoke (D_0046F5A0, 0x1C60 bytes) */
    Effect_New(D_0044E578, 0x1C60, smoke_init);
    return 1;
}

extern void *D_00476BD0[];
extern const char *D_003FF110[], *D_0040AC10[];   /* room object names */
extern void func_002FF650(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

static inline void effect476bd0_init(void **o) {
    o[0] = D_00476BD0;
    o[0x3040 / 4] = D_00469D00;
    ((s32 *)o)[0x3044 / 4] = -1;
    o[0x3040 / 4] = D_0046FC30;
    o[0x3078 / 4] = D_00469D00;
    ((s32 *)o)[0x307C / 4] = -1;
    o[0x3078 / 4] = D_0046FC30;
}

/* byte 3 0: a D_00476BD0 effect (0x36C0 bytes) spawned, its slot kept in event var 0; else that
 * slot's effect removed */
s32 func_002B4310(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(D_0044E578, 0x36C0, effect476bd0_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, slot);
    } else {
        func_002D6170(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0));
    }
    return 1;
}

/* a pendulum (room object D_003FF110[byte 3]): byte 4 0 still; 1 its phase +0x30 on by 2
 * degrees (a tick sound at (-85, 30, 90) each turn), swinging 15 degrees (+0x14) */
s32 func_002AED60(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    u32 mode = cmd[4];
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003FF110[cmd[3]]);

    if (mode == 0) {
        AT(o, 0x30, f32) = 0.0f;
    } else if (mode == 1) {
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + 2.0f;
        if (!(AT(o, 0x30, f32) < 360.0f)) {
            f32 at[4] __attribute__((aligned(16)));

            AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
            at[0] = -85.0f;
            at[1] = 30.0f;
            at[2] = 90.0f;
            func_002FF650(D_0044E560, 0x40000001, 6, at, 0, 0);
        }
        AT(o, 0x14, f32) = kPi.f * (15.0f * func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
        if (!(AT(o, 0x14, f32) <= kPi.f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kTwoPi.f;
        }
    }
    return 1;
}

/* four room objects (D_0040AC10) pressed in (+0x24 down 0.2 to -0.7, a sound as each starts) while
 * event flag i is set, else back up 0.2 to 0; byte 3 0 all reset */
s32 func_002B37D0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3E4CCCCD}, kLow = {0xBF333333};
    VObject *objs = D_00456DF8, *ev = D_0044E4D0, *snd = D_0044E560;
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040AC10[i]);

        if (o == NULL) {
            continue;
        }
        if (cmd[3] == 0) {
            AT(o, 0x24, f32) = 0.0f;
        } else if (VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, i & 0xFF) != 0) {
            if (AT(o, 0x24, f32) == 0.0f) {
                f32 at[4] __attribute__((aligned(16)));

                sceVu0CopyVector(at, (f32 *)(o + 0x20));
                func_002FF650(snd, 6, 6, at, 0, 0);
            }
            AT(o, 0x24, f32) = AT(o, 0x24, f32) - kStep.f;
            if (AT(o, 0x24, f32) < kLow.f) {
                AT(o, 0x24, f32) = kLow.f;
            }
        } else {
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + kStep.f;
            if (!(AT(o, 0x24, f32) <= 0.0f)) {
                AT(o, 0x24, f32) = 0.0f;
            }
        }
    }
    return 1;
}

extern const char *D_0047AAB8[], *D_00409940, *D_0040B508, *D_003FCB00[];   /* room object names */

/* two wheels (D_0047AAB8) rocking 4 degrees (+0x18) through their phase +0x30, 6 degrees a step
 * (byte 3 1; 0 reset), the first one's creak (-366, 30, -25) at each turn */
s32 func_002AE1B0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *objs = D_00456DF8, *snd = D_0044E560;
    f32 at[4] __attribute__((aligned(16)));
    s32 i;

    at[0] = -366.0f;
    at[1] = 30.0f;
    at[2] = -25.0f;
    for (i = 0; i < 2; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0047AAB8[i]);

        if (o == NULL) {
            continue;
        }
        switch (cmd[3]) {
        case 0:
            AT(o, 0x30, f32) = 0.0f;
            AT(o, 0x18, f32) = 0.0f;
            if (i == 0) {
                func_002FF650(snd, 0, 6, at, 0, 0);
            }
            break;
        case 1:
            AT(o, 0x30, f32) = AT(o, 0x30, f32) + 6.0f;
            if (!(AT(o, 0x30, f32) < 360.0f)) {
                if (i == 0) {
                    func_002FF650(snd, 0, 6, at, 0, 0);
                }
                AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
            }
            AT(o, 0x18, f32) = kPi.f * (4.0f * func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
            break;
        }
    }
    return 1;
}

/* room object `name`'s animation by event var 0 (12..): byte 3 0 forward (+0x74) to frame
 * (var - 12) / 18, 1 back (+0x78) to (var - 12) / 16, 2 / 3 back at 0 / 1; +0x7C kept 0..1 */
static inline __attribute__((always_inline)) s32 var0_anim(u8 *cmd, const char *name) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    u32 v;

    if (o == NULL) {
        return 1;
    }
    v = VCALL(D_0044E4D0, 0x34, u32 (*)(VObject *, s32))(D_0044E4D0, 0);
    switch (cmd[3]) {
    case 0:
        if (v < 0xC) {
            v = 0xC;
        }
        if (v >= 0x1F) {
            v = 0x1E;
        }
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(v - 0xC) / 18.0f;
        break;
    case 1:
        if (v < 0xC) {
            v = 0xC;
        }
        if (v >= 0x1D) {
            v = 0x1C;
        }
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(v - 0xC) / 16.0f;
        break;
    case 2:
    case 3:
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(cmd[3] - 2);
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

s32 func_002B31A0(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, D_00409940);
}

s32 func_002B3B70(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, D_0040B508);
}

/* five pendulums (D_003FCB00[byte 3]) of their own periods and swings: byte 4 0 still at a phase
 * offset (+0x34) 60 x the index, 1 swinging on (+0x30, +0x14) */
s32 func_002AD7B0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003FCB00[cmd[3]]);
    f32 period = 360.0f, swing = 15.0f;

    switch (cmd[3]) {
    case 2:
        break;
    case 0:
        period = 180.0f;
        swing = 5.0f;
        break;
    case 1:
        period = 210.0f;
        swing = 5.0f;
        break;
    case 3:
        period = 180.0f;
        swing = 10.0f;
        break;
    case 4:
        period = 240.0f;
        break;
    }
    switch (cmd[4]) {
    case 0:
        AT(o, 0x30, f32) = 0.0f;
        AT(o, 0x34, f32) = 60.0f * (f32)(u32)cmd[3];
        break;
    case 1:
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + 360.0f / period;
        if (!(AT(o, 0x30, f32) + AT(o, 0x34, f32) < 360.0f)) {
            AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
        }
        AT(o, 0x14, f32) = kPi.f * (swing * func_0031C248(kPi.f * (AT(o, 0x30, f32) + AT(o, 0x34, f32)) / 180.0f)) / 180.0f;
        if (!(AT(o, 0x14, f32) <= kPi.f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kTwoPi.f;
        }
        break;
    }
    return 1;
}

extern const char *D_003F43A0, *D_003FA760, *D_003F17B8, *D_003F17C8, *D_003F0DC4, *D_003FA078;   /* room object names */
extern u32 D_0047E36C;   /* menu buttons pressed this frame (MENU_*) */
extern u32 D_0047E364;   /* menu buttons, repeating */
extern VObject *D_0044E4F8;   /* the camera director's interface */
extern VObject *D_0044E550;   /* random numbers */

/* a dial `o` on progress var `var` (0..6, 30 degrees each, from `off`): byte 3 of `step` 0 set
 * to it (`hide` also clears its +0), 1 turned by left / right (event +0x60 1 when changed, 0 when
 * confirmed / cancelled), 2 turning to it a degree a step (event +0x5C 1 there), 3 wait (2) */
static inline __attribute__((always_inline)) s32 dial_step(u32 step, u8 *o, u32 var, s32 off, s32 hide) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kDeg = {0x3C8EFA35};

    switch (step) {
    case 0:
        if (hide) {
            AT(o, 0x0, u8) = 0;
        }
        AT(o, 0x14, f32) = kPi.f * (f32)((s32)(Progress_GetVar(gProgress, var) & 0xFF) * 30 + off) / 180.0f;
        break;
    case 1:
        if (((D_0047E36C >> 4) & 1) | ((D_0047E36C >> 5) & 1)) {
            VCALL(D_0044E4D0, 0x60, void (*)(VObject *, s32))(D_0044E4D0, 0);
        } else {
            Progress *p = gProgress;
            u8 v = Progress_GetVar(p, var);

            if ((D_0047E364 >> 3) & 1) {
                if (v != 0) {
                    v = v - 1;
                }
            } else if (((D_0047E364 >> 1) & 1) && v < 6) {
                v = v + 1;
            }
            if (v != (u8)Progress_GetVar(p, var)) {
                AT(p, 0x9C + var, u8) = v;
                VCALL(D_0044E4D0, 0x60, void (*)(VObject *, s32))(D_0044E4D0, 1);
            }
        }
        break;
    case 2: {
        f32 deg = 180.0f * AT(o, 0x14, f32) / kPi.f;
        f32 d = deg - (f32)((s32)(Progress_GetVar(gProgress, var) & 0xFF) * 30 + off);

        if (!(d <= 1.0f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kDeg.f;
        } else if (d < -1.0f) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) + kDeg.f;
        } else {
            VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 1);
        }
        break;
    }
    case 3:
        return 2;
    }
    return 1;
}

/* the dial D_003F43A0 on progress var 3 */
s32 func_002AAD60(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F43A0);

    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[3], o, 3, 0, 0);
}

/* two dials (byte 3: D_003F17B8 on var 0, D_003F17C8 on var 1, from -90 degrees), byte 4 the step */
s32 func_002A9F30(void *self, void *a1, u8 *cmd) {
    u8 *o;
    u32 var;

    if (cmd[3] == 0) {
        o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F17B8);
        var = 0;
    } else {
        o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F17C8);
        var = 1;
    }
    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[4], o, var & 0xFF, -90, 1);
}

/* a lid (D_003FA760, +0x24 its height, +0x34 its speed): byte 3 0 up, 1 shut; 2 falling and
 * bouncing shut (the first landing clears progress flag 0x50 and, unless the director says no,
 * thuds), 2 while moving; 3 a random rattle up, 2 while it stays below */
s32 func_002ACD50(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kShut = {0xC1CA6666}, k01 = {0x3DCCCCCD}, kBounce = {0xBE4CCCCD},
        kStill = {0x3CA3D70A}, k04 = {0x3ECCCCCD};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003FA760);
    f32 v;

    if (o == NULL) {
        return 1;
    }
    switch (cmd[3]) {
    case 0:
        AT(o, 0x24, f32) = 0.0f;
        AT(o, 0x30, f32) = 1.0f;
        AT(o, 0x34, f32) = 0.0f;
        return 1;
    case 1:
        AT(o, 0x24, f32) = kShut.f;
        AT(o, 0x34, f32) = 0.0f;
        return 1;
    case 2:
        AT(o, 0x34, f32) = AT(o, 0x34, f32) - k01.f;
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + AT(o, 0x34, f32);
        if (AT(o, 0x24, f32) < kShut.f) {
            if (!(AT(o, 0x30, f32) <= 0.0f)) {
                Progress *p = gProgress;

                AT(o, 0x30, f32) = -1.0f;
                AT(p, 0x7C, u32) &= 0xFFFEFFFF;
                if ((u8)VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *, Progress *))(D_0044E4F8, p) == 0) {
                    f32 at[4] __attribute__((aligned(16)));

                    at[1] = 30.0f;
                    at[2] = 30.0f;
                    at[0] = 0.0f;
                    func_002FF650(D_0044E560, 2, 6, at, 0, 0);
                }
            }
            AT(o, 0x24, f32) = kShut.f;
            AT(o, 0x34, f32) = AT(o, 0x34, f32) * kBounce.f;
        }
        v = AT(o, 0x34, f32);
        if (v <= 0.0f) {
            v = -v;
        }
        if (v < kStill.f) {
            AT(o, 0x24, f32) = kShut.f;
            return 1;
        }
        return 2;
    case 3:
        if (AT(o, 0x34, f32) < 0.0f) {
            AT(o, 0x34, f32) = 1.0f;
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + k04.f;
        } else {
            AT(o, 0x34, f32) = -1.0f;
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + k01.f;
        }
        v = (0.0f + AT(o, 0x24, f32)) + k01.f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
        AT(o, 0x24, f32) = v;
        if (v <= 0.0f) {
            return 2;
        }
        AT(o, 0x24, f32) = 0.0f;
        return 1;
    }
    return 1;
}

extern s32 func_00178980(Progress *p, s32 room, s32 exit);   /* the door at that exit is open */

/* Fiona in move 5, room 0x1D's flag 0 not set and its exit 0's door shut */
s32 func_002ADE30(void) {
    Progress *p;

    if (AT(gCharPlayer, 0xFC, s32) != 5) {
        return 0;
    }
    p = gProgress;
    if (Progress_CurRoomFlag(p, 0x1D, 0) != 0 || func_00178980(p, 0x1D, 0) != 0) {
        return 0;
    }
    return 1;
}

extern void *D_0046FF20[];

static inline void dust_init(void **o) {
    o[0] = D_0046FF20;
    o[0x610 / 4] = D_00469D00;
    ((s32 *)o)[0x614 / 4] = -1;
    o[0x610 / 4] = D_0046FC30;
}

/* a lever (D_003F0DC4, tilt +0x18 between -10 and 0 degrees): byte 3 0 back 2 degrees, 1 pulled
 * (-10) with a puff of grey dust at it */
s32 func_002A9740(void *self, void *a1, u8 *cmd) {
    /* (volatile: a compile-time fold of the pulled case would round as IEEE, not as the EE) */
    static const volatile union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F0DC4);
    s32 dust = 0;
    f32 deg;

    if (o == NULL) {
        return 1;
    }
    deg = 180.0f * AT(o, 0x18, f32) / kPi.f;
    switch (cmd[3]) {
    case 0:
        deg += 2.0f;
        break;
    case 1:
        dust = 1;
        deg = -10.0f;
        break;
    }
    if (!(deg <= 0.0f)) {
        deg = 0.0f;
    }
    if (deg < -10.0f) {
        deg = -10.0f;
    }
    AT(o, 0x18, f32) = kPi.f * deg / 180.0f;
    if (dust != 0) {
        u8 *mgr = D_0044E578;
        s32 slot = Effect_New(mgr, 0x720, dust_init);
        struct {
            f32 pos[4];
            s32 kind, r, g, b, size;
        } dp __attribute__((aligned(16)));

        dp.pos[0] = AT(o, 0x20, f32);
        dp.pos[1] = AT(o, 0x24, f32);
        dp.pos[2] = AT(o, 0x28, f32);
        dp.pos[3] = 1.0f;
        dp.b = 0x80;
        dp.g = 0x80;
        dp.r = 0x80;
        dp.kind = 0;
        func_002D6090(mgr, slot, &dp);
    }
    return 1;
}

/* frame (v - lo) / div (v kept in lo..hi) forward (fwd) or back; Fiona's sound 0 as it starts */
static inline __attribute__((always_inline)) void var0_frame(u8 *o, u32 v, u32 lo, u32 hi, f32 div, s32 fwd) {
    if (v == lo) {
        func_00122C20(&gCharPlayer->a, 0, 6, 0, 0, NULL);
    }
    if (v < lo) {
        v = lo;
    }
    if (v > hi) {
        v = hi;
    }
    AT(o, 0x74, s32) = fwd;
    AT(o, 0x78, s32) = !fwd;
    AT(o, 0x7C, f32) = (f32)(v - lo) / div;
}

/* the room object D_003FA078's animation by event var 0 (byte 3 picks the range: 0 back over
 * 43..54, 1 / 2 / 5 forward over 12..21, 16..28, 11..18; 3 / 4 back at 0 / 1), +0x7C kept 0..1 */
static inline s32 var0_obj_anim(const char *name, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    u32 v;

    if (o == NULL) {
        return 1;
    }
    v = VCALL(D_0044E4D0, 0x34, u32 (*)(VObject *, s32))(D_0044E4D0, 0);
    switch (cmd[3]) {
    case 0:
        var0_frame(o, v, 0x2B, 0x36, 11.0f, 0);
        break;
    case 1:
        var0_frame(o, v, 0xC, 0x15, 9.0f, 1);
        break;
    case 2:
        var0_frame(o, v, 0x10, 0x1C, 12.0f, 1);
        break;
    case 3:
    case 4:
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(cmd[3] - 3);
        break;
    case 5:
        var0_frame(o, v, 0xB, 0x12, 7.0f, 1);
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

s32 func_002AC790(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(D_003FA078, cmd);
}

extern const char *D_00410F38[];   /* the three dials' object names */
extern u8 D_0047B254[3];           /* their progress vars */
extern f32 func_002E2D00(f32 angle);   /* wrapped into -pi..pi */

/* the three dials (D_00410F38, progress vars D_0047B254: 0..3, 90 degrees each), event var 0 the
 * one picked: byte 3 0 set (the original sets the picked one three times), 1 up / down picks
 * one (event var 0), left / right turns it (event +0x5C 3), cancel leaves (+0x60 2); 2 turning
 * to it 4 degrees a step, and there: solved at 1 / 0 / 2 (+0x60 2, +0x5C 4), else +0x60 3; 3
 * wait */
s32 func_002B53B0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kStep = {0x3D8EFA35};
    VObject *ev = D_0044E4D0;
    u32 sel = (u8)VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
    VObject *objs = D_00456DF8;
    const char **name = &D_00410F38[sel];
    u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, *name);
    s32 i;

    switch (cmd[3]) {
    case 0: {
        Progress *p = gProgress;

        for (i = 0; i < 3; i++) {
            u8 *d = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, *name);

            if (d != NULL) {
                u32 v = (u8)Progress_GetVar(p, D_0047B254[i]);

                AT(d, 0x14, f32) = func_002E2D00(kPi.f * (f32)(s32)(v * 90) / 180.0f);
            }
        }
        return 1;
    }
    case 1:
        if ((D_0047E36C >> 5) & 1) {
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 2);
            return 1;
        }
        if (D_0047E36C & 1) {
            sel = sel != 0 ? (sel - 1) & 0xFF : 2;
        } else if ((D_0047E36C >> 2) & 1) {
            sel = sel < 2 ? (sel + 1) & 0xFF : 0;
        }
        if (sel == (u32)VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0)) {
            u8 *var = &D_0047B254[sel];
            Progress *p = gProgress;
            u32 v = (u8)Progress_GetVar(p, *var);

            if ((D_0047E364 >> 3) & 1) {
                v = v != 0 ? (v - 1) & 0xFF : 3;
            } else if ((D_0047E364 >> 1) & 1) {
                v = v < 3 ? (v + 1) & 0xFF : 0;
            }
            if (v != (u8)Progress_GetVar(p, *var)) {
                AT(p, 0x9C + *var, u8) = v;
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
            }
        } else {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, sel);
        }
        return 1;
    case 2: {
        u8 *var = &D_0047B254[sel];
        Progress *p = gProgress;
        u32 v = (u8)Progress_GetVar(p, *var);
        f32 d = func_002E2D00(AT(o, 0x14, f32) - kPi.f * (f32)(s32)(v * 90) / 180.0f);

        if (!(d <= kStep.f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kStep.f;
        } else if (d < -kStep.f) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) + kStep.f;
        } else {
            Progress *q = gProgress;

            v = (u8)Progress_GetVar(q, *var);
            AT(o, 0x14, f32) = func_002E2D00(kPi.f * (f32)(s32)(v * 90) / 180.0f);
            if ((u8)Progress_GetVar(q, D_0047B254[0]) == 1 && (u8)Progress_GetVar(p, D_0047B254[1]) == 0 &&
                (u8)Progress_GetVar(p, D_0047B254[2]) == 2) {
                VObject *e = D_0044E4D0;

                VCALL(e, 0x60, void (*)(VObject *, s32))(e, 2);
                VCALL(e, 0x5C, void (*)(VObject *, s32))(e, 4);
            } else {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 3);
            }
        }
        return 1;
    }
    case 3:
        return 2;
    }
    return 1;
}

extern void *D_0044E570;   /* nav mesh */
/* room 54 step (D_00428040): find the nav mesh's door regions again */
s32 func_0030FA60(void) {
    VObject *nav = (VObject *)D_0044E570;

    VCALL(nav, 0x4C, void (*)(VObject *))(nav);
    return 1;
}

extern void *D_00477E10[], *D_00479400[];

static void effect_77E10_init(void **obj) {
    obj[0] = D_00477E10;
}

static void effect_79400_init(void **obj) {
    obj[0] = D_00479400;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* room 55 (D_004210F8): the 0x18-byte effect D_00477E10 started with 0 or 1 by byte 3; 0 also
 * lays a floor quad (effect 0x1B: 60 x 60 at height 45), else the 0x6D0-byte effect D_00479400
 * is made too */
s32 func_00305FB0(void *self, void *a1, u8 *cmd) {
    u8 *mgr = D_0044E578;
    s32 slot = Effect_New(mgr, 0x18, effect_77E10_init);
    s32 on;

    if (cmd[3] == 0) {
        u8 *fx;
        u32 q[20] __attribute__((aligned(16)));

        on = 0;
        fx = D_0044E4C0;
        room_effect_new(fx, 0x1B, D_00472F60);
        q[0] = 0x41F00000;   /* (30, 45, -30) */
        q[1] = 0x42340000;
        q[2] = 0xC1F00000;
        q[3] = 0x3F800000;
        q[4] = 0xC1F00000;   /* (-30, 45, -30) */
        q[5] = 0x42340000;
        q[6] = 0xC1F00000;
        q[7] = 0x3F800000;
        q[8] = 0x41F00000;   /* (30, 45, 30) */
        q[9] = 0x42340000;
        q[10] = 0x41F00000;
        q[11] = 0x3F800000;
        q[12] = 0xC1F00000;  /* (-30, 45, 30) */
        q[13] = 0x42340000;
        q[14] = 0x41F00000;
        q[15] = 0x3F800000;
        q[16] = 0;
        q[17] = 0x80;
        q[18] = 0x3F800000;
        func_00266C70(fx, 0x1B, q);
    } else {
        on = 1;
        Effect_New(mgr, 0x6D0, effect_79400_init);
    }
    func_002D6090(mgr, slot, &on);
    return 1;
}

extern void *D_00477AC0[];

static void effect_77AC0_init(void **obj) {
    obj[0] = D_00477AC0;
    obj[0x1840 / 4] = D_00469D00;
    ((s32 *)obj)[0x1844 / 4] = -1;
    obj[0x1840 / 4] = D_0046FC30;
    obj[0x1878 / 4] = D_00469D00;
    ((s32 *)obj)[0x187C / 4] = -1;
    obj[0x1878 / 4] = D_0046FC30;
}

/* room 66 (D_0041F568): byte 4 0 starts the 0x1BC0-byte effect D_00477AC0 (parameters from
 * byte 3), its slot kept in event variable byte 3 + 3; else that effect is sent 0xFF (stop) */
s32 func_00300A20(void *self, void *a1, u8 *cmd) {
    if (cmd[4] == 0) {
        u8 *mgr = D_0044E578;
        s32 slot = Effect_New(mgr, 0x1BC0, effect_77AC0_init);

        func_002D6090(mgr, slot, cmd + 3);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, (cmd[3] + 3) & 0xFF, slot);
    } else {
        s32 slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, (cmd[3] + 3) & 0xFF);
        u8 stop = 0xFF;

        func_002D6090(D_0044E578, slot, &stop);
    }
    return 1;
}

extern const char *const D_003FC680, *const D_0042A0E8, *const D_00400C38, *const D_0042C354;   /* "fan" (rooms 0x1A / 0x31 / 0x21 / 0x32) */

/* the room's object `name` turns 1.15 degrees a frame */
static inline __attribute__((always_inline)) void fan_turn(const char *name) {
    static const union { u32 u; f32 f; } kStep = {0x3CA46C8A}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);

    if (o != NULL) {
        f32 a = AT(o, 0x14, f32) + kStep.f;

        AT(o, 0x14, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x14, f32) = a - k2Pi.f;
        }
    }
}

/* room 0x1A (D_003FC660): the fan turns */
s32 func_002AD600(void) {
    fan_turn(D_003FC680);
    return 1;
}

/* room 0x31 (D_0042A0C8): the fan turns, except while a movie plays (gProgress +0x54) */
s32 func_0031E460(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0042A0E8);
    return 1;
}

extern void *D_0046EA40[];

/* rooms 0x31 / 0x32 (D_0042A0B8, D_0042C298): the room's effect 1 made anew as D_0046EA40 */
s32 func_0031E510(void) {
    room_effect_new(D_0044E4C0, 1, D_0046EA40);
    return 1;
}

s32 func_00321590(void) {
    room_effect_new(D_0044E4C0, 1, D_0046EA40);
    return 1;
}

/* room 0x4C (D_0040AC08): character 0x11's model parts 0xA2 / 0xA4 / 0xAC get bit 2 when byte
 * 3 is 0, else lose it */
s32 func_002B3660(void *self, void *a1, u8 *cmd) {
    u8 *c = (u8 *)gCharacters[func_001770D0(gProgress, 0x11) & 0xFF];

#ifdef HG_NATIVE
    if (c == NULL) {   /* character 0x11 absent (the PS2 writes through junk) */
        return 1;
    }
#endif
    if (cmd[3] == 0) {
        AT(AT(c, 0xF0, u8 *), 0xA2, u8) |= 2;
        AT(AT(c, 0xF0, u8 *), 0xA4, u8) |= 2;
        AT(AT(c, 0xF0, u8 *), 0xAC, u8) |= 2;
    } else {
        AT(AT(c, 0xF0, u8 *), 0xA2, u8) &= ~2;
        AT(AT(c, 0xF0, u8 *), 0xA4, u8) &= ~2;
        AT(AT(c, 0xF0, u8 *), 0xAC, u8) &= ~2;
    }
    return 1;
}

extern const char *const D_004022B0, *const D_004022B4, *const D_004022B8;   /* "jimen", "kama", "sumi" */
extern f32 func_0031C058(f32 x);   /* cosf */

/* room 0x24 (D_004022C8): the kiln's ground, kiln and charcoal glow - byte 3 0 sets them up
 * (+0x74 0, +0x78 1, glow +0x7C 0, phase +0x30 -pi), else the glow pulses (0.5 + cos(phase) /
 * 2, the phase on by 12 degrees) */
s32 func_002AFF80(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kMinusPi = {0xC0490FDB}, kStep = {0x3E567750}, kPi = {0x40490FDB},
        k2Pi = {0x40C90FDB};
    const char *names[3];
    s32 i;

    names[0] = D_004022B0;
    names[1] = D_004022B4;
    names[2] = D_004022B8;
    if (cmd[3] == 0) {
        VObject *objs = D_00456DF8;

        for (i = 0; i < 3; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, names[i]);

            if (o != NULL) {
                AT(o, 0x74, s32) = 0;
                AT(o, 0x78, s32) = 1;
                AT(o, 0x7C, s32) = 0;
                AT(o, 0x30, f32) = kMinusPi.f;
            }
        }
    } else {
        VObject *objs = D_00456DF8;

        for (i = 0; i < 3; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, names[i]);
            f32 a;

            if (o == NULL) {
                continue;
            }
            AT(o, 0x7C, f32) = 0.5f + 0.5f * func_0031C058(AT(o, 0x30, f32));
            a = AT(o, 0x30, f32) + kStep.f;
            AT(o, 0x30, f32) = a;
            if (!(a <= kPi.f)) {
                AT(o, 0x30, f32) = a - k2Pi.f;
            }
        }
    }
    return 1;
}

/* a lit quad at x 0.49 .. 9.5, z 69.5 .. 60.5, from the floor to 22 (rooms 0x21 and 0x32) */
static const u32 sQuadWindow[16] = {
    0x3EFB2FEC, 0x41B00000, 0x428B0113, 0x3F800000, 0x4118089A, 0x41B00000, 0x4271F660, 0x3F800000,
    0x3EFB2FEC, 0x00000000, 0x428B0113, 0x3F800000, 0x4118089A, 0x00000000, 0x4271F660, 0x3F800000,
};

/* room 0x21 (D_00400BA8) */
s32 func_002AF4E0(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* room 0x32 (D_0042C2B8) */
s32 func_00321340(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* room 0x4B (D_00409910): a lit quad at x -63.65 .. -55.65, z 104.5, from 4 to 21 */
s32 func_002B33B0(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC27E999A, 0x41A80000, 0x42D10000, 0x3F800000, 0xC25E999A, 0x41A80000, 0x42D10000, 0x3F800000,
        0xC27E999A, 0x40800000, 0x42D10000, 0x3F800000, 0xC25E999A, 0x40800000, 0x42D10000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}

/* room 0x4E (D_0040B4D8): a lit quad at x -44 .. -36, z 60, from 54 to 71 */
s32 func_002B3D80(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC2300000, 0x428E0000, 0x42700000, 0x3F800000, 0xC2100000, 0x428E0000, 0x42700000, 0x3F800000,
        0xC2300000, 0x42580000, 0x42700000, 0x3F800000, 0xC2100000, 0x42580000, 0x42700000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}

extern void *D_00470A70[];

static void effect_70A70_init(void **obj) {
    obj[0] = D_00470A70;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

/* room 0x02 (D_003F03A0): the 0xE60-byte effect D_00470A70 by byte 3 - 0 made (its slot kept
 * in event variable 0), 1 that one sent 0 (stop), else one more made and sent 1 */
s32 func_002A91F0(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(D_0044E578, 0xE60, effect_70A70_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, slot);
    } else if (cmd[3] == 1) {
        s32 slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0);
        s32 arg = 0;

        func_002D6090(D_0044E578, slot, &arg);
    } else {
        u8 *mgr = D_0044E578;
        s32 slot = Effect_New(mgr, 0xE60, effect_70A70_init);
        s32 arg = 1;

        func_002D6090(mgr, slot, &arg);
    }
    return 1;
}

/* room 0x08 (D_003F31D8): the hanging object named by the handler's string 0xA - byte 3 0 sets
 * it still (+0x30 / +0x38 0, travel +0x3C 0.9); 1: the player's travel (+0x3C, its last move's
 * length) past 5 makes it creak (sounds 4 / 5 by turns, event bit 7) and swing for 20 frames:
 * its tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 a frame */
s32 func_002AA600(VObject *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k09 = {0x3F666666}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    const char *name = VCALL(self, 0x34, const char *(*)(VObject *, s32))(self, 0xA);
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    f32 t, a;

    if (o == NULL) {
        return 1;
    }
    if (cmd[3] == 0) {
        AT(o, 0x30, s32) = 0;
        AT(o, 0x38, s32) = 0;
        AT(o, 0x3C, f32) = k09.f;
        return 1;
    }
    if (cmd[3] != 1) {
        return 1;
    }
    if (gCharPlayer != NULL) {
        f32 d[4] __attribute__((aligned(16)));

        sceVu0CopyVector(d, (f32 *)((u8 *)gCharPlayer + 0x40));
        sceVu0SubVector(d, (f32 *)((u8 *)gCharPlayer + 0x10), d);
        t = AT(o, 0x3C, f32) + __builtin_sqrtf(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
        AT(o, 0x3C, f32) = t;
        if (!(t <= 5.0f)) {
            VObject *ev = D_0044E4D0;

            AT(o, 0x38, f32) = 20.0f;
            AT(o, 0x3C, f32) = 0.0f;
            if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 7) == 1) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 7);
                func_00122C20(&gCharPlayer->a, 4, 6, 0, 0, NULL);
            } else {
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 7);
                func_00122C20(&gCharPlayer->a, 5, 6, 0, 0, NULL);
            }
        }
    }
    t = AT(o, 0x38, f32);
    if (t <= 0.0f) {
        return 1;
    }
    AT(o, 0x38, f32) = t - 1.0f;
    if (t - 1.0f < 0.0f) {
        AT(o, 0x38, f32) = 0.0f;
    }
    a = AT(o, 0x30, f32) + 36.0f;
    AT(o, 0x30, f32) = a;
    if (!(a < 360.0f)) {
        AT(o, 0x30, f32) = a - 360.0f;
    }
    a = kPi.f * (1.0f + func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
    AT(o, 0x10, f32) = a;
    if (!(a <= kPi.f)) {
        AT(o, 0x10, f32) = a - k2Pi.f;
    }
    return 1;
}

extern void *D_00478BE0[];

static void effect_78BE0_init(void **obj) {
    obj[0] = D_00478BE0;
}

/* room 0x4E (D_0040B4F8): the 0x10-byte effect D_00478BE0 made */
s32 func_002B3AA0(void) {
    Effect_New(D_0044E578, 0x10, effect_78BE0_init);
    return 1;
}

/* a lit quad (room effect 0x1A) at x 40.5 .. 49.5, z -10.5 .. -19.5, from 1.5 to 30 (rooms 0x21
 * and 0x32) */
static const u32 sQuadDoor[16] = {
    0x4221F660, 0x41F00000, 0xC127F766, 0x3F800000, 0x42460227, 0x41F00000, 0xC19C1340, 0x3F800000,
    0x4221F660, 0x3FC00000, 0xC127F766, 0x3F800000, 0x42460227, 0x3FC00000, 0xC19C1340, 0x3F800000,
};

/* room 0x21 (D_00400BC8) */
s32 func_002AF300(void *self, void *a1, u8 *cmd) {
    return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
}

/* room 0x21 (D_00400B98): the fan turns, except while a movie plays */
s32 func_002AF680(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_00400C38);
    return 1;
}

extern const char *const D_003F03FC;   /* "doramukan" (the drum can) */

/* room 0x02 (D_003F03B0): the drum can's wobble by byte 3 - 0 still (rest height +0x38 = its
 * height), 2 struck (+0x34 strength 1), 1 each frame: the strength fades by 0.2 while it bobs
 * 0.2 x strength x sin(phase +0x30, on by 90 degrees) about the rest height */
s32 func_002A9080(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F03FC);
    f32 t;

#ifdef HG_NATIVE
    if (o == NULL) {   /* (the PS2 writes through junk) */
        return 1;
    }
#endif
    switch (cmd[3]) {
    case 2:
        AT(o, 0x30, s32) = 0;
        AT(o, 0x34, f32) = 1.0f;
        break;
    case 1:
        t = AT(o, 0x34, f32) - 0x1.99999ap-3f /* 0.2 */;
        AT(o, 0x34, f32) = t;
        if (t <= 0.0f) {
            AT(o, 0x34, f32) = 0.0f;
            AT(o, 0x30, f32) = 0.0f;
            AT(o, 0x24, f32) = AT(o, 0x38, f32);
            break;
        }
        t = AT(o, 0x30, f32) + 90.0f;
        AT(o, 0x30, f32) = t;
        if (!(t < 360.0f)) {
            AT(o, 0x30, f32) = t - 360.0f;
        }
        AT(o, 0x24, f32) = AT(o, 0x38, f32) + 0x1.99999ap-3f /* 0.2 */ *
            (AT(o, 0x34, f32) * func_0031C248(0x1.921fb6p+1f /* pi */ * AT(o, 0x30, f32) / 180.0f));
        break;
    case 0:
        AT(o, 0x34, s32) = 0;
        AT(o, 0x30, s32) = 0;
        AT(o, 0x38, f32) = AT(o, 0x24, f32);
        break;
    default:
        return 1;
    }
    return 1;
}

extern void *func_00266C40(u8 *fx, s32 n);   /* the room's effect n */

/* room 0x1C (D_003FD350): a sound (0xC0000000, bank 6) at the room's effect 1 */
s32 func_002ADB10(void) {
    u8 *e = func_00266C40(D_0044E4C0, 1);
    f32 at[4] __attribute__((aligned(16)));

    at[0] = AT(e, 0x20, f32);
    at[1] = AT(e, 0x24, f32);
    at[2] = AT(e, 0x28, f32);
    func_002FF650(D_0044E560, 0xC0000000, 6, at, 0, 0);
    return 1;
}

/* room 0x02 (D_003F03C0): the player is about and down at floor level (y <= 0) */
s32 func_002A8FF0(void) {
    if (gCharPlayer != NULL && gCharPlayer->a.active != 0 && gCharPlayer->a.pos[1] <= 0.0f) {
        return 1;
    }
    return 0;
}

extern const char *const D_0042C328;   /* "a_fragment0" */
extern void *D_00479560[];

/* room 0x32 (D_0042C2D8): byte 3 0..3 the lit quad (room effect 0x1A) as room 0x21's; 4 and up
 * the mirror fragment's reflection (room effect 0x1A, D_00479560) on the object "a_fragment0":
 * 1.8 across, -0.1 down, strength 1, kind 2, alpha 0xFF */
s32 func_00320FF0(void *self, void *a1, u8 *cmd) {
    u8 *o;

    if (cmd[3] < 4) {
        return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
    }
    o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0042C328);
    if (o != NULL) {
        u8 *fx;
        struct {
            f32 size, drop, strength;
            void *obj;
            s32 kind, alpha;
        } arg __attribute__((aligned(16)));

        AT(&arg.drop, 0, u32) = 0xBDCCCCCD;   /* -0.1 */
        AT(&arg.size, 0, u32) = 0x3FE66666;   /* 1.8 */
        arg.strength = 1.0f;
        fx = D_0044E4C0;
        arg.obj = o;
        arg.kind = 2;
        arg.alpha = 0xFF;
        room_effect_new(fx, 0x1A, D_00479560);
        func_00266C70(fx, 0x1A, &arg);
    }
    return 1;
}

/* room 0x32 (D_0042C2A8): the fan turns, except while a movie plays */
s32 func_003214E0(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0042C354);
    return 1;
}

extern const char *D_0047B02C;   /* "fan" (room 0x37) */

/* room 0x37 (D_004469A0): the fan turns, except while a movie plays */
s32 func_0036A400(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0047B02C);
    return 1;
}

extern f32 D_0047B280;   /* room 0x62: the dropped thing's fall speed */

/* room 0x62 (D_00422318): the room's effect 0 dropped by byte 3 - 0 at rest (speed 0), 1 raised
 * by 0.5; else it falls (gravity 0.5 a frame, turning 0.16) and bounces off 0.7 losing 70%
 * (a sound each bounce) until slower than 0.2 (+0x74 set: landed; 1), else still going (2) */
s32 func_00308C20(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kBounce = {0xBE99999A};   /* -0.3 */
    u8 *e = func_00266C40(D_0044E4C0, 0);
    f32 v, y;

    switch (cmd[3]) {
    case 0:
        D_0047B280 = 0.0f;
        return 1;
    case 1:
        AT(e, 0x28, f32) = AT(e, 0x28, f32) + 0.5f;
        return 1;
    case 2:
        AT(e, 0x74, s32) = 0;
        break;
    default:
        AT(e, 0x74, s32) = 0;
        break;
    }
    AT(e, 0x28, f32) = AT(e, 0x28, f32) + 0x1.47ae14p-3f /* 0.16 */;
    v = D_0047B280 - 0.5f;
    D_0047B280 = v;
    y = AT(e, 0x24, f32) + v;
    AT(e, 0x24, f32) = y;
    if (y < 0x1.666666p-1f /* 0.7 */) {
        f32 at[4] __attribute__((aligned(16)));

        AT(e, 0x24, f32) = 0x1.666666p-1f;
        D_0047B280 = v * kBounce.f;
        sceVu0CopyVector(at, (f32 *)(e + 0x20));
        func_002FF650(D_0044E560, 0, 6, at, 0, 0);
        if (D_0047B280 < 0x1.99999ap-3f /* 0.2 */) {
            AT(e, 0x74, s32) = 1;
            return 1;
        }
    }
    return 2;
}

extern const char *const D_00438D00;   /* room 0x6A's object */

/* room 0x6A (D_00438CF0): its object's animation (+0x74 forward, +0x78 back) at a point +0x7C
 * (0..1) by byte 3 - 0 / 1 from event variable 0 (12..30 over 18, 12..28 over 16), 2 / 3 at the
 * start / end */
s32 func_00344180(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00438D00);
    u32 n;

    if (o == NULL) {
        return 1;
    }
    n = VCALL(D_0044E4D0, 0x34, u32 (*)(VObject *, s32))(D_0044E4D0, 0);
    switch (cmd[3]) {
    case 0:
        if (n < 0xC) {
            n = 0xC;
        }
        if (!(n < 0x1F)) {
            n = 0x1E;
        }
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(n - 0xC) / 18.0f;
        break;
    case 1:
        if (n < 0xC) {
            n = 0xC;
        }
        if (!(n < 0x1D)) {
            n = 0x1C;
        }
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(n - 0xC) / 16.0f;
        break;
    case 2:
    case 3:
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(cmd[3] - 2);
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

/* room 0x6A (D_00438CE0): a lit quad at x 120, z 47 .. 39, from 4 to 21 */
s32 func_00344390(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0x42F00000, 0x41A80000, 0x423C0000, 0x3F800000, 0x42F00000, 0x41A80000, 0x421C0000, 0x3F800000,
        0x42F00000, 0x40800000, 0x423C0000, 0x3F800000, 0x42F00000, 0x40800000, 0x421C0000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}

/* room 0x48 (D_004267F8): the handler's objects 2 and 3 swing (phases 60 degrees apart) - byte 3
 * 0 sets them still; 1: while slower than 5, Hewie's movement (the squared length of his last
 * step, +0x3C) past 1 makes them swing for 20 frames (the first also creaks: sounds 4 / 5 by
 * turns, event bit 0x11); the tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 */
s32 func_0030F4E0(VObject *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *objs = D_00456DF8;
    VObject *ev = D_0044E4D0;
    s32 i;

    for (i = 0; i < 2; i++) {
        const char *name = VCALL(self, 0x34, const char *(*)(VObject *, s32))(self, i + 2);
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, name);
        f32 t, a;

        if (o == NULL) {
            continue;
        }
        if (cmd[3] == 0) {
            AT(o, 0x30, s32) = 0;
            AT(o, 0x34, f32) = 60.0f * (f32)i;
            AT(o, 0x38, s32) = 0;
            AT(o, 0x3C, s32) = 0;
            continue;
        }
        if (cmd[3] != 1) {
            continue;
        }
        if (gCharPartner != NULL && AT(o, 0x38, f32) < 5.0f) {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0CopyVector(d, (f32 *)((u8 *)gCharPartner + 0x40));
            sceVu0SubVector(d, d, (f32 *)((u8 *)gCharPartner + 0x10));
            t = AT(o, 0x3C, f32) + (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
            AT(o, 0x3C, f32) = t;
            if (!(t <= 1.0f)) {
                AT(o, 0x38, f32) = 20.0f;
                AT(o, 0x3C, f32) = 0.0f;
                if (i == 0) {
                    if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 0x11) == 1) {
                        VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0x11);
                        func_00122C20(&gCharPlayer->a, 4, 6, 0, 0, NULL);
                    } else {
                        VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0x11);
                        func_00122C20(&gCharPlayer->a, 5, 6, 0, 0, NULL);
                    }
                }
            }
        }
        t = AT(o, 0x38, f32);
        if (t <= 0.0f) {
            continue;
        }
        AT(o, 0x38, f32) = t - 1.0f;
        if (t - 1.0f < 0.0f) {
            AT(o, 0x38, f32) = 0.0f;
        }
        a = AT(o, 0x30, f32) + 36.0f;
        AT(o, 0x30, f32) = a;
        if (!(a + AT(o, 0x34, f32) < 360.0f)) {
            AT(o, 0x30, f32) = a - 360.0f;
        }
        a = kPi.f * (1.0f + func_0031C248(kPi.f * (AT(o, 0x30, f32) + AT(o, 0x34, f32)) / 180.0f)) / 180.0f;
        AT(o, 0x10, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x10, f32) = a - k2Pi.f;
        }
    }
    return 1;
}

extern void *D_00479890[];

static void effect_79890_init(void **obj) {
    obj[0] = D_00479890;
}

/* room 0x48 (D_00426818): byte 3 0 starts the wall shadow (D_00479890, its slot in event
 * variable 2); else that one is ended */
s32 func_0030F350(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(D_0044E578, 8, effect_79890_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 2, slot);
    } else {
        func_002D6090(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 2), NULL);
    }
    return 1;
}

extern void *D_004795A0[];

static void effect_795A0_init(void **obj) {
    obj[0] = D_004795A0;
}

/* room 0x60 (D_00429148): the floor light (room effect 0x1B, 20 x 20 at y -0.2) by byte 3 - 1
 * removed with its glow effect (event variable 1); 0 made, with the glow (D_004795A0); then (and
 * for other values) its strength from event variable 0 (0..4: 0, 30, 60, 90, 128), also sent to
 * the glow */
s32 func_003106E0(void *self, void *a1, u8 *cmd) {
    u32 q[20] __attribute__((aligned(16)));
    VObject *ev;

    if (cmd[3] == 1) {
        func_002670F0(D_0044E4C0, 0x1B);
        func_002D6170(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 1));
        return 1;
    }
    if (cmd[3] == 0) {
        s32 slot;

        room_effect_new(D_0044E4C0, 0x1B, D_00472F60);
        slot = Effect_New(D_0044E578, 0x10, effect_795A0_init);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 1, slot);
    }
    ev = D_0044E4D0;
    q[16] = 0;
    q[0] = 0x41200000;   /* (10, -0.2, -10) */
    q[8] = 0x41200000;
    q[1] = 0xBE4CCCCD;
    q[2] = 0xC1200000;
    q[3] = 0x3F800000;
    q[4] = 0xC1200000;   /* (-10, -0.2, -10) */
    q[5] = 0xBE4CCCCD;
    q[6] = 0xC1200000;
    q[12] = 0xC1200000;
    q[7] = 0x3F800000;
    q[9] = 0xBE4CCCCD;   /* (10, -0.2, 10) */
    q[13] = 0xBE4CCCCD;
    q[10] = 0x41200000;
    q[14] = 0x41200000;
    q[11] = 0x3F800000;
    q[15] = 0x3F800000;
    q[19] = 0;
    switch (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0)) {
    case 0:
        q[19] = 0;
        break;
    case 1:
        q[19] = 0x1E;
        break;
    case 2:
        q[19] = 0x3C;
        break;
    case 3:
        q[19] = 0x5A;
        break;
    case 4:
        q[19] = 0x80;
        break;
    }
    q[18] = 0x3F800000;
    q[17] = q[19];
    func_00266C70(D_0044E4C0, 0x1B, q);
    if (q[19] != 0) {
        func_002D6090(D_0044E578, VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1), &q[19]);
    }
    return 1;
}

extern const char *const D_00438700[];   /* "dial0".."dial2", then (D_0043870C) "dial3".."dial5" lit */
extern u8 D_0047AE80[];                  /* the dials' progress variables (0x18..0x1A) */
extern u32 D_0047E36C;                   /* menu buttons pressed (MENU_*) */
extern u32 D_0047E364;                   /* menu buttons repeating */

/* a dial's angle for its setting (0..3, a quarter turn each) */
static inline __attribute__((always_inline)) f32 dial_angle(Progress *p, s32 k) {
    return 0x1.921fb6p+1f * (f32)(s32)((u8)Progress_GetVar(p, D_0047AE80[k]) * 90) / 180.0f;
}

/* room 0x69 (D_004386F8): the three-dial lock by byte 3 - 0 the dials (and their lit twins) set
 * to their settings; 1 the player at it: up / down pick the dial (event variable 0; its lit twin
 * shown), left / right turn it (its setting, sound bit 3), cancel leaves (event bit 2); 2 the
 * picked dial turns 4 degrees a frame to its setting, then - 1, 0, 2 - the lock opens (event
 * bits 2 off, 4) */
s32 func_00343A00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k4 = {0x3D8EFA35}, kM4 = {0xBD8EFA35};

    switch (cmd[3]) {
    case 0: {
        VObject *objs = D_00456DF8;
        Progress *p = gProgress;
        s32 i;

        for (i = 0; i < 6; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[i]);

            if (o != NULL) {
                AT(o, 0x14, f32) = func_002E2D00(dial_angle(p, i % 3));
            }
        }
        break;
    }
    case 1: {
        VObject *ev = D_0044E4D0;
        u8 old = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
        u8 sel = old;

        if (D_0047E36C & 0x20) {
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 2);
            break;
        }
        if (D_0047E36C & 1) {
            sel = sel == 0 ? 2 : sel - 1;
        } else if (D_0047E36C & 4) {
            sel = sel < 2 ? sel + 1 : 0;
        }
        if (sel != old) {
            VObject *objs = D_00456DF8;
            u8 *o;

            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[old])) != NULL) {
                AT(o, 0x0, u8) = 0;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[old + 3])) != NULL) {
                AT(o, 0x0, u8) = 1;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel])) != NULL) {
                AT(o, 0x0, u8) = 1;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel + 3])) != NULL) {
                AT(o, 0x0, u8) = 0;
            }
            ev = D_0044E4D0;
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, sel);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 6);
        } else {
            u8 *var = &D_0047AE80[sel];
            Progress *p = gProgress;
            u8 v = Progress_GetVar(p, *var);

            if (D_0047E364 & 8) {
                v = v == 0 ? 3 : v - 1;
            } else if (D_0047E364 & 2) {
                v = v < 3 ? v + 1 : 0;
            }
            if (v != (u8)Progress_GetVar(p, *var)) {
                AT(p, 0x9C + *var, u8) = v;
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
            }
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 6);
        }
        break;
    }
    case 2: {
        VObject *ev = D_0044E4D0;
        u8 sel = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
        VObject *objs = D_00456DF8;
        u8 *o1 = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel]);
        u8 *o2 = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel + 3]);
        Progress *p4 = gProgress;
        f32 d;

#ifdef HG_NATIVE
        if (o1 == NULL || o2 == NULL) {   /* (the PS2 writes through junk) */
            break;
        }
#endif
        d = func_002E2D00(AT(o1, 0x14, f32) - dial_angle(p4, sel));
        if (!(d <= k4.f)) {
            AT(o1, 0x14, f32) = AT(o1, 0x14, f32) - k4.f;
            AT(o2, 0x14, f32) = AT(o2, 0x14, f32) - k4.f;
        } else if (d < kM4.f) {
            AT(o1, 0x14, f32) = AT(o1, 0x14, f32) + k4.f;
            AT(o2, 0x14, f32) = AT(o2, 0x14, f32) + k4.f;
        } else {
            Progress *p = gProgress;
            f32 a = func_002E2D00(dial_angle(p, sel));

            AT(o2, 0x14, f32) = a;
            AT(o1, 0x14, f32) = a;
            if ((u8)Progress_GetVar(p, D_0047AE80[0]) == 1 && (u8)Progress_GetVar(p4, D_0047AE80[1]) == 0 &&
                (u8)Progress_GetVar(p4, D_0047AE80[2]) == 2) {
                VObject *e = D_0044E4D0;

                VCALL(e, 0x60, void (*)(VObject *, s32))(e, 2);
                VCALL(e, 0x5C, void (*)(VObject *, s32))(e, 4);
            } else {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 3);
            }
        }
        break;
    }
    default:
        return 1;
    }
    return 1;
}

/* Fiona's model +0xD0 (0, 1.5, -2.5) and +0xCC(1) when byte 3 is 0, else (0, 1.5, -1.5) and
 * +0xCC(0) */
s32 func_002A8AF0(void *self, void *a1, u8 *cmd) {
    void *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xD0, void (*)(void *, f32, f32, f32))(m, 0.0f, 1.5f, -2.5f);
        VCALL(m, 0xCC, void (*)(void *, s32))(m, 1);
    } else {
        VCALL(m, 0xD0, void (*)(void *, f32, f32, f32))(m, 0.0f, 1.5f, -1.5f);
        VCALL(m, 0xCC, void (*)(void *, s32))(m, 0);
    }
    return 1;
}

extern void *D_004737D0[];

static void glow4_init(void **obj) {
    obj[0] = D_004737D0;
    obj[0x40 / 4] = D_00469D00;
    ((s32 *)obj)[0x44 / 4] = -1;
    obj[0x40 / 4] = D_0046FC30;
}

/* byte 4 0: the effect D_004737D0 (grey, 0x18, size 30) at one of four spots by byte 3 (-332 /
 * -368, 100.8, -12 / 55 / 165 / 165), its slot in event var byte 3; else that effect removed */
static inline s32 glow4_spot(u8 *cmd, u32 first) {
    static const union { u32 u; f32 f; } kY = {0x42C9999A};   /* 100.8 */
    u8 k = cmd[3];

    if (cmd[4] == 0) {
        u8 *mgr = D_0044E578;
        s32 slot = Effect_New(mgr, 0x80, glow4_init);
        s32 p[8] __attribute__((aligned(16)));

        p[0] = 0x80;
        p[1] = 0x80;
        p[2] = 0x80;
        p[3] = 0x18;
        switch ((u32)(k - first)) {
        case 0:
            AT(&p[4], 0, f32) = -332.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = -12.0f;
            break;
        case 1:
            AT(&p[4], 0, f32) = -368.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = 55.0f;
            break;
        case 2:
            AT(&p[4], 0, f32) = -368.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = 165.0f;
            break;
        case 3:
            AT(&p[4], 0, f32) = -332.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = 165.0f;
            break;
        }
        AT(&p[7], 0, f32) = 30.0f;
        func_002D6090(mgr, slot, p);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, k, slot);
    } else {
        func_002D6170(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, k));
    }
    return 1;
}

s32 func_002A8BA0(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 0);
}

extern const char *const D_0040C160;   /* an object's name */

/* lower the object (D_0040C160)'s +0x14 by 0.025 a frame down to -0.78, then event 6 (+0x5C) */
s32 func_002B4280(void) {
    static const union { u32 u; f32 f; } kStep = {0x3CCCCCCD}, kLow = {0xBF47AE14};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0040C160);
    f32 y = AT(o, 0x14, f32) - kStep.f;

    AT(o, 0x14, f32) = y;
    if (y <= kLow.f) {
        AT(o, 0x14, f32) = kLow.f;
        VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 6);
    }
    return 1;
}

/* ---- rooms 0x20 / 0x21 / 0x23 ---- */

extern void *D_00469C20[];   /* Actor base vtable */
extern const char *const D_003FF124;     /* room 0x20's falling object */
extern const char *const D_0040187C;
extern s32 func_00177BF0(Progress *p, s32 a, s32 slot);
extern void func_001247E0(Actor *a, const f32 *delta);   /* moved on the nav mesh */
extern u32 func_00208F80(VObject *ev, s32 id);           /* (events +0x38) a character's script value */

static inline u8 *room_obj(const char *name) {
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
}

/* nav triangle `i`'s record, or NULL */
static inline u8 *nav_tri(u32 i) {
    u8 *nm = (u8 *)D_0044E570;

    return (i < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) ? AT(nm, 0x4, u8 *) + i * 0x50 : NULL;
}

/* room 0x20 (D_003FF0F8): object byte 3 becomes event point byte 4 (radii 5) */
s32 func_002AE4E0(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_003FF110[cmd[3]]);

    if (o == NULL) {
        return 0;
    }
    VCALL(D_0044E4D0, 0x6C, void (*)(VObject *, u8, f32 *, f32, f32))(D_0044E4D0, cmd[4], (f32 *)(o + 0x20), 5.0f,
                                                                       5.0f);
    return 1;
}

/* room 0x20 (D_003FF0E8): the character's script value is at least be32 bytes 3..6 */
s32 func_002AE570(void *self, u8 *chr, u8 *cmd) {
    u32 v = VCALL(D_0044E4D0, 0x38, u32 (*)(VObject *, s32))(D_0044E4D0, chr[0x153C]) & 0xFFFF;

    return !(v < ((u32)cmd[3] << 24 | (u32)cmd[4] << 16 | (u32)cmd[5] << 8 | cmd[6]));
}

/* room 0x20 (D_003FF0D8): the player, free and within 5 of the falling object, knocks it - it
 * gets a push (0, 1, 1) turned by her facing, and its nav triangle */
s32 func_002AE5E0(void) {
    u8 *o = room_obj(D_003FF124);
    Character *p;
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));

    if (o == NULL) {
        return 0;
    }
    p = gCharPlayer;
    if (p == NULL || p->a.active != 1 || AT(p, 0x29, u8) != 0) {
        return 0;
    }
    if (VCALL((VObject *)p, 0x74, s32 (*)(VObject *, f32 *))((VObject *)p, v) == 0) {
        return 0;
    }
    sceVu0SubVector(v, v, (f32 *)(o + 0x20));
    if (25.0f < v[1] * v[1] + v[0] * v[0] + v[2] * v[2]) {
        return 0;
    }
    sceVu0CopyVector(v, (f32 *)((u8 *)p + 0x50));
    v[3] = 1.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, v);
    v[2] = 1.0f;
    v[3] = 1.0f;
    v[0] = 0.0f;
    v[1] = 1.0f;
    sceVu0ApplyMatrix((f32 *)(o + 0x30), m, v);
    AT(o, 0x3C, s32) = VCALL((VObject *)D_0044E570, 0x3C, s32 (*)(VObject *, f32 *, s32))(
        (VObject *)D_0044E570, (f32 *)(o + 0x20), 0);
    return 1;
}

/* room 0x20 (D_003FF0C8): the character turns to face object byte 3 */
s32 func_002AE790(void *self, u8 *chr, u8 *cmd) {
    u8 *o = room_obj(D_003FF110[cmd[3]]);

    if (o != NULL) {
        AT(chr, 0x10C, f32) = func_002E2D00(func_0031C5C0(AT(o, 0x20, f32) - AT(chr, 0x10, f32),
                                                          AT(o, 0x28, f32) - AT(chr, 0x18, f32)));
        chr[0xE1] = 0;
        AT(chr, 0xF4, s32) = 0xF;
    }
    return 1;
}

static void dust_cloud_init(void **obj) {
    obj[0] = D_0046FF20;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* room 0x20 (D_003FF0B8): a dust cloud where the falling object is */
s32 func_002AE830(void) {
    u8 *o = room_obj(D_003FF124);
    s32 slot;
    struct {
        f32 pos[4];
        s32 kind, a, b, c, d;
    } prm __attribute__((aligned(16)));

    if (o == NULL) {
        return 1;
    }
    slot = Effect_New(D_0044E578, 0x720, dust_cloud_init);
    prm.pos[0] = AT(o, 0x20, f32);
    prm.pos[1] = AT(o, 0x24, f32);
    prm.pos[2] = AT(o, 0x28, f32);
    prm.pos[3] = 1.0f;
    prm.d = 0x10;
    prm.kind = 2;
    prm.c = 0x50;
    prm.b = 0x50;
    prm.a = 0x50;
    func_002D6090(D_0044E578, slot, &prm);
    return 1;
}

/* room 0x20 (D_003FF0A8): the falling object falls (gravity 0.2 a frame on its velocity +0x30,
 * spinning 1 degree a frame) along the nav mesh, events bit 1 set while it lies on open floor;
 * landing on floor that isn't 0x10000 raises dust */
s32 func_002AE9C0(void *self) {
    u8 *o = room_obj(D_003FF124);
    u8 *nm, *t;
    s32 tri;
    f32 y;
    f32 g[4] __attribute__((aligned(16)));
    Actor a;

    if (o == NULL) {
        return 1;
    }
    tri = AT(o, 0x3C, s32);
    if (tri == -1) {
        return 1;
    }
    if (tri & 0x80000000) {   /* landed */
        t = nav_tri(tri & 0x7FFFFFFF);
        if (t == NULL || (AT(t, 0x3C, u32) & 0x20020008)) {
            VCALL(D_0044E4D0, 0x60, void (*)(VObject *, s32))(D_0044E4D0, 1);
        }
        return 1;
    }
    g[1] = -0x1.99999a0000000p-3f /* 0.2 */;
    g[0] = 0.0f;
    g[3] = 1.0f;
    g[2] = 0.0f;
    sceVu0AddVector((f32 *)(o + 0x30), g, (f32 *)(o + 0x30));
    VCALL(D_0044E4D0, 0x60, void (*)(VObject *, s32))(D_0044E4D0, 1);
    a.vtbl = D_00469C20;
    a.slot = 0x0FFFFFFF;
    a.flags24 = 0x1000000;
    y = AT(o, 0x24, f32);
    a.navMask = 0x20020008;
    a.navTri = tri;
    sceVu0CopyVector(a.pos, (f32 *)(o + 0x20));
    func_001247E0(&a, (f32 *)(o + 0x30));
    nm = (u8 *)D_0044E570;
    VCALL((VObject *)nm, 0x14, void (*)(VObject *, u32, f32 *))((VObject *)nm, a.navTri, a.pos);
    y += AT(o, 0x34, f32);
    sceVu0CopyVector((f32 *)(o + 0x20), a.pos);
    AT(o, 0x3C, u32) = a.navTri;
    if (a.navTri == (u32)-1) {
        a.vtbl = D_00469C20;
        return 1;
    }
    if (!(y <= a.pos[1])) {
        f32 r;

        AT(o, 0x24, f32) = y;
        r = AT(o, 0x14, f32) + 0x1.1df46ap-6f /* 1 degree */;
        AT(o, 0x14, f32) = r;
        if (!(r <= 0x1.921fb6p+1f /* pi */)) {
            AT(o, 0x14, f32) = r - 0x1.921fb6p+2f /* 2 pi */;
        }
    } else {
        VCALL(D_0044E4D0, 0x60, void (*)(VObject *, s32))(D_0044E4D0, 7);
        AT(o, 0x38, f32) = 0.0f;
        AT(o, 0x34, f32) = 0.0f;
        AT(o, 0x30, f32) = 0.0f;
        t = (a.navTri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) ? AT(nm, 0x4, u8 *) + a.navTri * 0x50 : NULL;
        if (t == NULL || (AT(t, 0x3C, u32) & 0x20020008)) {
            VCALL(D_0044E4D0, 0x60, void (*)(VObject *, s32))(D_0044E4D0, 1);
        }
        if (t != NULL && (AT(t, 0x3C, u32) & 0x2018000) != 0x10000) {
            func_002AE830();
        }
        AT(o, 0x3C, u32) |= 0x80000000;
        VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 1);
    }
    a.vtbl = D_00469C20;
    return 1;
}

/* room 0x20 (D_003FF088): the falling object back up in place */
s32 func_002AEEE0(void) {
    u8 *o = room_obj(D_003FF124);

#ifdef HG_NATIVE
    if (o == NULL) {   /* (the PS2 writes through junk) */
        return 1;
    }
#endif
    o[0] = 0;
    AT(o, 0x20, u32) = 0x411A8F5C;   /* 9.66 */
    AT(o, 0x24, u32) = 0x3F028F5C;   /* 0.51 */
    AT(o, 0x28, u32) = 0x42BACCCD;   /* 93.4 */
    AT(o, 0x10, u32) = 0x3FC8F5C3;   /* 1.57 */
    AT(o, 0x14, u32) = 0x3F4A3D71;   /* 0.79 */
    AT(o, 0x18, u32) = 0xC048F5C3;   /* -3.14 */
    return 1;
}

/* room 0x20 (D_003FF078): the player's action byte 3 (7) at (-30, 0, 90) */
s32 func_002AEF60(void *self, void *a1, u8 *cmd) {
    f32 at[4] __attribute__((aligned(16)));

    at[0] = -30.0f;
    at[2] = 90.0f;
    at[1] = 0.0f;
    at[3] = 1.0f;
    func_00122C20(&gCharPlayer->a, cmd[3], 7, 0, 0, at);
    return 1;
}

/* room 0x21 (D_00400BF0): the pursuer, about and not in state 2, is in its mode 2 but in
 * another room than the current one (the player about too) */
s32 func_002AF0F0(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0) {
        return 0;
    }
    if (AT(s, 0xC4, s32) == 2 || AT(s, 0x153C, u8) != 2) {
        return 0;
    }
    return AT(s, 0x30, s32) != VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress);
}

/* room 0x21 (D_00400BE0): Fiona's model +0x1570 (byte 3 0) / +0x1574 (1) = byte 4 */
s32 func_002AF1D0(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 3)]->motion;

    switch (cmd[3]) {
    case 1:
        AT(m, 0x1574, s32) = cmd[4];
        break;
    case 0:
        AT(m, 0x1570, s32) = cmd[4];
        break;
    }
    return 1;
}

/* room 0x21 (D_00400BD0): the player's model +0xC8 vector by byte 3 */
s32 func_002AF250(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;
    f32 v[4] __attribute__((aligned(16)));

    if (cmd[3] == 0) {
        v[0] = 0.0f;
        v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
        v[2] = -0x1.47ae140000000p-7f /* 0.01 */;
    } else if (cmd[3] == 1) {
        v[2] = 0.0f;
        v[0] = 0x1.47ae140000000p-6f /* 0.02 */;
        v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
    } else {
        v[0] = 0.0f;
        v[2] = 0.0f;
        v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
    }
    VCALL(m, 0xC8, void (*)(VObject *, f32 *))(m, v);
    return 1;
}

/* room 0x23 (D_00401848): byte 3 0 a progress name, 1 wait for character 3 (2 while not), else
 * done */
s32 func_002AFA00(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_0040187C);
        return 1;
    case 1:
        return func_0016CD60(gProgress, 3, 0) == 0 ? 2 : 1;
    }
    func_0016CD30(gProgress);
    return 1;
}

/* room 0x23 (D_00401838): Fiona's model +0x9A0 / +0x9A8: 0 (byte 3 1) or 0.12 / 0.2 */
s32 func_002AFAA0(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 3)]->motion;

    if (cmd[3] == 1) {
        AT(m, 0x9A0, f32) = 0.0f;
        AT(m, 0x9A8, f32) = 0.0f;
    } else {
        AT(m, 0x9A0, u32) = 0x3DF5C28F;   /* 0.12 */
        AT(m, 0x9A8, u32) = 0x3E4CCCCD;   /* 0.2 */
    }
    return 1;
}

/* room 0x23 (D_00401828): none of the six slots' func_00177BF0 bits 0..3, and the stalker is
 * about but not active, in mode 2, 6 or 7 */
s32 func_002AFB50(void) {
    u32 acc = 0;
    s32 i;
    u8 *c;
    u8 k;

    for (i = 0; i < 6; i++) {
        acc |= (u8)func_00177BF0(gProgress, 0, i & 0xFF);
    }
    if (acc & 0xF) {
        return 0;
    }
    c = D_0044F808;
    if (c == NULL || AT(c, 0x28, u8) == 1) {
        return 0;
    }
    k = AT(c, 0x153C, u8);
    return k == 2 || k == 6 || k == 7;
}

/* ---- rooms 0x02 .. 0x12 ---- */

extern const char *const D_003F0DBC, *const D_003F0DC0;   /* room 0x03 (and D_003F0DC4) */
extern const char *const D_003F17B4;   /* room 0x04 (and D_003F17B8 / D_003F17C8) */
extern const char *const D_003F17CC, *const D_003F17D0, *const D_003F17D4, *const D_003F17D8, *const D_003F17DC;
extern const f32 D_003F2180[4][4];   /* room 0x06: where its four objects go */
extern const char D_0047A9A0[7];     /* room 0x06: the first object's name (its 6th letter counts on) */
extern const char *const D_003F5440[];                  /* room 0x0C (+13: the three pairs) */
extern const char *const D_003F6F48, *const D_003F6F4C, *const D_003F6F50;   /* room 0x0F */
extern void *D_00479AC0[], *D_00471060[], *D_00470E20[];
extern void func_002EFB70(u8 *panic, f32 amount);
extern s32 func_001B9000(VObject *r, u32 rgba);

/* an object's angle field `off` set, if it is there */
static inline void obj_angle(const char *name, u32 off, u32 bits) {
    u8 *o = room_obj(name);

    if (o != NULL) {
        AT(o, off, u32) = bits;
    }
}

static void effect_479ac0_init(void **obj) {
    obj[0] = D_00479AC0;
}

/* an effect of class D_00479AC0 (0x10 bytes) on object `o` with value `f` */
static inline void obj_effect(u8 *o, u32 f) {
    struct {
        u8 *o;
        u32 f;
    } prm;
    s32 slot = Effect_New(D_0044E578, 0x10, effect_479ac0_init);

    prm.f = f;
    prm.o = o;
    func_002D6090(D_0044E578, slot, &prm);
}

/* room 0x02 (D_003F03C8): the pursuer is about, in a mode other than 0, 1 or 5, and progress
 * +0x1130 isn't 0xFE */
s32 func_002A8F50(void) {
    Character *s = gCharPursuer;

    if (s == NULL || s->a.active == 0 || AT(s, 0xE8, s32) == 0 || AT(s, 0xE8, s32) == 1 || AT(s, 0xE8, s32) == 5) {
        return 0;
    }
    if (AT(gProgress, 0x1130, u8) == 0xFE) {
        return 0;
    }
    return 1;
}

/* room 0x02 (D_003F0388): the panic (progress +0x7B8) raised to 80 */
s32 func_002A9460(void) {
    u8 *p = (u8 *)gProgress;
    f32 t = 80.0f - AT(p, 0x7BC, f32);

    if (!(t < 0.0f)) {
        func_002EFB70(p + 0x7B8, t);
    }
    return 1;
}

/* room 0x03 (D_003F0D90): three objects turned (-60, -60 degrees about x; -90 about z) */
s32 func_002A99A0(void) {
    obj_angle(D_003F0DBC, 0x10, 0xBF860A92);
    obj_angle(D_003F0DC0, 0x10, 0xBF860A92);
    obj_angle(D_003F0DC4, 0x18, 0xBFC90FDB);
    return 1;
}

/* room 0x04 (D_003F17A0): an effect on one object (byte 3 0: at 90 degrees) or the other (0) */
s32 func_002A9B70(void *self, void *a1, u8 *cmd) {
    u8 *o;
    u32 f;

    if (cmd[3] == 0) {
        o = room_obj(D_003F17B8);
        f = 0x3FC90FDB;
    } else {
        o = room_obj(D_003F17C8);
        f = 0;
    }
    obj_effect(o, f);
    return 1;
}

/* room 0x04 (D_003F1790): byte 3 0: room effect 0x1B (D_00479560) on its object, a box (640,
 * -560, 1000, 0, 0x60); else the effect gone */
s32 func_002A9CE0(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        u8 *o = room_obj(D_003F17B4);

        if (o != NULL) {
            struct {
                f32 x, y, z;
                u8 *o;
                s32 a, b;
            } prm;

            prm.a = 0;
            prm.x = 640.0f;
            prm.y = -560.0f;
            prm.z = 1000.0f;
            prm.b = 0x60;
            prm.o = o;
            room_effect_new(D_0044E4C0, 0x1B, D_00479560);
            func_00266C70(D_0044E4C0, 0x1B, &prm);
        }
    } else {
        func_002670F0(D_0044E4C0, 0x1B);
    }
    return 1;
}

/* room 0x04 (D_003F1780): five objects turned -75 / 75 degrees in turn */
s32 func_002A9E20(void) {
    obj_angle(D_003F17CC, 0x14, 0xBFA78D37);
    obj_angle(D_003F17D0, 0x14, 0x3FA78D37);
    obj_angle(D_003F17D4, 0x14, 0xBFA78D37);
    obj_angle(D_003F17D8, 0x14, 0x3FA78D37);
    obj_angle(D_003F17DC, 0x14, 0xBFA78D37);
    return 1;
}

/* room 0x06 (D_003F21C0): its four objects (the name's 6th letter counting) to their places */
s32 func_002AA380(void) {
    char name[7];
    s32 i;

    for (i = 0; i < 7; i++) {
        name[i] = D_0047A9A0[i];
    }
    for (i = 0; i < 4; i++) {
        u8 *o = room_obj(name);

#ifdef HG_NATIVE
        if (o != NULL)   /* (the PS2 writes through junk) */
#endif
        sceVu0CopyVector((f32 *)(o + 0x20), (f32 *)D_003F2180[i]);
        name[5]++;
    }
    return 1;
}

/* room 0x08 (D_003F31F0): the stalker is there but not about */
s32 func_002AA580(void) {
    u8 *c = D_0044F808;

    return c != NULL && AT(c, 0x28, u8) == 0;
}

/* room 0x08 (D_003F31C0): the cutscene director's +0x6C 3 (byte 3 0) or 2 */
s32 func_002AA8C0(void *self, void *a1, u8 *cmd) {
    VCALL(D_0044FE10, 0x6C, void (*)(VObject *, s32))(D_0044FE10, cmd[3] == 0 ? 3 : 2);
    return 1;
}

/* room 0x09 (D_003F3C50): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in another
 * room than 9 (the player about too) */
s32 func_002AAA60(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;
    u8 k;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    return AT(s, 0x30, s32) != 9;
}

/* room 0x0A (D_003F4388): an effect on its object at -2.88 */
s32 func_002AAC30(void) {
    obj_effect(room_obj(D_003F43A0), 0xC0384E89);
    return 1;
}

/* room 0x0B (D_003F4640): the player is 20 .. 120 from (x, z) = s16 bytes 3..4, 5..6 */
s32 func_002AB130(void *self, void *a1, u8 *cmd) {
    f32 dx = (f32)(s16)(cmd[3] << 8 | cmd[4]) - gCharPlayer->a.pos[0];
    f32 dz = (f32)(s16)(cmd[5] << 8 | cmd[6]) - gCharPlayer->a.pos[2];
    f32 d = ee_sqrtf(dz * dz + dx * dx);

    return !(d < 20.0f) && d <= 120.0f;
}

/* room 0x0C (D_003F5428): three objects (pair byte 4) swing: byte 3 0 set up (rest +0x30, phase
 * +0x34 half a turn apart, swing +0x3C 0.75 / 0.5), 1 a step (phase on 60 degrees, the swing
 * down 0.1, x = rest + swing x sin(phase); 2 once still), 2 all to x 10. 2 while any swings */
s32 func_002AB300(void *self, void *a1, u8 *cmd) {
    s32 moving = 0;
    s32 i;

    for (i = 0; i < 3; i++) {
        u8 *o = room_obj(D_003F5440[cmd[4] + i * 2 + 13]);
        f32 a;

        switch (cmd[3]) {
        case 0:
            sceVu0CopyVector((f32 *)(o + 0x30), (f32 *)(o + 0x20));
            AT(o, 0x34, f32) = 0.5f * (f32)i;
            AT(o, 0x3C, f32) = 0.5f + 0.25f * (f32)(u32)(i != 2);
            break;
        case 1:
            a = AT(o, 0x34, f32) + 0x1.0c1524p+0f /* 60 degrees */;
            AT(o, 0x34, f32) = a;
            if (!(a <= 0x1.921fb6p+1f /* pi */)) {
                AT(o, 0x34, f32) = a - 0x1.921fb6p+2f /* 2 pi */;
            }
            AT(o, 0x3C, f32) = AT(o, 0x3C, f32) - 0x1.99999ap-4f /* 0.1 */;
            AT(o, 0x20, f32) = AT(o, 0x30, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
            if (!(AT(o, 0x3C, f32) <= 0.0f)) {
                moving = 1;
                break;
            }
            AT(o, 0x20, f32) = 10.0f;
            break;
        case 2:
            AT(o, 0x20, f32) = 10.0f;
            break;
        }
    }
    return moving ? 2 : 1;
}

/* room 0x0C (D_003F5418): the screen darkened as the cutscene runs past frame 0x4AE (32 a
 * frame, up to 0x80) */
s32 func_002AB510(void) {
    u32 a = (u32)(VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 0x4AE) << 5;

    if (a > 0x80) {
        a = 0x80;
    }
    func_001B9000(D_0044E4F0, a << 24);
    return 1;
}

/* room 0x0C (D_003F5408): script variable byte 3 down by the player's hit (byte 4: 1 from the
 * weak blow 0x1A, else 5) or the pursuer's (+0x108), not below 0 */
s32 func_002AB580(void *self, void *a1, u8 *cmd) {
    VObject *ev = D_0044E4D0;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[3]);
    s32 k;

    if (cmd[4] != 0) {
        k = AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;
    } else {
        k = VCALL((VObject *)gCharPursuer, 0x108, s32 (*)(VObject *))((VObject *)gCharPursuer);
    }
    if (k > 0) {
        v -= k;
        if (v < 0) {
            v = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[3], v);
    }
    return 1;
}

/* room 0x0C (D_003F53F8): room effect 0x1C (a depth range) widening with the cutscene from frame
 * 0x2D0: near 1 + 1.5 t (at most 46), far 64.4 + 1.7 t (at most 116.5) */
s32 func_002AB660(void) {
    f32 t = (f32)(VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 0x2D0);
    f32 r[4];
    f32 v;

    room_effect_new(D_0044E4C0, 0x1C, D_0046EC60);
    v = 1.0f + 1.5f * t;
    r[0] = v <= 46.0f ? v : 46.0f;
    r[1] = v <= 46.0f ? v : 46.0f;
    v = 0x1.019999ap+6f /* 64.4 */ + 0x1.b33334p+0f /* 1.7 */ * t;
    r[2] = v <= 116.5f ? v : 116.5f;
    r[3] = v <= 116.5f ? v : 116.5f;
    func_00266C70(D_0044E4C0, 0x1C, r);
    return 1;
}

/* room 0x0F (D_003F6F28): the player's model +0xD0 (0, 1.5, -2 / -1.5 by byte 3) and +0xCC */
s32 func_002ABB10(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xD0, void (*)(VObject *, f32, f32, f32))(m, 0.0f, 1.5f, -2.0f);
        VCALL(m, 0xCC, void (*)(VObject *, s32))(m, 1);
    } else {
        VCALL(m, 0xD0, void (*)(VObject *, f32, f32, f32))(m, 0.0f, 1.5f, -1.5f);
        VCALL(m, 0xCC, void (*)(VObject *, s32))(m, 0);
    }
    return 1;
}

/* room 0x0F (D_003F6F18): the player's model +0xBC (1, 0.25) or (0, 0) by byte 3 */
s32 func_002ABBC0(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xBC, void (*)(VObject *, s32, f32))(m, 1, 0.25f);
    } else {
        VCALL(m, 0xBC, void (*)(VObject *, s32, f32))(m, 0, 0.0f);
    }
    return 1;
}

static void effect_471060_init(void **obj) {
    obj[0] = D_00471060;
}

/* room 0x0F (D_003F6F08): an effect (D_00471060, 0x840 bytes) with its box */
s32 func_002ABC20(void) {
    s32 slot = Effect_New(D_0044E578, 0x840, effect_471060_init);
    f32 prm[9];

    prm[1] = 140.0f;
    prm[2] = -9.5f;
    prm[0] = 0.0f;
    prm[3] = 90.0f;
    prm[6] = 0.0f;
    prm[4] = 18.0f;
    prm[8] = 0.0f;
    prm[5] = 110.0f;
    prm[7] = -90.0f;
    func_002D6090(D_0044E578, slot, prm);
    return 1;
}

/* an object's +0x14 back to 0 (the PS2 writes through junk when it isn't there) */
static inline void obj_unturn(const char *name) {
    u8 *o = room_obj(name);

#ifdef HG_NATIVE
    if (o == NULL) {
        return;
    }
#endif
    AT(o, 0x14, s32) = 0;
}

/* room 0x0F (D_003F6EF8): three objects' +0x14 back to 0 */
s32 func_002ABD50(void) {
    obj_unturn(D_003F6F48);
    obj_unturn(D_003F6F4C);
    obj_unturn(D_003F6F50);
    return 1;
}

/* room 0x10 (D_003F78F8): the stalker is there, not about, in mode 2, 6 or 7 */
s32 func_002AC040(void) {
    u8 *c = D_0044F808;
    u8 k;

    if (c == NULL || AT(c, 0x28, u8) != 0) {
        return 0;
    }
    k = AT(c, 0x153C, u8);
    return k == 2 || k == 6 || k == 7;
}

/* room 0x12 (D_003F9160): the pursuer is about, in a mode other than 0 and 3 */
s32 func_002AC2D0(void) {
    Character *s = gCharPursuer;

    return s != NULL && s->a.active != 0 && AT(s, 0xE8, s32) != 3 && AT(s, 0xE8, s32) != 0;
}

static void smoke_puffs_init(void **obj) {
    obj[0] = D_00470E20;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

/* room 0x12 (D_003F9150): byte 3 0: the smoke puffs (D_00470E20), their slot in script
 * variable 0; else that slot started */
s32 func_002AC370(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(D_0044E578, 0x1C60, smoke_puffs_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, slot);
    } else {
        func_002D6090(D_0044E578,
                      VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0), NULL);
    }
    return 1;
}

/* ---- rooms 0x15 .. 0x66 (second batch) ---- */

extern const char *const D_003FB370[];   /* room 0x18's objects */
extern const char *const D_0040C140[];   /* room 0x4F's objects */
extern const char *const D_0047ABEC;     /* room 0x5C's dial */
extern const char *const D_004123E8;     /* room 0x5D's lever */
extern const char *const D_004123D0[];   /* room 0x5D's (+2: four objects) */
extern s32 D_0047B250;                   /* room 0x4C: what the player has done so far */
extern void *D_00476BB0[], *D_00472370[];
extern u8 *D_0044F258;                   /* the creatures: 7 pointers; +0x28 vtable */
extern VObject *D_00456E00;
extern VObject *D_0044E558;              /* the doors */
extern VObject *D_0044E550;              /* random numbers */
extern void func_0025F810(u8 *o);
extern f32 func_00124490(void *a, f32 *p);
extern s32 func_00183190(void *f);

/* room effect 0x1F's colour pulsing with script variable `var` (0..0x60 round): three channels
 * a third of the way apart, each 8 x (distance from the middle - 16), then the rest */
static inline __attribute__((always_inline)) void colour_pulse(s32 var, u8 c3, u8 c4, u8 c5, u8 c6, u8 c7) {
    VObject *ev = D_0044E4D0;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, var);
    u8 c[9];
    s32 k;

    for (k = 0; k < 3; k++) {
        s32 d = 0x30 - (v + k * 0x20) % 0x60;

        if (d <= 0) {
            d = -d;
        }
        d = d < 0x11 ? 0 : d - 0x10;
        d <<= 3;
        c[k] = d < 0x100 ? d : 0xFF;
    }
    c[3] = c3;
    c[7] = c7;
    c[4] = c4;
    c[5] = c5;
    c[6] = c6;
    c[8] = 0;
    func_00266C70(D_0044E4C0, 0x1F, c);
    v++;
    if ((u32)v > 0x60) {
        v = 0;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, var, v);
}

/* room 0x15 (D_003FA738): Hewie is about in room 0xF in state 0x2F or 0x52 */
s32 func_002ACC90(void) {
    Character *c = gCharacters[(u8)func_001770D0(gProgress, 1)];
    s32 s;

    if (c == NULL || c->a.active == 0 || AT(c, 0x30, s32) != 0xF) {
        return 0;
    }
    s = AT(c, 0xF3564, s32);
    return s == 0x2F || s == 0x52;
}

/* room 0x18 (D_003FB358): object byte 3's func_0025F810 */
s32 func_002AD160(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_003FB370[cmd[3]]);

    if (o != NULL) {
        func_0025F810(o);
    }
    return 1;
}

/* room 0x19 (D_003FC010): the pursuer (about, not in state 2, in mode 2, 6 or 7) while Hewie is
 * controlled: in another room, or 30 or more from the player */
s32 func_002AD2F0(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;
    Progress *g;
    u8 k;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    g = gProgress;
    if ((u8)func_00177620(g) != 2) {
        return 0;
    }
    if (AT(s, 0x30, s32) != VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)) {
        return 1;
    }
    return !(func_00124490(&s->a, p->a.pos) < 30.0f);
}

/* room 0x1C (D_003FD338): room effect 0x1C (a depth range) with the cutscene from frame 0x14A:
 * near 1 .. 1 + 1.4 t (at most 67.6), far 48.6 + 4 t (at most 230) */
s32 func_002ADB70(void) {
    f32 t = (f32)(VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 0x14A);
    f32 r[4];
    f32 v;

    room_effect_new(D_0044E4C0, 0x1C, D_0046EC60);
    r[0] = 1.0f;
    v = 1.0f + 0x1.6666660000000p+0f /* 1.4 */ * t;
    r[1] = v <= 0x1.0e66660000000p+6f /* 67.6 */ ? v : 0x1.0e66660000000p+6f /* 67.6 */;
    r[2] = v <= 0x1.0e66660000000p+6f /* 67.6 */ ? v : 0x1.0e66660000000p+6f /* 67.6 */;
    v = 0x1.84cccc0000000p+5f /* 48.6 */ + 4.0f * t;
    r[3] = v <= 230.0f ? v : 230.0f;
    func_00266C70(D_0044E4C0, 0x1C, r);
    return 1;
}

/* room 0x1D (D_003FD990): door 0's +0x74 (0, or -0.08 by byte 3) */
s32 func_002ADEE0(void *self, void *a1, u8 *cmd) {
    VCALL(D_0044E558, 0x74, void (*)(VObject *, s32, f32))(D_0044E558, 0, cmd[3] == 0 ? 0.0f : -0x1.47ae140000000p-4f /* 0.08 */);
    return 1;
}

/* room 0x1D (D_003FD980): script variable 0 = 2 .. 5 at random */
s32 func_002ADF40(void) {
    s32 r = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 3;

    VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, r + 2);
    return 1;
}

/* (D_004022E0's table) room effect 0x1F's colour pulsing by script variable 0 */
s32 func_002AFD60(void) {
    colour_pulse(0, 0x50, 0x24, 0x2A, 0x2A, 0x46);
    return 1;
}

/* room 0x4C (D_0040ABF0): byte 3 0 starts counting what the player does (her +0x1AD710 on), 1
 * adds this frame's (func_00183190); at 35 events bit 0x13 */
s32 func_002B3720(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        D_0047B250 = 0;
        AT(gCharPlayer, 0x1AD710, u8) = 1;
        AT(gCharPlayer, 0x1AD714, s32) = 0;
        break;
    case 1:
        D_0047B250 += func_00183190(gCharPlayer);
        if (D_0047B250 >= 0x23) {
            VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 0x13);
        }
        break;
    }
    return 1;
}

static void effect_476bb0_init(void **obj) {
    obj[0] = D_00476BB0;
    obj[0x6010 / 4] = D_00469D00;
    ((s32 *)obj)[0x6014 / 4] = -1;
    obj[0x6010 / 4] = D_0046FC30;
}

/* room 0x4F (D_0040C0F8): an effect (D_00476BB0, 0x7460 bytes), not started */
s32 func_002B4480(void) {
    Effect_New(D_0044E578, 0x7460, effect_476bb0_init);
    return 1;
}

/* room 0x4F (D_0040C0E8): object byte 3 by byte 4: 0 up (+0x10 0), 1 down (-0.65), 2 lowered a
 * step (0.02, not during a movie); once down, events bit 3 */
s32 func_002B4570(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_0040C140[cmd[3]]);
    f32 a;

    switch (cmd[4]) {
    case 0:
        AT(o, 0x10, f32) = 0.0f;
        break;
    case 1:
        AT(o, 0x10, u32) = 0xBF266666;
        break;
    case 2:
        if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
            return 1;
        }
        a = AT(o, 0x10, f32) - 0x1.47ae140000000p-6f /* 0.02 */;
        AT(o, 0x10, f32) = a;
        if (a <= -0x1.4ccccc0000000p-1f /* 0.65 */) {
            AT(o, 0x10, f32) = -0x1.4ccccc0000000p-1f /* 0.65 */;
            VCALL(D_0044E4D0, 0x5C, void (*)(VObject *, s32))(D_0044E4D0, 3);
        }
        break;
    }
    return 1;
}

/* room 0x51 (D_0040E340): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves */
s32 func_002B49F0(void *self, void *a1, u8 *cmd) {
    if (cmd[3] != 0) {
        return VCALL(D_0044E558, 0x30, s32 (*)(VObject *, s32))(D_0044E558, 0) != 0 ? 1 : 2;
    }
    VCALL(D_0044E558, 0xC, void (*)(VObject *, s32, s32, s32, s32))(D_0044E558, 0, 1, 0, 0);
    return 1;
}

/* room 0x56 (D_0040F468): creatures 7..9 in the current room on a live triangle D_00456E00
 * says yes to: +0x10, then the creature list's +0x28 */
s32 func_002B4DE0(void) {
    u8 *list = D_0044F258;
    Progress *g = gProgress;
    VObject *chk = D_00456E00;
    s32 k;

    for (k = 7; k < 10; k++) {
        VObject *c = AT(list, 0x1C + (k - 7) * 4, VObject *);

        if (c == NULL || AT(c, 0x30, s32) != VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)
            || AT(c, 0x34, s32) == -1
            || (u8)VCALL(chk, 0x14, s32 (*)(VObject *, s32, s32))(chk, AT(c, 0x34, s32), 0) != 1) {
            continue;
        }
        VCALL(c, 0x10, void (*)(VObject *))(c);
        VCALL_AT(list, 0x28, 0x28, void (*)(u8 *, s32))(list, k & 0xFF);
    }
    return 1;
}

/* room 0x59 (D_00410B80): character 0xFE's model +0x9E0 = 0.1 (byte 3 0) or 0 */
s32 func_002B5210(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9E0, f32) = 0x1.99999a0000000p-4f /* 0.1 */;
    } else {
        AT(m, 0x9E0, f32) = 0.0f;
    }
    return 1;
}

static void effect_472370_init(void **obj) {
    obj[0] = D_00472370;
    obj[0x370 / 4] = D_00469D00;
    ((s32 *)obj)[0x374 / 4] = -1;
    obj[0x370 / 4] = D_0046FC30;
}

/* room 0x5C (D_00411B50): the first creature within 3 of (59.1, 1.43) is put away with an
 * effect (D_00472370) above it - blue (+0x1571 below 0x12) or red - and its action 0x8B */
s32 func_002B59E0(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        Character *c = AT(D_0044F258, i * 4, Character *);
        f32 dz, dx;

        if (c == NULL) {
            continue;
        }
        dz = c->a.pos[2] - 0x1.6e147a0000000p+0f /* 1.43 */;
        dx = c->a.pos[0] - 0x1.d8cccc0000000p+5f /* 59.1 */;
        if (dz * dz + dx * dx < 9.0f) {
            struct {
                f32 pos[4];
                u8 col[4];
            } prm __attribute__((aligned(16)));
            s32 slot;

            c->a.active = 0;
            {
                /* (Effect_New, with the heap's +0x10 handed the loop index in a2 as the
                   original leaves it there - unused) */
                u8 *mgr = D_0044E578;
                void *mem = VCALL(EFFECT_HEAP(mgr), 0x10, void *(*)(VObject *, u32, s32))(EFFECT_HEAP(mgr), 0x4A0, i);
                s32 j;

                slot = -1;
                if (mem != NULL) {
                    for (j = 0; j < EFFECT_NUM_SLOTS; j++) {
                        if (EFFECT_SLOTS(mgr)[j] == NULL) {
                            void **obj = func_002D63C0(0x4A0, mem);

                            if (obj != NULL) {
                                effect_472370_init(obj);
                            }
                            EFFECT_SLOTS(mgr)[j] = obj;
                            VCALL(EFFECT_SLOTS(mgr)[j], 0xC, void (*)(void **))(EFFECT_SLOTS(mgr)[j]);
                            slot = j;
                            break;
                        }
                    }
                }
            }
            sceVu0CopyVector(prm.pos, c->a.pos);
            prm.pos[1] = 12.0f + c->a.pos[1] + AT(c, 0x1554, f32);
            if (AT(c, 0x1571, u8) < 0x12) {
                prm.col[2] = 0x80;
                prm.col[0] = 0x30;
                prm.col[3] = 0x60;
                prm.col[1] = 0x30;
            } else {
                prm.col[0] = 0x80;
                prm.col[1] = 0x30;
                prm.col[3] = 0x60;
                prm.col[2] = 0x30;
            }
            func_002D6090(D_0044E578, slot, &prm);
            func_00122C20(&c->a, 0x8B, 5, 0, 0, NULL);
            return 1;
        }
    }
    return 0;
}

/* room 0x5C (D_00411B40): the dial (+0x7C, 0..1) from script variable 0 by byte 3: 0 0x2B..0x38
 * (/ 13, +0x74 0 / +0x78 1), 1 0xC..0x20 (/ 20), 2 7..0x12 (/ 11) (+0x74 1 / +0x78 0) */
s32 func_002B5C30(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_0047ABEC);
    u32 v;

    if (o == NULL) {
        return 1;
    }
    v = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0);
    switch (cmd[3]) {
    case 0:
        v = v < 0x2B ? 0x2B : v;
        v = v < 0x39 ? v : 0x38;
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(v - 0x2B) / 13.0f;
        break;
    case 1:
        v = v < 0xC ? 0xC : v;
        v = v < 0x21 ? v : 0x20;
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(v - 0xC) / 20.0f;
        break;
    case 2:
        v = v < 7 ? 7 : v;
        v = v < 0x13 ? v : 0x12;
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(v - 7) / 11.0f;
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

/* room 0x5D (D_004123C0): the lever at -60 / 0 / 60 degrees by byte 3 */
s32 func_002B5FA0(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_004123E8);

    if (o != NULL) {
        switch (cmd[3]) {
        case 0:
            AT(o, 0x14, u32) = 0xBF860A92;
            break;
        case 2:
            AT(o, 0x14, u32) = 0x3F860A92;
            break;
        default:
            AT(o, 0x14, s32) = 0;
            break;
        }
    }
    return 1;
}

/* room 0x5D (D_004123B0): four objects 60 to the left */
s32 func_002B6040(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *o = room_obj(D_004123D0[i + 2]);

        if (o != NULL) {
            AT(o, 0x20, f32) = AT(o, 0x20, f32) - 60.0f;
        }
    }
    return 1;
}

/* room 0x2F (D_00412E80): the pursuer's func_0029A710 */
s32 func_002CCA80(void) {
    return func_0029A710(gCharPursuer);
}

/* room 0x30 (D_004134B0): the pursuer's func_0029A710 */
s32 func_002CCBC0(void) {
    return func_0029A710(gCharPursuer);
}

/* room 0x66 (D_0041F5B0): the character's x (byte 3 0) or z is at least be32 bytes 4..7 / 1000 */
s32 func_003002E0(void *self, u8 *chr, u8 *cmd) {
    f32 v = (f32)(s32)((u32)cmd[4] << 24 | (u32)cmd[5] << 16 | (u32)cmd[6] << 8 | cmd[7]) / 1000.0f;

    if (cmd[3] == 0) {
        return !(AT(chr, 0x10, f32) < v);
    }
    return !(AT(chr, 0x18, f32) < v);
}

/* room 0x66 (D_0041F5A0): in the eight letters of script variables 0 and 1, from the place in
 * variable 2: the next 'L' / 'R' - matched by byte 3 (0 'L', else 'R') is stepped over (1) */
s32 func_003003C0(void *self, void *a1, u8 *cmd) {
    VObject *ev = D_0044E4D0;
    s32 w[2];
    u8 *b = (u8 *)w;
    u32 i;
    s32 r = 0;

    w[0] = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
    w[1] = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);
    i = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2);
    for (; i < 8; i++) {
        if (b[i] == 'L' || b[i] == 'R') {
            break;
        }
    }
    if (i < 8) {
        if (cmd[3] == 0) {
            if (b[i] == 'L') {
                i++;
                r = 1;
            }
        } else if (b[i] == 'R') {
            i++;
            r = 1;
        }
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 2, i);
    return r;
}

/* room 0x66 (D_0041F590): room effect 0x1F's colour pulsing by script variable 9 */
s32 func_00300520(void) {
    colour_pulse(9, 0x5A, 0x44, 0x46, 0x52, 0x4B);
    return 1;
}

/* ---- room 0x66's fires, room 0x62's swinging object, a room creature class ---- */

extern void *D_00478B50[];
extern const char *const D_0047AD08[];   /* room 0x62's objects */
extern const PTMF D_00422348;            /* a creature state */
extern void *D_00471290[], *D_0046D810[], *D_0046C220[], *D_00469C60[];
extern void func_00124E40(Actor *a);

static void fire_init(void **obj) {
    obj[0] = D_00478B50;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

/* room 0x66 (D_0041F580): byte 4 0 lights a fire (D_00478B50, kind byte 3), its slot in script
 * variable 8; else that fire put out (-1) */
s32 func_00300650(void *self, void *a1, u8 *cmd) {
    if (cmd[4] == 0) {
        s32 slot = Effect_New(D_0044E578, 0x1C60, fire_init);
        s32 prm[4] = {0, 0, 0, 0};   /* (zeroed past the two words, as the original's stack) */

        prm[0] = cmd[3];
        prm[1] = 1;
        func_002D6090(D_0044E578, slot, prm);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 8, slot);
    } else {
        s32 off[4] = {-1, 0, 0, 0};

        func_002D6090(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 8), off);
    }
    return 1;
}

/* room 0x66 (D_0041F570): character 8 sinks 0.1 a frame: a fire (kind byte 3) as it starts
 * (slot in variable 7), put out below -24; done (1) below -25, else wait (2) */
s32 func_003007E0(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[(u8)func_001770D0(gProgress, 8)];
    f32 y = c->a.pos[1] - 0x1.99999a0000000p-4f /* 0.1 */;

    c->a.pos[1] = y;
    if (y <= -0x1.99999a0000000p-3f /* 0.2 */) {
        if (y < -24.0f) {
            VObject *ev = D_0044E4D0;
            s32 s = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 7);

            if (s >= 0) {
                s32 off[4] = {-1, 0, 0, 0};

                func_002D6090(D_0044E578, s, off);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 7, -1);
            }
        }
    } else {
        s32 slot = Effect_New(D_0044E578, 0x1C60, fire_init);
        s32 prm[4] = {0, 0, 0, 0};   /* (zeroed past the two words, as the original's stack) */

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 7, slot);
        prm[0] = cmd[3];
        prm[1] = 0;
        func_002D6090(D_0044E578, slot, prm);
    }
    return c->a.pos[1] < -25.0f ? 1 : 2;
}

/* room 0x62 (D_00422300): object byte 4 swings: byte 3 0 starts it (phase +0x30 0, size +0x34
 * 0.01; object 0 with sound 7), else a step (+0x10 = size x sin(phase), phase on 60 degrees,
 * the size down 0.001); 2 until it is still */
s32 func_00308D70(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_0047AD08[cmd[4]]);

    if (cmd[3] == 0) {
        AT(o, 0x30, f32) = 0.0f;
        AT(o, 0x34, u32) = 0x3C23D70A;   /* 0.01 */
        if (cmd[4] == 0) {
            func_002FF650(D_0044E560, 7, 6, (f32 *)(o + 0x20), 0, 0);
        }
        return 1;
    }
    AT(o, 0x10, f32) = AT(o, 0x34, f32) * func_0031C248(0x1.921fb6p+2f /* 2 pi */ * AT(o, 0x30, f32) / 360.0f);
    AT(o, 0x30, f32) = AT(o, 0x30, f32) + 60.0f;
    AT(o, 0x34, f32) = AT(o, 0x34, f32) - 0x1.0624de0000000p-10f /* 0.001 */;
    return AT(o, 0x34, f32) <= 0.0f ? 1 : 2;
}

/* ---- creature class D_00471290 (a pursuer-like character) ---- */

/* +0x8 destructor (0x471290 -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the model freed
 * for slots 3..5 */
Character *func_00308EC0(Character *c, s32 flags) {
    if (c != NULL) {
        c->a.vtbl = D_00471290;
        c->a.vtbl = D_0046D810;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = D_0046C220;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = D_00469C60;
        c->a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(&c->a);
        }
    }
    return c;
}

/* a state: +0x114 5, the state D_00422348 at +0x174C, +0x1758 -1, then +0x260 */
void func_00308FF0(Character *c) {
    VCALL(c, 0x114, void (*)(Character *, s32))(c, 5);
    ptmf_set(&AT(c, 0x174C, PTMF), &D_00422348);
    AT(c, 0x1758, s32) = -1;
    VCALL(c, 0x260, void (*)(Character *))(c);
}

/* each frame: its state (+0xA0), then back to +0x114 5 unless already (+0x175C) */
void func_00309080(Character *c) {
    if (ptmf_test(&AT(c, 0xA0, PTMF))) {
        ptmf_scall(c, &AT(c, 0xA0, PTMF));
    }
    if (AT(c, 0x175C, s32) != 5) {
        VCALL(c, 0x114, void (*)(Character *, s32))(c, 5);
    }
}

/* ---- rooms 0x48 / 0x60 ---- */

extern void func_00124F20(void *c, s32 a);

/* room 0x48 (D_00426850): door 0 of room 0x48 (Progress_CurRoomFlag) */
s32 func_0030F1A0(void) {
    return Progress_CurRoomFlag(gProgress, 0x48, 0);
}

/* room 0x48 (D_00426840): the creatures (10) in play in the current room: the list's +0x2C */
s32 func_0030F1F0(void) {
    u8 *list = D_0044F258;
    Progress *g = gProgress;
    s32 i;

    for (i = 0; i < 10; i++) {
        u8 *c = AT(list, i * 4, u8 *);

        if (c != NULL && AT(c, 0x28, u8) == 1
            && AT(c, 0x30, s32) == VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)) {
            VCALL_AT(list, 0x28, 0x2C, void (*)(u8 *, s32, s32))(list, i & 0xFF, 0);
        }
    }
    return 1;
}

/* room 0x48 (D_00426830): the player's func_00124F20(0) */
s32 func_0030F2B0(void) {
    func_00124F20(gCharPlayer, 0);
    return 1;
}

/* room 0x48 (D_00426820): character 0xFE's model +0x9E8 = -0.15 (byte 3 0) or 0 */
s32 func_0030F2E0(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9E8, u32) = 0xBE19999A;
    } else {
        AT(m, 0x9E8, f32) = 0.0f;
    }
    return 1;
}

/* room 0x48 (D_00426800): character 0x26's +0xE4 cleared */
s32 func_0030F490(void) {
    AT(gCharacters[(u8)func_001770D0(gProgress, 0x26)], 0xE4, u8) = 0;
    return 1;
}

/* room 0x60 (D_00429190): the player's model +0xCC 0 (byte 3 0) or 1 */
s32 func_00310280(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    VCALL(m, 0xCC, void (*)(VObject *, s32))(m, cmd[3] == 0 ? 0 : 1);
    return 1;
}

/* room 0x60 (D_00429170): character 0xFE's model +0x9FC 0.4 / +0xA00 1 (byte 3 0), or 0 */
s32 func_00310450(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9FC, f32) = 0x1.99999a0000000p-2f /* 0.4 */;
        AT(m, 0xA00, u8) = 1;
    } else {
        AT(m, 0x9FC, f32) = 0.0f;
        AT(m, 0xA00, u8) = 0;
    }
    return 1;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern s32 D_0047B2C0;
extern s32 D_0047B2D8;
extern s32 D_0047B2E8;

/* as func_002E6E20 */
s32 func_0036E1E0(void) {
    Effect_New(D_0044E578, 0xC0, effect_C0_init);
    return 1;
}

/* as func_002E7560 */
s32 func_0036ED30(void *self, void *a1, u8 *cmd) { return nudge(&D_0047B2C0, cmd, 1.0f); }

/* (as func_002E7600)  character kind 0x1A: byte 3 0 starts func_0032D270(2, -6, 257); else waits (2) until
 * func_0032D150 says done */
s32 func_0036EDD0(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[func_001770D0(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        func_0032D270(c, 2, -6.0f, 257.0f);
        return 1;
    }
    return func_0032D150(c) == 0 ? 2 : 1;
}

/* as func_002E7020 */
s32 func_0036F5C0(void) {
    Effect_New(D_0044E578, 0x10, effect_10_init);
    return 1;
}

/* as func_002E70F0 */
s32 func_0036F7E0(void *self, void *a1, u8 *cmd) { return nudge(&D_0047B2D8, cmd, 2.0f); }

/* as func_002E7560 */
s32 func_0036FBA0(void *self, void *a1, u8 *cmd) { return nudge(&D_0047B2E8, cmd, 1.0f); }

/* (as func_0030F2B0)  room 0x48 (D_00426830): the player's func_00124F20(0) */
s32 func_0036FE40(void) {
    func_00124F20(gCharPlayer, 0);
    return 1;
}

/* (as func_0030F2B0)  room 0x48 (D_00426830): the player's func_00124F20(0) */
s32 func_00378A00(void) {
    func_00124F20(gCharPlayer, 0);
    return 1;
}

/* (as func_002AF300)  room 0x21 (D_00400BC8) */
s32 func_00378B80(void *self, void *a1, u8 *cmd) {
    return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
}

/* (as func_002AF4E0)  room 0x21 (D_00400BA8) */
s32 func_00378D20(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* (as func_0036A400)  room 0x37 (D_004469A0): the fan turns, except while a movie plays */
s32 func_00378EC0(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(*(const char *const *)D_0047B0C0);   /* (the first of its names) */
    return 1;
}

/* ---- two more room effects (as func_002E7020 / func_002E6E20) ---- */

extern void *D_0047A410[], *D_00479A60[];

static void effect_47a410_init(void **obj) {
    obj[0] = D_0047A410;
}

static void effect_479a60_init(void **obj) {
    obj[0] = D_00479A60;
    obj[0x550 / 4] = D_00469D00;
    ((s32 *)obj)[0x554 / 4] = -1;
    obj[0x550 / 4] = D_0046FC30;
}

/* an effect D_0047A410 (8 bytes), not started */
s32 func_0034B6E0(void) {
    Effect_New(D_0044E578, 0x8, effect_47a410_init);
    return 1;
}

/* an effect D_00479A60 (0x640 bytes, its quad drawer at +0x550), not started */
s32 func_0032DF20(void) {
    Effect_New(D_0044E578, 0x640, effect_479a60_init);
    return 1;
}

/* (as func_002ABC20) an effect D_00471060 with its box */
s32 func_0032DDF0(void) {
    s32 slot = Effect_New(D_0044E578, 0x840, effect_471060_init);
    f32 prm[9];

    prm[1] = -33.0f;
    prm[2] = 2.0f;
    prm[0] = 0.0f;
    prm[3] = -165.0f;
    prm[6] = 0.0f;
    prm[4] = 50.0f;
    prm[8] = 0.0f;
    prm[5] = 100.0f;
    prm[7] = -180.0f;
    func_002D6090(D_0044E578, slot, prm);
    return 1;
}

extern const char *const D_00429130;

/* room 0x60 (as func_002AFA00, for character 0xFE): byte 3 0 a progress name, 1 wait for
 * character 0xFE (2 while not), else done */
s32 func_00310640(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_00429130);
        return 1;
    case 1:
        return func_0016CD60(gProgress, 0xFE, 0) == 0 ? 2 : 1;
    }
    func_0016CD30(gProgress);
    return 1;
}

extern const char *D_0047B0FC;   /* a room object's name */

/* (as func_002AC790) the same for the room object D_0047B0FC */
s32 func_00379F00(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(D_0047B0FC, cmd);
}

extern const char *D_00449540;   /* a room object's name */

/* (as func_002ABDD0) the room object D_00449540 by event variable 2 */
s32 func_00379AF0(void *self, void *a1, u8 *cmd) {
    return var_fade(D_00449540, 2, cmd);
}

/* (as func_00378D20) the window quad lit with flags 0x40 */
s32 func_0031E2C0(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x40);
}

extern void *D_0047A3F0[];

static void effect_7A3F0_init(void **obj) {
    obj[0] = D_0047A3F0;
}

/* (as func_0030F350) byte 3 0 starts the effect D_0047A3F0 (its slot in event variable 9);
   else that one is ended (func_002D6170) */
s32 func_00339F00(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(D_0044E578, 0x10, effect_7A3F0_init);

        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 9, slot);
    } else {
        func_002D6170(D_0044E578, VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 9));
    }
    return 1;
}

/* (as func_002A8BA0) the same four spots for bytes 3..6 */
s32 func_0037AFB0(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 3);
}

/* (as func_002B1A00) three hanging things (+0x34 0..2), pushed by the square of Fiona's step
   past 1, event flag 4 with sounds 4 / 5 */
s32 func_00352F50(VObject *self, void *a1, u8 *cmd) {
    return hangers_swing(self, cmd, 0, 3, 1.0f, 0, 4, 4, 5);
}

extern void *D_004799D0[], *D_0047A430[], *D_00479F50[];

static void effect_799d0_init(void **obj) {
    obj[0] = D_004799D0;
}

static void effect_7a430_init(void **obj) {
    obj[0] = D_0047A430;
}

/* (as func_002B11D0) the 0xD40-byte effect D_004799D0 started with byte 3 */
s32 func_0033A0B0(void *self, void *a1, u8 *cmd) {
    u8 *mgr = D_0044E578;
    u8 b = cmd[3];

    func_002D6090(mgr, Effect_New(mgr, 0xD40, effect_799d0_init), &b);
    return 1;
}

/* (as func_002B11D0) the 0x14-byte effect D_0047A430 started with byte 3 as a word */
s32 func_0034A250(void *self, void *a1, u8 *cmd) {
    u8 *mgr = D_0044E578;
    s32 w = cmd[3];

    func_002D6090(mgr, Effect_New(mgr, 0x14, effect_7a430_init), &w);
    return 1;
}

static void effect_1c60_init(void **obj) {
    obj[0] = D_00479F50;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

/* (as func_002B2A80) byte 3 0: the effect D_00479F50 spawned (told 1), its slot in event var
   0; 1: it is told 0 */
s32 func_00342A30(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0: {
        u8 *mgr = D_0044E578;
        s32 slot = Effect_New(mgr, 0x1C60, effect_1c60_init);

        func_002D6090(mgr, slot, (void *)1);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, slot);
        break;
    }
    case 1: {
        s32 slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 0);

        if (slot != -1) {
            func_002D6090(D_0044E578, slot, NULL);
        }
        break;
    }
    }
    return 1;
}

extern Character *gCharacters[];

/* (as func_002E69B0) pushed by the character slot byte 4 names */
s32 func_0036F150(VObject *self, void *a1, u8 *cmd) {
    return swing_three_by(self, cmd, 1);
}

/* (as func_002AFAA0)  the kind-0xB character's model +0xCC8: 0 (byte 3 1) or -0.02 */
s32 func_0033A040(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xB)]->motion;

    if (cmd[3] == 1) {
        AT(m, 0xCC8, s32) = 0;
    } else {
        AT(m, 0xCC8, u32) = 0xBCA3D70A;   /* -0.02 */
    }
    return 1;
}

/* script variable byte 3 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0 */
static inline s32 var_down_by_hit(u8 *cmd) {
    VObject *ev = D_0044E4D0;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[3]);
    s32 k = AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;

    if (k > 0) {
        v -= k;
        if (v < 0) {
            v = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[3], v);
    }
    return 1;
}

/* (as func_002AB580, the player only) */
s32 func_0033A8E0(void *self, void *a1, u8 *cmd) {
    return var_down_by_hit(cmd);
}

/* (as func_0033A8E0) */
s32 func_0033FFC0(void *self, void *a1, u8 *cmd) {
    return var_down_by_hit(cmd);
}

/* the player's model tint: white (byte 3 0) or blue halved */
s32 func_00320F70(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xC0, void (*)(VObject *, f32, f32, f32))(m, 1.0f, 1.0f, 1.0f);
    } else {
        VCALL(m, 0xC0, void (*)(VObject *, f32, f32, f32))(m, 1.0f, 1.0f, 0.5f);
    }
    return 1;
}

extern void *D_004795C0[];

static void effect_795C0_init(void **obj) {
    obj[0] = D_004795C0;
}

/* (as func_00300A20)  byte 4 0 starts the 8-byte effect D_004795C0 (parameters from byte 3), its
 * slot kept in event variable byte 3 + 2; else that effect is ended */
s32 func_003104D0(void *self, void *a1, u8 *cmd) {
    if (cmd[4] == 0) {
        u8 *mgr = D_0044E578;
        s32 slot = Effect_New(mgr, 8, effect_795C0_init);

        func_002D6090(mgr, slot, cmd + 3);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, (cmd[3] + 2) & 0xFF, slot);
    } else {
        s32 slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, (cmd[3] + 2) & 0xFF);

        func_002D6170(D_0044E578, slot);
    }
    return 1;
}

/* (as func_002B2BF0)  the depth range (effect 0x1C) from cutscene frame 825: 1 / 1 + 0.3 a frame
 * up to 21 / 80 + 0.7 a frame up to 120 / 160 */
s32 func_003420A0(void) {
    static const union { u32 u; f32 f; } kNear = {0x3E999999}, kFar = {0x3F333333};
    u8 *fx = D_0044E4C0;
    f32 t = (f32)(VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 825);
    f32 r[4] __attribute__((aligned(16)));

    room_effect_new(fx, 0x1C, D_0046EC60);
    r[0] = 1.0f;
    r[1] = 1.0f + kNear.f * t;
    if (!(r[1] <= 21.0f)) {
        r[1] = 21.0f;
    }
    r[2] = 80.0f + kFar.f * t;
    if (!(r[2] <= 120.0f)) {
        r[2] = 120.0f;
    }
    r[3] = 160.0f;
    func_00266C70(fx, 0x1C, r);
    return 1;
}

extern const char *D_004365E4, *D_004365E8;

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for func_0016CD60(0, 0); else func_0016CD30 */
s32 func_00341300(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_004365E4);
        return 1;
    case 1:
        func_0016CEC0(gProgress, D_004365E8);
        return 1;
    case 2:
        return func_0016CD60(gProgress, 0, 0) == 0 ? 2 : 1;
    }
    func_0016CD30(gProgress);
    return 1;
}

/* ---- the countdown clock (progress +0xFC4 time up, +0xFC5 / +0xFC6 minutes / seconds) ----
 * Each room's frame hook draws it at (431, 395) through the event manager's text +0x78: "59:59"
 * (and the flag pinned to 1) while time is up, else "MM:SS" built in its own buffer. */
static inline s32 clock_draw(char *buf, const char *full) {
    u8 *p = (u8 *)gProgress;
    VObject *ev;

    if (p[0xFC4] > 0) {
        ev = D_0044E4D0;
        VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
            ev, 0x1AF, 0x18B, 0, full, 0x80, 0x30, 0x10, 0x15);
        p[0xFC4] = 1;
        return 1;
    }
    buf[0] = p[0xFC5] / 10 + '0';
    buf[2] = ':';
    buf[1] = p[0xFC5] % 10 + '0';
    buf[3] = p[0xFC6] / 10 + '0';
    buf[5] = 0;
    buf[4] = p[0xFC6] % 10 + '0';
    ev = D_0044E4D0;
    VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
        ev, 0x1AF, 0x18B, 0, buf, 0x80, 0x30, 0x10, 0x15);
    return 1;
}

extern char D_0047B298[], D_0047B2A0[], D_0047B2A8[], D_0047B2B0[], D_0047B2B8[], D_0047B2C8[], D_0047B2D0[], D_0047B2E0[], D_0047B2F0[], D_0047B2F8[], D_0047B300[], D_0047B308[], D_0047B310[], D_0047B318[], D_0047B320[], D_0047B328[], D_0047B330[], D_0047B338[];
extern const char D_004636E0[], D_004636E8[], D_004636F0[], D_004636F8[], D_00463700[], D_00463720[], D_00463728[], D_00463730[], D_00463738[], D_00463928[], D_00463930[], D_00463938[], D_00463958[], D_004639B8[], D_004639E0[], D_00463A18[], D_00463A38[], D_00463A40[];   /* "59:59" */

s32 func_0036E090(void) { return clock_draw(D_0047B298, D_004636E0); }
s32 func_0036E400(void) { return clock_draw(D_0047B2A0, D_004636E8); }
s32 func_0036E6B0(void) { return clock_draw(D_0047B2A8, D_004636F0); }
s32 func_0036E930(void) { return clock_draw(D_0047B2B0, D_004636F8); }
s32 func_0036EBE0(void) { return clock_draw(D_0047B2B8, D_00463700); }
s32 func_0036F000(void) { return clock_draw(D_0047B2C8, D_00463720); }
s32 func_0036F690(void) { return clock_draw(D_0047B2D0, D_00463728); }
s32 func_0036FA50(void) { return clock_draw(D_0047B2E0, D_00463730); }
s32 func_0036FE70(void) { return clock_draw(D_0047B2F0, D_00463738); }
s32 func_00378A30(void) { return clock_draw(D_0047B2F8, D_00463928); }
s32 func_00379140(void) { return clock_draw(D_0047B300, D_00463930); }
s32 func_003793F0(void) { return clock_draw(D_0047B308, D_00463938); }
s32 func_003796D0(void) { return clock_draw(D_0047B310, D_00463958); }
s32 func_003799A0(void) { return clock_draw(D_0047B318, D_004639B8); }
s32 func_00379DB0(void) { return clock_draw(D_0047B320, D_004639E0); }
s32 func_0037A450(void) { return clock_draw(D_0047B328, D_00463A18); }
s32 func_0037A780(void) { return clock_draw(D_0047B330, D_00463A38); }
s32 func_0037AA30(void) { return clock_draw(D_0047B338, D_00463A40); }

/* ---- the slam shake: frame hooks of Lorenzo's (kind 0xA) rooms shake the camera (+0x6C, 0.5)
 * when his slam lands ---- */
extern s32 func_0030BB70(void *p);   /* is his slam at its impact key (lorenzo.c) */

static inline s32 slam_shake(void) {
    if (gCharPursuer == NULL || AT(gCharPursuer, 0x153C, u8) != 0xA) {
        return 1;
    }
    if (func_0030BB70(D_0044F808) != 0) {
        VCALL(D_0044E4B8, 0x6C, void (*)(VObject *, f32))(D_0044E4B8, 0.5f);
    }
    return 1;
}

s32 func_0032C7A0(void) { return slam_shake(); }
s32 func_0033F2B0(void) { return slam_shake(); }
s32 func_0033F460(void) { return slam_shake(); }
s32 func_0033F5F0(void) { return slam_shake(); }
s32 func_0033F7B0(void) { return slam_shake(); }
s32 func_003401A0(void) { return slam_shake(); }
s32 func_00340F50(void) { return slam_shake(); }
s32 func_00341690(void) { return slam_shake(); }
s32 func_00343830(void) { return slam_shake(); }
s32 func_00350F60(void) { return slam_shake(); }
s32 func_00352C00(void) { return slam_shake(); }
s32 func_00352D90(void) { return slam_shake(); }
s32 func_0035D2A0(void) { return slam_shake(); }
s32 func_0035D430(void) { return slam_shake(); }
s32 func_0035D5C0(void) { return slam_shake(); }
s32 func_0035D750(void) { return slam_shake(); }

/* ---- the countdown's start / stop hooks ---- */
extern void func_00136620(void *h);   /* Hewie restarted (hewie.c) */
extern void func_002A76E0(u8 *p);     /* four bytes cleared */

/* the countdown on (+0x1FBEC1), Hewie restarted, the camera director +0x40 (14), the clock
 * (+0xFC4..) zeroed */
static inline s32 clock_start(void) {
    u8 *p = (u8 *)gProgress;

    p[0x1FBEC1] = 1;
    func_00136620(gCharPartner);
    VCALL(D_0044E4F8, 0x40, void (*)(VObject *, f32))(D_0044E4F8, 14.0f);
    func_002A76E0(p + 0xFC4);
    return 1;
}

/* the countdown off, the camera director +0x40 (-1) */
static inline s32 clock_stop(void) {
    AT(gProgress, 0x1FBEC1, u8) = 0;
    VCALL(D_0044E4F8, 0x40, void (*)(VObject *, f32))(D_0044E4F8, -1.0f);
    return 1;
}

s32 func_0036FFC0(void) { return clock_start(); }
s32 func_00378F70(void) { return clock_start(); }
s32 func_0036DEE0(void) { return clock_stop(); }
s32 func_0037B290(void) { return clock_stop(); }

/* byte 3 to the player's func_00124F20 while she's active */
s32 func_0034A650(void *self, void *a1, u8 *cmd) {
    if (gCharPlayer != NULL && AT(gCharPlayer, 0x28, u8) != 0) {
        func_00124F20(gCharPlayer, cmd[3]);
    }
    return 1;
}

/* the room object named by D_0047B110[0]: +0x24 -25.3, +0x34 0 */
s32 func_0037A5A0(void) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, *(const char **)D_0047B110);

    if (o != NULL) {
        AT(o, 0x24, u32) = 0xC1CA6666;   /* -25.3 */
        AT(o, 0x34, s32) = 0;
    }
    return 1;
}

extern const char *D_004400F0[];

/* sound 3 (bank 6) at the room object named D_004400F0[0] */
s32 func_0034AB40(void) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_004400F0[0]);

    if (o != NULL) {
        func_002FF650(D_0044E560, 3, 6, (f32 *)(o + 0x20), 0, 0);
    }
    return 1;
}

/* D_00479FB0 effect message: none sets +0x34 (done); else +0x2C its word 0, and below 2 in word 1
 * +0x30 30 */
void func_0036A530(u8 *o, s32 *m) {
    if (m == NULL) {
        AT(o, 0x34, u8) = 1;
        return;
    }
    AT(o, 0x2C, s32) = m[0];
    if (m[1] < 2) {
        AT(o, 0x30, s32) = 30;
    }
}

/* its set up: +0x34 0, +0x30 -1, ten random 0..15 from +0x4 */
void func_0036A7F0(u8 *o) {
    VObject *rnd;
    s32 i;

    AT(o, 0x34, u8) = 0;
    AT(o, 0x30, s32) = -1;
    rnd = D_0044E550;
    for (i = 0; i < 10; i++) {
        AT(o, 0x4 + i * 4, s32) = VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0xF;
    }
}

/* an effect message: +0xC its word 0 (word 1 set: +0x8 too); a 0 there becomes 3 / 3 and +0x10 */
void func_00370090(u8 *o, s32 *m) {
    if (m == NULL) {
        return;
    }
    AT(o, 0xC, f32) = AT(m, 0x0, f32);
    if (m[1] != 0) {
        AT(o, 0x8, f32) = AT(o, 0xC, f32);
    }
    if (AT(o, 0xC, f32) == 0.0f) {
        AT(o, 0xC, u32) = 0x40400000;   /* 3 */
        AT(o, 0x8, u32) = 0x40400000;
        AT(o, 0x10, u8) = 1;
    }
}

/* the placed things of kind 9 / 10 (+0x20) all reset (+0x28) */
s32 func_00342230(void) {
    VObject *list = D_0044F260;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *o = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, i);

        if (o != NULL && (u32)(AT(o, 0x20, s32) - 9) < 2) {
            AT(o, 0x28, u8) = 0;
        }
    }
    return 1;
}

/* the screen fade (renderer +0x90): byte 3 0 full 0x80; else clearing from cutscene frame 120
 * (16 a frame) */
s32 func_0032DD70(void *self, void *a1, u8 *cmd) {
    s32 a = 0x80;

    if (cmd[3] != 0) {
        a = 0x80 - ((VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10) - 120) << 4);
        if (a < 0) {
            a = 0;
        }
    }
    VCALL(D_0044E4F0, 0x90, s32 (*)(VObject *, u32))(D_0044E4F0, a << 24);
    return 1;
}

/* the countdown clock (+0xFC4..+0xFC6) saved in script variables 0..2 */
static inline s32 clock_save(void) {
    u8 *p = (u8 *)gProgress;
    VObject *ev = D_0044E4D0;

    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, p[0xFC4]);
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, p[0xFC5]);
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 2, p[0xFC6]);
    return 1;
}

s32 func_0036DE60(void) { return clock_save(); }
s32 func_0037B210(void) { return clock_save(); }

/* the pursuer is active and out of view: state 3 (+0xE8), or the camera's on-screen test
 * (+0xD4) fails */
s32 func_00340E90(void) {
    if (gCharPursuer == NULL || AT(gCharPursuer, 0x28, u8) == 0) {
        return 0;
    }
    if (AT(gCharPursuer, 0xE8, s32) == 3) {
        return 1;
    }
    if ((u8)VCALL(D_0044E4B8, 0xD4, s32 (*)(VObject *, void *))(D_0044E4B8, (u8 *)gCharPursuer + 0x10) == 0) {
        return 1;
    }
    return 0;
}

/* script variable 0 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0 */
s32 func_00341A40(void) {
    VObject *ev = D_0044E4D0;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);

    v -= AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;
    if (v < 0) {
        v = 0;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, v);
    return 1;
}

/* an effect set up: +0x4 a random angle (-pi..pi), +0x8 / +0xC 2, +0x10 on, +0x11 off */
void func_00371030(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 r;

    AT(o, 0x11, u8) = 0;
    r = VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550);
    AT(o, 0x4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    AT(o, 0x8, u32) = 0x40000000;   /* 2 */
    AT(o, 0xC, u32) = 0x40000000;
    AT(o, 0x10, u8) = 1;
}

/* script variables 7 / 8 (the player's spot) in 151..269 / 171..219 */
s32 func_00339E30(void) {
    VObject *ev = D_0044E4D0;
    u16 x = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 7);
    u16 z = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 8);

    return x >= 0x97 && x < 0x10E && z >= 0xAB && z < 0xDC;
}

extern void func_00177FA0(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f);

/* a noise at (-70 or 70 by byte 3, 14, 0): byte 4 0 / 1 / 2 kind 1 / 2 / 4 */
s32 func_003422B0(void *self, void *a1, u8 *cmd) {
    f32 pos[4] __attribute__((aligned(16)));
    u32 which;

    pos[0] = cmd[3] == 0 ? -70.0f : 70.0f;
    pos[1] = 14.0f;
    pos[2] = 0.0f;
    pos[3] = 1.0f;
    switch (cmd[4]) {
    case 2:
        which = 4;
        break;
    case 1:
        which = 2;
        break;
    case 0:
        which = 1;
        break;
    default:
        which = (u32)(unsigned long)a1;   /* never set on the PS2: the register still holds a1 */
        break;
    }
    func_00177FA0(gProgress, pos, which & 0xFF, 2, 5, 0, 10.0f);
    return 1;
}

/* the placed things of kinds 0, 2, 3, 5, 7 and 8 the event manager finds in area 0xB (+0x10):
 * their timer (+0xE4) to 300000 */
s32 func_0033FA60(void) {
    VObject *list = D_0044F260, *ev = D_0044E4D0;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *o = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, i);

        if (o == NULL) {
            continue;
        }
        switch (AT(o, 0x20, u32)) {
        case 0:
        case 2:
        case 3:
        case 5:
        case 7:
        case 8:
            if ((u8)VCALL(ev, 0x10, s32 (*)(VObject *, void *, s32, s32))(ev, o + 0x10, 0xB, -1) == 1) {
                AT(o, 0xE4, s32) = 300000;
            }
            break;
        }
    }
    return 1;
}

/* Hewie's +0x14C8 to script variable 2 (byte 3 0), or back from it (1; 0 there gives 10) */
s32 func_0033FB40(void *self, void *a1, u8 *cmd) {
    VObject *ev;

    switch (cmd[3]) {
    case 0:
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 2, AT(gCharPartner, 0x14C8, s32));
        break;
    case 1:
        ev = D_0044E4D0;
        if (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2) == 0) {
            AT(gCharPartner, 0x14C8, s32) = 10;
        } else {
            AT(gCharPartner, 0x14C8, s32) = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2);
        }
        break;
    }
    return 1;
}

extern s32 func_00365850(void *p);   /* (lorenzo.c) his slam at its impact key, second form */

/* (as slam_shake, both of Lorenzo's forms: 0xA and 0x27) */
s32 func_00342500(void) {
    if (gCharPursuer == NULL) {
        return 1;
    }
    if (AT(gCharPursuer, 0x153C, u8) == 0xA) {
        if (func_0030BB70(D_0044F808) != 0) {
            VCALL(D_0044E4B8, 0x6C, void (*)(VObject *, f32))(D_0044E4B8, 0.5f);
        }
    } else if (AT(gCharPursuer, 0x153C, u8) == 0x27) {
        if (func_00365850(D_0044F808) != 0) {
            VCALL(D_0044E4B8, 0x6C, void (*)(VObject *, f32))(D_0044E4B8, 0.5f);
        }
    }
    return 1;
}

/* door 0 swung by script variable 0: byte 3 0 sets it to -90; 1 opens it 10 degrees a step to 0
 * (doors +0x74), waiting (2) until there */
s32 func_00340C50(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *ev;
    s32 a;

    switch (cmd[3]) {
    case 0:
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 0, -90);
        return 1;
    case 1:
        ev = D_0044E4D0;
        a = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0) + 10;
        if (a >= 0) {
            a = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, a);
        VCALL(D_0044E558, 0x74, void (*)(VObject *, s32, f32))(D_0044E558, 0, kPi.f * (f32)a / 180.0f);
        return a < 0 ? 2 : 1;
    }
    return 1;
}

/* its update (func_00371030's): done (+0x11) stops it (0); else the angle (+0x4) turns on by
 * 50..70 degrees, and +0x8 eases toward +0xC by 0.1 */
s32 func_00370F00(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTenth = {0x3DCCCCCD};
    f32 r;

    if (AT(o, 0x11, u8) == 1) {
        return 0;
    }
    r = VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550);
    AT(o, 0x4, f32) = AT(o, 0x4, f32) + kPi.f * (50.0f + 20.0f * r) / 180.0f;
    AT(o, 0x4, f32) = func_002E2D00(AT(o, 0x4, f32));
    if (AT(o, 0x8, f32) != AT(o, 0xC, f32)) {
        if (AT(o, 0x8, f32) <= AT(o, 0xC, f32)) {
            AT(o, 0x8, f32) = AT(o, 0x8, f32) + kTenth.f;
            if (!(AT(o, 0x8, f32) <= AT(o, 0xC, f32))) {
                AT(o, 0x8, f32) = AT(o, 0xC, f32);
            }
        } else {
            AT(o, 0x8, f32) = AT(o, 0x8, f32) - kTenth.f;
            if (AT(o, 0x8, f32) < AT(o, 0xC, f32)) {
                AT(o, 0x8, f32) = AT(o, 0xC, f32);
            }
        }
    }
    return 1;
}

extern const char *D_00434888;

/* the room object named D_00434888 swung: byte 3 0 starts it (rest +0x30 from +0x20, phase
 * +0x34 0, amplitude +0x3C 1); 1 steps the phase back 60 degrees and the amplitude down 0.25,
 * height +0x28 = +0x38 + amplitude * sin, waiting (2) until it has died out */
s32 func_0033FE90(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3F860A92}, kNegPi = {0xC0490FDB}, k2Pi = {0x40C90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00434888);
    f32 a;

    switch (cmd[3]) {
    case 0:
        sceVu0CopyVector((f32 *)(o + 0x30), (f32 *)(o + 0x20));
        AT(o, 0x34, s32) = 0;
        AT(o, 0x3C, u32) = 0x3F800000;   /* 1 */
        return 1;
    case 1:
        a = AT(o, 0x34, f32) - kStep.f;
        AT(o, 0x34, f32) = a;
        if (a < kNegPi.f) {
            AT(o, 0x34, f32) = a + k2Pi.f;
        }
        AT(o, 0x3C, f32) = AT(o, 0x3C, f32) - 0.25f;
        AT(o, 0x28, f32) = AT(o, 0x38, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
        return AT(o, 0x3C, f32) <= 0.0f ? 1 : 2;
    }
    return 1;
}

#include "charaction.h"

extern void func_002A8410(void *o);         /* an action block reset */
extern void func_002EF9E0(void *o, f32 v);

/* the player (active, +0xE0 clear) put in action 0xB / 0x21 / 0xFF unless held (7); then
 * progress +0x7B8 gets 50 */
s32 func_0034ABB0(void) {
    CharAction act;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0 || AT(gCharPlayer, 0xE0, u8) != 0) {
        return 1;
    }
    func_002A8410(&act);
    act.state = 0xB;
    act.a = 0x21;
    act.b = 0xFF;
    if (AT(gCharPlayer, 0x14E8, s32) != 7) {
        char_set_action((u8 *)gCharPlayer, &act);
    }
    func_002EF9E0((u8 *)gProgress + 0x7B8, 50.0f);
    return 1;
}

extern const char *D_00437D48[];   /* room objects 10..15 */

/* the things that fell below -30: placed things of kind 9 are reset (+0x28) and each counts down
 * script variable 6 (and the event manager's +0x5C); room objects 10..15 that did get +0 set */
s32 func_003428D0(void) {
    VObject *ev = D_0044E4D0, *list, *objs;
    s32 n = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    s32 i;

    list = D_0044F260;
    for (i = 0; i < 0x80; i++) {
        u8 *o = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, i);

        if (o != NULL && AT(o, 0x20, s32) == 9 && AT(o, 0x14, f32) < -30.0f) {
            AT(o, 0x28, u8) = 0;
            VCALL(ev, 0x5C, void (*)(VObject *))(ev);
            n--;
        }
    }
    objs = D_00456DF8;
    for (i = 10; i < 16; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D48[i - 10]);

        if (AT(o, 0x24, f32) < -30.0f) {
            AT(o, 0x0, u8) = 1;
        }
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, n);
    return 1;
}

/* character 0xFE's model +0x9E0 / +0x9E4 / +0x9E8: byte 3 0 -0.2 / 0.2 / -0.2; 1 eases them by
 * script variable 6 (a step a call, waiting (2) for 60) to 0 / 0.3 / 0; else 0 / 0.3 / 0 */
s32 func_003102E0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD}, kN02 = {0xBE4CCCCD}, k01 = {0x3DCCCCCE},
                                          k03 = {0x3E99999A};
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;
    VObject *ev;
    s32 v;
    f32 a, b;

    if (cmd[3] == 0) {
        AT(m, 0x9E0, f32) = kN02.f;
        AT(m, 0x9E4, f32) = k02.f;
        AT(m, 0x9E8, f32) = kN02.f;
        return 1;
    }
    if (cmd[3] != 1) {
        AT(m, 0x9E0, s32) = 0;
        AT(m, 0x9E4, f32) = k03.f;
        AT(m, 0x9E8, s32) = 0;
        return 1;
    }
    ev = D_0044E4D0;
    v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    a = kN02.f + k02.f * (f32)v / 60.0f;
    b = k02.f + k01.f * (f32)v / 60.0f;
    AT(m, 0x9E0, f32) = a;
    AT(m, 0x9E4, f32) = b;
    AT(m, 0x9E8, f32) = a;
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, v + 1);
    return v + 1 < 61 ? 2 : 1;
}

extern const char *D_00435978;

/* the room object named D_00435978 falling over: byte 3 0 starts it (angle +0x30, speed +0x34
 * and acceleration +0x38 0, jerk +0x3C 0.005); 1 steps them, its tilt +0x10 = (1 - sin(90 -
 * angle)) * pi/2, waiting (2) until the angle reaches 90; 2 puts it down (sin(pi/2)) */
s32 func_00340AD0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00435978);
    f32 a;

    switch (cmd[3]) {
    case 0:
        AT(o, 0x30, s32) = 0;
        AT(o, 0x34, s32) = 0;
        AT(o, 0x38, s32) = 0;
        AT(o, 0x3C, u32) = 0x3BA3D70A;   /* 0.005 */
        return 1;
    case 1:
        AT(o, 0x38, f32) = AT(o, 0x38, f32) + AT(o, 0x3C, f32);
        AT(o, 0x34, f32) = AT(o, 0x34, f32) + AT(o, 0x38, f32);
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + AT(o, 0x34, f32);
        a = AT(o, 0x30, f32) < 90.0f ? AT(o, 0x30, f32) : 90.0f;
        AT(o, 0x10, f32) = (1.0f - func_0031C248(kPi.f * (90.0f - a) / 180.0f)) * kHalfPi.f;
        return AT(o, 0x30, f32) < 90.0f ? 2 : 1;
    case 2:
        AT(o, 0x10, f32) = func_0031C248(kHalfPi.f);
        return 1;
    }
    return 1;
}

extern const char *D_0042E408;

/* the room object named D_0042E408 swung about its rest (+0x30 from +0x20): byte 3 0 starts it
 * (phase +0x34 0, amplitude +0x3C 0.25); 1 steps the phase on 60 degrees and the amplitude down
 * 0.05, x +0x20 / z +0x28 = rest + amplitude * sin, waiting (2) until it has died out; 2 puts it
 * at (-16.5, -5.6) */
s32 func_0032E2A0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3F860A92}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB},
                                          kDecay = {0x3D4CCCCD};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0042E408);
    f32 a;

    switch (cmd[3]) {
    case 0:
        sceVu0CopyVector((f32 *)(o + 0x30), (f32 *)(o + 0x20));
        AT(o, 0x34, s32) = 0;
        AT(o, 0x3C, u32) = 0x3E800000;   /* 0.25 */
        return 1;
    case 1:
        a = AT(o, 0x34, f32) + kStep.f;
        AT(o, 0x34, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x34, f32) = a - k2Pi.f;
        }
        AT(o, 0x3C, f32) = AT(o, 0x3C, f32) - kDecay.f;
        AT(o, 0x20, f32) = AT(o, 0x30, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
        AT(o, 0x28, f32) = AT(o, 0x38, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
        return AT(o, 0x3C, f32) <= 0.0f ? 1 : 2;
    case 2:
        AT(o, 0x20, u32) = 0xC1840000;   /* -16.5 */
        AT(o, 0x28, u32) = 0xC0B340E1;   /* -5.6017 */
        return 1;
    }
    return 1;
}

extern const char *D_00437D40[];   /* room objects 8, 9 */

/* room objects 8 / 9 raised (+0x24 down 0.2 a call to 0) while the event manager's +0x58 test 4 /
 * 5 holds, else lowered back (up 0.4 a call to 0.7) */
s32 func_00342370(void) {
    static const union { u32 u; f32 f; } kDown = {0x3E4CCCCD}, kTop = {0x3F333333}, kUp = {0x3ECCCCCD};
    VObject *objs = D_00456DF8, *ev = D_0044E4D0;
    s32 k;

    for (k = 8; k < 10; k++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D40[k - 8]);
        u8 on = 0;
        f32 y;

        if (k == 8) {
            if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 4) == 1) {
                on = 1;
            }
        } else if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 5) == 1) {
            on = 1;
        }
        if (o == NULL) {
            continue;
        }
        y = AT(o, 0x24, f32);
        if (on == 1) {
            if (!(y <= 0.0f)) {
                y -= kDown.f;
                AT(o, 0x24, f32) = y;
                if (y < 0.0f) {
                    AT(o, 0x24, f32) = 0.0f;
                }
            }
        } else if (y < kTop.f) {
            y += kUp.f;
            AT(o, 0x24, f32) = y;
            if (!(y <= kTop.f)) {
                AT(o, 0x24, f32) = kTop.f;
            }
        }
    }
    return 1;
}

extern void *D_0047A2F0[];

static void effect_7A2F0_init(void **obj) {
    obj[0] = D_0047A2F0;
}

/* (as func_002B2A80)  byte 3 0: the 0x14-byte effect D_0047A2F0 spawned, its slot in script
 * variable 10; 1 / 2: it is sent (byte 4, byte 3 - 1) */
s32 func_00341EF0(void *self, void *a1, u8 *cmd) {
    f32 msg[2];
    s32 slot;

    switch (cmd[3]) {
    case 0:
        slot = Effect_New(D_0044E578, 0x14, effect_7A2F0_init);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 10, slot);
        break;
    case 1:
    case 2:
        slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 10);
        msg[0] = (f32)cmd[4];
        msg[1] = (f32)(cmd[3] - 1);
        func_002D6090(D_0044E578, slot, msg);
        break;
    }
    return 1;
}

extern s32 func_00183190(void *f);   /* Fiona's struggle shakes this frame (fiona.c) */

/* a struggle: byte 3 0 resets Fiona's shake tracking (+0x1AD710 / +0x1AD714); 1 adds her shakes
 * to script variable byte 4, with a grunt (voice 0x3D or 0x45 at random) when the cool-down
 * variable byte 6 is out (it then runs 45 / 60), and at 100 the event byte 5 (+0x5C) */
s32 func_003413C0(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    s32 v, n;

    switch (cmd[3]) {
    case 0:
        AT(gCharPlayer, 0x1AD710, u8) = 1;
        AT(gCharPlayer, 0x1AD714, s32) = 0;
        break;
    case 1:
        ev = D_0044E4D0;
        v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[4]);
        n = func_00183190(gCharPlayer);
        if (n != 0 && VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[6]) == 0) {
            if (VCALL(D_0044E550, 0x10, u32 (*)(VObject *))(D_0044E550) & 1) {
                func_00122C20(&gCharPlayer->a, 0x3D, 5, 0, 0, NULL);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[6], 45);
            } else {
                func_00122C20(&gCharPlayer->a, 0x45, 5, 0, 0, NULL);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[6], 60);
            }
        }
        n += v;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[4], n);
        if ((u32)n >= 100) {
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, cmd[5]);
        }
        break;
    }
    return 1;
}

extern void func_001267F0(void *c, s32 light);

/* the screen fade (renderer +0x70) by script variable 1 with Hewie's light 0xF: byte 3 0 clear
 * (0x808080), 2 full (0x80808080); 1 fades in by 0x10 a call and 3 back out, waiting (2), Hewie's
 * +0xE4 set once there */
s32 func_0033A980(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    u32 c;

    switch (cmd[3]) {
    case 0:
        AT(gCharPartner, 0xE4, u8) = 0;
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, u32))(D_0044E4D0, 1, 0x808080);
        func_001267F0(gCharPartner, 0xF);
        return 1;
    case 1:
        ev = D_0044E4D0;
        c = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1);
        VCALL(D_0044E4F0, 0x70, void (*)(VObject *, u32))(D_0044E4F0, c);
        if (c != 0x80808080) {
            AT(gCharPartner, 0xE4, u8) = 0;
            VCALL(ev, 0x30, void (*)(VObject *, s32, u32))(ev, 1, c + 0x10000000);
            func_001267F0(gCharPartner, 0xF);
            return 2;
        }
        AT(gCharPartner, 0xE4, u8) = 1;
        return 1;
    case 2:
        AT(gCharPartner, 0xE4, u8) = 0;
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, u32))(D_0044E4D0, 1, 0x80808080);
        func_001267F0(gCharPartner, 0xF);
        return 1;
    case 3:
        ev = D_0044E4D0;
        c = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1);
        VCALL(D_0044E4F0, 0x70, void (*)(VObject *, u32))(D_0044E4F0, c);
        if (c != 0x808080) {
            AT(gCharPartner, 0xE4, u8) = 0;
            VCALL(ev, 0x30, void (*)(VObject *, s32, u32))(ev, 1, c - 0x10000000);
            func_001267F0(gCharPartner, 0xF);
            return 2;
        }
        AT(gCharPartner, 0xE4, u8) = 1;
        return 1;
    }
    return 1;
}

extern void *D_004726E0[];   /* the strand (creature.c) */

static void strand_init(void **obj) {
    obj[0] = D_004726E0;
    obj[0x40 / 4] = D_00469D00;
    ((s32 *)obj)[0x44 / 4] = -1;
    obj[0x40 / 4] = D_0046FC30;
}

/* D_00479FB0's update: while the pursuer is in state 4 / action 0x11 (and not stopped, +0x34,
 * nor its delay +0x30 run out) each of its ten timers (+0x4..) counts down; at most one at 0 a
 * frame restarts (10..41) and lets a strand (kind = the pursuer's, +0x2C) drip */
s32 func_0036A590(u8 *o) {
    VObject *rnd;
    u8 *mgr;
    u8 spawned = 0;
    s32 i;

    if (AT(gCharPursuer, 0xF8, s32) != 4 || AT(gCharPursuer, 0xFC, s32) != 0x11 || AT(o, 0x34, u8) == 1) {
        return 0;
    }
    if (AT(o, 0x30, s32) >= 0) {
        AT(o, 0x30, s32) = AT(o, 0x30, s32) - 1;
        if (AT(o, 0x30, s32) <= 0) {
            return 0;
        }
    }
    rnd = D_0044E550;
    mgr = D_0044E578;
    for (i = 0; i < 10; i++) {
        s32 *t = &AT(o, 0x4 + i * 4, s32);

        if (*t != 0) {
            (*t)--;
        } else if (!spawned) {
            s32 msg[2];
            s32 slot;

            spawned = 1;
            *t = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x1F) + 10;
            slot = Effect_New(mgr, 0x220, strand_init);
            msg[0] = AT(gCharPursuer, 0x153C, u8);
            msg[1] = AT(o, 0x2C, s32);
            func_002D6090(mgr, slot, msg);
        }
    }
    return 1;
}

/* ---- three 0x1C30-byte effects D_00479B00 (grey 0x303030) at once, each told its spot ---- */
extern void *D_00479B00[];

typedef struct GreyMsg {
    f32 a, b, c, d, e;
    u32 rgb;
    s16 spot;
} GreyMsg;

static void effect_79B00_init(void **obj) {
    obj[0] = D_00479B00;
}

static inline void grey_send(u8 *mgr, GreyMsg *m, s16 spot, f32 b, f32 c, f32 d) {
    m->b = b;
    m->spot = spot;
    m->c = c;
    m->d = d;
    m->e = d;
    func_002D6090(mgr, Effect_New(mgr, 0x1C30, effect_79B00_init), m);
}

static inline s32 grey_three(f32 a, const s16 *spot, const f32 *b, const f32 *c, const f32 *d) {
    u8 *mgr = D_0044E578;
    GreyMsg m;
    s32 i;

    m.a = a;
    m.rgb = 0x303030;
    for (i = 0; i < 3; i++) {
        grey_send(mgr, &m, spot[i], b[i], c[i], d[i]);
    }
    return 1;
}

s32 func_00340FE0(void) {
    static const s16 spot[3] = {0, 1, 6};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.0f, 0.0f, 0.0f}, d[3] = {1.0f, 1.0f, 1.0f};

    return grey_three(50.0f, spot, b, c, d);
}

s32 func_003406A0(void) {
    static const s16 spot[3] = {7, 4, 3};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.5f, 0.0f, 0.0f}, d[3] = {0.5f, 0.5f, 1.0f};

    return grey_three(60.0f, spot, b, c, d);
}

s32 func_00341720(void) {
    static const s16 spot[3] = {1, 3, 0xB};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.0f, 0.0f, 0.5f}, d[3] = {0.5f, 0.5f, 0.5f};

    return grey_three(70.0f, spot, b, c, d);
}

s32 func_00343180(void) {
    static const s16 spot[3] = {2, 7, 5};
    static const f32 b[3] = {0.0f, 0.0f, 0.5f}, c[3] = {0.0f, 0.0f, 0.0f}, d[3] = {0.5f, 0.5f, 0.5f};

    return grey_three(50.0f, spot, b, c, d);
}

/* the clock frozen: while event flag 1 (+0x58) is set, the time saved in script variables
 * (clock_save: 0 time up -> "59:59", 1 / 2 minutes / seconds) instead of the running one */
static inline s32 clock_draw_saved(char *buf, const char *full) {
    VObject *ev = D_0044E4D0;
    u32 t;

    if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 1) == 0) {
        return clock_draw(buf, full);
    }
    if (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0) != 0) {
        VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
            ev, 0x1AF, 0x18B, 0, full, 0x80, 0x30, 0x10, 0x15);
        return 1;
    }
    ev = D_0044E4D0;
    buf[0] = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1) / 10 + '0';
    t = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1);
    buf[2] = ':';
    buf[1] = t % 10 + '0';
    buf[3] = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 2) / 10 + '0';
    t = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 2);
    buf[5] = 0;
    buf[4] = t % 10 + '0';
    VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
        ev, 0x1AF, 0x18B, 0, buf, 0x80, 0x30, 0x10, 0x15);
    return 1;
}

extern char D_0047B290[], D_0047B340[];
extern const char D_004636D8[], D_00463A48[];   /* "59:59" */

s32 func_0036DB90(void) { return clock_draw_saved(D_0047B290, D_004636D8); }
s32 func_0037ACE0(void) { return clock_draw_saved(D_0047B340, D_00463A48); }

/* the EE's float to int: past the top it holds at 0x7FFFFFFF */
static inline s32 ee_ftoi(f32 v) {
    return v >= 2147483648.0f ? 0x7FFFFFFF : (s32)v;
}

/* the screen fade (renderer +0x70, alpha in the top byte) and the stalker's light by cutscene
 * frame: byte 3 0 up from frame 361 (2.04 a frame), 1 down to frame 605 (2.51), 2 up from 1140
 * and 3 down to 1203 (5.57); full (0x80) leaves it to light 0xA, else light 0x23 (its +0xE4
 * cleared); other bytes light 0x11 and the fade off (+0x64) */
s32 func_0032E010(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k0 = {0x40028F5C}, k1 = {0x4020A3D7}, k2 = {0x40B23D71};
    u32 t = VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10);
    s32 a;

    switch (cmd[3]) {
    case 0:
        a = ee_ftoi(k0.f * (f32)(t - 361));
        break;
    case 1:
        a = ee_ftoi(k1.f * (f32)(605 - t));
        break;
    case 2:
        a = ee_ftoi(k2.f * (f32)(t - 1140));
        break;
    case 3:
        a = ee_ftoi(k2.f * (f32)(1203 - t));
        break;
    default:
        func_001267F0(D_0044F808, 0x11);
        VCALL(D_0044E4F0, 0x64, void (*)(VObject *, u32, s32))(D_0044E4F0, 0x808080, 0);
        return 1;
    }
    if (a >= 0x80) {
        func_001267F0(D_0044F808, 0xA);
        return 1;
    }
    if (a < 0) {
        a = 0;
    }
    AT(D_0044F808, 0xE4, u8) = 0;
    func_001267F0(D_0044F808, 0x23);
    VCALL(D_0044E4F0, 0x70, void (*)(VObject *, u32))(D_0044E4F0, (u32)a << 24 | 0x808080);
    return 1;
}

/* (as func_0032E010)  byte 3 2 up from frame 1268 (2.79 a frame, light 0x23) and 3 from frame 25
 * (4.27, light 0xF), held at 0x80; 1 the fade fully on with light 0xA, else off with light 0x11
 * (+0x64) */
s32 func_0033A1C0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k2 = {0x40328F5C}, k3 = {0x4088A3D7};
    u32 t = VCALL(D_0044FE10, 0x34, s32 (*)(VObject *))(D_0044FE10);
    u32 a, s = 0;

    switch (cmd[3]) {
    case 2:
    case 3:
        a = cmd[3] == 2 ? (u32)(k2.f * (f32)(t - 1268)) : (u32)(k3.f * (f32)(t - 25));
        if (a > 0x80) {
            a = 0x80;
        }
        AT(D_0044F808, 0xE4, u8) = 0;
        VCALL(D_0044E4F0, 0x70, void (*)(VObject *, u32))(D_0044E4F0, a << 24 | 0x808080);
        func_001267F0(D_0044F808, cmd[3] == 2 ? 0x23 : 0xF);
        return 1;
    case 1:
        s = 0xFF;
        break;
    }
    if (s >= 0xFF) {
        func_001267F0(D_0044F808, 0xA);
        s = 0xFF;
    } else {
        func_001267F0(D_0044F808, 0x11);
    }
    VCALL(D_0044E4F0, 0x64, void (*)(VObject *, u32, s32))(D_0044E4F0, s << 24 | 0x808080, 0);
    return 1;
}

extern void *D_0047A310[];

static void effect_7A310_init(void **obj) {
    obj[0] = D_0047A310;
    obj[0xF10 / 4] = D_00469D00;
    ((s32 *)obj)[0xF14 / 4] = -1;
    obj[0xF10 / 4] = D_0046FC30;
    obj[0xF48 / 4] = D_00469D00;
    ((s32 *)obj)[0xF4C / 4] = -1;
    obj[0xF48 / 4] = D_0046FC30;
}

/* (as func_002A91F0)  the 0x10D0-byte effect D_0047A310: byte 3 0 / 1 one made and sent 1 / 0,
 * its slot kept in script variable 11; 2 that one sent nothing */
s32 func_00341C30(void *self, void *a1, u8 *cmd) {
    u8 *mgr;
    s32 slot, arg;

    switch (cmd[3]) {
    case 0:
    case 1:
        mgr = D_0044E578;
        slot = Effect_New(mgr, 0x10D0, effect_7A310_init);
        arg = cmd[3] == 0;
        func_002D6090(mgr, slot, &arg);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, 11, slot);
        break;
    case 2:
        slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, 11);
        func_002D6090(D_0044E578, slot, NULL);
        break;
    }
    return 1;
}

extern void *D_00479F70[];

static void effect_79F70_init(void **obj) {
    obj[0] = D_00479F70;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

/* the 0x20E0-byte effect D_00479F70 (sent byte 3): byte 3 0 / 1 one made, its slot in script
 * variable 2 / 3; 2 / 3 the one in variable 2 / 3 (if any) sent nothing */
s32 func_003425D0(void *self, void *a1, u8 *cmd) {
    u8 *mgr;
    u8 arg = cmd[3];
    s32 slot;

    switch (cmd[3]) {
    case 0:
    case 1:
        mgr = D_0044E578;
        slot = Effect_New(mgr, 0x20E0, effect_79F70_init);
        func_002D6090(mgr, slot, &arg);
        VCALL(D_0044E4D0, 0x30, void (*)(VObject *, s32, s32))(D_0044E4D0, cmd[3] + 2, slot);
        break;
    case 2:
    case 3:
        slot = VCALL(D_0044E4D0, 0x34, s32 (*)(VObject *, s32))(D_0044E4D0, cmd[3]);
        if (slot != -1) {
            func_002D6090(D_0044E578, slot, NULL);
        }
        break;
    }
    return 1;
}

extern void *D_00479870[];
extern const char *D_00441140[];   /* room objects 0..9 */

/* the room object by name, out of line (keeps a2 untouched for difftest) */
static __attribute__((noinline)) u8 *obj_named(const char *name) {
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
}

static void effect_79870_init(void **obj) {
    obj[0] = D_00479870;
}

/* the 0x10-byte effect D_00479870 on room object k + 1 (byte 4 = k, 1..8; script variable 11 - k
 * keeps its slot): made on first use when byte 3 is set, then sent (on byte 3, index 8 - k, the
 * variable, the object), with sound 1 at the object when on and the camera director's +0x38 is
 * clear */
s32 func_0034B210(void *self, void *a1, u8 *cmd) {
    u32 k = cmd[4];
    u8 var = 11 - k, idx = 8 - k;   /* (k 0 / past 8: unset on the PS2) */
    u32 name = k + 1;
    u8 on = cmd[3];
    VObject *ev = D_0044E4D0;
    s32 slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, var);
    u8 *o;
    struct {
        u8 on, idx, var, pad;
        u8 *obj;
    } msg;

    if (slot == -1) {
        if (on == 0) {
            return 1;
        }
        slot = Effect_New(D_0044E578, 0x10, effect_79870_init);
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, var, slot);
    }
    o = obj_named(D_00441140[name]);
    msg.idx = idx;
    msg.var = var;
    msg.on = on;
    msg.pad = 0;
    msg.obj = o;
    func_002D6090(D_0044E578, slot, &msg);
    if (VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *))(D_0044E4F8) == 0 && on != 0) {
        func_002FF650(D_0044E560, 1, 6, (f32 *)(o + 0x20), 0, 0);
    }
    return 1;
}

extern const char *D_00434890, *D_00434894;

/* a turning machine: the wheel D_00434890 (angle +0x18, height +0x24 5.1) driven by the belt
 * D_00434894 (offset +0x20 wrapping at 10, height +0x24 -3, speed +0x30). Byte 3 0 sets it up
 * (speed 0.4); 1 runs it a frame (both shaking by up to 0.05); 2 also slows it by 0.01, waiting
 * (2) until it stops. (The wheel's wrap steps +0x10, not the angle.) */
s32 func_0033FC10(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k51 = {0x40A33333}, k16Pi = {0x42490FDB}, k2Pi = {0x40C90FDB},
                                          kPi = {0x40490FDB}, k01 = {0x3DCCCCCD}, k001 = {0x3C23D70A};
    VObject *objs = D_00456DF8, *rnd;
    u8 *w = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00434890);
    u8 *b = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00434894);
    f32 v;

    switch (cmd[3]) {
    case 0:
        AT(w, 0x18, s32) = 0;
        AT(w, 0x24, f32) = k51.f;
        AT(b, 0x20, s32) = 0;
        AT(b, 0x24, u32) = 0xC0400000;   /* -3 */
        AT(b, 0x30, u32) = 0x3ECCCCCD;   /* 0.4 */
        return 1;
    case 1:
    case 2:
        break;
    default:
        return 1;
    }
    AT(w, 0x18, f32) = AT(w, 0x18, f32) + k2Pi.f * (AT(b, 0x30, f32) / k16Pi.f);
    if (!(AT(w, 0x10, f32) <= kPi.f)) {
        do {
            AT(w, 0x10, f32) = AT(w, 0x10, f32) - k2Pi.f;
        } while (!(AT(w, 0x10, f32) <= kPi.f));
    }
    rnd = D_0044E550;
    AT(w, 0x24, f32) = k51.f + k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(b, 0x20, f32) = AT(b, 0x20, f32) + AT(b, 0x30, f32);
    AT(b, 0x24, f32) = -3.0f + k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    if (!(AT(b, 0x20, f32) < 10.0f)) {
        do {
            AT(b, 0x20, f32) = AT(b, 0x20, f32) - 10.0f;
        } while (!(AT(b, 0x20, f32) < 10.0f));
    }
    if (cmd[3] != 2) {
        return 1;
    }
    v = AT(b, 0x30, f32) - k001.f;
    AT(b, 0x30, f32) = v;
    if (v <= 0.0f) {
        AT(b, 0x30, f32) = 0.0f;
        return 1;
    }
    return 2;
}

extern const char *D_00437D30[];   /* room objects 4, 5 */

/* room objects 4 / 5 spinning (+0x10) at a speed (+0x30) eased by script variable 4 / 5: byte 3 0
 * sets them up (speed and base 4 degrees, top 20); else each frame (unless the progress' +0x54
 * says no): state 0 slows by 5% of the base to 0, 1 speeds by 2% up to the base, 2 by 10% up to
 * the top, 3 jumps to the base */
s32 func_003434A0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k005 = {0x3D4CCCCD}, k002 = {0x3CA3D70A}, k01 = {0x3DCCCCCD},
                                          kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *objs = D_00456DF8, *ev;
    s32 k;

    if (cmd[3] == 0) {
        for (k = 4; k < 6; k++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D30[k - 4]);

            if (o != NULL) {
                AT(o, 0x30, u32) = 0x3D8EFA35;   /* 4 degrees */
                AT(o, 0x34, u32) = 0x3D8EFA35;
                AT(o, 0x38, u32) = 0x3EB2B8C2;   /* 20 degrees */
            }
        }
        return 1;
    }
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    ev = D_0044E4D0;
    for (k = 4; k < 6; k++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D30[k - 4]);
        s32 state = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, k == 4 ? 4 : 5);
        f32 v;

        if (o == NULL) {
            continue;
        }
        switch (state) {
        case 0:
            v = AT(o, 0x30, f32) - k005.f * AT(o, 0x34, f32);
            AT(o, 0x30, f32) = v;
            if (v < 0.0f) {
                AT(o, 0x30, f32) = 0.0f;
            }
            break;
        case 1:
            AT(o, 0x30, f32) = AT(o, 0x30, f32) + k002.f * AT(o, 0x34, f32);
            if (!(AT(o, 0x30, f32) <= AT(o, 0x34, f32))) {
                AT(o, 0x30, f32) = AT(o, 0x34, f32);
            }
            break;
        case 2:
            AT(o, 0x30, f32) = AT(o, 0x30, f32) + k01.f * AT(o, 0x34, f32);
            if (!(AT(o, 0x30, f32) <= AT(o, 0x38, f32))) {
                AT(o, 0x30, f32) = AT(o, 0x38, f32);
            }
            break;
        case 3:
            AT(o, 0x30, f32) = AT(o, 0x34, f32);
            break;
        }
        v = AT(o, 0x10, f32) + AT(o, 0x30, f32);
        AT(o, 0x10, f32) = v;
        if (!(v <= kPi.f)) {
            AT(o, 0x10, f32) = v - k2Pi.f;
        }
    }
    return 1;
}

extern void *D_00479F30[];
extern void func_002E3190(f32 (*m)[4], f32 angle);   /* turn about y */
extern void func_00120F90(void *o, u32 tri, f32 *pos, f32 *rot, f32 *front);

static void effect_79F30_init(void **obj) {
    obj[0] = D_00479F30;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

/* the placed thing 10 brought back (list +0x14 / +0x8, its +0xC, +0x28 on) and put on one of five
 * spots round a circle (script variable 1, then on by 2 of 5): turned to the spot's angle (with a
 * little random), 2.1 up and 1.5..2 out on triangle 0x3B; then effect D_00479F30 on it */
s32 func_00342BC0(void) {
    static const union { u32 u; f32 f; } kStep = {0x3FA0D97C}, kJit = {0x3F80ADFD}, kHalf = {0x3F00ADFD};
    VObject *list = D_0044F260, *ev, *rnd;
    u8 *t;
    s32 n, k;
    f32 a;
    f32 one[4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    f32 front[4] __attribute__((aligned(16)));
    f32 off[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));

    VCALL(list, 0x14, void (*)(VObject *, s32))(list, 10);
    t = VCALL(list, 0x8, u8 *(*)(VObject *, s32))(list, 10);
    if (t == NULL) {
        return 1;
    }
    VCALL((VObject *)t, 0xC, void (*)(VObject *))((VObject *)t);
    AT(t, 0x28, u8) = 1;
    ev = D_0044E4D0;
    n = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);
    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    rnd = D_0044E550;
    a = kJit.f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd) + kStep.f * (f32)n;
    a = func_002E2D00(a - kHalf.f);
    rot[0] = 0.0f;
    rot[1] = 0.0f;
    rot[2] = a;
    func_002E3190(m, a);
    off[0] = 0.0f;
    off[1] = 0x1.0ccccc0000000p+1f /* 2.1 */;
    off[2] = 1.5f + 0.5f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
    off[3] = 1.0f;
    sceVu0ApplyMatrix(front, m, off);
    func_00120F90(t, 0x3B, one, rot, front);
    k = n + 2;
    if (k >= 5) {
        k -= 5;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, k);
    func_002D6090(D_0044E578, Effect_New(D_0044E578, 0xE60, effect_79F30_init), t);
    return 1;
}

/* up to 6 things out (script variable 6 counts them): 1..3 more placed things of kind 9 (+0x8),
 * each tied to the first room object 10..15 flagged (+0 = 1) (+0x122 its index), set up (+0xC,
 * +0x28 on) and dropped at a random spot 40..50 out and 25..40 up in any direction that lands on
 * the nav mesh (+0x3C) */
s32 func_00342E80(void) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *ev = D_0044E4D0, *rnd, *list, *objs, *nav;
    s32 n = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    s32 cnt, i;

    if (n >= 6) {
        return 1;
    }
    rnd = D_0044E550;
    cnt = (s32)(3.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd)) + 1;
    if (6 - n < cnt) {
        cnt = 6 - n;
    }
    list = D_0044F260;
    objs = D_00456DF8;
    nav = (VObject *)D_0044E570;
    for (i = 0; i < cnt; i++) {
        u8 *t = VCALL(list, 0x8, u8 *(*)(VObject *, s32))(list, 9);
        s32 k, tri;
        f32 off[4] __attribute__((aligned(16)));
        f32 p[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));

        if (t == NULL) {
            continue;
        }
        n++;
        for (k = 10; k < 16; k++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D48[k - 10]);

            if (o[0] == 1) {
                break;
            }
        }
        AT(t, 0x122, s16) = k - 10;
        VCALL((VObject *)t, 0xC, void (*)(VObject *))((VObject *)t);
        AT(t, 0x28, u8) = 1;
        do {
            f32 r, ang;

            r = VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
            off[0] = 0.0f;
            ang = kPi.f - 2.0f * (kPi.f * r);
            off[1] = 40.0f + 10.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
            r = VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
            off[3] = 0.0f;
            off[2] = 25.0f + 15.0f * r;
            sceVu0UnitMatrix(m);
            sceVu0RotMatrixY(m, m, ang);
            sceVu0ApplyMatrix(p, m, off);
            tri = VCALL(nav, 0x3C, s32 (*)(VObject *, f32 *, s32))(nav, p, 0);
        } while (tri == -1);
        AT(t, 0x34, s32) = tri;
        sceVu0CopyVector((f32 *)(t + 0x10), p);
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, n);
    return 1;
}
