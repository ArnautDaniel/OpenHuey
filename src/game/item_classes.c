/* The item classes (pool entries, vtable D_0046C790): their destructors. Each item has its own
 * class (+0x28 loads its picture, +0x3C uses it) on one of a few middle classes (D_0046C8D0
 * the plain item, D_0046CB50 ..., see the vtables); the destructors only step the vtable back
 * down the chain and free the entry. */
#include "common.h"
#include "game.h"
#include "item.h"
#include "globals.h"
#include "progress.h"
#include "actor.h"

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

/* operator delete for pool entries: nothing (the pool is dropped at once) */
void func_0025FEF0(void *p) {
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


/* used at event spot `spot` of room `room` while Fiona stands in it: event `ev` */
static s32 use_at_spot(s32 room, s32 spot, s32 ev) {
    VObject *ev_mgr;

    if (VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) != room) {
        return 0;
    }
    ev_mgr = gEvents;
    if (!item_at_spot(ev_mgr, gCharPlayer, spot)) {
        return 0;
    }
    item_event(ev_mgr, 0, ev, gCharPlayer);
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

/* use: composure -100, stamina -1800 */
s32 func_00266A80(void *o) {
    item_meters(-100.0f, -1800);
    return 1;
}

/* use: Progress +0x9E0 -0.2666 for 300 frames (+0x9E4) */
s32 func_00266C10(void *o) {
    AT(gProgress, 0x9E0, u32) = 0xBE888889;   /* -0.26666668 */
    AT(gProgress, 0x9E4, s32) = 300;
    return 1;
}

/* used at open door `door` of room `room`: event `ev`, flag 0x18 */
static s32 use_at_door(s32 room, u32 door, s32 ev) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != room || !item_door_open(p, door)) {
        return 0;
    }
    item_event(gEvents, 0, ev, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* D_00473020: at door 0 of room 0x4F, event 1 */
s32 func_0031D050(void *o) {
    return use_at_door(0x4F, 0, 1);
}

/* D_004730C0: at door 4 of room 0x55, event 2 */
s32 func_0031D630(void *o) {
    return use_at_door(0x55, 4, 2);
}

/* D_00474BC0: at door 1 of room 0x25 unless Progress +0x20 bit 0x800000, event 4 */
s32 func_0032CD30(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != 0x25 || (AT(p, 0x20, u32) & 0x800000) || !item_door_open(p, 1)) {
        return 0;
    }
    item_event(gEvents, 0, 4, gCharPlayer);
    Progress_SetFlag(p, 0x18);
    return 4;
}

/* D_00478F00: at spot 3 of room 0x92: flag 0x18, event 4 */
s32 func_00351B40(void *o) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0x92, 3)) {
        return 0;
    }
    Progress_SetFlag(p, 0x18);
    item_event(gEvents, 0, 4, gCharPlayer);
    return 4;
}


/* no use here: unless Progress +0x30 bit 0x8000, while item `id` is held, a sound (bank 0xC,
   5); else nothing */
static s32 use_sound_only(s32 need_24_4, s32 id) {
    Progress *p = gProgress;

    if ((AT(p, 0x30, u32) & 0x8000) || (need_24_4 && !(AT(p, 0x24, u32) & 4)) ||
        VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, id) == 0) {
        return 0;
    }
    VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0xC, 5);
    return 8;
}

/* D_0046D370 */
s32 func_00266370(void *o) {
    return use_sound_only(0, 0x24B);
}

/* D_004703F0: at door 0 of room 6: event 1, flag 0x18; else (with Progress +0x24 bit 4) the
   sound while item 0x232 is held */
s32 func_002EEC00(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 6 && item_door_open(p, 0)) {
        item_event(gEvents, 0, 1, gCharPlayer);
        Progress_SetFlag(p, 0x18);
        return 4;
    }
    return use_sound_only(1, 0x232);
}

/* at spot 0x11 of room 0xF once route 8 is open (func_00178610): event 0x16; else on the altar */
static s32 use_route8_or_offer(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0xF && func_00178610(p, 8) != 0 &&
        item_at_spot(gEvents, gCharPlayer, 0x11)) {
        item_event(gEvents, 0, 0x16, gCharPlayer);
        return 4;
    }
    return item_offer(p, o);
}

/* D_0046C7E0 */
s32 func_00264A60(void *o) {
    return use_route8_or_offer(o);
}

/* D_0046C830 */
s32 func_00264CE0(void *o) {
    return use_route8_or_offer(o);
}

/* D_00470FB0: at open door 1 of room 0x14: event 4, flag 0x18; else on the altar */
s32 func_00303CD0(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x14 && item_door_open(p, 1)) {
        item_event(gEvents, 0, 4, gCharPlayer);
        Progress_SetFlag(p, 0x18);
        return 4;
    }
    return item_offer(p, o);
}

/* at spot 0xB of room 0xC7: event 3, or 0x10 with Progress +0x2C bit 0x4000 */
static s32 use_room_c7(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0xC7, 0xB)) {
        return 0;
    }
    item_event(gEvents, 0, (AT(p, 0x2C, u32) & 0x4000) ? 0x10 : 3, gCharPlayer);
    return 4;
}

/* D_00478D70 */
s32 func_00351390(void *o) {
    return use_room_c7();
}

/* D_00478DC0 */
s32 func_00351550(void *o) {
    return use_room_c7();
}

/* D_00478EB0: at room 0x82's open door 0 with route 0xE2: event 2; at room 0x8C's with route
   0xE6: event 8 */
s32 func_00351920(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x82) {
        return item_door_route(p, 0, 0xE2) ? item_event_flag(p, 2) : 0;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x8C && item_door_route(p, 0, 0xE6)) {
        return item_event_flag(p, 8);
    }
    return 0;
}

/* (as func_00351D80) at spot 3 of room 0x53: event 0; else on the altar */
s32 func_0031CE10(void *o) {
    Progress *p = gProgress;

    if (item_room_spot(p, 0x53, 3)) {
        return item_event_flag(p, 0);
    }
    return item_offer(p, o);
}

/* D_00478FA0: at spot 5 of room 0: event 0xF; else on the altar */
s32 func_00351D80(void *o) {
    Progress *p = gProgress;

    if (item_room_spot(p, 0, 5)) {
        return item_event_flag(p, 0xF);
    }
    return item_offer(p, o);
}

/* D_00472F80: in room 0x4B, at open door 2 with route 0x51: event 1; door 3 with route 0x52:
   event 3; else on the altar */
s32 func_0031CB40(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x4B) {
        if (item_door_route(p, 2, 0x51)) {
            return item_event_flag(p, 1);
        }
        if (item_door_route(p, 3, 0x52)) {
            return item_event_flag(p, 3);
        }
    }
    return item_offer(p, o);
}

/* where one of the five medallions (D_00471080 ..) is placed: room, spot, the events' +0x30
 * mode (-1: not called) and Fiona's event; room 0x40's only without Progress +0x2C bit 0x20 */
typedef struct ItemSlot {
    u8 room, spot;
    s8 mode;
    u8 ev;
} ItemSlot;

static s32 place_item(void *o, const ItemSlot *t, s32 n) {
    Progress *p = gProgress;
    s32 i;

    for (i = 0; i < n; i++, t++) {
        if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != t->room) {
            continue;
        }
        if (t->room == 0x40 && (AT(p, 0x2C, u32) & 0x20)) {
            continue;
        }
        if (!item_at_spot(gEvents, gCharPlayer, t->spot)) {
            continue;
        }
        if (t->mode >= 0) {
            VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, t->mode, AT(o, 0x4, s32));
        }
        return item_event_flag(p, t->ev);
    }
    return 0;
}

s32 func_00306BC0(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1}, {0x56, 4, 0, 0xA}, {0x57, 0xD, 1, 3},
        {0x69, 0xB, 1, 4},  {0x47, 8, -1, 1}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_003071D0(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1},   {0x47, 8, 0, 2}, {0x56, 4, 0, 0xA},
        {0x69, 0xB, 1, 4},  {0x57, 0xD, -1, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_003077E0(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1},   {0x47, 8, 0, 2}, {0x56, 4, 0, 0xA},
        {0x57, 0xD, 1, 3},  {0x69, 0xB, -1, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_00307DF0(void *o) {
    static const ItemSlot t[7] = {
        {0x47, 8, 0, 2},    {0x56, 4, 0, 0xA}, {0x57, 0xD, 1, 3}, {0x69, 0xB, 1, 4},
        {0x40, 0x11, 0, 0}, {0x42, 5, -1, 0},  {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

s32 func_00308400(void *o) {
    static const ItemSlot t[7] = {
        {0x42, 5, 1, 1},   {0x47, 8, 0, 2},   {0x56, 4, 0, 0xA},    {0x57, 0xD, 1, 3},
        {0x69, 0xB, 1, 4}, {0x40, 0x11, 0, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* the three statues' pedestals (room 0x54, spots 0xD / 0x11 / 0x14): a pedestal takes the
 * statue while neither of its two Progress +0x28 bits is set; else on the altar */
typedef struct ItemPedestal {
    u32 taken;   /* +0x28 bits that block it */
    u8 spot, ev;
} ItemPedestal;

static s32 place_statue(void *o, const ItemPedestal *t) {
    Progress *p = gProgress;
    s32 i;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x54) {
        for (i = 0; i < 3; i++, t++) {
            if (!(AT(p, 0x28, u32) & t->taken) && item_at_spot(gEvents, gCharPlayer, t->spot)) {
                return item_event_flag(p, t->ev);
            }
        }
    }
    return item_offer(p, o);
}

/* D_00475CC0 */
s32 func_00338FA0(void *o) {
    static const ItemPedestal t[3] = {{0x180, 0xD, 0}, {0xC00, 0x11, 3}, {0x6000, 0x14, 6}};

    return place_statue(o, t);
}

/* D_00475D10 */
s32 func_00339350(void *o) {
    static const ItemPedestal t[3] = {{0xA00, 0x11, 4}, {0x140, 0xD, 1}, {0x5000, 0x14, 7}};

    return place_statue(o, t);
}

/* D_00475D60 */
s32 func_00339700(void *o) {
    static const ItemPedestal t[3] = {{0x3000, 0x14, 8}, {0xC0, 0xD, 2}, {0x600, 0x11, 5}};

    return place_statue(o, t);
}

/* D_00473070: the medallions' slots without the altar */
s32 func_0031D1B0(void *o) {
    static const ItemSlot t[5] = {
        {0x42, 5, 1, 1}, {0x47, 8, 0, 2}, {0x57, 0xD, 1, 3}, {0x69, 0xB, 1, 4}, {0x56, 4, -1, 9},
    };

    return place_item(o, t, 5);
}

extern f32 func_00124490(void *a, const f32 *p);   /* distance */

/* D_004775C0: in room 0x52 with Hewie at hand (+0x40), up and within 20, Fiona not busy
 * (+0xE8): event 0xB; else on the altar; else the sound while item 0x239 is held, or (with
 * Progress +0x30 bit 0x8000) while Hewie is in the room being played and item 0x27F is held */
s32 func_003445D0(void *o) {
    Progress *p = gProgress;
    VObject *ev_mgr = gEvents;
    s32 r;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x52) {
        u8 *h = (u8 *)gCharPartner;

        if (VCALL(o, 0x40, s32 (*)(void *))(o) != 0 && AT(h, 0xC4, s32) != 2 && AT(gCharPlayer, 0xE8, s32) == 0 &&
            func_00124490(gCharPlayer, (f32 *)(h + 0x10)) < 20.0f) {
            item_event(ev_mgr, 0, 0xB, gCharPlayer);
            return 4;
        }
    }
    r = item_offer(p, o);
    if (r != 0) {
        return r;
    }
    if (!(AT(p, 0x30, u32) & 0x8000)) {
        return use_sound_only(0, 0x239);
    }
    {
        s32 room = AT(gCharPartner, 0x30, s32);

        if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
            VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, 0x27F) != 0) {
            VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0xC, 5);
            return 8;
        }
    }
    return 0;
}

/* free the file loader's 0x4000000 area if it is in state 2 */
void func_002600C0(void) {
    VObject *ld = gFileLoader;

    if (VCALL(ld, 0x28, s32 (*)(VObject *, s32))(ld, 0x4000000) == 2) {
        VCALL(ld, 0x14, void (*)(VObject *, s32))(ld, 0x4000000);
    }
}

/* an item's +0x18 set from a message { kind (0xFF: none), sub, byte 2, pad, word }: +0x4 0 when
 * unset (+0x5 0xFF), +0x7 the kind, +0x5 the sub (0xFF without a kind), +0x6, +0xC; +0x8 0 */
void func_0035BC30(u8 *o, const u8 *m) {
    if (m == NULL) {
        return;
    }
    if (o[5] == 0xFF) {
        o[4] = 0;
    }
    o[7] = m[0];
    if (o[7] == 0xFF) {
        o[5] = 0xFF;
    } else {
        o[5] = m[1];
    }
    o[6] = m[2];
    AT(o, 0xC, s32) = AT(m, 0x4, s32);
    AT(o, 0x8, s32) = 0;
}

extern void func_002FF650(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* the room-object glow (vtable D_00479870, made by func_0034B210) +0x10 update: 0 while unset
 * (+0x5 0xFF). On (+0x7) it fades in (+0x4 up to 0x40 by 2), off it fades out; at 0 it is unset,
 * frees its script variable (+0x6, events +0x30) and plays sound 6 at the object (+0xC, +0x20)
 * unless the camera director's +0x38 is set - all 0. Its phase +0x8 runs 0..45 by 2 a frame */
s32 func_0035C8C0(u8 *o) {
    f32 t;

    if (o[5] == 0xFF) {
        return 0;
    }
    if (o[7] != 0) {
        if (o[4] != 0x40) {
            o[4] += 2;
        }
    } else {
        o[4] -= 2;
        if (o[4] == 0) {
            o[5] = 0xFF;
            VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, o[6], -1);
            if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0) {
                func_002FF650(gSound, 2, 6, (f32 *)(AT(o, 0xC, u8 *) + 0x20), 0, 0);
            }
            return 0;
        }
    }
    t = AT(o, 0x8, f32) + 2.0f;
    AT(o, 0x8, f32) = t;
    if (!(t < 45.0f)) {
        AT(o, 0x8, f32) = t - 45.0f;
    }
    return 1;
}

extern const char D_0045D818[], D_0045D820[], D_0045D828[], D_0045D838[], D_0045D840[], D_0045D848[],
    D_0045D850[], D_0045D858[], D_0045D860[], D_0045D870[], D_0045D878[], D_0045D880[];
extern f32 func_00124490(void *a, const f32 *pos);   /* distance */

/* the word item `it` carries (vtable +0x20) is `w` (the first 8 letters) */
static inline s32 item_named(VObject *it, const char *w) {
    const char *n = VCALL(it, 0x20, const char *(*)(VObject *))(it);
    const char *q = w;
    u32 i;

    for (i = 0; i < 8; i++, q++) {
        if (n[i] != *q) {
            return 0;
        }
        if (n[i] == 0) {
            break;
        }
    }
    return 1;
}

/* the elements' flag `bit` (Progress +0x1C bits) set */
static inline void item_progress_bit(Progress *p, s32 bit) {
    AT(p, 0x1C + (bit >> 5) * 4, u32) |= 1 << (bit & 0x1F);
}

/* +0x3C use (the word plate, vtable D_0046F430): what Fiona does with the word it carries, room
 * by room (6: an event started) -
 *  0x23, spot 3: event 5
 *  0x24, spot 6, until progress +0x1C bit 27: "EMETH" (bit 26, event 0xA) / "METH" (bit 25,
 *        event 0xB) / "SALTATIO" (event 0xE), each with flag 0x18; anything else event 5
 *  0x26, spot 0xA, until +0x7C bit 12: "MAGNUS" event 0xB (flag 0x18), else 0xC
 *  0x0F, route 8 taken, spot 0x11: "REST" event 0x14 (flag 0x18), else 0x17
 *  0x49, spot 2: event 1
 *  0x66: spot 0x14 event 9; spot 0x16 "SALTATIO" event 0x11, else the word to the events (+0x30
 *        0 / 1) and event 0xA
 *  0x4F, until +0x24 bit 16: the elements "MERCURY" / "SULFUR" / "SALT" (flags 0x4D..0x4F, done:
 *        +0x24 bits 13..15). With two done and Hewie with her (or at spot 0xA): event 5, flag
 *        0x18; else spot 0xA event 0xA (4); spots 8 / 9 (until +0x24 bits 17 / 18) events 6 / 7.
 *        The word given: its flag and the events' +0x5C, else +0x60
 *  0x27 with +0x30 bit 14, spot 3: "ALCHYMIA" / "ADAMAS" / "POWDER" / "MORGAN" (not yet used:
 *        +0x88 bits 8..11) to the events (+0x30 1: 0x88 / 0x8C / 0x83 / 0x89) and event 8, else 9
 * elsewhere 0 */
s32 func_002D27E0(VObject *it) {
    Progress *p = gProgress;
    VObject *ev = gEvents;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x23 && item_at_spot(ev, gCharPlayer, 3)) {
        item_event(ev, 0, 5, gCharPlayer);
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x24 && !(AT(p, 0x1C, u32) & 0x8000000) &&
        item_at_spot(ev, gCharPlayer, 6)) {
        if (item_named(it, D_0045D818)) {
            AT(p, 0x1C, u32) |= 0x4000000;
            item_event(ev, 0, 0xA, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else if (item_named(it, D_0045D820)) {
            AT(p, 0x1C, u32) |= 0x2000000;
            item_event(ev, 0, 0xB, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else if (item_named(it, D_0045D828)) {
            item_event(ev, 0, 0xE, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 5, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x26 && !(AT(p, 0x7C, u32) & 0x1000) &&
        item_at_spot(ev, gCharPlayer, 0xA)) {
        if (item_named(it, D_0045D838)) {
            item_event(ev, 0, 0xB, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 0xC, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0xF && func_00178610(p, 8) != 0 &&
        item_at_spot(ev, gCharPlayer, 0x11)) {
        if (item_named(it, D_0045D840)) {
            item_event(ev, 0, 0x14, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 0x17, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x49 && item_at_spot(ev, gCharPlayer, 2)) {
        item_event(ev, 0, 1, gCharPlayer);
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x66) {
        if (item_at_spot(ev, gCharPlayer, 0x14)) {
            item_event(ev, 0, 9, gCharPlayer);
            return 6;
        }
        if (item_at_spot(ev, gCharPlayer, 0x16)) {
            if (item_named(it, D_0045D828)) {
                item_event(ev, 0, 0x11, gCharPlayer);
            } else {
                const s32 *w = VCALL(it, 0x20, const s32 *(*)(VObject *))(it);

                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, w[0]);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, w[1]);
                item_event(ev, 0, 0xA, gCharPlayer);
            }
            return 6;
        }
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x4F && !(AT(p, 0x24, u32) & 0x10000)) {
        s32 given = 0, done = 0, bit = -1;

        if (AT(p, 0x24, u32) & 0x2000) {
            done = 1;
        } else if (item_named(it, D_0045D848)) {
            given = 1;
            bit = 0x4D;
        }
        if (AT(p, 0x24, u32) & 0x4000) {
            done++;
        } else if (item_named(it, D_0045D850)) {
            given = 1;
            bit = 0x4E;
        }
        if (AT(p, 0x24, u32) & 0x8000) {
            done++;
        } else if (item_named(it, D_0045D858)) {
            given = 1;
            bit = 0x4F;
        }
        if (done == 2) {
            u8 *h = (u8 *)gCharPartner;

            if (h != NULL && AT(h, 0x28, u8) == 1 && AT(h, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
                AT(h, 0xC4, s32) != 2 &&
                (item_at_spot(ev, gCharPlayer, 0xA) || func_00124490(gCharPlayer, (f32 *)(h + 0x10)) < 20.0f)) {
                item_event(ev, 0, 5, gCharPlayer);
                Progress_SetFlag(p, 0x18);
                if (!given) {
                    VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
                } else {
                    item_progress_bit(p, bit);
                    VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
                }
                return 6;
            }
            if (item_at_spot(ev, gCharPlayer, 0xA)) {
                item_event(ev, 0, 0xA, gCharPlayer);
                return 4;
            }
        } else if (item_at_spot(ev, gCharPlayer, 8) && !(AT(p, 0x24, u32) & 0x20000)) {
            item_event(ev, 0, 6, gCharPlayer);
            if (!given) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            } else {
                item_progress_bit(p, bit);
                Progress_SetFlag(p, 0x18);
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
            }
            return 6;
        } else if (item_at_spot(ev, gCharPlayer, 9) && !(AT(p, 0x24, u32) & 0x40000)) {
            item_event(ev, 0, 7, gCharPlayer);
            if (!given) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            } else {
                item_progress_bit(p, bit);
                Progress_SetFlag(p, 0x18);
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
            }
            return 6;
        }
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x27 && (AT(p, 0x30, u32) & 0x4000) &&
        item_at_spot(ev, gCharPlayer, 3)) {
        if (item_named(it, D_0045D860) && !(AT(p, 0x88, u32) & 0x100)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x88);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D870) && !(AT(p, 0x88, u32) & 0x200)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x8C);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D878) && !(AT(p, 0x88, u32) & 0x400)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x83);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D880) && !(AT(p, 0x88, u32) & 0x800)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x89);
            item_event(ev, 0, 8, gCharPlayer);
        } else {
            item_event(ev, 0, 9, gCharPlayer);
        }
        return 6;
    }
    return 0;
}

#ifdef HG_NATIVE
#include "gl2d.h"
#include "sce/libvu0.h"

extern void *D_0046D7A0[], *D_00469D00[];
extern void func_0026B180(void *drawer, u32 rgba, s32 layer, s32 sub);
extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */

/* is clip-space point `p` (camera +0x48) inside the view */
static s32 glow_in_view(f32 (*clip)[4], const f32 *pt) {
    f32 v[4] __attribute__((aligned(16)));

    sceVu0ApplyMatrix(v, clip, (f32 *)pt);
    return v[0] <= v[3] && !(v[0] < -v[3]) && v[1] <= v[3] && !(v[1] < -v[3]) && v[2] <= v[3] && !(v[2] < -v[3]);
}

/* the glow +0x14 draw (while set): around its room object (+0xC, at floor height -1)
 * - a disc of radius r (by its kind +0x5) fading out from the middle (alpha +0x4), added into
 *   layer 0x26 (the bloom's mask); only when all of it is in view (kind 0: the disc left out
 *   then, and the screen also brightened, func_0026B180 0x40808080 in layer 0x28)
 * - eight faint cyan strips standing 40 high at radius r + 5, turning with its phase (+0x8),
 *   added in layer 2, when all in view */
void func_0035BCA0(u8 *o) {
    static const union { u32 u; f32 f; } kRadius[8] = {
        {0x409A3D71}, {0x40800000}, {0x407F5C29}, {0x409D70A4}, {0x408D1EB8}, {0x407C28F6}, {0x405E147B}, {0x411A3D71},
    };
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 pt[18][4] __attribute__((aligned(16)));
    u8 *obj = AT(o, 0xC, u8 *);
    f32 c[4], r, r2;
    s32 i, clipped = 0;

    if (o[5] == 0xFF) {
        return;
    }
    c[0] = AT(obj, 0x20, f32);
    c[1] = -1.0f;
    c[2] = AT(obj, 0x28, f32);
    c[3] = 1.0f;
    r = o[5] < 8 ? kRadius[o[5]].f : 0.0f;
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);

    /* the disc: the middle, then 16 points round it */
    sceVu0CopyVector(pt[0], c);
    for (i = 0; i < 16; i++) {
        f32 a = kPi.f * (22.5f * (f32)i) / 180.0f;

        pt[i + 1][0] = c[0] + r * func_0031C248(a);
        pt[i + 1][1] = c[1];
        pt[i + 1][2] = c[2] - r * func_0031C058(a);
        pt[i + 1][3] = 1.0f;
    }
    for (i = 0; i < 17; i++) {
        if (!glow_in_view(clip, pt[i])) {
            if (o[5] != 0) {
                return;
            }
            clipped = 1;
        }
    }
    if (o[5] == 0) {
        u8 drawer[0x20] __attribute__((aligned(16)));

        AT(drawer, 0x0, void **) = D_0046D7A0;
        AT(drawer, 0x4, s32) = -1;
        func_0026B180(drawer, 0x40808080, 0x28, 0);
        AT(drawer, 0x0, void **) = D_00469D00;
    }
    if (!clipped) {
        f32 tri[3][4];
        f32 st[3][2] = {{0}};
        u8 col[3][4] = {{0x80, 0x80, 0x80, 0}, {0}, {0}};

        col[0][3] = o[4];
        glr_layer(0x26);
        for (i = 0; i < 16; i++) {   /* the fan, closing on its first rim point */
            sceVu0CopyVector(tri[0], pt[0]);
            sceVu0CopyVector(tri[1], pt[i + 1]);
            sceVu0CopyVector(tri[2], pt[(i + 1) % 16 + 1]);
            AT(&tri[0][3], 0, u32) = AT(&tri[1][3], 0, u32) = AT(&tri[2][3], 0, u32) = 0;
            glr_strip((const f32 *)clip, 3, &tri[0][0], &st[0][0], &col[0][0], NULL, 0,
                      0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
        }
        glr_layer(-1);
    }

    /* the strips: the middle at the bottom and 40 up, then 8 pairs round it */
    r2 = 5.0f + r;
    sceVu0CopyVector(pt[0], c);
    sceVu0CopyVector(pt[1], c);
    pt[1][1] += 40.0f;
    for (i = 0; i < 8; i++) {
        f32 a = kPi.f * (45.0f * (f32)i + AT(o, 0x8, f32)) / 180.0f;

        sceVu0CopyVector(pt[2 + i * 2], c);
        pt[2 + i * 2][0] = c[0] + r2 * func_0031C248(a);
        pt[2 + i * 2][2] = c[2] - r2 * func_0031C058(a);
        sceVu0CopyVector(pt[3 + i * 2], pt[2 + i * 2]);
        pt[3 + i * 2][1] += 40.0f;
    }
    for (i = 0; i < 18; i++) {
        if (!glow_in_view(clip, pt[i])) {
            return;
        }
    }
    glr_layer(2);
    for (i = 0; i < 8; i++) {
        static const u8 kCol[4][4] = {{0x00, 0x20, 0x20, 0x10}, {0, 0, 0x40, 0}, {0, 0, 0x20, 0}, {0, 0, 0x10, 0}};
        f32 q[4][4];
        f32 st[4][2] = {{0}};
        s32 k;

        sceVu0CopyVector(q[0], pt[0]);
        sceVu0CopyVector(q[1], pt[2 + i * 2]);
        sceVu0CopyVector(q[2], pt[1]);
        sceVu0CopyVector(q[3], pt[3 + i * 2]);
        for (k = 0; k < 4; k++) {
            AT(&q[k][3], 0, u32) = 0;
        }
        glr_strip((const f32 *)clip, 4, &q[0][0], &st[0][0], &kCol[0][0], NULL, 0, 0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
    }
    glr_layer(-1);
}
#endif

/* progress +0x84 bits 22..26 as bits 0..4 */
u8 func_00303F00(void) {
    u32 b = AT(gProgress, 0x84, u32);
    u8 v = 0;

    if (b & 0x400000) {
        v |= 1;
    }
    if (b & 0x800000) {
        v |= 2;
    }
    if (b & 0x1000000) {
        v |= 4;
    }
    if (b & 0x2000000) {
        v |= 8;
    }
    if (b & 0x4000000) {
        v |= 0x10;
    }
    return v;
}
