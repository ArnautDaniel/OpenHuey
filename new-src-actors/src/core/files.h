/* The game's data files (paths like "ST_000/ST_003.PAC"; "../" for the disc's own files beside
 * DATA.CVM, e.g. "../MODULES/LIBSD.IRX"), from the disc image or folder chosen at start-up - or
 * from a layer over it: the mods' library/ folders (docs/library-and-mods.md), the last added
 * looked in first. */
#ifndef FILES_H
#define FILES_H

#include <stddef.h>
#include <stdint.h>

#include "../platform/disc.h"

/* the whole file (malloc'd, NUL-terminated past the end), or NULL; size in *size */
uint8_t *files_read(const char *path, size_t *size);
int files_exist(const char *path);
DiscFile *files_open(const char *path);   /* (streamed: the movies) */

/* a folder laid out as the library is (data/ in it, the disc's own files beside) over the
 * data; all of them taken away */
void files_layer_add(const char *dir);
void files_layers_clear(void);
/* the file from the layers only (a mod's), NULL if none has it */
uint8_t *files_read_layers(const char *path, size_t *size);

#endif
