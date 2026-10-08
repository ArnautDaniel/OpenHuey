\ story/rooms/castle-b1-6.fs - the event scripts of room castle-b1-6 ($C; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-6
USING: room-names story.words story.shared ;

\ room 0x0C (Room0C_Cmd01_ptmf): script variable byte 3 down by the player's hit (byte 4: 1 from
\ the weak blow 0x1A, else 5) or the pursuer's (+0x108), not below 0
: castle-b1-6.cmd01 ( b0 b1 -- )  drop drop s" castle-b1-6.cmd01" stub-step ;
\ room 0x0C (Room0C_Cmd02_ptmf): the screen darkened as the cutscene runs past frame 0x4AE (32 a
\ frame, up to 0x80)
: castle-b1-6.cmd02 ( -- )  s" castle-b1-6.cmd02" stub-step ;
\ room 0x0C (Room0C_Cmd03_ptmf): three objects (pair byte 4) swing: byte 3 0 set up (rest +0x30,
\ phase +0x34 half a turn apart, swing +0x3C 0.75 / 0.5), 1 a step (phase on 60 degrees, the
\ swing down 0.1, x = rest + swing x sin(phase); 2 once still), 2 all to x 10. 2 while any
\ swings
: castle-b1-6.cmd03 ( b0 b1 -- )  drop drop s" castle-b1-6.cmd03" stub-step ;

: castle-b1-6.enter ( -- )   \ 003F4650
    room-sounds
    $15 1.0 0 bgm
    $23 story-flag? not if
        0 1 $14 door-bits
    then
    1 1 $14 door-bits
    6 1 object-show
    $35 story-flag? not if
        0 25 var-set
    else
        $D 1 object-show
        5 1 $14 door-bits
    then
    $36 story-flag? not if
        1 25 var-set
    else
        $E 1 object-show
        3 1 $14 door-bits
    then
    $34 story-flag? not if
        9 0 object-show
        $A 1 object-show
        $16 story-flag? if
            7 1 $14 door-bits
            1 0 8 nav-group
        then
    else
        9 1 object-show
        $A 0 object-show
        1 0 8 nav-group
    then
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    1 $2300 sound-volume
;

: castle-b1-6.char-enter ( -- )   \ 003F46E0
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
;

: castle-b1-6.phase1 ( -- )   \ 003F4720
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    3 1 0 1 chars-area-camera
    4 2 1 1 chars-area-camera
    8 0 -1 1 chars-area-camera
    9 1 0 1 chars-area-camera
    $A 0 -1 1 chars-area-camera
    $B 1 0 1 chars-area-camera
    $23 story-flag? $15 story-flag? not and $16 story-flag? not and 0 7 char-entered-area? and 2 ebit? not and if
        2 ebit-set
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 char-action? if
            0 0 0 action-force
        else
            1 0 0 action-force
        then
    then
    $35 story-flag? not if
        0 10.0 0.0 -37.0 7 20 0 zone
        $FE 0 char-in-zone? 0 0 char-in-zone? or if
            0 $F1 8 action
        then
    then
    $36 story-flag? not if
        1 10.0 0.0 37.0 7 20 0 zone
        $FE 1 char-in-zone? 0 1 char-in-zone? or if
            0 $F2 9 action
        then
    then
    $15 story-flag? $16 story-flag? not and if
        $FE 2 char-C4? if
            $28 door-unlock
            $16 story-flag-set
            $1B state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
        $35 story-flag? $36 story-flag? and if
            $28 door-unlock
            $16 story-flag-set
            $1B state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 2 action-force
            else
                1 0 2 action-force
            then
        then
    then
    2 166.49 6.0 0.6 $24 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: castle-b1-6.phase2 ( -- )   \ 003F4850
    $10 state-flag? if
        -2147483646 scene-request? if
            5 3 1 scene-change
        then
    then
    0 5 char-in-area? 0 45 $32 char-heading? and if
        5 4 0 scene-change
    then
    $15 story-flag? not $16 story-flag? or if
        0 $C char-in-area? 0 10 -40 $32 char-faces-xz? and if
            5 6 0 scene-change
        then
        0 $D char-in-area? 0 10 40 $32 char-faces-xz? and if
            5 6 0 scene-change
        then
    then
    $34 story-flag? if
        0 $E $32 char-faces-area? if
            5 5 0 scene-change
        then
    else $16 story-flag? if
        0 $E $32 char-faces-area? if
            5 $B 0 scene-change
        then
    then then
;

: castle-b1-6.act00 ( -- )   \ 003F48C0
    $18 state-flag-set
    1 self-scripted
    $1B state-flag-set
    self-wait-done
    -1 self-move-16
    $FF 1.0 0 bgm
    $F $44 fade
    2 0 char-remove
    $1B partner-load
    0 state-flag-clear
    2 char-unload
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
    1 0 $14 door-bits
    1 char-activate
    $C 0 209 hewie-to-room
    $FE $C 206 2 stalker-to-room
    $FE 0 -1 char-camera
    $FE char-activate
    $FE 1 char-no-shadow
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
    8 state-flag-set
    $FE 0 char-no-shadow
    6 story-flag-set
    $11 door-unlock
    1 1 $14 door-bits
    6 1 object-show
    $28 door-open-clear
    $28 door-lock
    doors-room-in
    $10 state-flag-set
    0 3 -90 char-to-tri-facing
    1 char-activate
    $C 0 209 hewie-to-room
    1 $D1 -90 char-to-tri-facing
    $FE $C 206 2 stalker-to-room
    $FE char-activate
    $FE 2 stalker-mode
    $FE 0 stalker-search-delay
    $FE $CE 90 char-to-tri-facing
    $FE char-full-health
    $15 story-flag-set
    hewie-controlled? not if
        0 2 1 char-camera
        0 camera-follow
    else
        1 2 1 char-camera
        1 camera-follow
    then
    camera-restart
    8 state-flag-clear
    $1B state-flag-clear
    1 1.0 0 bgm
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-6.act01 ( -- )   \ 003F4A00
    $FF 1.0 0 bgm
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F 4 fade
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
    wait-fade
    1 action-end
    1 char-done
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
    $10 state-flag-clear
    2 0 char-remove
    1 char-activate
    50 hewie-trust
    0 $11C 56.239 -3.659 -45 char-to-xz
    $C 0 78 hewie-to-room
    1 $4E 57.799 0.951 0 char-to-xz
    7 1 $14 door-bits
    1 0 8 nav-group
    hewie-controlled? not if
        0 1 0 char-camera
        0 camera-follow
    else
        1 1 0 char-camera
        1 camera-follow
    then
    camera-restart
    $28 door-open-clear
    $28 door-unlock
    doors-room-in
    $15 1.0 0 bgm
    $1B state-flag-clear
    $F $41 fade
    wait-fade
    $28 resident-flag-set
    $22F item-give
    $8297 item-give
    shared.act9C
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-6.act02 ( -- )   \ 003F4B10
    $FF 1.0 0 bgm
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $13 state-flag-set
    $E state-flag-set
    $F 4 fade
    8 0 movie-play
    7 cutscene-start
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
    0 $F9 7 action
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
    $26 resident-flag-set
    8 state-flag-set
    9 1 object-show
    $F 0 object-show
    $10 0 object-show
    $11 0 object-show
    $12 0 object-show
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    self-frames-reset
    self-wait-16
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
    0 $F9 $A action
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    6 0 $14 door-bits
    $10 state-flag-clear
    $13 state-flag-clear
    $E state-flag-clear
    50 hewie-trust
    2 0 char-remove
    1 char-activate
    0 $11C 56.239 -3.659 -45 char-to-xz
    $C 0 78 hewie-to-room
    1 $4E 57.799 0.951 0 char-to-xz
    hewie-controlled? not if
        0 1 0 char-camera
        0 camera-follow
    else
        1 1 0 char-camera
        1 camera-follow
    then
    camera-restart
    $28 door-open-clear
    $28 door-unlock
    doors-room-in
    $A 0 object-show
    1 0 8 nav-group
    $34 story-flag-set
    0 panic-stage
    $15 1.0 0 bgm
    $1B state-flag-clear
    $F $41 fade
    wait-fade
    $20B item-give
    $8296 item-give
    shared.act9C
    $27 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-6.act03 ( -- )   \ 0047A9CC
    $B message
    self-idle-or-end
;

: castle-b1-6.act04 ( -- )   \ 003F4CD0
    $18 state-flag-set
    self-wait-done
    160.0 0.0 self-turn-to-xz
    self-wait-done
    $A01 $A self-anim-blend
    self-wait-anim
    6 message
    wait-message
    $F 6 fade
    wait-fade
    7 message
    wait-message
    $F 7 fade
    wait-fade
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $23 story-flag? not if
        8 message
        wait-message
        self-wait-anim
        1 self-scripted
        0 answer? if
            $FF 1.0 0 bgm
            $F $44 fade
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
            $16 state-flag-clear
            1 creatures-clear
            $24 story-flag-set
            $25 story-flag-set
            $2C story-flag-set
            $2D story-flag-set
            $2E story-flag-set
            $2F story-flag-set
            $38 story-flag-set
            $28 $C8 movie-param
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
            0 self-move-16
            0 $CD -90 char-to-tri-facing
            camera-restart
            1 char-activate
            $C 0 401 hewie-to-room
            1 $191 0 char-to-tri-facing
            1 char-full-health
            1 0 char-set-C4
            $28 door-open-set
            doors-room-in
            0 0 $14 door-bits
            $23 story-flag-set
            $15 1.0 0 bgm
            $F $41 fade
            wait-fade
            6 message-param-room
            6 1 item-give-count
            0 6 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
            30 hewie-trust
        then
        0 self-scripted
    else
        $A message
        wait-message
        self-wait-anim
    then
    $18 state-flag-clear
    self-idle-or-end
;

: castle-b1-6.act05 ( -- )   \ 003F4E30
    1 ebit? not if
        $F message
        wait-message
        1 ebit-set
    else
        $A01 $A self-anim-blend
        $10 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: castle-b1-6.act06 ( -- )   \ 003F4E50
    self-wait-done
    $34 story-flag? if
        $E message
        wait-message
    else 0 ebit? not if
        $C message
        wait-message
        0 ebit-set
    else
        $D message
        wait-message
        0 ebit-clear
    then then
    self-idle-or-end
;

: castle-b1-6.act07 ( -- )   \ 003F4E70
    begin
        3 cutscene-shot? not while
        yield
    repeat
    begin
        4 cutscene-shot? not while
        46.0 46.0 116.5 116.5 depth-range
        yield
    repeat
    begin
        $4AE cutscene-cue-reached? not while
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        castle-b1-6.cmd02
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: castle-b1-6.act08 ( -- )   \ 003F4EB0
    $15 story-flag? $16 story-flag? not and if
        $FE 0 char-in-zone? if
            0 0 castle-b1-6.cmd01
        else
            0 1 castle-b1-6.cmd01
        then
    then
    0 0 var? if
        10.0 10.0 -35.0 0 -2144325584 3 -9.0 scene-effect-8C
        $D 1 object-show
        5 1 $14 door-bits
        $35 story-flag-set
        1 6 10.0 10.0 -40.0 0 0 sound
    else
        0 6 10.0 10.0 -40.0 0 0 sound
    then
    0 0 castle-b1-6.cmd03
    1 0 castle-b1-6.cmd03
    self-frames-reset
    $14 self-wait-frames
    self-idle-or-end
;

: castle-b1-6.act09 ( -- )   \ 003F4F30
    $15 story-flag? $16 story-flag? not and if
        $FE 1 char-in-zone? if
            1 0 castle-b1-6.cmd01
        else
            1 1 castle-b1-6.cmd01
        then
    then
    1 0 var? if
        10.0 10.0 35.0 0 -2144325584 3 -9.0 scene-effect-8C
        $E 1 object-show
        3 1 $14 door-bits
        $36 story-flag-set
        1 6 10.0 10.0 40.0 0 0 sound
    else
        0 6 10.0 10.0 40.0 0 0 sound
    then
    0 1 castle-b1-6.cmd03
    1 1 castle-b1-6.cmd03
    self-frames-reset
    $14 self-wait-frames
    self-idle-or-end
;

: castle-b1-6.act0A ( -- )   \ 003F4FB0
    begin
        5 cutscene-shot? if
            6 1 $14 door-bits
        then
        6 cutscene-shot? if
            6 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-b1-6.act0B ( -- )   \ 003F4FC8
    self-wait-done
    $1D02 $A self-anim-blend
    $11 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-b1-6.act0C ( -- )   \ 003F4FE0
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    6 1 object-show
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
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
    $28 $C8 movie-param
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
    0 0 $14 door-bits
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-b1-6.act0D ( -- )   \ 003F50A0
    self-wait-done
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    -1 self-move-16
    $1B partner-load
    2 char-unload
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
    1 0 $14 door-bits
    1 char-activate
    $C 0 209 hewie-to-room
    $FE $C 206 2 stalker-to-room
    $FE char-activate
    $FE 1 char-no-shadow
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
    $FE 0 char-no-shadow
    2 0 char-remove
    1 1 $14 door-bits
    6 1 object-show
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-b1-6.act0E ( -- )   \ 003F5170
    self-wait-done
    $D 1 object-show
    5 1 $14 door-bits
    $E 1 object-show
    3 1 $14 door-bits
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-unload
    8 0 movie-play
    7 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 7 action
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-b1-6.act0F ( -- )   \ 003F5230
    self-wait-done
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-unload
    9 1 object-show
    $F 0 object-show
    $10 0 object-show
    $11 0 object-show
    $12 0 object-show
    1 0 char-visible
    1 action-end
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    self-frames-reset
    self-wait-16
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
    0 $F9 $A action
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    6 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-b1-6.act10 ( -- )   \ 003F5300
    self-wait-done
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-unload
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-6.enter castle-b1-6 0 room-script!
' castle-b1-6.char-enter castle-b1-6 6 room-script!
' castle-b1-6.phase1 castle-b1-6 1 room-script!
' castle-b1-6.phase2 castle-b1-6 2 room-script!
' castle-b1-6.act00 castle-b1-6 $00 action-script!
' castle-b1-6.act01 castle-b1-6 $01 action-script!
' castle-b1-6.act02 castle-b1-6 $02 action-script!
' castle-b1-6.act03 castle-b1-6 $03 action-script!
' castle-b1-6.act04 castle-b1-6 $04 action-script!
' castle-b1-6.act05 castle-b1-6 $05 action-script!
' castle-b1-6.act06 castle-b1-6 $06 action-script!
' castle-b1-6.act07 castle-b1-6 $07 action-script!
' castle-b1-6.act08 castle-b1-6 $08 action-script!
' castle-b1-6.act09 castle-b1-6 $09 action-script!
' castle-b1-6.act0A castle-b1-6 $0A action-script!
' castle-b1-6.act0B castle-b1-6 $0B action-script!
' castle-b1-6.act0C castle-b1-6 $0C action-script!
' castle-b1-6.act0D castle-b1-6 $0D action-script!
' castle-b1-6.act0E castle-b1-6 $0E action-script!
' castle-b1-6.act0F castle-b1-6 $0F action-script!
' castle-b1-6.act10 castle-b1-6 $10 action-script!
