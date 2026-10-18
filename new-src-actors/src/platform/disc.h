/* Where the game's files come from: the player's own Haunting Ground disc image (.iso), its
 * DATA.CVM, or the folder DATA.CVM was extracted to (the developers'). Chosen at start-up - given,
 * remembered from last time, or picked with a file dialog. */
#ifndef DISC_H
#define DISC_H

#include <stddef.h>

typedef struct DiscFile DiscFile;

/* find the game data at start-up: `given` (the command line / $HG_DATA) if set, else the disc
 * chosen last time, else `fallback`, else ask the player with a file dialog (not `headless`).
 * 0: none (a message said why) */
/* use the game data at `path` (an .iso, a DATA.CVM or an extracted folder): 1 if it is there */
int disc_open(const char *path);
int disc_pick(const char *why);   /* the player chooses the disc image (opened): 0 if they cancel */
/* what was wrong with the last path disc_open refused */
const char *disc_error(void);

/* a file of the game's data ("ST_000/ST_003.PAC"; "\" separates folders as on the PS2, case
 * doesn't matter), NULL if it isn't there. A name starting "../" is beside DATA.CVM on the disc
 * ("../MODULES/LIBSD.IRX", "../SLUS_210.75"). */
DiscFile *disc_file_open(const char *path);
/* the same in another folder laid out as the data is (a mod's: "../" its parent) */
DiscFile *disc_file_open_in(const char *folder, const char *path);
/* a file already in memory (malloc'd: taken, freed with it) */
DiscFile *disc_file_memory(unsigned char *data, long size);
size_t disc_file_read(DiscFile *f, void *buf, size_t n);
void disc_file_seek(DiscFile *f, long pos);   /* from the start */
long disc_file_size(const DiscFile *f);
void disc_file_close(DiscFile *f);
/* the whole file (malloc'd, NUL-terminated past its end), or NULL; its size in *size */
unsigned char *disc_read_all(const char *path, size_t *size);
unsigned char *disc_file_read_all(DiscFile *f, size_t *size);   /* (closes it) */

/* the game's executable, SLUS_210.75 (on the disc; beside a DATA.CVM or the folder; $HG_EXE),
 * NULL if there is none */
DiscFile *disc_exe(void);

/* the folder the player's files (saves, settings, the disc chosen) are kept in, ending with / */
const char *disc_user_dir(void);

/* the files of an opened image (the import's): how many, and each one's path and size */
int disc_is_image(void);
int disc_entries(void);
int disc_entry(int i, char *path, size_t n, long *size);
const char *disc_folder(void);

#endif
