/* The item classes (pool entries, vtable D_0046C790): their destructors. Each item has its own
 * class (+0x28 loads its picture, +0x3C uses it) on one of a few middle classes (D_0046C8D0
 * the plain item, D_0046CB50 ..., see the vtables); the destructors only step the vtable back
 * down the chain and free the entry. */
#include "common.h"
#include "game.h"

extern void func_0025FEF0(void *p);   /* operator delete (pool entries) */
extern void *D_0046C790[];            /* a pool entry */
extern void *D_0046C7E0[], *D_0046C830[], *D_0046C880[], *D_0046C8D0[], *D_0046C920[], *D_0046C970[], *D_0046C9C0[], *D_0046CA10[], *D_0046CA60[], *D_0046CAB0[], *D_0046CB00[], *D_0046CB50[], *D_0046CBA0[], *D_0046CBF0[], *D_0046CC40[], *D_0046CC90[], *D_0046CCE0[], *D_0046CD30[], *D_0046CD80[], *D_0046CDD0[], *D_0046CE20[], *D_0046CE70[], *D_0046CEC0[], *D_0046CF10[], *D_0046CF60[], *D_0046CFB0[], *D_0046D000[], *D_0046D050[], *D_0046D0A0[], *D_0046D0F0[], *D_0046D140[], *D_0046D190[], *D_0046D1E0[], *D_0046D230[], *D_0046D280[], *D_0046D2D0[], *D_0046D320[], *D_0046D370[], *D_0046D3C0[], *D_0046D410[], *D_0046D460[], *D_0046D4B0[], *D_0046D500[], *D_0046D550[], *D_0046D5A0[], *D_0046D5F0[], *D_0046D640[], *D_0046D690[], *D_0046D6E0[], *D_0046EE40[], *D_0046EE90[], *D_0046EEE0[], *D_0046EF30[], *D_0046EF80[], *D_0046EFD0[], *D_0046F430[], *D_0046F610[], *D_0046F660[], *D_004703F0[], *D_00470FB0[], *D_00471080[], *D_004710D0[], *D_00471120[], *D_00471170[], *D_004711C0[], *D_00472F80[], *D_00472FD0[], *D_00473020[], *D_00473070[], *D_004730C0[], *D_00474BC0[], *D_00475300[], *D_00475350[], *D_004753A0[], *D_004753F0[], *D_00475440[], *D_00475490[], *D_004754E0[], *D_00475530[], *D_00475580[], *D_004755D0[], *D_00475620[], *D_00475670[], *D_004756C0[], *D_00475710[], *D_00475760[], *D_004757B0[], *D_00475800[], *D_00475850[], *D_00475CC0[], *D_00475D10[], *D_00475D60[], *D_00476450[], *D_00476B00[], *D_00476B60[], *D_00477030[], *D_00477080[], *D_004770D0[], *D_00477120[], *D_00477170[], *D_004775C0[], *D_00478C80[], *D_00478CD0[], *D_00478D20[], *D_00478D70[], *D_00478DC0[], *D_00478E10[], *D_00478E60[], *D_00478EB0[], *D_00478F00[], *D_00478F50[], *D_00478FA0[], *D_00479820[];

/* a middle class: its vtable, then the pool entry's */
static inline void *item_dtor2(void *o, s32 flags, void **own) {
    if (o != NULL) {
        AT(o, 0x0, void **) = own;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046C790;
        }
        if ((s16)flags > 0) {
            func_0025FEF0(o);
        }
    }
    return o;
}

/* an item class: its vtable, its middle class's, the pool entry's */
static inline void *item_dtor3(void *o, s32 flags, void **own, void **mid) {
    if (o != NULL) {
        AT(o, 0x0, void **) = own;
        if (o != NULL) {
            AT(o, 0x0, void **) = mid;
            if (o != NULL) {
                AT(o, 0x0, void **) = D_0046C790;
            }
        }
        if ((s16)flags > 0) {
            func_0025FEF0(o);
        }
    }
    return o;
}

/* the middle classes */

void *func_002640C0(void *o, s32 flags) { return item_dtor2(o, flags, D_0046C8D0); }
void *func_00263DC0(void *o, s32 flags) { return item_dtor2(o, flags, D_0046C920); }
void *func_00263A90(void *o, s32 flags) { return item_dtor2(o, flags, D_0046CB50); }
void *func_00263970(void *o, s32 flags) { return item_dtor2(o, flags, D_0046CCE0); }
void *func_00263850(void *o, s32 flags) { return item_dtor2(o, flags, D_0046CE70); }
void *func_00263730(void *o, s32 flags) { return item_dtor2(o, flags, D_0046D000); }
void *func_00263520(void *o, s32 flags) { return item_dtor2(o, flags, D_0046D320); }
void *func_00263250(void *o, s32 flags) { return item_dtor2(o, flags, D_0046D640); }
void *func_002D2750(void *o, s32 flags) { return item_dtor2(o, flags, D_0046F430); }
void *func_00263C10(void *o, s32 flags) { return item_dtor2(o, flags, D_00476B00); }

/* the items */
void *func_002649F0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046C7E0, D_0046C8D0); }
void *func_00264C70(void *o, s32 flags) { return item_dtor3(o, flags, D_0046C830, D_0046C8D0); }
void *func_00264EC0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046C880, D_0046C8D0); }
void *func_00264F70(void *o, s32 flags) { return item_dtor3(o, flags, D_0046C970, D_0046CB50); }
void *func_00265060(void *o, s32 flags) { return item_dtor3(o, flags, D_0046C9C0, D_0046CB50); }
void *func_00265110(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CA10, D_0046CB50); }
void *func_002651C0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CA60, D_0046CB50); }
void *func_00265270(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CAB0, D_0046CB50); }
void *func_00265320(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CB00, D_0046CB50); }
void *func_002653D0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CBA0, D_0046CCE0); }
void *func_002654B0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CBF0, D_0046CCE0); }
void *func_00265560(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CC40, D_0046CCE0); }
void *func_00265610(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CC90, D_0046CCE0); }
void *func_002656C0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CD30, D_0046CE70); }
void *func_002657A0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CD80, D_0046CE70); }
void *func_00265850(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CDD0, D_0046CE70); }
void *func_00265900(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CE20, D_0046CE70); }
void *func_002659B0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CEC0, D_0046D000); }
void *func_00265A90(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CF10, D_0046D000); }
void *func_00265B40(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CF60, D_0046D000); }
void *func_00265BF0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046CFB0, D_0046D000); }
void *func_00265CA0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D050, D_0046D320); }
void *func_00265D80(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D0A0, D_0046D320); }
void *func_00265E30(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D0F0, D_0046D320); }
void *func_00265EE0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D140, D_0046D320); }
void *func_00265F90(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D190, D_0046D320); }
void *func_00266040(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D1E0, D_0046D320); }
void *func_002660F0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D230, D_0046D320); }
void *func_002661A0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D280, D_0046D320); }
void *func_00266250(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D2D0, D_0046D320); }
void *func_00266300(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D370, D_0046D640); }
void *func_00266450(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D3C0, D_0046D640); }
void *func_00266500(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D410, D_0046D640); }
void *func_002665B0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D460, D_0046D640); }
void *func_00266660(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D4B0, D_0046D640); }
void *func_00266710(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D500, D_0046D640); }
void *func_002667C0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D550, D_0046D640); }
void *func_00266870(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D5A0, D_0046D640); }
void *func_00266920(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D5F0, D_0046D640); }
void *func_002669D0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D690, D_0046C920); }
void *func_00266B60(void *o, s32 flags) { return item_dtor3(o, flags, D_0046D6E0, D_0046C920); }
void *func_002CCBD0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046EE40, D_0046C8D0); }
void *func_002CCD30(void *o, s32 flags) { return item_dtor3(o, flags, D_0046EE90, D_0046C8D0); }
void *func_002CCF90(void *o, s32 flags) { return item_dtor3(o, flags, D_0046EEE0, D_0046C8D0); }
void *func_002CD130(void *o, s32 flags) { return item_dtor3(o, flags, D_0046EF30, D_0046C8D0); }
void *func_002CD2A0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046EF80, D_0046C8D0); }
void *func_002CD420(void *o, s32 flags) { return item_dtor3(o, flags, D_0046EFD0, D_0046C8D0); }
void *func_002D7910(void *o, s32 flags) { return item_dtor3(o, flags, D_0046F610, D_0046D640); }
void *func_002D79C0(void *o, s32 flags) { return item_dtor3(o, flags, D_0046F660, D_0046D640); }
void *func_002EEB60(void *o, s32 flags) { return item_dtor3(o, flags, D_004703F0, D_0046C8D0); }
void *func_00303C30(void *o, s32 flags) { return item_dtor3(o, flags, D_00470FB0, D_0046C8D0); }
void *func_00306B20(void *o, s32 flags) { return item_dtor3(o, flags, D_00471080, D_0046C8D0); }
void *func_00307130(void *o, s32 flags) { return item_dtor3(o, flags, D_004710D0, D_0046C8D0); }
void *func_00307740(void *o, s32 flags) { return item_dtor3(o, flags, D_00471120, D_0046C8D0); }
void *func_00307D50(void *o, s32 flags) { return item_dtor3(o, flags, D_00471170, D_0046C8D0); }
void *func_00308360(void *o, s32 flags) { return item_dtor3(o, flags, D_004711C0, D_0046C8D0); }
void *func_0031CAA0(void *o, s32 flags) { return item_dtor3(o, flags, D_00472F80, D_0046C8D0); }
void *func_0031CD70(void *o, s32 flags) { return item_dtor3(o, flags, D_00472FD0, D_0046C8D0); }
void *func_0031CFB0(void *o, s32 flags) { return item_dtor3(o, flags, D_00473020, D_0046C8D0); }
void *func_0031D110(void *o, s32 flags) { return item_dtor3(o, flags, D_00473070, D_0046C8D0); }
void *func_0031D590(void *o, s32 flags) { return item_dtor3(o, flags, D_004730C0, D_0046C8D0); }
void *func_0032CC90(void *o, s32 flags) { return item_dtor3(o, flags, D_00474BC0, D_0046C8D0); }
void *func_00331450(void *o, s32 flags) { return item_dtor3(o, flags, D_00475300, D_0046C920); }
void *func_003315F0(void *o, s32 flags) { return item_dtor3(o, flags, D_00475350, D_0046C920); }
void *func_00331790(void *o, s32 flags) { return item_dtor3(o, flags, D_004753A0, D_0046C920); }
void *func_003318B0(void *o, s32 flags) { return item_dtor3(o, flags, D_004753F0, D_0046C920); }
void *func_003319D0(void *o, s32 flags) { return item_dtor3(o, flags, D_00475440, D_0046C920); }
void *func_00331B60(void *o, s32 flags) { return item_dtor3(o, flags, D_00475490, D_0046C920); }
void *func_00331CE0(void *o, s32 flags) { return item_dtor3(o, flags, D_004754E0, D_0046C920); }
void *func_00331E20(void *o, s32 flags) { return item_dtor3(o, flags, D_00475530, D_0046C920); }
void *func_00331F70(void *o, s32 flags) { return item_dtor3(o, flags, D_00475580, D_0046C920); }
void *func_00332120(void *o, s32 flags) { return item_dtor3(o, flags, D_004755D0, D_0046C920); }
void *func_003322E0(void *o, s32 flags) { return item_dtor3(o, flags, D_00475620, D_0046C920); }
void *func_003323C0(void *o, s32 flags) { return item_dtor3(o, flags, D_00475670, D_00476B00); }
void *func_00332620(void *o, s32 flags) { return item_dtor3(o, flags, D_004756C0, D_00476B00); }
void *func_00332830(void *o, s32 flags) { return item_dtor3(o, flags, D_00475710, D_00476B00); }
void *func_00332A10(void *o, s32 flags) { return item_dtor3(o, flags, D_00475760, D_00476B00); }
void *func_00332C50(void *o, s32 flags) { return item_dtor3(o, flags, D_004757B0, D_00476B00); }
void *func_00332E70(void *o, s32 flags) { return item_dtor3(o, flags, D_00475800, D_00476B00); }
void *func_00333060(void *o, s32 flags) { return item_dtor3(o, flags, D_00475850, D_00476B00); }
void *func_00338F00(void *o, s32 flags) { return item_dtor3(o, flags, D_00475CC0, D_0046C8D0); }
void *func_003392B0(void *o, s32 flags) { return item_dtor3(o, flags, D_00475D10, D_0046C8D0); }
void *func_00339660(void *o, s32 flags) { return item_dtor3(o, flags, D_00475D60, D_0046C8D0); }
void *func_0033AE70(void *o, s32 flags) { return item_dtor3(o, flags, D_00476450, D_0046D640); }
void *func_0033BD50(void *o, s32 flags) { return item_dtor3(o, flags, D_00476B60, D_0046D640); }
void *func_0033E850(void *o, s32 flags) { return item_dtor3(o, flags, D_00477030, D_0046C8D0); }
void *func_0033E9C0(void *o, s32 flags) { return item_dtor3(o, flags, D_00477080, D_0046C8D0); }
void *func_0033EB30(void *o, s32 flags) { return item_dtor3(o, flags, D_004770D0, D_0046C8D0); }
void *func_0033ECA0(void *o, s32 flags) { return item_dtor3(o, flags, D_00477120, D_0046C8D0); }
void *func_0033EE10(void *o, s32 flags) { return item_dtor3(o, flags, D_00477170, D_0046C8D0); }
void *func_00344520(void *o, s32 flags) { return item_dtor3(o, flags, D_004775C0, D_0046C8D0); }
void *func_003510B0(void *o, s32 flags) { return item_dtor3(o, flags, D_00478C80, D_0046C8D0); }
void *func_00351170(void *o, s32 flags) { return item_dtor3(o, flags, D_00478CD0, D_0046C8D0); }
void *func_00351230(void *o, s32 flags) { return item_dtor3(o, flags, D_00478D20, D_0046C8D0); }
void *func_003512F0(void *o, s32 flags) { return item_dtor3(o, flags, D_00478D70, D_0046C8D0); }
void *func_003514B0(void *o, s32 flags) { return item_dtor3(o, flags, D_00478DC0, D_0046C8D0); }
void *func_00351670(void *o, s32 flags) { return item_dtor3(o, flags, D_00478E10, D_0046C8D0); }
void *func_003517D0(void *o, s32 flags) { return item_dtor3(o, flags, D_00478E60, D_0046C8D0); }
void *func_00351880(void *o, s32 flags) { return item_dtor3(o, flags, D_00478EB0, D_0046C8D0); }
void *func_00351AA0(void *o, s32 flags) { return item_dtor3(o, flags, D_00478F00, D_0046C8D0); }
void *func_00351C20(void *o, s32 flags) { return item_dtor3(o, flags, D_00478F50, D_0046C8D0); }
void *func_00351CE0(void *o, s32 flags) { return item_dtor3(o, flags, D_00478FA0, D_0046C8D0); }
void *func_0035BB20(void *o, s32 flags) { return item_dtor3(o, flags, D_00479820, D_0046C8D0); }

/* ---- the small methods (the base pool entry's and the items' own) ----
 * +0xC its id (+0x4), +0x10 / +0x14 / +0x20 class constants, +0x18 how it is held (1 a stack
 * counted at +0x10, 2 a counter), +0x1C its name / picture cell, +0x24 its data (+0x10) */

/* +0xC (D_0046C790, D_0046C7E0, D_0046C830, ...) */
s32 func_0025FC90(void *o) {
    return AT(o, 0x4, s32);
}

/* +0x10 (D_0046C790) */
s32 func_0025FCA0(void *o) {
    return -1;
}

/* +0x14 (D_0046C790) */
s32 func_0025FCB0(void *o) {
    return 0;
}

/* +0x18 (D_0046C790) */
s32 func_0025FCC0(void *o) {
    return 0;
}

/* +0x1C (D_0046C790, D_0046C7E0, D_0046C830, ...) */
s32 func_0025FCD0(void *o) {
    return 0;
}

/* +0x20 (D_0046C790, D_0046C7E0, D_0046C830, ...) */
s32 func_0025FCE0(void *o) {
    return 0;
}

/* +0x24 (D_0046C790, D_0046C7E0, D_0046C830, ...) */
void *func_0025FCF0(void *o) {
    return (u8 *)o + 0x10;
}

/* +0x3C (D_0046C790, D_0046C8D0, D_0046C920, ...) */
s32 func_0025FEE0(void *o) {
    return 0;
}

/* +0x10 (D_0046C7E0, D_0046C830, D_0046C880, ...) */
s32 func_00264C40(void *o) {
    return 0;
}

/* +0x14 (D_0046C7E0, D_0046C830, D_0046C880, ...) */
s32 func_00264C50(void *o) {
    return 0x1;
}

/* +0x18 (D_0046C7E0, D_0046C830, D_0046C880, ...) */
s32 func_00264C60(void *o) {
    return 0;
}

/* +0x3C (D_0046C880) */
s32 func_00264F30(void *o) {
    return 0;
}

/* +0x10 (D_0046C920, D_0046D690, D_0046D6E0, ...) */
s32 func_00264F40(void *o) {
    return 0x2;
}

/* +0x14 (D_0046C920, D_0046D690, D_0046D6E0) */
s32 func_00264F50(void *o) {
    return 0x5;
}

/* +0x18 (D_0046C920, D_0046D690, D_0046D6E0, ...) */
s32 func_00264F60(void *o) {
    return 0x1;
}

/* +0x1C (D_0046C970) */
s32 func_00265010(void *o) {
    return 0xA10;
}

/* +0x10 (D_0046C970, D_0046C9C0, D_0046CA10, ...) */
s32 func_00265020(void *o) {
    return 0x8;
}

/* +0x18 (D_0046C970, D_0046C9C0, D_0046CA10, ...) */
s32 func_00265030(void *o) {
    return 0x1;
}

/* +0x14 (D_0046C970, D_0046C9C0, D_0046CA10, ...) */
s32 func_00265040(void *o) {
    return 0x5;
}

/* +0x3C (D_0046C970, D_0046C9C0, D_0046CA10, ...) */
s32 func_00265050(void *o) {
    return 0;
}

/* +0x1C (D_0046C9C0) */
s32 func_00265100(void *o) {
    return 0xA20;
}

/* +0x1C (D_0046CA10) */
s32 func_002651B0(void *o) {
    return 0xA30;
}

/* +0x1C (D_0046CA60) */
s32 func_00265260(void *o) {
    return 0xA40;
}

/* +0x1C (D_0046CAB0) */
s32 func_00265310(void *o) {
    return 0xA50;
}

/* +0x1C (D_0046CB00) */
s32 func_002653C0(void *o) {
    return 0xA60;
}

/* +0x1C (D_0046CBA0) */
s32 func_00265470(void *o) {
    return 0x310;
}

/* +0x10 (D_0046CBA0, D_0046CBF0, D_0046CC40, ...) */
s32 func_00265480(void *o) {
    return 0x4;
}

/* +0x14 (D_0046CBA0, D_0046CBF0, D_0046CC40, ...) */
s32 func_00265490(void *o) {
    return 0x6;
}

/* +0x18 (D_0046CBA0, D_0046CBF0, D_0046CC40, ...) */
s32 func_002654A0(void *o) {
    return 0;
}

/* +0x1C (D_0046CBF0) */
s32 func_00265550(void *o) {
    return 0x320;
}

/* +0x1C (D_0046CC40) */
s32 func_00265600(void *o) {
    return 0x330;
}

/* +0x1C (D_0046CC90) */
s32 func_002656B0(void *o) {
    return 0x340;
}

/* +0x1C (D_0046CD30) */
s32 func_00265760(void *o) {
    return 0x410;
}

/* +0x10 (D_0046CD30, D_0046CD80, D_0046CDD0, ...) */
s32 func_00265770(void *o) {
    return 0x5;
}

/* +0x14 (D_0046CD30, D_0046CD80, D_0046CDD0, ...) */
s32 func_00265780(void *o) {
    return 0x6;
}

/* +0x18 (D_0046CD30, D_0046CD80, D_0046CDD0, ...) */
s32 func_00265790(void *o) {
    return 0;
}

/* +0x1C (D_0046CD80) */
s32 func_00265840(void *o) {
    return 0x420;
}

/* +0x1C (D_0046CDD0) */
s32 func_002658F0(void *o) {
    return 0x430;
}

/* +0x1C (D_0046CE20) */
s32 func_002659A0(void *o) {
    return 0x440;
}

/* +0x1C (D_0046CEC0) */
s32 func_00265A50(void *o) {
    return 0x510;
}

/* +0x10 (D_0046CEC0, D_0046CF10, D_0046CF60, ...) */
s32 func_00265A60(void *o) {
    return 0x9;
}

/* +0x14 (D_0046CEC0, D_0046CF10, D_0046CF60, ...) */
s32 func_00265A70(void *o) {
    return 0x6;
}

/* +0x18 (D_0046CEC0, D_0046CF10, D_0046CF60, ...) */
s32 func_00265A80(void *o) {
    return 0;
}

/* +0x1C (D_0046CF10) */
s32 func_00265B30(void *o) {
    return 0x520;
}

/* +0x1C (D_0046CF60) */
s32 func_00265BE0(void *o) {
    return 0x530;
}

/* +0x1C (D_0046CFB0) */
s32 func_00265C90(void *o) {
    return 0x540;
}

/* +0x1C (D_0046D050) */
s32 func_00265D40(void *o) {
    return 0x820;
}

/* +0x10 (D_0046D050, D_0046D0A0, D_0046D0F0, ...) */
s32 func_00265D50(void *o) {
    return 0x6;
}

/* +0x14 (D_0046D050, D_0046D0A0, D_0046D0F0, ...) */
s32 func_00265D60(void *o) {
    return 0x6;
}

/* +0x18 (D_0046D050, D_0046D0A0, D_0046D0F0, ...) */
s32 func_00265D70(void *o) {
    return 0x1;
}

/* +0x1C (D_0046D0A0) */
s32 func_00265E20(void *o) {
    return 0x610;
}

/* +0x1C (D_0046D0F0) */
s32 func_00265ED0(void *o) {
    return 0x710;
}

/* +0x1C (D_0046D140) */
s32 func_00265F80(void *o) {
    return 0x830;
}

/* +0x1C (D_0046D190) */
s32 func_00266030(void *o) {
    return 0x620;
}

/* +0x1C (D_0046D1E0) */
s32 func_002660E0(void *o) {
    return 0x720;
}

/* +0x1C (D_0046D230) */
s32 func_00266190(void *o) {
    return 0x630;
}

/* +0x1C (D_0046D280) */
s32 func_00266240(void *o) {
    return 0x730;
}

/* +0x1C (D_0046D2D0) */
s32 func_002662F0(void *o) {
    return 0x810;
}

/* +0x10 (D_0046D370, D_0046D3C0, D_0046D410, ...) */
s32 func_00266420(void *o) {
    return 0x7;
}

/* +0x14 (D_0046D370, D_0046D3C0, D_0046D410, ...) */
s32 func_00266430(void *o) {
    return 0x1;
}

/* +0x18 (D_0046D370, D_0046D3C0, D_0046D410, ...) */
s32 func_00266440(void *o) {
    return 0;
}

/* +0x3C (D_0046D3C0) */
s32 func_002664C0(void *o) {
    return 0;
}

/* +0x3C (D_0046D410) */
s32 func_00266570(void *o) {
    return 0;
}

/* +0x3C (D_0046D460) */
s32 func_00266620(void *o) {
    return 0;
}

/* +0x3C (D_0046D4B0) */
s32 func_002666D0(void *o) {
    return 0;
}

/* +0x3C (D_0046D500) */
s32 func_00266780(void *o) {
    return 0;
}

/* +0x3C (D_0046D550) */
s32 func_00266830(void *o) {
    return 0;
}

/* +0x3C (D_0046D5A0) */
s32 func_002668E0(void *o) {
    return 0;
}

/* +0x3C (D_0046D5F0) */
s32 func_00266990(void *o) {
    return 0;
}

/* +0x1C (D_0046D690) */
s32 func_00266A40(void *o) {
    return 0x110;
}

/* +0x1C (D_0046D6E0) */
s32 func_00266BD0(void *o) {
    return 0x120;
}

/* +0x18 (D_0046EE90) */
s32 func_002CCDA0(void *o) {
    return 0x2;
}

/* +0x10 (D_0046F430) */
s32 func_002D27B0(void *o) {
    return 0x1;
}

/* +0x14 (D_0046F430) */
s32 func_002D27C0(void *o) {
    return 0x5;
}

/* +0x18 (D_0046F430) */
s32 func_002D27D0(void *o) {
    return 0;
}

/* +0x20 (D_0046F430) */
void *func_002D3A50(void *o) {
    return (u8 *)o + 0x10;
}

/* +0x3C (D_0046F610) */
s32 func_002D79B0(void *o) {
    return 0;
}

/* +0x1C (D_00475300) */
s32 func_003314D0(void *o) {
    return 0x130;
}

/* +0x1C (D_00475350) */
s32 func_00331670(void *o) {
    return 0x140;
}

/* +0x1C (D_004753A0) */
s32 func_00331810(void *o) {
    return 0x150;
}

/* +0x1C (D_004753F0) */
s32 func_00331930(void *o) {
    return 0x160;
}

/* +0x1C (D_00475440) */
s32 func_00331A50(void *o) {
    return 0x170;
}

/* +0x1C (D_00475490) */
s32 func_00331BE0(void *o) {
    return 0x180;
}

/* +0x1C (D_004754E0) */
s32 func_00331D60(void *o) {
    return 0x190;
}

/* +0x1C (D_00475530) */
s32 func_00331EA0(void *o) {
    return 0x1A0;
}

/* +0x1C (D_00475580) */
s32 func_00331FF0(void *o) {
    return 0x1B0;
}

/* +0x1C (D_004755D0) */
s32 func_003321A0(void *o) {
    return 0x1C0;
}

/* +0x1C (D_00475620) */
s32 func_00332360(void *o) {
    return 0x1E0;
}

/* +0x1C (D_00475670) */
s32 func_00332430(void *o) {
    return 0x210;
}

/* +0x10 (D_00475670, D_004756C0, D_00475710, ...) */
s32 func_00332440(void *o) {
    return 0x3;
}

/* +0x14 (D_00475670, D_004756C0, D_00475710, ...) */
s32 func_00332450(void *o) {
    return 0x5;
}

/* +0x18 (D_00475670, D_004756C0, D_00475710, ...) */
s32 func_00332460(void *o) {
    return 0x1;
}

/* +0x1C (D_004756C0) */
s32 func_00332690(void *o) {
    return 0x220;
}

/* +0x1C (D_00475710) */
s32 func_003328A0(void *o) {
    return 0x230;
}

/* +0x1C (D_00475760) */
s32 func_00332A90(void *o) {
    return 0x240;
}

/* +0x1C (D_004757B0) */
s32 func_00332CD0(void *o) {
    return 0x250;
}

/* +0x1C (D_00475800) */
s32 func_00332EF0(void *o) {
    return 0x260;
}

/* +0x1C (D_00475850) */
s32 func_003330E0(void *o) {
    return 0x270;
}

/* +0x3C (D_00476450) */
s32 func_0033AF10(void *o) {
    return 0;
}

/* +0x3C (D_00476B60) */
s32 func_0033BDF0(void *o) {
    return 0;
}

/* +0x14 (D_00477030) */
s32 func_0033E8C0(void *o) {
    return 0x5;
}

/* +0x14 (D_00477080) */
s32 func_0033EA30(void *o) {
    return 0x5;
}

/* +0x14 (D_004770D0) */
s32 func_0033EBA0(void *o) {
    return 0x5;
}

/* +0x14 (D_00477120) */
s32 func_0033ED10(void *o) {
    return 0x5;
}

/* +0x14 (D_00477170) */
s32 func_0033EE80(void *o) {
    return 0x5;
}

/* +0x14 (D_004775C0) */
s32 func_00344590(void *o) {
    return 0x5;
}

/* +0x3C (D_00478E60) */
s32 func_00351870(void *o) {
    return 0;
}

/* +0x3C (D_00479820) */
s32 func_0035BBC0(void *o) {
    return 0;
}


/* ---- +0x28: start loading the item's picture into `dst` ---- */

extern VObject *gFileLoader;

#define ITEM_LOAD(name, dst) \
    VCALL(gFileLoader, 0xC, s32 (*)(VObject *, const char *, s32, s32, s32))(gFileLoader, name, dst, 0x4000000, 0)

extern const char D_0045A7C0[], D_0045A8A0[], D_0045A8C0[], D_0045A8E0[], D_0045A900[], D_0045A920[], D_0045A940[], D_0045A960[], D_0045A980[], D_0045A9A0[], D_0045AA40[], D_0045AA60[], D_0045AA80[], D_0045AAA0[], D_0045AAC0[], D_0045AAE0[], D_0045AB00[], D_0045AB20[], D_0045AB40[], D_0045ACA0[], D_0045ACC0[], D_0045E280[], D_0045EA80[], D_00463360[];

/* "ITEM00\ITEM_FFF.TEX" */
s32 func_0025FF50(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A7C0, dst);
}

/* "ITEM01\ITEM_023.TEX" */
s32 func_00265D10(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AB40, dst);
}

/* "ITEM01\ITEM_024.TEX" */
s32 func_00265DF0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AB20, dst);
}

/* "ITEM01\ITEM_027.TEX" */
s32 func_00265EA0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AB00, dst);
}

/* "ITEM01\ITEM_02B.TEX" */
s32 func_00265F50(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AAE0, dst);
}

/* "ITEM01\ITEM_025.TEX" */
s32 func_00266000(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AAC0, dst);
}

/* "ITEM01\ITEM_028.TEX" */
s32 func_002660B0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AAA0, dst);
}

/* "ITEM01\ITEM_026.TEX" */
s32 func_00266160(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AA80, dst);
}

/* "ITEM01\ITEM_029.TEX" */
s32 func_00266210(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AA60, dst);
}

/* "ITEM01\ITEM_02A.TEX" */
s32 func_002662C0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045AA40, dst);
}

/* "ITEM03\ITEM_04A.TEX" */
s32 func_002663F0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A9A0, dst);
}

/* "ITEM03\ITEM_053.TEX" */
s32 func_002664D0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A980, dst);
}

/* "ITEM03\ITEM_054.TEX" */
s32 func_00266580(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A960, dst);
}

/* "ITEM03\ITEM_055.TEX" */
s32 func_00266630(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A940, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
s32 func_002666E0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A920, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
s32 func_00266790(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A900, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
s32 func_00266840(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8E0, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
s32 func_002668F0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8C0, dst);
}

/* "ITEM00\ITEM_FFF.TEX" */
s32 func_002669A0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045A8A0, dst);
}

/* "ITEM00\ITEM_00A.TEX" */
s32 func_00266A50(void *o, s32 dst) {
    return ITEM_LOAD(D_0045ACA0, dst);
}

/* "ITEM00\ITEM_009.TEX" */
s32 func_00266BE0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045ACC0, dst);
}

/* "ITEM00\ITEM_006.TEX" */
s32 func_002EEBD0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045E280, dst);
}

/* "ITEM00\ITEM_007.TEX" */
s32 func_00303CA0(void *o, s32 dst) {
    return ITEM_LOAD(D_0045EA80, dst);
}

/* "ITEM04\ITEM_062.TEX" */
s32 func_0035BB90(void *o, s32 dst) {
    return ITEM_LOAD(D_00463360, dst);
}

/* ---- the base pool entry's (an item's) stack ---- */

#define ITEM_HELD(o) (VCALL(o, 0x18, s32 (*)(void *))(o) & 0xFF)

/* +0x2C add `n` to a stack (+0x10, at most 98 more than the first): 1 if it took them */
s32 func_0025FD00(void *o, s32 n) {
    if (AT(o, 0x4, s32) != -1 && ITEM_HELD(o) == 1 && AT(o, 0x10, u32) < 98) {
        AT(o, 0x10, u32) += n & 0xFF;
        if (AT(o, 0x10, u32) >= 99) {
            AT(o, 0x10, u32) = 98;
        }
        return 1;
    }
    return 0;
}

/* +0x30 take one off a stack: 1 if there was more than one */
s32 func_0025FDA0(void *o) {
    if (AT(o, 0x4, s32) != -1 && ITEM_HELD(o) == 1 && AT(o, 0x10, s32) != 0) {
        AT(o, 0x10, s32) -= 1;
        return 1;
    }
    return 0;
}

/* +0x34 how many there are (0: none) */
s32 func_0025FE10(void *o) {
    if (AT(o, 0x4, s32) == -1) {
        return 0;
    }
    return ITEM_HELD(o) == 1 ? AT(o, 0x10, s32) + 1 : 1;
}

/* +0x38 a counter (held 2) counts one more, unless it is -1 */
s32 func_0025FE70(void *o) {
    if (AT(o, 0x4, s32) != -1 && ITEM_HELD(o) == 2 && AT(o, 0x10, s32) != -1) {
        AT(o, 0x10, s32) += 1;
    }
    return 0;
}

extern s32 func_00138EC0(void *partner);
extern void *gCharPartner;

/* +0x40 the partner's func_00138EC0 (0 without one) */
s32 func_0025FF10(void *o) {
    if (gCharPartner != NULL) {
        return func_00138EC0(gCharPartner);
    }
    return 0;
}

/* ---- +0x3C: using an item ----
 * Returns what the menu does next: 0 nothing happens, 1 used up, 2 a flag set, 4 an event
 * started (Fiona's state 5). */

#include "progress.h"

extern VObject *D_0044E4D0;   /* the events */
extern u8 *gCharPlayer;

/* used at event spot `spot` of room `room` while Fiona stands in it: her state 5 (0, `ev`) and
   the event `ev` starts */
static s32 use_at_spot(s32 room, s32 spot, s32 ev) {
    VObject *ev_mgr;

    if (VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) != room) {
        return 0;
    }
    ev_mgr = D_0044E4D0;
    if (VCALL(ev_mgr, 0x10, s32 (*)(VObject *, u8 *, s32, s32))(ev_mgr, gCharPlayer + 0x10, spot, -1) == 0) {
        return 0;
    }
    AT(gCharPlayer, 0x14E8, s32) = 5;
    AT(gCharPlayer, 0x14EC, s32) = 0;
    AT(gCharPlayer, 0x14F0, s32) = ev;
    VCALL(ev_mgr, 0x18, void (*)(VObject *, s32, s32, u8 *))(ev_mgr, 0, ev, gCharPlayer);
    return 4;
}

/* D_0046EE40: at spot 0xA of room 0xF, event 0x1 */
s32 func_002CCC70(void *o) {
    return use_at_spot(0xF, 0xA, 0x1);
}

/* D_00477030: at spot 0x17 of room 0x48, event 0x0 */
s32 func_0033E900(void *o) {
    return use_at_spot(0x48, 0x17, 0x0);
}

/* D_00477080: at spot 0x17 of room 0x48, event 0x17 */
s32 func_0033EA70(void *o) {
    return use_at_spot(0x48, 0x17, 0x17);
}

/* D_004770D0: at spot 0x17 of room 0x48, event 0x18 */
s32 func_0033EBE0(void *o) {
    return use_at_spot(0x48, 0x17, 0x18);
}

/* D_00477120: at spot 0x17 of room 0x48, event 0x19 */
s32 func_0033ED50(void *o) {
    return use_at_spot(0x48, 0x17, 0x19);
}

/* D_00477170: at spot 0x17 of room 0x48, event 0x1A */
s32 func_0033EEC0(void *o) {
    return use_at_spot(0x48, 0x17, 0x1A);
}

/* D_00478E10: at spot 0xB of room 0xC7, event 0x4 */
s32 func_00351710(void *o) {
    return use_at_spot(0xC7, 0xB, 0x4);
}
