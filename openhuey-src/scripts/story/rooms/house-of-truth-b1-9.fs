\ story/rooms/house-of-truth-b1-9.fs - the event scripts of room house-of-truth-b1-9 ($97; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-9
USING: room-names facts story.words story.shared flag-names floor-flags ;

\ ---- this room's own event bits and script variables ----
0 constant area0-reacted               \ (bit 0) her reaction in area 0 made (once a visit)

\ room 0x97: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-9.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-9.enter ( -- )   \ 00437D60
    0 solid-floor floor-on
    2 keeps-hewie-off keeps-stalkers-off or floor-on
    stalker-who $F3 1 door-lock-for
    hewie-who $F3 1 door-lock-for
;

: house-of-truth-b1-9.char-enter ( -- )   \ 00437D80
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 2 player-camera
    then
    1 2 2 area-camera
;

: house-of-truth-b1-9.phase1 ( -- )   \ 00437DC0
    area0-reacted ebit? not if
        fiona-who 0 char-in-area? if
            area0-reacted ebit-set
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        $87 room-preload
    then
    fiona-who 5 char-entered-area? if
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
    stalker-who action-end
    1 summon-take
    0 room-frames-set
    $31 1.0 0 bgm
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-b1-9 register-scripts   \ (its phases and actions, by their names)
