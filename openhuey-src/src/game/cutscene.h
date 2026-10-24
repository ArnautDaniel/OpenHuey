/* The cutscene director (src/game/director.c): the game's scenes played in the room, timed by a
 * movie. A scene NAME is a folder: NAME.DH (the script: its length in frames, records - one per
 * shot - with the frames they span and the actors / doors / object groups in them), MARK.BIN
 * (signals by frame), PARAMS.BIN (light and effect cues) and the shots CUTnnn.DP (per actor a
 * motion file - animation 0x8000 - the camera's keys, the doors' and groups' keys).
 *
 * The scripts drive it: start (cutscene-start), step it until ready (status 2), let it start,
 * give it the movie's frame each frame and update. Its status: 1 loading, 2 ready, 3 playing,
 * 5 over. Actors are characters by script id (actor i -> id kMap[i]), found through the event
 * state's characters; the room's doors and object groups keyed too. At a cut (signal 0) its
 * characters' hanging parts settle afresh (Fiona_FollowAnim, Pursuer_PlaceModel) and the glow's
 * trails are wiped (the renderer's +0x5C: Renderer_GlowClear). */
#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "camera.h"

int cutscene_start(const char *name);   /* Cutscene_Set38 + Start: 0 if its files aren't there */
void cutscene_run_state(void);          /* the state machine a step (Cutscene_RunState) */
void cutscene_start_when_ready(void);   /* (Cutscene_SetNoEnd: +0x205) */
void cutscene_set_frame(int frame);     /* (Cutscene_PushC) */
void cutscene_next_event(void);         /* to its next event's frame - 0, 1, 2.. from its start (Cutscene_NextEvent) */
void cutscene_update(void);             /* this frame's camera and actors (Cutscene_Update) */
void cutscene_end(void);                /* the actors given back (Cutscene_End) */
/* told, as the actors are given back, where the scene leaves each it moved - at the end of the
 * last shot it is in, played out or skipped (hga's: the original leaves them where the scene was) */
void cutscene_on_place(void (*fn)(int actor, Vec3 pos, float yaw));
/* told, on each frame the scene moves on, where each character in the shot now is (its body moved) */
void cutscene_on_follow(void (*fn)(int actor, Vec3 pos));
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
int cutscene_active(void);              /* playing (cast, not over or ended) */
/* the scene has taken model `actor` (its cast, Cutscene_Cast .. ReleaseActors: the character
 * disabled - +0xE2 - its own frame not run, its model the scene's) */
int cutscene_has(int actor);
int cutscene_letterbox(void);           /* bars at the top and bottom */
void cutscene_set_letterbox_off(int off);
void cutscene_leave_alone(int i);       /* actor i left to its own motion by the cast (Cutscene_Mark82) */
void cutscene_keep_model(int i);        /* actor i's model left as it is when taken (Cutscene_Mark83) */

#endif
