/* The game's data files, from wherever the disc layer found them (platform/disc.c), under the
 * layers over them (the mods'). */
#include "files.h"

#include <stdio.h>
#include <string.h>

#include "zip.h"

#define LAYERS 64

typedef struct Layer {
    char dir[1024];    /* a folder's data/; or in the zip, its library/ */
    const Zip *zip;
} Layer;

static Layer sLayers[LAYERS];
static int sNLayers;

void files_layer_add(const char *dir) {
    if (sNLayers < LAYERS) {
        snprintf(sLayers[sNLayers].dir, sizeof(sLayers[0].dir), "%s/data", dir);
        sLayers[sNLayers++].zip = NULL;
    }
}

void files_layer_add_zip(const Zip *z, const char *prefix) {
    if (sNLayers < LAYERS) {
        snprintf(sLayers[sNLayers].dir, sizeof(sLayers[0].dir), "%slibrary/", prefix);
        sLayers[sNLayers++].zip = z;
    }
}

static void zip_path(const Layer *l, const char *path, char *name, size_t n) {   /* (its name in the zip) */
    if (strncmp(path, "../", 3) == 0 || strncmp(path, "..\\", 3) == 0) {
        snprintf(name, n, "%s%s", l->dir, path + 3);
    } else {
        snprintf(name, n, "%sdata/%s", l->dir, path);
    }
}

static DiscFile *open_in_zip(const Layer *l, const char *path) {
    char name[1300];
    size_t size;
    uint8_t *data;
    int i;

    zip_path(l, path, name, sizeof(name));
    if ((i = zip_find(l->zip, name)) < 0 || (data = zip_read(l->zip, i, &size)) == NULL) {
        return NULL;
    }
    return disc_file_memory(data, (long)size);
}

void files_layers_clear(void) { sNLayers = 0; }

/* ---- the language ---- */

static const char *const kLanguages[FILES_LANGUAGES] = {"en", "fr", "de", "it", "es"};
static int sLanguage;
static unsigned sLanguageSerial;

const char *files_language_code(int i) { return i >= 0 && i < FILES_LANGUAGES ? kLanguages[i] : NULL; }

int files_language(void) { return sLanguage; }

void files_language_set(const char *code) {
    int i, to = 0;

    for (i = 0; code != NULL && i < FILES_LANGUAGES; i++) {
        if (strcmp(code, kLanguages[i]) == 0) {
            to = i;
        }
    }
    if (to != sLanguage) {
        sLanguage = to;
        sLanguageSerial++;
    }
}

unsigned files_language_serial(void) { return sLanguageSerial; }

static int localized_as(int lang, const char *path, char *out, size_t n) {
    const char *slash = strrchr(path, '/'), *dot = strrchr(path, '.');

    if (lang <= 0 || lang >= FILES_LANGUAGES) {
        return 0;
    }
    if (dot == NULL || (slash != NULL && dot < slash)) {   /* (no extension: the code at its end) */
        snprintf(out, n, "%s.%s", path, kLanguages[lang]);
    } else {
        snprintf(out, n, "%.*s.%s%s", (int)(dot - path), path, kLanguages[lang], dot);
    }
    return 1;
}

int files_localized(const char *path, char *out, size_t n) { return localized_as(sLanguage, path, out, n); }

int files_language_present(int i) {   /* (by its system messages: every language has them) */
    char path[64];

    if (i == 0) {
        return 1;
    }
    return localized_as(i, "SUBSCR/MSG_BASE.txt", path, sizeof(path)) && files_exist(path);
}

static DiscFile *open_layers(const char *path) {
    DiscFile *f = NULL;
    int i;

    for (i = sNLayers - 1; i >= 0 && f == NULL; i--) {
        f = sLayers[i].zip != NULL ? open_in_zip(&sLayers[i], path) : disc_file_open_in(sLayers[i].dir, path);
    }
    return f;
}

DiscFile *files_open(const char *path) {
    DiscFile *f = open_layers(path);

    return f != NULL ? f : disc_file_open(path);
}

uint8_t *files_read(const char *path, size_t *size) { return disc_file_read_all(files_open(path), size); }

uint8_t *files_read_layers(const char *path, size_t *size) { return disc_file_read_all(open_layers(path), size); }

int files_exist(const char *path) {   /* (a zipped mod's file: found in its directory, not unpacked) */
    char name[1300];
    DiscFile *f;
    int i;

    for (i = sNLayers - 1; i >= 0; i--) {
        if (sLayers[i].zip != NULL) {
            zip_path(&sLayers[i], path, name, sizeof(name));
            if (zip_find(sLayers[i].zip, name) >= 0) {
                return 1;
            }
        } else if ((f = disc_file_open_in(sLayers[i].dir, path)) != NULL) {
            disc_file_close(f);
            return 1;
        }
    }
    f = disc_file_open(path);
    disc_file_close(f);
    return f != NULL;
}
