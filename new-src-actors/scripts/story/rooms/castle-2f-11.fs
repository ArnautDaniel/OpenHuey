\ story/rooms/castle-2f-11.fs - the event scripts of room castle-2f-11 ($1F; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-11
USING: room-names story.words story.shared ;

\ two wheels (Room1F_ObjectNames) rocking 4 degrees (+0x18) through their phase +0x30, 6 degrees
\ a step (byte 3 1; 0 reset), the first one's creak (-366, 30, -25) at each turn
: castle-2f-11.cmd00 ( b0 -- )  drop s" castle-2f-11.cmd00" stub-step ;

: castle-2f-11.enter ( -- )   \ 003FE300
    room-sounds
    0 castle-2f-11.cmd00
    0 $F1 1 action
    $A -33.7 23.2 -126.0 $C $60 $60 $60 $20 specks
    $C -92.2 23.2 -92.0 $C $60 $60 $60 $20 specks
    $A -118.0 23.2 -48.5 $C $60 $60 $60 $20 specks
    1 $2300 sound-volume
;

: castle-2f-11.char-enter ( -- )   \ 003FE350
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
;

: castle-2f-11.phase1 ( -- )   \ 003FE3D0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 -366.29 0.0 -24.86 $23 29 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 29.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-11.phase2 ( -- )   \ 003FE420
    0 4 char-in-area? 0 30 $32 char-heading? and if
        5 0 0 scene-change
    then
;

: castle-2f-11.act00 ( -- )   \ 003FE430
    self-wait-done
    60 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: castle-2f-11.act01 ( -- )   \ 0047AAA8
    begin
        1 castle-2f-11.cmd00
        yield
    again
;

\ ---- registered ----
' castle-2f-11.enter castle-2f-11 0 room-script!
' castle-2f-11.char-enter castle-2f-11 6 room-script!
' castle-2f-11.phase1 castle-2f-11 1 room-script!
' castle-2f-11.phase2 castle-2f-11 2 room-script!
' castle-2f-11.act00 castle-2f-11 $00 action-script!
' castle-2f-11.act01 castle-2f-11 $01 action-script!
