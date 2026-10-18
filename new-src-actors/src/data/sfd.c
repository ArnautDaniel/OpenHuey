/* The movies' stream: see sfd.h (the demux as platform/movie.c plays it). */
#include "sfd.h"

#include <stdlib.h>
#include <string.h>

static int fill(SfdReader *r) {
    if (r->pos < r->len) {
        return 1;
    }
    r->len = (int)r->read(r->user, r->buf, sizeof(r->buf));
    r->pos = 0;
    return r->len > 0;
}

static int byte(SfdReader *r) { return fill(r) ? r->buf[r->pos++] : -1; }

static int bytes(SfdReader *r, uint8_t *dst, int n) {
    int got = 0;

    while (got < n && fill(r)) {
        int k = r->len - r->pos < n - got ? r->len - r->pos : n - got;

        if (dst != NULL) {
            memcpy(dst + got, r->buf + r->pos, (size_t)k);
        }
        r->pos += k;
        got += k;
    }
    return got;
}

/* a PES packet's header: its length (MPEG-1's stuffing and fields, or MPEG-2's) */
static int pes_header(const uint8_t *pes, int len) {
    int hdr = 0;

    if ((pes[0] & 0xC0) == 0x80) {   /* MPEG-2 */
        return 3 + pes[2];
    }
    while (hdr < len && pes[hdr] == 0xFF) {
        hdr++;
    }
    if (hdr < len && (pes[hdr] & 0xC0) == 0x40) {
        hdr += 2;
    }
    if (hdr < len && (pes[hdr] & 0xF0) == 0x20) {
        hdr += 5;
    } else if (hdr < len && (pes[hdr] & 0xF0) == 0x30) {
        hdr += 10;
    } else {
        hdr++;
    }
    return hdr;
}

int sfd_adx_playable(const uint8_t *h, size_t size) {
    return size >= 0x14 && h[0] == 0x80 && h[1] == 0 && h[4] == 3 && h[5] == 18 && h[6] == 4 && h[7] >= 1 && h[7] <= 2;
}

int sfd_next(SfdReader *r, int *id_out, const uint8_t **data, int *len_out) {
    for (;;) {
        int c = byte(r), id, len, hdr;

        if (c < 0) {
            return 0;
        }
        if (c != 0) {
            continue;
        }
        if ((c = byte(r)) != 0) {
            continue;
        }
        while ((c = byte(r)) == 0) {
        }
        if (c != 1) {
            continue;
        }
        if ((id = byte(r)) < 0) {
            return 0;
        }
        if (id == 0xB9) {   /* program end */
            return 0;
        }
        if (id == 0xBA) {   /* pack header */
            int b = byte(r);

            if ((b & 0xC0) == 0x40) {   /* MPEG-2 */
                bytes(r, NULL, 8);
                bytes(r, NULL, byte(r) & 7);
            } else {
                bytes(r, NULL, 7);
            }
            continue;
        }
        if (id < 0xBB) {
            continue;
        }
        len = byte(r) << 8;
        len |= byte(r);
        if (len <= 0 || bytes(r, r->pes, len) != len) {
            return 0;
        }
        if (id != 0xC0 && id != 0xE0) {
            continue;
        }
        hdr = pes_header(r->pes, len);
        if (hdr >= len) {
            continue;
        }
        *id_out = id;
        *data = r->pes + hdr;
        *len_out = len - hdr;
        return 1;
    }
}

/* ---- the split: the same walk over the file in memory, every byte but the sound packets'
 * copied (so the video's packets are read from the .mpg as they were) ---- */

int sfd_split(const uint8_t *s, size_t size, uint8_t **mpg, size_t *mpg_size, uint8_t **adx, size_t *adx_size) {
    size_t i = 0, kept = 0, from = 0, sound = 0;
    uint8_t *m = malloc(size), *a = malloc(size);

    if (m == NULL || a == NULL || size < 4 || memcmp(s, "\0\0\1\xBA", 4) != 0) {
        free(m);
        free(a);
        return 0;
    }
    while (i + 4 <= size) {
        size_t start, len;
        int id;

        if (s[i] != 0 || s[i + 1] != 0 || s[i + 2] != 1) {   /* (as the reader: up to a start code) */
            i++;
            continue;
        }
        start = i;
        id = s[i + 3];
        i += 4;
        if (id == 0xB9) {
            break;
        }
        if (id == 0xBA) {
            if (i < size && (s[i] & 0xC0) == 0x40) {
                i += 1 + 8;
                i += i < size ? 1u + (s[i] & 7u) : 0;
            } else {
                i += 1 + 7;
            }
            continue;
        }
        if (id < 0xBB || i + 2 > size) {
            continue;
        }
        len = (size_t)(s[i] << 8 | s[i + 1]);
        i += 2;
        if (len == 0 || i + len > size) {
            break;
        }
        if (id == 0xC0) {   /* the sound: out of the stream, into the ADX */
            int hdr = pes_header(s + i, (int)len);

            if ((size_t)hdr < len) {
                memcpy(a + sound, s + i + hdr, len - (size_t)hdr);
                sound += len - (size_t)hdr;
            }
            memcpy(m + kept, s + from, start - from);
            kept += start - from;
            from = i + len;
        }
        i += len;
    }
    memcpy(m + kept, s + from, size - from);
    kept += size - from;
    *mpg = m;
    *mpg_size = kept;
    *adx = a;
    *adx_size = sound;
    return 1;
}
