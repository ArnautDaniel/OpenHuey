/* The game's messages: see messages.h. */
#include "messages.h"

#include "../core/files.h"
#include "../data/pac.h"
#include "engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *sBase, *sSub;   /* SUBSCR/MSG_BASE.BIN, MSG_SUB.BIN */
static size_t sBaseSize, sSubSize;
static MessageLayout sLayout;
static uint8_t sParams[4][64];   /* gMessageParams: copied text, NUL-ended */

/* message `id`'s text in a table (u16 count, then u16 offsets from the table's start) */
static const uint8_t *text_in(const uint8_t *table, size_t size, int id) {
    uint16_t n, off;

    if (table == NULL || size < 4) {
        return NULL;
    }
    memcpy(&n, table, 2);
    if (id < 0 || id >= n || (size_t)(id + 2) * 2 > size) {
        return NULL;
    }
    memcpy(&off, table + id * 2 + 2, 2);
    return off < size ? table + off : NULL;
}

static void load_tables(void) {
    if (sBase == NULL) {
        sBase = files_read("SUBSCR/MSG_BASE.BIN", &sBaseSize);
    }
    if (sSub == NULL) {
        sSub = files_read("SUBSCR/MSG_SUB.BIN", &sSubSize);
    }
}

const uint8_t *message_text(int id) {
    size_t size;
    const uint8_t *room;

    load_tables();
    if (id & 0x8000) {
        return text_in(sBase, sBaseSize, id & 0x7FFF);
    }
    if (id & 0x4000) {
        return text_in(sSub, sSubSize, id & 0x3FFF);
    }
    room = pac_section(&gEngine.room.pac, PAC_TEXT, &size);
    return text_in(room, size, id);
}

void message_set_param(int slot, int id) {   /* Msg_SetParamSystem: system message 0x100 + id's first line */
    const uint8_t *src = message_text((id & 0xFFFF) + 0x8100);
    int i = 0;

    if (slot < 0 || slot >= 4) {
        return;
    }
    while (src != NULL && i < 63 && src[i] != 0 && src[i] != 1) {
        sParams[slot][i] = src[i];
        i++;
    }
    sParams[slot][i] = 0;
}

/* ---- laying a message out in pages of lines ---- */

static MessagePage *page(void) { return &sLayout.pages[sLayout.npages - 1]; }

static void new_line(void) {
    MessagePage *p = page();

    if (p->nlines < MESSAGE_LINES) {
        p->nlines++;
    }
}

static void new_page(void) {
    if (sLayout.npages < MESSAGE_PAGES) {
        sLayout.npages++;
        memset(page(), 0, sizeof(MessagePage));
        page()->nlines = 1;
    }
}

static void put(char ch) {
    MessagePage *p = page();
    char *line = p->lines[p->nlines - 1];
    size_t n = strlen(line);

    if (n + 1 < MESSAGE_LINE_CHARS) {
        line[n] = ch;
        line[n + 1] = 0;
    }
}

static void put_text(const char *s) {
    while (*s) {
        put(*s++);
    }
}

const MessageLayout *message_layout(int id) {
    const uint8_t *p = message_text(id), *stack[4];
    int depth = 0, steps = 0, hidden = 0, table_bits = id & 0xC000;

    memset(&sLayout, 0, sizeof(sLayout));
    sLayout.id = id;
    sLayout.npages = 1;
    page()->nlines = 1;
    if (p == NULL) {
        snprintf(sLayout.pages[0].lines[0], MESSAGE_LINE_CHARS, "(message %04X)", id);
        return &sLayout;
    }
    while (steps++ < 4096) {
        uint8_t c = *p;

        switch (c) {
        case 0x00:   /* end, or back to the string around this one */
            if (depth == 0) {
                goto done;
            }
            p = stack[--depth];
            continue;
        case 0x01:
            new_line();
            p++;
            continue;
        case 0x02:
            new_page();
            p++;
            continue;
        case 0x03: case 0x07: case 0x09: case 0x0A: case 0x0C: case 0x10:   /* one operand */
            p += 2;
            continue;
        case 0x0B: case 0x19:   /* two */
            p += 3;
            continue;
        case 0x0E:   /* a choice: the options (0x0F) follow */
            sLayout.choice = 1;
            sLayout.choice_flags = p[1];
            p += 3;
            continue;
        case 0x0F:   /* an option, leading to message hi:lo (0xFFFF: none, the window closes) */
            if (sLayout.noptions < MESSAGE_OPTIONS) {
                MessageOption *o = &sLayout.options[sLayout.noptions++];

                o->page = sLayout.npages - 1;
                o->line = page()->nlines - 1;
                o->col = (int)strlen(page()->lines[o->line]);
                o->leads_to = p[1] << 8 | p[2];
            }
            put_text("  ");
            p += 3;
            continue;
        case 0x08:   /* a parameter (a system message the scripts chose) */
        case 0x13: { /* a name (not kept yet) */
            const uint8_t *s = c == 0x08 && p[1] < 4 ? sParams[p[1]] : NULL;

            if (s != NULL && depth < 4) {
                stack[depth++] = p + 2;
                p = s;
            } else {
                put_text(c == 0x08 ? "..." : "?");
                p += 2;
            }
            continue;
        }
        case 0x11: {   /* go on with message hi:lo (in the same table) */
            const uint8_t *s = message_text(table_bits | (p[1] << 8 | p[2]));

            if (s == NULL) {
                goto done;
            }
            p = s;
            continue;
        }
        case 0x15: {   /* system message 0x100 + hi:lo, inserted */
            const uint8_t *s = message_text(0x8100 + (p[1] << 8 | p[2]));

            if (s != NULL && depth < 4) {
                stack[depth++] = p + 3;
                p = s;
            } else {
                p += 3;
            }
            continue;
        }
        case 0x18:   /* furigana: the reading (not shown here) */
            hidden = 1;
            p++;
            continue;
        case 0x17:
            hidden = 0;
            p++;
            continue;
        case 0x04: case 0x0D: case 0x12: case 0x14: case 0x16: case 0x05: case 0x06:
            p++;
            continue;
        case 0x1A: case 0x1B: case 0x1C: case 0x1D:   /* a glyph of another font */
            if (!hidden) {
                put('?');
            }
            p += 2;
            continue;
        default:
            if (!hidden) {
                put(c >= 0x20 && c < 0x7F ? (char)c : '?');
            }
            p++;
        }
    }
done:
    for (int k = 0; k < sLayout.npages; k++) {   /* (a page's empty last lines: dropped) */
        MessagePage *pp = &sLayout.pages[k];

        while (pp->nlines > 1 && pp->lines[pp->nlines - 1][0] == 0) {
            pp->nlines--;
        }
    }
    /* (a page left with only an empty last line: drop it) */
    while (sLayout.npages > 1 && page()->nlines == 1 && page()->lines[0][0] == 0) {
        sLayout.npages--;
    }
    return &sLayout;
}
