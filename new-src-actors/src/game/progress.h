/* The game's state that the event scripts read and change: the story so far (Progress: what a
 * save keeps) and the event system's own state while in a room (EventState). C owns both;
 * Forth reaches them through `progress` / `event-state` and the field words (bind_state).
 *
 * The original keeps these in its progress object and the event object (src/game/progress.c,
 * src/game/event.c); the offsets in the comments are theirs. */
#ifndef PROGRESS_H
#define PROGRESS_H

#include <stdint.h>

#include "../forth/forth.h"

#define STORY_FLAGS 0x400       /* the scripts use up to 0x36B */
#define STATE_FLAGS 64          /* the original has 46 */
#define PROGRESS_VARS 64        /* the scripts use up to 0x33 */
#define RESIDENT_FLAGS 128
#define ROOM_COUNT 0x110
#define CHARACTERS 6            /* the original's character slots: 0 Fiona, 1 Hewie, 2.. others */
#define SCRIPT_SLOTS 17         /* the action scripts running at once */
#define SCRIPT_VARS 32
#define DOORS 400

typedef struct Progress {
    uint32_t story[STORY_FLAGS / 32];       /* the scenario flags (+0x1C) */
    uint32_t state[STATE_FLAGS / 32];       /* control, panic, ... (+0x8) */
    uint8_t vars[PROGRESS_VARS];            /* the byte variables (+0x9C) */
    uint32_t resident[RESIDENT_FLAGS / 32]; /* kept across games: unlocks (the game's +0x24) */
    uint32_t visited[(ROOM_COUNT + 31) / 32];   /* rooms entered (+0xDC) */
    /* each door's state (+0x124): bit 0 held, 1 open, 2 passable, 3 LOCKED (the decomp's
     * Progress_UnlockDoor / LockDoor / DoorUnlocked are named the wrong way round), bits 4..7
     * the sides it is locked from */
    uint32_t doors[DOORS];
    uint32_t closed_off[(DOORS + 31) / 32];      /* doors closed off for good (the rooms' +0x4) */
    /* the inventory (the item manager, gSubScreen +0x8): how many of each item (ids under 0xAD:
     * Items_Give / Items_Count / Items_UseId) */
    uint8_t items[0x100];
    /* the files: key things and documents found (ids 0x2xx; SubScreen_AddFile, its list at
     * +0x15F8: 128, 0-terminated, in the order found) */
    uint16_t files[128];
    int32_t items_counted;                  /* items given by the scripts' 0x83 (+0xFBE) */
} Progress;

/* a character as the scripts see it (the original's character object: +0x30 its room, +0xE0
 * busy, +0x14E8 its script state, 5: running an action script; +0x153C its script id) */
typedef struct ScriptChar {
    int32_t present;    /* in the game (the original's +0x28) */
    int32_t id;         /* its script id: 0 Fiona, 1 Hewie, ... (-1: slot empty) */
    int32_t room;
    int32_t scripted;   /* doing a scripted action: its action script drives it */
    int32_t actor;      /* new-src's actor (-1: none) */
    int32_t cam_set;    /* the camera set and path for when the camera follows it (+0xE8, +0xEC) */
    int32_t cam_path;
    int32_t move;       /* the move a script gave it (+0xF4: 2 idle, 5 / 10 walk / run to a point,
                         * 7 / 8 / 16 an animation, ...), 0 none */
    int32_t move_done;  /* that move is done (+0xE1) */
    int32_t move_anim;  /* its animation */
    float target[3];    /* where it goes (+0x110) */
    float face;         /* the heading it ends with (+0x10C), radians; > 9: keep */
    float pos[3];       /* where it is (+0x10) */
    float prev[3];      /* where it was the frame before (+0x40) */
    /* what the characters' own behaviour (Fiona's, Hewie's, the stalkers') keeps */
    int32_t tri;        /* its nav triangle (+0x34), -1 none */
    int32_t cond;       /* its condition (+0xC4): 0 well, 1 hurt, 2 down */
    int32_t hp;         /* its health */
    int32_t mode, sub;  /* what it is doing: the move mode (+0xF8 on Hewie, moveMode) and its
                         * sub-mode (moveSub); Fiona's 4 / 9 or 0x12: held */
    int32_t disabled;   /* out of play: hidden away, not moving (+0x29) */
    int32_t req, req_arg;   /* a pending request from outside (+0x14E8 / +0x14EC: 12 Fiona's
                             * call, 13 her command, ...) */
    int32_t req_x[6];       /* the rest of that block (+0x14F0..: [2] .. [7]) */
    float radius, height;   /* (+0xC8 / +0xCC) */
    int32_t silent;     /* its own sounds (Actor_PlaySound) don't play (+0x2C) */
    int32_t req2[8];    /* the request kept for when a joint action starts (+0x1508: state2) */
} ScriptChar;

/* an action script running (the original's 0x18-byte contexts at +0x564) */
typedef struct ScriptSlot {
    int32_t task;       /* the Forth task running it (0: the slot is free) */
    int32_t who;        /* its character's slot (-1: a scene script, no character) */
    int32_t id;         /* the id it was started for (+0x13): what `self-is?` compares */
    int32_t frames;     /* frames it has run (+0x14): `self-frames-reset`, `self-wait-frames` */
} ScriptSlot;

typedef struct EventState {
    int32_t room;                   /* the room the scripts are for (+0x560) */
    int32_t vars[SCRIPT_VARS];      /* the script variables (+0x810), cleared on entering */
    uint32_t bits;                  /* the event bits (+0x890), cleared on entering */
    int32_t counter;                /* the event counter (+0x703) */
    int32_t exit;                   /* the exit taken (+0x702) */
    int32_t room_frames;            /* phase 1 calls in this room (+0x704) */
    int32_t result;                 /* the event result (+0x934) */
    int32_t message;                /* the message window's text (-1: closed) (+0x708 task) */
    int32_t answer;                 /* the window's chosen option (+0x750) */
    int32_t message_owner;          /* the character whose script opened it (+0x80C), -1 none */
    int32_t slot;                   /* the action slot running now (-1: a phase script) */
    int32_t self_id;                /* the running context's id (+0x13; 0xFF in a phase) */
    int32_t self_char;              /* the running context's character slot (+0x0), -1 none */
    int32_t leaving;                /* an exit was taken: no more actions start (progress +0x4) */
    int32_t camera_char;            /* the character slot the camera follows (+0x1130), $FF none */
    int32_t self_frames;            /* a phase script's frame count (its context's +0x14) */
    ScriptSlot slots[SCRIPT_SLOTS];
    ScriptChar chars[CHARACTERS];
} EventState;

extern Progress gProgress;
extern EventState gEvents;

/* `progress`, `event-state` and their fields, in the vocabulary `game-state` */
void bind_state(Forth *f);

#endif
