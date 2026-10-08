\ story/rooms/house-of-truth-1f-10.fs - the event scripts of room house-of-truth-1f-10 ($9B; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-10
USING: room-names story.words story.shared ;

\ (as Room2D_Cmd02) three hanging things (+0x34 0..2), pushed by the square of Fiona's step past
\ 1, event flag 4 with sounds 4 / 5
: house-of-truth-1f-10.cmd00 ( b0 -- )  drop s" house-of-truth-1f-10.cmd00" stub-step ;

: house-of-truth-1f-10.enter ( -- )   \ 00443870
    room-sounds
    0 house-of-truth-1f-10.cmd00
    $2E1 story-flag? not if
        0 29.7 5.25 32.54 flicker-sprite
    then
;

: house-of-truth-1f-10.act04 ( -- )   \ 00443CF0
    $FE camera-follow
    1 ebit-clear
    $24 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $24 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $24 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $24 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 $FE 5 action
    else
        $78 1 item-cooldown
    then
    $24 pvar-inc
    exit
;

: house-of-truth-1f-10.char-enter ( -- )   \ 00443890
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
    0 self-is? if
        $FE exit-taken? if
            0 $7E 60.0 45.0 180 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 1 action
        then
        0 exit-taken? if
            2 map-page
        then
    then
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            house-of-truth-1f-10.act04
        else
            2 ebit-clear
        then
    then
;

: house-of-truth-1f-10.phase1 ( -- )   \ 00443910
    5 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 60.0 20.0 50.0 0 0 sound
            5 ebit-set
        then
    else
        $C0000008 6 60.0 20.0 50.0 0 0 sound
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $4000000A 6 58.0 21.0 -76.0 0 0 sound
            else 0 1 var? if
                $4000000B 6 58.0 21.0 -76.0 0 0 sound
            else 0 2 var? if
                $4000000C 6 58.0 21.0 -76.0 0 0 sound
            else 0 3 var? if
                $4000000D 6 58.0 21.0 -76.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    4 0 0 1 chars-area-camera
    5 2 2 1 chars-area-camera
    0 7 char-entered-area? if
        0 exit-prepare
    then
    0 8 char-entered-area? if
        $27 room-preload
    then
    0 9 char-entered-area? if
        1 map-page
    then
    0 9 char-left-area? if
        2 map-page
    then
    0 30.47 0.0 -9.97 $B 10 0 zone
    0 0 8 char-zone-bits? if
        1 house-of-truth-1f-10.cmd00
    then
    1 60.49 0.0 45.99 $14 18 0 zone
    3 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    3 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    1 60.49 0.0 45.99 $14 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: house-of-truth-1f-10.phase2 ( -- )   \ 00443AB0
    0 $A char-in-area? 0 0 $3C char-heading? and if
        $FE char-here? not if
            5 2 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 3 char-in-area? 0 90 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 6 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    $2E1 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: house-of-truth-1f-10.phase3 ( -- )   \ 00443B00
    2 camera-mode? not if
        38.0 78.0 -60.2 70.0 55.0 -60.2 38.0 60.0 -60.2 70.0 40.0 -60.2 lights-doorway
    then
    2 camera-mode? not 0 camera-mode? not and if
        90.0 90.0 -40.5 60.0 90.0 -40.5 90.0 52.0 -40.5 60.0 52.0 -40.5 lights-doorway
    then
    91.0 51.8 -63.0 91.0 51.8 -40.0 70.0 51.8 -63.0 70.0 51.8 -40.0 lights-doorway
;

: house-of-truth-1f-10.act00 ( -- )   \ 00443BA0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    60.0 55.0 self-turn-to-xz
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    0 message
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

: house-of-truth-1f-10.act01 ( -- )   \ 00443BE0
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

: house-of-truth-1f-10.act03 ( -- )   \ 00443CC0
    0 ebit? if
        2 avoid-prompt
    then
    9 state-flag-clear
    0 camera-follow
    $18 state-flag-set
    $11F 32.119 -37.94 180 $207 5 self-move-to
    self-wait-done
    4 $14 self-anim-blend
    self-wait-anim
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    self-idle-or-end
;

: house-of-truth-1f-10.act02 ( -- )   \ 00443C00
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 ebit-clear
    self-wait-done
    $11A 31.33 -24.45 0 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    $800 self-anim
    self-wait-anim
    $28 32.119 -7.5 0 $803 5 self-move-to
    self-wait-done
    $28 32.119 -7.5 180 $803 5 self-move-to
    self-wait-done
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] house-of-truth-1f-10.act03 goto
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
            9 state-flag-clear
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] house-of-truth-1f-10.act03 goto
            then
            $18 state-flag-set
            $11A 31.33 -24.45 180 $803 5 self-move-to
            self-wait-done
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            self-idle-or-end
        then
    again
;

: house-of-truth-1f-10.act05 ( -- )   \ 00443D40
    self-wait-done
    $11F 40.89 -25.76 -5 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: house-of-truth-1f-10.act06 ( -- )   \ 00443D70
    self-wait-done
    29.7 32.54 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E1 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

\ ---- registered ----
' house-of-truth-1f-10.enter house-of-truth-1f-10 0 room-script!
' house-of-truth-1f-10.char-enter house-of-truth-1f-10 6 room-script!
' house-of-truth-1f-10.phase1 house-of-truth-1f-10 1 room-script!
' house-of-truth-1f-10.phase2 house-of-truth-1f-10 2 room-script!
' house-of-truth-1f-10.phase3 house-of-truth-1f-10 3 room-script!
' house-of-truth-1f-10.act00 house-of-truth-1f-10 $00 action-script!
' house-of-truth-1f-10.act01 house-of-truth-1f-10 $01 action-script!
' house-of-truth-1f-10.act02 house-of-truth-1f-10 $02 action-script!
' house-of-truth-1f-10.act03 house-of-truth-1f-10 $03 action-script!
' house-of-truth-1f-10.act04 house-of-truth-1f-10 $04 action-script!
' house-of-truth-1f-10.act05 house-of-truth-1f-10 $05 action-script!
' house-of-truth-1f-10.act06 house-of-truth-1f-10 $06 action-script!
