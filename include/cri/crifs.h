#ifndef CRI_CRIFS_H
#define CRI_CRIFS_H

/* CRI file system (ROFS / ADXF): native/platform/crifs.c on PC. */
#include "common.h"

extern s32 ADXF_GetStat(void *f);
extern s32 ADXF_ReadNw(void *f, s32 nsct, u32 buf);
extern void ADXF_Seek(void *f, s32 pos, s32 type);
extern void ADXF_Stop(void *f);
extern void ADXF_StopNw(void *f);
extern void *ADXF_OpenInDir(const char *name, void *dir);   /* ADXF open in a folder */
extern void ADXF_Close(void *f);   /* ADXF close */
extern s32 ADXF_FileSectors(void *f);   /* ADXF file size (sectors) */
extern s32 ROFS_LoadDir(const char *dir, void *list, s32 max);   /* ROFS_LoadDir (s16: 0 = done) */

extern s32 ADXF_GetStat(void *f);
extern s32 ADXF_ReadNw(void *f, s32 nsct, u32 buf);
extern void ADXF_Seek(void *f, s32 pos, s32 type);
extern void ADXF_Stop(void *f);
extern void ADXF_StopNw(void *f);
extern void *ADXF_OpenInDir(const char *name, void *dir);   /* ADXF open in a folder */
extern void ADXF_Close(void *f);   /* ADXF close */
extern s32 ADXF_FileSectors(void *f);   /* ADXF file size (sectors) */
extern s32 ROFS_LoadDir(const char *dir, void *list, s32 max);   /* ROFS_LoadDir (s16: 0 = done) */

extern s32 ADXF_GetStat(void *f);
extern s32 ADXF_ReadNw(void *f, s32 nsct, u32 buf);
extern void ADXF_Seek(void *f, s32 pos, s32 type);
extern void ADXF_Stop(void *f);
extern void ADXF_StopNw(void *f);
extern void *ADXF_OpenInDir(const char *name, void *dir);   /* ADXF open in a folder */
extern void ADXF_Close(void *f);   /* ADXF close */
extern s32 ADXF_FileSectors(void *f);   /* ADXF file size (sectors) */
extern s32 ROFS_LoadDir(const char *dir, void *list, s32 max);   /* ROFS_LoadDir (s16: 0 = done) */

#endif /* CRI_CRIFS_H */
