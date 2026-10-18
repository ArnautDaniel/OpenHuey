/* The game's message tables as text (docs/library-and-mods.md): the library's form of the
 * message files (SUBSCR/MSG_*.BIN) and the rooms' text (a .PAC's section 10), and read back
 * into a table as the game's (u16 count, u16 offsets, the texts: game/messages.h).
 *
 *   \ comments, before the first message
 *   #12                 message 12; its text on the lines after, up to the next #id line
 *   Hello.{page}Hi.     a line break in the file is the game's (0x01); blank lines count
 *   #13 -               message 13: none (an offset outside the table)
 *
 * In the text (UTF-8): the game's font (SUBSCR/MSG_BASE.TEX) as the characters it draws - ASCII,
 * "®" (0x7B), the accented letters (0x80..0xBF: Latin-1's 0xC0..0xFF), "{#}" a "#" at a line's
 * start - its buttons and symbols as words in braces: {circle} {triangle} {square} {cross} {L1}
 * {L2} {R1} {R2} {select} {start} {mercury} {sun} {moon} {mars} {venus} {saturn} {jupiter}
 * {earth}; the control codes (include/text.h) too:
 *   {page} {colour n} {/colour} {param n} {wait n} {at n} {box w h} {speed n} {fast} {choice f x}
 *   {option $id} {close n} {then $id} {centre} {name n} {right} {system $id} {furigana}
 *   {reading} {/furigana} {child a b} {font1 $xx} {font2 $xx} {font3 $xx} {small $xx} {unused7 n}
 * and any other byte {$xx}. Numbers decimal or $hex. */
#ifndef MSGTEXT_H
#define MSGTEXT_H

#include <stddef.h>
#include <stdint.h>

#define MSGTEXT_FORMAT 1

/* the table as text (malloc'd, NUL-terminated); `about` its first comment line. NULL if it
 * isn't a table */
char *msgtext_write(const uint8_t *table, size_t size, const char *about, size_t *len);
/* the text read into a table (malloc'd); NULL if it can't be (what and where on stderr, `name`
 * the file's) */
uint8_t *msgtext_read(const char *text, size_t len, const char *name, size_t *size);
/* a message's length up to its end (the 0 that isn't a control code's operand), or -1 if it runs
 * past `max` */
long msgtext_length(const uint8_t *p, size_t max);

#endif
