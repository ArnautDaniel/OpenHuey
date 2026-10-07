/* The engine: everything the game is made of, in one place. C owns this state; Forth reads and
 * changes it through the words in forth/bind_engine.c. */
#ifndef ENGINE_H
#define ENGINE_H

#include "../forth/forth.h"
#include "../platform/platform.h"
#include "actor.h"
#include "camera.h"
#include "console.h"
#include "room.h"
#include "world.h"

#define ENGINE_HOOKS 32
#define TICKS_PER_SECOND 60

typedef struct Engine {
    Forth *forth;
    Input input;           /* this frame's input, as the game sees it (none while the console is open) */
    bool held[SDL_SCANCODE_COUNT];   /* keys held down by scripts (`key-hold`: demos, tests) */
    bool held_last[SDL_SCANCODE_COUNT];     /* (as they were at the last tick's start) */
    bool held_pressed[SDL_SCANCODE_COUNT];  /* went down since: they count as pressed this tick */
    Camera camera;
    Room room;
    World world;           /* how the rooms connect */
    Actor actors[MAX_ACTORS];
    Console console;
    Word *hooks[ENGINE_HOOKS];   /* run every tick (`on-tick`) */
    int nhooks;
    Word *draw_hooks[ENGINE_HOOKS];   /* run every frame to draw 2D (`on-draw`) */
    int ndraw_hooks;
    Vec3 clear;            /* the background colour */
    RoomLight stage[3];    /* lights set by scripts (a scene of their own, e.g. the title): */
    int nstage;            /* used in place of the room's when there are any */
    Vec3 stage_ambient;    /* (0..128, as a room's) */
    long ticks;
    char hud[128];         /* a line of text at the bottom of the screen ("" none) */
    char screenshot[256];  /* a screenshot to save after this frame ("" none) */
    int width, height;     /* the window, in pixels */
} Engine;

extern Engine gEngine;

/* the Forth words for the engine */
void bind_engine(Forth *f);
/* one game tick: tasks, then the per-tick hooks */
void engine_tick(Engine *e);
/* the scripts' 2D drawing for this frame (the on-draw hooks) */
void engine_draw_2d(Engine *e);

#endif
