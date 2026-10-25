/* Reading .zip files (a zipped mod, docs/library-and-mods.md): their files stored or deflated,
 * read whole into memory. No zip64, no encryption. And writing one (a bug report's, to attach to
 * an issue). */
#ifndef ZIP_H
#define ZIP_H

#include <stddef.h>
#include <stdint.h>

typedef struct Zip Zip;

Zip *zip_open(const char *path);   /* NULL if it isn't a zip hga reads (the reason on stderr) */
void zip_close(Zip *z);

int zip_count(const Zip *z);
const char *zip_name(const Zip *z, int i);   /* "folder/file" ("/" between folders) */
/* the file of that name (case doesn't matter, "\" as "/"): its index, or -1 */
int zip_find(const Zip *z, const char *name);
/* file i, whole (malloc'd, NUL-terminated past its end), or NULL; its size in *size */
uint8_t *zip_read(const Zip *z, int i, size_t *size);

/* a zip at `path` of the files `names` (n of them) in the folder `dir`, each as "folder/name"
 * (deflated, or stored where that is no smaller); files not there are left out. 0 if it couldn't
 * be written (the reason on stderr) */
int zip_write(const char *path, const char *dir, const char *folder, const char *const *names, int n);

#endif
