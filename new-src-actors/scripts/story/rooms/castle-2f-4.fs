\ story/rooms/castle-2f-4.fs - the event scripts of room castle-2f-4 ($31; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-4
USING: room-names story.words story.shared ;

\ Room31_Cmd00
: castle-2f-4.cmd00 ( -- )  s" castle-2f-4.cmd00" stub-step ;
\ Room31_Cmd01
: castle-2f-4.cmd01 ( -- )  s" castle-2f-4.cmd01" stub-step ;
\ Room31_Cmd02
: castle-2f-4.cmd02 ( b0 -- )  drop s" castle-2f-4.cmd02" stub-step ;

: castle-2f-4.enter ( -- )   \ 00429E40
    castle-2f-4.cmd00
    0 1 $14 door-bits
    1 0 1 effect-string
    1 0 $14 door-bits
    0 $F1 2 action
;

: castle-2f-4.char-enter ( -- )   \ 00429E60
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
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
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
            0 $A8 45 char-to-tri-facing
            8 state-flag-set
            0 0 0 action
        then
    then
    1 self-is? if
    then
    $FE self-is? if
    then
;

: castle-2f-4.phase1 ( -- )   \ 00429F10
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
;

: castle-2f-4.act00 ( -- )   \ 00429F30
    $300 story-flag-clear
    self-wait-done
    1 2 movie-play
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
    0 $F9 1 action
    $21 room-preload
    $29 state-flag-set
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
    8 state-flag-set
    $80 exit-check
    0 $A char-layer
    self-idle-or-end
;

: castle-2f-4.act01 ( -- )   \ 00429FC0
    begin
        4 cutscene-shot? not while
        yield
    repeat
    0 $1E char-layer
    begin
        5 cutscene-shot? not while
        yield
    repeat
    0 $A char-layer
    0 1 char-no-shadow
    begin
        6 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    begin
        9 cutscene-shot? not while
        yield
    repeat
    2 castle-2f-4.cmd02
    0 $1E char-layer
    begin
        $A cutscene-shot? not while
        yield
    repeat
    1 castle-2f-4.cmd02
    0 $A char-layer
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: castle-2f-4.act02 ( -- )   \ 0047AD60
    begin
        castle-2f-4.cmd01
        yield
    again
;

: castle-2f-4.act03 ( -- )   \ 0042A010
    self-wait-done
    0 $F1 2 action
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 1 action
    $29 state-flag-set
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-4.enter castle-2f-4 0 room-script!
' castle-2f-4.char-enter castle-2f-4 6 room-script!
' castle-2f-4.phase1 castle-2f-4 1 room-script!
' castle-2f-4.act00 castle-2f-4 $00 action-script!
' castle-2f-4.act01 castle-2f-4 $01 action-script!
' castle-2f-4.act02 castle-2f-4 $02 action-script!
' castle-2f-4.act03 castle-2f-4 $03 action-script!
