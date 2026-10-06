/* The sub screen: the menu system of the title and of the in-game menus (vtable SubScreen_vtable, at
 * SceneTitle +0x74A80; code 0x384C00..0x3A0000). Its textures are SUBSCR\SUBBASE.TEX and
 * SUBSCR\SUBBACK.TEX. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "progress.h"
#include "text.h"
#include "subscreen.h"
#include "input.h"
#include "sound.h"
#include "gs.h"
#include "renderer.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "actor.h"
#include "pursuer.h"
#include "memcard.h"
#include "navmesh.h"
#include "music.h"
#include "effects.h"
#include "fiona.h"
#include "heap.h"
#include "hewie.h"
#include "items.h"
#include "model.h"
#include "movie.h"
#include "scene_game.h"
#include "libc.h"
#include "msl.h"
#include "daniella.h"
#include "loader.h"
#include "vecmath.h"
#include "pad.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "system.h"
#include "sce/iop.h"
#include "item.h"
#include "director.h"
#include "doors.h"
#include "room.h"
#include "placed.h"
#include "room_map.h"
#include "lights.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "gl2d.h"
#include "lorenzo.h"
#include "riccardo.h"
#include "tintstalker.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

extern VObject *gStageMusic;
extern void *PoolEntry_vtable[];      /* a pool entry */

void SubScreen_StateItems(SubScreen *s);
void SubScreen_DrawLoadWait(SubScreen *s);
void SubScreen_Open(SubScreen *s);

static const char sSubBase[] = "SUBSCR\\SUBBASE.TEX";
static const char sSubBack[] = "SUBSCR\\SUBBACK.TEX";

extern const u8 D_0044BF10[];
s32 SubScreen_LoaderIdle(void *pool);
u32 Items_FirstFree(u8 *o, u8 row);
void *Items_GiveWithData(u8 *items, u32 id, u64 *data);
s32 SubScreen_EntryMotions(void *o, u32 i);   /* (a u8) */
void *Kind14Model_ctor(u8 *m);
u32 SubScreen_FileCount(u8 *o);

extern void *D_0046EC80[];

extern u8 *kMapRooms[];     /* per map: its rooms (0x18-byte entries, -1 terminated); NULL ends */
extern void **kMapPages[];  /* per map: its pages (by the entry's +0x4) */
extern u16 *D_0041F8B0[];      /* per map: each page's title message */
extern u8 D_00420570[];        /* the alternative room entries (0x18 each) */
#define MAP_CUR(m) AT(m, 0x10C, s8)

#define MAP_PAGE(m) AT(m, 0x10E, s8)

#define MAP_PICTURE_AREA 0x6000000   /* the file loader's area for the page's picture */

extern void *SubScreen_vtable[];          /* the title work */
extern void *D_0046A090[], *D_0046A078[], *D_004699E0[], *BlockPool_vtable[], *D_0046A068[];
extern void *D_0046A058[];
extern void TextObj_Release(u8 *o);
/* the entry pool's destructor body: its list, its 192 entries, the global */
static inline void Pool_Destroy(u8 *pool) {
    AT(pool, 0x0, void **) = D_0046A078;
    if (pool + 0x1208 != NULL) {
        AT(pool, 0x1208, void **) = BlockPool_vtable;
        if (pool + 0x1208 != NULL) {
            AT(pool, 0x1208, void **) = D_004699E0;
        }
    }
    func_001002C0(pool + 8, PoolEntry_dtor, 0x18, 0xC0);
    if (pool != NULL) {
        gSubPool = NULL;
    }
}

static inline void task_end_child(Task *t) {
    if (t != NULL && t->child != NULL) {
        Task_dtor(t->child, 1);
        t->child = NULL;
    }
}

/* the maps the player has (progress +0x84 bits 22..26 as bits 0..4, as Progress_MapsHeld) */
static inline u8 map_owned(void) {
    u32 b = AT(gProgress, 0x84, u32);
    u8 v = 0;

    if (b & 0x400000) {
        v |= 1;
    }
    if (b & 0x800000) {
        v |= 2;
    }
    if (b & 0x1000000) {
        v |= 4;
    }
    if (b & 0x2000000) {
        v |= 8;
    }
    if (b & 0x4000000) {
        v |= 0x10;
    }
    return v;
}

/* can map `map` be shown: one the player has, with pages */
static inline s32 map_shown(s8 map) {
    u8 owned;

    if (map == -1) {
        return 0;
    }
    owned = map_owned();
    if (owned == 0) {
        return 0;
    }
    if (kMapPages[map] == NULL) {
        return 0;
    }
    return (owned & (1 << map)) ? 1 : 0;
}

/* the current map's last page */
static inline void map_last_page(u8 *m) {
    MAP_PAGE(m) = 0;
    while (kMapPages[MAP_CUR(m)][MAP_PAGE(m) + 1] != NULL) {
        MAP_PAGE(m)++;
    }
}

/* the first map from 0 that can be shown; none: back to `map` / `page` */
static inline void map_first(u8 *m, s8 map, s8 page) {
    MAP_CUR(m) = 0;
    for (;;) {
        if (kMapPages[MAP_CUR(m)] == NULL) {
            MAP_CUR(m) = map;
            *(volatile s8 *)&MAP_PAGE(m) = page;   /* (stored again, unchanged, as the original) */
            return;
        }
        if (map_shown(MAP_CUR(m))) {
            return;
        }
        MAP_CUR(m)++;
    }
}

s32 Map_LeftRight(u8 *m);
void Map_HereArrow(u8 *m);
s32 Map_PageLoading(u8 *m);

/* the entry pool (gSubPool): destructor */
/* 0x00130920 */
void *SubPool_dtor(u8 *pool, s32 flags) {
    if (pool != NULL) {
        Pool_Destroy(pool);
        if ((s16)flags > 0) {
            func_00100490(pool);
        }
    }
    return pool;
}
/* operator delete for pool entries: nothing (the pool is dropped at once) */
/* 0x0025FEF0 */
void SubPool_delete(void *p) {
}
/* placement new (pool entries) */
/* 0x0025FF00 */
void *SubPool_new(u32 size, void *p) {
    return p;
}

/* the file loader's +0x28 (0x4000000) is 2 */
/* 0x00260130 */
s32 SubScreen_LoaderIdle(void *pool) {
    return VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x4000000) == 2;
}

/* the entries' pool: the first 64 entries constructed again, the tables cleared, the block
 * pool (+0x1208) over all 192 */
void SubPool_Reset(u8 *pool) {
    s32 i, j;

    for (i = 0; i < 0x40; i++) {
        u8 *e = SubPool_new(0x18, pool + 8 + i * 0x18);

        if (e != NULL) {
            AT(e, 0x0, void **) = PoolEntry_vtable;
            AT(e, 0x4, s32) = -1;
            AT(e, 0x8, u8) = 0;
            AT(e, 0x10, s64) = 0;
        }
    }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 0x40; i++) {
            AT(pool, 0x12E0 + j * 0x100 + i * 4, s32) = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        AT(pool, 0x15E0 + i * 4, s32) = 0;
    }
    BlockPool_Init((BlockPool *)(pool + 0x1208), pool + 8, 0x18, 0xC0, pool + 0x1220);
}

/* Options defaults: sound mode from the sound driver (+0x6C), the video mode and screen offset
 * from the renderer, +4 on, +8 = 1.0. */
/* 0x002A7AA0 */
void Options_Defaults(u8 *o) {
    VObject *r;
    u8 *disp;

    o[0] = VCALL(gSound, 0x6C, s32 (*)(VObject *))(gSound);
    r = gRenderer;
    o[1] = VCALL(r, 0x28, u8 (*)(VObject *))(r);
    disp = VCALL(r, 0x2C, u8 *(*)(VObject *))(r);
    o[2] = (s8)disp[0x1F];
    o[3] = (s8)disp[0x20];
    o[4] = 1;
    o[5] = 0;
    o[6] = 0;
    *(u32 *)(o + 8) = 0x3F800000;   /* 1.0f */
}

/* destructor (vtable D_0046EC80) */
/* 0x002C65C0 */
void *Obj46EC80_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EC80;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the sub screen (SubScreen_vtable): its load / save screens, text object and two text tasks, then
 * the base (D_0046A090): the pool's entries, the globals gSubPool / gSubScreen cleared */
/* 0x002D0110 */
void *SubScreen_dtor(SubScreen *w, s32 flags) {
    if (w != NULL) {
        u8 *o = (u8 *)w;

        w->vtbl = SubScreen_vtable;
        AT(o, 0xA8AC0, void **) = D_0046A058;
        Task_dtor((Task *)(o + 0xA8AD8), -1);
        AT(o, 0x97980, void **) = D_0046A068;
        TextObj_Release(o + 0x97980);
        Task_dtor((Task *)(o + 0x97984), -1);
        task_end_child(&w->text);
        task_end_child(&w->ask);
        w->vtbl = D_0046A090;
        AT(o, 0x8, void **) = D_0046A078;
        AT(o, 0x1210, void **) = BlockPool_vtable;
        AT(o, 0x1210, void **) = D_004699E0;
        func_001002C0(o + 0x10, PoolEntry_dtor, 0x18, 0xC0);
        gSubPool = NULL;
        gSubScreen = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return w;
}

/* the map turned to page `page` if its room (+0x108) is shown there too */
/* 0x00303E60 */
void Map_TurnTo(u8 *m, s8 page) {
    u8 *e;

    if (AT(m, 0x108, s32) == -1 || AT(m, 0x10D, s8) == -1 || AT(m, 0x10F, s8) == page) {
        return;
    }
    e = kMapRooms[AT(m, 0x10C, s8)];
    if (e == NULL) {
        return;
    }
    for (; AT(e, 0, s32) != -1; e += 0x18) {
        if (AT(e, 0, s32) == AT(m, 0x108, s32) && AT(e, 4, s8) == page) {
            AT(m, 0x10E, s8) = page;
            AT(m, 0x10F, s8) = page;
            return;
        }
    }
}

/* Left / right on the map: the previous / next page, past the ends the previous / next map the
 * player has (round), its last / first page. A new page has its picture loaded: 1. */
/* 0x00303F90 */
s32 Map_LeftRight(u8 *m) {
    u32 pad = gMenuPressed;
    s8 map = MAP_CUR(m), page = MAP_PAGE(m);

    if (pad & MENU_LEFT) {
        if (map_shown(map) && page != 0) {
            MAP_PAGE(m)--;
        } else if (map == -1) {
            map_first(m, map, page);
            if (MAP_CUR(m) == -1) {
                return 0;
            }
            map_last_page(m);
        } else {
            for (;;) {
                if (MAP_CUR(m) != 0) {
                    MAP_CUR(m)--;
                } else {
                    do {
                        MAP_CUR(m)++;
                    } while (kMapPages[MAP_CUR(m) + 1] != NULL);
                }
                if (MAP_CUR(m) == map || map_shown(MAP_CUR(m))) {
                    break;
                }
            }
            map_last_page(m);
        }
    } else if (pad & MENU_RIGHT) {
        if (map_shown(map) && kMapPages[map][page + 1] != NULL) {
            MAP_PAGE(m)++;
        } else if (map == -1) {
            map_first(m, map, page);
            if (MAP_CUR(m) == -1) {
                return 0;
            }
            MAP_PAGE(m) = 0;
        } else {
            for (;;) {
                MAP_CUR(m)++;
                if (kMapPages[MAP_CUR(m)] == NULL) {
                    MAP_CUR(m) = 0;
                }
                if (MAP_CUR(m) == map || map_shown(MAP_CUR(m))) {
                    break;
                }
            }
            MAP_PAGE(m) = 0;
        }
    }
    if (map == MAP_CUR(m) && page == MAP_PAGE(m)) {
        return 0;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, void *, void *, u32, s32))(
        gFileLoader, kMapPages[MAP_CUR(m)][MAP_PAGE(m)], m + 0x140, MAP_PICTURE_AREA, 0);
    return 1;
}

/* the "you are here" arrow: Fiona's spot on the page of the map she is on (the room's entry:
 * +0x8 / +0xC x / z scale, +0x10 / +0x14 x / y offset; map 2's rooms 0x100..0x105 take theirs
 * from D_00420570 by the events' +0x70), a 32 x 32 arrow (texture group 0x18 #0, 0x1C0, 0x60)
 * turned to her heading, layer 0x30 */
/* 0x003048C0 */
void Map_HereArrow(u8 *m) {
    s32 room = AT(m, 0x108, s32);
    s8 map = MAP_CUR(m), page = MAP_PAGE(m);
    u8 *e;
    s32 found = 0;
    f32 fx, fy, a, s, c;
    s32 x0, y0, x1, y1, x2, y2, x3, y3;

    if (room == -1 || map == -1 || page == -1 || gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0) {
        return;
    }
    for (e = kMapRooms[map]; AT(e, 0, s32) != -1; e += 0x18) {
        if (AT(e, 0, s32) == room && AT(e, 4, s8) == page) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return;
    }
    if (map == 2 && (u32)room >= 0x100 && (u32)room < 0x106) {
        s32 n = VCALL(gEvents, 0x70, s32 (*)(VObject *))(gEvents);

        if (n < 0) {
            return;
        }
        e = D_00420570 + (n + 0xE) * 0x18;
    }
    fx = AT(e, 0x10, f32) + 512.0f * (AT(gCharPlayer, 0x10, f32) / AT(e, 0x8, f32)) / 640.0f;
    fy = 128.0f + (AT(e, 0x14, f32) + AT(gCharPlayer, 0x18, f32) / AT(e, 0xC, f32));
    a = -AT(gCharPlayer, 0x54, f32);
    /* the corners (-16, 16) (16, 16) (-16, -16) (16, -16) turned by a, x squeezed 512 / 640 */
    s = func_0031C248(a);
    c = func_0031C058(a);
    x0 = (s32)(fx + 512.0f * (-16.0f * c - 16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y0 = (s32)(fy + (16.0f * c + -16.0f * s));
    s = func_0031C248(a);
    c = func_0031C058(a);
    x1 = (s32)(fx + 512.0f * (16.0f * c - 16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y1 = (s32)(fy + (16.0f * c + 16.0f * s));
    s = func_0031C248(a);
    c = func_0031C058(a);
    x2 = (s32)(fx + 512.0f * (-16.0f * c - -16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y2 = (s32)(fy + (-16.0f * c + -16.0f * s));
    s = func_0031C248(a);
    c = func_0031C058(a);
    x3 = (s32)(fx + 512.0f * (16.0f * c - -16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y3 = (s32)(fy + (-16.0f * c + 16.0f * s));
    VCALL(gRenderer, 0x84, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, u32,
                                     s32, s32, s32, s32))(
        gRenderer, x0, y0, x1, y1, x2, y2, x3, y3, 0x1C0, 0x60, 0x20, 0x20, 0x20808080, 0, 0x18, 0x30, 6);
}

/* 1 while the page's picture is still loading; else, when the page can be shown, its
 * picture's texture made resident (texture cache +0x10) */
/* 0x00304D70 */
s32 Map_PageLoading(u8 *m) {
    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, MAP_PICTURE_AREA) == 2) {
        return 1;
    }
    if (map_shown(MAP_CUR(m))) {
        VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, m + 0x140, 0x28);
    }
    return 0;
}

/* the map page each frame: with any map, left / right (Map_LeftRight) with a sound on a new
 * page; the page's picture (texture group 0x28) with the arrow when it is Fiona's own page,
 * and its title centred at the top (no map or page: message 0x85) */
/* 0x00304F50 */
void Map_PageFrame(u8 *m) {
    u8 *text;

    if (Map_PageLoading(m)) {
        return;
    }
    if (map_owned() != 0 && Map_LeftRight(m)) {
        VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0x2A, 5);
        return;
    }
    if (MAP_CUR(m) == -1 || MAP_PAGE(m) == -1 || !map_shown(MAP_CUR(m))) {
        text = Task_MessageText(m + 4, 0x85);
    } else {
        text = Task_MessageText(m + 4, D_0041F8B0[MAP_CUR(m)][MAP_PAGE(m)]);
    }
    if (map_shown(MAP_CUR(m))) {
        if (MAP_CUR(m) != -1 && MAP_PAGE(m) != -1) {
            VCALL(gRenderer, 0x7C, s32 (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32,
                                            s32))(gRenderer, 0, 0x80, 0x200, 0x100, 0, 0, 0x200, 0x100, 0x80808080,
                                                  0, 0x28, 0x30, 0);
        }
        if (MAP_CUR(m) == AT(m, 0x10D, s8) && MAP_PAGE(m) == AT(m, 0x10F, s8)) {
            Map_HereArrow(m);
        }
    }
    if (text != NULL) {
        u32 w = Text_LineWidth((Task *)(m + 4), text, 0x10) & 0xFFFF;

        Task_ShowText((Task *)(m + 4), 0xA6 - (w >> 1), 0x48, 0x80, text, 0x80, 0x33, 0x10, 0x15);
    }
}

/* back to the map / page the player is in (+0x10D / +0x10F) and, when it can be shown, its
 * picture loaded */
/* 0x00305380 */
void Map_BackToPlayer(u8 *m) {
    MAP_CUR(m) = AT(m, 0x10D, s8);
    MAP_PAGE(m) = AT(m, 0x10F, s8);
    if (AT(m, 0x108, s32) == -1 || AT(m, 0x10D, s8) == -1 || AT(m, 0x10F, s8) == -1) {
        return;
    }
    if (!map_shown(MAP_CUR(m))) {
        return;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, void *, void *, u32, s32))(
        gFileLoader, kMapPages[MAP_CUR(m)][MAP_PAGE(m)], m + 0x140, MAP_PICTURE_AREA, 0);
}

/* SceneGame +0x101EBC0 (the map): find which map and page show room `room` (+0x108 the room,
 * +0x10C/+0x10D the map, +0x10E/+0x10F the page; -1: none) */
/* 0x00305520 */
void Map_FindRoom(u8 *m, s32 room) {
    s32 i;
    u8 *e;

    AT(m, 0x108, s32) = -1;
    AT(m, 0x10D, s8) = -1;
    AT(m, 0x10C, s8) = -1;
    AT(m, 0x10F, s8) = -1;
    AT(m, 0x10E, s8) = -1;
    if (room == -1 || (u32)room >= 0x110) {
        return;
    }
    for (i = 0; kMapRooms[i] != NULL; i++) {
        for (e = kMapRooms[i]; AT(e, 0, s32) != -1; e += 0x18) {
            if (AT(e, 0, s32) == room && kMapPages[i] != NULL &&
                kMapPages[i][AT(e, 4, s8)] != NULL) {
                AT(m, 0x108, s32) = room;
                AT(m, 0x10D, s8) = i;
                AT(m, 0x10C, s8) = i;
                AT(m, 0x10F, s8) = AT(e, 4, s8);
                AT(m, 0x10E, s8) = AT(e, 4, s8);
                return;
            }
        }
    }
}

/* start: load the textures, reset */
void SubScreen_Start(SubScreen *s) {
    VObject *ld = gFileLoader;
    s32 i;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBase, s->baseTex, 0x6000000, 0);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sSubBack, s->pageTex, 0x6000000, 0);
    SubPool_Reset(s->pool);
    for (i = 0; i < 9; i++) {
        s->unk97740[i] = 0;
    }
    for (i = 0; i < 0x80; i++) {
        s->unk15F8[i] = 0;
    }
    s->mode = 0;
    s->frame = 0;
    s->showBehind = 1;
    s->unkA8C50 = 0;
    for (i = 0; i < 3; i++) {
        s->unkA8C51[i] = 0;
    }
    s->unkA8C55 = 0;
    s->optCursor = 0;
    s->open = 0;
    s->resumeKind = 0;
    s->kind = 0;
    ptmf_set_fn(&s->resume, SubScreen_StateItems);
    ptmf_set_fn(&s->draw, SubScreen_DrawLoadWait);
}

/* the master volume (0..1): sound effects, voices (gStageMusic), the movie, the music */
static void opt_apply_volume(VObject *snd, f32 vol) {
    VCALL(snd, 0xA8, void (*)(VObject *, f32))(snd, vol);
    if (gStageMusic != NULL) {
        VCALL(gStageMusic, 0x20, void (*)(VObject *, f32))(gStageMusic, vol);
    }
    if (gMovie != NULL) {
        f32 *v = &AT(gMovie, 0x1D0, f32);

        *v = vol;
        if (vol < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        Movie_ApplyVolume(gMovie);
    }
    if (gAdx != NULL) {
        f32 *v = &AT(gAdx, 0x11C, f32);

        *v = vol;
        if (vol < 0.0f) {
            *v = 0.0f;
        }
        if (!(*v <= 1.0f)) {
            *v = 1.0f;
        }
        Bgm_ApplyVolume(gAdx);
    }
}

/* apply the options: controller layout, vibration, sound output, volume, screen position */
void SubScreen_ApplyOptions(VObject *s) {
    u8 *opt = gGamePtr + 0x30;
    VObject *o;
    f32 vol;

    VCALL(s, 0x34, void (*)(VObject *, s32))(s, (s8)opt[6]);
    o = gRumble;
    VCALL(o, 0x10, void (*)(VObject *))(o);
    if ((s8)opt[4] == 1) {
        VCALL(o, 0x28, void (*)(VObject *, s32))(o, 1);
    } else {
        VCALL(o, 0x28, void (*)(VObject *, s32))(o, 0);
    }
    o = gSound;
    VCALL(o, 0x68, void (*)(VObject *, s32))(o, (s8)opt[0]);
    vol = *(f32 *)(opt + 8);
    opt_apply_volume(o, vol);
    VCALL(gRenderer, 0x30, void (*)(VObject *, s32, s32))(gRenderer, (s8)opt[2], (s8)opt[3]);
}

extern u16 D_0044B5E0[][6][2];   /* per controller layout: 6 x (button, its name's message) */
extern u8 kButtonMap[];          /* button mapping (+0xA: the six configurable actions) */
extern u8 D_0047B150[6];         /* the name slot of each action */

/* +0x34 apply controller layout `type`: map the six actions and name their buttons */
void SubScreen_SetLayout(SubScreen *s, s32 type) {
    s32 i;

    for (i = 0; i < 6; i++) {
        kButtonMap[0xA + i] = D_0044B5E0[type][i][0];
        Msg_SetName(&s->ask, D_0047B150[i], D_0044B5E0[type][i][1]);
    }
}

void Costumes_Setup(SubScreen *s);
void Results_Setup(SubScreen *s);
extern void *SynthBase_vtable[];       /* the 0x15C helper's base vtable */
extern void *SynthPot_vtable[];
extern void *SlotMachine_vtable[];

void Options_StateList(SubScreen *s);
void SubScreen_StateSave(SubScreen *s);
void SubScreen_StateWordMake(SubScreen *s);
void SubScreen_StatePageObject(SubScreen *s);
void SubScreen_StateLoad(SubScreen *s);
void Costumes_State(SubScreen *s);
void SubScreen_StateMovies(SubScreen *s);
void SubScreen_StateExtrasMenu(SubScreen *s);
void SubScreen_StateExtras(SubScreen *s);
void Results_State(SubScreen *s);
void Gallery_StateMusic(SubScreen *s);
void Gallery_StateArtList(SubScreen *s);
void SubScreen_StateEntries(SubScreen *s);
void SubScreen_DrawFadeIn(SubScreen *s);
void SubScreen_DrawOpening(SubScreen *s);

static const char sSubSave[] = "SUBSCR\\SUBSAVE.TEX";
static const char sPlate[] = "SUBSCR\\PLATE.TEX";
static const char sSynSlot[] = "SUBSCR\\SYN_SLOT.TEX";
static const char sSubMg[] = "SUBSCR\\SUBMG.TEX";
static const char sGalMovie[] = "SUBSCR\\GAL_MOVIE.TEX";
static const char sThumbnail[] = "SUBSCR\\THUMBNAIL.TXS";
static const char sGalModel[] = "SUBSCR\\GAL_MODEL.TEX";
static const char sGalMusic[] = "SUBSCR\\GAL_MUSIC.TEX";
static const char sGalArt[] = "SUBSCR\\GAL_ART.TEX";
static const char sArtLen[] = "SUBSCR\\ART00.LEN";
static const char sGalType[] = "SUBSCR\\GAL_TYPE.TEX";

static void sub_load(SubScreen *s, const char *name, void *dst) {
    VObject *ld = gFileLoader;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, dst, 0x6000000, 0);
}

static void sub_free_vram(void) {
    VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, 0x19);
}

static void sub_se(void) {
    Sound_Play(gSound, SE_OPEN, SE_BANK_MENU);
}

/* the options being edited: a copy of the system data's */
static void sub_copy_options(SubScreen *s) {
    u8 *sys = gGamePtr;
    s32 i;

    for (i = 0; i < 7; i++) {
        s->opt[i] = sys[0x30 + i];
    }
    s->optVolume = AT(sys, 0x38, f32);
}

/* the save slot screen's helper object (0x15C bytes at +0xA8C80) */
static void sub_new_slots(SubScreen *s, void **vtbl) {
    u8 *p = SynthBase_new(0x15C, s->page);

    if (p != NULL) {
        AT(p, 0, void **) = SynthBase_vtable;
        Task_Construct((Task *)(p + 0x14));
        VCALL((VObject *)p, 0xC, void (*)(VObject *))((VObject *)p);
        AT(p, 0, void **) = vtbl;
        VCALL((VObject *)p, 0xC, void (*)(VObject *))((VObject *)p);
    }
}

/* open the sub screen in mode +4: 0 the in-game menu, 5 options, 6 load (title), the rest
 * save screens and the galleries */
void SubScreen_Open(SubScreen *s) {
    u8 *sys;
    u8 n;

    s->open = 1;
    switch (s->mode) {
    case 0:
        sub_copy_options(s);
        if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
            s->kind = 0xA;
            ptmf_set_fn(&s->state, Options_StateList);
        } else {
            Map_BackToPlayer(s->textObj);
            s->kind = s->resumeKind;
            s->state = s->resume;
        }
        sub_se();
        break;
    case 1:
        sub_free_vram();
        sub_load(s, sSubSave, s->pageTex);
        SaveScreen_Init(&s->card, s->saveBuf0, s->saveBuf1);
        s->card.state = 0;
        s->card.hidden = 0;
        s->kind = 0x85;
        ptmf_set_fn(&s->state, SubScreen_StateSave);
        break;
    case 2:
        sub_free_vram();
        sub_load(s, sPlate, s->pageTex);
        s->unkA8C57 = 0;
        for (n = 0; n < 8; n++) {
            s->unkA8C58[n] = 0;
        }
        s->unkA8C60 = 0;
        s->kind = 0x84;
        ptmf_set_fn(&s->state, SubScreen_StateWordMake);
        sub_se();
        break;
    case 3:
        sub_free_vram();
        sub_load(s, sSynSlot, s->pageTex);
        sub_new_slots(s, SlotMachine_vtable);
        s->kind = 0x86;
        ptmf_set_fn(&s->state, SubScreen_StatePageObject);
        sub_se();
        break;
    case 4:
        sub_free_vram();
        sub_load(s, sSynSlot, s->pageTex);
        sub_new_slots(s, SynthPot_vtable);
        s->kind = 0x87;
        ptmf_set_fn(&s->state, SubScreen_StatePageObject);
        sub_se();
        break;
    case 5:
        sub_copy_options(s);
        sub_se();
        s->kind = 0x8A;
        ptmf_set_fn(&s->state, Options_StateList);
        break;
    case 6:
        sub_free_vram();
        sub_load(s, sSubSave, s->pageTex);
        SaveScreen_Init(&s->card, NULL, NULL);
        s->card.state = 0;
        s->card.hidden = 0;
        s->kind = 0x85;
        ptmf_set_fn(&s->state, SubScreen_StateLoad);
        break;
    case 7:
        sub_free_vram();
        sub_load(s, sSubMg, s->pageTex);
        s->kind = 0x80;
        Costumes_Setup(s);
        ptmf_set_fn(&s->state, Costumes_State);
        break;
    case 8: {
        VObject *ld;
        Progress *p;

        sub_free_vram();
        ld = gFileLoader;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sGalMovie, s->pageTex, 0x6000000, 0);
        p = gProgress;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sThumbnail, (u8 *)p + 0x16C0, 0x6000000, 0);
        s->kind = 0x8B;
        s->page[0] = Progress_GetVar(p, 0x2B);
        ptmf_set_fn(&s->state, SubScreen_StateMovies);
        break;
    }
    case 9:
        SaveScreen_Init(&s->card, s->saveBuf0, s->saveBuf1);
        s->card.state = 0;
        s->card.hidden = 1;
        s->kind = 0x85;
        ptmf_set_fn(&s->state, SubScreen_StateSave);
        break;
    case 10:
        /* the extras menu: entries 0..2, then 3 and 4 when unlocked, then 5 */
        sub_free_vram();
        sub_load(s, sSubMg, s->pageTex);
        s->kind = 0x80;
        s->page[0] = 0;
        s->page[1] = 1;
        s->page[2] = 2;
        n = 3;
        sys = gGamePtr;
        if ((AT(sys, 0x2C, u32) & 0x40000) != 0) {
            s->page[3] = 3;
            n++;
        }
        if ((AT(sys, 0x2C, u32) & 0x80000) != 0) {
            s->page[n] = 4;
            n++;
        }
        s->page[n] = 5;
        s->page[6] = n + 1;
        s->page[7] = 0;
        ptmf_set_fn(&s->state, SubScreen_StateExtrasMenu);
        break;
    case 11:
        sub_free_vram();
        sub_load(s, sGalModel, s->pageTex);
        s->kind = 0x8C;
        s->page[0] = 0;
        ptmf_set_fn(&s->state, SubScreen_StateExtras);
        break;
    case 12:
        s->kind = 0x80;
        Results_Setup(s);
        ptmf_set_fn(&s->state, Results_State);
        break;
    case 13:
        sub_free_vram();
        sub_load(s, sGalMusic, s->pageTex);
        s->kind = 0x8D;
        s->page[0] = 0;
        s->page[2] = 0xFF;
        s->page[1] = 0xFF;
        ptmf_set_fn(&s->state, Gallery_StateMusic);
        break;
    case 14: {
        VObject *ld;
        void *buf;

        sub_free_vram();
        ld = gFileLoader;
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sGalArt, s->pageTex, 0x6000000, 0);
        s->kind = 0x8E;
        s->page[0] = 0;
        s->page[1] = 0;
        s->page[2] = 0;
        s->page[3] = 1;
        buf = VCALL((VObject *)gProgress, 0x88, void *(*)(VObject *))((VObject *)gProgress);
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sArtLen, buf, 0x6000000, 0);
        SUB_PAGE(s, 4, s16) = 0;
        SUB_PAGE(s, 6, s16) = 0;
        s->page[8] = 0;
        s->page[9] = 1;
        s->page[10] = 0;
        s->page[11] = 0;
        s->page[12] = 0;
        ptmf_set_fn(&s->state, Gallery_StateArtList);
        break;
    }
    case 15:
        sub_free_vram();
        sub_load(s, sGalType, s->pageTex);
        s->kind = 0x8F;
        s->page[0] = 0;
        ptmf_set_fn(&s->state, SubScreen_StateEntries);
        break;
    }
    s->showBehind = 1;
    s->fading = 1;
    s->fade = 0;
    s->fadeStep = 0x10;
    ptmf_set_fn(&s->draw, SubScreen_DrawFadeIn);
    if (s->mode == 7 || s->mode == 0xC || s->mode == 0xA) {
        s->fadeStep = 8;
        ptmf_set_fn(&s->draw, SubScreen_DrawOpening);
    }
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* per frame: run the draw/step state (+0x16FC); true while the menu behind should be drawn */
s32 SubScreen_Update(SubScreen *s) {
    ptmf_scall(s, &s->draw);
    s->frame++;
    return s->showBehind;
}

/* state: wait for the textures, upload them (VRAM slots 0x18, 0x19), set the screen up */
void SubScreen_DrawLoadWait(SubScreen *s) {
    VObject *tc;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) == 2) {
        return;
    }
    tc = gTexCache;
    VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s->baseTex, 0x18);
    VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s->pageTex, 0x19);
    SubScreen_Open(s);
}

extern s8 kSubScreenFadePanels[][4];       /* per screen kind: up to 4 panels to draw while fading */
extern u8 gLanguage;
extern void *Overlay_vtable[];       /* overlay vtable */
extern void *Helper469D00_vtable[];       /* its base */
void SubScreen_DrawFadeFromBlack(SubScreen *s);

/* a fixed piece of the screen (SUBBACK.TEX, VRAM group 0x19): texture, u, v, w, h, x, y */
extern u16 D_0044C160[][7];
/* a movable part (SUBBASE.TEX, group 0x18): u, v, w, h, screen w, h, CLUT, texture, blending
 * (0 normal, 1..3 fixed alpha variants) */
extern u16 D_0044C280[][9];

#ifdef HG_NATIVE

/* draw panel `id` (D_0044C160: its w x h texels at u, v, at x, y) mixed over the screen by
 * fixed alpha `alpha`, in renderer layer `layer` */
void SubScreen_DrawPanel(void *s, s32 id, s32 alpha, s32 layer) {
    u16 *e = D_0044C160[(u8)id];
    u8 *tex;
    s32 slot = TexCache_Resident(e[0], 0x19, (u8)layer, &tex);

    if (slot == -1) {
        return;
    }
    gl2d_sprite((u8)layer, e[5], e[6], e[5] + e[3], e[6] + e[4], tex, e[1], e[2], e[1] + e[3], e[2] + e[4],
                0x80808080, 0, gl2d_blend(((u64)(u8)alpha << 32) | 0x64));
}

/* draw part `part` (D_0044C280) at x, y with alpha `alpha`, in layer 0x30 (0x33 with `top`):
 * its blending 0 normal, 1 subtracted / 2 mixed / 3 added by the fixed alpha */
void SubScreen_DrawPart(void *s, s32 x, s32 y, s32 part, s32 alpha, s32 top) {
    static const u8 kAlpha[4] = {0x44, 0x62, 0x64, 0x68};
    u16 *e = D_0044C280[(u8)part];
    s32 layer = top ? 0x33 : 0x30;
    u8 *tex;
    s32 slot = TexCache_Resident(e[7], 0x18, layer, &tex);
    s16 sx = x, sy = y;

    if (slot == -1) {
        return;
    }
    gl2d_sprite(layer, sx, sy, sx + e[4], sy + e[5], tex, e[0], e[1], e[0] + e[2], e[1] + e[3], 0x80808080, e[6],
                gl2d_blend(((u64)(u8)alpha << 32) | kAlpha[e[8] & 3]));
}
#else
void SubScreen_DrawPanel(void *s, s32 id, s32 alpha, s32 layer);
void SubScreen_DrawPart(void *s, s32 x, s32 y, s32 part, s32 alpha, s32 top);
#endif

/* the fade: a black overlay over the menu for screens 0x80.., else the screen's panels
 * (kSubScreenFadePanels) drawn at the fade's alpha */
static void sub_draw_fade(SubScreen *s) {
    u8 kind;

    kind = s->kind;
    if (kind & 0x80) {
        /* a full-screen black overlay */
        u8 ov[0x100] __attribute__((aligned(16)));

        AT(ov, 0x24, s32) = 0;
        AT(ov, 0x0, void **) = Overlay_vtable;
        AT(ov, 0x4, s32) = -1;
        AT(ov, 0x10, s32) = -1;
        AT(ov, 0x14, u8) = 0;
        Overlay_SetColor(ov, (u32)s->fade << 24);
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, ov, 0x31, 0);
        AT(ov, 0x0, void **) = Helper469D00_vtable;
    } else {
        u8 alpha = s->fade;

        if (alpha != 0 && kind != 0xFF) {
            s8 *panel = kSubScreenFadePanels[kind & 0x7F];
            s32 i;

            for (i = 0; i < 4; i++, panel++) {
                if (*panel < 0) {
                    break;
                }
                SubScreen_DrawPanel(s, (u8)*panel, alpha, 0x31);
            }
        }
    }
}

/* the sound and music volume follow the fade (`vol` = 1 - fade / 255) */
static void sub_set_volume(SubScreen *s, f32 vol) {
    u8 *bgm;

    VCALL(gSound, 0x94, void (*)(VObject *, s32))(gSound, (u8)(0xFF - s->fade));
    bgm = gAdx;
    AT(bgm, 0x114, f32) = vol;
    if (vol < 0.0f) {
        AT(bgm, 0x114, f32) = 0.0f;
    }
    if (!(AT(bgm, 0x114, f32) <= 1.0f)) {
        AT(bgm, 0x114, f32) = 1.0f;
    }
    Bgm_ApplyVolume((Bgm *)bgm);
    if (gStageMusic != NULL) {
        VCALL(gStageMusic, 0x44, void (*)(VObject *, f32))(gStageMusic, vol);
    }
}

/* draw state: fade the screen in (the music and sound down with it); at full the screen takes
 * over (virtual +0x28, its texture uploaded) and the fade turns round (SubScreen_DrawFadeFromBlack) */
void SubScreen_DrawFadeIn(SubScreen *s) {
    f32 vol;

    s->fade += s->fadeStep;
    if (s->fade >= 0x80) {
        s->showBehind = 0;
        s->fade = 0x80;
        if (s->mode == 0
            || VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
            VCALL((VObject *)s, 0x28, void (*)(VObject *))((VObject *)s);
            if (s->mode != 0) {
                VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, s->pageTex, 0x19);
            }
            gLanguage = 2;
            s->fadeStep = -0x20;
            ptmf_set_fn(&s->draw, SubScreen_DrawFadeFromBlack);
        }
    }

    vol = 100.0f * (f32)(0xFF - s->fade) / 255.0f / 100.0f;
    sub_set_volume(s, vol);

    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

void SubScreen_DrawRun(SubScreen *s);

/* draw state: run the screen (+0x1708) while fading back from black; at 0 the fade is done
 * (SubScreen_DrawRun) */
void SubScreen_DrawFadeFromBlack(SubScreen *s) {

    s->fade += s->fadeStep;
    if (s->fade < 0) {
        s->fading = 0;
        s->fade = 0;
        s->quietClose = 0;
        s->close = 0;
        ptmf_set_fn(&s->draw, SubScreen_DrawRun);
    }
    ptmf_scall(s, &s->state);
    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* +0x28 the screen takes over */
void SubScreen_TakeOver(SubScreen *s) {
    s->tab = 0;
    s->unkA8DDE = 1;
}

/* (the scene) its part +0x97980's Map_TurnTo */
/* 0x00384C50 */
s32 SubScreen_Part97980(u8 *g, s32 a1) {
    return ((s32 (*)(u8 *, s32))Map_TurnTo)(g + 0x97980, a1);   /* (void: v0 as it was left) */
}

/* 0x00384C60 */
s32 SubScreen_Byte97A8F(u8 *g) {
    return AT(g, 0x97A8F, s8);
}

/* bit n of the scene's 0x97740 bitmap set / tested */
/* 0x00384C70 */
void SubScreen_SetBit(u8 *g, s32 n) {
    AT(g, 0x97740 + (n >> 5) * 4, u32) |= 1u << (n & 0x1F);
}

/* 0x00384CB0 */
s32 SubScreen_TestBit(u8 *g, s32 n) {
    return (AT(g, 0x97740 + (n >> 5) * 4, u32) & (1u << (n & 0x1F))) != 0;
}

void Options_StateLayout(SubScreen *s);
void Options_StateVibration(SubScreen *s);
void Options_StateSound(SubScreen *s);
void Options_StateVolume(SubScreen *s);
void Options_StatePosition(SubScreen *s);
void Options_StateDefaults(SubScreen *s);
void SubScreen_StateFiles(SubScreen *s);
void Options_Draw(SubScreen *s, s32 editing);

/* in-game, not in a special scene (gProgress +0x1FBEC1) */
static s32 sub_ingame_menu(SubScreen *s) {
    return gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 0 && s->mode == 0;
}

/* state: the options list (controller, vibration, sound, brightness / position, ...): up / down
 * choose of 5, confirm opens the entry's editor, the default button asks to restore the
 * defaults; in game the page buttons switch to the other menu pages */
void Options_StateList(SubScreen *s) {
    static void (*const sEditors[5])(SubScreen *) = {
        Options_StateLayout, Options_StateVibration, Options_StateSound, Options_StateVolume, Options_StatePosition,
    };

    if (!s->fading) {
        if (gMenuPressed & MENU_CONFIRM) {
            if (s->optCursor < 5) {
                ptmf_set_fn(&s->state, sEditors[s->optCursor]);
            }
            Sound_PlaySE(SE_DECIDE);
        } else if (gMenuPressed & MENU_UP) {
            if (s->optCursor != 0) {
                s->optCursor--;
            } else {
                s->optCursor = 4;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_DOWN) {
            s->optCursor++;
            if (s->optCursor >= 5) {
                s->optCursor = 0;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_DEFAULT) {
            Task_Open(&s->ask, 0x84);
            ptmf_set_fn(&s->state, Options_StateDefaults);
            Sound_PlaySE(SE_DECIDE);
        } else if ((gMenuPressed & MENU_NEXT) && sub_ingame_menu(s)) {
            s->unkA8C50 = 0;
            ptmf_set_fn(&s->state, SubScreen_StateItems);
            ptmf_set_fn(&s->resume, SubScreen_StateItems);
            Sound_PlaySE(SE_PAGE);
        } else if ((gMenuPressed & MENU_PREV) && sub_ingame_menu(s)) {
            ptmf_set_fn(&s->state, SubScreen_StateFiles);
            ptmf_set_fn(&s->resume, SubScreen_StateFiles);
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_CANCEL) {
            s->close = 1;
        }
    }
    Options_Draw(s, 0);
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, 0x82), 0x80, 0x30, 0x10, 0x15);
}

/* the first free (0) of the 128 halfwords from +0x15F8; 128 if none */
/* 0x003941C0 */
u32 SubScreen_FileCount(u8 *o) {
    u32 i;

    for (i = 0; i < 0x80; i++) {
        if (AT(o, 0x15F8 + i * 2, u16) == 0) {
            break;
        }
    }
    return i;
}

extern u16 D_0044B570[2][4][7];   /* [special scene][controller layout]: the action names */
extern u16 D_0044B558[5];         /* the options' rows (y) */

static const char sPosX[] = "X : %d";
static const char sPosY[] = "Y : %d";

/* one line of text in the options' text task */
static void opt_text(SubScreen *s, s32 x, s32 y, s32 color, s32 id) {
    Task *t = &s->text;

    Task_ShowText(t, x, y, color, Task_MessageText(t, id), 0x80, 0x30, 0x10, 0x15);
}

/* a line centred on x = 0x160 */
static void opt_text_centred(SubScreen *s, s32 y, s32 id) {
    s32 x = 0x160 - (Task_MessageWidth(&s->text, id, 0x10) >> 1);

    opt_text(s, x, y, 0x80, id);
}

/* draw the options: the panels, the controller layout's action names, vibration, sound output,
 * volume bar, screen position, and the cursor bar; `editing` highlights the current entry */
void Options_Draw(SubScreen *s, s32 editing) {
    u8 special = 0;
    u8 kind;
    u16 (*names)[7];
    s32 i, w;
    u16 bar;

    if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
        special = 1;
    }
    if (s->mode != 0) {
        s->kind = 0x8A;
    } else if (gProgress != NULL && AT(gProgress, 0x1FBEC1, u8) == 1) {
        s->kind = 0xA;
    } else {
        s->kind = 3;
    }
    kind = s->kind;
    if (kind != 0xFF) {
        s8 *panel = kSubScreenFadePanels[kind & 0x7F];

        for (i = 0; i < 4; i++, panel++) {
            if (*panel < 0) {
                break;
            }
            SubScreen_DrawPanel(s, (u8)*panel, 0x80, 0x30);
        }
    }

    /* controller layout */
    SubScreen_DrawPart(s, 0x20, 0x54, 0x10, 0x80, 0);
    opt_text(s, 0x48, 0x5A, s->optCursor == 0 && editing ? 0x82 : 0x80, 0x60);
    names = D_0044B570[special];
    w = 0x160 - (Task_MessageWidth(&s->text, 0x61, 0x10) >> 1);
    opt_text(s, w, 0x5A, 0x80, names[s->opt[6]][0]);
    opt_text(s, 0x40, 0x74, 0x80, 0x65);
    opt_text(s, 0x60, 0x74, 0x80, names[s->opt[6]][1]);
    opt_text(s, 0x40, 0x94, 0x80, 0x66);
    opt_text(s, 0x60, 0x94, 0x80, names[s->opt[6]][2]);
    opt_text(s, 0x40, 0xB4, 0x80, 0x67);
    opt_text(s, 0x60, 0xB4, 0x80, names[s->opt[6]][3]);
    opt_text(s, 0x40, 0xD4, 0x80, 0x68);
    opt_text(s, 0x60, 0xD4, 0x80, names[s->opt[6]][4]);
    opt_text(s, 0x110, 0x74, 0x80, 0x69);
    opt_text(s, 0x130, 0x74, 0x80, 0x73);
    opt_text(s, 0x110, 0x94, 0x80, 0x6A);
    opt_text(s, 0x130, 0x94, 0x80, 0x74);
    opt_text(s, 0x110, 0xB4, 0x80, 0x6B);
    opt_text(s, 0x130, 0xB4, 0x80, names[s->opt[6]][5]);
    opt_text(s, 0x110, 0xD4, 0x80, 0x6C);
    opt_text(s, 0x130, 0xD4, 0x80, names[s->opt[6]][6]);

    /* vibration */
    SubScreen_DrawPart(s, 0x20, 0xF4, 0x11, 0x80, 0);
    opt_text(s, 0x48, 0xFA, s->optCursor == 1 && editing ? 0x82 : 0x80, 0x77);
    opt_text_centred(s, 0xFA, s->opt[4] == 1 ? 0x78 : 0x79);

    /* sound output */
    SubScreen_DrawPart(s, 0x20, 0x114, 0x12, 0x80, 0);
    opt_text(s, 0x48, 0x11A, s->optCursor == 2 && editing ? 0x82 : 0x80, 0x7A);
    opt_text_centred(s, 0x11A, s->opt[0] == 0 ? 0x7B : s->opt[0] == 1 ? 0x7C : 0x7D);

    /* volume: a bar from 0x118 to 0x198 */
    SubScreen_DrawPart(s, 0x20, 0x134, 0x13, 0x80, 0);
    opt_text(s, 0x48, 0x13A, s->optCursor == 3 && editing ? 0x82 : 0x80, 0x7E);
    w = 0x118 - Task_MessageWidth(&s->text, 0x7F, 0x10);
    opt_text(s, w, 0x13A, 0x80, 0x7F);
    opt_text(s, 0x1A8, 0x13A, 0x80, 0x80);
    bar = (u32)(128.0f * s->optVolume);
    SubScreen_DrawPart(s, (u16)(bar + 0x118), 0x134, 0x19, 0x80, 0);

    /* screen position */
    SubScreen_DrawPart(s, 0x20, 0x154, 0x14, 0x80, 0);
    opt_text(s, 0x48, 0x15A, s->optCursor == 4 && editing ? 0x82 : 0x80, 0x81);
    Task_Printf(&s->text, 0x11C, 0x15A, 0x80, sPosX, s->opt[2]);
    Task_Printf(&s->text, 0x170, 0x15A, 0x80, sPosY, s->opt[3]);

    /* the cursor bar */
    SubScreen_DrawPart(s, 0x20, D_0044B558[s->optCursor], 0x1C, 0x80, 0);
}

void SubScreen_DrawFadeOut(SubScreen *s);

/* draw state: the screen is up; run it until it asks to close (+0xA8DE4, or in game the
 * menu button: Progress flag 4), then fade out (SubScreen_DrawFadeOut) */
void SubScreen_DrawRun(SubScreen *s) {
    Progress *p = gProgress;

    if (p != NULL && s->mode != 0) {
        Progress_ClearFlag(p, 4);
    }
    ptmf_scall(s, &s->state);
    if (s->close != 1 && (p == NULL || !Progress_TestFlag(p, 4))) {
        return;
    }
    if (p != NULL && AT(p, 0x1FBEC1, u8) == 0 && s->mode == 0) {
        s->resumeKind = s->kind;   /* reopen on this page */
    }
    s->fading = 1;
    s->fade = 0;
    s->fadeStep = 0x20;
    ptmf_set_fn(&s->draw, SubScreen_DrawFadeOut);
    if (p != NULL) {
        Progress_ClearFlag(p, 4);
    }
    if (s->quietClose == 0) {
        Sound_PlaySE(SE_CLOSE);
    }
}

void SubScreen_DrawFadeBack(SubScreen *s);

/* draw state: fade back to black over the screen; at black restore the menu's background
 * texture (the screens that replaced it) and fade the game back in (SubScreen_DrawFadeBack) */
void SubScreen_DrawFadeOut(SubScreen *s) {
    u8 mode;

    s->fade += s->fadeStep;
    if (s->fade < 0x80) {
        ptmf_scall(s, &s->state);
    } else {
        mode = s->mode;
        if (mode != 0 && mode != 5) {
            sub_free_vram();
            sub_load(s, sSubBack, s->pageTex);
        }
        gLanguage = 1;
        s->showBehind = 1;
        s->fade = 0x80;
        s->fadeStep = -0x10;
        mode = s->mode;
        if (mode == 0xB || mode == 8 || (u32)(mode - 0xD) < 3) {
            VCALL(gCamDirector, 0x30, void (*)(VObject *, s32))(gCamDirector, 0);
        }
        if (s->mode == 8) {
            s->fade = 0;
            Progress_SetFlag(gProgress, 8);
        }
        ptmf_set_fn(&s->draw, SubScreen_DrawFadeBack);
    }
    sub_draw_fade(s);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* draw state: fade the menu back in (once the background texture is loaded); at the end the
 * screen is closed (in game: Progress flag 4) and the next update opens it again
 * (SubScreen_Open) */
void SubScreen_DrawFadeBack(SubScreen *s) {
    f32 vol;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
        s->fade += s->fadeStep;
    }
    if (s->fade < 0) {
        s->fade = 0;
        VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, s->pageTex, 0x19);
        ptmf_set_fn(&s->draw, SubScreen_Open);
        if (gProgress != NULL) {
            Progress_SetFlag(gProgress, 4);
        } else {
            s->open = 0;
        }
        vol = 1.0f;
    } else {
        sub_draw_fade(s);
        if (gProgress != NULL) {
            Progress_ClearFlag(gProgress, 4);
        }
        vol = 100.0f * (f32)(0xFF - s->fade) / 255.0f / 100.0f;
    }
    sub_set_volume(s, vol);
}

/* state: the save-data screen (the card UI at +0xA8AC0) for loading; when it finishes
 * (+0xA8AC4 < 0) the screen closes */
void SubScreen_StateLoad(SubScreen *s) {

    s->kind = 0x85;
    if (s->card.state >= 0) {
        SaveScreen_Load(&s->card);
        return;
    }
    if (!s->fading) {
        s->close = 1;
    }
    SaveScreen_Draw(&s->card, 1);
}

/* the blinking of the editors' arrows: |0x80 - (frame * 4 & 0xFF)| */
static u8 opt_blink(SubScreen *s) {
    s32 a = 0x80 - (u8)(s->frame << 2);

    return a > 0 ? (u8)a : (u8)-a;
}

/* an editor's frame: the options with the entry lit, blinking arrows either side of the value,
 * the help line */
static void opt_editor_draw(SubScreen *s, s32 xl, s32 xr, s32 y) {
    u8 a;

    Options_Draw(s, 1);
    a = opt_blink(s);
    SubScreen_DrawPart(s, xl, y, 0x1A, a, 0);
    SubScreen_DrawPart(s, xr, y, 0x1B, a, 0);
    opt_text(s, 0x46, 0x186, 0x80, 0x83);
}

/* in-game menu button: closes the editor like cancel */
static s32 opt_cancelled(void) {
    return (gMenuPressed & MENU_CANCEL) || (gProgress != NULL && Progress_TestFlag(gProgress, 4));
}

/* leave an editor: back to the list */
static void opt_back(SubScreen *s, s32 se) {
    ptmf_set_fn(&s->state, Options_StateList);
    Sound_PlaySE(se);
}

/* editing the controller layout (4 types): left / right choose, confirm applies it, cancel
 * (or the in-game menu button) restores it */
void Options_StateLayout(SubScreen *s) {
    s8 *opt = (s8 *)gGamePtr + 0x30;

    if (!s->fading) {
        if (gMenuPressed & MENU_LEFT) {
            if (s->opt[6] != 0) {
                s->opt[6]--;
            } else {
                s->opt[6] = 3;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_RIGHT) {
            s->opt[6]++;
            if (s->opt[6] >= 4) {
                s->opt[6] = 0;
            }
            Sound_PlaySE(SE_CURSOR);
        }
        if (gMenuPressed & MENU_CONFIRM) {
            opt[6] = s->opt[6];
            VCALL((VObject *)s, 0x34, void (*)(VObject *, s32))((VObject *)s, s->opt[6]);
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            s->opt[6] = opt[6];
            opt_back(s, 0x2C);
        }
    }
    opt_editor_draw(s, 0x120, 0x180, 0x54);
}

/* the vibration's on / off: the actuator's +0x28 */
#define VIB_ENABLE(o, on) VCALL(o, 0x28, void (*)(VObject *, s32))(o, on)

/* editing the vibration: left / right toggle it (turning it on buzzes the pad) */
void Options_StateVibration(SubScreen *s) {
    s8 *opt = (s8 *)gGamePtr + 0x30;
    VObject *o;

    if (!s->fading) {
        if ((gMenuPressed & MENU_LEFT) || (gMenuPressed & MENU_RIGHT)) {
            o = gRumble;
            if (s->opt[4] == 1) {
                s->opt[4] = 0;
                VCALL(o, 0x10, void (*)(VObject *))(o);
                VIB_ENABLE(o, 0);
            } else {
                s->opt[4] = 1;
                VCALL(o, 0x10, void (*)(VObject *))(o);
                VIB_ENABLE(o, 1);
                VCALL(o, 0x14, void (*)(VObject *, s32, s32, s32))(o, 0, 1, 4);
                VCALL(o, 0x18, void (*)(VObject *, s32, s32, s32))(o, 0, 0x80, 4);
            }
            Sound_PlaySE(SE_CURSOR);
        }
        if (gMenuPressed & MENU_CONFIRM) {
            o = gRumble;
            opt[4] = s->opt[4];
            VCALL(o, 0x10, void (*)(VObject *))(o);
            VIB_ENABLE(o, opt[4] == 1);
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            o = gRumble;
            s->opt[4] = opt[4];
            VCALL(o, 0x10, void (*)(VObject *))(o);
            VIB_ENABLE(o, opt[4] == 1);
            opt_back(s, 0x2C);
        }
    }
    opt_editor_draw(s, 0xE0, 0x1C0, 0xF4);
}

/* the sound driver's output mode (0 mono, 1 stereo, 2 surround) */
#define SND_OUTPUT(o, mode) VCALL(o, 0x68, void (*)(VObject *, s32))(o, mode)

/* editing the sound output: left / right cycle it (heard at once) */
void Options_StateSound(SubScreen *s) {
    s8 *opt = (s8 *)gGamePtr + 0x30;

    if (!s->fading) {
        if (gMenuPressed & MENU_LEFT) {
            s->opt[0] = s->opt[0] == 0 ? 2 : s->opt[0] == 1 ? 0 : 1;
            SND_OUTPUT(gSound, s->opt[0]);
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_RIGHT) {
            s->opt[0] = s->opt[0] == 0 ? 1 : s->opt[0] == 1 ? 2 : 0;
            SND_OUTPUT(gSound, s->opt[0]);
            Sound_PlaySE(SE_CURSOR);
        }
        if (gMenuPressed & MENU_CONFIRM) {
            opt[0] = s->opt[0];
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            s->opt[0] = opt[0];
            SND_OUTPUT(gSound, s->opt[0]);
            opt_back(s, 0x2C);
        }
    }
    opt_editor_draw(s, 0xE0, 0x1C0, 0x114);
}

/* editing the volume: left / right in steps of 1/64 (heard at once) */
void Options_StateVolume(SubScreen *s) {
    u8 *opt = (u8 *)gGamePtr + 0x30;
    VObject *snd;
    f32 v;

    if (!s->fading) {
        if (gMenuPressed & MENU_LEFT) {
            if (!(s->optVolume < 0.0f)) {
                s->optVolume = v = s->optVolume - 0.015625f;
                if (v < 0.0f) {
                    s->optVolume = 0.0f;
                }
                snd = gSound;
                VCALL(snd, 0xA8, void (*)(VObject *, f32))(snd, s->optVolume);
                Sound_Play(snd, SE_CURSOR, SE_BANK_MENU);
            }
        } else if (gMenuPressed & MENU_RIGHT) {
            if (s->optVolume < 1.0f) {
                s->optVolume = v = s->optVolume + 0.015625f;
                if (!(v < 1.0f)) {
                    s->optVolume = 1.0f;
                }
                snd = gSound;
                VCALL(snd, 0xA8, void (*)(VObject *, f32))(snd, s->optVolume);
                Sound_Play(snd, SE_CURSOR, SE_BANK_MENU);
            }
        }
        if (gMenuPressed & MENU_CONFIRM) {
            snd = gSound;
            v = s->optVolume;
            AT(opt, 8, f32) = v;
            opt_apply_volume(snd, v);
            Sound_Play(snd, SE_DECIDE, SE_BANK_MENU);
            ptmf_set_fn(&s->state, Options_StateList);
        } else if (opt_cancelled()) {
            snd = gSound;
            s->optVolume = AT(opt, 8, f32);
            opt_apply_volume(snd, AT(opt, 8, f32));
            Sound_Play(snd, SE_CANCEL, SE_BANK_MENU);
            ptmf_set_fn(&s->state, Options_StateList);
        }
    }
    opt_editor_draw(s, 0xE0, 0x1C0, 0x134);
}

#define SCREEN_POS(x, y) VCALL(gRenderer, 0x30, void (*)(VObject *, s32, s32))(gRenderer, x, y)

/* editing the screen position: the D-pad moves it (-32..32 each way, seen at once) */
void Options_StatePosition(SubScreen *s) {
    s8 *opt = (s8 *)gGamePtr + 0x30;
    s32 a;

    if (!s->fading) {
        u32 pad = gMenuPressed;

        if (pad & MENU_UP) {
            if (s->opt[3] >= -0x1F) {
                s->opt[3]--;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        } else if (pad & MENU_DOWN) {
            if (s->opt[3] < 0x20) {
                s->opt[3]++;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        }
        pad = gMenuPressed;
        if (pad & MENU_LEFT) {
            if (s->opt[2] >= -0x1F) {
                s->opt[2]--;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        } else if (pad & MENU_RIGHT) {
            if (s->opt[2] < 0x20) {
                s->opt[2]++;
                SCREEN_POS(s->opt[2], s->opt[3]);
                Sound_PlaySE(SE_CURSOR);
            }
        }
        if (gMenuPressed & MENU_CONFIRM) {
            opt[2] = s->opt[2];
            opt[3] = s->opt[3];
            opt_back(s, 0x2B);
        } else if (opt_cancelled()) {
            s->opt[2] = opt[2];
            s->opt[3] = opt[3];
            SCREEN_POS(opt[2], opt[3]);
            opt_back(s, 0x2C);
        }
    }
    Options_Draw(s, 1);
    /* the corner marks pulse */
    a = 0x40 - ((s->frame << 1) & 0x7F);
    a = (a > 0 ? a : -a) + 0x20;
    SubScreen_DrawPart(s, 0x10, 0x10, 0x15, (u8)a, 0);
    SubScreen_DrawPart(s, 0x1D2, 0x10, 0x16, (u8)a, 0);
    SubScreen_DrawPart(s, 0x10, 0x190, 0x17, (u8)a, 0);
    SubScreen_DrawPart(s, 0x1D2, 0x190, 0x18, (u8)a, 0);
    opt_text(s, 0x46, 0x186, 0x80, 0x83);
}

/* the "restore the defaults?" question (message 0x84): on yes the options go back to mono,
 * vibration on, full volume, centred, layout A, and are applied */
void Options_StateDefaults(SubScreen *s) {
    Task *t = &s->ask;

    Options_Draw(s, 0);
    if (t->mode) {
        Task_Run(t);
        return;
    }
    if (t->answer == 0) {
        s8 *sys = gGamePtr;
        VObject *o;

        sys[0x36] = 0;
        sys[0x34] = 1;
        sys[0x30] = 1;
        AT(sys, 0x38, f32) = 1.0f;
        sys[0x32] = 0;
        sys[0x33] = 0;
        sub_copy_options(s);
        VCALL((VObject *)s, 0x34, void (*)(VObject *, s32))((VObject *)s, sys[0x36]);
        o = gRumble;
        VCALL(o, 0x10, void (*)(VObject *))(o);
        VIB_ENABLE(o, sys[0x34] == 1);
        o = gSound;
        SND_OUTPUT(o, sys[0x30]);
        opt_apply_volume(o, AT(sys, 0x38, f32));
        SCREEN_POS(sys[0x32], sys[0x33]);
    }
    ptmf_set_fn(&s->state, Options_StateList);
}

/* add `id` to the list at +0x15F8 (128 u16, 0-terminated): 1 if added, 0 if already there or
 * the list is full */
/* 0x00394200 */
s32 SubScreen_AddFile(SubScreen *s, u32 id) {
    u16 *list = &AT(s, 0x15F8, u16);
    u32 i;

    for (i = 0; i < 0x80; i++) {
        if (list[i] == (u16)id) {
            return 0;
        }
        if (list[i] == 0) {
            list[i] = id;
            return 1;
        }
    }
    return 0;
}

/* +0x24 the in-game tab, drawn each gameplay frame: base parts 0xC (at y 0x60) and 0xB (at y
 * 0x8C) slide in from the left (state 2) as page[0x15C] grows to 0x40, stay (3) and slide out
 * to the right (4), fading with the slide */
/* 0x00384CE0 */
void SubScreen_DrawTab(SubScreen *s) {
    u8 t = s->page[0x15C];

    switch (s->tab) {
    case 2:
        SubScreen_DrawPart(s, (u16)(0xAC - (0x40 - t)), 0x60, 0xC, (u8)(t * 2), 0);
        SubScreen_DrawPart(s, (u16)(0xC0 - (0x40 - s->page[0x15C])), 0x8C, 0xB, s->page[0x15C], 0);
        break;
    case 3:
        SubScreen_DrawPart(s, 0xAC, 0x60, 0xC, 0x80, 0);
        SubScreen_DrawPart(s, 0xC0, 0x8C, 0xB, 0x40, 0);
        break;
    case 4:
        SubScreen_DrawPart(s, (u16)(0xEC - t), 0x60, 0xC, (u8)(t * 2), 0);
        SubScreen_DrawPart(s, (u16)(0x100 - s->page[0x15C]), 0x8C, 0xB, s->page[0x15C], 0);
        break;
    }
}

/* ---- text helpers left (2026-10-05); the sub-screen's text task at +0x97868 ---- */

extern const char str_N_N[];

#define SUB_TEXT(s) ((Task *)((u8 *)(s) + 0x97868))

/* the page counter at the top right: "<page + 1> / <count>" (str_N_N) and its caption
 * (message 0x1C6), faded by +0xA8C62 */
/* 0x0037E480 */
void SubScreen_DrawPageCount(u8 *s) {
    char buf[16];
    u8 *msg;

    func_0026EDD0(buf, 10, str_N_N, s[0xA8C81] + 1, SubScreen_EntryMotions(s, s[0xA8C80]) & 0xFF);
    Task_ShowText(SUB_TEXT(s), 0x1B8, 0x10, 0, (u8 *)buf, 0x80 - AT(s, 0xA8C62, s16), 0x33, 0x10, 0x15);
    msg = Task_MessageText(SUB_TEXT(s), 0x1C6);
    Task_ShowText(SUB_TEXT(s), 0x160, 0x10, 0, msg, 0x80 - AT(s, 0xA8C62, s16), 0x33, 0x10, 0x15);
}

/* the two lines of the current entry (+0x15F8, two message ids per entry +0xA8C55; 0 none) at
 * x 0x2D, y 0x54 / 0xF4 */
/* 0x0037E580 */
void SubScreen_DrawFileLines(u8 *s) {
    s32 i;

    for (i = 0; i < 2; i++) {
        u16 id = AT(s, 0x15F8 + (s[0xA8C55] * 2 + i) * 2, u16);

        if (id != 0) {
            Task_ShowText(SUB_TEXT(s), 0x2D, 0x54 + i * 0xA0, 0x80, Task_MessageText(SUB_TEXT(s), id), 0x80, 0x30,
                          0x10, 0x15);
        }
    }
}

/* the file's last page: its entries (+0x15F8, SubScreen_FileCount) two to a page */
/* 0x0037E650 */
u8 SubScreen_FileLastPage(u8 *s) {
    return (u8)(((s32)SubScreen_FileCount(s) - 1) / 2);
}

/* its items (+0x8) Items_UseEquipped with 2 */
/* 0x00384C20 */
void SubScreen_UseEquipped(u8 *o) {
    Items_UseEquipped(o + 0x8, 2);
}

/* ---- the in-game menu's item page ---- */

extern void Loader_FreePictureArea(void);
extern void SubScreen_DrawItemGrid(SubScreen *s, s32 a);               /* the item grid */
extern const PTMF SubScreen_StateItemQuestion_ptmf, SubScreen_StateItemActions_ptmf, SubScreen_StateMap_ptmf, SubScreen_StateMap_ptmf2, Options_StateList_ptmf, Options_StateList_ptmf2;

#define SUB_LIST(s) ((s)->unkA8C50)              /* the item list shown: 0 items, 1 / 2 the others */
#define SUB_CURSOR(s) ((s)->unkA8C51[SUB_LIST(s)])   /* its cursor: 16 a page, rows of 8 */

/* The item page: the cursor moves along its row of 8 (up / down, round) and between the rows
 * and pages (left / right, round); confirm uses the item (kind 7: a question, SubScreen_StateItemQuestion_ptmf; else
 * started, SubScreen_StateItemActions_ptmf); list 1 sorts with the default button; next / previous turn to the
 * other lists and then the map page (SubScreen_StateMap_ptmf / SubScreen_StateMap_ptmf2) or the page before (Options_StateList_ptmf /
 * Options_StateList_ptmf2); cancel closes. Then its panels, the grid and the help line. */
/* 0x00396900 */
void SubScreen_StateItems(SubScreen *s) {
    u8 *items = s->pool;

    if (!s->fading) {
        u32 n = Items_FirstFree(items, SUB_LIST(s));
        s32 last = n != 0 ? (s32)(n - 1) / 16 : 0;
        u32 pad = gMenuPressed;
        u8 *cur = &SUB_CURSOR(s);
        s32 c = *cur;
        u8 old = c;

        if (pad & MENU_CONFIRM) {
            if (Items_Id(items, SUB_LIST(s), SUB_CURSOR(s)) != -1) {
                if (Items_Kind(items, SUB_LIST(s), SUB_CURSOR(s)) == 7) {
                    Task_Open(&s->ask, Items_Id(items, SUB_LIST(s), SUB_CURSOR(s)) & 0xFFFF);
                    ptmf_set(&s->state, &SubScreen_StateItemQuestion_ptmf);
                } else {
                    s->padA8C54 = 0;
                    s->page[0x15C] = 0;
                    Loader_FreePictureArea();
                    Items_StartUse(items, SUB_LIST(s), SUB_CURSOR(s), (u8 *)s + 0x94F40);
                    ptmf_set(&s->state, &SubScreen_StateItemActions_ptmf);
                }
                Sound_PlaySE(SE_DECIDE);
            }
        } else if (pad & MENU_UP) {
            *cur = c != 0 ? (c - 1) % 8 + (c >> 3) * 8 : 7;
        } else if (pad & MENU_DOWN) {
            *cur = (c + 1) % 8 + (c >> 3) * 8;
        } else if (pad & MENU_LEFT) {
            if (c < 8) {
                *cur = c + ((last + 1) * 16 - 8);
            } else {
                *cur -= 8;
            }
        } else if (pad & MENU_RIGHT) {
            if (last < (c + 8) / 16) {
                *cur = c % 8;
            } else {
                *cur += 8;
            }
        } else if (SUB_LIST(s) == 1 && (pad & MENU_DEFAULT)) {
            Items_Sort(items, SUB_LIST(s));
            Sound_PlaySE(SE_DECIDE);
        } else if (gMenuPressed & MENU_NEXT) {
            if (SUB_LIST(s) < 2) {
                SUB_LIST(s)++;
                old = SUB_CURSOR(s);
            } else {
                Map_BackToPlayer(s->textObj);
                ptmf_set(&s->state, &SubScreen_StateMap_ptmf);
                ptmf_set(&s->resume, &SubScreen_StateMap_ptmf2);
            }
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_PREV) {
            if (SUB_LIST(s) != 0) {
                SUB_LIST(s)--;
                old = SUB_CURSOR(s);
            } else {
                ptmf_set(&s->state, &Options_StateList_ptmf);
                ptmf_set(&s->resume, &Options_StateList_ptmf2);
            }
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_CANCEL) {
            s->close = 1;
        }
        if (old != SUB_CURSOR(s)) {
            Sound_PlaySE(SE_CURSOR);
        }
    }
    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    if (s->kind != 0xFF) {
        s8 *panel = kSubScreenFadePanels[s->kind & 0x7F];
        s32 i;

        for (i = 0; i < 4 && panel[i] >= 0; i++) {
            SubScreen_DrawPanel(s, (u8)panel[i], 0x80, 0x30);
        }
    }
    SubScreen_DrawItemGrid(s, 1);
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, SUB_LIST(s) == 1 ? 0x5B : 0x5D), 0x80, 0x30,
                  0x10, 0x15);
}

extern const char kFmtSlash[];   /* "%2d/%2d" */
extern const char kFmtString[];   /* "%s" */
extern const char str_xN_3[];   /* "x%2d" */

/* The item grid of the list shown: the page (16 places, two columns of 8 down the screen) the
 * cursor is on (past the last: the last item), its number of the pages and, with more than
 * one, the two arrows blinking; each item's icon (by kind), name (message 0x8100 + id; the
 * word plate's word in parameter 3) in grey or, equipped, highlighted, and its count; the
 * cursor. */
/* 0x003949B0 */
void SubScreen_DrawItemGrid(SubScreen *s, s32 a) {
    u8 *items = s->pool;
    u32 n = Items_FirstFree(items, SUB_LIST(s));
    s32 last = n != 0 ? (s32)(n - 1) / 16 : 0;
    s32 i;

    if (last < (s32)(SUB_CURSOR(s) >> 4)) {
        SUB_CURSOR(s) = Items_FirstFree(items, SUB_LIST(s)) - 1;
    }
    Task_Printf(&s->text, 0x186, 0x176, 0x80, kFmtSlash, (SUB_CURSOR(s) >> 4) + 1, last + 1);
    if (last != 0) {
        s32 b = 0x80 - ((s->frame << 2) & 0xFF);
        u8 alpha = b > 0 ? b : -b;

        SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
        SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
    }
    for (i = 0; i < 16; i++) {
        u8 k = i + (SUB_CURSOR(s) >> 4) * 16;
        s32 id = Items_Id(items, SUB_LIST(s), k);
        u8 color = (s8)Items_EquipState(items, SUB_LIST(s), k) == 1 ? 0x82 : 0x80;

        if (id != -1) {
            s32 x = (i / 8) * 0xDA, y = (i % 8) * 35;

            SubScreen_DrawPart(s, (u16)(x + 0x20), (u16)(y + 0x5E), (u8)Items_Kind(items, SUB_LIST(s), k), 0x80, 0);
            if (id == 0x3F) {
                const u8 *w = (const u8 *)Items_Field20(items, SUB_LIST(s), k);
                char name[9];
                s32 j;

                for (j = 0; j < 8; j++) {
                    name[j] = w[j];
                }
                name[8] = 0;
                Msg_PrintfParam(&s->text, 3, kFmtString, name);
            }
            Task_ShowText(&s->text, x + 0x46, y + 0x64, color, Task_MessageText(&s->text, (id + 0x8100) & 0xFFFF), 0x80,
                          0x30, 0x10, 0x15);
            if (Items_IsCounted(items, SUB_LIST(s), k) == 1) {
                Task_Printf(&s->text, x + 0xDE, y + 0x64, color, str_xN_3, Items_HowMany(items, SUB_LIST(s), k));
            }
        }
        if (SUB_CURSOR(s) % 16 == i) {
            SubScreen_DrawPart(s, (u16)((i / 8) * 0xDA + 0x20), (u16)((i % 8) * 35 + 0x5E), 0x1C, 0x80, 0);
        }
    }
}

/* ---- the in-game menu's map page ---- */

extern u8 Progress_MapsHeld(void);      /* the maps the player has (bits) */
extern const PTMF SubScreen_StateFiles_ptmf, SubScreen_StateFiles_ptmf2, SubScreen_StateItems_ptmf11, SubScreen_StateItems_ptmf12;

/* draw the screen kind `kind`'s panels */
static inline void sub_panels(SubScreen *s) {
    if (s->kind != 0xFF) {
        s8 *panel = kSubScreenFadePanels[s->kind & 0x7F];
        s32 i;

        for (i = 0; i < 4 && panel[i] >= 0; i++) {
            SubScreen_DrawPanel(s, (u8)panel[i], 0x80, 0x30);
        }
    }
}

/* The map page: next / previous turn to the page after (SubScreen_StateFiles_ptmf / SubScreen_StateFiles_ptmf2) or back to
 * the item lists' last (SubScreen_StateItems_ptmf11 / SubScreen_StateItems_ptmf12); cancel closes. Its panels, the map and,
 * when there are other maps to turn to, the two arrows blinking. */
/* 0x00394670 */
void SubScreen_StateMap(SubScreen *s) {
    if (!s->fading) {
        if (gMenuPressed & MENU_NEXT) {
            ptmf_set(&s->state, &SubScreen_StateFiles_ptmf);
            ptmf_set(&s->resume, &SubScreen_StateFiles_ptmf2);
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_PREV) {
            SUB_LIST(s) = 2;
            ptmf_set(&s->state, &SubScreen_StateItems_ptmf11);
            ptmf_set(&s->resume, &SubScreen_StateItems_ptmf12);
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_CANCEL) {
            s->close = 1;
        }
    }
    s->kind = 1;
    sub_panels(s);
    Map_PageFrame(s->textObj);
    {
        u8 maps = Progress_MapsHeld();

        if (maps != 0 && !(maps == 4 && AT(s->textObj, 0x10C, s8) == 2)) {
            s32 b = 0x80 - ((s->frame << 2) & 0xFF);
            u8 alpha = b > 0 ? b : -b;

            SubScreen_DrawPart(s, 0xA, 0xF0, 0x1A, alpha, 0);
            SubScreen_DrawPart(s, 0x1D6, 0xF0, 0x1B, alpha, 0);
        }
    }
}

/* ---- the in-game menu's file page ---- */

extern const PTMF Options_StateList_ptmf3, Options_StateList_ptmf4, SubScreen_StateMap_ptmf3, SubScreen_StateMap_ptmf4;

#define SUB_FILE_PAGE(s) ((s)->unkA8C55)

/* The file page: left / right turn its pages (round), next / previous turn to the page after
 * (Options_StateList_ptmf3 / Options_StateList_ptmf4) or back to the map (SubScreen_StateMap_ptmf3 / SubScreen_StateMap_ptmf4); cancel closes. Its
 * panels, the page number and, with more than one, the arrows blinking; the file. */
/* 0x00394260 */
void SubScreen_StateFiles(SubScreen *s) {
    u8 last = SubScreen_FileLastPage((u8 *)s);

    if (!s->fading) {
        u8 old = SUB_FILE_PAGE(s);

        if (gMenuPressed & MENU_LEFT) {
            if (old != 0) {
                SUB_FILE_PAGE(s)--;
            } else {
                SUB_FILE_PAGE(s) = last;
            }
        } else if (gMenuPressed & MENU_RIGHT) {
            if (old < last) {
                SUB_FILE_PAGE(s)++;
            } else {
                SUB_FILE_PAGE(s) = 0;
            }
        } else if (gMenuPressed & MENU_NEXT) {
            ptmf_set(&s->state, &Options_StateList_ptmf3);
            ptmf_set(&s->resume, &Options_StateList_ptmf4);
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_PREV) {
            Map_BackToPlayer(s->textObj);
            ptmf_set(&s->state, &SubScreen_StateMap_ptmf3);
            ptmf_set(&s->resume, &SubScreen_StateMap_ptmf4);
            Sound_PlaySE(SE_PAGE);
        } else if (gMenuPressed & MENU_CANCEL) {
            s->close = 1;
        }
        if (old != SUB_FILE_PAGE(s)) {
            Sound_PlaySE(SE_CURSOR);
        }
    }
    s->kind = 2;
    sub_panels(s);
    Task_Printf(&s->text, 0x186, 0x186, 0x80, kFmtSlash, SUB_FILE_PAGE(s) + 1, last + 1);
    if (last != 0) {
        s32 b = 0x80 - ((s->frame << 2) & 0xFF);
        u8 alpha = b > 0 ? b : -b;

        SubScreen_DrawPart(s, 0x168, 0x180, 0x1A, alpha, 0);
        SubScreen_DrawPart(s, 0x1B3, 0x180, 0x1B, alpha, 0);
    }
    SubScreen_DrawFileLines((u8 *)s);
}

/* ---- costumes ---- */

/* the costume worn (equipment slot 2, items 0x90..0x9B) as 0..8; -1 none */
/* 0x00385370 */
s32 SubScreen_Costume(SubScreen *s) {
    static const s8 kIndex[12] = {0, 1, 2, 3, 4, 5, -1, 6, 7, -1, -1, 8};
    u32 k = Items_Equipped(s->pool, 2) - 0x90;

    return k < 12 ? kIndex[k] : -1;
}

/* the costume worn as its model variant 0x31..0x33; 0 none */
/* 0x00385410 */
s32 SubScreen_CostumeModel(SubScreen *s) {
    static const u8 kModel[12] = {0x31, 0x32, 0x33, 0x31, 0x32, 0x33, 0, 0x32, 0x33, 0, 0, 0x33};
    u32 k = Items_Equipped(s->pool, 2) - 0x90;

    return k < 12 ? kModel[k] : 0;
}

extern void SubScreen_DrawEntries(SubScreen *s);
extern const PTMF SubScreen_StateEntries_ptmf2;

/* state: a question being asked (its page drawn, SubScreen_DrawEntries); once answered, SubScreen_StateEntries_ptmf2 */
/* 0x00385F60 */
void SubScreen_StateQuestion(SubScreen *s) {
    SubScreen_DrawEntries(s);
    Task_Run(&s->ask);
    if (AT(&s->ask, 0x10, u8) == 0) {
        ptmf_set(&s->state, &SubScreen_StateEntries_ptmf2);
    }
}

/* ---- page openings and states ---- */

extern void Gallery_DrawArtList(SubScreen *s);
extern const PTMF Gallery_StatePicture_ptmf;

/* open the page: its start values (+4 / +6 0x100 / 0xE0, +9 set), fading in (step 0x10), set
 * up (Gallery_DrawArtList), then state Gallery_StatePicture_ptmf */
/* 0x00387E00 */
void SubScreen_StatePageOpen(SubScreen *s) {
    SUB_PAGE(s, 0x4, s16) = 0x100;
    SUB_PAGE(s, 0x6, s16) = 0xE0;
    SUB_PAGE(s, 0x8, u8) = 0;
    SUB_PAGE(s, 0x9, u8) = 1;
    SUB_PAGE(s, 0xA, u8) = 0;
    SUB_PAGE(s, 0xB, u8) = 0;
    SUB_PAGE(s, 0xC, u8) = 0;
    s->fading = 1;
    s->fade = 0;
    s->fadeStep = 0x10;
    Gallery_DrawArtList(s);
    ptmf_set(&s->state, &Gallery_StatePicture_ptmf);
}

/* state: the page's own object (+0xA8C80, screen kind 0x86) runs (+0x10); when it is done
 * (+0x158) it ends (+0x14) and progress flag 4 is set */
/* 0x00390790 */
void SubScreen_StatePageObject(SubScreen *s) {
    VObject *o = (VObject *)s->page;

    s->kind = 0x86;
    sub_panels(s);
    if (o != NULL) {
        VCALL(o, 0x10, void (*)(VObject *))(o);
    }
    if (s->fading || (o != NULL && AT(o, 0x158, u8) == 0)) {
        return;
    }
    if (o != NULL) {
        VCALL(o, 0x14, void (*)(VObject *))(o);
    }
    Progress_SetFlag(gProgress, 4);
}

/* state: the save screens (screen kind 0x85): done (card state < 0) it closes (progress flag 4)
 * and stays drawn; else the card runs (flag 4 cleared) */
/* 0x003912E0 */
void SubScreen_StateSave(SubScreen *s) {
    s->kind = 0x85;
    if (s->card.state < 0) {
        if (!s->fading) {
            s->close = 1;
            if (gProgress != NULL) {
                Progress_SetFlag(gProgress, 4);
            }
        }
        SaveScreen_Draw(&s->card, 0);
        return;
    }
    BootCard_StateSave(&s->card);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
}

/* is extra `k` unlocked (the system data's +0x24 / +0x2C flags) */
/* 0x0038DF90 */
s32 SubScreen_ExtraUnlocked(SubScreen *s, u8 k) {
    u32 f24 = AT(gGamePtr, 0x24, u32), f2C;

    switch (k) {
    case 3:
        return (f24 & 0x2) != 0;
    case 5:
        return (f24 & 0x8) != 0;
    case 8:
        f2C = AT(gGamePtr, 0x2C, u32);
        if (!(f2C & 0x400000)) {
            return 0;
        }
        return (f24 & 0x100) || (f24 & 0x1000);
    case 10: case 12: case 14:
    case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
        return (AT(gGamePtr, 0x2C, u32) & 0x400000) != 0;
    case 16: case 18: case 20: case 22:
        return (AT(gGamePtr, 0x2C, u32) & 0x2000000) != 0;
    case 15: case 17: case 19: case 21: case 23:
        return (AT(gGamePtr, 0x2C, u32) & 0x200000) != 0;
    }
    return (f24 & 0x1) != 0;
}

extern u16 kExtraUnlocks[][4];   /* per entry: its unlock flag (system data +0x24 bits), .., .., the
                                 message to follow (0xFFFF none) */
extern const PTMF SubScreen_StateQuestion_ptmf, SubScreen_StateEntries_ptmf;

/* state: a question being asked; once answered, with the page's entry (page[0]) unlocked and a
 * message to follow, that message (SubScreen_StateQuestion_ptmf), else SubScreen_StateEntries_ptmf */
/* 0x00386000 */
void SubScreen_StateEntryQuestion(SubScreen *s) {
    SubScreen_DrawEntries(s);
    Task_Run(&s->ask);
    if (AT(&s->ask, 0x10, u8) == 0) {
        u16 *e = kExtraUnlocks[SUB_PAGE(s, 0x0, u8)];

        if ((AT(gGamePtr, 0x24 + (e[0] >> 5) * 4, u32) & (1 << (e[0] & 0x1F))) && e[3] != 0xFFFF) {
            Task_Open(&s->ask, e[3]);
            ptmf_set(&s->state, &SubScreen_StateQuestion_ptmf);
        } else {
            ptmf_set(&s->state, &SubScreen_StateEntries_ptmf);
        }
    }
}

/* the menu's state from a save `save`: the three lists' items (ids +0x1068, 0xFFFF none; their
 * data +0x11E8), the four equipped (+0x17E8: list << 8 | place), nine words (+0x1044, to
 * +0x97740) and the file's entries (+0x17F0, each given to vtable +0xC) */
/* 0x00385030 */
void SubScreen_FromSave(SubScreen *s, u8 *save) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 0x40; i++) {
            u16 id = AT(save, 0x1068 + l * 0x80 + i * 2, u16);

            if (id != 0xFFFF) {
                Items_GiveWithData(s->pool, id, (u64 *)(save + 0x11E8 + l * 0x200 + i * 8));
            }
        }
    }
    for (i = 0; i < 4; i++) {
        u16 v = AT(save, 0x17E8 + i * 2, u16);

        if (v != 0xFFFF) {
            Items_Equip(s->pool, (v >> 8) & 0xFF, v & 0xFF);
        }
    }
    for (i = 0; i < 9; i++) {
        s->unk97740[i] = AT(save, 0x1044 + i * 4, s32);
    }
    for (i = 0; i < 0x80; i++) {
        u16 v = AT(save, 0x17F0 + i * 2, u16);

        if (v != 0) {
            VCALL(s, 0xC, void (*)(SubScreen *, u32))(s, v);
        }
    }
}

/* the menu's state into a save `save` (as SubScreen_FromSave reads it back) */
/* 0x003851B0 */
void SubScreen_ToSave(SubScreen *s, u8 *save) {
    u32 l, i;

    for (i = 0; i < 4; i++) {
        AT(save, 0x17E8 + i * 2, u16) = 0xFFFF;
    }
    for (l = 0; l < 3; l++) {
        for (i = 0; i < 0x40; i++) {
            AT(save, 0x1068 + l * 0x80 + i * 2, u16) = Items_Id(s->pool, l, i);
            if (AT(save, 0x1068 + l * 0x80 + i * 2, u16) != 0xFFFF) {
                AT(save, 0x11E8 + l * 0x200 + i * 8, u64) = *Items_Data(s->pool, l, i);
                if ((s8)Items_EquipState(s->pool, l, i) == 1) {
                    AT(save, 0x17E8 + Items_KindSlot(s->pool, l, i) * 2, u16) = (u8)l << 8 | (u8)i;
                }
            }
        }
    }
    for (i = 0; i < 9; i++) {
        AT(save, 0x1044 + i * 4, s32) = s->unk97740[i];
    }
    for (i = 0; i < 0x80; i++) {
        AT(save, 0x17F0 + i * 2, u16) = s->unk15F8[i];
    }
}

/* the galleries' files of entry `k`: the model (D_0044B7B0, ".PCK") and its texture
 * (D_0044B830, ".TEX") to the buffers at +0xA8DEC / +0xA8DF0, then, when they have one, a
 * marker file (D_0044B930, ".MRK") to +0xA8DF8 and a second texture (pstr_O_FIN_FIN_200) to +0xA8DF4
 * (else those are cleared) */
extern const char *D_0044B7B0[], *D_0044B830[], *pstr_O_FIN_FIN_200[], *D_0044B930[];
extern const char str_N_PCK[], str_N_TEX[], str_N_MRK[];   /* "%s.PCK", "%s.TEX", "%s.MRK" */

/* 0x0038D620 */
void Gallery_LoadPck(SubScreen *s, u8 k) {
    VObject *ld = gFileLoader;
    char name[0x20];

    func_0026EDD0(name, 0x20, str_N_PCK, D_0044B7B0[k]);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, AT(s, 0xA8DEC, void *), 0x10000000, 0);
    func_0026EDD0(name, 0x20, str_N_TEX, D_0044B830[k]);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, AT(s, 0xA8DF0, void *), 0x10000000, 0);
    if (D_0044B930[k][0] != 0) {
        func_0026EDD0(name, 0x20, str_N_MRK, D_0044B930[k]);
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, AT(s, 0xA8DF8, void *), 0x10000000,
                                                                           0);
    } else {
        AT(s, 0xA8DF8, void *) = NULL;
    }
    if (pstr_O_FIN_FIN_200[k][0] != 0) {
        func_0026EDD0(name, 0x20, str_N_TEX, pstr_O_FIN_FIN_200[k]);
        VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, name, AT(s, 0xA8DF4, void *), 0x10000000,
                                                                           0);
    } else {
        AT(s, 0xA8DF4, void *) = NULL;
    }
}

extern const PTMF SubScreen_StateItems_ptmf10;

/* state: the item page with a question up: its panels, the grid, a box and the question; once
 * answered, the item under the cursor is used (Items_Use), SubScreen_StateItems_ptmf10 */
/* 0x00394E40 */
void SubScreen_StateItemQuestion(SubScreen *s) {
    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    sub_panels(s);
    SubScreen_DrawItemGrid(s, 1);
    Task_DrawBox(&s->text, 0x100, 0xE0, 0x1A0, 0x160, 0x60, 0x30);
    Task_Run(&s->ask);
    if (AT(&s->ask, 0x10, u8) == 0) {
        Items_Use(s->pool, SUB_LIST(s), SUB_CURSOR(s));
        ptmf_set(&s->state, &SubScreen_StateItems_ptmf10);
    }
}

#define SUB_SLIDE(s) ((s)->page[0x15C])   /* the tab's slide, 0..0x40 by 4 */

/* +0x20 the in-game tab: `cmd` 0 the item `id` started (Items_Notify), slide reset; 1 waiting
 * for it (when it is done, the menu's textures made resident again); 2 sliding in, 3 shown, 4
 * sliding out (at the end the screen takes over, +0x28). 1 while it moves; nothing (0) while
 * +0xA8DDE is set */
/* 0x00384E60 */
s32 SubScreen_TabCommand(SubScreen *s, u8 cmd, s32 id) {
    VObject *tc;

    if (cmd != 0 && s->unkA8DDE == 1) {
        return 0;
    }
    s->tab = cmd;
    switch (cmd) {
    case 0:
        Items_Notify(s->pool, id, (s32)((u8 *)s + 0x94F40));
        SUB_SLIDE(s) = 0;
        s->unkA8DDE = 0;
        return 0;
    case 1:
        if (SubScreen_LoaderIdle(s->pool) != 0) {
            return 1;
        }
        tc = gTexCache;
        VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, s->baseTex, 0x18);
        VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, (u8 *)s + 0x94F40, 0x27);
        return 0;
    case 2:
        if (SUB_SLIDE(s) < 0x3D) {
            SUB_SLIDE(s) += 4;
            return 1;
        }
        return 0;
    case 4:
        if (SUB_SLIDE(s) >= 4) {
            SUB_SLIDE(s) -= 4;
            return 1;
        }
        VCALL(s, 0x28, void (*)(SubScreen *))(s);
        return 0;
    }
    return 0;
}

extern const PTMF SubScreen_StateItems_ptmf7;

/* state: the word plate's question - its list's panels, the grid, the word being made
 * (+0xA8C58, as parameter 3) and the question; once answered, SubScreen_StateItems_ptmf7 */
/* 0x00395460 */
void SubScreen_StateWordPlate(SubScreen *s) {
    char word[9];
    s32 i;

    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    sub_panels(s);
    SubScreen_DrawItemGrid(s, 1);
    for (i = 0; i < 8; i++) {
        word[i] = s->unkA8C58[i];
    }
    word[8] = 0;
    Msg_PrintfParam(&s->text, 3, kFmtString, word);
    if (AT(&s->ask, 0x10, u8) != 0) {
        Task_Run(&s->ask);
    } else {
        ptmf_set(&s->state, &SubScreen_StateItems_ptmf7);
    }
}

extern const PTMF SubScreen_StateItems_ptmf9;

/* state: "equip it?" - its list's panels, the grid and the question; once answered yes (+0x48
 * clear) the item under the cursor is equipped (a costume, list 1: put on Fiona), SubScreen_StateItems_ptmf9 */
/* 0x00395000 */
void SubScreen_StateEquipAsk(SubScreen *s) {
    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    sub_panels(s);
    SubScreen_DrawItemGrid(s, 1);
    if (AT(&s->ask, 0x10, u8) != 0) {
        Task_Run(&s->ask);
        return;
    }
    if (AT(&s->ask, 0x48, u8) == 0) {
        Items_Equip(s->pool, SUB_LIST(s), SUB_CURSOR(s));
        if (SUB_LIST(s) == 1) {
            Fiona_ShowEquipment((Fiona *)gCharPlayer);
        }
    }
    ptmf_set(&s->state, &SubScreen_StateItems_ptmf9);
}

/* ---- the galleries ---- */

extern s32 *kGalleryMotions[];   /* per entry: its motion */

#define SUB_GALLERY_MODEL(s) AT(s, 0xA8DE8, u8 *)
#define MOTION_END(m) (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 0x20)

/* the model gallery's motions: entries 3 / 4 switch the model's state (+0x2C, or +0x30 at
 * motion 0xE00); entry 5 runs on 0x800 -> 0x801 -> 0x802 as each ends; otherwise, when the
 * motion ends (unless it loops), the entry's own again (kGalleryMotions; past 0x19 blended in) */
/* 0x0038A740 */
void Gallery_ModelMotions(SubScreen *s) {
    u8 *m = SUB_GALLERY_MODEL(s);
    u8 k;

    if (m == NULL) {
        return;
    }
    k = SUB_PAGE(s, 0x0, u8);
    if ((u32)(k - 3) < 2) {
        if (AT(m, 0x55C, s32) == 0xE00) {
            VCALL(m, 0x30, void (*)(u8 *))(m);
        } else {
            VCALL(m, 0x2C, void (*)(u8 *))(m);
        }
    }
    k = SUB_PAGE(s, 0x0, u8);
    m = SUB_GALLERY_MODEL(s);
    if (!MOTION_END(m)) {
        return;
    }
    if (k == 5) {
        s32 next = AT(m, 0x55C, s32) == 0x801 ? 0x802 : AT(m, 0x55C, s32) == 0x800 ? 0x801 : -1;

        if (next != -1) {
            Motion_PlayTable(m, next, -1);
            return;
        }
    }
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 1) {
        return;
    }
    if (k < 0x18 || k == 0x19) {
        Motion_PlayTable(m, kGalleryMotions[k][0], -1);
    } else {
        Motion_PlayWith(m, kGalleryMotions[k][0], 1, -1, 5.0f);
    }
}

extern const PTMF SubScreen_StateWordPlate_ptmf4, SubScreen_StateItems_ptmf8;

/* state: "throw one away?" for an item with a word - its list's panels, the grid, the word and
 * the question; answered yes, one of it goes (a costume, list 1: Fiona changes back) and
 * message 0x57 (SubScreen_StateWordPlate_ptmf4); no: SubScreen_StateItems_ptmf8 */
/* 0x003951E0 */
void SubScreen_StateThrowAsk(SubScreen *s) {
    char word[9];
    s32 i;

    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    sub_panels(s);
    SubScreen_DrawItemGrid(s, 1);
    for (i = 0; i < 8; i++) {
        word[i] = s->unkA8C58[i];
    }
    word[8] = 0;
    Msg_PrintfParam(&s->text, 3, kFmtString, word);
    if (AT(&s->ask, 0x10, u8) != 0) {
        Task_Run(&s->ask);
    } else if (AT(&s->ask, 0x48, u8) == 0) {
        Items_UseOne(s->pool, SUB_LIST(s), SUB_CURSOR(s));
        if (SUB_LIST(s) == 1) {
            Fiona_ShowEquipment((Fiona *)gCharPlayer);
        }
        Task_Open(&s->ask, 0x57);
        ptmf_set(&s->state, &SubScreen_StateWordPlate_ptmf4);
    } else {
        ptmf_set(&s->state, &SubScreen_StateItems_ptmf8);
    }
}

/* ---- the screen's frame ---- */

extern void *Overlay_vtable[], *D_0046EC80[], *Helper469D00_vtable[];
extern const PTMF SubScreen_DrawClosing_ptmf;

/* the running screen each frame: its state; behind it a darkening overlay (pages, kind 0x80..)
 * or its panels at the fade's alpha (layer 0x31); depth of field set from 151; asked to close
 * (or progress flag 4): fading back (draw SubScreen_DrawClosing_ptmf), with sound 0x95 unless quiet */
/* 0x00397AB0 */
void SubScreen_StateRun(SubScreen *s) {
    Progress *p = gProgress;
    struct {
        void **vtbl;
        s32 a;
        f32 v[4];
    } dof;

    if (p != NULL && s->mode != 0) {
        Progress_ClearFlag(p, 4);
    }
    ptmf_scall(s, &s->state);
    if (s->kind & 0x80) {
        u8 ov[0x100] __attribute__((aligned(16)));

        AT(ov, 0x0, void **) = Overlay_vtable;
        AT(ov, 0x4, s32) = -1;
        AT(ov, 0x10, s32) = -1;
        AT(ov, 0x14, u8) = 0;
        AT(ov, 0x24, s32) = 0;
        Overlay_SetColor(ov, (u32)s->fade << 24);
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, ov, 0x31, 0);
        AT(ov, 0x0, void **) = Helper469D00_vtable;
    } else {
        u8 alpha = s->fade;

        if (alpha != 0 && s->kind != 0xFF) {
            s8 *panel = kSubScreenFadePanels[s->kind & 0x7F];
            s32 i;

            for (i = 0; i < 4 && panel[i] >= 0; i++) {
                SubScreen_DrawPanel(s, (u8)panel[i], alpha, 0x31);
            }
        }
    }
    dof.a = -1;
    dof.vtbl = D_0046EC80;
    DepthBand_Queue((u8 *)&dof, 1.0f, 151.0f, 2000.0f, 2000.0f);
    if (s->close == 1 || (p != NULL && Progress_TestFlag(p, 4))) {
        s->fading = 1;
        s->fade = 0x40;
        s->fadeStep = 8;
        ptmf_set(&s->draw, &SubScreen_DrawClosing_ptmf);
        if (p != NULL) {
            Progress_ClearFlag(p, 4);
        }
        if (!s->quietClose) {
            Sound_PlaySE(SE_CLOSE);
        }
    }
    dof.vtbl = Helper469D00_vtable;
}

/* ---- the clear results ---- */

/* the results page set up: the ending's flag noted (system data +0x2C bit 18 / 19 by the
 * difficulty, progress var 0x2E 0 / 1; page[2] set when new), the play time (59:59 at most)
 * placed among that difficulty's three best (system data +0x3C + difficulty * 12: hours,
 * minutes, seconds; page[3] its rank, 0xFF none) */
/* 0x00388D30 */
void Results_Setup(SubScreen *s) {
    u8 *sys;
    u8 diff, h, m, sec;
    s32 t, i, k;

    SUB_PAGE(s, 0x0, u8) = 0;
    SUB_PAGE(s, 0x1, u8) = 0x1E;
    SUB_PAGE(s, 0x2, u8) = 0;
    SUB_PAGE(s, 0x3, u8) = 0xFF;
    SUB_PAGE(s, 0x4, u8) = 0;
    SUB_PAGE(s, 0x5, u8) = 0x40;
    diff = Progress_GetVar(gProgress, 0x2E);
    if (diff == 0) {
        if (!(AT(gGamePtr, 0x2C, u32) & 0x40000)) {
            AT(gGamePtr, 0x2C, u32) |= 0x40000;
            SUB_PAGE(s, 0x2, u8) = 1;
        }
    } else if (diff == 1) {
        if (!(AT(gGamePtr, 0x2C, u32) & 0x80000)) {
            AT(gGamePtr, 0x2C, u32) |= 0x80000;
            SUB_PAGE(s, 0x2, u8) = 1;
        }
    }
    h = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0);
    m = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 1);
    sec = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 2);
    if (h != 0) {
        m = 0x3B;
        h = 0;
        sec = 0x3B;
    }
    t = (h * 60 + m) * 60 + sec;
    sys = gGamePtr + diff * 12;
    for (i = 0; i < 3; i++) {
        u8 *e = sys + 0x3C + i * 4;

        if (t < (e[0] * 60 + e[1]) * 60 + e[2]) {
            SUB_PAGE(s, 0x3, u8) = i;
            for (k = 2; i < k; k--) {
                sys[0x3C + k * 4] = sys[0x38 + k * 4];
                sys[0x3D + k * 4] = sys[0x39 + k * 4];
                sys[0x3E + k * 4] = sys[0x3A + k * 4];
            }
            e[0] = h;
            e[1] = m;
            e[2] = sec;
            return;
        }
    }
}

/* ---- the entry list (the file's index) ---- */

extern u8 gLanguage;           /* the language */
extern u8 D_0047B180[][2];      /* per group of 8: its first entry and the end */
extern const char str_N_7[];

/* the entry list (screen kind 0x8F): the language set to 1, the two headings, the entries of
 * the current one's group (page[0] / 8) with their numbers and titles ("???" until unlocked,
 * kExtraUnlocks's flag), the current one highlighted; the group number of 2 */
/* 0x00385C30 */
void SubScreen_DrawEntries(SubScreen *s) {
    u8 g;
    s32 k;

    s->kind = 0x8F;
    sub_panels(s);
    gLanguage = 1;
    Task_ShowText(&s->text, 0x30, 0x3B, 0x80, Task_MessageText(&s->text, 0xF), 0x80, 0x30, 0x10, 0x15);
    Task_ShowText(&s->text, 0x58, 0x3B, 0x80, Task_MessageText(&s->text, 0x2B), 0x80, 0x30, 0x10, 0x15);
    g = SUB_PAGE(s, 0x0, u8) >> 3;
    for (k = D_0047B180[g][0]; k < D_0047B180[g][1]; k++) {
        u16 *e = kExtraUnlocks[k];
        u8 color = k == SUB_PAGE(s, 0x0, u8) ? 0x82 : 0x80;
        s32 y = (k % 8) * 35 + 0x5E;

        Task_Printf(&s->text, 0x30, y, color, str_N_7, k + 1);
        if (AT(gGamePtr, 0x24 + (e[0] >> 5) * 4, u32) & (1 << (e[0] & 0x1F))) {
            Task_ShowText(&s->text, 0x58, y, color, Task_MessageText(&s->text, e[1]), 0x80, 0x30, 0x10, 0x15);
        } else {
            Task_ShowText(&s->text, 0x58, y, color, Task_MessageText(&s->text, 0x16E), 0x80, 0x30, 0x10, 0x15);
        }
    }
    Task_Printf(&s->text, 0x186, 0x176, 0x80, kFmtSlash, g + 1, 2);
}

/* ---- the model gallery's model ---- */

extern u8 *Gallery_MakeModel(SubScreen *s, u8 k);   /* the gallery model made */
extern f32 kGalleryCameraDistance[];      /* per entry: the camera's extra distance */
extern f32 D_0044B9F4[][3];   /* per entry: the camera's height */
extern const PTMF Gallery_StateModel_ptmf;

/* state: the model gallery's files loading; then shown (state Gallery_StateModel_ptmf): the model made
 * (+0x4D8 / +0x4D9 set), the camera taken (+0xA8E00 / +0xA8E10) and aimed (26, 0, -27,
 * distance 10 + kGalleryCameraDistance, height D_0044B9F4), entries 6..8 idling (motions 0x1F00 / 0x2000 /
 * 0x2102) and the entry's first motion (kGalleryMotions; past 0x19 blended in) */
/* 0x0038B900 */
void Gallery_StateModelLoad(SubScreen *s) {
    VObject *cam;
    u8 *m;
    u8 k;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x10000000) == 2) {
        return;
    }
    s->fade = 0x80;
    ptmf_set(&s->state, &Gallery_StateModel_ptmf);
    SUB_PAGE(s, 0x1, u8) = 0;
    SUB_GALLERY_MODEL(s) = Gallery_MakeModel(s, SUB_PAGE(s, 0x0, u8));
    AT(SUB_GALLERY_MODEL(s), 0x4D8, u8) = 1;
    AT(SUB_GALLERY_MODEL(s), 0x4D9, u8) = 1;
    VCALL(gCamDirector, 0x14, void (*)(VObject *))(gCamDirector);
    cam = gCamera;
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, (f32 *)((u8 *)s + 0xA8E00));
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, (f32 *)((u8 *)s + 0xA8E10));
    AT(s, 0xA8E20, f32) = 26.0f;
    AT(s, 0xA8E24, f32) = 0.0f;
    AT(s, 0xA8E28, f32) = -27.0f;
    AT(s, 0xA8E2C, f32) = 1.0f;
    AT(s, 0xA8E30, f32) = 10.0f + kGalleryCameraDistance[SUB_PAGE(s, 0x0, u8)];
    AT(s, 0xA8E34, f32) = 0.0f;
    AT(s, 0xA8E38, f32) = D_0044B9F4[SUB_PAGE(s, 0x0, u8)][0];
    AT(s, 0xA8E3C, u8) = 1;
    k = SUB_PAGE(s, 0x0, u8);
    if (k >= 6 && k < 9) {
        Motion_PlayWith(SUB_GALLERY_MODEL(s), 0x1F00, 1, -1, 5.0f);
        Motion_PlayWith(SUB_GALLERY_MODEL(s), 0x2000, 1, -1, 5.0f);
        Motion_PlayWith(SUB_GALLERY_MODEL(s), 0x2102, 1, -1, 5.0f);
    }
    k = SUB_PAGE(s, 0x0, u8);
    m = SUB_GALLERY_MODEL(s);
    if (k < 0x18 || k == 0x19) {
        Motion_PlayTable(m, kGalleryMotions[k][SUB_PAGE(s, 0x1, u8)], -1);
    } else {
        Motion_PlayWith(m, kGalleryMotions[k][SUB_PAGE(s, 0x1, u8)], 1, -1, 5.0f);
    }
    SUB_PAGE(s, 0x2, u8) = 0;
}

/* ---- the extras list ---- */

/* the extras list (screen kind 0x8C): the language 1, the headings, the eight entries of the
 * current one's group (titles 0x70 + entry, "???" until unlocked, SubScreen_ExtraUnlocked), the current
 * one highlighted; the help line, the group number of 4 and the arrows blinking */
/* 0x0038DBE0 */
void SubScreen_DrawExtras(SubScreen *s) {
    u8 base;
    u32 i;
    s32 b, x;
    u8 alpha;

    s->kind = 0x8C;
    sub_panels(s);
    gLanguage = 1;
    Task_ShowText(&s->text, 0x30, 0x3B, 0x80, Task_MessageText(&s->text, 0xF), 0x80, 0x30, 0x10, 0x15);
    Task_ShowText(&s->text, 0x58, 0x3B, 0x80, Task_MessageText(&s->text, 0x10), 0x80, 0x30, 0x10, 0x15);
    base = SUB_PAGE(s, 0x0, u8) & ~7;
    for (i = 0; i < 8; i++) {
        u32 k = base + i;
        u16 msg;
        u8 color;

        if (k >= 0x20) {
            continue;
        }
        msg = SubScreen_ExtraUnlocked(s, k) ? (u16)(base + 0x70 + i) : 0x16E;
        color = SUB_PAGE(s, 0x0, u8) == k ? 0x82 : 0x80;
        Task_Printf(&s->text, 0x30, 0x5E + i * 35, color, str_N_7, k + 1);
        Task_ShowText(&s->text, 0x58, 0x5E + i * 35, color, Task_MessageText(&s->text, msg), 0x80, 0x30, 0x10, 0x15);
    }
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, 0x12), 0x80, 0x30, 0x10, 0x15);
    x = Task_MessageWidth(&s->text, 0x12, 0x10) + 0x56;
    Task_ShowText(&s->text, x, 0x186, 0x80, Task_MessageText(&s->text, 0x13), 0x80, 0x30, 0x10, 0x15);
    Task_Printf(&s->text, 0x186, 0x176, 0x80, kFmtSlash, (base >> 3) + 1, 4);
    b = 0x80 - ((s->frame << 2) & 0xFF);
    alpha = b > 0 ? b : -b;
    SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
    SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
}

/* ---- the screen fading out / in ---- */

extern const char str_SUBSCR_SUBBACK_TEX[];   /* "SUBSCR\\SUBBACK.TEX" */
extern const PTMF SubScreen_Open_ptmf, SubScreen_StateRun_ptmf;

/* the fade's background: a darkening overlay (pages, kind 0x80..) or the panels at the fade's
 * alpha, layer 0x31 */
static void sub_fade_back(SubScreen *s) {
    if (s->kind & 0x80) {
        u8 ov[0x100] __attribute__((aligned(16)));

        AT(ov, 0x8, s32) = 0;   /* not set by the original either (its stack there was clear) */
        AT(ov, 0x0, void **) = Overlay_vtable;
        AT(ov, 0x4, s32) = -1;
        AT(ov, 0x10, s32) = -1;
        AT(ov, 0x14, u8) = 0;
        AT(ov, 0x24, s32) = 0;
        Overlay_SetColor(ov, (u32)s->fade << 24);
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, ov, 0x31, 0);
        AT(ov, 0x0, void **) = Helper469D00_vtable;
    } else {
        u8 alpha = s->fade;

        if (alpha != 0 && s->kind != 0xFF) {
            s8 *panel = kSubScreenFadePanels[s->kind & 0x7F];
            s32 i;

            for (i = 0; i < 4 && panel[i] >= 0; i++) {
                SubScreen_DrawPanel(s, (u8)panel[i], alpha, 0x31);
            }
        }
    }
}

/* the music's (+0x94) and the voices' (gStageMusic +0x44) level `f` 0..1 */
static void sub_fade_sound(f32 f) {
    VCALL(gSound, 0x94, void (*)(VObject *, u32))(gSound, (u8)(u32)(255.0f * f));
    if (gStageMusic != NULL) {
        VCALL(gStageMusic, 0x44, void (*)(VObject *, f32))(gStageMusic, f);
    }
}

/* draw: closing - the page's textures given back and SUBBACK.TEX reloaded at the start, the fade
 * from 0x40 to 0x80 (sound coming back up); once loaded, the screen's own textures made resident,
 * draw SubScreen_Open_ptmf and progress flags 8 and 4 */
/* 0x003970D0 */
void SubScreen_DrawClosing(SubScreen *s) {
    Progress *p = gProgress;
    struct {
        void **vtbl;
        s32 a;
        f32 v[4];
    } dof;
    f32 f;

    if (p != NULL) {
        Progress_ClearFlag(p, 4);
    }
    if (s->fade == 0x40) {
        VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, 0x19);
        VCALL(gFileLoader, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(gFileLoader, str_SUBSCR_SUBBACK_TEX, s->pageTex,
                                                                                   0x6000000, 0);
    }
    s->fade += s->fadeStep;
    if (s->fade < 0x80) {
        f = 100.0f * (f32)(s->fade - 0x40) / 64.0f / 100.0f;
    } else {
        s->fade = 0x80;
        if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
            VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, s->pageTex, 0x19);
            if (p != NULL) {
                ptmf_set(&s->draw, &SubScreen_Open_ptmf);
                Progress_SetFlag(gProgress, 8);
                Progress_SetFlag(gProgress, 4);
            }
        }
        f = 1.0f;
    }
    sub_fade_back(s);
    dof.a = -1;
    dof.vtbl = D_0046EC80;
    DepthBand_Queue((u8 *)&dof, 1.0f, 151.0f, 2000.0f, 2000.0f);
    sub_fade_sound(f);
    dof.vtbl = Helper469D00_vtable;
}

/* draw: opening - once the textures are loaded the fade runs up to 0x40 (sound going down);
 * there the screen starts (flags cleared; modes 7 / 10 make their page resident) with draw
 * SubScreen_StateRun_ptmf. Depth of field drawing in from far to near with it. */
/* 0x00398100 */
void SubScreen_DrawOpening(SubScreen *s) {
    static const union { u32 u; f32 f; } kTenth = {0x3DCCCCCD};
    struct {
        void **vtbl;
        s32 a;
        f32 v[4];
    } dof;
    f32 f;

    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
        s->fade += s->fadeStep;
    }
    if (s->fade >= 0x40) {
        s->fading = 0;
        s->fade = 0x40;
        s->quietClose = 0;
        s->close = 0;
        if (s->mode == 7 || s->mode == 10) {
            VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, s->pageTex, 0x19);
        }
        ptmf_set(&s->draw, &SubScreen_StateRun_ptmf);
    }
    f = 100.0f * (f32)(0x40 - s->fade) / 64.0f / 100.0f;
    sub_fade_sound(f);
    sub_fade_back(s);
    dof.a = -1;
    dof.vtbl = D_0046EC80;
    DepthBand_Queue((u8 *)&dof, 1.0f, 10.0f * (kTenth.f + 15.0f * (100.0f * (f32)s->fade / 64.0f / 100.0f)), 2000.0f,
                  2000.0f);
    if (gProgress != NULL) {
        Progress_ClearFlag(gProgress, 4);
    }
    dof.vtbl = Helper469D00_vtable;
}

extern const PTMF SubScreen_StateEntryQuestion_ptmf;

/* state: the entry list - up / down through the group (0..7, 8..10, round), left / right /
 * next / previous to the other group, confirm opens the entry (kExtraUnlocks's message, state
 * SubScreen_StateEntryQuestion_ptmf), cancel closes; then the list, the help line and the arrows */
/* 0x00386150 */
void SubScreen_StateEntries(SubScreen *s) {
    s32 b, x;
    u8 alpha;

    if (!s->fading) {
        u32 pad = gMenuPressed;
        u8 *k = &SUB_PAGE(s, 0x0, u8);

        if (pad & MENU_CONFIRM) {
            Task_Open(&s->ask, kExtraUnlocks[*k][2]);
            ptmf_set(&s->state, &SubScreen_StateEntryQuestion_ptmf);
            Sound_PlaySE(SE_DECIDE);
        } else if (pad & MENU_UP) {
            *k = *k == 0 ? 7 : *k == 8 ? 10 : *k - 1;
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_DOWN) {
            *k = *k == 7 ? 0 : *k == 10 ? 8 : *k + 1;
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & (MENU_LEFT | MENU_RIGHT | MENU_PREV | MENU_NEXT)) {
            *k = *k < 8 ? 8 : 0;
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_CANCEL) {
            s->close = 1;
        }
    }
    SubScreen_DrawEntries(s);
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, 0x12), 0x80, 0x30, 0x10, 0x15);
    x = Task_MessageWidth(&s->text, 0x12, 0x10) + 0x56;
    Task_ShowText(&s->text, x, 0x186, 0x80, Task_MessageText(&s->text, 0x13), 0x80, 0x30, 0x10, 0x15);
    b = 0x80 - ((s->frame << 2) & 0xFF);
    alpha = b > 0 ? b : -b;
    SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
    SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
}

extern const PTMF Gallery_StateModelChosen_ptmf;

/* state: the extras list - up / down within the group of 8 (round; 31 at most), left /
 * previous and right / next to the group before / after (round), confirm an unlocked extra
 * (a fade from 0, progress +0x84, state Gallery_StateModelChosen_ptmf; a buzzer if locked), cancel (progress
 * flag 4); then the list */
/* 0x0038D7E0 */
void SubScreen_StateExtras(SubScreen *s) {
    if (!s->fading) {
        u8 *k = &SUB_PAGE(s, 0x0, u8);
        u8 base = *k & ~7;
        u32 pad = gMenuPressed;

        if (pad & MENU_UP) {
            (*k)--;
            if ((s8)*k < base) {
                *k = base + 7;
            }
            if (*k >= 0x20) {
                *k = 0x1F;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_DOWN) {
            (*k)++;
            if (base + 7 < *k) {
                *k = base;
            }
            if (*k >= 0x20) {
                *k = base;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & (MENU_LEFT | MENU_PREV)) {
            *k -= 8;
            if ((s8)*k < 0) {
                *k = 0x1F;
            }
            *k &= ~7;
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & (MENU_RIGHT | MENU_NEXT)) {
            *k += 8;
            if (*k >= 0x20) {
                *k = 0;
            }
            *k &= ~7;
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_CANCEL) {
            Progress_SetFlag(gProgress, 4);
        } else if (gMenuPressed & MENU_CONFIRM) {
            if (*k < 0x20 && SubScreen_ExtraUnlocked(s, *k)) {
                s->fade = 0;
                s->fadeStep = 0x10;
                VCALL(gProgress, 0x84, void (*)(Progress *))(gProgress);
                ptmf_set(&s->state, &Gallery_StateModelChosen_ptmf);
                Sound_PlaySE(SE_DECIDE);
            } else {
                Sound_PlaySE(SE_BUZZER);
            }
        }
    }
    SubScreen_DrawExtras(s);
}

/* ---- the costume page ---- */

/* the costume page set up: the costumes worn kept (progress vars 0x26 / 0x27 to 0x28 / 0x29);
 * Fiona's choices (page[0..5]: 0, 2, 3, 7, 6, 8 - the ones not unlocked marked 0x80, the
 * current one's place in page[7]) and Hewie's (page[8..10]: 0, 1, 2, its place in page[12];
 * only with him here - his model refreshed first when in the room); the cursors (13..16) */
/* 0x0038F7D0 */
void Costumes_Setup(SubScreen *s) {
    Progress *p = gProgress;
    u8 *sys;
    s32 i;

    Progress_SetVar(p, 0x28, Progress_GetVar(p, 0x26));
    Progress_SetVar(p, 0x29, Progress_GetVar(p, 0x27));
    SUB_PAGE(s, 0x0, u8) = 0;
    SUB_PAGE(s, 0x1, u8) = 2;
    SUB_PAGE(s, 0x2, u8) = 3;
    SUB_PAGE(s, 0x3, u8) = 7;
    SUB_PAGE(s, 0x4, u8) = 6;
    SUB_PAGE(s, 0x5, u8) = 8;
    SUB_PAGE(s, 0x6, u8) = 0xFF;
    SUB_PAGE(s, 0x7, u8) = 0xFF;
    sys = gGamePtr;
    if (!(AT(sys, 0x24, u32) & 0x2)) {
        SUB_PAGE(s, 0x3, u8) |= 0x80;
    }
    if (!(AT(sys, 0x24, u32) & 0x8)) {
        SUB_PAGE(s, 0x5, u8) |= 0x80;
    }
    if (p != NULL && gCharPlayer != NULL && AT(gCharPlayer, 0x28, u8) != 0) {
        for (i = 0; SUB_PAGE(s, i, u8) != 0xFF; i++) {
            if (SUB_PAGE(s, i, u8) == (u8)Progress_GetVar(p, 0x26)) {
                SUB_PAGE(s, 0x7, u8) = i;
                break;
            }
        }
    }
    SUB_PAGE(s, 0x8, u8) = 0;
    SUB_PAGE(s, 0x9, u8) = 1;
    SUB_PAGE(s, 0xA, u8) = 2;
    SUB_PAGE(s, 0xB, u8) = 0xFF;
    SUB_PAGE(s, 0xC, u8) = 0xFF;
    if (!(AT(sys, 0x2C, u32) & 0x400000) || !((AT(sys, 0x24, u32) & 0x100) || (AT(sys, 0x24, u32) & 0x1000))) {
        SUB_PAGE(s, 0xA, u8) |= 0x80;
    }
    if ((AT(gCharPartner, 0x30, s32) == 0x37 || (AT(p, 0x1C, u32) & 0x1000000)) && p != NULL && gCharPartner != NULL &&
        AT(gCharPartner, 0x28, u8) != 0) {
        u8 *h = (u8 *)gCharPartner;

        if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == AT(h, 0x30, s32) && Hewie_FionaReachable((Hewie *)h)) {
            VCALL(h, 0x90, void (*)(void *))(h);
            VCALL(h, 0x7C, void (*)(void *))(h);
        }
        if (AT(gCharPartner, 0xE0, u8) == 0) {
            for (i = 8; SUB_PAGE(s, i, u8) != 0xFF; i++) {
                if (SUB_PAGE(s, i, u8) == (u8)Progress_GetVar(p, 0x27)) {
                    SUB_PAGE(s, 0xC, u8) = i - 8;
                    break;
                }
            }
        }
    }
    SUB_PAGE(s, 0xD, u8) = 0;
    SUB_PAGE(s, 0xE, u8) = 0;
    SUB_PAGE(s, 0xF, u8) = SUB_PAGE(s, 0x7, u8);
    SUB_PAGE(s, 0x10, u8) = SUB_PAGE(s, 0xC, u8);
}

/* ---- the art gallery's list ---- */

extern u16 D_0044BFE0[];   /* per picture: its unlock flag (system data +0x24 bits) */

/* the art gallery's list (screen kind 0x8E): the language 1, the headings, the eight pictures
 * of the current one's group (titles 0x190 + picture, "???" until unlocked), the current one
 * highlighted; the help line, the group number of 6 and the arrows blinking */
/* 0x00386550 */
void Gallery_DrawArtList(SubScreen *s) {
    u8 g;
    s32 k, x, b;
    u8 alpha;

    s->kind = 0x8E;
    sub_panels(s);
    gLanguage = 1;
    Task_ShowText(&s->text, 0x30, 0x3B, 0x80, Task_MessageText(&s->text, 0xF), 0x80, 0x30, 0x10, 0x15);
    Task_ShowText(&s->text, 0x58, 0x3B, 0x80, Task_MessageText(&s->text, 0x10), 0x80, 0x30, 0x10, 0x15);
    g = SUB_PAGE(s, 0x0, u8) >> 3;
    for (k = g * 8; k < (g + 1) * 8 && k < 0x30; k++) {
        u16 f = D_0044BFE0[k];
        u8 color = k == SUB_PAGE(s, 0x0, u8) ? 0x82 : 0x80;
        s32 y = (k % 8) * 35 + 0x5E;

        Task_Printf(&s->text, 0x30, y, color, str_N_7, k + 1);
        if (AT(gGamePtr, 0x24 + (f >> 5) * 4, u32) & (1 << (f & 0x1F))) {
            Task_ShowText(&s->text, 0x58, y, color, Task_MessageText(&s->text, (k + 0x190) & 0xFFFF), 0x80, 0x30, 0x10,
                          0x15);
        } else {
            Task_ShowText(&s->text, 0x58, y, color, Task_MessageText(&s->text, 0x16E), 0x80, 0x30, 0x10, 0x15);
        }
    }
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, 0x12), 0x80, 0x30, 0x10, 0x15);
    x = Task_MessageWidth(&s->text, 0x12, 0x10) + 0x56;
    Task_ShowText(&s->text, x, 0x186, 0x80, Task_MessageText(&s->text, 0x13), 0x80, 0x30, 0x10, 0x15);
    Task_Printf(&s->text, 0x186, 0x176, 0x80, kFmtSlash, g + 1, 6);
    b = 0x80 - ((s->frame << 2) & 0xFF);
    alpha = b > 0 ? b : -b;
    SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
    SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
}

/* ---- the model gallery's camera ---- */

extern f32 gLeftStick, gStickVertical, D_0047E3B8;   /* the sticks (-1..1) */
extern f32 D_0044B9F0[][3], D_0044B9F8[][3];     /* per entry: the height's lowest / highest */

#define GALLERY_DIST(s) AT(s, 0xA8E30, f32)
#define GALLERY_TURN(s) AT(s, 0xA8E34, f32)
#define GALLERY_HEIGHT(s) AT(s, 0xA8E38, f32)

/* a stick past its dead zone (0.3), scaled to -1..1; 0 inside it */
static inline s32 stick_out(f32 v, f32 *out) {
    static const union { u32 u; f32 f; } kDead = {0x3E99999A}, kRange = {0x3F333333};
    f32 a = v <= 0.0f ? -v : v;

    if (a < kDead.f) {
        return 0;
    }
    *out = (v <= 0.0f ? v + kDead.f : v - kDead.f) / kRange.f;
    return 1;
}

/* the model gallery's camera by the sticks: the right one in (distance, entry's nearest .. 60)
 * and round (the turn kept within -pi..pi), the left one up and down (within the entry's
 * heights); L3 sets it back */
/* 0x0038A2E0 */
void Gallery_ModelCamera(SubScreen *s) {
    static const union { u32 u; f32 f; } kStep = {0x3DB2B8C3};
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB}, kNegPi = {0xC0490FDB};
    f32 d;

    if (stick_out(gStickVertical, &d)) {
        GALLERY_DIST(s) = GALLERY_DIST(s) + d;
        if (GALLERY_DIST(s) < kGalleryCameraDistance[SUB_PAGE(s, 0x0, u8)]) {
            GALLERY_DIST(s) = kGalleryCameraDistance[SUB_PAGE(s, 0x0, u8)];
        }
        if (!(GALLERY_DIST(s) <= 60.0f)) {
            GALLERY_DIST(s) = 60.0f;
        }
    }
    if (stick_out(gLeftStick, &d)) {
        GALLERY_TURN(s) = GALLERY_TURN(s) + d * kStep.f;
        while (GALLERY_TURN(s) < kNegPi.f) {
            GALLERY_TURN(s) += kTwoPi.f;
        }
        while (!(GALLERY_TURN(s) <= kPi.f)) {
            GALLERY_TURN(s) -= kTwoPi.f;
        }
    }
    if (stick_out(D_0047E3B8, &d)) {
        GALLERY_HEIGHT(s) = GALLERY_HEIGHT(s) - 0.5f * d;
        if (!(GALLERY_HEIGHT(s) <= D_0044B9F8[SUB_PAGE(s, 0x0, u8)][0])) {
            GALLERY_HEIGHT(s) = D_0044B9F8[SUB_PAGE(s, 0x0, u8)][0];
        }
        if (GALLERY_HEIGHT(s) < D_0044B9F0[SUB_PAGE(s, 0x0, u8)][0]) {
            GALLERY_HEIGHT(s) = D_0044B9F0[SUB_PAGE(s, 0x0, u8)][0];
        }
    }
    if (gPadPressed & PAD_L3) {
        AT(s, 0xA8E20, f32) = 26.0f;
        AT(s, 0xA8E24, f32) = 0.0f;
        AT(s, 0xA8E28, f32) = -27.0f;
        AT(s, 0xA8E2C, f32) = 1.0f;
        GALLERY_DIST(s) = 10.0f + kGalleryCameraDistance[SUB_PAGE(s, 0x0, u8)];
        GALLERY_TURN(s) = 0.0f;
        GALLERY_HEIGHT(s) = D_0044B9F4[SUB_PAGE(s, 0x0, u8)][0];
    }
}

/* ---- the music gallery ---- */

extern u8 D_0044BF30[][6];   /* per track: its unlock flag (u16), title (u16), BGM number */

#define MUSIC_WANT(track, pause, restart) \
    VCALL(gMusic, 0x8, void (*)(VObject *, s32, s32, s32, f32))(gMusic, track, pause, restart, 1.0f)
#define SUB_MUSIC_NEXT(s) SUB_PAGE(s, 0x1, u8)      /* the track to start (0xFF none) */
#define SUB_MUSIC_PLAYING(s) SUB_PAGE(s, 0x2, u8)   /* the track playing (0xFF none) */

static inline s32 music_unlocked(u8 k) {
    u16 f = AT(D_0044BF30[k], 0x0, u16);

    return (AT(gGamePtr, 0x24 + (f >> 5) * 4, u32) & (1 << (f & 0x1F))) != 0;
}

/* the music gallery (screen kind 0x8D): the language 1, the headings, the tracks of the
 * current one's group (titles, "???" until unlocked), the current one highlighted and the one
 * playing marked; the help lines (other while one plays), the group number of 4, the arrows */
/* 0x00388400 */
void Gallery_DrawMusic(SubScreen *s) {
    u8 g;
    s32 k, b;
    u8 *msg, alpha;

    s->kind = 0x8D;
    sub_panels(s);
    gLanguage = 1;
    Task_ShowText(&s->text, 0x30, 0x3B, 0x80, Task_MessageText(&s->text, 0xF), 0x80, 0x30, 0x10, 0x15);
    Task_ShowText(&s->text, 0x58, 0x3B, 0x80, Task_MessageText(&s->text, 0x10), 0x80, 0x30, 0x10, 0x15);
    g = SUB_PAGE(s, 0x0, u8) >> 3;
    for (k = g * 8; k < (g + 1) * 8 && k < 0x1B; k++) {
        u8 color;
        s32 y = (k % 8) * 35 + 0x5E;

        if (k == SUB_PAGE(s, 0x0, u8)) {
            color = 0x82;
        } else {
            color = k == SUB_MUSIC_PLAYING(s) ? 0x81 : 0x80;
        }
        Task_Printf(&s->text, 0x30, y, color, str_N_7, k + 1);
        if (music_unlocked(k)) {
            Task_ShowText(&s->text, 0x58, y, color, Task_MessageText(&s->text, AT(D_0044BF30[k], 0x2, u16)), 0x80,
                          0x30, 0x10, 0x15);
        } else {
            Task_ShowText(&s->text, 0x58, y, color, Task_MessageText(&s->text, 0x16E), 0x80, 0x30, 0x10, 0x15);
        }
    }
    Task_ShowText(&s->text, 0x46, 0x186, 0x80, Task_MessageText(&s->text, 0x1C8), 0x80, 0x30, 0x10, 0x15);
    msg = Task_MessageText(&s->text, SUB_MUSIC_PLAYING(s) == 0xFF ? 0x13 : 0x1C7);
    Task_ShowText(&s->text, Task_MessageWidth(&s->text, 0x12, 0x10) + 0x56, 0x186, 0x80, msg, 0x80, 0x30, 0x10, 0x15);
    Task_Printf(&s->text, 0x186, 0x176, 0x80, kFmtSlash, g + 1, 4);
    b = 0x80 - ((s->frame << 2) & 0xFF);
    alpha = b > 0 ? b : -b;
    SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
    SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
}

/* state: the music gallery - confirm an unlocked track (the music stopped, it queued and
 * marked playing; a buzzer if locked), the cursor round its group of 8 (up / down; 27 tracks)
 * and through the groups (left / previous, right / next), cancel stops the music (or, when
 * the stream is busy, closes); once the stream is free the queued track starts. Then the list. */
/* 0x003888A0 */
void Gallery_StateMusic(SubScreen *s) {
    if (!s->fading) {
        u8 *k = &SUB_PAGE(s, 0x0, u8);
        u32 pad = gMenuPressed;

        if (pad & MENU_CONFIRM) {
            if (music_unlocked(*k)) {
                MUSIC_WANT(0xFF, 0, 0);
                SUB_MUSIC_PLAYING(s) = *k;
                SUB_MUSIC_NEXT(s) = *k;
            } else {
                Sound_PlaySE(SE_BUZZER);
            }
        } else if (pad & MENU_UP) {
            if (*k == 0) {
                *k = 7;
            } else {
                *k = (*k - 1) % 8 + (*k >> 3) * 8;
            }
            if (*k >= 0x1B) {
                *k = 0x1A;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_DOWN) {
            *k = (*k + 1) % 8 + (*k >> 3) * 8;
            if (*k >= 0x1B) {
                *k = 0x18;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & (MENU_LEFT | MENU_PREV)) {
            *k = (*k >> 3) == 0 ? 0x18 : ((*k >> 3) - 1) * 8;
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & (MENU_RIGHT | MENU_NEXT)) {
            *k = (*k >> 3) < 3 ? ((*k >> 3) + 1) * 8 : 0;
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_CANCEL) {
            if (Bgm_IsPlaying(gAdx) == 0) {
                MUSIC_WANT(0xFF, 0, 0);
                SUB_MUSIC_NEXT(s) = 0xFF;
            } else {
                s->close = 1;
            }
        }
        if (Bgm_IsPlaying(gAdx) != 0) {
            if (SUB_MUSIC_NEXT(s) != 0xFF) {
                MUSIC_WANT(D_0044BF30[SUB_MUSIC_NEXT(s)][4], 0, 1);
                SUB_MUSIC_NEXT(s) = 0xFF;
            } else if (SUB_MUSIC_PLAYING(s) != 0xFF) {
                SUB_MUSIC_PLAYING(s) = 0xFF;
            }
        }
    }
    Gallery_DrawMusic(s);
}

/* ---- the art gallery's picture ---- */

extern const PTMF Gallery_StateArtView_ptmf;

/* state: a picture chosen - fading out (to 0x80); then (the file loaded) the picture unpacked
 * from the work buffer (progress +0x88; offsets by picture % 8) to +0x200000: runs of bytes
 * (bit 7 set: the next byte repeated n & 0x7F times; else n bytes as they are) for 0x70000
 * bytes (pictures 21..: 0x38000), then its 0x400-byte palette; fading in from 0x80 (state
 * Gallery_StateArtView_ptmf). The list under a darkening overlay or the panels */
/* 0x00387920 */
void Gallery_StatePicture(SubScreen *s) {
    s->fade += s->fadeStep;
    if (s->fade >= 0x80) {
        s->fading = 0;
        s->fade = 0x80;
    }
    if (!s->fading && VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x6000000) != 2) {
        u8 *buf = VCALL(gProgress, 0x88, u8 *(*)(Progress *))(gProgress);
        u8 k = SUB_PAGE(s, 0x0, u8);
        const u8 *src = buf + AT(buf, (k % 8) * 4, u32);
        u8 *dst = buf + 0x200000;
        s32 size = (k < 0x15 ? 0x380 : 0x1C0) << 9, n = 0, i;

        while (n < size) {
            u8 c = *src;

            if ((c & 0x80) == 0x80) {
                c &= 0x7F;
                for (i = 0; i < c; i++) {
                    *dst++ = src[1];
                }
                n += c;
                src += 2;
            } else {
                src++;
                for (i = 0; i < c; i++) {
                    *dst++ = *src++;
                }
                n += c;
            }
        }
        for (i = 0; i < 0x400; i++) {
            *dst++ = *src++;
        }
        s->fading = 1;
        s->fade = 0x80;
        s->fadeStep = 0;
        ptmf_set(&s->state, &Gallery_StateArtView_ptmf);
    }
    Gallery_DrawArtList(s);
    sub_fade_back(s);
}

/* ---- the model gallery's start ---- */

extern void *Tint_vtable[];
extern const PTMF Gallery_StateModelLoad_ptmf;

/* keep room effect `k` (its 0x90 bytes from +0x10) in `save` and remove it */
static inline void gallery_effect_keep(u8 *fx, s32 k, u8 *save) {
    u64 *src = (u64 *)(RoomEffects_Get(fx, k) + 0x10);
    s32 i;

    for (i = 0; i < 18; i++) {
        AT(save, i * 8, u64) = src[i];
    }
    RoomEffects_Release(fx, k);
}

/* state: a model chosen in the extras - fading out (to 0x80); then the work buffer (progress
 * +0x88) split for the gallery's files (+0xA8DEC.. : +0, +0x200000, +0x242000, +0x244000 and
 * progress +0x16C0) and they loaded (Gallery_LoadPck), the renderer reset (+0x1C), state
 * Gallery_StateModelLoad_ptmf; two lights (55, 55, 50 / 65, 60, 60 at 15 and 30 degrees), the director's
 * +0x7C, the room effects 0x1D..0x1F kept (+0xA8E40..) and removed, effect 0x1F made anew (a
 * Tint_vtable) and started. The extras list under a darkening overlay or the panels */
/* 0x0038BC70 */
void Gallery_StateModelChosen(SubScreen *s) {
    if (s->fade < 0x80) {
        s->fade += s->fadeStep;
    } else {
        s->fade = 0x80;
    }
    if (s->fade == 0x80) {
        Progress *p = gProgress;
        u8 *buf = VCALL(p, 0x88, u8 *(*)(Progress *))(p);

        if (buf != NULL) {
            static const union { u32 u; f32 f; } k15 = {0x3E860A92}, k30 = {0x3F060A92};
            f32 v[4] __attribute__((aligned(16)));
            VObject *lights;
            u8 *fx, **slot;
            u8 msg[8];
            void *mem;

            AT(s, 0xA8DEC, u8 *) = buf;
            AT(s, 0xA8DF0, u8 *) = buf + 0x200000;
            AT(s, 0xA8DF8, u8 *) = buf + 0x242000;
            AT(s, 0xA8DFC, u8 *) = buf + 0x244000;
            SUB_GALLERY_MODEL(s) = NULL;
            AT(s, 0xA8DF4, u8 *) = (u8 *)p + 0x16C0;
            Gallery_LoadPck(s, SUB_PAGE(s, 0x0, u8));
            VCALL(gRenderer, 0x1C, void (*)(VObject *))(gRenderer);
            ptmf_set(&s->state, &Gallery_StateModelLoad_ptmf);
            v[0] = 55.0f;
            v[1] = 55.0f;
            v[3] = 0.0f;
            v[2] = 50.0f;
            lights = gLights;
            VCALL(lights, 0x48, void (*)(VObject *, s32, f32 *, f32))(lights, 1, v, 0.0f);
            v[0] = 65.0f;
            v[3] = 0.0f;
            v[1] = 60.0f;
            v[2] = 60.0f;
            VCALL(lights, 0x4C, void (*)(VObject *, s32, s32, f32 *, f32, f32))(lights, 0, 1, v, k15.f, k30.f);
            VCALL(gCutscene, 0x7C, void (*)(VObject *, s32))(gCutscene, 1);
            fx = gRoomEffects;
            gallery_effect_keep(fx, 0x1F, (u8 *)s + 0xA8E40);
            gallery_effect_keep(fx, 0x1D, (u8 *)s + 0xA8ED0);
            gallery_effect_keep(fx, 0x1E, (u8 *)s + 0xA8F60);
            msg[0] = msg[1] = msg[2] = msg[3] = msg[4] = msg[5] = msg[6] = msg[7] = 0;
            slot = (u8 **)(fx + 0x14B4);
            if (*slot != NULL) {
                VCALL(gRoomEffects + 0x1400, 0x14, void (*)(void *, void *))(gRoomEffects + 0x1400, *slot);
                *slot = NULL;
            }
            mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
            if (mem != NULL) {
                u8 *e = RoomEffects_new(0xA0, mem);

                if (e != NULL) {
                    AT(e, 0x0, void **) = Tint_vtable;
                }
                *slot = e;
                VCALL(*slot, 0xC, void (*)(void *))(*slot);
            }
            fx = gRoomEffects;
            RoomEffects_Get(fx, 0x1F);
            RoomEffects_Send(fx, 0x1F, msg);
        }
    }
    SubScreen_DrawExtras(s);
    sub_fade_back(s);
}

extern const char str_SUBSCR_ARTN_LEN[];   /* the pictures' file of a group */
extern const PTMF SubScreen_StatePageOpen_ptmf;

/* load the art gallery's group `g` of pictures into the work buffer (any load in progress
 * cancelled first) */
static inline void art_group_load(SubScreen *s, VObject *ld, u8 g) {
    char name[0x20];

    SUB_PAGE(s, 0x3, u8) = 1;
    func_0026EDD0(name, 0x20, str_SUBSCR_ARTN_LEN, g);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(
        ld, name, VCALL(gProgress, 0x88, void *(*)(Progress *))(gProgress), 0x6000000, 0);
}

/* state: the art gallery's list - up / down round the group of 8 (48 pictures), left /
 * previous and right / next to the group before / after (its pictures loaded), confirm an
 * unlocked picture (state SubScreen_StatePageOpen_ptmf; a buzzer if locked), cancel closes; then the list */
/* 0x00387F00 */
void Gallery_StateArtList(SubScreen *s) {
    if (!s->fading) {
        VObject *ld = gFileLoader;
        u8 *k = &SUB_PAGE(s, 0x0, u8);
        u32 pad;

        if (VCALL(ld, 0x28, s32 (*)(VObject *, u32))(ld, 0x6000000) != 2) {
            SUB_PAGE(s, 0x3, u8) = 0;
        }
        SUB_PAGE(s, 0x1, u8) = *k;
        pad = gMenuPressed;
        if (pad & MENU_CONFIRM) {
            u16 f = D_0044BFE0[*k];

            if (AT(gGamePtr, 0x24 + (f >> 5) * 4, u32) & (1 << (f & 0x1F))) {
                ptmf_set(&s->state, &SubScreen_StatePageOpen_ptmf);
                Sound_PlaySE(SE_DECIDE);
            } else {
                Sound_PlaySE(SE_BUZZER);
            }
        } else if (pad & MENU_UP) {
            if (*k == 0) {
                *k = 7;
            } else {
                *k = (*k - 1) % 8 + (*k >> 3) * 8;
            }
            if (*k >= 0x30) {
                *k = 0x2F;
            }
        } else if (pad & MENU_DOWN) {
            *k = (*k + 1) % 8 + (*k >> 3) * 8;
            if (*k >= 0x30) {
                *k = 0x30;
            }
        } else if (pad & (MENU_LEFT | MENU_PREV)) {
            if (SUB_PAGE(s, 0x3, u8) != 0) {
                VCALL(ld, 0x14, void (*)(VObject *, u32))(ld, 0x6000000);
            }
            *k = (*k >> 3) == 0 ? 0x28 : ((*k >> 3) - 1) * 8;
            art_group_load(s, ld, *k >> 3);
        } else if (pad & (MENU_RIGHT | MENU_NEXT)) {
            if (SUB_PAGE(s, 0x3, u8) != 0) {
                VCALL(ld, 0x14, void (*)(VObject *, u32))(ld, 0x6000000);
            }
            *k = (*k >> 3) < 5 ? ((*k >> 3) + 1) * 8 : 0;
            art_group_load(s, ld, *k >> 3);
        } else if (pad & MENU_CANCEL) {
            s->close = 1;
        }
        if (SUB_PAGE(s, 0x1, u8) != *k) {
            Sound_PlaySE(SE_CURSOR);
        }
    }
    Gallery_DrawArtList(s);
}

/* ---- the clear results ---- */

extern VObject *gAvoidPrompt;   /* the results' pictures (+0x10: x, y, part, alpha) */
extern const char str_1st[], str_2nd[], str_3rd[];   /* "1st" / "2nd" / "3rd" */
extern const char str_N_N_2[], D_00464040[];                 /* "%02d:%02d", "-----" */

#define RESULTS_PART(x, y, part, alpha) \
    VCALL(gAvoidPrompt, 0x10, void (*)(VObject *, s32, s32, s32, s32))(gAvoidPrompt, x, y, part, alpha)

/* state: the clear results - shown in steps (page[0], 30 frames each, page[1]); the "clear"
 * banner pulsing (alpha page[5] between 0x60 and 0x80); then the difficulty's three best
 * times (rank, minutes:seconds, "-----" for none; the new one, page[3], lit; a mark by a new
 * best) and finally the new ending's note (page[2]); confirm or cancel ends (flag 4) */
/* 0x00388FF0 */
void Results_State(SubScreen *s) {
    static const char *const kRank[3] = {str_1st, str_2nd, str_3rd};
    Progress *p;
    u8 diff;
    s32 i;

    if (SUB_PAGE(s, 0x0, u8) == 0 || SUB_PAGE(s, 0x0, u8) == 1) {
        if (--SUB_PAGE(s, 0x1, u8) == 0) {
            SUB_PAGE(s, 0x0, u8)++;
            SUB_PAGE(s, 0x1, u8) = 0x1E;
        }
    }
    if (SUB_PAGE(s, 0x4, u8) == 0) {
        SUB_PAGE(s, 0x5, u8) += 4;
        if (SUB_PAGE(s, 0x5, u8) == 0x80) {
            SUB_PAGE(s, 0x4, u8) = 1;
        }
    } else {
        SUB_PAGE(s, 0x5, u8) -= 4;
        if (SUB_PAGE(s, 0x5, u8) == 0x60) {
            SUB_PAGE(s, 0x4, u8) = 0;
        }
    }
    RESULTS_PART(0x30, 0x70, 2, SUB_PAGE(s, 0x5, u8));
    p = gProgress;
    diff = Progress_GetVar(p, 0x2E);
    if (SUB_PAGE(s, 0x0, u8) == 0) {
        return;
    }
    RESULTS_PART(0x110, 0x100, 3, 0x80);
    RESULTS_PART(0xC0, 0x100, 4, 0x80);
    for (i = 0; i < 3; i++) {
        u8 *e = gGamePtr + diff * 12 + 0x3C + i * 4;
        u8 lit = SUB_PAGE(s, 0x3, u8) == i;
        u8 h, m, sec;

        if (i == 0 && SUB_PAGE(s, 0x3, u8) == 0) {
            RESULTS_PART(0x50, 0x110, 1, 0x80);
        }
        Task_PrintfEx(&s->text, 0xD8, 0x110 + i * 0x20, lit, 0x80, 0x33, kRank[i]);
        h = e[0];
        m = e[1];
        sec = e[2];
        if (h == 0x63 && m == 0x3B && sec == 0x3B) {
            Task_PrintfEx(&s->text, 0x105, 0x110 + i * 0x20, lit, 0x80, 0x33, D_00464040);
            continue;
        }
        if (h != 0) {
            m = 0x3B;
            sec = 0x3B;
        }
        Task_PrintfEx(&s->text, 0x108, 0x110 + i * 0x20, lit, 0x80, 0x33, str_N_N_2, m, sec);
    }
    if (SUB_PAGE(s, 0x0, u8) < 2) {
        return;
    }
    if (SUB_PAGE(s, 0x2, u8) != 0) {
        RESULTS_PART(0x50, 0xD0, 0, 0x80);
    }
    if (gMenuPressed & MENU_CANCEL) {
        Progress_SetFlag(p, 4);
    } else if (gMenuPressed & MENU_CONFIRM) {
        Progress_SetFlag(p, 4);
    }
}

/* ---- the model gallery's models ---- */

/* the gallery's model `k` made in the work memory (+0xA8DFC): its class by the entry, the
 * loaded .PCK's parts hooked up (+0x4C0 / +0x4D0 / +0x4CC / +0x4C4), its textures as texture
 * set 2 (+0xA8DF0, and +0xA8DF4 if any), the marker file (+0xA8DF8, model +0x4D4), set up
 * (+0xC), +0x24 2; entries 11 / 12 take variant 0 (+0x34), 15 / 16 the other (+0x2C). NULL past
 * entry 31. */
/* 0x0038C160 */
u8 *Gallery_MakeModel(SubScreen *s, u8 k) {
    u8 *mem = AT(s, 0xA8DFC, u8 *), *m, *pck;
    VObject *bm;

#define NEW(size, ctor) (mem == NULL ? NULL : (m = Model_new(size, mem)) == NULL ? NULL : (u8 *)(ctor))
    switch (k) {
    case 0: m = NEW(0x1270, EventHumanModel_ctor(m, 0)); break;
    case 1: m = NEW(0x18E0, Costume2Model_ctor(m)); break;
    case 2: m = NEW(0x17A0, Costume3Model_ctor(m)); break;
    case 3: m = NEW(0xD50, Costume7Model_ctor(m, 7)); break;
    case 4: m = NEW(0xDE0, Costume6Model_ctor(m, 6)); break;
    case 5: m = NEW(0x9B0, Costume8Model_ctor(m)); break;
    case 6: m = NEW(0xB90, DogModel_ctor(m, 0)); break;
    case 7: m = NEW(0xB90, DogModelA_ctor(m, 1)); break;
    case 8: m = NEW(0xB90, DogModelB_ctor(m, 2)); break;
    case 9: case 10: m = NEW(0xBA0, DebilitasModel_ctor(m)); break;
    case 11: case 12: m = NEW(0x1580, DaniellaModel_ctor(m)); break;
    case 13: case 14: m = NEW(0x1490, RiccardoModel_ctor(m)); break;
    case 15: case 16: m = NEW(0x1310, Kind23Model_ctor(m)); break;
    case 17: case 18: m = NEW(0xD00, LorenzoModel_ctor(m)); break;
    case 19: case 20: m = NEW(0x1500, Kind09Model_ctor(m)); break;
    case 21: case 22: m = NEW(0x1160, Lorenzo2Model_ctor(m)); break;
    case 23: m = NEW(0x9A0, Kind12Model_ctor(m)); break;
    case 25: m = NEW(0x890, Kind33Model_ctor(m)); break;
    case 27: m = NEW(0x890, Kind14Model_ctor(m)); break;
    case 24: case 26: case 28: case 29: case 30: case 31: m = NEW(0x890, ModelBase_ctor(m)); break;
    default:
        return NULL;
    }
#undef NEW
    pck = AT(s, 0xA8DEC, u8 *);
    AT(m, 0x4C0, u8 *) = AT(pck, 0x4, s32) ? pck + AT(pck, 0x4, s32) : NULL;
    AT(m, 0x4D0, u8 *) = AT(pck, 0x8, s32) ? pck + AT(pck, 0x8, s32) : NULL;
    AT(m, 0x4CC, u8 *) = AT(pck, 0xC, s32) ? pck + AT(pck, 0xC, s32) : NULL;
    AT(m, 0x4C4, u8 *) = AT(pck, 0x10, s32) ? pck + AT(pck, 0x10, s32) : NULL;
    bm = gBootMessage;
    VCALL(bm, 0x8, void (*)(VObject *, s32, void *))(bm, 2, AT(s, 0xA8DF0, void *));
    if (AT(s, 0xA8DF4, void *) != NULL) {
        VCALL(bm, 0x10, void (*)(VObject *, s32, void *, s32))(bm, 2, AT(s, 0xA8DF4, void *), 0);
    }
    AT(m, 0x4D4, void *) = AT(s, 0xA8DF8, void *);
    VCALL(m, 0xC, void (*)(u8 *))(m);
    AT(m, 0x24, u8) = 2;
    if (k == 0x10 || k == 0xF) {
        VCALL(m, 0x2C, void (*)(u8 *))(m);
    } else if (k == 0xC || k == 0xB) {
        VCALL(m, 0x34, void (*)(u8 *, s32))(m, 0);
    }
    return m;
}

/* ---- the word plate's letters ---- */

typedef struct WordKey {
    u16 ch;     /* the letter; '@' done */
    u16 x, y;   /* its place on the screen */
} WordKey;

extern WordKey D_0044B640[3][10];
extern const char str_N_9[];                    /* the plates left */

#define SUB_WORD_CURSOR(s) ((s)->unkA8C57)          /* row << 4 | column */
#define SUB_WORD(s) ((s)->unkA8C58)                 /* the word, 8 letters */
#define SUB_WORD_LEN(s) ((s)->unkA8C60)
#define WORD_KEY(c) (D_0044B640[(c) >> 4][(c) & 0xF])

/* the key on from `c` along its row (right, else left; round), past the key's other cells */
static u8 word_step(u8 c, s32 right) {
    WordKey *row = D_0044B640[c >> 4];
    s32 col = c & 0xF, next;

    for (;;) {
        next = right ? (col + 1) % 10 : col != 0 ? col - 1 : 9;
        if (row[col].ch != row[next].ch) {
            break;
        }
        col = next;
    }
    return (c & 0xF0) + next;
}

/* the cursor on the first key showing letter `ch` */
static void word_find(SubScreen *s, u16 ch) {
    s32 r, c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 10; c++) {
            if (ch == D_0044B640[r][c].ch) {
                SUB_WORD_CURSOR(s) = r << 4 | c;
                return;
            }
        }
    }
}

/* in the golem rooms (0x23 / 0x49 / 0x66) */
static inline s32 word_golem_room(Progress *p) {
    return VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x23 || VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x49 ||
           VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x66;
}

/* state: making a word plate - confirm types the key's letter (8 at most, then the cursor on
 * "done") or, on done, makes the plate (fewer than 10, a word typed: the word plate added, the
 * word in parameter 3, flag 4, progress +0x20 bit 0; in the golem rooms quietly, sound 2);
 * cancel takes a letter back (none: closes); start goes to done; up / down by rows, left /
 * right by keys, next / previous to the letter after / before. Then the word, its cursor, the
 * key's light and the plates left */
/* 0x003908B0 */
void SubScreen_StateWordMake(SubScreen *s) {
    s32 i;
    u8 ch[2];

    if (!s->fading) {
        if (gMenuPressed & MENU_CONFIRM) {
            u16 c = WORD_KEY(SUB_WORD_CURSOR(s)).ch;

            if (c == '@') {
                if ((u32)Items_CountItem3F(s->pool) < 10) {
                    if (SUB_WORD_LEN(s) != 0) {
                        Progress *p;
                        char word[9];

                        Items_NewItem3F(s->pool, SUB_WORD(s));
                        for (i = 0; i < 8; i++) {
                            word[i] = SUB_WORD(s)[i];
                        }
                        word[8] = 0;
                        Msg_PrintfParam(&s->text, 3, kFmtString, word);
                        p = gProgress;
                        Progress_SetFlag(p, 4);
                        if (word_golem_room(p)) {
                            Sound_Play(gSound, 2, 6);
                            s->quietClose = 1;
                        }
                        AT(p, 0x20, u32) |= 1;
                    }
                } else {
                    Progress_SetFlag(gProgress, 4);
                }
            } else if (SUB_WORD_LEN(s) < 8) {
                SUB_WORD(s)[SUB_WORD_LEN(s)] = c;
                SUB_WORD_LEN(s)++;
                if (SUB_WORD_LEN(s) == 8) {
                    SUB_WORD_CURSOR(s) = 0x27;
                }
                if (word_golem_room(gProgress)) {
                    Sound_Play(gSound, 0, 6);
                } else {
                    Sound_PlaySE(SE_DECIDE);
                }
            }
        } else if (gMenuPressed & MENU_CANCEL) {
            if (SUB_WORD_LEN(s) == 0) {
                Progress_SetFlag(gProgress, 4);
            } else {
                SUB_WORD_LEN(s)--;
                SUB_WORD(s)[SUB_WORD_LEN(s)] = 0;
                Sound_PlaySE(SE_CANCEL);
            }
        } else {
            u8 old = SUB_WORD_CURSOR(s);

            if (gPadPressed & PAD_START) {
                SUB_WORD_CURSOR(s) = 0x27;
            }
            if (gMenuPressed & MENU_UP) {
                SUB_WORD_CURSOR(s) += SUB_WORD_CURSOR(s) < 0x10 ? 0x20 : -0x10;
            } else if (gMenuPressed & MENU_DOWN) {
                SUB_WORD_CURSOR(s) += SUB_WORD_CURSOR(s) < 0x20 ? 0x10 : -0x20;
            }
            if (gMenuPressed & MENU_LEFT) {
                SUB_WORD_CURSOR(s) = word_step(SUB_WORD_CURSOR(s), 0);
            } else if (gMenuPressed & MENU_RIGHT) {
                SUB_WORD_CURSOR(s) = word_step(SUB_WORD_CURSOR(s), 1);
            }
            if (gMenuPressed & MENU_NEXT) {
                u16 c = WORD_KEY(SUB_WORD_CURSOR(s)).ch;

                word_find(s, c == 'Z' ? '@' : c + 1);
            } else if (gMenuPressed & MENU_PREV) {
                u16 c = WORD_KEY(SUB_WORD_CURSOR(s)).ch;

                word_find(s, c == '@' ? 'Z' : c - 1);
            }
            if (old != SUB_WORD_CURSOR(s)) {
                Sound_PlaySE(SE_CURSOR);
            }
        }
    }
    s->kind = 0x84;
    sub_panels(s);
    ch[1] = 0;
    for (i = 0; i < 8; i++) {
        if (SUB_WORD(s)[i] != 0) {
            ch[0] = SUB_WORD(s)[i];
            Task_ShowText(&s->text, 0x68 + i * 0x28, 0x54, 0, ch, 0x80, 0x30, 0x20, 0x24);
        }
    }
    if (SUB_WORD_LEN(s) < 8) {
        SubScreen_DrawPart(s, (u16)(SUB_WORD_LEN(s) * 40 + 0x60), 0x50, 0xF, 0x80, 0);
    }
    SubScreen_DrawPart(s, WORD_KEY(SUB_WORD_CURSOR(s)).x, WORD_KEY(SUB_WORD_CURSOR(s)).y, 0xE, 0x40, 0);
    SubScreen_DrawPart(s, 0x180, 0x140, 0xD, 0x80, 0);
    Task_Printf(&s->text, 0x198, 0x148, 0, str_N_9, 10 - Items_CountItem3F(s->pool));
}

/* ---- the movie list ---- */

extern const u8 D_0044B730[];     /* each movie's seen flag */
extern const char str_N_8[];   /* "%3d" */

#define MOVIE_COUNT 0x6E

typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

/* movie `i` was seen (or everything is open) */
static s32 movie_seen(s32 i) {
    u32 *flags = &AT(gGamePtr, 0x24, u32);
    u8 f;

    if (flags[0] & 0x200000) {
        return 1;
    }
    f = D_0044B730[i];
    return (flags[f >> 5] & (1 << (f & 0x1F))) != 0;
}

/* The movie list (kind 0x8B), 8 a page: up / down round the page, left / right (prev / next)
 * turn it; cancel ends (flag 4, var 0x2B the cursor, 0x2A none), confirm on a seen movie ends
 * playing it (var 0x2A). Its panels, the titles ("???" unseen), the page count, the blinking
 * arrows and the cursor's thumbnail beside the list. */
/* 0x0038E0C0 */
void SubScreen_StateMovies(SubScreen *s) {
    Task *t = &s->text;
    s32 base, i;

    if (!s->fading) {
        u8 cur = s->page[0];
        u32 pad = gMenuPressed;

        base = (cur >> 3) * 8;
        if (pad & MENU_UP) {
            s->page[0] = cur - 1;
            if ((s8)s->page[0] < base) {
                s->page[0] = base + 7;
            }
            if (s->page[0] >= MOVIE_COUNT) {
                s->page[0] = MOVIE_COUNT - 1;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_DOWN) {
            s->page[0] = s->page[0] + 1;
            if (base + 7 < s->page[0]) {
                s->page[0] = base;
            }
            if (s->page[0] >= MOVIE_COUNT) {
                s->page[0] = base;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & (MENU_LEFT | MENU_PREV)) {
            s->page[0] -= 8;
            if ((s8)s->page[0] < 0) {
                s->page[0] = MOVIE_COUNT - 1;
            }
            s->page[0] = (s->page[0] >> 3) * 8;
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & (MENU_RIGHT | MENU_NEXT)) {
            s->page[0] += 8;
            s->page[0] = (s->page[0] >> 3) * 8;
            if (s->page[0] >= MOVIE_COUNT) {
                s->page[0] = 0;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_CANCEL) {
            Progress *p = gProgress;

            Progress_SetFlag(p, 4);
            Progress_SetVar(p, 0x2B, s->page[0]);
            Progress_SetVar(p, 0x2A, 0xFF);
        } else if (pad & MENU_CONFIRM) {
            if (cur < MOVIE_COUNT && movie_seen(cur)) {
                Progress *p = gProgress;

                Progress_SetFlag(p, 4);
                Progress_SetVar(p, 0x2B, s->page[0]);
                Progress_SetVar(p, 0x2A, s->page[0]);
                Sound_PlaySE(SE_DECIDE);
                s->quietClose = 1;
            } else {
                Sound_PlaySE(SE_BUZZER);
            }
        }
    }

    s->kind = 0x8B;
    sub_panels(s);
    gLanguage = 1;
    Task_ShowText(t, 0x30, 0x3B, 0x80, Task_MessageText(t, 0xF), 0x80, 0x30, 0x10, 0x15);
    Task_ShowText(t, 0x58, 0x3B, 0x80, Task_MessageText(t, 0x10), 0x80, 0x30, 0x10, 0x15);
    base = (s->page[0] >> 3) * 8;
    for (i = 0; i < 8; i++) {
        s32 n = base + i, y = 0x5E + i * 0x23;
        u16 id;
        u8 color;

        if (n >= MOVIE_COUNT) {
            continue;
        }
        id = movie_seen(n) ? (u16)(base + 0x100 + i) : 0x16E;
        color = s->page[0] == n ? 0x82 : 0x80;
        Task_Printf(t, 0x30, y, color, str_N_8, n + 1);
        Task_ShowText(t, 0x58, y, color, Task_MessageText(t, id), 0x80, 0x30, 0x10, 0x15);
    }
    Task_ShowText(t, 0x46, 0x186, 0x80, Task_MessageText(t, 0x12), 0x80, 0x30, 0x10, 0x15);
    Task_ShowText(t, (u16)(Task_MessageWidth(t, 0x12, 0x10) + 0x56), 0x186, 0x80, Task_MessageText(t, 0x13), 0x80,
                  0x30, 0x10, 0x15);
    Task_Printf(t, 0x186, 0x176, 0x80, kFmtSlash, base / 8 + 1, 0xE);
    {
        s32 b = 0x80 - ((s->frame << 2) & 0xFF);
        u8 alpha = b > 0 ? b : -b;

        SubScreen_DrawPart(s, 0x168, 0x170, 0x1A, alpha, 0);
        SubScreen_DrawPart(s, 0x1B3, 0x170, 0x1B, alpha, 0);
    }
    if (s->page[0] < MOVIE_COUNT) {
        if (movie_seen(s->page[0])) {
            VObject *tc = gTexCache;
            u8 *thumbs = (u8 *)gProgress + 0x16C0;

            VCALL(tc, 0x10, void (*)(VObject *, void *, s32))(tc, thumbs + ((u32 *)thumbs)[s->page[0] + 1], 0x27);
            VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0x158, 0xB0, 0x70, 0x60, 0, 0, 0x70, 0x60, 0x80808080,
                                                 0, 0x27, 0x30, -1);
            VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0x27);
        } else {
            VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0x158, 0xB0, 0x70, 0x60, 0x10, 0, 0x70, 0x60, 0x80808080,
                                                 0, 0x1B, 0x30, 1);
        }
    }
    VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0x140, 0x80, 0x10, 0xC0, 0, 0, 0x10, 0xC0, 0x80808080, 0, 0x1B,
                                         0x30, 0);
}

/* ---- the costume page's choosing ---- */

typedef struct {
    u16 name;   /* the costume's name */
    u16 note;   /* its description */
} CostumeText;
extern const CostumeText kCostumeTexts[9];   /* Fiona's six, then Hewie's three */

#define COST_FIONA(s, i) SUB_PAGE(s, (i), u8)        /* Fiona's costumes (0x80 locked, 0xFF ends) */
#define COST_FIONA_SEL(s) SUB_PAGE(s, 0x7, u8)       /* the one worn */
#define COST_HEWIE(s, i) SUB_PAGE(s, 0x8 + (i), u8)  /* Hewie's */
#define COST_HEWIE_SEL(s) SUB_PAGE(s, 0xC, u8)       /* his worn (0xFF: he isn't here) */
#define COST_ROW(s) SUB_PAGE(s, 0xD, u8)             /* 0 Fiona, 1 Hewie, 2 done */
#define COST_OPEN(s) SUB_PAGE(s, 0xE, u8)            /* the row's list is open */
#define COST_FIONA_CUR(s) SUB_PAGE(s, 0xF, u8)
#define COST_HEWIE_CUR(s) SUB_PAGE(s, 0x10, u8)

/* one tile of the costume page's frames (sheet 0x19) */
static void cost_tile(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v) {
    VCALL(gRenderer, 0x7C, RectFn)(gRenderer, x, y, w, h, u, v, w, h, 0x80808080, 2, 0x19, 0x33, 1);
}

/* one row of a frame: its tiles' x, width and u across (`n` of them), at `y` from the
 * sheet's row `v` (0x30 top, 0x50 middle, 0x70 bottom) */
static void cost_row(const s16 (*cols)[3], s32 n, s32 y, s32 h, s32 v) {
    s32 i;

    for (i = 0; i < n; i++) {
        cost_tile(cols[i][0], y, cols[i][1], h, cols[i][2], v);
    }
}

/* the costume page's frames: the rows' (one more with Hewie here), then - not on "done" -
 * the row's costumes' (as many as are unlocked) and, with that list open, the description's */
/* 0x0038EAE0 */
void Costumes_DrawFrames(SubScreen *s) {
    static const s16 kRows[2][3] = {{0x10, 0x68, 0}, {0x78, 0x20, 0x98}};
    static const s16 kList[3][3] = {{0xA8, 0x98, 0}, {0x140, 0x78, 0x20}, {0x1B8, 0x38, 0x80}};
    static const s16 kNote[4][3] = {{0x10, 0x98, 0}, {0xA8, 0x78, 0x20}, {0x120, 0x78, 0x20}, {0x198, 0x58, 0x60}};
    s32 rows = COST_HEWIE_SEL(s) == 0xFF ? 3 : 4;
    s32 i, n, k;

    cost_row(kRows, 2, 0x18, 0x20, 0x30);
    for (i = 1; i < rows; i++) {
        cost_row(kRows, 2, 0x18 + i * 0x20, 0x20, 0x50);
    }
    cost_row(kRows, 2, 0x18 + i * 0x20, 0x20, 0x70);
    if (COST_ROW(s) == 2) {
        return;
    }
    cost_row(kList, 3, 0x20, 0x20, 0x30);
    k = COST_ROW(s) == 0 ? 0 : 8;
    n = 0;
    do {
        if (!(SUB_PAGE(s, k, u8) & 0x80)) {
            n++;
        }
    } while (SUB_PAGE(s, k++, u8) != 0xFF);
    for (i = 1; i < n + 1; i++) {
        cost_row(kList, 3, 0x20 + i * 0x20, 0x20, 0x50);
    }
    cost_row(kList, 3, 0x20 + i * 0x20, 0x20, 0x70);
    if (COST_OPEN(s) == 0) {
        return;
    }
    cost_row(kNote, 4, 0x128, 0x20, 0x30);
    for (i = 1; i < 3; i++) {
        cost_row(kNote, 4, 0x128 + i * 0x20, 0x20, 0x50);
    }
    cost_row(kNote, 4, 0x188, 0x10, 0x50);
    cost_row(kNote, 4, 0x198, 0x20, 0x70);
}

/* the costumes chosen are worn (vars 0x26 / 0x27) and the page ends (flag 4) */
static void costume_done(SubScreen *s, s32 hewie) {
    Progress *p = gProgress;

    Progress_SetVar(p, 0x26, COST_FIONA(s, COST_FIONA_SEL(s)));
    if (hewie) {
        Progress_SetVar(p, 0x27, COST_HEWIE(s, COST_HEWIE_SEL(s)));
    }
    Progress_SetFlag(p, 4);
}

/* state: the costume page. Up / down pick the row (Fiona, Hewie when he is here, done) or,
 * with the row's list open, the costume (past the locked ones, round); left / prev / cancel
 * and right / next / confirm close and open the list, confirm in it choosing the costume and
 * cancel on the rows going to "done", where cancel or confirm wear the choice and end. Then
 * the costumes drawn (Costumes_DrawFrames), the rows, the open costume's description and the
 * row's costumes (the cursor 2, the one worn 1). With no costume of Fiona's at all it just
 * ends. */
/* 0x0038FBE0 */
void Costumes_State(SubScreen *s) {
    s32 hewie = COST_HEWIE_SEL(s) != 0xFF;
    s32 quiet = 0, changed = 0, d = 0, n;
    u8 c0, c1, c2;
    u16 note;

    if (COST_FIONA_SEL(s) == 0xFF) {
        if (!s->fading) {
            Progress_SetFlag(gProgress, 4);
        }
        return;
    }
    if (COST_OPEN(s) == 0) {
        u8 row = COST_ROW(s), r;

        if (gMenuPressed & MENU_UP) {
            d = -1;
        } else if (gMenuPressed & MENU_DOWN) {
            d = 1;
        }
        if (d != 0) {
            r = row + d;
            if (r & 0x80) {
                r = 0;
            }
            if (r >= 3) {
                r = 2;
            }
            if (!hewie && r == 1) {
                r = d < 0 ? 0 : 2;
            }
            if (r != row) {
                changed = 1;
                COST_ROW(s) = r;
            }
        }
    } else {
        if (gMenuPressed & MENU_UP) {
            d = -1;
        } else if (gMenuPressed & MENU_DOWN) {
            d = 1;
        }
        if (d != 0) {
            u8 old, c;

            if (COST_ROW(s) == 0) {
                old = c = COST_FIONA_CUR(s);
                do {
                    c += d;
                    if (c & 0x80) {
                        c = 5;
                    } else if (c >= 6) {
                        c = 0;
                    }
                } while (COST_FIONA(s, c) & 0x80);
                if (old != c) {
                    changed = 1;
                    COST_FIONA_CUR(s) = c;
                }
            } else {
                old = c = COST_HEWIE_CUR(s);
                do {
                    c += d;
                    if (c & 0x80) {
                        c = 2;
                    } else if (c >= 3) {
                        c = 0;
                    }
                } while (COST_HEWIE(s, c) & 0x80);
                if (old != c) {
                    changed = 1;
                    COST_HEWIE_CUR(s) = c;
                }
            }
        }
    }
    if (!changed) {
        u32 pad = gMenuPressed;
        u8 old = COST_OPEN(s);

        d = 0;
        if (pad & (MENU_PREV | MENU_LEFT | MENU_CANCEL)) {
            d = -1;
        } else if (pad & (MENU_NEXT | MENU_RIGHT | MENU_CONFIRM)) {
            d = 1;
        }
        COST_OPEN(s) += (u8)d;
        if (COST_OPEN(s) & 0x80) {
            COST_OPEN(s) = 0;
        }
        if (COST_OPEN(s) >= 2) {
            COST_OPEN(s) = 1;
        }
        if (COST_OPEN(s) == 1 && COST_ROW(s) == 2) {
            COST_OPEN(s) = 0;
        }
        if (old != COST_OPEN(s)) {
            changed = 1;
            if (gMenuPressed & MENU_CANCEL) {
                if (COST_OPEN(s) == 0) {
                    quiet = 1;
                }
            } else if (gMenuPressed & MENU_CONFIRM) {
                if (old == 0) {
                    quiet = 1;
                }
            }
        }
    }
    if (changed) {
        if (!quiet) {
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & MENU_CANCEL) {
            Sound_PlaySE(SE_CANCEL);
        } else if (gMenuPressed & MENU_CONFIRM) {
            Sound_PlaySE(SE_DECIDE);
        }
    } else if ((gMenuPressed & MENU_CONFIRM) && COST_OPEN(s) != 0) {
        if (COST_ROW(s) == 0) {
            COST_FIONA_SEL(s) = COST_FIONA_CUR(s);
        } else {
            COST_HEWIE_SEL(s) = COST_HEWIE_CUR(s);
        }
        Sound_PlaySE(SE_DECIDE);
        COST_OPEN(s) = 0;
    }
    if (!s->fading) {
        if ((gMenuPressed & MENU_CANCEL) && !quiet) {
            if (COST_OPEN(s) == 0) {
                if (COST_ROW(s) != 2) {
                    COST_ROW(s) = 2;
                    Sound_PlaySE(SE_CANCEL);
                } else {
                    costume_done(s, hewie);
                }
            }
        } else if ((gMenuPressed & MENU_CONFIRM) && COST_OPEN(s) == 0 && COST_ROW(s) == 2) {
            costume_done(s, hewie);
        }
    }

    Costumes_DrawFrames(s);
    c0 = c1 = c2 = 0;
    note = 0;
    switch (COST_ROW(s)) {
    case 0:
        c0 = COST_OPEN(s) == 1 ? 1 : 2;
        note = kCostumeTexts[COST_FIONA_CUR(s)].note;
        break;
    case 1:
        c1 = COST_OPEN(s) == 1 ? 1 : 2;
        note = kCostumeTexts[6 + COST_HEWIE_CUR(s)].note;
        break;
    case 2:
        c2 = 2;
        break;
    }
    n = 0;
    Task_ShowText(&s->text, 0x30, 0x40, c0, Task_MessageText(&s->text, 0x42), 0x80, 0x33, 0x10, 0x15);
    n++;
    if (hewie) {
        Task_ShowText(&s->text, 0x30, n * 0x20 + 0x40, c1, Task_MessageText(&s->text, 0x43), 0x80, 0x33, 0x10, 0x15);
        n++;
    }
    Task_ShowText(&s->text, 0x30, n * 0x20 + 0x40, c2, Task_MessageText(&s->text, 0x44), 0x80, 0x33, 0x10, 0x15);
    if (COST_ROW(s) != 2) {
        s32 i, y = 0x48;

        if (COST_OPEN(s) != 0) {
            Task_ShowText(&s->text, 0x30, 0x150, 0, Task_MessageText(&s->text, note), 0x80, 0x33, 0x10, 0x15);
        }
        if (COST_ROW(s) == 0) {
            for (i = 0; COST_FIONA(s, i) != 0xFF; i++) {
                if (!(COST_FIONA(s, i) & 0x80)) {
                    u8 c = i == COST_FIONA_CUR(s) && COST_OPEN(s) ? 2 : i == COST_FIONA_SEL(s) ? 1 : 0;

                    Task_ShowText(&s->text, 0xC0, y, c, Task_MessageText(&s->text, kCostumeTexts[i].name), 0x80, 0x33,
                                  0x10, 0x15);
                    y += 0x20;
                }
            }
        } else {
            for (i = 0; COST_HEWIE(s, i) != 0xFF; i++) {
                if (!(COST_HEWIE(s, i) & 0x80)) {
                    u8 c = i == COST_HEWIE_CUR(s) && COST_OPEN(s) ? 2 : i == COST_HEWIE_SEL(s) ? 1 : 0;

                    Task_ShowText(&s->text, 0xC0, y, c, Task_MessageText(&s->text, kCostumeTexts[6 + i].name), 0x80,
                                  0x33, 0x10, 0x15);
                    y += 0x20;
                }
            }
        }
    }
}

/* ---- the extras menu ---- */

#define EXTRA_COUNT(s) SUB_PAGE(s, 0x6, u8)   /* the entries (page[0..]: which ones, 0..5) */
#define EXTRA_CUR(s) SUB_PAGE(s, 0x7, u8)

/* state: the extras menu (kind 0x80) - up / down round the entries; the frames (one longer
 * per entry past three); for the entry under the cursor its picture (the extras' frame for 0;
 * the difficulties' (1..4) with their three best times) and its note; cancel leaves (var
 * 0x2E none), confirm on a difficulty or the last entry picks it (var 0x2E: 0..3, 0xFF) */
/* 0x003894F0 */
void SubScreen_StateExtrasMenu(SubScreen *s) {
    Task *t = &s->text;
    s32 i;
    u8 e;

    if (gMenuPressed & MENU_UP) {
        Sound_PlaySE(SE_CURSOR);
        if (EXTRA_CUR(s) == 0) {
            EXTRA_CUR(s) = EXTRA_COUNT(s) - 1;
        } else {
            EXTRA_CUR(s) = EXTRA_CUR(s) - 1;
        }
    } else if (gMenuPressed & MENU_DOWN) {
        Sound_PlaySE(SE_CURSOR);
        EXTRA_CUR(s) = EXTRA_CUR(s) + 1;
        if (EXTRA_CUR(s) == EXTRA_COUNT(s)) {
            EXTRA_CUR(s) = 0;
        }
    }
    SubScreen_DrawPart(s, 0x40, 0x10, 0x1D, 0x80, 1);
    SubScreen_DrawPart(s, 0, 0x40, 0x20, 0x80, 1);
    SubScreen_DrawPart(s, 0, 0x70, 0x21, 0x80, 1);
    SubScreen_DrawPart(s, 0, 0x90, 0x21, 0x80, 1);
    SubScreen_DrawPart(s, 0, 0xB0, 0x21, 0x80, 1);
    switch (EXTRA_COUNT(s)) {
    case 4:
        SubScreen_DrawPart(s, 0, 0xD0, 0x22, 0x80, 1);
        break;
    case 5:
        SubScreen_DrawPart(s, 0, 0xD0, 0x21, 0x80, 1);
        SubScreen_DrawPart(s, 0, 0xF0, 0x22, 0x80, 1);
        break;
    case 6:
        SubScreen_DrawPart(s, 0, 0xD0, 0x21, 0x80, 1);
        SubScreen_DrawPart(s, 0, 0xF0, 0x21, 0x80, 1);
        SubScreen_DrawPart(s, 0, 0x110, 0x22, 0x80, 1);
        break;
    }
    e = SUB_PAGE(s, EXTRA_CUR(s), u8);
    if (e == 0) {
        SubScreen_DrawPart(s, 0x160, 0x40, 0x23, 0x80, 1);
        SubScreen_DrawPart(s, 0xC0, 0x40, 0x24, 0x80, 1);
        for (i = 0; i < 6; i++) {
            SubScreen_DrawPart(s, 0x160, 0x80 + i * 0x20, 0x25, 0x80, 1);
        }
        SubScreen_DrawPart(s, 0x160, 0x140, 0x27, 0x80, 1);
        for (i = 0; i < 6; i++) {
            SubScreen_DrawPart(s, 0xC0, 0x80 + i * 0x20, 0x26, 0x80, 1);
        }
        SubScreen_DrawPart(s, 0xC0, 0x140, 0x28, 0x80, 1);
        SubScreen_DrawPart(s, 0x160, 0x150, 0x29, 0x80, 1);
        SubScreen_DrawPart(s, 0xC0, 0x150, 0x2A, 0x80, 1);
    } else if (e < 5) {
        SubScreen_DrawPart(s, 0xC0, 0x40, e == 1 || e == 3 ? 0x1E : 0x1F, 0x80, 1);
        SubScreen_DrawPart(s, 0xC0, 0x140, 0x20, 0x80, 1);
        SubScreen_DrawPart(s, 0xC0, 0x170, 0x21, 0x80, 1);
        SubScreen_DrawPart(s, 0xC0, 0x190, 0x22, 0x80, 1);
        Task_PrintfEx(t, 0xE0, 0x15A, 0, 0x80, 0x33, str_1st);
        Task_PrintfEx(t, 0xE0, 0x172, 0, 0x80, 0x33, str_2nd);
        Task_PrintfEx(t, 0xE0, 0x18A, 0, 0x80, 0x33, str_3rd);
    }
    for (i = 0; i < EXTRA_COUNT(s); i++) {
        u8 k = SUB_PAGE(s, i, u8);

        if (k < 6) {
            Task_ShowText(t, 0x20, 0x68 + i * 0x20, EXTRA_CUR(s) == i ? 2 : 0, Task_MessageText(t, 0x60 + k), 0x80,
                          0x33, 0x10, 0x15);
        }
    }
    e = SUB_PAGE(s, EXTRA_CUR(s), u8);
    if (e == 0) {
        Task_ShowText(t, 0xE0, 0x68, 0, Task_MessageText(t, 0x66), 0x80, 0x33, 0x10, 0x15);
    } else if (e < 5) {
        Task_ShowText(t, 0xE0, 0x70, 0, Task_MessageText(t, 0x66 + e), 0x80, 0x33, 0x10, 0x15);
    }
    e = SUB_PAGE(s, EXTRA_CUR(s), u8);
    if (e >= 1 && e <= 4) {
        u8 *rec = (u8 *)gGamePtr + (e - 1) * 12;

        for (i = 0; i < 3; i++, rec += 4) {
            u8 m = rec[0x3C], sec = rec[0x3D], f = rec[0x3E];

            if (m == 99 && sec == 59 && f == 59) {
                Task_PrintfEx(t, 0x121, 0x15A + i * 0x18, 0, 0x80, 0x33, D_00464040);
            } else {
                Task_PrintfEx(t, 0x124, 0x15A + i * 0x18, 0, 0x80, 0x33, str_N_N_2, sec, f);
            }
        }
    }
    if (!s->fading) {
        if (gMenuPressed & MENU_CANCEL) {
            Sound_PlaySE(SE_CANCEL);
            Progress_SetFlag(gProgress, 4);
            Progress_SetVar(gProgress, 0x2E, 0xFF);
        } else if (gMenuPressed & MENU_CONFIRM) {
            e = SUB_PAGE(s, EXTRA_CUR(s), u8);
            if (e >= 1 && e <= 5) {
                Progress *p;

                Sound_PlaySE(SE_DECIDE);
                p = gProgress;
                Progress_SetFlag(p, 4);
                Progress_SetVar(p, 0x2E, e == 5 ? 0xFF : e - 1);
            }
        }
    }
}

/* 0x0038A2C0 */
s32 SubScreen_EntryMotions(void *o, u32 i) {
    return D_0044BF10[i];
}

/* ---- the model gallery ---- */

extern void *Fog_vtable[], *ScreenBlend_vtable[];
extern const PTMF SubScreen_StateExtras_ptmf, SubScreen_DrawFadeFromBlack_ptmf;

#define GALLERY_STEP(s) SUB_PAGE(s, 0x2, u8)   /* 0 fading in, 1 shown, 2..4 the help, 5 leaving */
#define GALLERY_BARS(s) AT(s, 0xA8E3C, u8)    /* the name and help bars shown */

/* the entry's motion page[1] played (past 0x19 blended in) */
static void gallery_motion(SubScreen *s, u8 *m) {
    u8 k = SUB_PAGE(s, 0x0, u8);

    if (k < 0x18 || k == 0x19) {
        Motion_PlayTable(m, kGalleryMotions[k][SUB_PAGE(s, 0x1, u8)], -1);
    } else {
        Motion_PlayWith(m, kGalleryMotions[k][SUB_PAGE(s, 0x1, u8)], 1, -1, 5.0f);
    }
}

/* room effect `k`'s slot made anew (class `vtbl`) */
static void gallery_effect_new(u8 *fx, VObject **slot, void **vtbl) {
    VObject *pool = (VObject *)(fx + 0x1400);
    void *mem;

    if (*slot != NULL) {
        VCALL(pool, 0x14, void (*)(VObject *, void *))(pool, *slot);
        *slot = NULL;
    }
    mem = VCALL(pool, 0x10, void *(*)(VObject *, u32))(pool, 0xA0);
    if (mem != NULL) {
        VObject *e = RoomEffects_new(0xA0, mem);

        if (e != NULL) {
            e->vtbl = vtbl;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(VObject *))(*slot);
    }
}

/* room effect `k` given back its 0x90 bytes kept in `save` */
static void gallery_effect_back(u8 *fx, s32 k, const u8 *save) {
    u64 *dst = (u64 *)(RoomEffects_Get(fx, k) + 0x10);
    s32 i;

    for (i = 0; i < 18; i++) {
        dst[i] = AT(save, i * 8, u64);
    }
}

/* the model gallery left: the message and model gone, the work buffers cleared, the camera
 * and lights given back, the room effects 0x1F, 0x1D and 0x1E restored (made anew as they
 * were), fading back to the list (SubScreen_StateExtras_ptmf / SubScreen_DrawFadeFromBlack_ptmf) */
static void gallery_leave(SubScreen *s) {
    VObject *msg = gBootMessage;
    VObject *m = (VObject *)SUB_GALLERY_MODEL(s);
    VObject *cam;
    u8 *fx;

    ptmf_set(&s->state, &SubScreen_StateExtras_ptmf);
    VCALL(msg, 0x14, void (*)(VObject *, s32))(msg, 2);
    VCALL(msg, 0xC, void (*)(VObject *, s32))(msg, 2);
    VCALL(m, 0x10, void (*)(VObject *))(m);
    AT(s, 0xA8DF0, u8 *) = NULL;
    AT(s, 0xA8DEC, u8 *) = NULL;
    AT(s, 0xA8DFC, u8 *) = NULL;
    AT(s, 0xA8DF8, u8 *) = NULL;
    AT(s, 0xA8DF4, u8 *) = NULL;
    if (m != NULL) {
        VCALL(m, 0x8, void (*)(VObject *, s32))(m, 1);
    }
    cam = gCamera;
    SUB_GALLERY_MODEL(s) = NULL;
    VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, AT(s, 0xA8E00, f32), AT(s, 0xA8E04, f32),
                                                         AT(s, 0xA8E08, f32));
    VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, AT(s, 0xA8E10, f32), AT(s, 0xA8E14, f32),
                                                         AT(s, 0xA8E18, f32));
    VCALL(cam, 0x14, void (*)(VObject *))(cam);
    VCALL(gCamDirector, 0x10, void (*)(VObject *))(gCamDirector);
    VCALL(gLights, 0x48, void (*)(VObject *, s32, f32 *, f32))(gLights, 0, NULL, 0.0f);
    fx = gRoomEffects;
    RoomEffects_Release(fx, 0x1F);
    gallery_effect_new(gRoomEffects, &AT(fx, 0x14B4, VObject *), Tint_vtable);
    gallery_effect_back(gRoomEffects, 0x1F, (u8 *)s + 0xA8E40);
    gallery_effect_new(gRoomEffects, &AT(gRoomEffects, 0x14AC, VObject *), Fog_vtable);
    gallery_effect_back(gRoomEffects, 0x1D, (u8 *)s + 0xA8ED0);
    gallery_effect_new(gRoomEffects, &AT(gRoomEffects, 0x14B0, VObject *), ScreenBlend_vtable);
    gallery_effect_back(fx, 0x1E, (u8 *)s + 0xA8F60);
    s->fading = 1;
    s->fade = 0x80;
    s->fadeStep = -0x20;
    ptmf_set(&s->draw, &SubScreen_DrawFadeFromBlack_ptmf);
}

/* state: the model gallery. Fading in; then cancel leaves (fading out, gallery_leave) and
 * start shows the help (fading a shade in, start again, back out). Confirm replays the
 * motion, next / previous step through the entry's motions (D_0044BF10, round), select the bars,
 * else the stick turns and moves the camera (Gallery_ModelCamera); the camera placed round the
 * model, the room effects run, the model drawn on a backdrop. The help's text, the bars (the
 * entry's name, the controls, SubScreen_DrawPageCount) and the fade */
/* 0x0038A990 */
void Gallery_StateModel(SubScreen *s) {
    Task *t = &s->text;
    u8 *m;

    Gallery_ModelMotions(s);
    switch (GALLERY_STEP(s)) {
    case 0:
        if (s->fade > 0) {
            s->fade -= 0x20;
        } else {
            GALLERY_STEP(s) = 1;
        }
        break;
    case 2:
        if (s->fade < 0x60) {
            s->fade += 0x10;
        } else {
            GALLERY_STEP(s) = 3;
        }
        break;
    case 3:
        if (gPadPressed & PAD_START) {
            GALLERY_STEP(s) = 4;
            Sound_PlaySE(SE_DECIDE);
        }
        break;
    case 4:
        if (s->fade > 0) {
            s->fade -= 0x10;
        } else {
            GALLERY_STEP(s) = 1;
        }
        break;
    case 5:
        if (s->fade < 0x80) {
            s->fade += 0x20;
        } else {
            gallery_leave(s);
        }
        break;
    default:
        if (gMenuPressed & MENU_CANCEL) {
            GALLERY_STEP(s) = 5;
            Sound_PlaySE(SE_CANCEL);
        } else if (gPadPressed & PAD_START) {
            GALLERY_STEP(s) = 2;
            Sound_PlaySE(SE_DECIDE);
        }
        break;
    }
    m = SUB_GALLERY_MODEL(s);
    if (m != NULL) {
        f32 mat[4][4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));
        f32 at[4] __attribute__((aligned(16)));
        VObject *cam;

        if (gMenuPressed & MENU_CONFIRM) {
            gallery_motion(s, m);
        } else if (gMenuPressed & MENU_NEXT) {
            SUB_PAGE(s, 0x1, u8)++;
            if (D_0044BF10[SUB_PAGE(s, 0x0, u8)] - 1 < SUB_PAGE(s, 0x1, u8)) {
                SUB_PAGE(s, 0x1, u8) = 0;
            }
            gallery_motion(s, SUB_GALLERY_MODEL(s));
        } else if (gMenuPressed & MENU_PREV) {
            SUB_PAGE(s, 0x1, u8)--;
            if ((s8)SUB_PAGE(s, 0x1, u8) < 0) {
                SUB_PAGE(s, 0x1, u8) = D_0044BF10[SUB_PAGE(s, 0x0, u8)] - 1;
            }
            gallery_motion(s, SUB_GALLERY_MODEL(s));
        } else if (gPadPressed & PAD_SELECT) {
            GALLERY_BARS(s) ^= 1;
            Sound_PlaySE(SE_DECIDE);
        } else {
            Gallery_ModelCamera(s);
        }
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = GALLERY_DIST(s);
        v[3] = 1.0f;
        sceVu0UnitMatrix(mat);
        sceVu0RotMatrixY(mat, mat, GALLERY_TURN(s));
        sceVu0TransMatrix(mat, mat, (f32 *)((u8 *)s + 0xA8E20));
        mat[3][1] = GALLERY_HEIGHT(s);
        sceVu0ApplyMatrix(v, mat, v);
        cam = gCamera;
        VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
        sceVu0CopyVector(at, (f32 *)((u8 *)s + 0xA8E20));
        at[1] = at[1] + GALLERY_HEIGHT(s);
        VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, at[0], at[1], at[2]);
        VCALL(cam, 0x14, void (*)(VObject *))(cam);
        RoomEffects_Draw(gRoomEffects);
        if (SUB_GALLERY_MODEL(s) != NULL) {
            sceVu0UnitMatrix(mat);
            sceVu0TransMatrix(mat, mat, (f32 *)((u8 *)s + 0xA8E20));
            m = SUB_GALLERY_MODEL(s);
            VCALL(m, 0x28, void (*)(u8 *, f32 (*)[4]))(m, mat);
            Motion_Hands(SUB_GALLERY_MODEL(s));
            Motion_Eyes(SUB_GALLERY_MODEL(s));
            Motion_Update(SUB_GALLERY_MODEL(s));
            m = SUB_GALLERY_MODEL(s);
            VCALL(m, 0x3C, void (*)(u8 *))(m);
            m = SUB_GALLERY_MODEL(s);
            VCALL(m, 0x38, void (*)(u8 *, s32, s32, s32))(m, 0xA, 0x2E, 0);
        }
        VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x1C0, 0, 0, 0, 0, 0x805C503C, -1, 0, 1, -1);
    }
    if (GALLERY_STEP(s) == 3) {
        Task_ShowText(t, 0x20, 0x20, 0, Task_MessageText(t, 0x1C4), 0x80, 0x33, 0x10, 0x15);
    }
    if (GALLERY_BARS(s)) {
        VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x38, 0, 0, 0, 0, 0x40000000, -1, 0, 0x31, -1);
        VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0x188, 0x200, 0x38, 0, 0, 0, 0, 0x40000000, -1, 0, 0x31, -1);
        Task_ShowText(t, 0x20, 0x10, 1, Task_MessageText(t, (u16)(SUB_PAGE(s, 0x0, u8) + 0x70)), 0x80 - s->fade, 0x33,
                      0x10, 0x15);
        Task_ShowText(t, 0x20, 0x198, 0, Task_MessageText(t, 0x9E), 0x80 - s->fade, 0x33, 0x10, 0x15);
        SubScreen_DrawPageCount((u8 *)s);
    }
    sub_fade_back(s);
}

/* ---- the art gallery's picture ---- */

extern const PTMF Gallery_StateArtList_ptmf;

#define ART_X(s) SUB_PAGE(s, 0x4, s16)       /* the picture's centre on screen */
#define ART_Y(s) SUB_PAGE(s, 0x6, s16)
#define ART_ZOOM(s) SUB_PAGE(s, 0x8, u8)     /* shown at full size */
#define ART_BARS(s) SUB_PAGE(s, 0x9, u8)     /* the name and help bars shown */
#define ART_HELP(s) SUB_PAGE(s, 0xA, u8)
#define ART_STEP(s) SUB_PAGE(s, 0xB, u8)     /* 0 fading in, 1 / 2 the help, 3 / 4 leaving, 0xFF shown */
#define ART_TURN(s) SUB_PAGE(s, 0xC, u8)     /* the moves turned round */

/* a stick past its half way (it pans the picture, 8 pixels at full) */
#define ART_STICK(v) ((v) < -0.5f || !((v) <= 0.5f))

#ifdef HG_NATIVE
/* state: an art gallery picture, the work buffer's 512-wide image (896 lines up to picture
 * 0x15, else 448) at half or full size. Fades in; select the bars, start the help (a shade
 * over it, start again back), confirm turns the moves round, the d-pad / left stick pan, L3
 * centres, R3 zooms about the centre, kept on screen; cancel fades out back to the list
 * (Gallery_StateArtList_ptmf). Then the fade's background, the picture (a GL sprite of the image sent to VRAM)
 * and the bars (its name, the help) */
/* 0x00386990 */
void Gallery_StateArtView(SubScreen *s) {
    Task *t = &s->text;

    switch (ART_STEP(s)) {
    case 0:
    case 2:
        if (s->fade > 0) {
            s->fade -= 0x10;
        } else {
            s->fading = 0;
            s->fade = 0;
            ART_STEP(s) = 0xFF;
        }
        break;
    case 1:
        if (s->fade < 0x60) {
            s->fade += 0x10;
        } else {
            s->fading = 0;
            s->fade = 0x60;
            ART_STEP(s) = 0xFF;
        }
        break;
    case 3:
        if (s->fade < 0x80) {
            s->fade += 0x10;
        } else {
            s->fading = 1;
            s->fade = 0x80;
            s->fadeStep = -0x10;
            ART_STEP(s) = 4;
        }
        break;
    case 4:
        if (s->fade > 0) {
            s->fade -= 0x10;
        } else {
            s->fading = 0;
            s->fade = 0;
            s->fadeStep = 0;
            ptmf_set(&s->state, &Gallery_StateArtList_ptmf);
        }
        break;
    default:
        if (gPadPressed & PAD_SELECT) {
            ART_BARS(s) ^= 1;
            Sound_PlaySE(SE_DECIDE);
        } else if (gPadPressed & PAD_START) {
            if (!s->fading) {
                ART_HELP(s) ^= 1;
                s->fading = 1;
                ART_STEP(s) = ART_HELP(s) ? 1 : 2;
                Sound_PlaySE(SE_DECIDE);
            }
        } else {
            s16 d;

            if (gMenuPressed & MENU_CONFIRM) {
                ART_TURN(s) ^= 1;
                if (ART_TURN(s) == 0) {
                    SubScreen_DrawPart(s, 0x2C, 0xD8, 0x1A, 0x80, 0);
                    SubScreen_DrawPart(s, 0x1B4, 0xD8, 0x1B, 0x80, 0);
                } else {
                    SubScreen_DrawPart(s, 0x1B4, 0xD8, 0x1A, 0x80, 0);
                    SubScreen_DrawPart(s, 0x2C, 0xD8, 0x1B, 0x80, 0);
                }
                Sound_PlaySE(SE_DECIDE);
            }
            d = ART_TURN(s) ? -1 : 1;
            if (gPadHeld & PAD_UP) {
                ART_Y(s) -= d;
            }
            if (gPadHeld & PAD_DOWN) {
                ART_Y(s) += d;
            }
            if (gPadHeld & PAD_LEFT) {
                ART_X(s) -= d;
            }
            if (gPadHeld & PAD_RIGHT) {
                ART_X(s) += d;
            }
            if (ART_STICK(gLeftStick)) {
                ART_X(s) = (s32)((f32)ART_X(s) + (8.0f * (f32)d) * gLeftStick);
            }
            if (ART_STICK(gStickVertical)) {
                ART_Y(s) = (s32)((f32)ART_Y(s) + (8.0f * (f32)d) * gStickVertical);
            }
            if (gPadPressed & PAD_L3) {
                ART_X(s) = 0x100;
                ART_Y(s) = 0xE0;
                Sound_PlaySE(SE_DECIDE);
            }
            if (gPadPressed & PAD_R3) {
                ART_ZOOM(s) ^= 1;
                if (ART_ZOOM(s) == 0) {
                    ART_X(s) = ((ART_X(s) - 0x100) >> 1) + 0x100;
                    ART_Y(s) = ((ART_Y(s) - 0xE0) >> 1) + 0xE0;
                } else {
                    ART_X(s) = ((ART_X(s) - 0x100) << 1) + 0x100;
                    ART_Y(s) = ((ART_Y(s) - 0xE0) << 1) + 0xE0;
                }
                Sound_PlaySE(SE_DECIDE);
            }
            if (ART_ZOOM(s) == 0) {
                if (ART_X(s) < 0x80) {
                    ART_X(s) = 0x80;
                } else if (ART_X(s) > 0x180) {
                    ART_X(s) = 0x180;
                }
                if (ART_Y(s) < 0x70) {
                    ART_Y(s) = 0x70;
                } else if (ART_Y(s) > 0x150) {
                    ART_Y(s) = 0x150;
                }
            } else {
                if (ART_X(s) < 0) {
                    ART_X(s) = 0;
                } else if (ART_X(s) > 0x200) {
                    ART_X(s) = 0x200;
                }
                if (ART_Y(s) < 0) {
                    ART_Y(s) = 0;
                } else if (ART_Y(s) > 0x1C0) {
                    ART_Y(s) = 0x1C0;
                }
            }
        }
        if (!s->fading) {
            if (ART_HELP(s)) {
                Task_ShowText(t, 0x20, 0x20, 0, Task_MessageText(t, 0x1C5), 0x80, 0x33, 0x10, 0x15);
            } else if (gMenuPressed & MENU_CANCEL) {
                s->fading = 1;
                ART_STEP(s) = 3;
                Sound_PlaySE(SE_CANCEL);
            }
        }
        break;
    }
    sub_fade_back(s);
    if (ART_STEP(s) == 4) {
        Gallery_DrawArtList(s);
        return;
    }
    {
        s32 h = SUB_PAGE(s, 0x0, u8) < 0x15 ? 0x380 : 0x1C0;
        s32 hw, hh;
        u8 *buf;

        if (ART_ZOOM(s) == 0) {
            hw = 0x80;
            hh = h >> 2;
        } else {
            hw = 0x100;
            hh = h >> 1;
        }
        buf = VCALL(gProgress, 0x88, u8 *(*)(Progress *))(gProgress);
        if ((u8)VCALL(gRenderer, 0x48, s32 (*)(VObject *, u8 *, s32, s32, s32, s32))(gRenderer, buf + 0x200000,
                                                                                     0x200, h, 0xC0000, 1)) {
            const u8 *tex = gl2d_image(0xC0000 >> 6);

            if (tex != NULL) {
                gl2d_sprite(1, ART_X(s) - hw, ART_Y(s) - hh, ART_X(s) + hw, ART_Y(s) + hh, tex, 0, 0, 0x200, h,
                            0x80808080, 0, 0);
            }
        }
        if (ART_BARS(s)) {
            VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x38, 0, 0, 0, 0, 0x40000000, -1, 0, 0x31, -1);
            VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0x188, 0x200, 0x38, 0, 0, 0, 0, 0x40000000, -1, 0, 0x31,
                                            -1);
            Task_ShowText(t, 0x20, 0x10, 1, Task_MessageText(t, (u16)(SUB_PAGE(s, 0x0, u8) + 0x190)), 0x80 - s->fade,
                          0x33, 0x10, 0x15);
            Task_ShowText(t, 0x20, 0x198, 0, Task_MessageText(t, 0x9E), 0x80 - s->fade, 0x33, 0x10, 0x15);
        }
    }
}
#endif

/* ---- the item's actions ---- */

/* its actions (1 use, 2 equip, 4 examine;
                                                      0x80000000 its note can change) */
extern const char D_00464238[];                    /* the cursor */
extern const PTMF SubScreen_StateWordPlate_ptmf, SubScreen_StateItemQuestion_ptmf2, SubScreen_StateWordPlate_ptmf2, SubScreen_StateItems_ptmf, SubScreen_StateWordPlate_ptmf3, SubScreen_StateItems_ptmf2, SubScreen_StateItems_ptmf3,
    SubScreen_StateItems_ptmf4, SubScreen_StateEquipAsk_ptmf, SubScreen_StateThrowAsk_ptmf, SubScreen_StateItems_ptmf5, SubScreen_StateItems_ptmf6;

typedef struct {
    u16 act;   /* the action's bit (0 back) */
    u16 msg;   /* its name */
} ItemAction;
extern const ItemAction D_0044B260[6];   /* back, use, equip, examine, take off */

#define SUB_ACTION(s) ((s)->padA8C54)   /* the action under the cursor */

/* the item under the cursor: its id (and kind, `kind` given), its actions (`list`, back last;
 * their count returned), its equipment status in `equip`, its word copied when it is the word
 * plate (0x3F) and its name given to the questions (parameter 1) */
static s32 item_actions(SubScreen *s, s32 *id, s32 *kind, u32 *acts, s8 *equip, u8 *list) {
    u8 *items = s->pool;
    s32 n = 0, b;

    *id = Items_Id(items, SUB_LIST(s), SUB_CURSOR(s));
    if (kind != NULL) {
        *kind = Items_Kind(items, SUB_LIST(s), SUB_CURSOR(s));
    }
    *acts = Items_Actions(items, SUB_LIST(s), SUB_CURSOR(s));
    *equip = Items_EquipState(items, SUB_LIST(s), SUB_CURSOR(s));
    for (b = 0; b < 3; b++) {
        if (*acts & (1 << b)) {
            list[n++] = 1 << b;
        }
    }
    list[n++] = 0;
    if (*id == 0x3F) {
        const u8 *w = (const u8 *)Items_Field20(items, SUB_LIST(s), SUB_CURSOR(s));
        u32 i;

        for (i = 0; i < 8; i++) {
            s->unkA8C58[i] = w[i];
        }
    }
    Msg_SetParamSystem(&s->ask, 1, *id & 0xFFFF);
    return n;
}

/* the item can't be used now: in a chase (2), or with the stalker here in the room */
static s32 item_usable(void) {
    Progress *p = gProgress;
    u8 *h = (u8 *)gCharSlot2;

    if ((u8)Progress_GameMode(p) == 2) {
        return 0;
    }
    if (h != NULL && AT(h, 0x28, u8) != 0 && AT(h, 0xE0, u8) == 0 &&
        VCALL(p, 0xC, s32 (*)(Progress *))(p) == AT(h, 0x30, s32)) {
        return 0;
    }
    return 1;
}

/* the item under the cursor used (its use's result: 0 / 8 nothing happened, 4 the screen
 * closes onto it, 1 (an item of kind 2) / 2 a message, else done) */
static void item_use(SubScreen *s, s32 id, s32 kind) {
    u8 r;

    if ((u32)kind < 2 && !item_usable()) {
        Task_Open(&s->ask, 0x54);
        Sound_PlaySE(SE_BUZZER);
        ptmf_set(&s->state, &SubScreen_StateWordPlate_ptmf);
        return;
    }
    if (kind == 7) {
        Task_Open(&s->ask, id & 0xFFFF);
        ptmf_set(&s->state, &SubScreen_StateItemQuestion_ptmf2);
        return;
    }
    r = Items_Use(s->pool, SUB_LIST(s), SUB_CURSOR(s));
    if (r == 0 || r == 8) {
        Task_Open(&s->ask, 0x52);
        if (!(r & 8)) {
            Sound_PlaySE(SE_BUZZER);
        }
        ptmf_set(&s->state, &SubScreen_StateWordPlate_ptmf2);
    } else if (r & 4) {
        VCALL(s, 0x38, void (*)(SubScreen *, s32))(s, id);
        ptmf_set(&s->state, &SubScreen_StateItems_ptmf);
        if (!(r & 8)) {
            Sound_PlaySE(SE_DECIDE);
        }
        s->close = 1;
        s->quietClose = 1;
    } else if (((r & 1) && kind == 2) || (r & 2)) {
        VCALL(s, 0x38, void (*)(SubScreen *, s32))(s, id);
        Task_Open(&s->ask, 0x55);
        if (!(r & 8)) {
            Sound_PlaySE(0x84);
        }
        ptmf_set(&s->state, &SubScreen_StateWordPlate_ptmf3);
    } else {
        ptmf_set(&s->state, &SubScreen_StateItems_ptmf2);
    }
}

/* state: the item's actions. Up / down round them; confirm: back (SubScreen_StateItems_ptmf5), examine (a
 * question, SubScreen_StateThrowAsk_ptmf), equip / take off (Fiona's costume changing; equipping where another
 * is: a question naming it, SubScreen_StateEquipAsk_ptmf) or use (item_use); next / previous step to the list's
 * next / previous item (its picture loaded); cancel puts the cursor on back (SubScreen_StateItems_ptmf6).
 * Then the list's panels and grid and, staying here: the picture (sliding in), the name,
 * count and note, and the actions' box with the cursor */
/* 0x00395630 */
void SubScreen_StateItemActions(SubScreen *s) {
    u8 *items = s->pool;
    Task *t = &s->text;
    u8 list[4];
    s32 stay = 1, redo = 0;
    s32 id, kind, n, i;
    u32 acts;
    s8 equip;

    n = item_actions(s, &id, &kind, &acts, &equip, list);
    if (!s->fading) {
        u32 pad = gMenuPressed;

        if (pad & MENU_CONFIRM) {
            switch (list[SUB_ACTION(s)]) {
            case 0:
                ptmf_set(&s->state, &SubScreen_StateItems_ptmf5);
                Sound_PlaySE(SE_DECIDE);
                break;
            case 4:
                Task_Open(&s->ask, 0x56);
                Sound_PlaySE(SE_DECIDE);
                ptmf_set(&s->state, &SubScreen_StateThrowAsk_ptmf);
                break;
            case 2:
                if (equip == 2) {
                    s32 on = Items_Equipped(items, (u8)Items_KindSlot(items, SUB_LIST(s), SUB_CURSOR(s)));

                    Msg_SetParamSystem(&s->ask, 1, on & 0xFFFF);
                    Task_Open(&s->ask, 0x5C);
                    Sound_PlaySE(SE_DECIDE);
                    ptmf_set(&s->state, &SubScreen_StateEquipAsk_ptmf);
                } else if (equip == 1) {
                    Items_Unequip(items, SUB_LIST(s), SUB_CURSOR(s));
                    if (SUB_LIST(s) == 1) {
                        Fiona_ShowEquipment((Fiona *)gCharPlayer);
                    }
                    Sound_PlaySE(SE_DECIDE);
                    ptmf_set(&s->state, &SubScreen_StateItems_ptmf4);
                } else if (equip == 0) {
                    Items_Equip(items, SUB_LIST(s), SUB_CURSOR(s));
                    if (SUB_LIST(s) == 1) {
                        Fiona_ShowEquipment((Fiona *)gCharPlayer);
                    }
                    Sound_PlaySE(SE_DECIDE);
                    ptmf_set(&s->state, &SubScreen_StateItems_ptmf3);
                }
                break;
            case 1:
                item_use(s, id, kind);
                break;
            }
            stay = 0;
        } else if (pad & MENU_UP) {
            if (SUB_ACTION(s) != 0) {
                SUB_ACTION(s)--;
            } else {
                SUB_ACTION(s) = n - 1;
            }
            Sound_PlaySE(SE_CURSOR);
        } else if (pad & MENU_DOWN) {
            SUB_ACTION(s) = (SUB_ACTION(s) + 1) % n;
            Sound_PlaySE(SE_CURSOR);
        } else if (gMenuPressed & (MENU_NEXT | MENU_PREV)) {
            u8 old;

            redo = 1;
            SUB_ACTION(s) = 0;
            old = SUB_CURSOR(s);
            if (gMenuPressed & MENU_NEXT) {
                SUB_CURSOR(s) = old + 1;
                if (SUB_CURSOR(s) >= Items_FirstFree(items, SUB_LIST(s))) {
                    SUB_CURSOR(s) = 0;
                }
            } else if (old != 0) {
                SUB_CURSOR(s)--;
            } else {
                SUB_CURSOR(s) = Items_FirstFree(items, SUB_LIST(s)) - 1;
            }
            if (old != SUB_CURSOR(s)) {
                s->page[0x15C] = 0;
                Loader_FreePictureArea();
                Items_StartUse(items, SUB_LIST(s), SUB_CURSOR(s), (u8 *)s + 0x94F40);
                Sound_PlaySE(SE_CURSOR);
            }
        } else if (gMenuPressed & MENU_CANCEL) {
            SUB_ACTION(s) = n - 1;
            ptmf_set(&s->state, &SubScreen_StateItems_ptmf6);
            Sound_PlaySE(SE_CANCEL);
        }
    }
    if (redo) {
        n = item_actions(s, &id, NULL, &acts, &equip, list);
    }
    if (SUB_LIST(s) == 0) {
        s->kind = 0;
    } else {
        s->kind = SUB_LIST(s) == 1 ? 8 : 9;
    }
    sub_panels(s);
    SubScreen_DrawItemGrid(s, 1);
    if (stay) {
        char word[9];
        s32 note;

        SubScreen_DrawPart(s, 0x10, 0x30, 0xA, 0x80, 0);
        if (SubScreen_LoaderIdle(items) == 0) {
            VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, (u8 *)s + 0x94F40, 0x27);
            if (s->page[0x15C] < 0x40) {
                s->page[0x15C] += 8;
            }
            SubScreen_DrawPart(s, 0x50, 0x70, 0xB, s->page[0x15C], 0);
        }
        for (i = 0; i < 8; i++) {
            word[i] = s->unkA8C58[i];
        }
        word[8] = 0;
        Msg_PrintfParam(t, 3, kFmtString, word);
        Task_ShowText(t, 0x38, 0x49, 0x80, Task_MessageText(t, (u16)(id + 0x8100)), 0x80, 0x30, 0x10, 0x15);
        if (Items_IsCounted(items, SUB_LIST(s), SUB_CURSOR(s)) == 1) {
            Task_Printf(t, 0xD2, 0x49, 0x80, str_xN_3, Items_HowMany(items, SUB_LIST(s), SUB_CURSOR(s)));
        }
        Task_DrawBox(t, 0x100, 0x16E, 0x19C, 0x66, 0x60, 0x30);
        note = id + 0x100;
        if ((acts & 0x80000000) && !VCALL(s, 0x3C, s32 (*)(SubScreen *, s32))(s, id)) {
            note = 0x13D;
        }
        Task_ShowText(t, 0x32, 0x142, 0, Task_MessageText(t, note & 0xFFFF), 0x80, 0x30, 0x10, 0x15);
        Task_DrawBox(t, 0x1A9, n * 10 + 0xDD, 0x46, n * 20, 0x60, 0x30);
        for (i = 0; i < n; i++) {
            s32 k;

            for (k = 0; k < 5 && D_0044B260[k].act != list[i]; k++) {
            }
            if (k == 2 && equip == 1) {
                k = 4;
            }
            Task_ShowText(t, 0x190, 0xD8 + i * 0x14, 0, Task_MessageText(t, D_0044B260[k].msg), 0x80, 0x30, 0x10,
                          0x15);
        }
        Task_Printf(t, 0x17C, SUB_ACTION(s) * 20 + 0xD8, 9, D_00464238);
    }
}
