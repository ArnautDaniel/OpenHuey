\ story/rooms/off-map-d1.fs - the event scripts of room off-map-d1 ($D1; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d1
USING: room-names story.words story.shared story.clock ;

\ ---- this room's own event bits and script variables ----
0 constant cond-was-off                \ (bit 0) the room's condition was false last frame: its hook waits a frame once it holds

\ as Room106_Cmd00
: off-map-d1.cmd00 ( -- )  effects:one-drip ;   \ (as chaos-forest-8's: a drop falling now and then)
\ room 0xD1: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d1.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d1.cond00? ( -- flag )  true ;

: off-map-d1.enter ( -- )   \ 00446C00
    room-sounds
    $19 1.0 0 bgm
    off-map-d1.cmd00
;

: off-map-d1.char-enter ( -- )   \ 00446C10
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
;

: off-map-d1.phase1 ( -- )   \ 00446C90
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    hewie-who 2 char-entered-area? if
        0 exit-prepare
    then
    hewie-who 3 char-entered-area? if
        $1A 4 1 char-load
        $E 5 1 char-load
        1 exit-prepare
    then
    hewie-who 3 char-left-area? if
        4 0 char-remove
        5 0 char-remove
    then
;

: off-map-d1.phase3 ( -- )   \ 00446CF0
    off-map-d1.cond00? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-d1.cmd01
    then then
;

: off-map-d1.phase2 ( -- )   \ 0047B048
;

: off-map-d1.act00 ( -- )   \ 0047B04C
    self-idle-or-end
;

\ ---- registered ----
off-map-d1 register-scripts   \ (its phases and actions, by their names)
