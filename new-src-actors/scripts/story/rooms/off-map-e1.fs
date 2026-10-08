\ story/rooms/off-map-e1.fs - the event scripts of room off-map-e1 ($E1; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e1
USING: room-names story.words story.shared ;

\ room 0xE1: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e1.cmd00 ( -- )  s" off-map-e1.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e1.cond00? ( -- flag )  s" off-map-e1.cond00?" stub-flag ;

: off-map-e1.enter ( -- )   \ 00448590
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 6.0 21.8 -3.2 0 effect-86
    1 29.0 41.8 41.9 0 effect-86
    2 -46.8 93.8 18.5 0 effect-86
    1 $2300 sound-volume
;

: off-map-e1.char-enter ( -- )   \ 004485E0
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
                0 4 3 char-camera
                0 camera-follow
            else
                1 4 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 3 char-camera
            0 camera-follow
        else
            1 4 3 char-camera
            1 camera-follow
        then
    then then
    2 4 3 area-camera
;

: off-map-e1.phase1 ( -- )   \ 00448660
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $D 0 0 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 4 3 1 chars-area-camera
    $17 3 -1 1 chars-area-camera
    1 4 char-left-area? 1 5 char-entered-area? or if
        2 exit-prepare
    then
    1 6 char-left-area? if
        1 exit-prepare
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
;

: off-map-e1.phase3 ( -- )   \ 00448740
    -8.0 82.0 -5.0 -8.0 82.0 45.0 -8.0 30.0 -5.0 -8.0 30.0 45.0 lights-doorway
    -5.3 54.0 -4.1 -5.3 54.0 47.0 -58.0 54.0 -4.1 -58.0 54.0 47.0 lights-doorway
    15.0 30.0 -3.0 45.0 30.0 -3.0 15.0 15.0 -3.0 45.0 15.0 -3.0 lights-doorway
    15.0 15.0 -3.0 45.0 15.0 -3.0 15.0 0.0 -3.0 45.0 0.0 -3.0 lights-doorway
    -11.0 30.0 -3.0 15.0 30.0 -3.0 -11.0 15.0 -3.0 15.0 15.0 -3.0 lights-doorway
    -11.0 15.0 -3.0 15.0 15.0 -3.0 -11.0 0.0 -3.0 15.0 0.0 -3.0 lights-doorway
    off-map-e1.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-e1.cmd00
    then then
;

: off-map-e1.phase2 ( -- )   \ 0047B0C8
;

: off-map-e1.act00 ( -- )   \ 0047B0CC
    self-idle-or-end
;

\ ---- registered ----
' off-map-e1.enter off-map-e1 0 room-script!
' off-map-e1.char-enter off-map-e1 6 room-script!
' off-map-e1.phase1 off-map-e1 1 room-script!
' off-map-e1.phase3 off-map-e1 3 room-script!
' off-map-e1.phase2 off-map-e1 2 room-script!
' off-map-e1.act00 off-map-e1 $00 action-script!
