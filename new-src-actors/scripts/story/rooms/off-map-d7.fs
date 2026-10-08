\ story/rooms/off-map-d7.fs - the event scripts of room off-map-d7 ($D7; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d7
USING: room-names story.words story.shared ;

\ as Room107_Cmd00
: off-map-d7.cmd00 ( b0 -- )  drop s" off-map-d7.cmd00" stub-step ;
\ room 0xD7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d7.cmd01 ( -- )  s" off-map-d7.cmd01" stub-step ;
\ as Room107_Cmd01
: off-map-d7.cmd02 ( -- )  s" off-map-d7.cmd02" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-d7.cond01? ( -- flag )  s" off-map-d7.cond01?" stub-flag ;

: off-map-d7.enter ( -- )   \ 00447A70
    room-sounds
    $19 1.0 0 bgm
    $35E story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
    off-map-d7.cmd02
;

: off-map-d7.char-enter ( -- )   \ 00447A90
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
;

: off-map-d7.phase1 ( -- )   \ 00447AD0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    0 ebit? $35E story-flag? not and if
        off-map-d7.cond01? if
            $35E story-flag-set
            $F1 action-end
            0 $F1 1 action
        then
    then
;

: off-map-d7.phase3 ( -- )   \ 00447B00
    off-map-d7.cond01? not if
        1 ebit-set
    else 1 ebit? if
        1 ebit-clear
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
    5 char-unload
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
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 off-map-d7.cmd00
    1 off-map-d7.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

\ ---- registered ----
' off-map-d7.enter off-map-d7 0 room-script!
' off-map-d7.char-enter off-map-d7 6 room-script!
' off-map-d7.phase1 off-map-d7 1 room-script!
' off-map-d7.phase3 off-map-d7 3 room-script!
' off-map-d7.phase2 off-map-d7 2 room-script!
' off-map-d7.phase5 off-map-d7 5 room-script!
' off-map-d7.act00 off-map-d7 $00 action-script!
' off-map-d7.act01 off-map-d7 $01 action-script!
