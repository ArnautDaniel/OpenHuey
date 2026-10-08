\ story/rooms/old-mansion-3f-1.fs - the event scripts of room old-mansion-3f-1 ($60; Belli Castle: Old Mansion 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-3f-1
USING: room-names story.words story.shared flag-names ;

\ room 0x60 (D_00429148): the floor light (room effect 0x1B, 20 x 20 at y -0.2) by byte 3 - 1
\ removed with its glow effect (event variable 1); 0 made, with the glow (FloorGlow_vtable);
\ then (and for other values) its strength from event variable 0 (0..4: 0, 30, 60, 90, 128),
\ also sent to the glow
: old-mansion-3f-1.cmd00 ( b0 -- )  drop s" old-mansion-3f-1.cmd00" stub-step ;
\ room 0x60 (as Room23_Cmd01, for character 0xFE): byte 3 0 a progress name, 1 wait for
\ character 0xFE (2 while not), else done
: old-mansion-3f-1.cmd01 ( b0 -- )  drop s" old-mansion-3f-1.cmd01" stub-step ;
\ (as Room66_Effect) byte 4 0 starts the 8-byte effect Room60Effect_vtable (parameters from byte
\ 3), its slot kept in event variable byte 3 + 2; else that effect is ended
: old-mansion-3f-1.cmd02 ( b0 b1 -- )  drop drop s" old-mansion-3f-1.cmd02" stub-step ;
\ room 0x60 (Room60_Cmd03_ptmf): character 0xFE's model +0x9FC 0.4 / +0xA00 1 (byte 3 0), or 0
: old-mansion-3f-1.cmd03 ( b0 -- )  drop s" old-mansion-3f-1.cmd03" stub-step ;
\ character 0xFE's model +0x9E0 / +0x9E4 / +0x9E8: byte 3 0 -0.2 / 0.2 / -0.2; 1 eases them by
\ script variable 6 (a step a call, waiting (2) for 60) to 0 / 0.3 / 0; else 0 / 0.3 / 0
: old-mansion-3f-1.cmd04 ( b0 -- )  drop s" old-mansion-3f-1.cmd04" stub-step ;
\ room 0x60 (Room60_Cmd05_ptmf): the player's model +0xCC 0 (byte 3 0) or 1
: old-mansion-3f-1.cmd05 ( b0 -- )  drop s" old-mansion-3f-1.cmd05" stub-step ;

: old-mansion-3f-1.enter ( -- )   \ 004280A0
    room-sounds
    9 0 $14 door-bits
    $A 0 $14 door-bits
    $56 story-flag? not if
        0 1 $14 door-bits
        1 0 $14 door-bits
        0 0 0 $217 $4E2 obstacle-place
        1 1 0 $12D $3F3 obstacle-place
        2 2 0 $249 $519 obstacle-place
        3 3 0 $272 $544 obstacle-place
        0 ebit-clear
        1 ebit-clear
        2 ebit-clear
        3 ebit-clear
        7 0 $14 door-bits
        0 0 var-set
    else
        0 0 $14 door-bits
        1 1 $14 door-bits
        0 0 0 $20A $4D8 obstacle-place
        1 1 0 $153 $415 obstacle-place
        2 2 0 $15C $41E obstacle-place
        3 3 0 $213 $4DE obstacle-place
        0 obstacle-stop
        1 obstacle-stop
        2 obstacle-stop
        3 obstacle-stop
        0 ebit-set
        1 ebit-set
        2 ebit-set
        3 ebit-set
        7 1 $14 door-bits
        0 0 var-set
        1 0 8 nav-group
        $338 story-flag? not if
            9 1 $14 door-bits
        else
            $A 1 $14 door-bits
        then
        $7F story-flag? not if
            $7F story-flag? not if
                0 1.29 1.0 -17.02 flicker-sprite
            then
        then
    then
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    2 0 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
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
    $17 1 object-show
    $18 1 object-show
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 old-mansion-3f-1.cmd00
    1 $2300 sound-volume
;

: old-mansion-3f-1.char-enter ( -- )   \ 004281E0
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
;

: old-mansion-3f-1.phase1 ( -- )   \ 00428220
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    6 sound-bank-loaded? if
        $C0000002 6 0.0 0.0 0.0 0 0 sound
    then
    0 ebit? not if
        0 $20A obstacle-on? if
            0 obstacle-stop
            0 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            0 0 old-mansion-3f-1.cmd02
        then
    then
    1 ebit? not if
        1 $153 obstacle-on? if
            1 obstacle-stop
            1 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            1 0 old-mansion-3f-1.cmd02
        then
    then
    2 ebit? not if
        2 $15C obstacle-on? if
            2 obstacle-stop
            2 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            2 0 old-mansion-3f-1.cmd02
        then
    then
    3 ebit? not if
        3 $213 obstacle-on? if
            3 obstacle-stop
            3 ebit-set
            0 0 var? if
                $40000002 6 0.0 0.0 0.0 0 0 sound
            then
            0 var-inc
            0 1 6 char-sound
            3 0 old-mansion-3f-1.cmd02
        then
    then
    $55 story-flag? not 0 4 char-entered-area? and 7 ebit? not and if
        7 ebit-set
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 char-action? if
            0 0 0 action-force
        else
            1 0 0 action-force
        then
    then
    $55 story-flag? $56 story-flag? not and if
        0 ebit? 1 ebit? and 2 ebit? and 3 ebit? and if
            0 0.0 0.0 0.0 $A 10 0 zone
            $FE 0 3 char-zone-bits? if
                $56 story-flag-set
                $7B story-flag-set
                force-calm state-flag-set
                0 counter-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 8 action-force
                else
                    1 0 8 action-force
                then
            then
        then
        $FE char-dead? if
            6 ebit-set
        then
        6 ebit? if
            $FE char-dead? not if
                $23D item-give
            then
        then
    then
    3 old-mansion-3f-1.cmd00
    0 0 var? if
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 1 $14 door-bits
    else 0 1 var? if
        3 1 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 0 $14 door-bits
    else 0 2 var? if
        3 0 $14 door-bits
        4 1 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 0 $14 door-bits
    else 0 3 var? if
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 1 $14 door-bits
        6 0 $14 door-bits
        8 0 $14 door-bits
    else 0 4 var? if
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 1 $14 door-bits
        8 0 $14 door-bits
    else
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
        8 1 $14 door-bits
    then then then then then
;

: old-mansion-3f-1.phase2 ( -- )   \ 00428460
    scene-5-pending state-flag? if
        -2147483646 scene-request? if
            5 6 1 scene-change
        then
    then
    $56 story-flag? if
        0 5 $2D char-faces-area? if
            5 7 0 scene-change
        then
    then
    $7F story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: old-mansion-3f-1.phase3 ( -- )   \ 004284A0
    50.0 -29.0 -143.0 -8.8 9.3 -143.0 50.0 -50.0 -143.0 -8.8 -10.0 -143.0 lights-doorway
    66.0 -39.0 -143.0 50.0 -29.0 -143.0 69.0 -55.0 -143.0 50.0 -50.0 -143.0 lights-doorway
;

: old-mansion-3f-1.act00 ( -- )   \ 00428510
    3 stalker-kind? if
        4 ebit-set
    else
        4 ebit-clear
    then
    2 0 char-remove
    stalkers-stay state-flag-set
    1 self-scripted
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    force-calm state-flag-set
    self-wait-done
    4 ebit? if
        $23 partner-load
        $338 story-flag-clear
    else
        $24 partner-load
        $338 story-flag-set
    then
    summoner-on state-flag-clear
    2 char-unload
    $F $44 fade
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
    world-held state-flag-set
    $1D 3 $FF char-load
    0 old-mansion-3f-1.cmd01
    1 char-activate
    $60 0 433 hewie-to-room
    $FE $60 1571 2 stalker-to-room
    $FE 2 2 char-camera
    $FE action-end
    $FE char-done
    3 4 0 char-model-op
    3 char-unload
    0 $F9 1 action
    2 1 $14 door-bits
    $FE char-activate
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
    world-held state-flag-set
    0 $247 -0.592 1.575 180 char-to-xz
    1 $2CF 0.346 -5.781 180 char-to-xz
    $FE $50D -1.47 -23.64 0 char-to-xz
    2 0 $14 door-bits
    3 0 char-remove
    2 old-mansion-3f-1.cmd01
    1 old-mansion-3f-1.cmd03
    2 old-mansion-3f-1.cmd04
    0 old-mansion-3f-1.cmd05
    $70 door-open-clear
    $70 door-lock
    doors-room-in
    scene-5-pending state-flag-set
    $55 story-flag-set
    summoner-on state-flag-clear
    0 $247 -0.592 1.575 180 char-to-xz
    1 $2CF 0.346 -5.781 180 char-to-xz
    $FE $50D -1.47 -23.64 0 char-to-xz
    $FE 2 2 char-camera
    force-calm state-flag-clear
    8 1.0 0 bgm
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    self-idle-or-end
;

: old-mansion-3f-1.act01 ( -- )   \ 004286A0
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd05
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 old-mansion-3f-1.cmd05
    begin
        $468 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $477 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $484 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $499 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $49D cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4B3 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $4B4 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4D2 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $4D7 cutscene-cue-reached? not while
        yield
    repeat
    begin
        $4F2 cutscene-cue-reached? not while
        $2D sprites-additive
        yield
    repeat
    begin
        $E cutscene-shot? not while
        yield
    repeat
    0 old-mansion-3f-1.cmd03
    begin
        $11 cutscene-shot? not while
        yield
    repeat
    0 old-mansion-3f-1.cmd04
    begin
        $12 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd03
    begin
        $B31 cutscene-cue-reached? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd04
    begin
        $19 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd01
    begin
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-3f-1.act02 ( -- )   \ 00428750
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $FF panic-stage? not if
        $FE self-look-at
        yield
        0 5 char-in-area? if
            0.0 0.0 self-turn-to-xz
            self-wait-done
            $F00 $A self-anim-blend
            self-wait-anim
            $402 $A self-anim-blend
            self-wait-anim
        then
    then
    1 wait-counter
    $FF 1.0 0 bgm
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    $F 4 fade
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
    $FF self-look-at
    yield
    0 1 char-visible
    1 action-end
    1 char-done
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 $F9 3 action
    $1E 1 object-show
    $338 story-flag? not if
        9 1 $14 door-bits
    else
        $A 1 $14 door-bits
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
    2 6 sound-stop
    $1E 1 object-show
    scene-5-pending state-flag-clear
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    force-calm state-flag-clear
    2 0 char-remove
    7 1 $14 door-bits
    0 0 var-set
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
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
    $17 1 object-show
    $18 1 object-show
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 self-scripted
    0 panic-stage
    $7F story-flag? not if
        0 1.29 1.0 -17.02 flicker-sprite
    then
    0 0 char-visible
    1 char-activate
    1 char-full-health
    0 $1E6 -22.22 -47.34 27 char-to-xz
    1 $30 -14.71 -55.27 21 char-to-xz
    1 self-anim
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    1 0 8 nav-group
    1 old-mansion-3f-1.cmd00
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    0 camera-follow
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    $70 door-unlock
    $FE $60 0 room-doors-state
    $FE $63 0 room-doors-state
    self-idle-or-end
;

: old-mansion-3f-1.act03 ( -- )   \ 00428950
    begin
        2 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-show
    begin
        4 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-show
    begin
        5 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-3f-1.cmd00
    0 0 var-set
    begin
        $35A cutscene-cue-reached? not while
        yield
    repeat
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    begin
        6 cutscene-shot? not while
        yield
    repeat
    $1E 1 object-show
    0 0 old-mansion-3f-1.cmd02
    1 0 old-mansion-3f-1.cmd02
    2 0 old-mansion-3f-1.cmd02
    3 0 old-mansion-3f-1.cmd02
    begin
        $369 cutscene-cue-reached? not while
        yield
    repeat
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    2 6 sound-stop
    begin
        7 cutscene-shot? not while
        yield
    repeat
    2 0 old-mansion-3f-1.cmd02
    3 0 old-mansion-3f-1.cmd02
    begin
        $378 cutscene-cue-reached? not if
            yield
        else
            2 1 old-mansion-3f-1.cmd02
            3 1 old-mansion-3f-1.cmd02
            $A cutscene-shot? not if
                yield
            else
                $1E 1 object-show
                begin
                    0 cutscene-cue-reached? while
                    yield
                repeat
                self-idle-or-end
            then
        then
    again
;

: old-mansion-3f-1.act04 ( -- )   \ 00428A10
    $FE camera-follow
    1 self-scripted
    self-wait-done
    $FE $1D char-file-load
    0 -0.03 0.0 -90 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    $8000 5 self-anim-blend
    self-frames-reset
    $18D self-wait-frames
    counter-inc
    self-wait-anim
    begin
        yield
    again
;

: old-mansion-3f-1.act05 ( -- )   \ 00428A40
    self-wait-done
    0.0 -17.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $7F story-flag-set
    0 effect-remove
    $11 message-param-room
    $11 1 item-give-count
    0 $11 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    50 hewie-trust
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-3f-1.act06 ( -- )   \ 0047AD40
    1 message
    self-idle-or-end
;

: old-mansion-3f-1.act07 ( -- )   \ 00428A90
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    5 ebit? not if
        2 message
        wait-message
        5 ebit-set
    else
        $A02 $A self-anim-blend
        self-wait-anim
        3 message
        wait-message
        5 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-3f-1.act08 ( -- )   \ 00428AC0
    $F 0 fade
    stalkers-stay state-flag-set
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    1 self-scripted
    self-wait-done
    wait-fade
    world-held state-flag-set
    yield
    0 $FE 9 action-force
    1 wait-counter
    0 1 char-visible
    1 action-end
    1 char-done
    $FF 1.0 0 bgm
    $FF 3 -1 char-camera
    0 panic-stage
    counter-inc
    $F 1 fade
    3 wait-counter
    wait-fade
    $F 4 fade
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
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 $F9 3 action
    $1E 1 object-show
    $338 story-flag? not if
        9 1 $14 door-bits
    else
        $A 1 $14 door-bits
    then
    $1E $C8 movie-param
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
    2 6 sound-stop
    $1E 1 object-show
    0 0 char-visible
    scene-5-pending state-flag-clear
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    force-calm state-flag-clear
    2 0 char-remove
    7 1 $14 door-bits
    0 0 var-set
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
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
    $17 1 object-show
    $18 1 object-show
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 self-scripted
    0 panic-stage
    $7F story-flag? not if
        0 1.29 1.0 -17.02 flicker-sprite
    then
    0 0 char-visible
    1 char-activate
    1 char-full-health
    0 $1E6 -22.22 -47.34 27 char-to-xz
    0 self-move-16
    1 $30 -14.71 -55.27 21 char-to-xz
    $29 0 hewie-action
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    1 0 8 nav-group
    1 old-mansion-3f-1.cmd00
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    0 camera-follow
    $F $41 fade
    wait-fade
    $23E item-give
    $8282 item-give
    shared.act9C
    stalkers-stay state-flag-clear
    $70 door-unlock
    $FE $60 0 room-doors-state
    $FE $63 0 room-doors-state
    self-idle-or-end
;

: old-mansion-3f-1.act09 ( -- )   \ 00428CD0
    1 self-scripted
    self-wait-done
    $FE $1D char-file-load
    $FE char-file-use
    counter-inc
    2 wait-counter
    $FE $1AB 0.25 0.25 -90 char-to-xz
    $8000 self-anim
    self-frames-reset
    $96 self-wait-frames
    $FE 3 6 char-sound
    self-frames-reset
    $F7 self-wait-frames
    self-wait-anim
    counter-inc
    begin
        yield
    again
;

: old-mansion-3f-1.act0A ( -- )   \ 00428D10
    self-wait-done
    9 0 $14 door-bits
    $A 0 $14 door-bits
    8 1 $14 door-bits
    0 1 $14 door-bits
    1 0 $14 door-bits
    0 0 0 $217 $4E2 obstacle-place
    1 1 0 $12D $3F3 obstacle-place
    2 2 0 $249 $519 obstacle-place
    3 3 0 $272 $544 obstacle-place
    0 ebit-clear
    1 ebit-clear
    2 ebit-clear
    3 ebit-clear
    7 0 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
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
    $17 1 object-show
    $18 1 object-show
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 old-mansion-3f-1.cmd00
    $23 partner-load
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
    $1D 3 $FF char-load
    0 old-mansion-3f-1.cmd01
    3 4 0 char-model-op
    3 char-unload
    0 $F9 1 action
    2 1 $14 door-bits
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
    2 0 $14 door-bits
    3 0 char-remove
    2 old-mansion-3f-1.cmd01
    1 old-mansion-3f-1.cmd03
    2 old-mansion-3f-1.cmd04
    0 old-mansion-3f-1.cmd05
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-3f-1.act0B ( -- )   \ 00428E70
    self-wait-done
    9 0 $14 door-bits
    $A 0 $14 door-bits
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 0 0 $20A $4D8 obstacle-place
    1 1 0 $153 $415 obstacle-place
    2 2 0 $15C $41E obstacle-place
    3 3 0 $213 $4DE obstacle-place
    0 obstacle-stop
    1 obstacle-stop
    2 obstacle-stop
    3 obstacle-stop
    0 ebit-set
    1 ebit-set
    2 ebit-set
    3 ebit-set
    7 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 1 $14 door-bits
    8 0 $14 door-bits
    2 0 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
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
    $17 1 object-show
    $18 1 object-show
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    0 4 var-set
    0 old-mansion-3f-1.cmd00
    0 0 old-mansion-3f-1.cmd02
    1 0 old-mansion-3f-1.cmd02
    2 0 old-mansion-3f-1.cmd02
    3 0 old-mansion-3f-1.cmd02
    $23 partner-load
    2 char-unload
    0 1 char-visible
    $FF 3 -1 char-camera
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
    0 0 $14 door-bits
    1 1 $14 door-bits
    0 $F9 3 action
    $1E 1 object-show
    9 1 $14 door-bits
    $1E $C8 movie-param
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
    2 6 sound-stop
    $1E 1 object-show
    2 0 char-remove
    7 1 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    8 1 $14 door-bits
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
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
    $17 1 object-show
    $18 1 object-show
    $19 1 object-show
    $1A 1 object-show
    $1B 1 object-show
    1 old-mansion-3f-1.cmd00
    0 1 old-mansion-3f-1.cmd02
    1 1 old-mansion-3f-1.cmd02
    2 1 old-mansion-3f-1.cmd02
    3 1 old-mansion-3f-1.cmd02
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-3f-1.enter old-mansion-3f-1 0 room-script!
' old-mansion-3f-1.char-enter old-mansion-3f-1 6 room-script!
' old-mansion-3f-1.phase1 old-mansion-3f-1 1 room-script!
' old-mansion-3f-1.phase2 old-mansion-3f-1 2 room-script!
' old-mansion-3f-1.phase3 old-mansion-3f-1 3 room-script!
' old-mansion-3f-1.act00 old-mansion-3f-1 $00 action-script!
' old-mansion-3f-1.act01 old-mansion-3f-1 $01 action-script!
' old-mansion-3f-1.act02 old-mansion-3f-1 $02 action-script!
' old-mansion-3f-1.act03 old-mansion-3f-1 $03 action-script!
' old-mansion-3f-1.act04 old-mansion-3f-1 $04 action-script!
' old-mansion-3f-1.act05 old-mansion-3f-1 $05 action-script!
' old-mansion-3f-1.act06 old-mansion-3f-1 $06 action-script!
' old-mansion-3f-1.act07 old-mansion-3f-1 $07 action-script!
' old-mansion-3f-1.act08 old-mansion-3f-1 $08 action-script!
' old-mansion-3f-1.act09 old-mansion-3f-1 $09 action-script!
' old-mansion-3f-1.act0A old-mansion-3f-1 $0A action-script!
' old-mansion-3f-1.act0B old-mansion-3f-1 $0B action-script!
