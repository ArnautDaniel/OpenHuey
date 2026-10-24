/* The updater (docs/library-and-mods.md, "Updating"): the newest release build - the rolling
 * `openhuey-latest` - downloaded with the system's curl, unpacked with its tar, and put in place of
 * what a release ships (the program, scripts/, docs/ ...) beside the executable. The player's
 * library/, user/ and mods/ are never touched. Only a release build updates itself (or any build
 * given OPENHUEY_UPDATE_URL: the tests). */
#ifndef UPDATE_H
#define UPDATE_H

#include "forth.h"

/* `openhuey --update`: checked, downloaded and installed here, progress on stderr. 0: up to date
 * or updated; 1: it failed (said why) */
int update_run(void);

/* at start-up: what an update before left (update/: its download, the files it replaced) removed */
void update_cleanup(void);

/* the title's UPDATE (scripts/updater.fs): updater? update-start update-state update-progress
 * update-text update-restart openhuey-build */
void bind_update(Forth *f);

#endif
