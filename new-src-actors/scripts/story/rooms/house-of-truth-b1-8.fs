\ story/rooms/house-of-truth-b1-8.fs - the event scripts of room house-of-truth-b1-8 ($96; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-8
USING: room-names story.words story.shared ;

\ room 0x96: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-8.cmd00 ( -- )  s" house-of-truth-b1-8.cmd00" stub-step ;

: house-of-truth-b1-8.enter ( -- )   \ 004446E0
    $348 story-flag-set
    $354 story-flag? if
        $FE $96 2 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    $FE $96 1 room-doors-state
    1 $96 1 room-doors-state
;

: house-of-truth-b1-8.char-enter ( -- )   \ 00444710
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
            $96 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: house-of-truth-b1-8.phase1 ( -- )   \ 00444750
    1 ebit? not if
        0 0 char-in-area? if
            1 ebit-set
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
        0 1 char-in-area? if
            1 ebit-set
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
        $345 story-flag? $346 story-flag? and $347 story-flag? and $348 story-flag? and if
            0 ebit-set
            $88 room-preload
        else
            0 ebit-clear
            $87 room-preload
        then
    then
    0 3 char-entered-area? if
        $345 story-flag? not if
            $93 room-preload
        else
            $87 room-preload
        then
    then
    shared.act98
    house-of-truth-b1-8.cmd00
;

: house-of-truth-b1-8.phase2 ( -- )   \ 004447C0
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
;

: house-of-truth-b1-8.act00 ( -- )   \ 004447D0
    1 self-scripted
    0 ebit? if
        $FF 1.0 0 bgm
        $27 state-flag-clear
    then
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-8.act01 ( -- )   \ 004447F0
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-8.act02 ( -- )   \ 00444800
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-8.act03 ( -- )   \ 00444810
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    4 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-8.phase5 ( -- )   \ 0047AFF0
    $12 state-flag-clear
;

\ ---- registered ----
' house-of-truth-b1-8.enter house-of-truth-b1-8 0 room-script!
' house-of-truth-b1-8.char-enter house-of-truth-b1-8 6 room-script!
' house-of-truth-b1-8.phase1 house-of-truth-b1-8 1 room-script!
' house-of-truth-b1-8.phase2 house-of-truth-b1-8 2 room-script!
' house-of-truth-b1-8.act00 house-of-truth-b1-8 $00 action-script!
' house-of-truth-b1-8.act01 house-of-truth-b1-8 $01 action-script!
' house-of-truth-b1-8.act02 house-of-truth-b1-8 $02 action-script!
' house-of-truth-b1-8.act03 house-of-truth-b1-8 $03 action-script!
' house-of-truth-b1-8.phase5 house-of-truth-b1-8 5 room-script!
