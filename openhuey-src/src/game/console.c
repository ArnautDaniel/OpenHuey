#include "console.h"

#include "../render/render.h"

#include <stdio.h>
#include <string.h>

static void new_line(Console *c) {
    c->head = (c->head + 1) % CONSOLE_LINES;
    c->log[c->head][0] = 0;
    c->col = 0;
}

void console_output(void *ctx, const char *s, size_t n) {
    Console *c = ctx;
    size_t i;

    fwrite(s, 1, n, stdout);
    fflush(stdout);
    for (i = 0; i < n; i++) {
        if (s[i] == '\n') {
            new_line(c);
        } else if (s[i] != '\r') {
            if (c->col >= CONSOLE_COLS - 1) {
                new_line(c);
            }
            c->log[c->head][c->col++] = s[i];
            c->log[c->head][c->col] = 0;
        }
    }
}

static void submit(Console *c, Forth *f) {
    c->input[c->len] = 0;
    console_output(c, "> ", 2);
    console_output(c, c->input, (size_t)c->len);
    console_output(c, "\n", 1);
    if (c->len > 0 && (c->nhistory == 0 || strcmp(c->history[(c->nhistory - 1) % CONSOLE_HISTORY], c->input) != 0)) {
        snprintf(c->history[c->nhistory % CONSOLE_HISTORY], CONSOLE_COLS, "%s", c->input);
        c->nhistory++;
    }
    c->browse = c->nhistory;
    if (forth_eval(f, c->input, (size_t)c->len, "console") == 0 && !f->compiling) {
        if (c->col > 0) {
            console_output(c, " ", 1);
        }
        console_output(c, "ok\n", 3);
    }
    c->len = 0;
}

static void recall(Console *c, int step) {
    int first = c->nhistory > CONSOLE_HISTORY ? c->nhistory - CONSOLE_HISTORY : 0;

    c->browse += step;
    if (c->browse < first) {
        c->browse = first;
    }
    if (c->browse >= c->nhistory) {
        c->browse = c->nhistory;
        c->len = 0;
        return;
    }
    snprintf(c->input, CONSOLE_COLS, "%s", c->history[c->browse % CONSOLE_HISTORY]);
    c->len = (int)strlen(c->input);
}

void console_update(Console *c, const Input *in, Forth *f) {
    const char *t;

    if (!c->open) {
        return;
    }
    for (t = in->text; *t != 0; t++) {
        if (*t != '`' && *t >= 32 && *t < 127 && c->len < CONSOLE_COLS - 1) {
            c->input[c->len++] = *t;
        }
    }
    if (in->pressed[SDL_SCANCODE_BACKSPACE] && c->len > 0) {
        c->len--;
    }
    if (in->pressed[SDL_SCANCODE_UP]) {
        recall(c, -1);
    }
    if (in->pressed[SDL_SCANCODE_DOWN]) {
        recall(c, 1);
    }
    if (in->pressed[SDL_SCANCODE_RETURN] || in->pressed[SDL_SCANCODE_KP_ENTER]) {
        submit(c, f);
    }
}

void console_draw(const Console *c, int w, int h) {
    float scale = h >= 900 ? 3.0f : 2.0f, lh = render_line_height(scale), y;
    int rows = (int)((float)h * 0.5f / lh) - 1, i;

    if (!c->open) {
        return;
    }
    render_rect(0, 0, (float)w, (rows + 1) * lh + 8, 0x101418E0);
    y = 4;
    for (i = rows - 1; i >= 1; i--) {   /* the log's last lines, oldest at the top */
        const char *line = c->log[(c->head - i + 1 + CONSOLE_LINES * 2) % CONSOLE_LINES];

        render_text(6, y, scale, 0xC8D0D8FF, line, (int)strlen(line));
        y += lh;
    }
    y += lh * 0.25f;
    render_text(6, y, scale, 0xFFE070FF, "> ", 2);
    render_text(6 + render_text_width(scale, 2), y, scale, 0xFFFFFFFF, c->input, c->len);
    render_rect(6 + render_text_width(scale, 2 + c->len), y, render_text_width(scale, 1) - scale, 7 * scale, 0xFFE070C0);
}
