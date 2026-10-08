\ story/rooms/old-mansion-3f-2.fs - the event scripts of room old-mansion-3f-2 ($63; Belli Castle: Old Mansion 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-3f-2
USING: room-names story.words story.shared ;

\ room 0x63: the script's character walks to Fiona's nav triangle (character move 6, with
\ 0x204).
: old-mansion-3f-2.cmd00 ( -- )  s" old-mansion-3f-2.cmd00" stub-step ;

defer old-mansion-3f-2.act05
defer old-mansion-3f-2.act06
defer old-mansion-3f-2.act07
: old-mansion-3f-2.enter ( -- )   \ 00421130
    room-sounds
    $46 story-flag? not if
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 0 action-force
    then
    $57 story-flag? not if
        1 char-here? 1 0 char-in-nav-group? and if
            0 exit-taken? if
                1 $187 char-to-tri
            else
                1 $195 char-to-tri
            then
        then
        1 0 $1000000 nav-group
        1 1 $1000000 nav-group
        1 2 $1000000 nav-group
        1 3 $1000000 nav-group
    then
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
    1 $2300 sound-volume
;

: old-mansion-3f-2.char-enter ( -- )   \ 004211D0
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

: old-mansion-3f-2.phase1 ( -- )   \ 00421250
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $57 story-flag? not if
        1 2 char-C4? not $FE char-here? not and 1 char-here? and if
            35 fiona-started? 45 fiona-started? or if
                0 0 -55 $32 char-faces-xz? 0 0 -100 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 5 action
                    then
                then
                0 0 55 $32 char-faces-xz? 0 0 110 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 6 action
                    then
                then
            then
            44 fiona-started? if
                hewie-stays? if
                    0 1 8 action
                then
            then
        then
        0 1 char-in-nav-group? if
            1 1 var? not if
                0 ebit-set
            then
            1 1 var-set
        else 0 2 char-in-nav-group? if
            1 2 var? not if
                0 ebit-set
            then
            1 2 var-set
        else 0 3 char-in-nav-group? if
            1 3 var? not if
                0 ebit-set
            then
            1 3 var-set
        else
            1 0 var-set
        then then then
        0 ebit? if
            4 ebit? not if
                0 1 6 char-sound
                0 0 var? if
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 1 action-force
                    else
                        1 0 1 action-force
                    then
                else 0 1 var? if
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 2 action-force
                    else
                        1 0 2 action-force
                    then
                else 0 2 var? if
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 3 action-force
                    else
                        1 0 3 action-force
                    then
                then then then
                0 var-inc
                4 ebit-set
            then
            0 ebit-clear
        then
    then
    $57 story-flag? not if
        5 ebit? if
            $23C item-give
            $8281 item-give
        then
        0 -0.27 0.0 10.71 $51 8 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $92 action
                        then
                    then
                then
            then
        then
    then
    1 0.0 0.0 -68.2 $12 19 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-3f-2.phase2 ( -- )   \ 00421400
    0 6 char-in-area? 0 0 -68 $41 char-faces-xz? and if
        5 9 0 scene-change
    then
;

: old-mansion-3f-2.phase5 ( -- )   \ 00421420
    $57 story-flag? not if
        1 char-here? 1 0 char-in-nav-group? and if
            0 0 char-in-area? if
                1 $187 char-to-tri
            else
                1 $195 char-to-tri
            then
        then
    then
;

: old-mansion-3f-2.act00 ( -- )   \ 00421440
    $18 state-flag-set
    1 self-scripted
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
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 0 $14 door-bits
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
    wait-fade
    0 self-move-16
    0 $135 0.064 100.888 171 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    $46 story-flag-set
    0 self-scripted
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-3f-2.act04 ( -- )   \ 00421700
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    $B 1 object-show
    $C 1 object-show
    $D 1 object-show
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    0 1 $14 door-bits
    exit
;

: old-mansion-3f-2.act01 ( -- )   \ 00421500
    $18 state-flag-set
    1 self-scripted
    4 ebit-set
    $F $44 fade
    self-wait-done
    3 1 movie-play
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
    1 char-busy? not if
        1 char-here? 1 2 char-C4? and if
            0 1 $86 action
        then
    then
    $FF 1 char-visible
    0 0 $14 door-bits
    $14 $78 movie-param
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
    wait-fade
    1 char-here? 1 2 char-C4? and if
        1 0 char-visible
        1 action-end
    then
    $FF 0 char-visible
    old-mansion-3f-2.act04
    $F $41 fade
    wait-fade
    4 ebit-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-3f-2.act02 ( -- )   \ 004215C0
    $18 state-flag-set
    1 self-scripted
    4 ebit-set
    $F $44 fade
    self-wait-done
    5 1 movie-play
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
    1 char-busy? not if
        1 char-here? 1 2 char-C4? and if
            0 1 $86 action
        then
    then
    $FF 1 char-visible
    0 0 $14 door-bits
    $14 $78 movie-param
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
    wait-fade
    1 char-here? 1 2 char-C4? and if
        1 0 char-visible
        1 action-end
    then
    $FF 0 char-visible
    old-mansion-3f-2.act04
    $F $41 fade
    wait-fade
    4 ebit-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-3f-2.act03 ( -- )   \ 00421680
    $2C state-flag-set
    $18 state-flag-set
    1 self-scripted
    $19 state-flag-set
    4 ebit-set
    $F $44 fade
    self-wait-done
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    1 action-end
    1 char-done
    $FF 1 char-visible
    0 0 $14 door-bits
    $10 $46 movie-param
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
    $30 resident-flag-set
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: old-mansion-3f-2.act08 ( -- )   \ 00421840
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 $1000000 nav-group
    old-mansion-3f-2.cmd00
    begin
        self-done? not if
            35 fiona-started? 45 fiona-started? or if
                0 0 -55 $32 char-faces-xz? 0 0 -100 $32 char-faces-xz? and if
                    self-idle
                    self-wait-done
                    ['] old-mansion-3f-2.act05 goto
                then
                0 0 55 $32 char-faces-xz? 0 0 110 $32 char-faces-xz? and if
                    self-idle
                    self-wait-done
                    ['] old-mansion-3f-2.act06 goto
                then
            then
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act07 goto
            then
            yield
        else
            1 0 char-in-nav-group? if
                ['] old-mansion-3f-2.act07 goto
            else
                1 0 $1000000 nav-group
            then
            self-idle-or-end
        then
    again
;

:noname   \ old-mansion-3f-2.act06 (00421790; deferred: used before it is defined)
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 $1000000 nav-group
    $11F $204 6 self-move-tri
    begin
        self-done? not if
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act07 goto
            then
            44 fiona-started? if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act08 goto
            then
            yield
        else
            1 0 $1000000 nav-group
            self-idle-or-end
        then
    again
; is old-mansion-3f-2.act06

:noname   \ old-mansion-3f-2.act07 (004217D0; deferred: used before it is defined)
    self-wait-done
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-done
    0 self-look-at
    yield
    begin
        35 fiona-started? 45 fiona-started? or if
            0 0 -55 $32 char-faces-xz? 0 0 -100 $32 char-faces-xz? and if
                $FF self-look-at
                yield
                $101 self-anim
                self-wait-anim
                -1 self-move-16
                ['] old-mansion-3f-2.act05 goto
            then
            0 0 55 $32 char-faces-xz? 0 0 110 $32 char-faces-xz? and if
                $FF self-look-at
                yield
                $101 self-anim
                self-wait-anim
                -1 self-move-16
                ['] old-mansion-3f-2.act06 goto
            then
        then
        44 fiona-started? if
            $FF self-look-at
            yield
            $101 self-anim
            self-wait-anim
            -1 self-move-16
            ['] old-mansion-3f-2.act08 goto
        then
        yield
    again
; is old-mansion-3f-2.act07

:noname   \ old-mansion-3f-2.act05 (00421750; deferred: used before it is defined)
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 $1000000 nav-group
    $187 $204 6 self-move-tri
    begin
        self-done? not if
            39 fiona-started? 4 ebit? or if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act07 goto
            then
            44 fiona-started? if
                self-idle
                self-wait-done
                ['] old-mansion-3f-2.act08 goto
            then
            yield
        else
            1 0 $1000000 nav-group
            self-idle-or-end
        then
    again
; is old-mansion-3f-2.act05

: old-mansion-3f-2.act09 ( -- )   \ 004218B0
    self-wait-done
    0.0 -68.0 self-turn-to-xz
    self-wait-done
    $57 story-flag? not if
        $A00 self-anim
        self-frames-reset
        self-wait-16
        0 message
        wait-message
        self-wait-anim
        0 answer? if
            $904 self-anim
            self-wait-anim
            0 2 6 char-sound
            $57 story-flag-set
            30 hewie-trust
            0 0 $1000000 nav-group
            0 1 $1000000 nav-group
            0 2 $1000000 nav-group
            0 3 $1000000 nav-group
            $905 self-anim
            self-wait-anim
        then
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-3f-2.act0A ( -- )   \ 00421910
    self-wait-done
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
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

: old-mansion-3f-2.act0B ( -- )   \ 004219E0
    self-wait-done
    6 0.0 24.0 -60.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 12.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 36.0 $A $80 $80 $80 $40 specks
    6 0.0 24.0 90.0 $A $80 $80 $80 $40 specks
    $FF 1 char-visible
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $14 $78 movie-param
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
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $14 $78 movie-param
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
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $10 $46 movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-3f-2.enter old-mansion-3f-2 0 room-script!
' old-mansion-3f-2.char-enter old-mansion-3f-2 6 room-script!
' old-mansion-3f-2.phase1 old-mansion-3f-2 1 room-script!
' old-mansion-3f-2.phase2 old-mansion-3f-2 2 room-script!
' old-mansion-3f-2.phase5 old-mansion-3f-2 5 room-script!
' old-mansion-3f-2.act00 old-mansion-3f-2 $00 action-script!
' old-mansion-3f-2.act01 old-mansion-3f-2 $01 action-script!
' old-mansion-3f-2.act02 old-mansion-3f-2 $02 action-script!
' old-mansion-3f-2.act03 old-mansion-3f-2 $03 action-script!
' old-mansion-3f-2.act04 old-mansion-3f-2 $04 action-script!
' old-mansion-3f-2.act05 old-mansion-3f-2 $05 action-script!
' old-mansion-3f-2.act06 old-mansion-3f-2 $06 action-script!
' old-mansion-3f-2.act07 old-mansion-3f-2 $07 action-script!
' old-mansion-3f-2.act08 old-mansion-3f-2 $08 action-script!
' old-mansion-3f-2.act09 old-mansion-3f-2 $09 action-script!
' old-mansion-3f-2.act0A old-mansion-3f-2 $0A action-script!
' old-mansion-3f-2.act0B old-mansion-3f-2 $0B action-script!
