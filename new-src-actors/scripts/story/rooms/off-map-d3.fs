\ story/rooms/off-map-d3.fs - the event scripts of room off-map-d3 ($D3; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d3
USING: room-names story.words story.shared ;

\ room 0xD3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d3.cmd00 ( -- )  s" off-map-d3.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-d3.cond00? ( -- flag )  s" off-map-d3.cond00?" stub-flag ;

: off-map-d3.char-enter ( -- )   \ 00446F30
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    3 3 3 area-camera
;

: off-map-d3.phase1 ( -- )   \ 00447030
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
    8 2 2 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 5 char-entered-area? if
        1 exit-prepare
    then
    1 6 char-entered-area? if
        2 exit-prepare
    then
    1 7 char-entered-area? if
        3 exit-prepare
    then
;

: off-map-d3.phase3 ( -- )   \ 004470D0
    off-map-d3.cond00? not if
        0 ebit-set
    else 0 ebit? if
        0 ebit-clear
    else
        off-map-d3.cmd00
    then then
;

: off-map-d3.enter ( -- )   \ 0047B058
    $19 1.0 0 bgm
;

: off-map-d3.phase2 ( -- )   \ 0047B060
;

: off-map-d3.act00 ( -- )   \ 0047B064
    self-idle-or-end
;

\ ---- registered ----
' off-map-d3.char-enter off-map-d3 6 room-script!
' off-map-d3.phase1 off-map-d3 1 room-script!
' off-map-d3.phase3 off-map-d3 3 room-script!
' off-map-d3.enter off-map-d3 0 room-script!
' off-map-d3.phase2 off-map-d3 2 room-script!
' off-map-d3.act00 off-map-d3 $00 action-script!
