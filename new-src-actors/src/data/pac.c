#include "pac.h"

#include <stdio.h>
#include <stdlib.h>
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

void pac_room_part(int id, const char *name, char *out, size_t n) {
    snprintf(out, n, "ST_%03X/ST_%03X.room/%s", id & ~7, id, name);
}

void pac_room_path(int id, char *out, size_t n) {
    snprintf(out, n, "ST_%03X/ST_%03X.PAC", id & ~7, id);
}

uint8_t *pac_strip(const Pac *p, unsigned drop, size_t *size) {
    uint32_t start[PAC_SECTIONS], len[PAC_SECTIONS], first = (uint32_t)p->size, at;
    uint8_t *out;
    int i, k;

    if (p->data == NULL || p->size < PAC_SECTIONS * 4 || (out = malloc(p->size)) == NULL) {
        return NULL;
    }
    for (i = 0; i < PAC_SECTIONS; i++) {   /* each section's extent: up to the next, as pac_section */
        size_t n = 0;

        start[i] = offset(p, i);
        len[i] = pac_section(p, i, &n) != NULL ? (uint32_t)n : 0;
        if (len[i] != 0 && start[i] < first) {
            first = start[i];
        }
    }
    memcpy(out, p->data, first);   /* (the offsets, and whatever is before the first section) */
    at = first;
    for (;;) {   /* the kept sections in their order in the file (one shared by two: once) */
        int next = -1;

        for (i = 0; i < PAC_SECTIONS; i++) {
            if (len[i] != 0 && !(drop >> i & 1) && start[i] >= first &&
                (next < 0 || start[i] < start[next])) {
                next = i;
            }
        }
        if (next < 0) {
            break;
        }
        memcpy(out + at, p->data + start[next], len[next]);
        for (k = 0; k < PAC_SECTIONS; k++) {
            if (k != next && start[k] == start[next] && len[k] != 0 && !(drop >> k & 1)) {
                memcpy(out + k * 4, &at, 4);
                len[k] = 0;
            }
        }
        memcpy(out + next * 4, &at, 4);
        at += len[next];
        len[next] = 0;
    }
    for (i = 0; i < PAC_SECTIONS; i++) {
        if (drop >> i & 1) {
            memset(out + i * 4, 0, 4);
        }
    }
    *size = at;
    return out;
}
