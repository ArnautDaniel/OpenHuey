/* The Forth console: ` opens it over the game. Lines typed are evaluated; everything Forth
 * prints goes to its log (and to stdout). Up/down walk the history. */
#ifndef CONSOLE_H
#define CONSOLE_H

#include "../forth/forth.h"
#include "../platform/platform.h"

#include <stddef.h>

#define CONSOLE_LINES 200
#define CONSOLE_COLS 160
#define CONSOLE_HISTORY 32

typedef struct Console {
    int open;
    char log[CONSOLE_LINES][CONSOLE_COLS];
    int head;                /* the line being written */
    int col;
    char input[CONSOLE_COLS];
    int len;
    char history[CONSOLE_HISTORY][CONSOLE_COLS];
    int nhistory, browse;
} Console;

/* Forth's output function (ctx: the Console) */
void console_output(void *ctx, const char *s, size_t n);
/* handle this frame's typing (when open) */
void console_update(Console *c, const Input *in, Forth *f);
void console_draw(const Console *c, int w, int h);

#endif
