\ story/rooms/off-map-34.fs - the event scripts of room off-map-34 ($34; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-34
USING: room-names story.words story.shared flag-names ;

: off-map-34.char-enter ( -- )   \ 00442950
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

: off-map-34.act00 ( -- )   \ 00442980
    4 resident-flag? not if
        no-pause state-flag-set
    then
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
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
    0 $F9 1 action
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
    world-held state-flag-set
    $17 action-end
    $17 char-done
    $20 4 $FF char-load
    $33 room-preload
    $80 exit-check
    self-idle-or-end
;

: off-map-34.act01 ( -- )   \ 00442A20
    0 1 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $547 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 1 $14 door-bits
    5 0 $14 door-bits
    begin
        $569 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $587 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 0 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    begin
        $592 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 0 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    begin
        $59E cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 1 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $5AE cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        $5D4 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 0 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    begin
        $5EE cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 1 $14 door-bits
    5 0 $14 door-bits
    begin
        $610 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 1 $14 door-bits
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    begin
        cutscene-near-end? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    self-idle-or-end
;

: off-map-34.act02 ( -- )   \ 00442B80
    self-wait-done
    $17 3 $FF char-load
    3 char-unload
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
    $20 4 $FF char-load
    $33 room-preload
    3 0 char-remove
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: off-map-34.act4D ( -- )   \ 003F263F
    $F1 exit-check
    fade-finish
    0 self-is? 0 exit-taken? and if
        0 0 char-to-exit
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then
    \ (never runs in the original: an else outside any block)
    \   1 self-is? 0 exit-taken? and if
    \   1 0 char-to-exit
    \   hewie-controlled? not if
    \   0 4 -1 char-camera
    \   0 camera-follow
    \   else
    \   1 4 -1 char-camera
    \   1 camera-follow
    \   then
    \   then
    0 4 -1 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    2 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    3 3 3 area-camera
    0 self-is? if
        $80 exit-taken? $81 exit-taken? or $82 exit-taken? or if
            0 $10E 70 char-to-tri-facing
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            $81 exit-taken? if
                0 0 6 action
            else
                0 0 0 action
            then
        then
    then
    $FE self-is? if
        fiona-hidden state-flag? if
            5 ebit-set
            9 call-action   \ (this room has no script 9: the original reads past its table)
        else
            5 ebit-clear
        then
    then
;

: off-map-34.enter ( -- )   \ 0047AF58
;

: off-map-34.phase1 ( -- )   \ 0047AF5C
;

\ ---- registered ----
' off-map-34.char-enter off-map-34 6 room-script!
' off-map-34.act00 off-map-34 $00 action-script!
' off-map-34.act01 off-map-34 $01 action-script!
' off-map-34.act02 off-map-34 $02 action-script!
' off-map-34.act4D off-map-34 $4D action-script!
' off-map-34.enter off-map-34 0 room-script!
' off-map-34.phase1 off-map-34 1 room-script!
