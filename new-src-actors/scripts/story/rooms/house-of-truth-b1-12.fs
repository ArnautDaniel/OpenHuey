\ story/rooms/house-of-truth-b1-12.fs - the event scripts of room house-of-truth-b1-12 ($9A; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-12
USING: room-names story.words story.shared ;

\ room 0x9A: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-12.cmd00 ( -- )  s" house-of-truth-b1-12.cmd00" stub-step ;

: house-of-truth-b1-12.char-enter ( -- )   \ 00443740
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    3 1 1 area-camera
;

: house-of-truth-b1-12.phase1 ( -- )   \ 004437C0
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
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    house-of-truth-b1-12.cmd00
    $A9 story-flag? $AA story-flag? not and if
        0 $C char-entered-area? if
            $AA story-flag-set
            $FE $9A 1065 2 stalker-to-room
            $FE 0 stalker-mode
            0 $FE 0 action
        then
    then
;

: house-of-truth-b1-12.enter ( -- )   \ 0047AFB0
    1 1 8 nav-group
;

: house-of-truth-b1-12.phase2 ( -- )   \ 0047AFB8
;

: house-of-truth-b1-12.act00 ( -- )   \ 00443830
    1 self-scripted
    $FF 1 char-visible
    self-wait-done
    $FF 0 char-visible
    $FE $429 170 char-to-tri-facing
    $FE 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    0 self-scripted
    $FE 0 stalker-mode
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-b1-12.char-enter house-of-truth-b1-12 6 room-script!
' house-of-truth-b1-12.phase1 house-of-truth-b1-12 1 room-script!
' house-of-truth-b1-12.enter house-of-truth-b1-12 0 room-script!
' house-of-truth-b1-12.phase2 house-of-truth-b1-12 2 room-script!
' house-of-truth-b1-12.act00 house-of-truth-b1-12 $00 action-script!
