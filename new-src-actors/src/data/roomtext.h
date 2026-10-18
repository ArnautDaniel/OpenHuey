/* A room's sections as text (docs/library-and-mods.md): the library's form of the PAC sections
 * the engine reads as plain records - ST_000/ST_003.room/<file> - and read back into the
 * section's bytes, as the game has them (attached to the room's PAC as it loads: data/pac.h).
 *
 *   lights.txt (section 4: src/game/lights.c Lights_TakeRoom)
 *     lights <count>                        the count as the section has it
 *     ambient <r g b>
 *     light <x y z w> <r g b> <intensity> <range> <u1 u2> <shadow>   (a light, <count> of them)
 *   cameras.txt (section 5: the camera setups)
 *     camera <eye x y z> <fov, degrees: 0 60> <target x y z> <w>   (each, to the one marked -1)
 *   both: tail <hex>                         what follows, as it is (the end mark, padding)
 *
 * A number is a float as the section has it, or #xxxxxxxx (a word's bits, when a float can't say
 * them: not a number). \ comments; blank lines are nothing. */
#ifndef ROOMTEXT_H
#define ROOMTEXT_H

#include <stddef.h>
#include <stdint.h>

#define ROOMTEXT_FORMAT 1

/* the file a section is written to ("lights.txt"), NULL if it isn't one of these */
const char *roomtext_file(int section);
/* the section as text (malloc'd), `about` its first comment; NULL if it isn't as this expects */
char *roomtext_write(int section, const uint8_t *sec, size_t size, const char *about, size_t *len);
/* the text read back into the section's bytes (malloc'd); NULL if it can't be (said, with `name`) */
uint8_t *roomtext_read(int section, const char *text, size_t len, const char *name, size_t *size);

#endif
