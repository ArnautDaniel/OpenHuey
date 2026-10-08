\ story/rooms/old-mansion-b1-3.fs - the event scripts of room old-mansion-b1-3 ($4C; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-3
USING: room-names story.words story.shared flag-names ;

\ four room objects (Room4C_ObjectNames) pressed in (+0x24 down 0.2 to -0.7, a sound as each
\ starts) while event flag i is set, else back up 0.2 to 0; byte 3 0 all reset
: old-mansion-b1-3.cmd00 ( b0 -- )  drop s" old-mansion-b1-3.cmd00" stub-step ;
\ room 0x4C (Room4C_Cmd01_ptmf): byte 3 0 starts counting what the player does (her +0x1AD710
\ on), 1 adds this frame's (Fiona_Shakes); at 35 events bit 0x13
: old-mansion-b1-3.cmd01 ( b0 -- )  drop s" old-mansion-b1-3.cmd01" stub-step ;
\ room 0x4C (D_0040AC08): character 0x11's model parts 0xA2 / 0xA4 / 0xAC get bit 2 when byte 3
\ is 0, else lose it
: old-mansion-b1-3.cmd02 ( b0 -- )  drop s" old-mansion-b1-3.cmd02" stub-step ;

: old-mansion-b1-3.enter ( -- )   \ 00409980
    room-sounds
    0 old-mansion-b1-3.cmd00
    $48 story-flag? not if
        0 -0.5 18.5 -35.5 2 effect-86
        1 9.5 18.5 -35.0 2 effect-86
        2 4.8 25.5 -37.5 2 effect-86
        3 4.8 6.2 -33.4 2 effect-86
    else
        0 -0.5 18.5 -35.5 3 effect-86
        1 9.5 18.5 -35.0 3 effect-86
        2 4.8 25.5 -37.5 3 effect-86
        3 4.8 6.2 -33.4 3 effect-86
    then
    3 char-unload
    $11 char-activate
    $49 story-flag? if
        0 old-mansion-b1-3.cmd02
    else
        1 old-mansion-b1-3.cmd02
    then
    $11 $9005 1 0 char-anim-hold
    $11 9.97 0.0 -0.59 0 char-to-xyz
    $330 story-flag? not if
        $A 1 object-show
        $B 0 object-show
        $F 1 object-show
        $10 0 object-show
    else
        $A 0 object-show
        $B 1 object-show
        $F 0 object-show
        $10 1 object-show
    then
    2 1 $14 door-bits
    $E 1 object-show
    5 0 $14 door-bits
    $26A story-flag? not if
        5 -20.96 8.5 5.57 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-b1-3.act0F ( -- )   \ 0040A7D0
    $18 ebit-clear
    $17 0 pvar? if
        0 chance? if
            $18 ebit-set
        then
    else $17 1 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    else $17 2 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    else $17 3 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    then then then then
    2 creature-action? if
        $18 ebit-set
    then
    $18 ebit? if
        0 $FE $10 action
    else
        $78 1 item-cooldown
    then
    $17 pvar-inc
    exit
;

: old-mansion-b1-3.char-enter ( -- )   \ 00409A80
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    $FE self-is? if
        fiona-hidden state-flag? if
            $19 ebit-set
            old-mansion-b1-3.act0F
        else
            $19 ebit-clear
        then
    then
;

: old-mansion-b1-3.phase1 ( -- )   \ 00409AD0
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    0 ebit-clear
    0 $89 char-on-tri? 0 $B1 char-on-tri? or if
        0 ebit-set
    then
    1 $89 char-on-tri? 1 $B1 char-on-tri? or 1 char-here? and if
        0 ebit-set
    then
    $FE $89 char-on-tri? $FE $B1 char-on-tri? or $FE char-here? and if
        0 ebit-set
    then
    1 ebit-clear
    0 $90 char-on-tri? 0 $B2 char-on-tri? or if
        1 ebit-set
    then
    1 $90 char-on-tri? 1 $B2 char-on-tri? or 1 char-here? and if
        1 ebit-set
    then
    $FE $90 char-on-tri? $FE $B2 char-on-tri? or $FE char-here? and if
        1 ebit-set
    then
    2 ebit-clear
    0 $97 char-on-tri? 0 $B3 char-on-tri? or if
        2 ebit-set
    then
    1 $97 char-on-tri? 1 $B3 char-on-tri? or 1 char-here? and if
        2 ebit-set
    then
    $FE $97 char-on-tri? $FE $B3 char-on-tri? or $FE char-here? and if
        2 ebit-set
    then
    3 ebit-clear
    0 $83 char-on-tri? 0 $B0 char-on-tri? or if
        3 ebit-set
    then
    1 $83 char-on-tri? 1 $B0 char-on-tri? or 1 char-here? and if
        3 ebit-set
    then
    $FE $83 char-on-tri? $FE $B0 char-on-tri? or $FE char-here? and if
        3 ebit-set
    then
    1 old-mansion-b1-3.cmd00
    $48 story-flag? not if
        1 ebit? 2 ebit? and if
            8 ebit? not 4 ebit? not and if
                0 0 var? if
                    0 var-inc
                then
                4 ebit-set
                9 6 -0.5 18.5 -35.5 0 0 sound
                0 effect-remove
            then
            8 ebit-set
        else
            8 ebit-clear
        then
        0 ebit? 3 ebit? and if
            9 ebit? not 5 ebit? not and if
                0 1 var? if
                    0 var-inc
                then
                5 ebit-set
                9 6 9.5 18.5 -35.0 0 0 sound
                1 effect-remove
            then
            9 ebit-set
        else
            9 ebit-clear
        then
        1 ebit? 3 ebit? and if
            $A ebit? not 6 ebit? not and if
                0 2 var? if
                    0 var-inc
                then
                6 ebit-set
                9 6 4.8 25.5 -37.5 0 0 sound
                2 effect-remove
            then
            $A ebit-set
        else
            $A ebit-clear
        then
        0 ebit? 2 ebit? and if
            $B ebit? not 7 ebit? not and if
                0 3 var? if
                    0 var-inc
                then
                7 ebit-set
                9 6 4.8 6.2 -33.4 0 0 sound
                3 effect-remove
            then
            $B ebit-set
        else
            $B ebit-clear
        then
        0 ebit? 1 ebit? and if
            $1C ebit? not if
                4 ebit? 5 ebit? or 6 ebit? or 7 ebit? or if
                    8 6 4.8 6.2 -33.4 0 0 sound
                then
                4 ebit-clear
                5 ebit-clear
                6 ebit-clear
                7 ebit-clear
                0 -0.5 18.5 -35.5 2 effect-86
                1 9.5 18.5 -35.0 2 effect-86
                2 4.8 25.5 -37.5 2 effect-86
                3 4.8 6.2 -33.4 2 effect-86
                0 0 var-set
            then
            $1C ebit-set
        else
            $1C ebit-clear
        then
        2 ebit? 3 ebit? and if
            $1D ebit? not if
                4 ebit? 5 ebit? or 6 ebit? or 7 ebit? or if
                    8 6 4.8 6.2 -33.4 0 0 sound
                then
                4 ebit-clear
                5 ebit-clear
                6 ebit-clear
                7 ebit-clear
                0 -0.5 18.5 -35.5 2 effect-86
                1 9.5 18.5 -35.0 2 effect-86
                2 4.8 25.5 -37.5 2 effect-86
                3 4.8 6.2 -33.4 2 effect-86
                0 0 var-set
            then
            $1D ebit-set
        else
            $1D ebit-clear
        then
        4 ebit? 5 ebit? and 6 ebit? and 7 ebit? and if
            0 char-busy? not if
                $48 story-flag-set
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
    then
    1 char-here? 1 0 char-C4? and if
        $E ebit? not if
            0 23.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        $E ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            0 23.0 0.0 -22.5 7 5 0 zone
            1 0 9 char-zone-bits? not if
                $E ebit-clear
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $F ebit? not if
            1 11.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        $F ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            1 11.0 0.0 -22.5 7 5 0 zone
            1 1 9 char-zone-bits? not if
                $F ebit-clear
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $10 ebit? not if
            2 -1.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 2 9 char-zone-bits? if
                        $10 ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            2 -1.0 0.0 -22.5 7 5 0 zone
            1 2 9 char-zone-bits? not if
                $10 ebit-clear
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        $11 ebit? not if
            3 -13.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        $11 ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            3 -13.0 0.0 -22.5 7 5 0 zone
            1 3 9 char-zone-bits? not if
                $11 ebit-clear
            then
        then
    then
    $15 ebit? $16 ebit? not and if
        4 10.8 0.0 14.07 $A 200 0 zone
        1 4 char-in-zone? if
            $16 ebit-set
        then
    then
    $15 ebit? not if
        5 37.99 0.0 24.16 $14 14 0 zone
        $1A ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 5 9 char-zone-bits? if
                        $1A ebit-set
                        $32 chance? if
                            $1F 5 var-set
                            0 1 $92 action
                        then
                    then
                then
            then
        then
        5 37.99 0.0 24.16 $14 14 0 zone
        1 char-here? 2 game-mode? not and if
            hewie-can-command? if
                1 5 9 char-zone-bits? if
                    $1F 5 var-set
                    $1F 14.0 hewie-look-zone
                then
            then
        then
        $48 story-flag? not if
            6 11.81 0.0 -0.03 $14 6 0 zone
            $1B ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 6 9 char-zone-bits? if
                            $1B ebit-set
                            $64 chance? if
                                $1F 6 var-set
                                0 1 $8A action
                            then
                        then
                    then
                then
            then
            6 11.81 0.0 -0.03 $14 6 0 zone
            1 char-here? 1 game-mode? and if
                hewie-can-command? if
                    1 6 9 char-zone-bits? if
                        $1F 6 var-set
                        $1F 6.0 hewie-look-zone
                    then
                then
            then
        then
    then
    $1E ebit? not if
        6 sound-bank-loaded? if
            $40000004 6 40.0 5.0 -26.0 0 0 sound
            $1E ebit-set
        then
    else
        $C0000004 6 40.0 5.0 -26.0 0 0 sound
    then
;

: old-mansion-b1-3.phase2 ( -- )   \ 0040A020
    0 3 char-in-area? 0 90 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 4 char-in-area? 0 10 0 $32 char-faces-xz? and if
        $48 story-flag? not if
            5 2 0 scene-change
        else $49 story-flag? not if
            5 5 4 scene-change
        else
            5 4 0 scene-change
        then then
    then
    0 5 char-in-area? 0 45 $32 char-heading? and if
        $330 story-flag? not if
            5 6 0 scene-change
        else $FE char-here? not if
            5 6 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    0 6 char-in-area? 0 0 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 7 char-in-area? 0 55 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 8 char-in-area? 0 45 $32 char-heading? and if
        5 9 0 scene-change
    then
    0 9 char-in-area? 0 0 $32 char-heading? and if
        5 $A 0 scene-change
    then
    0 $A char-in-area? 0 -22 $32 char-heading? and if
        5 $B 0 scene-change
    then
    0 $B char-in-area? 0 -45 $32 char-heading? and if
        5 $C 0 scene-change
    then
    0 $C char-in-area? 0 -45 $32 char-heading? and if
        5 $D 0 scene-change
    then
    $26A story-flag? not if
        7 5 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
;

: old-mansion-b1-3.act00 ( -- )   \ 0040A0F0
    0 counter-set
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    7 1 movie-play
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
    0 $F9 $13 action
    $FF 1 char-visible
    1 char-here? if
        0 1 1 action-force
        1 wait-counter
    then
    $10 $FF movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    wait-fade
    50 hewie-trust
    0 self-move-16
    camera-restart
    $FF 0 char-visible
    counter-inc
    0 -0.5 18.5 -35.5 3 effect-86
    1 9.5 18.5 -35.0 3 effect-86
    2 4.8 25.5 -37.5 3 effect-86
    3 4.8 6.2 -33.4 3 effect-86
    $11 $9005 1 0 char-anim-hold
    $11 9.97 0.0 -0.59 0 char-to-xyz
    $11 0 char-no-shadow
    $FE $4C 0 room-doors-state
    $F $41 fade
    wait-fade
    $827E item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act01 ( -- )   \ 0040A200
    begin
        1 char-busy? not while
        yield
    repeat
    $FF 1 char-visible
    counter-inc
    2 wait-counter
    $FF 0 char-visible
    self-idle-or-end
;

: old-mansion-b1-3.act02 ( -- )   \ 0040A220
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $47 story-flag? not if
        $8D 9.0 10.0 180 $FFFF 5 self-move-to
        self-wait-done
        0 message
        wait-message
        0 answer? if
            $F $44 fade
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
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $FF panic-stage? if
                3 panic-stage
            then
            $FFFF message-prepare
            1 result? if
                6 cutscene-control
                1.0 movie-volume
                effects-arena-flip
                yield
                movie-skipped state-flag-clear
                $F 1 fade
                $A cutscene-control
                begin
                    movie-skipped state-flag? not if
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
                world-held state-flag-set
                effects-arena-flip
                begin
                    movie-playing? while
                    yield
                repeat
                yield
                pause-wanted state-flag-set
                8 cutscene-control
            then
            wait-fade
            0 self-move-16
            0 $2F 10.8 14.07 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            $11 $9005 0 0 char-anim-hold
            camera-restart
            1 0 char-visible
            1 action-end
            world-held state-flag-set
            0 8 char-file-load
            0 char-file-use
            world-held state-flag-clear
            $F $41 fade
        then
    then
    0 answer? if
        self-wait-done
        $47 story-flag? if
            0 8 char-file-load
            0 char-file-use
            1 27.0 10.0 -40.0 0.0 event-camera
            0 $2F 10.8 14.07 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9000 0 0 char-anim-hold
            $8002 5 self-anim-blend
            self-frames-reset
            $17 self-wait-frames
            0 5 6 char-sound
            self-wait-anim
        else
            1 27.0 10.0 -40.0 0.0 event-camera
            0 $2F 12.34 13.19 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9001 0 0 char-anim-hold
            $8003 self-anim
        then
        $15 ebit-set
        $16 ebit-clear
        $13 ebit-clear
        $14 ebit-clear
        0 old-mansion-b1-3.cmd01
        begin
            hewie-can-command? if
                $61 0 hewie-action
            then
            0 control-action? 1 control-action? or if
                $1F ebit? not if
                    $1F ebit-set
                    0 $38 5 char-sound
                then
            then
            $1F ebit? if
                2 var-inc
                2 60 var? if
                    2 0 var-set
                    $1F ebit-clear
                then
            then
            1 old-mansion-b1-3.cmd01
            1 var-inc
            1 12 var? if
                $A threat-add
                1 0 var-set
            then
            panic-98? if
                $13 ebit-set
                $14 ebit-set
            then
            $13 ebit? not if
                self-at-motion-event? if
                    0 panic-stage? 1 panic-stage? or if
                        $11 $9001 0 5 char-anim-hold
                        $8003 5 self-anim-blend
                    then
                    2 panic-stage? 3 panic-stage? or if
                        $11 $9002 0 5 char-anim-hold
                        $8004 5 self-anim-blend
                    then
                then
            then
            $16 ebit? not if
                self-at-motion-event? not if
                    self-frames-reset
                    1 self-wait-frames
                    false
                else $13 ebit? not if
                    self-frames-reset
                    1 self-wait-frames
                    false
                else
                    true
                then then
            else
                true
            then
        until
        $15 ebit-clear
        0 0.0 0.0 0.0 0.0 event-camera
        hewie-can-command? if
            0 0 hewie-action
        then
        $11 $9003 0 5 char-anim-hold
        $8005 3 self-anim-blend
        self-wait-anim
        $11 $9005 0 5 char-anim-hold
        $14 ebit? if
            4 panic-stage
        then
        $47 story-flag-set
        $2F resident-flag-set
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act03 ( -- )   \ 0040A490
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    world-frozen state-flag-set
    1 20.0 -15.0 0.0 0.0 event-camera
    0 $97 5.0 -27.0 180 char-to-xz
    self-frames-reset
    4 self-wait-frames
    3 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    4 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act04 ( -- )   \ 0040A4F0
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    $D ebit? not if
        1 message
        wait-message
        $D ebit-set
    else
        2 message
        wait-message
        $D ebit-clear
    then
    self-idle-or-end
;

: old-mansion-b1-3.act05 ( -- )   \ 0040A510
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $49 story-flag-set
    0 old-mansion-b1-3.cmd02
    $F message-param-room
    $F 1 item-give-count
    0 $F item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-3.act0E ( -- )   \ 0040A790
    $17 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    0 2 6 char-sound
    $E 2 object-anim
    self-wait-anim
    0 self-noclip
    $17 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act06 ( -- )   \ 0040A560
    $330 story-flag? not if
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        $13 message
        wait-message
    else
        stalkers-stay state-flag-set
        $19 ebit-clear
        1 self-scripted
        0 9 char-file-load
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        1 self-noclip
        0 char-file-use
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        2 0 $14 door-bits
        $E 0 object-show
        $E 0 object-anim
        self-frames-reset
        $24 self-wait-frames
        0 2 6 char-sound
        self-wait-anim
        -1 self-move-16
        0 self-noclip
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-set
        $17 ebit-clear
        0 avoid-prompt
        $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
            0 3 6 char-sound
        then
        $FF 3 -1 char-camera
        begin
            0 2 pad? not if
                $FF panic-stage? $17 ebit? or if
                    $FF panic-stage? not if
                        0 $43 5 char-sound
                    then
                    ['] old-mansion-b1-3.act0E goto
                else
                    2 panic-grow
                    6 fiona-calm
                    $1E fiona-recovery-lower
                    $19 ebit? $FE char-here? not and if
                        $19 ebit-clear
                        1 avoid-prompt
                    then
                    yield
                    false
                then
            else
                true
            then
        until
        $FE char-here? if
            ['] old-mansion-b1-3.act0E goto
        then
        0 camera-follow
        fiona-hidden state-flag-clear
        1 self-noclip
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 2 6 char-sound
        $E 1 object-anim
        self-wait-anim
        0 self-noclip
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-b1-3.act07 ( -- )   \ 0040A650
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0 9 char-file-load
    5 9.83 27.45 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $330 story-flag? not if
        $8004 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        $A 1 object-show
        $B 0 object-show
        $F 1 object-show
        $10 0 object-show
        $B 0 object-anim
        $10 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        0 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        0 0 6 char-sound
        self-wait-anim
        $330 story-flag-set
    else
        $8003 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        $A 0 object-show
        $B 1 object-show
        $F 0 object-show
        $10 1 object-show
        $A 0 object-anim
        $F 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        0 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        5 self-wait-frames
        0 7 6 char-sound
        self-wait-anim
        $330 story-flag-clear
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-3.act08 ( -- )   \ 0040A700
    self-wait-done
    110 self-turn-angle
    self-wait-done
    $A message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-3.act09 ( -- )   \ 0040A710
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $C ebit? not if
        $B message
        wait-message
        $C ebit-set
    else
        $C message
        wait-message
        $C ebit-clear
    then
    self-idle-or-end
;

: old-mansion-b1-3.act0A ( -- )   \ 0040A728
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $D message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-3.act0B ( -- )   \ 0040A738
    self-wait-done
    -45 self-turn-angle
    self-wait-done
    $E message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-3.act0C ( -- )   \ 0040A750
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $12 ebit? not if
        $F message
        wait-message
        $12 ebit-set
    else
        10.0 10.0 0.0 self-look-at-point
        yield
        $10 message
        wait-message
        $FF self-look-at
        yield
    then
    self-idle-or-end
;

: old-mansion-b1-3.act0D ( -- )   \ 0040A780
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    $12 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-3.act10 ( -- )   \ 0040A820
    3 self-is? $22 self-is? or if
        1 self-scripted
        things-clear
        0 counter-set
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 $11 action-force
        1 self-scripted
        self-wait-done
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        counter-inc
        begin
            yield
        again
    else
        self-wait-done
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        7 26.16 27.36 118 $FFFF 5 self-move-to
        self-wait-done
        $1601 self-anim
        self-wait-anim
        $17 ebit-set
        10 self-move-16
        begin
            0 char-busy? while
            yield
        repeat
        $FE 0 stalker-mode
    then
    self-idle-or-end
;

: old-mansion-b1-3.act11 ( -- )   \ 0040A890
    capture-no-end state-flag-set
    no-pause state-flag-set
    1 wait-counter
    $F $44 fade
    $12 1 movie-play
    $11 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    1 action-end
    1 char-done
    $A 1 object-show
    $B 1 object-show
    3 stalker-kind? $22 stalker-kind? or if
        3 4 0 char-model-op
    then
    $10 $50 movie-param
    0 1 char-visible
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        movie-loop
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $F 1 fade
        $A cutscene-control
        begin
            7 cutscene-control
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
        until
    then
    $2E resident-flag-set
    0 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: old-mansion-b1-3.act12 ( -- )   \ 0040A910
    self-wait-done
    -20.96 5.57 self-turn-to-xz
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
            $26A story-flag-set
            5 effect-remove
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

: old-mansion-b1-3.act13 ( -- )   \ 0040A968
    $11 1 char-no-shadow
    begin
        2 cutscene-shot? not while
        yield
    repeat
    $11 0 char-no-shadow
    self-idle-or-end
;

: old-mansion-b1-3.act14 ( -- )   \ 0040A980
    self-wait-done
    3 partner-load
    2 char-unload
    $12 1 movie-play
    $11 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $A 1 object-show
    $B 1 object-show
    $F 0 object-show
    $10 1 object-show
    $E 1 object-show
    3 4 0 char-model-op
    $10 $50 movie-param
    0 1 char-visible
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    0 0 char-visible
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-3.act15 ( -- )   \ 0040AA30
    self-wait-done
    $A 1 object-show
    $B 0 object-show
    $F 1 object-show
    $10 0 object-show
    $E 1 object-show
    $11 3 $FF char-load
    3 char-unload
    $F $44 fade
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
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-3.act16 ( -- )   \ 0040AAD0
    self-wait-done
    $A 1 object-show
    $B 0 object-show
    $F 1 object-show
    $10 0 object-show
    $E 1 object-show
    $11 3 $FF char-load
    3 char-unload
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $13 action
    $FF 1 char-visible
    $10 $FF movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    $FF 0 char-visible
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-b1-3.enter old-mansion-b1-3 0 room-script!
' old-mansion-b1-3.char-enter old-mansion-b1-3 6 room-script!
' old-mansion-b1-3.phase1 old-mansion-b1-3 1 room-script!
' old-mansion-b1-3.phase2 old-mansion-b1-3 2 room-script!
' old-mansion-b1-3.act00 old-mansion-b1-3 $00 action-script!
' old-mansion-b1-3.act01 old-mansion-b1-3 $01 action-script!
' old-mansion-b1-3.act02 old-mansion-b1-3 $02 action-script!
' old-mansion-b1-3.act03 old-mansion-b1-3 $03 action-script!
' old-mansion-b1-3.act04 old-mansion-b1-3 $04 action-script!
' old-mansion-b1-3.act05 old-mansion-b1-3 $05 action-script!
' old-mansion-b1-3.act06 old-mansion-b1-3 $06 action-script!
' old-mansion-b1-3.act07 old-mansion-b1-3 $07 action-script!
' old-mansion-b1-3.act08 old-mansion-b1-3 $08 action-script!
' old-mansion-b1-3.act09 old-mansion-b1-3 $09 action-script!
' old-mansion-b1-3.act0A old-mansion-b1-3 $0A action-script!
' old-mansion-b1-3.act0B old-mansion-b1-3 $0B action-script!
' old-mansion-b1-3.act0C old-mansion-b1-3 $0C action-script!
' old-mansion-b1-3.act0D old-mansion-b1-3 $0D action-script!
' old-mansion-b1-3.act0E old-mansion-b1-3 $0E action-script!
' old-mansion-b1-3.act0F old-mansion-b1-3 $0F action-script!
' old-mansion-b1-3.act10 old-mansion-b1-3 $10 action-script!
' old-mansion-b1-3.act11 old-mansion-b1-3 $11 action-script!
' old-mansion-b1-3.act12 old-mansion-b1-3 $12 action-script!
' old-mansion-b1-3.act13 old-mansion-b1-3 $13 action-script!
' old-mansion-b1-3.act14 old-mansion-b1-3 $14 action-script!
' old-mansion-b1-3.act15 old-mansion-b1-3 $15 action-script!
' old-mansion-b1-3.act16 old-mansion-b1-3 $16 action-script!
