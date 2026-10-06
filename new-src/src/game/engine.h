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
    Camera camera;
    Room room;
    World world;           /* how the rooms connect */
    Actor actors[MAX_ACTORS];
    Console console;
    Word *hooks[ENGINE_HOOKS];   /* run every tick (`on-tick`) */
    int nhooks;
    Vec3 clear;            /* the background colour */
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

#endif
