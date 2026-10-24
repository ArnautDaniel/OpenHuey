/* `forth`: the Forth system on its own with OpenHuey's game state and actor kernel, for trying
 * things and for the tests (ClankForth's forth_tool_main: `forth`, `forth a.fs b.fs`, `forth -i a.fs`).
 * The scripts' folder is a root its USING:s are looked for in. */
#include "forth.h"
#include "../game/progress.h"
#include "../hactor/hactor.h"

#include <stdlib.h>

static void bind(Forth *f) {
    const char *dir = getenv("HG2_SCRIPTS") != NULL ? getenv("HG2_SCRIPTS") : HG2_SCRIPT_DIR;

    bind_state(f);
    bind_hactor(f);
    forth_add_root(f, dir);
}

int main(int argc, char **argv) {
    return forth_tool_main(argc, argv, bind);
}
