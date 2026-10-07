/* The camera director: the game's camera through a room (src/game/camera.c CamDirector_*,
 * CamPath_*; src/game/vecmath.c Spline_*). A room has camera sets (PAC section 5: an eye, a
 * point looked at and a view angle each) and camera paths (section 6: splines with an eye
 * track and a look-at track). The scripts pick a set and a path for each character
 * (`char-camera`, `area-camera`); the director takes those of the character it follows: on a
 * new set it cuts to it; with a path it slides along it, staying nearest the character; with
 * none it turns to keep the character in view.
 *
 * It keeps its own eye, look-at point and view angle (the original's camera), which go to the
 * engine's camera each frame (camdir_apply). */
#ifndef CAMDIRECTOR_H
#define CAMDIRECTOR_H

#include "../core/mathx.h"
#include "camera.h"

#include <stdint.h>

/* a cubic Bezier spline over several components (keys of 8 floats: time, value, in-tangent at
 * [3], out-tangent at [5]; each component's keys in a row) */
typedef struct Spline {
    float t;            /* the current time */
    const float *keys;
    int n;              /* keys per component */
    int tmin, tmax;     /* the first and last key's time */
    int seg;            /* the key the current time is after */
    int dims;
} Spline;

typedef struct CamDirector {
    /* the path (+0x0 .. +0x34) */
    float step_look;    /* time steps along the look-at track for 0.195 units a time */
    float step_eye;
    Spline path;
    float len_look;     /* the tracks' lengths */
    float len_eye;
    const int32_t *data;   /* the room's camera paths (section 6), NULL: none */
    int npaths;
    int loaded;         /* the path loaded (+0x34) */
    /* the target */
    int target;         /* the actor followed (-1: none) (+0xB0) */
    int last_target;    /* (+0xB4) */
    Vec3 offset;        /* added to its position (+0xC0) */
    Vec3 point;         /* the point to look at without a target (+0xA0: the set's) */
    float move_rate;    /* how fast it follows along the path (+0x50, 6) */
    /* the setup */
    int set, path_no;           /* wanted (+0x6C, +0x70; path -1: none) */
    int last_set, last_path;    /* taken (+0x78, +0x7C) */
    int event;          /* an event drives it (+0xF4): nothing moves */
    /* the view angle (horizontal, across a 4:3 picture, radians) */
    float fov;          /* now (+0x80) */
    float set_fov;      /* the set's (+0x84) */
    float fov_dip;      /* how far it dips after a cut (+0x88) */
    uint8_t cut;        /* frames left of the dip (+0x8C; 0x80: settled) */
    uint32_t ease;      /* frames into easing back (+0x90) */
    /* the camera it drives */
    Vec3 eye, look;
} CamDirector;

extern CamDirector gCamDir;

/* no set, no path, no target (CamDirector_NewRoom: the start of play) */
void camdir_new_room(CamDirector *d);
/* the room's camera sets (section 5) and paths (section 6, NULL: none) taken and the camera put
 * at the current set / path (CamDirector_RoomStart: each room) */
void camdir_room_start(CamDirector *d, const float *sets, int nsets, const int32_t *paths);
/* the set and path to use (CamDirector_SetSetup) */
void camdir_set_setup(CamDirector *d, int set, int path);
/* follow actor `target` (-1: none) with an offset (CamDirector_Follow) */
void camdir_follow(CamDirector *d, int target, Vec3 offset);
/* each frame: the view angle eases, the camera along its path (CamDirector_Ease) */
void camdir_ease(CamDirector *d);
/* each frame: a new set or path taken (CamDirector_Track) */
void camdir_track(CamDirector *d);
/* each frame: the camera placed (CamDirector_ModeNormal) */
void camdir_update(CamDirector *d);
/* start over on the current setup (CamDirector_Restart) */
void camdir_restart(CamDirector *d);
/* the setup changed since it was taken (CamDirector_SetupChanged) */
int camdir_setup_changed(const CamDirector *d);
/* the engine's camera made the director's (its view angle is vertical, the game's across a 4:3
 * picture) */
void camdir_apply(const CamDirector *d, Camera *c);

#endif
