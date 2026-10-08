\ story/rooms/house-of-truth-b1-9.fs - the event scripts of room house-of-truth-b1-9 ($97; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-9
USING: room-names story.words story.shared flag-names ;

\ room 0x97: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-9.cmd00 ( -- )  s" house-of-truth-b1-9.cmd00" stub-step ;

: house-of-truth-b1-9.enter ( -- )   \ 00437D60
    1 0 8 nav-group
    1 2 $5000000 nav-group
    $FE $F3 1 door-lock-for
    1 $F3 1 door-lock-for
;

: house-of-truth-b1-9.char-enter ( -- )   \ 00437D80
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
;

: house-of-truth-b1-9.phase1 ( -- )   \ 00437DC0
    0 ebit? not if
        0 0 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 4 char-entered-area? if
        $87 room-preload
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    house-of-truth-b1-9.cmd00
;

: house-of-truth-b1-9.phase2 ( -- )   \ 0047AE74
;

: house-of-truth-b1-9.act00 ( -- )   \ 00437E10
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $FE action-end
    1 summon-take
    0 room-frames-set
    $31 1.0 0 bgm
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-b1-9.enter house-of-truth-b1-9 0 room-script!
' house-of-truth-b1-9.char-enter house-of-truth-b1-9 6 room-script!
' house-of-truth-b1-9.phase1 house-of-truth-b1-9 1 room-script!
' house-of-truth-b1-9.phase2 house-of-truth-b1-9 2 room-script!
' house-of-truth-b1-9.act00 house-of-truth-b1-9 $00 action-script!
