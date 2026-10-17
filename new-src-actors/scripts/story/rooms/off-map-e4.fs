\ story/rooms/off-map-e4.fs - the event scripts of room off-map-e4 ($E4; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e4
USING: room-names story.words story.shared story.clock ;

\ (as Room0F_Cmd00) the room object RoomE4_ObjectNames by event variable 2
: off-map-e4.cmd00 ( b0 -- )  s" fumi_yuka" 2 var-fade ;   \ (as castle-1f-4's, by event variable 2)
\ room 0xE4: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e4.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e4.cond00? ( -- flag )  true ;

: off-map-e4.enter ( -- )   \ 00449250
    room-sounds
    0 0 $14 door-bits
    1 0 object-hidden
    2 0 object-hidden
    3 0 object-hidden
    4 1 object-hidden
    5 1 object-hidden
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 flame-in
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
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
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
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    hewie-who 5 char-left-area? hewie-who 6 char-entered-area? or if
        3 exit-prepare
    then
    hewie-who 7 char-left-area? hewie-who 8 char-entered-area? or if
        1 exit-prepare
    then
    hewie-who 9 char-left-area? if
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
        hewie-who $D char-in-area? if
            2 1 var-set
            hewie-who 0 6 char-sound
        else fiona-who $D char-in-area? if
            2 1 var-set
            fiona-who 0 6 char-sound
        else stalker-who $D char-in-area? if
            2 1 var-set
            stalker-who 0 6 char-sound
        then then then
    else hewie-who $D char-in-area? not fiona-who $D char-in-area? not and stalker-who $D char-in-area? not and if
        2 0 var-set
    then then
    1 off-map-e4.cmd00
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
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
