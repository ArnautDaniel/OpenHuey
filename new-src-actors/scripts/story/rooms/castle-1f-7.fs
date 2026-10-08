\ story/rooms/castle-1f-7.fs - the event scripts of room castle-1f-7 ($15; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-7
USING: room-names story.words story.shared ;

\ a lid (pstr_kousi_2, +0x24 its height, +0x34 its speed): byte 3 0 up, 1 shut; 2 falling and
\ bouncing shut (the first landing clears progress flag 0x50 and, unless the director says no,
\ thuds), 2 while moving; 3 a random rattle up, 2 while it stays below
: castle-1f-7.cmd00 ( b0 -- )  drop s" castle-1f-7.cmd00" stub-step ;
\ room 0x15 (Room15_Cond00_ptmf): Hewie is about in room 0xF in state 0x2F or 0x52
: castle-1f-7.cond00? ( -- flag )  s" castle-1f-7.cond00?" stub-flag ;

: castle-1f-7.enter ( -- )   \ 003FA090
    room-sounds
    0 0 var-set
    $80 exit-taken? if
        1 castle-1f-7.cmd00
        $13 story-flag-clear
    else
        $FE char-here? $FE 0 char-in-nav-group? and if
            $FE $F0 char-to-tri
        then
        1 exit-taken? if
        else
            $310 story-flag-clear
            $13 story-flag-clear
        then
        $310 story-flag? not $13 story-flag? not and if
            1 0 $20000 nav-group
            1 castle-1f-7.cmd00
        else
            $13 story-flag-set
            0 0 $20000 nav-group
            0 1 var-set
            0 castle-1f-7.cmd00
        then
    then
    $14 story-flag? not if
        2 0 object-show
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 1 $1000000 nav-group
    else
        2 1 object-show
        0 1 $14 door-bits
        1 0 $14 door-bits
    then
;

: castle-1f-7.char-enter ( -- )   \ 003FA110
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
    0 self-is? if
        $80 exit-taken? if
            0 $C7 -60 char-to-tri-facing
            1 30.0 15.0 0.0 0.0 event-camera
            0 $F0 7 action
        then
    then
    1 self-is? if
        $14 story-flag? not if
            $316 story-flag? not if
                0 1 8 action
            then
        then
    then
;

: castle-1f-7.phase1 ( -- )   \ 003FA1C0
    0 ebit? not if
        6 sound-bank-loaded? if
            $14 story-flag? not if
                $40000003 6 -10.0 15.0 0.0 0 0 sound
            then
            0 ebit-set
        then
    else
        $C0000003 6 -10.0 15.0 0.0 0 0 sound
        $C0000001 6 0.0 30.0 30.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 0 0 1 chars-area-camera
    0 2 char-left-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $13 story-flag? not if
        0 0 var-set
    else castle-1f-7.cond00? not if
        0 0 var-set
    then then
    1 $15 0 room-doors-state
    $13 story-flag? if
        0 0 var? if
            0 0 char-in-nav-group? 1 0 char-in-nav-group? or $FE 0 char-in-nav-group? or if
                1 $15 1 room-doors-state
            else
                $13 story-flag-clear
                1 0 $20000 nav-group
                0 $F1 3 action
            then
        then
    then
    $14 story-flag? not if
        0 $11 char-entered-area? 1 ebit? not and if
            1 ebit-set
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
;

: castle-1f-7.phase2 ( -- )   \ 003FA2C0
    0 8 char-in-area? 0 -45 $2D char-heading? and if
        5 1 0 scene-change
    then
    $14 story-flag? not $13 story-flag? not and if
        0 9 char-in-area? 0 -45 $2D char-heading? and if
            5 4 0 scene-change
        then
    then
    0 $A char-in-area? 0 -45 $2D char-heading? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 90 $2D char-heading? and if
        5 5 0 scene-change
    then
    0 $C char-in-area? 0 $D char-in-area? or 0 45 $2D char-heading? and if
        5 5 0 scene-change
    then
    0 $E char-in-area? 0 0 $2D char-heading? and if
        5 5 0 scene-change
    then
    -2147483646 scene-request? if
        5 6 1 scene-change
    then
;

: castle-1f-7.phase3 ( -- )   \ 003FA330
    33.0 23.0 -5.9 45.0 23.0 -5.9 33.0 11.0 -5.9 45.0 11.0 -5.9 lights-doorway
    33.0 11.0 -5.9 45.0 11.0 -5.9 33.0 0.0 -5.9 45.0 0.0 -5.9 lights-doorway
    45.0 23.0 -5.1 33.0 23.0 -5.1 45.0 0.0 -5.1 33.0 0.0 -5.1 lights-doorway
;

: castle-1f-7.act00 ( -- )   \ 003FA3D0
    $2C state-flag-set
    $19 state-flag-set
    1 self-scripted
    $FE action-end
    $FE char-done
    self-frames-reset
    self-wait-16
    yield
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
    $12 $1E movie-param
    1 action-end
    1 char-done
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
    3 6 sound-stop
    1.0 sound-volume-scale
    $1C resident-flag-set
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: castle-1f-7.act01 ( -- )   \ 003FA450
    self-wait-done
    $14 story-flag? not if
        0 3 char-file-load
        $BF -39.071 35.745 -90 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 -10.0 0.0 0.0 event-camera
        0 message
        wait-message
        0 char-file-use
        0 answer? if
            2 0 object-anim
            $8000 $A self-anim-blend
            self-frames-reset
            $32 self-wait-frames
            0 6 -44.0 18.0 36.0 0 0 sound
            3 6 sound-stop
            1 0 $14 door-bits
            $14 story-flag-set
            40 hewie-trust
            $FE $15 0 room-doors-state
            0 1 $1000000 nav-group
            9 door-unlock
            self-wait-anim
            $8277 item-give
        then
        self-frames-reset
        self-wait-16
        0 0.0 0.0 0.0 0.0 event-camera
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-7.act02 ( -- )   \ 0047AA28
    self-idle-or-end
;

: castle-1f-7.act03 ( -- )   \ 003FA4F0
    $310 story-flag-set
    $40000001 6 0.0 30.0 30.0 0 0 sound
    2 castle-1f-7.cmd00
    self-idle-or-end
;

: castle-1f-7.act04 ( -- )   \ 0047AA30
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: castle-1f-7.act05 ( -- )   \ 003FA510
    self-wait-done
    7 0 var? if
        2 message
        wait-message
        7 1 var-set
    else 7 1 var? if
        3 message
        wait-message
        7 2 var-set
    else
        4 message
        wait-message
        7 0 var-set
    then then
    self-idle-or-end
;

: castle-1f-7.act06 ( -- )   \ 003FA550
    self-wait-done
    9 self-through-door
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
        6 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-7.act07 ( -- )   \ 003FA580
    $17 state-flag-set
    8 state-flag-clear
    $F 1 fade
    wait-fade
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $F room-preload
    $40000001 6 0.0 30.0 30.0 0 0 sound
    3 castle-1f-7.cmd00
    2 6 0.0 30.0 30.0 0 0 sound
    self-frames-reset
    $1E self-wait-frames
    $F 0 fade
    wait-fade
    8 state-flag-set
    0 0.0 0.0 0.0 0.0 event-camera
    3 6 sound-stop
    $80 exit-check
    self-idle-or-end
;

: castle-1f-7.act08 ( -- )   \ 003FA5F0
    self-wait-done
    $F5 -9.214 -23.873 -40 $FFFF $A self-move-to
    self-wait-done
    $14 story-flag? not if
        4 self-anim
        self-frames-reset
        $3C self-wait-frames
        $316 story-flag-set
        $14 story-flag? not if
            $1B03 self-anim
            self-wait-anim
            $14 story-flag? not if
                $1B03 self-anim
                self-wait-anim
                $14 story-flag? not if
                    $1B03 self-anim
                    self-wait-anim
                    $14 story-flag? not if
                        $1B03 self-anim
                        self-wait-anim
                        $14 story-flag? not if
                            $1B03 self-anim
                            self-wait-anim
                            $14 story-flag? not if
                                $1B03 self-anim
                                self-wait-anim
                                $14 story-flag? not if
                                    0 self-look-at
                                    yield
                                    4 self-anim
                                    self-frames-reset
                                    $28 self-wait-frames
                                    $FF self-look-at
                                    yield
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    self-idle-or-end
;

: castle-1f-7.act09 ( -- )   \ 003FA660
    self-wait-done
    1 castle-1f-7.cmd00
    2 0 object-show
    0 0 $14 door-bits
    1 1 $14 door-bits
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
    $12 $1E movie-param
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

\ ---- registered ----
' castle-1f-7.enter castle-1f-7 0 room-script!
' castle-1f-7.char-enter castle-1f-7 6 room-script!
' castle-1f-7.phase1 castle-1f-7 1 room-script!
' castle-1f-7.phase2 castle-1f-7 2 room-script!
' castle-1f-7.phase3 castle-1f-7 3 room-script!
' castle-1f-7.act00 castle-1f-7 $00 action-script!
' castle-1f-7.act01 castle-1f-7 $01 action-script!
' castle-1f-7.act02 castle-1f-7 $02 action-script!
' castle-1f-7.act03 castle-1f-7 $03 action-script!
' castle-1f-7.act04 castle-1f-7 $04 action-script!
' castle-1f-7.act05 castle-1f-7 $05 action-script!
' castle-1f-7.act06 castle-1f-7 $06 action-script!
' castle-1f-7.act07 castle-1f-7 $07 action-script!
' castle-1f-7.act08 castle-1f-7 $08 action-script!
' castle-1f-7.act09 castle-1f-7 $09 action-script!
