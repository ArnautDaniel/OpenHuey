\ story/rooms/water-tower-7f-1.fs - the event scripts of room water-tower-7f-1 ($C5; Water Tower: 7F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-7f-1
USING: room-names story.words story.shared ;

\ the 0x10-byte effect ObjectGlow_vtable on room object k + 1 (byte 4 = k, 1..8; script variable
\ 11 - k keeps its slot): made on first use when byte 3 is set, then sent (on byte 3, index 8 -
\ k, the variable, the object), with sound 1 at the object when on and the camera director's
\ +0x38 is clear
: water-tower-7f-1.cmd00 ( b0 b1 -- )  drop drop s" water-tower-7f-1.cmd00" stub-step ;

: water-tower-7f-1.enter ( -- )   \ 004401B0
    room-sounds
    $22 state-flag-set
    $30 1.0 0 bgm
    2 8 var-set
    3 -1 var-set
    4 -1 var-set
    5 -1 var-set
    6 -1 var-set
    7 -1 var-set
    8 -1 var-set
    9 -1 var-set
    $A -1 var-set
    1 8 water-tower-7f-1.cmd00
    $FE action-end
    1 summon-take
    $34A story-flag? not if
        $34A story-flag-set
        1 creatures-clear
    then
    1 $2300 sound-volume
;

: water-tower-7f-1.char-enter ( -- )   \ 00440210
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
;

: water-tower-7f-1.phase1 ( -- )   \ 00440250
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    6 sound-bank-loaded? if
        $C var-inc
        $C 30 var? if
            $C 0 var-set
            $B 0 var? if
                $40000005 6 -127.0 1.7 10.0 0 0 sound
            else $B 1 var? if
                $40000006 6 -127.0 1.7 10.0 0 0 sound
            else $B 2 var? if
                $40000007 6 -127.0 1.7 10.0 0 0 sound
            else $B 3 var? if
                $40000008 6 -127.0 1.7 10.0 0 0 sound
            then then then then
            $B var-inc
            $B 4 var? if
                $B 0 var-set
            then
        then
    then
    8 ebit? not if
        0 0 var? if
            9 ebit-set
        else
            9 ebit-clear
        then
        1 0 var? if
            $A ebit-set
        else
            $A ebit-clear
        then
        0 0 var-set
        1 0 var-set
        0 0.0 0.0 0.0 $A 5 0 zone
        0 0 8 char-zone-bits? if
            0 1 var-set
        then
        1 0 8 char-zone-bits? if
            1 1 var-set
        then
        1 -22.0 0.0 0.0 5 5 0 zone
        0 1 8 char-zone-bits? if
            0 2 var-set
        then
        1 1 8 char-zone-bits? if
            1 2 var-set
        then
        2 8.0 0.0 -30.0 5 5 0 zone
        0 0 var? if
            0 2 8 char-zone-bits? if
                0 3 var-set
            then
        then
        1 0 var? if
            1 2 8 char-zone-bits? if
                1 3 var-set
            then
        then
        3 34.65 0.0 20.0 5 5 0 zone
        0 0 var? if
            0 3 8 char-zone-bits? if
                0 4 var-set
            then
        then
        1 0 var? if
            1 3 8 char-zone-bits? if
                1 4 var-set
            then
        then
        4 -25.0 0.0 43.31 5 5 0 zone
        0 0 var? if
            0 4 8 char-zone-bits? if
                0 5 var-set
            then
        then
        1 0 var? if
            1 4 8 char-zone-bits? if
                1 5 var-set
            then
        then
        5 -40.37 0.0 -40.38 5 5 0 zone
        0 0 var? if
            0 5 8 char-zone-bits? if
                0 6 var-set
            then
        then
        1 0 var? if
            1 5 8 char-zone-bits? if
                1 6 var-set
            then
        then
        6 48.58 0.0 -48.58 5 5 0 zone
        0 0 var? if
            0 6 8 char-zone-bits? if
                0 7 var-set
            then
        then
        1 0 var? if
            1 6 8 char-zone-bits? if
                1 7 var-set
            then
        then
        7 57.25 0.0 57.25 5 5 0 zone
        0 0 var? if
            0 7 8 char-zone-bits? if
                0 8 var-set
            then
        then
        1 0 var? if
            1 7 8 char-zone-bits? if
                1 8 var-set
            then
        then
        9 ebit? 0 0 var? not and if
            0 1 var? if
                0 $11 6 char-sound
            else 0 2 var? if
                0 $10 6 char-sound
            else 0 3 var? if
                0 $F 6 char-sound
            else 0 4 var? if
                0 $E 6 char-sound
            else 0 5 var? if
                0 $D 6 char-sound
            else 0 6 var? if
                0 $C 6 char-sound
            else 0 7 var? if
                0 $B 6 char-sound
            else 0 8 var? if
                0 $A 6 char-sound
            then then then then then then then then
        else $A ebit? 1 0 var? not and if
            1 1 var? if
                1 $11 6 char-sound
            else 1 2 var? if
                1 $10 6 char-sound
            else 1 3 var? if
                1 $F 6 char-sound
            else 1 4 var? if
                1 $E 6 char-sound
            else 1 5 var? if
                1 $D 6 char-sound
            else 1 6 var? if
                1 $C 6 char-sound
            else 1 7 var? if
                1 $B 6 char-sound
            else 1 8 var? if
                1 $A 6 char-sound
            then then then then then then then then
        then then
        9 ebit? not 0 0 var? and if
        else $A ebit? not 1 0 var? and if
        then then
        2 8 var? if
            0 8 var? 1 8 var? or if
                2 7 var-set
                1 7 water-tower-7f-1.cmd00
            then
        else 2 7 var? if
            0 8 var? 1 8 var? or if
                0 7 var? 1 7 var? or if
                    2 6 var-set
                    1 6 water-tower-7f-1.cmd00
                then
            else
                2 8 var-set
                0 7 water-tower-7f-1.cmd00
                0 ebit-clear
            then
        else 2 6 var? if
            0 7 var? 1 7 var? or if
                0 6 var? 1 6 var? or if
                    2 5 var-set
                    1 5 water-tower-7f-1.cmd00
                then
            else
                0 6 water-tower-7f-1.cmd00
                0 8 var? 1 8 var? or if
                    2 7 var-set
                    1 ebit-clear
                else
                    2 8 var-set
                    0 7 water-tower-7f-1.cmd00
                    1 ebit-clear
                    0 ebit-clear
                then
            then
        else 2 5 var? if
            0 6 var? 1 6 var? or if
                0 5 var? 1 5 var? or if
                    2 4 var-set
                    1 4 water-tower-7f-1.cmd00
                then
            else
                0 5 water-tower-7f-1.cmd00
                0 7 var? 1 7 var? or if
                    2 6 var-set
                    2 ebit-clear
                else
                    0 6 water-tower-7f-1.cmd00
                    0 8 var? 1 8 var? or if
                        2 7 var-set
                        1 ebit-clear
                    else
                        2 8 var-set
                        0 7 water-tower-7f-1.cmd00
                        1 ebit-clear
                        0 ebit-clear
                    then
                then
            then
        else 2 4 var? if
            0 5 var? 1 5 var? or if
                0 4 var? 1 4 var? or if
                    2 3 var-set
                    1 3 water-tower-7f-1.cmd00
                then
            else
                0 4 water-tower-7f-1.cmd00
                0 6 var? 1 6 var? or if
                    2 5 var-set
                    3 ebit-clear
                else
                    0 5 water-tower-7f-1.cmd00
                    0 7 var? 1 7 var? or if
                        2 6 var-set
                        2 ebit-clear
                    else
                        0 6 water-tower-7f-1.cmd00
                        0 8 var? 1 8 var? or if
                            2 7 var-set
                            1 ebit-clear
                        else
                            2 8 var-set
                            0 7 water-tower-7f-1.cmd00
                            1 ebit-clear
                            0 ebit-clear
                        then
                    then
                then
            then
        else 2 3 var? if
            0 4 var? 1 4 var? or if
                0 3 var? 1 3 var? or if
                    2 2 var-set
                    1 2 water-tower-7f-1.cmd00
                then
            else
                0 3 water-tower-7f-1.cmd00
                0 5 var? 1 5 var? or if
                    2 4 var-set
                    4 ebit-clear
                else
                    0 4 water-tower-7f-1.cmd00
                    0 6 var? 1 6 var? or if
                        2 5 var-set
                        3 ebit-clear
                    else
                        0 5 water-tower-7f-1.cmd00
                        0 7 var? 1 7 var? or if
                            2 6 var-set
                            2 ebit-clear
                        else
                            0 6 water-tower-7f-1.cmd00
                            0 8 var? 1 8 var? or if
                                2 7 var-set
                                1 ebit-clear
                            else
                                2 8 var-set
                                0 7 water-tower-7f-1.cmd00
                                1 ebit-clear
                                0 ebit-clear
                            then
                        then
                    then
                then
            then
        else 2 2 var? if
            0 3 var? 1 3 var? or if
                0 2 var? 1 2 var? or if
                    2 1 var-set
                    1 1 water-tower-7f-1.cmd00
                then
            else
                0 2 water-tower-7f-1.cmd00
                0 4 var? 1 4 var? or if
                    2 3 var-set
                    5 ebit-clear
                else
                    0 3 water-tower-7f-1.cmd00
                    0 5 var? 1 5 var? or if
                        2 4 var-set
                        4 ebit-clear
                    else
                        0 4 water-tower-7f-1.cmd00
                        0 6 var? 1 6 var? or if
                            2 5 var-set
                            3 ebit-clear
                        else
                            0 5 water-tower-7f-1.cmd00
                            0 7 var? 1 7 var? or if
                                2 6 var-set
                                2 ebit-clear
                            else
                                0 6 water-tower-7f-1.cmd00
                                0 8 var? 1 8 var? or if
                                    2 7 var-set
                                    1 ebit-clear
                                else
                                    2 8 var-set
                                    0 7 water-tower-7f-1.cmd00
                                    1 ebit-clear
                                    0 ebit-clear
                                then
                            then
                        then
                    then
                then
            then
        else 2 1 var? if
            0 2 var? 1 2 var? or if
                0 1 var? 1 1 var? or if
                    8 ebit-set
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 0 action-force
                    else
                        1 0 0 action-force
                    then
                then
            else
                0 1 water-tower-7f-1.cmd00
                0 3 var? 1 3 var? or if
                    2 2 var-set
                    6 ebit-clear
                else
                    0 2 water-tower-7f-1.cmd00
                    0 4 var? 1 4 var? or if
                        2 3 var-set
                        5 ebit-clear
                    else
                        0 3 water-tower-7f-1.cmd00
                        0 5 var? 1 5 var? or if
                            2 4 var-set
                            4 ebit-clear
                        else
                            0 4 water-tower-7f-1.cmd00
                            0 6 var? 1 6 var? or if
                                2 5 var-set
                                3 ebit-clear
                            else
                                0 5 water-tower-7f-1.cmd00
                                0 7 var? 1 7 var? or if
                                    2 6 var-set
                                    2 ebit-clear
                                else
                                    0 6 water-tower-7f-1.cmd00
                                    0 8 var? 1 8 var? or if
                                        2 7 var-set
                                        1 ebit-clear
                                    else
                                        2 8 var-set
                                        0 7 water-tower-7f-1.cmd00
                                        1 ebit-clear
                                        0 ebit-clear
                                    then
                                then
                            then
                        then
                    then
                then
            then
        then then then then then then then then
        35 fiona-started? if
            2 8 var? 0 8 var? not and if
                0 57 57 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 7 var? 0 7 var? not and if
                0 49 -49 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 6 var? 0 6 var? not and if
                0 -40 -40 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 5 var? 0 5 var? not and if
                0 -25 43 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 4 var? 0 4 var? not and if
                0 35 20 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 3 var? 0 3 var? not and if
                0 8 -30 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 2 var? 0 2 var? not and if
                0 -22 0 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
            2 1 var? if
                0 0 0 $32 char-faces-xz? if
                    hewie-stays? if
                        0 1 1 action
                    then
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            7 ebit? not if
                0 0.0 0.0 0.0 $A 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 0 9 char-zone-bits? if
                            7 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                0 0.0 0.0 0.0 $C 5 0 zone
                1 0 9 char-zone-bits? not if
                    7 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            6 ebit? not if
                1 -22.0 0.0 0.0 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 1 9 char-zone-bits? if
                            6 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                1 -22.0 0.0 0.0 6 5 0 zone
                1 1 9 char-zone-bits? not if
                    6 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            5 ebit? not if
                2 8.0 0.0 -30.0 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 2 9 char-zone-bits? if
                            5 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                2 8.0 0.0 -30.0 6 5 0 zone
                1 2 9 char-zone-bits? not if
                    5 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            4 ebit? not if
                3 34.65 0.0 20.0 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 3 9 char-zone-bits? if
                            4 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                3 34.65 0.0 20.0 7 5 0 zone
                1 3 9 char-zone-bits? not if
                    4 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            3 ebit? not if
                4 -25.0 0.0 43.31 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 4 9 char-zone-bits? if
                            3 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                4 -25.0 0.0 43.31 7 5 0 zone
                1 4 9 char-zone-bits? not if
                    3 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            2 ebit? not if
                5 -40.37 0.0 -40.38 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 5 9 char-zone-bits? if
                            2 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                5 -40.37 0.0 -40.38 6 5 0 zone
                1 5 9 char-zone-bits? not if
                    2 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            1 ebit? not if
                6 48.58 0.0 -48.58 4 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 6 9 char-zone-bits? if
                            1 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                6 48.58 0.0 -48.58 6 5 0 zone
                1 6 9 char-zone-bits? not if
                    1 ebit-clear
                then
            then
        then
        1 char-here? 1 0 char-C4? and if
            0 ebit? not if
                7 57.25 0.0 57.25 5 5 0 zone
                2 game-mode? not if
                    hewie-can-command? if
                        1 7 9 char-zone-bits? if
                            0 ebit-set
                            $80 0 hewie-action
                        then
                    then
                then
            else
                7 57.25 0.0 57.25 7 5 0 zone
                1 7 9 char-zone-bits? not if
                    0 ebit-clear
                then
            then
        then
    then
;

: water-tower-7f-1.phase2 ( -- )   \ 00440E00
    0 $D $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 $E $32 char-faces-area? if
        5 3 0 scene-change
    then
;

: water-tower-7f-1.act00 ( -- )   \ 00440E20
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $FF 1.0 0 bgm
    $F $54 fade
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
    things-clear
    -1 self-move-16
    $C6 room-preload
    $10 $FF movie-param
    0 $F9 2 action
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
    50 hewie-trust
    8 state-flag-set
    $FF door-reopen-unlock
    $101 door-close-off-lock
    exits-rebuild
    $C6 0 -1 hewie-to-room
    $3E resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: water-tower-7f-1.act01 ( -- )   \ 00440EE0
    self-wait-done
    hewie-bark
    self-wait-done
    2 8 var? if
        $37 57.25 57.25 -175 $204 5 self-move-to
        self-wait-done
    else 2 7 var? if
        $5C 48.58 -48.58 -84 $204 5 self-move-to
        self-wait-done
    else 2 6 var? if
        $FA -40.37 -40.38 10 $204 5 self-move-to
        self-wait-done
    else 2 5 var? if
        $1B -25.0 43.31 111 $204 5 self-move-to
        self-wait-done
    else 2 4 var? if
        $3F 34.65 20.0 -151 $204 5 self-move-to
        self-wait-done
    else 2 3 var? if
        $6A 8.0 -30.0 -44 $204 5 self-move-to
        self-wait-done
    else 2 2 var? if
        4 -22.0 0.0 89 $204 5 self-move-to
        self-wait-done
    else 2 1 var? if
        $D8 0.0 0.0 89 $204 5 self-move-to
        self-wait-done
    then then then then then then then then
    0 $A hewie-anim-root
    self-frames-reset
    $A self-wait-frames
    self-idle-or-end
;

: water-tower-7f-1.act02 ( -- )   \ 00440FC0
    begin
        $33C cutscene-cue-reached? not while
        yield
    repeat
    0 8 water-tower-7f-1.cmd00
    0 7 water-tower-7f-1.cmd00
    0 6 water-tower-7f-1.cmd00
    0 5 water-tower-7f-1.cmd00
    0 4 water-tower-7f-1.cmd00
    0 3 water-tower-7f-1.cmd00
    0 2 water-tower-7f-1.cmd00
    0 1 water-tower-7f-1.cmd00
    self-idle-or-end
;

: water-tower-7f-1.act03 ( -- )   \ 00441000
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0.0 -105.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-1.act04 ( -- )   \ 00441030
    self-wait-done
    3 -1 var-set
    4 -1 var-set
    5 -1 var-set
    6 -1 var-set
    7 -1 var-set
    8 -1 var-set
    9 -1 var-set
    $A -1 var-set
    1 8 water-tower-7f-1.cmd00
    1 7 water-tower-7f-1.cmd00
    1 6 water-tower-7f-1.cmd00
    1 5 water-tower-7f-1.cmd00
    1 4 water-tower-7f-1.cmd00
    1 3 water-tower-7f-1.cmd00
    1 2 water-tower-7f-1.cmd00
    1 1 water-tower-7f-1.cmd00
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
    $10 $FF movie-param
    0 $F9 2 action
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: water-tower-7f-1.phase5 ( -- )   \ 0047AF20
    $22 state-flag-clear
;

\ ---- registered ----
' water-tower-7f-1.enter water-tower-7f-1 0 room-script!
' water-tower-7f-1.char-enter water-tower-7f-1 6 room-script!
' water-tower-7f-1.phase1 water-tower-7f-1 1 room-script!
' water-tower-7f-1.phase2 water-tower-7f-1 2 room-script!
' water-tower-7f-1.act00 water-tower-7f-1 $00 action-script!
' water-tower-7f-1.act01 water-tower-7f-1 $01 action-script!
' water-tower-7f-1.act02 water-tower-7f-1 $02 action-script!
' water-tower-7f-1.act03 water-tower-7f-1 $03 action-script!
' water-tower-7f-1.act04 water-tower-7f-1 $04 action-script!
' water-tower-7f-1.phase5 water-tower-7f-1 5 room-script!
