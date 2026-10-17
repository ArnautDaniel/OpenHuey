\ story/rooms/off-map-d8.fs - the event scripts of room off-map-d8 ($D8; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d8
USING: room-names story.words story.shared story.clock story-names ;

\ ---- this room's own event bits and script variables ----
0 constant figure-scene-on             \ (bit 0) the figure's scene begun (act00)
1 constant cond-was-off                \ (bit 1) the room's condition was false last frame: its hook waits a frame once it holds

\ as Room109_Cmd01
: off-map-d8.cmd00 ( b0 -- )  1e room-nudge ;   \ (as chaos-forest-11's)
\ room 0xD8: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d8.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ (as Room10A_Cond00) last frame's noise requests (gProgress +0x10D4) of kind 0xD8 / 0xD7 and
\ loudness 0x20 or more
: off-map-d8.cond00? ( -- flag )  loud-here? ;
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d8.cond01? ( -- flag )  true ;

: off-map-d8.enter ( -- )   \ 00447BD0
    room-sounds
    $19 1.0 0 bgm
    x-d8-flag1 story-flag? not if
        $E 5 $FF char-load
        0 1 track 0 action
    then
;

: off-map-d8.char-enter ( -- )   \ 00447BF0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
;

: off-map-d8.phase1 ( -- )   \ 00447CF0
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    hewie-who 4 char-entered-area? if
        0 exit-prepare
    then
    hewie-who 5 char-entered-area? hewie-who 6 char-entered-area? or hewie-who 8 char-entered-area? or if
        1 exit-prepare
    then
    hewie-who 7 char-entered-area? if
        2 exit-prepare
    then
    hewie-who 9 char-entered-area? if
        3 exit-prepare
    then
    figure-scene-on ebit? x-d8-flag1 story-flag? not and if
        off-map-d8.cond00? if
            x-d8-flag1 story-flag-set
            1 track action-end
            0 1 track 1 action
        then
    then
;

: off-map-d8.phase3 ( -- )   \ 00447DA0
    off-map-d8.cond01? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-d8.cmd01
    then then
;

: off-map-d8.phase2 ( -- )   \ 0047B090
;

: off-map-d8.phase5 ( -- )   \ 0047B098
    $E action-end
    $E char-done
;

: off-map-d8.act00 ( -- )   \ 00447DC0
    5 char-start
    figure-scene-on ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -25.61 38.13 -136.96 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: off-map-d8.act01 ( -- )   \ 00447E00
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    8 wait-frames
    0 off-map-d8.cmd00
    1 off-map-d8.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

\ ---- registered ----
off-map-d8 register-scripts   \ (its phases and actions, by their names)
