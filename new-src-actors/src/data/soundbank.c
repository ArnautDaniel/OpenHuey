/* The game's sound banks: see soundbank.h. */
#include "soundbank.h"
#include "samples.h"

#include "../core/files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int soundbank_load(SoundBank *b, const char *name) {
    char path[256];

    memset(b, 0, sizeof(*b));
    snprintf(path, sizeof(path), "%s.HD", name);
    b->hd = files_read(path, &b->hd_size);
    snprintf(path, sizeof(path), "%s.SDT", name);
    b->sdt = files_read(path, &b->sdt_size);
    b->samples = samples_load(name);
    if (b->samples == NULL) {
        snprintf(path, sizeof(path), "%s.BD", name);
        b->bd = files_read(path, &b->bd_size);
    }
    if (b->hd == NULL || b->sdt == NULL || (b->bd == NULL && b->samples == NULL)) {
        soundbank_free(b);
        return 0;
    }
    return 1;
}

void soundbank_free(SoundBank *b) {
    free(b->hd);
    free(b->sdt);
    free(b->bd);
    samples_free(b->samples);
    memset(b, 0, sizeof(*b));
}
