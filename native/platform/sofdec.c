/* CRI Sofdec (MPEG movies, mwPly*): not available on PC yet. Creating a player fails, which the
 * game handles by ending the movie at once (the boot logo, the opening, ...). */
#include <stddef.h>

/* mwPlyInitSfdFx */
void mwPlyInitSfdFx(void *prm) { (void)prm; }

/* mwPlyFinishSfdFx (PS2 0x0023AE40) */
void func_0023AE40(void) {}

/* mwPlyCalcWorkCprmSfd: work buffer size for the creation parameters */
int mwPlyCalcWorkCprmSfd(void *cprm) { (void)cprm; return 0x40000; }

/* mwPlyCreateSofdec (PS2 0x00238BF0) */
void *func_00238BF0(void *cprm) { (void)cprm; return NULL; }

/* (PS2 0x0023E878) a player setting (event command 0xDA / the movie scene): no player here */
void func_0023E878(void *ply, unsigned a, unsigned b, int c) { (void)ply; (void)a; (void)b; (void)c; }
