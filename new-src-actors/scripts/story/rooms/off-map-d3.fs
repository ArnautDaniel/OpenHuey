\ story/rooms/off-map-d3.fs - the event scripts of room off-map-d3 ($D3; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d3
USING: room-names story.words story.shared story.clock ;

\ room 0xD3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d3.cmd00 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-d3.cond00? ( -- flag )  true ;

: off-map-d3.char-enter ( -- )   \ 00446F30
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 2 player-camera
    then then
    0 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        3 3 player-camera
    then then
    3 3 3 area-camera
;

: off-map-d3.phase1 ( -- )   \ 00447030
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    8 2 2 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 4 4 1 chars-area-camera
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
