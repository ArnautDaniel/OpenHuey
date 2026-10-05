/* The item classes' constructors func_00263220..func_002645A0 (the ones the give-item switch,
 * func_00261090, uses): a pool entry (D_0046C790) with the item id at +0x4, then the class vtable. */
#include "common.h"

typedef struct ItemObj {
    void *vtbl;
    s32 id;
    u8 flag;
    u64 data; /* +0x10 */
} ItemObj;

extern void *D_0046C790[], *D_0046F430[];
extern void *D_0046C7E0[];
extern void *D_0046C830[];
extern void *D_0046C880[];
extern void *D_0046C970[];
extern void *D_0046C9C0[];
extern void *D_0046CA10[];
extern void *D_0046CA60[];
extern void *D_0046CAB0[];
extern void *D_0046CB00[];
extern void *D_0046CBA0[];
extern void *D_0046CBF0[];
extern void *D_0046CC40[];
extern void *D_0046CC90[];
extern void *D_0046CD30[];
extern void *D_0046CD80[];
extern void *D_0046CDD0[];
extern void *D_0046CE20[];
extern void *D_0046CEC0[];
extern void *D_0046CF10[];
extern void *D_0046CF60[];
extern void *D_0046CFB0[];
extern void *D_0046D050[];
extern void *D_0046D0A0[];
extern void *D_0046D0F0[];
extern void *D_0046D140[];
extern void *D_0046D190[];
extern void *D_0046D1E0[];
extern void *D_0046D230[];
extern void *D_0046D280[];
extern void *D_0046D2D0[];
extern void *D_0046D370[];
extern void *D_0046D3C0[];
extern void *D_0046D410[];
extern void *D_0046D460[];
extern void *D_0046D4B0[];
extern void *D_0046D500[];
extern void *D_0046D550[];
extern void *D_0046D5A0[];
extern void *D_0046D5F0[];
extern void *D_0046D690[];
extern void *D_0046D6E0[];
extern void *D_0046F610[];
extern void *D_0046F660[];
extern void *D_004730C0[];
extern void *D_00474BC0[];
extern void *D_00475300[];
extern void *D_00475350[];
extern void *D_004753A0[];
extern void *D_004753F0[];
extern void *D_00475440[];
extern void *D_00475490[];
extern void *D_004754E0[];
extern void *D_00475530[];
extern void *D_00475580[];
extern void *D_004755D0[];
extern void *D_00475620[];
extern void *D_00475670[];
extern void *D_004756C0[];
extern void *D_00475710[];
extern void *D_00475760[];
extern void *D_004757B0[];
extern void *D_00475800[];
extern void *D_00475850[];
extern void *D_00475CC0[];
extern void *D_00475D10[];
extern void *D_00475D60[];
extern void *D_00476450[];
extern void *D_00476B60[];
extern void *D_00477030[];
extern void *D_00477080[];
extern void *D_004770D0[];
extern void *D_00477120[];
extern void *D_00477170[];
extern void *D_004775C0[];
extern void *D_00478C80[];
extern void *D_00478CD0[];
extern void *D_00478D20[];
extern void *D_00478D70[];
extern void *D_00478DC0[];
extern void *D_00478E10[];
extern void *D_00478E60[];
extern void *D_00478EB0[];
extern void *D_00478F00[];
extern void *D_00478F50[];
extern void *D_00478FA0[];
extern void *D_00479820[];

ItemObj *func_00263220(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xAC;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D5F0;
    return self;
}

ItemObj *func_002632B0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xAB;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D5A0;
    return self;
}

ItemObj *func_002632E0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xAA;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D550;
    return self;
}

ItemObj *func_00263310(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA9;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D500;
    return self;
}

ItemObj *func_00263340(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA8;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D4B0;
    return self;
}

ItemObj *func_00263370(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA7;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D460;
    return self;
}

ItemObj *func_002633A0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA6;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D410;
    return self;
}

ItemObj *func_002633D0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA5;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D3C0;
    return self;
}

ItemObj *func_00263400(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA4;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D370;
    return self;
}

ItemObj *func_00263430(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA3;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00476B60;
    return self;
}

ItemObj *func_00263460(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA2;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00476450;
    return self;
}

ItemObj *func_00263490(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA1;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046F660;
    return self;
}

ItemObj *func_002634C0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xA0;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046F610;
    return self;
}

ItemObj *func_002634F0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x9B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D2D0;
    return self;
}

ItemObj *func_00263580(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x98;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D280;
    return self;
}

ItemObj *func_002635B0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x97;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D230;
    return self;
}

ItemObj *func_002635E0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x95;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D1E0;
    return self;
}

ItemObj *func_00263610(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x94;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D190;
    return self;
}

ItemObj *func_00263640(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x93;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D140;
    return self;
}

ItemObj *func_00263670(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x92;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D0F0;
    return self;
}

ItemObj *func_002636A0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x91;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D0A0;
    return self;
}

ItemObj *func_002636D0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x90;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D050;
    return self;
}

ItemObj *func_00263700(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x8D;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CFB0;
    return self;
}

ItemObj *func_00263790(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x8C;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CF60;
    return self;
}

ItemObj *func_002637C0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x8B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CF10;
    return self;
}

ItemObj *func_002637F0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x8A;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CEC0;
    return self;
}

ItemObj *func_00263820(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x89;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CE20;
    return self;
}

ItemObj *func_002638B0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x88;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CDD0;
    return self;
}

ItemObj *func_002638E0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x87;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CD80;
    return self;
}

ItemObj *func_00263910(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x86;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CD30;
    return self;
}

ItemObj *func_00263940(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x83;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CC90;
    return self;
}

ItemObj *func_002639D0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x82;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CC40;
    return self;
}

ItemObj *func_00263A00(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x81;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CBF0;
    return self;
}

ItemObj *func_00263A30(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x80;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CBA0;
    return self;
}

ItemObj *func_00263A60(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x75;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CB00;
    return self;
}

ItemObj *func_00263AF0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x74;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CAB0;
    return self;
}

ItemObj *func_00263B20(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x73;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CA60;
    return self;
}

ItemObj *func_00263B50(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x72;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046CA10;
    return self;
}

ItemObj *func_00263B80(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x71;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046C9C0;
    return self;
}

ItemObj *func_00263BB0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x70;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046C970;
    return self;
}

ItemObj *func_00263BE0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x66;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475850;
    return self;
}

ItemObj *func_00263C70(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x65;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475800;
    return self;
}

ItemObj *func_00263CA0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x64;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004757B0;
    return self;
}

ItemObj *func_00263CD0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x63;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475760;
    return self;
}

ItemObj *func_00263D00(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x62;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475710;
    return self;
}

ItemObj *func_00263D30(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x61;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004756C0;
    return self;
}

ItemObj *func_00263D60(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x60;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475670;
    return self;
}

ItemObj *func_00263D90(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x4C;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475620;
    return self;
}

ItemObj *func_00263E20(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x4B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004755D0;
    return self;
}

ItemObj *func_00263E50(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x4A;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475580;
    return self;
}

ItemObj *func_00263E80(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x49;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475530;
    return self;
}

ItemObj *func_00263EB0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x48;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004754E0;
    return self;
}

ItemObj *func_00263EE0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x47;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475490;
    return self;
}

ItemObj *func_00263F10(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x46;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475440;
    return self;
}

ItemObj *func_00263F40(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x45;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004753F0;
    return self;
}

ItemObj *func_00263F70(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x44;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004753A0;
    return self;
}

ItemObj *func_00263FA0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x43;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475350;
    return self;
}

ItemObj *func_00263FD0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x42;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475300;
    return self;
}

ItemObj *func_00264000(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x41;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D6E0;
    return self;
}

ItemObj *func_00264030(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x40;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D690;
    return self;
}

ItemObj *func_00264090(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x3E;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046C880;
    return self;
}

ItemObj *func_00264120(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x29;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00479820;
    return self;
}

ItemObj *func_00264150(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x28;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478FA0;
    return self;
}

ItemObj *func_00264180(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x27;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478F50;
    return self;
}

ItemObj *func_002641B0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x26;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478F00;
    return self;
}

ItemObj *func_002641E0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x25;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478EB0;
    return self;
}

ItemObj *func_00264210(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x24;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478E60;
    return self;
}

ItemObj *func_00264240(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x23;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478E10;
    return self;
}

ItemObj *func_00264270(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x22;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478DC0;
    return self;
}

ItemObj *func_002642A0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x21;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478D70;
    return self;
}

ItemObj *func_002642D0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x20;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478D20;
    return self;
}

ItemObj *func_00264300(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1F;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478CD0;
    return self;
}

ItemObj *func_00264330(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1E;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00478C80;
    return self;
}

ItemObj *func_00264360(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1D;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004775C0;
    return self;
}

ItemObj *func_00264390(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1C;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00477170;
    return self;
}

ItemObj *func_002643C0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1B;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00477120;
    return self;
}

ItemObj *func_002643F0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x1A;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004770D0;
    return self;
}

ItemObj *func_00264420(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x19;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00477080;
    return self;
}

ItemObj *func_00264450(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x18;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00477030;
    return self;
}

ItemObj *func_00264480(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x17;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475D60;
    return self;
}

ItemObj *func_002644B0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x16;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475D10;
    return self;
}

ItemObj *func_002644E0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x15;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00475CC0;
    return self;
}

ItemObj *func_00264510(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x14;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_00474BC0;
    return self;
}

ItemObj *func_00264540(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x13;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046C830;
    return self;
}

ItemObj *func_00264570(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x12;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046C7E0;
    return self;
}

ItemObj *func_002645A0(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0x11;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_004730C0;
    return self;
}
/* item 0x3F's class takes its id as an argument; func_00261090 calls it without one, so the id
 * is whatever a1 held: the object's own address (see there) */
ItemObj *func_00264060(ItemObj *self, s32 id) {
    self->vtbl = D_0046C790;
    self->id = id;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046F430;
    return self;
}
