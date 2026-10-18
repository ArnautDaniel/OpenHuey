/* The game's data files, from wherever the disc layer found them (platform/disc.c), under the
 * layers over them (the mods'). */
#include "files.h"

#include <stdio.h>

#define LAYERS 32

static char sLayers[LAYERS][1024];   /* each layer's data/ folder */
static int sNLayers;

void files_layer_add(const char *dir) {
    if (sNLayers < LAYERS) {
        snprintf(sLayers[sNLayers++], sizeof(sLayers[0]), "%s/data", dir);
    }
}

void files_layers_clear(void) { sNLayers = 0; }

static DiscFile *open_layers(const char *path) {
    DiscFile *f = NULL;
    int i;

    for (i = sNLayers - 1; i >= 0 && f == NULL; i--) {
        f = disc_file_open_in(sLayers[i], path);
    }
    return f;
}

DiscFile *files_open(const char *path) {
    DiscFile *f = open_layers(path);

    return f != NULL ? f : disc_file_open(path);
}

uint8_t *files_read(const char *path, size_t *size) { return disc_file_read_all(files_open(path), size); }

uint8_t *files_read_layers(const char *path, size_t *size) { return disc_file_read_all(open_layers(path), size); }

int files_exist(const char *path) {
    DiscFile *f = files_open(path);

    disc_file_close(f);
    return f != NULL;
}
