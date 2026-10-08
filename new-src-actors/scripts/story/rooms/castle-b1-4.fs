\ story/rooms/castle-b1-4.fs - the event scripts of room castle-b1-4 ($7; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-4
USING: room-names story.words story.shared ;

: castle-b1-4.enter ( -- )   \ 003F21E0
    room-sounds
    0 0 var-set
    1 0 var-set
;

: castle-b1-4.char-enter ( -- )   \ 003F21F0
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
    0 self-is? if
        1 exit-taken? if
            2 map-page
        then
    then
    0 -11.28 26.25 0.0 0 effect-86
    1 0.0 35.2 11.3 0 effect-86
    2 11.3 59.2 0.0 0 effect-86
    3 0.0 83.2 -11.3 0 effect-86
    4 -11.28 105.2 0.0 0 effect-86
    1 $3FFF sound-volume
;

: castle-b1-4.phase1 ( -- )   \ 003F22D0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        1 exit-prepare
    then
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 map-page
    then
    0 3 char-left-area? if
        2 map-page
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    4 4 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -34.0 102.0 100.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -34.0 102.0 100.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -34.0 102.0 100.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -34.0 102.0 100.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: castle-b1-4.phase2 ( -- )   \ 003F24B0
    0 6 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 7 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
;

: castle-b1-4.act00 ( -- )   \ 003F24D0
    self-wait-done
    35.0 10.0 self-turn-to-xz
    self-wait-done
    $1D01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-4.enter castle-b1-4 0 room-script!
' castle-b1-4.char-enter castle-b1-4 6 room-script!
' castle-b1-4.phase1 castle-b1-4 1 room-script!
' castle-b1-4.phase2 castle-b1-4 2 room-script!
' castle-b1-4.act00 castle-b1-4 $00 action-script!
