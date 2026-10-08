\ story/rooms/house-of-truth-1f-8.fs - the event scripts of room house-of-truth-1f-8 ($91; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-8
USING: room-names story.words story.shared flag-names ;

\ room 0x91 (Room91_Cmd00_ptmf): door 0's +0x68 (0)
: house-of-truth-1f-8.cmd00 ( -- )  s" house-of-truth-1f-8.cmd00" stub-step ;
\ script variable 0 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0
: house-of-truth-1f-8.cmd01 ( -- )  s" house-of-truth-1f-8.cmd01" stub-step ;
\ room 0x91: three grey smoke effects (Effect79B00, size 70) at the room's spots 1, 3, 0xB
\ (grey_three).
: house-of-truth-1f-8.cmd02 ( -- )  s" house-of-truth-1f-8.cmd02" stub-step ;
\ room 0x91: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-8.cmd03 ( -- )  s" house-of-truth-1f-8.cmd03" stub-step ;

: house-of-truth-1f-8.enter ( -- )   \ 00436600
    room-sounds
    0 72.4 17.2 117.2 1 effect-86
    1 101.9 66.0 87.1 4 effect-86
    2 97.6 29.5 144.8 1 effect-86
    4 101.9 66.0 87.4 4 effect-86
    $A3 story-flag? if
        1 0 $20000 nav-group
        0 1 $14 door-bits
    then
    $34E story-flag? if
        5 1.0 0 bgm
    then
    $2F3 story-flag? $2F4 story-flag? not and if
        3 86.0 1.0 68.0 flicker-sprite
    then
    $80 exit-taken? not if
        shared.act95
        $A3 story-flag? if
            house-of-truth-1f-8.cmd02
        then
    else
        2 ebit-set
    then
;

: house-of-truth-1f-8.char-enter ( -- )   \ 00436690
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
            0 $1FC 22.0 100.0 -90 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 1 action
            0.0 sound-volume-scale
        then
    then
;

: house-of-truth-1f-8.phase1 ( -- )   \ 00436780
    2 ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        2 exit-prepare
    then
    $A3 story-flag? $A4 story-flag? not and if
        0 6 char-entered-area? 1 ebit? not and if
            1 ebit-set
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
    $EB door-locked? if
        4 -37.0 0.0 177.0 7 20 0 zone
        0 4 char-in-zone? if
            0 $F1 4 action
        then
        1 char-here? 1 2 char-C4? not and 0 $C char-in-area? and 1 $C char-in-area? and 0 -37 177 $3C char-faces-xz? and 0 control-action? and if
            hewie-stays? if
                0 1 5 action
            then
        then
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    $A6 story-flag? not if
        0 $B char-entered-area? if
            $A6 story-flag-set
            $91 0 467 $88 0 -1 $5A 0.0 creature-place
            $91 0 131 $89 0 -1 $FFA6 0.0 creature-place
        then
    then
    $2F3 story-flag? not if
        3 86.0 0.0 68.0 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 755 var-set
                $1A 756 var-set
                $1B 3 var-set
                $1C 86000 var-set
                $1D 1000 var-set
                $1E 68000 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    house-of-truth-1f-8.cmd03
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 48.0 0.0 115.0 0 0 sound
            else 1 1 var? if
                $40000006 6 48.0 0.0 115.0 0 0 sound
            else 1 2 var? if
                $40000007 6 48.0 0.0 115.0 0 0 sound
            else 1 3 var? if
                $40000008 6 48.0 0.0 115.0 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: house-of-truth-1f-8.phase2 ( -- )   \ 00436A00
    0 $A char-in-area? 0 -22 $32 char-heading? and if
        5 $84 3 scene-change
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 6 1 scene-change
        then
    then
    $2F3 story-flag? $2F4 story-flag? not and if
        3 3 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: house-of-truth-1f-8.phase3 ( -- )   \ 00436A40
    $A3 story-flag? 2 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        2 ebit-set
    then
;

: house-of-truth-1f-8.act00 ( -- )   \ 00436A60
    stalkers-stay state-flag-set
    1 self-scripted
    2 exit-prepare
    $F $54 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-1f-8.act01 ( -- )   \ 00436A80
    1 self-scripted
    0 exit-prepare
    self-wait-done
    $A4 story-flag-set
    $91 0 493 hewie-to-room
    1 $1ED 45 char-to-tri-facing
    $FE char-activate
    $FE $92 -1 2 stalker-to-room
    $FE 0 stalker-mode
    $FE $8D 1 room-doors-state
    hewie-no-attack state-flag-set
    camera-restart
    house-of-truth-1f-8.cmd02
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    $F $11 fade
    wait-fade
    0 $F1 3 action
    self-frames-reset
    8 self-wait-frames
    0 $43 5 char-sound
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    $1E self-wait-frames
    $802 self-anim
    2 message
    wait-message
    $34E story-flag-set
    5 1.0 0 bgm
    $34F story-flag-set
    $F1 action-end
    shared.act95
    0 5 var-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-8.act02 ( -- )   \ 00436B10
    self-wait-done
    86.0 68.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $75 message-param-room
        $75 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F4 story-flag-set
            3 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
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

: house-of-truth-1f-8.act03 ( -- )   \ 00436B70
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    0 0 $1E rumble
    1 $FF $1E rumble
    $31 $87 0.0 0.0 0.0 0 0 sound
    self-frames-reset
    begin
        $1E frames? not while
        1.0 camera-shake
        yield
    repeat
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    begin
        0.1 camera-shake
        yield
    again
;

: house-of-truth-1f-8.act04 ( -- )   \ 00436BD0
    0 exit-door-open? not if
        house-of-truth-1f-8.cmd01
        $91 5 -37.0 10.0 177.0 0 0 sound
        0 0 var? if
            $EB door-unlock
            house-of-truth-1f-8.cmd00
            $EB door-open-set
        then
    then
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: house-of-truth-1f-8.act05 ( -- )   \ 00436C10
    self-wait-done
    hewie-bark
    self-wait-done
    $202 -4.422 167.237 -73 $FFFF $A self-move-to
    self-wait-done
    $90 -32.0 177.5 0.5 50 hewie-go-to
    self-frames-reset
    8 self-wait-frames
    0 exit-door-open? not if
        $EB door-unlock
        $91 5 -37.0 10.0 177.0 0 0 sound
        house-of-truth-1f-8.cmd00
        $EB door-open-set
    then
    self-wait-done
    self-idle-or-end
;

: house-of-truth-1f-8.act06 ( -- )   \ 00436C60
    self-wait-done
    0 ebit? not if
        0 self-through-exit
        self-wait-done
        $60B self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        0 ebit-set
    then
    0 message
    wait-message
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-1f-8.enter house-of-truth-1f-8 0 room-script!
' house-of-truth-1f-8.char-enter house-of-truth-1f-8 6 room-script!
' house-of-truth-1f-8.phase1 house-of-truth-1f-8 1 room-script!
' house-of-truth-1f-8.phase2 house-of-truth-1f-8 2 room-script!
' house-of-truth-1f-8.phase3 house-of-truth-1f-8 3 room-script!
' house-of-truth-1f-8.act00 house-of-truth-1f-8 $00 action-script!
' house-of-truth-1f-8.act01 house-of-truth-1f-8 $01 action-script!
' house-of-truth-1f-8.act02 house-of-truth-1f-8 $02 action-script!
' house-of-truth-1f-8.act03 house-of-truth-1f-8 $03 action-script!
' house-of-truth-1f-8.act04 house-of-truth-1f-8 $04 action-script!
' house-of-truth-1f-8.act05 house-of-truth-1f-8 $05 action-script!
' house-of-truth-1f-8.act06 house-of-truth-1f-8 $06 action-script!
