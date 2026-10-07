#include "pac.h"

#include <stdio.h>
#include <string.h>

static uint32_t offset(const Pac *p, int i) {
    uint32_t o;

    memcpy(&o, p->data + i * 4, 4);
    return o;
}

const uint8_t *pac_section(const Pac *p, int i, size_t *size) {
    uint32_t start, end = (uint32_t)p->size;
    int k;

    if (p->data == NULL || p->size < PAC_SECTIONS * 4 || i < 0 || i >= PAC_SECTIONS) {
        return NULL;
    }
    start = offset(p, i);
    if (start == 0 || start >= p->size) {
        return NULL;
    }
    for (k = 0; k < PAC_SECTIONS; k++) {   /* the nearest section after it */
        uint32_t o = offset(p, k);

        if (o > start && o < end) {
            end = o;
        }
    }
    if (size != NULL) {
        *size = end - start;
    }
    return p->data + start;
}

void pac_room_path(int id, char *out, size_t n) {
    snprintf(out, n, "ST_%03X/ST_%03X.PAC", id & ~7, id);
}
