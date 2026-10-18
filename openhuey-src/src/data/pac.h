/* Room files (ST_xxx.PAC): 17 section offsets from the start of the file (0: no section).
 * What each section is, as far as the decomp shows (src/game/room.c RoomMgr_MakeCurrent):
 *
 *   0, 1, 16  navigation mesh          7   doors                13  room effects
 *   2         event script             8   more door data       14  obstacles
 *   3         the room mesh            9   texture bank         15  placed objects (with 12)
 *   4         lights                   10  text                 11  second texture bank
 *   5         cameras                  12  placed objects
 *   6         (kept, use unknown) */
#ifndef PAC_H
#define PAC_H

#include <stddef.h>
#include <stdint.h>

enum {
    PAC_NAV = 0, PAC_NAV2 = 1, PAC_EVENTS = 2, PAC_MESH = 3, PAC_LIGHTS = 4, PAC_CAMERAS = 5,
    PAC_SECTION6 = 6, PAC_DOORS = 7, PAC_DOORS2 = 8, PAC_TEXTURES = 9, PAC_TEXT = 10,
    PAC_TEXTURES2 = 11, PAC_PLACED = 12, PAC_EFFECTS = 13, PAC_OBSTACLES = 14, PAC_PLACED2 = 15,
    PAC_NAV3 = 16, PAC_SECTIONS = 17
};

typedef struct Pac {
    uint8_t *data;
    size_t size;
} Pac;

/* section i: its address and its size (up to the next section, or the end), or NULL */
const uint8_t *pac_section(const Pac *p, int i, size_t *size);
/* the path of room `id` ("ST_000/ST_003.PAC": rooms come in folders of eight) */
void pac_room_path(int id, char *out, size_t n);
/* the library's converted parts of a room are in a folder beside its PAC: "ST_000/ST_003.room/"
 * + `name` (docs/library-and-mods.md) */
void pac_room_part(int id, const char *name, char *out, size_t n);
/* the import's: the PAC less the sections in `drop` (bit i: section i - converted, so the library
 * holds them elsewhere), the others' bytes and sizes as they were (malloc'd); NULL if malformed */
uint8_t *pac_strip(const Pac *p, unsigned drop, size_t *size);
/* a section's bytes put back (the library's text of it read: data/roomtext.h), at the PAC's end
 * (16-byte aligned); 0 if out of memory */
int pac_attach(Pac *p, int section, const uint8_t *bytes, size_t n);

#endif
