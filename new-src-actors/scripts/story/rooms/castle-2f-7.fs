\ story/rooms/castle-2f-7.fs - the event scripts of room castle-2f-7 ($23; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-7
USING: room-names story.words story.shared ;

\ room 0x23 (Room23_Cmd00_ptmf): Fiona's model +0x9A0 / +0x9A8: 0 (byte 3 1) or 0.12 / 0.2
: castle-2f-7.cmd00 ( b0 -- )  drop s" castle-2f-7.cmd00" stub-step ;
\ room 0x23 (Room23_Cmd01_ptmf): byte 3 0 a progress name, 1 wait for character 3 (2 while not),
\ else done
: castle-2f-7.cmd01 ( b0 -- )  drop s" castle-2f-7.cmd01" stub-step ;
\ room 0x23 (Room23_Cond00_ptmf): none of the six slots' PursuerGroup_Fields bits 0..3, and the
\ stalker is about but not active, in mode 2, 6 or 7
: castle-2f-7.cond00? ( -- flag )  s" castle-2f-7.cond00?" stub-flag ;

: castle-2f-7.enter ( -- )   \ 004010A0
    room-sounds
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 scene-effect-71000
    6 1 object-show
;

: castle-2f-7.char-enter ( -- )   \ 004010E0
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
    0 self-is? if
        $80 exit-taken? if
            0 0 6 action
            0 1 $14 door-bits
            1 0 $14 door-bits
        else
            0 0 $14 door-bits
            1 1 $14 door-bits
        then
    then
;

: castle-2f-7.phase1 ( -- )   \ 00401140
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    0 -35.84 0.0 32.47 $12 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 36.32 0.0 24.37 $F 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-7.phase2 ( -- )   \ 004011B0
    0 3 $2D char-faces-area? if
        $1F story-flag? not if
            5 0 0 scene-change
        else
            5 3 0 scene-change
        then
    then
    0 4 $2D char-faces-area? if
        5 1 0 scene-change
    then
    0 5 $2D char-faces-area? if
        5 2 0 scene-change
    then
;

: castle-2f-7.act00 ( -- )   \ 004011E0
    $18 state-flag-set
    1 self-scripted
    $F 4 fade
    self-wait-done
    2 0 movie-play
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
    $FF panic-stage? if
        3 panic-stage
    then
    6 message-prepare
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
    $1F story-flag-set
    0 ebit-set
    2 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    yield
    $20 story-flag? if
        3 0 movie-play
        1 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
        0 $F9 4 action
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
        6 1 object-show
    then
    0 $7A 31.61 18.93 33 char-to-xz
    0 self-move-16
    camera-restart
    $F 1 fade
    wait-fade
    $20 story-flag? if
        $3F message-param-room
        0 $3F item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        begin
            $F9 char-busy? while
            yield
        repeat
        $225 item-give
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-7.act01 ( -- )   \ 00401350
    self-wait-done
    0 0 var? if
        -36.2 33.3 self-turn-to-xz
        self-wait-done
        $A00 self-anim
        self-frames-reset
        $32 self-wait-frames
        $D message
        wait-message
        0 1 var-set
    else
        $E message
        wait-message
    then
    self-idle-or-end
;

: castle-2f-7.act02 ( -- )   \ 0047AB00
    self-wait-done
    $C message
    wait-message
    self-idle-or-end
;

: castle-2f-7.act03 ( -- )   \ 00401380
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $1B story-flag? $307 story-flag? not and castle-2f-7.cond00? and 0 exit-door-open? and if
        $11 door-open-clear
        doors-room-in
        0 0 self-door-knock
        self-frames-reset
        5 self-wait-frames
        0 $43 5 char-sound
        $F04 5 self-anim-blend
        1 $FF 8 rumble
        self-frames-reset
        $10 self-wait-frames
        $307 story-flag-set
        $5A threat-raise
        self-wait-anim
    else item-3F-under-10? if
        0 ebit? not if
            $11 message
            wait-message
            0 ebit-set
        then
        $20 story-flag? if
            $20 story-flag-clear
            2 subscreen-open
            begin
                4 state-flag? while
                yield
            repeat
            yield
            $20 story-flag? if
                $3F message-param-room
                0 $3F item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
            then
            $20 story-flag-set
        else
            2 subscreen-open
            begin
                4 state-flag? while
                yield
            repeat
            yield
            $20 story-flag? if
                $F 4 fade
                3 0 movie-play
                1 cutscene-start
                yield
                yield
                2 cutscene-control
                begin
                    3 cutscene-control
                    2 cutscene-mode? not while
                    yield
                repeat
                wait-fade
                0 $F9 4 action
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
                0 $7A 31.61 18.93 33 char-to-xz
                0 self-move-16
                camera-restart
                6 1 object-show
                $F 1 fade
                wait-fade
                $3F message-param-room
                0 $3F item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
                begin
                    $F9 char-busy? while
                    yield
                repeat
                $225 item-give
            then
        then
    else 0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $12 message
        wait-message
    then then then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-7.act04 ( -- )   \ 00401500
    begin
        $69 cutscene-cue-reached? not while
        yield
    repeat
    1 6 35.0 10.0 25.0 0 0 sound
    self-idle-or-end
;

: castle-2f-7.act05 ( -- )   \ 0047AB08
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: castle-2f-7.act06 ( -- )   \ 00401520
    self-wait-done
    0 $7A 31.61 18.93 33 char-to-xz
    0 1 char-visible
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
    0 castle-2f-7.cmd01
    0 castle-2f-7.cmd00
    0 $F9 7 action
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
    8 state-flag-set
    2 castle-2f-7.cmd01
    1 castle-2f-7.cmd00
    0 0 char-visible
    3 0 char-remove
    4 0 char-remove
    $25 resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-2f-7.act07 ( -- )   \ 004015D0
    begin
        3 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    1 castle-2f-7.cmd01
    self-idle-or-end
;

: castle-2f-7.act08 ( -- )   \ 004015E0
    room-sounds
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    self-wait-done
    6 1 object-show
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 scene-effect-71000
    2 0 movie-play
    0 cutscene-start
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
    6 message-prepare
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
    3 0 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 4 action
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

: castle-2f-7.act09 ( -- )   \ 00401720
    self-wait-done
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 scene-effect-71000
    6 1 object-show
    $FF 1 char-visible
    0 1 $14 door-bits
    1 0 $14 door-bits
    3 3 $FF char-load
    4 4 $FF char-load
    3 char-unload
    4 char-unload
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
    0 castle-2f-7.cmd01
    0 castle-2f-7.cmd00
    0 $F9 7 action
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
    8 state-flag-set
    2 castle-2f-7.cmd01
    1 castle-2f-7.cmd00
    3 0 char-remove
    4 0 char-remove
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-7.enter castle-2f-7 0 room-script!
' castle-2f-7.char-enter castle-2f-7 6 room-script!
' castle-2f-7.phase1 castle-2f-7 1 room-script!
' castle-2f-7.phase2 castle-2f-7 2 room-script!
' castle-2f-7.act00 castle-2f-7 $00 action-script!
' castle-2f-7.act01 castle-2f-7 $01 action-script!
' castle-2f-7.act02 castle-2f-7 $02 action-script!
' castle-2f-7.act03 castle-2f-7 $03 action-script!
' castle-2f-7.act04 castle-2f-7 $04 action-script!
' castle-2f-7.act05 castle-2f-7 $05 action-script!
' castle-2f-7.act06 castle-2f-7 $06 action-script!
' castle-2f-7.act07 castle-2f-7 $07 action-script!
' castle-2f-7.act08 castle-2f-7 $08 action-script!
' castle-2f-7.act09 castle-2f-7 $09 action-script!
