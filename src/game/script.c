/* The script machine base (the event system derives from it): +0x4 the pc, +0x8 the block
 * depth. Commands are below 0xF0; the control ops structure them:
 *   F0 if, F1 if not, F2 and, F3 and not, F4 or, F5 or not (each followed by a condition),
 *   F6 / FA open an unconditional block, F7 else, F8 / FB end, F9 end + skip, FF end of script.
 * Vtable: +0xC step over a command, +0x10 evaluate the condition at the pc (and step over it),
 * +0x14 step over a condition. */
#include "common.h"
#include "game.h"
#include "script.h"

#define SC_PC(s) AT(s, 0x4, u8 *)
#define SC_DEPTH(s) AT(s, 0x8, u8)

/* start running `script` */
void func_00121890(u8 *s, u8 *script) {
    SC_PC(s) = script;
    SC_DEPTH(s) = 0;
}

/* skip the rest of the current block: to its else (F7) or past its end, stepping over nested
 * blocks */
void func_00121380(u8 *s) {
    u8 depth = SC_DEPTH(s);
    u8 found = 0;

    do {
        u8 op = *SC_PC(s);

        if (op < 0xF0) {
            VCALL(s, 0xC, void (*)(u8 *))(s);
            continue;
        }
        switch (op) {
        case 0xF0:
        case 0xF1:
            SC_PC(s)++;
            VCALL(s, 0x14, void (*)(u8 *))(s);
            depth++;
            break;
        case 0xF6:
        case 0xFA:
            depth++;
            SC_PC(s)++;
            break;
        case 0xF7:
            SC_PC(s)++;
            if (depth == SC_DEPTH(s)) {
                found = 1;
            }
            break;
        case 0xF8:
        case 0xF9:
        case 0xFB:
            SC_PC(s)++;
            if (depth == SC_DEPTH(s)) {
                SC_DEPTH(s)--;
                found = 1;
            } else {
                depth--;
            }
            break;
        default:
            SC_PC(s)++;
            VCALL(s, 0x14, void (*)(u8 *))(s);
            break;
        }
    } while (found != 1);
}

/* an if: evaluate its condition chain (the and / or ops combine left to right); false: skip
 * to the else or the end */
void func_001214F0(u8 *s) {
    u8 depth = SC_DEPTH(s);
    u8 done = 0;
    u8 result = 0;
    s32 r;

    do {
        switch (*SC_PC(s)) {
        case 0xF0:
            if (depth == SC_DEPTH(s)) {
                SC_DEPTH(s)++;
                SC_PC(s)++;
                result = VCALL(s, 0x10, s32 (*)(u8 *))(s);
            } else {
                done = 1;
            }
            break;
        case 0xF1:
            if (depth == SC_DEPTH(s)) {
                SC_DEPTH(s)++;
                SC_PC(s)++;
                result = VCALL(s, 0x10, s32 (*)(u8 *))(s) == 0;
            } else {
                done = 1;
            }
            break;
        case 0xF2:
            SC_PC(s)++;
            r = (u8)VCALL(s, 0x10, s32 (*)(u8 *))(s);
            result = (result & r) != 0;
            break;
        case 0xF3:
            SC_PC(s)++;
            r = VCALL(s, 0x10, s32 (*)(u8 *))(s) == 0;
            result = (result & r) != 0;
            break;
        case 0xF4:
            SC_PC(s)++;
            r = (u8)VCALL(s, 0x10, s32 (*)(u8 *))(s);
            result = (result | r) != 0;
            break;
        case 0xF5:
            SC_PC(s)++;
            r = VCALL(s, 0x10, s32 (*)(u8 *))(s) == 0;
            result = (result | r) != 0;
            break;
        case 0xF6:
            result = 1;
            done = 1;
            SC_DEPTH(s)++;
            SC_PC(s)++;
            break;
        default:
            done = 1;
            break;
        }
    } while (done != 1);
    if (!result) {
        func_00121380(s);
    }
}

/* run the control ops at the pc, up to the next command (or the script's end) */
void func_00121730(u8 *s) {
    u8 op;

    while ((op = *SC_PC(s)) >= 0xF0) {
        if (op < 0xF7) {
            func_001214F0(s);
        } else if (op == 0xF7) {
            SC_PC(s)++;
            func_00121380(s);
        } else if (op == 0xF8) {
            SC_DEPTH(s)--;
            SC_PC(s)++;
        } else if (op == 0xFA) {
            SC_DEPTH(s)++;
            SC_PC(s)++;
        } else if (op == 0xF9) {
            SC_DEPTH(s)--;
            SC_PC(s)++;
            func_00121380(s);
        } else if (op == 0xFB) {
            SC_DEPTH(s)--;
            SC_PC(s)++;
        } else if (op == 0xFF) {
            return;
        }
        /* (0xFC..0xFE: not handled; the original spins on them) */
    }
}
