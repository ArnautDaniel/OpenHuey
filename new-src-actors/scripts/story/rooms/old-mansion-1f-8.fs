\ story/rooms/old-mansion-1f-8.fs - the event scripts of room old-mansion-1f-8 ($48; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-8
USING: room-names story.words story.shared ;

\ room 0x48 (D_004267F8): the handler's objects 2 and 3 swing (phases 60 degrees apart) - byte 3
\ 0 sets them still; 1: while slower than 5, Hewie's movement (the squared length of his last
\ step, +0x3C) past 1 makes them swing for 20 frames (the first also creaks: sounds 4 / 5 by
\ turns, event bit 0x11); the tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36
: old-mansion-1f-8.cmd00 ( b0 -- )  drop s" old-mansion-1f-8.cmd00" stub-step ;
\ room 0x48 (Room48_Cmd01_ptmf): character 0x26's +0xE4 cleared
: old-mansion-1f-8.cmd01 ( -- )  s" old-mansion-1f-8.cmd01" stub-step ;
\ room 0x48 (D_00426818): byte 3 0 starts the wall shadow (WallShadow_vtable, its slot in event
\ variable 2); else that one is ended
: old-mansion-1f-8.cmd02 ( b0 -- )  drop s" old-mansion-1f-8.cmd02" stub-step ;
\ room 0x48 (Room48_Cmd03_ptmf): character 0xFE's model +0x9E8 = -0.15 (byte 3 0) or 0
: old-mansion-1f-8.cmd03 ( b0 -- )  drop s" old-mansion-1f-8.cmd03" stub-step ;
\ room 0x48 (Room48_Cmd05_ptmf): the creatures (10) in play in the current room: the list's
\ +0x2C
: old-mansion-1f-8.cmd05 ( -- )  s" old-mansion-1f-8.cmd05" stub-step ;
\ room 0x48 (Room48_Cond00_ptmf): door 0 of room 0x48 (Progress_CurRoomFlag)
: old-mansion-1f-8.cond00? ( -- flag )  s" old-mansion-1f-8.cond00?" stub-flag ;

: old-mansion-1f-8.enter ( -- )   \ 004252D0
    room-sounds
    $19 1.0 0 bgm
    1 0 $300000 nav-group
    $7D door-locked? if
        0 4 $200000 nav-group
        1 4 $100000 nav-group
    then
    $26E story-flag? not if
        0 122.09 8.28 10.26 flicker-sprite
    then
    $26F story-flag? not if
        1 -131.47 1.0 9.32 flicker-sprite
    then
    $270 story-flag? not if
        2 -140.37 16.0 -25.97 flicker-sprite
    then
    8 1 item-count? not 9 1 item-count? not and $A 1 item-count? not and $B 1 item-count? not and $C 1 item-count? not and $10 1 item-count? not and if
        0 ebit-set
        3 -111.0 17.0 -115.0 flicker-sprite
    then
    $50 story-flag? if
        $79 story-flag? not if
            $79 story-flag-set
            $48 0 97 4 8 -1 0 0.0 creature-place
            $48 0 488 4 8 -1 0 0.0 creature-place
        then
    then
    1 char-here? 119 hewie-action? and if
        0 2 8 nav-group
        1 2 $B0 nav-group
        1 8 char-on-tri? not if
            1 8 76.218 -13.075 90 char-to-xz
            1800 2 hewie-anim
        then
    else
        0 2 $30 nav-group
        1 2 $88 nav-group
        1 2 char-in-nav-group? if
            1 $175 char-to-tri
        then
    then
    0 old-mansion-1f-8.cmd00
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    $75 story-flag? $76 story-flag? not and if
        $FE action-end
        $FE char-done
    then
    $76 story-flag? not if
        1 3 $20000 nav-group
        $26 3 $FF char-load
        $80 story-flag? if
            0 $F1 $1C action
        then
        0 old-mansion-1f-8.cmd02
    then
;

: old-mansion-1f-8.char-enter ( -- )   \ 00425460
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
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    2 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 5 exit-taken? and if
            0 5 char-to-exit
            hewie-controlled? not if
                0 5 5 char-camera
                0 camera-follow
            else
                1 5 5 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 5 exit-taken? and if
        1 5 char-to-exit
        hewie-controlled? not if
            0 5 5 char-camera
            0 camera-follow
        else
            1 5 5 char-camera
            1 camera-follow
        then
    then then
    5 5 5 area-camera
    0 self-is? if
        0 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-8.act23 ( -- )   \ 00426720
    $79 $89 door-copy
    $79 door-reopen-unlock
    exits-rebuild
    0 4 $100000 nav-group
    1 4 $200000 nav-group
    1 char-here? if
        0 hewie-side
    then
    $FE char-here? if
        $FE 0 stalker-mode
    else stalker-active? $FE 2 char-C4? not and if
        $FE $FFFF -1 2 stalker-to-room
    then then
    old-mansion-1f-8.cmd05
    $12 ebit-clear
    exit
;

: old-mansion-1f-8.phase1 ( -- )   \ 00425520
    $10 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 -138.0 8.0 12.0 0 0 sound
            $10 ebit-set
        then
    else
        $C0000000 6 -138.0 8.0 12.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 5 char-in-area? 0 char-busy? not and if
            5 exit-check
        then
    else 1 5 char-in-area? 1 char-busy? not and if
        5 exit-check
    then then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    $B 5 5 1 chars-area-camera
    0 $C char-entered-area? if
        0 exit-prepare
    then
    0 $D char-entered-area? if
        2 exit-prepare
    then
    0 $E char-entered-area? if
        5 exit-prepare
    then
    0 $C char-entered-area? if
        $1C 3 0 char-load
    then
    0 $C char-left-area? if
        3 0 char-remove
    then
    0 6 char-entered-area? if
        2 map-page
    then
    0 6 char-left-area? if
        1 map-page
    then
    3 75.003 7.0 -9.266 $E 20 0 zone
    1 3 8 char-zone-bits? if
        1 old-mansion-1f-8.cmd00
    then
    1 char-here? if
        118 hewie-action? not if
            35 fiona-started? 1 2 char-C4? not and if
                75 7 -9 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and $FE char-here? not and 0 75 -9 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 9 action
                    then
                then
            then
        else
            44 fiona-started? if
                0 1 $A action
            then
            $FF panic-stage? not if
                0 char-busy? not $A state-flag? or if
                    3 stalker-kind-here? $22 stalker-kind-here? or if
                        $FE 2 char-C4? not if
                            75 7 -9 point-on-camera? not 0 char-unseen? not and $FE char-unseen? not and if
                                0 counter-set
                                0 1 $C action
                                0 $FE $D action
                                $FF panic-stage? if
                                    3 panic-stage
                                then
                                0 0 char-action? if
                                    0 0 $B action-force
                                else
                                    1 0 $B action-force
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    $13 ebit? 1 char-busy? not and if
        1 ebit-clear
        $E ebit-clear
    then
    0 ebit? if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not $FE char-here? not and if
                0 control-action? if
                    -111 17 -115 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                        0 -111 -115 $32 char-faces-xz? if
                            hewie-stays? if
                                0 0 5 action
                            then
                        then
                    then
                then
            then
        then
    then
    $2C4 story-flag? not if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not if
                0 control-action? if
                    75 7 95 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                        0 75 95 $32 char-faces-xz? if
                            hewie-stays? if
                                0 0 7 action
                            then
                        then
                    then
                then
            then
        then
    then
    9 ebit? not if
        4 80.25 7.0 -6.0 $10 4 0 zone
        0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
            0 5 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
        1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
            1 $A 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
        $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
            $FE 5 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
    then
    $A ebit? not if
        5 67.4 7.0 -6.7 $10 4 0 zone
        0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
            0 5 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
        1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
            1 $A 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
        $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
            $FE 5 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
    then
    0 ebit? if
        6 -111.73 15.0 -115.37 $3C 6 0 zone
        $B ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 6 9 char-zone-bits? if
                        $B ebit-set
                        $64 chance? if
                            $1F 6 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        6 -111.73 15.0 -115.37 $3C 6 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 6 9 char-zone-bits? if
                    $1F 6 var-set
                    $1F 0.0 hewie-look-zone
                then
            then
        then
    then
    $2C4 story-flag? not if
        7 77.32 0.0 95.28 $3C 10 0 zone
        $C ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 7 9 char-zone-bits? if
                        $C ebit-set
                        $64 chance? if
                            $1F 7 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        7 77.32 0.0 95.28 $3C 10 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 7 9 char-zone-bits? if
                    $1F 7 var-set
                    $1F 8.0 hewie-look-zone
                then
            then
        then
    then
    $12 ebit? if
        old-mansion-1f-8.cond00? not if
            old-mansion-1f-8.act23
        then
    then
;

: old-mansion-1f-8.phase2 ( -- )   \ 004258D0
    -2147483646 scene-request? if
        0 1 char-group-bit4? 0 1 char-in-area? and if
            5 1 1 scene-change
        then
    then
    0 ebit? if
        0 $F char-in-area? 0 -60 $3C char-heading? and if
            5 $1D 0 scene-change
        then
    then
    $26E story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
    $270 story-flag? not if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    0 $18 char-in-area? 0 -40 $3C char-heading? and if
        5 $E 0 scene-change
    then
    0 $10 char-in-area? 0 0 $3C char-heading? and if
        5 $F 0 scene-change
    then
    0 $11 char-in-area? 0 -40 $3C char-heading? and if
        5 $10 0 scene-change
    then
    0 $12 char-in-area? 0 -60 $3C char-heading? and if
        5 $11 0 scene-change
    then
    0 $13 char-in-area? 0 45 $3C char-heading? and if
        5 $12 0 scene-change
    then
    0 $14 char-in-area? 0 62 $3C char-heading? and if
        5 $13 0 scene-change
    then
    0 $15 char-in-area? 0 90 $3C char-heading? and if
        5 $14 0 scene-change
    then
    0 $16 char-in-area? 0 -30 $3C char-heading? and if
        5 $15 0 scene-change
    then
    0 $17 char-in-area? 0 45 $3C char-heading? and if
        5 $16 0 scene-change
    then
;

: old-mansion-1f-8.phase5 ( -- )   \ 004259B0
    1 char-busy? if
        1 action-end
        1 ebit? if
            1 $1E7 94.1 -5.96 90 char-to-xz
        else $D ebit? if
            1 $BC char-to-tri
            $18 state-flag-clear
        else $E ebit? if
            1 $64 char-to-tri
            $18 state-flag-clear
        then then then
    then
    $12 ebit? if
        old-mansion-1f-8.act23
    then
;

: old-mansion-1f-8.act00 ( -- )   \ 004259F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $18 message-param-room
    $18 item-use
    $7C door-unlock
    $76 story-flag-set
    0 3 $20000 nav-group
    $F $54 fade
    $FF 1.0 0 bgm
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    8 state-flag-set
    1 char-here? if
        $F ebit-set
        1 action-end
        1 char-done
    then
    $F1 action-end
    3 char-unload
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    $F $51 fade
    $19 1.0 0 bgm
    $F ebit? if
        1 char-activate
        $F ebit-clear
        1 $19F 140 char-to-tri-facing
    then
    3 0 char-remove
    1 old-mansion-1f-8.cmd02
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    0 state-flag-set
    $FE $48 0 room-doors-state
    $FE $66 1 room-doors-state
    self-idle-or-end
;

: old-mansion-1f-8.act01 ( -- )   \ 00425AE0
    self-wait-done
    $7D self-through-door
    self-wait-done
    0 $7D char-not-at-door? if
        $609 self-anim
    else
        $608 self-anim
    then
    self-frames-reset
    $14 self-wait-frames
    0 $72 5 char-sound
    $7D door-unlock
    $12 ebit-set
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8008 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-8.act02 ( -- )   \ 00425B10
    self-wait-done
    122.09 10.26 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $26E story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: old-mansion-1f-8.act03 ( -- )   \ 00425B70
    self-wait-done
    -131.47 9.32 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $26F story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: old-mansion-1f-8.act04 ( -- )   \ 00425BD0
    self-wait-done
    -140.37 -25.97 self-turn-to-xz
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
            $270 story-flag-set
            2 effect-remove
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

: old-mansion-1f-8.act05 ( -- )   \ 00425C30
    0 counter-set
    0 1 6 action-force
    $D ebit-set
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C00 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 wait-counter
    self-frames-reset
    self-wait-16
    self-frames-reset
    self-wait-16
    $8284 item-give
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act06 ( -- )   \ 00425C70
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $130 -83.0 -83.0 -120 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $133 -111.0 -108.0 180 $204 5 self-move-to
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    3 effect-remove
    0 ebit-clear
    10 hewie-trust
    8 message-param-room
    8 1 item-give-count
    0 8 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    counter-inc
    0 self-turn-angle
    self-wait-done
    $130 -83.0 -83.0 60 $FFFF $A self-move-to
    self-wait-done
    $AF story-flag? if
        5 0 creature-count
    then
    0 self-noclip
    $18 state-flag-clear
    $D ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act07 ( -- )   \ 00425D00
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 8 action-force
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

: old-mansion-1f-8.act08 ( -- )   \ 00425D30
    1 self-scripted
    $E ebit-set
    $13 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $67 149.0 91.0 64 $FFFF $A self-move-to
    self-wait-done
    -84 self-turn-angle
    self-wait-done
    $13 ebit-clear
    1 self-noclip
    $66 129.619 91.48 1.7 10 hewie-go-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $2C4 story-flag? not if
        $A state-flag? 0 char-busy? not or if
            $97 $63 item-count? not if
                10 hewie-trust
            then
            $97 message-param-room
            $97 $63 item-count? if
                $8010 message
                wait-message
            else
                $2C4 story-flag-set
                $97 1 item-give-count
                0 $97 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
        then
    then
    $1ED 43.7 110.48 -90 $FFFF $A self-move-to
    self-wait-done
    110 self-turn-angle
    self-wait-done
    $1EE 76.12 104.76 0.5 5 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    $E ebit-clear
    self-idle-or-end
;

: old-mansion-1f-8.act09 ( -- )   \ 00425DF0
    $13 ebit-set
    1 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $175 92.855 -13.871 -90 $FFFF $A self-move-to
    self-wait-done
    $13 ebit-clear
    1 self-scripted
    0 2 8 nav-group
    1 2 $30 nav-group
    8 76.218 -13.075 -90 $204 5 self-move-to
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $102 $A self-anim-blend
    self-wait-anim
    3600 2 hewie-anim
    1 ebit-clear
    self-idle-or-end
;

: old-mansion-1f-8.act0A ( -- )   \ 00425E40
    1 self-scripted
    1 ebit-set
    self-wait-done
    9 ebit-clear
    $A ebit-clear
    $175 92.855 -13.871 90 $204 5 self-move-to
    self-wait-done
    -1 self-move-16
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-action
    0 2 $30 nav-group
    1 2 8 nav-group
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act0B ( -- )   \ 00425E80
    $18 state-flag-set
    1 self-noclip
    1 self-scripted
    $F $54 fade
    $FF 1.0 0 bgm
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    3 4 0 char-model-op
    $1B state-flag-set
    yield
    $1B state-flag-clear
    0 1 char-no-shadow
    0 $F9 $1F action
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    1 old-mansion-1f-8.cmd03
    $FE action-end
    3 summon-take
    5 hewie-trust
    8 state-flag-set
    0 $1BB 112.81 45.98 32 char-to-xz
    hewie-controlled? not if
        0 3 3 char-camera
        0 camera-follow
    else
        1 3 3 char-camera
        1 camera-follow
    then
    0 0 char-no-shadow
    yield
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    $23A item-give
    $8280 item-give
    $31 resident-flag-set
    counter-inc
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: old-mansion-1f-8.act0C ( -- )   \ 00425F70
    1 wait-counter
    0 0 hewie-anim
    0 0 hewie-action
    1 $1DE 100.17 -23.46 -120 char-to-xz
    $1D state-flag-clear
    self-idle-or-end
;

: old-mansion-1f-8.act0D ( -- )   \ 0047AD30
    1 wait-counter
    self-idle-or-end
;

: old-mansion-1f-8.act0E ( -- )   \ 00425FA0
    self-wait-done
    -154.0 -28.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    2 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        1 message
        wait-message
    then
    2 ebit? not if
        0 var-inc
    then
    2 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act0F ( -- )   \ 00425FE0
    self-wait-done
    -86.29 -39.06 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        2 message
        wait-message
    then
    3 ebit? not if
        0 var-inc
    then
    3 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act10 ( -- )   \ 00426020
    self-wait-done
    40.56 -52.11 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    4 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        3 message
        wait-message
    then
    4 ebit? not if
        0 var-inc
    then
    4 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act11 ( -- )   \ 00426060
    self-wait-done
    77.34 6.2 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    5 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        4 message
        wait-message
    then
    5 ebit? not if
        0 var-inc
    then
    5 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act12 ( -- )   \ 004260A0
    self-wait-done
    127.66 -13.38 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    6 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        5 message
        wait-message
    then
    6 ebit? not if
        0 var-inc
    then
    6 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act13 ( -- )   \ 004260E0
    self-wait-done
    144.96 39.88 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    7 ebit? 0 0 var? not and 0 1 var? not and if
        7 message
        wait-message
    else
        6 message
        wait-message
    then
    7 ebit? not if
        0 var-inc
    then
    7 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act14 ( -- )   \ 00426120
    self-wait-done
    $47 138.0 -98.0 180 $FFFF 5 self-move-to
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 ebit? not if
        8 message
        wait-message
        $F 6 fade
        wait-fade
        9 message
        wait-message
        $F 7 fade
        wait-fade
        8 ebit-set
    else
        $A message
        wait-message
        8 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-8.act15 ( -- )   \ 00426160
    self-wait-done
    73.11 -9.81 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 char-here? 118 hewie-action? and 1 8 char-on-tri? and if
        $C message
        wait-message
    else
        $B message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-8.act16 ( -- )   \ 00426190
    self-wait-done
    $1A2 113.0 -122.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 30.0 0.0 event-camera
    1 self-scripted
    $76 story-flag? not if
        $80 story-flag? not if
            $18 state-flag-set
            0 8 char-file-load
            $14 message
            wait-message
            0 answer? if
                0 char-file-use
                3 char-unload
                $26 char-activate
                $26 127.567 0.0 -123.393 -90 char-to-xyz
                old-mansion-1f-8.cmd01
                $A02 self-anim
                self-wait-anim
                0 $F1 $1E action
                self-frames-reset
                $F self-wait-frames
                0 $43 5 char-sound
                $8000 self-anim
                self-wait-anim
                3 panic-stage? not if
                    3 panic-stage
                then
                $8001 self-anim
                self-wait-anim
                $8003 self-anim
                self-wait-anim
                $80 story-flag-set
            then
            $18 state-flag-clear
        else
            1 0 var? if
                $D message
                wait-message
            then
            1 1 var? if
                $E message
                wait-message
            then
            1 2 var? if
                $800 self-anim
                self-wait-anim
                $F message
                wait-message
                $802 self-anim
                self-wait-anim
            then
            1 var-inc
            1 3 var? if
                1 0 var-set
            then
        then
    else
        $800 self-anim
        self-wait-anim
        $10 message
        wait-message
        $802 self-anim
        self-wait-anim
    then
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: old-mansion-1f-8.act1B ( -- )   \ 00426300
    $F $54 fade
    $FF 1.0 0 bgm
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    8 state-flag-set
    $F1 action-end
    1 char-here? if
        $F ebit-set
        1 action-end
        1 char-done
    then
    3 char-unload
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    0 $1A2 108.0 -122.0 90 char-to-xz
    $F ebit? if
        1 char-activate
        $F ebit-clear
        1 $19F 140 char-to-tri-facing
    then
    $80 story-flag-set
    0 $F1 $1C action
    0 8 char-file-load
    0 char-file-use
    $8003 self-anim
    3 panic-stage? not if
        3 panic-stage
    then
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    self-wait-anim
    $32 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act17 ( -- )   \ 00426280
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $19 message-param-room
    $19 item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act18 ( -- )   \ 004262A0
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $1A message-param-room
    $1A item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act19 ( -- )   \ 004262C0
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $1B message-param-room
    $1B item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act1A ( -- )   \ 004262E0
    self-wait-done
    $18 state-flag-set
    1 self-scripted
    $1C message-param-room
    $1C item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act1C ( -- )   \ 004263E0
    3 char-unload
    $26 char-activate
    $26 127.567 0.0 -123.393 -90 char-to-xyz
    old-mansion-1f-8.cmd01
    $26 $9001 1 0 char-anim-hold
    self-idle-or-end
;

: old-mansion-1f-8.act1D ( -- )   \ 00426400
    self-wait-done
    -112.26 -123.08 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-8.act1E ( -- )   \ 00426418
    $26 $9000 0 0 char-anim-hold
    $26 wait-char-anim
    $26 $9001 1 0 char-anim-hold
    self-idle-or-end
;

: old-mansion-1f-8.act1F ( -- )   \ 00426430
    0 old-mansion-1f-8.cmd03
    begin
        2 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-1f-8.cmd03
    self-idle-or-end
;

: old-mansion-1f-8.act20 ( -- )   \ 00426440
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    3 partner-load
    2 char-unload
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    3 4 0 char-model-op
    0 1 char-no-shadow
    0 $F9 $1F action
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    1 old-mansion-1f-8.cmd03
    0 0 char-no-shadow
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-1f-8.act21 ( -- )   \ 00426540
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    0 old-mansion-1f-8.cmd02
    $26 3 $FF char-load
    3 char-unload
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-1f-8.act22 ( -- )   \ 00426630
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    0 old-mansion-1f-8.cmd02
    $26 3 $FF char-load
    3 char-unload
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-8.enter old-mansion-1f-8 0 room-script!
' old-mansion-1f-8.char-enter old-mansion-1f-8 6 room-script!
' old-mansion-1f-8.phase1 old-mansion-1f-8 1 room-script!
' old-mansion-1f-8.phase2 old-mansion-1f-8 2 room-script!
' old-mansion-1f-8.phase5 old-mansion-1f-8 5 room-script!
' old-mansion-1f-8.act00 old-mansion-1f-8 $00 action-script!
' old-mansion-1f-8.act01 old-mansion-1f-8 $01 action-script!
' old-mansion-1f-8.act02 old-mansion-1f-8 $02 action-script!
' old-mansion-1f-8.act03 old-mansion-1f-8 $03 action-script!
' old-mansion-1f-8.act04 old-mansion-1f-8 $04 action-script!
' old-mansion-1f-8.act05 old-mansion-1f-8 $05 action-script!
' old-mansion-1f-8.act06 old-mansion-1f-8 $06 action-script!
' old-mansion-1f-8.act07 old-mansion-1f-8 $07 action-script!
' old-mansion-1f-8.act08 old-mansion-1f-8 $08 action-script!
' old-mansion-1f-8.act09 old-mansion-1f-8 $09 action-script!
' old-mansion-1f-8.act0A old-mansion-1f-8 $0A action-script!
' old-mansion-1f-8.act0B old-mansion-1f-8 $0B action-script!
' old-mansion-1f-8.act0C old-mansion-1f-8 $0C action-script!
' old-mansion-1f-8.act0D old-mansion-1f-8 $0D action-script!
' old-mansion-1f-8.act0E old-mansion-1f-8 $0E action-script!
' old-mansion-1f-8.act0F old-mansion-1f-8 $0F action-script!
' old-mansion-1f-8.act10 old-mansion-1f-8 $10 action-script!
' old-mansion-1f-8.act11 old-mansion-1f-8 $11 action-script!
' old-mansion-1f-8.act12 old-mansion-1f-8 $12 action-script!
' old-mansion-1f-8.act13 old-mansion-1f-8 $13 action-script!
' old-mansion-1f-8.act14 old-mansion-1f-8 $14 action-script!
' old-mansion-1f-8.act15 old-mansion-1f-8 $15 action-script!
' old-mansion-1f-8.act16 old-mansion-1f-8 $16 action-script!
' old-mansion-1f-8.act17 old-mansion-1f-8 $17 action-script!
' old-mansion-1f-8.act18 old-mansion-1f-8 $18 action-script!
' old-mansion-1f-8.act19 old-mansion-1f-8 $19 action-script!
' old-mansion-1f-8.act1A old-mansion-1f-8 $1A action-script!
' old-mansion-1f-8.act1B old-mansion-1f-8 $1B action-script!
' old-mansion-1f-8.act1C old-mansion-1f-8 $1C action-script!
' old-mansion-1f-8.act1D old-mansion-1f-8 $1D action-script!
' old-mansion-1f-8.act1E old-mansion-1f-8 $1E action-script!
' old-mansion-1f-8.act1F old-mansion-1f-8 $1F action-script!
' old-mansion-1f-8.act20 old-mansion-1f-8 $20 action-script!
' old-mansion-1f-8.act21 old-mansion-1f-8 $21 action-script!
' old-mansion-1f-8.act22 old-mansion-1f-8 $22 action-script!
' old-mansion-1f-8.act23 old-mansion-1f-8 $23 action-script!
