\ story/rooms/house-of-truth-1f-4.fs - the event scripts of room house-of-truth-1f-4 ($8C; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-4
USING: room-names story.words story.shared flag-names ;

\ (as RoomC7_Cmd01)
: house-of-truth-1f-4.cmd00 ( b0 -- )  drop s" house-of-truth-1f-4.cmd00" stub-step ;
\ the room object named pstr_dynamo swung: byte 3 0 starts it (rest +0x30 from +0x20, phase
\ +0x34 0, amplitude +0x3C 1); 1 steps the phase back 60 degrees and the amplitude down 0.25,
\ height +0x28 = +0x38 + amplitude * sin, waiting (2) until it has died out
: house-of-truth-1f-4.cmd01 ( b0 -- )  drop s" house-of-truth-1f-4.cmd01" stub-step ;
\ a turning machine: the wheel pstr_roller (angle +0x18, height +0x24 5.1) driven by the belt
\ pstr_belt (offset +0x20 wrapping at 10, height +0x24 -3, speed +0x30). Byte 3 0 sets it up
\ (speed 0.4); 1 runs it a frame (both shaking by up to 0.05); 2 also slows it by 0.01, waiting
\ (2) until it stops. (The wheel's wrap steps +0x10, not the angle.)
: house-of-truth-1f-4.cmd02 ( b0 -- )  drop s" house-of-truth-1f-4.cmd02" stub-step ;
\ Hewie's +0x14C8 to script variable 2 (byte 3 0), or back from it (1; 0 there gives 10)
: house-of-truth-1f-4.cmd03 ( b0 -- )  drop s" house-of-truth-1f-4.cmd03" stub-step ;
\ the placed things of kinds 0, 2, 3, 5, 7 and 8 the event manager finds in area 0xB (+0x10):
\ their timer (+0xE4) to 300000
: house-of-truth-1f-4.cmd04 ( -- )  s" house-of-truth-1f-4.cmd04" stub-step ;

: house-of-truth-1f-4.enter ( -- )   \ 00433CC0
    room-sounds
    $9A story-flag? not $9B story-flag? not and if
        0 3 var-set
        7 1 object-show
        8 0 object-show
        9 0 object-show
        $A 1 object-show
        $B 1 object-show
        $C 0 object-show
        $D 0 object-show
        $E 0 object-show
        $F 0 object-show
        $10 0 object-show
        $11 0 object-show
        $12 0 object-show
        $13 0 object-show
        $14 0 object-show
        $15 0 object-show
        $16 0 object-show
    else
        7 1 object-show
        8 0 object-show
        9 0 object-show
        $A 1 object-show
        $B 1 object-show
        $C 1 object-show
        $D 1 object-show
        $E 1 object-show
        $F 1 object-show
        $10 1 object-show
        $11 1 object-show
        $12 1 object-show
        $13 1 object-show
        $14 1 object-show
        $15 1 object-show
        $16 1 object-show
        $9C story-flag? not if
            0 0.0 -2.2 65.0 flicker-sprite
        then
    then
    0 0 $14 door-bits
    $9B story-flag? not if
        force-followed state-flag-set
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-4.char-enter ( -- )   \ 00433D80
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    1 3 3 area-camera
;

: house-of-truth-1f-4.phase1 ( -- )   \ 00433E00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 3 3 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $9B story-flag? not if
        1 ebit? not if
            0 50.0 0.0 -28.0 5 11 0 zone
            0 0 char-in-zone? if
                0 $F1 1 action
            then
        then
    then
    $9A story-flag? $9B story-flag? not and if
        1 ebit? 2 ebit? not and if
            $FE $B char-in-area? if
                2 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 4 action-force
                else
                    1 0 4 action-force
                then
            else 0 $B char-entered-area? if
                2 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 3 action-force
                else
                    1 0 3 action-force
                then
            then then
        then
        3 ebit? if
            0 6 char-entered-area? if
                0 house-of-truth-1f-4.cmd03
                1 action-end
                1 char-done
                1 char-activate
                1 house-of-truth-1f-4.cmd03
                house-of-truth-1f-4.cmd04
                $21 chance? if
                    1 $146 char-to-tri
                else $32 chance? if
                    1 $130 char-to-tri
                else
                    1 $11C char-to-tri
                then then
                1 0 $1000000 nav-group
                3 ebit-clear
            else 1 0 char-in-nav-group? not if
                1 0 $1000000 nav-group
                3 ebit-clear
            then then
        then
        7 ebit? if
            0 6 char-entered-area? if
                house-of-truth-1f-4.cmd04
                7 ebit-clear
            then
        then
    then
    3 44.93 0.0 -26.49 $1B 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 10.0 hewie-look-zone
            then
        then
    then
    1 ebit? if
        $C0000002 6 24.0 5.0 65.0 0 0 sound
    then
;

: house-of-truth-1f-4.phase2 ( -- )   \ 00433F50
    0 $A char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    -2147483646 scene-request? if
        5 7 1 scene-change
    then
    $9B story-flag? $9C story-flag? not and if
        1 0.0 -3.2 65.0 5 5 0 zone
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    $9A story-flag? not if
        0 $C $32 char-faces-area? if
            5 $A 0 scene-change
        then
    then
;

: house-of-truth-1f-4.act00 ( -- )   \ 00433FA0
    self-wait-done
    50.0 -32.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    $9B story-flag? not if
        0 ebit? not if
            0 message
            wait-message
            0 ebit-set
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-4.act01 ( -- )   \ 00433FD0
    0 0 6 char-sound
    1 $FF 4 rumble
    0 house-of-truth-1f-4.cmd00
    0 house-of-truth-1f-4.cmd01
    1 house-of-truth-1f-4.cmd01
    0 0 var? if
        $9A story-flag? not if
            begin
                fiona-free? not while
                yield
            repeat
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 2 action-force
            else
                1 0 2 action-force
            then
        else
            0 $F2 6 action
        then
    then
    self-frames-reset
    $1E self-wait-frames
    self-idle-or-end
;

: house-of-truth-1f-4.act02 ( -- )   \ 00434020
    force-followed state-flag-clear
    scene-locked state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    house-of-truth-1f-4.cmd04
    self-frames-reset
    1 self-wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
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
    0 $F9 9 action
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
    0 $106 46.98 -19.5 -44 char-to-xz
    1 char-activate
    1 $53 23.54 2.5 -94 char-to-xz
    $FE char-activate
    $FE $8C 103 2 stalker-to-room
    $FE $67 -69.97 2.34 98 char-to-xz
    $FE 3 3 char-camera
    $E5 door-open-clear
    1 0 self-door-knock
    doors-room-in
    $E5 door-lock
    scene-5-pending state-flag-set
    $FE char-full-health
    6 0 object-show
    7 1 object-show
    8 0 object-show
    9 0 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    $9A story-flag-set
    $F $51 fade
    wait-fade
    $46 resident-flag-set
    $250 item-give
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    0 self-scripted
    0 $F2 6 action
    self-idle-or-end
;

: house-of-truth-1f-4.act03 ( -- )   \ 00434180
    capture-no-end state-flag-set
    no-pause state-flag-set
    scene-locked state-flag-set
    1 self-scripted
    house-of-truth-1f-4.cmd04
    self-frames-reset
    1 self-wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    $F2 action-end
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
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
    $47 resident-flag-set
    0 game-over-flag
    caught state-flag-set
    1.0 sound-volume-scale
    self-idle-or-end
;

: house-of-truth-1f-4.act04 ( -- )   \ 00434200
    scene-locked state-flag-set
    1 self-scripted
    house-of-truth-1f-4.cmd04
    self-frames-reset
    1 self-wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    $40000001 6 sound-stop
    $F2 action-end
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $64 $FF movie-param
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
    $FE action-end
    3 summon-take
    summoner-on state-flag-clear
    $E5 door-unlock
    0 $7A -10.04 37.76 18 char-to-xz
    1 char-activate
    1 $9D 6.01 27.57 18 char-to-xz
    50 hewie-trust
    $9B story-flag-set
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    6 0 object-show
    7 1 object-show
    8 0 object-show
    9 0 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    0 0.0 -2.2 65.0 flicker-sprite
    0 0 $14 door-bits
    1 exit-prepare
    $F $51 fade
    wait-fade
    $48 resident-flag-set
    0 0 $1000000 nav-group
    scene-locked state-flag-clear
    scene-5-pending state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-4.act05 ( -- )   \ 00434350
    1 self-scripted
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 effect-remove
    $25 message-param-room
    $25 1 item-give-count
    0 $25 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    $9C story-flag-set
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-4.act06 ( -- )   \ 004343A0
    0 1 $14 door-bits
    $40000002 6 24.0 5.0 65.0 0 0 sound
    1 ebit-set
    0 1 $14 door-bits
    3 ebit-set
    7 ebit-set
    1 0 var-set
    0 house-of-truth-1f-4.cmd02
    begin
        1 house-of-truth-1f-4.cmd02
        self-frames-reset
        1 self-wait-frames
        1 var-inc
        1 240 var? until
    $40000001 6 24.0 5.0 65.0 0 0 sound
    1 ebit-clear
    0 0 $14 door-bits
    2 house-of-truth-1f-4.cmd02
    0 0 $1000000 nav-group
    0 3 var-set
    0 0 $14 door-bits
    self-idle-or-end
;

: house-of-truth-1f-4.act07 ( -- )   \ 00434410
    scene-5-pending state-flag? if
        3 message
    else
        self-wait-done
        0 self-through-exit
        self-wait-done
        4 ebit? not 2 game-mode? or $FF panic-stage? or if
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
                4 message
                wait-message
            then
            4 ebit-set
        else
            self-frames-reset
            4 self-wait-frames
            0 $BF 90.5 0.0 90 char-to-xz
            world-frozen state-flag-set
            1 self-scripted
            1 20.0 0.0 0.0 2.0 event-camera
            self-frames-reset
            4 self-wait-frames
            5 message
            wait-message
            self-frames-reset
            4 self-wait-frames
            0 0.0 0.0 0.0 0.0 event-camera
            world-frozen state-flag-clear
            0 self-scripted
            4 ebit-clear
        then
        $251 item-give
    then
    self-idle-or-end
;

: house-of-truth-1f-4.act08 ( -- )   \ 004344B0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 3 6 char-sound
    $25 message-param-room
    $E2 door-locked? not if
        $25 item-use
    then
    $E6 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-4.act09 ( -- )   \ 004344F0
    0 0 $14 door-bits
    begin
        $F cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $12 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $13 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $14 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $17 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $28 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $29 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $2A cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $2B cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $2C cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $31 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $37 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $38 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $39 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $3A cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $3B cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $3C cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $53 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $54 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        $63 cutscene-cue-reached? not while
        yield
    repeat
    0 0 $14 door-bits
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    0 1 $14 door-bits
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: house-of-truth-1f-4.act0A ( -- )   \ 00434600
    self-wait-done
    6 ebit? not if
        6 message
        wait-message
        6 ebit-set
    else
        $1D01 $A self-anim-blend
        self-wait-anim
        7 message
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-4.act0B ( -- )   \ 00434620
    self-wait-done
    7 1 object-show
    $A 1 object-show
    $B 1 object-show
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
    0 $F9 9 action
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
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: house-of-truth-1f-4.act0C ( -- )   \ 004346B0
    self-wait-done
    7 1 object-show
    $A 1 object-show
    $B 1 object-show
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
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: house-of-truth-1f-4.act0D ( -- )   \ 00434740
    self-wait-done
    7 1 object-show
    $A 1 object-show
    $B 1 object-show
    $B 3 $FF char-load
    3 char-unload
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $64 $FF movie-param
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

: house-of-truth-1f-4.phase5 ( -- )   \ 0047AE50
    force-followed state-flag-clear
;

\ ---- registered ----
' house-of-truth-1f-4.enter house-of-truth-1f-4 0 room-script!
' house-of-truth-1f-4.char-enter house-of-truth-1f-4 6 room-script!
' house-of-truth-1f-4.phase1 house-of-truth-1f-4 1 room-script!
' house-of-truth-1f-4.phase2 house-of-truth-1f-4 2 room-script!
' house-of-truth-1f-4.act00 house-of-truth-1f-4 $00 action-script!
' house-of-truth-1f-4.act01 house-of-truth-1f-4 $01 action-script!
' house-of-truth-1f-4.act02 house-of-truth-1f-4 $02 action-script!
' house-of-truth-1f-4.act03 house-of-truth-1f-4 $03 action-script!
' house-of-truth-1f-4.act04 house-of-truth-1f-4 $04 action-script!
' house-of-truth-1f-4.act05 house-of-truth-1f-4 $05 action-script!
' house-of-truth-1f-4.act06 house-of-truth-1f-4 $06 action-script!
' house-of-truth-1f-4.act07 house-of-truth-1f-4 $07 action-script!
' house-of-truth-1f-4.act08 house-of-truth-1f-4 $08 action-script!
' house-of-truth-1f-4.act09 house-of-truth-1f-4 $09 action-script!
' house-of-truth-1f-4.act0A house-of-truth-1f-4 $0A action-script!
' house-of-truth-1f-4.act0B house-of-truth-1f-4 $0B action-script!
' house-of-truth-1f-4.act0C house-of-truth-1f-4 $0C action-script!
' house-of-truth-1f-4.act0D house-of-truth-1f-4 $0D action-script!
' house-of-truth-1f-4.phase5 house-of-truth-1f-4 5 room-script!
