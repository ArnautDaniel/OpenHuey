/* The game's messages (include/text.h in the decomp has the byte code). Three tables of u16
 * offsets then text: the system messages (SUBSCR/MSG_BASE.BIN; ids with bit 15), the second
 * table (SUBSCR/MSG_SUB.BIN; bit 14), a scene's own (message_file; bit 13) and the room's own
 * (PAC section 10). A message is ASCII
 * with control codes: 0x01 new line, 0x02 page, 0x0E a choice whose options (0x0F hi lo) each
 * lead to another message, 0x11 hi lo go on with another, 0x15 hi lo a system message inserted,
 * 0x08 n a parameter, colours, speeds, waits.
 *
 * Here a message is laid out as pages of plain lines with its options, and each character's
 * colour (0x03 n, 0x04 back) and the frame it is typed on (Task_StateType: 0x0C n the speed - a
 * glyph then waits Text_GlyphFrames[n] frames, 0x0D back to 0; 0x09 n a pause of n; 0x10 n: the
 * message closes n frames after it is typed, 0 never by a button); the window that shows it is
 * Forth's. 0x0A n places the box (Task_Layout: from the page it is on); 0x12 / 0x14 centre a line
 * or put it to the right (Task_DrawPage). */
#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

#define MESSAGE_PAGES 16
#define MESSAGE_LINES 16
#define MESSAGE_LINE_CHARS 96
#define MESSAGE_OPTIONS 8
#define MESSAGE_PAUSES 16

typedef struct MessagePage {
    char lines[MESSAGE_LINES][MESSAGE_LINE_CHARS];
    uint16_t at[MESSAGE_LINES][MESSAGE_LINE_CHARS];      /* the frame (from the page's start) each is typed on */
    uint8_t colour[MESSAGE_LINES][MESSAGE_LINE_CHARS];   /* its colour (the font's palette) */
    uint8_t align[MESSAGE_LINES];    /* the line's: 0 from the left, 1 centred (0x12), 2 to the right (0x14) */
    int nlines;
    int place;                       /* the box's place (0x0A n: preset n & 0x7F, bit 7 no box) from
                                      * this page on; -1 none given yet */
    int typed;                       /* the frame the page is typed out on (then the button) */
    uint16_t pause[MESSAGE_PAUSES];  /* the frames its pauses (0x09) start on: holding cancel types up to the next */
    int npauses;
} MessagePage;

typedef struct MessageOption {
    int page, line, col;   /* where its text is (col: characters into the line) */
    int leads_to;       /* the message it opens, 0xFFFF none */
} MessageOption;

typedef struct MessageLayout {
    int id;
    MessagePage pages[MESSAGE_PAGES];
    int npages;
    int choice, choice_flags;   /* a choice (0x0E and its flags byte) */
    int close_after;            /* 0x10 n: closes n frames after its last page is typed (0: never by
                                 * a button); -1 none */
    MessageOption options[MESSAGE_OPTIONS];
    int noptions;
} MessageLayout;

/* message `id`'s raw text (NULL: none) */
const uint8_t *message_text(int id);
/* the frame typing on `page` reaches after `frame`, holding cancel: its next pause, else its end */
int message_skip(const MessageLayout *l, int page, int frame);
/* laid out (the one layout: valid until the next call) */
const MessageLayout *message_layout(int id);
/* a table of a scene's own from a file (ids with bit 13; NULL: none): 1 loaded */
int message_file(const char *path);
/* parameter `slot` shows system message `id` (Msg_SetParamSystem) */
void message_set_param(int slot, int id);
/* parameter `slot` shows this text (Msg_PrintfParam) */
void message_set_param_text(int slot, const char *text, int len);

#endif
