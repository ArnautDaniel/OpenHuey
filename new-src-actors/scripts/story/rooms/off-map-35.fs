\ story/rooms/off-map-35.fs - the event scripts of room off-map-35 ($35; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-35
USING: room-names story.words story.shared flag-names ;

: off-map-35.char-enter ( -- )   \ 00443080
    0 self-is? if
        $80 exit-taken? if
            0 0 0 action
        then
        $81 exit-taken? if
            $34 story-flag? if
                0 0 1 action
            else
                0 0 2 action
            then
        then
    then
;

: off-map-35.act00 ( -- )   \ 004430A0
    3 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    stalkers-stay state-flag-set
    1 char-activate
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
    0 1 char-no-shadow
    1 1 char-no-shadow
    $1F $44 $4E $6C $64 $66 $6C $60 $46 0 9 effect-string
    $B090C8BA 0 1 screen-blend
    $40424C74 $4244489C 8.54 25.61 1 fog
    wait-fade
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    $E room-preload
    0 0 char-no-shadow
    1 0 char-no-shadow
    $80 exit-check
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-35.act01 ( -- )   \ 00443170
    1 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    2 3 $FF char-load
    3 char-unload
    1 char-activate
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
    wait-fade
    0 $F9 3 action
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    $36 room-preload
    0 0 char-no-shadow
    $80 exit-check
    self-idle-or-end
;

: off-map-35.act02 ( -- )   \ 00443210
    2 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    1 char-activate
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
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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
    $2D 1 pvar-set
    2 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: off-map-35.act03 ( -- )   \ 004432A0
    begin
        $11 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        $12 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: off-map-35.act04 ( -- )   \ 004432C0
    self-wait-done
    2 3 $FF char-load
    3 char-unload
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
    wait-fade
    0 $F9 3 action
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
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
    $36 room-preload
    0 0 char-no-shadow
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: off-map-35.act05 ( -- )   \ 00443350
    self-wait-done
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
    $FF panic-stage? if
        3 panic-stage
    then
    1 message-prepare
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

: off-map-35.act06 ( -- )   \ 004433E0
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
    0 1 char-no-shadow
    1 1 char-no-shadow
    $1F $44 $4E $6C $64 $66 $6C $60 $46 0 9 effect-string
    $B090C8BA 0 1 screen-blend
    $40424C74 $4244489C 8.54 25.61 1 fog
    wait-fade
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    $E room-preload
    0 0 char-no-shadow
    1 0 char-no-shadow
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: off-map-35.enter ( -- )   \ 0047AF98
;

\ ---- registered ----
' off-map-35.char-enter off-map-35 6 room-script!
' off-map-35.act00 off-map-35 $00 action-script!
' off-map-35.act01 off-map-35 $01 action-script!
' off-map-35.act02 off-map-35 $02 action-script!
' off-map-35.act03 off-map-35 $03 action-script!
' off-map-35.act04 off-map-35 $04 action-script!
' off-map-35.act05 off-map-35 $05 action-script!
' off-map-35.act06 off-map-35 $06 action-script!
' off-map-35.enter off-map-35 0 room-script!
