/* A bank of textures, wherever it comes from (docs/library-and-mods.md): a .TEX bank in memory
 * (a room's .PAC section, a file not converted), or the library's converted form - the bank's
 * .TEX path with ".texbank/" for ".TEX": bank.txt and a PNG for each texture (RGBA, the pixels the
 * game decodes: alpha 0..255), 16-colour ones that switch palettes a PNG for each palette that
 * differs. Read through files_read: a mod's PNGs over the library's.
 *
 *   bank.txt:  hga-texbank 1
 *              textures <n>
 *              texture <i> <w> <h> <psm> <images>      (images: its PNGs, i.png then i-1.png ...)
 *              palettes <i> <image for palette 0> ... <for 31>   (only with more than one image)
 */
#ifndef TEXBANK_H
#define TEXBANK_H

#include <stddef.h>
#include <stdint.h>

#define TEXBANK_FORMAT 1

typedef struct TexBank TexBank;

/* the bank at `path` ("FOLDER/NAME.TEX"): its converted form if there is one, else the file;
 * NULL if neither (or the file isn't a bank) */
TexBank *texbank_load(const char *path);
/* a .TEX bank in memory (kept by the caller while the TexBank is in use); NULL if it isn't one */
TexBank *texbank_wrap(const uint8_t *bank, size_t size);
/* ... taken: freed with the TexBank (NULL - and `bank` freed - if it isn't one) */
TexBank *texbank_take(uint8_t *bank, size_t size);
/* the bank at `path` ("ST_000/ST_003.room/textures.TEX": a room's): its converted form if there is
 * one, else `bank` in memory (a mod's PNGs over it, as over a file not converted); NULL if neither */
TexBank *texbank_load_or_wrap(const char *path, const uint8_t *bank, size_t size);
void texbank_free(TexBank *b);

int texbank_count(const TexBank *b);
/* texture i's size and its original pixel format (TEX_*): 0 if there is no texture i */
int texbank_info(const TexBank *b, int i, int *w, int *h, int *psm);
/* texture i decoded to RGBA8 (w*h*4 bytes) in palette `csa` (0..31, a 16-colour texture's; -1
 * the first): 1 on success */
int texbank_rgba(const TexBank *b, int i, int csa, uint8_t *rgba);

/* the import's conversion: the .TEX bank into the folder `dir` (made; ends with "/"), each file
 * written told to `wrote` (its name in the folder, its size); 0 if it isn't a bank or a file
 * can't be written */
typedef void (*TexbankWrote)(void *user, const char *name, long size);
int texbank_convert(const uint8_t *bank, size_t size, const char *dir, TexbankWrote wrote, void *user);

#endif
