\ story/rooms/house-of-truth-b1-3.fs - the event scripts of room house-of-truth-b1-3 ($86; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-3
USING: room-names story.words story.shared ;

\ room 0x86: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-3.cmd00 ( -- )  s" house-of-truth-b1-3.cmd00" stub-step ;

: house-of-truth-b1-3.enter ( -- )   \ 0042C770
    $A7 story-flag? not if
        0 state-flag-clear
        $A7 story-flag-set
        $FE action-end
        1 summon-take
    then
    1 $2300 sound-volume
;

: house-of-truth-b1-3.char-enter ( -- )   \ 0042C790
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
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

: house-of-truth-b1-3.phase1 ( -- )   \ 0042C810
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    house-of-truth-b1-3.cmd00
;

: house-of-truth-b1-3.act00 ( -- )   \ 0047ADB0
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-b1-3.enter house-of-truth-b1-3 0 room-script!
' house-of-truth-b1-3.char-enter house-of-truth-b1-3 6 room-script!
' house-of-truth-b1-3.phase1 house-of-truth-b1-3 1 room-script!
' house-of-truth-b1-3.act00 house-of-truth-b1-3 $00 action-script!
