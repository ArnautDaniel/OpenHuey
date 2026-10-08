\ story/rooms/old-mansion-2f-13.fs - the event scripts of room old-mansion-2f-13 ($6A; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-13
USING: room-names story.words story.shared flag-names ;

\ room 0x6A (D_00438CE0): a lit quad at x 120, z 47 .. 39, from 4 to 21
: old-mansion-2f-13.cmd00 ( b0 -- )  drop s" old-mansion-2f-13.cmd00" stub-step ;
\ room 0x6A (D_00438CF0): its object's animation (+0x74 forward, +0x78 back) at a point +0x7C
\ (0..1) by byte 3 - 0 / 1 from event variable 0 (12..30 over 18, 12..28 over 16), 2 / 3 at the
\ start / end
: old-mansion-2f-13.cmd01 ( b0 -- )  drop s" old-mansion-2f-13.cmd01" stub-step ;

: old-mansion-2f-13.enter ( -- )   \ 00438730
    room-sounds
    2 0 object-show
    $32D story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
        $32E story-flag? not if
            3 old-mansion-2f-13.cmd01
            1 old-mansion-2f-13.cmd00
        else
            2 old-mansion-2f-13.cmd01
            0 old-mansion-2f-13.cmd00
        then
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 old-mansion-2f-13.cmd00
        1 0 $2008000 nav-group
    then
    $2AD story-flag? $2AE story-flag? not and if
        0 88.5 1.0 71.56 flicker-sprite
    then
    1 $3FFF sound-volume
    0 9 0.4 0.0 0.2 0.5 zone-rect
;

: old-mansion-2f-13.char-enter ( -- )   \ 004387A0
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
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            0 0 0 action
            1 $59 char-in-room? if
                $6A 0 55 hewie-to-room
            then
        then
    then
;

: old-mansion-2f-13.phase1 ( -- )   \ 004388C0
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
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $A 2 2 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        2 exit-prepare
    then
    0 5 char-entered-area? 0 6 char-entered-area? or if
        0 exit-prepare
    then
    0 7 char-entered-area? 0 8 char-entered-area? or if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        3 0 char-remove
    then
    0 7 char-entered-area? if
        $1C 3 1 char-load
    then
    0 8 char-entered-area? if
        $56 story-flag? not $7B story-flag? not and if
            4 0 char-remove
        then
    then
    0 9 char-entered-area? if
        $56 story-flag? not $7B story-flag? not and if
            $21 4 3 char-load
        then
        3 exit-prepare
    then
    1 ebit? not if
        $32D story-flag? not $32E story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                $FE $E char-entered-area? $FE 0 char-action? and if
                    0 $FE 2 action
                then
            then
        then
    then
    $2AD story-flag? not if
        0 88.5 0.0 71.56 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 685 var-set
                $1A 686 var-set
                $1B 0 var-set
                $1C 88500 var-set
                $1D 1000 var-set
                $1E 71560 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    0 ebit? if
        $FE char-here? if
            0 ebit-clear
        then
    then
;

: old-mansion-2f-13.phase2 ( -- )   \ 004389F0
    0 exit-door-open? not 0 ebit? and if
        0 0 char-group-bit4? if
            0 scene-ending
        then
    then
    2 ebit? not if
        0 $E char-in-area? 0 45 $3C char-heading? and if
            $32D story-flag? not if
                $FE char-here? not if
                    5 1 5 scene-change
                else $FE $E char-in-area? $FE 0 40 chars-within? or if
                    $8016 scene-ending
                else
                    5 1 5 scene-change
                then then
            else
                5 3 0 scene-change
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        $FE char-busy? not if
            2 ebit-clear
        then
    else
        2 ebit-clear
    then then
    $2AD story-flag? $2AE story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
;

: old-mansion-2f-13.phase5 ( -- )   \ 00438A70
    2 ebit? if
        $32E story-flag-set
        $32D story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            $FE 1 stalker-rage
        then
    then
;

: old-mansion-2f-13.act00 ( -- )   \ 00438A90
    1 self-scripted
    0 ebit-set
    $F $41 fade
    0 $28 5 char-sound
    self-wait-done
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-13.act01 ( -- )   \ 00438AB0
    1 self-scripted
    1 ebit-set
    self-wait-done
    0 0 char-file-load
    $F8 114.64 43.0 90 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $32E story-flag? not if
        0 old-mansion-2f-13.cmd00
        0 0 var-set
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            0 old-mansion-2f-13.cmd01
            yield
            0 12 var? if
                0 0 6 char-sound
            then
            0 30 var? not while
            0 var-inc
        repeat
        self-wait-anim
        2 old-mansion-2f-13.cmd01
        $32E story-flag-set
    else
        0 0 var-set
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            1 old-mansion-2f-13.cmd01
            yield
            0 12 var? if
                0 0 6 char-sound
            then
            0 28 var? not while
            0 var-inc
        repeat
        self-wait-anim
        3 old-mansion-2f-13.cmd01
        $32E story-flag-clear
        1 old-mansion-2f-13.cmd00
    then
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-13.act02 ( -- )   \ 00438B60
    2 ebit-set
    self-wait-done
    $FE 1 char-file-load
    $F8 114.64 43.0 90 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 5 6 char-sound
    $8000 5 self-anim-blend
    self-wait-anim
    $E00 5 self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    $FE 1 6 char-sound
    $32D story-flag-set
    $FE 1 stalker-rage
    0 0 $14 door-bits
    1 1 $14 door-bits
    1 0 $2008000 nav-group
    1 old-mansion-2f-13.cmd00
    120.0 16.0 45.0 0 943208504 4 -15.5 scene-effect-8C
    120.0 16.0 41.0 0 943208504 4 -15.5 scene-effect-8C
    120.0 10.0 45.0 0 943208504 4 -9.5 scene-effect-8C
    120.0 10.0 41.0 0 943208504 4 -9.5 scene-effect-8C
    120.0 4.0 45.0 0 943208504 4 -3.5 scene-effect-8C
    120.0 4.0 41.0 0 943208504 4 -3.5 scene-effect-8C
    self-wait-anim
    2 ebit-clear
    self-idle-or-end
;

: old-mansion-2f-13.act03 ( -- )   \ 00438C48
    self-wait-done
    90 self-turn-angle
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: old-mansion-2f-13.act04 ( -- )   \ 00438C60
    self-wait-done
    88.5 71.56 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2AE story-flag-set
            0 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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
' old-mansion-2f-13.enter old-mansion-2f-13 0 room-script!
' old-mansion-2f-13.char-enter old-mansion-2f-13 6 room-script!
' old-mansion-2f-13.phase1 old-mansion-2f-13 1 room-script!
' old-mansion-2f-13.phase2 old-mansion-2f-13 2 room-script!
' old-mansion-2f-13.phase5 old-mansion-2f-13 5 room-script!
' old-mansion-2f-13.act00 old-mansion-2f-13 $00 action-script!
' old-mansion-2f-13.act01 old-mansion-2f-13 $01 action-script!
' old-mansion-2f-13.act02 old-mansion-2f-13 $02 action-script!
' old-mansion-2f-13.act03 old-mansion-2f-13 $03 action-script!
' old-mansion-2f-13.act04 old-mansion-2f-13 $04 action-script!
