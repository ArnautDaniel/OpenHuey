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
extern s32 ADXT_IsPlaying(void *adxt);   /* ADX: still playing */
extern void ADXT_SetOutputMono(s32 mono);   /* ADX: mono output */
extern void ADXT_StartFname(void *adxt, char *name);   /* ADXT_StartFname */
extern void CriFs_SetDir(s32 a, s32 dir);   /* CRI file system: the current folder */
extern s32 func_001EEA38(void);
extern void mwPly_ExecServer(void);   /* CRI middleware server */

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
extern s32 ADXT_IsPlaying(void *adxt);   /* ADX: still playing */
extern void ADXT_SetOutputMono(s32 mono);   /* ADX: mono output */
extern void ADXT_StartFname(void *adxt, char *name);   /* ADXT_StartFname */
extern void CriFs_SetDir(s32 a, s32 dir);   /* CRI file system: the current folder */
extern s32 func_001EEA38(void);
extern void mwPly_ExecServer(void);   /* CRI middleware server */

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
extern s32 ADXT_IsPlaying(void *adxt);   /* ADX: still playing */
extern void ADXT_SetOutputMono(s32 mono);   /* ADX: mono output */
extern void ADXT_StartFname(void *adxt, char *name);   /* ADXT_StartFname */
extern void CriFs_SetDir(s32 a, s32 dir);   /* CRI file system: the current folder */
extern s32 func_001EEA38(void);
extern void mwPly_ExecServer(void);   /* CRI middleware server */

#endif /* CRI_ADX_H */
