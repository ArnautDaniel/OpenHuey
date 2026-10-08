\ story/rooms/house-of-truth-1f-2.fs - the event scripts of room house-of-truth-1f-2 ($88; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-2
USING: room-names story.words story.shared ;

\ room 0x88: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-2.cmd00 ( -- )  s" house-of-truth-1f-2.cmd00" stub-step ;

: house-of-truth-1f-2.enter ( -- )   \ 00432CA0
    room-sounds
    $298 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2E8 story-flag? not if
            1 -0.4 71.0 -123.2 flicker-sprite
        then
    then
    0 1 0.812 0.687 0.187 0.312 zone-rect
    $2E6 story-flag? not if
        0 -33.17 71.0 33.19 flicker-sprite
    then
    $2F1 story-flag? $2F2 story-flag? not and if
        2 35.0 71.0 33.0 flicker-sprite
    then
    1 $51 $10000000 nav-tri-flags
    1 $21C $10000000 nav-tri-flags
    1 $2F $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $23B $10000000 nav-tri-flags
    1 $43 $10000000 nav-tri-flags
    1 $22C $10000000 nav-tri-flags
    1 $350 $10000000 nav-tri-flags
    1 $41 $10000000 nav-tri-flags
    1 $22D $10000000 nav-tri-flags
    1 $354 $10000000 nav-tri-flags
    1 $232 $10000000 nav-tri-flags
    1 $355 $10000000 nav-tri-flags
    1 $34C $10000000 nav-tri-flags
    1 $35C $10000000 nav-tri-flags
    1 $351 $10000000 nav-tri-flags
    1 $48 $10000000 nav-tri-flags
    1 $235 $10000000 nav-tri-flags
    1 $356 $10000000 nav-tri-flags
    1 $234 $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $23D $10000000 nav-tri-flags
    1 $56 $10000000 nav-tri-flags
    1 $31 $10000000 nav-tri-flags
    1 $21E $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
;

: house-of-truth-1f-2.char-enter ( -- )   \ 00432DF0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 6 6 char-camera
                0 camera-follow
            else
                1 6 6 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 6 6 char-camera
            0 camera-follow
        else
            1 6 6 char-camera
            1 camera-follow
        then
    then then
    0 6 6 area-camera
    0 self-is? if
        $FE exit-taken? if
            0 $28 -60.0 42.0 180 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
        $80 exit-taken? if
            0 $3F -23.0 25.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            $88 0 64 hewie-to-room
            1 $40 -90 char-to-tri-facing
            0 0 3 action
        then
    then
;

: house-of-truth-1f-2.phase1 ( -- )   \ 00432E90
    2 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 -60.0 20.0 50.0 0 0 sound
            2 ebit-set
        then
    else
        $C0000008 6 -60.0 20.0 50.0 0 0 sound
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000009 6 -78.0 21.0 46.5 0 0 sound
            else 0 1 var? if
                $4000000A 6 -78.0 21.0 46.5 0 0 sound
            else 0 2 var? if
                $4000000B 6 -78.0 21.0 46.5 0 0 sound
            else 0 3 var? if
                $4000000C 6 -78.0 21.0 46.5 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    4 0 0 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 2 2 1 chars-area-camera
    7 3 3 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 4 4 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 5 5 1 chars-area-camera
    $10 5 5 1 chars-area-camera
    $11 5 5 1 chars-area-camera
    $17 0 0 1 chars-area-camera
    $18 7 7 1 chars-area-camera
    $19 3 3 1 chars-area-camera
    $1A 6 6 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        $27 room-preload
    then
    0 $15 char-entered-area? 0 $16 char-entered-area? or if
        1 map-page
    then
    0 $15 char-left-area? 0 $16 char-left-area? or if
        2 map-page
    then
    1 -61.4 0.0 46.49 $14 18 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    1 -61.4 0.0 46.49 $14 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    0 0 char-action? $FF panic-stage? not and 0 1 char-entered-area? and if
        0 0 7 action
    then
    $2F1 story-flag? not if
        3 35.0 70.0 33.0 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 753 var-set
                $1A 754 var-set
                $1B 2 var-set
                $1C 35000 var-set
                $1D 71000 var-set
                $1E 33000 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    house-of-truth-1f-2.cmd00
;

: house-of-truth-1f-2.phase2 ( -- )   \ 004330C0
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $13 char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $14 char-in-area? 0 0 $32 char-heading? and if
        5 1 0 scene-change
    then
    $2E6 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $298 story-flag? not if
        0 -0.4 70.0 -123.2 5 8 1 zone
        $FF 0 char-in-zone? if
            $298 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -0.4 70.0 -123.2 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -0.4 70.0 -123.2 0 0 sound
            $40 $34E noise
            1 -0.4 71.0 -123.2 flicker-sprite
        then
    else $2E8 story-flag? not if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
    $2F1 story-flag? $2F2 story-flag? not and if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: house-of-truth-1f-2.phase3 ( -- )   \ 004331B0
    -20.0 76.0 -132.0 20.0 76.0 -132.0 -20.0 50.0 -132.0 20.0 50.0 -132.0 lights-doorway
    38.0 78.0 -60.2 70.0 55.0 -60.2 38.0 60.0 -60.2 70.0 40.0 -60.2 lights-doorway
    10.0 73.0 -85.5 43.0 73.0 -85.5 10.0 40.0 -85.5 43.0 40.0 -85.5 lights-doorway
    -43.0 73.0 -85.5 -10.0 73.0 -85.5 -43.0 40.0 -85.5 -10.0 40.0 -85.5 lights-doorway
    -70.0 55.0 -60.2 -38.0 78.0 -60.2 -70.0 40.0 -60.2 -38.0 60.0 -60.2 lights-doorway
;

: house-of-truth-1f-2.act00 ( -- )   \ 004332B0
    0 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $117 0.0 -1.1 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        0 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        0 $84 5 char-sound
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0.0 5.0 self-turn-to-xz
        self-wait-done
        1 message
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act01 ( -- )   \ 00433370
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    -60.0 55.0 self-turn-to-xz
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    2 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        8 state-flag-set
        $FE exit-check
    else
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: house-of-truth-1f-2.act02 ( -- )   \ 004333B0
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    1 map-page
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-2.act03 ( -- )   \ 004333D0
    1 self-scripted
    self-wait-done
    1 map-page
    $27 room-preload
    $F $41 fade
    wait-fade
    0 state-flag-set
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-2.act04 ( -- )   \ 004333F0
    self-wait-done
    -33.17 33.19 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E6 story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: house-of-truth-1f-2.act05 ( -- )   \ 00433450
    self-wait-done
    -0.4 -123.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E8 story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: house-of-truth-1f-2.act06 ( -- )   \ 004334B0
    self-wait-done
    35.0 33.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $98 message-param-room
        $98 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F2 story-flag-set
            2 effect-remove
            $98 1 item-give-count
            0 $98 item-tab
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

: house-of-truth-1f-2.act07 ( -- )   \ 00433510
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    3 message
    wait-message
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-1f-2.enter house-of-truth-1f-2 0 room-script!
' house-of-truth-1f-2.char-enter house-of-truth-1f-2 6 room-script!
' house-of-truth-1f-2.phase1 house-of-truth-1f-2 1 room-script!
' house-of-truth-1f-2.phase2 house-of-truth-1f-2 2 room-script!
' house-of-truth-1f-2.phase3 house-of-truth-1f-2 3 room-script!
' house-of-truth-1f-2.act00 house-of-truth-1f-2 $00 action-script!
' house-of-truth-1f-2.act01 house-of-truth-1f-2 $01 action-script!
' house-of-truth-1f-2.act02 house-of-truth-1f-2 $02 action-script!
' house-of-truth-1f-2.act03 house-of-truth-1f-2 $03 action-script!
' house-of-truth-1f-2.act04 house-of-truth-1f-2 $04 action-script!
' house-of-truth-1f-2.act05 house-of-truth-1f-2 $05 action-script!
' house-of-truth-1f-2.act06 house-of-truth-1f-2 $06 action-script!
' house-of-truth-1f-2.act07 house-of-truth-1f-2 $07 action-script!
