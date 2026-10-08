\ story/rooms/castle-2f-10.fs - the event scripts of room castle-2f-10 ($17; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-10
USING: room-names story.words story.shared ;

: castle-2f-10.enter ( -- )   \ 00414390
    room-sounds
    0 0 0 obstacle-place-saved
    1 1 0 obstacle-place-saved
    2 2 0 obstacle-place-saved
    3 3 0 obstacle-place-saved
    1 $2300 sound-volume
;

: castle-2f-10.char-enter ( -- )   \ 004143B0
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
;

: castle-2f-10.phase1 ( -- )   \ 00414430
    0 1 char-in-area? 0 char-busy? not and if
        0 obstacle-save
        1 obstacle-save
        2 obstacle-save
        3 obstacle-save
        0 obstacle-keep-spot
        1 obstacle-keep-spot
        2 obstacle-keep-spot
        3 obstacle-keep-spot
        0 exit-check
    then
    0 2 char-in-area? 0 char-busy? not and if
        0 obstacle-save
        1 obstacle-save
        2 obstacle-save
        3 obstacle-save
        0 obstacle-keep-spot
        1 obstacle-keep-spot
        2 obstacle-keep-spot
        3 obstacle-keep-spot
        1 exit-check
    then
    5 0 0 1 chars-area-camera
    6 1 1 1 chars-area-camera
    0 0 var-set
    0 $455 obstacle-on? 0 $459 obstacle-on? or if
        0 obstacle-stop
        0 var-inc
    then
    1 $455 obstacle-on? 1 $459 obstacle-on? or if
        1 obstacle-stop
        0 var-inc
    then
    2 $473 obstacle-on? if
        2 obstacle-stop
        0 var-inc
    then
    3 $4B8 obstacle-on? if
        3 obstacle-stop
        0 var-inc
    then
    0 4 var? if
        0 0 0 action
    then
;

: castle-2f-10.phase2 ( -- )   \ 0047AC10
;

: castle-2f-10.act00 ( -- )   \ 004144C0
    $18 state-flag-set
    1 self-scripted
    begin
        0 char-busy? not while
        yield
    repeat
    $1202 0 self-anim-blend
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 self-anim
    self-frames-reset
    $3C self-wait-frames
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
    things-clear
    -1 self-move-16
    5 room-preload
    $28 $B4 movie-param
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
    $1C door-reopen-unlock
    $1D door-reopen-unlock
    $38 door-close-off-lock
    $39 door-close-off-lock
    exits-rebuild
    $FE 5 0 room-doors-state
    $FE 6 0 room-doors-state
    0 camera-mode? if
        $80 exit-check
    else
        $81 exit-check
    then
    self-idle-or-end
;

: castle-2f-10.act01 ( -- )   \ 004145A0
    self-wait-done
    0 0 0 $455 $CA5 obstacle-place
    1 1 0 $459 $CCD obstacle-place
    2 2 0 $473 $CE7 obstacle-place
    3 3 0 $4B8 $D2C obstacle-place
    $FF 1 char-visible
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
    $28 $B4 movie-param
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
    $FF 1 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-2f-10.enter castle-2f-10 0 room-script!
' castle-2f-10.char-enter castle-2f-10 6 room-script!
' castle-2f-10.phase1 castle-2f-10 1 room-script!
' castle-2f-10.phase2 castle-2f-10 2 room-script!
' castle-2f-10.act00 castle-2f-10 $00 action-script!
' castle-2f-10.act01 castle-2f-10 $01 action-script!
