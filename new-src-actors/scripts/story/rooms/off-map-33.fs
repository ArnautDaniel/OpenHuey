\ story/rooms/off-map-33.fs - the event scripts of room off-map-33 ($33; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-33
USING: room-names story.words story.shared flag-names ;

: off-map-33.char-enter ( -- )   \ 00442DA0
    0 self-is? if
        $88 story-flag? if
            $80 exit-taken? if
                0 0 0 char-to-tri-facing
                world-held state-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 0 action-force
            then
        then
    then
;

: off-map-33.act00 ( -- )   \ 00442DD0
    4 resident-flag? not if
        no-pause state-flag-set
    then
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    self-wait-done
    0 1 char-visible
    $17 1 char-no-shadow
    4 char-unload
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
    0 $F9 1 action
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
    4 0 char-remove
    $2D 3 pvar-set
    world-held state-flag-set
    2 game-over-flag
    caught state-flag-set
    $17 0 char-no-shadow
    self-idle-or-end
;

: off-map-33.act01 ( -- )   \ 00442E80
    self-frames-reset
    1 self-wait-frames
    begin
        4 cutscene-shot? if
            0 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: off-map-33.act02 ( -- )   \ 00442EA0
    self-wait-done
    0 1 $14 door-bits
    $17 3 $FF char-load
    3 char-unload
    0 1 char-visible
    $17 1 char-no-shadow
    4 char-unload
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
    0 $F9 1 action
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
    $37 room-preload
    3 0 char-remove
    4 0 char-remove
    $17 0 char-no-shadow
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: off-map-33.enter ( -- )   \ 0047AF70
    0 1 $14 door-bits
;

: off-map-33.phase1 ( -- )   \ 0047AF78
;

\ ---- registered ----
' off-map-33.char-enter off-map-33 6 room-script!
' off-map-33.act00 off-map-33 $00 action-script!
' off-map-33.act01 off-map-33 $01 action-script!
' off-map-33.act02 off-map-33 $02 action-script!
' off-map-33.enter off-map-33 0 room-script!
' off-map-33.phase1 off-map-33 1 room-script!
