\ story/rooms/castle-1f-1.fs - the event scripts of room castle-1f-1 ($1; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-1
USING: room-names story.words story.shared ;

: castle-1f-1.enter ( -- )   \ 003EE770
    $37 story-flag? $24 story-flag? not and if
        $24 story-flag-set
        1 0 443 4 8 -1 0 0.0 creature-place
    then
    $3B story-flag? not if
        1 exit-door-open? if
            4 0.1 0 bgm
        else
            4 0.01 0 bgm
        then
    then
    $12 story-flag? not if
        1 1 $14 door-bits
        1 0 8 nav-group
    else
        0 1 $14 door-bits
    then
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $A 103.4 28.8 -51.9 $A $80 $80 $80 $20 specks
        $10 83.3 29.0 14.8 $E $80 $80 $80 $50 specks
        $10 -44.8 13.0 -13.0 $E $80 $80 $80 $40 specks
        $C -87.5 12.9 36.0 $A $80 $80 $80 $40 specks
    then
;

: castle-1f-1.char-enter ( -- )   \ 003EE820
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    2 0 -1 area-camera
    0 self-is? if
        $301 story-flag? $302 story-flag? not and if
            0.0 sound-volume-scale
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
            0 $B7 0 char-to-tri-facing
            $303 story-flag? if
                1 0 306 hewie-to-room
                1 $132 0 char-to-tri-facing
            then
            $302 story-flag-set
            0 ebit-set
            0 $F1 1 action
        then
    then
;

: castle-1f-1.phase1 ( -- )   \ 003EE920
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    8 0 -1 1 chars-area-camera
    9 1 0 1 chars-area-camera
    $A 1 0 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    $C 0 -1 -1 chars-area-camera
    $340 story-flag? not if
        $D 3 -1 1 chars-area-camera
    then
    $12 story-flag? not if
        0 3 char-entered-area? if
            1 exit-prepare
        then
        0 4 char-entered-area? if
            0 exit-prepare
        then
    else
        0 3 char-entered-area? 0 5 char-entered-area? or if
            1 exit-prepare
        then
        0 4 char-entered-area? if
            0 exit-prepare
        then
        0 6 char-entered-area? if
            2 exit-prepare
        then
    then
    0 ebit? if
        $FE char-here? if
            0 ebit-clear
        then
    then
    $234 story-flag? not if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not if
                0 control-action? if
                    -40 -16 60 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                        0 -40 60 $32 char-faces-xz? if
                            hewie-stays? if
                                0 0 3 action
                            then
                        then
                    then
                then
            then
        then
        2 -33.33 -16.93 69.69 $28 6 0 zone
        3 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 2 9 char-zone-bits? if
                        3 ebit-set
                        $64 chance? if
                            $1F 2 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        2 -33.33 -16.93 69.69 $28 6 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    $1F 2 var-set
                    $1F 6.0 hewie-look-zone
                then
            then
        then
    then
    1 ebit? not if
        0 -46.0 -16.0 70.0 $C 4 0 zone
        0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
            0 5 -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
            1 $A -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
            $FE 5 -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
    then
    2 ebit? not if
        1 -23.0 -16.0 67.0 $C 4 0 zone
        0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
            0 5 -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
            1 $A -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
            $FE 5 -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
    then
    $340 story-flag? not if
        0 $E char-in-area? fiona-free? and if
            2 game-mode? 0 $FE 70 chars-within? not and 2 game-mode? not or if
                $340 story-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 5 action-force
                else
                    1 0 5 action-force
                then
            then
        then
    then
    $3B story-flag? not if
        1 exit-door-open? not if
            4 0.01 0 bgm
        else
            4 0.1 0 bgm
        then
    then
;

: castle-1f-1.phase2 ( -- )   \ 003EEB90
    $12 story-flag? not if
        0 7 char-in-area? 0 -67 $2D char-heading? and if
            5 0 0 scene-change
        then
    then
    1 exit-door-open? not 0 ebit? and if
        0 1 char-group-bit4? if
            0 scene-ending
        then
    then
;

: castle-1f-1.phase3 ( -- )   \ 003EEBC0
    -41.7 -6.2 58.0 -70.6 -6.2 58.0 -41.7 -18.6 58.0 -70.6 -18.6 58.0 lights-doorway
;

: castle-1f-1.phase5 ( -- )   \ 003EEC00
    0 ebit? $FE char-here? not and $FE 2 char-C4? and if
        $FE action-end
        2 summon-take
    then
    1 char-busy? if
        1 action-end
        1 $FE -40.709 45.642 180 char-to-xz
        $18 state-flag-clear
    then
;

: castle-1f-1.act00 ( -- )   \ 003EEC28
    self-wait-done
    $1D02 $A self-anim-blend
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-1.act01 ( -- )   \ 003EEC40
    0 $28 5 char-sound
    5 0 0 music
    7 state-flag-set
    yield
    7 state-flag-clear
    8 state-flag-clear
    $AF story-flag? not if
        $22A item-add
    then
    $F $51 fade
    wait-fade
    $AF story-flag? not if
        $C $85 0.0 0.0 0.0 0 0 sound
    then
    self-idle-or-end
;

: castle-1f-1.act02 ( -- )   \ 0047A980
    self-wait-done
    0 message
    self-idle-or-end
;

: castle-1f-1.act03 ( -- )   \ 003EEC80
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 4 action-force
        $C00 self-anim
        0 1 char-wait-motion
        0 $2E 5 char-sound
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $18 state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-1.act04 ( -- )   \ 003EECB0
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $1E0 -35.67 31.26 -10 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $1BD -42.443 71.623 1.0 45 hewie-go-to
    self-wait-done
    $1BE -31.549 70.055 150 $FFFF 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $A state-flag? 0 char-busy? not or if
        $95 $63 item-count? not if
            10 hewie-trust
        then
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $234 story-flag-set
            $95 1 item-give-count
            0 $95 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    then
    $1C0 -42.94 81.76 176 $FFFF 5 self-move-to
    self-wait-done
    $FE -40.709 45.642 1.0 45 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: castle-1f-1.act05 ( -- )   \ 003EED70
    1 self-scripted
    self-wait-done
    $17 state-flag-set
    $24 state-flag-set
    $F 6 fade
    wait-fade
    $40AA message
    wait-message
    $F 7 fade
    wait-fade
    $340 story-flag-set
    $AA message-param-room
    $AA 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    $24 state-flag-clear
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-1.enter castle-1f-1 0 room-script!
' castle-1f-1.char-enter castle-1f-1 6 room-script!
' castle-1f-1.phase1 castle-1f-1 1 room-script!
' castle-1f-1.phase2 castle-1f-1 2 room-script!
' castle-1f-1.phase3 castle-1f-1 3 room-script!
' castle-1f-1.phase5 castle-1f-1 5 room-script!
' castle-1f-1.act00 castle-1f-1 $00 action-script!
' castle-1f-1.act01 castle-1f-1 $01 action-script!
' castle-1f-1.act02 castle-1f-1 $02 action-script!
' castle-1f-1.act03 castle-1f-1 $03 action-script!
' castle-1f-1.act04 castle-1f-1 $04 action-script!
' castle-1f-1.act05 castle-1f-1 $05 action-script!
