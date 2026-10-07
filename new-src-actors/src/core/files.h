/* The game's data files: the extracted DATA.CVM folder (paths like "ST_000/ST_003.PAC"). */
#ifndef FILES_H
#define FILES_H

#include <stddef.h>
#include <stdint.h>

void files_set_root(const char *dir);
const char *files_root(void);
/* the whole file (malloc'd, NUL-terminated past the end), or NULL; size in *size */
uint8_t *files_read(const char *path, size_t *size);
int files_exist(const char *path);

#endif
