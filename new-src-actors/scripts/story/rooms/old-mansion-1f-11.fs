\ story/rooms/old-mansion-1f-11.fs - the event scripts of room old-mansion-1f-11 ($4E; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-11
USING: room-names story.words story.shared flag-names ;

\ room 0x4E (D_0040B4D8): a lit quad at x -44 .. -36, z 60, from 54 to 71
: old-mansion-1f-11.cmd00 ( b0 -- )  drop s" old-mansion-1f-11.cmd00" stub-step ;
\ room 0x4E: the curtain ("Curtain_2") animated by event variable 0 (var0_anim: byte 3 0
\ forward, 1 back, 2 / 3 back at rest).
: old-mansion-1f-11.cmd01 ( b0 -- )  drop s" old-mansion-1f-11.cmd01" stub-step ;
\ room 0x4E (D_0040B4F8): the 0x10-byte effect Room4EEffect_vtable made
: old-mansion-1f-11.cmd02 ( -- )  s" old-mansion-1f-11.cmd02" stub-step ;

: old-mansion-1f-11.enter ( -- )   \ 0040AE90
    room-sounds
    $11 1.0 0 bgm
    $45 story-flag? not if
        0 1 $14 door-bits
        1 0 $10020000 nav-group
    else
        1 1 $14 door-bits
        1 1 $10020000 nav-group
        1 2 $300000 nav-group
        1 char-here? if
            1 1 char-in-nav-group? 1 2 char-in-nav-group? or if
                1 $D char-to-tri
            then
        then
    then
    $32B story-flag? not if
        2 0 $14 door-bits
        3 1 $14 door-bits
        $32C story-flag? not if
            3 old-mansion-1f-11.cmd01
            1 old-mansion-1f-11.cmd00
        else
            2 old-mansion-1f-11.cmd01
            0 old-mansion-1f-11.cmd00
        then
    else
        2 1 $14 door-bits
        3 0 $14 door-bits
        1 old-mansion-1f-11.cmd00
        1 4 $2008000 nav-group
    then
    $299 story-flag? $29A story-flag? not and if
        0 13.96 51.0 -5.34 flicker-sprite
    then
    old-mansion-1f-11.cmd02
    1 $2300 sound-volume
    0 7 0.4 0.0 0.2 0.5 zone-rect
;

: old-mansion-1f-11.char-enter ( -- )   \ 0040AF40
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
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
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-11.phase1 ( -- )   \ 0040B040
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
        $76 story-flag? not if
            3 0 char-remove
        then
    then
    0 7 char-entered-area? if
        $76 story-flag? not if
            $26 3 $FF char-load
        then
        3 exit-prepare
    then
    1 ebit? not if
        $32B story-flag? not $32C story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                $FE $13 char-entered-area? $FE 0 char-action? and if
                    0 $FE 4 action
                then
            then
        then
    then
    $32B story-flag? not $32C story-flag? and if
        0 -39.93 50.0 59.59 $C 6 0 zone
        4 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        4 ebit-set
                        $32 chance? if
                            $1F 0 var-set
                            0 1 $93 action
                        then
                    then
                then
            then
        then
    then
    $299 story-flag? not if
        3 13.96 50.0 -5.34 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 665 var-set
                $1A 666 var-set
                $1B 0 var-set
                $1C 13960 var-set
                $1D 51000 var-set
                $1E -5340 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: old-mansion-1f-11.phase2 ( -- )   \ 0040B160
    $45 story-flag? if
        0 $10 char-in-area? 0 0 $32 char-heading? and if
            5 0 0 scene-change
        then
        0 $11 char-in-area? 0 90 $32 char-heading? and if
            5 1 0 scene-change
        then
    else 0 $12 char-in-area? 0 -45 $32 char-heading? and if
        5 2 0 scene-change
    then then
    2 ebit? not if
        0 $13 char-in-area? 0 0 $3C char-heading? and if
            $32B story-flag? not if
                $FE char-here? not if
                    5 3 5 scene-change
                else $FE $13 char-in-area? $FE 0 40 chars-within? or if
                    $8016 scene-ending
                else
                    5 3 5 scene-change
                then then
            else
                5 5 0 scene-change
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        $FE char-busy? not if
            2 ebit-clear
        then
    else
        2 ebit-clear
    then then
    $299 story-flag? $29A story-flag? not and if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: old-mansion-1f-11.phase5 ( -- )   \ 0040B200
    2 ebit? if
        $32C story-flag-set
        $32B story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            $FE 1 stalker-rage
        then
    then
    1 char-here? 1 2 char-C4? and if
        1 1 char-in-nav-group? if
            1 1 char-heal
            1 0 char-set-C4
        then
    then
;

: old-mansion-1f-11.act00 ( -- )   \ 0040B230
    self-wait-done
    0 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-11.act01 ( -- )   \ 0040B240
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-11.act02 ( -- )   \ 0040B250
    0 ebit? not if
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        1 message
        wait-message
        0 ebit-set
    else
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-11.act03 ( -- )   \ 0040B2A0
    1 self-scripted
    1 ebit-set
    self-wait-done
    0 0 char-file-load
    $56 -40.0 54.64 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $32C story-flag? not if
        0 old-mansion-1f-11.cmd00
        0 0 var-set
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            0 old-mansion-1f-11.cmd01
            yield
            0 30 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        2 old-mansion-1f-11.cmd01
        $32C story-flag-set
    else
        0 0 var-set
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            1 old-mansion-1f-11.cmd01
            yield
            0 28 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        3 old-mansion-1f-11.cmd01
        $32C story-flag-clear
        1 old-mansion-1f-11.cmd00
    then
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-11.act04 ( -- )   \ 0040B350
    2 ebit-set
    self-wait-done
    $FE 1 char-file-load
    $56 -40.0 54.64 0 $FFFF 5 self-move-to
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
    $32B story-flag-set
    $FE 1 6 char-sound
    $FE 1 stalker-rage
    2 1 $14 door-bits
    3 0 $14 door-bits
    1 4 $2008000 nav-group
    1 old-mansion-1f-11.cmd00
    -37.0 66.0 60.0 0 538976288 4 -15.5 scene-effect-8C
    -41.0 66.0 60.0 0 538976288 4 -15.5 scene-effect-8C
    -37.0 60.0 60.0 0 538976288 4 -9.5 scene-effect-8C
    -41.0 60.0 60.0 0 538976288 4 -9.5 scene-effect-8C
    -37.0 54.0 60.0 0 538976288 4 -3.5 scene-effect-8C
    -41.0 54.0 60.0 0 538976288 4 -3.5 scene-effect-8C
    self-wait-anim
    2 ebit-clear
    self-idle-or-end
;

: old-mansion-1f-11.act05 ( -- )   \ 0040B438
    self-wait-done
    0 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-11.act06 ( -- )   \ 0040B450
    self-wait-done
    13.96 -5.34 self-turn-to-xz
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
            $29A story-flag-set
            0 effect-remove
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

\ ---- registered ----
' old-mansion-1f-11.enter old-mansion-1f-11 0 room-script!
' old-mansion-1f-11.char-enter old-mansion-1f-11 6 room-script!
' old-mansion-1f-11.phase1 old-mansion-1f-11 1 room-script!
' old-mansion-1f-11.phase2 old-mansion-1f-11 2 room-script!
' old-mansion-1f-11.phase5 old-mansion-1f-11 5 room-script!
' old-mansion-1f-11.act00 old-mansion-1f-11 $00 action-script!
' old-mansion-1f-11.act01 old-mansion-1f-11 $01 action-script!
' old-mansion-1f-11.act02 old-mansion-1f-11 $02 action-script!
' old-mansion-1f-11.act03 old-mansion-1f-11 $03 action-script!
' old-mansion-1f-11.act04 old-mansion-1f-11 $04 action-script!
' old-mansion-1f-11.act05 old-mansion-1f-11 $05 action-script!
' old-mansion-1f-11.act06 old-mansion-1f-11 $06 action-script!
