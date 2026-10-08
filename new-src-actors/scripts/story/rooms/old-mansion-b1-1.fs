\ story/rooms/old-mansion-b1-1.fs - the event scripts of room old-mansion-b1-1 ($44; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-1
USING: room-names story.words story.shared ;

: old-mansion-b1-1.enter ( -- )   \ 00415BA0
    $264 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $28F story-flag? not if
            0 60.5 0.9 -33.4 flicker-sprite
        then
    then
    0 6 0.812 0.687 0.187 0.312 zone-rect
    $1C $C5 char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 39.3 0.0 50.4 -90 char-to-xyz
        $1C $900A 1 0 char-anim-hold
    then
    0 $F1 0 action
    $22 state-flag-set
    $2C6 story-flag? not if
        1 -6.17 39.94 -29.06 flicker-sprite
    then
    1 $2300 sound-volume
    1 1 8 nav-group
    1 $51 $10000000 nav-tri-flags
    1 $50 $10000000 nav-tri-flags
    1 $EF $10000000 nav-tri-flags
    1 $DB $10000000 nav-tri-flags
    1 $FA $10000000 nav-tri-flags
    1 $F9 $10000000 nav-tri-flags
    1 $55 $10000000 nav-tri-flags
    1 $DA $10000000 nav-tri-flags
    1 $F3 $10000000 nav-tri-flags
    1 $F8 $10000000 nav-tri-flags
;

: old-mansion-b1-1.char-enter ( -- )   \ 00415C90
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    1 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 4 char-camera
                0 camera-follow
            else
                1 4 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
    then then
    2 4 4 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    3 1 1 area-camera
    0 self-is? if
        $80 exit-taken? if
            8 state-flag-set
            0.0 sound-volume-scale
            0 0 2 action
        else
            room-sounds
            $13 1.0 0 bgm
        then
        1 exit-taken? 2 exit-taken? or if
            1 map-page
        then
        0 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-b1-1.phase1 ( -- )   \ 00415DB0
    0 exit-usable? if
        0 exit-check
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
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        3 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    1 39.3 0.0 50.4 5 10 1 zone
    $FF 1 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 0 var? not if
        2 var-inc
        2 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            2 0 var-set
        then
    then
    0 5 char-entered-area? if
        0 map-page
    then
    0 5 char-left-area? if
        1 map-page
    then
    0 $A char-entered-area? if
        1 map-page
    then
    0 $A char-left-area? if
        2 map-page
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        2 game-mode? not if
            0 hewie-side? 0 $200000 char-on-nav-flags? and if
                -6 40 -29 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                    0 -6 -29 $3C char-faces-xz? if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 3 action
                            then
                        then
                    then
                then
            then
        then
    then
    2 38.73 0.0 50.28 $1A 16 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    1 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 38.73 0.0 50.28 $1A 16 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $2C6 story-flag? not if
        3 -9.31 39.5 -32.27 $32 13 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        3 -9.31 39.5 -32.27 $32 13 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    $1F 3 var-set
                    $1F 0.0 hewie-look-zone
                then
            then
        then
    then
;

: old-mansion-b1-1.phase2 ( -- )   \ 00415FA0
    $264 story-flag? not if
        0 60.5 -0.1 -33.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $264 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            60.5 -0.1 -33.4 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 60.5 -0.1 -33.4 0 0 sound
            $40 $123 noise
            0 60.5 0.9 -33.4 flicker-sprite
        then
    else $28F story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
;

: old-mansion-b1-1.phase3 ( -- )   \ 00416030
    37.0 -50.0 50.0 82.5 -50.0 50.0 37.0 -90.0 50.0 82.5 -90.0 50.0 lights-doorway
    52.0 -32.0 45.0 68.0 -32.0 45.0 52.0 -90.0 45.0 68.0 -90.0 45.0 lights-doorway
    -29.0 -4.0 50.0 37.0 -48.0 50.0 -8.0 -80.0 50.0 37.0 -80.0 50.0 lights-doorway
    5.0 0.0 -40.0 -65.0 0.0 -40.0 5.0 -80.0 -40.0 -65.0 -80.0 -40.0 lights-doorway
    -50.0 18.5 -40.0 -50.0 18.5 5.5 -50.0 -50.0 -40.0 -50.0 -50.0 5.5 lights-doorway
;

: old-mansion-b1-1.phase5 ( -- )   \ 00416130
    $1C action-end
    $1C char-done
    $18 state-flag? if
        1 action-end
        1 $76 -40.52 -28.0 -90 char-to-xz
        $18 state-flag-clear
    then
;

: old-mansion-b1-1.act00 ( -- )   \ 00416150
    0 exit-taken? 3 exit-taken? or if
        3 $1C char-loaded? not if
            $1C 3 $FF char-load
            3 char-unload
            $1C char-activate
            $1C 39.3 0.0 50.4 -90 char-to-xyz
            $1C $900A 1 0 char-anim-hold
            $1C 1 char-visible
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
                $1C $900B 0 3 char-anim-hold
                $1C wait-char-anim
                $1C $900A 0 3 char-anim-hold
                1 0 var-set
                0 5 var? if
                    $1C wait-char-anim
                    0 0 var-set
                then
            then
            $1C char-at-motion-event? if
                $1C $900A 0 0 char-anim-hold
                1 0 var-set
            else
                1 var-inc
            then
            self-frames-reset
            1 self-wait-frames
        again
    else
        3 $1C char-loaded? not if
            $1C 3 $FF char-load
            3 char-unload
            $1C char-activate
            $1C 39.3 0.0 50.4 -90 char-to-xyz
            $1C $900A 1 0 char-anim-hold
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
                $1C $900B 0 3 char-anim-hold
                $1C wait-char-anim
                $1C $900A 0 3 char-anim-hold
                1 0 var-set
                0 5 var? if
                    $1C wait-char-anim
                    0 0 var-set
                then
            then
            $1C char-at-motion-event? if
                $1C $900A 0 0 char-anim-hold
                1 0 var-set
            else
                1 var-inc
            then
            self-frames-reset
            1 self-wait-frames
        again
    then
    self-idle-or-end
;

: old-mansion-b1-1.act01 ( -- )   \ 004162E0
    self-wait-done
    60.5 -33.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $28F story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: old-mansion-b1-1.act02 ( -- )   \ 00416340
    1 self-scripted
    self-wait-done
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
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    0 1 char-to-exit
    1 char-activate
    $44 1 161 hewie-to-room
    1 1 char-to-exit
    $FE $47 -1 2 stalker-to-room
    stalker-item-cooldown
    $FE 0 stalker-mode
    hewie-controlled? not if
        0 3 3 char-camera
        0 camera-follow
    else
        1 3 3 char-camera
        1 camera-follow
    then
    yield
    camera-restart
    room-sounds
    $13 1.0 0 bgm
    $F $51 fade
    wait-fade
    $23F item-give
    $12 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-1.act03 ( -- )   \ 004163F0
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    hewie-bark
    self-wait-done
    $C -59.57 -30.97 90 $FFFF $A self-move-to
    self-wait-done
    0 1 8 nav-group
    1 1 $30 nav-group
    $129 -11.17 -29.06 0.3 50 hewie-go-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $2C6 story-flag? not if
        $A state-flag? 0 char-busy? not or if
            $71 $63 item-count? not if
                10 hewie-trust
            then
            $71 message-param-room
            $71 $63 item-count? if
                $8010 message
                wait-message
            else
                $2C6 story-flag-set
                $71 1 item-give-count
                0 $71 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            $2C6 story-flag? if
                1 effect-remove
            then
        then
    then
    $129 -11.46 -30.1 -90 $FFFF 5 self-move-to
    self-wait-done
    $C -59.57 -30.97 2.0 50 hewie-go-to
    self-wait-done
    1 1 8 nav-group
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-b1-1.enter old-mansion-b1-1 0 room-script!
' old-mansion-b1-1.char-enter old-mansion-b1-1 6 room-script!
' old-mansion-b1-1.phase1 old-mansion-b1-1 1 room-script!
' old-mansion-b1-1.phase2 old-mansion-b1-1 2 room-script!
' old-mansion-b1-1.phase3 old-mansion-b1-1 3 room-script!
' old-mansion-b1-1.phase5 old-mansion-b1-1 5 room-script!
' old-mansion-b1-1.act00 old-mansion-b1-1 $00 action-script!
' old-mansion-b1-1.act01 old-mansion-b1-1 $01 action-script!
' old-mansion-b1-1.act02 old-mansion-b1-1 $02 action-script!
' old-mansion-b1-1.act03 old-mansion-b1-1 $03 action-script!
