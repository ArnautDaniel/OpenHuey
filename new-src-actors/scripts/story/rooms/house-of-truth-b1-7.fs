\ story/rooms/house-of-truth-b1-7.fs - the event scripts of room house-of-truth-b1-7 ($95; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-7
USING: room-names story.words story.shared flag-names ;

\ room 0x95: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-7.cmd00 ( -- )  s" house-of-truth-b1-7.cmd00" stub-step ;

: house-of-truth-b1-7.enter ( -- )   \ 004444E0
    $347 story-flag-set
    $354 story-flag? if
        $FE $95 124 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $2E5 story-flag? not if
        0 -119.22 1.0 -96.82 flicker-sprite
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    $FE $95 1 room-doors-state
    1 $95 1 room-doors-state
;

: house-of-truth-b1-7.char-enter ( -- )   \ 00444530
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
            $95 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: house-of-truth-b1-7.phase1 ( -- )   \ 00444570
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
        $346 story-flag? not if
            $94 room-preload
        else
            $87 room-preload
        then
    then
    0 3 char-entered-area? if
        $348 story-flag? not if
            $96 room-preload
        else
            $87 room-preload
        then
    then
    shared.act98
    house-of-truth-b1-7.cmd00
;

: house-of-truth-b1-7.phase2 ( -- )   \ 004445D0
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $2E5 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: house-of-truth-b1-7.act00 ( -- )   \ 00444600
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act01 ( -- )   \ 00444610
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act02 ( -- )   \ 00444620
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act03 ( -- )   \ 00444630
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    3 message
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-7.act04 ( -- )   \ 00444650
    self-wait-done
    -119.22 -96.82 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E5 story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: house-of-truth-b1-7.phase5 ( -- )   \ 0047AFE8
    scene-locked state-flag-clear
;

\ ---- registered ----
' house-of-truth-b1-7.enter house-of-truth-b1-7 0 room-script!
' house-of-truth-b1-7.char-enter house-of-truth-b1-7 6 room-script!
' house-of-truth-b1-7.phase1 house-of-truth-b1-7 1 room-script!
' house-of-truth-b1-7.phase2 house-of-truth-b1-7 2 room-script!
' house-of-truth-b1-7.act00 house-of-truth-b1-7 $00 action-script!
' house-of-truth-b1-7.act01 house-of-truth-b1-7 $01 action-script!
' house-of-truth-b1-7.act02 house-of-truth-b1-7 $02 action-script!
' house-of-truth-b1-7.act03 house-of-truth-b1-7 $03 action-script!
' house-of-truth-b1-7.act04 house-of-truth-b1-7 $04 action-script!
' house-of-truth-b1-7.phase5 house-of-truth-b1-7 5 room-script!
