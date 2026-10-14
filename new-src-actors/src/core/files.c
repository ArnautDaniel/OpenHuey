/* The game's data files, from wherever the disc layer found them (platform/disc.c). */
#include "files.h"

#include "../platform/disc.h"

uint8_t *files_read(const char *path, size_t *size) {
    return disc_read_all(path, size);
}

int files_exist(const char *path) {
    DiscFile *f = disc_file_open(path);

    disc_file_close(f);
    return f != NULL;
}
