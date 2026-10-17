\ story/rooms/off-map-e6.fs - the event scripts of room off-map-e6 ($E6; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e6
USING: room-names story.words story.shared story.clock floor-flags ;

\ the room object named by RoomE6_ObjectNames[0]: +0x24 -25.3, +0x34 0
: off-map-e6.cmd00 ( -- )  s" kousi" object-here? 0= if  exit  then  -25.3e s" kousi" place-y object!  0e s" kousi" $34 object! ;   \ (the lattice down out of the way)
\ room 0xE6: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e6.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e6.cond00? ( -- flag )  true ;

: off-map-e6.enter ( -- )   \ 004498F0
    0 barred-floor floor-on
    off-map-e6.cmd00
    1 1 object-hidden
    0 part-shown
    1 part-hidden
;

: off-map-e6.char-enter ( -- )   \ 00449910
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
;

: off-map-e6.phase1 ( -- )   \ 00449990
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 0 0 1 chars-area-camera
    hewie-who 2 char-left-area? if
        0 exit-prepare
    then
    hewie-who 3 char-entered-area? if
        1 exit-prepare
    then
;

: off-map-e6.phase3 ( -- )   \ 004499E0
    off-map-e6.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-e6.cmd01
    then then
    33.0 23.0 -5.9 45.0 23.0 -5.9 33.0 11.0 -5.9 45.0 11.0 -5.9 lights-doorway
    33.0 11.0 -5.9 45.0 11.0 -5.9 33.0 0.0 -5.9 45.0 0.0 -5.9 lights-doorway
    45.0 23.0 -5.1 33.0 23.0 -5.1 45.0 0.0 -5.1 33.0 0.0 -5.1 lights-doorway
;

: off-map-e6.phase2 ( -- )   \ 0047B100
;

: off-map-e6.act00 ( -- )   \ 0047B104
    self-idle-or-end
;

\ ---- registered ----
off-map-e6 register-scripts   \ (its phases and actions, by their names)
