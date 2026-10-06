/* VRAM / texture manager (system +0x30CF40, vtable 0x46B050, global gVram): hands out
 * areas of the PS2's 4 MB of video memory (the renderer's layers, textures, CLUTs). */
#include "common.h"
#include "game.h"


/* allocation entry (0x12 bytes, 64 at +0x98) */
typedef struct VramEntry {
    /* 0x00 */ u8 used;
    /* 0x01 */ u8 locked;    /* kept by +0x20 */
    /* 0x02 */ s16 w;
    /* 0x04 */ s16 h;
    /* 0x06 */ s16 psm;      /* 0xFF: CLUT only */
    /* 0x08 */ s16 page;     /* first page (texture pages, or upper-bit pages for 8/4-bit H formats) */
    /* 0x0A */ s16 npages;
    /* 0x0C */ s16 cpsm;     /* CLUT format, 0xFF none */
    /* 0x0E */ s16 cregion;  /* CLUT region (page from 0xFC000) */
    /* 0x10 */ s16 cslot;    /* slot in the region */
} VramEntry;

/* +0xC init: 8 free regions, 64 empty entries */
void func_001C2970(u8 *v) {
    VramEntry *e = (VramEntry *)(v + 0x98);
    s32 i;

    for (i = 0; i < 8; i++) {
        AT(v, 4 + i * 8, s16) = 0xFF;
        AT(v, 6 + i * 8, s16) = 0;
        AT(v, 8 + i * 8, s16) = 0;
        AT(v, 10 + i * 8, s16) = 0;
    }
    AT(v, 0x44, s32) = 0;
    AT(v, 0x48, s32) = 0;
    AT(v, 0x4C, s32) = 0;
    AT(v, 0x50, s32) = 0;
    for (i = 0; i < 64; i++, e++) {
        e->used = 0;
        e->locked = 0;
        e->w = 0;
        e->h = 0;
        e->psm = 0xFF;
        e->page = 0;
        e->npages = 0;
        e->cpsm = 0xFF;
        e->cregion = 0;
        e->cslot = 0;
    }
    AT(v, 0x518, s32) = 0;
}

/* CLUT region (8 at +4): CLUT format (0xFF: free), which slots are taken, how many, of how many
 * (16 for 16-bit CLUTs, 8 for 32-bit) */
typedef struct VramClutRegion {
    u16 psm;
    u16 mask;
    u16 count;
    u16 capacity;
} VramClutRegion;

#define VRAM_CLUT_REGION(v, i) ((VramClutRegion *)((v) + 4) + (i))
#define VRAM_PAGEMAP(v) ((u32 *)((v) + 0x44))     /* 1 bit per page */
#define VRAM_PAGEMAP_HI(v) ((u32 *)((v) + 0x54))  /* 2 bits per page: 8 / 4-bit formats in the
                                                   * unused upper bits of 24-bit pages */
#define PSM_NONE 0xFF
#define PSMT8H 0x1B
#define PSMT4HL 0x24
#define PSMT4HH 0x2C

extern s32 func_001C0AD0(u8 *v, s32 psm, s32 w, s32 h);        /* allocate pages (s16, -1) */
extern s32 func_001C08B0(u8 *v, u16 *psm, s32 w, s32 h);       /* ... in 24-bit pages' upper bits */
extern s32 func_001BF8F0(u8 *v, u32 addr, u16 *psm, s32 w, s32 h);   /* ... at a given address */
extern s32 func_001C0D40(u8 *v, s32 clutpsm);                  /* allocate a CLUT slot (s16, -1) */
extern u32 func_001C04B0(u8 *v, s32 psm, u32 w, u32 h);        /* size in pages */

/* +0x18 allocate a w x h texture of format psm (0xFF: CLUT only) at `addr` (< 0: anywhere), with
 * a CLUT of format clutpsm (0xFF: none; 0 / 2 for indexed formats). Returns the entry, -1 if
 * it doesn't fit (whatever was taken is given back). */
s32 func_001C1E00(u8 *v, s32 addr, s32 psm_, s32 w, s32 h, s32 clutpsm_) {
    u16 psm = psm_;
    u16 clutpsm = clutpsm_;
    VramEntry *e;
    s32 i, pos, clut;
    u32 p, end;

    switch (psm) {
    case 0x00: case 0x01: case 0x02:   /* 32 / 24 / 16-bit: no CLUT */
        if (clutpsm != PSM_NONE) {
            return -1;
        }
        break;
    case PSM_NONE: case PSMT8H: case PSMT4HH: case PSMT4HL: case 0x13: case 0x14:
        if (clutpsm != 2 && clutpsm != 0) {
            return -1;
        }
        break;
    default:
        return -1;
    }
    e = (VramEntry *)(v + 0x98);
    for (i = 0; i < 64; i++, e++) {
        if (e->used) {
            continue;
        }
        pos = -1;   /* (the PS2 code leaves this unset for CLUT-only allocations) */
        if (addr >= 0) {
            pos = (s16)func_001BF8F0(v, addr, &psm, w, h);
        } else if (psm == PSMT4HL || psm == PSMT4HH || psm == PSMT8H) {
            pos = (s16)func_001C08B0(v, &psm, w, h);
        } else if (psm != PSM_NONE) {
            pos = (s16)func_001C0AD0(v, psm, w, h);
        }
        clut = (s16)func_001C0D40(v, clutpsm);
        if ((psm == PSM_NONE || pos >= 0) && (clutpsm == PSM_NONE || clut >= 0)) {
            e->used = 1;
            e->psm = psm;
            e->page = pos;
            e->w = w;
            e->h = h;
            e->npages = func_001C04B0(v, psm, w, h);
            e->cpsm = clutpsm_;
            e->cregion = clut >> 4;
            e->cslot = clut & 0xF;
            return i;
        }
        /* give back what was taken */
        if (clut != -1) {
            VramClutRegion *rg = VRAM_CLUT_REGION(v, clut >> 4);

            rg->mask &= ~(1 << (clut & 0xF));
            if (--rg->count == 0) {
                rg->psm = 0xFF;
                rg->mask = 0;
                rg->count = 0;
                rg->capacity = 0;
            }
        }
        if (pos != -1) {
            end = pos + (u16)e->npages;
            switch (psm) {
            case PSMT8H:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP_HI(v)[p * 2 >> 5] &= ~(3 << (p * 2 & 0x1F));
                }
                break;
            case PSMT4HL:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP_HI(v)[p * 2 >> 5] &= ~(1 << (p * 2 & 0x1F));
                }
                break;
            case PSMT4HH:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP_HI(v)[p * 2 >> 5] &= ~(2 << (p * 2 & 0x1F));
                }
                break;
            default:
                for (p = pos; p < end; p++) {
                    VRAM_PAGEMAP(v)[p >> 5] &= ~(1 << (p & 0x1F));
                }
                break;
            }
        }
        return -1;
    }
    return -1;
}

/* +0x14 allocate anywhere */
s32 func_001C2930(VObject *v, s32 psm, s32 w, s32 h, s32 clutpsm) {
    return VCALL(v, 0x18, s32 (*)(VObject *, s32, s32, s32, s32, s32))(v, -1, psm, w, h, clutpsm);
}

/* +0x10 reset (init again) */
void func_001C2960(VObject *v) {
    VCALL(v, 0xC, void (*)(VObject *))(v);
}

/* +0x24 keep entry `id` resident (not freed by +0x20) */
void func_001C1460(u8 *v, s32 id) {
    VramEntry *e = (VramEntry *)(v + 0x98) + id;

    if (e->used) {
        e->locked = 1;
    }
}

/* +0x1C free entry `id`: its pages (8 / 4-bit H formats: their upper bits, both halves or one;
 * else the page bits), then its CLUT slot (the region emptied with its last) */
void func_001C1520(u8 *v, s32 id) {
    VramEntry *e;
    u32 p, end;

    if (id < 0) {
        return;
    }
    e = (VramEntry *)(v + 0x98) + id;
    if (!e->used) {
        return;
    }
    e->used = 0;
    e->locked = 0;
    switch ((u16)e->psm) {
    case PSMT4HH:
    case PSMT4HL:
    case PSMT8H: {
        u32 bits = (u16)e->psm == PSMT4HH ? 2 : (u16)e->psm == PSMT4HL ? 1 : 3;

        if (e->page == -1) {
            break;
        }
        end = e->page + (u16)e->npages;
        for (p = e->page; p < end; p++) {
            VRAM_PAGEMAP_HI(v)[(p * 2) >> 5] &= ~(bits << ((p * 2) & 0x1F));
        }
        break;
    }
    default:
        if (e->page == -1) {
            break;
        }
        end = e->page + (u16)e->npages;
        for (p = e->page; p < end; p++) {
            VRAM_PAGEMAP(v)[p >> 5] &= ~(1u << (p & 0x1F));
        }
        break;
    }
    if ((u16)e->cpsm != PSM_NONE) {
        s16 c = (s16)(e->cregion * 16 | (u16)e->cslot);

        if (c != -1) {
            VramClutRegion *r = VRAM_CLUT_REGION(v, c >> 4);

            r->mask &= ~(1 << (c & 0xF));
            if (--r->count == 0) {
                r->psm = PSM_NONE;
                r->mask = 0;
                r->count = 0;
                r->capacity = 0;
            }
        }
    }
}

/* +0x20 free every entry that isn't kept resident (+0x1C) */
void func_001C14A0(VObject *v) {
    VramEntry *e = (VramEntry *)((u8 *)v + 0x98);
    u32 i;

    for (i = 0; i < 64; i++, e++) {
        if (e->used && !e->locked) {
            VCALL(v, 0x1C, void (*)(VObject *, u32))(v, i);
        }
    }
}

/* Allocate a CLUT slot of format clutpsm (0 / 2): region * 16 + slot, or -1. */
s32 func_001C0D40(u8 *v, s32 clutpsm_) {
    u16 clutpsm = clutpsm_;
    VramClutRegion *rg;
    u32 i, j;

    if (clutpsm != 2 && clutpsm != 0) {
        return -1;
    }
    for (i = 0, rg = VRAM_CLUT_REGION(v, 0); i < 8; i++, rg++) {
        if (rg->psm != 0xFF && rg->count < rg->capacity && clutpsm == rg->psm) {
            for (j = 0; j < rg->capacity; j++) {
                if (!(rg->mask & (1 << j))) {
                    rg->mask |= 1 << j;
                    rg->count++;
                    return (s16)(i * 16 + j);
                }
            }
        }
    }
    for (i = 0, rg = VRAM_CLUT_REGION(v, 0); i < 8; i++, rg++) {
        if (rg->psm == 0xFF) {
            rg->psm = clutpsm_;
            rg->mask = 1;
            rg->count = 1;
            rg->capacity = clutpsm == 2 ? 16 : 8;
            return (s16)(i * 16);
        }
    }
    return -1;
}

/* Size of a w x h texture of format psm in pages (a page is 8 KB: 64x32 at 32 bits, 64x64 at 16,
 * 128x64 at 8, 128x128 at 4); 0 for other formats. */
u32 func_001C04B0(u8 *v, s32 psm, u32 w, u32 h) {
    u32 pw, ph;

    switch ((u16)psm) {
    case 0x00: case 0x01: case PSMT8H: case PSMT4HH: case PSMT4HL:
        pw = 64;
        ph = 32;
        break;
    case 0x02:
        pw = 64;
        ph = 64;
        break;
    case 0x13:
        pw = 128;
        ph = 64;
        break;
    case 0x14:
        pw = 128;
        ph = 128;
        break;
    default:
        return 0;
    }
    return (w + pw - 1) / pw * ((h + ph - 1) / ph);
}

#define VRAM_HI_PAGES 0x110   /* pages below 0x88000 (the 24-bit frame buffers' upper bits) */
#define VRAM_PAGES 0x78       /* texture pages from 0xC0000 */
#define HI_FREE(v, p, m) (!(VRAM_PAGEMAP_HI(v)[(u32)(p) * 2 >> 5] & ((m) << ((u32)(p) * 2 & 0x1F))))
#define HI_TAKE(v, p, m) (VRAM_PAGEMAP_HI(v)[(u32)(p) * 2 >> 5] |= (m) << ((u32)(p) * 2 & 0x1F))

/* First fit of n texture pages; marks them. */
s32 func_001C0AD0(u8 *v, s32 psm, s32 w, s32 h) {
    s32 n = func_001C04B0(v, psm, w, h);
    s32 start = -1, left = 0;
    u32 p, end;

    if (n == 0) {
        return -1;
    }
    for (p = 0; p < VRAM_PAGES; p++) {
        if (VRAM_PAGEMAP(v)[p >> 5] & (1 << (p & 0x1F))) {
            start = -1;
            continue;
        }
        if (start < 0) {
            left = n;
            start = p;
        }
        if (--left == 0) {
            end = start + n;
            for (p = (u16)start; p < end; p++) {
                VRAM_PAGEMAP(v)[p >> 5] |= 1 << (p & 0x1F);
            }
            return (s16)start;
        }
    }
    return -1;
}

/* First fit of n upper-bit pages with bits `m` free (not marked). */
s32 func_001C0570(u8 *v, s32 n, s32 m) {
    s32 start = -1, left = 0;
    u32 p;

    if (n == 0) {
        return -1;
    }
    for (p = 0; p < VRAM_HI_PAGES; p++) {
        if (!HI_FREE(v, p, m)) {
            start = -1;
            continue;
        }
        if (start < 0) {
            left = (u16)n;
            start = p;
        }
        if (--left == 0) {
            return (s16)start;
        }
    }
    return -1;
}

/* First fit of n upper-bit pages with both halves free (8-bit); marks them. */
s32 func_001C0610(u8 *v, s32 n) {
    s32 start = -1, left = 0;
    u32 p, end;

    if (n == 0) {
        return -1;
    }
    for (p = 0; p < VRAM_HI_PAGES; p++) {
        if (!HI_FREE(v, p, 2) || !HI_FREE(v, p, 1)) {
            start = -1;
            continue;
        }
        if (start < 0) {
            left = (u16)n;
            start = p;
        }
        if (--left == 0) {
            end = start + (u16)n;
            for (p = start; p < end; p++) {
                HI_TAKE(v, p, 3);
            }
            return (s16)start;
        }
    }
    return -1;
}

/* Upper-bit pages for a 4 / 8-bit texture: 8-bit takes both halves (PSMT8H); 4-bit takes the
 * lower half (PSMT4HL) or the upper (PSMT4HH), whichever fits first. *psm is set to the choice. */
s32 func_001C08B0(u8 *v, u16 *psm, s32 w, s32 h) {
    u32 n = func_001C04B0(v, *psm, w, h) & 0xFFFF;
    s32 lo, hi, p;

    if (n == 0) {
        return -1;
    }
    switch (*psm) {
    case PSMT8H:
        return func_001C0610(v, n);
    case PSMT4HL:
    case PSMT4HH:
        lo = (s16)func_001C0570(v, n, 1);
        hi = (s16)func_001C0570(v, n, 2);
        if (lo != -1 && (hi == -1 || hi >= lo)) {
            *psm = PSMT4HL;
            for (p = lo; (u32)p < lo + n; p++) {
                HI_TAKE(v, p, 1);
            }
            return lo;
        }
        if (hi != -1) {
            *psm = PSMT4HH;
            for (p = hi; (u32)p < hi + n; p++) {
                HI_TAKE(v, p, 2);
            }
            return hi;
        }
        return -1;
    }
    return -1;
}

/* Pages at VRAM byte address `addr`: below 0x88000 upper-bit pages (4-bit: lower half if free,
 * else upper; 8-bit: both), 0xC0000..0xFC000 texture pages. -1 if taken or elsewhere. */
s32 func_001BF8F0(u8 *v, u32 addr, u16 *psm, s32 w, s32 h) {
    s32 n, start, okLo, okHi;
    u32 p, end;

    if (addr < 0x88000) {
        switch (*psm) {
        case PSMT4HL: case PSMT4HH: case 0x14:
            *psm = PSMT4HH;
            n = func_001C04B0(v, *psm, w, h);
            start = (s16)(addr >> 11);
            end = start + n;
            okLo = 1;
            for (p = start; p < end; p++) {
                if (!HI_FREE(v, p, 1) || p >= VRAM_HI_PAGES) {
                    okLo = 0;
                    break;
                }
            }
            okHi = 1;
            for (p = start; p < end; p++) {
                if (!HI_FREE(v, p, 2) || p >= VRAM_HI_PAGES) {
                    okHi = 0;
                    break;
                }
            }
            if (okLo) {
                *psm = PSMT4HL;
                if (start != -1) {
                    for (p = start; p < (u32)(start + n); p++) {
                        HI_TAKE(v, p, 1);
                    }
                }
                return start;
            }
            if (okHi) {
                *psm = PSMT4HH;
                if (start != -1) {
                    for (p = start; p < (u32)(start + n); p++) {
                        HI_TAKE(v, p, 2);
                    }
                }
                return start;
            }
            return -1;
        case 0x13: case PSMT8H:
            *psm = PSMT8H;
            n = func_001C04B0(v, *psm, w, h);
            start = (s16)(addr >> 11);
            end = start + n;
            for (p = start; p < end; p++) {
                if (!HI_FREE(v, p, 3) || p >= VRAM_HI_PAGES) {
                    return -1;
                }
            }
            if (start != -1) {
                for (p = start; p < (u32)(start + n); p++) {
                    HI_TAKE(v, p, 3);
                }
            }
            return start;
        }
        return -1;
    }
    if (addr >= 0xC0000 && addr < 0xFC000) {
        n = func_001C04B0(v, *psm, w, h);
        start = (s16)((addr - 0xC0000) >> 11);
        end = start + n;
        for (p = start; p < end; p++) {
            if ((VRAM_PAGEMAP(v)[p >> 5] & (1 << (p & 0x1F))) || p >= VRAM_PAGES) {
                return -1;
            }
        }
        if (start != -1) {
            for (p = start; p < (u32)(start + n); p++) {
                VRAM_PAGEMAP(v)[p >> 5] |= 1 << (p & 0x1F);
            }
        }
        return start;
    }
    return -1;
}

#define VRAM_ENTRY(v, id) ((VramEntry *)((v) + 0x98) + (id))

/* +0x6C CLUT address of entry `id` (64-word blocks): CLUT pages from 0xFC000, 16 slots per page
 * for 16-bit CLUTs, 8 for 32-bit */
u32 func_001BFDB0(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return ((e->cregion << 11) + 0xFC000 + (u32)((u16)e->cslot << 11) / ((u16)e->cpsm == 2 ? 16 : 8)) >> 6;
}

/* +0x68 texture address of entry `id` (64-word blocks) */
u32 func_001BFE10(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);
    u16 psm = e->psm;

    if (psm != PSMT8H && psm != PSMT4HL && psm != PSMT4HH) {
        return (u32)((e->page << 11) + 0xC0000) >> 6;
    }
    return (u32)(e->page << 11) >> 6;
}

/* +0x64 / +0x60 height / width of entry `id` */
u16 func_001BFE80(u8 *v, s32 id) { return VRAM_ENTRY(v, id)->h; }
u16 func_001BFEA0(u8 *v, s32 id) { return VRAM_ENTRY(v, id)->w; }

/* a .TEX file's texture entry */
typedef struct TexHeader {
    u8 psm, cpsm, pad[2];
    u16 w, h, imageQwc, clutQwc;
    s32 data;
} TexHeader;

/* +0x5C allocate room for texture `t` and fill it (+0x48); `upper`: 8 / 4-bit textures go to the
 * frame buffers' upper bits. The entry, -1 if there's no room. */
s32 func_001BFEC0(VObject *v, TexHeader *t, s32 upper) {
    u16 psm;
    s32 id;
    u8 *image, *clut;

    if (t == NULL) {
        return -1;
    }
    psm = t->psm;
    if (upper != 0 && (u32)(psm - 0x13) < 2) {
        psm = psm == 0x14 ? PSMT4HL : PSMT8H;
    }
    id = VCALL(v, 0x14, s32 (*)(VObject *, s32, s32, s32, s32))(v, (u8)psm, t->w, t->h, t->cpsm);
    if (id != -1) {
        image = NULL;
        clut = NULL;
        if (t->imageQwc != 0) {
            image = (u8 *)t + t->data;
        }
        if (t->clutQwc != 0) {
            clut = t->imageQwc != 0 ? image + t->imageQwc * 16 : (u8 *)t + t->data;
        }
        VCALL(v, 0x48, void (*)(VObject *, s32, u8 *, u8 *))(v, id, image, clut);
    }
    return id;
}

/* +0x58 / +0x54 texture `n` of a loaded .TEX file (a count, then 0x10-byte entries) through
 * +0x5C, upper bits or not; -1 if there's none */
s32 func_001BFFC0(VObject *v, u8 *tex, u32 n) {
    if (tex == NULL || n >= AT(tex, 0x0, u32)) {
        return -1;
    }
    return VCALL(v, 0x5C, s32 (*)(VObject *, TexHeader *, s32))(v, (TexHeader *)(tex + 0x10 + n * 0x10), 1);
}

s32 func_001C0020(VObject *v, u8 *tex, u32 n) {
    if (tex == NULL || n >= AT(tex, 0x0, u32)) {
        return -1;
    }
    return VCALL(v, 0x5C, s32 (*)(VObject *, TexHeader *, s32))(v, (TexHeader *)(tex + 0x10 + n * 0x10), 0);
}

extern VObject *gFileLoader;
extern void *func_00100550(u32 size);   /* malloc */
extern void func_00100470(void *p);     /* free */

/* +0x50 / +0x4C load .TEX file `name` (into a buffer of whole sectors, 64-byte aligned) and
 * allocate its first texture through +0x5C, upper bits or not; the entry, -1 if none */
static inline s32 vram_load_tex(VObject *v, const char *name, s32 upper) {
    VObject *ld = gFileLoader;
    s32 size = VCALL(ld, 0x30, s32 (*)(VObject *, const char *))(ld, name);
    u8 *buf = func_00100550((size + 0x7FF) / 0x800 * 0x800 + 0x3F);
#ifdef HG_NATIVE
    u8 *tex = (u8 *)((u32)(buf + 0x3F) / 64 * 64);   /* (PC heaps can be above 2 GB) */
#else
    u8 *tex = (u8 *)((s32)(buf + 0x3F) / 64 * 64);
#endif
    s32 id;

    VCALL(ld, 0x34, void (*)(VObject *, const char *, void *))(ld, name, tex);
    if (tex == NULL || AT(tex, 0x0, u32) == 0) {
        id = -1;
    } else {
        id = VCALL(v, 0x5C, s32 (*)(VObject *, TexHeader *, s32))(v, (TexHeader *)(tex + 0x10), upper);
    }
    func_00100470(buf);
    return id;
}

s32 func_001C0080(VObject *v, const char *name) {
    return vram_load_tex(v, name, 1);
}

s32 func_001C0190(VObject *v, const char *name) {
    return vram_load_tex(v, name, 0);
}

/* log2 of a texture size, rounded up (at most 10) */
static inline u64 Vram_Log2(u32 n) {
    u32 p = 1;
    u64 l = 0;

    while (p < n) {
        l++;
        p *= 2;
        if (l >= 11) {
            break;
        }
    }
    return l;
}

/* GS TEX0 for entry `id` (format psm, w x h, CLUT format cpsm): TCC on; `high` the bits from 55 */
static inline u64 Vram_Tex0(u8 *v, s32 id, u32 psm, u32 w, u32 h, u32 cpsm, u64 high) {
    VramEntry *e = VRAM_ENTRY(v, id);
    u32 slots = (u16)e->cpsm == 2 ? 16 : 8;
    u64 th = Vram_Log2(h), tw;
    u32 tbp;

    if ((u16)e->psm != PSMT8H && (u16)e->psm != PSMT4HL && (u16)e->psm != PSMT4HH) {
        tbp = (e->page << 11) + 0xC0000;
    } else {
        tbp = e->page << 11;
    }
    tw = Vram_Log2(w);
    return high | (u64)(cpsm & 0xFFFF) << 51 |
           (u64)(s64)(s32)(((e->cregion << 11) + 0xFC000 + (u32)((u16)e->cslot << 11) / slots) >> 6) << 37 |
           th << 30 | (u64)(psm & 0xFFFF) << 20 | (u64)(s64)(s32)((w + 63) >> 6) << 14 |
           (u64)(s64)(s32)(tbp >> 6) | tw << 26 | (u64)4 << 32;
}

/* +0x28 TEX0 for entry `id` as a psm w x h texture, loading its CLUT (CLD 1) */
u64 func_001C12E0(u8 *v, s32 id, s32 psm, u32 w, u32 h, s32 cpsm) {
    return Vram_Tex0(v, id, psm, w, h, cpsm, (u64)0x20000000 << 32);
}

/* +0x2C ... as an 8-bit indexed texture */
u64 func_001C1160(u8 *v, s32 id, u32 w, u32 h, s32 cpsm) {
    return Vram_Tex0(v, id, 0x13, w, h, cpsm, (u64)0x20000000 << 32);
}

/* +0x30 ... with CLUT offset `csa` and no CLUT load */
u64 func_001C0FE0(u8 *v, s32 id, u32 csa, s32 psm, u32 w, u32 h, s32 cpsm) {
    return Vram_Tex0(v, id, psm, w, h, cpsm, (u64)csa << 56);
}

/* +0x34 TEX2 for entry `id`: format psm, CLUT `csa` of the entry's CLUT region (format cpsm) */
u64 func_001C0F40(u8 *v, s32 id, u32 csa, s32 psm, s32 cpsm) {
    VramEntry *e = VRAM_ENTRY(v, id);
    u32 slots = (u16)e->cpsm == 2 ? 16 : 8;
    u32 cbp = ((e->cregion << 11) + 0xFC000 + (u32)((u16)e->cslot << 11) / slots) >> 6;

    return ((u64)csa << 56) | ((u64)(cpsm & 0xFFFF) << 51) | ((u64)cbp << 37) | ((u64)(psm & 0xFFFF) << 20);
}

/* +0x38 .. +0x44: the same, with the entry's own format and size */
u64 func_001C0F10(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x28, u64 (*)(u8 *, s32, s32, u32, u32, s32))(v, id, (u16)e->psm, (u16)e->w, (u16)e->h, (u16)e->cpsm);
}

u64 func_001C0EE0(u8 *v, s32 id) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x2C, u64 (*)(u8 *, s32, u32, u32, s32))(v, id, (u16)e->w, (u16)e->h, (u16)e->cpsm);
}

u64 func_001C0EB0(u8 *v, s32 id, u32 csa) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x30, u64 (*)(u8 *, s32, u32, s32, u32, u32, s32))(v, id, csa, (u16)e->psm, (u16)e->w, (u16)e->h, (u16)e->cpsm);
}

u64 func_001C0E80(u8 *v, s32 id, u32 csa) {
    VramEntry *e = VRAM_ENTRY(v, id);

    return VCALL(v, 0x34, u64 (*)(u8 *, s32, u32, s32, s32))(v, id, csa, (u16)e->psm, (u16)e->cpsm);
}

extern void sceGsSetDefLoadImage(void *lp, s16 dbp, s16 dbw, s16 dpsm, s16 x, s16 y, s16 w, s16 h);
extern void sceGsExecLoadImage(void *lp, const void *src);
extern void sceGsSyncPath(s32 mode, s32 timeout);
extern void FlushCache(s32 mode);
extern u8 D_003B3040[][2];   /* where CLUT n goes in a CLUT block, 16-colour CLUTs (x, y) */
extern u8 D_003B3050[][2];   /* ... 256-colour (psm 2) */

/* upload slot `slot`'s texture `pix` and its CLUT `clut` (either NULL: not sent) at once
 * (slots of 0x12 bytes at +0x98: used, w, h, psm, page, CLUT psm (0xFF none), CLUT page, index) */
void func_001C02A0(u8 *v, s32 slot, const void *pix, const void *clut) {
    u8 li[0x60] __attribute__((aligned(16)));
    u8 *e;
    u16 cpsm;

    if (slot < 0) {
        return;
    }
    e = v + 0x98 + slot * 0x12;
    if (!e[0]) {
        return;
    }
    if (pix != NULL) {
        u16 psm = AT(e, 0x6, u16);
        u32 tbp;

        if (psm == 0x1B || psm == 0x24 || psm == 0x2C) {
            tbp = (u32)(AT(e, 0x8, s16) << 11) >> 6;   /* (in the upper bits of a 24-bit page) */
        } else {
            tbp = (u32)((AT(e, 0x8, s16) << 11) + 0xC0000) >> 6;
        }
        sceGsSetDefLoadImage(li, (s16)tbp, (s16)((AT(e, 0x2, u16) + 63) / 64), (s16)psm, 0, 0,
                             (s16)AT(e, 0x2, u16), AT(e, 0x4, s16));
        FlushCache(0);
        sceGsExecLoadImage(li, pix);
        sceGsSyncPath(0, 0);
    }
    cpsm = AT(e, 0xC, u16);
    if (cpsm == 0xFF || clut == NULL) {
        return;
    }
    {
        u8 *at = cpsm == 2 ? D_003B3050[AT(e, 0x10, u16)] : D_003B3040[AT(e, 0x10, u16)];
        u32 cbp = (u32)((AT(e, 0xE, s16) << 11) + 0xFC000) >> 6;

        sceGsSetDefLoadImage(li, (s16)cbp, 1, (s16)cpsm, (s16)at[0], (s16)at[1], 0x10, 0x10);
        FlushCache(0);
        sceGsExecLoadImage(li, clut);
        sceGsSyncPath(0, 0);
    }
}
