/* Mods (docs/library-and-mods.md): folders in mods/ beside the library, each with a manifest
 * (mod.fs), files over the library's (library/) and scripts loaded after the game's in a
 * vocabulary of the mod's own (scripts/<name>.fs). */
#ifndef MODS_H
#define MODS_H

#include "../forth/forth.h"

#define MODS_API_MAJOR 1   /* the mod API: the game's messages, broadcasts and hooks (a mod's */
#define MODS_API_MINOR 0   /* `requires: hga 1 0`): the minor grows with additions */

/* the mods turned on, decided at start-up: those in `folder` (NULL: none - the tests) as
 * mods/order.txt says, then the folders `named` (--mod); their files layered over the data */
void mods_find(const char *folder, const char *const *named, int nnamed);
/* each mod's scripts, after the game's (game.fs) */
void mods_load_scripts(Forth *f, const char *game_scripts);
/* the mods found, in load order (the mods menu): i's name, whether it is on and if not why */
int mods_count(void);
const char *mods_name(int i, int *on, const char **why);
/* the console's `.mods`, `mod-api` */
void bind_mods(Forth *f);

#endif
