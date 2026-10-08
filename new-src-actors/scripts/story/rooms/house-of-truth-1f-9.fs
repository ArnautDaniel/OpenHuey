\ story/rooms/house-of-truth-1f-9.fs - the event scripts of room house-of-truth-1f-9 ($92; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-9
USING: room-names story.words story.shared ;

\ room objects 4 / 5 spinning (+0x10) at a speed (+0x30) eased by script variable 4 / 5: byte 3
\ 0 sets them up (speed and base 4 degrees, top 20); else each frame (unless the progress' +0x54
\ says no): state 0 slows by 5% of the base to 0, 1 speeds by 2% up to the base, 2 by 10% up to
\ the top, 3 jumps to the base
: house-of-truth-1f-9.cmd00 ( b0 -- )  drop s" house-of-truth-1f-9.cmd00" stub-step ;
\ room 0x92: three grey smoke effects (Effect79B00, size 50) at the room's spots 2, 7, 5
\ (grey_three).
: house-of-truth-1f-9.cmd01 ( -- )  s" house-of-truth-1f-9.cmd01" stub-step ;
\ up to 6 things out (script variable 6 counts them): 1..3 more placed things of kind 9 (+0x8),
\ each tied to the first room object 10..15 flagged (+0 = 1) (+0x122 its index), set up (+0xC,
\ +0x28 on) and dropped at a random spot 40..50 out and 25..40 up in any direction that lands on
\ the nav mesh (+0x3C)
: house-of-truth-1f-9.cmd02 ( -- )  s" house-of-truth-1f-9.cmd02" stub-step ;
\ the placed thing 10 brought back (list +0x14 / +0x8, its +0xC, +0x28 on) and put on one of
\ five spots round a circle (script variable 1, then on by 2 of 5): turned to the spot's angle
\ (with a little random), 2.1 up and 1.5..2 out on triangle 0x3B; then effect SmokeTrail_vtable
\ on it
: house-of-truth-1f-9.cmd03 ( -- )  s" house-of-truth-1f-9.cmd03" stub-step ;
\ (as Room49_Cmd02) byte 3 0: the effect WispColumn_vtable spawned (told 1), its slot in event
\ var 0; 1: it is told 0
: house-of-truth-1f-9.cmd04 ( b0 -- )  drop s" house-of-truth-1f-9.cmd04" stub-step ;
\ the things that fell below -30: placed things of kind 9 are reset (+0x28) and each counts down
\ script variable 6 (and the event manager's +0x5C); room objects 10..15 that did get +0 set
: house-of-truth-1f-9.cmd05 ( -- )  s" house-of-truth-1f-9.cmd05" stub-step ;
\ the 0x20E0-byte effect SparkSpray_vtable (sent byte 3): byte 3 0 / 1 one made, its slot in
\ script variable 2 / 3; 2 / 3 the one in variable 2 / 3 (if any) sent nothing
: house-of-truth-1f-9.cmd06 ( b0 -- )  drop s" house-of-truth-1f-9.cmd06" stub-step ;
\ (as slam_shake, both of Lorenzo's forms: 0xA and 0x27)
: house-of-truth-1f-9.cmd07 ( -- )  s" house-of-truth-1f-9.cmd07" stub-step ;
\ room objects 8 / 9 raised (+0x24 down 0.2 a call to 0) while the event manager's +0x58 test 4
\ / 5 holds, else lowered back (up 0.4 a call to 0.7)
: house-of-truth-1f-9.cmd08 ( -- )  s" house-of-truth-1f-9.cmd08" stub-step ;
\ a noise at (-70 or 70 by byte 3, 14, 0): byte 4 0 / 1 / 2 kind 1 / 2 / 4
: house-of-truth-1f-9.cmd09 ( b0 b1 -- )  drop drop s" house-of-truth-1f-9.cmd09" stub-step ;
\ Room92_Cmd0A
: house-of-truth-1f-9.cmd0A ( -- )  s" house-of-truth-1f-9.cmd0A" stub-step ;
\ Room92_Cmd0B
: house-of-truth-1f-9.cmd0B ( -- )  s" house-of-truth-1f-9.cmd0B" stub-step ;
\ Room92_Cmd0C
: house-of-truth-1f-9.cmd0C ( b0 b1 -- )  drop drop s" house-of-truth-1f-9.cmd0C" stub-step ;
\ Room92_Cmd0D
: house-of-truth-1f-9.cmd0D ( b0 -- )  drop s" house-of-truth-1f-9.cmd0D" stub-step ;

defer house-of-truth-1f-9.act0F
: house-of-truth-1f-9.enter ( -- )   \ 00436CF0
    room-sounds
    0 -36.4 13.4 54.4 1 effect-86
    1 58.7 10.0 -29.9 1 effect-86
    $A2 story-flag? if
        0 1 $14 door-bits
        0 $F1 9 action
        4 3 var-set
        5 3 var-set
    then
    $34E story-flag? if
        5 1.0 0 bgm
    then
    $80 exit-taken? not if
        shared.act95
        $A3 story-flag? if
            house-of-truth-1f-9.cmd01
            0 0 house-of-truth-1f-9.cmd0C
            0 house-of-truth-1f-9.cmd0D
        then
    else
        $B ebit-set
    then
    1 0 $18000020 nav-group
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
;

: house-of-truth-1f-9.char-enter ( -- )   \ 00436D70
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
    0 self-is? if
        $80 exit-taken? if
            0 $85 char-to-tri
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 $A action
            0.0 sound-volume-scale
        then
    then
;

: house-of-truth-1f-9.phase1 ( -- )   \ 00436DE0
    $A2 story-flag? if
        8 ebit? not if
            6 sound-bank-loaded? if
                $40000000 6 0.0 50.0 0.0 0 0 sound
                $40000001 6 0.0 0.0 0.0 0 0 sound
                8 ebit-set
            then
        else
            $C0000000 6 0.0 50.0 0.0 0 0 sound
            $C0000001 6 0.0 0.0 0.0 0 0 sound
            $C0000003 6 0.0 0.0 0.0 0 0 sound
            $C0000002 6 -70.0 15.0 0.0 0 0 sound
            $C0000006 6 70.0 15.0 0.0 0 0 sound
        then
    then
    $B ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    $A1 story-flag? not if
        0 control-action? 1 char-here? and 1 2 char-C4? not and $FE char-here? not and 0 4 char-in-area? and 0 0 65 $32 char-faces-xz? and if
            hewie-stays? if
                0 0 3 action
            then
        then
    then
    $A2 story-flag? $A3 story-flag? not and if
        $FE 2 char-C4? if
            $A3 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 5 action-force
            else
                1 0 5 action-force
            then
        then
    then
    $A2 story-flag? $A3 story-flag? not and if
        house-of-truth-1f-9.cmd05
        3 ebit? not if
            9 ebit? if
                3 ebit-set
                0 $F2 $B action
            then
        then
        4 ebit? not if
            0 5 char-in-area? 1 5 char-in-area? or $FE 5 char-in-area? or if
                4 ebit-set
                0 $F3 $C action
            then
        else 6 ebit? if
            0 7 char-in-area? 0 0 char-action? and if
                0 0 house-of-truth-1f-9.cmd09
            then
            1 7 char-in-area? 1 0 char-action? and if
                0 1 house-of-truth-1f-9.cmd09
            then
            $FE 7 char-in-area? $FE 0 char-action? and if
                0 2 house-of-truth-1f-9.cmd09
            then
        then then
        5 ebit? not if
            0 6 char-in-area? 1 6 char-in-area? or $FE 6 char-in-area? or if
                5 ebit-set
                0 $F4 $D action
            then
        else 7 ebit? if
            0 8 char-in-area? 0 0 char-action? and if
                1 0 house-of-truth-1f-9.cmd09
            then
            1 8 char-in-area? 1 0 char-action? and if
                1 1 house-of-truth-1f-9.cmd09
            then
            $FE 8 char-in-area? $FE 0 char-action? and if
                1 2 house-of-truth-1f-9.cmd09
            then
        then then
    else
        4 ebit? not if
            0 5 char-in-area? 1 5 char-in-area? or $FE 5 char-in-area? or if
                5 6 -40.0 0.0 -40.0 0 0 sound
                4 ebit-set
            then
        else 0 5 char-in-area? not 1 5 char-in-area? not and $FE 5 char-in-area? not and if
            4 ebit-clear
        then then
        5 ebit? not if
            0 6 char-in-area? 1 6 char-in-area? or $FE 6 char-in-area? or if
                5 6 44.0 0.0 37.0 0 0 sound
                5 ebit-set
            then
        else 0 6 char-in-area? not 1 6 char-in-area? not and $FE 6 char-in-area? not and if
            5 ebit-clear
        then then
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
    $A1 story-flag? not if
        3 1.06 0.0 59.99 $1A 14 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    4 0.21 0.0 0.03 $21 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
    house-of-truth-1f-9.cmd07
    house-of-truth-1f-9.cmd08
;

: house-of-truth-1f-9.phase2 ( -- )   \ 004370F0
    0 3 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    $10 state-flag? if
        -2147483646 scene-request? if
            5 6 1 scene-change
        then
    then
    $A3 story-flag? if
        2 0.0 0.0 0.0 $17 4 0 zone
        0 2 3 char-zone-bits? 0 0 0 $32 char-faces-xz? and if
            5 7 0 scene-change
        then
    then
;

: house-of-truth-1f-9.phase3 ( -- )   \ 00437140
    $A3 story-flag? $B ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        $B ebit-set
    then
;

: house-of-truth-1f-9.act02 ( -- )   \ 00437320
    1 self-look-at
    yield
    0 counter-set
    0 1 1 action-force
    1 wait-counter
    $A1 story-flag-set
    50 hewie-trust
    3 message
    wait-message
    $FF self-look-at
    yield
    $255 item-give
    $828C item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-9.act00 ( -- )   \ 00437160
    self-wait-done
    $A3 story-flag? not if
        $A1 story-flag? not if
            1 char-here? 1 2 char-C4? not and $FE char-here? not and if
                $18 state-flag-set
                1 self-scripted
                self-frames-reset
                8 self-wait-frames
                0 $31 0.0 58.0 0 char-to-xz
                1 15.0 10.0 0.0 5.0 event-camera
                $17 state-flag-set
                self-frames-reset
                8 self-wait-frames
                2 message
                wait-message
                self-frames-reset
                8 self-wait-frames
                $17 state-flag-clear
                0 0.0 0.0 0.0 0.0 event-camera
                0 $62 -10.0 54.0 140 char-to-xz
                ['] house-of-truth-1f-9.act02 goto
            else 0 ebit? not if
                self-frames-reset
                8 self-wait-frames
                0 $31 0.0 58.0 0 char-to-xz
                1 15.0 10.0 0.0 5.0 event-camera
                $17 state-flag-set
                self-frames-reset
                8 self-wait-frames
                0 message
                wait-message
                self-frames-reset
                8 self-wait-frames
                $17 state-flag-clear
                0 0.0 0.0 0.0 0.0 event-camera
                0 ebit-set
            else
                1 message
                wait-message
            then then
        else
            0.0 65.0 self-turn-to-xz
            self-wait-done
            self-frames-reset
            8 self-wait-frames
            0 $31 0.0 58.0 0 char-to-xz
            1 15.0 10.0 0.0 5.0 event-camera
            $17 state-flag-set
            self-frames-reset
            8 self-wait-frames
            4 message
            wait-message
            self-frames-reset
            8 self-wait-frames
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
    else 0 ebit? not if
        self-frames-reset
        8 self-wait-frames
        0 $31 0.0 58.0 0 char-to-xz
        1 15.0 10.0 0.0 5.0 event-camera
        $17 state-flag-set
        self-frames-reset
        8 self-wait-frames
        5 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        $17 state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
        0 ebit-set
    else
        0.0 65.0 self-turn-to-xz
        self-wait-done
        7 message
        wait-message
    then then
    self-idle-or-end
;

: house-of-truth-1f-9.act01 ( -- )   \ 004372F0
    1 self-scripted
    self-wait-done
    $31 3.0 55.0 -10 $204 5 self-move-to
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    0 self-look-at
    yield
    hewie-bark
    self-wait-done
    counter-inc
    $FF self-look-at
    yield
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-9.act03 ( -- )   \ 00437350
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C04 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    ['] house-of-truth-1f-9.act02 goto
;

: house-of-truth-1f-9.act04 ( -- )   \ 00437370
    0 state-flag-clear
    $FE action-end
    $FE char-done
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $F $44 fade
    3 1 movie-play
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
    1 action-end
    1 char-done
    0 1 $14 door-bits
    $26 item-use
    0 creatures-clear
    $A ebit-clear
    $10 $C8 movie-param
    0 $F9 8 action
    $FF 1.0 0 bgm
    $FF panic-stage? if
        3 panic-stage
    then
    9 message-prepare
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
    wait-fade
    $F9 action-end
    $1F $7B $32 $2A $64 $24 $2C $36 $4E 0 9 effect-string
    $8080805E 0 1 screen-blend
    0 self-move-16
    0 $7E 0.023 24.438 174 char-to-xz
    $FE action-end
    $FE char-done
    2 0 char-remove
    $27 partner-load
    2 char-unload
    $FE char-activate
    $FE $92 133 2 stalker-to-room
    $FE $85 -0.009 -44.982 30 char-to-xz
    $FE 0 0 char-camera
    $92 0 151 hewie-to-room
    1 $97 180 char-to-tri-facing
    4 0 object-show
    5 0 object-show
    0 $F1 9 action
    4 3 var-set
    5 3 var-set
    $ED door-open-clear
    doors-room-in
    $ED door-lock
    $A2 story-flag-set
    $10 state-flag-set
    4 ebit-clear
    5 ebit-clear
    7 20 var-set
    0 $F5 $F action
    $A ebit? not if
        0 0 house-of-truth-1f-9.cmd0C
    then
    2 2 house-of-truth-1f-9.cmd0C
    1 house-of-truth-1f-9.cmd0D
    7 1.0 0 bgm
    $F 1 fade
    wait-fade
    $4D resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-9.act05 ( -- )   \ 004374E0
    $FF 1.0 0 bgm
    $F2 action-end
    $F3 action-end
    $F4 action-end
    $F5 action-end
    4 ebit-clear
    5 ebit-clear
    6 ebit-clear
    7 ebit-clear
    3 ebit-clear
    9 ebit-clear
    $18 state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    1 1 movie-play
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
    1 action-end
    1 char-done
    house-of-truth-1f-9.cmd0A
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    4 3 var-set
    5 3 var-set
    2 house-of-truth-1f-9.cmd0D
    $10 $C8 movie-param
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
    $92 0 151 hewie-to-room
    1 $97 180 char-to-tri-facing
    50 hewie-trust
    0 self-move-16
    0 $2B 54.575 -2.153 -90 char-to-xz
    camera-restart
    $10 state-flag-clear
    $FE action-end
    $FE char-done
    2 0 char-remove
    $C partner-load
    2 char-unload
    $ED door-unlock
    4 0 object-show
    5 0 object-show
    $E6 door-lock
    $E7 door-lock
    $EB door-open-clear
    $EB door-lock
    house-of-truth-1f-9.cmd01
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    0 house-of-truth-1f-9.cmd0D
    $F $51 fade
    wait-fade
    $4E resident-flag-set
    $256 item-give
    $828D item-give
    shared.act9C
    $18 state-flag-clear
    0 self-scripted
    shared.act95
    self-idle-or-end
;

: house-of-truth-1f-9.act06 ( -- )   \ 0047AE6C
    8 message
    self-idle-or-end
;

: house-of-truth-1f-9.act07 ( -- )   \ 00437630
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not $A4 story-flag? not and if
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        6 message
        wait-message
        self-wait-anim
        1 ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-9.act08 ( -- )   \ 00437660
    depth-range-off
    begin
        $21C cutscene-cue-reached? not while
        yield
    repeat
    begin
        $2ED cutscene-cue-reached? not while
        1.0 1.0 80.0 160.0 depth-range
        yield
    repeat
    begin
        $348 cutscene-cue-reached? not while
        house-of-truth-1f-9.cmd0B
        yield
    repeat
    depth-range-off
    begin
        4 cutscene-shot? not while
        yield
    repeat
    depth-range-off
    $ED door-open-clear
    doors-room-in
    $1F $72 $96 $94 $6E $60 $56 $70 $3C 0 9 effect-string
    $9EEAEAFF 0 1 screen-blend
    begin
        5 cutscene-shot? not while
        yield
    repeat
    $1F $7B $32 $2A $64 $24 $2C $36 $4E 0 9 effect-string
    $8080805E 0 1 screen-blend
    begin
        $47E cutscene-cue-reached? not while
        yield
    repeat
    0 0 house-of-truth-1f-9.cmd0C
    $A ebit-set
    2 6 house-of-truth-1f-9.cmd0C
    begin
        $492 cutscene-cue-reached? not while
        yield
    repeat
    1 2 house-of-truth-1f-9.cmd0C
    begin
        8 cutscene-shot? not while
        yield
    repeat
    1 0 house-of-truth-1f-9.cmd0C
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    self-idle-or-end
;

: house-of-truth-1f-9.act09 ( -- )   \ 00437700
    0 house-of-truth-1f-9.cmd00
    begin
        1 house-of-truth-1f-9.cmd00
        yield
    again
;

: house-of-truth-1f-9.act0A ( -- )   \ 00437710
    1 self-scripted
    1 char-here? if
        $91 0 -1 hewie-to-room
    then
    self-wait-done
    $FF 1 char-visible
    house-of-truth-1f-9.cmd01
    0 0 house-of-truth-1f-9.cmd0C
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
    wait-fade
    8 state-flag-set
    $FE action-end
    $FE char-done
    $FF 0 char-visible
    1 house-of-truth-1f-9.cmd04
    $4F resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-1f-9.act0B ( -- )   \ 004377C0
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        0.3 camera-shake
        yield
    repeat
    0 house-of-truth-1f-9.cmd04
    $40000003 6 0.0 0.0 0.0 0 0 sound
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    house-of-truth-1f-9.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    house-of-truth-1f-9.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    house-of-truth-1f-9.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    1 house-of-truth-1f-9.cmd04
    9 ebit-clear
    3 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-9.act0C ( -- )   \ 00437870
    $40000002 6 -70.0 15.0 0.0 0 0 sound
    4 2 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    0 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    6 ebit-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    4 0 var-set
    2 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    6 ebit-clear
    self-frames-reset
    $384 self-wait-frames
    4 1 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    4 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-9.act0D ( -- )   \ 004378D0
    $40000006 6 70.0 15.0 0.0 0 0 sound
    5 2 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    1 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    7 ebit-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    5 0 var-set
    3 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    7 ebit-clear
    self-frames-reset
    $384 self-wait-frames
    5 1 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    5 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-9.act0E ( -- )   \ 0047AE70
    self-idle-or-end
;

:noname   \ house-of-truth-1f-9.act0F (00437930; deferred: used before it is defined)
    7 0 var? 7 1 var? or 7 2 var? or if
        8 10 var-set
        begin
            0.5 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        house-of-truth-1f-9.cmd02
        8 10 var-set
        begin
            1.0 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        8 10 var-set
        begin
            0.5 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        7 150 var-set
    then
    self-frames-reset
    $1E self-wait-frames
    7 var-dec
    7 var-dec
    $32 chance? if
        7 var-dec
    then
    ['] house-of-truth-1f-9.act0F goto
; is house-of-truth-1f-9.act0F

: house-of-truth-1f-9.act10 ( -- )   \ 004379B0
    self-wait-done
    0 -36.4 13.4 54.4 1 effect-86
    1 58.7 10.0 -29.9 1 effect-86
    $A 3 $FF char-load
    3 char-unload
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 $14 door-bits
    $10 $C8 movie-param
    0 $F9 8 action
    $FF panic-stage? if
        3 panic-stage
    then
    9 message-prepare
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

: house-of-truth-1f-9.act11 ( -- )   \ 00437A70
    self-wait-done
    $A 3 $FF char-load
    3 char-unload
    0 0 house-of-truth-1f-9.cmd0C
    1 1 movie-play
    0 cutscene-start
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
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $10 $C8 movie-param
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

: house-of-truth-1f-9.act12 ( -- )   \ 00437B20
    self-wait-done
    0 -36.4 13.4 54.4 1 effect-86
    1 58.7 10.0 -29.9 1 effect-86
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $FF 1 char-visible
    house-of-truth-1f-9.cmd01
    0 0 house-of-truth-1f-9.cmd0C
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-1f-9.enter house-of-truth-1f-9 0 room-script!
' house-of-truth-1f-9.char-enter house-of-truth-1f-9 6 room-script!
' house-of-truth-1f-9.phase1 house-of-truth-1f-9 1 room-script!
' house-of-truth-1f-9.phase2 house-of-truth-1f-9 2 room-script!
' house-of-truth-1f-9.phase3 house-of-truth-1f-9 3 room-script!
' house-of-truth-1f-9.act00 house-of-truth-1f-9 $00 action-script!
' house-of-truth-1f-9.act01 house-of-truth-1f-9 $01 action-script!
' house-of-truth-1f-9.act02 house-of-truth-1f-9 $02 action-script!
' house-of-truth-1f-9.act03 house-of-truth-1f-9 $03 action-script!
' house-of-truth-1f-9.act04 house-of-truth-1f-9 $04 action-script!
' house-of-truth-1f-9.act05 house-of-truth-1f-9 $05 action-script!
' house-of-truth-1f-9.act06 house-of-truth-1f-9 $06 action-script!
' house-of-truth-1f-9.act07 house-of-truth-1f-9 $07 action-script!
' house-of-truth-1f-9.act08 house-of-truth-1f-9 $08 action-script!
' house-of-truth-1f-9.act09 house-of-truth-1f-9 $09 action-script!
' house-of-truth-1f-9.act0A house-of-truth-1f-9 $0A action-script!
' house-of-truth-1f-9.act0B house-of-truth-1f-9 $0B action-script!
' house-of-truth-1f-9.act0C house-of-truth-1f-9 $0C action-script!
' house-of-truth-1f-9.act0D house-of-truth-1f-9 $0D action-script!
' house-of-truth-1f-9.act0E house-of-truth-1f-9 $0E action-script!
' house-of-truth-1f-9.act0F house-of-truth-1f-9 $0F action-script!
' house-of-truth-1f-9.act10 house-of-truth-1f-9 $10 action-script!
' house-of-truth-1f-9.act11 house-of-truth-1f-9 $11 action-script!
' house-of-truth-1f-9.act12 house-of-truth-1f-9 $12 action-script!
