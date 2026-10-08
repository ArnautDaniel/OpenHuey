\ story/rooms/castle-1f-18.fs - the event scripts of room castle-1f-18 ($1C; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-18
USING: room-names story.words story.shared flag-names ;

\ room 0x1C (Room1C_Cmd00_ptmf): room effect 0x1C (a depth range) with the cutscene from frame
\ 0x14A: near 1 .. 1 + 1.4 t (at most 67.6), far 48.6 + 4 t (at most 230)
: castle-1f-18.cmd00 ( -- )  s" castle-1f-18.cmd00" stub-step ;
\ room 0x1C (D_003FD350): a sound (0xC0000000, bank 6) at the room's effect 1
: castle-1f-18.cmd01 ( -- )  s" castle-1f-18.cmd01" stub-step ;

: castle-1f-18.act06 ( -- )   \ 003FD070
    0 $FF $E2 0 $D $FF $D8 1 butterflies
    1 $FF $E2 0 $A $FF $D8 0 butterflies
    2 $FF $E1 0 $B $FF $D9 0 butterflies
    3 $FF $E2 0 $C $FF $D7 0 butterflies
    4 $FF $E3 0 $E $FF $DA 0 butterflies
    5 $FF $E3 0 $D $FF $D9 0 butterflies
    6 $FF $E4 0 $C $FF $DA 0 butterflies
    7 $FF $E3 0 $D $FF $DB 0 butterflies
    8 $FF $E5 0 $E $FF $D8 0 butterflies
    9 $FF $E3 0 $F $FF $D8 0 butterflies
    $A $FF $E5 0 $C $FF $D7 0 butterflies
    exit
;

: castle-1f-18.enter ( -- )   \ 003FCB30
    room-sounds
    $26 door-locked? if
        0 0 $33 0 $14 $FF $E3 1 butterflies
        1 0 $33 0 $16 $FF $E2 0 butterflies
        2 0 $33 0 $12 $FF $E2 0 butterflies
        3 0 $33 0 $E $FF $E2 0 butterflies
        4 0 $33 0 $C $FF $E2 0 butterflies
        5 0 $33 0 8 $FF $E2 0 butterflies
        6 0 $33 0 $10 $FF $E6 0 butterflies
        7 0 $33 0 $18 $FF $E2 0 butterflies
        8 0 $32 0 $14 $FF $E2 0 butterflies
        9 0 $30 0 $10 $FF $E2 0 butterflies
        $A 0 $30 0 $C $FF $E6 0 butterflies
        1 0 $20000000 nav-group
        1 1 $1000000 nav-group
    else
        0 1 $14 door-bits
        castle-1f-18.act06
        1 1 $20000000 nav-group
    then
;

: castle-1f-18.char-enter ( -- )   \ 003FCBC0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    0 1 0 area-camera
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

: castle-1f-18.phase1 ( -- )   \ 003FCC40
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 1 0 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    7 story-flag? not if
        0 55.0 4.0 -30.0 $A 5 0 zone
        1 55.0 4.0 -30.0 $14 50 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
    else
        0 -34.0 0.0 -44.0 $A 5 0 zone
        1 -34.0 0.0 -44.0 $14 50 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        1 ebit-set
                        $1E chance? if
                            $1F 1 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
    then
    2 ebit? not if
        6 sound-bank-loaded? if
            7 story-flag? not if
                $40000000 6 55.0 4.0 -30.0 0 0 sound
            else
                $40000000 6 -34.0 0.0 -44.0 0 0 sound
            then
            2 ebit-set
        then
    else 7 story-flag? not if
        castle-1f-18.cmd01
    else
        $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    then then
;

: castle-1f-18.phase2 ( -- )   \ 003FCD70
    7 story-flag? not if
        0 9 $5A char-faces-area? if
            5 2 0 scene-change
        then
    then
    0 0 char-action? $FF panic-stage? not and if
        0 0 2 char-zone-bits? 0 0 2 char-zone-bits-before? not and if
            0 0 1 action
        then
    then
    0 7 char-in-area? 0 90 $32 char-heading? and if
        5 $85 3 scene-change
    then
    0 8 char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
;

: castle-1f-18.act00 ( -- )   \ 003FCDC0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
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
    0 1 $14 door-bits
    0 $F9 3 action
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
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
    0 self-move-16
    0 $10B -33.329 -29.383 -177 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    1 item-use
    2 item-use
    $26 door-unlock
    7 story-flag-set
    castle-1f-18.act06
    0 0 $20000000 nav-group
    0 1 $1000000 nav-group
    1 1 $20000000 nav-group
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    100 hewie-trust
    self-idle-or-end
;

: castle-1f-18.act01 ( -- )   \ 003FCEA0
    self-wait-done
    7 story-flag? not if
        55.0 -30.0 self-turn-to-xz
        self-wait-done
    else
        -34.0 -44.0 self-turn-to-xz
        self-wait-done
    then
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    7 story-flag? not if
        1 message
        wait-message
        $229 item-give
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-18.act02 ( -- )   \ 003FCEE0
    self-wait-done
    $EC -32.523 -39.444 -130 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 10.0 0.0 0.0 event-camera
    $A01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-18.act03 ( -- )   \ 003FCF30
    0 0 $A 0 $14 $FF $E3 1 butterflies
    0 $FF $DC 0 $B $FF $D1 0 7 effect-string
    1 0 $34 0 $15 $FF $E0 0 butterflies
    2 0 $33 0 $12 $FF $E1 0 butterflies
    3 0 $34 0 $E $FF $E0 0 butterflies
    4 0 $35 0 $17 $FF $E4 0 butterflies
    5 0 $36 0 $13 $FF $E2 0 butterflies
    6 0 $35 0 $F $FF $E1 0 butterflies
    7 0 $36 0 $10 $FF $E0 0 butterflies
    8 0 $33 0 $D $FF $E2 0 butterflies
    9 0 $34 0 $12 $FF $E1 0 butterflies
    $A 0 $32 0 $D $FF $E4 0 butterflies
    1 $FF $E2 0 $A $FF $D8 0 7 effect-string
    2 $FF $E1 0 $B $FF $D9 0 7 effect-string
    3 $FF $E2 0 $C $FF $D7 0 7 effect-string
    4 $FF $E3 0 $E $FF $DA 0 7 effect-string
    5 $FF $E3 0 $D $FF $D9 0 7 effect-string
    6 $FF $E4 0 $C $FF $DA 0 7 effect-string
    7 $FF $E3 0 $D $FF $DB 0 7 effect-string
    8 $FF $E5 0 $E $FF $D8 0 7 effect-string
    9 $FF $E3 0 $F $FF $D8 0 7 effect-string
    $A $FF $E5 0 $C $FF $D7 0 7 effect-string
    begin
        $D2 cutscene-cue-reached? not while
        yield
    repeat
    $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    begin
        $14A cutscene-cue-reached? not while
        1.0 1.0 1.0 48.6 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        castle-1f-18.cmd00
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: castle-1f-18.act04 ( -- )   \ 0047AA70
    self-idle-or-end
;

: castle-1f-18.act05 ( -- )   \ 003FD050
    self-wait-done
    0 ebit? not if
        2 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: castle-1f-18.act07 ( -- )   \ 003FD0E0
    room-sounds
    self-wait-done
    0 0 $33 0 $14 $FF $E3 1 butterflies
    1 0 $33 0 $16 $FF $E2 0 butterflies
    2 0 $33 0 $12 $FF $E2 0 butterflies
    3 0 $33 0 $E $FF $E2 0 butterflies
    4 0 $33 0 $C $FF $E2 0 butterflies
    5 0 $33 0 8 $FF $E2 0 butterflies
    6 0 $33 0 $10 $FF $E6 0 butterflies
    7 0 $33 0 $18 $FF $E2 0 butterflies
    8 0 $32 0 $14 $FF $E2 0 butterflies
    9 0 $30 0 $10 $FF $E2 0 butterflies
    $A 0 $30 0 $C $FF $E6 0 butterflies
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
    0 1 $14 door-bits
    0 $F9 8 action
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $40000000 6 55.0 4.0 -30.0 0 0 sound
    castle-1f-18.cmd01
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

: castle-1f-18.act08 ( -- )   \ 003FD1F0
    0 0 $A 0 $14 $FF $E3 1 butterflies
    0 $FF $DC 0 $B $FF $D1 0 7 effect-string
    1 0 $34 0 $15 $FF $E0 0 butterflies
    2 0 $33 0 $12 $FF $E1 0 butterflies
    3 0 $34 0 $E $FF $E0 0 butterflies
    4 0 $35 0 $17 $FF $E4 0 butterflies
    5 0 $36 0 $13 $FF $E2 0 butterflies
    6 0 $35 0 $F $FF $E1 0 butterflies
    7 0 $36 0 $10 $FF $E0 0 butterflies
    8 0 $33 0 $D $FF $E2 0 butterflies
    9 0 $34 0 $12 $FF $E1 0 butterflies
    $A 0 $32 0 $D $FF $E4 0 butterflies
    1 $FF $E2 0 $A $FF $D8 0 7 effect-string
    2 $FF $E1 0 $B $FF $D9 0 7 effect-string
    3 $FF $E2 0 $C $FF $D7 0 7 effect-string
    4 $FF $E3 0 $E $FF $DA 0 7 effect-string
    5 $FF $E3 0 $D $FF $D9 0 7 effect-string
    6 $FF $E4 0 $C $FF $DA 0 7 effect-string
    7 $FF $E3 0 $D $FF $DB 0 7 effect-string
    8 $FF $E5 0 $E $FF $D8 0 7 effect-string
    9 $FF $E3 0 $F $FF $D8 0 7 effect-string
    $A $FF $E5 0 $C $FF $D7 0 7 effect-string
    begin
        $D2 cutscene-cue-reached? not while
        castle-1f-18.cmd01
        yield
    repeat
    $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    yield
    begin
        $14A cutscene-cue-reached? not while
        castle-1f-18.cmd01
        1.0 1.0 1.0 48.6 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        castle-1f-18.cmd01
        castle-1f-18.cmd00
        yield
    repeat
    wait-fade
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-18.enter castle-1f-18 0 room-script!
' castle-1f-18.char-enter castle-1f-18 6 room-script!
' castle-1f-18.phase1 castle-1f-18 1 room-script!
' castle-1f-18.phase2 castle-1f-18 2 room-script!
' castle-1f-18.act00 castle-1f-18 $00 action-script!
' castle-1f-18.act01 castle-1f-18 $01 action-script!
' castle-1f-18.act02 castle-1f-18 $02 action-script!
' castle-1f-18.act03 castle-1f-18 $03 action-script!
' castle-1f-18.act04 castle-1f-18 $04 action-script!
' castle-1f-18.act05 castle-1f-18 $05 action-script!
' castle-1f-18.act06 castle-1f-18 $06 action-script!
' castle-1f-18.act07 castle-1f-18 $07 action-script!
' castle-1f-18.act08 castle-1f-18 $08 action-script!
