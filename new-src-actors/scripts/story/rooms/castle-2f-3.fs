\ story/rooms/castle-2f-3.fs - the event scripts of room castle-2f-3 ($21; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-3
USING: room-names story.words story.shared flag-names ;

: castle-2f-3.cmd00 ( -- )  s" castle-2f-3.cmd00" stub-step ;
\ room 0x21 (D_00400B98): the fan turns, except while a movie plays
: castle-2f-3.cmd01 ( -- )  s" castle-2f-3.cmd01" stub-step ;
\ room 0x21 (D_00400BA8)
: castle-2f-3.cmd02 ( b0 -- )  drop s" castle-2f-3.cmd02" stub-step ;
\ Sets bit 1 of the flag byte three times (inlined setter calls); returns 1.
: castle-2f-3.cmd03 ( -- )  s" castle-2f-3.cmd03" stub-step ;
\ room 0x21 (D_00400BC8)
: castle-2f-3.cmd04 ( b0 -- )  drop s" castle-2f-3.cmd04" stub-step ;
\ room 0x21 (Room21_Cmd05_ptmf): the player's model +0xC8 vector by byte 3
: castle-2f-3.cmd05 ( b0 -- )  drop s" castle-2f-3.cmd05" stub-step ;
\ room 0x21 (Room21_Cmd06_ptmf): Fiona's model +0x1570 (byte 3 0) / +0x1574 (1) = byte 4
: castle-2f-3.cmd06 ( b0 b1 -- )  drop drop s" castle-2f-3.cmd06" stub-step ;
\ room 0x21 (Room21_Cond00_ptmf): the pursuer, about and not in state 2, is in its mode 2 but in
\ another room than the current one (the player about too)
: castle-2f-3.cond00? ( -- flag )  s" castle-2f-3.cond00?" stub-flag ;

: castle-2f-3.enter ( -- )   \ 003FF170
    room-sounds
    castle-2f-3.cmd00
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
        1 noise-level
    then
    0 $F1 $12 action
    $25E story-flag? $25F story-flag? not and if
        2 -9.0 1.0 -11.0 flicker-sprite
    then
    0 story-flag? not if
        $3D door-lock
    then
    2 story-flag? 3 story-flag? not and if
        $3D door-lock
    then
    $17 story-flag? 0 story-flag? not and if
        1 1 $14 door-bits
    else
        1 0 $14 door-bits
    then
    0 0 var-set
    1 0 var-set
    1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
;

: castle-2f-3.act04 ( -- )   \ 003FFA90
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE $A char-file-load
    then
    5 ebit-clear
    2 0 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 1 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 2 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else 2 3 pvar? if
        $32 chance? if
            5 ebit-set
        then
    then then then then
    $C ebit? if
        5 ebit-clear
        $C ebit-clear
        force-chased state-flag-clear
    else
        2 creature-action? if
            5 ebit-set
        then
        4 stalker-alert? if
            5 ebit-clear
        then
    then
    5 ebit? if
        0 $FE $B action
    else
        $78 1 item-cooldown
    then
    2 pvar-inc
    exit
;

: castle-2f-3.char-enter ( -- )   \ 003FF200
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
    0 self-is? if
        $1B story-flag? not $D story-flag? and if
            $3A story-flag? not if
                $3A story-flag-set
                0 exit-taken? $FF panic-stage? not and if
                    castle-2f-3.cond00? if
                        0 0 $23 action
                    then
                then
            then
        then
        $80 exit-taken? if
            1 story-flag? 2 story-flag? not and if
                force-calm state-flag-clear
                0.0 sound-volume-scale
                hewie-controlled? not if
                    0 2 2 char-camera
                    0 camera-follow
                else
                    1 2 2 char-camera
                    1 camera-follow
                then
                0 $A8 45 char-to-tri-facing
                world-held state-flag-set
                0 0 8 action
            then
        then
        $81 exit-taken? if
            $18 story-flag? not if
                3 story-flag? $B story-flag? not and if
                    0.0 sound-volume-scale
                    0 $128 -38.951 -26.464 90 char-to-xz
                    0 0 9 action
                    $18 story-flag-set
                then
            then
        then
    then
    1 self-is? if
    then
    $FE self-is? if
        fiona-hidden state-flag? if
            $B ebit-set
            $300 story-flag? 2 creature-action? not and if
                2 self-is? 6 self-is? or 7 self-is? or if
                    $78 1 item-cooldown
                    0 $FE 6 action
                else
                    castle-2f-3.act04
                then
            else
                castle-2f-3.act04
            then
        else
            $B ebit-clear
        then
    then
;

: castle-2f-3.phase1 ( -- )   \ 003FF340
    3 ebit? not if
        6 sound-bank-loaded? if
            $300 story-flag? if
                $40000001 6 0.0 3.0 7.0 0 0 sound
            then
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            3 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000001 6 0.0 3.0 7.0 0 0 sound
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
    then
    0 story-flag? not if
        $17 story-flag? not if
            0 $E char-entered-area? $D ebit? not and if
                $D ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 7 action-force
                else
                    1 0 7 action-force
                then
            then
        then
    then
    $25E story-flag? not if
        7 -9.0 0.0 -11.0 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 606 var-set
                $1A 607 var-set
                $1B 2 var-set
                $1C -9000 var-set
                $1D 1000 var-set
                $1E -11000 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
    0 -1.41 -8.0 8.99 $D 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 -46.87 0.0 -38.5 $14 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    2 -39.23 0.0 -59.47 $14 6 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    3 -37.71 -8.0 63.24 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 46.35 0.0 -40.12 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    5 39.18 0.0 -19.5 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    6 -0.22 -8.0 59.41 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-3.phase2 ( -- )   \ 003FF650
    0 4 char-in-area? 0 90 $5A char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 0 0 scene-change
        then
    then
    0 story-flag? not if
        $17 story-flag? if
            0 $10 char-in-area? 0 90 $32 char-heading? and if
                5 $D 0 scene-change
            then
        then
        -2147483646 scene-request? if
            5 $C 1 scene-change
        then
    else 2 story-flag? 3 story-flag? not and if
        -2147483646 scene-request? if
            5 $E 1 scene-change
        then
        0 5 char-in-area? 0 90 $3C char-heading? and if
            5 $F 0 scene-change
        then
        0 $D char-in-area? 0 22 $3C char-heading? and if
            5 $10 0 scene-change
        then
    else 0 5 char-in-area? 0 90 $3C char-heading? and if
        $D story-flag? not if
            5 $16 0 scene-change
        else $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then then
    then then then
    0 $B char-in-area? 0 -67 $3C char-heading? and if
        5 $14 0 scene-change
    then
    0 $C char-in-area? 0 45 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $15 0 scene-change
        then
    then
    0 $11 char-in-area? 0 0 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $17 0 scene-change
        then
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $18 0 scene-change
        then
    then
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $1C 0 scene-change
        then
    then
    0 $13 $32 char-faces-area? if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $1D 0 scene-change
        then
    then
    0 $14 char-in-area? 0 -45 $32 char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $1E 0 scene-change
        then
    then
    0 $15 char-in-area? 0 90 $32 char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else $AE story-flag? not if
            5 $1F 0 scene-change
        else
            5 $26 0 scene-change
        then then
    then
    0 $D char-in-area? 0 $16 char-in-area? or 0 22 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else 2 story-flag? not if
            5 $20 0 scene-change
        else
            5 $22 0 scene-change
        then then
    then
    $17 story-flag? if
        0 7 char-in-area? 0 -45 $32 char-heading? and if
            0 story-flag? not if
                5 $C 0 scene-change
            else
                5 $84 3 scene-change
            then
        then
    then
    $C ebit? -2147483646 scene-request? and if
        5 $24 1 scene-change
    then
    $25E story-flag? $25F story-flag? not and if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 $19 4 scene-change
        then
    then
;

: castle-2f-3.phase3 ( -- )   \ 003FF810
    -2.0 7.6 -47.0 -2.0 7.6 -67.0 -2.0 0.0 -47.0 -2.0 0.0 -67.0 lights-doorway
;

: castle-2f-3.act00 ( -- )   \ 003FF850
    self-wait-done
    $6A -1.0 13.5 160 $FFFF 5 self-move-to
    self-wait-done
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $306 story-flag? not if
        $306 story-flag-set
        $29 message
        wait-message
    then
    $902 self-anim
    self-wait-anim
    5 0 pvar? 5 1 pvar? or if
        $300 story-flag? not if
            $300 story-flag-set
            0 0 6 char-sound
            $40000001 6 0.0 3.0 7.0 0 0 sound
            $40 $6A noise
            1 noise-level
        else
            $300 story-flag-clear
            0 2 6 char-sound
            1 6 sound-stop
            0 noise-level
        then
        $903 self-anim
        self-wait-anim
    else
        $903 self-anim
        self-wait-anim
        $28 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act03 ( -- )   \ 003FFA60
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    counter-inc
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-2f-3.act01 ( -- )   \ 003FF8F0
    stalkers-stay state-flag-set
    $B ebit-clear
    1 self-scripted
    0 counter-set
    6 ebit-clear
    7 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 50 chars-within? and if
        0 1 2 action-force
        6 ebit-set
    then
    0 8 char-file-load
    self-wait-done
    6 ebit? if
        $27 14.5 -37.5 180 $FFFF 5 self-move-to
        self-wait-done
        0 char-file-use
        1 self-look-at
        yield
        begin
            7 ebit? not while
            yield
        repeat
        counter-inc
        1 self-noclip
        $FF self-look-at
        yield
        $8000 5 self-anim-9
        self-frames-reset
        $1C self-wait-frames
        0 $7E 5 char-sound
        self-wait-anim
    else
        0 char-file-use
        $27 $8004 5 14.5 -37.5 180 self-walk-anim
        self-wait-done
        counter-inc
        1 self-noclip
        0 $7E 5 char-sound
        $8005 $A self-anim-9
        self-wait-anim
    then
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    4 ebit-clear
    0 avoid-prompt
    $C ebit? if
        $FE $10 448 2 stalker-to-room
        $FE 0 stalker-mode
        $3C door-unlock
        $3D door-unlock
        force-chased state-flag-set
    then
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-2f-3.act03 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                $B ebit? $FE char-here? not and if
                    $B ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $C ebit? if
                $C ebit-clear
                force-chased state-flag-clear
            then
            counter-inc
            $FE char-here? if
                ['] castle-2f-3.act03 goto
            then
            0 camera-follow
            fiona-hidden state-flag-clear
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

: castle-2f-3.act02 ( -- )   \ 003FFA10
    1 self-scripted
    1 9 char-file-load
    self-wait-done
    $88 18.5 -37.5 180 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    7 ebit-set
    1 wait-counter
    1 self-noclip
    $8000 5 self-anim-blend
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                $8002 5 self-anim-9
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-3.act05 ( -- )   \ 0047AAC8
    self-wait-done
    -1 self-move-16
    self-wait-anim
    self-idle
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act06 ( -- )   \ 003FFB00
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $6A -4.0 13.0 135 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    $FE 2 6 char-sound
    1 6 sound-stop
    0 noise-level
    $300 story-flag-clear
    5 pvar-inc
    $FE 3 stalker-mode
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: castle-2f-3.act07 ( -- )   \ 003FFB50
    self-wait-done
    $F $54 fade
    3 3 $FF char-load
    3 char-unload
    0 1 char-no-shadow
    1 2 movie-play
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
    1 castle-2f-3.cmd02
    1 castle-2f-3.cmd04
    0 $F9 $A action
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    wait-fade
    world-held state-flag-set
    0 0 char-no-shadow
    3 0 char-remove
    1 1 $14 door-bits
    2 castle-2f-3.cmd05
    $3C door-open-clear
    doors-room-in
    $3C door-lock
    0 $10E -27.455 -48.354 107 char-to-xz
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    $17 story-flag-set
    $F $51 fade
    wait-fade
    $201 item-give
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act08 ( -- )   \ 003FFC30
    self-wait-done
    2 story-flag-set
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    1 exit-prepare
    0 $A8 46 char-to-tri-facing
    $3C door-open-clear
    $3D door-open-clear
    $3D door-lock
    doors-room-in
    camera-restart
    $F $51 fade
    wait-fade
    $204 item-give
    2 30 var-set
    0 $F2 $1A action
    self-idle-or-end
;

: castle-2f-3.act11 ( -- )   \ 00400020
    $8001 self-anim
    self-wait-anim
    begin
        32 hewie-action? while
        yield
    repeat
    hewie-no-attack state-flag-set
    $3D door-unlock
    0 counter-set
    0 $FE $13 action
    1 wait-counter
    1 self-scripted
    hewie-no-attack state-flag-clear
    self-wait-done
    $FF self-look-at
    yield
    $F $54 fade
    7 2 movie-play
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
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    0 self-move-16
    self-wait-anim
    $3D door-open-clear
    doors-room-in
    1 0 self-door-knock
    0 1 char-no-shadow
    $FF panic-stage? if
        3 panic-stage
    then
    $14 message-prepare
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
    world-held state-flag-set
    0 0 char-no-shadow
    1 char-activate
    1 0 char-set-C4
    1 char-full-health
    $21 0 33 hewie-to-room
    1 $21 145 char-to-tri-facing
    hewie-commandable state-flag-set
    $29 0 hewie-action
    1 hewie-wait-5
    $B story-flag-set
    0 $13 -16.325 -66.999 -8 char-to-xz
    0 self-move-16
    self-wait-anim
    0 exit-prepare
    stalkers-blind state-flag-clear
    $FE action-end
    1 summon-take
    self-wait-done
    camera-restart
    world-held state-flag-clear
    traps-off state-flag-clear
    $F $51 fade
    wait-fade
    $207 item-give
    $F 6 fade
    wait-fade
    $40A7 message
    wait-message
    $F 7 fade
    wait-fade
    $A7 message-param-room
    $A7 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    $AF story-flag? if
        $F 6 fade
        wait-fade
        $32 message
        wait-message
        $F 7 fade
        wait-fade
    then
    0 hewie-wait-5
    $8274 item-give
    0 self-scripted
    self-idle-or-end
;

: castle-2f-3.act09 ( -- )   \ 003FFC70
    traps-off state-flag-set
    force-calm state-flag-set
    self-wait-done
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
    0 $F9 $25 action
    $21 0 176 hewie-to-room
    $FE $21 20 2 stalker-to-room
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
    world-held state-flag-set
    scene-locked state-flag-clear
    0 $A char-layer
    0 3 0.0 light
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    1 char-activate
    1 0 char-set-C4
    1 char-full-health
    $21 0 176 hewie-to-room
    1 $B0 -35.452 -8.852 180 char-to-xz
    hewie-commandable state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    stalkers-blind state-flag-set
    $FE $21 20 2 stalker-to-room
    $FE char-activate
    $FE 2 stalker-mode
    $FE 0 stalker-search-delay
    $FE $14 -15.648 -48.68 0 char-to-xz
    $FE char-full-health
    0 $FE $1B action
    0 $B char-file-load
    0 char-file-use
    0 $A -39.575 -27.807 90 char-to-xz
    camera-restart
    $8001 self-anim
    self-wait-anim
    force-calm state-flag-clear
    5 0 0 music
    $F $91 fade
    wait-fade
    self-frames-reset
    begin
        $2A message
        $F0 frames? 0 control-action? or if
            $8002 self-anim
            self-frames-reset
            $10 self-wait-frames
            0 $38 5 char-sound
            hewie-no-attack state-flag-clear
            $20 0 hewie-action
            $2A message-close
            self-wait-anim
            $FE self-look-at
            yield
            ['] castle-2f-3.act11 goto
        else
            yield
        then
    again
;

: castle-2f-3.act0A ( -- )   \ 003FFDD0
    begin
        1 1 castle-2f-3.cmd06
        8 cutscene-shot? $A cutscene-shot? or if
            1 $1E castle-2f-3.cmd06
        then
        0 $14 castle-2f-3.cmd06
        6 cutscene-shot? 8 cutscene-shot? or $A cutscene-shot? or $F cutscene-shot? or $11 cutscene-shot? or $13 cutscene-shot? or if
            0 $28 castle-2f-3.cmd06
        then
        3 cutscene-shot? if
            1 1 $14 door-bits
        then
        $15 cutscene-shot? if
            0 castle-2f-3.cmd05
        then
        $16 cutscene-shot? if
            1 castle-2f-3.cmd05
        then
        $17 cutscene-shot? if
            2 castle-2f-3.cmd05
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-2f-3.act0B ( -- )   \ 003FFE30
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        4 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        4 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-2f-3.act0C ( -- )   \ 0047AAD0
    self-wait-done
    $20 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act0D ( -- )   \ 003FFE90
    self-wait-done
    $1F message
    wait-message
    0 answer? if
        $F $54 fade
        $D 3 $FF char-load
        $12 4 $FF char-load
        $18 5 $FF char-load
        3 char-unload
        4 char-unload
        5 char-unload
        $D 1 char-no-shadow
        3 0 movie-play
        2 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
        wait-fade
        0 $F9 $21 action
        $FF panic-stage? if
            3 panic-stage
        then
        8 message-prepare
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
        world-held state-flag-set
        $D 0 char-no-shadow
        3 0 char-remove
        4 0 char-remove
        5 0 char-remove
        new-game-sounds state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
        effects-arena-flip
        0 char-in
        begin
            yield
            4 sound-bank-loaded? until
        1 0 $14 door-bits
        0 $21 5.752 -41.411 178 char-to-xz
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        camera-restart
        $3C door-unlock
        $3D door-unlock
        0 story-flag-set
        no-flee state-flag-clear
        $F $51 fade
        wait-fade
        $202 item-give
        1 $A self-anim-blend
        self-wait-anim
        10 self-move-16
        $12 message
        wait-message
        $F 6 fade
        wait-fade
        $40A6 message
        wait-message
        $AE story-flag? if
            $40AB message
            wait-message
        then
        $F 7 fade
        wait-fade
        $A6 message-param-room
        $A6 1 item-give-count
        $83 $85 0.0 0.0 0.0 0 0 sound
        $801B message
        wait-message
        $AE story-flag? if
            $34B story-flag-set
            $AB message-param-room
            $AB 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
        then
    then
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act0E ( -- )   \ 0047AAD8
    self-wait-done
    $C message
    wait-message
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act0F ( -- )   \ 0047AAE0
    self-wait-done
    $22 message
    wait-message
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act10 ( -- )   \ 00400000
    self-wait-done
    $80 -1.546 59.728 45 $FFFF 5 self-move-to
    self-wait-done
    $902 self-anim
    self-wait-anim
    $D message
    wait-message
    $903 self-anim
    self-wait-anim
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act12 ( -- )   \ 0047AAE8
    begin
        castle-2f-3.cmd01
        yield
    again
;

: castle-2f-3.act13 ( -- )   \ 00400180
    1 self-scripted
    self-wait-done
    $FF self-look-at
    yield
    $C4 $203 6 self-move-tri
    self-wait-done
    0 exit-door-open? not if
        0 3 self-move-slot
        self-wait-done
    then
    $76 -17.0 -83.0 180 $203 5 self-move-to
    self-wait-done
    1 counter-set
    0 self-scripted
    begin
        yield
    again
;

: castle-2f-3.act14 ( -- )   \ 004001B0
    self-wait-done
    -40.0 -60.0 self-turn-to-xz
    self-wait-done
    $23 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act15 ( -- )   \ 004001C0
    self-wait-done
    $B5 43.571 -39.929 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $312 story-flag-set
    0 ebit? not if
        $24 message
        wait-message
        0 ebit-set
    else
        $25 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act16 ( -- )   \ 00400220
    self-wait-done
    $26 message
    wait-message
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    self-wait-16
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    $27 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act17 ( -- )   \ 00400250
    self-wait-done
    $9F -37.0 63.0 0 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $314 story-flag-set
    1 ebit? not if
        $2D message
        wait-message
        1 ebit-set
    else $312 story-flag? $313 story-flag? or if
        $2E message
        wait-message
    else
        $2D message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act18 ( -- )   \ 004002C0
    self-wait-done
    9 -40.418 -39.428 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $313 story-flag-set
    2 ebit? not if
        $2B message
        wait-message
        2 ebit-set
    else $312 story-flag? $314 story-flag? and if
        $2C message
        wait-message
    else
        $2B message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act19 ( -- )   \ 00400330
    self-wait-done
    -9.0 -11.0 self-turn-to-xz
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
            $25F story-flag-set
            2 effect-remove
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

: castle-2f-3.act1A ( -- )   \ 00400390
    begin
        yield
        2 var-dec
        2 0 var? if
            $1E chance? if
                $F 6 228.0 5.0 78.0 0 0 sound
                2 180 var-set
            else
                2 60 var-set
            then
        then
    again
;

: castle-2f-3.act1B ( -- )   \ 004003D0
    self-wait-done
    0 self-look-at
    yield
    begin
        $19 chance? if
            $1302 $A self-anim-blend
            self-wait-anim
        else $19 chance? if
            $1305 $A self-anim-blend
            self-wait-anim
        then then
        yield
    again
;

: castle-2f-3.act1C ( -- )   \ 004003F0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $E8 -25.0 63.0 -5 char-to-xz
    1 20.0 10.0 40.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 ebit? not if
        1 message
        wait-message
        9 ebit-set
    else
        2 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act1D ( -- )   \ 00400440
    self-wait-done
    5.0 40.5 self-turn-to-xz
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act1E ( -- )   \ 00400450
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $55 -40.0 12.0 -90 char-to-xz
    1 30.0 20.0 -60.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act1F ( -- )   \ 00400498
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $40 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act20 ( -- )   \ 004004B0
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $A ebit? not if
        6 message
        wait-message
        $A ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-3.act21 ( -- )   \ 004004D0
    begin
        2 cutscene-shot? if
            castle-2f-3.cmd03
        then
        5 cutscene-shot? if
            1 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-2f-3.act22 ( -- )   \ 004004E8
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $31 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act23 ( -- )   \ 00400500
    stalkers-stay state-flag-set
    self-wait-done
    0 4 self-move-slot
    self-wait-done
    1 $A self-anim-blend
    $F message
    self-wait-anim
    10 self-move-16
    wait-message
    $F 6 fade
    wait-fade
    $40A5 message
    wait-message
    $F 7 fade
    wait-fade
    $A5 message-param-room
    $A5 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    $3C door-open-clear
    $3C door-lock
    $3D door-lock
    doors-room-in
    force-chased state-flag-set
    $C ebit-set
    self-idle-or-end
;

: castle-2f-3.act24 ( -- )   \ 0047AAF0
    self-wait-done
    $11 message
    wait-message
    self-idle-or-end
;

: castle-2f-3.act25 ( -- )   \ 00400560
    begin
        0 cutscene-shot? not while
        yield
    repeat
    0 $1E char-layer
    begin
        1 cutscene-shot? not while
        yield
    repeat
    0 $A char-layer
    begin
        6 cutscene-shot? not while
        yield
    repeat
    2 3 1.5 light
    begin
        7 cutscene-shot? not while
        yield
    repeat
    0 3 0.0 light
    self-idle-or-end
;

: castle-2f-3.act26 ( -- )   \ 004005A0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message
    wait-message
    0 answer? if
        7 subscreen-open
        begin
            subscreen-wanted state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            0 $10 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            1 char-here? if
                1 2 char-C4? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-2f-3.act27 ( -- )   \ 004006F0
    self-wait-done
    1 1 $14 door-bits
    1 fiona-costume
    0 char-in
    3 3 $FF char-load
    3 char-unload
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    1 1 $14 door-bits
    0 1 char-no-shadow
    1 2 movie-play
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
    1 castle-2f-3.cmd02
    1 castle-2f-3.cmd04
    0 $F9 $A action
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    wait-fade
    world-held state-flag-set
    0 0 char-no-shadow
    $26 1 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        1 fiona-costume
        $26 1 pvar-set
    else $26 0 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
    else $26 2 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        2 fiona-costume
        $26 2 pvar-set
    else $26 3 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
    else $26 6 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        6 fiona-costume
        $26 6 pvar-set
    else $26 7 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        7 fiona-costume
        $26 7 pvar-set
    else $26 8 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        8 fiona-costume
        $26 8 pvar-set
    then then then then then then then
    0 char-in
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-2f-3.act28 ( -- )   \ 00400840
    self-wait-done
    1 fiona-costume
    0 char-in
    $D 3 $FF char-load
    $12 4 $FF char-load
    $18 5 $FF char-load
    3 char-unload
    4 char-unload
    5 char-unload
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    1 1 $14 door-bits
    $D 1 char-no-shadow
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    0 $F9 $21 action
    $FF panic-stage? if
        3 panic-stage
    then
    8 message-prepare
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
    world-held state-flag-set
    $D 0 char-no-shadow
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $26 1 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        1 fiona-costume
        $26 1 pvar-set
    else $26 0 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
    else $26 2 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        2 fiona-costume
        $26 2 pvar-set
    else $26 3 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
    else $26 6 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        6 fiona-costume
        $26 6 pvar-set
    else $26 7 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        7 fiona-costume
        $26 7 pvar-set
    else $26 8 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        8 fiona-costume
        $26 8 pvar-set
    then then then then then then then
    0 char-in
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-2f-3.act29 ( -- )   \ 00400980
    self-wait-done
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    2 3 $FF char-load
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
    0 $F9 $25 action
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
    0 $A char-layer
    0 3 0.0 light
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-2f-3.act2A ( -- )   \ 00400A30
    self-wait-done
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    7 2 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 char-no-shadow
    0 self-scripted
    $FF panic-stage? if
        3 panic-stage
    then
    $14 message-prepare
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
    world-held state-flag-set
    0 0 char-no-shadow
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-3.enter castle-2f-3 0 room-script!
' castle-2f-3.char-enter castle-2f-3 6 room-script!
' castle-2f-3.phase1 castle-2f-3 1 room-script!
' castle-2f-3.phase2 castle-2f-3 2 room-script!
' castle-2f-3.phase3 castle-2f-3 3 room-script!
' castle-2f-3.act00 castle-2f-3 $00 action-script!
' castle-2f-3.act01 castle-2f-3 $01 action-script!
' castle-2f-3.act02 castle-2f-3 $02 action-script!
' castle-2f-3.act03 castle-2f-3 $03 action-script!
' castle-2f-3.act04 castle-2f-3 $04 action-script!
' castle-2f-3.act05 castle-2f-3 $05 action-script!
' castle-2f-3.act06 castle-2f-3 $06 action-script!
' castle-2f-3.act07 castle-2f-3 $07 action-script!
' castle-2f-3.act08 castle-2f-3 $08 action-script!
' castle-2f-3.act09 castle-2f-3 $09 action-script!
' castle-2f-3.act0A castle-2f-3 $0A action-script!
' castle-2f-3.act0B castle-2f-3 $0B action-script!
' castle-2f-3.act0C castle-2f-3 $0C action-script!
' castle-2f-3.act0D castle-2f-3 $0D action-script!
' castle-2f-3.act0E castle-2f-3 $0E action-script!
' castle-2f-3.act0F castle-2f-3 $0F action-script!
' castle-2f-3.act10 castle-2f-3 $10 action-script!
' castle-2f-3.act11 castle-2f-3 $11 action-script!
' castle-2f-3.act12 castle-2f-3 $12 action-script!
' castle-2f-3.act13 castle-2f-3 $13 action-script!
' castle-2f-3.act14 castle-2f-3 $14 action-script!
' castle-2f-3.act15 castle-2f-3 $15 action-script!
' castle-2f-3.act16 castle-2f-3 $16 action-script!
' castle-2f-3.act17 castle-2f-3 $17 action-script!
' castle-2f-3.act18 castle-2f-3 $18 action-script!
' castle-2f-3.act19 castle-2f-3 $19 action-script!
' castle-2f-3.act1A castle-2f-3 $1A action-script!
' castle-2f-3.act1B castle-2f-3 $1B action-script!
' castle-2f-3.act1C castle-2f-3 $1C action-script!
' castle-2f-3.act1D castle-2f-3 $1D action-script!
' castle-2f-3.act1E castle-2f-3 $1E action-script!
' castle-2f-3.act1F castle-2f-3 $1F action-script!
' castle-2f-3.act20 castle-2f-3 $20 action-script!
' castle-2f-3.act21 castle-2f-3 $21 action-script!
' castle-2f-3.act22 castle-2f-3 $22 action-script!
' castle-2f-3.act23 castle-2f-3 $23 action-script!
' castle-2f-3.act24 castle-2f-3 $24 action-script!
' castle-2f-3.act25 castle-2f-3 $25 action-script!
' castle-2f-3.act26 castle-2f-3 $26 action-script!
' castle-2f-3.act27 castle-2f-3 $27 action-script!
' castle-2f-3.act28 castle-2f-3 $28 action-script!
' castle-2f-3.act29 castle-2f-3 $29 action-script!
' castle-2f-3.act2A castle-2f-3 $2A action-script!
