\ story/rooms/off-map-e2.fs - the event scripts of room off-map-e2 ($E2; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e2
USING: room-names story.words story.shared story.clock floor-flags ;

\ room 0xE2: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e2.cmd00 ( -- )  clock-draw ;   \ (each frame: the clock)
\ the progress object's +0x7C (ProgressBase_Slot7C: always 1)
: off-map-e2.cond00? ( -- flag )  true ;

: off-map-e2.enter ( -- )   \ 004488B0
    0 1.2 15.55 -24.813 0 flame-in
    1 -1.26 15.55 -24.365 0 flame-in
    2 4.387 15.55 -6.264 0 flame-in
    3 2.327 15.55 -4.824 0 flame-in
    4 -3.559 15.55 -5.985 0 flame-in
    5 -1.887 15.55 -0.304 0 flame-in
    6 1.651 15.55 3.417 0 flame-in
    7 2.473 15.55 5.786 0 flame-in
    8 -2.85 15.55 13.116 0 flame-in
    9 -0.431 15.55 13.794 0 flame-in
    $A 3.347 15.55 19.167 0 flame-in
    $B 1.908 15.55 24.905 0 flame-in
    $C -0.597 15.55 25.104 0 flame-in
    1 part-hidden
    $2E 1 pvar? if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
;

: off-map-e2.char-enter ( -- )   \ 00448990
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
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
    hewie-who 2 char-entered-area? if
        0 exit-prepare
    then
    hewie-who 3 char-left-area? if
        1 exit-prepare
    then
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
    1 1 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
    4 4 8 -4 0 zone-at-effect
    fiona-who 4 3 char-zone-bits? fiona-who 4 3 char-zone-bits-before? not and if
        fiona-who 4 char-effect-moving
    then
    hewie-who 4 3 char-zone-bits? hewie-who 4 3 char-zone-bits-before? not and if
        hewie-who 4 char-effect-moving
    then
    stalker-who 4 3 char-zone-bits? stalker-who 4 3 char-zone-bits-before? not and if
        stalker-who 4 char-effect-moving
    then
    5 5 8 -4 0 zone-at-effect
    fiona-who 5 3 char-zone-bits? fiona-who 5 3 char-zone-bits-before? not and if
        fiona-who 5 char-effect-moving
    then
    hewie-who 5 3 char-zone-bits? hewie-who 5 3 char-zone-bits-before? not and if
        hewie-who 5 char-effect-moving
    then
    stalker-who 5 3 char-zone-bits? stalker-who 5 3 char-zone-bits-before? not and if
        stalker-who 5 char-effect-moving
    then
    6 6 8 -4 0 zone-at-effect
    fiona-who 6 3 char-zone-bits? fiona-who 6 3 char-zone-bits-before? not and if
        fiona-who 6 char-effect-moving
    then
    hewie-who 6 3 char-zone-bits? hewie-who 6 3 char-zone-bits-before? not and if
        hewie-who 6 char-effect-moving
    then
    stalker-who 6 3 char-zone-bits? stalker-who 6 3 char-zone-bits-before? not and if
        stalker-who 6 char-effect-moving
    then
    7 7 8 -4 0 zone-at-effect
    fiona-who 7 3 char-zone-bits? fiona-who 7 3 char-zone-bits-before? not and if
        fiona-who 7 char-effect-moving
    then
    hewie-who 7 3 char-zone-bits? hewie-who 7 3 char-zone-bits-before? not and if
        hewie-who 7 char-effect-moving
    then
    stalker-who 7 3 char-zone-bits? stalker-who 7 3 char-zone-bits-before? not and if
        stalker-who 7 char-effect-moving
    then
    8 8 8 -4 0 zone-at-effect
    fiona-who 8 3 char-zone-bits? fiona-who 8 3 char-zone-bits-before? not and if
        fiona-who 8 char-effect-moving
    then
    hewie-who 8 3 char-zone-bits? hewie-who 8 3 char-zone-bits-before? not and if
        hewie-who 8 char-effect-moving
    then
    stalker-who 8 3 char-zone-bits? stalker-who 8 3 char-zone-bits-before? not and if
        stalker-who 8 char-effect-moving
    then
    9 9 8 -4 0 zone-at-effect
    fiona-who 9 3 char-zone-bits? fiona-who 9 3 char-zone-bits-before? not and if
        fiona-who 9 char-effect-moving
    then
    hewie-who 9 3 char-zone-bits? hewie-who 9 3 char-zone-bits-before? not and if
        hewie-who 9 char-effect-moving
    then
    stalker-who 9 3 char-zone-bits? stalker-who 9 3 char-zone-bits-before? not and if
        stalker-who 9 char-effect-moving
    then
    $A $A 8 -4 0 zone-at-effect
    fiona-who $A 3 char-zone-bits? fiona-who $A 3 char-zone-bits-before? not and if
        fiona-who $A char-effect-moving
    then
    hewie-who $A 3 char-zone-bits? hewie-who $A 3 char-zone-bits-before? not and if
        hewie-who $A char-effect-moving
    then
    stalker-who $A 3 char-zone-bits? stalker-who $A 3 char-zone-bits-before? not and if
        stalker-who $A char-effect-moving
    then
    $B $B 8 -4 0 zone-at-effect
    fiona-who $B 3 char-zone-bits? fiona-who $B 3 char-zone-bits-before? not and if
        fiona-who $B char-effect-moving
    then
    hewie-who $B 3 char-zone-bits? hewie-who $B 3 char-zone-bits-before? not and if
        hewie-who $B char-effect-moving
    then
    stalker-who $B 3 char-zone-bits? stalker-who $B 3 char-zone-bits-before? not and if
        stalker-who $B char-effect-moving
    then
    $C $C 8 -4 0 zone-at-effect
    fiona-who $C 3 char-zone-bits? fiona-who $C 3 char-zone-bits-before? not and if
        fiona-who $C char-effect-moving
    then
    hewie-who $C 3 char-zone-bits? hewie-who $C 3 char-zone-bits-before? not and if
        hewie-who $C char-effect-moving
    then
    stalker-who $C 3 char-zone-bits? stalker-who $C 3 char-zone-bits-before? not and if
        stalker-who $C char-effect-moving
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
off-map-e2 register-scripts   \ (its phases and actions, by their names)
