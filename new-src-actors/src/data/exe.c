#include "exe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exe_load(Exe *e, const char *path) {
    FILE *fp = fopen(path, "rb");
    long n;

    memset(e, 0, sizeof(*e));
    if (fp == NULL) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    e->data = malloc((size_t)n);
    if (e->data == NULL || fread(e->data, 1, (size_t)n, fp) != (size_t)n || n < 0x34 ||
        memcmp(e->data, "\x7f" "ELF", 4) != 0) {
        fclose(fp);
        exe_free(e);
        return 0;
    }
    fclose(fp);
    e->size = (size_t)n;
    return 1;
}

void exe_free(Exe *e) {
    free(e->data);
    memset(e, 0, sizeof(*e));
}

/* through the program headers: where a loaded address comes from in the file */
const uint8_t *exe_at(const Exe *e, uint32_t vaddr, size_t n) {
    uint32_t phoff, i;
    uint16_t phnum;

    if (e->data == NULL) {
        return NULL;
    }
    memcpy(&phoff, e->data + 0x1C, 4);
    memcpy(&phnum, e->data + 0x2C, 2);
    for (i = 0; i < phnum && phoff + (i + 1) * 32 <= e->size; i++) {
        uint32_t ph[6];

        memcpy(ph, e->data + phoff + i * 32, sizeof(ph));   /* type, offset, vaddr, paddr, filesz, memsz */
        if (ph[0] == 1 && vaddr >= ph[2] && vaddr + n <= ph[2] + ph[4] && ph[1] + (vaddr - ph[2]) + n <= e->size) {
            return e->data + ph[1] + (vaddr - ph[2]);
        }
    }
    return NULL;
}
