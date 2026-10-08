\ story/rooms/old-mansion-2f-6.fs - the event scripts of room old-mansion-2f-6 ($59; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-6
USING: room-names story.words story.shared ;

\ room 0x59 (Room59_Cmd00_ptmf): character 0xFE's model +0x9E0 = 0.1 (byte 3 0) or 0
: old-mansion-2f-6.cmd00 ( b0 -- )  drop s" old-mansion-2f-6.cmd00" stub-step ;

: old-mansion-2f-6.enter ( -- )   \ 00410070
    room-sounds
    $324 story-flag? if
        8 1 object-show
    then
    $323 story-flag? if
        9 1 object-show
    then
    0 0 $14 door-bits
    $88 story-flag? $89 story-flag? not and if
        $80 exit-taken? if
            4 ebit-set
        then
    then
    4 ebit? not if
        $276 story-flag? not if
            0 4.58 9.0 14.22 flicker-sprite
        then
    then
;

: old-mansion-2f-6.char-enter ( -- )   \ 004100B0
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
        4 ebit? if
            0 $4B 0 char-to-tri-facing
            8 state-flag-set
            0.0 sound-volume-scale
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 6 action-force
        then
    then
;

: old-mansion-2f-6.phase1 ( -- )   \ 00410110
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
;

: old-mansion-2f-6.phase2 ( -- )   \ 00410140
    $324 story-flag? not if
        0 7 char-in-area? 0 45 $32 char-heading? and if
            3 stalker-kind? $22 stalker-kind? or 4 stalker-kind? or 2 game-mode? and $FF panic-stage? not and $FE 2 char-C4? not and if
                $FE char-here? if
                    $8016 scene-ending
                else
                    5 1 6 scene-change
                then
            else
                5 3 0 scene-change
            then
        then
    then
    $323 story-flag? not if
        0 8 char-in-area? 0 -45 $32 char-heading? and if
            3 stalker-kind? $22 stalker-kind? or 4 stalker-kind? or 2 game-mode? and $FF panic-stage? not and $FE 2 char-C4? not and if
                $FE char-here? if
                    $8016 scene-ending
                else
                    5 2 6 scene-change
                then
            else
                5 4 0 scene-change
            then
        then
    then
    0 9 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $A char-in-area? 0 -15 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 $B char-in-area? 0 -16 -17 $3C char-faces-xz? and if
        5 7 0 scene-change
    then
    0 $C char-in-area? 0 -35 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 $E char-in-area? 0 0 $3C char-heading? and if
        5 9 0 scene-change
    then
    0 $D char-in-area? 0 10 $3C char-heading? and if
        5 $A 0 scene-change
    then
    $276 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    0 $F $3C char-faces-area? if
        5 $B 0 scene-change
    then
;

: old-mansion-2f-6.act00 ( -- )   \ 00410230
    2 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 $A char-file-load
        $7A 30.0 40.5 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        7 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        0 $84 5 char-sound
        3 avoid-prompt
        2 ebit-set
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        8 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-2f-6.act01 ( -- )   \ 004102F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    3 stalker-kind? $22 stalker-kind? or if
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
        3 4 0 char-model-op
    else
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
        3 5 0 char-model-op
    then
    wait-fade
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    $10 $FF movie-param
    3 stalker-kind? $22 stalker-kind? or if
        0 $F9 $D action
    else
        0 $F9 $C action
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
    3 stalker-kind? $22 stalker-kind? or if
        1 old-mansion-2f-6.cmd00
    then
    wait-fade
    8 1 object-show
    0 self-move-16
    3 stalker-kind? $22 stalker-kind? or if
        0 $4A -7.575 18.619 90 char-to-xz
        $FE $59 16 2 stalker-to-room
        $FE 2 2 char-camera
        $FE $10 -90 char-to-tri-facing
        $FE 0 stalker-mode
        $35 resident-flag-set
    else
        0 $4A -7.719 18.622 90 char-to-xz
        $FE $59 130 2 stalker-to-room
        $FE 2 2 char-camera
        $FE $82 -90 char-to-tri-facing
        $FE 0 stalker-mode
        $37 resident-flag-set
    then
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    camera-restart
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $59 0 3 hewie-to-room
            1 3 -7.85 29.94 52 char-to-xz
        then
    then
    $324 story-flag-set
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-6.act02 ( -- )   \ 00410470
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    3 stalker-kind? $22 stalker-kind? or if
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
        3 4 0 char-model-op
    else
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
        $10 $FF movie-param
        3 5 0 char-model-op
    then
    wait-fade
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    7 state-flag-set
    yield
    7 state-flag-clear
    0 $F9 $C action
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
    9 1 object-show
    0 self-move-16
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $59 0 56 hewie-to-room
            1 $38 -4.97 35.12 -145 char-to-xz
        then
    then
    $323 story-flag-set
    3 stalker-kind? $22 stalker-kind? or if
        0 $4A 180 char-to-tri-facing
        $FE $59 16 2 stalker-to-room
        $FE $10 23.591 11.878 -30 char-to-xz
        $FE char-activate
        $FE stalker-knock-down
        $34 resident-flag-set
        $80 door-open-clear
        $80 exit-check
    else
        0 $4A -6.968 18.812 -130 char-to-xz
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
        camera-restart
        $FE action-end
        3 summon-take
        $F $41 fade
        wait-fade
        $36 resident-flag-set
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-2f-6.act03 ( -- )   \ 004105E0
    self-wait-done
    32.0 -3.5 self-turn-to-xz
    self-wait-done
    0 ebit? not if
        3 message
        wait-message
        0 ebit-set
    else
        5 message
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-6.act04 ( -- )   \ 00410600
    self-wait-done
    -29.0 -1.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        4 message
        wait-message
        1 ebit-set
    else
        6 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-6.act05 ( -- )   \ 00410620
    self-wait-done
    4.58 14.22 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $276 story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: old-mansion-2f-6.act06 ( -- )   \ 00410680
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    self-wait-done
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 0 $14 door-bits
    3 0 char-remove
    $C0 room-preload
    8 state-flag-set
    $3A resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: old-mansion-2f-6.act07 ( -- )   \ 00410720
    self-wait-done
    0 $A char-in-area? if
        -30 self-turn-angle
        self-wait-done
        $A02 self-anim
    else
        -16.0 -17.0 self-turn-to-xz
        self-wait-done
        $A01 self-anim
    then
    self-frames-reset
    $28 self-wait-frames
    5 ebit? not if
        5 ebit-set
        9 message
        wait-message
    else
        $E message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act08 ( -- )   \ 00410750
    self-wait-done
    -70 self-turn-angle
    self-wait-done
    6 ebit? not if
        6 ebit-set
        $A message
        wait-message
    else
        $60A self-anim
        self-frames-reset
        $B self-wait-frames
        0 $28 5 char-sound
        self-wait-anim
        $B message
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        wait-message
    then
    self-idle-or-end
;

: old-mansion-2f-6.act09 ( -- )   \ 00410780
    self-wait-done
    2.0 45.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $C message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act0A ( -- )   \ 004107A0
    self-wait-done
    14.2 44.7 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    $D message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act0B ( -- )   \ 004107C0
    self-wait-done
    6.5 14.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $F message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act0C ( -- )   \ 004107D8
    0 exit-door-open? not if
        self-frames-reset
        $F self-wait-frames
        0 1 self-door-knock
    then
    self-idle-or-end
;

: old-mansion-2f-6.act0D ( -- )   \ 004107F0
    0 exit-door-open? not if
        self-frames-reset
        $F self-wait-frames
        0 1 self-door-knock
    then
    begin
        6 cutscene-shot? not while
        yield
    repeat
    0 old-mansion-2f-6.cmd00
    self-idle-or-end
;

: old-mansion-2f-6.act0E ( -- )   \ 00410810
    self-wait-done
    0 0 $14 door-bits
    3 partner-load
    2 char-unload
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
    3 4 0 char-model-op
    0 $F9 $C action
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
    9 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-2f-6.act0F ( -- )   \ 004108B0
    self-wait-done
    0 0 $14 door-bits
    3 partner-load
    2 char-unload
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
    3 4 0 char-model-op
    $10 $FF movie-param
    0 $F9 $D action
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
    1 old-mansion-2f-6.cmd00
    8 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-2f-6.act10 ( -- )   \ 00410950
    self-wait-done
    0 0 $14 door-bits
    4 partner-load
    2 char-unload
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
    $10 $FF movie-param
    3 5 0 char-model-op
    0 $F9 $C action
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
    9 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-2f-6.act11 ( -- )   \ 004109F0
    self-wait-done
    0 0 $14 door-bits
    4 partner-load
    2 char-unload
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
    3 5 0 char-model-op
    $10 $FF movie-param
    0 $F9 $C action
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
    8 1 object-show
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-2f-6.act12 ( -- )   \ 00410A90
    self-wait-done
    0 0 $14 door-bits
    $17 3 $FF char-load
    3 char-unload
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    0 0 $14 door-bits
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-6.enter old-mansion-2f-6 0 room-script!
' old-mansion-2f-6.char-enter old-mansion-2f-6 6 room-script!
' old-mansion-2f-6.phase1 old-mansion-2f-6 1 room-script!
' old-mansion-2f-6.phase2 old-mansion-2f-6 2 room-script!
' old-mansion-2f-6.act00 old-mansion-2f-6 $00 action-script!
' old-mansion-2f-6.act01 old-mansion-2f-6 $01 action-script!
' old-mansion-2f-6.act02 old-mansion-2f-6 $02 action-script!
' old-mansion-2f-6.act03 old-mansion-2f-6 $03 action-script!
' old-mansion-2f-6.act04 old-mansion-2f-6 $04 action-script!
' old-mansion-2f-6.act05 old-mansion-2f-6 $05 action-script!
' old-mansion-2f-6.act06 old-mansion-2f-6 $06 action-script!
' old-mansion-2f-6.act07 old-mansion-2f-6 $07 action-script!
' old-mansion-2f-6.act08 old-mansion-2f-6 $08 action-script!
' old-mansion-2f-6.act09 old-mansion-2f-6 $09 action-script!
' old-mansion-2f-6.act0A old-mansion-2f-6 $0A action-script!
' old-mansion-2f-6.act0B old-mansion-2f-6 $0B action-script!
' old-mansion-2f-6.act0C old-mansion-2f-6 $0C action-script!
' old-mansion-2f-6.act0D old-mansion-2f-6 $0D action-script!
' old-mansion-2f-6.act0E old-mansion-2f-6 $0E action-script!
' old-mansion-2f-6.act0F old-mansion-2f-6 $0F action-script!
' old-mansion-2f-6.act10 old-mansion-2f-6 $10 action-script!
' old-mansion-2f-6.act11 old-mansion-2f-6 $11 action-script!
' old-mansion-2f-6.act12 old-mansion-2f-6 $12 action-script!
