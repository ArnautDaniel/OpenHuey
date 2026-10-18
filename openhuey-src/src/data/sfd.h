/* The game's movies (CRI Sofdec, .SFD): an MPEG program stream - MPEG-1 / 2 video (stream 0xE0)
 * and the sound as an ADX stream (0xC0) - and the library's form of them (docs/library-and-mods.md):
 * X.SFD -> X.mpg, the same stream less its sound packets (the video's packets byte for byte: a
 * plain MPEG file), and X.flac, its sound (data/musicsrc.h). */
#ifndef SFD_H
#define SFD_H

#include <stddef.h>
#include <stdint.h>

#define SFD_PES 0x10000

/* reading a stream: `read` fills buf with up to n bytes, how many (0 at its end) */
typedef struct SfdReader {
    size_t (*read)(void *user, uint8_t *buf, size_t n);
    void *user;
    uint8_t buf[0x10000];
    int pos, len;
    uint8_t pes[SFD_PES];
} SfdReader;

/* the next video (0xE0) or sound (0xC0) packet's payload: its stream in *id, the payload at
 * *data (in the reader, till the next call), its length; 0 at the stream's end */
int sfd_next(SfdReader *r, int *id, const uint8_t **data, int *len);

/* the sound's ADX header as the player takes it (4-bit, 18-byte blocks, 1 or 2 channels): 1, or
 * 0 - a movie with other sound plays silent, so its library form has none */
int sfd_adx_playable(const uint8_t *adx, size_t size);

/* the import's: the SFD split - the stream less its sound packets (*mpg) and the sound's ADX
 * bytes as they come (*adx); both malloc'd. 0 if it isn't a program stream */
int sfd_split(const uint8_t *sfd, size_t size, uint8_t **mpg, size_t *mpg_size, uint8_t **adx, size_t *adx_size);

#endif
