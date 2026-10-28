/* full_only.h - a check that runs long (every file of the game's data converted) only in a full run
 * ($OPENHUEY_FULL_TESTS set, not 0); in a quick one it is skipped (ctest's SKIP_RETURN_CODE 77) */
#ifndef FULL_ONLY_H
#define FULL_ONLY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void full_only(const char *what) {
    const char *full = getenv("OPENHUEY_FULL_TESTS");

    if (full == NULL || strcmp(full, "0") == 0) {
        printf("%s: a full run's (OPENHUEY_FULL_TESTS=1) - skipped\n", what);
        exit(77);
    }
}
#endif
