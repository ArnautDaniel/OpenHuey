\ story/rooms/off-map-d1.fs - the event scripts of room off-map-d1 ($D1; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d1
USING: room-names story.words story.shared ;

\ as Room106_Cmd00
: off-map-d1.cmd00 ( -- )  s" off-map-d1.cmd00" stub-step ;
\ room 0xD1: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d1.cmd01 ( -- )  s" off-map-d1.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-d1.cond00? ( -- flag )  s" off-map-d1.cond00?" stub-flag ;

: off-map-d1.enter ( -- )   \ 00446C00
    room-sounds
    $19 1.0 0 bgm
    off-map-d1.cmd00
;

: off-map-d1.char-enter ( -- )   \ 00446C10
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

: off-map-d1.phase1 ( -- )   \ 00446C90
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
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    1 2 char-entered-area? if
        0 exit-prepare
    then
    1 3 char-entered-area? if
        $1A 4 1 char-load
        $E 5 1 char-load
        1 exit-prepare
    then
    1 3 char-left-area? if
        4 0 char-remove
        5 0 char-remove
    then
;

: off-map-d1.phase3 ( -- )   \ 00446CF0
    off-map-d1.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
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
' off-map-d1.enter off-map-d1 0 room-script!
' off-map-d1.char-enter off-map-d1 6 room-script!
' off-map-d1.phase1 off-map-d1 1 room-script!
' off-map-d1.phase3 off-map-d1 3 room-script!
' off-map-d1.phase2 off-map-d1 2 room-script!
' off-map-d1.act00 off-map-d1 $00 action-script!
