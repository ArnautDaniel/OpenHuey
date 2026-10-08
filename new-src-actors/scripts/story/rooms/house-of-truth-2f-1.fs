\ story/rooms/house-of-truth-2f-1.fs - the event scripts of room house-of-truth-2f-1 ($82; House of Truth: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-2f-1
USING: room-names story.words story.shared ;

: house-of-truth-2f-1.enter ( -- )   \ 004314B0
    $297 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $2FA story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $2FC story-flag? not if
            7 -17.0 71.0 -41.0 flicker-sprite
        then
    then
    $2FB story-flag? not if
        1 2 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    0 5 0.812 0.687 0.187 0.312 zone-rect
    0 -102.0 65.4 -31.4 4 effect-86
    1 -43.2 46.0 -31.2 1 effect-86
    2 -72.4 17.2 -1.75 1 effect-86
    3 -102.0 66.0 87.6 4 effect-86
    4 -43.0 46.0 87.7 1 effect-86
    5 -72.4 17.2 117.2 1 effect-86
    8 -101.8 66.0 -31.0 4 effect-86
    9 -101.8 66.0 88.0 4 effect-86
    $2EB story-flag? $2EC story-flag? not and if
        6 -50.0 71.0 -15.0 flicker-sprite
    then
;

: house-of-truth-2f-1.char-enter ( -- )   \ 004315E0
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 6 6 char-camera
                0 camera-follow
            else
                1 6 6 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 6 6 char-camera
            0 camera-follow
        else
            1 6 6 char-camera
            1 camera-follow
        then
    then then
    3 6 6 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    4 5 5 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $C3 -9.0 95.0 180 char-to-xz
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            $82 1 194 hewie-to-room
            1 $C2 0 char-to-tri-facing
            0 0 3 action
        else
            room-sounds
        then
    then
;

: house-of-truth-2f-1.phase1 ( -- )   \ 00431750
    $94 story-flag? if
        0 exit-usable? if
            0 exit-check
        then
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 4 4 1 chars-area-camera
    $F 5 5 1 chars-area-camera
    $10 6 6 1 chars-area-camera
    $11 7 7 1 chars-area-camera
    $12 3 3 1 chars-area-camera
    $13 7 7 1 chars-area-camera
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        4 exit-prepare
    then
    $94 story-flag? not if
        0 0 char-entered-area? 2 ebit? not and if
            2 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
    $297 story-flag? not if
        0 -57.4 70.0 114.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $297 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -57.4 70.0 114.5 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -57.4 70.0 114.5 0 0 sound
            $40 $103 noise
        then
    then
    $2FB story-flag? not if
        9 -3.0 70.0 -41.0 5 8 1 zone
        $FF 9 char-in-zone? if
            $2FB story-flag-set
            0 2 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -3.0 70.0 -41.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -3.0 70.0 -41.0 0 0 sound
            $40 $86 noise
            $82 1 147 $24 5 -1 0 0.0 creature-place
        then
    then
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    5 4 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    6 5 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 5 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 5 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 5 char-effect-moving
    then
    $2EB story-flag? not if
        7 -50.0 70.0 -15.0 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 747 var-set
                $1A 748 var-set
                $1B 6 var-set
                $1C -50000 var-set
                $1D 71000 var-set
                $1E -15000 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
;

: house-of-truth-2f-1.phase2 ( -- )   \ 00431A60
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 1 1 scene-change
        then
    then
    0 $14 char-in-area? 0 45 $32 char-heading? and if
        5 4 0 scene-change
    then
    $2EB story-flag? $2EC story-flag? not and if
        7 6 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    $2FA story-flag? not if
        8 -17.0 70.0 -41.0 5 8 1 zone
        $FF 8 char-in-zone? if
            $2FA story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -17.0 70.0 -41.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -17.0 70.0 -41.0 0 0 sound
            $40 $95 noise
            7 -17.0 71.0 -41.0 flicker-sprite
        then
    else $2FC story-flag? not if
        8 7 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then then
;

: house-of-truth-2f-1.act00 ( -- )   \ 00431B20
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    0 creatures-clear
    $12 state-flag-set
    0 exit-prepare
    0 self-scripted
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-2f-1.act01 ( -- )   \ 00431B40
    self-wait-done
    0 self-through-exit
    self-wait-done
    1 ebit? not 2 game-mode? or $FF panic-stage? or if
        $FF panic-stage? 2 game-mode? or if
            $60B self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $609 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            0 message
            wait-message
        then
        1 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $26E -10.0 135.3 0 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 0.0 0.0 2.0 event-camera
        self-frames-reset
        4 self-wait-frames
        1 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        1 ebit-clear
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: house-of-truth-2f-1.act02 ( -- )   \ 00431BD0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 3 6 char-sound
    $25 message-param-room
    $E6 door-locked? not if
        $25 item-use
    then
    $E2 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-1.act03 ( -- )   \ 00431C10
    1 self-scripted
    self-wait-done
    camera-restart
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    effects-arena-flip
    0 char-in
    begin
        yield
        4 sound-bank-loaded? until
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    $E6 door-lock
    $EA door-lock
    $103 door-lock
    1 $8E 1 room-doors-state
    $FE $8E 1 room-doors-state
    $FE $8C 1 room-doors-state
    $FE char-activate
    $FE $82 622 2 stalker-to-room
    $FE $26E 180 char-to-tri-facing
    $FE 1 1 char-camera
    0 state-flag-set
    stalker-item-cooldown
    room-sounds
    $F $41 fade
    wait-fade
    $24F item-give
    $12 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-1.act04 ( -- )   \ 00431CC0
    self-wait-done
    0 $FB -34.0 99.0 90 char-to-xz
    1 46.0 10.0 150.0 0.0 event-camera
    2 message
    wait-message
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: house-of-truth-2f-1.act05 ( -- )   \ 00431CF0
    self-wait-done
    -50.0 -15.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $2EC story-flag-set
            6 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: house-of-truth-2f-1.act06 ( -- )   \ 00431D50
    self-wait-done
    -17.0 -41.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $2FC story-flag-set
            7 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

\ ---- registered ----
' house-of-truth-2f-1.enter house-of-truth-2f-1 0 room-script!
' house-of-truth-2f-1.char-enter house-of-truth-2f-1 6 room-script!
' house-of-truth-2f-1.phase1 house-of-truth-2f-1 1 room-script!
' house-of-truth-2f-1.phase2 house-of-truth-2f-1 2 room-script!
' house-of-truth-2f-1.act00 house-of-truth-2f-1 $00 action-script!
' house-of-truth-2f-1.act01 house-of-truth-2f-1 $01 action-script!
' house-of-truth-2f-1.act02 house-of-truth-2f-1 $02 action-script!
' house-of-truth-2f-1.act03 house-of-truth-2f-1 $03 action-script!
' house-of-truth-2f-1.act04 house-of-truth-2f-1 $04 action-script!
' house-of-truth-2f-1.act05 house-of-truth-2f-1 $05 action-script!
' house-of-truth-2f-1.act06 house-of-truth-2f-1 $06 action-script!
