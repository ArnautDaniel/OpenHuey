\ story/rooms/off-map-e4.fs - the event scripts of room off-map-e4 ($E4; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e4
USING: room-names story.words story.shared ;

\ (as Room0F_Cmd00) the room object RoomE4_ObjectNames by event variable 2
: off-map-e4.cmd00 ( b0 -- )  drop s" off-map-e4.cmd00" stub-step ;
\ room 0xE4: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e4.cmd01 ( -- )  s" off-map-e4.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e4.cond00? ( -- flag )  s" off-map-e4.cond00?" stub-flag ;

: off-map-e4.enter ( -- )   \ 00449250
    room-sounds
    0 0 $14 door-bits
    1 0 object-show
    2 0 object-show
    3 0 object-show
    4 1 object-show
    5 1 object-show
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 effect-86
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    0 0 var-set
    1 0 var-set
    1 $2300 sound-volume
    0 off-map-e4.cmd00
;

: off-map-e4.char-enter ( -- )   \ 004492C0
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
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
;

: off-map-e4.phase1 ( -- )   \ 00449380
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    1 5 char-left-area? 1 6 char-entered-area? or if
        3 exit-prepare
    then
    1 7 char-left-area? 1 8 char-entered-area? or if
        1 exit-prepare
    then
    1 9 char-left-area? if
        2 exit-prepare
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -17.0 22.0 -9.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -17.0 22.0 -9.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -17.0 22.0 -9.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -17.0 22.0 -9.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    2 0 var? if
        1 $D char-in-area? if
            2 1 var-set
            1 0 6 char-sound
        else 0 $D char-in-area? if
            2 1 var-set
            0 0 6 char-sound
        else $FE $D char-in-area? if
            2 1 var-set
            $FE 0 6 char-sound
        then then then
    else 1 $D char-in-area? not 0 $D char-in-area? not and $FE $D char-in-area? not and if
        2 0 var-set
    then then
    1 off-map-e4.cmd00
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
;

: off-map-e4.phase3 ( -- )   \ 004494F0
    off-map-e4.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-e4.cmd01
    then then
;

: off-map-e4.phase2 ( -- )   \ 0047B0E8
;

: off-map-e4.act00 ( -- )   \ 0047B0EC
    self-idle-or-end
;

\ ---- registered ----
' off-map-e4.enter off-map-e4 0 room-script!
' off-map-e4.char-enter off-map-e4 6 room-script!
' off-map-e4.phase1 off-map-e4 1 room-script!
' off-map-e4.phase3 off-map-e4 3 room-script!
' off-map-e4.phase2 off-map-e4 2 room-script!
' off-map-e4.act00 off-map-e4 $00 action-script!
