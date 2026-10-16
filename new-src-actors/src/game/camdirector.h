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

#include "../hactor/hactor.h"

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
    int target;         /* who is followed (-1: none) (+0xB0): an HActor id, read from its body */
    int last_target;    /* (+0xB4) */
    Vec3 offset;        /* added to its position (+0xC0) */
    Vec3 point;         /* the point to look at without a target (+0xA0: the set's) */
    float move_rate;    /* how fast it follows along the path (+0x50, 6) */
    /* the setup */
    int set, path_no;           /* wanted (+0x6C, +0x70; path -1: none) */
    int last_set, last_path;    /* taken (+0x78, +0x7C) */
    int scene;          /* a cutscene has the camera (+0xF4, CamDirector_ModeEvent): the director idle */
    int event;          /* the story's own shot of the one followed (+0x69): placed by the camera actor */
    int started;        /* a room has started it (nothing to direct before) */
    int changed;        /* the setup changed this frame (CamDirector_SetupChanged, as the phase-3 scripts see it) */
    /* the view angle (horizontal, across a 4:3 picture, radians) */
    float fov;          /* now (+0x80) */
    float set_fov;      /* the set's (+0x84) */
    float fov_dip;      /* how far it dips after a cut (+0x88) */
    uint8_t cut;        /* frames left of the dip (+0x8C; 0x80: settled) */
    uint32_t ease;      /* frames into easing back (+0x90) */
    /* the camera it drives */
    Vec3 eye, look;
    float shake;        /* the eye moved at random by up to this each way, each frame (Camera +0x4) */
    int shown;          /* the camera set last taken (Camera +0x29C) */
    int shown_now, shown_before;   /* as of the last two frames (Camera +0x90, +0x94): a cut between them */
    /* who it follows and the characters' setups (the progress' +0x1130; Character +0xE8 / +0xEC) */
    int followed;       /* an actor, -1 nobody */
    int player;         /* Fiona: whom it falls back on */
    int setups[HACTOR_MAX][2];
} CamDirector;

extern CamDirector gCamDir;

/* no set, no path, no target (CamDirector_NewRoom: the start of play) */
void camdir_new_room(CamDirector *d);
/* the room's camera sets (section 5) and paths (section 6, NULL: none) taken and the camera put
 * at the current set / path (CamDirector_RoomStart: each room) */
void camdir_room_start(CamDirector *d, const float *sets, int nsets, const int32_t *paths);
/* the set and path to use (CamDirector_SetSetup) */
void camdir_set_setup(CamDirector *d, int set, int path);
/* follow `target` (an HActor id; -1: none) with an offset (CamDirector_Follow) */
void camdir_follow(CamDirector *d, int target, Vec3 offset);
/* a character's camera setup (Progress_CameraSetup: the caller says it is in this room or Fiona) */
void camdir_char_setup(CamDirector *d, int who, int set, int path);
/* every character's setup gone (a room change) */
void camdir_forget_setups(CamDirector *d);
/* whom it follows, checked (Progress_CameraFollow): nobody stays nobody; someone gone, elsewhere
 * or - not Fiona - without a setup: Fiona (and followed) */
int camdir_follow_check(CamDirector *d, int who);
/* follow `who` (-1: nobody) and take its setup (Progress_CameraOn) */
void camdir_camera_on(CamDirector *d, int who);
/* each frame: whom it follows checked, and their setup taken (SceneGame's play frame) */
void camdir_take_followed(CamDirector *d);
/* a cutscene's start of play has the camera (CamDirector_ModeEvent), and its end gives it back
 * (CamDirector_ModeDefault: the setup taken anew) */
void camdir_scene(CamDirector *d, int on);
/* the setup taken anew, as if it had changed (the event's shot over) */
void camdir_retake(CamDirector *d);
/* on to the room's next camera set, no path, following nobody (CamDirector_NextSetup: the game over) */
void camdir_next_set(CamDirector *d);
/* how fast it follows along a path (CamDirector_Set50: below 0 the usual 6) */
void camdir_rate(CamDirector *d, float rate);
/* outside a camera's view volume as the original sees it (a 4:3 picture, 1 to 2000 deep:
 * Camera_OutOfView) */
int camera_out_of_view(const Camera *c, Vec3 p);
/* the shake on the engine's camera, after any cutscene's shot (Camera_ViewMatrix) */
void camdir_shake(const CamDirector *d, Camera *c);
/* each frame: the view angle eases, the camera along its path (CamDirector_Ease) */
void camdir_ease(CamDirector *d);
/* each frame: a new set or path taken (CamDirector_Track) */
void camdir_track(CamDirector *d);
/* each frame: the setup remembered, the camera placed, and the engine's camera made the
 * director's (CamDirector_Update: ModeNormal / Remember, Camera_Update); the event's shot is
 * placed by the camera actor */
void camdir_update(CamDirector *d);
/* the setup remembered as taken (the room start's update) */
void camdir_remember(CamDirector *d);
/* the camera cut to another set at the last update (Fiona's controls: Camera +0x90 / +0x94) */
int camdir_cut(const CamDirector *d);
/* start over on the current setup (CamDirector_Restart) */
void camdir_restart(CamDirector *d);
/* the setup changed since it was taken (CamDirector_SetupChanged) */
int camdir_setup_changed(const CamDirector *d);
/* the engine's camera made the director's (its view angle is vertical, the game's across a 4:3
 * picture) */
void camdir_apply(const CamDirector *d, Camera *c);

#endif
