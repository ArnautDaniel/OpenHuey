/* `forth`: the Forth system on its own, for trying things and for the tests.
 *
 *   forth                  a prompt
 *   forth a.fs b.fs        run the files, then exit (status: `exit-status` if a file set it,
 *                          1 if a file had an error, else 0)
 *   forth -i a.fs          run the files, then a prompt */
#include "forth.h"
#include "../game/progress.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int sStatus;

static void p_exit_status(Forth *f, Word *w) {   /* ( n -- ) the tool's exit status */
    (void)w;
    sStatus = (int)forth_pop(f);
}

static void repl(Forth *f) {
    char line[1024];

    while (!f->bye && fgets(line, sizeof(line), stdin) != NULL) {
        if (forth_eval(f, line, strlen(line), "input") == 0 && !f->compiling) {
            printf(" ok\n");
        }
    }
}

int main(int argc, char **argv) {
    Forth *f = forth_new(16 << 20);
    const char *dir = getenv("HG2_SCRIPTS") != NULL ? getenv("HG2_SCRIPTS") : HG2_SCRIPT_DIR;
    char prelude[1024];
    int i, interactive = argc == 1, failed = 0;

    forth_prim(f, "exit-status", p_exit_status);
    bind_state(f);
    forth_add_root(f, dir);
    snprintf(prelude, sizeof(prelude), "%s/prelude.fs", dir);
    if (forth_include(f, prelude) != 0) {
        return 1;
    }
    for (i = 1; i < argc && !f->bye; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            interactive = 1;
        } else {
            char root[1024], *slash;

            snprintf(root, sizeof(root), "%s", argv[i]);   /* its folder: for its USING:s */
            slash = strrchr(root, '/');
            if (slash != NULL) {
                *slash = 0;
                forth_add_root(f, root);
            }
            if (forth_include(f, argv[i]) != 0) {
                failed = 1;
            }
        }
    }
    if (interactive) {
        repl(f);
    }
    forth_free(f);
    return sStatus != 0 ? sStatus : failed;
}
