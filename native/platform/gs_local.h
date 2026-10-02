/* Software GS (gs.c): internal interface for the platform layer. */
#ifndef GS_LOCAL_H
#define GS_LOCAL_H

#include <stdint.h>

enum {
    PSMCT32 = 0x00, PSMCT24 = 0x01, PSMCT16 = 0x02, PSMCT16S = 0x0A,
    PSMT8 = 0x13, PSMT4 = 0x14, PSMT8H = 0x1B, PSMT4HL = 0x24, PSMT4HH = 0x2C,
    PSMZ32 = 0x30, PSMZ24 = 0x31, PSMZ16 = 0x32, PSMZ16S = 0x3A
};

typedef struct GsVertex {
    int x, y;              /* 12.4 fixed point, window coordinates + offset */
    uint32_t z;
    float r, g, b, a, q, s, t, u, v, fog;
} GsVertex;

static inline int abs_i(int v) { return v < 0 ? -v : v; }

void gs_init(void);
int gs_selftest(void);
void gs_write_reg(uint32_t reg, uint64_t v);
uint32_t gs_gif(const uint64_t *qw, uint32_t n);
void gs_set_display(uint64_t pmode, uint64_t smode2, uint64_t dispfb, uint64_t display, uint64_t bgcolor);
void gs_display(uint32_t *out, int maxw, int maxh, int *w, int *h);

/* DMA (dma.c) */
void dma_send(int chan, uint32_t tag, int tte);

#endif
