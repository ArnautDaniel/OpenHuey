\ story/rooms/off-map-d5.fs - the event scripts of room off-map-d5 ($D5; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d5
USING: room-names story.words story.shared ;

\ (as Room109_Cmd00) character kind 0x1A: byte 3 0 starts Kind26_MoveTo(2, -6, 257); else waits
\ (2) until Kind26_MoveDone says done
: off-map-d5.cmd00 ( b0 -- )  drop s" off-map-d5.cmd00" stub-step ;
\ as Room109_Cmd01
: off-map-d5.cmd01 ( b0 -- )  drop s" off-map-d5.cmd01" stub-step ;
\ room 0xD5: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d5.cmd02 ( -- )  s" off-map-d5.cmd02" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-d5.cond00? ( -- flag )  s" off-map-d5.cond00?" stub-flag ;

: off-map-d5.enter ( -- )   \ 00447300
    room-sounds
    $19 1.0 0 bgm
    $1E chance? if
        $1A 4 $FF char-load
        0 $F1 0 action
    then
    $35B story-flag? not if
        $E 5 $FF char-load
        0 $F2 1 action
    then
;

: off-map-d5.char-enter ( -- )   \ 00447330
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    1 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    3 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    4 2 2 area-camera
;

: off-map-d5.phase1 ( -- )   \ 00447460
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    $A 3 3 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 3 3 1 chars-area-camera
    1 5 char-entered-area? if
        0 exit-prepare
    then
    1 6 char-entered-area? if
        1 exit-prepare
    then
    1 7 char-entered-area? if
        2 exit-prepare
    then
    1 8 char-entered-area? if
        3 exit-prepare
    then
    1 9 char-entered-area? if
        4 exit-prepare
    then
    0 ebit? $35B story-flag? not and if
        0 -24.0 8.31 -132.0 $19 10 0 zone
        0 0 2 char-zone-bits? 1 0 2 char-zone-bits? or if
            $35B story-flag-set
            $F2 action-end
            0 $F2 2 action
        then
    then
;

: off-map-d5.phase5 ( -- )   \ 00447550
    $1A action-end
    $1A char-done
    $E action-end
    $E char-done
;

: off-map-d5.phase3 ( -- )   \ 00447560
    off-map-d5.cond00? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
    else
        off-map-d5.cmd02
    then then
;

: off-map-d5.act00 ( -- )   \ 00447580
    4 char-unload
    $1A char-activate
    $1A 1 3.0 235.0 180 char-to-xz
    $1A $9000 1 0 char-anim-hold
    0 off-map-d5.cmd00
    1 off-map-d5.cmd00
    $1A action-end
    $1A char-done
    self-idle-or-end
;

: off-map-d5.act01 ( -- )   \ 004475B0
    5 char-unload
    0 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -21.17 31.84 -137.07 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: off-map-d5.act02 ( -- )   \ 004475F0
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 off-map-d5.cmd01
    1 off-map-d5.cmd01
    $E action-end
    $E char-done
    self-idle-or-end
;

\ ---- registered ----
' off-map-d5.enter off-map-d5 0 room-script!
' off-map-d5.char-enter off-map-d5 6 room-script!
' off-map-d5.phase1 off-map-d5 1 room-script!
' off-map-d5.phase5 off-map-d5 5 room-script!
' off-map-d5.phase3 off-map-d5 3 room-script!
' off-map-d5.act00 off-map-d5 $00 action-script!
' off-map-d5.act01 off-map-d5 $01 action-script!
' off-map-d5.act02 off-map-d5 $02 action-script!
