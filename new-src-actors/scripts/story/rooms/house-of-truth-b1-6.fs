\ story/rooms/house-of-truth-b1-6.fs - the event scripts of room house-of-truth-b1-6 ($94; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-6
USING: room-names story.words story.shared ;

\ room 0x94: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-6.cmd00 ( -- )  s" house-of-truth-b1-6.cmd00" stub-step ;

: house-of-truth-b1-6.enter ( -- )   \ 00444390
    $346 story-flag-set
    $354 story-flag? if
        $FE $94 2 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    $FE $94 1 room-doors-state
    1 $94 1 room-doors-state
;

: house-of-truth-b1-6.char-enter ( -- )   \ 004443C0
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
            $94 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: house-of-truth-b1-6.phase1 ( -- )   \ 00444400
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
        $348 story-flag? not if
            $96 room-preload
        else
            $87 room-preload
        then
    then
    0 3 char-entered-area? if
        $347 story-flag? not if
            $95 room-preload
        else
            $87 room-preload
        then
    then
    shared.act98
    house-of-truth-b1-6.cmd00
;

: house-of-truth-b1-6.phase2 ( -- )   \ 00444460
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: house-of-truth-b1-6.act00 ( -- )   \ 00444470
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-6.act01 ( -- )   \ 00444480
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-6.act02 ( -- )   \ 00444490
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-6.act03 ( -- )   \ 004444A0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    2 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-6.phase5 ( -- )   \ 0047AFE0
    $12 state-flag-clear
;

\ ---- registered ----
' house-of-truth-b1-6.enter house-of-truth-b1-6 0 room-script!
' house-of-truth-b1-6.char-enter house-of-truth-b1-6 6 room-script!
' house-of-truth-b1-6.phase1 house-of-truth-b1-6 1 room-script!
' house-of-truth-b1-6.phase2 house-of-truth-b1-6 2 room-script!
' house-of-truth-b1-6.act00 house-of-truth-b1-6 $00 action-script!
' house-of-truth-b1-6.act01 house-of-truth-b1-6 $01 action-script!
' house-of-truth-b1-6.act02 house-of-truth-b1-6 $02 action-script!
' house-of-truth-b1-6.act03 house-of-truth-b1-6 $03 action-script!
' house-of-truth-b1-6.phase5 house-of-truth-b1-6 5 room-script!
