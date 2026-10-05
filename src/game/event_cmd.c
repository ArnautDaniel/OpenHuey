/* The event script commands (opcodes 0x00..0xDA; 0xF0.. are control ops, func_00121730).
 * A command reads its operands at the script pc (event +0x4); afterwards the pc advances (vt
 * +0xC) unless the command waits (+0x700) or jumped (+0x701). Operands are big-endian. */
#include "common.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#endif
#include "game.h"
#include "progress.h"
#include "task.h"
#include "hewie.h"
#include "sce/libvu0.h"

extern void *gCharacters[6];
extern u8 *gCharPlayer;
extern u8 *gCharPartner;
extern VObject *D_0044E4B8;   /* the camera */
extern VObject *D_0044E568;   /* the rooms */
extern u8 D_0047B350;         /* the message language set */
extern VObject *D_00456E00;
extern void func_0016D480(Progress *p, s32 room);
extern void func_001793A0(Progress *p, s32 slot, s32 a, s32 b);
extern void func_001792C0(Progress *p, u8 slot);
extern void func_002EC470(void *o, s32);
extern void func_00177200(Progress *p, s32 slot);
extern void func_002003C0(VObject *ev);
extern VObject *D_0044E558;   /* the doors */

/* (these return a byte the callers mask: declared s32, cast at the use) */
extern s32 func_001770D0(Progress *p, s32 id);   /* character id -> gCharacters index (0xFF) */
extern s32 func_001785B0(Progress *p, s32 room, u32 exit);
extern s32 func_00178300(Progress *p, s32 room, s32 n, s32 partner);
extern s32 func_00177620(Progress *p);
extern s32 func_001FBF70(VObject *ev, s32 id);
extern void func_001FBE90(VObject *ev, s32 a, s32 b);
extern VObject *D_0044E570;   /* the nav mesh */
extern u8 *D_0044E4C0;        /* the room effects */
extern u8 *func_00266C40(void *fx, s32 k);   /* effect slot k (NULL: none) */
extern void *func_002672F0(u32 size, void *place);   /* placement new */
extern u8 *func_00208ED0(u8 *e);                   /* a D_0046FF40 effect */
extern u8 *func_00208EF0(u8 *e);                   /* a D_0046FF00 effect */
extern VObject *D_0044E4C8;   /* the scene's lights */
extern VObject *D_0044E560;   /* the sound driver */
extern VObject *D_0044E4F0;   /* the renderer */
extern void func_00122C20(u8 *c, s32 a, s32 b, s32, s32, s32);
extern s32 func_001F4770(u8 *model, s32, s32, s32);
extern void func_001267F0(u8 *c, s32 n);
extern s32 func_00266C70(u8 *fx, s32 n, void *arg);
extern void *gCharPursuer;
extern VObject *D_0044F808;   /* the stalker */
extern VObject *D_0044FE08;   /* the obstacles */
extern VObject *D_0044E4F8;   /* the camera director's interface */
extern void *D_0044E958;      /* the movie playing */
extern VObject *D_0044E988;   /* the item manager */
extern void func_002EF9E0(void *o, f32 v);
extern void func_002EC3C0(void *o, u32 id);
extern void func_002EC450(void *o, u32 id);
extern void func_002670F0(u8 *fx, s32 n);
extern void func_00182E80(u8 *f);
extern void func_0019A280(u8 *f, s32 n);
extern void func_0019A210(u8 *f, s32 n);
extern u8 *D_003D6760[];   /* the built-in action scripts (ids 0x80..) */
/* opcode groups handled elsewhere */
extern void func_001FFE00(VObject *ev);
extern void func_002013F0(VObject *ev);
extern void func_00200870(VObject *ev);
extern void func_00200B00(VObject *ev);

#define PC(ev) AT(ev, 0x4, u8 *)
#define EV_WAIT(ev) AT(ev, 0x700, u8)
#define EV_JUMPED(ev) AT(ev, 0x701, u8)
#define SG_CONTROL 0x1FBEC1   /* from the progress (SceneGame +0x40): 0 Fiona is controlled */

static inline u32 be16(const u8 *p) {
    return (p[0] << 8 | p[1]) & 0xFFFF;
}

static inline s32 be32(const u8 *p) {
    return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

/* the character with script id `id` (NULL: none) */
static u8 *char_by_id(Progress *p, s32 id) {
    u8 i = (u8)func_001770D0(p, id);

    return i != 0xFF ? (u8 *)gCharacters[i] : NULL;
}

/* 0x00: a door / exit check */
static void cmd_exit(VObject *ev, Progress *p, const u8 *pc) {
    s32 state;

    if (pc[1] & 0x80) {
        AT(p, 0x4, s32) = 1;
        AT(ev, 0x702, u8) = PC(ev)[1];
        return;
    }
    if (AT(p, SG_CONTROL, u8) == 0) {
        state = AT(gCharPlayer, 0xF8, s32);
        if ((u8)func_001785B0(p, AT(ev, 0x560, s32), pc[1]) == 1) {
            return;
        }
        if ((u8)func_00178300(p, AT(ev, 0x560, s32), PC(ev)[1], 0) == 0) {
            return;
        }
        if (state != 0 && state != 10) {
            return;
        }
    } else {
        state = AT(gCharPartner, 0xF8, s32);
        if ((u8)func_001785B0(p, AT(ev, 0x560, s32), pc[1]) == 1) {
            return;
        }
        if ((u8)func_00178300(p, AT(ev, 0x560, s32), PC(ev)[1], 1) == 0) {
            return;
        }
        if (state != 0) {
            return;
        }
    }
    AT(ev, 0x702, u8) = VCALL(p, 0x10, u8 (*)(Progress *, s32))(p, PC(ev)[1]);
}

/* make character `id` do its scripted action `act` (a script-driven state 5) */
static void char_script_action(VObject *ev, Progress *p, s32 now) {
    const u8 *pc = PC(ev);
    s32 act = pc[3];
    u8 *c = char_by_id(p, pc[2]);

    AT(c, 0x14E8, s32) = 5;
    AT(c, 0x14EC, s32) = now;
    AT(c, 0x14F0, s32) = act;
    pc = PC(ev);
    VCALL(ev, 0xE0, void (*)(VObject *, s32, s32))(ev, func_001770D0(p, pc[2]), pc[3]);
}

/* 0x05 / 0x92: start a character's scripted action (0x92 with operand 1: at once) */
static void cmd_action(VObject *ev, Progress *p, const u8 *pc) {
    const u8 *q;

    if (AT(p, 0x4, s32) != 0) {
        return;
    }
    if (pc[2] >= 0xF0 && pc[2] < 0xFB) {
        u8 k = (u8)func_001FBF70(ev, pc[2]);

        if (AT(ev, 0x564 + k * 0x18, s32) == 0 || PC(ev)[0] == 0x92) {
            func_001FBE90(ev, PC(ev)[2], PC(ev)[3]);
        }
    } else {
        u8 *c = char_by_id(p, pc[2]);

        if (c != NULL) {
            q = PC(ev);
            if (q[0] == 0x92 && q[1] == 1) {
                char_script_action(ev, p, 1);
            } else if ((AT(c, 0xE0, u8) == 0 && AT(c, 0x14E8, s32) != 5) || q[0] == 0x92) {
                char_script_action(ev, p, 0);
            }
        }
    }
    q = PC(ev);
    if (q[0] == 0x92 && q[2] == 0) {
        VCALL(p, 0x44, void (*)(Progress *, s32))(p, 0);
    }
}

/* 0x49: end a character's scripted action */
static void cmd_action_end(VObject *ev, Progress *p, const u8 *pc) {
    u8 *c;
    u8 i;

    if (pc[1] >= 0xF0 && pc[1] < 0xFB) {
        u8 k = (u8)func_001FBF70(ev, pc[1]);

        AT(ev, 0x564 + k * 0x18, s32) = 0;
        return;
    }
    i = (u8)func_001770D0(p, pc[1]);
    if (i != 0xFF) {
        AT(ev, 0x57C + i * 0x18, s32) = 0;
    }
    c = char_by_id(p, PC(ev)[1]);
    if (c == NULL || AT(c, 0x28, u8) != 1) {
        c = NULL;
    }
    if (c != NULL && AT(c, 0xE0, u8)) {
        AT(c, 0xE1, u8) = 0;
        AT(c, 0xF4, s32) = 1;
    }
}

static void set_pending_ending(Progress *p) {
    AT(p, 0x1134, u32) = 0x80000005;
    AT(p, 0x113C, s32) = 0;
    AT(p, 0x1138, s32) = 0;
    AT(p, 0x1151, u8) = 0;
    AT(p, 0x1152, u16) = 0x8016;
}

/* 0x0A: request a scene change (+0x1134) if allowed in the current situation */
static void cmd_scene_change(VObject *ev, Progress *p, const u8 *pc) {
    s32 ok = 0;
    s32 late;

    if (AT(p, 0x1134, s32) == 5) {
        return;
    }
    if ((u8)Progress_TestFlag(p, 0x12) == 1) {
        return;
    }
    if (AT(gCharPlayer, 0xE0, u8) == 1) {
        return;
    }
    late = AT(p, 0x7B8, u8) >= 4;
    switch (pc[3]) {
    case 0:
        if (!late) {
            if ((u8)func_00177620(p) == 2) {
                set_pending_ending(p);
            } else {
                ok = 1;
            }
        }
        break;
    case 1:
    case 7:
        ok = 1;
        break;
    case 2:
    case 4:
    case 5:
    case 6:
        ok = !late;
        break;
    case 3:
        if (late) {
            break;
        }
        if ((AT(p, 0x30, u32) & 0x8000) != 0 && AT(gCharPartner, 0xC4, s32) == 2) {
            break;
        }
        switch (VCALL(p, 0x58, u8 (*)(Progress *))(p)) {
        case 0:
        case 2:
            ok = 1;
            break;
        default:
            set_pending_ending(p);
            break;
        }
        break;
    }
    if (!ok) {
        return;
    }
    pc = PC(ev);
    if (pc[1] == 5 && (AT(gCharPlayer, 0xE0, u8) != 0 || AT(gCharPlayer, 0x14E8, s32) == 5)) {
        return;
    }
    AT(p, 0x1134, s32) = pc[1];
    AT(p, 0x113C, s32) = PC(ev)[2];
    AT(p, 0x1138, s32) = 0;
    AT(p, 0x1151, u8) = PC(ev)[3];
}

extern s32 func_001FC390(VObject *ev, void *c, s32 area);   /* a character's relation to an area */
extern s32 func_00171160(Progress *p, u32 id);        /* load character id as the partner */
extern s32 func_0016D670(Progress *p, u32 id, u32 slot);
extern void func_00177350(Progress *p, s32 slot);
extern void func_001772B0(Progress *p, s32 slot);
extern void func_00177300(Progress *p, s32 slot);
extern s32 func_00177260(Progress *p, s32 slot);
extern void func_002ECB50(u8 *p);
extern void func_0029EF80(void *c, s32 room);
extern s32 func_0016D6D0(Progress *p, u32 id, u32 slot);
extern void func_001FFC70(VObject *ev);
extern u8 *D_0044E980;            /* the running light / sound source */
extern VObject *D_0044E970;       /* light / sound sources */
extern VObject *D_00456DF0;       /* the music */
extern void *D_003D6A40[];        /* the fades' steps by kind */
extern s32 func_002D2120(u8 *o);
extern void func_002D20A0(u8 *o);
extern void func_002CF6F0(u8 *fade);
extern void func_002CF3A0(u8 *fade, s32 kind);   /* jump a fade to its end */
extern void func_001771A0(Progress *p, s32 who);
void func_001FBE00(VObject *ev, s32 prio, void *step);
extern void func_002DE030(void *motion, s32 anim, s32 blend, s32 loop, f32 speed);
extern void func_0029F040(void *c, s32 slot);

extern void func_00177200(Progress *p, s32 slot);
extern void func_00218C20(void *c, u32 v);
extern void func_0029B190(void *c);
extern void func_0029AF20(void *c);
extern void func_0029AC50(void *c);
extern void func_0029A940(void *c);

/* event commands on a pursuer-type character (pc[1]; slots 2..5): 0x2A its +0x1660 = be32,
 * 0x2E be16 (0xFFFF: one step on in +0x1620, below +0x1621) to func_00218C20, 0x2F action
 * pc[2] (0, 2, 3), 0x30 another, 0x3E put into room be16 (0xFFFF: its own) at be16 how pc[6]
 * (at most 2), 0x31 its +0x31C with pc[2] != 0 */
void func_00200870(VObject *ev) {
    Progress *p = gProgress;
    u8 i = func_001770D0(p, PC(ev)[1]);
    u8 *c = i >= 2 && i < 6 ? (u8 *)gCharacters[i] : NULL;
    const u8 *pc;

    if (c == NULL) {
        return;
    }
    pc = PC(ev);
    switch (pc[0]) {
    case 0x2A:
        AT(c, 0x1660, s32) = be32(pc + 2);
        break;
    case 0x2E: {
        u32 v = be16(pc + 2) & 0xFFFF;

        if (v != 0xFFFF) {
            func_00218C20(c, v);
        } else if (AT(c, 0x1620, u8) + 1 < AT(c, 0x1621, u8)) {
            AT(c, 0x1620, u8) = AT(c, 0x1620, u8) + 1;
        }
        break;
    }
    case 0x2F:
        switch (pc[2]) {
        case 0:
            func_0029B190(c);
            break;
        case 2:
            func_0029AF20(c);
            break;
        case 3:
            func_0029AC50(c);
            break;
        }
        break;
    case 0x30:
        func_0029A940(c);
        break;
    case 0x3E: {
        s32 room;

        func_00177200(p, i);
        pc = PC(ev);
        room = be16(pc + 2) & 0xFFFF;
        if (room == 0xFFFF) {
            room = AT(c, 0x30, s32);
        }
        VCALL((VObject *)c, 0x64, void (*)(VObject *, s32, s32, s32))((VObject *)c, room, (s16)be16(pc + 4),
                                                                      pc[6] < 2 ? pc[6] : 2);
        break;
    }
    case 0x31:
        VCALL((VObject *)c, 0x31C, void (*)(VObject *, s32))((VObject *)c, pc[2] != 0);
        break;
    }
}

/* room effect slot pc[1] (32, D_0044E4C0 +0x1438) made anew from the effects' pool (+0x1400)
 * with constructor `ctor`, then set going (func_00266C70) at (3 x be32 / 1000; with `kind`
 * pc[14]) */
static void room_effect_new(VObject *ev, u8 *(*ctor)(u8 *), s32 kind) {
    u8 *fx = D_0044E4C0;
    const u8 *pc = PC(ev);
    struct {
        f32 pos[4];
        s32 a, kind;
    } arg __attribute__((aligned(16)));

    if (pc[1] < 0x20) {
        u8 **slot = (u8 **)(fx + 0x1438) + pc[1];
        void *mem;

        if (*slot != NULL) {
            VCALL(fx + 0x1400, 0x14, void (*)(void *, void *))(fx + 0x1400, *slot);
            *slot = NULL;
        }
        mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
        if (mem != NULL) {
            u8 *e = func_002672F0(0xA0, mem);

            if (e != NULL) {
                e = ctor(e);
            }
            *slot = e;
            VCALL(*slot, 0xC, void (*)(u8 *))(*slot);
        }
    }
    arg.pos[0] = (f32)be32(PC(ev) + 2) / 1000.0f;
    arg.pos[1] = (f32)be32(PC(ev) + 6) / 1000.0f;
    arg.pos[2] = (f32)be32(PC(ev) + 0xA) / 1000.0f;
    arg.pos[3] = 1.0f;
    arg.a = 0;
    if (kind) {
        arg.kind = PC(ev)[0xE];
    }
    func_00266C70(D_0044E4C0, PC(ev)[1], &arg);
}

extern VObject *D_00456DF8;   /* the room's placed objects (+0x18 by name) */
extern void func_0025F9D0(u8 *o, s32 anim);
extern void func_0025F810(u8 *o);

/* event command 0x50: the room's placed object named by the room handler (+0x34 of pc[2]):
 * pc[1] 0 shown (pc[3]), 1 / 2 animation pc[3] once / looped, 3 animation reset, 4 hidden and
 * stopped */
void func_001FFB70(VObject *ev) {
    VObject *room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);
    const char *name = VCALL(room, 0x34, const char *(*)(VObject *, s32))(room, PC(ev)[2]);
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    const u8 *pc = PC(ev);

#ifdef HG_NATIVE
    if (o == NULL) {   /* (the PS2 writes to low memory) */
        return;
    }
#endif
    switch (pc[1]) {
    case 0:
        AT(o, 0x0, u8) = pc[3] != 0;
        break;
    case 1:
        func_0025F9D0(o, pc[3]);
        AT(o, 0x1, u8) = 0;
        break;
    case 2:
        func_0025F9D0(o, pc[3]);
        AT(o, 0x1, u8) = 1;
        break;
    case 3:
        AT(o, 0x94, s32) = 0;
        AT(o, 0xA0, s32) = 0;
        AT(o, 0x98, s32) = 0;
        AT(o, 0x9C, s32) = 0;
        break;
    case 4:
        AT(o, 0x0, u8) = 0;
        func_0025F810(o);
        break;
    }
}

/* (0x25 / 0x2C) the script `id` (0x80..: built in, else the room's, vtable +0x24) */
static u8 *script_by_id(VObject *ev, u32 id) {
    if (id & 0x80) {
        return D_003D6760[id];
    } else {
        VObject *room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);

        return VCALL(room, 0x24, u8 *(*)(VObject *, s32))(room, id);
    }
}

void func_002029B0(VObject *ev) {
    Progress *p;
    const u8 *pc;

    EV_JUMPED(ev) = 0;
    p = gProgress;
    pc = PC(ev);
#ifdef HG_NATIVE
    {
        extern void hg_debug_evlog(const void *ev, const u8 *pc);

        hg_debug_evlog(ev, pc);
    }
#endif
    switch (pc[0]) {
    case 0x00:
        cmd_exit(ev, p, pc);
        break;
    case 0x01:
        VCALL(p, 0x14, void (*)(Progress *, s32))(p, pc[1]);
        break;
    case 0x3A:
        VCALL(p, 0x18, void (*)(Progress *, u32))(p, be16(pc + 1));
        break;
    case 0x03: {
        u32 msg = be16(pc + 1);

        D_0047B350 = (msg & 0x4000) ? 2 : 1;
        Task_Open((Task *)((u8 *)ev + 0x708), be16(PC(ev) + 1) & ~0x4000);
        AT(ev, 0x80C, s32) = *AT(ev, 0x6FC, s32 *);
        break;
    }
    case 0x05:
    case 0x92:
        cmd_action(ev, p, pc);
        break;
    case 0x49:
        cmd_action_end(ev, p, pc);
        break;
    case 0x09:   /* wait for the message window */
        if (AT(ev, 0x718, u8)) {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x0A:
        cmd_scene_change(ev, p, pc);
        break;
    case 0x0B: {   /* camera: +0xB4 / +0xB0 with a value in 1/1000 */
        f32 v = (f32)be32(pc + 2) / 1000.0f;

        if (pc[1] == 0) {
            VCALL(D_0044E4B8, 0xB4, void (*)(VObject *, f32))(D_0044E4B8, v);
        } else {
            VCALL(D_0044E4B8, 0xB0, void (*)(VObject *, f32))(D_0044E4B8, v);
        }
        break;
    }
    case 0x1D:   /* D_00456E00 +0xC (operand 1 = 1) or +0x10 with a byte and a word */
        if (pc[1] == 1) {
            VCALL(D_00456E00, 0xC, void (*)(VObject *, s32, s32))(D_00456E00, pc[2], be32(pc + 3));
        } else {
            VCALL(D_00456E00, 0x10, void (*)(VObject *, s32, s32))(D_00456E00, pc[2], be32(pc + 3));
        }
        break;
    case 0xA8:
        func_0016D480(p, AT(ev, 0x560, s32));
        break;
    case 0x36:
        func_001793A0(p, (u8)func_001770D0(p, pc[1]), (s8)PC(ev)[2], (s8)PC(ev)[3]);
        break;
    case 0x28: {   /* camera follows character pc[1] (0xFF: nobody) */
        u8 i = (u8)func_001770D0(p, pc[1]);

        if (pc[1] == 0xFF || i != 0xFF) {
            func_001792C0(p, i);
        }
        break;
    }
    case 0x14:
        if ((u8)func_001770D0(p, pc[1]) == 2) {
            func_002EC470((u8 *)p + 0x764, 0);
        } else {
            func_00177200(p, (u8)func_001770D0(p, PC(ev)[1]));
        }
        break;
    case 0x41:
        Progress_IncVar(p, pc[1]);
        break;
    case 0xA1:   /* rebuild the rooms' exits */
        VCALL(D_0044E568, 0x90, void (*)(VObject *))(D_0044E568);
        break;
    case 0x59:
        func_002003C0(ev);
        break;
    case 0x29: {   /* camera setup (pc[2], pc[3]) for the characters in this room inside area pc[1] */
        s32 i;

        for (i = 0; i < 6; i++) {
            u8 *c = (u8 *)gCharacters[i];
            u32 area;

            if (c == NULL || AT(ev, 0x560, s32) != AT(c, 0x30, s32)) {
                continue;
            }
            area = VCALL(D_0044E568, 0x48, u32 (*)(VObject *, s32, s32))(D_0044E568, AT(ev, 0x560, s32), PC(ev)[1]);
            if ((u8)VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, u32, s32))(ev, (f32 *)(c + 0x10), area & 0xFFFF,
                                                                          AT(c, 0x34, s32)) == 1) {
                AT(c, 0xE8, s32) = (s8)PC(ev)[2];
                AT(c, 0xEC, s32) = (s8)PC(ev)[3];
            }
        }
        break;
    }
    case 0x4C: {   /* a door's state, in the progress and on the door */
        s32 a = pc[1];

        VCALL(p, 0x68, void (*)(Progress *, s32, s32, s32))(p, pc[2], a, pc[3]);
        VCALL(D_0044E558, 0x84, void (*)(VObject *, s32, s32, s32))(D_0044E558, PC(ev)[2], a, PC(ev)[3]);
        break;
    }
    case 0x33:   /* an effect with a string argument */
        if (pc[2] != 0) {
            func_00266C70(D_0044E4C0, pc[1], (void *)(pc + 3));
        }
        break;
    case 0x22: {   /* the room handler's +0x28 with a string: bit 1 wait, bit 0 go on, else jumped */
        VObject *room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);
        u8 r = (u8)VCALL(room, 0x28, s32 (*)(VObject *, s32, u8 *, const u8 *))(
            room, pc[1], *AT(ev, 0x6FC, u8 **), pc);

        if (r & 2) {
            EV_WAIT(ev) = 1;
        } else if (!(r & 1)) {
            EV_JUMPED(ev) = 1;
        }
        break;
    }
    case 0x8D: {   /* zone pc[1]: an id and a rectangle (x0, z0, x1, z1) in 1/1000 */
        s32 k;

        AT(ev, 0x894 + pc[1] * 0x14, s32) = pc[2];
        for (k = 0; k < 4; k++) {
            AT(ev, 0x898 + PC(ev)[1] * 0x14 + k * 4, f32) = (f32)be32(PC(ev) + 3 + k * 4) / 1000.0f;
        }
        break;
    }
    case 0x27: {   /* each character whose relation to area pc[1] is pc[4]: +0xE8 / +0xEC = pc[2] / pc[3] */
        s32 i;

        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL) {
                const u8 *q = PC(ev);

                if ((s8)func_001FC390(ev, gCharacters[i], q[1]) == (s8)q[4]) {
                    AT(gCharacters[i], 0xE8, s32) = (s8)q[2];
                    AT(gCharacters[i], 0xEC, s32) = (s8)PC(ev)[3];
                }
            }
        }
        break;
    }
    case 0x11:   /* bring in character pc[1] as the partner (slot 2) */
        if ((u8)func_00171160(p, pc[1]) == 1) {
            func_00177350(p, 2);
            func_001772B0(p, 2);
            func_002ECB50((u8 *)p + 0x764);
        }
        break;
    case 0x12:   /* bring in character pc[1] in slot pc[2], placed at exit pc[3] of this room (0xFF: as is) */
        if ((u8)func_0016D670(p, pc[1], pc[2]) == 1) {
            func_00177350(p, PC(ev)[2]);
            if (PC(ev)[3] == 0xFF) {
                func_001772B0(p, PC(ev)[2]);
            } else {
                s32 room = VCALL(D_0044E568, 0x18, s32 (*)(VObject *, s32, u32))(D_0044E568, AT(ev, 0x560, s32),
                                                                                PC(ev)[3]);

                func_0029EF80(gCharacters[PC(ev)[2]], room);
            }
        }
        break;
    case 0x13: {   /* take out the character in slot pc[1] (wait while it can't go) */
        u8 *c = gCharacters[pc[1]];

        if (c != NULL && AT(c, 0xD0, u8) == 0 && AT(c, 0xD1, u8) == 0) {
            if ((u8)func_00177260(p, pc[1])) {
                EV_WAIT(ev) = 1;
            } else {
                func_00177300(p, PC(ev)[1]);
                if (PC(ev)[1] == 2) {
                    AT(p, 0x875, u8) = 0;
                }
            }
        }
        break;
    }
    case 0xB9:   /* bring in character pc[1] in slot pc[2] (second kind) */
        if ((u8)func_0016D6D0(p, pc[1], pc[2]) == 1) {
            func_00177350(p, PC(ev)[2]);
        }
        break;
    case 0x3C: {   /* the progress' +0x24 for character pc[1]: value be16 pc[2..3], pc[4] */
        u8 who = (u8)func_001770D0(p, pc[1]);

        VCALL(p, 0x24, void (*)(Progress *, u32, u32, u32))(p, (u16)be16(PC(ev) + 2), who, PC(ev)[4]);
        break;
    }
    case 0x6A: case 0x6B: case 0x6C:
        func_001FFC70(ev);
        break;
    case 0x57:   /* the event's +0xF8 with pc[1] */
        VCALL(ev, 0xF8, void (*)(VObject *, s32))(ev, pc[1]);
        break;
    case 0x7E:   /* a light / sound source pc[1] on (pc[6]) at be32 pc[2..5] / 1000; pc[6] 0xFF: stop the
                  * running one (D_0044E980) */
        if (pc[6] == 0xFF) {
            u8 *o = D_0044E980;

            if (o != NULL && func_002D2120(o)) {
                func_002D20A0(o);
            }
        } else {
            VCALL(D_0044E970, 0x8, void (*)(VObject *, s32, s32, s32, f32))(D_0044E970, pc[1], pc[6] != 0, 0,
                                                                          (f32)be32(pc + 2) / 1000.0f);
        }
        break;
    case 0x5C: {   /* a screen fade (+0x20): pc[1] frames, kind pc[2] & 0xF (1 in, 4 out); bits 0xC0 the
                    * music / sound fade with it (+0x38), bits 0x30 the volume ramp (Progress +0x1120) */
        u8 *fade = (u8 *)ev + 0x20;
        VObject *snd;
        u8 kind;

        func_002CF6F0(fade);
        AT(ev, 0x11F0, u8) = PC(ev)[1];
        AT(ev, 0x11F1, u8) = PC(ev)[2] & 0xF;
        AT(ev, 0x11F2, u8) = 1;
        AT(fade, 0x18, f32) = (f32)(0x80 / AT(ev, 0x11F0, u8));
        func_001FBE00(ev, 0xFA, D_003D6A40[AT(ev, 0x11F1, u8)]);
        kind = AT(ev, 0x11F1, u8);
        snd = D_00456DF0;
        switch (PC(ev)[2] & 0xC0) {
        case 0x80:
            if (snd != NULL) {
                if (kind == 4) {
                    VCALL(snd, 0x38, void (*)(VObject *, s32, s32))(snd, 1, 0);
                } else if (kind == 1) {
                    VCALL(snd, 0x38, void (*)(VObject *, s32, s32))(snd, 1, 0xFF);
                }
            }
            break;
        case 0x40:
            if (snd != NULL) {
                if (kind == 4) {
                    VCALL(snd, 0x38, void (*)(VObject *, s32, s32))(snd, 0x5A, 0);
                } else if (kind == 1) {
                    VCALL(snd, 0x38, void (*)(VObject *, s32, s32))(snd, 0x5A, 0xFF);
                }
            }
            break;
        }
        kind = AT(ev, 0x11F1, u8);
        switch (PC(ev)[2] & 0x30) {
        case 0x20:
            if (D_00456DF0 != NULL) {
                if (kind == 4) {
                    AT(p, 0x1120, f32) = -1.0f;
                } else if (kind == 1) {
                    AT(p, 0x1120, f32) = 1.0f;
                }
            }
            break;
        case 0x10:
            if (D_00456DF0 != NULL) {
                if (kind == 4) {
                    AT(p, 0x1120, f32) = -1.0f * (1.0f / (f32)AT(ev, 0x11F0, u8));
                } else if (kind == 1) {
                    AT(p, 0x1120, f32) = 1.0f / (f32)AT(ev, 0x11F0, u8);
                }
            }
            break;
        }
        break;
    }
    case 0x5D:   /* finish the fade now (if one runs) */
        if (AT(ev, 0x11F2, u8) != 0) {
            func_002CF3A0((u8 *)ev + 0x20, AT(ev, 0x11F1, u8));
        }
        AT(ev, 0x11F3, u8) = 1;
        break;
    case 0x5E:   /* the fade is over */
        AT(ev, 0x11F2, u8) = 0;
        break;
    case 0x5F:   /* wait for the fade */
        if (AT(ev, 0x11F2, u8) != 0) {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x15:   /* the progress' character pc[1] (+ func_001771A0) */
        func_001771A0(p, (u8)func_001770D0(p, pc[1]));
        break;
    case 0x23:   /* mark the loop point (just after this) in the script (+0x6FC: +0x8, its +0x11 = +0x8) */
        AT(AT(ev, 0x6FC, u8 *), 0x8, u8 *) = (u8 *)pc + 1;
        AT(AT(ev, 0x6FC, u8 *), 0x11, u8) = AT(ev, 0x8, u8);
        break;
    case 0x24:   /* back to the loop point */
        PC(ev) = AT(AT(ev, 0x6FC, u8 *), 0x8, u8 *);
        AT(ev, 0x8, u8) = AT(AT(ev, 0x6FC, u8 *), 0x11, u8);
        EV_JUMPED(ev) = 1;
        break;
    case 0x9D: {   /* character pc[1] plays animation be16 pc[2..3] (blend pc[4], speed pc[5]), held */
        u8 *c = gCharacters[(u8)func_001770D0(p, pc[1])];

#ifdef HG_NATIVE
        if ((u8)func_001770D0(p, pc[1]) >= 6 || c == NULL) {
            break;   /* (a character the PC build doesn't load yet) */
        }
#endif
        AT(c, 0xE0, u8) = 1;
        AT(c, 0xE2, u8) = 1;
        AT(c, 0xE3, u8) = 1;
        AT(c, 0x29, u8) = 0;
        func_002DE030(AT(c, 0xF0, void *), (u16)be16(PC(ev) + 2), PC(ev)[4], -1, (f32)PC(ev)[5]);
        break;
    }
    case 0x9E: {   /* wait for character pc[1]'s animation to come round (track flag 0x20) */
        u8 *c = gCharacters[(u8)func_001770D0(p, pc[1])];

#ifdef HG_NATIVE
        if ((u8)func_001770D0(p, pc[1]) >= 6 || c == NULL) {
            EV_WAIT(ev) = 1;   /* (not loaded in the PC build yet: its script idles) */
            break;
        }
#endif
        if ((AT(AT(AT(c, 0xF0, u8 *), 0x6A4, u8 *), 0x18, u32) & 0x20) == 0) {
            EV_WAIT(ev) = 1;
        }
        break;
    }
    case 0xBA:   /* hand the character in slot pc[1] to slot pc[2] (wait while that is taken) */
        if (gCharacters[pc[1]] != NULL) {
            if ((u8)func_00177260(p, pc[2])) {
                EV_WAIT(ev) = 1;
            } else {
                func_0029F040(gCharacters[PC(ev)[1]], PC(ev)[2]);
            }
        }
        break;
    case 0x7C:   /* zone pc[1] (32): on, kind pc[18]; centre (3 x be32 / 1000), radius, height */
        if (pc[1] < 0x20) {
            u8 *z = (u8 *)ev + pc[1] * 0x30;

            AT(z, 0xBF4, u8) = 1;
            AT(z, 0xBF5, u8) = pc[0x12];
            AT(z, 0xC00, f32) = (f32)be32(pc + 2) / 1000.0f;
            AT(z, 0xC04, f32) = (f32)be32(pc + 6) / 1000.0f;
            AT(z, 0xC08, f32) = (f32)be32(pc + 0xA) / 1000.0f;
            AT(z, 0xC0C, f32) = 1.0f;
            AT(z, 0xC10, f32) = (f32)(u16)be16(pc + 0xE);
            AT(z, 0xC14, f32) = (f32)(s16)be16(pc + 0x10);
        }
        break;
    case 0x58:
        VCALL(ev, 0xFC, void (*)(VObject *, s32))(ev, pc[1]);
        break;
    case 0x90:   /* light pc[2]: 0 back to the room's own; 1 / 2 its value 7 / 11 scaled by be32 /
                  * 1000 (others: set as it is) */
        if (pc[1] == 0) {
            VCALL(D_0044E4C8, 0x24, void (*)(VObject *, s32))(D_0044E4C8, pc[2]);
        } else {
            VObject *lights = D_0044E4C8;
            f32 got[12] __attribute__((aligned(16)));
            f32 l[12] __attribute__((aligned(16)));
            s32 k;

            /* +0x1C returns the light by value: its buffer first, then the object */
            VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(got, lights, pc[2]);
            for (k = 0; k < 12; k++) {
                l[k] = got[k];
            }
            pc = PC(ev);
            if (pc[1] == 2) {
                l[11] = l[11] * ((f32)be32(pc + 3) / 1000.0f);
            } else if (pc[1] == 1) {
                l[7] = l[7] * ((f32)be32(pc + 3) / 1000.0f);
            }
            VCALL(lights, 0x20, void (*)(VObject *, f32 *, s32))(lights, l, PC(ev)[2]);
        }
        break;
    case 0x32:   /* sound channel pc[1]'s volume (+0x7C) */
        VCALL(D_0044E560, 0x7C, void (*)(VObject *, s32, s32))(D_0044E560, pc[1], be16(pc + 2) & 0xFFFF);
        break;
    case 0x86:   /* room effect pc[1] (32) made anew (a D_0046FF40 effect) at (3 x be32 / 1000),
                  * kind pc[14] */
        room_effect_new(ev, func_00208ED0, 1);
        break;
    case 0x7F:   /* the same with a D_0046FF00 effect, no kind */
        room_effect_new(ev, func_00208EF0, 0);
        break;
    case 0x26:   /* script variable pc[1] (+0x810) = be32 */
        AT((u8 *)ev + pc[1] * 4, 0x810, s32) = be32(pc + 2);
        break;
    case 0x50:
        func_001FFB70(ev);
        break;
    case 0x66: {   /* a lit doorway for the lights (+0x38): four corners (be32 / 1000), its facing
                    * from the first three, its middle */
        f32 q[6][4] __attribute__((aligned(16)));
        f32 a[4] __attribute__((aligned(16)));
        f32 b[4] __attribute__((aligned(16)));
        s32 i;

        for (i = 0; i < 4; i++) {
            q[i][0] = (f32)be32(PC(ev) + 1 + i * 12) / 1000.0f;
            q[i][1] = (f32)be32(PC(ev) + 5 + i * 12) / 1000.0f;
            q[i][2] = (f32)be32(PC(ev) + 9 + i * 12) / 1000.0f;
            q[i][3] = 1.0f;
        }
        sceVu0SubVector(a, q[1], q[0]);
        sceVu0SubVector(b, q[2], q[0]);
        sceVu0OuterProduct(q[4], a, b);
        sceVu0Normalize(q[4], q[4]);
        q[5][0] = q[3][0] + 0.5f * (q[0][0] - q[3][0]);
        q[5][1] = q[2][1] + 0.5f * (q[1][1] - q[2][1]);
        q[5][2] = q[3][2] + 0.5f * (q[0][2] - q[3][2]);
        q[5][3] = 1.0f;
        VCALL(D_0044E4C8, 0x38, void (*)(VObject *, f32 *))(D_0044E4C8, q[0]);
        break;
    }
    case 0x7D:   /* zone pc[1] (32) around room effect pc[2]: kind pc[7], radius, height */
        if (pc[1] < 0x20) {
            u8 *e = func_00266C40(D_0044E4C0, pc[2]);

            if (e != NULL) {
                u8 *z;

                pc = PC(ev);
                z = (u8 *)ev + pc[1] * 0x30;
                AT(z, 0xBF4, u8) = 1;
                AT(z, 0xBF5, u8) = pc[7];
                AT(z, 0xC00, f32) = AT(e, 0x20, f32);
                AT(z, 0xC04, f32) = AT(e, 0x24, f32);
                AT(z, 0xC08, f32) = AT(e, 0x28, f32);
                AT(z, 0xC0C, f32) = 1.0f;
                AT(z, 0xC10, f32) = (f32)(u16)be16(pc + 3);
                AT(z, 0xC14, f32) = (f32)(s16)be16(pc + 5);
            }
        }
        break;
    case 0x16:   /* the counter +0x703 = pc[1] */
        AT(ev, 0x703, u8) = pc[1];
        break;
    case 0x17:   /* counter +1 */
        AT(ev, 0x703, u8)++;
        break;
    case 0x18:   /* wait until the counter is pc[1] */
        if (AT(ev, 0x703, u8) != pc[1]) {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x25: {   /* go to script pc[1] (its loop and return points cleared) */
        u8 *to = script_by_id(ev, pc[1]);

        PC(ev) = to;
        AT(ev, 0x8, u8) = 0;
        AT(AT(ev, 0x6FC, u8 *), 0x8, s32) = 0;
        AT(AT(ev, 0x6FC, u8 *), 0x11, u8) = 0;
        AT(AT(ev, 0x6FC, u8 *), 0xC, s32) = 0;
        AT(AT(ev, 0x6FC, u8 *), 0x12, u8) = 0;
        AT(AT(ev, 0x6FC, u8 *), 0x14, u16) = 0;
        EV_JUMPED(ev) = 1;
        break;
    }
    case 0x2C: {   /* call script pc[1] (0x2D returns after this) */
        u8 *to;

        AT(AT(ev, 0x6FC, u8 *), 0xC, const u8 *) = pc + 2;
        AT(AT(ev, 0x6FC, u8 *), 0x12, u8) = AT(ev, 0x8, u8);
        to = script_by_id(ev, PC(ev)[1]);
        PC(ev) = to;
        AT(ev, 0x8, u8) = 0;
        EV_JUMPED(ev) = 1;
        break;
    }
    case 0x2D:   /* return from 0x2C */
        PC(ev) = AT(AT(ev, 0x6FC, u8 *), 0xC, u8 *);
        AT(ev, 0x8, u8) = AT(AT(ev, 0x6FC, u8 *), 0x12, u8);
        AT(AT(ev, 0x6FC, u8 *), 0xC, s32) = 0;
        AT(AT(ev, 0x6FC, u8 *), 0x12, u8) = 0;
        EV_JUMPED(ev) = 1;
        break;
    case 0x34: {   /* renderer +0x2C's +0x1C (on / off) */
        u8 *r = VCALL(D_0044E4F0, 0x2C, u8 *(*)(VObject *))(D_0044E4F0);

        AT(r, 0x1C, u8) = PC(ev)[1] != 0;
        break;
    }
    case 0x37:   /* script variable pc[1] +1 */
        AT(ev, 0x810 + pc[1] * 4, s32)++;
        break;
    case 0x38:   /* script variable pc[1] -1 */
        AT(ev, 0x810 + pc[1] * 4, s32)--;
        break;
    case 0x43: {   /* the progress' +0x7B8 level to pc[1] (0..100) */
        f32 v = (f32)(u32)pc[1];

        if (v < 0.0f) {
            v = 0.0f;
        }
        if (!(v <= 100.0f)) {
            v = 100.0f;
        }
        func_002EF9E0((u8 *)p + 0x7B8, v);
        break;
    }
    case 0x44:
        VCALL(p, 0x6C, void (*)(Progress *))(p);
        break;
    case 0x46:
        VCALL(D_0044E558, 0x7C, void (*)(VObject *))(D_0044E558);
        break;
    case 0x4A:
        VCALL(D_0044E4F8, 0x18, void (*)(VObject *))(D_0044E4F8);
        break;
    case 0x4E:   /* Fiona's +0x1AD5F4 / +0x1AD5F8 cleared */
        AT(gCharPlayer, 0x1AD5F8, s32) = 0;
        AT(gCharPlayer, 0x1AD5F4, s32) = 0;
        break;
    case 0x4F:
        func_00182E80(gCharPlayer);
        break;
    case 0x51:   /* this script's +0xAC with be16 pc[1..2] */
        VCALL(ev, 0xAC, void (*)(VObject *, u32))(ev, be16(pc + 1));
        AT(ev, 0x80C, s32) = 0;
        break;
    case 0x52:   /* the movie's +0x1C4 set */
        if (D_0044E958 != NULL) {
            AT(D_0044E958, 0x1C4, u8) = 1;
        }
        break;
    case 0x53:   /* obstacle pc[1]: +0x10 (pc[2], pc[3], be16 pc[4], be16 pc[6]) */
        VCALL(D_0044FE08, 0x10, void (*)(VObject *, s32, s32, s32, u32, u32))(
            D_0044FE08, pc[1], pc[2], pc[3], be16(pc + 4), be16(pc + 6));
        break;
    case 0x54:
        VCALL(D_0044FE08, 0x38, void (*)(VObject *, s32))(D_0044FE08, pc[1]);
        break;
    case 0x55:   /* this script's +0xB0 with be32 pc[1..4], pc[5] */
        VCALL(ev, 0xB0, void (*)(VObject *, s32, s32))(ev, be32(pc + 1), pc[5]);
        break;
    case 0x5A:   /* item be16 pc[1..2] in (pc[3]) / out */
        if (pc[3] != 0) {
            func_002EC3C0((u8 *)p + 0x764, be16(pc + 1));
        } else {
            func_002EC450((u8 *)p + 0x764, be16(pc + 1));
        }
        break;
    case 0x5B:   /* the stalker's item (+0x2D4) out */
        if (gCharPursuer != NULL) {
            func_002EC450((u8 *)p + 0x764, VCALL(D_0044F808, 0x2D4, u32 (*)(VObject *))(D_0044F808));
        }
        break;
    case 0x64:   /* room effect 0x1C gone */
        func_002670F0(D_0044E4C0, 0x1C);
        break;
    case 0x69:   /* sound driver +0x18 (be32 pc[1..4], pc[5]) */
        VCALL(D_0044E560, 0x18, void (*)(VObject *, s32, s32))(D_0044E560, be32(pc + 1), pc[5]);
        break;
    case 0x6D:   /* the item manager's +0x4 = pc[1]; progress flag 4 */
        AT(D_0044E988, 0x4, u8) = pc[1];
        Progress_SetFlag(p, 4);
        break;
    case 0x72:
        VCALL(D_0044FE08, 0x14, void (*)(VObject *, s32, s32, s32))(D_0044FE08, pc[1], pc[2], pc[3]);
        break;
    case 0x73:
        VCALL(D_0044FE08, 0x40, void (*)(VObject *, s32, u32, u32))(
            D_0044FE08, pc[1], be16(pc + 2), be16(pc + 4));
        break;
    case 0x74:
        VCALL(D_0044FE08, 0x44, void (*)(VObject *, s32))(D_0044FE08, pc[1]);
        break;
    case 0x75:
        VCALL(D_0044FE08, 0x48, void (*)(VObject *, s32))(D_0044FE08, pc[1]);
        break;
    case 0x76:
        VCALL(D_0044FE08, 0x4C, void (*)(VObject *, s32, s32))(D_0044FE08, pc[1], pc[2]);
        break;
    case 0x84:
        func_002EC470((u8 *)p + 0x764, pc[1]);
        break;
    case 0x8B:
        AT(p, 0x73EB00, u8) = pc[1];
        break;
    case 0x91:   /* the noise level setting */
        AT(p, 0x1114, u8) = pc[1];
        break;
    case 0x94:
        func_0019A280(gCharPlayer, pc[1]);
        break;
    case 0x95:
        func_0019A210(gCharPlayer, pc[1]);
        break;
    case 0x02: case 0x04: case 0x1F: case 0x3B: case 0x3D: case 0x45: case 0x47: case 0x48:
    case 0x67: case 0x79: case 0x7B: case 0x87: case 0x8F: case 0xAE: case 0xB3: case 0xB5:
        func_002013F0(ev);
        break;
    case 0x39: case 0x3F: case 0x63: case 0x77: case 0x78: case 0x7A: case 0x85: case 0xAF:
    case 0xB0: case 0xBB: case 0xBD: case 0xC3: case 0xC4: case 0xC5: case 0xC6: case 0xD0:
        func_00200B00(ev);
        break;
    case 0x2A: case 0x2E: case 0x2F: case 0x30: case 0x31: case 0x3E:
        func_00200870(ev);
        break;
    case 0x60: case 0x61: case 0x62: case 0x6E: case 0x89:
        func_001FFE00(ev);
        break;
    case 0x06: case 0x07: case 0x08: case 0x0C: case 0x0D: case 0x0E: case 0x0F: case 0x10:
    case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1E: case 0x20: case 0x21: case 0x2B:
    case 0x42: case 0x4B: case 0x56: case 0x6F: case 0x70: case 0x81: case 0x8E: case 0x9A:
    case 0xA4: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xC7:
        break;
    default:
        if (pc[0] < 0xDB) {
#ifdef HG_NATIVE
            extern void hg_debug_todo_opcode(s32 op);

            hg_debug_todo_opcode(pc[0]);
#endif
        }
        break;
    }
    if (!EV_WAIT(ev) && !EV_JUMPED(ev)) {
        VCALL(ev, 0xC, void (*)(VObject *))(ev);
    }
}

/* the commands' lengths (0: 3 + the byte at +2: commands 0x22 and 0x33 carry a string) */
static const u8 sCmdLength[0xDB] = {
    2, 2, 4, 3, 3, 4, 1, 3, 1, 1, 4, 6, 16, 1, 6, 1,   /* 00 */
    3, 2, 4, 2, 2, 2, 2, 1, 2, 3, 1, 1, 1, 7, 2, 3,   /* 10 */
    2, 2, 0, 1, 1, 2, 6, 5, 2, 4, 6, 2, 2, 1, 4, 3,   /* 20 */
    2, 3, 4, 0, 2, 9, 4, 2, 2, 9, 3, 6, 5, 3, 7, 6,   /* 30 */
    3, 2, 2, 2, 1, 7, 1, 6, 2, 2, 1, 3, 4, 5, 1, 1,   /* 40 */
    4, 3, 1, 8, 2, 6, 3, 2, 2, 4, 4, 1, 3, 1, 1, 1,   /* 50 */
    3, 2, 2, 3, 1, 17, 49, 2, 20, 6, 4, 2, 1, 2, 3, 2,   /* 60 */
    2, 5, 4, 6, 2, 2, 3, 5, 1, 14, 17, 3, 19, 8, 7, 14,   /* 70 */
    2, 5, 18, 4, 2, 3, 15, 3, 4, 3, 15, 2, 23, 19, 9, 3,   /* 80 */
    7, 2, 4, 2, 2, 2, 6, 2, 2, 3, 3, 7, 18, 6, 2, 19,   /* 90 */
    20, 1, 3, 2, 5, 3, 3, 2, 1, 35, 17, 13, 2, 2, 7, 13,   /* A0 */
    3, 5, 3, 16, 2, 3, 1, 2, 6, 3, 3, 2, 1, 1, 3, 5,   /* B0 */
    2, 2, 5, 5, 5, 6, 6, 3, 18, 2, 5, 2, 4, 2, 1, 5,   /* C0 */
    3, 2, 1, 5, 5, 3, 1, 3, 1, 17, 8,   /* D0 */
};

/* the event's +0xC: step the pc over the current command */
void func_001FF9E0(VObject *ev) {
    u8 *pc = PC(ev);

    if (pc[0] >= 0xDB) {
#ifdef HG_NATIVE
        if (getenv("HG_EVSTUCK")) {
            fprintf(stderr, "event skip stuck at %p: %02X %02X %02X %02X %02X %02X\n", (void *)pc, pc[0], pc[1], pc[2],
                    pc[3], pc[4], pc[5]);
            {
                s32 k;

                for (k = -160; k < 0; k++) {
                    fprintf(stderr, "%02X%s", pc[k], (k & 15) == 15 ? "\n" : " ");
                }
            }
            exit(4);
        }
#endif
        return;
    }
    if (sCmdLength[pc[0]] == 0) {
        PC(ev) = pc + pc[2] + 3;
    } else {
        PC(ev) = PC(ev) + sCmdLength[pc[0]];
    }
}


/* pi as the original's constant (ee-gcc rounds the literal 3.1415927f down) */
static const union {
    u32 u;
    f32 f;
} sPi = {0x40490FDB};

#define DEG(v) (sPi.f * (f32)(s16)(v) / 180.0f)

/* place character c on nav triangle `tri` (vt+0x28; angle / position optional) in the event's
 * room, keeping its byte +0x2D */
static void char_place(VObject *ev, u8 *c, s32 tri, f32 *angle, f32 *pos) {
    u8 keep = AT(c, 0x2D, u8);

    VCALL(c, 0x28, void (*)(u8 *, s32, f32 *, f32 *))(c, tri, angle, pos);
    AT(c, 0x2D, u8) = keep;
    AT(c, 0x30, s32) = AT(ev, 0x560, s32);
}

/* commands on a character (operand 1: its id, 0xFF: the script's own) */
void func_002013F0(VObject *ev) {
    const u8 *pc = PC(ev);
    u8 *c;
    f32 angle;
    f32 pos[4] __attribute__((aligned(16)));

    if (pc[1] == 0xFF) {
        c = *AT(ev, 0x6FC, u8 **);
    } else {
        c = char_by_id(gProgress, pc[1]);
    }
    if (c == NULL) {
        return;
    }
    pc = PC(ev);
    switch (pc[0]) {
    case 0x02:   /* onto a triangle */
        char_place(ev, c, be16(pc + 2), NULL, NULL);
        break;
    case 0x04: {   /* to a room point */
        s32 tri = VCALL(D_0044E568, 0x2C, s32 (*)(VObject *, s32, f32 *))(D_0044E568, pc[2], pos);

        char_place(ev, c, tri, NULL, pos);
        break;
    }
    case 0x3B:   /* onto a triangle, facing */
        angle = DEG(be16(pc + 4));
        char_place(ev, c, be16(PC(ev) + 2), &angle, NULL);
        break;
    case 0x3D:
        AT(c, 0x2C, u8) = pc[2] != 0;
        break;
    case 0x45:
        func_00122C20(c, be32(pc + 2), pc[6], 0, 0, 0);
        break;
    case 0x47:
        AT(c, 0xC4, s32) = be32(pc + 2);
        break;
    case 0x48:
        AT(c, 0x14C8, s32) = AT(c, 0x14CC, s32);
        break;
    case 0x1F: {   /* visible flag, if in the current room */
        s32 room = AT(c, 0x30, s32);

        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            AT(c, 0x29, u8) = PC(ev)[2] != 0;
        }
        break;
    }
    case 0x67:   /* find its triangle */
        AT(c, 0x34, s32) = VCALL(D_0044E570, 0x3C, s32 (*)(VObject *, f32 *, s32))(D_0044E570, (f32 *)(c + 0x10), 0);
        break;
    case 0x79: {   /* to (x, z) on a triangle, facing */
        pos[0] = (f32)be32(pc + 4) / 1000.0f;
        pos[2] = (f32)be32(PC(ev) + 8) / 1000.0f;
        pos[3] = 1.0f;
        VCALL(D_0044E570, 0x14, void (*)(VObject *, s32, f32 *))(D_0044E570, be16(PC(ev) + 2), pos);
        angle = DEG(be16(PC(ev) + 0xC));
        char_place(ev, c, be16(PC(ev) + 2), &angle, pos);
        break;
    }
    case 0xB3: {   /* to (x, y, z), facing */
        f32 *at = (f32 *)(c + 0x10);
        s32 tri;

        at[0] = (f32)be32(pc + 2) / 1000.0f;
        at[2] = (f32)be32(PC(ev) + 0xA) / 1000.0f;
        at[3] = 1.0f;
        angle = DEG(be16(PC(ev) + 0xE));
        AT(c, 0x34, s32) = VCALL(D_0044E570, 0x3C, s32 (*)(VObject *, f32 *, s32))(D_0044E570, at, 0);
        tri = AT(c, 0x34, s32);
        {
            u8 keep = AT(c, 0x2D, u8);

            VCALL(c, 0x28, void (*)(u8 *, s32, f32 *, f32 *))(c, tri, &angle, at);
            AT(c, 0x2D, u8) = keep;
        }
        at[1] = (f32)be32(PC(ev) + 6) / 1000.0f;
        AT(c, 0x30, s32) = AT(ev, 0x560, s32);
        AT(c, 0x54, f32) = angle;
        sceVu0UnitMatrix((f32 (*)[4])(c + 0x60));
        sceVu0RotMatrixY((f32 (*)[4])(c + 0x60), (f32 (*)[4])(c + 0x60), angle);
        break;
    }
    case 0x7B:   /* wait for its model's flags */
        if ((pc[2] & (u8)func_001F4770(AT(c, 0xF0, u8 *), 0, 0, 1)) == 0) {
            AT(ev, 0x700, u8) = 1;
        }
        break;
    case 0x8F:
        AT(AT(c, 0xF0, u8 *), 0x4D9, u8) = pc[2] != 0;
        break;
    case 0xAE:
        if (pc[6] == 0) {
            AT(c, 0xE4, u8) = 1;
        } else {
            AT(c, 0xE4, u8) = 0;
            VCALL(D_0044E4F0, 0x70, void (*)(VObject *, s32))(D_0044E4F0, be32(PC(ev) + 2));
            func_001267F0(c, 0xF);
        }
        break;
    case 0xB5:
        func_001267F0(c, pc[2]);
        break;
    case 0x87: {   /* an effect at it: 1 if it hasn't moved (from +0x40), else 2 */
        f32 a[4] __attribute__((aligned(16)));
        f32 b[4] __attribute__((aligned(16)));
        struct {
            u32 pad[4];
            s32 moving;
        } arg;
        f32 dx, dy, dz;

        sceVu0CopyVector(a, (f32 *)(c + 0x10));
        sceVu0CopyVector(b, (f32 *)(c + 0x40));
        dy = a[1] - b[1];
        dx = a[0] - b[0];
        dz = a[2] - b[2];
        arg.moving = dy * dy + dx * dx + dz * dz < 0.5f ? 1 : 2;
        func_00266C70(D_0044E4C0, PC(ev)[2], &arg);
        break;
    }
    }
}

extern VObject *D_0044E560;   /* the sound driver */
extern u8 *D_0044E978;        /* resident data */
extern void func_00178450(Progress *p, u32 n);
extern void func_00178500(Progress *p, u32 n);
extern void func_00178630(Progress *p, u32 n);
extern void func_00178A60(Progress *p, u32 n);
extern void func_00178A30(Progress *p, u32 n);
extern void func_00260BB0(void *o, u32 n);

static inline void flag_set(u8 *words, s32 n) {
    AT(words, (n >> 5) * 4, u32) |= 1 << (n & 0x1F);
}

static inline void flag_clear(u8 *words, s32 n) {
    AT(words, (n >> 5) * 4, u32) &= ~(1 << (n & 0x1F));
}

/* 0x59: flag and counter commands (sub-op pc[1], operand pc[2..3]) */
void func_002003C0(VObject *ev) {
    Progress *p = gProgress;
    const u8 *pc = PC(ev);
    u32 n;

    if (pc[1] >= 0x13) {
        return;
    }
    n = be16(pc + 2);
    switch (pc[1]) {
    case 0x00:
        flag_set((u8 *)p + 0x1C, n);
        break;
    case 0x01:
        flag_clear((u8 *)p + 0x1C, n);
        break;
    case 0x02:
        Progress_SetFlag(p, n);
        break;
    case 0x03:
        Progress_ClearFlag(p, n);
        break;
    case 0x08:
        VCALL(D_0044E568, 0x64, void (*)(VObject *, u32))(D_0044E568, n);
        /* fallthrough */
    case 0x04:
        func_00178450(p, be16(PC(ev) + 2));
        break;
    case 0x07:
        VCALL(D_0044E568, 0x68, void (*)(VObject *, u32))(D_0044E568, n);
        /* fallthrough */
    case 0x05:
        func_00178500(p, be16(PC(ev) + 2));
        break;
    case 0x06:
        func_00178630(p, n);
        break;
    case 0x09:
        func_00178A60(p, n);
        break;
    case 0x0A:
        func_00178A30(p, n);
        break;
    case 0x0B:   /* a message parameter */
        Msg_SetParamSystem((u8 *)ev + 0x708, 0,
                           VCALL(ev, 0xD0, u32 (*)(VObject *, u32))(ev, n) & 0xFFFF);
        break;
    case 0x0C:
        func_00260BB0((u8 *)D_0044E988 + 8, n);
        break;
    case 0x0D:   /* give / take an item, with the pickup sound */
        if (AT(p, 0x30, u32) & 0x8000) {
            if ((n & 0x8000) && VCALL(D_0044E988, 0xC, s32 (*)(VObject *, u32))(D_0044E988, n & 0x7FFF)) {
                VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0xC, 5);
            }
        } else {
            if (!(n & 0x8000) && VCALL(D_0044E988, 0xC, s32 (*)(VObject *, u32))(D_0044E988, n)) {
                VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0xC, 5);
            }
        }
        break;
    case 0x0E:
        VCALL(D_0044E988, 0xC, s32 (*)(VObject *, u32))(D_0044E988, n);
        break;
    case 0x0F:   /* the flag in event variable n */
        flag_set((u8 *)p + 0x1C, AT(ev, 0x810 + n * 4, s32));
        break;
    case 0x10:
        flag_clear((u8 *)p + 0x1C, AT(ev, 0x810 + n * 4, s32));
        break;
    case 0x11:
        VCALL(D_0044E988, 0x38, void (*)(VObject *, u32))(D_0044E988, n);
        break;
    case 0x12:   /* a resident flag */
        flag_set(D_0044E978 + 0x24, n);
        break;
    }
}

/* ---- character scripts (a character's own script, its context at event +0x6FC): movement and
 * action commands; any other command runs as a normal one ---- */

extern f32 func_002E2D00(f32 angle);   /* wrap an angle into -pi..pi */
extern f32 func_0031C5C0(f32 x, f32 z);   /* heading of (x, z) */
extern void func_0010E5F0(f32 *out, const f32 *v);   /* libvu0: copy x, y, z */

/* the character's next action: state +0xF4 (its parameters +0x100.. set before), not done */
#define CHAR_ACT(c, state) (AT(c, 0xE1, u8) = 0, AT(c, 0xF4, s32) = (state))

static inline u32 opt16(u32 v) {
    return v == 0xFFFF ? (u32)-1 : v;
}

void func_00201B90(VObject *ev) {
    Progress *p = gProgress;
    u8 *ctx = AT(ev, 0x6FC, u8 *);
    u8 *c = AT(ctx, 0x0, u8 *);
    const u8 *pc;
    f32 pos[4] __attribute__((aligned(16)));
    f32 tmp[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));

    EV_JUMPED(ev) = 0;
    pc = PC(ev);
    switch (pc[0]) {
    case 0x06:   /* wait until it is idle; a character id -1 ends this character's script */
        if (c == (u8 *)-1 || (c != NULL && AT(c, 0xE0, u8))) {
            if (c != (u8 *)-1) {
                AT(c, 0xE1, u8) = 0;
                AT(c, 0xF4, s32) = 1;
            }
            if (AT(ev, 0x80C, u8 *) == c) {
                AT(ev, 0x80C, s32) = 0;
            }
            AT(AT(ev, 0x6FC, u8 *), 0x0, s32) = 0;
        }
        EV_WAIT(ev) = 1;
        break;
    case 0x07:   /* walk to triangle */
        AT(c, 0x104, u32) = be16(pc + 1);
        CHAR_ACT(c, 7);
        break;
    case 0x81:
        AT(c, 0x104, u32) = be16(pc + 1);
        AT(c, 0x108, u32) = be16(PC(ev) + 3);
        CHAR_ACT(c, 8);
        break;
    case 0x08:   /* wait for the action to finish (its motion flag 0x20) */
        if (AT(c, 0xF4, s32) == 0 &&
            (AT(AT(AT(c, 0xF0, u8 *), 0x6A4, u8 *), 0x18, u32) & 0x20) != 0) {
            AT(c, 0xE1, u8) = 1;
        } else {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x0C: {   /* go to (x, z) on a triangle, then face */
        static const union { u32 u; f32 f; } kPi = {0x40490FDB};
        f32 a;
        u32 t;

        pos[0] = (f32)be32(pc + 3) / 1000.0f;
        pos[1] = 0.0f;
        pos[2] = (f32)be32(PC(ev) + 7) / 1000.0f;
        pos[3] = 1.0f;
        a = func_002E2D00(kPi.f * (f32)(s16)be16(PC(ev) + 0xB) / 180.0f);
        AT(c, 0x104, u32) = be16(PC(ev) + 1);
        t = be16(PC(ev) + 0xD);
        AT(c, 0x108, u32) = opt16(t);
        sceVu0CopyVector((f32 *)(c + 0x110), pos);
        AT(c, 0x10C, f32) = a;
        CHAR_ACT(c, PC(ev)[0xF]);
        break;
    }
    case 0x0E:
        AT(c, 0x104, u32) = be16(pc + 1);
        AT(c, 0x108, u32) = opt16(be16(PC(ev) + 3));
        CHAR_ACT(c, PC(ev)[5]);
        break;
    case 0x10:
        AT(c, 0x100, u32) = pc[1];
        CHAR_ACT(c, PC(ev)[2]);
        break;
    case 0x0D:   /* wait until done */
        if (AT(c, 0xE1, u8) != 1) {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x0F:
        CHAR_ACT(c, 2);
        break;
    case 0x19:   /* wait for the context counter */
        if (AT(AT(ev, 0x6FC, u8 *), 0x14, u16) != be16(pc + 1)) {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x1A:
        AT(AT(ev, 0x6FC, u8 *), 0x14, u16) = 0;
        break;
    case 0x1B:   /* yield a frame */
        PC(ev) = PC(ev) + 1;
        EV_WAIT(ev) = 1;
        break;
    case 0x1C:
        if (AT(AT(ev, 0x6FC, u8 *), 0x14, u16) != 0x10) {
            EV_WAIT(ev) = 1;
        }
        break;
    case 0x1E: {   /* to a room point, facing its direction point */
        VObject *rooms = D_0044E568;
        s32 tri = VCALL(rooms, 0x30, s32 (*)(VObject *, s32, f32 *))(rooms, pc[1], pos);
        f32 a;

        if (tri == -1) {
            break;
        }
        AT(c, 0x30, s32) = AT(ev, 0x560, s32);
        AT(c, 0x34, s32) = tri;
        sceVu0CopyVector((f32 *)(c + 0x10), pos);
        VCALL(rooms, 0x34, void (*)(VObject *, s32, f32 *))(rooms, PC(ev)[1], tmp);
        sceVu0SubVector(dir, tmp, pos);
        a = func_0031C5C0(dir[0], dir[2]);
        AT(c, 0x54, f32) = a;
        sceVu0UnitMatrix((f32 (*)[4])(c + 0x60));
        sceVu0RotMatrixY((f32 (*)[4])(c + 0x60), (f32 (*)[4])(c + 0x60), a);
        AT(c, 0x124, s32) = AT(c, 0x128, s32);
        break;
    }
    case 0x20:
        AT(c, 0x2B, u8) = pc[1] != 0;
        break;
    case 0x21:
        AT(c, 0x2D, u8) = pc[1] != 0;
        if (AT(c, 0x153C, u8) == 0 && PC(ev)[1] == 1 && AT(c, 0x14E8, s32) == 4) {
            /* her script state is replaced by a fresh one (the original copies a local whose
             * first word is 0, the rest left as it was on the stack) */
            AT(c, 0x14E8, s32) = 0;
            AT(c, 0x14EC, s32) = 0;
            AT(c, 0x14F0, s32) = 0;
            AT(c, 0x14F4, s32) = 0;
            AT(c, 0x14F8, s32) = 0;
            AT(c, 0x14FC, f32) = 0.0f;
            AT(c, 0x1500, s32) = 0;
            AT(c, 0x1504, u8) = 0;
            AT(c, 0x1505, u8) = 0;
            AT(c, 0x1506, u16) = 0;
        }
        break;
    case 0x2B:   /* follow character pc[1] (0xFF: none) */
        if (pc[1] == 0xFF) {
            AT(c, 0x100, s32) = 0xFF;
            CHAR_ACT(c, 0xC);
        } else {
            u8 i = (u8)func_001770D0(p, pc[1]);

            if (i < 6) {
                AT(c, 0x100, s32) = i;
                CHAR_ACT(c, 0xC);
            }
        }
        break;
    case 0xAB:   /* go to (x, y, z) */
        pos[0] = (f32)be32(pc + 1) / 1000.0f;
        pos[1] = (f32)be32(PC(ev) + 5) / 1000.0f;
        pos[3] = 1.0f;
        pos[2] = (f32)be32(PC(ev) + 9) / 1000.0f;
        sceVu0CopyVector((f32 *)(c + 0x110), pos);
        CHAR_ACT(c, 0xD);
        break;
    case 0x42: {   /* turn to character pc[1] */
        u8 i = (u8)func_001770D0(p, pc[1]);

        if (i < 6) {
            AT(c, 0x100, s32) = i;
            CHAR_ACT(c, 0xE);
        }
        break;
    }
    case 0x4B: {   /* turn to an angle */
        static const union { u32 u; f32 f; } kPi = {0x40490FDB};

        AT(c, 0x10C, f32) = func_002E2D00(kPi.f * (f32)(s16)be16(pc + 1) / 180.0f);
        CHAR_ACT(c, 0xF);
        break;
    }
    case 0x8E: {   /* turn to (x, z) */
        f32 x = (f32)be32(pc + 1) / 1000.0f - AT(c, 0x10, f32);
        f32 z = (f32)be32(pc + 5) / 1000.0f - AT(c, 0x18, f32);

        AT(c, 0x10C, f32) = func_002E2D00(func_0031C5C0(x, z));
        CHAR_ACT(c, 0xF);
        break;
    }
    case 0x56: {   /* door pc[1]: knock or try it (who: the player for event ids 0xF0..) */
        u8 *who = AT(AT(ev, 0x6FC, u8 *), 0x13, u8) >= 0xF0 ? gCharPlayer : c;

        if (VCALL(D_0044E558, 0x34, s32 (*)(VObject *, s32, f32 *))(D_0044E558, pc[1], tmp) != 0) {
            break;
        }
        if (who != NULL) {
            func_00122C20(who, PC(ev)[2] == 1 ? 0x27 : 0x28, 5, 0, 0, (s32)tmp);
        }
        break;
    }
    case 0x6F:
    case 0x70:
    case 0x9A: {   /* go through an exit (0x9A: by door id); 0x70 the other way */
        VObject *rooms = D_0044E568;
        VObject *doors;
        u8 exit;
        s32 mode;

        if (pc[0] == 0x9A) {
            exit = (u8)VCALL(rooms, 0x3C, s32 (*)(VObject *, u32, s32))(rooms, be16(pc + 1), AT(ev, 0x560, s32));
        } else {
            exit = pc[1];
        }
        VCALL(rooms, 0x10, u32 (*)(VObject *, s32, s32))(rooms, AT(ev, 0x560, s32), exit);
        if ((u8)VCALL(rooms, 0x70, s32 (*)(VObject *, s32, s32))(rooms, AT(ev, 0x560, s32), exit) == 0) {
            VCALL(rooms, 0x34, void (*)(VObject *, s32, f32 *))(rooms, exit, tmp);
        } else {
            sceVu0CopyVector(tmp, (f32 *)(AT(AT(ev, 0x6FC, u8 *), 0x0, u8 *) + 0x10));
        }
        doors = D_0044E558;
        if (PC(ev)[0] != 0x70) {
            mode = VCALL(doors, 0x18, s32 (*)(VObject *, s32, f32 *))(doors, exit, tmp) ? 2 : 0;
        } else {
            mode = VCALL(doors, 0x18, s32 (*)(VObject *, s32, f32 *))(doors, exit, tmp) ? 3 : 1;
        }
        AT(c, 0x104, s32) = VCALL(doors, 0x14, s32 (*)(VObject *, s32, s32, f32 *, f32 *, s32))(
            doors, exit, mode, pos, dir, AT(AT(AT(ev, 0x6FC, u8 *), 0x0, u8 *), 0x153C, u8) != 0);
        AT(c, 0x108, s32) = -1;
        sceVu0CopyVector((f32 *)(c + 0x110), pos);
        AT(c, 0x10C, f32) = dir[1];
        CHAR_ACT(c, 5);
        break;
    }
    case 0xA4:
        AT(c, 0x104, u32) = be16(pc + 1);
        AT(c, 0x108, u32) = be16(PC(ev) + 3);
        CHAR_ACT(c, 9);
        break;
    case 0xAA: {
        static const union { u32 u; f32 f; } kPi = {0x40490FDB};

        AT(c, 0x100, u32) = be16(pc + 1);
        AT(c, 0x104, u32) = be16(PC(ev) + 3);
        AT(c, 0x108, s32) = (s16)be16(PC(ev) + 5);
        pos[1] = 0.0f;
        pos[0] = (f32)be32(PC(ev) + 7) / 1000.0f;
        pos[3] = 1.0f;
        pos[2] = (f32)be32(PC(ev) + 0xB) / 1000.0f;
        sceVu0CopyVector((f32 *)(c + 0x110), pos);
        AT(c, 0x10C, f32) = func_002E2D00(kPi.f * (f32)(s16)be16(PC(ev) + 0xF) / 180.0f);
        CHAR_ACT(c, 0x11);
        break;
    }
    case 0xAC:
        AT(c, 0xE4, u8) = pc[1] != 0;
        break;
    case 0xAD: {   /* turn to zone pc[1]'s point (if the zone is set) */
        s32 k = VCALL(ev, 0xA0, s32 (*)(VObject *, s32))(ev, pc[1]);
        u8 *zone = (u8 *)ev + k * 0x30 + 0xBF0;

        if (AT(zone, 0x4, u8)) {
            func_0010E5F0(tmp, (f32 *)(zone + 0x10));
            tmp[3] = 1.0f;
        }
        AT(c, 0x10C, f32) = func_002E2D00(func_0031C5C0(tmp[0] - AT(c, 0x10, f32), tmp[2] - AT(c, 0x18, f32)));
        CHAR_ACT(c, 0xF);
        break;
    }
    case 0xC7:
        AT(c, 0x108, s32) = (s16)be16(pc + 1);
        CHAR_ACT(c, 0x10);
        break;
    default:
        func_002029B0(ev);
        EV_JUMPED(ev) = 1;
        break;
    }
    if (!EV_WAIT(ev) && !EV_JUMPED(ev)) {
        VCALL(ev, 0xC, void (*)(VObject *))(ev);
    }
}

/* a script's character id to a character slot: below 0xF0 (and 0xFB..) one-based (id + 1),
 * 0xF0 the player (0), 0xF1..0xFA slots 7..0x10 */
s32 func_001FBF70(VObject *ev, s32 id) {
    u8 x = id;

    if (x < 0xF0 || x >= 0xFB) {
        return (u8)(id + 1);
    }
    if (x == 0xF0) {
        return 0;
    }
    return x - 0xF1 + 7;
}



/* start action script `script` (0x80..: built in, else the room's, vtable +0x24) for
 * character id `id`: its slot gets a fresh context (character -1: none needed) */
void func_001FBE90(VObject *ev, s32 id, s32 script) {
    u8 *e = (u8 *)ev;
    u8 *pc;
    u8 *s;

    script &= 0xFF;
    if (script & 0x80) {
        pc = D_003D6760[script];
    } else {
        VObject *room = (VObject *)(e + 0x120 + AT(e, 0x560, s32) * 4);

        pc = VCALL(room, 0x24, u8 *(*)(VObject *, s32))(room, script);
    }
    if (pc == NULL) {
        return;
    }
    s = e + 0x564 + (u8)func_001FBF70(ev, id) * 0x18;
    AT(s, 0x0, s32) = -1;
    AT(s, 0x4, u8 *) = NULL;
    AT(s, 0x10, u8) = 0;
    AT(s, 0x8, s32) = 0;
    AT(s, 0x11, u8) = 0;
    AT(s, 0xC, s32) = 0;
    AT(s, 0x12, u8) = 0;
    AT(s, 0x13, u8) = 0xFF;
    AT(s, 0x14, u16) = 0;
    AT(s, 0x13, u8) = id;
    AT(s, 0x4, u8 *) = pc;
}


/* music commands: 0x6A sub-op pc[1] on the music (D_00456DF0): 0 +0x38 fade (pc[2], pc[3]),
 * 1 +0x40, 2 +0xC then +0x1C, 3 wait while +0x10 says it isn't ready, 4 +0x18, 5 +0x4C; 0x6B
 * the progress' +0x48 with pc[1]; 0x6C its +0x4C */
void func_001FFC70(VObject *ev) {
    VObject *mus = D_00456DF0;
    const u8 *pc = PC(ev);

    switch (pc[0]) {
    case 0x6A:
        if (mus == NULL) {
            break;
        }
        switch (pc[1]) {
        case 0:
            VCALL(mus, 0x38, void (*)(VObject *, s32, s32))(mus, pc[2], pc[3]);
            break;
        case 1:
            VCALL(mus, 0x40, void (*)(VObject *))(mus);
            break;
        case 2:
            VCALL(mus, 0xC, void (*)(VObject *))(mus);
            VCALL(mus, 0x1C, void (*)(VObject *))(mus);
            break;
        case 3:
            if (!(u8)VCALL(mus, 0x10, s32 (*)(VObject *))(mus)) {
                EV_WAIT(ev) = 1;
            }
            break;
        case 4:
            VCALL(mus, 0x18, void (*)(VObject *))(mus);
            break;
        case 5:
            VCALL(mus, 0x4C, void (*)(VObject *))(mus);
            break;
        }
        break;
    case 0x6B:
        VCALL(gProgress, 0x48, void (*)(Progress *, s32))(gProgress, pc[1]);
        break;
    case 0x6C:
        VCALL(gProgress, 0x4C, void (*)(Progress *))(gProgress);
        break;
    }
}


/* ---- Hewie (opcodes 0x39 0x3F 0x63 0x77 0x78 0x7A 0x85 0xAF 0xB0 0xBB 0xBD 0xC3..0xC6 0xD0) ---- */

extern void func_00130AF0(u8 *h, s32 act, s32 arg);           /* his action */
extern void func_0013D1F0(u8 *h, s32 add);                    /* his trust */
extern void func_00138AD0(u8 *h, s32 anim, s32 loop);
extern void func_001654E0(u8 *h, s32 a, u32 b);

/* a position in thousandths (3 x be32) */
static void be32_pos(f32 *v, const u8 *p) {
    v[0] = (f32)be32(p) / 1000.0f;
    v[1] = (f32)be32(p + 4) / 1000.0f;
    v[2] = (f32)be32(p + 8) / 1000.0f;
    v[3] = 1.0f;
}

/* zone pc[1]'s point into v, if the zone is set */
static void zone_point(VObject *ev, u32 id, f32 *v) {
    s32 k = VCALL(ev, 0xA0, s32 (*)(VObject *, s32))(ev, id);
    u8 *zone = (u8 *)ev + k * 0x30 + 0xBF0;

    if (AT(zone, 0x4, u8)) {
        func_0010E5F0(v, (f32 *)(zone + 0x10));
        v[3] = 1.0f;
    }
}

/* Hewie's commands (nothing without him; most need him active):
 *   0x39 action be32 pc+1, its argument be32 pc+5      0x3F his +0x64 (be16 pc+1, be16 pc+4, pc[3])
 *   0x63 turn to angle pc+1 (degrees), action 0x72     0x77 func_001654E0 (be16 pc+1, be16 pc+3)
 *   0x78 action 0x12 (a character action, +0xF4)        0x7A to a position (x, z, y), be16 pc+1 / pc+0xF
 *   0x85 trust + be16 pc+1                              0xAF to a position (action 0x14)
 *   0xB0 to zone pc[1]'s point (pc[2] 0: action 0x14, 1: 0xD)
 *   0xBB in his room, waiting (+0xF355C 5) with +0xF3588 = pc[1] != 0
 *   0xBD +0xF3688 = 300   0xC3 action 0x15 with be16 pc+1 / pc+3   0xC4 animation be32 pc+1
 *   0xC5 / 0xC6 a point to go to (+0xF3630, once: +0xF3620) - zone pc[1]'s / character pc[1]'s
 *     position, raised by be32 pc+2 thousandths
 *   0xD0 his side of the room be16 pc+1 (0..2, else -1) */
void func_00200B00(VObject *ev) {
    u8 *h = gCharPartner;
    const u8 *pc;
    f32 v[4] __attribute__((aligned(16))) = {0};   /* (a zone not set leaves it as it was) */

    if (h == NULL) {
        return;
    }
    pc = PC(ev);
    if (pc[0] != 0x3F && pc[0] != 0x85 && AT(h, 0x28, u8) == 0) {
        return;
    }
    switch (pc[0]) {
    case 0x39:
        func_00130AF0(h, be32(pc + 1), be32(pc + 5));
        break;
    case 0x3F:
        VCALL((VObject *)h, 0x64, void (*)(void *, u32, s32, s8))(h, be16(pc + 1), (s16)be16(pc + 4), pc[3]);
        break;
    case 0x63: {
        static const union { u32 u; f32 f; } kPi = {0x40490FDB};

        AT(h, 0x10C, f32) = func_002E2D00(kPi.f * (f32)(s16)be16(pc + 1) / 180.0f);
        func_00130AF0(h, 0x72, 0);
        break;
    }
    case 0x77:
        func_001654E0(h, (s16)be16(pc + 1), be16(pc + 3));
        break;
    case 0x78:
        CHAR_ACT(h, 0x12);
        break;
    case 0x7A:
        AT(h, 0x104, s32) = be16(pc + 1);
        pc = PC(ev);
        v[0] = (f32)be32(pc + 3) / 1000.0f;   /* (stored x, z, y) */
        v[1] = (f32)be32(pc + 0xB) / 1000.0f;
        v[2] = (f32)be32(pc + 7) / 1000.0f;
        v[3] = 1.0f;
        sceVu0CopyVector((f32 *)(h + 0x110), v);
        AT(h, 0x108, s32) = (s16)be16(PC(ev) + 0xF);
        CHAR_ACT(h, 0x13);
        break;
    case 0x85:
        func_0013D1F0(h, (s16)be16(pc + 1));
        break;
    case 0xAF:
        be32_pos(v, pc + 1);
        sceVu0CopyVector((f32 *)(h + 0x110), v);
        CHAR_ACT(h, 0x14);
        break;
    case 0xB0:
        zone_point(ev, pc[1], v);
        v[3] = 1.0f;
        switch (PC(ev)[2]) {
        case 0:
            sceVu0CopyVector((f32 *)(h + 0x110), v);
            CHAR_ACT(h, 0x14);
            break;
        case 1:
            sceVu0CopyVector((f32 *)(h + 0x110), v);
            CHAR_ACT(h, 0xD);
            break;
        }
        break;
    case 0xBB:
        if (AT(ev, 0x560, s32) == AT(h, 0x30, s32)) {
            HW(h, 0xF355C, s32) = 5;
            HW(h, 0xF3588, u8) = pc[1] != 0;
        }
        break;
    case 0xBD:
        HW(h, 0xF3688, s16) = 300;
        break;
    case 0xC3:
        AT(h, 0x104, s32) = be16(pc + 1);
        AT(h, 0x108, s32) = be16(PC(ev) + 3);
        CHAR_ACT(h, 0x15);
        break;
    case 0xC4:
        func_00138AD0(h, be32(pc + 1), -1);
        break;
    case 0xC5:
        zone_point(ev, pc[1], v);
        pc = PC(ev);
        v[3] = 1.0f;
        v[1] += (f32)be32(pc + 2) / 1000.0f;
        if (HW(h, 0xF3620, u8) == 0) {
            HW(h, 0xF3620, u8) = 1;
            sceVu0CopyVector((f32 *)(h + 0xF3630), v);
        }
        break;
    case 0xC6: {
        u32 k = (u8)func_001770D0(gProgress, pc[1]);
        u8 *c = k != 0xFF ? (u8 *)gCharacters[k] : NULL;

        if (c == NULL || AT(c, 0x28, u8) == 0 || AT(ev, 0x560, s32) != AT(c, 0x30, s32)) {
            break;
        }
        sceVu0CopyVector(v, (f32 *)(c + 0x10));
        v[1] += (f32)be32(PC(ev) + 2) / 1000.0f;
        if (HW(h, 0xF3620, u8) == 0) {
            HW(h, 0xF3620, u8) = 1;
            sceVu0CopyVector((f32 *)(h + 0xF3630), v);
        }
        break;
    }
    case 0xD0: {
        u32 side = be16(pc + 1);

        HEWIE_SIDE(h) = side < 3 ? side : (u32)-1;
        break;
    }
    }
}

/* ---- movies and the cutscene director (opcodes 0x60 / 0x61 / 0x62 / 0x6E / 0x89) ---- */

extern VObject *D_0044FE10;   /* the cutscene director */
extern s32 func_001768B0(Progress *p, const char *path, u32 kind);
extern s32 func_002B6410(void *movie);   /* 2 playing, 0 done, -1 none */
extern s32 func_002B64F0(void *movie);
extern s32 func_002B6640(void *movie);   /* restarted: 2 / 0 / -1 as func_002B6410 */
extern void func_0023E878(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */

#define EV_RESULT(ev) AT(ev, 0x934, s32)
#define EV_CUE(ev) AT(ev, 0xBE4, s32)
#define EV_CUE_PREV(ev) AT(ev, 0xBE8, s32)

typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

/* the room handler's string pc[1] (+0x34) */
static const char *room_string(VObject *ev, u32 i) {
    VObject *room = (VObject *)((u8 *)ev + 0x120 + AT(ev, 0x560, s32) * 4);

    return VCALL(room, 0x34, const char *(*)(VObject *, u32))(room, i);
}

/* 0x60 play the movie named by the room (pc[1]) as class pc[2]; 0x61 every active character
 * +0x78, then the cutscene director restarts (+0x8) on the room's script pc[1] (+0x38); 0x6E
 * the movie's Sofdec setting (pc[1], pc[2]); 0x89 prepare message pc[1..2]; 0x62 by pc[1]:
 *   0 movie state -> +0x934     1 stop the movie (+0x14)    2 restart the movie: done waits
 *   3 director +0x10            4 director +0x10, then +0x14 unless mode 5 or +0x44 is 0
 *   5 the director's cue before and after +0x60 (+0xBE8 / +0xBE4)   6 cues off, +0x40, +0x10
 *   7 next cue from the movie, to the director (+0x30); its button 11 toggles flag 0x29
 *   8 camera director +0x10, director +0x48, flag 0x29 off    9 / 10 pause / resume the movie
 *   11 a half-black screen    12 show the prepared message as often as the director says */
void func_001FFE00(VObject *ev) {
    const u8 *pc = PC(ev);
    VObject *d = D_0044FE10;
    VObject *mv = D_0044E958;
    s32 i, n, r;

    switch (pc[0]) {
    case 0x60:
        EV_RESULT(ev) = 0;
        pc = PC(ev);
        func_001768B0(gProgress, room_string(ev, pc[1]), pc[2]);
        return;
    case 0x61:
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
                VCALL(gCharacters[i], 0x78, void (*)(VObject *))(gCharacters[i]);
            }
        }
        VCALL(d, 0x8, void (*)(VObject *))(d);
        VCALL(d, 0x38, void (*)(VObject *, const char *))(d, room_string(ev, PC(ev)[1]));
        return;
    case 0x6E:
        func_0023E878(AT(mv, 0x14, void *), pc[1], pc[2], 0);
        return;
    case 0x89:
        Task_Prepare((Task *)((u8 *)ev + 0x708), (pc[1] << 8 | pc[2]) & 0xFFFF);
        return;
    }
    switch (pc[1]) {
    case 0:
        EV_RESULT(ev) = mv != NULL ? func_002B6410(mv) : -1;
        break;
    case 1:
        if (mv != NULL) {
            VCALL((VObject *)mv, 0x14, void (*)(VObject *))(mv);
        }
        EV_RESULT(ev) = 0;
        break;
    case 2:
        r = mv != NULL ? func_002B6640(mv) : -1;
        if (r == 0) {
            EV_WAIT(ev) = 1;
        } else if (r > 0) {
            EV_RESULT(ev) = 1;
        } else {
            EV_RESULT(ev) = -1;
        }
        break;
    case 3:
        VCALL(d, 0x10, void (*)(VObject *))(d);
        break;
    case 4:
        VCALL(d, 0x10, void (*)(VObject *))(d);
        if (VCALL(d, 0x2C, s32 (*)(VObject *))(d) != 5 && VCALL(d, 0x44, s32 (*)(VObject *))(d) != 0) {
            VCALL(d, 0x14, void (*)(VObject *))(d);
        }
        break;
    case 5:
        EV_CUE_PREV(ev) = VCALL(d, 0x34, s32 (*)(VObject *))(d);
        VCALL(d, 0x60, void (*)(VObject *))(d);
        EV_CUE(ev) = VCALL(d, 0x34, s32 (*)(VObject *))(d);
        break;
    case 6:
        EV_CUE_PREV(ev) = -1;
        EV_CUE(ev) = -1;
        VCALL(d, 0x40, void (*)(VObject *))(d);
        VCALL(d, 0x10, void (*)(VObject *))(d);
        break;
    case 7:
        EV_CUE_PREV(ev) = EV_CUE(ev);
        if (mv != NULL) {
            EV_CUE(ev) = func_002B64F0(mv);
        }
        VCALL(d, 0x30, void (*)(VObject *, s32))(d, EV_CUE(ev));
        if (VCALL(d, 0x54, s32 (*)(VObject *, s32, s32))(d, 0xB, 0) & 1) {
            if (Progress_TestFlag(gProgress, 0x29)) {
                Progress_ClearFlag(gProgress, 0x29);
            } else {
                Progress_SetFlag(gProgress, 0x29);
            }
        }
        break;
    case 8:
        VCALL(D_0044E4F8, 0x10, void (*)(VObject *))(D_0044E4F8);
        VCALL(d, 0x48, void (*)(VObject *))(d);
        Progress_ClearFlag(gProgress, 0x29);
        break;
    case 9:
    case 10:
        if (mv != NULL && (func_002B6410(mv) == 2) == (pc[1] == 10)) {
            VObject *sfd = AT(mv, 0x14, VObject *);

            VCALL(sfd, 0x28, void (*)(VObject *, s32))(sfd, pc[1] == 9);
        }
        break;
    case 11:
        VCALL(D_0044E4F0, 0x7C, RectFn)(D_0044E4F0, 0, 0, 0x200, 0x1C0, 0, 0, 0, 0, 0x80000000, -1, 0, 0x33, -1);
        break;
    case 12: {
        u8 out[2];

        n = VCALL(d, 0x54, s32 (*)(VObject *, u32, u8 *))(d, (u8)VCALL(d, 0x84, s32 (*)(VObject *))(d), out);
        for (i = 0; i < n; i++) {
            Task_ShowPrepared((Task *)((u8 *)ev + 0x708));
        }
        break;
    }
    }
}



/* start a step `step` (with `prio`) in the event's step slot for `prio` (func_001FBF70) (+0x564, 0x18 each:
 * +0 -1, +4 the step, +0x13 its priority, the rest cleared); nothing for NULL */
void func_001FBE00(VObject *ev, s32 prio, void *step) {
    u8 *t;

    if (step == NULL) {
        return;
    }
    t = (u8 *)ev + 0x564 + (u8)func_001FBF70(ev, prio) * 0x18;
    AT(t, 0x0, s32) = -1;
    AT(t, 0x4, s32) = 0;
    AT(t, 0x10, u8) = 0;
    AT(t, 0x8, s32) = 0;
    AT(t, 0x11, u8) = 0;
    AT(t, 0xC, s32) = 0;
    AT(t, 0x12, u8) = 0;
    AT(t, 0x13, u8) = 0xFF;
    AT(t, 0x14, u16) = 0;
    AT(t, 0x13, u8) = prio;
    AT(t, 0x4, void *) = step;
}
