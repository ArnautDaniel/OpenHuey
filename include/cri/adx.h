#ifndef CRI_ADX_H
#define CRI_ADX_H

/* CRI ADX streams (ADXT): native/platform/adx.c on PC. */
#include "common.h"

extern void *ADXT_Create(s32 maxch, void *work, s32 size);
extern void ADXT_Destroy(void *adxt);
extern s32 ADXT_GetStat(void *adxt);
extern s32 ADXT_IsReadyPlayStart(void *adxt);
extern void ADXT_Pause(void *adxt, s32 on);
extern void ADXT_SetLpFlg(void *adxt, s32 on);
extern void ADXT_SetOutPan(void *adxt, s32 ch, s32 pan);
extern void ADXT_SetOutVol(void *adxt, s32 vol);
extern void ADXT_SetReloadSct(void *adxt, s32 n);
extern void ADXT_SetWaitPlayStart(void *adxt, s32 on);
extern void ADXT_Stop(void *adxt);
extern s32 func_001D3E20(void *adxt);   /* ADX: still playing */
extern void func_001D4750(s32 mono);   /* ADX: mono output */
extern void func_001D4A20(void *adxt, char *name);   /* ADXT_StartFname */
extern void func_001E7430(s32 a, s32 dir);   /* CRI file system: the current folder */
extern s32 func_001EEA38(void);
extern void func_0023C310(void);   /* CRI middleware server */

extern void *ADXT_Create(s32 maxch, void *work, s32 size);
extern void ADXT_Destroy(void *adxt);
extern s32 ADXT_GetStat(void *adxt);
extern s32 ADXT_IsReadyPlayStart(void *adxt);
extern void ADXT_Pause(void *adxt, s32 on);
extern void ADXT_SetLpFlg(void *adxt, s32 on);
extern void ADXT_SetOutPan(void *adxt, s32 ch, s32 pan);
extern void ADXT_SetOutVol(void *adxt, s32 vol);
extern void ADXT_SetReloadSct(void *adxt, s32 n);
extern void ADXT_SetWaitPlayStart(void *adxt, s32 on);
extern void ADXT_Stop(void *adxt);
extern s32 func_001D3E20(void *adxt);   /* ADX: still playing */
extern void func_001D4750(s32 mono);   /* ADX: mono output */
extern void func_001D4A20(void *adxt, char *name);   /* ADXT_StartFname */
extern void func_001E7430(s32 a, s32 dir);   /* CRI file system: the current folder */
extern s32 func_001EEA38(void);
extern void func_0023C310(void);   /* CRI middleware server */

extern void *ADXT_Create(s32 maxch, void *work, s32 size);
extern void ADXT_Destroy(void *adxt);
extern s32 ADXT_GetStat(void *adxt);
extern s32 ADXT_IsReadyPlayStart(void *adxt);
extern void ADXT_Pause(void *adxt, s32 on);
extern void ADXT_SetLpFlg(void *adxt, s32 on);
extern void ADXT_SetOutPan(void *adxt, s32 ch, s32 pan);
extern void ADXT_SetOutVol(void *adxt, s32 vol);
extern void ADXT_SetReloadSct(void *adxt, s32 n);
extern void ADXT_SetWaitPlayStart(void *adxt, s32 on);
extern void ADXT_Stop(void *adxt);
extern s32 func_001D3E20(void *adxt);   /* ADX: still playing */
extern void func_001D4750(s32 mono);   /* ADX: mono output */
extern void func_001D4A20(void *adxt, char *name);   /* ADXT_StartFname */
extern void func_001E7430(s32 a, s32 dir);   /* CRI file system: the current folder */
extern s32 func_001EEA38(void);
extern void func_0023C310(void);   /* CRI middleware server */

#endif /* CRI_ADX_H */
