/* The executable's tables (docs/library-and-mods.md): what the engine reads of SLUS_210.75,
 * by name. The library holds each as a file of its own (library/exe/<name>.bin: the bytes the
 * game has there, or for a table of pointers the same made whole - obstacle-kinds); the engine
 * reads it through files_read ("../exe/<name>.bin": a mod's over the library's). Without a
 * library (the developers' extracted data) the executable is read, by the table's address. */
#ifndef EXETABLES_H
#define EXETABLES_H

#include <stddef.h>
#include <stdint.h>

#include "exe.h"

#define EXETABLES_FORMAT 1

/* table `name`: its bytes (kept: valid while the program runs) and how many; NULL if there is
 * no such table, or it can't be read */
const uint8_t *exetable(const char *name, size_t *size);
/* the executable to read tables from when the library hasn't them (taken: freed with it) */
void exetables_use_exe(Exe *e);

/* the import's: each table out of the executable, as its library file's bytes (malloc'd); the
 * names in turn (i = 0, 1 ... until NULL) */
const char *exetables_name(int i);
uint8_t *exetables_extract(const Exe *e, const char *name, size_t *size);

#endif
