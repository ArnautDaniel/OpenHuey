/* The game's messages: see messages.h. */
#include "messages.h"

#include "../core/files.h"
#include "../data/msgtext.h"
#include "../data/pac.h"
#include "engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *sBase, *sSub;   /* SUBSCR/MSG_BASE.BIN, MSG_SUB.BIN */
static size_t sBaseSize, sSubSize;
static uint8_t *sExtra;         /* a scene's own table (message_file: the ending's MSG_END.BIN) */
static size_t sExtraSize;
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

/* a table's text file read into a table (data/msgtext.h); NULL if there is none */
static uint8_t *read_text_file(const char *txt, size_t *size) {
    uint8_t *text, *table;
    size_t len;

    if ((text = files_read(txt, &len)) == NULL) {
        return NULL;
    }
    table = msgtext_read((const char *)text, len, txt, size);
    free(text);
    return table;
}

/* a table: the library's text of it ("SUBSCR/MSG_BASE.BIN" -> "SUBSCR/MSG_BASE.txt"), else the
 * file itself */
static uint8_t *read_text(const char *path, size_t *size) {
    char txt[300];
    const char *dot = strrchr(path, '.');

    snprintf(txt, sizeof(txt), "%.*s.txt", (int)(dot != NULL ? dot - path : (long)strlen(path)), path);
    return read_text_file(txt, size);
}

static uint8_t *read_table(const char *path, size_t *size) {
    uint8_t *t = read_text(path, size);

    return t != NULL ? t : files_read(path, size);
}

static void load_tables(void) {
    if (sBase == NULL) {
        sBase = read_table("SUBSCR/MSG_BASE.BIN", &sBaseSize);
    }
    if (sSub == NULL) {
        sSub = read_table("SUBSCR/MSG_SUB.BIN", &sSubSize);
    }
}

/* the room's own table: the library's ST_000/ST_003.room/messages.txt, read when the room is
 * first asked about; else the room's .PAC section */
static const uint8_t *room_table(size_t *size) {
    static int room = -2;
    static uint8_t *text;
    static size_t text_size;
    char path[64];

    if (gEngine.room.id != room) {
        room = gEngine.room.id;
        free(text);
        text = NULL;
        if (room >= 0) {
            pac_room_part(room, "messages.txt", path, sizeof(path));
            text = read_text_file(path, &text_size);
        }
    }
    if (text != NULL) {
        *size = text_size;
        return text;
    }
    return pac_section(&gEngine.room.pac, PAC_TEXT, size);
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
    if (id & 0x2000) {
        return text_in(sExtra, sExtraSize, id & 0x1FFF);
    }
    room = room_table(&size);
    return text_in(room, size, id);
}

int message_file(const char *path) {
    free(sExtra);
    sExtraSize = 0;
    sExtra = path != NULL ? read_table(path, &sExtraSize) : NULL;
    return sExtra != NULL;
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

void message_set_param_text(int slot, const char *text, int len) {   /* Msg_PrintfParam: the text itself */
    int i;

    if (slot < 0 || slot >= 4) {
        return;
    }
    for (i = 0; i < len && i < 63 && text[i] != 0; i++) {
        sParams[slot][i] = (uint8_t)text[i];
    }
    sParams[slot][i] = 0;
}

/* ---- laying a message out in pages of lines ---- */

static MessagePage *page(void) { return &sLayout.pages[sLayout.npages - 1]; }

/* typing (Task_StateType): the frame now (from the page's start), the speed, the colour */
static const uint8_t kGlyphFrames[8] = {0, 1, 4, 0, 0x60, 0, 0, 0};   /* Text_GlyphFrames (0x47B140) */
static int sFrame, sSpeed, sColour;

static void new_line(void) {
    MessagePage *p = page();

    if (p->nlines < MESSAGE_LINES) {
        p->nlines++;
    }
}

static void new_page(void) {
    page()->typed = sFrame;
    if (sLayout.npages < MESSAGE_PAGES) {
        sLayout.npages++;
        memset(page(), 0, sizeof(MessagePage));
        page()->nlines = 1;
    }
    sFrame = 0;   /* (Task_StateNextPage: typing starts again) */
}

static void put(char ch) {
    MessagePage *p = page();
    char *line = p->lines[p->nlines - 1];
    size_t n = strlen(line);

    if (n + 1 < MESSAGE_LINE_CHARS) {
        line[n] = ch;
        line[n + 1] = 0;
        p->at[p->nlines - 1][n] = (uint16_t)(sFrame < 0xFFFF ? sFrame : 0xFFFF);
        p->colour[p->nlines - 1][n] = (uint8_t)sColour;
    }
}

/* a glyph typed: then its speed's wait (and a frame to go on) */
static void typed(char ch) {
    int w = kGlyphFrames[sSpeed & 7];

    put(ch);
    if (w != 0) {
        sFrame += w + 1;
    }
}

int message_skip(const MessageLayout *l, int pg, int frame) {
    const MessagePage *p;
    int i;

    if (l == NULL || pg < 0 || pg >= l->npages) {
        return frame;
    }
    p = &l->pages[pg];
    for (i = 0; i < p->npauses; i++) {
        if (frame < p->pause[i]) {
            return p->pause[i];
        }
        if (frame == p->pause[i]) {
            return frame;   /* (pausing: Task_StatePause doesn't skip) */
        }
    }
    return frame < p->typed ? p->typed : frame;
}

static void put_text(const char *s) {
    while (*s) {
        put(*s++);
    }
}

#define BUTTON_NAMES 0x801C

const MessageLayout *message_layout(int id) {
    const uint8_t *p = message_text(id), *stack[4];
    int depth = 0, steps = 0, hidden = 0, table_bits = id & 0xC000;

    memset(&sLayout, 0, sizeof(sLayout));
    sFrame = sSpeed = sColour = 0;
    sLayout.close_after = -1;
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
        case 0x03:   /* a colour (the font's palette n) */
            sColour = p[1] & 0x7F;
            p += 2;
            continue;
        case 0x04:   /* back to the message's own */
            sColour = 0;
            p++;
            continue;
        case 0x0C:   /* the typing speed */
            sSpeed = p[1];
            p += 2;
            continue;
        case 0x0D:
            sSpeed = 0;
            p++;
            continue;
        case 0x09:   /* a pause of n frames (Task_StatePause: and one to go on) */
            if (page()->npauses < MESSAGE_PAUSES) {
                page()->pause[page()->npauses++] = (uint16_t)sFrame;
            }
            sFrame += p[1] + 1;
            p += 2;
            continue;
        case 0x10:   /* typing stops: closed n frames on (0: left up) */
            sLayout.close_after = p[1];
            goto done;
        case 0x07: case 0x0A:   /* one operand */
            p += 2;
            continue;
        case 0x0B: case 0x19:   /* two */
            p += 3;
            continue;
        case 0x0E:   /* a choice: the options (0x0F) follow */
            sLayout.choice = 1;
            sLayout.choice_flags = p[1];
            sSpeed = 0;
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
        case 0x13: { /* a button's name: slot n is system message $801C + n (the first controller
                      * layout's: SubScreen_SetLayout) */
            const uint8_t *s = c == 0x08 ? (p[1] < 4 ? sParams[p[1]] : NULL)
                             : p[1] < 6 ? message_text(BUTTON_NAMES + p[1]) : NULL;

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
        case 0x12: case 0x14: case 0x16: case 0x05: case 0x06:
            p++;
            continue;
        case 0x1A: case 0x1B: case 0x1C: case 0x1D:   /* a glyph of another font, or past 0xFF */
            if (!hidden) {
                typed('?');
            }
            p += 2;
            continue;
        default:
            if (!hidden) {
                typed(c >= 0x20 ? (char)c : '?');   /* (0x80 on: the font's own glyphs - the pad's buttons at 0xC0) */
            }
            p++;
        }
    }
done:
    page()->typed = sFrame;
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
