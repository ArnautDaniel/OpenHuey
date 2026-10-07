/* The game's executable (SLUS_210.75): some of the game's tables live only there (the door
 * table, the room table). They are read from the player's copy at start-up, by address. */
#ifndef EXE_H
#define EXE_H

#include <stddef.h>
#include <stdint.h>

typedef struct Exe {
    uint8_t *data;
    size_t size;
} Exe;

/* load and check it is the expected ELF; 0 if not */
int exe_load(Exe *e, const char *path);
void exe_free(Exe *e);
/* n bytes at a PS2 address, or NULL if it isn't in the file */
const uint8_t *exe_at(const Exe *e, uint32_t vaddr, size_t n);

#endif
