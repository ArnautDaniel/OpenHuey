/* The game library (docs/library-and-mods.md): the player's disc imported once into
 * library/ beside the executable - DATA.CVM's files in library/data/, the disc's own (the
 * executable, its modules) in library/ - with a manifest of what was made; the player's saves
 * and settings in user/ beside it. */
#ifndef LIBRARY_H
#define LIBRARY_H

/* the game data opened: a disc image or folder given (development, `--disc`), else the library,
 * else the developer's default, else (with a window) the player chooses the disc and it is
 * imported. 0: none (the reason printed) */
int library_choose(const char *given, const char *fallback, int headless);

/* `hga --import <disc>`: the disc imported into the library (headless: progress on stderr) */
int library_import_from(const char *disc, int headless);

const char *library_dir(void);        /* "<base>library/" */
const char *library_user_dir(void);   /* "<base>user/" (made when first asked for) */
const char *library_mods_dir(void);   /* "<base>mods/" (made when first asked for) */

#endif
