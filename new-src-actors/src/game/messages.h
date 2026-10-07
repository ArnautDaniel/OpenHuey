/* The game's messages (include/text.h in the decomp has the byte code). Three tables of u16
 * offsets then text: the system messages (SUBSCR/MSG_BASE.BIN; ids with bit 15), the second
 * table (SUBSCR/MSG_SUB.BIN; bit 14) and the room's own (PAC section 10). A message is ASCII
 * with control codes: 0x01 new line, 0x02 page, 0x0E a choice whose options (0x0F hi lo) each
 * lead to another message, 0x11 hi lo go on with another, 0x15 hi lo a system message inserted,
 * 0x08 n a parameter, colours, speeds, waits.
 *
 * Here a message is laid out as pages of plain lines (colours, speeds and waits dropped), with
 * its options; the window that shows it is Forth's. */
#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

#define MESSAGE_PAGES 16
#define MESSAGE_LINES 8
#define MESSAGE_LINE_CHARS 96
#define MESSAGE_OPTIONS 8

typedef struct MessagePage {
    char lines[MESSAGE_LINES][MESSAGE_LINE_CHARS];
    int nlines;
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
    MessageOption options[MESSAGE_OPTIONS];
    int noptions;
} MessageLayout;

/* message `id`'s raw text (NULL: none) */
const uint8_t *message_text(int id);
/* laid out (the one layout: valid until the next call) */
const MessageLayout *message_layout(int id);
/* parameter `slot` shows system message `id` (Msg_SetParamSystem) */
void message_set_param(int slot, int id);

#endif
