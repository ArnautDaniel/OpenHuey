\ story/rooms/house-of-truth-b1-1.fs - the event scripts of room house-of-truth-b1-1 ($84; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-1
USING: room-names story.words story.shared ;

\ room 0x84: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-1.cmd00 ( -- )  s" house-of-truth-b1-1.cmd00" stub-step ;

: house-of-truth-b1-1.enter ( -- )   \ 00432420
    0 1 $14 door-bits
    0 1 object-show
    1 1 object-show
    2 1 object-show
;

: house-of-truth-b1-1.char-enter ( -- )   \ 00432440
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
    0 self-is? if
        $80 exit-taken? if
            0 $2A 0 char-to-tri-facing
            8 state-flag-set
            0.0 sound-volume-scale
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 0 action-force
        then
    then
;

: house-of-truth-b1-1.phase1 ( -- )   \ 004324A0
    0 exit-usable? if
        0 exit-check
    then
    house-of-truth-b1-1.cmd00
;

: house-of-truth-b1-1.phase2 ( -- )   \ 0047AE18
;

: house-of-truth-b1-1.act00 ( -- )   \ 004324B0
    $18 state-flag-set
    1 self-scripted
    $FE action-end
    $FE char-done
    self-wait-done
    9 3 $FF char-load
    3 char-unload
    0 0 $14 door-bits
    4 0 movie-play
    3 cutscene-start
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
    3 0 char-remove
    $FE action-end
    $FE char-done
    0 $25 -1.96 -22.63 0 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    $95 story-flag-set
    1 0 0 music
    $EF door-unlock
    $EF door-open-set
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    effects-arena-flip
    0 char-in
    begin
        yield
        4 sound-bank-loaded? until
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    $FE char-activate
    $FE $84 44 2 stalker-to-room
    $FE $2C -140 char-to-tri-facing
    $FE 0 0 char-camera
    0 state-flag-set
    stalker-item-cooldown
    1 action-end
    1 char-done
    1 char-activate
    $85 0 82 hewie-to-room
    0 1 $14 door-bits
    0 1 object-show
    1 1 object-show
    2 1 object-show
    $F $51 fade
    wait-fade
    $4B resident-flag-set
    $253 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-1.act01 ( -- )   \ 00432620
    self-wait-done
    9 3 $FF char-load
    $A 4 $FF char-load
    3 char-unload
    4 char-unload
    4 0 movie-play
    3 cutscene-start
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
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' house-of-truth-b1-1.enter house-of-truth-b1-1 0 room-script!
' house-of-truth-b1-1.char-enter house-of-truth-b1-1 6 room-script!
' house-of-truth-b1-1.phase1 house-of-truth-b1-1 1 room-script!
' house-of-truth-b1-1.phase2 house-of-truth-b1-1 2 room-script!
' house-of-truth-b1-1.act00 house-of-truth-b1-1 $00 action-script!
' house-of-truth-b1-1.act01 house-of-truth-b1-1 $01 action-script!
