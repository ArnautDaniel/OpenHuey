/* The game's data files (paths like "ST_000/ST_003.PAC"; "../" for the disc's own files beside
 * DATA.CVM, e.g. "../MODULES/LIBSD.IRX"), from the disc image or folder chosen at start-up. */
#ifndef FILES_H
#define FILES_H

#include <stddef.h>
#include <stdint.h>

/* the whole file (malloc'd, NUL-terminated past the end), or NULL; size in *size */
uint8_t *files_read(const char *path, size_t *size);
int files_exist(const char *path);

#endif
