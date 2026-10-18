/* The art gallery's pictures (artpic.h). */
#include "artpic.h"

#include <stdlib.h>
#include <string.h>

#include "tex.h"

uint8_t *artpic_bank(const uint8_t *file, size_t file_size, int picture, size_t *size, int *h) {
    size_t lines = picture < 21 ? 896 : 448, n = ARTPIC_W * lines, at, got = 0;
    uint8_t *bank, *img;
    uint32_t from;

    if (picture < 0 || picture >= 48 || file_size < 32) {
        return NULL;
    }
    memcpy(&from, file + (picture % 8) * 4, 4);
    if (from >= file_size || (bank = calloc(1, 32 + n + 1024)) == NULL) {
        return NULL;
    }
    img = bank + 32;   /* (the bank: its count, its entry, then the image and the CLUT) */
    at = from;
    while (got < n && at < file_size) {
        uint8_t c = file[at];

        if (c & 0x80) {
            size_t k = c & 0x7F;

            if (at + 1 >= file_size || got + k > n) {
                break;
            }
            memset(img + got, file[at + 1], k);
            got += k;
            at += 2;
        } else {
            if (at + 1 + c > file_size || got + c > n) {
                break;
            }
            memcpy(img + got, file + at + 1, c);
            got += c;
            at += 1 + c;
        }
    }
    if (got < n || at + 1024 > file_size) {
        free(bank);
        return NULL;
    }
    memcpy(img + n, file + at, 1024);
    {
        TexEntry e = {TEX_T8, TEX_CT32, {0, 0}, ARTPIC_W, (uint16_t)lines, (uint16_t)(n / 16), 1024 / 16, 16};
        uint32_t count = 1;

        memcpy(bank, &count, 4);
        memcpy(bank + 16, &e, sizeof(e));
    }
    *size = 32 + n + 1024;
    *h = (int)lines;
    return bank;
}
