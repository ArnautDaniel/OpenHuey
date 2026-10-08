\ story/rooms/old-mansion-1f-3.fs - the event scripts of room old-mansion-1f-3 ($41; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-3
USING: room-names story.words story.shared ;

: old-mansion-1f-3.enter ( -- )   \ 0041CA90
    room-sounds
    $1C $60 char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 6.38 0.0 -112.79 170 char-to-xyz
        $1C $9002 1 0 char-anim-hold
    then
    0 $F1 1 action
    $22 state-flag-set
    $322 story-flag? if
        1 char-here? 1 0 char-in-nav-group? and if
            1 $13 char-to-tri
        then
        0 1 $14 door-bits
        1 0 $20000 nav-group
    then
    $277 story-flag? not if
        0 -10.64 9.5 -116.64 flicker-sprite
    then
    $2BF story-flag? $2C0 story-flag? not and if
        1 -19.52 1.0 -13.68 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-1f-3.char-enter ( -- )   \ 0041CB20
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    0 3 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    2 2 2 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $1A 160 char-to-tri-facing
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
    then
;

: old-mansion-1f-3.phase1 ( -- )   \ 0041CC00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 1 1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 3 -1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 3 char-entered-area? 0 5 char-entered-area? or if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 6.38 0.0 -112.79 5 10 1 zone
    $FF 0 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 7 var? if
        $1C 2 6 char-sound
    then
    1 47 var? if
        $1C 2 6 char-sound
    then
    $2BF story-flag? not if
        4 -19.52 0.0 -13.68 $A 5 0 zone
        1 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 703 var-set
                $1A 704 var-set
                $1B 1 var-set
                $1C -19520 var-set
                $1D 1000 var-set
                $1E -13680 var-set
                $1F 4 var-set
                0 1 $8B action
            then
        then
    then
    1 -25.44 0.0 -54.79 $C 10 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    2 7.31 0.0 -110.05 $1E 6 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    2 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 7.31 0.0 -110.05 $1E 6 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-3.phase2 ( -- )   \ 0041CD90
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    0 $D char-in-area? 0 -70 $3C char-heading? and if
        5 5 0 scene-change
    then
    $277 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $2BF story-flag? $2C0 story-flag? not and if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: old-mansion-1f-3.phase3 ( -- )   \ 0041CDF0
    -10.0 12.0 -89.0 6.0 12.0 -89.0 -10.0 0.0 -89.0 6.0 0.0 -89.0 lights-doorway
    -37.0 8.0 -83.5 -31.0 8.0 -83.5 -37.0 0.0 -83.5 -31.0 0.0 -83.5 lights-doorway
    -37.0 17.0 -83.5 -31.0 17.0 -83.5 -37.0 8.0 -83.5 -31.0 8.0 -83.5 lights-doorway
    -40.0 8.0 -83.5 -34.0 8.0 -83.5 -40.0 0.0 -83.5 -34.0 0.0 -83.5 lights-doorway
    -40.0 17.0 -83.5 -34.0 17.0 -83.5 -40.0 8.0 -83.5 -34.0 8.0 -83.5 lights-doorway
;

: old-mansion-1f-3.phase5 ( -- )   \ 0041CEE8
    0 2 char-in-area? if
        $1C action-end
        $1C char-done
    then
;

: old-mansion-1f-3.act00 ( -- )   \ 0041CF00
    self-wait-done
    $FE self-touching? not if
        $56 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $56 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $56 door-unlock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: old-mansion-1f-3.act01 ( -- )   \ 0041CF40
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 6.38 0.0 -112.79 170 char-to-xyz
        $1C $9002 1 0 char-anim-hold
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
            $1C $9003 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9002 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9002 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-1f-3.act02 ( -- )   \ 0041D000
    self-wait-done
    fiona-recover
    2 panic-stage
    $FE 3 stalker-mode
    $B01 0 self-anim-blend
    yield
    $F $41 fade
    $B02 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-fade
    self-idle-or-end
;

: old-mansion-1f-3.act03 ( -- )   \ 0041D020
    self-wait-done
    -10.64 -116.64 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $277 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-3.act04 ( -- )   \ 0041D080
    self-wait-done
    -19.52 -13.68 self-turn-to-xz
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
            $2C0 story-flag-set
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

: old-mansion-1f-3.act05 ( -- )   \ 0041D0E0
    self-wait-done
    2.0 -125.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message
    wait-message
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-3.enter old-mansion-1f-3 0 room-script!
' old-mansion-1f-3.char-enter old-mansion-1f-3 6 room-script!
' old-mansion-1f-3.phase1 old-mansion-1f-3 1 room-script!
' old-mansion-1f-3.phase2 old-mansion-1f-3 2 room-script!
' old-mansion-1f-3.phase3 old-mansion-1f-3 3 room-script!
' old-mansion-1f-3.phase5 old-mansion-1f-3 5 room-script!
' old-mansion-1f-3.act00 old-mansion-1f-3 $00 action-script!
' old-mansion-1f-3.act01 old-mansion-1f-3 $01 action-script!
' old-mansion-1f-3.act02 old-mansion-1f-3 $02 action-script!
' old-mansion-1f-3.act03 old-mansion-1f-3 $03 action-script!
' old-mansion-1f-3.act04 old-mansion-1f-3 $04 action-script!
' old-mansion-1f-3.act05 old-mansion-1f-3 $05 action-script!
