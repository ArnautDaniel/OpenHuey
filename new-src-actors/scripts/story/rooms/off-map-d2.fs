\ story/rooms/off-map-d2.fs - the event scripts of room off-map-d2 ($D2; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d2
USING: room-names story.words story.shared story.clock ;

\ ---- this room's own event bits and script variables ----
0 constant cond-was-off                \ (bit 0) the room's condition was false last frame: its hook waits a frame once it holds

\ room 0xD2: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d2.cmd00 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d2.cond00? ( -- flag )  true ;

: off-map-d2.char-enter ( -- )   \ 00446D60
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
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

: off-map-d2.phase1 ( -- )   \ 00446E60
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
    hewie-who 4 char-entered-area? if
        0 exit-prepare
    then
    hewie-who 5 char-entered-area? if
        1 exit-prepare
    then
    hewie-who 6 char-entered-area? if
        2 exit-prepare
    then
    hewie-who 7 char-entered-area? if
        3 exit-prepare
    then
;

: off-map-d2.phase3 ( -- )   \ 00446EE0
    off-map-d2.cond00? not if
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
    else
        off-map-d2.cmd00
    then then
;

: off-map-d2.enter ( -- )   \ 0047B054
;

\ ---- registered ----
off-map-d2 register-scripts   \ (its phases and actions, by their names)
