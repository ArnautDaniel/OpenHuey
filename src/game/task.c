/* Tasks: the UI boxes / text the boot sequence, menus and messages draw (a state machine each,
 * see include/scene_boot.h). */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "scene_boot.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

extern void func_00382D30(Task *t, s32 kind);

/* update: count the frame, run the state */
void func_00384BA0(Task *t) {
    AT(t, 0x80, s32)++;
    ptmf_scall(t, &t->state);
}

/* draw (kinds 1..3; 0 and 4 draw nothing) */
void func_00384B60(Task *t) {
    if (t->unk10 != 0 && t->unk10 != 4) {
        func_00382D30(t, t->unk10);
    }
}

/* the state of a task with nothing to do */
void Task_StateIdle(Task *t) {
}
