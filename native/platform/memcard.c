/* Memory card: the PC build will keep saves as host files; the PS2 memory-card plumbing is
 * replaced here. */

/* memory card setup (system +0x390, PS2 0x00226570): MCMAN / MCSERV modules, sceMcInit */
void func_00226570(void *mc) { (void)mc; }
