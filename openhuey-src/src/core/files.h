/* The game's data files (paths like "ST_000/ST_003.PAC"; "../" for the disc's own files beside
 * DATA.CVM, e.g. "../MODULES/LIBSD.IRX"), from the disc image or folder chosen at start-up - or
 * from a layer over it: the mods' library/ folders (docs/library-and-mods.md), the last added
 * looked in first. */
#ifndef FILES_H
#define FILES_H

#include <stddef.h>
#include <stdint.h>

#include "../platform/disc.h"

/* the whole file (malloc'd, NUL-terminated past the end), or NULL; size in *size */
uint8_t *files_read(const char *path, size_t *size);
int files_exist(const char *path);
DiscFile *files_open(const char *path);   /* (streamed: the movies) */

/* The language the game's words and the pictures with words in them are in. A library imported
 * from the European disc has the other languages' files beside the English ones, a code before
 * the extension: "SUBSCR/MSG_BASE.fr.txt" by "SUBSCR/MSG_BASE.txt", "SUBSCR/SUBBACK.de.texbank/"
 * by "SUBSCR/SUBBACK.texbank/" (docs/library-and-mods.md). Their readers ask for the language's
 * file first (files_localized), the English one when there is none. */
#define FILES_LANGUAGES 5
/* language i's code (0 "en", then "fr" "de" "it" "es" - the European disc's order), NULL past them */
const char *files_language_code(int i);
/* the language set (0 English, the default); set: an unknown code is English. Each change counts
 * (files_language_serial): what holds words or pictures read before it reads them again */
int files_language(void);
void files_language_set(const char *code);
unsigned files_language_serial(void);
/* the path with the language's code before its extension ("SUBSCR/SUBBACK.TEX" -> "SUBSCR/
 * SUBBACK.fr.TEX"): 1 if the language isn't English, 0 (and nothing written) if it is */
int files_localized(const char *path, char *out, size_t n);
/* the library has language i's words (English always) */
int files_language_present(int i);

/* a folder laid out as the library is (data/ in it, the disc's own files beside) over the
 * data; all of them taken away */
void files_layer_add(const char *dir);
/* the same in a zip: its `prefix`library/ ("" or "name/" - a zipped folder) */
struct Zip;
void files_layer_add_zip(const struct Zip *z, const char *prefix);
void files_layers_clear(void);
/* the file from the layers only (a mod's), NULL if none has it */
uint8_t *files_read_layers(const char *path, size_t *size);

#endif
