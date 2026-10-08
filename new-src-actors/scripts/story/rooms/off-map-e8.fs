\ story/rooms/off-map-e8.fs - the event scripts of room off-map-e8 ($E8; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e8
USING: room-names story.words story.shared ;

\ room 0xE8: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e8.cmd00 ( -- )  s" off-map-e8.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e8.cond00? ( -- flag )  s" off-map-e8.cond00?" stub-flag ;

: off-map-e8.char-enter ( -- )   \ 00449EE0
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
;

: off-map-e8.phase1 ( -- )   \ 00449F60
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 1 1 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 4 4 1 chars-area-camera
    1 4 char-entered-area? if
        1 exit-prepare
    then
    1 5 char-entered-area? if
        2 exit-prepare
    then
;

: off-map-e8.phase3 ( -- )   \ 00449FB0
    off-map-e8.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-e8.cmd00
    then then
    30.0 54.1 -51.5 -45.0 54.1 -51.5 30.0 54.1 -90.0 -45.0 54.1 -90.0 lights-doorway
    70.0 54.1 -23.0 57.0 54.1 -56.0 100.0 54.1 -30.0 80.0 54.1 -65.0 lights-doorway
    77.0 54.1 18.0 70.0 54.1 -23.0 100.0 54.1 30.0 100.0 54.1 -30.0 lights-doorway
    52.0 54.1 48.0 77.0 54.1 18.0 82.0 54.1 65.0 100.0 54.1 30.0 lights-doorway
    -78.0 54.1 -20.0 -77.0 54.1 20.0 -100.0 54.1 -25.0 -100.0 54.1 20.0 lights-doorway
    -60.0 54.1 -50.0 -78.0 54.1 -20.0 -80.0 54.1 -60.0 -100.0 54.1 -25.0 lights-doorway
    65.0 20.0 18.0 75.0 20.0 13.0 65.0 0.0 18.0 75.0 0.0 13.0 lights-doorway
    74.5 20.0 -19.5 68.5 20.0 -24.5 74.5 0.0 -19.5 68.5 0.0 -24.5 lights-doorway
    53.0 20.0 -48.0 53.0 20.0 -60.0 53.0 0.0 -48.0 53.0 0.0 -60.0 lights-doorway
    -55.0 20.0 -62.0 -55.0 20.0 -47.0 -55.0 0.0 -62.0 -55.0 0.0 -47.0 lights-doorway
    -70.0 20.0 -26.0 -78.0 20.0 -22.0 -70.0 0.0 -26.0 -78.0 0.0 -22.0 lights-doorway
    -78.0 20.0 20.0 -69.0 20.0 25.0 -78.0 0.0 20.0 -69.0 0.0 25.0 lights-doorway
    -55.0 20.0 47.0 -55.0 20.0 54.0 -55.0 0.0 47.0 -55.0 0.0 54.0 lights-doorway
    52.0 20.0 57.0 52.0 20.0 47.0 52.0 0.0 57.0 52.0 0.0 47.0 lights-doorway
;

: off-map-e8.enter ( -- )   \ 0047B120
    1 $2300 sound-volume
;

: off-map-e8.phase2 ( -- )   \ 0047B128
;

: off-map-e8.act00 ( -- )   \ 0047B12C
    self-idle-or-end
;

\ ---- registered ----
' off-map-e8.char-enter off-map-e8 6 room-script!
' off-map-e8.phase1 off-map-e8 1 room-script!
' off-map-e8.phase3 off-map-e8 3 room-script!
' off-map-e8.enter off-map-e8 0 room-script!
' off-map-e8.phase2 off-map-e8 2 room-script!
' off-map-e8.act00 off-map-e8 $00 action-script!
