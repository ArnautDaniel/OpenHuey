\ story/rooms/front-garden-4.fs - the event scripts of room front-garden-4 ($2B; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-4
USING: room-names story.words story.shared flag-names ;

\ character kind 0x1A: byte 3 0 starts Kind26_MoveToB(2, -290, 42); else waits (2) until
\ Kind26_MoveDoneB says done
: front-garden-4.cmd00 ( b0 -- )  drop s" front-garden-4.cmd00" stub-step ;

: front-garden-4.enter ( -- )   \ 00405640
    room-sounds
    $16 1.0 0 bgm
    0 ebit-clear
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    then
    $213 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    0 5 0.812 0.687 0.187 0.312 zone-rect
    $222 story-flag? not if
        0 -202.89 1.0 -6.76 flicker-sprite
    then
    $240 story-flag? $241 story-flag? not and if
        1 -283.8 1.0 15.5 flicker-sprite
    then
    $1E chance? if
        0 $F1 6 action
    else
        $1A action-end
        $1A char-done
    then
;

: front-garden-4.char-enter ( -- )   \ 004056D0
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
    $FE self-is? if
        fiona-half-hidden state-flag? 0 3 char-in-area? and $30B story-flag? not and $37 exit-door-open? and if
            0 $FE 0 action
        then
    then
;

: front-garden-4.phase1 ( -- )   \ 00405730
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    $FE char-busy? if
        fiona-half-hidden state-flag? not if
            $FE action-end
            0 ebit-clear
        then
    then
    $213 story-flag? not if
        0 -120.0 0.0 -13.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $213 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -120.0 0.0 -13.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -120.0 0.0 -13.0 0 0 sound
            $40 $124 noise
        then
    then
    $240 story-flag? not if
        2 -283.8 0.0 15.5 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 576 var-set
                $1A 577 var-set
                $1B 1 var-set
                $1C -283800 var-set
                $1D 1000 var-set
                $1E 15500 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    $1A char-here? 0 game-mode? and if
        1 char-here? if
            $1A 1 70 chars-within? if
                $1A 0.0 hewie-look-char
            then
        then
    then
    2 ebit? not if
        3 -304.86 0.0 49.5 $14 5 0 zone
        $1A 3 8 char-zone-bits? if
            2 ebit-set
            $1A 3 6 char-sound
        then
    then
;

: front-garden-4.phase2 ( -- )   \ 00405850
    0 4 char-in-area? 0 -140 0 $3C char-faces-xz? and if
        5 3 0 scene-change
    then
    0 5 char-in-area? 0 90 $3C char-heading? and if
        5 4 0 scene-change
    then
    0 6 char-in-area? 0 0 $3C char-heading? and if
        5 5 0 scene-change
    then
    $222 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    $240 story-flag? $241 story-flag? not and if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: front-garden-4.act00 ( -- )   \ 004058B0
    self-wait-done
    $A8 -178.0 43.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 exit-door-open? if
        $1F -212.0 8.0 -180 $FFFF 5 self-move-to
        self-wait-done
        $1600 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 4 self-move-slot
        self-wait-done
        $FE 3 stalker-mode
        $30B story-flag-set
    then
    self-idle-or-end
;

: front-garden-4.act01 ( -- )   \ 00405900
    self-wait-done
    -202.89 -6.76 self-turn-to-xz
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
            $222 story-flag-set
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

: front-garden-4.act02 ( -- )   \ 00405960
    self-wait-done
    -283.8 15.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $241 story-flag-set
            1 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
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

: front-garden-4.act03 ( -- )   \ 004059C0
    self-wait-done
    -140.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        0 message
        wait-message
        1 ebit-set
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: front-garden-4.act04 ( -- )   \ 004059E0
    self-wait-done
    180 self-turn-angle
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: front-garden-4.act05 ( -- )   \ 004059F0
    self-wait-done
    0 story-flag? not if
        0 self-turn-angle
        self-wait-done
        4 message
        wait-message
    else
        $1C -190.323 61.999 0 $FFFF 5 self-move-to
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $1200 self-anim
        self-wait-anim
        0 0 6 char-sound
        $1203 self-anim
        self-frames-reset
        self-wait-16
        $1202 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        3 message
        wait-message
    then
    self-idle-or-end
;

: front-garden-4.act06 ( -- )   \ 00405A40
    3 char-unload
    $1A char-activate
    $1A -240.0 0.0 43.0 90 char-to-xyz
    $1A $9000 1 0 char-anim-hold
    0 front-garden-4.cmd00
    1 front-garden-4.cmd00
    3 0 char-remove
    self-idle-or-end
;

\ ---- registered ----
' front-garden-4.enter front-garden-4 0 room-script!
' front-garden-4.char-enter front-garden-4 6 room-script!
' front-garden-4.phase1 front-garden-4 1 room-script!
' front-garden-4.phase2 front-garden-4 2 room-script!
' front-garden-4.act00 front-garden-4 $00 action-script!
' front-garden-4.act01 front-garden-4 $01 action-script!
' front-garden-4.act02 front-garden-4 $02 action-script!
' front-garden-4.act03 front-garden-4 $03 action-script!
' front-garden-4.act04 front-garden-4 $04 action-script!
' front-garden-4.act05 front-garden-4 $05 action-script!
' front-garden-4.act06 front-garden-4 $06 action-script!
