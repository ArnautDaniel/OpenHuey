/* Where the game's files come from (disc.c): the Haunting Ground disc image (.iso), its DATA.CVM,
 * or the folder DATA.CVM was extracted to. */
#ifndef HG_DISC_H
#define HG_DISC_H

#include <stddef.h>

typedef struct HgFile HgFile;

/* find the game data at startup: `given` (the command line / $HG_DATA) if set, else the disc chosen
 * last time, else `fallback`, else ask the player with a file dialog. 0: none (a message said why) */
int hg_disc_choose(const char *given, const char *fallback);
/* use the game data at `path` (an .iso, a DATA.CVM or an extracted folder): 1 if it is there */
int hg_disc_open(const char *path);
/* what was wrong with the last path hg_disc_open refused */
const char *hg_disc_error(void);

/* open `name` in `folder` ("" or NULL: the top) of the game data, NULL if it isn't there; names
 * are matched without regard to case, and "\" separates folders as on the PS2 */
HgFile *hg_file_open(const char *folder, const char *name);
size_t hg_file_read(HgFile *f, void *buf, size_t n);
int hg_file_seek(HgFile *f, long pos);   /* from the start; 0 = done */
long hg_file_size(const HgFile *f);
long hg_file_tell(const HgFile *f);
void hg_file_close(HgFile *f);

/* the game's executable, SLUS_210.75 (on the disc, or found next to a DATA.CVM / folder;
 * $HG_SLUS), NULL if there is none; and where it is */
HgFile *hg_disc_exe(void);
const char *hg_disc_exe_path(void);

/* the folder the player's files (saves, options, the disc path) are kept in */
const char *hg_save_dir(void);

#endif
