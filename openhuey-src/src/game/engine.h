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
#define TICKS_PER_SECOND 30   /* the game's frame rate (src/game/system.c: two vblanks a frame) */

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
    /* scripted lighting on top of the room's (the original's Lights +0x950, Lights_Scripted; a
     * cutscene's cues set it): the room lights' colours scaled, the ambient raised, and up to
     * two lights from the camera's side, turned `up` (about the axis across the view) and
     * `about` (about the vertical) from the direction back to the eye */
    struct {
        int on;
        Vec3 ambient;
        float scale;
        struct {
            int on;
            Vec3 color;
            float up, about;
        } cam[2];
    } extra;
    /* reflecting quads (the room effect Reflection: `reflect!`): the characters drawn again,
     * mirrored in its plane, at its strength (0..0x80); `screen`: over the whole picture;
     * `panels` (the strength word's top bits: 1 the window's, 2 the door's - castle 2F) the
     * reflection again on the panels either side, moved by the quad's width on screen */
    struct {
        int strength, screen, panels;
        int only;   /* (a mirror fragment's) the one actor it reflects, -1 all on the eye's side */
        Vec3 q[4];
    } refl[4];
    long ticks;
    int testing;      /* run by --test: nothing kept on disk (the system data) */
    int paused;            /* a menu is up: actors and the room stand still (`pause!`) */
    int still;             /* the models and the room stand still, the actors run (`still!`: the game over) */
    int alone;             /* an actor drawn alone (its id + 1; 0: the scene as it is) - the model gallery's */
    int black_frame;       /* the next picture black under the 2D (a scene's end: the scripts' 0x62 op 11) */
    char hud[128];         /* a line of text at the bottom of the screen ("" none) */
    char screenshot[256];  /* a screenshot to save after this frame ("" none) */
    int width, height;     /* the window, in pixels */
} Engine;

extern Engine gEngine;

/* the Forth words for the engine */
void bind_engine(Forth *f);
void bind_engine_reset(void);       /* the 2D, movie, music, sounds, cutscene as at the start */
int bind_engine_reset_wanted(void); /* `soft-reset` asked for (once) */
/* one game tick: tasks, then the per-tick hooks */
void engine_tick(Engine *e);
/* how far a character has faded going out through a doorway (0 none .. 1 gone: main.c) */
float doorway_faded(const Engine *e, const Actor *a);
/* a sound at a point of the room, as it is (Sound_PlayBankAt: the effects' own) */
void engine_sound_at(uint32_t id, int bank, Vec3 at);
/* the scripts' 2D drawing for this frame (the on-draw hooks) */
void engine_draw_2d(Engine *e);

#endif
