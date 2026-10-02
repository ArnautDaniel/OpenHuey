/* DMA on PC: walk a source chain (the tags the game builds) and hand the data to the GIF
 * (channel 2) or to VIF1 (channel 1: VIF codes, VU1 - see vu1.c).
 *
 * Addresses in tags are 28 bits: the native build is 32-bit with its data below 0x10000000. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gs_local.h"

static uint32_t *sWords;   /* the chain's data as one stream (VIF: words; GIF: quadwords) */
static size_t sCount, sCap;

static void push_words(const uint32_t *w, size_t n) {
    if (sCount + n > sCap) {
        sCap = (sCount + n) * 2 + 1024;
        sWords = realloc(sWords, sCap * 4);
    }
    memcpy(sWords + sCount, w, n * 4);
    sCount += n;
}

static void *host(uint32_t addr) { return (void *)(uintptr_t)(addr & 0x0FFFFFF0u); }

/* ---- chain walking ---- */

void dma_send(int chan, uint32_t tagaddr, int tte) {
    uint32_t stack[2], sp = 0, guard = 0;
    uint32_t addr = tagaddr;
    int done = 0;

    sCount = 0;
    while (!done && guard++ < 1000000) {
        const uint64_t *tag = host(addr);
        uint32_t qwc = (uint32_t)(tag[0] & 0xFFFF), id = (uint32_t)(tag[0] >> 28) & 7;
        uint32_t taddr = (uint32_t)(tag[0] >> 32) & 0x7FFFFFF0u;
        uint32_t data = addr + 16, next = 0;
        static const uint32_t pad[2];

        if (chan == 1) {
            push_words(pad, 2);   /* the DMA tag's half keeps the VIF stream quadword-aligned */
            if (tte) {
                push_words((const uint32_t *)tag + 2, 2);
            } else {
                push_words(pad, 2);
            }
        }
        switch (id) {
        case 0:   /* refe */
            data = taddr;
            done = 1;
            break;
        case 1:   /* cnt */
            next = data + qwc * 16;
            break;
        case 2:   /* next */
            next = taddr;
            break;
        case 3: case 4:   /* ref / refs */
            data = taddr;
            next = addr + 16;
            break;
        case 5:   /* call */
            if (sp < 2) {
                stack[sp++] = data + qwc * 16;
            }
            next = taddr;
            break;
        case 6:   /* ret */
            if (sp > 0) {
                next = stack[--sp];
            } else {
                done = 1;
            }
            break;
        case 7:   /* end */
            done = 1;
            break;
        }
        if (qwc) {
            push_words(host(data), qwc * 4);
        }
        addr = next;
    }
    if (chan == 2) {
        gs_gif((const uint64_t *)sWords, (uint32_t)(sCount / 4));
    } else if (chan == 1) {
        vif1_run(sWords, sCount);
    }
}
