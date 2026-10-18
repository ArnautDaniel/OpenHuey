/* Message tables as text: see msgtext.h. */
#include "msgtext.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Code {
    uint8_t byte;
    const char *word;
    int operands;   /* bytes after it */
    int id;         /* its operands a message id (hi lo): written $hex */
} Code;

static const Code kCodes[] = {
    {0x02, "page", 0, 0},     {0x03, "colour", 1, 0}, {0x04, "/colour", 0, 0},   {0x08, "param", 1, 0},
    {0x09, "wait", 1, 0},     {0x0A, "at", 1, 0},     {0x0B, "box", 2, 0},       {0x0C, "speed", 1, 0},
    {0x0D, "fast", 0, 0},     {0x0E, "choice", 2, 0}, {0x0F, "option", 2, 1},    {0x10, "close", 1, 0},
    {0x11, "then", 2, 1},     {0x12, "centre", 0, 0}, {0x13, "name", 1, 0},      {0x14, "right", 0, 0},
    {0x15, "system", 2, 1},   {0x16, "furigana", 0, 0}, {0x18, "reading", 0, 0}, {0x17, "/furigana", 0, 0},
    {0x19, "child", 2, 0},    {0x1A, "font1", 1, 2},  {0x1B, "font2", 1, 2},     {0x1C, "font3", 1, 2},
    {0x1D, "small", 1, 2},    {0x07, "unused7", 1, 0},
};   /* (operands as the game's reader takes them: src/game/text.c TextCursor_Step; id 2: a glyph, $xx) */
#define NCODES (int)(sizeof(kCodes) / sizeof(kCodes[0]))

static const char *const kGlyphs[] = {   /* 0xC0.. : the font's buttons and symbols */
    "circle", "triangle", "square", "cross", "L1", "L2", "R1", "R2", "select", "start",
    "mercury", "sun", "moon", "mars", "venus", "saturn", "jupiter", NULL, NULL, "earth",
};
#define NGLYPHS (int)(sizeof(kGlyphs) / sizeof(kGlyphs[0]))

/* the font's letters beyond ASCII as Unicode: 0x7B "®", 0x80..0xBF Latin-1's 0xC0..0xFF (but
 * the two it has no glyph for, "×" and "÷"); 0 none */
static unsigned unicode_of(uint8_t b) {
    if (b == 0x7B) {
        return 0xAE;
    }
    return b >= 0x80 && b <= 0xBF && b != 0x97 && b != 0xB7 ? b + 0x40u : 0;
}

static const Code *code_of(uint8_t b) {
    int i;

    for (i = 0; i < NCODES; i++) {
        if (kCodes[i].byte == b) {
            return &kCodes[i];
        }
    }
    return NULL;
}

long msgtext_length(const uint8_t *p, size_t max) {
    size_t i = 0;

    while (i < max && p[i] != 0) {
        const Code *c = code_of(p[i]);

        i += 1 + (c != NULL ? (size_t)c->operands : 0);
    }
    return i < max ? (long)i : -1;
}

/* ---- writing ---- */

typedef struct Out {
    char *s;
    size_t n, cap;
} Out;

static void add(Out *o, const char *fmt, ...) {
    va_list ap;
    int k;

    for (;;) {
        va_start(ap, fmt);
        k = vsnprintf(o->s != NULL ? o->s + o->n : NULL, o->s != NULL ? o->cap - o->n : 0, fmt, ap);
        va_end(ap);
        if (o->s != NULL && (size_t)k < o->cap - o->n) {
            o->n += (size_t)k;
            return;
        }
        o->cap = o->cap ? o->cap * 2 + (size_t)k : 65536;
        o->s = realloc(o->s, o->cap);
    }
}

static uint16_t u16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }

char *msgtext_write(const uint8_t *table, size_t size, const char *about, size_t *len) {
    Out o = {0};
    int n, id;

    if (table == NULL || size < 2 || (size_t)(u16(table) + 1) * 2 > size) {
        return NULL;
    }
    n = u16(table);
    add(&o, "\\ %s\n\\ hga-messages %d: #id, then its text (docs/library-and-mods.md, src/data/msgtext.h)\n", about,
        MSGTEXT_FORMAT);
    for (id = 0; id < n; id++) {
        size_t at = u16(table + 2 + id * 2), i;
        long length = at < size ? msgtext_length(table + at, size - at) : -1;
        int line_start = 1;

        if (length < 0) {
            add(&o, "#%d -\n", id);
            continue;
        }
        add(&o, "#%d\n", id);
        for (i = at; i < at + (size_t)length;) {
            uint8_t b = table[i];
            const Code *c = code_of(b);

            if (b == 0x01) {
                add(&o, "\n");
                line_start = 1;
                i++;
                continue;
            }
            if (c != NULL) {
                const uint8_t *op = table + i + 1;

                if (c->id == 1) {
                    add(&o, "{%s $%04X}", c->word, op[0] << 8 | op[1]);
                } else if (c->id == 2) {
                    add(&o, "{%s $%02X}", c->word, op[0]);
                } else if (c->operands == 2) {
                    add(&o, "{%s %d %d}", c->word, op[0], op[1]);
                } else if (c->operands == 1) {
                    add(&o, "{%s %d}", c->word, op[0]);
                } else {
                    add(&o, "{%s}", c->word);
                }
                i += 1 + (size_t)c->operands;
            } else if (unicode_of(b) != 0) {
                unsigned u = unicode_of(b);

                add(&o, "%c%c", 0xC0 | u >> 6, 0x80 | (u & 0x3F));
                i++;
            } else if (b >= 0xC0 && b - 0xC0 < NGLYPHS && kGlyphs[b - 0xC0] != NULL) {
                add(&o, "{%s}", kGlyphs[b - 0xC0]);
                i++;
            } else if (b == '#' && line_start) {
                add(&o, "{#}");
                i++;
            } else if (b >= 0x20 && b < 0x7F) {
                add(&o, "%c", b);
                i++;
            } else {
                add(&o, "{$%02X}", b);
                i++;
            }
            line_start = 0;
        }
        add(&o, "\n");
    }
    *len = o.n;
    return o.s;
}

/* ---- reading ---- */

typedef struct Bytes {
    uint8_t *b;
    size_t n, cap;
} Bytes;

static void byte(Bytes *t, uint8_t v) {
    if (t->n == t->cap) {
        t->cap = t->cap ? t->cap * 2 : 65536;
        t->b = realloc(t->b, t->cap);
    }
    t->b[t->n++] = v;
}

static int number(const char **p, long *v) {   /* decimal or $hex, after spaces */
    char *end;

    while (**p == ' ') {
        (*p)++;
    }
    *v = **p == '$' ? strtol(*p + 1, &end, 16) : strtol(*p, &end, 10);
    if (end == *p || (**p == '$' && end == *p + 1)) {
        return 0;
    }
    *p = end;
    return 1;
}

static int fail(const char *name, int line, const char *what) {
    fprintf(stderr, "messages: %s:%d: %s\n", name, line, what);
    return 0;
}

/* one line of a message's text into t */
static int text_line(Bytes *t, const char *s, const char *end, const char *name, int line) {
    while (s < end) {
        if ((unsigned char)*s >= 0x80) {   /* a letter of the font's beyond ASCII (two bytes in UTF-8) */
            unsigned u = s + 1 < end && ((unsigned char)*s & 0xE0) == 0xC0 ? ((unsigned char)s[0] & 0x1Fu) << 6 | ((unsigned char)s[1] & 0x3Fu) : 0;
            int b;

            for (b = 0x7B; b < 0xC0 && (u == 0 || unicode_of((uint8_t)b) != u); b++) {
            }
            if (b == 0xC0) {
                return fail(name, line, "a character the game's font hasn't (its own glyphs: {$xx})");
            }
            byte(t, (uint8_t)b);
            s += 2;
            continue;
        }
        if (*s != '{') {
            if ((unsigned char)*s < 0x20 || *s == 0x7B || (unsigned char)*s == 0x7F) {
                return fail(name, line, "a control character in the text");
            }
            byte(t, (uint8_t)*s++);
            continue;
        }
        {
            const char *close = memchr(s, '}', (size_t)(end - s)), *p;
            char word[24];
            size_t wl;
            int i;
            long v;

            if (close == NULL) {
                return fail(name, line, "a { without its }");
            }
            p = s + 1;
            for (wl = 0; p + wl < close && p[wl] != ' ' && wl < sizeof(word) - 1; wl++) {
                word[wl] = p[wl];
            }
            word[wl] = 0;
            p += wl;
            if (strcmp(word, "#") == 0) {
                byte(t, '#');
            } else if (word[0] == '$') {
                const char *q = word;

                if (!number(&q, &v) || *q != 0 || v < 0 || v > 255) {
                    return fail(name, line, "{$xx}: a byte, in hex");
                }
                byte(t, (uint8_t)v);
            } else {
                for (i = 0; i < NGLYPHS && (kGlyphs[i] == NULL || strcmp(kGlyphs[i], word) != 0); i++) {
                }
                if (i < NGLYPHS) {
                    byte(t, (uint8_t)(0xC0 + i));
                    s = close + 1;
                    continue;
                }
                for (i = 0; i < NCODES && strcmp(kCodes[i].word, word) != 0; i++) {
                }
                if (i == NCODES) {
                    return fail(name, line, "a {word} the messages don't have");
                }
                byte(t, kCodes[i].byte);
                if (kCodes[i].id == 1) {
                    if (!number(&p, &v) || v < 0 || v > 0xFFFF) {
                        return fail(name, line, "a message id ($hex) wanted");
                    }
                    byte(t, (uint8_t)(v >> 8));
                    byte(t, (uint8_t)v);
                } else {
                    int k;

                    for (k = 0; k < kCodes[i].operands; k++) {
                        if (!number(&p, &v) || v < 0 || v > 255) {
                            return fail(name, line, "a number 0..255 wanted");
                        }
                        byte(t, (uint8_t)v);
                    }
                }
                while (*p == ' ') {
                    p++;
                }
                if (p != close) {
                    return fail(name, line, "more in the {...} than its word takes");
                }
            }
            s = close + 1;
        }
    }
    return 1;
}

uint8_t *msgtext_read(const char *text, size_t len, const char *name, size_t *size) {
    Bytes body = {0}, out = {0};
    long *at = NULL, cap = 0, n = 0, id = -1, i;
    const char *s = text, *end = text + len;
    int line = 0, ok = 1, first_line = 1;

    while (s < end && ok) {
        const char *eol = memchr(s, '\n', (size_t)(end - s)), *le;

        eol = eol != NULL ? eol : end;
        le = eol > s && eol[-1] == '\r' ? eol - 1 : eol;
        line++;
        if (*s == '#' && le > s + 1 && isdigit((unsigned char)s[1])) {   /* #id */
            const char *p = s + 1;
            long v;

            if (id >= 0) {
                byte(&body, 0);
            }
            number(&p, &v);
            if (v != n) {
                ok = fail(name, line, "the messages' ids go 0, 1, 2 ... in order");
                break;
            }
            if (n == cap) {
                cap = cap ? cap * 2 : 1024;
                at = realloc(at, sizeof(long) * (size_t)cap);
            }
            while (p < le && *p == ' ') {
                p++;
            }
            at[n++] = p < le && *p == '-' ? -1 : (long)body.n;   /* (#id -: none) */
            id = at[n - 1] < 0 ? -1 : v;
            first_line = 1;
        } else if (id >= 0) {
            if (!first_line) {
                byte(&body, 0x01);
            }
            ok = text_line(&body, s, le, name, line);
            first_line = 0;
        } else if (n == 0 && *s != '\\' && le > s) {
            ok = fail(name, line, "text before the first #id");
        }
        s = eol + 1;
    }
    if (ok && id >= 0) {
        byte(&body, 0);
    }
    if (ok && (n > 0xFFFF || (size_t)(n + 1) * 2 + body.n > 0xFFFF)) {
        ok = fail(name, line, "too big for a table (64 KB)");
    }
    if (!ok) {
        free(at);
        free(body.b);
        return NULL;
    }
    byte(&out, (uint8_t)n);
    byte(&out, (uint8_t)(n >> 8));
    for (i = 0; i < n; i++) {   /* (none: past the table's end) */
        long o = at[i] < 0 ? 0xFFFF : (n + 1) * 2 + at[i];

        byte(&out, (uint8_t)o);
        byte(&out, (uint8_t)(o >> 8));
    }
    for (i = 0; i < (long)body.n; i++) {
        byte(&out, body.b[i]);
    }
    free(at);
    free(body.b);
    *size = out.n;
    return out.b;
}
