\ story/rooms/front-garden-1.fs - the event scripts of room front-garden-1 ($0; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-1
USING: room-names story.words story.shared flag-names ;

\ room 0x00: a grey glow (Effect737D0, size 30) at one of four spots picked by byte 3
\ (glow4_spot from spot 0): byte 4 0 starts it, its slot kept in event variable byte 3; else it
\ is removed.
: front-garden-1.cmd00 ( b0 b1 -- )  drop drop s" front-garden-1.cmd00" stub-step ;
\ Fiona's model +0xD0 (0, 1.5, -2.5) and +0xCC(1) when byte 3 is 0, else (0, 1.5, -1.5) and
\ +0xCC(0)
: front-garden-1.cmd01 ( b0 -- )  drop s" front-garden-1.cmd01" stub-step ;

: front-garden-1.enter ( -- )   \ 003ED800
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $80C0F2AA 0 1 screen-blend
        3 1 $14 door-bits
        4 1 $14 door-bits
        5 1 $14 door-bits
        6 1 $14 door-bits
        7 1 $14 door-bits
        8 1 $14 door-bits
        0 0 front-garden-1.cmd00
        1 0 front-garden-1.cmd00
        2 0 front-garden-1.cmd00
        3 0 front-garden-1.cmd00
        $10 -368.0 100.0 55.0 $A $80 $80 $80 $40 specks
        $A -332.0 100.8 -12.0 $E $80 $80 $80 $40 specks
        $10 -332.0 100.8 165.0 $E $80 $80 $80 $40 specks
        $10 -368.0 100.8 165.0 $C $80 $80 $80 $40 specks
        $A -327.5 82.0 236.3 $E $80 $80 $80 $50 specks
        $A -372.5 82.0 236.3 $E $80 $80 $80 $50 specks
    then
    4 story-flag? not if
        0 1 $14 door-bits
    else
        0 0 $14 door-bits
    then
    $20E story-flag? not if
        1 1 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
    then
    0 4 0.0 0.687 0.187 0.312 zone-rect
    $23C story-flag? $23D story-flag? not and if
        0 -331.4 61.0 239.2 flicker-sprite
    then
    0 door-locked? not if
        $2C story-flag? not if
            $2C story-flag-set
            0 0 338 4 15 -1 0 0.0 creature-place
            0 0 320 4 15 -1 0 0.0 creature-place
        then
    then
    $280 story-flag? not if
        1 -366.0 72.0 26.0 flicker-sprite
    then
;

: front-garden-1.char-enter ( -- )   \ 003ED960
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
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

: front-garden-1.phase1 ( -- )   \ 003ED9E0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    6 1 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 2 0 1 chars-area-camera
    9 0 -1 1 chars-area-camera
    0 1 char-entered-area? if
        0 exit-prepare
    then
    0 2 char-entered-area? if
        1 exit-prepare
    then
    4 story-flag? not if
        1 char-here? 1 2 char-C4? not and if
            0 $A char-in-area? 1 $B char-in-area? and if
                0 -204 -117 $32 char-faces-xz? if
                    35 fiona-started? if
                        0 counter-set
                        0 $F1 $C action
                        0 1 $B action-force
                    then
                then
            then
        then
    then
    $20E story-flag? not if
        0 -284.0 60.0 -97.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $20E story-flag-set
            0 1 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -284.0 60.0 -97.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -284.0 60.0 -97.0 0 0 sound
            $40 $185 noise
        then
    then
    $23C story-flag? not if
        1 -331.4 60.0 239.2 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 572 var-set
                $1A 573 var-set
                $1B 0 var-set
                $1C -331400 var-set
                $1D 61000 var-set
                $1E 239200 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    2 story-flag? if
        0 $C char-entered-area? if
            $1E chance? if
                0 $F8 8 action
            then
        then
        0 $C char-left-area? if
            $1E chance? if
                0 $F8 8 action
            then
        then
        0 $D char-entered-area? if
            $1E chance? if
                0 $F7 8 action
            then
        then
        0 $D char-left-area? if
            $1E chance? if
                0 $F7 8 action
            then
        then
        0 $E char-entered-area? if
            $1E chance? if
                0 $F6 8 action
            then
        then
        0 $E char-left-area? if
            $1E chance? if
                0 $F5 8 action
            then
        then
        $14 chance? if
            1 chance? if
                0 $F8 8 action
            else 1 chance? if
                0 $F7 8 action
            else 1 chance? if
                0 $F6 8 action
            else 1 chance? if
                0 $F5 8 action
            then then then then
        then
    then
    4 story-flag? not if
        1 $F char-in-area? if
            2 -210.0 60.0 -114.0 $4B 20 0 zone
            0 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 2 9 char-zone-bits? if
                            0 ebit-set
                            $64 chance? if
                                $1F 2 var-set
                                0 1 $88 action
                            then
                        then
                    then
                then
            then
        then
    then
;

: front-garden-1.phase2 ( -- )   \ 003EDBC0
    0 5 char-in-area? 0 0 $32 char-heading? and if
        $F story-flag? not if
            5 9 0 scene-change
        else
            5 4 0 scene-change
        then
    then
    0 char-busy? not 4 story-flag? not and if
        0 4 $2D char-faces-area? if
            0 story-flag? not if
                5 7 0 scene-change
            else $10 story-flag? not if
                5 0 0 scene-change
            else
                5 5 0 scene-change
            then then
        then
    then
    $23C story-flag? $23D story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $D 4 scene-change
        then
    then
    $280 story-flag? not if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $E 4 scene-change
        then
    then
;

: front-garden-1.phase3 ( -- )   \ 003EDC40
    camera-setup-changed? if
        2 camera-mode? if
            1.0 1.0 100.0 200.0 depth-range
        else
            depth-range-off
        then
    then
    -370.0 76.0 178.0 -370.0 76.0 109.0 -370.0 20.0 178.0 -370.0 20.0 109.0 lights-doorway
    2 camera-mode? if
        -380.0 74.0 250.0 -368.0 74.0 175.0 -380.0 22.0 250.0 -368.0 22.0 175.0 lights-doorway
    then
    -370.0 76.0 110.0 -370.0 76.0 41.0 -370.0 20.0 110.0 -370.0 20.0 41.0 lights-doorway
    -370.0 76.0 38.0 -370.0 76.0 -8.0 -370.0 20.0 39.0 -370.0 20.0 -8.0 lights-doorway
;

: front-garden-1.act00 ( -- )   \ 003EDD30
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $E message
    2 story-flag? not if
        4 1 movie-play
        2 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
    else
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
    then
    wait-message
    $F $44 fade
    wait-fade
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    $28 $FF movie-param
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
    0 $1B0 -242.807 -79.976 -173 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    $10 story-flag-set
    $5E resident-flag-set
    $F $41 fade
    wait-fade
    $223 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: front-garden-1.act01 ( -- )   \ 0047A970
    4 message
    self-wait-done
    wait-message
    self-idle-or-end
;

: front-garden-1.act02 ( -- )   \ 003EDE20
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
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 $F9 $10 action
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
    1 front-garden-1.cmd01
    scene-locked state-flag-clear
    0 self-move-16
    0 $148 -350.123 232.183 -11 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    $F story-flag-set
    $F $41 fade
    wait-fade
    $1A resident-flag-set
    $222 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: front-garden-1.act03 ( -- )   \ 0047A978
    ['] front-garden-1.act02 goto
;

: front-garden-1.act04 ( -- )   \ 003EDEE0
    self-wait-done
    $1D02 $A self-anim-blend
    $C message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: front-garden-1.act05 ( -- )   \ 003EDEF0
    self-wait-done
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $F message
    wait-message
    self-idle-or-end
;

: front-garden-1.act06 ( -- )   \ 003EDF00
    1 self-scripted
    self-wait-done
    1 self-look-at
    yield
    begin
        1 $A char-in-area? not while
        yield
    repeat
    $FF 3 -1 char-camera
    $FF self-look-at
    yield
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $A00 $A self-anim-blend
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 self-look-at
    yield
    1 wait-counter
    $FF self-look-at
    yield
    $8276 item-give
    0 self-scripted
    self-idle-or-end
;

: front-garden-1.act07 ( -- )   \ 003EDF40
    self-wait-done
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $D message
    wait-message
    self-idle-or-end
;

: front-garden-1.act08 ( -- )   \ 003EDF50
    $F8 self-is? if
        3 0 $14 door-bits
        0 1 front-garden-1.cmd00
        yield
        3 1 $14 door-bits
        0 0 front-garden-1.cmd00
        yield
        3 0 $14 door-bits
        0 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            3 1 $14 door-bits
            0 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            0 1 front-garden-1.cmd00
            yield
            3 1 $14 door-bits
            0 0 front-garden-1.cmd00
            yield
            3 0 $14 door-bits
            0 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            3 1 $14 door-bits
            0 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            0 1 front-garden-1.cmd00
            yield
            3 1 $14 door-bits
            0 0 front-garden-1.cmd00
            yield
            3 0 $14 door-bits
            0 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            3 1 $14 door-bits
            0 0 front-garden-1.cmd00
            yield
            3 0 $14 door-bits
            0 1 front-garden-1.cmd00
            yield
            3 1 $14 door-bits
            0 0 front-garden-1.cmd00
            yield
            3 0 $14 door-bits
            0 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        3 1 $14 door-bits
        0 0 front-garden-1.cmd00
        yield
        3 0 $14 door-bits
        0 1 front-garden-1.cmd00
        yield
        3 1 $14 door-bits
        0 0 front-garden-1.cmd00
    else $F7 self-is? if
        4 0 $14 door-bits
        1 1 front-garden-1.cmd00
        yield
        4 1 $14 door-bits
        1 0 front-garden-1.cmd00
        yield
        4 0 $14 door-bits
        1 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            4 1 $14 door-bits
            1 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            1 1 front-garden-1.cmd00
            yield
            4 1 $14 door-bits
            1 0 front-garden-1.cmd00
            yield
            4 0 $14 door-bits
            1 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            4 1 $14 door-bits
            1 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            1 1 front-garden-1.cmd00
            yield
            4 1 $14 door-bits
            1 0 front-garden-1.cmd00
            yield
            4 0 $14 door-bits
            1 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            4 1 $14 door-bits
            1 0 front-garden-1.cmd00
            yield
            4 0 $14 door-bits
            1 1 front-garden-1.cmd00
            yield
            4 1 $14 door-bits
            1 0 front-garden-1.cmd00
            yield
            4 0 $14 door-bits
            1 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        4 1 $14 door-bits
        1 0 front-garden-1.cmd00
        yield
        4 0 $14 door-bits
        1 1 front-garden-1.cmd00
        yield
        4 1 $14 door-bits
        1 0 front-garden-1.cmd00
    else $F6 self-is? if
        5 0 $14 door-bits
        2 1 front-garden-1.cmd00
        yield
        5 1 $14 door-bits
        2 0 front-garden-1.cmd00
        yield
        5 0 $14 door-bits
        2 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            5 1 $14 door-bits
            2 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            2 1 front-garden-1.cmd00
            yield
            5 1 $14 door-bits
            2 0 front-garden-1.cmd00
            yield
            5 0 $14 door-bits
            2 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            5 1 $14 door-bits
            2 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            2 1 front-garden-1.cmd00
            yield
            5 1 $14 door-bits
            2 0 front-garden-1.cmd00
            yield
            5 0 $14 door-bits
            2 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            5 1 $14 door-bits
            2 0 front-garden-1.cmd00
            yield
            5 0 $14 door-bits
            2 1 front-garden-1.cmd00
            yield
            5 1 $14 door-bits
            2 0 front-garden-1.cmd00
            yield
            5 0 $14 door-bits
            2 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        5 1 $14 door-bits
        2 0 front-garden-1.cmd00
        yield
        5 0 $14 door-bits
        2 1 front-garden-1.cmd00
        yield
        5 1 $14 door-bits
        2 0 front-garden-1.cmd00
    else $F5 self-is? if
        6 0 $14 door-bits
        3 1 front-garden-1.cmd00
        yield
        6 1 $14 door-bits
        3 0 front-garden-1.cmd00
        yield
        6 0 $14 door-bits
        3 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            6 1 $14 door-bits
            3 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            3 1 front-garden-1.cmd00
            yield
            6 1 $14 door-bits
            3 0 front-garden-1.cmd00
            yield
            6 0 $14 door-bits
            3 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            6 1 $14 door-bits
            3 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            3 1 front-garden-1.cmd00
            yield
            6 1 $14 door-bits
            3 0 front-garden-1.cmd00
            yield
            6 0 $14 door-bits
            3 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            6 1 $14 door-bits
            3 0 front-garden-1.cmd00
            yield
            6 0 $14 door-bits
            3 1 front-garden-1.cmd00
            yield
            6 1 $14 door-bits
            3 0 front-garden-1.cmd00
            yield
            6 0 $14 door-bits
            3 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        6 1 $14 door-bits
        3 0 front-garden-1.cmd00
        yield
        6 0 $14 door-bits
        3 1 front-garden-1.cmd00
        yield
        6 1 $14 door-bits
        3 0 front-garden-1.cmd00
    then then then then
    self-idle-or-end
;

: front-garden-1.act09 ( -- )   \ 003EE2A0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    scene-locked state-flag-set
    2 story-flag? not if
        ['] front-garden-1.act02 goto
    else
        ['] front-garden-1.act03 goto
    then
    self-idle-or-end
;

: front-garden-1.act0A ( -- )   \ 0047A97C
    self-idle-or-end
;

: front-garden-1.act0B ( -- )   \ 003EE2C0
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    0 0 8 nav-group
    1 0 $10 nav-group
    1 0 $20 nav-group
    $208 -206.0 -116.0 138 $FFFF $A self-move-to
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    4 story-flag-set
    10 hewie-trust
    0 message-param-room
    0 1 item-give-count
    0 0 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    self-wait-done
    0 0 $14 door-bits
    40 hewie-trust
    0 camera-follow
    $54 $FFFF $B self-move-tri
    self-wait-done
    1 0 8 nav-group
    1 counter-set
    0 self-scripted
    self-idle-or-end
;

: front-garden-1.act0C ( -- )   \ 003EE340
    begin
        fiona-free? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 6 action-force
    self-idle-or-end
;

: front-garden-1.act0D ( -- )   \ 003EE360
    self-wait-done
    -331.4 239.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $23D story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: front-garden-1.act0E ( -- )   \ 003EE3C0
    self-wait-done
    -366.0 26.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $280 story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: front-garden-1.act0F ( -- )   \ 003EE420
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $35 room-preload
    $F $44 fade
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: front-garden-1.act10 ( -- )   \ 003EE440
    0 front-garden-1.cmd01
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 front-garden-1.cmd01
    self-idle-or-end
;

: front-garden-1.act11 ( -- )   \ 003EE450
    self-wait-done
    $80C0F2AA 0 1 screen-blend
    3 1 $14 door-bits
    4 1 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 0 front-garden-1.cmd00
    1 0 front-garden-1.cmd00
    2 0 front-garden-1.cmd00
    3 0 front-garden-1.cmd00
    $10 -368.0 100.0 55.0 $A $80 $80 $80 $40 specks
    $A -332.0 100.8 -12.0 $E $80 $80 $80 $40 specks
    $10 -332.0 100.8 165.0 $E $80 $80 $80 $40 specks
    $10 -368.0 100.8 165.0 $C $80 $80 $80 $40 specks
    $A -327.5 82.0 236.3 $E $80 $80 $80 $50 specks
    $A -372.5 82.0 236.3 $E $80 $80 $80 $50 specks
    0 1 $14 door-bits
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
    $28 $FF movie-param
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

: front-garden-1.act12 ( -- )   \ 003EE580
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
    $80C0F2AA 0 1 screen-blend
    3 1 $14 door-bits
    4 1 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 0 front-garden-1.cmd00
    1 0 front-garden-1.cmd00
    2 0 front-garden-1.cmd00
    3 0 front-garden-1.cmd00
    $10 -368.0 100.0 55.0 $A $80 $80 $80 $40 specks
    $A -332.0 100.8 -12.0 $E $80 $80 $80 $40 specks
    $10 -332.0 100.8 165.0 $E $80 $80 $80 $40 specks
    $10 -368.0 100.8 165.0 $C $80 $80 $80 $40 specks
    $A -327.5 82.0 236.3 $E $80 $80 $80 $50 specks
    $A -372.5 82.0 236.3 $E $80 $80 $80 $50 specks
    0 1 $14 door-bits
    0 $F9 $10 action
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
    1 front-garden-1.cmd01
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' front-garden-1.enter front-garden-1 0 room-script!
' front-garden-1.char-enter front-garden-1 6 room-script!
' front-garden-1.phase1 front-garden-1 1 room-script!
' front-garden-1.phase2 front-garden-1 2 room-script!
' front-garden-1.phase3 front-garden-1 3 room-script!
' front-garden-1.act00 front-garden-1 $00 action-script!
' front-garden-1.act01 front-garden-1 $01 action-script!
' front-garden-1.act02 front-garden-1 $02 action-script!
' front-garden-1.act03 front-garden-1 $03 action-script!
' front-garden-1.act04 front-garden-1 $04 action-script!
' front-garden-1.act05 front-garden-1 $05 action-script!
' front-garden-1.act06 front-garden-1 $06 action-script!
' front-garden-1.act07 front-garden-1 $07 action-script!
' front-garden-1.act08 front-garden-1 $08 action-script!
' front-garden-1.act09 front-garden-1 $09 action-script!
' front-garden-1.act0A front-garden-1 $0A action-script!
' front-garden-1.act0B front-garden-1 $0B action-script!
' front-garden-1.act0C front-garden-1 $0C action-script!
' front-garden-1.act0D front-garden-1 $0D action-script!
' front-garden-1.act0E front-garden-1 $0E action-script!
' front-garden-1.act0F front-garden-1 $0F action-script!
' front-garden-1.act10 front-garden-1 $10 action-script!
' front-garden-1.act11 front-garden-1 $11 action-script!
' front-garden-1.act12 front-garden-1 $12 action-script!
