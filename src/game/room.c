/* Mesh drawing (the room's parts, vtable RoomMeshes_vtable; placed objects' models) with OpenGL. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "hewie.h"
#include "room.h"
#include "vecmath.h"
#include "renderer.h"
#include "progress.h"
#include "actor.h"
#include "navmesh.h"
#include "memcard.h"
#include "doors.h"
#include "scene_game.h"
#include "sound.h"
#include "msl.h"
#include "input.h"
#include "ptmf.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "system.h"
#include "text.h"
#include "libc.h"
#include "sce/iop.h"
#include "pursuer.h"
#include "effectmgr.h"
#include "charaction.h"
#include "gl2d.h"
#include "music.h"
#include "camera.h"
#include "heap.h"
#include "char_load.h"
#include "creature.h"
#include "event.h"
#include "gameover.h"
#include "items.h"
#include "model.h"
#include "movie.h"
#include "fiona.h"
#include "pause.h"
#include "placed.h"
#include "room_map.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "effects.h"
#include "cri/adx.h"
#include "subscreen.h"
#include "director.h"
#include "lights.h"
#include "sce/intc.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

#ifdef HG_NATIVE
/* the PC renderer (native/platform/glr.h): batches are drawn with OpenGL instead of the VU1 */
static f32 sGlMvp[4][4];   /* the current batch's local-to-clip matrix */
static const void *sGlTex; /* its texture's .TEX entry (NULL: untextured) */
static u64 sGlTex0;
static u32 sGlPrim;
static u32 sGlKindPrim;    /* the kind-4 part's blending (RoomMesh_Billboard): GLR_PRIM_ADD | NOZW */
#define GLR_PRIM_ADD 0x10000u
#define GLR_PRIM_NOZW 0x20000u
#endif

void RoomMesh_Billboard(u8 *o);                 /* a kind-4 part's batch */
extern void RoomMesh_ViewEdge(u8 *o, s32 which);
void RoomMesh_BatchOutOfView(u8 *o);
extern void RoomMesh_PlaceBatch(u8 *o);
s32 *RoomMesh_WriteBatchLit(u8 *o, s32 *batch);   /* the lit layout's batch writer: the next batch */
extern s32 *RoomMesh_WriteBatch0(u8 *o, s32 *batch);

extern void *PlacedMesh_vtable[], *Helper469D00_vtable[];
void *PlacedMesh_dtor(u8 *o, s32 flags);

extern void *PlacedModelBase_vtable[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

void PlacedObjects_SetPair(void *p, s32 a, s32 b);
void *PlacedModelBase_dtor(u8 *o, s32 flags);
s32 PlacedObjects_Sum(u8 *self);
s32 PlacedObjects_CallDtor(void *self);

extern void *RoomMeshes_vtable[];
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *RoomMeshes_dtor(u8 *o, s32 flags);
void *RoomMeshes_ctor(u8 *p);

extern void *PlacedObjects_vtable[];
extern void *gRoomObjects;
extern u8 DoorShadow_vtable[];
void *PlacedModelHolder_ctor(u8 *p);
extern void NavMesh_DropPending(u8 *p);
extern void Doors_ReleaseRequests(u8 *p);
extern void Pair_Clear(u8 *p);
extern void Obstacle_ElemCtor(void *p);
extern const PTMF sGameStateNull;
extern void Doors_Reset(u8 *p);
extern char str_ST_N_ST_N_PAC[];   /* "ST_%03X\\ST_%03X.PAC" */
extern u8 gLanguage;
extern u8 *Ending_Text;   /* the current room's section 10 */
extern void Doors_TakeRoom(u8 *d, u8 *sec);
extern s32 RoomMgr_TakeSection14(u8 *o, u8 *sec);
extern void RoomMgr_TakeAreas(u8 *rm, u8 *sec);
extern void RoomMgr_ResetAreas(u8 *rm);
extern void RoomEffects_TakeRoom(u8 *o, u8 *sec);
extern void PlacedObject_Clear(u8 *o);
extern void PlacedObject_Init0(u8 *o, u8 *def);
extern void PlacedObject_Init1(u8 *o, u8 *def);
extern void PlacedObject_Init2(u8 *o, u8 *def);
extern void PlacedObject_Init3(u8 *o, u8 *def);
extern void Doors_Draw(void *doors);
extern void Doors_Update(void *doors);
extern void RoomEffects_ReleaseAll(u8 *fx);           /* all the room's effects back to the pool */
extern void PlacedObject_Animate(u8 *obj);
extern void PlacedObject_Draw(u8 *obj, s32 layer);
extern void RoomEffects_ReleaseAll(u8 *fx);
extern void RoomEffects_TakeRoom(u8 *o, u8 *sec);
static inline void clear_if_set(u8 *p, u32 off) {
    if (AT(p, off, s32) != 0) {
        AT(p, off, s32) = 0;
    }
}

#define ROOM_SLOT_DONE 0x80000000   /* +0x3C0[slot]: the slot's load has been handled */

/* A room PAC starts with 17 section offsets (0: none); the room manager keeps pointers to
 * them at +0x9980.. */
#define ROOM_SEC(rm, at) AT(rm, at, u8 *)

static u8 *room_section(u8 *pac, s32 i) {
    u32 off = AT(pac, i * 4, u32);

    return off != 0 ? pac + off : NULL;
}

static void placed_init(u8 *o, s32 kind, u8 *def) {
    AT(o, 0x4, s32) = kind;
    AT(o, 0x48, s32) = AT(o, 0x4, s32);
    AT(o, 0x70, u8 *) = def;
    sceVu0CopyVector((f32 *)(o + 0x10), (f32 *)(def + 0x10));
    sceVu0CopyVector((f32 *)(o + 0x20), (f32 *)(def + 0x20));
}

void *PlacedModelHolder_ctor(u8 *p);
void *PlacedMeshHolder_ctor(u8 *p);
void PlacedObject_Clear(u8 *e);
void PlacedObjects_ClearAll(u8 *p);
s32 RoomMgr_TakeSection14(u8 *o, u8 *sec);
void RoomMgr_TakeAreas(u8 *rm, u8 *sec);
void RoomMgr_ResetAreas(u8 *rm);
void PlacedObject_Release(u8 *obj);
void PlacedObjects_ReleaseAll(u8 *o);
u8 *PlacedObjects_RecordEntry(u8 *o, const char *name, u32 k);
void PlacedObjects_Noop34(u8 *o);
u8 *PlacedObjects_Named(u8 *o, const char *name);
void PlacedObjects_Create(u8 *o);
void PlacedObject_Init0(u8 *o, u8 *def);
void PlacedObject_Init1(u8 *o, u8 *def);
void PlacedObject_Init2(u8 *o, u8 *def);
void PlacedObject_Init3(u8 *o, u8 *def);
void PlacedObjects_Update(u8 *o);
void PlacedObject_Animate(u8 *obj);
void PlacedObjects_Draw(u8 *o);
void PlacedObject_Draw(u8 *obj, s32 layer);

/* draw the room, each frame: the room mesh's parts (PAC section header +0x998C: offsets of
 * the opaque part, two alpha parts drawn in layer 8 under the 0x1D effect (fog) else 0x19,
 * and the parts for layers 0x26 and 0x1F) are queued with the renderer (+0xC) as the room
 * object (+0x8 mesh, +0x18 part); the lights are reset (+0x18) before each pass. Then the
 * extra object +0x340 (flag 0x80), the doors, +0x9380 and +0x6740. */
/* 0x0011FB20 */
void RoomMgr_Draw(u8 *rm, s32 slot) {
    VObject *lights;
    void *fx;
    s32 o0, o1, o2, o4, o5;
    s32 layer;

    if (AT(rm, 0x998C, u8 *) == NULL) {
        return;
    }
    lights = gTexCache;
    VCALL(lights, 0x18, void (*)(VObject *))(lights);
    AT(rm, 0x64, s32) = -1;
    o0 = AT(AT(rm, 0x998C, u8 *), 0x0, s32);
    o2 = AT(AT(rm, 0x998C, u8 *), 0x8, s32);
    o4 = AT(AT(rm, 0x998C, u8 *), 0x10, s32);
    o5 = AT(AT(rm, 0x998C, u8 *), 0x14, s32);
    o1 = AT(AT(rm, 0x998C, u8 *), 0x4, s32);
    if (o0 != 0) {
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o0;
        AT(rm, 0x18, s32) = 0;
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, rm, 1, 0);
    }
    VCALL(lights, 0x18, void (*)(VObject *))(lights);
    AT(rm, 0x64, s32) = -1;
    fx = gRoomEffects != NULL ? RoomEffects_Get(gRoomEffects, 0x1D) : NULL;
    layer = (fx != NULL && AT(fx, 0x1C, s32) != 0) ? 8 : 0x19;
    if (o1 != 0) {
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o1;
        AT(rm, 0x18, s32) = 1;
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, rm, layer, 0);
    }
    if (o4 != 0) {
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o4;
        AT(rm, 0x18, s32) = 4;
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, rm, layer, 0);
        AT(rm, 0x8B, u8) = 1;
    }
    if (o2 != 0) {
        VCALL(lights, 0x18, void (*)(VObject *))(lights);
        AT(rm, 0x64, s32) = -1;
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o2;
        AT(rm, 0x18, s32) = 2;
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, rm, 0x26, 0);
    }
    if (o5 != 0) {
        VCALL(lights, 0x18, void (*)(VObject *))(lights);
        AT(rm, 0x64, s32) = -1;
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o5;
        AT(rm, 0x18, s32) = 5;
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, rm, 0x1F, 0);
    }
    VCALL(lights, 0x18, void (*)(VObject *))(lights);
    AT(rm, 0x358, s32) = -1;
    if (AT(rm, 0x35C, u8) & 0x80) {
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, rm + 0x340, 1, 0);
    }
    Doors_Draw(rm + 0x1640);
    Obstacles_ModelsFollow(rm + 0x9380);
    VCALL(rm + 0x6740, 0x24, void (*)(void *))(rm + 0x6740);
}
/* make room slot `slot` the current room: find its sections and hand them to the model
 * set (+0x3E0), the camera, lights, collision (+0x1640), texture cache, ... */
/* 0x0011FFB0 */
void RoomMgr_MakeCurrent(u8 *rm, s32 slot) {
    u8 *pac = rm + slot * 0x2A0000 + 0x99C0;
    VObject *o;

    ROOM_SEC(rm, 0x9980) = room_section(pac, 0);
    if (ROOM_SEC(rm, 0x9980) == NULL) {
        ROOM_SEC(rm, 0x9994) = NULL;
    }
    if (ROOM_SEC(rm, 0x9980) != NULL) {
        ROOM_SEC(rm, 0x9984) = room_section(pac, 1);
        ROOM_SEC(rm, 0x99BC) = room_section(pac, 16);
        if (ROOM_SEC(rm, 0x9984) != NULL) {
            NavMeshSet_Take(rm + 0x3E0, ROOM_SEC(rm, 0x9980), ROOM_SEC(rm, 0x9984),
                          ROOM_SEC(rm, 0x99BC));
        }
        o = (VObject *)(rm + 0x3E0);
        VCALL(o, 0x4C, void (*)(VObject *))(o);
        ROOM_SEC(rm, 0x9988) = room_section(pac, 2);
        if (gEvents != NULL) {
            VCALL(gEvents, 0xC, void (*)(VObject *, void *))(gEvents, ROOM_SEC(rm, 0x9988));
        }
        ROOM_SEC(rm, 0x9994) = room_section(pac, 4);
        ROOM_SEC(rm, 0x99A0) = room_section(pac, 7);
        Doors_TakeRoom(rm + 0x1640, ROOM_SEC(rm, 0x99A0));
        ROOM_SEC(rm, 0x99A4) = room_section(pac, 8);
        ROOM_SEC(rm, 0x99B4) = room_section(pac, 14);
        RoomMgr_TakeSection14(rm + 0x9360, ROOM_SEC(rm, 0x99B4));
    }
    ROOM_SEC(rm, 0x9998) = room_section(pac, 5);
    ROOM_SEC(rm, 0x999C) = room_section(pac, 6);
    if (AT(pac, 0xC, u32) == 0) {
        ROOM_SEC(rm, 0x998C) = NULL;
        ROOM_SEC(rm, 0x9990) = NULL;
    } else {
        ROOM_SEC(rm, 0x998C) = pac + AT(pac, 0xC, u32);
        RoomMgr_TakeAreas(rm, ROOM_SEC(rm, 0x998C));
        if (AT(pac, 0x24, u32) != 0) {
            ROOM_SEC(rm, 0x9990) = pac + AT(pac, 0x24, u32);
            VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, ROOM_SEC(rm, 0x9990), 0);
        }
        RoomMgr_ResetAreas(rm);
        AT(rm, 0x35C, u8) = 0;
    }
    VCALL(gLights, 0xC, void (*)(VObject *, void *))(gLights, ROOM_SEC(rm, 0x9994));
    o = (VObject *)(rm + 0x1640);
    VCALL(o, 0x24, void (*)(VObject *, void *))(o, ROOM_SEC(rm, 0x99A4));
    VCALL(gCamera, 0x78, void (*)(VObject *, void *))(gCamera, ROOM_SEC(rm, 0x9998));
    if (AT(pac, 0x28, u32) != 0) {
        ROOM_SEC(rm, 0x99A8) = pac + AT(pac, 0x28, u32);
        gLanguage = 1;
    } else {
        ROOM_SEC(rm, 0x99A8) = NULL;
    }
    Ending_Text = ROOM_SEC(rm, 0x99A8);
    if (AT(pac, 0x2C, u32) != 0) {
        ROOM_SEC(rm, 0x99AC) = pac + AT(pac, 0x2C, u32);
        VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(gTexCache, ROOM_SEC(rm, 0x99AC), 0x15);
    } else {
        ROOM_SEC(rm, 0x99AC) = NULL;
        VCALL(gTexCache, 0x14, void (*)(VObject *, s32))(gTexCache, 0x15);
    }
    ROOM_SEC(rm, 0x99B0) = room_section(pac, 12);
    ROOM_SEC(rm, 0x99B8) = room_section(pac, 15);
    o = (VObject *)(rm + 0x6740);
    VCALL(o, 0x10, void (*)(VObject *, void *, void *))(o, ROOM_SEC(rm, 0x99B0), ROOM_SEC(rm, 0x99B8));
    VCALL(o, 0x14, void (*)(VObject *))(o);
    RoomEffects_TakeRoom((u8 *)gRoomEffects, room_section(pac, 13));
    Obstacles_Reset(rm + 0x9380);
    VCALL(gRenderer, 0x1C, void (*)(VObject *))(gRenderer);
}
/* 0x00120530 */
void PlacedObjects_SetPair(void *p, s32 a, s32 b) {
    FLD(p, 0x4, s32) = a;
    FLD(p, 0x8, s32) = b;
}

#ifdef HG_NATIVE
/* vtable +0xC of a mesh (the room's parts, +0x8; part kind +0x18), drawn with OpenGL. Per batch
 * (until -1): its texture (+0x80, TEX0 looked up only when it changed), colour bits
 * (+0x84..+0x8A) and local matrix (stored transposed at +0x90); the camera's clip matrix times
 * it and the batch's state (texture, PRIM bits) go to the vertex writer (by +0x4: RoomMesh_WriteBatch0,
 * or the lit layout RoomMesh_WriteBatchLit). Kind 4 parts blend as their flip book says
 * (RoomMesh_Billboard); kind 5 parts draw without depth writes, each batch normal or additive by
 * its header's bit 8. 0 without a mesh. */
/* 0x0025E2B0 */
s32 RoomMesh_Draw(u8 *o) {
    VObject *cam;
    s32 *mesh;
    f32 b[4][4] __attribute__((aligned(16)));

    AT(o, 0x4, s32) = AT(o, 0x68, s32);
    AT(o, 0xD4, s32) = 0;
    RoomMesh_ViewEdge(o, 0);
    RoomMesh_ViewEdge(o, 1);
    mesh = AT(o, 0x8, s32 *);
    if (mesh == NULL) {
        return 0;
    }
    if (mesh[0] != -1) {
        cam = gCamera;
        do {
            s32 newTex = 0;
            u32 w, kind5 = 0;
            s32 i;

            AT(o, 0x7C, s32) = mesh[0];
            AT(o, 0x80, s32) = mesh[1];
            w = mesh[2];
            AT(o, 0x84, u8) = w;
            AT(o, 0x85, u8) = (s32)(w & 0xFF00) >> 8;
            AT(o, 0x86, u16) = 1 << (AT(o, 0x85, u8) & 0xF);
            AT(o, 0x88, u16) = 1 << ((AT(o, 0x85, u8) & 0xF0) >> 4);
            AT(o, 0x8A, u8) = (w & 0xFF000000) >> 24;
            if (AT(o, 0x18, s32) == 5) {
                kind5 = GLR_PRIM_NOZW | ((mesh[3] >> 8) & 1 ? GLR_PRIM_ADD : 0);
            }
            mesh += 4;
            if (AT(o, 0x80, s32) != -1) {
                newTex = 1;
                if (AT(o, 0x80, s32) != AT(o, 0x64, s32)) {
                    AT(o, 0x10, u64) = TexCache_Tex0(AT(o, 0x80, s32));
                }
            }
            for (i = 0; i < 4; i++) {
                AT(o, 0x90 + i * 4, f32) = AT(mesh, 0x0, f32);
                AT(o, 0xA0 + i * 4, f32) = AT(mesh, 0x4, f32);
                AT(o, 0xB0 + i * 4, f32) = AT(mesh, 0x8, f32);
                AT(o, 0xC0 + i * 4, f32) = AT(mesh, 0xC, f32);
                mesh += 4;
            }
            if (AT(o, 0x60, u8) != 0) {
                RoomMesh_BatchOutOfView(o);
            } else {
                AT(o, 0xD0, u8) = 0xFF;
            }
            if (AT(o, 0x85, u8) != 0) {
                RoomMesh_PlaceBatch(o);
            }
            if (AT(o, 0x18, s32) == 4) {
                RoomMesh_Billboard(o);
            }
            VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, b);
            sceVu0MulMatrix(b, b, (f32 (*)[4])(o + 0x90));
            for (i = 0; i < 4; i++) {
                sGlMvp[i][0] = b[i][0];
                sGlMvp[i][1] = b[i][1];
                sGlMvp[i][2] = b[i][2];
                sGlMvp[i][3] = b[i][3];
            }
            sGlTex0 = newTex ? AT(o, 0x10, u64) : 0;
            sGlTex = newTex ? VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, AT(o, 0x80, s32), 0)
                            : NULL;
            sGlPrim = (newTex << 4) | 0xC | AT(o, 0x84, u8) << 6 | (AT(o, 0x18, s32) == 4 ? sGlKindPrim : kind5);
            if (AT(o, 0x4, s32) == 1) {
                mesh = RoomMesh_WriteBatchLit(o, mesh);
            } else if (AT(o, 0x4, s32) == 0) {
                mesh = RoomMesh_WriteBatch0(o, mesh);
            }
        } while (mesh[0] != -1);
    }
    return 1;
}
#endif

/* is the loader's request for file slot k (+0x3C0, -1 none) done (its state 2)? */
/* 0x00120540 */
s32 RoomMgr_SlotLoaded(u8 *o, s32 k) {
    s32 req = AT(o, 0x3C0 + k * 4, s32);

    if (req == -1) {
        return 0;
    }
    return VCALL(gFileLoader, 0x28, s32 (*)(void *, u32))(gFileLoader, (u32)req & 0x7FFFFFFF) == 2 ? 1 : 0;
}

/* start loading file slot k (+0x3C0) unless it already is (bit 31): the loader +0x14, the slot
 * marked, its callback (+0x3C8, a PTMF each) back to none */
/* 0x001205A0 */
void RoomMgr_LoadSlot(u8 *o, s32 k) {
    u32 *slot = (u32 *)(o + 0x3C0) + k;

    if (*slot & 0x80000000) {
        return;
    }
    VCALL(gFileLoader, 0x14, void (*)(void *, u32))(gFileLoader, *slot);
    *slot |= 0x80000000;
    AT(o, 0x3C8 + k * 12, PTMF) = sGameStateNull;
}

/* the room, each frame: the doors (with a room loaded), +0x9380, +0x6740 (+0x28), and
 * whether Progress +0x54 lets the alpha parts show (+0x8B) */
/* 0x0011FEB0 */
void RoomMgr_Update(u8 *rm) {
    if (AT(rm, 0x998C, u8 *) != NULL) {
        Doors_Update(rm + 0x1640);
    }
    Obstacles_Update(rm + 0x9380);
    VCALL(rm + 0x6740, 0x28, void (*)(void *))(rm + 0x6740);
    AT(rm, 0x8B, u8) = VCALL(gProgress, 0x54, s32 (*)(void *))(gProgress);
}

/* the room left: its doors, obstacles, placed objects (+0x6740 +0x20) and effects released,
 * and the texture cache's room groups (0, 0x15) dropped */
/* 0x0011FF30 */
void RoomMgr_Leave(u8 *rm) {
    VObject *tc;

    Doors_Release((VObject *)(rm + 0x1640));
    Obstacles_Release(rm + 0x9380);
    VCALL((VObject *)(rm + 0x6740), 0x20, void (*)(VObject *))((VObject *)(rm + 0x6740));
    RoomEffects_ReleaseAll((u8 *)gRoomEffects);
    tc = gTexCache;
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0);
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0x15);
}

/* whether room slot `slot` is still loading (its handler +0x3C8[slot] runs once the file is
 * in) */
/* 0x00120660 */
s32 RoomMgr_SlotLoading(u8 *rm, s32 slot) {
    u32 tag = AT(rm, 0x3C0 + slot * 4, u32);
    PTMF *done;

    if (tag & ROOM_SLOT_DONE) {
        return 0;
    }
    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, tag) == 2) {
        return 1;
    }
    done = &AT(rm, 0x3C8 + slot * 12, PTMF);
    if (!ptmf_test(done)) {
        return 0;
    }
    ptmf_scall_1(rm, done, slot);
    return 1;
}

/* load room `room` (ST_xxx\ST_xxx.PAC) into slot `slot` (+0x99C0, 0x2A0000 each); a load
 * still in flight in that slot is first finished (its handler +0x3C8[slot]) or cancelled */
/* 0x00120720 */
void RoomMgr_LoadRoom(u8 *rm, u32 room, s32 slot) {
    char path[0x100];
    char name[0x100];
    u32 *cur;
    PTMF *done;
    s32 busy;
    VObject *loader;

    if (room >= 0x110) {
        AT(rm, 0x3C8 + slot * 12, PTMF) = sGameStateNull;
        return;
    }
    cur = &AT(rm, 0x3C0 + slot * 4, u32);
    busy = 0;
    if (!(*cur & ROOM_SLOT_DONE)) {
        if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, *cur) == 2) {
            busy = 1;
        } else {
            done = &AT(rm, 0x3C8 + slot * 12, PTMF);
            if (ptmf_test(done)) {
                ptmf_scall_1(rm, done, slot);
                busy = 1;
            }
        }
    }
    if (busy && !(*cur & ROOM_SLOT_DONE)) {
        VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, *cur);   /* cancel */
        *cur |= ROOM_SLOT_DONE;
        AT(rm, 0x3C8 + slot * 12, PTMF) = sGameStateNull;
    }
    *cur = room;
    msl_snprintf(name, sizeof(name), str_ST_N_ST_N_PAC, room & ~7, room);
    msl_strcpy(path, name);
    loader = gFileLoader;
    if (VCALL(loader, 0x30, s32 (*)(VObject *, char *))(loader, path) > 0) {
        VCALL(loader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))(
            loader, path, rm + slot * 0x2A0000 + 0x99C0, room, 0);
    }
    AT(rm, 0x3C8 + slot * 12, PTMF) = sGameStateNull;
}

/* the room manager (SceneGame +0x73EE80): drop everything for a new room */
/* 0x00120980 */
void RoomMgr_Clear(u8 *m) {
    static const u16 offs[16] = {
        0x9980, 0x9984, 0x9988, 0x9994, 0x9998, 0x999C, 0x99A0, 0x99A4,
        0x99A8, 0x99AC, 0x99B0, 0x99B4, 0x99B8, 0x99BC, 0x998C, 0x9990,
    };
    VObject *tc;
    s32 i;

    NavMesh_DropPending(m + 0x3E0);
    for (i = 0; i < 16; i++) {
        clear_if_set(m, offs[i]);
    }
    for (i = 0; i < 2; i++) {
        AT(m, 0x3C0 + i * 4, s32) = -1;   /* the room buffers' rooms */
    }
    Doors_ReleaseRequests(m + 0x1640);
    VCALL((VObject *)(m + 0x6740), 0xC, void (*)(VObject *))((VObject *)(m + 0x6740));
    Pair_Clear(m + 0x9360);
    Obstacle_ElemCtor(m + 0x9380);
    tc = gTexCache;
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0);
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0x15);
}

/* the room manager's constructor body (SceneGame +0x73EE80): no room-load state, then reset */
/* 0x00120C80 */
void RoomMgr_ctor(u8 *m) {
    Doors_Reset(m + 0x1640);
    VCALL((VObject *)(m + 0x6740), 0x8, void (*)(VObject *))((VObject *)(m + 0x6740));
    AT(m, 0x3D4, PTMF) = sGameStateNull;
    AT(m, 0x3C8, PTMF) = AT(m, 0x3D4, PTMF);
    RoomMgr_Clear(m);
}

/* destructor (vtable RoomMeshes_vtable) */
/* 0x0025C850 */
void *RoomMeshes_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = RoomMeshes_vtable;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* destructor (vtable PlacedMesh_vtable) */
/* 0x0025E950 */
void *PlacedMesh_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = PlacedMesh_vtable;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* the view's side edge `which` (0 left, 1 right): the camera's direction (+0xA0) turned about
 * its up axis (+0xA4) by fov / 1.3, at the look-at distance from the eye -> +0x20 + 16 * which;
 * then that point swung back about y -> +0x40 + 16 * which (the edge planes for culling) */
/* 0x0025DB10 */
void RoomMesh_ViewEdge(u8 *o, s32 which) {
    static const union { u32 u; f32 f; } k13 = {0x3FA66666};
    VObject *cam = gCamera;
    f32 eye[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 e[4] __attribute__((aligned(16)));
    f32 len, ang;

    q[3] = q[2] = q[1] = q[0] = 0.0f;
    VCALL(cam, 0x24, void (*)(VObject *, f32 *))(cam, eye);
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, at);
    VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, dir);
    VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, up);
    sceVu0SubVector(t, at, eye);
    len = __builtin_sqrtf(sceVu0InnerProduct(t, t));
    ang = VCALL(cam, 0x64, f32 (*)(VObject *))(cam) / k13.f;
    Quat_FromAxisAngle(q, up, which == 0 ? ang : -ang);
    m[3][3] = 1.0f;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;   /* unset in the original (garbage times w); no NaN / infinity on PC */
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    Quat_ToMatrix(q, m);
    sceVu0ApplyMatrix(t, m, dir);
    sceVu0ScaleVector(t, t, len);
    sceVu0AddVector(p, eye, t);
    AT(o, 0x20 + (which & 0xFF) * 16, f32) = p[0];
    AT(o, 0x24 + (which & 0xFF) * 16, f32) = p[1];
    AT(o, 0x28 + (which & 0xFF) * 16, f32) = p[2];
    AT(o, 0x2C + (which & 0xFF) * 16, f32) = p[3];
    sceVu0UnitMatrix(r);
    sceVu0SubVector(t, eye, p);
    sceVu0InnerProduct(t, t);
    sceVu0RotMatrixY(r, r, which == 0 ? -ang : ang);
    sceVu0ApplyMatrix(t, r, t);
    sceVu0AddVector(e, p, t);
    AT(o, 0x40 + (which & 0xFF) * 16, f32) = e[0];
    AT(o, 0x44 + (which & 0xFF) * 16, f32) = e[1];
    AT(o, 0x48 + (which & 0xFF) * 16, f32) = e[2];
    AT(o, 0x4C + (which & 0xFF) * 16, f32) = e[3];
}

s32 RoomMesh_FlipBook(u8 *o, const f32 *st);   /* a kind-4 batch's animated texture coordinates */
s32 func_002B7500(u8 *batch);   /* send a batch's vertices to VU1 */

/* mode 0 batch writer: the batch's vertex count (+0x7C; count & 3 gives the padding) locates
 * its sections - texture coordinates (s, t), colours (RGBA bytes), positions (x, y, z, w) -
 * and it is sent unless hidden (+0xD0 not 0xFF, or its group +0x8A is off in the mask +0x6C;
 * kind 4 parts take their texture coordinates from RoomMesh_FlipBook). Returns the next batch. */
/* 0x0025E100 */
s32 *RoomMesh_WriteBatch0(u8 *o, s32 *batch) {
    struct {
        s32 n;
        s32 *batch;
        f32 *xyz;   /* x, y, z, w per vertex */
        void *st;   /* texture coordinates (s, t floats) */
        u8 *rgba;   /* colours (0x80 = 1.0) */
    } a;
    s32 n = AT(o, 0x7C, s32);
    u32 fmt = n & 3;
    u8 *rgba = (u8 *)batch + n * 8;
    u8 *xyz;
    s32 ok = 1;
    s32 show;

    if (fmt == 3 || fmt == 1) {
        rgba += 8;
    }
    xyz = rgba + n * 4;
    switch (fmt) {
    case 3:
        xyz += 4;
        break;
    case 2:
        xyz += 8;
        break;
    case 1:
        xyz += 0xC;
        break;
    }
    a.n = 0;
    a.xyz = NULL;
    a.st = NULL;
    a.rgba = NULL;
    a.rgba = rgba;
    a.n = AT(o, 0x7C, s32);
    a.xyz = (f32 *)xyz;
    a.batch = batch;
    if (AT(o, 0x18, s32) == 4) {
        ok = RoomMesh_FlipBook(o, (const f32 *)batch);
        a.st = o + 0x240 + (AT(o, 0xD4, s32) - 1) * 32;
    } else {
        a.st = batch;
    }
    if (AT(o, 0xD0, u8) == 0xFF) {
        u8 g = AT(o, 0x8A, u8);

        show = g == 0 || (AT(o, 0x6C + (g >> 5) * 4, u32) & (1u << (g & 0x1F)));
        if (show && ok == 1) {
#ifdef HG_NATIVE
            glr_strip(&sGlMvp[0][0], a.n, a.xyz, (f32 *)a.st, a.rgba, sGlTex, sGlTex0, sGlPrim);
#else
            func_002B7500((u8 *)&a);
#endif
        }
    }
    AT(o, 0x64, s32) = AT(o, 0x80, s32);
    return (s32 *)(xyz + AT(o, 0x7C, s32) * 16);
}

#ifdef HG_NATIVE
#include <stdlib.h>

/* mode 1 (the lit layout) batch writer, with OpenGL: the vertex count (+0x7C; count & 3 the
 * padding) locates its sections - texture coordinates (s, t, q floats), colours (two words a
 * vertex, not decoded yet: drawn grey), positions (x, y, z, w with the GS flags) - drawn unless
 * hidden as in mode 0. No surveyed room uses it. Returns the next batch. */
/* 0x0025DD80 */
s32 *RoomMesh_WriteBatchLit(u8 *o, s32 *batch) {
    static f32 *st;
    static u8 *rgba;
    static s32 cap;
    s32 n = AT(o, 0x7C, s32), i;
    u8 *stq = (u8 *)batch;
    u8 *col = stq + n * 12 + (n & 3) * 4;
    u8 *xyz = col + n * 8 + ((n & 3) == 1 || (n & 3) == 3 ? 8 : 0);

    if (n <= 0) {
        AT(o, 0x64, s32) = AT(o, 0x80, s32);
        return batch;
    }
    glr_todo("room mesh: the lit layout's colours (mode 1)");
    if (AT(o, 0xD0, u8) == 0xFF) {
        u8 g = AT(o, 0x8A, u8);

        if ((g == 0 || (AT(o, 0x6C + (g >> 5) * 4, u32) & (1u << (g & 0x1F)))) && n > 0) {
            if (n > cap) {
                cap = n;
                st = realloc(st, cap * 8);
                rgba = realloc(rgba, cap * 4);
            }
            for (i = 0; i < n; i++) {
                st[i * 2] = AT(stq, i * 12, f32);
                st[i * 2 + 1] = AT(stq, i * 12 + 4, f32);
                AT(rgba, i * 4, u32) = 0x80808080;
            }
            glr_strip(&sGlMvp[0][0], n, (const f32 *)xyz, st, rgba, sGlTex, sGlTex0, sGlPrim);
        }
    }
    AT(o, 0x64, s32) = AT(o, 0x80, s32);
    return (s32 *)(xyz + n * 16);
}
#endif

/* the room manager: reset the area state */
/* 0x0025E0D0 */
void RoomMgr_ResetAreas(u8 *rm) {
    AT(rm, 0x6C, s32) = 0;
    AT(rm, 0x70, s32) = 0;
    AT(rm, 0x74, s32) = 0;
    AT(rm, 0x78, s32) = 0;
    AT(rm, 0x4, s32) = 0;
    AT(rm, 0x68, s32) = 0;
    AT(rm, 0x60, u8) = 0;
    AT(rm, 0x8B, u8) = 0;
}

/* a quadword copy done inline (lq / sq), not through libvu0 */
static inline void vec_set(f32 *d, const f32 *s) {
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

/* face batch matrix `m` (+0x90, rows x / y / z / translation) towards the eye: z towards it
 * (upright: in the floor plane, y stays up; else y from the camera's up) */
static void mesh_face_eye(u8 *o, VObject *cam, s32 upright) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 z[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 y[4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(m);
    VCALL(cam, 0x24, void (*)(VObject *, f32 *))(cam, eye);
    if (!upright) {
        VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, m[1]);
    }
    t[0] = AT(o, 0xC0, f32);
    t[1] = AT(o, 0xC4, f32);
    t[2] = AT(o, 0xC8, f32);
    t[3] = AT(o, 0xCC, f32);
    sceVu0SubVector(v, eye, t);
    if (upright) {
        v[1] = 0.0f;
    }
    sceVu0Normalize(v, v);
    vec_set(z, v);
    sceVu0CopyVector(m[2], z);
    if (upright) {
        sceVu0OuterProduct(v, m[1], z);
    } else {
        sceVu0OuterProduct(v, z, m[1]);
    }
    sceVu0Normalize(v, v);
    vec_set(x, v);
    sceVu0CopyVector(m[0], x);
    sceVu0OuterProduct(v, z, x);
    sceVu0Normalize(v, v);
    vec_set(y, v);
    sceVu0CopyVector(m[1], y);
    sceVu0CopyVector(m[3], (f32 *)(o + 0xC0));
    sceVu0CopyMatrix((f32 (*)[4])(o + 0x90), m);
}

/* the side of line a -> b that point p (x, z) is on (twice the signed area) */
static inline f32 side_of(f32 ax, f32 az, f32 bx, f32 bz, f32 px, f32 pz) {
    return ax * (bz - pz) + px * (az - bz) + bx * (pz - az);
}

/* a batch out of view: its place (+0xC0 / +0xC8) against the view's edges (RoomMesh_ViewEdge: the
 * side points +0x20 / +0x30, swung back +0x40 / +0x50) seen from the eye - outside both side
 * edges, or outside all three of the edges behind; +0xD0 0xFF hides it (else 0) */
/* 0x0025D970 */
void RoomMesh_BatchOutOfView(u8 *o) {
    f32 eye[4] __attribute__((aligned(16)));
    f32 px = AT(o, 0xC0, f32), pz = AT(o, 0xC8, f32);
    f32 ax = AT(o, 0x20, f32), az = AT(o, 0x28, f32), bx = AT(o, 0x30, f32), bz = AT(o, 0x38, f32);
    f32 cx = AT(o, 0x40, f32), cz = AT(o, 0x48, f32), dx = AT(o, 0x50, f32), dz = AT(o, 0x58, f32);
    u8 sides = 0, back = 0;

    VCALL(gCamera, 0x24, void (*)(VObject *, f32 *))(gCamera, eye);
    if (!(side_of(eye[0], eye[2], ax, az, px, pz) <= 0.0f)) {
        sides++;
    }
    if (side_of(eye[0], eye[2], bx, bz, px, pz) < 0.0f) {
        sides++;
    }
    if (!(side_of(cx, cz, ax, az, px, pz) <= 0.0f)) {
        back++;
    }
    if (side_of(dx, dz, bx, bz, px, pz) < 0.0f) {
        back++;
    }
    if (side_of(cx, cz, dx, dz, px, pz) < 0.0f) {
        back++;
    }
    AT(o, 0xD0, u8) = 0;
    if (back == 3) {
        AT(o, 0xD0, u8) = 0xFF;
    }
    if (sides == 2) {
        AT(o, 0xD0, u8) = 0xFF;
    }
}

/* a batch's view-dependent placement: parallax (+0x86 = 2 / 4 / 8 / 16: moved sideways by
 * 0.2 / 0.25 / 0.33 / 0.5 of the camera's offset +0x20) and billboards (+0x88 = 2 upright, 4
 * facing the eye) */
/* 0x0025D560 */
void RoomMesh_PlaceBatch(u8 *o) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD}, k033 = {0x3EA8F5C3};
    VObject *cam = gCamera;
    f32 ofs[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 k;

    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, ofs);
    VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, dir);
    VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, up);
    sceVu0OuterProduct(side, dir, up);
    sceVu0Normalize(side, side);
    switch (AT(o, 0x86, u16)) {
    case 2:
        k = k02.f;
        break;
    case 4:
        k = 0.25f;
        break;
    case 8:
        k = k033.f;
        break;
    case 16:
        k = 0.5f;
        break;
    default:
        k = 0.0f;
        break;
    }
    if (k != 0.0f) {
        AT(o, 0xC0, f32) = AT(o, 0xC0, f32) + (k * ofs[0]) * side[0];
        AT(o, 0xC8, f32) = AT(o, 0xC8, f32) + (k * ofs[2]) * side[2];
    }
    if (AT(o, 0x88, u16) == 2) {
        mesh_face_eye(o, cam, 1);
    } else if (AT(o, 0x88, u16) == 4) {
        mesh_face_eye(o, gCamera, 0);
    }
}

extern void func_00267D60(u8 *o);
extern void func_00267AB0(u8 *o);
extern void func_002677C0(u8 *o);
extern void func_00267560(u8 *o);

#ifdef HG_NATIVE
#include <stdlib.h>

/* ---- PC: placed objects' models with OpenGL (what func_00267560 .. func_00267D60 send) ----
 *
 * The model (+0x30) holds one part: +0x40 vertex count, +0x44 texture id (-1 none), streams at
 * the offsets +0x4C (texture coordinates), +0x50 (normals / colours), +0x54 (positions), +0x58
 * (strip flags) from the model, +0x60 the positions' start. The object's flags +0x40 bit 0
 * blend it. Its matrix: scaled, turned by +0x20 / +0x24 / +0x28 (X, Y, Z), at +0x10. */

/* the object's local-to-clip matrix (the camera's clip matrix times its placement) */
static void obj_mvp(u8 *o, f32 scale, f32 (*mvp)[4]) {
    f32 world[4][4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(world);
    world[0][0] = world[1][1] = world[2][2] = scale;
    sceVu0RotMatrixX(world, world, AT(o, 0x20, f32));
    sceVu0RotMatrixY(world, world, AT(o, 0x24, f32));
    sceVu0RotMatrixZ(world, world, AT(o, 0x28, f32));
    sceVu0TransMatrix(world, world, (f32 *)(o + 0x10));
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, mvp);
    sceVu0MulMatrix(mvp, mvp, world);
}

static void obj_strip(u8 *o, f32 (*mvp)[4], s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba) {
    u8 *model = AT(o, 0x30, u8 *);
    s32 tex = AT(model, 0x44, s32);

    glr_strip(&mvp[0][0], n, xyzw, st, rgba,
              tex == -1 ? NULL : VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, tex, 0),
              tex == -1 ? 0 : TexCache_Tex0(tex), 0xC | (tex != -1 ? 0x10 : 0) | (AT(o, 0x40, u8) & 1 ? 0x40 : 0));
}

/* a rigid part (func_00267D60): compressed streams, model scale 32 - s16 position deltas from
 * the s32 start, / 4096; u16 texture coordinates / 32768; u8 strip flags (1: no triangle).
 * The original lights it on VU1 (ambient 128); drawn unlit at 1.0 for now. */
static void gl_rigid(u8 *o) {
    static f32 *xyzw, *st;
    static u8 *rgba;
    static s32 cap;
    f32 mvp[4][4] __attribute__((aligned(16)));
    u8 *model = AT(o, 0x30, u8 *);
    s32 n = AT(model, 0x40, s32), i;
    const s32 *start = (const s32 *)(model + 0x60);
    const s16 *d = (const s16 *)(model + AT(model, 0x54, s32));
    const u16 *uv = (const u16 *)(model + AT(model, 0x4C, s32));
    const u8 *strip = model + AT(model, 0x58, s32);
    s32 x = start[0], y = start[1], z = start[2];

    if (n <= 0) {
        return;
    }
    if (n > cap) {
        cap = n;
        xyzw = realloc(xyzw, cap * 16);
        st = realloc(st, cap * 8);
        rgba = realloc(rgba, cap * 4);
    }
    for (i = 0; i < n; i++) {
        x += d[i * 3];
        y += d[i * 3 + 1];
        z += d[i * 3 + 2];
        xyzw[i * 4] = x / 4096.0f;
        xyzw[i * 4 + 1] = y / 4096.0f;
        xyzw[i * 4 + 2] = z / 4096.0f;
        AT(&xyzw[i * 4 + 3], 0, u32) = strip[i] & 1 ? 0x8000 : 0;
        st[i * 2] = uv[i * 2] / 32768.0f;
        st[i * 2 + 1] = uv[i * 2 + 1] / 32768.0f;
        AT(rgba, i * 4, u32) = 0x80808080;
    }
    obj_mvp(o, 32.0f, mvp);
    obj_strip(o, mvp, n, xyzw, st, rgba);
}

/* a part in the room's batch layout (func_00267AB0): float positions with GS flags words,
 * float texture coordinates, RGBA colours; model scale 1 */
static void gl_batch(u8 *o) {
    f32 mvp[4][4] __attribute__((aligned(16)));
    u8 *model = AT(o, 0x30, u8 *);

    obj_mvp(o, 1.0f, mvp);
    obj_strip(o, mvp, AT(model, 0x40, s32), (const f32 *)(model + AT(model, 0x54, s32)),
              (const f32 *)(model + AT(model, 0x4C, s32)), model + AT(model, 0x50, s32));
}

/* a morphing part (func_00267560 / func_002677C0): two key frames, +0x34 and +0x38, blended by
 * +0x3C (frame table at the model's +0x70 per frame, 8 bytes each: +0x4 its s16 positions /
 * 4096, +0x0 its RGBA colours, or for the lit kind s16 normals / 32768), u16 texture
 * coordinates / 32768 (+0x4C), u8 strip flags (+0x50); model scale 32. The lit kind's light is
 * fixed (axis directions, colour 64, ambient 128) and so always comes out at 0x80. */
static void gl_morph(u8 *o, s32 lit) {
    static f32 *xyzw, *st;
    static u8 *rgba;
    static s32 cap;
    f32 mvp[4][4] __attribute__((aligned(16)));
    u8 *model = AT(o, 0x30, u8 *);
    s32 n = AT(model, 0x40, s32), i, c;
    f32 t = AT(o, 0x3C, f32), w = 1.0f - t;
    const s16 *pa = (const s16 *)(model + AT(model, AT(o, 0x34, s32) * 8 + 0x74, s32));
    const s16 *pb = (const s16 *)(model + AT(model, AT(o, 0x38, s32) * 8 + 0x74, s32));
    const u8 *ca = model + AT(model, AT(o, 0x34, s32) * 8 + 0x70, s32);
    const u8 *cb = model + AT(model, AT(o, 0x38, s32) * 8 + 0x70, s32);
    const u16 *uv = (const u16 *)(model + AT(model, 0x4C, s32));
    const u8 *strip = model + AT(model, 0x50, s32);

    if (n <= 0) {
        return;
    }
    if (n > cap) {
        cap = n;
        xyzw = realloc(xyzw, cap * 16);
        st = realloc(st, cap * 8);
        rgba = realloc(rgba, cap * 4);
    }
    for (i = 0; i < n; i++) {
        for (c = 0; c < 3; c++) {
            xyzw[i * 4 + c] = pa[i * 3 + c] / 4096.0f * w + pb[i * 3 + c] / 4096.0f * t;
        }
        AT(&xyzw[i * 4 + 3], 0, u32) = strip[i] & 1 ? 0x8000 : 0;
        st[i * 2] = uv[i * 2] / 32768.0f;
        st[i * 2 + 1] = uv[i * 2 + 1] / 32768.0f;
        for (c = 0; c < 4; c++) {
            rgba[i * 4 + c] = lit ? (c < 3 ? 0x80 : 0x7F) : (u8)(s32)((f32)ca[i * 4 + c] * w + (f32)cb[i * 4 + c] * t);
        }
    }
    obj_mvp(o, 32.0f, mvp);
    obj_strip(o, mvp, n, xyzw, st, rgba);
}

static void gl_placed_object(u8 *o) {
    AT(o, 0x20, f32) = Angle_Wrap(AT(o, 0x20, f32));
    AT(o, 0x24, f32) = Angle_Wrap(AT(o, 0x24, f32));
    AT(o, 0x28, f32) = Angle_Wrap(AT(o, 0x28, f32));
    switch (AT(o, 0x8, u32) & 3) {
    case 0:
        gl_rigid(o);
        break;
    case 2:
        gl_batch(o);
        break;
    case 1:
        gl_morph(o, 1);
        break;
    default:
        gl_morph(o, 0);
        break;
    }
}
#endif

#ifdef HG_NATIVE
/* ---- PC: a positioned room mesh (doors and the like; the original PlacedMesh_Draw builds VU1
 * packets with func_0025EF40 / func_0025EB70 per batch) drawn with OpenGL ----
 *
 * The mesh (+0x8) is the room's batch list: per batch a header { vertex count, texture id (-1
 * none), flags (bits 0..7 the GS PRIM ABE etc., 24..31 a group shown by the mask +0x90) }, a
 * 4 x 4 matrix (columns), then the vertices in the room's mode 0 layout (RoomMesh_WriteBatch0); -1
 * ends it. The object stands at +0x70 turned by +0x80. Mode +0x68 1 (the lit layout) isn't
 * read yet. */
/* 0x0025F0A0 */
s32 PlacedMesh_Draw(u8 *o) {
    VObject *cam = gCamera;
    f32 t[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 w[4][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 mvp[4][4] __attribute__((aligned(16)));
    s32 *mesh = AT(o, 0x8, s32 *);
    s32 i;

    AT(o, 0x4, s32) = AT(o, 0x68, s32);
    if (mesh == NULL) {
        return 0;
    }
    if (AT(o, 0x4, s32) != 0) {
        glr_todo("positioned room mesh, lit layout (func_0025EB70)");
        return 1;
    }
    sceVu0UnitMatrix(t);
    sceVu0TransMatrix(t, t, (f32 *)(o + 0x70));
    sceVu0UnitMatrix(r);
    sceVu0RotMatrix(r, r, (f32 *)(o + 0x80));
    sceVu0MulMatrix(w, t, r);
    while (mesh[0] != -1) {
        s32 n = mesh[0], tex = mesh[1];
        u32 flags = (u32)mesh[2];
        u8 g = flags >> 24;
        u8 *st, *rgba, *xyz;

        AT(o, 0xA0, s32) = n;
        AT(o, 0xA4, s32) = tex;
        AT(o, 0xA8, s32) = flags & 0xFF;
        AT(o, 0xAC, u8) = g;
        for (i = 0; i < 4; i++) {
            m[0][i] = AT(mesh, 0x10 + i * 16, f32);
            m[1][i] = AT(mesh, 0x14 + i * 16, f32);
            m[2][i] = AT(mesh, 0x18 + i * 16, f32);
            m[3][i] = AT(mesh, 0x1C + i * 16, f32);
        }
        mesh += 0x50 / 4;
        sceVu0MulMatrix(m, w, m);
        VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, mvp);
        sceVu0MulMatrix(mvp, mvp, m);
        st = (u8 *)mesh;
        rgba = st + n * 8 + ((n & 3) == 3 || (n & 3) == 1 ? 8 : 0);
        xyz = rgba + n * 4 + ((n & 3) == 3 ? 4 : (n & 3) == 2 ? 8 : (n & 3) == 1 ? 0xC : 0);
        if (g == 0 || (AT(o, 0x90 + (g >> 5) * 4, u32) & (1u << (g & 0x1F)))) {
            glr_strip(&mvp[0][0], n, (const f32 *)xyz, (const f32 *)st, rgba,
                      tex == -1 ? NULL : VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, tex, 0),
                      tex == -1 ? 0 : TexCache_Tex0(tex), (tex != -1 ? 0x10 : 0) | 0xC | (flags & 0xFF) << 6);
        }
        mesh = (s32 *)(xyz + n * 16);
    }
    return 1;
}
#endif

/* back to the definition's two vectors */
/* 0x0025F810 */
void PlacedObject_ToDef(u8 *o) {
    u8 *def = AT(o, 0x70, u8 *);

    AT(o, 0x10, f32) = AT(def, 0x10, f32);
    AT(o, 0x14, f32) = AT(def, 0x14, f32);
    AT(o, 0x18, f32) = AT(def, 0x18, f32);
    AT(o, 0x20, f32) = AT(def, 0x20, f32);
    AT(o, 0x24, f32) = AT(def, 0x24, f32);
    AT(o, 0x28, f32) = AT(def, 0x28, f32);
}

/* 0x0025F850 */
void PlacedObject_Init3(u8 *o, u8 *def) {
    placed_init(o, 3, def);
}

/* 0x0025F8B0 */
void PlacedObject_Init2(u8 *o, u8 *def) {
    placed_init(o, 2, def);
}

/* 0x0025F910 */
void PlacedObject_Init1(u8 *o, u8 *def) {
    placed_init(o, 1, def);
}

/* 0x0025F970 */
void PlacedObject_Init0(u8 *o, u8 *def) {
    placed_init(o, 0, def);
}

/* start animation `id` of the object (+0x94 its data from the placed objects' +0x1C, played
   by +0x98 from the start) */
/* 0x0025F9D0 */
void PlacedObject_StartAnim(u8 *o, s32 id) {
    u8 *a;

    AT(o, 0x94, u8 *) = VCALL((VObject *)gRoomObjects, 0x1C, u8 *(*)(void *, u8 *, s32))(gRoomObjects, AT(o, 0x70, u8 *), id);
    a = AT(o, 0x94, u8 *);
    if (a != NULL) {
        Triple_Set(o + 0x98, AT(a, 0x4, s32), (s32)(a + AT(a, 0x8, s32)), AT(a, 0x0, s32));
        AT(o, 0x90, s32) = 0;
    }
}

/* a placed object's animation, each frame: its track (+0x98 { keys, format, count }) at the
 * frame +0x90 gives its position (+0x10; format 9: +0x20); at the end it loops (+0x1) or
 * stops */
/* 0x0025FA50 */
void PlacedObject_Animate(u8 *obj) {
    if (AT(obj, 0x94, s32) == 0 || AT(obj, 0x98, void *) == NULL) {
        return;
    }
    if (AT(obj, 0x9C, u16) == 9) {
        Track_Sample(obj + 0x98, (f32 *)(obj + 0x20), (f32)AT(obj, 0x90, s32));
    } else {
        Track_Sample(obj + 0x98, (f32 *)(obj + 0x10), (f32)AT(obj, 0x90, s32));
    }
    AT(obj, 0x90, s32)++;
    if (AT(obj, 0x90, s32) < AT(obj, 0xA0, s32)) {
        return;
    }
    if (AT(obj, 0x1, u8) != 0) {
        AT(obj, 0x90, s32) = 0;
    } else {
        AT(obj, 0x94, s32) = 0;
        AT(obj, 0xA0, s32) = 0;
        AT(obj, 0x98, s32) = 0;
        AT(obj, 0x9C, s32) = 0;
    }
}

/* draw a placed object in pass `layer` (layer by its name +0x70: "g_..." 0x26, "a_..." 0x19
 * (alpha, +0x80 set), others 1): its model (+0x40) takes its position and rotation */
/* 0x0025FB10 */
void PlacedObject_Draw(u8 *obj, s32 layer) {
    const u8 *name;
    s32 own;

    sceVu0CopyVector((f32 *)(obj + 0x50), (f32 *)(obj + 0x20));
    sceVu0CopyVector((f32 *)(obj + 0x60), (f32 *)(obj + 0x10));
    name = AT(obj, 0x70, const u8 *);
#ifdef HG_NATIVE
    if (name == NULL) {
        name = (const u8 *)"";   /* the PS2 reads its address 0 here */
    }
#endif
    AT(obj, 0x80, u8) = 0;
    if ((name[0] == 'g' || name[0] == 'G') && name[1] == '_') {
        own = 0x26;
    } else if ((name[0] == 'a' || name[0] == 'A') && name[1] == '_') {
        own = 0x19;
        AT(obj, 0x80, u8) = 1;
    } else {
        own = 1;
    }
    if (own == layer) {
        VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, obj + 0x40, layer, 0);
    }
}

/* a placed object released: its state cleared and its animation key (+0x98) reset */
/* 0x0025FC10 */
void PlacedObject_Release(u8 *obj) {
    AT(obj, 0x0, u8) = 0;
    AT(obj, 0x1, u8) = 0;
    AT(obj, 0x8, s32) = 0;
    AT(obj, 0x70, s32) = 0;
    AT(obj, 0x74, s32) = 0;
    AT(obj, 0x78, s32) = 0;
    AT(obj, 0x7C, s32) = 0;
    AT(obj, 0x90, s32) = 0;
    AT(obj, 0x94, s32) = 0;
    Triple_Set(obj + 0x98, 0, 0, 0);
}

/* one of the 64 (0xB0 bytes): cleared */
/* 0x0025FC50 */
void PlacedObject_Clear(u8 *e) {
    AT(e, 0x0, u8) = 0;
    AT(e, 0x1, u8) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x70, s32) = 0;
    AT(e, 0x74, s32) = 0;
    AT(e, 0x78, s32) = 0;
    AT(e, 0x7C, s32) = 0;
    AT(e, 0x90, s32) = 0;
    AT(e, 0x94, s32) = 0;
    Triple_Set(e + 0x98, 0, 0, 0);
}

/* destructor (vtable PlacedModelBase_vtable) */
/* 0x00267500 */
void *PlacedModelBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = PlacedModelBase_vtable;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* a positioned room mesh's last batch is shown: its group (+0xAC) is 0 or on in the mask
 * (+0x90) */
/* 0x0025EEE0 */
s32 PlacedMesh_LastShown(u8 *o) {
    u8 g = AT(o, 0xAC, u8);

    return g == 0 || (AT(o, 0x90 + (g >> 5) * 4, u32) & (1u << (g & 0x1F))) != 0;
}

/* vtable +0xC of a placed object's model: its packets by its flags (+0x8 bit 0, bit 1) */
/* 0x00268090 */
s32 PlacedModel_Draw(u8 *o) {
#ifdef HG_NATIVE
    gl_placed_object(o);
    return 1;
#endif
    if (AT(o, 0x8, u32) & 1) {
        if (AT(o, 0x8, u32) & 2) {
            func_00267560(o);
        } else {
            func_002677C0(o);
        }
    } else if (AT(o, 0x8, u32) & 2) {
        func_00267AB0(o);
    } else {
        func_00267D60(o);
    }
    return 1;
}

/* room manager +0x9360: take PAC section 14 (+0x4; its count at +0x8) */
/* 0x002A88A0 */
s32 RoomMgr_TakeSection14(u8 *o, u8 *sec) {
    if (sec != NULL) {
        AT(o, 0x8, s32) = AT(sec, 0, s32);
        AT(o, 0x4, u8 *) = sec;
    } else {
        AT(o, 0x8, s32) = 0;
        AT(o, 0x4, u8 *) = NULL;
    }
    return 0;
}

/* 0x002C8D90 */
s32 PlacedObjects_Sum(u8 *self) {
    s32 *v = *(s32 **)(self + 0x4);

    return v[0] + v[1] + v[2] + v[3];
}

/* +0x28 the placed objects, each frame: every active one (bit set in +0xC, 64 slots of 0xB0
 * from +0x20) updates */
/* 0x002C8DC0 */
void PlacedObjects_Update(u8 *o) {
    s32 i;

    for (i = 0; i < 64; i++) {
        if (AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F))) {
            PlacedObject_Animate(o + 0x20 + i * 0xB0);
        }
    }
}

/* the placed objects, drawn each frame in three passes (layers 1, 0x19, 0x26; the texture
 * cache +0x18 and gBootMessage +0x20 reset before each): every active one (bit in +0xC) that
 * isn't hidden (+0x0) */
/* 0x002C8E50 */
void PlacedObjects_Draw(u8 *o) {
    static const s32 kLayers[3] = {1, 0x19, 0x26};
    VObject *tc = gTexCache;
    VObject *msg;
    s32 pass, i;

    for (pass = 0; pass < 3; pass++) {
        if (pass == 0) {
            VCALL(tc, 0x18, void (*)(VObject *))(tc);
            msg = gBootMessage;
            VCALL(msg, 0x20, void (*)(VObject *))(msg);
        } else {
            VCALL(tc, 0x18, void (*)(VObject *))(tc);
            VCALL(msg, 0x20, void (*)(VObject *))(msg);
        }
        for (i = 0; i < 64; i++) {
            u8 *obj = o + 0x20 + i * 0xB0;

            if ((AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F))) && AT(obj, 0x0, u8) == 0) {
                PlacedObject_Draw(obj, kLayers[pass]);
            }
        }
    }
}

/* room manager +0x6740 +0x20: each placed object in use released and its bit cleared */
/* 0x002C9010 */
void PlacedObjects_ReleaseAll(u8 *o) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        u8 *obj = o + 0x20 + i * 0xB0;
        u32 k;

        if (!(AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F)))) {
            continue;
        }
        PlacedObject_Release(obj);
        k = (u32)(obj - (o + 0x20)) / 0xB0;
        AT(o, 0xC + (k >> 5) * 4, u32) &= ~(1u << (k & 0x1F));
    }
}

/* room manager +0x6740 +0x1C: entry k of the named record in its table (+0x8: count, then
 * offsets; each record a name, +0x10 its count, +0x14 the offset of its 0x10-byte entries) */
/* 0x002C90F0 */
u8 *PlacedObjects_RecordEntry(u8 *o, const char *name, u32 k) {
    u8 *tbl = AT(o, 0x8, u8 *);
    u32 j;

    if (tbl == NULL) {
        return NULL;
    }
    for (j = 0; j < AT(tbl, 0x0, u32); j++) {
        u8 *e = AT(o, 0x8, u8 *) + AT(tbl, 0x4 + j * 4, u32);

        if (msl_strcmp((const char *)e, name) == 0) {
            if (k < AT(e, 0x10, u32)) {
                return e + AT(e, 0x14, u32) + k * 0x10;
            }
            return NULL;
        }
    }
    return NULL;
}

/* room manager +0x6740 +0x18: the placed object named `name` (its +0x70), or NULL */
/* 0x002C91D0 */
u8 *PlacedObjects_Named(u8 *o, const char *name) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        u8 *obj = o + 0x20 + i * 0xB0;

#ifdef HG_NATIVE
        if (AT(obj, 0x70, const char *) == NULL) {   /* unnamed (the PS2 compares with RAM at 0) */
            continue;
        }
#endif
        if ((AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F))) &&
            msl_strcmp(AT(obj, 0x70, const char *), name) == 0) {
            return obj;
        }
    }
    return NULL;
}

/* room manager +0x6740 +0x14: create the room's placed objects from its table (+0x4: four
 * counts, one per kind, then offsets from +0x20), each in a free slot of 64 (0xB0 each from
 * +0x20, use bitmap +0xC) */
/* 0x002C92A0 */
void PlacedObjects_Create(u8 *o) {
    u8 *h = AT(o, 0x4, u8 *);
    s32 end0, end1, end2, total;
    s32 i, j;

    if (h == NULL) {
        return;
    }
    end0 = AT(h, 0x0, s32);
    end1 = end0 + AT(h, 0x4, s32);
    end2 = end1 + AT(h, 0x8, s32);
    total = end2 + AT(h, 0xC, s32);
    for (i = 0; i < total; i++) {
        u8 *slot = NULL;
        u8 *def;

        for (j = 0; j < 0x40; j++) {
            u32 *used = &AT(o, 0xC + (j >> 5) * 4, u32);

            if (!(*used & (1 << (j & 0x1F)))) {
                *used |= 1 << (j & 0x1F);
                slot = o + j * 0xB0 + 0x20;
                PlacedObject_Clear(slot);
                break;
            }
        }
        if (slot == NULL) {
            continue;
        }
        def = h + AT(h, 0x20 + i * 4, u32);
        if (i < end0) {
            PlacedObject_Init0(slot, def);
        } else if (i < end1) {
            PlacedObject_Init1(slot, def);
        } else if (i < end2) {
            PlacedObject_Init2(slot, def);
        } else {
            PlacedObject_Init3(slot, def);
        }
    }
}

/* room manager +0x6740 +0x34 */
/* 0x002C9460 */
void PlacedObjects_Noop34(u8 *o) {
}

/* Tail call to this->vfunc_0x8() */
/* 0x002C9470 */
s32 PlacedObjects_CallDtor(void *self) {
    return VCALL(self, 0x8, s32 (*)(void *))(self);
}

/* the manager of 64 (PlacedObjects_vtable) +0x8: clear them all */
/* 0x002C9480 */
void PlacedObjects_ClearAll(u8 *p) {
    s32 i;

    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, s32) = 0;
    for (i = 0; i < 0x40; i++) {
        PlacedObject_Clear(p + 0x20 + i * 0xB0);
    }
}

/* a manager of 64 objects of 0xB0 bytes (vtable PlacedObjects_vtable, global gRoomObjects) */
/* 0x002D1160 */
void *PlacedObjects_ctor(u8 *p) {
    AT(p, 0x0, void **) = PlacedObjects_vtable;
    gRoomObjects = p;
    __construct_array(p + 0x20, PlacedModelHolder_ctor, QuadEntry_dtor, 0xB0, 0x40);
    return p;
}

/* 0x002D11C0 */
void *PlacedModelHolder_ctor(u8 *p) {
    F(p, 0x40, void *) = Helper469D00_vtable;
    F(p, 0x44, s32) = -1;
    F(p, 0x40, void *) = PlacedModelBase_vtable;
    F(p, 0xA0, u32) = 0;
    F(p, 0x98, u32) = 0;
    F(p, 0x9C, u32) = 0;
    return p;
}

/* 0x002D1260 */
void *PlacedMeshHolder_ctor(u8 *p) {
    F(p, 0x80, void *) = Helper469D00_vtable;
    F(p, 0x84, s32) = -1;
    F(p, 0x80, void *) = PlacedMesh_vtable;
    F(p, 0x120, u32) = 0;
    F(p, 0x124, u32) = 0;
    F(p, 0xE4, s32) = -1;
    F(p, 0x128, u32) = 0;
    p[0xE0] = 0;
    F(p, 0xE8, u32) = 0;
    F(p, 0x190, void *) = Helper469D00_vtable;
    F(p, 0x194, s32) = -1;
    F(p, 0x190, void *) = DoorShadow_vtable;
    return p;
}

/* 0x002D1360 */
void *RoomMeshes_ctor(u8 *p) {
    F(p, 0x0, void *) = Helper469D00_vtable;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = RoomMeshes_vtable;
    F(p, 0x7C, u32) = 0;
    F(p, 0x80, u32) = 0;
    F(p, 0x64, s32) = -1;
    p[0x84] = 0;
    p[0x60] = 0;
    F(p, 0x68, u32) = 0;
    p[0x85] = 0;
    return p;
}

/* a kind-4 batch's texture coordinates, a flip book: its entry (+0xD8 + 7 x +0xD4: frames,
 * frame time, type (2: stop on the last frame), frame, timer, u step, v step in 1/256) steps a
 * frame when its timer runs out (unless +0x8B holds it; the coordinates in use, +0x240 + 32 x
 * index, are last frame's +0x140 ones), each frame moving the batch's own u (st) on by the u
 * step; past 1 they wrap back by whole units (and down to 0 at least) and v moves on by that
 * many v steps. Hidden batches (group +0x8A off in the mask +0x6C) are just passed. Moves on
 * to the next batch (+0xD4); 1 */
/* 0x0025CB50 */
s32 RoomMesh_FlipBook(u8 *o, const f32 *st) {
    static const union { u32 u; f32 f; } k256th = {0x3B800000}, k16th = {0x3C800000};
    u8 *ent = o + 0xD8 + AT(o, 0xD4, s32) * 7;
    u8 g = AT(o, 0x8A, u8);
    f32 *uv;
    f32 m, fw;
    s32 k, whole;

    if (g != 0 && !(AT(o, 0x6C + (g >> 5) * 4, u32) & (1u << (g & 0x1F)))) {
        AT(o, 0xD4, s32) += 1;
        return 1;
    }
    if (AT(o, 0x8B, u8) == 0) {
        ent[4] -= 1;
        for (k = 0; k < 8; k++) {
            AT(o, 0x240 + AT(o, 0xD4, s32) * 32 + k * 4, f32) = AT(o, 0x140 + AT(o, 0xD4, s32) * 32 + k * 4, f32);
        }
    }
    if (ent[4] != 0) {
        AT(o, 0xD4, s32) += 1;
        return 1;
    }
    ent[3] += 1;
    if (!(ent[3] < ent[0])) {
        ent[3] = ent[2] == 2 ? ent[0] - 1 : 0;
    }
    ent[4] = ent[1];
    for (k = 0; k < 4; k++) {
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2] = st[k * 2] + (f32)ent[3] * ((f32)ent[5] * k256th.f);
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2 + 1] = st[k * 2 + 1];
    }
    uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
    if (uv[0] <= 1.0f && uv[2] <= 1.0f && uv[4] <= 1.0f && uv[6] <= 1.0f) {
        AT(o, 0xD4, s32) += 1;
        return 1;
    }
    m = uv[0];
    for (k = 1; k < 4; k++) {
        if (m < uv[k * 2]) {
            m = uv[k * 2];
        }
    }
    whole = (s32)(m - k16th.f);
    fw = (f32)whole;
    for (k = 0; k < 4; k++) {
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2] = uv[k * 2] - fw;
    }
    uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
    if (uv[0] < 0.0f || uv[2] < 0.0f || uv[4] < 0.0f || uv[6] < 0.0f) {
        m = uv[0];
        for (k = 1; k < 4; k++) {
            if (!(m <= uv[k * 2])) {
                m = uv[k * 2];
            }
        }
        for (k = 0; k < 4; k++) {
            uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
            uv[k * 2] = uv[k * 2] - m;
        }
    }
    for (k = 0; k < 4; k++) {
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2 + 1] = uv[k * 2 + 1] + (f32)whole * ((f32)ent[6] * k256th.f);
    }
    AT(o, 0xD4, s32) += 1;
    return 1;
}

/* the room manager: take up to 8 areas from PAC section 3 (+0x10: offset of a list of
 * 0xC0-byte entries, -1 terminated). Per area: 7 bytes at +0xD8 (+0x8 bits 16..23, the low and
 * high nibble of +0xC, 0, the low nibble again, the size of its (x, z) quad in 1/256: width,
 * depth) and the quad (4 points in 0..1 from entry +0x50) at +0x140 and +0x240 (0x20 each) */
/* 0x0025D370 */
void RoomMgr_TakeAreas(u8 *rm, u8 *sec) {
    u8 *e;
    s32 j;
    s32 k;

    if (AT(sec, 0x10, u32) == 0) {
        return;
    }
    e = sec + AT(sec, 0x10, u32);
    if (AT(e, 0, s32) == -1) {
        return;
    }
    for (j = 0; ; ) {
        u8 *b = rm + 0xD8 + j * 7;
        f32 *q = &AT(rm, 0x140 + j * 0x20, f32);
        f32 minX = 1.0f, maxX = 0.0f, minZ = 1.0f, maxZ = 0.0f;

        b[0] = (AT(e, 0x8, u32) & 0xFF0000) >> 16;
        b[1] = AT(e, 0xC, u32) & 0xF;
        b[2] = (AT(e, 0xC, u32) & 0xF0) >> 4;
        b[3] = 0;
        b[4] = b[1];
        for (k = 0; k < 4; k++) {
            f32 x = AT(e, 0x50 + k * 8, f32);
            f32 z;

            if (!(minX <= x)) {
                minX = x;
            }
            if (maxX < x) {
                maxX = x;
            }
            q[k * 2] = x;
            z = AT(e, 0x54 + k * 8, f32);
            if (!(minZ <= z)) {
                minZ = z;
            }
            if (maxZ < z) {
                maxZ = z;
            }
            q[k * 2 + 1] = z;
        }
        b[5] = (u8)(u32)(maxX * 256.0f - minX * 256.0f);
        b[6] = (u8)(u32)(maxZ * 256.0f - minZ * 256.0f);
        for (k = 0; k < 8; k++) {
            AT(rm, 0x240 + j * 0x20 + k * 4, f32) = q[k];
        }
        if (++j >= 8) {
            return;
        }
        e += 0xC0;
        if (AT(e, 0, s32) == -1) {
            return;
        }
    }
}

#ifdef HG_NATIVE
/* a kind-4 room part's batch (its entry +0xD8 + 7 x +0xD4, type at +2): types 1 / 3 / 4 / 6
 * face the camera (1 and 6 staying upright) - the batch matrix (+0x90) turned to the view about
 * its own place; then its blending (for the batch's GL draw): types 4..6 additive (+0x84 on) without
 * depth writes, the others normal with depth writes */
/* 0x0025C8C0 */
void RoomMesh_Billboard(u8 *o) {
    u8 *ent = o + 0xD8 + AT(o, 0xD4, s32) * 7;
    u8 type = ent[2];

    if (type == 1 || type == 3 || type == 4 || type == 6) {
        VObject *cam = gCamera;
        f32 m[4][4] __attribute__((aligned(16)));
        f32 b[4][4] __attribute__((aligned(16)));

        sceVu0CopyMatrix(m, (f32 (*)[4])(o + 0x90));
        m[3][0] = 0.0f;   /* turned about its own place */
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        sceVu0UnitMatrix(b);
        VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, b[2]);
        if (ent[2] == 1 || ent[2] == 6) {
            b[2][1] = 0.0f;
            sceVu0Normalize(b[2], b[2]);
        }
        VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, b[1]);
        sceVu0OuterProduct(b[0], b[1], b[2]);
        sceVu0Normalize(b[0], b[0]);
        sceVu0OuterProduct(b[1], b[0], b[2]);
        sceVu0Normalize(b[1], b[1]);
        sceVu0MulMatrix(m, b, m);
        sceVu0TransMatrix((f32 (*)[4])(o + 0x90), m, (f32 *)(o + 0xC0));
    }
    if ((u8)(ent[2] - 4) < 3) {
        AT(o, 0x84, u8) = 1;
        sGlKindPrim = GLR_PRIM_ADD | GLR_PRIM_NOZW;
    } else {
        sGlKindPrim = 0;
    }
}
#endif

/* reset a room mesh drawer: no matrix row (+0x90), no mesh (+0x4), parallax off (+0x60, +0x68) */
/* 0x0025EEC0 */
void RoomMesh_Reset(u8 *o) {
    AT(o, 0x90, s32) = 0;
    AT(o, 0x94, s32) = 0;
    AT(o, 0x98, s32) = 0;
    AT(o, 0x9C, s32) = 0;
    AT(o, 0x4, s32) = 0;
    AT(o, 0x68, s32) = 0;
    AT(o, 0x60, u8) = 0;
}
