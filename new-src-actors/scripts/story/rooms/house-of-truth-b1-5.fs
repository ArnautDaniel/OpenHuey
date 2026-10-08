\ story/rooms/house-of-truth-b1-5.fs - the event scripts of room house-of-truth-b1-5 ($93; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-5
USING: room-names story.words story.shared flag-names ;

\ Room93_Cmd00
: house-of-truth-b1-5.cmd00 ( -- )  s" house-of-truth-b1-5.cmd00" stub-step ;

: house-of-truth-b1-5.enter ( -- )   \ 00444170
    $345 story-flag-set
    $354 story-flag? if
        $FE $93 124 2 stalker-to-room
        $FE 0 stalker-mode
        $FE 1 1 char-camera
    then
    $2E4 story-flag? not if
        0 -117.05 1.0 101.81 flicker-sprite
    then
    $31 1.0 0 bgm
    1 0 $5000000 nav-group
    $FE $93 1 room-doors-state
    1 $93 1 room-doors-state
;

: house-of-truth-b1-5.char-enter ( -- )   \ 004441C0
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
            $93 0 235 hewie-to-room
            1 $EB -90 char-to-tri-facing
            0 0 2 action
        then
    then
;

: house-of-truth-b1-5.phase1 ( -- )   \ 00444200
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
        $347 story-flag? not if
            $95 room-preload
        else
            $87 room-preload
        then
    then
    0 3 char-entered-area? if
        $345 story-flag? $346 story-flag? and $347 story-flag? and $348 story-flag? and if
            0 ebit-set
            $88 room-preload
        else
            0 ebit-clear
            $87 room-preload
        then
    then
    shared.act98
    house-of-truth-b1-5.cmd00
;

: house-of-truth-b1-5.phase2 ( -- )   \ 00444270
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $2E4 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: house-of-truth-b1-5.act00 ( -- )   \ 004442A0
    1 self-scripted
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act01 ( -- )   \ 004442B0
    1 self-scripted
    0 ebit? if
        $FF 1.0 0 bgm
        keep-room-sounds state-flag-clear
    then
    $F 0 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act02 ( -- )   \ 004442D0
    1 self-scripted
    self-wait-done
    $F 1 fade
    wait-fade
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act03 ( -- )   \ 004442E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $F 6 fade
    wait-fade
    1 message
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-5.act04 ( -- )   \ 00444300
    self-wait-done
    -117.05 101.81 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E4 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: house-of-truth-b1-5.phase5 ( -- )   \ 0047AFD8
    scene-locked state-flag-clear
;

\ ---- registered ----
' house-of-truth-b1-5.enter house-of-truth-b1-5 0 room-script!
' house-of-truth-b1-5.char-enter house-of-truth-b1-5 6 room-script!
' house-of-truth-b1-5.phase1 house-of-truth-b1-5 1 room-script!
' house-of-truth-b1-5.phase2 house-of-truth-b1-5 2 room-script!
' house-of-truth-b1-5.act00 house-of-truth-b1-5 $00 action-script!
' house-of-truth-b1-5.act01 house-of-truth-b1-5 $01 action-script!
' house-of-truth-b1-5.act02 house-of-truth-b1-5 $02 action-script!
' house-of-truth-b1-5.act03 house-of-truth-b1-5 $03 action-script!
' house-of-truth-b1-5.act04 house-of-truth-b1-5 $04 action-script!
' house-of-truth-b1-5.phase5 house-of-truth-b1-5 5 room-script!
