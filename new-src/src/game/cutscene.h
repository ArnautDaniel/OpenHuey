/* The cutscene director (src/game/director.c): the game's scenes played in the room, timed by a
 * movie. A scene NAME is a folder: NAME.DH (the script: its length in frames, records - one per
 * shot - with the frames they span and the actors / doors / object groups in them), MARK.BIN
 * (signals by frame), PARAMS.BIN (light and effect cues) and the shots CUTnnn.DP (per actor a
 * motion file - animation 0x8000 - the camera's keys, the doors' and groups' keys).
 *
 * The scripts drive it: start (cutscene-start), step it until ready (status 2), let it start,
 * give it the movie's frame each frame and update. Its status: 1 loading, 2 ready, 3 playing,
 * 5 over. Actors are characters by script id (actor i -> id kMap[i]), found through the event
 * state's characters. Not yet: the doors, the object groups, the flash (signal 0) and the
 * rumble. */
#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "camera.h"

int cutscene_start(const char *name);   /* Cutscene_Set38 + Start: 0 if its files aren't there */
void cutscene_run_state(void);          /* the state machine a step (Cutscene_RunState) */
void cutscene_start_when_ready(void);   /* (Cutscene_SetNoEnd: +0x205) */
void cutscene_set_frame(int frame);     /* (Cutscene_PushC) */
void cutscene_update(void);             /* this frame's camera and actors (Cutscene_Update) */
void cutscene_end(void);                /* the actors given back (Cutscene_End) */
int cutscene_status(void);
int cutscene_playing_shot(void);        /* the playing buffer's shot is in (EntryDone) */
int cutscene_frame(void);
int cutscene_length(void);
int cutscene_shot_at(int frame);        /* -1 none */
int cutscene_near_end(void);
int cutscene_signal_count(int bit);     /* signal `bit` since the last frame */
int cutscene_signal_total(int bit);     /* its count so far, less one (Cutscene_Get206) */
/* the camera this frame, if the shot keys it: 1 and the engine camera set */
int cutscene_camera(Camera *c);
int cutscene_letterbox(void);           /* bars at the top and bottom */
void cutscene_set_letterbox_off(int off);

#endif
