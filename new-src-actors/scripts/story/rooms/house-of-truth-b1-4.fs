\ story/rooms/house-of-truth-b1-4.fs - the event scripts of room house-of-truth-b1-4 ($87; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-4
USING: room-names story.words story.shared flag-names ;

\ room 0x87: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-4.cmd00 ( -- )  s" house-of-truth-b1-4.cmd00" stub-step ;

: house-of-truth-b1-4.enter ( -- )   \ 00432B10
    $350 story-flag? not if
        room-sounds
        keep-room-sounds state-flag-set
    then
    $345 story-flag-clear
    $346 story-flag-clear
    $347 story-flag-clear
    $348 story-flag-clear
    $F1 door-open-clear
    $F2 door-open-clear
    $104 door-open-clear
    $105 door-open-clear
    $106 door-open-clear
    $107 door-open-clear
    $108 door-open-clear
    $109 door-open-clear
    $10A door-open-clear
    $10B door-open-clear
    doors-room-in
    $354 story-flag? if
        $FE $87 2 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    $FE $87 1 room-doors-state
    1 $87 1 room-doors-state
;

: house-of-truth-b1-4.char-enter ( -- )   \ 00432B80
    0 self-is? if
        $80 exit-taken? if
            0 $129 129.0 0.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $87 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: house-of-truth-b1-4.phase1 ( -- )   \ 00432BC0
    0 ebit? not if
        0 0 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            0 ebit-set
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        $93 room-preload
    then
    0 3 char-entered-area? if
        $94 room-preload
    then
    shared.act98
    house-of-truth-b1-4.cmd00
;

: house-of-truth-b1-4.phase2 ( -- )   \ 00432C10
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: house-of-truth-b1-4.act00 ( -- )   \ 00432C20
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-4.act01 ( -- )   \ 00432C30
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-4.act02 ( -- )   \ 00432C40
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    $350 story-flag? not if
        $350 story-flag-set
    else
        $254 item-give
    then
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-4.act03 ( -- )   \ 00432C60
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-4.phase5 ( -- )   \ 0047AE38
    scene-locked state-flag-clear
;

\ ---- registered ----
' house-of-truth-b1-4.enter house-of-truth-b1-4 0 room-script!
' house-of-truth-b1-4.char-enter house-of-truth-b1-4 6 room-script!
' house-of-truth-b1-4.phase1 house-of-truth-b1-4 1 room-script!
' house-of-truth-b1-4.phase2 house-of-truth-b1-4 2 room-script!
' house-of-truth-b1-4.act00 house-of-truth-b1-4 $00 action-script!
' house-of-truth-b1-4.act01 house-of-truth-b1-4 $01 action-script!
' house-of-truth-b1-4.act02 house-of-truth-b1-4 $02 action-script!
' house-of-truth-b1-4.act03 house-of-truth-b1-4 $03 action-script!
' house-of-truth-b1-4.phase5 house-of-truth-b1-4 5 room-script!
