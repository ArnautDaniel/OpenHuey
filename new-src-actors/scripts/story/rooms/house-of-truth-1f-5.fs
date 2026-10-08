\ story/rooms/house-of-truth-1f-5.fs - the event scripts of room house-of-truth-1f-5 ($8D; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-5
USING: room-names story.words story.shared flag-names ;

\ room 0x8D: three grey smoke effects (Effect79B00, size 60) at the room's spots 7, 4, 3
\ (grey_three).
: house-of-truth-1f-5.cmd00 ( -- )  s" house-of-truth-1f-5.cmd00" stub-step ;
\ byte 3: 0 made (lights 0x14 on characters 3 and 0) and 1 lit on character 3 (second kind, size
\ 1, sparks from its own slot); 2 ending, 3 ended; 4 / 6 sized 1 / 0.5; 5 / 7 shrinking with
\ variable 1 (a step a call, to 0.5 + v / 200 or v / 334)
: house-of-truth-1f-5.cmd01 ( b0 -- )  drop s" house-of-truth-1f-5.cmd01" stub-step ;
\ room 0x8D: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-5.cmd02 ( -- )  s" house-of-truth-1f-5.cmd02" stub-step ;

: house-of-truth-1f-5.enter ( -- )   \ 004348E0
    room-sounds
    2 0 var-set
    3 0 var-set
    $A4 story-flag? $96 story-flag? not and if
        1 ebit-set
    then
    1 ebit? if
        stalkers-stay state-flag-set
    then
    $A4 story-flag? if
        1 0 $20000 nav-group
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $96 story-flag? if
        2 1 $14 door-bits
    then
    $34E story-flag? if
        5 1.0 0 bgm
    then
    shared.act95
    $A3 story-flag? if
        house-of-truth-1f-5.cmd00
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-5.char-enter ( -- )   \ 00434940
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
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
;

: house-of-truth-1f-5.phase1 ( -- )   \ 00434A00
    3 ebit? not if
        $A4 story-flag? if
            2 ebit? not if
                6 sound-bank-loaded? if
                    $40000000 6 0.0 20.0 100.0 0 0 sound
                    2 ebit-set
                then
            else
                $C0000000 6 -60.0 20.0 50.0 0 0 sound
            then
        then
    then
    4 ebit? if
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
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    0 $B char-entered-area? if
        1 ebit? if
            1 ebit-clear
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
    house-of-truth-1f-5.cmd02
    3 ebit? not if
        6 sound-bank-loaded? if
            3 var-inc
            3 30 var? if
                3 0 var-set
                2 0 var? if
                    $40000003 6 28.0 20.0 -71.0 0 0 sound
                else 2 1 var? if
                    $40000004 6 28.0 20.0 -71.0 0 0 sound
                else 2 2 var? if
                    $40000005 6 28.0 20.0 -71.0 0 0 sound
                else 2 3 var? if
                    $40000006 6 28.0 20.0 -71.0 0 0 sound
                then then then then
                2 var-inc
                2 4 var? if
                    2 0 var-set
                then
            then
        then
    then
;

: house-of-truth-1f-5.phase2 ( -- )   \ 00434B50
    0 $A char-in-area? 0 67 $32 char-heading? and if
        $96 story-flag? not if
            5 $84 3 scene-change
        else
            5 6 0 scene-change
        then
    then
    0 $C char-in-area? 0 0 $32 char-heading? and if
        $96 story-flag? if
            5 1 0 scene-change
        else
            5 2 0 scene-change
        then
    then
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            $96 story-flag? not if
                5 3 1 scene-change
            else
                5 6 0 scene-change
            then
        then
        0 1 char-group-bit4? if
            $96 story-flag? not if
                5 4 1 scene-change
            else
                5 6 0 scene-change
            then
        then
        0 2 char-group-bit4? if
            $96 story-flag? if
                5 6 0 scene-change
            then
        then
    then
    $96 story-flag? if
        0 0.0 0.0 74.25 8 5 0 zone
        0 0 8 char-zone-bits? if
            5 8 0 scene-change
        then
    then
;

: house-of-truth-1f-5.phase3 ( -- )   \ 00434BE0
    $A3 story-flag? 4 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        4 ebit-set
    then
;

: house-of-truth-1f-5.phase5 ( -- )   \ 00434C00
    1 ebit? if
        stalkers-stay state-flag-clear
    then
;

: house-of-truth-1f-5.act00 ( -- )   \ 00434C10
    1 self-scripted
    $34E story-flag-clear
    $FF 1.0 0 bgm
    $F8 action-end
    $F $14 fade
    self-wait-done
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    $1E 3 $FF char-load
    $1F 4 $FF char-load
    force-followed state-flag-set
    3 char-unload
    4 char-unload
    0 1 $14 door-bits
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
    0 $F9 5 action
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
    2 house-of-truth-1f-5.cmd01
    3 0 char-remove
    4 0 char-remove
    0 $63 -0.61 47.71 0 char-to-xz
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    1 char-activate
    $8D 0 123 hewie-to-room
    1 $7B -20 char-to-tri-facing
    hewie-no-attack state-flag-clear
    2 1 $14 door-bits
    $EA door-open-clear
    $EA door-lock
    doors-room-in
    3 0.0 0.0 65.0 $80 $80 $80 1 dust
    $F $11 fade
    wait-fade
    $51 resident-flag-set
    $257 item-give
    $828E item-give
    stalkers-stay state-flag-clear
    $96 story-flag-set
    1 ebit-clear
    shared.act95
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-5.act01 ( -- )   \ 00434D50
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    2 message
    wait-message
    0 answer? if
        $F $54 fade
        $35 room-preload
        $1C 1.0 1 bgm
        wait-fade
        3 ebit-set
        $30 7 sound-stop
        0 6 sound-stop
        3 6 sound-stop
        4 6 sound-stop
        5 6 sound-stop
        6 6 sound-stop
        begin
            0 adx? not while
            yield
        repeat
        1.0 sound-volume-scale
        0 0.0 $FF bgm
        begin
            1 adx? not while
            yield
        repeat
        $81 exit-check
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-5.act02 ( -- )   \ 00434DC0
    self-wait-done
    $CA 0.0 96.99 0 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $1200 self-anim
    self-wait-anim
    $1203 self-anim
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 message
    wait-message
    self-idle-or-end
;

: house-of-truth-1f-5.act03 ( -- )   \ 00434DF0
    self-wait-done
    0 self-through-exit
    self-wait-done
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
    self-idle-or-end
;

: house-of-truth-1f-5.act04 ( -- )   \ 00434E20
    self-wait-done
    1 self-through-exit
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60B self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $609 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        4 message
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-5.act05 ( -- )   \ 00434E50
    begin
        0 cutscene-shot? not while
        yield
    repeat
    0 house-of-truth-1f-5.cmd01
    begin
        2 cutscene-shot? not while
        yield
    repeat
    2 house-of-truth-1f-5.cmd01
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 house-of-truth-1f-5.cmd01
    begin
        4 cutscene-shot? not while
        yield
    repeat
    3 house-of-truth-1f-5.cmd01
    begin
        $1E0 cutscene-cue-reached? not while
        yield
    repeat
    4 house-of-truth-1f-5.cmd01
    begin
        7 cutscene-shot? not while
        yield
    repeat
    2 house-of-truth-1f-5.cmd01
    begin
        8 cutscene-shot? not while
        yield
    repeat
    0 house-of-truth-1f-5.cmd01
    begin
        9 cutscene-shot? not while
        yield
    repeat
    2 house-of-truth-1f-5.cmd01
    begin
        $A cutscene-shot? not while
        yield
    repeat
    0 house-of-truth-1f-5.cmd01
    begin
        $B cutscene-shot? not while
        yield
    repeat
    2 house-of-truth-1f-5.cmd01
    begin
        $C cutscene-shot? not while
        yield
    repeat
    0 house-of-truth-1f-5.cmd01
    6 house-of-truth-1f-5.cmd01
    begin
        $58C cutscene-cue-reached? not while
        yield
    repeat
    4 house-of-truth-1f-5.cmd01
    begin
        $D cutscene-shot? not while
        yield
    repeat
    2 house-of-truth-1f-5.cmd01
    begin
        $E cutscene-shot? not while
        yield
    repeat
    0 house-of-truth-1f-5.cmd01
    begin
        $65E cutscene-cue-reached? not while
        yield
    repeat
    1 334 var-set
    begin
        0 cutscene-cue-reached? while
        7 house-of-truth-1f-5.cmd01
        yield
    repeat
    self-idle-or-end
;

: house-of-truth-1f-5.act06 ( -- )   \ 00434F10
    self-wait-done
    1 self-anim
    5 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-5.act07 ( -- )   \ 00434F20
    self-wait-done
    house-of-truth-1f-5.cmd00
    $1E 3 $FF char-load
    $1F 4 $FF char-load
    3 char-unload
    4 char-unload
    0 1 $14 door-bits
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
    0 $F9 5 action
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
    2 house-of-truth-1f-5.cmd01
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: house-of-truth-1f-5.act08 ( -- )   \ 00434FC8
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    3 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-1f-5.enter house-of-truth-1f-5 0 room-script!
' house-of-truth-1f-5.char-enter house-of-truth-1f-5 6 room-script!
' house-of-truth-1f-5.phase1 house-of-truth-1f-5 1 room-script!
' house-of-truth-1f-5.phase2 house-of-truth-1f-5 2 room-script!
' house-of-truth-1f-5.phase3 house-of-truth-1f-5 3 room-script!
' house-of-truth-1f-5.phase5 house-of-truth-1f-5 5 room-script!
' house-of-truth-1f-5.act00 house-of-truth-1f-5 $00 action-script!
' house-of-truth-1f-5.act01 house-of-truth-1f-5 $01 action-script!
' house-of-truth-1f-5.act02 house-of-truth-1f-5 $02 action-script!
' house-of-truth-1f-5.act03 house-of-truth-1f-5 $03 action-script!
' house-of-truth-1f-5.act04 house-of-truth-1f-5 $04 action-script!
' house-of-truth-1f-5.act05 house-of-truth-1f-5 $05 action-script!
' house-of-truth-1f-5.act06 house-of-truth-1f-5 $06 action-script!
' house-of-truth-1f-5.act07 house-of-truth-1f-5 $07 action-script!
' house-of-truth-1f-5.act08 house-of-truth-1f-5 $08 action-script!
