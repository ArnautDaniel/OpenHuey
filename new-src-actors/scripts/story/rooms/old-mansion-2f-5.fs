\ story/rooms/old-mansion-2f-5.fs - the event scripts of room old-mansion-2f-5 ($58; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-5
USING: room-names story.words story.shared ;

: old-mansion-2f-5.enter ( -- )   \ 0040F910
    room-sounds
    3 0 var-set
    4 0 var-set
    $261 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $262 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $28D story-flag? not if
            0 10.5 1.0 -28.0 flicker-sprite
        then
    then
    0 3 0.0 0.5 0.312 0.187 zone-rect
    $1C $17F char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
    then
    0 $F1 0 action
    $22 state-flag-set
    $56 story-flag? if
        $78 story-flag? not if
            $78 story-flag-set
            $58 0 291 $81 3 -1 0 0.0 creature-place
            $58 0 426 $81 3 -1 0 0.0 creature-place
            $58 0 205 $81 3 -1 0 0.0 creature-place
        then
    then
    $2C1 story-flag? $2C2 story-flag? not and if
        1 7.38 1.0 20.58 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-2f-5.act03 ( -- )   \ 0040FEC0
    3 ebit-set
    2 ebit-clear
    $15 0 pvar? if
        $A chance? if
            2 ebit-set
        then
    else $15 1 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $15 2 pvar? if
        $32 chance? if
            2 ebit-set
        then
    else $15 3 pvar? if
        $4B chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 $FE 4 action
    else
        $78 1 item-cooldown
    then
    $15 pvar-inc
    exit
;

: old-mansion-2f-5.char-enter ( -- )   \ 0040FA20
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 6 exit-taken? and if
            0 6 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 6 exit-taken? and if
        1 6 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    6 1 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            3 ebit-set
            old-mansion-2f-5.act03
        else
            3 ebit-clear
        then
    then
;

: old-mansion-2f-5.phase1 ( -- )   \ 0040FAB0
    0 exit-usable? if
        0 exit-check
    then
    6 exit-usable? if
        6 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        6 exit-prepare
    then
    $261 story-flag? not if
        0 -10.3 0.0 -93.8 5 8 1 zone
        $FF 0 char-in-zone? if
            $261 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -10.3 0.0 -93.8 0 -2145378256 0 0.0 scene-effect-8C
            $88 5 -10.3 0.0 -93.8 0 0 sound
            $40 $E1 noise
        then
    then
    2 -3.0 0.0 43.0 5 10 1 zone
    $FF 2 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 1 var? if
        $1C 2 6 char-sound
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    4 ebit-set
                    $50 chance? if
                        $1F 3 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    $2C1 story-flag? not if
        4 7.38 0.0 20.58 $A 5 0 zone
        1 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 705 var-set
                $1A 706 var-set
                $1B 1 var-set
                $1C 7380 var-set
                $1D 1000 var-set
                $1E 20580 var-set
                $1F 4 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        4 var-inc
        4 30 var? if
            4 0 var-set
            3 0 var? if
            else 3 1 var? if
            else 3 2 var? if
            else 3 3 var? if
            then then then then
            3 var-inc
            3 4 var? if
                3 0 var-set
            then
        then
    then
;

: old-mansion-2f-5.phase2 ( -- )   \ 0040FC70
    0 6 char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $262 story-flag? not if
        1 10.5 0.0 -28.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $262 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            10.5 0.0 -28.0 0 -2145378256 0 0.0 scene-effect-8C
            $88 5 10.5 0.0 -28.0 0 0 sound
            $40 $1AD noise
            0 10.5 1.0 -28.0 flicker-sprite
        then
    else $28D story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
    $2C1 story-flag? $2C2 story-flag? not and if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: old-mansion-2f-5.act00 ( -- )   \ 0040FD30
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            1 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9005 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9004 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9004 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-2f-5.act02 ( -- )   \ 0040FE90
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    1 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-5.act01 ( -- )   \ 0040FDF0
    $18 state-flag-set
    3 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $106 $8004 5 -4.6 -231.1 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    1 ebit-clear
    0 avoid-prompt
    $FF 2 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 1 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] old-mansion-2f-5.act02 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? $FE char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] old-mansion-2f-5.act02 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-2f-5.act04 ( -- )   \ 0040FF10
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 6 char-heading-for? 6 exit-door-open? not and if
        6 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $106 -4.6 -222.0 180 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        $FE char-file-use
        4 $FE 1 char-model-op
        $8000 self-anim
        self-wait-anim
        4 $FE 0 char-model-op
    else
        $1601 self-anim
        self-wait-anim
    then
    1 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-2f-5.act05 ( -- )   \ 0040FF70
    self-wait-done
    10.5 -28.0 self-turn-to-xz
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
            $28D story-flag-set
            0 effect-remove
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

: old-mansion-2f-5.act06 ( -- )   \ 0040FFD0
    self-wait-done
    7.38 20.58 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C2 story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: old-mansion-2f-5.phase5 ( -- )   \ 0047ABD0
    $1C action-end
    $1C char-done
;

\ ---- registered ----
' old-mansion-2f-5.enter old-mansion-2f-5 0 room-script!
' old-mansion-2f-5.char-enter old-mansion-2f-5 6 room-script!
' old-mansion-2f-5.phase1 old-mansion-2f-5 1 room-script!
' old-mansion-2f-5.phase2 old-mansion-2f-5 2 room-script!
' old-mansion-2f-5.act00 old-mansion-2f-5 $00 action-script!
' old-mansion-2f-5.act01 old-mansion-2f-5 $01 action-script!
' old-mansion-2f-5.act02 old-mansion-2f-5 $02 action-script!
' old-mansion-2f-5.act03 old-mansion-2f-5 $03 action-script!
' old-mansion-2f-5.act04 old-mansion-2f-5 $04 action-script!
' old-mansion-2f-5.act05 old-mansion-2f-5 $05 action-script!
' old-mansion-2f-5.act06 old-mansion-2f-5 $06 action-script!
' old-mansion-2f-5.phase5 old-mansion-2f-5 5 room-script!
