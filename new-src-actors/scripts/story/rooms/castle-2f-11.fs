\ story/rooms/castle-2f-11.fs - the event scripts of room castle-2f-11 ($1F; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-11
USING: room-names story.words story.shared ;

\ two wheels (Room1F_ObjectNames) rocking 4 degrees (+0x18) through their phase +0x30, 6 degrees
\ a step (byte 3 1; 0 reset), the first one's creak (-366, 30, -25) at each turn
: castle-2f-11.cmd00 ( b0 -- )   \ (the two wheels rocking 4 degrees, 6 a step (1; 0 reset), the first creaking at each turn)
    {: op | a n :}
    2 0 do  i room-object to n to a  a n object-here? if
        op 0= if  0e a n $30 object!  0e a n turn-z object!  i 0= if  0 6 0 0 -366e 30e -25e sound  then  then
        op 1 = if
            6e a n $30 object+!
            a n $30 object@ 360e f< 0= if  -360e a n $30 object+!  i 0= if  0 6 0 0 -366e 30e -25e sound  then  then
            a n $30 object@ deg>rad fsin 4e f* deg>rad a n turn-z object!
        then
    then  loop ;

: castle-2f-11.enter ( -- )   \ 003FE300
    room-sounds
    0 castle-2f-11.cmd00
    0 1 track 1 action
    $A -33.7 23.2 -126.0 $C $60 $60 $60 $20 specks
    $C -92.2 23.2 -92.0 $C $60 $60 $60 $20 specks
    $A -118.0 23.2 -48.5 $C $60 $60 $60 $20 specks
    1 $2300 sound-volume
;

: castle-2f-11.char-enter ( -- )   \ 003FE350
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
;

: castle-2f-11.phase1 ( -- )   \ 003FE3D0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    0 -366.29 0.0 -24.86 $23 29 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 29.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-11.phase2 ( -- )   \ 003FE420
    fiona-who 4 char-in-area? fiona-who 30 $32 char-heading? and if
        0 unhurried offer-action
    then
;

: castle-2f-11.act00 ( -- )   \ 003FE430
    self-wait-done
    60 self-turn-angle
    self-wait-done
    0 say   \ "This crest... Why is it slanted like this?"
    self-idle-or-end
;

: castle-2f-11.act01 ( -- )   \ 0047AAA8
    begin
        1 castle-2f-11.cmd00
        yield
    again
;

\ ---- registered ----
castle-2f-11 register-scripts   \ (its phases and actions, by their names)
