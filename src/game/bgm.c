/* Streamed music (CRI ADX): methods of the ADX sound system (system +0x305280, global
 * D_0044E980) for its music stream. ADXT_* are CRI's library (native: silent). */
#include "common.h"
#include "game.h"
#include "ptmf.h"

typedef struct Bgm {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ void *work;      /* the stream's work buffer (0x231E4 bytes), NULL: none */
    /* 0x008 */ void *adxt;      /* the stream (ADXT handle) */
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 pad10[0x110 - 0x10];
    /* 0x110 */ f32 volume[5];   /* multiplied together; [3] the master volume */
} Bgm;

_Static_assert(__builtin_offsetof(Bgm, volume) == 0x110, "Bgm.volume");

extern VObject *D_0044E560;      /* the sound driver: +0x6C the output mode */
extern u8 *D_0044E978;           /* +0x38: the master volume */
extern void ADXT_Destroy(void *adxt);
extern void *ADXT_Create(s32 maxch, void *work, s32 size);
extern void ADXT_SetReloadSct(void *adxt, s32 n);
extern void ADXT_SetOutVol(void *adxt, s32 vol);
extern void ADXT_SetOutPan(void *adxt, s32 ch, s32 pan);
extern void ADXT_SetLpFlg(void *adxt, s32 on);
extern void ADXT_SetWaitPlayStart(void *adxt, s32 on);
extern void func_001D4750(s32 mono);   /* ADX: mono output */

/* Set up the music stream on work buffer `work` (NULL: none): looping, stereo or mono as the
 * sound settings say, full volume. */
void func_002D2370(Bgm *b, void *work) {
    if (b->work != NULL) {
        if (b->adxt != NULL) {
            ADXT_Destroy(b->adxt);
            b->adxt = NULL;
        }
        b->work = NULL;
        b->unkC = 0;
    }
    b->work = work;
    if (b->work != NULL) {
        if (b->adxt != NULL) {
            ADXT_Destroy(b->adxt);
            b->adxt = NULL;
        }
        b->adxt = ADXT_Create(2, b->work, 0x231E4);
        if (b->adxt != NULL) {
            ADXT_SetReloadSct(b->adxt, 0x19);
        }
    }
    ADXT_SetOutVol(b->adxt, 0);
    ADXT_SetOutPan(b->adxt, 0, -0x80);
    ADXT_SetOutPan(b->adxt, 1, -0x80);
    if (D_0044E560 == NULL) {
        func_001D4750(0);
    } else {
        s8 mode = VCALL(D_0044E560, 0x6C, s32 (*)(VObject *))(D_0044E560);

        if (mode == 2 || mode == 1) {
            func_001D4750(0);
        } else if (mode == 0) {
            func_001D4750(1);
        } else {
            func_001D4750(0);
        }
    }
    ADXT_SetLpFlg(b->adxt, 1);
    ADXT_SetWaitPlayStart(b->adxt, 0);
    b->volume[3] = *(f32 *)(D_0044E978 + 0x38);
    b->volume[4] = 1.0f;
    b->volume[2] = 1.0f;
    b->volume[1] = 1.0f;
    b->volume[0] = 1.0f;
}
