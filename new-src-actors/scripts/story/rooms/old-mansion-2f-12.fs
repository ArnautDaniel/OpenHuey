\ story/rooms/old-mansion-2f-12.fs - the event scripts of room old-mansion-2f-12 ($69; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-12
USING: room-names story.words story.shared flag-names ;

\ room 0x69 (D_004386F8): the three-dial lock by byte 3 - 0 the dials (and their lit twins) set
\ to their settings; 1 the player at it: up / down pick the dial (event variable 0; its lit twin
\ shown), left / right turn it (its setting, sound bit 3), cancel leaves (event bit 2); 2 the
\ picked dial turns 4 degrees a frame to its setting, then - 1, 0, 2 - the lock opens (event
\ bits 2 off, 4)
: old-mansion-2f-12.cmd00 ( b0 -- )  drop s" old-mansion-2f-12.cmd00" stub-step ;

: old-mansion-2f-12.enter ( -- )   \ 00437E50
    room-sounds
    0 old-mansion-2f-12.cmd00
    3 1 object-show
    4 1 object-show
    5 1 object-show
    $2AF story-flag? $2B0 story-flag? not and if
        0 82.73 31.0 -85.2 flicker-sprite
    then
    1 -31.0 51.9 -132.6 1 effect-86
    2 0.0 51.9 -142.7 1 effect-86
    3 30.9 51.8 -132.6 1 effect-86
    4 50.0 51.9 -106.3 1 effect-86
    5 -50.0 51.9 -106.3 1 effect-86
    6 82.0 51.9 -62.7 1 effect-86
    7 160.0 51.9 -62.7 1 effect-86
    8 215.7 21.9 -62.6 1 effect-86
    $2CC story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $2CD story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    0 $C 0.812 0.687 0.187 0.312 zone-rect
    1 $3FFF sound-volume
;

: old-mansion-2f-12.char-enter ( -- )   \ 00437F50
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
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
;

: old-mansion-2f-12.phase1 ( -- )   \ 00437FD0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 2 1 1 chars-area-camera
    5 3 -1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 1 1 chars-area-camera
    8 0 0 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $2AF story-flag? not if
        2 82.73 30.0 -85.2 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 687 var-set
                $1A 688 var-set
                $1B 0 var-set
                $1C 82730 var-set
                $1D 31000 var-set
                $1E -85200 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    3 -103.36 30.0 -72.17 $19 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 288.99 0.0 -98.66 $19 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $2CC story-flag? not if
        0 -114.0 30.0 -48.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $2CC story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -114.0 30.0 -48.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -114.0 30.0 -48.0 0 0 sound
            $40 $DD noise
        then
    then
    $2CD story-flag? not if
        1 -114.0 30.0 -42.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $2CD story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -114.0 30.0 -42.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -114.0 30.0 -42.0 0 0 sound
            $40 $DF noise
        then
    then
    6 sound-bank-loaded? if
        3 var-inc
        3 30 var? if
            3 0 var-set
            2 0 var? if
                $40000005 6 74.0 30.0 -110.0 0 0 sound
            else 2 1 var? if
                $40000006 6 74.0 30.0 -110.0 0 0 sound
            else 2 2 var? if
                $40000007 6 74.0 30.0 -110.0 0 0 sound
            else 2 3 var? if
                $40000008 6 74.0 30.0 -110.0 0 0 sound
            then then then then
            2 var-inc
            2 4 var? if
                2 0 var-set
            then
        then
    then
;

: old-mansion-2f-12.phase2 ( -- )   \ 00438230
    0 $A $32 char-faces-area? if
        5 2 0 scene-change
    then
    0 $B char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 $C char-in-area? 0 22 $3C char-heading? and if
        5 6 0 scene-change
    then
    -2147483646 scene-request? 0 0 char-group-bit4? and if
        5 1 1 scene-change
    then
    0 $D char-in-area? 0 50 $32 char-heading? and if
        5 $84 3 scene-change
    then
    $2AF story-flag? $2B0 story-flag? not and if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: old-mansion-2f-12.phase3 ( -- )   \ 00438290
    251.4 20.0 -115.0 251.4 20.0 -90.0 251.4 0.0 -115.0 251.4 0.0 -90.0 lights-doorway
;

: old-mansion-2f-12.act00 ( -- )   \ 004382D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 4 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message
    wait-message
    $902 self-anim
    self-wait-anim
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act01 ( -- )   \ 00438340
    self-wait-done
    0 self-through-exit
    self-wait-done
    5 ebit? not 2 game-mode? or $FF panic-stage? or if
        $FF panic-stage? 2 game-mode? or if
            $60A self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $608 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            9 message
            wait-message
        then
        5 ebit-set
    else
        $800 self-anim
        self-wait-anim
        $801 self-anim
        $A message
        wait-message
        $802 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        5 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-12.act02 ( -- )   \ 00438390
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $54 story-flag? not if
        1 message
        wait-message
        0 answer? if
            0 $4B 293.136 -95.0 180 char-to-xz
            world-frozen state-flag-set
            1 5.0 0.0 0.0 2.0 event-camera
            2 message
            2 ebit-set
            0 0 var-set
            0 1 object-show
            3 0 object-show
            begin
                3 ebit? not if
                    1 old-mansion-2f-12.cmd00
                    3 ebit? if
                        1 6 293.0 12.0 -102.0 0 0 sound
                    then
                    6 ebit? if
                        0 6 293.0 12.0 -102.0 0 0 sound
                    then
                else
                    2 old-mansion-2f-12.cmd00
                then
                2 ebit? while
                yield
            repeat
            2 message-close
            world-frozen state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
            0 0 object-show
            1 0 object-show
            2 0 object-show
            3 1 object-show
            4 1 object-show
            5 1 object-show
            4 ebit? if
                2 6 318.0 12.0 -80.0 0 0 sound
                1 $FF $D2 rumble
                1 self-anim
                self-wait-anim
                0 self-anim
                self-frames-reset
                $32 self-wait-frames
                318.0 10.0 -80.0 self-look-at-point
                yield
                $72 5 318.0 12.0 -80.0 0 0 sound
                $74 door-unlock
                $62 door-unlock
                $54 story-flag-set
                self-frames-reset
                $1E self-wait-frames
                0 3 6 char-sound
                self-frames-reset
                $5A self-wait-frames
                $FF self-look-at
                yield
            else $333 story-flag? if
                $23B item-give
            then then
        else $333 story-flag? if
            $23B item-give
        then then
    else
        7 message
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act03 ( -- )   \ 004384E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 ebit? not if
        $F 6 fade
        wait-fade
        3 message
        wait-message
        1 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        1 ebit-clear
    then
    $244 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act04 ( -- )   \ 00438530
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $7F -103.0 -75.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 4 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    5 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-12.act05 ( -- )   \ 00438650
    self-wait-done
    82.73 -85.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message
            wait-message
        else
            $2B0 story-flag-set
            0 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
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

: old-mansion-2f-12.act06 ( -- )   \ 004386B0
    self-wait-done
    -69.5 -46.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-12.phase5 ( -- )   \ 0047AE7C
;

\ ---- registered ----
' old-mansion-2f-12.enter old-mansion-2f-12 0 room-script!
' old-mansion-2f-12.char-enter old-mansion-2f-12 6 room-script!
' old-mansion-2f-12.phase1 old-mansion-2f-12 1 room-script!
' old-mansion-2f-12.phase2 old-mansion-2f-12 2 room-script!
' old-mansion-2f-12.phase3 old-mansion-2f-12 3 room-script!
' old-mansion-2f-12.act00 old-mansion-2f-12 $00 action-script!
' old-mansion-2f-12.act01 old-mansion-2f-12 $01 action-script!
' old-mansion-2f-12.act02 old-mansion-2f-12 $02 action-script!
' old-mansion-2f-12.act03 old-mansion-2f-12 $03 action-script!
' old-mansion-2f-12.act04 old-mansion-2f-12 $04 action-script!
' old-mansion-2f-12.act05 old-mansion-2f-12 $05 action-script!
' old-mansion-2f-12.act06 old-mansion-2f-12 $06 action-script!
' old-mansion-2f-12.phase5 old-mansion-2f-12 5 room-script!
