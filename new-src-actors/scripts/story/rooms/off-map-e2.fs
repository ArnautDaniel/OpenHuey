\ story/rooms/off-map-e2.fs - the event scripts of room off-map-e2 ($E2; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e2
USING: room-names story.words story.shared ;

\ room 0xE2: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e2.cmd00 ( -- )  s" off-map-e2.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e2.cond00? ( -- flag )  s" off-map-e2.cond00?" stub-flag ;

: off-map-e2.enter ( -- )   \ 004488B0
    0 1.2 15.55 -24.813 0 effect-86
    1 -1.26 15.55 -24.365 0 effect-86
    2 4.387 15.55 -6.264 0 effect-86
    3 2.327 15.55 -4.824 0 effect-86
    4 -3.559 15.55 -5.985 0 effect-86
    5 -1.887 15.55 -0.304 0 effect-86
    6 1.651 15.55 3.417 0 effect-86
    7 2.473 15.55 5.786 0 effect-86
    8 -2.85 15.55 13.116 0 effect-86
    9 -0.431 15.55 13.794 0 effect-86
    $A 3.347 15.55 19.167 0 effect-86
    $B 1.908 15.55 24.905 0 effect-86
    $C -0.597 15.55 25.104 0 effect-86
    1 0 $14 door-bits
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
;

: off-map-e2.char-enter ( -- )   \ 00448990
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
;

: off-map-e2.phase1 ( -- )   \ 00448A10
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 0 0 1 chars-area-camera
    3 1 1 -1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    1 2 char-entered-area? if
        0 exit-prepare
    then
    1 3 char-left-area? if
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
    3 3 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    4 4 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    5 5 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 5 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 5 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 5 char-effect-moving
    then
    6 6 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 6 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 6 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 6 char-effect-moving
    then
    7 7 8 -4 0 zone-at-effect
    0 7 3 char-zone-bits? 0 7 3 char-zone-bits-before? not and if
        0 7 char-effect-moving
    then
    1 7 3 char-zone-bits? 1 7 3 char-zone-bits-before? not and if
        1 7 char-effect-moving
    then
    $FE 7 3 char-zone-bits? $FE 7 3 char-zone-bits-before? not and if
        $FE 7 char-effect-moving
    then
    8 8 8 -4 0 zone-at-effect
    0 8 3 char-zone-bits? 0 8 3 char-zone-bits-before? not and if
        0 8 char-effect-moving
    then
    1 8 3 char-zone-bits? 1 8 3 char-zone-bits-before? not and if
        1 8 char-effect-moving
    then
    $FE 8 3 char-zone-bits? $FE 8 3 char-zone-bits-before? not and if
        $FE 8 char-effect-moving
    then
    9 9 8 -4 0 zone-at-effect
    0 9 3 char-zone-bits? 0 9 3 char-zone-bits-before? not and if
        0 9 char-effect-moving
    then
    1 9 3 char-zone-bits? 1 9 3 char-zone-bits-before? not and if
        1 9 char-effect-moving
    then
    $FE 9 3 char-zone-bits? $FE 9 3 char-zone-bits-before? not and if
        $FE 9 char-effect-moving
    then
    $A $A 8 -4 0 zone-at-effect
    0 $A 3 char-zone-bits? 0 $A 3 char-zone-bits-before? not and if
        0 $A char-effect-moving
    then
    1 $A 3 char-zone-bits? 1 $A 3 char-zone-bits-before? not and if
        1 $A char-effect-moving
    then
    $FE $A 3 char-zone-bits? $FE $A 3 char-zone-bits-before? not and if
        $FE $A char-effect-moving
    then
    $B $B 8 -4 0 zone-at-effect
    0 $B 3 char-zone-bits? 0 $B 3 char-zone-bits-before? not and if
        0 $B char-effect-moving
    then
    1 $B 3 char-zone-bits? 1 $B 3 char-zone-bits-before? not and if
        1 $B char-effect-moving
    then
    $FE $B 3 char-zone-bits? $FE $B 3 char-zone-bits-before? not and if
        $FE $B char-effect-moving
    then
    $C $C 8 -4 0 zone-at-effect
    0 $C 3 char-zone-bits? 0 $C 3 char-zone-bits-before? not and if
        0 $C char-effect-moving
    then
    1 $C 3 char-zone-bits? 1 $C 3 char-zone-bits-before? not and if
        1 $C char-effect-moving
    then
    $FE $C 3 char-zone-bits? $FE $C 3 char-zone-bits-before? not and if
        $FE $C char-effect-moving
    then
;

: off-map-e2.phase3 ( -- )   \ 00448CD0
    off-map-e2.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-e2.cmd00
    then then
;

: off-map-e2.phase2 ( -- )   \ 0047B0D4
;

: off-map-e2.act00 ( -- )   \ 0047B0D8
    self-idle-or-end
;

\ ---- registered ----
' off-map-e2.enter off-map-e2 0 room-script!
' off-map-e2.char-enter off-map-e2 6 room-script!
' off-map-e2.phase1 off-map-e2 1 room-script!
' off-map-e2.phase3 off-map-e2 3 room-script!
' off-map-e2.phase2 off-map-e2 2 room-script!
' off-map-e2.act00 off-map-e2 $00 action-script!
