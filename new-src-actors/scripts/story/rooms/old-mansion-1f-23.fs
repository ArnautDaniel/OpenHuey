\ story/rooms/old-mansion-1f-23.fs - the event scripts of room old-mansion-1f-23 ($6F; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-23
USING: room-names story.words story.shared ;

: old-mansion-1f-23.enter ( -- )   \ 0043A8F0
    room-sounds
    0 0 var-set
    1 0 var-set
    $335 story-flag? not if
        0 1 $14 door-bits
    then
    $29F story-flag? $2A0 story-flag? not and if
        0 0.03 1.0 -85.54 flicker-sprite
    then
    $2A1 story-flag? $2A2 story-flag? not and if
        1 109.8 -9.0 -94.58 flicker-sprite
    then
    1 $2300 sound-volume
    1 $9D $10000000 nav-tri-flags
    1 $8D $10000000 nav-tri-flags
    1 $2B $10000000 nav-tri-flags
    1 $90 $10000000 nav-tri-flags
    1 $8A $10000000 nav-tri-flags
    1 $C3 $10000000 nav-tri-flags
    1 $87 $10000000 nav-tri-flags
    1 $7C $10000000 nav-tri-flags
    1 $BA $10000000 nav-tri-flags
    1 $9E $10000000 nav-tri-flags
    1 $8F $10000000 nav-tri-flags
    1 $2E $10000000 nav-tri-flags
    1 $92 $10000000 nav-tri-flags
    1 $8C $10000000 nav-tri-flags
    1 $C6 $10000000 nav-tri-flags
    1 $88 $10000000 nav-tri-flags
    1 $C2 $10000000 nav-tri-flags
    1 $86 $10000000 nav-tri-flags
    1 $B8 $10000000 nav-tri-flags
    1 $7A $10000000 nav-tri-flags
    1 $BE $10000000 nav-tri-flags
    1 $80 $10000000 nav-tri-flags
    1 $C0 $10000000 nav-tri-flags
    1 $84 $10000000 nav-tri-flags
    1 $B3 $10000000 nav-tri-flags
    1 $1E $10000000 nav-tri-flags
    1 $9F $10000000 nav-tri-flags
    1 $42 $10000000 nav-tri-flags
    1 $BD $10000000 nav-tri-flags
    1 $7F $10000000 nav-tri-flags
    1 $BB $10000000 nav-tri-flags
    1 $7D $10000000 nav-tri-flags
;

: old-mansion-1f-23.act02 ( -- )   \ 0043AE10
    2 ebit-set
    1 ebit-clear
    $14 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $14 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $14 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $14 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 $FE 3 action
    else
        $78 1 item-cooldown
    then
    $14 pvar-inc
    exit
;

: old-mansion-1f-23.char-enter ( -- )   \ 0043AA40
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
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
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
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            old-mansion-1f-23.act02
        else
            2 ebit-clear
        then
    then
;

: old-mansion-1f-23.phase1 ( -- )   \ 0043AB10
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
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    0 3 char-entered-area? 0 5 char-entered-area? or if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    $29F story-flag? not if
        0 0.03 0.0 -85.54 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 671 var-set
                $1A 672 var-set
                $1B 0 var-set
                $1C 30 var-set
                $1D 1000 var-set
                $1E -85540 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    $2A1 story-flag? not if
        1 109.8 -10.0 -94.58 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 673 var-set
                $1A 674 var-set
                $1B 1 var-set
                $1C 109800 var-set
                $1D -9000 var-set
                $1E -94580 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 79.45 0.0 -93.49 0 0 sound
            else 0 1 var? if
                $40000004 6 79.45 0.0 -93.49 0 0 sound
            else 0 2 var? if
                $40000005 6 79.45 0.0 -93.49 0 0 sound
            else 0 3 var? if
                $40000006 6 79.45 0.0 -93.49 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: old-mansion-1f-23.phase2 ( -- )   \ 0043ACB0
    0 $B char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $C char-in-area? 0 -45 $3C char-heading? and if
        $FE char-here? not if
            5 0 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $335 story-flag? not if
        0 $D char-in-area? 0 45 $32 char-heading? and if
            5 6 4 scene-change
        then
    then
    $29F story-flag? $2A0 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $2A1 story-flag? $2A2 story-flag? not and if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: old-mansion-1f-23.act01 ( -- )   \ 0043ADE0
    0 ebit? if
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
    0 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-23.act00 ( -- )   \ 0043AD20
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $20 $8004 5 105.64 -103.65 -90 self-walk-anim
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
    0 ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 1 6 char-sound
    then
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] old-mansion-1f-23.act01 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? $FE char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] old-mansion-1f-23.act01 goto
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

: old-mansion-1f-23.act03 ( -- )   \ 0043AE60
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $20 113.0 -103.65 -90 $FFFF 5 self-move-to
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
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-1f-23.act04 ( -- )   \ 0043AEC0
    self-wait-done
    0.03 -85.54 self-turn-to-xz
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
            $2A0 story-flag-set
            0 effect-remove
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

: old-mansion-1f-23.act05 ( -- )   \ 0043AF20
    self-wait-done
    109.8 -94.58 self-turn-to-xz
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
            $2A2 story-flag-set
            1 effect-remove
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

: old-mansion-1f-23.act06 ( -- )   \ 0043AF80
    self-wait-done
    127.0 -125.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $335 story-flag-set
    0 0 $14 door-bits
    $1E message-param-room
    $1E 1 item-give-count
    0 $1E item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-23.phase5 ( -- )   \ 0047AEBC
;

\ ---- registered ----
' old-mansion-1f-23.enter old-mansion-1f-23 0 room-script!
' old-mansion-1f-23.char-enter old-mansion-1f-23 6 room-script!
' old-mansion-1f-23.phase1 old-mansion-1f-23 1 room-script!
' old-mansion-1f-23.phase2 old-mansion-1f-23 2 room-script!
' old-mansion-1f-23.act00 old-mansion-1f-23 $00 action-script!
' old-mansion-1f-23.act01 old-mansion-1f-23 $01 action-script!
' old-mansion-1f-23.act02 old-mansion-1f-23 $02 action-script!
' old-mansion-1f-23.act03 old-mansion-1f-23 $03 action-script!
' old-mansion-1f-23.act04 old-mansion-1f-23 $04 action-script!
' old-mansion-1f-23.act05 old-mansion-1f-23 $05 action-script!
' old-mansion-1f-23.act06 old-mansion-1f-23 $06 action-script!
' old-mansion-1f-23.phase5 old-mansion-1f-23 5 room-script!
