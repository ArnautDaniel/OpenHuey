\ story/rooms/off-map-36.fs - the event scripts of room off-map-36 ($36; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-36
USING: room-names story.words story.shared flag-names ;

: off-map-36.char-enter ( -- )   \ 00443E60
    0 self-is? if
        $80 exit-taken? if
            0 0 0 action
        then
    then
;

: off-map-36.enter ( -- )   \ 0047AFC0
;

: off-map-36.act00 ( -- )   \ 00443E70
    1 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    1 action-end
    1 char-done
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
    3 0 char-remove
    $2D 0 pvar-set
    2 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: off-map-36.act01 ( -- )   \ 00443F10
    self-wait-done
    $FF 1 char-visible
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
    $37 room-preload
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
    3 0 char-remove
    $FF 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' off-map-36.char-enter off-map-36 6 room-script!
' off-map-36.enter off-map-36 0 room-script!
' off-map-36.act00 off-map-36 $00 action-script!
' off-map-36.act01 off-map-36 $01 action-script!
