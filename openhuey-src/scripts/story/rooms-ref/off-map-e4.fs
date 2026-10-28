\ story/rooms/off-map-e4.fs - the event scripts of room off-map-e4 ($E4; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e4
USING: room-names story.words story.shared story.clock floor-flags ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant cond-was-off                \ (bit 0) the room's condition was false last frame: its hook waits a frame once it holds
0 constant sound-step-var              \ (var 0) which of its sounds next (0..3)
1 constant sound-frames-var            \ (var 1) frames to the next sound (30)
2 constant called-var                  \ (var 2) Hewie or she in area $D (his call made)
0 constant came-in-sound               \ (snd 0) whoever came into area $D (a call?)
5 constant ambient1-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the first
6 constant ambient2-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the second
7 constant ambient3-sound              \ (snd 7) its ambient sounds in turn (every 30 frames): the third
8 constant ambient4-sound              \ (snd 8) its ambient sounds in turn (every 30 frames): the fourth

\ (as Room0F_Cmd00) the room object RoomE4_ObjectNames by event variable 2
: off-map-e4.cmd00 ( b0 -- )  s" fumi_yuka" 2 var-fade ;   \ (as castle-1f-4's, by event variable 2)
\ room 0xE4: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e4.cmd01 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e4.cond00? ( -- flag )  true ;

: off-map-e4.enter ( -- )   \ 00449250
    room-sounds
    0 part-hidden
    1 0 object-hidden
    2 0 object-hidden
    3 0 object-hidden
    4 1 object-hidden
    5 1 object-hidden
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 flame-in
    1 part-shown
    2 part-hidden
    3 part-shown
    4 part-hidden
    5 part-shown
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    1 $2300 sound-volume
    0 off-map-e4.cmd00
;

: off-map-e4.char-enter ( -- )   \ 004492C0
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
;

: off-map-e4.phase1 ( -- )   \ 00449380
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
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
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 -17.0 22.0 -9.0 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 -17.0 22.0 -9.0 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 -17.0 22.0 -9.0 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 -17.0 22.0 -9.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
    called-var 0 var? if
        hewie-who $D char-in-area? if
            called-var 1 var-set
            hewie-who came-in-sound 6 char-sound
        else fiona-who $D char-in-area? if
            called-var 1 var-set
            fiona-who came-in-sound 6 char-sound
        else stalker-who $D char-in-area? if
            called-var 1 var-set
            stalker-who came-in-sound 6 char-sound
        then then then
    else hewie-who $D char-in-area? not fiona-who $D char-in-area? not and stalker-who $D char-in-area? not and if
        called-var 0 var-set
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
        cond-was-off ebit-set
    else cond-was-off ebit? if
        cond-was-off ebit-clear
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
off-map-e4 register-scripts   \ (its phases and actions, by their names)
