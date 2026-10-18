\ story/rooms/off-map-d7.fs - the event scripts of room off-map-d7 ($D7; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d7
USING: room-names story.words story.shared story.clock story-names ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant figure-scene-on             \ (bit 0) the figure's scene begun
1 constant cond-was-off                \ (bit 1) the room's condition was false last frame: its hook waits a frame once it holds
10 constant figure-cry-sound           \ (snd 10) the figure's cry as it appears

\ as Room107_Cmd00
: off-map-d7.cmd00 ( b0 -- )  2e room-nudge ;   \ (as chaos-forest-9's)
\ room 0xD7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d7.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ as Room107_Cmd01
: off-map-d7.cmd02 ( -- )  $C $80808080  -225e -63.62e -1125e  1400e 0.5e 0.1e  effects:caustic drop ;   \ (as chaos-forest-9's: light rippling far off)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d7.cond01? ( -- flag )  true ;

: off-map-d7.enter ( -- )   \ 00447A70
    room-sounds
    $19 1.0 0 bgm
    x-d7-flag1 story-flag? not if
        $E 5 $FF char-load
        0 1 track 0 action
    then
    off-map-d7.cmd02
;

: off-map-d7.char-enter ( -- )   \ 00447A90
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
;

: off-map-d7.phase1 ( -- )   \ 00447AD0
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    figure-scene-on ebit? x-d7-flag1 story-flag? not and if
        off-map-d7.cond01? if
            x-d7-flag1 story-flag-set
            1 track action-end
            0 1 track 1 action
        then
    then
;

: off-map-d7.phase3 ( -- )   \ 00447B00
    off-map-d7.cond01? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-d7.cmd01
    then then
;

: off-map-d7.phase2 ( -- )   \ 0047B07C
;

: off-map-d7.phase5 ( -- )   \ 0047B080
    $E action-end
    $E char-done
;

: off-map-d7.act00 ( -- )   \ 00447B20
    5 char-start
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -339.86 125.95 -391.22 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: off-map-d7.act01 ( -- )   \ 00447B50
    $E figure-cry-sound 6 char-sound
    $E $9002 1 0 char-anim-hold
    8 wait-frames
    0 off-map-d7.cmd00
    1 off-map-d7.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

\ ---- registered ----
off-map-d7 register-scripts   \ (its phases and actions, by their names)
