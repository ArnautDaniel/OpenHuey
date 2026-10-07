\ events/map0.fs - the event scripts of the rooms on the game's map 0 (kMapRooms).
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.map0
USING: events.words events.builtin ;

\ ---- room $00 ----------------------------------------------------------------------------------

\ room 0x00: a grey glow (Effect737D0, size 30) at one of four spots picked by byte 3
\ (glow4_spot from spot 0): byte 4 0 starts it, its slot kept in event variable byte 3; else it
\ is removed.
: room00.cmd00 ( b0 b1 -- )  drop drop stub-step ;
\ Fiona's model +0xD0 (0, 1.5, -2.5) and +0xCC(1) when byte 3 is 0, else (0, 1.5, -1.5) and
\ +0xCC(0)
: room00.cmd01 ( b0 -- )  drop stub-step ;

: room00.enter ( -- )   \ 003ED800
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
        0 0 room00.cmd00
        1 0 room00.cmd00
        2 0 room00.cmd00
        3 0 room00.cmd00
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
    0 door-unlocked? not if
        $2C story-flag? not if
            $2C story-flag-set
            \ (nop-progress-74: no effect in this game)
            \ (nop-progress-74: no effect in this game)
        then
    then
    $280 story-flag? not if
        1 -366.0 72.0 26.0 flicker-sprite
    then
;

: room00.char-enter ( -- )   \ 003ED960
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

: room00.phase1 ( -- )   \ 003ED9E0
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
        \ (nop-progress-14: no effect in this game)
    then
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
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

: room00.phase2 ( -- )   \ 003EDBC0
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

: room00.phase3 ( -- )   \ 003EDC40
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

: room00.act00 ( -- )   \ 003EDD30
    $18 state-flag-set
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
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room00.act01 ( -- )   \ 0047A970
    4 message
    self-wait-done
    wait-message
    self-idle-or-end
;

: room00.act02 ( -- )   \ 003EDE20
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
    1 room00.cmd01
    $12 state-flag-clear
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
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room00.act03 ( -- )   \ 0047A978
    ['] room00.act02 goto
;

: room00.act04 ( -- )   \ 003EDEE0
    self-wait-done
    $1D02 $A self-anim-blend
    $C message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room00.act05 ( -- )   \ 003EDEF0
    self-wait-done
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $F message
    wait-message
    self-idle-or-end
;

: room00.act06 ( -- )   \ 003EDF00
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

: room00.act07 ( -- )   \ 003EDF40
    self-wait-done
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $D message
    wait-message
    self-idle-or-end
;

: room00.act08 ( -- )   \ 003EDF50
    $F8 self-is? if
        3 0 $14 door-bits
        0 1 room00.cmd00
        yield
        3 1 $14 door-bits
        0 0 room00.cmd00
        yield
        3 0 $14 door-bits
        0 1 room00.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            3 1 $14 door-bits
            0 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            0 1 room00.cmd00
            yield
            3 1 $14 door-bits
            0 0 room00.cmd00
            yield
            3 0 $14 door-bits
            0 1 room00.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            3 1 $14 door-bits
            0 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            3 0 $14 door-bits
            0 1 room00.cmd00
            yield
            3 1 $14 door-bits
            0 0 room00.cmd00
            yield
            3 0 $14 door-bits
            0 1 room00.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            3 1 $14 door-bits
            0 0 room00.cmd00
            yield
            3 0 $14 door-bits
            0 1 room00.cmd00
            yield
            3 1 $14 door-bits
            0 0 room00.cmd00
            yield
            3 0 $14 door-bits
            0 1 room00.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        3 1 $14 door-bits
        0 0 room00.cmd00
        yield
        3 0 $14 door-bits
        0 1 room00.cmd00
        yield
        3 1 $14 door-bits
        0 0 room00.cmd00
    else $F7 self-is? if
        4 0 $14 door-bits
        1 1 room00.cmd00
        yield
        4 1 $14 door-bits
        1 0 room00.cmd00
        yield
        4 0 $14 door-bits
        1 1 room00.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            4 1 $14 door-bits
            1 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            1 1 room00.cmd00
            yield
            4 1 $14 door-bits
            1 0 room00.cmd00
            yield
            4 0 $14 door-bits
            1 1 room00.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            4 1 $14 door-bits
            1 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            4 0 $14 door-bits
            1 1 room00.cmd00
            yield
            4 1 $14 door-bits
            1 0 room00.cmd00
            yield
            4 0 $14 door-bits
            1 1 room00.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            4 1 $14 door-bits
            1 0 room00.cmd00
            yield
            4 0 $14 door-bits
            1 1 room00.cmd00
            yield
            4 1 $14 door-bits
            1 0 room00.cmd00
            yield
            4 0 $14 door-bits
            1 1 room00.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        4 1 $14 door-bits
        1 0 room00.cmd00
        yield
        4 0 $14 door-bits
        1 1 room00.cmd00
        yield
        4 1 $14 door-bits
        1 0 room00.cmd00
    else $F6 self-is? if
        5 0 $14 door-bits
        2 1 room00.cmd00
        yield
        5 1 $14 door-bits
        2 0 room00.cmd00
        yield
        5 0 $14 door-bits
        2 1 room00.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            5 1 $14 door-bits
            2 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            2 1 room00.cmd00
            yield
            5 1 $14 door-bits
            2 0 room00.cmd00
            yield
            5 0 $14 door-bits
            2 1 room00.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            5 1 $14 door-bits
            2 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            5 0 $14 door-bits
            2 1 room00.cmd00
            yield
            5 1 $14 door-bits
            2 0 room00.cmd00
            yield
            5 0 $14 door-bits
            2 1 room00.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            5 1 $14 door-bits
            2 0 room00.cmd00
            yield
            5 0 $14 door-bits
            2 1 room00.cmd00
            yield
            5 1 $14 door-bits
            2 0 room00.cmd00
            yield
            5 0 $14 door-bits
            2 1 room00.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        5 1 $14 door-bits
        2 0 room00.cmd00
        yield
        5 0 $14 door-bits
        2 1 room00.cmd00
        yield
        5 1 $14 door-bits
        2 0 room00.cmd00
    else $F5 self-is? if
        6 0 $14 door-bits
        3 1 room00.cmd00
        yield
        6 1 $14 door-bits
        3 0 room00.cmd00
        yield
        6 0 $14 door-bits
        3 1 room00.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            6 1 $14 door-bits
            3 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            3 1 room00.cmd00
            yield
            6 1 $14 door-bits
            3 0 room00.cmd00
            yield
            6 0 $14 door-bits
            3 1 room00.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            6 1 $14 door-bits
            3 0 room00.cmd00
            self-frames-reset
            2 self-wait-frames
            6 0 $14 door-bits
            3 1 room00.cmd00
            yield
            6 1 $14 door-bits
            3 0 room00.cmd00
            yield
            6 0 $14 door-bits
            3 1 room00.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            6 1 $14 door-bits
            3 0 room00.cmd00
            yield
            6 0 $14 door-bits
            3 1 room00.cmd00
            yield
            6 1 $14 door-bits
            3 0 room00.cmd00
            yield
            6 0 $14 door-bits
            3 1 room00.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        6 1 $14 door-bits
        3 0 room00.cmd00
        yield
        6 0 $14 door-bits
        3 1 room00.cmd00
        yield
        6 1 $14 door-bits
        3 0 room00.cmd00
    then then then then
    self-idle-or-end
;

: room00.act09 ( -- )   \ 003EE2A0
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    $12 state-flag-set
    2 story-flag? not if
        ['] room00.act02 goto
    else
        ['] room00.act03 goto
    then
    self-idle-or-end
;

: room00.act0A ( -- )   \ 0047A97C
    self-idle-or-end
;

: room00.act0B ( -- )   \ 003EE2C0
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

: room00.act0C ( -- )   \ 003EE340
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

: room00.act0D ( -- )   \ 003EE360
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

: room00.act0E ( -- )   \ 003EE3C0
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

: room00.act0F ( -- )   \ 003EE420
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    \ (nop-progress-18: no effect in this game)
    $F $44 fade
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    8 state-flag-set
    $80 exit-check
    self-idle-or-end
;

: room00.act10 ( -- )   \ 003EE440
    0 room00.cmd01
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 room00.cmd01
    self-idle-or-end
;

: room00.act11 ( -- )   \ 003EE450
    self-wait-done
    $80C0F2AA 0 1 screen-blend
    3 1 $14 door-bits
    4 1 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    7 1 $14 door-bits
    8 1 $14 door-bits
    0 0 room00.cmd00
    1 0 room00.cmd00
    2 0 room00.cmd00
    3 0 room00.cmd00
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

: room00.act12 ( -- )   \ 003EE580
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
    0 0 room00.cmd00
    1 0 room00.cmd00
    2 0 room00.cmd00
    3 0 room00.cmd00
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
    1 room00.cmd01
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room00.enter $00 0 room-script!
' room00.char-enter $00 6 room-script!
' room00.phase1 $00 1 room-script!
' room00.phase2 $00 2 room-script!
' room00.phase3 $00 3 room-script!
' room00.act00 $00 $00 action-script!
' room00.act01 $00 $01 action-script!
' room00.act02 $00 $02 action-script!
' room00.act03 $00 $03 action-script!
' room00.act04 $00 $04 action-script!
' room00.act05 $00 $05 action-script!
' room00.act06 $00 $06 action-script!
' room00.act07 $00 $07 action-script!
' room00.act08 $00 $08 action-script!
' room00.act09 $00 $09 action-script!
' room00.act0A $00 $0A action-script!
' room00.act0B $00 $0B action-script!
' room00.act0C $00 $0C action-script!
' room00.act0D $00 $0D action-script!
' room00.act0E $00 $0E action-script!
' room00.act0F $00 $0F action-script!
' room00.act10 $00 $10 action-script!
' room00.act11 $00 $11 action-script!
' room00.act12 $00 $12 action-script!

\ ---- room $01 ----------------------------------------------------------------------------------

: room01.enter ( -- )   \ 003EE770
    $37 story-flag? $24 story-flag? not and if
        $24 story-flag-set
        \ (nop-progress-74: no effect in this game)
    then
    $3B story-flag? not if
        1 exit-door-open? if
            4 0.1 0 bgm
        else
            4 0.01 0 bgm
        then
    then
    $12 story-flag? not if
        1 1 $14 door-bits
        1 0 8 nav-group
    else
        0 1 $14 door-bits
    then
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $A 103.4 28.8 -51.9 $A $80 $80 $80 $20 specks
        $10 83.3 29.0 14.8 $E $80 $80 $80 $50 specks
        $10 -44.8 13.0 -13.0 $E $80 $80 $80 $40 specks
        $C -87.5 12.9 36.0 $A $80 $80 $80 $40 specks
    then
;

: room01.char-enter ( -- )   \ 003EE820
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    2 0 -1 area-camera
    0 self-is? if
        $301 story-flag? $302 story-flag? not and if
            0.0 sound-volume-scale
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
            0 $B7 0 char-to-tri-facing
            $303 story-flag? if
                1 0 306 hewie-to-room
                1 $132 0 char-to-tri-facing
            then
            $302 story-flag-set
            0 ebit-set
            0 $F1 1 action
        then
    then
;

: room01.phase1 ( -- )   \ 003EE920
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    8 0 -1 1 chars-area-camera
    9 1 0 1 chars-area-camera
    $A 1 0 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    $C 0 -1 -1 chars-area-camera
    $340 story-flag? not if
        $D 3 -1 1 chars-area-camera
    then
    $12 story-flag? not if
        0 3 char-entered-area? if
            \ (nop-progress-14: no effect in this game)
        then
        0 4 char-entered-area? if
            \ (nop-progress-14: no effect in this game)
        then
    else
        0 3 char-entered-area? 0 5 char-entered-area? or if
            \ (nop-progress-14: no effect in this game)
        then
        0 4 char-entered-area? if
            \ (nop-progress-14: no effect in this game)
        then
        0 6 char-entered-area? if
            \ (nop-progress-14: no effect in this game)
        then
    then
    0 ebit? if
        $FE char-here? if
            0 ebit-clear
        then
    then
    $234 story-flag? not if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not if
                0 control-action? if
                    -40 -16 60 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                        0 -40 60 $32 char-faces-xz? if
                            hewie-stays? if
                                0 0 3 action
                            then
                        then
                    then
                then
            then
        then
        2 -33.33 -16.93 69.69 $28 6 0 zone
        3 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 2 9 char-zone-bits? if
                        3 ebit-set
                        $64 chance? if
                            $1F 2 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        2 -33.33 -16.93 69.69 $28 6 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    $1F 2 var-set
                    $1F 6.0 hewie-look-zone
                then
            then
        then
    then
    1 ebit? not if
        0 -46.0 -16.0 70.0 $C 4 0 zone
        0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
            0 5 -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
            1 $A -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
            $FE 5 -46.0 -16.0 70.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
    then
    2 ebit? not if
        1 -23.0 -16.0 67.0 $C 4 0 zone
        0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
            0 5 -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
            1 $A -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
            $FE 5 -23.0 -16.0 67.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
    then
    $340 story-flag? not if
        0 $E char-in-area? fiona-free? and if
            2 game-mode? 0 $FE 70 chars-within? not and 2 game-mode? not or if
                $340 story-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 5 action-force
                else
                    1 0 5 action-force
                then
            then
        then
    then
    $3B story-flag? not if
        1 exit-door-open? not if
            4 0.01 0 bgm
        else
            4 0.1 0 bgm
        then
    then
;

: room01.phase2 ( -- )   \ 003EEB90
    $12 story-flag? not if
        0 7 char-in-area? 0 -67 $2D char-heading? and if
            5 0 0 scene-change
        then
    then
    1 exit-door-open? not 0 ebit? and if
        0 1 char-group-bit4? if
            0 scene-ending
        then
    then
;

: room01.phase3 ( -- )   \ 003EEBC0
    -41.7 -6.2 58.0 -70.6 -6.2 58.0 -41.7 -18.6 58.0 -70.6 -18.6 58.0 lights-doorway
;

: room01.phase5 ( -- )   \ 003EEC00
    0 ebit? $FE char-here? not and $FE 2 char-C4? and if
        $FE action-end
        2 summon-take
    then
    1 char-busy? if
        1 action-end
        1 $FE -40.709 45.642 180 char-to-xz
        $18 state-flag-clear
    then
;

: room01.act00 ( -- )   \ 003EEC28
    self-wait-done
    $1D02 $A self-anim-blend
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room01.act01 ( -- )   \ 003EEC40
    0 $28 5 char-sound
    5 0 0 music
    7 state-flag-set
    yield
    7 state-flag-clear
    8 state-flag-clear
    $AF story-flag? not if
        $22A item-add
    then
    $F $51 fade
    wait-fade
    $AF story-flag? not if
        $C $85 0.0 0.0 0.0 0 0 sound
    then
    self-idle-or-end
;

: room01.act02 ( -- )   \ 0047A980
    self-wait-done
    0 message
    self-idle-or-end
;

: room01.act03 ( -- )   \ 003EEC80
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 4 action-force
        $C00 self-anim
        0 1 char-wait-motion
        0 $2E 5 char-sound
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $18 state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: room01.act04 ( -- )   \ 003EECB0
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $1E0 -35.67 31.26 -10 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $1BD -42.443 71.623 1.0 45 hewie-go-to
    self-wait-done
    $1BE -31.549 70.055 150 $FFFF 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $A state-flag? 0 char-busy? not or if
        $95 $63 item-count? not if
            10 hewie-trust
        then
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $234 story-flag-set
            $95 1 item-give-count
            0 $95 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    then
    $1C0 -42.94 81.76 176 $FFFF 5 self-move-to
    self-wait-done
    $FE -40.709 45.642 1.0 45 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room01.act05 ( -- )   \ 003EED70
    1 self-scripted
    self-wait-done
    $17 state-flag-set
    $24 state-flag-set
    $F 6 fade
    wait-fade
    $40AA message
    wait-message
    $F 7 fade
    wait-fade
    $340 story-flag-set
    $AA message-param-room
    $AA 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    $24 state-flag-clear
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

' room01.enter $01 0 room-script!
' room01.char-enter $01 6 room-script!
' room01.phase1 $01 1 room-script!
' room01.phase2 $01 2 room-script!
' room01.phase3 $01 3 room-script!
' room01.phase5 $01 5 room-script!
' room01.act00 $01 $00 action-script!
' room01.act01 $01 $01 action-script!
' room01.act02 $01 $02 action-script!
' room01.act03 $01 $03 action-script!
' room01.act04 $01 $04 action-script!
' room01.act05 $01 $05 action-script!

\ ---- room $02 ----------------------------------------------------------------------------------

\ the room object pstr_ori's +0x24 by byte 3: 0 set (37.978 with progress flag 0x12, else 11), 1
\ up 0.25 to 37.978 (then event +0x5C (2)), 2 up 0.25, else down 0.25
: room02.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x02 (Room02_Cmd01_ptmf): the panic (progress +0x7B8) raised to 80
: room02.cmd01 ( -- )  stub-step ;
\ room 0x02 (D_003F03A0): the 0xE60-byte effect OrangeSparks_vtable by byte 3 - 0 made (its slot
\ kept in event variable 0), 1 that one sent 0 (stop), else one more made and sent 1
: room02.cmd02 ( b0 -- )  drop stub-step ;
\ room 0x02 (D_003F03B0): the drum can's wobble by byte 3 - 0 still (rest height +0x38 = its
\ height), 2 struck (+0x34 strength 1), 1 each frame: the strength fades by 0.2 while it bobs
\ 0.2 x strength x sin(phase +0x30, on by 90 degrees) about the rest height
: room02.cmd03 ( b0 -- )  drop stub-step ;
\ room 0x02 (D_003F03C0): the player is about and down at floor level (y <= 0)
: room02.cond00? ( -- flag )  stub-flag ;
\ room 0x02 (Room02_Cond01_ptmf): the pursuer is about, in a mode other than 0, 1 or 5, and
\ progress +0x1130 isn't 0xFE
: room02.cond01? ( -- flag )  stub-flag ;

: room02.enter ( -- )   \ 003EEDF0
    room-sounds
    0 1 $14 door-bits
    5 1 $14 door-bits
    1 3 $38 nav-group
    0 room02.cmd00
    $12 story-flag? not if
        1 4 8 nav-group
        2 0 object-show
        6 0 $14 door-bits
    else
        2 1 object-show
        6 1 $14 door-bits
    then
    $20A story-flag? not if
        1 1 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        $285 story-flag? not if
            0 215.5 0.0 2.6 flicker-sprite
        then
    then
    $20B story-flag? not if
        1 2 $8000000 nav-group
        3 1 $14 door-bits
        4 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        3 0 $14 door-bits
        4 1 $14 door-bits
        $286 story-flag? not if
            2 290.5 0.0 -99.5 flicker-sprite
        then
    then
    0 $C 0.562 0.5 0.25 0.5 zone-rect
    1 $C 0.812 0.687 0.187 0.312 zone-rect
    $24E story-flag? $24F story-flag? not and if
        1 292.2 0.0 -61.8 flicker-sprite
    then
    0 room02.cmd02
    3 0 4 $FF $FC 0 $50 1 butterflies
    0 room02.cmd03
    8 229.9 29.0 -63.9 $B $80 $80 $80 $30 specks
    $A 111.4 25.0 -58.5 $B $80 $80 $80 $30 specks
    $E 122.8 19.0 103.2 $E $80 $80 $80 $40 specks
    $C 63.3 19.0 151.6 $10 $80 $80 $80 $40 specks
    8 34.2 19.0 92.1 $C $80 $80 $80 $40 specks
    1 $38B $10000000 nav-group-2
    1 $1B9 $10000000 nav-group-2
    1 $38C $10000000 nav-group-2
    1 $38D $10000000 nav-group-2
    1 $386 $10000000 nav-group-2
    1 $38E $10000000 nav-group-2
;

: room02.act0E ( -- )   \ 003EFED0
    6 ebit-set
    $12 ebit-set
    4 ebit-clear
    $C 0 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $C 1 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $C 2 pvar? if
        $4B chance? if
            4 ebit-set
        then
    else $C 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    $FE char-unseen? not if
        4 ebit-set
    then
    2 creature-action? if
        4 ebit-set
    then
    4 stalker-alert? if
        4 ebit-clear
    then
    room02.cond01? not if
        $FE camera-follow
    then
    4 ebit? if
        0 $FE $F action
    else
        $78 1 item-cooldown
        $16 ebit-clear
    then
    0 stalker-alert? if
        $FE 2 stalker-mode
    then
    $C pvar-inc
    exit
;

: room02.char-enter ( -- )   \ 003EEF70
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
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 4 char-camera
            0 camera-follow
        else
            1 5 4 char-camera
            1 camera-follow
        then
    then then
    1 5 4 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
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
        $80 exit-taken? $81 exit-taken? or if
            5 ebit-set
            0 $324 180 char-to-tri-facing
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            0 0 2 action
            $80 exit-taken? if
                1 char-activate
                2 0 691 hewie-to-room
                1 $2B3 50 char-to-tri-facing
            then
        then
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
        1 exit-taken? if
            3 map-page
        then
    then
    $FE self-is? if
        $D ebit-clear
        9 state-flag? if
            room02.act0E
        else
            $12 ebit-clear
        then
    then
;

: room02.phase1 ( -- )   \ 003EF0D0
    $13 ebit? not if
        6 sound-bank-loaded? if
            $40000006 6 276.0 5.0 -146.0 0 0 sound
            $13 ebit-set
        then
    else
        $C0000001 6 110.0 -10.0 142.0 0 0 sound
        $C0000006 6 276.0 5.0 -146.0 0 0 sound
    then
    $34 story-flag? $21 story-flag? not and if
        0 exit-usable? if
            2 3 0 char-load
            0 exit-check
        then
    else 0 exit-usable? if
        0 exit-check
    then then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    5 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 3 3 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 3 3 1 chars-area-camera
    $11 4 -1 1 chars-area-camera
    $16 5 4 1 chars-area-camera
    0 4 char-entered-area? if
        $34 story-flag? $21 story-flag? not and if
            2 3 0 char-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? 0 6 char-entered-area? or if
        $34 story-flag? $21 story-flag? not and if
            3 0 char-remove
        then
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? 0 7 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 8 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        2 map-page
    then
    0 $16 char-entered-area? if
        3 map-page
    then
    $24E story-flag? not if
        8 292.2 -1.0 -61.8 $A 5 0 zone
        1 8 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 590 var-set
                $1A 591 var-set
                $1B 1 var-set
                $1C 292200 var-set
                $1D 0 var-set
                $1E -61800 var-set
                $1F 8 var-set
                0 1 $8B action
            then
        then
    then
    2 276.5 -1.0 -145.9 $E 4 0 zone
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        1 room02.cmd02
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        1 room02.cmd02
    then
    4 276.4 -1.0 -145.7 5 10 0 zone
    1 room02.cmd03
    $14 ebit? not if
        $FF 4 char-in-zone? if
            $14 ebit-set
            $40000007 6 276.0 5.0 -146.0 0 0 sound
            2 room02.cmd03
            1 room02.cmd02
            2 room02.cmd02
            2 room02.cmd02
            2 room02.cmd02
        then
    else 0 8 char-action? not if
        $14 ebit-clear
    then then
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        0 7 char-left-area? 0 8 char-left-area? or 0 $11 char-left-area? or if
            $17 ebit? not if
                0 $F1 $18 action
            then
        then
    then
    5 ebit? if
        $FE char-here? if
            5 ebit-clear
        then
    then
    $FE 2 char-C4? not if
        9 state-flag? 6 ebit? not and $FE char-here? and if
            $FE char-busy? not if
                $16 ebit-set
                room02.act0E
            then
        then
    then
    $FE 5 char-in-area? room02.cond00? and if
        $D ebit? not $FE 2 char-C4? not and if
            2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
                0 stalker-alert? 2 stalker-alert? or if
                    $FE char-busy? 4 ebit? and 9 state-flag? and $B ebit? not and if
                        $FE action-end
                    then
                    0 $FE $10 action
                then
            then
        then
    then
    $FE $1A char-in-area? 4 stalker-alert? and $FE 2 char-C4? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 $FE $13 action
        then
    then
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        7 -142.16 0.0 179.0 $4B 20 0 zone
        $A ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 7 9 char-zone-bits? if
                        $A ebit-set
                        $64 chance? if
                            $1F 7 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
    then
    0 ebit? not if
        1 $19 char-in-area? if
            6 5.346 -9.5 80.917 $28 20 0 zone
            9 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 6 9 char-zone-bits? if
                            9 ebit-set
                            $1E chance? if
                                $1F 6 var-set
                                0 1 $88 action
                            then
                        then
                    then
                then
            then
        then
    then
    $12 story-flag? not if
        5 50.0 -10.0 164.549 $1E 20 0 zone
        8 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 5 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 5 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    $C 274.93 -1.0 -145.73 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 $C 9 char-zone-bits? if
                $1F 12 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    $F ebit? not if
        $A 28.0 -9.0 72.0 $10 4 0 zone
        0 $A 3 char-zone-bits? 0 $A 3 char-zone-bits-before? not and if
            0 5 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
        1 $A 3 char-zone-bits? 1 $A 3 char-zone-bits-before? not and if
            1 $A 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
        $FE $A 3 char-zone-bits? $FE $A 3 char-zone-bits-before? not and if
            $FE 5 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
    then
    $10 ebit? not if
        $B 8.0 -9.0 70.0 $10 4 0 zone
        0 $B 3 char-zone-bits? 0 $B 3 char-zone-bits-before? not and if
            0 5 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
        1 $B 3 char-zone-bits? 1 $B 3 char-zone-bits-before? not and if
            1 $A 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
        $FE $B 3 char-zone-bits? $FE $B 3 char-zone-bits-before? not and if
            $FE 5 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
    then
    9 state-flag? if
        room02.cond01? if
            0 camera-follow
        else
            $FE camera-follow
        then
    then
    35 fiona-started? 45 fiona-started? or 1 2 char-C4? not and $FE char-here? not and 0 $13 char-in-area? and 1 $17 char-in-area? and 0 45 69 $32 char-faces-xz? and 0 ebit? not and if
        hewie-stays? if
            0 1 3 action
        then
    then
    44 fiona-started? 0 $13 char-in-area? and 1 3 char-in-nav-group? and if
        0 1 4 action
    then
    $15 ebit? 1 char-busy? not and if
        $15 ebit-clear
        0 ebit-clear
        $18 state-flag-clear
    then
;

: room02.phase2 ( -- )   \ 003EF5B0
    0 $18 char-in-area? 0 0 $3C char-heading? and if
        $FE char-here? not if
            5 $C 5 scene-change
        else $FE char-unseen? if
            5 $C 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    $12 story-flag? not if
        0 9 char-in-area? 0 22 $32 char-heading? and if
        then
    then
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $15 char-in-area? 0 0 $32 char-heading? and if
        5 6 0 scene-change
    then
    0 $A char-in-area? 0 -45 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 $14 char-in-area? 0 90 $32 char-heading? and if
        5 9 0 scene-change
    then
    9 276.5 -1.0 -145.9 7 10 0 zone
    0 9 2 char-zone-bits? 0 276 -146 $32 char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 exit-door-open? not 5 ebit? and if
        0 0 char-group-bit4? if
            7 scene-ending
        then
    then
    $24E story-flag? $24F story-flag? not and if
        8 1 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 $11 4 scene-change
        then
    then
    $20A story-flag? not if
        0 215.5 -1.0 2.6 5 8 1 zone
        $FF 0 char-in-zone? if
            $20A story-flag-set
            0 1 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            215.5 -1.0 2.6 0 -2145378272 0 0.0 scene-effect-8C
            3 6 215.5 -1.0 2.6 0 0 sound
            $40 $377 noise
            0 215.5 0.0 2.6 flicker-sprite
        then
    else $285 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $14 4 scene-change
        then
    then then
    $20B story-flag? not if
        1 290.5 -1.0 -99.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $20B story-flag-set
            0 2 $8000000 nav-group
            3 0 $14 door-bits
            4 1 $14 door-bits
            290.5 -1.0 -99.5 1 -2145378272 0 0.0 scene-effect-8C
            $88 5 290.5 -1.0 -99.5 0 0 sound
            $40 $380 noise
            2 290.5 0.0 -99.5 flicker-sprite
        then
    else $286 story-flag? not if
        1 2 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $15 4 scene-change
        then
    then then
;

: room02.phase3 ( -- )   \ 003EF780
    263.8 8.9 -109.2 263.8 8.9 -145.2 259.799 -39.1 -119.2 258.799 -39.1 -147.2 lights-doorway
    264.8 9.1 -68.9 264.8 9.1 -104.9 261.799 -30.9 -79.9 261.799 -30.9 -90.3 lights-doorway
    210.0 8.0 -71.0 267.0 8.0 -71.0 210.0 -40.0 -75.0 266.0 -40.0 -75.0 lights-doorway
    153.0 10.0 -71.0 211.0 10.0 -71.0 153.0 -38.0 -75.0 210.0 -38.0 -75.0 lights-doorway
    60.0 5.0 57.0 90.0 5.0 57.0 60.0 -40.0 53.0 89.0 -39.0 53.0 lights-doorway
    89.0 8.0 58.0 99.0 5.0 46.0 91.3 -40.0 53.0 99.0 -40.0 42.0 lights-doorway
    -162.0 25.5 122.8 -162.0 25.5 160.5 -162.0 -10.0 123.0 -162.0 -10.0 160.0 lights-doorway
;

: room02.phase5 ( -- )   \ 003EF8E0
    $2D state-flag-clear
    1 char-here? 1 3 char-in-nav-group? and if
        1 $101 char-to-tri
        $18 state-flag-clear
    then
    0 ebit? if
        $18 state-flag-clear
    then
    5 ebit? $FE char-here? not and $FE 2 char-C4? and if
        $FE action-end
        2 summon-take
    then
    $FE char-busy? if
        $C ebit? if
            $FE action-end
            $FE $192 180 char-to-tri-facing
            $C ebit-clear
            $B ebit-clear
        else $11 ebit? if
            $FE action-end
            0 summon-take
        then then
    then
;

: room02.act00 ( -- )   \ 003EF930
    $12 story-flag? not if
        $18 state-flag-set
        self-wait-done
        0 6 char-file-load
        $8C 49.9 164.0 0 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 20.0 0.0 0.0 event-camera
        0 message
        wait-message
        0 char-file-use
        begin
            6 sound-bank-loaded? not while
            yield
        repeat
        self-frames-reset
        self-wait-16
        0 0.0 0.0 0.0 0.0 event-camera
        1 self-scripted
        0 answer? if
            2 0 object-anim
            $8000 $A self-anim-blend
            self-frames-reset
            $20 self-wait-frames
            $40000005 6 50.0 -5.0 171.0 0 0 sound
            self-frames-reset
            $3E self-wait-frames
            $40000000 6 50.0 -5.0 171.0 0 0 sound
            self-wait-anim
            $14 door-lock
            -1 self-move-16
            self-wait-anim
            $12 story-flag-set
            $5F $1F9 noise
            0 $F1 $A action
            $23 story-flag? not if
                \ (nop-progress-74: no effect in this game)
                \ (nop-progress-74: no effect in this game)
            then
        then
        0 self-scripted
        0 ebit? not if
            $18 state-flag-clear
        then
    else
        self-wait-done
        1 message
        wait-message
    then
    self-idle-or-end
;

: room02.act01 ( -- )   \ 003EFA10
    1 self-scripted
    self-wait-done
    2 wait-counter
    1 item-use
    2 item-use
    10 hewie-trust
    1 message-param-room
    1 1 item-give-count
    0 1 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $827B item-give
    counter-inc
    0 self-scripted
    self-idle-or-end
;

: room02.act02 ( -- )   \ 003EFA50
    1 self-scripted
    7 state-flag-set
    self-frames-reset
    self-wait-16
    7 state-flag-clear
    0 $28 5 char-sound
    8 state-flag-clear
    $FE $29 129 2 stalker-to-room
    $FE stalker-knock-down
    $F $41 fade
    wait-fade
    $22C item-give
    0 self-scripted
    self-idle-or-end
;

: room02.act04 ( -- )   \ 003EFB20
    1 self-scripted
    self-wait-done
    $353 37.0 69.0 90 $FFFF $A self-move-to
    self-wait-done
    0 0 8 nav-group
    $267 52.0 69.0 90 $204 5 self-move-to
    self-wait-done
    1 3 8 nav-group
    -1 self-move-16
    0 self-scripted
    $2D state-flag-clear
    $18 state-flag-clear
    0 ebit-clear
    self-idle-or-end
;

: room02.act05 ( -- )   \ 003EFB70
    1 wait-counter
    hewie-bark
    self-wait-done
    $1DF $FFFF 6 self-move-tri
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    counter-inc
    3 wait-counter
    0 self-scripted
    self-idle-or-end
;

: room02.act03 ( -- )   \ 003EFA80
    $18 state-flag-set
    0 ebit-set
    $15 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $267 52.0 69.0 -90 $FFFF $A self-move-to
    self-wait-done
    $2D state-flag-set
    $15 ebit-clear
    1 self-scripted
    0 3 8 nav-group
    $353 40.0 69.0 -90 $204 5 self-move-to
    self-wait-done
    1 0 8 nav-group
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-anim
    begin
        0 $13 char-in-area? if
            44 fiona-started? if
                ['] room02.act04 goto
            then
            35 fiona-started? 45 fiona-started? or if
                hewie-stays? if
                    0 counter-set
                    0 $F2 $B action
                    ['] room02.act05 goto
                then
            then
        then
        0 $14 char-in-area? if
            44 fiona-started? 35 fiona-started? or 45 fiona-started? or if
                0 counter-set
                0 $F2 $B action
                ['] room02.act05 goto
            then
        then
        yield
    again
;

: room02.act06 ( -- )   \ 003EFB90
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        $18 state-flag-set
        self-wait-done
        6 message
        wait-message
        1 self-scripted
        0 answer? if
            $F $44 fade
            $B 3 $FF char-load
            3 char-unload
            $B char-activate
            self-wait-done
            3 0 movie-play
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
            0 $F9 $16 action
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $B 1 char-no-shadow
            1 0.9 camera-value
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
            1 1.0 camera-value
            0 self-move-16
            0 $EE -141.823 160.482 -2 char-to-xz
            camera-restart
            0 0 char-no-shadow
            $B 0 char-no-shadow
            1 0 char-visible
            1 action-end
            3 0 char-remove
            1 1 object-show
            room02.cmd01
            $F $41 fade
            wait-fade
            $27 story-flag-set
            $24 resident-flag-set
            $A1 message-param-room
            $A1 1 item-give-count
            0 $A1 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        0 ebit? not if
            $18 state-flag-clear
        then
        0 self-scripted
    else
        self-wait-done
        $27 story-flag? not if
            2 message
            wait-message
        else
            $1D02 $A self-anim-blend
            3 message
            wait-message
            self-wait-anim
        then
    then
    self-idle-or-end
;

: room02.act07 ( -- )   \ 0047A988
    self-idle-or-end
;

: room02.act08 ( -- )   \ 003EFCD0
    self-wait-done
    1 ebit? not if
        $A02 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        1 ebit-set
    else
        $A01 $A self-anim-blend
        5 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: room02.act09 ( -- )   \ 003EFCF0
    self-wait-done
    $7D 6.874 90.0 180 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 10.0 20.0 0.0 0.0 event-camera
    5 0 $14 door-bits
    4 message
    wait-message
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    5 1 $14 door-bits
    0 self-scripted
    self-idle-or-end
;

: room02.act0A ( -- )   \ 003EFD50
    $40000001 6 110.0 -10.0 142.0 0 0 sound
    self-frames-reset
    begin
        $40 frames? not while
        1 room02.cmd00
        yield
    repeat
    0 4 8 nav-group
    begin
        2 ebit? not while
        1 room02.cmd00
        yield
    repeat
    $40000002 6 110.0 -10.0 142.0 0 0 sound
    2 room02.cmd00
    self-frames-reset
    4 self-wait-frames
    3 room02.cmd00
    self-frames-reset
    4 self-wait-frames
    2 room02.cmd00
    self-frames-reset
    4 self-wait-frames
    3 room02.cmd00
    self-frames-reset
    4 self-wait-frames
    2 room02.cmd00
    self-frames-reset
    4 self-wait-frames
    3 room02.cmd00
    self-idle-or-end
;

: room02.act0B ( -- )   \ 003EFDD0
    begin
        fiona-free? not while
        yield
    repeat
    counter-inc
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 1 action-force
    self-idle-or-end
;

: room02.act0D ( -- )   \ 003EFE90
    3 ebit? if
        2 avoid-prompt
    then
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    $FE char-busy? $B ebit? not and $11 ebit? not and if
        3 ebit? not if
            $FE action-end
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: room02.act0C ( -- )   \ 003EFDF0
    $18 state-flag-set
    $12 ebit-clear
    1 self-scripted
    6 ebit-clear
    0 4 char-file-load
    self-wait-done
    $FF self-look-at
    yield
    0 char-file-use
    $132 $8004 5 240.0 -17.59 0 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    0 ebit? not if
        $18 state-flag-clear
    then
    9 state-flag-set
    3 ebit-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room02.act0D goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                $12 ebit? $FE char-here? not and if
                    $12 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room02.act0D goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room02.act0F ( -- )   \ 003EFF40
    self-wait-done
    $16 ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and if
            1 3 self-move-slot
            self-wait-done
        then
    else
        $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and $FE 1 char-in-area? and if
            1 3 self-move-slot
            self-wait-done
        then
    then
    $16 ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 5 char-file-load
    then
    $132 237.262 -39.055 0 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        3 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        3 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room02.act10 ( -- )   \ 003EFFD0
    $B ebit-set
    self-wait-done
    $FE 8 char-file-load
    $11 151.535 -15.151 180 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    $C ebit-set
    $8000 5 self-anim-9
    1 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        1 49 var? if
            0 153.0 -3.7 -51.0 0 0 0 0 dust
            0 148.0 -4.0 -51.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $FE 0 0 char-camera
    0 5 self-anim-blend
    self-wait-anim
    0 self-noclip
    0 self-scripted
    $C ebit-clear
    $B ebit-clear
    6 ebit-clear
    $D ebit-set
    self-idle-or-end
;

: room02.act11 ( -- )   \ 003F0060
    self-wait-done
    292.2 -61.8 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $24F story-flag-set
            1 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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

: room02.act12 ( -- )   \ 003F00C0
    self-wait-done
    276.5 -145.9 self-turn-to-xz
    self-wait-done
    $E ebit? not if
        8 message
        wait-message
        $E ebit-set
    else
        9 message
        wait-message
    then
    self-idle-or-end
;

: room02.act13 ( -- )   \ 003F00E0
    $11 ebit-set
    self-wait-done
    $FE 8 char-file-load
    $381 279.332 -83.7 90 $FFFF $A self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    $8001 5 self-anim-9
    1 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        1 54 var? if
            0 306.0 23.0 -79.0 0 0 0 0 dust
            0 306.0 23.0 -85.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $395 341.992 -29.2 90 $FFFF $A self-move-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $11 ebit-clear
    $FE action-end
    0 summon-take
    self-idle-or-end
;

: room02.act14 ( -- )   \ 003F0170
    self-wait-done
    215.5 2.6 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $285 story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: room02.act15 ( -- )   \ 003F01D0
    self-wait-done
    290.5 -99.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $62 message-param-room
        $62 $63 item-count? if
            $8010 message
            wait-message
        else
            $286 story-flag-set
            2 effect-remove
            $62 1 item-give-count
            0 $62 item-tab
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

: room02.act16 ( -- )   \ 003F0230
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        8 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: room02.act17 ( -- )   \ 003F0250
    self-wait-done
    $B 3 $FF char-load
    3 char-unload
    3 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $16 action
    1 0.9 camera-value
    $B 1 char-no-shadow
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
    1 1.0 camera-value
    0 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room02.act18 ( -- )   \ 003F02F0
    $17 ebit-set
    $40000004 6 -142.0 12.0 179.0 0 0 sound
    self-frames-reset
    $4B self-wait-frames
    $17 ebit-clear
    self-idle-or-end
;

' room02.enter $02 0 room-script!
' room02.char-enter $02 6 room-script!
' room02.phase1 $02 1 room-script!
' room02.phase2 $02 2 room-script!
' room02.phase3 $02 3 room-script!
' room02.phase5 $02 5 room-script!
' room02.act00 $02 $00 action-script!
' room02.act01 $02 $01 action-script!
' room02.act02 $02 $02 action-script!
' room02.act03 $02 $03 action-script!
' room02.act04 $02 $04 action-script!
' room02.act05 $02 $05 action-script!
' room02.act06 $02 $06 action-script!
' room02.act07 $02 $07 action-script!
' room02.act08 $02 $08 action-script!
' room02.act09 $02 $09 action-script!
' room02.act0A $02 $0A action-script!
' room02.act0B $02 $0B action-script!
' room02.act0C $02 $0C action-script!
' room02.act0D $02 $0D action-script!
' room02.act0E $02 $0E action-script!
' room02.act0F $02 $0F action-script!
' room02.act10 $02 $10 action-script!
' room02.act11 $02 $11 action-script!
' room02.act12 $02 $12 action-script!
' room02.act13 $02 $13 action-script!
' room02.act14 $02 $14 action-script!
' room02.act15 $02 $15 action-script!
' room02.act16 $02 $16 action-script!
' room02.act17 $02 $17 action-script!
' room02.act18 $02 $18 action-script!

\ ---- room $03 ----------------------------------------------------------------------------------

\ room 0x03 (Room03_Cmd00_ptmf): three objects turned (-60, -60 degrees about x; -90 about z)
: room03.cmd00 ( -- )  stub-step ;
\ a lever (pstr_kanagu, tilt +0x18 between -10 and 0 degrees): byte 3 0 back 2 degrees, 1 pulled
\ (-10) with a puff of grey dust at it
: room03.cmd01 ( b0 -- )  drop stub-step ;

: room03.enter ( -- )   \ 003F0450
    room-sounds
    $16 1.0 0 bgm
    9 story-flag? if
        room03.cmd00
    then
    $204 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $287 story-flag? not if
            0 72.5 49.0 8.5 flicker-sprite
        then
    then
    $205 story-flag? not if
        1 2 $8000000 nav-group
        3 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        3 0 $14 door-bits
        2 1 $14 door-bits
        $22A story-flag? not if
            1 53.0 114.0 -113.5 flicker-sprite
        then
    then
    $206 story-flag? not if
        1 1 $8000000 nav-group
        5 1 $14 door-bits
        4 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        5 0 $14 door-bits
        4 1 $14 door-bits
    then
    0 0 0.812 0.687 0.187 0.312 zone-rect
    $A 3.5 30.7 49.9 $10 $80 $80 $80 $40 specks
    $C 16.0 129.0 35.7 $10 $80 $80 $80 $60 specks
    $235 story-flag? not if
        2 9.646 0.163 27.266 flicker-sprite
    then
;

: room03.char-enter ( -- )   \ 003F0540
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
;

: room03.phase1 ( -- )   \ 003F0580
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    9 story-flag? not if
        3 1 1 1 chars-area-camera
        4 1 1 1 chars-area-camera
        5 0 0 1 chars-area-camera
        6 0 0 1 chars-area-camera
    then
    7 2 2 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    0 7 char-entered-area? if
        2 map-page
    then
    0 8 char-entered-area? if
        3 map-page
    then
    0 9 char-entered-area? if
        4 map-page
    then
    0 $A char-entered-area? if
        3 map-page
    then
    9 story-flag? not if
        0 room03.cmd01
        3 -11.0 113.0 -87.0 5 12 0 zone
        0 3 char-in-zone? if
            1 room03.cmd01
            1 $FF 4 rumble
            0 2 var? if
                0 var-inc
                0 $F2 $A action
            else
                0 $F1 3 action
            then
        then
    then
    $235 story-flag? not if
        1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
            2 game-mode? not if
                0 $100000 char-on-nav-flags? not if
                    0 control-action? if
                        4 0 45 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                            0 4 45 $32 char-faces-xz? if
                                hewie-stays? if
                                    0 0 6 action
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    $235 story-flag? not if
        4 9.646 0.163 27.266 $1E 6 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    $1F 4 var-set
                    $1F 6.0 hewie-look-zone
                then
            then
        then
    then
;

: room03.phase2 ( -- )   \ 003F06A0
    $204 story-flag? not if
        0 72.5 48.0 8.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $204 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            72.5 48.0 8.5 0 -2142220208 0 0.0 scene-effect-8C
            $88 5 72.5 48.0 8.5 0 0 sound
            $40 $111 noise
            0 72.5 49.0 8.5 flicker-sprite
        then
    else $287 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then then
    $205 story-flag? not if
        1 53.0 113.0 -113.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $205 story-flag-set
            0 2 $8000000 nav-group
            3 0 $14 door-bits
            2 1 $14 door-bits
            53.0 113.0 -113.5 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 53.0 113.0 -113.5 0 0 sound
            $40 $122 noise
            1 53.0 114.0 -113.5 flicker-sprite
        then
    else $22A story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then then
    $206 story-flag? not if
        2 53.0 113.0 -107.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $206 story-flag-set
            0 1 $8000000 nav-group
            5 0 $14 door-bits
            4 1 $14 door-bits
            53.0 113.0 -107.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 53.0 113.0 -107.0 0 0 sound
            $40 $123 noise
        then
    then
    0 2 char-in-area? 0 90 $32 char-heading? and if
        5 2 0 scene-change
    then
;

: room03.phase3 ( -- )   \ 003F0820
    78.0 127.0 -100.0 78.0 127.0 -19.0 78.0 90.0 -100.0 78.0 90.0 -19.0 lights-doorway
;

: room03.phase5 ( -- )   \ 003F0860
    1 char-busy? if
        1 action-end
        1 $CC 32.509 56.979 60 char-to-xz
        $18 state-flag-clear
    then
    $FE char-busy? 0 ebit? and if
        $FE $8F 2.4 -76.445 90 char-to-xz
        0 ebit-clear
    then
;

: room03.act00 ( -- )   \ 003F0890
    1 char-busy? if
        1 action-end
        1 $CC 32.509 56.979 60 char-to-xz
        $18 state-flag-clear
    then
    1 self-scripted
    $FE action-end
    $FE char-done
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $F $44 fade
    $FF 1.0 0 bgm
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
    $10 $FF movie-param
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
    $FE 2 char-file-load
    0 self-move-16
    0 $8C 1.471 -112.581 -161 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    yield
    camera-restart
    room03.cmd00
    9 story-flag-set
    $18 $35 door-copy
    $35 door-close-off-unlock
    exits-rebuild
    1 $2F char-in-room? if
        $30 0 -1 hewie-to-room
    then
    $FE char-activate
    $FE 3 153 2 stalker-to-room
    stalker-item-cooldown
    $FE -1 -1 char-camera
    $FE 1 char-visible
    $FE 1 char-silent
    0 $FE 5 action
    $16 1.0 0 bgm
    $F $41 fade
    wait-fade
    $22B item-give
    $18 state-flag-clear
    0 self-scripted
    \ (nop-progress-24: no effect in this game)
    0 ebit-clear
    self-idle-or-end
;

: room03.act01 ( -- )   \ 003F09C0
    self-wait-done
    9 story-flag? not if
        3 message
        wait-message
    else
        2 message
        wait-message
    then
    self-idle-or-end
;

: room03.act02 ( -- )   \ 003F09D0
    self-wait-done
    9 story-flag? not if
        $17 state-flag-set
        1 self-scripted
        0 $8E -6.625 -82.651 -144 char-to-xz
        1 10.0 15.0 0.0 0.0 event-camera
        0 0 var? if
            0 message
            wait-message
        else
            1 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        $17 state-flag-clear
        0 self-scripted
    else
        2 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room03.act03 ( -- )   \ 003F0A30
    0 0 6 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    0 var-inc
    self-idle-or-end
;

: room03.act04 ( -- )   \ 003F0A40
    self-wait-done
    53.0 -113.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $22A story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: room03.act05 ( -- )   \ 003F0AA0
    0 ebit-set
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    yield
    $FE $132 -37.43 44.367 180 char-to-xz
    $FE 0 0 char-camera
    1 self-noclip
    1 self-scripted
    $FE 0 char-visible
    $FE 0 char-silent
    $13C -45.739 -73.201 90 $FFFF $A self-move-to
    self-wait-done
    $8000 5 self-anim-9
    1 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        1 89 var? if
            0 5.0 115.0 -70.0 0 0 0 0 dust
            0 5.0 115.0 -76.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $FE 0 stalker-mode
    0 self-noclip
    0 self-scripted
    0 ebit-clear
    stalker-item-cooldown
    self-idle-or-end
;

: room03.act06 ( -- )   \ 003F0B30
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 7 action-force
        $C00 self-anim
        0 1 char-wait-motion
        0 $2E 5 char-sound
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $18 state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: room03.act07 ( -- )   \ 003F0B60
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $13 53.585 39.199 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $148 0.0 43.21 1.5 40 hewie-go-to
    self-wait-done
    $145 9.804 32.761 180 $FFFF 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $235 story-flag? not if
        $A state-flag? 0 char-busy? not or if
            $70 $63 item-count? not if
                10 hewie-trust
            then
            $70 message-param-room
            $70 $63 item-count? if
                $8010 message
                wait-message
            else
                $235 story-flag-set
                $70 1 item-give-count
                0 $70 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            $235 story-flag? if
                2 effect-remove
            then
        then
    then
    $148 -7.96 39.28 66 $FFFF 5 self-move-to
    self-wait-done
    $CC 32.509 56.979 1.5 50 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room03.act08 ( -- )   \ 003F0C20
    self-wait-done
    72.5 8.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $287 story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: room03.act09 ( -- )   \ 003F0C80
    self-wait-done
    $A 3.5 30.7 49.9 $10 $80 $80 $80 $40 specks
    $C 16.0 129.0 35.7 $10 $80 $80 $80 $60 specks
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
    $10 $FF movie-param
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

: room03.act0A ( -- )   \ 003F0D30
    $18 state-flag-set
    0 0 6 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 char-action? if
        0 0 0 action-force
    else
        1 0 0 action-force
    then
    self-idle-or-end
;

' room03.enter $03 0 room-script!
' room03.char-enter $03 6 room-script!
' room03.phase1 $03 1 room-script!
' room03.phase2 $03 2 room-script!
' room03.phase3 $03 3 room-script!
' room03.phase5 $03 5 room-script!
' room03.act00 $03 $00 action-script!
' room03.act01 $03 $01 action-script!
' room03.act02 $03 $02 action-script!
' room03.act03 $03 $03 action-script!
' room03.act04 $03 $04 action-script!
' room03.act05 $03 $05 action-script!
' room03.act06 $03 $06 action-script!
' room03.act07 $03 $07 action-script!
' room03.act08 $03 $08 action-script!
' room03.act09 $03 $09 action-script!
' room03.act0A $03 $0A action-script!

\ ---- room $04 ----------------------------------------------------------------------------------

\ two dials (byte 3: pstr_syuukouki2 on var 0, pstr_syuukouki1 on var 1, from -90 degrees), byte
\ 4 the step
: room04.cmd00 ( b0 b1 -- )  drop drop stub-step ;
\ room 0x04 (Room04_Cmd01_ptmf): five objects turned -75 / 75 degrees in turn
: room04.cmd01 ( -- )  stub-step ;
\ room 0x04 (Room04_Cmd02_ptmf): byte 3 0: room effect 0x1B (MirrorFragment_vtable) on its
\ object, a box (640, -560, 1000, 0, 0x60); else the effect gone
: room04.cmd02 ( b0 -- )  drop stub-step ;
\ room 0x04 (Room04_Cmd03_ptmf): an effect on one object (byte 3 0: at 90 degrees) or the other
\ (0)
: room04.cmd03 ( b0 -- )  drop stub-step ;

: room04.enter ( -- )   \ 003F0DF0
    $26 story-flag? not if
        room-sounds
        1 1 $20000 nav-group
    else
        room04.cmd01
        0 1 $20000 nav-group
    then
    $30 story-flag? not if
        2 1 $14 door-bits
        2 1 object-show
    else
        2 0 $14 door-bits
        0 0 room04.cmd00
    then
    1 0 room04.cmd00
    $207 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $289 story-flag? not if
            0 -73.5 -102.0 55.5 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    1 $2300 sound-volume
;

: room04.char-enter ( -- )   \ 003F0E80
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
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    1 2 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    0 self-is? if
        $80 exit-taken? $81 exit-taken? or if
            0 $56 -68.279 -15.589 -90 char-to-xz
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
            0 0 5 action
        then
    then
;

: room04.phase1 ( -- )   \ 003F0F70
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    1 52.29 -103.0 49.3 $1E 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 30.64 -103.0 -51.68 $1E 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    0 3 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
            $FE $C char-file-load
        then
        \ (nop-progress-14: no effect in this game)
    then
;

: room04.phase2 ( -- )   \ 003F1060
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    $26 story-flag? not if
        0 $E char-in-area? 0 -45 $32 char-heading? and if
            5 2 0 scene-change
        then
    then
    0 $C char-in-area? 0 54 49 $32 char-faces-xz? and if
        5 3 0 scene-change
    then
    0 $D char-in-area? 0 30 -51 $32 char-faces-xz? and if
        5 6 0 scene-change
    then
    $207 story-flag? not if
        0 -73.5 -103.0 55.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $207 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -73.5 -103.0 55.5 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -73.5 -103.0 55.5 0 0 sound
            $40 $1E3 noise
            0 -73.5 -102.0 55.5 flicker-sprite
        then
    else $289 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then then
;

: room04.act00 ( -- )   \ 003F1130
    self-wait-done
    $FE self-touching? not if
        $1E self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $1E char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $1E door-lock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: room04.act01 ( -- )   \ 003F1170
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    \ (nop-progress-18: no effect in this game)
    self-wait-done
    wait-fade
    $1B state-flag-set
    8 state-flag-set
    $80 exit-check
    self-idle-or-end
;

: room04.act02 ( -- )   \ 003F1190
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $A0 -76.0 -15.0 -90 char-to-xz
    $17 state-flag-set
    1 self-scripted
    1 20.0 -10.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    3 ebit? not if
        3 message
        wait-message
        3 ebit-set
    else
        4 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room04.act03 ( -- )   \ 003F11F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $26 story-flag? if
        1 message
        wait-message
    else $30 story-flag? not if
        self-frames-reset
        8 self-wait-frames
        $17 state-flag-set
        0 $73 61.0 50.0 -90 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        self-frames-reset
        8 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
    else
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        $17 state-flag-set
        0 $73 61.0 50.0 -90 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        5 message
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                0 1 room04.cmd00
                1 ebit? not if
                    0 6 54.0 -90.0 49.0 0 0 sound
                then
            else
                0 2 room04.cmd00
                1 ebit? 0 5 pvar? and if
                    1 6 54.0 -90.0 49.0 0 0 sound
                    0 room04.cmd03
                then
            then
            yield
        repeat
        5 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            ['] room04.act01 goto
        then
    then then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room04.act04 ( -- )   \ 003F1310
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    3 1 movie-play
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
    0 $F9 9 action
    $1E $C8 movie-param
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
    1 room04.cmd02
    2 0 $14 door-bits
    1 1 object-show
    2 0 object-show
    0 self-move-16
    0 $73 63.3 50.28 -68 char-to-xz
    1 0 char-visible
    1 action-end
    camera-restart
    4 item-use
    $30 story-flag-set
    $F $41 fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room04.act05 ( -- )   \ 003F13E0
    1 self-scripted
    self-wait-done
    $FE char-here? if
        0 $FE 7 action
        yield
    then
    $17 state-flag-set
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
    $80 exit-taken? if
        \ (nop-progress-18: no effect in this game)
    else
        \ (nop-progress-18: no effect in this game)
    then
    wait-fade
    $10 $FF movie-param
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
    $80 exit-taken? if
        8 state-flag-set
        $17 state-flag-clear
        $81 exit-check
    else
        $22 door-lock
        $26 story-flag-set
        0 0 room04.cmd00
        1 0 room04.cmd00
        room04.cmd01
        0 1 $20000 nav-group
        5 0 state-flag-16
        camera-restart
        $1B state-flag-clear
        $18 state-flag-clear
        $17 state-flag-clear
        0 self-scripted
        $F $41 fade
    then
    0 self-scripted
    self-idle-or-end
;

: room04.act06 ( -- )   \ 003F14C0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $26 story-flag? if
        1 message
        wait-message
    else
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        $17 state-flag-set
        0 $BE 30.0 -44.0 180 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        5 message
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                1 1 room04.cmd00
                1 ebit? not if
                    0 6 30.0 -90.0 -51.0 0 0 sound
                then
            else
                1 2 room04.cmd00
                1 ebit? 1 2 pvar? and if
                    1 6 30.0 -90.0 -51.0 0 0 sound
                    1 room04.cmd03
                then
            then
            yield
        repeat
        5 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            ['] room04.act01 goto
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room04.act07 ( -- )   \ 0047A990
    self-wait-done
    begin
        yield
    again
;

: room04.act08 ( -- )   \ 003F1590
    self-wait-done
    -73.5 55.5 self-turn-to-xz
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
            $289 story-flag-set
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

: room04.act09 ( -- )   \ 003F15F0
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    begin
        cutscene-near-end? not while
        0 room04.cmd02
        yield
    repeat
    self-idle-or-end
;

: room04.act0A ( -- )   \ 003F1610
    self-wait-done
    2 1 $14 door-bits
    2 1 object-show
    3 1 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 9 action
    $1E $C8 movie-param
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
    1 room04.cmd02
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room04.act0B ( -- )   \ 003F16B0
    self-wait-done
    $FF 1 char-visible
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
    \ (nop-progress-18: no effect in this game)
    $10 $FF movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room04.enter $04 0 room-script!
' room04.char-enter $04 6 room-script!
' room04.phase1 $04 1 room-script!
' room04.phase2 $04 2 room-script!
' room04.act00 $04 $00 action-script!
' room04.act01 $04 $01 action-script!
' room04.act02 $04 $02 action-script!
' room04.act03 $04 $03 action-script!
' room04.act04 $04 $04 action-script!
' room04.act05 $04 $05 action-script!
' room04.act06 $04 $06 action-script!
' room04.act07 $04 $07 action-script!
' room04.act08 $04 $08 action-script!
' room04.act09 $04 $09 action-script!
' room04.act0A $04 $0A action-script!
' room04.act0B $04 $0B action-script!

\ ---- room $05 ----------------------------------------------------------------------------------

: room05.enter ( -- )   \ 003F1800
    $5B story-flag? $5C story-flag? not and if
        room-sounds
        7 state-flag-set
        $21 state-flag-set
        $23 state-flag-set
        1 0 $1000010 nav-group
        $FE char-activate
        $FE 5 354 2 stalker-to-room
        0 $FE 0 char-model-op
        $FE 1 char-silent
        0 $FE 5 action
    then
    1 $2300 sound-volume
;

: room05.char-enter ( -- )   \ 003F1840
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    0 self-is? if
        $1D story-flag? not if
            $FF char-find-tri
            $80 exit-taken? if
                hewie-controlled? not if
                    0 2 2 char-camera
                    0 camera-follow
                else
                    1 2 2 char-camera
                    1 camera-follow
                then
            else hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then then
            0 0 0 action
        then
    then
;

: room05.phase1 ( -- )   \ 003F1930
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $5B story-flag? $5C story-flag? not and if
        0 -4.53 -46.0 -47.5 $1E 15 0 zone
        0 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        0 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        0 -4.53 -46.0 -47.5 $1E 15 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? $FE char-here? and if
        $FE $80000000 6 char-sound
    then
;

: room05.phase2 ( -- )   \ 003F1A30
    $5B story-flag? $5C story-flag? not and if
        0 3 $A 0 $2D char-touching-facing? if
            $61 story-flag? not if
                5 1 0 scene-change
            else
                5 2 0 scene-change
            then
        then
    then
;

: room05.phase5 ( -- )   \ 003F1A50
    7 state-flag-clear
    $5B story-flag? $5C story-flag? not and if
        1 $FE 0 char-model-op
        $FE 0 char-silent
        $FE action-end
        $FE char-done
        0 0 char-in-area? if
            $21 state-flag-clear
            $23 state-flag-clear
        then
    then
;

: room05.act00 ( -- )   \ 003F1A80
    yield
    camera-restart
    \ (nop-progress-14: no effect in this game)
    self-frames-reset
    self-wait-16
    8 state-flag-clear
    $F $41 fade
    $1D story-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room05.act01 ( -- )   \ 003F1AA0
    1 self-scripted
    $13 state-flag-set
    0 counter-set
    $FE action-end
    self-frames-reset
    1 self-wait-frames
    0 $FE 4 action
    1 wait-counter
    0 self-scripted
    $13 state-flag-clear
    self-idle-or-end
;

: room05.act02 ( -- )   \ 003F1AC0
    1 self-scripted
    $13 state-flag-set
    1 message
    wait-message
    0 self-scripted
    $13 state-flag-clear
    self-idle-or-end
;

: room05.act03 ( -- )   \ 0047A998
    begin
        $8001 self-anim
        self-wait-anim
    again
;

: room05.act04 ( -- )   \ 003F1AE0
    1 self-noclip
    1 self-scripted
    self-wait-done
    $8004 $14 self-anim-blend
    self-wait-anim
    $8003 7 self-anim-blend
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 0 6 char-sound
    0 message
    wait-message
    self-wait-anim
    $8005 7 self-anim-blend
    self-wait-anim
    $8001 $14 self-anim-blend
    self-wait-anim
    $61 story-flag-set
    1 counter-set
    ['] room05.act03 goto
;

: room05.act05 ( -- )   \ 003F1B20
    1 self-noclip
    1 self-scripted
    self-wait-done
    $FE char-file-use
    $FE $162 -4.85 -47.03 180 char-to-xz
    $FE 0 0 char-camera
    ['] room05.act03 goto
;

' room05.enter $05 0 room-script!
' room05.char-enter $05 6 room-script!
' room05.phase1 $05 1 room-script!
' room05.phase2 $05 2 room-script!
' room05.phase5 $05 5 room-script!
' room05.act00 $05 $00 action-script!
' room05.act01 $05 $01 action-script!
' room05.act02 $05 $02 action-script!
' room05.act03 $05 $03 action-script!
' room05.act04 $05 $04 action-script!
' room05.act05 $05 $05 action-script!

\ ---- room $06 ----------------------------------------------------------------------------------

\ room 0x06 (Room06_Cmd00_ptmf): its four objects (the name's 6th letter counting) to their
\ places
: room06.cmd00 ( -- )  stub-step ;

: room06.enter ( -- )   \ 003F1B60
    room-sounds
    $1D story-flag? not if
        1 exit-taken? if
            0 $584 $DF8 obstacle-save-at
            1 $65C $ED0 obstacle-save-at
            2 $76A $FDE obstacle-save-at
            3 $36E $BE2 obstacle-save-at
        else
            0 0 obstacle-model-back
            1 1 obstacle-model-back
            2 2 obstacle-model-back
            3 3 obstacle-model-back
        then
    else
        room06.cmd00
    then
    $5B story-flag? $5C story-flag? not and if
        7 state-flag-set
        $21 state-flag-set
        $23 state-flag-set
        $FE char-activate
        $FE 1 char-silent
        $FE 6 236 2 stalker-to-room
        0 $FE 0 char-model-op
        0 $FE 2 action
    then
    1 $2300 sound-volume
;

: room06.char-enter ( -- )   \ 003F1BD0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    2 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    3 0 -1 area-camera
;

: room06.phase1 ( -- )   \ 003F1CD0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    8 1 -1 1 chars-area-camera
    9 0 -1 1 chars-area-camera
    $E 1 -1 1 chars-area-camera
    $F 2 -1 1 chars-area-camera
    $10 1 -1 1 chars-area-camera
    $11 2 -1 1 chars-area-camera
    0 4 char-entered-area? 0 $C char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? 0 $B char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? 0 $A char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? 0 $D char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
;

: room06.phase2 ( -- )   \ 003F1D58
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
;

: room06.phase3 ( -- )   \ 003F1D70
    0.0 -2.8 -39.0 -83.0 -2.8 -39.0 0.0 -2.8 -60.0 -83.0 -2.8 -60.0 lights-doorway
    40.0 -2.8 -39.0 0.0 -2.8 -39.0 40.0 -2.8 -60.0 0.0 -2.8 -60.0 lights-doorway
    83.0 -2.8 -39.0 40.0 -2.8 -39.0 83.0 -2.8 -60.0 40.0 -2.8 -60.0 lights-doorway
    0.0 -2.8 39.0 40.0 -2.8 39.0 0.0 -2.8 60.0 40.0 -2.8 60.0 lights-doorway
    40.0 -2.8 39.0 83.0 -2.8 39.0 40.0 -2.8 60.0 83.0 -2.8 60.0 lights-doorway
    -83.0 -2.8 39.0 0.0 -2.8 39.0 -83.0 -2.8 60.0 0.0 -2.8 60.0 lights-doorway
    30.0 -2.0 -57.0 -20.0 -2.0 -57.0 30.0 -2.0 -80.0 -20.0 -2.0 -80.0 lights-doorway
    38.7 19.3 -60.1 20.2 19.3 -60.1 38.7 -4.0 -60.1 20.2 -4.0 -60.1 lights-doorway
;

: room06.phase5 ( -- )   \ 003F1F00
    7 state-flag-clear
    $5B story-flag? $5C story-flag? not and if
        1 $FE 0 char-model-op
        $FE 0 char-silent
        $FE action-end
        $FE char-done
        0 0 char-in-area? 0 1 char-in-area? or if
            $21 state-flag-clear
            $23 state-flag-clear
        then
    then
;

: room06.act00 ( -- )   \ 003F1F30
    self-wait-done
    0 self-through-exit
    self-wait-done
    0 ebit? not 2 game-mode? or $FF panic-stage? or if
        $FF panic-stage? 2 game-mode? or if
            $60B self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $609 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            1 message
            wait-message
        then
        0 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $12E 0.0 -76.5 180 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        self-frames-reset
        4 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $12E -1.25 -75.3 180 char-to-xz
        $17 state-flag-clear
        0 self-scripted
    then
    $258 item-give
    self-idle-or-end
;

: room06.act01 ( -- )   \ 003F1FD0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $34 self-through-door
    self-wait-done
    $41 story-flag? not if
        $F $44 fade
        $12 state-flag-set
        3 partner-load
        2 char-unload
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
        1 action-end
        1 char-done
        \ (nop-progress-18: no effect in this game)
        0 1 $14 door-bits
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
        wait-fade
        8 state-flag-set
        3 action-end
        3 char-done
        $80 exit-check
        0 0 $14 door-bits
    else
        $609 self-anim
        self-frames-reset
        $14 self-wait-frames
        0 0 6 char-sound
        6 message-param-room
        6 item-use
        $34 door-lock
        20 self-move-16
        self-frames-reset
        $14 self-wait-frames
        $8019 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room06.act02 ( -- )   \ 003F20B0
    1 self-noclip
    1 self-scripted
    $FE $129 -4.85 -47.03 180 char-to-xz
    $FE -1 -1 char-camera
    self-wait-done
    $FE char-file-use
    begin
        $8001 self-anim
        self-wait-anim
    again
;

: room06.act03 ( -- )   \ 003F20D0
    self-wait-done
    3 partner-load
    2 char-unload
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
    8 state-flag-set
    3 action-end
    3 char-done
    0 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room06.enter $06 0 room-script!
' room06.char-enter $06 6 room-script!
' room06.phase1 $06 1 room-script!
' room06.phase2 $06 2 room-script!
' room06.phase3 $06 3 room-script!
' room06.phase5 $06 5 room-script!
' room06.act00 $06 $00 action-script!
' room06.act01 $06 $01 action-script!
' room06.act02 $06 $02 action-script!
' room06.act03 $06 $03 action-script!

\ ---- room $07 ----------------------------------------------------------------------------------

: room07.enter ( -- )   \ 003F21E0
    room-sounds
    0 0 var-set
    1 0 var-set
;

: room07.char-enter ( -- )   \ 003F21F0
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
    0 self-is? if
        1 exit-taken? if
            2 map-page
        then
    then
    0 -11.28 26.25 0.0 0 effect-86
    1 0.0 35.2 11.3 0 effect-86
    2 11.3 59.2 0.0 0 effect-86
    3 0.0 83.2 -11.3 0 effect-86
    4 -11.28 105.2 0.0 0 effect-86
    1 $3FFF sound-volume
;

: room07.phase1 ( -- )   \ 003F22D0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        1 map-page
    then
    0 3 char-left-area? if
        2 map-page
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    4 4 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -34.0 102.0 100.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -34.0 102.0 100.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -34.0 102.0 100.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -34.0 102.0 100.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: room07.phase2 ( -- )   \ 003F24B0
    0 6 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 7 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
;

: room07.act00 ( -- )   \ 003F24D0
    self-wait-done
    35.0 10.0 self-turn-to-xz
    self-wait-done
    $1D01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

' room07.enter $07 0 room-script!
' room07.char-enter $07 6 room-script!
' room07.phase1 $07 1 room-script!
' room07.phase2 $07 2 room-script!
' room07.act00 $07 $00 action-script!

\ ---- room $08 ----------------------------------------------------------------------------------

\ room 0x08 (Room08_Cmd00_ptmf): the cutscene director's +0x6C 3 (byte 3 0) or 2
: room08.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x08 (D_003F31D8): the hanging object named by the handler's string 0xA - byte 3 0 sets
\ it still (+0x30 / +0x38 0, travel +0x3C 0.9); 1: the player's travel (+0x3C, its last move's
\ length) past 5 makes it creak (sounds 4 / 5 by turns, event bit 7) and swing for 20 frames:
\ its tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 a frame
: room08.cmd01 ( b0 -- )  drop stub-step ;
\ door be16 cmd[3..4]: Progress_DoorOpen
: room08.cond00? ( b0 b1 -- flag )  drop drop stub-flag ;
\ room 0x08 (Room08_Cond01_ptmf): the stalker is there but not about
: room08.cond01? ( -- flag )  stub-flag ;

: room08.enter ( -- )   \ 003F2500
    0 $35 room08.cond00? if
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $E story-flag? if
        $2D story-flag? not if
            $2D story-flag-set
            \ (nop-progress-74: no effect in this game)
        then
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        9 1 object-show
        room-sounds
        $1A 1.0 0 bgm
    else
        4 0 object-show
        5 0 object-show
        6 0 object-show
        7 0 object-show
        8 0 object-show
        9 0 object-show
    then
    7 story-flag? if
        $38 story-flag? not if
            $38 story-flag-set
            \ (nop-progress-74: no effect in this game)
            \ (nop-progress-74: no effect in this game)
        then
    then
    $31B story-flag? if
        room08.cond01? 0 state-flag? and if
            $FE char-activate
            $FE 8 376 2 stalker-to-room
            $FE 4 -1 char-camera
            $FE char-full-health
            $FE 0 stalker-mode
            $FE 0 stalker-search-delay
            $FE $178 120 char-to-tri-facing
            stalker-item-cooldown
        then
        $31B story-flag-clear
    then
    $250 story-flag? $251 story-flag? not and if
        0 -254.5 1.0 -95.0 flicker-sprite
    then
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    0 room08.cmd01
    $294 story-flag? not if
        1 -283.73 1.0 2.22 flicker-sprite
    then
;

: room08.act09 ( -- )   \ 003F2E60
    $FE camera-follow
    4 ebit-clear
    $B 0 pvar? if
        0 chance? if
            4 ebit-set
        then
    else $B 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $B 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $B 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    2 creature-action? if
        4 ebit-set
    then
    4 stalker-alert? if
        4 ebit-clear
    then
    4 ebit? if
        0 $FE $A action
    else
        $78 1 item-cooldown
    then
    $B pvar-inc
    exit
;

: room08.char-enter ( -- )   \ 003F2640
    hewie-controlled? not if
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
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then then
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
        9 state-flag? if
            5 ebit-set
            room08.act09
        else
            5 ebit-clear
        then
    then
;

: room08.phase1 ( -- )   \ 003F2780
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 1 1 1 chars-area-camera
    $F 0 0 1 chars-area-camera
    $14 0 0 1 chars-area-camera
    $15 3 3 1 chars-area-camera
    $16 0 0 1 chars-area-camera
    $17 2 2 1 chars-area-camera
    $18 0 0 1 chars-area-camera
    $19 4 -1 1 chars-area-camera
    $1A 0 0 1 chars-area-camera
    $1B 4 -1 1 chars-area-camera
    $1C 0 0 1 chars-area-camera
    $1D 4 -1 1 chars-area-camera
    0 6 char-entered-area? 0 $A char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? 0 8 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $B char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 -277.0 0.0 -142.0 6 10 0 zone
    0 0 8 char-zone-bits? if
        1 room08.cmd01
    then
    $250 story-flag? not if
        1 -254.5 0.0 -95.0 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 592 var-set
                $1A 593 var-set
                $1B 0 var-set
                $1C -254500 var-set
                $1D 1000 var-set
                $1E -95000 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
;

: room08.phase2 ( -- )   \ 003F28A0
    $294 story-flag? not if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $D 4 scene-change
        then
    then
    0 $13 char-in-area? if
        $FE char-here? not if
            5 7 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 $10 $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 $11 char-in-area? 0 45 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 $12 char-in-area? 0 45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $250 story-flag? $251 story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $C 4 scene-change
        then
    then
;

: room08.phase3 ( -- )   \ 003F2910
    -219.9 59.3 20.8 -249.2 59.3 -7.4 -194.2 59.3 -5.9 -223.5 59.3 -34.2 lights-doorway
    -180.0 20.0 -119.9 -200.0 20.0 -119.9 -180.0 0.0 -119.9 -200.0 0.0 -119.9 lights-doorway
;

: room08.phase5 ( -- )   \ 003F2980
    $FE char-busy? 6 ebit? and if
        $FE $1C0 -275.75 41.09 -60 char-to-xz
        $FE 0 0 char-camera
        6 ebit-clear
    then
;

: room08.act00 ( -- )   \ 003F29A0
    $E story-flag? not if
        1 self-scripted
        1 char-here? if
            0 1 4 action-force
        then
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
        1 1 char-visible
        1 room08.cmd00
        $1E $C8 movie-param
        3 char-unload
        4 3 char-hand-over
        room-sounds
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
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        9 1 object-show
        0 $1D0 -80 char-to-tri-facing
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
        camera-restart
        begin
            6 sound-bank-loaded? not while
            yield
        repeat
        $FE 8 383 2 stalker-to-room
        $FE -1 -1 char-camera
        $FE char-activate
        stalker-item-cooldown
        0 $FE 5 action
        1 char-here? if
            $2F 0 -1 hewie-to-room
        then
        $B01 0 self-anim-blend
        3 0 char-remove
        4 0 char-remove
        $1A 1.0 0 bgm
        $F $41 fade
        $B02 self-anim
        self-wait-anim
        $E story-flag-set
        $2D story-flag-set
        \ (nop-progress-74: no effect in this game)
        wait-fade
        $21 resident-flag-set
        $227 item-give
        0 self-scripted
    else
        self-wait-done
        camera-restart
        $F 1 fade
        wait-fade
        $12 state-flag-clear
    then
    $18 state-flag-clear
    0 self-scripted
    \ (nop-progress-14: no effect in this game)
    self-idle-or-end
;

: room08.act01 ( -- )   \ 003F2AE0
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room08.act02 ( -- )   \ 003F2B00
    1 ebit? not if
        2 message
        wait-message
        $902 self-anim
        self-wait-anim
        $903 self-anim
        self-wait-anim
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: room08.act03 ( -- )   \ 003F2B20
    2 ebit? not if
        2 message
        wait-message
        $902 self-anim
        self-wait-anim
        $903 self-anim
        self-wait-anim
        3 message
        wait-message
        2 ebit-set
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: room08.act04 ( -- )   \ 0047A9AC
    begin
        yield
    again
;

: room08.act05 ( -- )   \ 003F2B40
    6 ebit-set
    self-wait-done
    $FE $B char-file-load
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    yield
    $FE -219.01 60.17 14.42 -60 char-to-xyz
    1 self-noclip
    1 self-scripted
    $FE 0 char-visible
    $FE 0 char-silent
    0 7 -147.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -155.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -163.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -171.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -179.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -187.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -195.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -203.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    0 7 -211.0 60.0 14.5 0 0 sound
    self-frames-reset
    $A self-wait-frames
    1 7 -219.0 60.0 14.5 0 0 sound
    $8000 5 self-anim-9
    0 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        0 60 var? if
            0 -283.0 1.0 37.0 0 0 0 0 dust
            0 -280.0 1.0 42.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        0 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $FE 0 0 char-camera
    $FE char-full-health
    $FE 2 stalker-mode
    $FE 0 stalker-search-delay
    stalker-item-cooldown
    6 ebit-clear
    self-idle-or-end
;

: room08.act06 ( -- )   \ 003F2CC0
    yield
    camera-restart
    self-wait-done
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
    $1E $C8 movie-param
    \ (nop-progress-18: no effect in this game)
    3 char-unload
    4 3 char-hand-over
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
    $FE action-end
    $FE char-done
    8 state-flag-set
    3 0 char-remove
    4 0 char-remove
    $20 resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: room08.act08 ( -- )   \ 003F2E20
    3 ebit? if
        2 avoid-prompt
    then
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    9 state-flag-clear
    0 camera-follow
    $18 state-flag-set
    $114 -258.97 -140.95 79 $207 5 self-move-to
    self-wait-done
    4 $14 self-anim-blend
    self-wait-anim
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    self-idle-or-end
;

: room08.act07 ( -- )   \ 003F2D60
    $18 state-flag-set
    5 ebit-clear
    1 self-scripted
    3 ebit-clear
    self-wait-done
    $1D4 -267.0 -140.0 -110 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    $800 self-anim
    self-wait-anim
    $122 -291.573 -146.706 -110 $803 5 self-move-to
    self-wait-done
    $122 -291.573 -146.706 80 $803 5 self-move-to
    self-wait-done
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room08.act08 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? $FE char-here? not and if
                    5 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            9 state-flag-clear
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] room08.act08 goto
            then
            $18 state-flag-set
            $1D4 -267.0 -140.0 70 $803 5 self-move-to
            self-wait-done
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            self-idle-or-end
        then
    again
;

: room08.act0A ( -- )   \ 003F2EB0
    self-wait-done
    $1A1 -238.675 -139.193 -90 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    3 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room08.act0B ( -- )   \ 0047A9B0
    self-idle-or-end
;

: room08.act0C ( -- )   \ 003F2EE0
    self-wait-done
    -254.5 -95.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message
            wait-message
        else
            $251 story-flag-set
            0 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
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

: room08.act0D ( -- )   \ 003F2F40
    self-wait-done
    -283.73 2.22 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $294 story-flag-set
            1 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: room08.act0E ( -- )   \ 003F2FA0
    self-wait-done
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    $E 3 $FF char-load
    $F 4 char-load-2
    2 5 $FF char-load
    3 char-unload
    4 3 char-hand-over
    5 char-unload
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
    $1E $C8 movie-param
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
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room08.act0F ( -- )   \ 003F3090
    self-wait-done
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    $E 3 $FF char-load
    $F 4 char-load-2
    2 5 $FF char-load
    3 char-unload
    4 3 char-hand-over
    5 char-unload
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
    $1E $C8 movie-param
    1 room08.cmd00
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
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room08.enter $08 0 room-script!
' room08.char-enter $08 6 room-script!
' room08.phase1 $08 1 room-script!
' room08.phase2 $08 2 room-script!
' room08.phase3 $08 3 room-script!
' room08.phase5 $08 5 room-script!
' room08.act00 $08 $00 action-script!
' room08.act01 $08 $01 action-script!
' room08.act02 $08 $02 action-script!
' room08.act03 $08 $03 action-script!
' room08.act04 $08 $04 action-script!
' room08.act05 $08 $05 action-script!
' room08.act06 $08 $06 action-script!
' room08.act07 $08 $07 action-script!
' room08.act08 $08 $08 action-script!
' room08.act09 $08 $09 action-script!
' room08.act0A $08 $0A action-script!
' room08.act0B $08 $0B action-script!
' room08.act0C $08 $0C action-script!
' room08.act0D $08 $0D action-script!
' room08.act0E $08 $0E action-script!
' room08.act0F $08 $0F action-script!

\ ---- room $09 ----------------------------------------------------------------------------------

\ room 0x09 (Room09_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in
\ another room than 9 (the player about too)
: room09.cond00? ( -- flag )  stub-flag ;

: room09.enter ( -- )   \ 003F3240
    $3B story-flag? not if
        4 1.0 0 bgm
    then
    0 4 $1000000 nav-group
    $301 story-flag? not if
        1 0 $800088 nav-group
        1 1 $40 nav-group
    else
        2 1 object-show
        0 0 $800088 nav-group
        0 1 $40 nav-group
        0 1 $14 door-bits
        1 2 8 nav-group
        1 3 $40 nav-group
    then
    $223 story-flag? not if
        0 -6.0 11.5 -46.0 flicker-sprite
    then
    $3C story-flag? not if
        3 1 $14 door-bits
    then
    $3B story-flag? not if
        1 1 $14 door-bits
    else
        2 1 $14 door-bits
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 $2300 sound-volume
;

: room09.char-enter ( -- )   \ 003F32D0
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
        $301 story-flag? not if
            $3D story-flag? not if
                2 game-mode? $FF panic-stage? not and if
                    room09.cond00? if
                        $FE char-here? not if
                            0 0 $C action
                        then
                    then
                then
            then
        then
    then
;

: room09.phase1 ( -- )   \ 003F3330
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 1 1 1 chars-area-camera
    $301 story-flag? not if
        0 8 char-in-area? if
            2 game-mode? $FF panic-stage? not and if
                room09.cond00? if
                    0 0 0 action
                then
            then
        then
    then
    3 13.13 0.0 -30.63 $1C 11 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    4 ebit-set
                    $32 chance? if
                        $1F 3 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    3 13.13 0.0 -30.63 $1C 11 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    2 26.99 0.0 27.94 $18 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
;

: room09.phase2 ( -- )   \ 003F3400
    -2147483644 scene-request? if
        0 0 6 action
    else $301 story-flag? not if
        0 9 char-in-area? 0 90 $2D char-heading? and if
            5 7 0 scene-change
        then
        0 8 char-in-area? 0 0 $2D char-heading? and if
            5 8 0 scene-change
        then
    then then
    0 $A char-in-area? 0 -67 $5A char-heading? and if
        5 $A 0 scene-change
    then
    0 $B char-in-area? 0 45 $2D char-heading? and if
        5 $B 0 scene-change
    then
    0 $C char-in-area? 0 -45 $2D char-heading? and if
        5 $F 0 scene-change
    then
    $223 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
    $3C story-flag? not if
        1 10.0 6.0 -17.0 5 5 0 zone
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: room09.phase3 ( -- )   \ 003F34A0
    -0.9 11.8 -26.5 -2.9 11.7 -22.0 0.2 -1.0 -26.0 -1.9 -1.0 -22.0 lights-doorway
;

: room09.phase5 ( -- )   \ 003F34D8
    2 ebit? if
        $18 state-flag-clear
        $1F state-flag-clear
    then
;

: room09.act00 ( -- )   \ 003F34F0
    2 ebit? if
        $18 state-flag-clear
        $F1 action-end
    then
    1 self-scripted
    $301 story-flag-set
    \ (nop-progress-24: no effect in this game)
    $FE 9 103 2 stalker-to-room
    $FE $67 180 char-to-tri-facing
    $FE 1 char-visible
    $FE action-end
    $FE char-done
    $F $54 fade
    2 story-flag? not if
        6 4 movie-play
        5 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
    else
        1 4 movie-play
        0 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
    then
    wait-fade
    0 $F9 3 action
    1 char-here? if
        0 1 2 action-force
        $303 story-flag-set
    then
    0 1 char-no-shadow
    $FE 1 char-no-shadow
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
    1 char-here? if
        1 action-end
    then
    camera-restart
    $FE 9 143 2 stalker-to-room
    $FE $8F -34.144 33.682 90 char-to-xz
    $FE 0 0 char-camera
    $FE char-activate
    $FE stalker-knock-down
    2 story-flag? not if
        $18 resident-flag-set
    else
        $19 resident-flag-set
    then
    0 self-scripted
    8 state-flag-set
    0 0 char-no-shadow
    $FE 0 char-no-shadow
    $E door-open-clear
    \ (nop-progress-14: no effect in this game)
    0 exit-check
    \ (nop-progress-24: no effect in this game)
    $301 story-flag-set
    $302 story-flag-clear
    self-idle-or-end
;

: room09.act01 ( -- )   \ 003F3618
    room09.cond00? not if
        0 message
        wait-message
    else
        ['] room09.act00 goto
    then
    self-idle-or-end
;

: room09.act02 ( -- )   \ 003F3630
    self-wait-done
    $D 4.0 13.0 0 $FFFF $A self-move-to
    self-wait-done
    1 4 $1000000 nav-group
    self-idle-or-end
;

: room09.act03 ( -- )   \ 003F3650
    begin
        4 0 cutscene-passed? if
            0 exit-door-open? not if
                $E door-open-set
                doors-room-in
                $FE $28 5 char-sound
            then
        then
        4 1 cutscene-passed? if
            $FE $13 7 char-sound
        then
        4 2 cutscene-passed? if
            1 char-here? if
                0 1 4 action-force
            then
        then
        4 3 cutscene-passed? not while
        yield
    repeat
    self-idle-or-end
;

: room09.act04 ( -- )   \ 003F3690
    self-wait-done
    $A1 6.0 33.0 0 $FFFF $A self-move-to
    self-wait-done
    self-idle-or-end
;

: room09.act05 ( -- )   \ 003F36B0
    3 ebit-set
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $3B story-flag? not if
        $F $44 fade
        4 4 movie-play
        3 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
        wait-fade
        0 1 char-no-shadow
        0 $F2 $E action
        $FF panic-stage? if
            3 panic-stage
        then
        7 message-prepare
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
        $FF 1.0 0 bgm
        wait-fade
        0 0 char-no-shadow
        $F2 action-end
        1 0 $14 door-bits
        2 1 $14 door-bits
        2 0 object-show
        0 self-move-16
        0 $13C 23.771 -27.607 -73 char-to-xz
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        camera-restart
        $3B story-flag-set
        $F $41 fade
        wait-fade
    else
        10.0 -17.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $3C story-flag-set
        3 0 $14 door-bits
        $14 message-param-room
        $14 1 item-give-count
        0 $14 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $903 self-anim
        self-wait-anim
    then
    2 ebit? not if
        $18 state-flag-clear
    then
    0 self-scripted
    3 ebit-clear
    self-idle-or-end
;

: room09.act06 ( -- )   \ 003F37E0
    begin
        0 char-busy? not while
        yield
    repeat
    1 message
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: room09.act07 ( -- )   \ 0047A9B8
    0 message
    wait-message
    self-idle-or-end
;

: room09.act08 ( -- )   \ 003F3800
    self-wait-done
    $18 state-flag-set
    $17 state-flag-set
    1 self-scripted
    0 $DA -26.45 -13.04 0 char-to-xz
    1 20.0 -18.0 8.0 0.0 event-camera
    0 message
    wait-message
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    $18 state-flag-clear
    self-idle-or-end
;

: room09.act09 ( -- )   \ 003F3850
    self-wait-done
    -6.0 -46.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $223 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room09.act0A ( -- )   \ 003F38B0
    self-wait-done
    0 ebit? not if
        2 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: room09.act0B ( -- )   \ 003F38D0
    self-wait-done
    $77 24.585 26.685 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 60.0 -45.0 30.0 event-camera
    self-frames-reset
    $10 self-wait-frames
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    1 ebit? not if
        4 message
        wait-message
        1 ebit-set
    else
        5 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room09.act0C ( -- )   \ 003F3930
    $18 state-flag-set
    $1F state-flag-set
    1 self-scripted
    $3D story-flag-set
    2 ebit-set
    self-wait-done
    1 $A self-anim-blend
    6 message
    self-wait-anim
    10 self-move-16
    wait-message
    0 self-scripted
    0 $F1 $D action
    self-idle-or-end
;

: room09.act0D ( -- )   \ 003F3960
    self-frames-reset
    $F0 self-wait-frames
    begin
        3 ebit? while
        yield
    repeat
    $18 state-flag-clear
    $1F state-flag-clear
    2 ebit-clear
    self-idle-or-end
;

: room09.act0E ( -- )   \ 003F3980
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    $FF 1.0 0 bgm
    begin
        4 cutscene-shot? not while
        yield
    repeat
    1 0 $14 door-bits
    2 1 $14 door-bits
    self-idle-or-end
;

: room09.act0F ( -- )   \ 003F39A0
    $17 state-flag-set
    1 self-scripted
    0 $13B 20.181 -28.363 -90 char-to-xz
    1 10.0 20.0 -30.0 0.0 event-camera
    $3B story-flag? not if
        8 message
        wait-message
    else 5 ebit? not if
        9 message
        wait-message
        5 ebit-set
    else
        $A message
        wait-message
    then then
    self-frames-reset
    $10 self-wait-frames
    $17 state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room09.act10 ( -- )   \ 003F3A00
    self-wait-done
    4 1.0 0 bgm
    3 1 $14 door-bits
    1 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    4 4 movie-play
    3 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 char-no-shadow
    0 $F2 $E action
    $FF panic-stage? if
        3 panic-stage
    then
    7 message-prepare
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
    $FF 1.0 0 bgm
    0 0 char-no-shadow
    $F2 action-end
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room09.act11 ( -- )   \ 003F3AB0
    self-wait-done
    2 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    2 3 $FF char-load
    3 char-unload
    6 4 movie-play
    5 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 3 action
    0 1 char-no-shadow
    $FE 1 char-no-shadow
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room09.act12 ( -- )   \ 003F3B60
    self-wait-done
    2 1 $14 door-bits
    2 3 $FF char-load
    3 char-unload
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 3 action
    0 1 char-no-shadow
    $FE 1 char-no-shadow
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room09.enter $09 0 room-script!
' room09.char-enter $09 6 room-script!
' room09.phase1 $09 1 room-script!
' room09.phase2 $09 2 room-script!
' room09.phase3 $09 3 room-script!
' room09.phase5 $09 5 room-script!
' room09.act00 $09 $00 action-script!
' room09.act01 $09 $01 action-script!
' room09.act02 $09 $02 action-script!
' room09.act03 $09 $03 action-script!
' room09.act04 $09 $04 action-script!
' room09.act05 $09 $05 action-script!
' room09.act06 $09 $06 action-script!
' room09.act07 $09 $07 action-script!
' room09.act08 $09 $08 action-script!
' room09.act09 $09 $09 action-script!
' room09.act0A $09 $0A action-script!
' room09.act0B $09 $0B action-script!
' room09.act0C $09 $0C action-script!
' room09.act0D $09 $0D action-script!
' room09.act0E $09 $0E action-script!
' room09.act0F $09 $0F action-script!
' room09.act10 $09 $10 action-script!
' room09.act11 $09 $11 action-script!
' room09.act12 $09 $12 action-script!

\ ---- room $0A ----------------------------------------------------------------------------------

\ the dial pstr_syuukouki on progress var 3
: room0A.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x0A (Room0A_Cmd01_ptmf): an effect on its object at -2.88
: room0A.cmd01 ( -- )  stub-step ;

: room0A.enter ( -- )   \ 003F3C90
    $26 story-flag? not if
        room-sounds
    then
    $A story-flag? if
        $2F story-flag? not if
            $2F story-flag-set
            \ (nop-progress-74: no effect in this game)
        then
    then
    $24C story-flag? $24D story-flag? not and if
        0 39.5 81.0 -38.8 flicker-sprite
    then
    $282 story-flag? not if
        1 41.5 69.0 31.0 flicker-sprite
    then
    0 room0A.cmd00
    1 $2300 sound-volume
;

: room0A.char-enter ( -- )   \ 003F3CF0
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
        $80 exit-taken? $81 exit-taken? or if
            0 $EB -41.573 -3.787 50 char-to-xz
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            $80 exit-taken? if
                0 0 2 action
            else
                0 0 3 action
            then
        then
    then
;

: room0A.phase1 ( -- )   \ 003F3DA0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $24C story-flag? not if
        0 39.5 80.0 -38.8 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 588 var-set
                $1A 589 var-set
                $1B 0 var-set
                $1C 39500 var-set
                $1D 81000 var-set
                $1E -38800 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    1 -35.43 90.0 3.42 $10 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 16.78 80.0 -16.27 $17 8 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 8.0 hewie-look-zone
            then
        then
    then
;

: room0A.phase2 ( -- )   \ 003F3E90
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    0 6 $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 7 $32 char-faces-area? if
        5 5 0 scene-change
    then
    0 8 char-in-area? 0 90 $32 char-heading? and if
        5 6 0 scene-change
    then
    $24C story-flag? $24D story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 4 4 scene-change
        then
    then
    $282 story-flag? not if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then
;

: room0A.phase3 ( -- )   \ 003F3EF0
    -3.0 80.0 3.0 -10.0 80.0 -10.0 -2.1 65.0 3.0 -10.0 65.0 -10.0 lights-doorway
    10.0 80.0 10.0 -3.0 80.0 3.0 10.0 65.0 10.0 -3.0 65.0 3.0 lights-doorway
    50.0 100.0 10.0 10.0 100.0 10.0 50.0 40.0 10.0 10.0 40.0 10.0 lights-doorway
    -10.0 80.0 -10.0 -10.0 80.0 -30.0 -10.0 30.0 -10.0 -10.0 30.0 -30.0 lights-doorway
    -26.0 120.0 8.0 -26.0 120.0 13.0 -26.0 90.0 8.0 -26.0 90.0 13.0 lights-doorway
;

: room0A.act00 ( -- )   \ 003F3FF0
    self-wait-done
    $FE self-touching? not if
        $15 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $15 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $15 door-lock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: room0A.act02 ( -- )   \ 003F4110
    1 self-scripted
    self-wait-done
    $80 exit-taken? if
        $FE char-here? if
            0 $FE 7 action
            yield
        then
        $17 state-flag-set
    then
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
    \ (nop-progress-18: no effect in this game)
    wait-fade
    $17 state-flag-set
    $10 $FF movie-param
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
    $17 state-flag-clear
    3 ebit? if
        $80 exit-check
    else
        $81 exit-check
    then
    0 self-scripted
    self-idle-or-end
;

: room0A.act01 ( -- )   \ 003F4030
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $26 story-flag? if
        1 message
        wait-message
    else
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        $17 state-flag-set
        0 $E6 -43.61 1.45 62 char-to-xz
        1 11.0 0.0 0.0 0.0 event-camera
        2 message
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                1 room0A.cmd00
                1 ebit? not if
                    0 6 -36.0 -102.0 5.0 0 0 sound
                then
            else
                2 room0A.cmd00
                1 ebit? 3 3 pvar? and if
                    1 6 -36.0 -102.0 5.0 0 0 sound
                    room0A.cmd01
                then
            then
            yield
        repeat
        0 $EB -42.2 -4.35 52 char-to-xz
        2 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        $17 state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            3 ebit-set
            $1B state-flag-set
            $F $44 fade
            ['] room0A.act02 goto
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room0A.act03 ( -- )   \ 003F41C0
    1 self-scripted
    self-wait-done
    $22 door-lock
    $26 story-flag-set
    5 0 state-flag-16
    $1B state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    camera-restart
    \ (nop-progress-14: no effect in this game)
    $F $41 fade
    self-idle-or-end
;

: room0A.act04 ( -- )   \ 003F41E0
    self-wait-done
    39.5 -38.8 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $24D story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: room0A.act05 ( -- )   \ 003F4240
    self-wait-done
    18.0 -16.0 self-turn-to-xz
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: room0A.act06 ( -- )   \ 003F4250
    self-wait-done
    180 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room0A.act07 ( -- )   \ 0047A9C0
    self-wait-done
    begin
        yield
    again
;

: room0A.act08 ( -- )   \ 003F4260
    self-wait-done
    41.5 31.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $282 story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: room0A.act09 ( -- )   \ 003F42C0
    self-wait-done
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
    \ (nop-progress-18: no effect in this game)
    $10 $FF movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

' room0A.enter $0A 0 room-script!
' room0A.char-enter $0A 6 room-script!
' room0A.phase1 $0A 1 room-script!
' room0A.phase2 $0A 2 room-script!
' room0A.phase3 $0A 3 room-script!
' room0A.act00 $0A $00 action-script!
' room0A.act01 $0A $01 action-script!
' room0A.act02 $0A $02 action-script!
' room0A.act03 $0A $03 action-script!
' room0A.act04 $0A $04 action-script!
' room0A.act05 $0A $05 action-script!
' room0A.act06 $0A $06 action-script!
' room0A.act07 $0A $07 action-script!
' room0A.act08 $0A $08 action-script!
' room0A.act09 $0A $09 action-script!

\ ---- room $0B ----------------------------------------------------------------------------------

\ room 0x0B (Room0B_Cond00_ptmf): the player is 20 .. 120 from (x, z) = s16 bytes 3..4, 5..6
: room0B.cond00? ( b0 b1 b2 b3 -- flag )  drop drop drop drop stub-flag ;

: room0B.enter ( -- )   \ 003F43C0
    room-sounds
    1 0 $1000000 nav-group
    1 0 $10000000 nav-group
    0 0 char-file-load
    1 $2300 sound-volume
;

: room0B.char-enter ( -- )   \ 003F43E0
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
    1 self-is? if
        $317 story-flag? not 1 0 char-heading-for? and if
            0 1 5 action
        then
    then
;

: room0B.phase1 ( -- )   \ 003F4470
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    1 char-here? if
        35 fiona-started? 1 2 char-C4? not and if
            0 -80 0 $2D char-faces-xz? $FF $B0 0 0 room0B.cond00? and if
                hewie-stays? if
                    0 1 1 action
                then
            then
            0 10 0 $2D char-faces-xz? 0 $A 0 0 room0B.cond00? and if
                hewie-stays? if
                    0 1 2 action
                then
            then
            0 110 0 $2D char-faces-xz? 0 $6E 0 0 room0B.cond00? and if
                hewie-stays? if
                    0 1 3 action
                then
            then
        then
    then
    0 0 char-in-nav-group? if
        0 0 0 action
    then
;

: room0B.act00 ( -- )   \ 003F4500
    $2C state-flag-set
    1 self-scripted
    begin
        0 char-busy? not while
        yield
    repeat
    0 char-file-use
    0 $F2 6 action
    $8000 self-anim
    self-wait-anim
    self-frames-reset
    $3C self-wait-frames
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    0 1 6 char-sound
    self-frames-reset
    self-wait-16
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room0B.act01 ( -- )   \ 003F4540
    self-wait-done
    hewie-bark
    self-wait-done
    $A7 $FFFF 6 self-move-tri
    self-wait-done
    0 self-turn-to
    self-wait-done
    0 $F1 4 action
    self-idle-or-end
;

: room0B.act02 ( -- )   \ 003F4560
    self-wait-done
    hewie-bark
    self-wait-done
    $AB $FFFF 6 self-move-tri
    self-wait-done
    0 self-turn-to
    self-wait-done
    0 $F1 4 action
    self-idle-or-end
;

: room0B.act03 ( -- )   \ 003F4580
    self-wait-done
    hewie-bark
    self-wait-done
    $9B $FFFF 6 self-move-tri
    self-wait-done
    0 self-turn-to
    self-wait-done
    0 $F1 4 action
    self-idle-or-end
;

: room0B.act04 ( -- )   \ 003F45A0
    begin
        1 char-busy? while
        yield
    repeat
    $1D $29 hewie-action
    self-idle-or-end
;

: room0B.act05 ( -- )   \ 003F45C0
    self-wait-done
    $A4 -55.0 0.0 90 $FFFF $A self-move-to
    self-wait-done
    4 self-anim
    self-frames-reset
    $3C self-wait-frames
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    $1B03 self-anim
    self-wait-anim
    0 self-look-at
    yield
    $317 story-flag-set
    4 self-anim
    self-frames-reset
    $28 self-wait-frames
    $FF self-look-at
    yield
    self-idle-or-end
;

: room0B.act06 ( -- )   \ 003F4608
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    0 0 6 char-sound
    self-idle-or-end
;

' room0B.enter $0B 0 room-script!
' room0B.char-enter $0B 6 room-script!
' room0B.phase1 $0B 1 room-script!
' room0B.act00 $0B $00 action-script!
' room0B.act01 $0B $01 action-script!
' room0B.act02 $0B $02 action-script!
' room0B.act03 $0B $03 action-script!
' room0B.act04 $0B $04 action-script!
' room0B.act05 $0B $05 action-script!
' room0B.act06 $0B $06 action-script!

\ ---- room $0C ----------------------------------------------------------------------------------

\ room 0x0C (Room0C_Cmd01_ptmf): script variable byte 3 down by the player's hit (byte 4: 1 from
\ the weak blow 0x1A, else 5) or the pursuer's (+0x108), not below 0
: room0C.cmd01 ( b0 b1 -- )  drop drop stub-step ;
\ room 0x0C (Room0C_Cmd02_ptmf): the screen darkened as the cutscene runs past frame 0x4AE (32 a
\ frame, up to 0x80)
: room0C.cmd02 ( -- )  stub-step ;
\ room 0x0C (Room0C_Cmd03_ptmf): three objects (pair byte 4) swing: byte 3 0 set up (rest +0x30,
\ phase +0x34 half a turn apart, swing +0x3C 0.75 / 0.5), 1 a step (phase on 60 degrees, the
\ swing down 0.1, x = rest + swing x sin(phase); 2 once still), 2 all to x 10. 2 while any
\ swings
: room0C.cmd03 ( b0 b1 -- )  drop drop stub-step ;

: room0C.enter ( -- )   \ 003F4650
    room-sounds
    $15 1.0 0 bgm
    $23 story-flag? not if
        0 1 $14 door-bits
    then
    1 1 $14 door-bits
    6 1 object-show
    $35 story-flag? not if
        0 25 var-set
    else
        $D 1 object-show
        5 1 $14 door-bits
    then
    $36 story-flag? not if
        1 25 var-set
    else
        $E 1 object-show
        3 1 $14 door-bits
    then
    $34 story-flag? not if
        9 0 object-show
        $A 1 object-show
        $16 story-flag? if
            7 1 $14 door-bits
            1 0 8 nav-group
        then
    else
        9 1 object-show
        $A 0 object-show
        1 0 8 nav-group
    then
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    1 $2300 sound-volume
;

: room0C.char-enter ( -- )   \ 003F46E0
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
;

: room0C.phase1 ( -- )   \ 003F4720
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    3 1 0 1 chars-area-camera
    4 2 1 1 chars-area-camera
    8 0 -1 1 chars-area-camera
    9 1 0 1 chars-area-camera
    $A 0 -1 1 chars-area-camera
    $B 1 0 1 chars-area-camera
    $23 story-flag? $15 story-flag? not and $16 story-flag? not and 0 7 char-entered-area? and 2 ebit? not and if
        2 ebit-set
        $FF panic-stage? if
            3 panic-stage
        then
        0 0 char-action? if
            0 0 0 action-force
        else
            1 0 0 action-force
        then
    then
    $35 story-flag? not if
        0 10.0 0.0 -37.0 7 20 0 zone
        $FE 0 char-in-zone? 0 0 char-in-zone? or if
            0 $F1 8 action
        then
    then
    $36 story-flag? not if
        1 10.0 0.0 37.0 7 20 0 zone
        $FE 1 char-in-zone? 0 1 char-in-zone? or if
            0 $F2 9 action
        then
    then
    $15 story-flag? $16 story-flag? not and if
        $FE 2 char-C4? if
            $28 door-lock
            $16 story-flag-set
            $1B state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
        $35 story-flag? $36 story-flag? and if
            $28 door-lock
            $16 story-flag-set
            $1B state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 2 action-force
            else
                1 0 2 action-force
            then
        then
    then
    2 166.49 6.0 0.6 $24 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: room0C.phase2 ( -- )   \ 003F4850
    $10 state-flag? if
        -2147483646 scene-request? if
            5 3 1 scene-change
        then
    then
    0 5 char-in-area? 0 45 $32 char-heading? and if
        5 4 0 scene-change
    then
    $15 story-flag? not $16 story-flag? or if
        0 $C char-in-area? 0 10 -40 $32 char-faces-xz? and if
            5 6 0 scene-change
        then
        0 $D char-in-area? 0 10 40 $32 char-faces-xz? and if
            5 6 0 scene-change
        then
    then
    $34 story-flag? if
        0 $E $32 char-faces-area? if
            5 5 0 scene-change
        then
    else $16 story-flag? if
        0 $E $32 char-faces-area? if
            5 $B 0 scene-change
        then
    then then
;

: room0C.act00 ( -- )   \ 003F48C0
    $18 state-flag-set
    1 self-scripted
    $1B state-flag-set
    self-wait-done
    -1 self-move-16
    $FF 1.0 0 bgm
    $F $44 fade
    2 0 char-remove
    $1B partner-load
    0 state-flag-clear
    2 char-unload
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
    1 0 $14 door-bits
    1 char-activate
    $C 0 209 hewie-to-room
    $FE $C 206 2 stalker-to-room
    $FE 0 -1 char-camera
    $FE char-activate
    $FE 1 char-no-shadow
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
    $FE 0 char-no-shadow
    6 story-flag-set
    $11 door-lock
    1 1 $14 door-bits
    6 1 object-show
    $28 door-open-clear
    $28 door-unlock
    doors-room-in
    $10 state-flag-set
    0 3 -90 char-to-tri-facing
    1 char-activate
    $C 0 209 hewie-to-room
    1 $D1 -90 char-to-tri-facing
    $FE $C 206 2 stalker-to-room
    $FE char-activate
    $FE 2 stalker-mode
    $FE 0 stalker-search-delay
    $FE $CE 90 char-to-tri-facing
    $FE char-full-health
    $15 story-flag-set
    hewie-controlled? not if
        0 2 1 char-camera
        0 camera-follow
    else
        1 2 1 char-camera
        1 camera-follow
    then
    camera-restart
    8 state-flag-clear
    $1B state-flag-clear
    1 1.0 0 bgm
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room0C.act01 ( -- )   \ 003F4A00
    $FF 1.0 0 bgm
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F 4 fade
    $C 0 movie-play
    $B cutscene-start
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
    $10 state-flag-clear
    2 0 char-remove
    1 char-activate
    50 hewie-trust
    0 $11C 56.239 -3.659 -45 char-to-xz
    $C 0 78 hewie-to-room
    1 $4E 57.799 0.951 0 char-to-xz
    7 1 $14 door-bits
    1 0 8 nav-group
    hewie-controlled? not if
        0 1 0 char-camera
        0 camera-follow
    else
        1 1 0 char-camera
        1 camera-follow
    then
    camera-restart
    $28 door-open-clear
    $28 door-lock
    doors-room-in
    $15 1.0 0 bgm
    $1B state-flag-clear
    $F $41 fade
    wait-fade
    $28 resident-flag-set
    $22F item-give
    $8297 item-give
    builtin.act9C
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room0C.act02 ( -- )   \ 003F4B10
    $FF 1.0 0 bgm
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $13 state-flag-set
    $E state-flag-set
    $F 4 fade
    8 0 movie-play
    7 cutscene-start
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
    0 $F9 7 action
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
    $26 resident-flag-set
    8 state-flag-set
    9 1 object-show
    $F 0 object-show
    $10 0 object-show
    $11 0 object-show
    $12 0 object-show
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    self-frames-reset
    self-wait-16
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
    0 $F9 $A action
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    6 0 $14 door-bits
    $10 state-flag-clear
    $13 state-flag-clear
    $E state-flag-clear
    50 hewie-trust
    2 0 char-remove
    1 char-activate
    0 $11C 56.239 -3.659 -45 char-to-xz
    $C 0 78 hewie-to-room
    1 $4E 57.799 0.951 0 char-to-xz
    hewie-controlled? not if
        0 1 0 char-camera
        0 camera-follow
    else
        1 1 0 char-camera
        1 camera-follow
    then
    camera-restart
    $28 door-open-clear
    $28 door-lock
    doors-room-in
    $A 0 object-show
    1 0 8 nav-group
    $34 story-flag-set
    0 panic-stage
    $15 1.0 0 bgm
    $1B state-flag-clear
    $F $41 fade
    wait-fade
    $20B item-give
    $8296 item-give
    builtin.act9C
    $27 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room0C.act03 ( -- )   \ 0047A9CC
    $B message
    self-idle-or-end
;

: room0C.act04 ( -- )   \ 003F4CD0
    $18 state-flag-set
    self-wait-done
    160.0 0.0 self-turn-to-xz
    self-wait-done
    $A01 $A self-anim-blend
    self-wait-anim
    6 message
    wait-message
    $F 6 fade
    wait-fade
    7 message
    wait-message
    $F 7 fade
    wait-fade
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $23 story-flag? not if
        8 message
        wait-message
        self-wait-anim
        1 self-scripted
        0 answer? if
            $FF 1.0 0 bgm
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
            1 action-end
            1 char-done
            $16 state-flag-clear
            1 creatures-clear
            $24 story-flag-set
            $25 story-flag-set
            $2C story-flag-set
            $2D story-flag-set
            $2E story-flag-set
            $2F story-flag-set
            $38 story-flag-set
            $28 $C8 movie-param
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
            0 self-move-16
            0 $CD -90 char-to-tri-facing
            camera-restart
            1 char-activate
            $C 0 401 hewie-to-room
            1 $191 0 char-to-tri-facing
            1 char-full-health
            1 0 char-set-C4
            $28 door-open-set
            doors-room-in
            0 0 $14 door-bits
            $23 story-flag-set
            $15 1.0 0 bgm
            $F $41 fade
            wait-fade
            6 message-param-room
            6 1 item-give-count
            0 6 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
            30 hewie-trust
        then
        0 self-scripted
    else
        $A message
        wait-message
        self-wait-anim
    then
    $18 state-flag-clear
    self-idle-or-end
;

: room0C.act05 ( -- )   \ 003F4E30
    1 ebit? not if
        $F message
        wait-message
        1 ebit-set
    else
        $A01 $A self-anim-blend
        $10 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: room0C.act06 ( -- )   \ 003F4E50
    self-wait-done
    $34 story-flag? if
        $E message
        wait-message
    else 0 ebit? not if
        $C message
        wait-message
        0 ebit-set
    else
        $D message
        wait-message
        0 ebit-clear
    then then
    self-idle-or-end
;

: room0C.act07 ( -- )   \ 003F4E70
    begin
        3 cutscene-shot? not while
        yield
    repeat
    begin
        4 cutscene-shot? not while
        46.0 46.0 116.5 116.5 depth-range
        yield
    repeat
    begin
        $4AE cutscene-cue-reached? not while
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        room0C.cmd02
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: room0C.act08 ( -- )   \ 003F4EB0
    $15 story-flag? $16 story-flag? not and if
        $FE 0 char-in-zone? if
            0 0 room0C.cmd01
        else
            0 1 room0C.cmd01
        then
    then
    0 0 var? if
        10.0 10.0 -35.0 0 -2144325584 3 -9.0 scene-effect-8C
        $D 1 object-show
        5 1 $14 door-bits
        $35 story-flag-set
        1 6 10.0 10.0 -40.0 0 0 sound
    else
        0 6 10.0 10.0 -40.0 0 0 sound
    then
    0 0 room0C.cmd03
    1 0 room0C.cmd03
    self-frames-reset
    $14 self-wait-frames
    self-idle-or-end
;

: room0C.act09 ( -- )   \ 003F4F30
    $15 story-flag? $16 story-flag? not and if
        $FE 1 char-in-zone? if
            1 0 room0C.cmd01
        else
            1 1 room0C.cmd01
        then
    then
    1 0 var? if
        10.0 10.0 35.0 0 -2144325584 3 -9.0 scene-effect-8C
        $E 1 object-show
        3 1 $14 door-bits
        $36 story-flag-set
        1 6 10.0 10.0 40.0 0 0 sound
    else
        0 6 10.0 10.0 40.0 0 0 sound
    then
    0 1 room0C.cmd03
    1 1 room0C.cmd03
    self-frames-reset
    $14 self-wait-frames
    self-idle-or-end
;

: room0C.act0A ( -- )   \ 003F4FB0
    begin
        5 cutscene-shot? if
            6 1 $14 door-bits
        then
        6 cutscene-shot? if
            6 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room0C.act0B ( -- )   \ 003F4FC8
    self-wait-done
    $1D02 $A self-anim-blend
    $11 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room0C.act0C ( -- )   \ 003F4FE0
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    6 1 object-show
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
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
    $28 $C8 movie-param
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
    0 0 $14 door-bits
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room0C.act0D ( -- )   \ 003F50A0
    self-wait-done
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    -1 self-move-16
    $1B partner-load
    2 char-unload
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
    1 0 $14 door-bits
    1 char-activate
    $C 0 209 hewie-to-room
    $FE $C 206 2 stalker-to-room
    $FE char-activate
    $FE 1 char-no-shadow
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
    $FE 0 char-no-shadow
    2 0 char-remove
    1 1 $14 door-bits
    6 1 object-show
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room0C.act0E ( -- )   \ 003F5170
    self-wait-done
    $D 1 object-show
    5 1 $14 door-bits
    $E 1 object-show
    3 1 $14 door-bits
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-unload
    8 0 movie-play
    7 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 7 action
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room0C.act0F ( -- )   \ 003F5230
    self-wait-done
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-unload
    9 1 object-show
    $F 0 object-show
    $10 0 object-show
    $11 0 object-show
    $12 0 object-show
    1 0 char-visible
    1 action-end
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    self-frames-reset
    self-wait-16
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
    0 $F9 $A action
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    6 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room0C.act10 ( -- )   \ 003F5300
    self-wait-done
    9 0 object-show
    $A 1 object-show
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-unload
    $C 0 movie-play
    $B cutscene-start
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
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room0C.enter $0C 0 room-script!
' room0C.char-enter $0C 6 room-script!
' room0C.phase1 $0C 1 room-script!
' room0C.phase2 $0C 2 room-script!
' room0C.act00 $0C $00 action-script!
' room0C.act01 $0C $01 action-script!
' room0C.act02 $0C $02 action-script!
' room0C.act03 $0C $03 action-script!
' room0C.act04 $0C $04 action-script!
' room0C.act05 $0C $05 action-script!
' room0C.act06 $0C $06 action-script!
' room0C.act07 $0C $07 action-script!
' room0C.act08 $0C $08 action-script!
' room0C.act09 $0C $09 action-script!
' room0C.act0A $0C $0A action-script!
' room0C.act0B $0C $0B action-script!
' room0C.act0C $0C $0C action-script!
' room0C.act0D $0C $0D action-script!
' room0C.act0E $0C $0E action-script!
' room0C.act0F $0C $0F action-script!
' room0C.act10 $0C $10 action-script!

\ ---- room $0E ----------------------------------------------------------------------------------

: room0E.enter ( -- )   \ 003F5540
    $25A story-flag? $25B story-flag? not and if
        1 -70.0 61.0 54.1 flicker-sprite
    then
    $21F story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $220 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $22B story-flag? not if
            0 -68.0 1.0 -58.0 flicker-sprite
        then
    then
    $221 story-flag? not if
        1 2 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
        $28A story-flag? not if
            2 -66.0 1.0 55.0 flicker-sprite
        then
    then
    1 $2300 sound-volume
    $AF story-flag? if
        $3F story-flag? not if
            $3F story-flag-set
            \ (nop-progress-74: no effect in this game)
        then
    then
;

: room0E.char-enter ( -- )   \ 003F5610
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    0 self-is? if
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
        0 exit-taken? if
            3 map-page
        then
        $80 exit-taken? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        then
    then
    0 4 0.0 0.687 0.187 0.312 zone-rect
;

: room0E.phase1 ( -- )   \ 003F5710
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 1 1 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 4 4 1 chars-area-camera
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $D char-entered-area? if
        3 map-page
    then
    0 $D char-left-area? if
        2 map-page
    then
    $25A story-flag? not if
        3 -70.0 60.0 54.1 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 602 var-set
                $1A 603 var-set
                $1B 1 var-set
                $1C -70000 var-set
                $1D 61000 var-set
                $1E 54100 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: room0E.phase2 ( -- )   \ 003F57E0
    $21F story-flag? not if
        0 -61.0 0.0 -52.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $21F story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -61.0 0.0 -52.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -61.0 0.0 -52.0 0 0 sound
            $40 $55 noise
        then
    then
    $220 story-flag? not if
        1 -68.0 0.0 -58.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $220 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -68.0 0.0 -58.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -68.0 0.0 -58.0 0 0 sound
            $40 $2BF noise
            0 -68.0 1.0 -58.0 flicker-sprite
        then
    else $22B story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
    $221 story-flag? not if
        2 -66.0 0.0 55.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $221 story-flag-set
            0 2 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -66.0 0.0 55.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -66.0 0.0 55.0 0 0 sound
            $40 $295 noise
            2 -66.0 1.0 55.0 flicker-sprite
        then
    else $28A story-flag? not if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    $25A story-flag? $25B story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: room0E.phase3 ( -- )   \ 003F5980
    30.0 54.1 -51.5 -45.0 54.1 -51.5 30.0 54.1 -90.0 -45.0 54.1 -90.0 lights-doorway
    70.0 54.1 -23.0 57.0 54.1 -56.0 100.0 54.1 -30.0 80.0 54.1 -65.0 lights-doorway
    77.0 54.1 18.0 70.0 54.1 -23.0 100.0 54.1 30.0 100.0 54.1 -30.0 lights-doorway
    52.0 54.1 48.0 77.0 54.1 18.0 82.0 54.1 65.0 100.0 54.1 30.0 lights-doorway
    65.0 20.0 18.0 75.0 20.0 13.0 65.0 0.0 18.0 75.0 0.0 13.0 lights-doorway
    74.5 20.0 -19.5 68.5 20.0 -24.5 74.5 0.0 -19.5 68.5 0.0 -24.5 lights-doorway
    53.0 20.0 -48.0 53.0 20.0 -60.0 53.0 0.0 -48.0 53.0 0.0 -60.0 lights-doorway
    -55.0 20.0 -62.0 -55.0 20.0 -47.0 -55.0 0.0 -62.0 -55.0 0.0 -47.0 lights-doorway
    -70.0 20.0 -26.0 -78.0 20.0 -22.0 -70.0 0.0 -26.0 -78.0 0.0 -22.0 lights-doorway
    -78.0 20.0 20.0 -69.0 20.0 25.0 -78.0 0.0 20.0 -69.0 0.0 25.0 lights-doorway
    -55.0 20.0 47.0 -55.0 20.0 54.0 -55.0 0.0 47.0 -55.0 0.0 54.0 lights-doorway
    52.0 20.0 57.0 52.0 20.0 47.0 52.0 0.0 57.0 52.0 0.0 47.0 lights-doorway
;

: room0E.act00 ( -- )   \ 003F5BD0
    self-wait-done
    $FE self-touching? not if
        0 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 0 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    0 door-lock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: room0E.act01 ( -- )   \ 003F5C10
    self-wait-done
    -68.0 -58.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $22B story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: room0E.act02 ( -- )   \ 003F5C70
    self-wait-done
    -70.0 54.1 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $25B story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: room0E.act03 ( -- )   \ 003F5CD0
    self-wait-done
    -66.0 55.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message
            wait-message
        else
            $28A story-flag-set
            2 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
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

: room0E.act04 ( -- )   \ 003F5D30
    3 resident-flag? not if
        $19 state-flag-set
    then
    8 state-flag-set
    self-wait-done
    0 1 char-visible
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    $B 3 $FF char-load
    $10 4 $FF char-load
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
    3 char-unload
    4 char-unload
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
    $2D 2 pvar-set
    2 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room0E.act05 ( -- )   \ 003F5DE0
    self-wait-done
    0 1 char-visible
    $B 3 $FF char-load
    $10 4 $FF char-load
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
    \ (nop-progress-18: no effect in this game)
    3 char-unload
    4 char-unload
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

' room0E.enter $0E 0 room-script!
' room0E.char-enter $0E 6 room-script!
' room0E.phase1 $0E 1 room-script!
' room0E.phase2 $0E 2 room-script!
' room0E.phase3 $0E 3 room-script!
' room0E.act00 $0E $00 action-script!
' room0E.act01 $0E $01 action-script!
' room0E.act02 $0E $02 action-script!
' room0E.act03 $0E $03 action-script!
' room0E.act04 $0E $04 action-script!
' room0E.act05 $0E $05 action-script!

\ ---- room $0F ----------------------------------------------------------------------------------

\ room 0x0F: the floor plate ("fumi_yuka") fading: its +0x24 toward 1 while event variable 0 is
\ unset, toward 0 once set; byte 3 0 at once, else by 0.2 a step (var_fade).
: room0F.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x0F (Room0F_Cmd01_ptmf): three objects' +0x14 back to 0
: room0F.cmd01 ( -- )  stub-step ;
\ room 0x0F (Room0F_Cmd02_ptmf): an effect (DriftingFlecks_vtable, 0x840 bytes) with its box
: room0F.cmd02 ( -- )  stub-step ;
\ room 0x0F (Room0F_Cmd03_ptmf): the player's model +0xBC (1, 0.25) or (0, 0) by byte 3
: room0F.cmd03 ( b0 -- )  drop stub-step ;
\ room 0x0F (Room0F_Cmd04_ptmf): the player's model +0xD0 (0, 1.5, -2 / -1.5 by byte 3) and
\ +0xCC
: room0F.cmd04 ( b0 -- )  drop stub-step ;

: room0F.enter ( -- )   \ 003F5EC0
    room-sounds
    $11 story-flag? if
        0 0 $14 door-bits
        2 0 object-show
        3 0 object-show
        4 0 object-show
    else
        0 1 $14 door-bits
        2 0 object-show
        3 0 object-show
        4 0 object-show
    then
    7 door-unlocked? if
        1 0 $800008 nav-group
    then
    $80 exit-taken? if
        $13 story-flag-set
        0 1 var-set
        1 24 var-set
    else
        3 exit-taken? if
        else
            $310 story-flag-clear
            $13 story-flag-clear
        then
        $310 story-flag? if
            $13 story-flag-set
            0 1 var-set
            1 12 var-set
        else $13 story-flag? not if
            0 0 var-set
            1 0 var-set
        else
            0 1 var-set
            1 24 var-set
        then then
    then
    0 room0F.cmd00
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 22.8 14.65 68.75 0 effect-86
    $319 story-flag? not if
        1 1 $14 door-bits
        3 ebit-clear
    else
        2 1 $14 door-bits
        $239 story-flag? not if
            1 140.0 -9.0 94.0 flicker-sprite
        then
        3 ebit-set
    then
    $31F story-flag? not if
        4 1 $14 door-bits
    else
        4 0 $14 door-bits
    then
    8 door-unlocked? if
        5 0 $14 door-bits
    else
        5 1 $14 door-bits
    then
    3 0 var-set
    4 0 var-set
    1 $2300 sound-volume
;

: room0F.char-enter ( -- )   \ 003F5FC0
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
    0 self-is? if
        1 story-flag? 2 story-flag? not and if
            0.0 sound-volume-scale
            8 state-flag-set
            0 0 0 action
            3 0 $14 door-bits
            7 0 object-show
            8 0 object-show
        else
            3 1 $14 door-bits
            7 1 object-show
            8 1 object-show
        then
        $80 exit-taken? if
            0 $221 -90 char-to-tri-facing
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
            $17 state-flag-clear
            $311 story-flag-set
            0 0 $B action
        then
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
    then
;

: room0F.phase1 ( -- )   \ 003F6120
    6 sound-bank-loaded? if
        $11 story-flag? not if
            $C0000003 6 -72.0 10.0 15.0 0 0 sound
        else
            $C0000001 6 -72.0 10.0 15.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    3 story-flag? $31 story-flag? not and if
        1 exit-usable? if
            3 3 1 char-load
            1 exit-check
        then
    else 1 exit-usable? if
        1 exit-check
    then then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    3 story-flag? $31 story-flag? not and if
        0 6 char-entered-area? 0 9 char-left-area? or if
            3 0 char-remove
        then
    then
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-left-area? 0 6 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-left-area? 0 8 char-entered-area? or if
        3 story-flag? $31 story-flag? not and if
            3 3 1 char-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-left-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 0 var? if
        1 $D char-in-area? if
            $13 story-flag-set
            0 1 var-set
            1 0 6 char-sound
            0 $F1 $D action
        else 0 $D char-in-area? if
            $13 story-flag-set
            0 1 var-set
            0 0 6 char-sound
            $11 story-flag? $311 story-flag? not and $14 story-flag? not and if
                0 0 $A action
            else
                0 $F1 $D action
            then
        else $FE $D char-in-area? if
            $13 story-flag-set
            0 1 var-set
            $FE 0 6 char-sound
            0 $F1 $D action
        then then then
    else 1 $D char-in-area? not 0 $D char-in-area? not and $FE $D char-in-area? not and if
        $13 story-flag-clear
        0 0 var-set
        0 $F2 $D action
    then then
    1 room0F.cmd00
    $11 story-flag? not if
        0 -28.0 0.0 15.0 $A 15 0 zone
        0 0 char-in-zone? if
            0 $F5 $1C action
        then
    then
    2 140.0 -10.0 94.0 6 5 0 zone
    $319 story-flag? not if
        6 ebit? not if
            0 2 char-in-zone? if
                2 2 var? if
                    0 $F4 $1B action
                else
                    0 $F3 8 action
                then
            then
        then
    then
    6 sound-bank-loaded? if
        4 var-inc
        4 30 var? if
            4 0 var-set
            3 0 var? if
                $40000005 6 -17.0 22.0 -9.0 0 0 sound
            else 3 1 var? if
                $40000006 6 -17.0 22.0 -9.0 0 0 sound
            else 3 2 var? if
                $40000007 6 -17.0 22.0 -9.0 0 0 sound
            else 3 3 var? if
                $40000008 6 -17.0 22.0 -9.0 0 0 sound
            then then then then
            3 var-inc
            3 4 var? if
                3 0 var-set
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    $319 story-flag? not if
        3 140.0 -10.0 94.0 $1E 20 0 zone
        4 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        4 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    1 char-here? 1 0 char-C4? and if
        5 ebit? not if
            5 -38.5 0.0 -65.0 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    1 5 9 char-zone-bits? if
                        5 ebit-set
                        $80 0 hewie-action
                    then
                then
            then
        else
            5 -38.5 0.0 -65.0 7 5 0 zone
            1 5 9 char-zone-bits? not if
                5 ebit-clear
            then
        then
    then
;

: room0F.phase2 ( -- )   \ 003F6420
    0 $A char-in-area? if
        -2147483644 scene-request? if
            0 0 6 action
        else $11 story-flag? not 0 -45 $2D char-heading? and if
            5 2 0 scene-change
        then then
    then
    0 $E char-in-area? 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    0 $10 char-in-area? 0 0 $2D char-heading? and if
        5 $E 0 scene-change
    then
    0 $D char-in-area? if
        5 4 0 scene-change
    then
    0 $F char-in-area? 0 -45 $32 char-heading? and if
        5 $84 3 scene-change
    then
    3 ebit? $239 story-flag? not and if
        $239 story-flag? not if
            2 1 5 5 0 zone-at-effect
            0 2 3 char-zone-bits? if
                5 $10 4 scene-change
            then
        then
    else 0 2 2 char-zone-bits? 0 45 $32 char-heading? and if
        5 9 0 scene-change
    then then
    -2147483646 scene-request? if
        0 2 char-group-bit4? if
            5 $13 1 scene-change
        then
    then
    0 $11 char-in-area? 0 -45 $32 char-heading? and if
        5 $15 0 scene-change
    then
    $31F story-flag? not if
        4 131.392 -30.0 147.223 5 5 0 zone
        0 4 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
;

: room0F.act00 ( -- )   \ 003F64F0
    $FE $F 443 2 stalker-to-room
    $FE $1BB 0 char-to-tri-facing
    $FE action-end
    $FE char-done
    3 char-unload
    4 action-end
    4 char-done
    6 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $11 action
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
    1 room0F.cmd03
    $12 state-flag-clear
    0 0 char-silent
    \ (nop-progress-18: no effect in this game)
    fiona-calm-reset
    8 state-flag-set
    $FE action-end
    $FE char-done
    3 0 char-remove
    $80 exit-check
    self-idle-or-end
;

: room0F.act01 ( -- )   \ 003F65A0
    0 counter-set
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    0 item-use
    7 door-lock
    $11 story-flag-set
    0 $F0 5 action
    1 wait-counter
    0 $1DD -4.507 15.509 -90 char-to-xz
    0 self-move-16
    camera-restart
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room0F.act02 ( -- )   \ 003F65E0
    self-wait-done
    $1FD -16.0 14.0 -90 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 20.0 -10.0 0.0 0.0 event-camera
    0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $10 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    $228 item-give
    self-idle-or-end
;

: room0F.act03 ( -- )   \ 003F6640
    self-wait-done
    2 ebit? not if
        $16 message
        wait-message
        $C1 0.0 -84.0 0 $FFFF 5 self-move-to
        self-wait-done
        $18 message
        wait-message
        2 ebit-set
    else
        $17 message
        wait-message
    then
    self-idle-or-end
;

: room0F.act04 ( -- )   \ 003F6670
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    self-wait-16
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    $15 message
    wait-message
    self-idle-or-end
;

: room0F.act05 ( -- )   \ 003F6690
    $F $54 fade
    5 0 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    0 0 $14 door-bits
    2 0 object-show
    3 0 object-show
    4 0 object-show
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    0 $F9 $18 action
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
    1 room0F.cmd04
    0 0 $800008 nav-group
    room0F.cmd01
    1 0 char-visible
    1 action-end
    counter-inc
    $F $51 fade
    wait-fade
    self-idle-or-end
;

: room0F.act06 ( -- )   \ 003F6750
    begin
        0 char-busy? not while
        yield
    repeat
    $1203 0 self-anim-blend
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    1 ebit? not if
        $11 message
        1 ebit-set
    else
        $12 message
    then
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: room0F.act07 ( -- )   \ 003F6780
    self-wait-done
    self-frames-reset
    $19 self-wait-frames
    $5A threat-raise
    1 $FF 8 rumble
    0 $43 5 char-sound
    $F00 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    self-idle-or-end
;

: room0F.act08 ( -- )   \ 003F67A0
    1 $FF 4 rumble
    6 ebit-set
    0 9 6 char-sound
    0 139.0 -9.0 91.5 0 0 0 0 dust
    0 139.0 -9.0 94.5 0 0 0 0 dust
    begin
        fiona-free? not while
        yield
    repeat
    2 var-inc
    6 ebit-clear
    self-idle-or-end
;

: room0F.act09 ( -- )   \ 003F67E0
    self-wait-done
    $319 story-flag? not if
        $13 message
        wait-message
    else
        $14 message
        wait-message
    then
    self-idle-or-end
;

: room0F.act0A ( -- )   \ 003F67F0
    $18 state-flag-set
    1 self-scripted
    \ (nop-progress-18: no effect in this game)
    self-wait-done
    $F 0 fade
    wait-fade
    8 state-flag-set
    $12 state-flag-set
    $17 state-flag-set
    5 state-flag-set
    $FF panic-stage? if
        3 panic-stage
    then
    $F state-flag-set
    $80 exit-check
    self-idle-or-end
;

: room0F.act0B ( -- )   \ 003F6820
    1 self-scripted
    self-wait-done
    8 state-flag-clear
    camera-restart
    1 0 self-anim-blend
    $F state-flag-clear
    $F 1 fade
    wait-fade
    1 message
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    $12 state-flag-clear
    5 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room0F.act0C ( -- )   \ 003F6850
    self-wait-done
    1 ebit? not if
        $11 message
        wait-message
        1 ebit-set
    else
        $12 message
        wait-message
    then
    self-idle-or-end
;

: room0F.act0D ( -- )   \ 003F6870
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $310 story-flag-set
    $11 story-flag? not if
        $40000003 6 -72.0 10.0 15.0 0 0 sound
    else
        $40000001 6 -72.0 10.0 15.0 0 0 sound
    then
    $13 story-flag? not if
        begin
            1 0 var? not $13 story-flag? not and while
            1 var-dec
            self-frames-reset
            1 self-wait-frames
        repeat
        $13 story-flag? not if
            $11 story-flag? not if
                3 6 sound-stop
                $40000004 6 -72.0 10.0 15.0 0 0 sound
            else
                1 6 sound-stop
                $40000002 6 -72.0 10.0 15.0 0 0 sound
            then
            $310 story-flag-clear
        then
    else
        begin
            1 24 var? not $13 story-flag? and while
            1 var-inc
            self-frames-reset
            3 self-wait-frames
        repeat
        $13 story-flag? if
            $11 story-flag? not if
                3 6 sound-stop
                $40000004 6 -72.0 10.0 15.0 0 0 sound
            else
                1 6 sound-stop
                $40000002 6 -72.0 10.0 15.0 0 0 sound
            then
            $310 story-flag-clear
        then
    then
    self-idle-or-end
;

: room0F.act0E ( -- )   \ 003F6960
    self-wait-done
    65.0 107.0 self-turn-to-xz
    self-wait-done
    2 ebit? if
        2 message
        wait-message
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: room0F.act0F ( -- )   \ 003F6980
    self-frames-reset
    $29 self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    self-frames-reset
    $2E self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    self-frames-reset
    $25 self-wait-frames
    $4000000B 6 120.0 -10.0 92.0 0 0 sound
    $239 story-flag? not if
        1 140.0 -9.0 94.0 flicker-sprite
    then
    3 ebit-set
    self-idle-or-end
;

: room0F.act10 ( -- )   \ 003F69E0
    self-wait-done
    140.0 94.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $239 story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: room0F.act11 ( -- )   \ 003F6A40
    begin
        $F cutscene-shot? not while
        yield
    repeat
    $FE 1 char-no-shadow
    begin
        $10 cutscene-shot? not while
        yield
    repeat
    $FE 0 char-no-shadow
    begin
        $17 cutscene-shot? if
            0 room0F.cmd03
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room0F.act12 ( -- )   \ 003F6A70
    self-wait-done
    131.392 147.223 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $31F story-flag-set
    4 0 $14 door-bits
    $12 message-param-room
    $12 1 item-give-count
    0 $12 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: room0F.act13 ( -- )   \ 003F6AC0
    self-wait-done
    2 self-through-exit
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60B self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $609 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        6 message
        wait-message
    then
    self-idle-or-end
;

: room0F.act14 ( -- )   \ 003F6AF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    0 $C 6 char-sound
    5 1 $14 door-bits
    self-frames-reset
    $1E self-wait-frames
    0 $72 5 char-sound
    8 door-lock
    $903 self-anim
    130.0 -20.0 170.0 self-look-at-point
    yield
    self-frames-reset
    $3C self-wait-frames
    $FF self-look-at
    yield
    self-wait-anim
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room0F.act15 ( -- )   \ 003F6B50
    self-wait-done
    $A6 120.0 154.0 -90 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 15.0 -10.0 0.0 0.0 event-camera
    8 door-unlocked? if
        4 message
        wait-message
    else
        5 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room0F.act16 ( -- )   \ 003F6BB0
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    7 message
    wait-message
    self-idle-or-end
;

: room0F.act17 ( -- )   \ 003F6BE0
    self-wait-done
    $10F 120.5 153.0 -90 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $902 self-anim
    self-wait-anim
    0 $C 6 char-sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    self-frames-reset
    $1E self-wait-frames
    8 message
    wait-message
    self-idle-or-end
;

: room0F.act18 ( -- )   \ 003F6C10
    0 room0F.cmd04
    begin
        1 cutscene-shot? not while
        yield
    repeat
    1 room0F.cmd04
    self-idle-or-end
;

: room0F.act19 ( -- )   \ 003F6C20
    self-wait-done
    0 1 $14 door-bits
    2 0 object-show
    3 0 object-show
    4 0 object-show
    1 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 22.8 14.65 68.75 0 effect-86
    4 3 $FF char-load
    3 char-unload
    2 partner-load
    2 char-unload
    6 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $11 action
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
    1 room0F.cmd03
    2 0 char-remove
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room0F.act1A ( -- )   \ 003F6CF0
    self-wait-done
    5 0 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 0 $14 door-bits
    2 0 object-show
    3 0 object-show
    4 0 object-show
    0 22.8 14.65 68.75 0 effect-86
    0 $F9 $18 action
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
    1 room0F.cmd04
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room0F.act1B ( -- )   \ 003F6DA0
    1 $FF 4 rumble
    $319 story-flag-set
    0 9 6 char-sound
    $4000000A 6 140.0 -10.0 92.0 0 0 sound
    1 0 $14 door-bits
    2 1 $14 door-bits
    0 139.0 -9.0 91.5 0 0 0 0 dust
    0 139.0 -9.0 94.5 0 0 0 0 dust
    0 2 0.7 0.5 0.3 0.125 zone-rect
    137.0 -10.0 90.0 0 -2145378272 1 0.0 scene-effect-8C
    137.0 -10.0 95.0 0 -2145378272 1 0.0 scene-effect-8C
    room0F.cmd02
    0 $F1 $F action
    begin
        fiona-free? not while
        yield
    repeat
    0 0 7 action
    self-idle-or-end
;

: room0F.act1C ( -- )   \ 003F6E50
    0 $91 5 char-sound
    begin
        fiona-free? not while
        yield
    repeat
    0 0 $C action
    self-idle-or-end
;

' room0F.enter $0F 0 room-script!
' room0F.char-enter $0F 6 room-script!
' room0F.phase1 $0F 1 room-script!
' room0F.phase2 $0F 2 room-script!
' room0F.act00 $0F $00 action-script!
' room0F.act01 $0F $01 action-script!
' room0F.act02 $0F $02 action-script!
' room0F.act03 $0F $03 action-script!
' room0F.act04 $0F $04 action-script!
' room0F.act05 $0F $05 action-script!
' room0F.act06 $0F $06 action-script!
' room0F.act07 $0F $07 action-script!
' room0F.act08 $0F $08 action-script!
' room0F.act09 $0F $09 action-script!
' room0F.act0A $0F $0A action-script!
' room0F.act0B $0F $0B action-script!
' room0F.act0C $0F $0C action-script!
' room0F.act0D $0F $0D action-script!
' room0F.act0E $0F $0E action-script!
' room0F.act0F $0F $0F action-script!
' room0F.act10 $0F $10 action-script!
' room0F.act11 $0F $11 action-script!
' room0F.act12 $0F $12 action-script!
' room0F.act13 $0F $13 action-script!
' room0F.act14 $0F $14 action-script!
' room0F.act15 $0F $15 action-script!
' room0F.act16 $0F $16 action-script!
' room0F.act17 $0F $17 action-script!
' room0F.act18 $0F $18 action-script!
' room0F.act19 $0F $19 action-script!
' room0F.act1A $0F $1A action-script!
' room0F.act1B $0F $1B action-script!
' room0F.act1C $0F $1C action-script!

\ ---- room $10 ----------------------------------------------------------------------------------

\ room 0x10 (Room10_Cond00_ptmf): the stalker is there, not about, in mode 2, 6 or 7
: room10.cond00? ( -- flag )  stub-flag ;

: room10.enter ( -- )   \ 003F6F80
    room-sounds
    $1B story-flag? $308 story-flag? not and if
        1 exit-taken? if
            room10.cond00? if
                $308 story-flag-set
                7 state-flag-set
                $C door-open-clear
                doors-room-in
                0 ebit-set
                $13 state-flag-set
                $FE char-activate
                $FE $10 161 2 stalker-to-room
                $FE -1 -1 char-camera
                $FE char-full-health
                0 $FE 2 action
            then
        then
    then
    $200 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $28B story-flag? not if
            3 41.0 1.0 -19.0 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 6.0 21.8 -3.2 0 effect-86
    1 29.0 41.8 41.9 0 effect-86
    2 -46.8 93.8 18.5 0 effect-86
    5 ebit-clear
    $39 story-flag? $41 story-flag? not and if
        1 action-end
        1 char-done
    then
    1 $2300 sound-volume
;

: room10.char-enter ( -- )   \ 003F7060
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    0 3 -1 area-camera
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 3 char-camera
                0 camera-follow
            else
                1 4 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 3 char-camera
            0 camera-follow
        else
            1 4 3 char-camera
            1 camera-follow
        then
    then then
    2 4 3 area-camera
    0 self-is? if
        1 exit-taken? if
            2 ebit-set
        then
        $39 story-flag? $41 story-flag? not and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 5 action
        then
        0 exit-taken? 2 exit-taken? or if
            3 map-page
        then
    then
    $FE self-is? if
        6 ebit-clear
    then
;

: room10.phase1 ( -- )   \ 003F7150
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $D 0 0 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 4 3 1 chars-area-camera
    $17 3 -1 1 chars-area-camera
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-left-area? 0 5 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-left-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $12 char-entered-area? if
        2 ebit-set
    then
    0 $12 char-left-area? if
        2 ebit-clear
    then
    4 stalker-alert? not 6 ebit? not and $FE 2 char-C4? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 stalker-alert? 2 stalker-alert? or if
                $FE $11 char-in-area? 2 ebit? and if
                    0 $FE 4 action
                    6 ebit-set
                then
            then
        then
    then
    $41 story-flag? $42 story-flag? not and if
        1 2 pad? 2 2 pad? or 3 2 pad? or 0 control-action? or 1 control-action? or 2 control-action? or 3 control-action? or 4 control-action? or fiona-free? and if
            0 0 6 action
        then
        99.0 fiona-fear
    then
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    0 5 char-entered-area? if
        3 map-page
    then
    0 5 char-left-area? if
        2 map-page
    then
    6 sound-bank-loaded? if
        0 $80000000 6 char-sound
    then
;

: room10.phase2 ( -- )   \ 003F72D0
    0 $F char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $10 $2D char-faces-area? if
        5 1 0 scene-change
    then
    0 $13 char-in-area? 0 45 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $14 char-in-area? 0 45 $32 char-heading? and if
        5 8 0 scene-change
    then
    $41 story-flag? $42 story-flag? not and if
        -2147483646 scene-request? 2 scene-request? or if
            5 6 1 scene-change
        then
    then
    $200 story-flag? not if
        0 41.0 0.0 -19.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $200 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            41.0 0.0 -19.0 0 -2146430960 0 0.0 scene-effect-8C
            $88 5 41.0 0.0 -19.0 0 0 sound
            $40 $232 noise
            3 41.0 1.0 -19.0 flicker-sprite
        then
    else $28B story-flag? not if
        0 3 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then then
;

: room10.phase3 ( -- )   \ 003F73B0
    -8.0 82.0 -5.0 -8.0 82.0 45.0 -8.0 30.0 -5.0 -8.0 30.0 45.0 lights-doorway
    -5.3 54.0 -4.1 -5.3 54.0 47.0 -58.0 54.0 -4.1 -58.0 54.0 47.0 lights-doorway
    15.0 30.0 -3.0 45.0 30.0 -3.0 15.0 15.0 -3.0 45.0 15.0 -3.0 lights-doorway
    15.0 15.0 -3.0 45.0 15.0 -3.0 15.0 0.0 -3.0 45.0 0.0 -3.0 lights-doorway
    -11.0 30.0 -3.0 15.0 30.0 -3.0 -11.0 15.0 -3.0 15.0 15.0 -3.0 lights-doorway
    -11.0 15.0 -3.0 15.0 15.0 -3.0 -11.0 0.0 -3.0 15.0 0.0 -3.0 lights-doorway
;

: room10.act03 ( -- )   \ 003F7560
    $13 state-flag-clear
    7 state-flag-clear
    0 ebit-clear
    $FE action-end
    $FE char-done
    exit
;

: room10.phase5 ( -- )   \ 003F74E0
    0 ebit? if
        room10.act03
    then
    5 ebit? $FE char-busy? and if
        $FE action-end
        $FE $196 0 char-to-tri-facing
        $FE 0 char-no-shadow
        5 ebit-clear
    then
;

: room10.act00 ( -- )   \ 003F7500
    self-wait-done
    -177 self-turn-angle
    self-wait-done
    $A02 self-anim
    self-wait-anim
    4 message
    wait-message
    self-idle-or-end
;

: room10.act01 ( -- )   \ 003F7510
    self-wait-done
    1 ebit? not if
        40.0 -11.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        2 message
        wait-message
        1 ebit-set
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: room10.act02 ( -- )   \ 003F7530
    1 self-scripted
    self-wait-done
    1 $FF 8 rumble
    $FE $A1 -90 char-to-tri-facing
    0 exit-door-open? not if
        $1B4 $FFFF $B self-move-tri
        self-wait-done
        0 3 self-move-slot
        self-wait-done
    then
    $3C $FFFF $B self-move-tri
    self-wait-done
    0 self-scripted
    room10.act03
    self-idle-or-end
;

: room10.act04 ( -- )   \ 003F7570
    self-wait-done
    $FE 0 char-file-load
    $8A 31.949 -18.333 0 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    5 ebit-set
    $8000 5 self-anim-9
    0 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        0 60 var? if
            0 29.0 19.0 25.0 0 0 0 0 dust
            0 36.0 19.0 25.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        0 var-inc
        0 camera-mode? not if
            $FE 1 char-no-shadow
        else
            $FE 0 char-no-shadow
        then
        self-at-motion-event? not while
        yield
    repeat
    0 self-noclip
    0 self-scripted
    $FE 0 char-no-shadow
    5 ebit-clear
    $FE 2 stalker-mode
    self-idle-or-end
;

: room10.act05 ( -- )   \ 003F7600
    1 action-end
    1 char-done
    self-wait-done
    3 partner-load
    2 char-unload
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    $FE $6C 1 door-lock-for
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    1 door-reopen-lock
    $D door-reopen-lock
    $58 door-close-off-unlock
    $5A door-close-off-unlock
    $4D door-close-off-unlock
    $4E door-close-off-unlock
    $50 door-close-off-unlock
    $53 door-close-off-unlock
    $5C door-close-off-unlock
    $79 door-close-off-unlock
    exits-rebuild
    camera-restart
    2 door-unlock
    $34 door-unlock
    $62 door-unlock
    $44 door-unlock
    $47 door-unlock
    $4F door-unlock
    $56 door-unlock
    $67 door-unlock
    $7D door-unlock
    $51 door-unlock
    $52 door-unlock
    $64 door-unlock
    $6C door-unlock
    $74 door-unlock
    $78 door-unlock
    $5E door-unlock
    $76 door-unlock
    $7B door-unlock
    $7C door-unlock
    $63 door-unlock
    $54 door-unlock
    1 door-open-clear
    1 door-unlock
    3 door-open-clear
    3 door-unlock
    $C door-open-clear
    $C door-unlock
    $D door-lock
    $D door-open-set
    doors-room-in
    $14 state-flag-set
    $1E state-flag-set
    $D state-flag-set
    $12 state-flag-clear
    8 state-flag-set
    $41 story-flag-set
    5 state-flag-set
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
    \ (nop-progress-4C: no effect in this game)
    \ (nop-progress-48: no effect in this game)
    2 0 0 music
    3 0 0 music
    4 0 0 music
    1 char-activate
    $10 0 155 hewie-to-room
    1 $9B 0 char-to-tri-facing
    $28 state-flag-clear
    $F 1 fade
    wait-fade
    self-idle-or-end
;

: room10.act06 ( -- )   \ 003F77A0
    self-wait-done
    $100E self-anim
    0 0 6 char-sound
    self-frames-reset
    $14 self-wait-frames
    3 ebit? not if
        5 message
        3 ebit-set
    else
        6 message
        3 ebit-clear
    then
    2 $A self-anim-blend
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room10.act07 ( -- )   \ 003F77D0
    self-wait-done
    $17B 94.0 -27.5 90 $FFFF 5 self-move-to
    self-wait-done
    1 15.0 0.0 0.0 8.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 ebit? not if
        7 message
        wait-message
        4 ebit-set
    else
        8 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room10.act08 ( -- )   \ 003F7820
    self-wait-done
    $19A 94.0 27.5 90 $FFFF 5 self-move-to
    self-wait-done
    1 15.0 0.0 0.0 8.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room10.act09 ( -- )   \ 003F7870
    self-wait-done
    41.0 -19.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message
            wait-message
        else
            $28B story-flag-set
            3 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
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

' room10.enter $10 0 room-script!
' room10.char-enter $10 6 room-script!
' room10.phase1 $10 1 room-script!
' room10.phase2 $10 2 room-script!
' room10.phase3 $10 3 room-script!
' room10.phase5 $10 5 room-script!
' room10.act00 $10 $00 action-script!
' room10.act01 $10 $01 action-script!
' room10.act02 $10 $02 action-script!
' room10.act03 $10 $03 action-script!
' room10.act04 $10 $04 action-script!
' room10.act05 $10 $05 action-script!
' room10.act06 $10 $06 action-script!
' room10.act07 $10 $07 action-script!
' room10.act08 $10 $08 action-script!
' room10.act09 $10 $09 action-script!

\ ---- room $11 ----------------------------------------------------------------------------------

: room11.enter ( -- )   \ 003F7920
    0 1.2 15.55 -24.813 0 effect-86
    1 -1.26 15.55 -24.365 0 effect-86
    2 4.387 15.55 -6.264 0 effect-86
    3 2.327 15.55 -4.824 0 effect-86
    4 -3.559 15.55 -5.985 0 effect-86
    5 -1.887 15.55 -0.304 0 effect-86
    6 1.651 15.55 3.417 0 effect-86
    7 2.473 15.55 5.786 0 effect-86
    8 -2.85 15.55 13.116 0 effect-86
    9 -0.431 15.55 13.794 0 effect-86
    $A 3.347 15.55 19.167 0 effect-86
    $B 1.908 15.55 24.905 0 effect-86
    $C -0.597 15.55 25.104 0 effect-86
    $320 story-flag? not if
        1 1 $14 door-bits
    else
        1 0 $14 door-bits
    then
    $242 story-flag? $243 story-flag? not and if
        $E 34.2 1.0 60.5 flicker-sprite
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
;

: room11.char-enter ( -- )   \ 003F7A20
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
    0 self-is? if
        $80 exit-taken? if
            0 $E8 char-to-tri
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
    then
;

: room11.phase1 ( -- )   \ 003F7AC0
    0 exit-usable? if
        0 exit-check
    then
    3 story-flag? $31 story-flag? not and if
        1 exit-usable? if
            3 3 1 char-load
            1 exit-check
        then
    else 1 exit-usable? if
        1 exit-check
    then then
    2 0 0 1 chars-area-camera
    3 1 1 -1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        3 story-flag? $31 story-flag? not and if
            3 0 char-remove
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-left-area? if
        3 story-flag? $31 story-flag? not and if
            3 3 1 char-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    $242 story-flag? not if
        0 34.2 0.0 60.5 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 578 var-set
                $1A 579 var-set
                $1B 14 var-set
                $1C 34200 var-set
                $1D 1000 var-set
                $1E 60500 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    5 4 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 4 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 4 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 4 char-effect-moving
    then
    6 5 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 5 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 5 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 5 char-effect-moving
    then
    7 6 8 -4 0 zone-at-effect
    0 7 3 char-zone-bits? 0 7 3 char-zone-bits-before? not and if
        0 6 char-effect-moving
    then
    1 7 3 char-zone-bits? 1 7 3 char-zone-bits-before? not and if
        1 6 char-effect-moving
    then
    $FE 7 3 char-zone-bits? $FE 7 3 char-zone-bits-before? not and if
        $FE 6 char-effect-moving
    then
    8 7 8 -4 0 zone-at-effect
    0 8 3 char-zone-bits? 0 8 3 char-zone-bits-before? not and if
        0 7 char-effect-moving
    then
    1 8 3 char-zone-bits? 1 8 3 char-zone-bits-before? not and if
        1 7 char-effect-moving
    then
    $FE 8 3 char-zone-bits? $FE 8 3 char-zone-bits-before? not and if
        $FE 7 char-effect-moving
    then
    9 8 8 -4 0 zone-at-effect
    0 9 3 char-zone-bits? 0 9 3 char-zone-bits-before? not and if
        0 8 char-effect-moving
    then
    1 9 3 char-zone-bits? 1 9 3 char-zone-bits-before? not and if
        1 8 char-effect-moving
    then
    $FE 9 3 char-zone-bits? $FE 9 3 char-zone-bits-before? not and if
        $FE 8 char-effect-moving
    then
    $A 9 8 -4 0 zone-at-effect
    0 $A 3 char-zone-bits? 0 $A 3 char-zone-bits-before? not and if
        0 9 char-effect-moving
    then
    1 $A 3 char-zone-bits? 1 $A 3 char-zone-bits-before? not and if
        1 9 char-effect-moving
    then
    $FE $A 3 char-zone-bits? $FE $A 3 char-zone-bits-before? not and if
        $FE 9 char-effect-moving
    then
    $B $A 8 -4 0 zone-at-effect
    0 $B 3 char-zone-bits? 0 $B 3 char-zone-bits-before? not and if
        0 $A char-effect-moving
    then
    1 $B 3 char-zone-bits? 1 $B 3 char-zone-bits-before? not and if
        1 $A char-effect-moving
    then
    $FE $B 3 char-zone-bits? $FE $B 3 char-zone-bits-before? not and if
        $FE $A char-effect-moving
    then
    $C $B 8 -4 0 zone-at-effect
    0 $C 3 char-zone-bits? 0 $C 3 char-zone-bits-before? not and if
        0 $B char-effect-moving
    then
    1 $C 3 char-zone-bits? 1 $C 3 char-zone-bits-before? not and if
        1 $B char-effect-moving
    then
    $FE $C 3 char-zone-bits? $FE $C 3 char-zone-bits-before? not and if
        $FE $B char-effect-moving
    then
    $D $C 8 -4 0 zone-at-effect
    0 $D 3 char-zone-bits? 0 $D 3 char-zone-bits-before? not and if
        0 $C char-effect-moving
    then
    1 $D 3 char-zone-bits? 1 $D 3 char-zone-bits-before? not and if
        1 $C char-effect-moving
    then
    $FE $D 3 char-zone-bits? $FE $D 3 char-zone-bits-before? not and if
        $FE $C char-effect-moving
    then
;

: room11.phase2 ( -- )   \ 003F7E00
    $320 story-flag? not if
        $E 4.0 8.5 -20.0 5 5 0 zone
        0 $E 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    $242 story-flag? $243 story-flag? not and if
        0 $E 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    0 5 char-in-area? 0 -45 $3C char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 45 $3C char-heading? and if
        5 0 0 scene-change
    then
    0 7 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    -2147483646 scene-request? if
        5 5 1 scene-change
    then
;

: room11.act00 ( -- )   \ 003F7E80
    self-wait-done
    2 story-flag? not if
        6 message
        wait-message
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: room11.act01 ( -- )   \ 003F7E90
    self-wait-done
    4.0 -20.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $320 story-flag-set
    1 0 $14 door-bits
    $13 message-param-room
    $13 1 item-give-count
    0 $13 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room11.act02 ( -- )   \ 003F7EE0
    self-wait-done
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
    $FFFF message-close
    3 door-open-clear
    doors-room-in
    8 state-flag-set
    $1D 3 $FF char-load
    3 char-unload
    \ (nop-progress-14: no effect in this game)
    $32 $FF movie-param
    0 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    wait-fade
    8 state-flag-set
    3 action-end
    3 char-done
    0 $E8 char-to-tri
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    $39 story-flag-set
    $80 exit-check
    0 0 $14 door-bits
    self-idle-or-end
;

: room11.act03 ( -- )   \ 003F7FA0
    self-wait-done
    34.2 60.5 self-turn-to-xz
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
            $243 story-flag-set
            $E effect-remove
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

: room11.act04 ( -- )   \ 003F8000
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $A00 self-anim
    self-wait-anim
    8 message
    wait-message
    self-idle-or-end
;

: room11.act05 ( -- )   \ 003F8020
    self-wait-done
    $FE self-touching? not if
        3 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 3 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    3 door-lock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: room11.act06 ( -- )   \ 003F8060
    self-wait-done
    0 1.2 15.55 -24.813 0 effect-86
    1 -1.26 15.55 -24.365 0 effect-86
    2 4.387 15.55 -6.264 0 effect-86
    3 2.327 15.55 -4.824 0 effect-86
    4 -3.559 15.55 -5.985 0 effect-86
    5 -1.887 15.55 -0.304 0 effect-86
    6 1.651 15.55 3.417 0 effect-86
    7 2.473 15.55 5.786 0 effect-86
    8 -2.85 15.55 13.116 0 effect-86
    9 -0.431 15.55 13.794 0 effect-86
    $A 3.347 15.55 19.167 0 effect-86
    $B 1.908 15.55 24.905 0 effect-86
    $C -0.597 15.55 25.104 0 effect-86
    3 partner-load
    2 char-unload
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
    $1D 3 $FF char-load
    3 char-unload
    $32 $FF movie-param
    0 1 $14 door-bits
    $FF panic-stage? if
        3 panic-stage
    then
    2 message-prepare
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
    3 action-end
    3 char-done
    0 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room11.enter $11 0 room-script!
' room11.char-enter $11 6 room-script!
' room11.phase1 $11 1 room-script!
' room11.phase2 $11 2 room-script!
' room11.act00 $11 $00 action-script!
' room11.act01 $11 $01 action-script!
' room11.act02 $11 $02 action-script!
' room11.act03 $11 $03 action-script!
' room11.act04 $11 $04 action-script!
' room11.act05 $11 $05 action-script!
' room11.act06 $11 $06 action-script!

\ ---- room $12 ----------------------------------------------------------------------------------

\ room 0x12 (Room12_Cmd00_ptmf): byte 3 0: the smoke puffs (SmokePuffs_vtable), their slot in
\ script variable 0; else that slot started
: room12.cmd00 ( b0 -- )  drop stub-step ;

: room12.enter ( -- )   \ 003F8200
    room-sounds
    1 story-flag? not if
        0 1 $14 door-bits
    then
    0 -35.8 -50.5 -27.25 0 effect-86
    $203 story-flag? not if
        1 0 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        $22C story-flag? not if
            1 -52.5 -59.05 -1.5 flicker-sprite
        then
    then
    0 8 0.562 0.5 0.25 0.5 zone-rect
    $244 story-flag? $245 story-flag? not and if
        2 -18.1 -59.0 -8.1 flicker-sprite
    then
    $281 story-flag? not if
        3 -13.0 9.0 10.0 flicker-sprite
    then
    3 story-flag? $31 story-flag? not and if
        7 state-flag-set
        1 1 $1000010 nav-group
        3 char-unload
        3 char-activate
        0 3 0 char-model-op
        3 $9000 1 0 char-anim-hold
        3 $2A 14.85 17.65 0 char-to-xz
        4 0 object-anim-loop
        0 room12.cmd00
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 0 -19.5 -26.0 -31.0 4.0 4.0 100.0 0.0 10.0 scene-effect-71000
    2 0 -53.0 -22.0 -11.0 4.0 5.0 80.0 0.0 30.0 scene-effect-71000
    $321 story-flag? not if
        3 1 $14 door-bits
        4 0 $14 door-bits
        1 2 $20000 nav-group
    else
        3 0 $14 door-bits
        4 1 $14 door-bits
        0 2 $20000 nav-group
    then
    1 $2300 sound-volume
;

: room12.act10 ( -- )   \ 003F8DE0
    6 ebit-set
    8 ebit-set
    5 ebit-clear
    $E 0 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 1 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 2 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 3 pvar? if
        $64 chance? if
            5 ebit-set
        then
    then then then then
    $FE char-unseen? not if
        5 ebit-set
    then
    2 creature-action? if
        5 ebit-set
    then
    4 stalker-alert? if
        5 ebit-clear
    then
    $FE camera-follow
    5 ebit? if
        0 $FE $11 action
    else
        $78 1 item-cooldown
        $B ebit-clear
    then
    $E pvar-inc
    exit
;

: room12.char-enter ( -- )   \ 003F8350
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 5 -1 char-camera
                0 camera-follow
            else
                1 5 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then then
    0 5 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    1 2 -1 area-camera
    0 self-is? if
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
    1 self-is? if
    then
    $FE self-is? if
        9 state-flag? if
            room12.act10
        else
            8 ebit-clear
        then
    then
;

: room12.phase1 ( -- )   \ 003F83F0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 1 0 1 chars-area-camera
    3 2 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 3 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 3 1 1 chars-area-camera
    $10 1 0 1 chars-area-camera
    $11 5 -1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $13 char-entered-area? if
        2 map-page
    then
    0 $13 char-left-area? if
        1 map-page
    then
    1 story-flag? 2 story-flag? not and if
        0 5 char-entered-area? $A ebit? not and if
            $A ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 5 action-force
            else
                1 0 5 action-force
            then
        then
    then
    $244 story-flag? not if
        3 -18.1 -60.0 -8.1 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 580 var-set
                $1A 581 var-set
                $1B 2 var-set
                $1C -18100 var-set
                $1D -59000 var-set
                $1E -8100 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    3 story-flag? $31 story-flag? not and if
        $32 story-flag? not if
            2 16.5 0.0 16.5 $1E 10 0 zone
        else
            2 16.5 0.0 16.5 $A 10 0 zone
        then
        $32 story-flag? not if
            0 2 3 char-zone-bits? 2 camera-mode? and if
                $32 story-flag-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 0 action-force
                else
                    1 0 0 action-force
                then
            then
        else 0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
            0 0 char-action? $FF panic-stage? not and if
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 2 action-force
            then
        then then
        9 ebit? not if
            6 sound-bank-loaded? if
                $40000001 6 15.0 12.0 22.0 0 0 sound
                9 ebit-set
            then
        else
            $C0000001 6 15.0 12.0 22.0 0 0 sound
        then
        4 16.5 0.0 16.5 $19 10 0 zone
        7 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        7 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        6 16.5 0.0 16.5 $32 10 0 zone
        1 char-here? 1 game-mode? and if
            hewie-can-command? if
                1 6 9 char-zone-bits? if
                    $1F 6 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    $FE 2 char-C4? not if
        9 state-flag? 6 ebit? not and $FE char-here? and if
            $B ebit-set
            room12.act10
        then
    then
    $321 story-flag? not if
        5 22.0 -40.0 -32.0 6 12 0 zone
        0 5 char-in-zone? if
            0 0 6 char-sound
            1 $FF 4 rumble
            3 0 $14 door-bits
            4 1 $14 door-bits
            0 2 $20000 nav-group
            $321 story-flag-set
            0 8 0.234 0.5 0.328 0.5 zone-rect
            19.0 -40.0 -32.0 0 -2144325584 2 0.0 scene-effect-8C
            22.0 -40.0 -32.0 0 -2143272896 2 0.0 scene-effect-8C
            25.0 -40.0 -32.0 0 -2144325584 2 0.0 scene-effect-8C
            0 18.0 -36.0 -33.0 0 0 0 0 dust
            0 23.0 -38.0 -33.0 0 0 0 0 dust
            0 28.0 -36.0 -33.0 0 0 0 0 dust
        then
    then
    7 0.6 0.0 -3.32 $F 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 7 9 char-zone-bits? if
                $1F 7 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 -46.47 -60.0 -29.37 $E 19 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 8 9 char-zone-bits? if
                $1F 8 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
;

: room12.phase2 ( -- )   \ 003F8750
    $203 story-flag? not if
        1 -52.5 -60.05 -1.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $203 story-flag-set
            0 0 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -52.5 -60.05 -1.5 0 -2146424784 0 0.0 scene-effect-8C
            3 6 -52.5 -60.05 -1.5 0 0 sound
            $40 $209 noise
            1 -52.5 -59.05 -1.5 flicker-sprite
        then
    else $22C story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
    $244 story-flag? $245 story-flag? not and if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then
    $281 story-flag? not if
        9 3 5 5 0 zone-at-effect
        0 9 3 char-zone-bits? if
            5 $13 4 scene-change
        then
    then
    0 $12 $2D char-faces-area? if
        0 -45 $3C char-heading? if
            1 story-flag? not if
                0 $A $2D char-faces-area? if
                    5 4 4 scene-change
                then
            else $FE char-here? not if
                3 ebit-clear
                5 $E 5 scene-change
            else $FE char-unseen? if
                3 ebit-clear
                5 $E 5 scene-change
            else
                $8016 scene-ending
            then then then
        then
        0 45 $3C char-heading? if
            1 story-flag? not if
                0 $A $2D char-faces-area? if
                    5 4 4 scene-change
                then
            else $FE char-here? not if
                3 ebit-set
                5 $E 5 scene-change
            else $FE char-unseen? if
                3 ebit-set
                5 $E 5 scene-change
            else
                $8016 scene-ending
            then then then
        then
    then
    5 scene-request? not if
        0 9 char-in-area? 0 45 $32 char-heading? and if
            5 8 0 scene-change
        then
        0 $B char-in-area? 0 45 $32 char-heading? and if
            5 9 0 scene-change
        then
        0 $C char-in-area? 0 90 $32 char-heading? and if
            5 $A 0 scene-change
        then
        0 $D char-in-area? 0 0 $32 char-heading? and if
            5 $B 0 scene-change
        then
        3 story-flag? not if
            0 $E char-in-area? 0 0 $32 char-heading? and if
                5 $C 0 scene-change
            then
        else $31 story-flag? if
            0 $E char-in-area? 0 0 $32 char-heading? and if
                5 $16 0 scene-change
            then
        then then
        0 3 -2 $32 char-faces-xz? 0 -3 -2 $32 char-faces-xz? or 0 $F char-in-area? and if
            5 $D 0 scene-change
        then
    then
    0 5 3 char-zone-bits? 0 0 $32 char-heading? and if
        5 6 0 scene-change
    then
;

: room12.phase3 ( -- )   \ 003F8900
    -19.0 8.0 0.0 13.0 8.0 0.0 -19.0 0.0 0.0 13.0 0.0 0.0 lights-doorway
    -18.1 13.0 -2.7 -16.2 13.0 -2.7 -18.1 0.0 -2.7 -16.2 0.0 -2.7 lights-doorway
    -18.1 13.0 -1.0 -18.1 13.0 -2.7 -18.1 0.0 -1.0 -18.1 0.0 -2.7 lights-doorway
    -19.0 8.0 15.0 -19.0 8.0 0.0 -19.0 0.0 15.0 -19.0 0.0 0.0 lights-doorway
;

: room12.phase5 ( -- )   \ 003F89D0
    3 story-flag? $31 story-flag? not and if
        7 state-flag-clear
        1 $FE 0 char-model-op
        3 action-end
        3 char-done
    then
;

: room12.act00 ( -- )   \ 003F89F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    4 object-anim-reset
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
    1 room12.cmd00
    1 char-here? 1 2 char-C4? and if
        0 1 $86 action
    then
    $50 $FF movie-param
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
    3 $9000 1 0 char-anim-hold
    4 0 object-anim-loop
    4 0 object-show
    3 $2A 14.85 17.65 0 char-to-xz
    0 3 0 char-model-op
    1 0 char-visible
    1 action-end
    wait-fade
    0 self-move-16
    0 $18B 43.7 11.8 -75 char-to-xz
    hewie-controlled? not if
        0 2 -1 char-camera
        0 camera-follow
    else
        1 2 -1 char-camera
        1 camera-follow
    then
    camera-restart
    0 room12.cmd00
    $F $51 fade
    wait-fade
    $226 item-give
    $1B resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room12.act01 ( -- )   \ 003F8AF0
    self-wait-done
    -52.5 -1.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $22C story-flag-set
            1 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: room12.act02 ( -- )   \ 003F8B48
    self-wait-done
    3 self-look-at
    yield
    3 message
    wait-message
    $FF self-look-at
    yield
    self-idle-or-end
;

: room12.act03 ( -- )   \ 003F8B60
    self-wait-done
    $FE self-touching? not if
        4 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 4 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    4 door-lock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: room12.act04 ( -- )   \ 003F8BA0
    1 self-scripted
    self-wait-done
    -35.35 -34.5 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    0 0 $14 door-bits
    5 message-param-room
    5 1 item-give-count
    0 5 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    1 story-flag-set
    0 state-flag-clear
    5 door-open-clear
    doors-room-in
    0 self-scripted
    self-idle-or-end
;

: room12.act05 ( -- )   \ 003F8BF0
    $18 state-flag-set
    $1B state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
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
    $FE $12 25 2 stalker-to-room
    $FE action-end
    $FE char-done
    4 3 $FF char-load
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
    $FE action-end
    $FE char-done
    $12 state-flag-set
    0 1 char-silent
    camera-restart
    \ (nop-progress-24: no effect in this game)
    $18 state-flag-clear
    0 self-scripted
    0 exit-check
    self-idle-or-end
;

: room12.act06 ( -- )   \ 0047AA00
    self-wait-done
    $E message
    wait-message
    self-idle-or-end
;

: room12.act07 ( -- )   \ 0047AA08
    self-idle-or-end
;

: room12.act08 ( -- )   \ 003F8CA0
    self-wait-done
    1 ebit? not if
        8 message
        wait-message
        1 ebit-set
    else
        9 message
        wait-message
    then
    self-idle-or-end
;

: room12.act09 ( -- )   \ 003F8CB8
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    $D message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room12.act0A ( -- )   \ 003F8CD0
    self-wait-done
    2 ebit? not if
        $B message
        wait-message
        2 ebit-set
    else
        $C message
        wait-message
    then
    self-idle-or-end
;

: room12.act0B ( -- )   \ 0047AA10
    self-wait-done
    $A message
    wait-message
    self-idle-or-end
;

: room12.act0C ( -- )   \ 0047AA18
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room12.act0D ( -- )   \ 003F8CE8
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $1E self-wait-frames
    4 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room12.act0F ( -- )   \ 003F8DB0
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    $FE char-busy? if
        4 ebit? not if
            $FE action-end
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: room12.act0E ( -- )   \ 003F8D00
    $18 state-flag-set
    8 ebit-clear
    1 self-scripted
    6 ebit-clear
    0 5 char-file-load
    self-wait-done
    0 char-file-use
    3 ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    4 ebit-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room12.act0F goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                8 ebit? $FE char-here? not and if
                    8 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room12.act0F goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room12.act11 ( -- )   \ 003F8E40
    self-wait-done
    $B ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and if
            1 3 self-move-slot
            self-wait-done
        then
    else
        $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and $FE 1 char-in-area? and if
            1 3 self-move-slot
            self-wait-done
        then
    then
    $B ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 6 char-file-load
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        4 ebit-set
        self-wait-anim
    else
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        $1601 self-anim
        self-wait-anim
        4 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room12.act12 ( -- )   \ 003F8EE0
    self-wait-done
    -18.1 -8.1 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $245 story-flag-set
            2 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: room12.act13 ( -- )   \ 003F8F40
    self-wait-done
    -13.0 10.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $281 story-flag-set
            3 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: room12.act14 ( -- )   \ 003F8FA0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    2 3 $FF char-load
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room12.act15 ( -- )   \ 003F9040
    self-wait-done
    3 3 $FF char-load
    3 char-unload
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
    $50 $FF movie-param
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room12.act16 ( -- )   \ 003F90D0
    self-wait-done
    $C ebit? not if
        6 message
        wait-message
        $C ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

' room12.enter $12 0 room-script!
' room12.char-enter $12 6 room-script!
' room12.phase1 $12 1 room-script!
' room12.phase2 $12 2 room-script!
' room12.phase3 $12 3 room-script!
' room12.phase5 $12 5 room-script!
' room12.act00 $12 $00 action-script!
' room12.act01 $12 $01 action-script!
' room12.act02 $12 $02 action-script!
' room12.act03 $12 $03 action-script!
' room12.act04 $12 $04 action-script!
' room12.act05 $12 $05 action-script!
' room12.act06 $12 $06 action-script!
' room12.act07 $12 $07 action-script!
' room12.act08 $12 $08 action-script!
' room12.act09 $12 $09 action-script!
' room12.act0A $12 $0A action-script!
' room12.act0B $12 $0B action-script!
' room12.act0C $12 $0C action-script!
' room12.act0D $12 $0D action-script!
' room12.act0E $12 $0E action-script!
' room12.act0F $12 $0F action-script!
' room12.act10 $12 $10 action-script!
' room12.act11 $12 $11 action-script!
' room12.act12 $12 $12 action-script!
' room12.act13 $12 $13 action-script!
' room12.act14 $12 $14 action-script!
' room12.act15 $12 $15 action-script!
' room12.act16 $12 $16 action-script!

\ ---- room $13 ----------------------------------------------------------------------------------

\ room 0x13: the grate ("kousi"): byte 3 0 shut (turn 0), else swung open a quarter turn.
: room13.cmd00 ( b0 -- )  drop stub-step ;

: room13.enter ( -- )   \ 003F91A0
    room-sounds
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $10 152.0 92.2 -60.0 $A $80 $80 $80 $30 specks
        $E 128.0 92.2 -60.0 $C $80 $80 $80 $30 specks
        $10 152.0 56.7 12.9 $A $80 $80 $80 $30 specks
        $A 128.0 56.7 12.9 $A $80 $80 $80 $30 specks
    then
    $28 story-flag? not if
        0 room13.cmd00
        0 1 $20000 nav-group
        1 0 $20000 nav-group
    else
        1 room13.cmd00
        0 0 $20000 nav-group
        1 1 $20000 nav-group
    then
    $42 story-flag? not if
        6 1 $14 door-bits
    else
        7 1 $14 door-bits
    then
    2 story-flag? 3 story-flag? not and if
        0 30 var-set
        0 $F1 6 action
    then
    $20F story-flag? not if
        1 2 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $210 story-flag? not if
        1 3 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 3 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $283 story-flag? not if
            0 130.0 61.0 -237.0 flicker-sprite
        then
    then
    $211 story-flag? not if
        1 4 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 4 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    0 6 0.0 1.0 0.187 0.312 zone-rect
;

: room13.char-enter ( -- )   \ 003F92E0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    2 3 -1 area-camera
    $FE self-is? if
    then
;

: room13.phase1 ( -- )   \ 003F93A0
    $18 story-flag? not 3 story-flag? and $B story-flag? not and if
    else 0 exit-usable? if
        0 exit-check
    then then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 -1 1 chars-area-camera
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    3 story-flag? $28 story-flag? not and if
        0 $F char-entered-area? 0 $F char-left-area? or $FE char-here? not and if
            $28 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    $18 story-flag? not 3 story-flag? and $B story-flag? not and if
        0 0 char-entered-area? 0 ebit? not and if
            0 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 2 action-force
            else
                1 0 2 action-force
            then
        then
    then
    $20F story-flag? not if
        0 131.0 60.0 -244.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $20F story-flag-set
            0 2 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            131.0 60.0 -244.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 131.0 60.0 -244.0 0 0 sound
            $40 $1E4 noise
        then
    then
    $211 story-flag? not if
        2 137.0 60.0 -240.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $211 story-flag-set
            0 4 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            137.0 60.0 -240.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 137.0 60.0 -240.0 0 0 sound
            $40 $1E4 noise
        then
    then
;

: room13.phase2 ( -- )   \ 003F9530
    $28 story-flag? not 0 $E char-in-area? and 0 -45 $32 char-heading? and if
        5 0 1 scene-change
    then
    0 $10 char-in-area? 0 90 $3C char-heading? and if
        5 3 0 scene-change
    then
    0 $11 char-in-area? 0 -45 $3C char-heading? and if
        5 4 0 scene-change
    then
    $210 story-flag? not if
        1 130.0 60.0 -237.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $210 story-flag-set
            0 3 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            130.0 60.0 -237.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 130.0 60.0 -237.0 0 0 sound
            $40 $1E4 noise
            0 130.0 61.0 -237.0 flicker-sprite
        then
    else $283 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then then
;

: room13.phase3 ( -- )   \ 003F95F0
    151.9 66.3 -63.6 151.9 66.3 -43.5 151.9 44.1 -63.6 151.9 44.1 -43.8 lights-doorway
    148.0 97.0 -176.0 175.0 97.0 -176.0 148.0 97.0 -150.0 175.0 97.0 -150.0 lights-doorway
    109.0 124.0 -176.0 148.0 97.0 -176.0 109.0 124.0 -150.0 148.0 97.0 -150.0 lights-doorway
    148.0 101.0 -176.0 175.0 101.0 -176.0 148.0 97.0 -176.0 175.0 97.0 -176.0 lights-doorway
    109.0 128.0 -176.0 148.0 101.0 -176.0 109.0 124.0 -176.0 148.0 97.0 -176.0 lights-doorway
;

: room13.act00 ( -- )   \ 003F96E8
    self-wait-done
    $1D02 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room13.act01 ( -- )   \ 003F9700
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
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
    self-wait-done
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
    0 $20A 133.34 -76.62 -35 char-to-xz
    0 self-move-16
    camera-restart
    $28 story-flag-set
    $B door-lock
    1 room13.cmd00
    0 0 $20000 nav-group
    1 1 $20000 nav-group
    $18 state-flag-clear
    0 self-scripted
    $F $41 fade
    self-idle-or-end
;

: room13.act02 ( -- )   \ 003F97C0
    $18 state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    $12 state-flag-set
    \ (nop-progress-14: no effect in this game)
    $18 state-flag-clear
    0 self-scripted
    $81 exit-check
    0.0 sound-volume-scale
    self-idle-or-end
;

: room13.act03 ( -- )   \ 003F97F0
    self-wait-done
    70.0 -160.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    self-idle-or-end
;

: room13.act04 ( -- )   \ 003F9800
    self-wait-done
    100.0 -210.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    self-idle-or-end
;

: room13.act05 ( -- )   \ 003F9810
    self-wait-done
    130.0 -237.0 self-turn-to-xz
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
            $283 story-flag-set
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

: room13.act06 ( -- )   \ 003F9870
    begin
        yield
        0 var-dec
        0 0 var? if
            $1E chance? if
                $F 6 228.0 5.0 78.0 0 0 sound
                0 180 var-set
            else
                0 60 var-set
            then
        then
    again
;

: room13.act07 ( -- )   \ 003F98B0
    self-wait-done
    $10 152.0 92.2 -60.0 $A $80 $80 $80 $30 specks
    $E 128.0 92.2 -60.0 $C $80 $80 $80 $30 specks
    $10 152.0 56.7 12.9 $A $80 $80 $80 $30 specks
    $A 128.0 56.7 12.9 $A $80 $80 $80 $30 specks
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

' room13.enter $13 0 room-script!
' room13.char-enter $13 6 room-script!
' room13.phase1 $13 1 room-script!
' room13.phase2 $13 2 room-script!
' room13.phase3 $13 3 room-script!
' room13.act00 $13 $00 action-script!
' room13.act01 $13 $01 action-script!
' room13.act02 $13 $02 action-script!
' room13.act03 $13 $03 action-script!
' room13.act04 $13 $04 action-script!
' room13.act05 $13 $05 action-script!
' room13.act06 $13 $06 action-script!
' room13.act07 $13 $07 action-script!

\ ---- room $14 ----------------------------------------------------------------------------------

\ room 0x14: the curtain ("Cartain") animated by event variable 0 (var0_obj_anim: byte 3 picks
\ the frame range and direction).
: room14.cmd00 ( b0 -- )  drop stub-step ;

: room14.enter ( -- )   \ 003F99E0
    room-sounds
    $225 story-flag? not if
        0 78.0 6.5 20.0 flicker-sprite
    then
    $246 story-flag? $247 story-flag? not and if
        1 -44.9 -7.0 -12.9 flicker-sprite
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 room14.cmd00
    1 $3FFF sound-volume
;

: room14.act07 ( -- )   \ 003F9EB0
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 1 char-file-load
    then
    $FE camera-follow
    3 ebit-clear
    $F 0 pvar? if
        0 chance? if
            3 ebit-set
        then
    else $F 1 pvar? if
        0 chance? if
            3 ebit-set
        then
    else $F 2 pvar? if
        $19 chance? if
            3 ebit-set
        then
    else $F 3 pvar? if
        $19 chance? if
            3 ebit-set
        then
    then then then then
    2 creature-action? if
        3 ebit-set
    then
    4 stalker-alert? if
        3 ebit-clear
    then
    3 ebit? if
        0 $FE 8 action
    else
        $78 1 item-cooldown
    then
    $F pvar-inc
    exit
;

: room14.char-enter ( -- )   \ 003F9A30
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
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    1 2 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            5 ebit-set
            room14.act07
        else
            5 ebit-clear
        then
    then
;

: room14.phase1 ( -- )   \ 003F9AC0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    0 2 char-left-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $246 story-flag? not if
        2 -44.9 -8.0 -12.9 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 582 var-set
                $1A 583 var-set
                $1B 1 var-set
                $1C -44900 var-set
                $1D -7000 var-set
                $1E -12900 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
;

: room14.phase2 ( -- )   \ 003F9B60
    0 $A char-in-area? 0 90 $2D char-heading? and if
        5 0 0 scene-change
    then
    0 $B char-in-area? 0 90 $2D char-heading? and if
        5 1 0 scene-change
    then
    -2147483646 scene-request? if
        5 3 1 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 5 5 scene-change
        else
            $8016 scene-ending
        then
    then
    $225 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
    $246 story-flag? $247 story-flag? not and if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
;

: room14.act00 ( -- )   \ 0047AA20
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room14.act01 ( -- )   \ 003F9BD0
    0 ebit? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        0 0 char-file-load
        $89 -9.659 4.88 180 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        1 message
        wait-message
        0 char-file-use
        $8004 $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 ebit-set
        0 $84 5 char-sound
        self-frames-reset
        $1E self-wait-frames
        $31E story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message
            wait-message
            $F 7 fade
            wait-fade
            $31E story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        2 message
        wait-message
    then
    self-idle-or-end
;

: room14.act02 ( -- )   \ 003F9C90
    self-wait-done
    78.0 20.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $225 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room14.act03 ( -- )   \ 003F9CF0
    self-wait-done
    1 ebit? not 2 game-mode? or $FF panic-stage? or if
        1 self-through-exit
        self-wait-done
        $FF panic-stage? 2 game-mode? or if
            $60A self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $608 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            3 message
            wait-message
        then
        1 ebit-set
    else
        $95 -58.5 9.5 -105 $FFFF 5 self-move-to
        self-wait-done
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        4 message
        wait-message
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    then
    self-idle-or-end
;

: room14.act04 ( -- )   \ 003F9D50
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A self-through-door
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 2 6 char-sound
    7 message-param-room
    7 item-use
    $A door-lock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room14.act06 ( -- )   \ 003F9E60
    2 avoid-prompt
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 0 var-set
    4 ebit? if
        counter-inc
    then
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        2 room14.cmd00
        yield
        0 28 var? not while
        0 var-inc
    repeat
    3 room14.cmd00
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    self-wait-anim
    0 self-noclip
    4 ebit? if
        2 wait-counter
    then
    0 self-scripted
    self-idle-or-end
;

: room14.act09 ( -- )   \ 003F9F80
    2 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 0 var-set
    $8003 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        5 room14.cmd00
        yield
        0 18 var? not while
        0 var-inc
    repeat
    3 room14.cmd00
    self-wait-anim
    0 self-noclip
    2 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: room14.act05 ( -- )   \ 003F9D90
    $18 state-flag-set
    5 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    1 self-noclip
    1 self-scripted
    $15 60.918 -14.76 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $FF self-look-at
    yield
    0 0 var-set
    $8000 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        0 room14.cmd00
        yield
        0 54 var? not while
        0 var-inc
    repeat
    self-wait-anim
    4 room14.cmd00
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    2 ebit-clear
    4 ebit-clear
    0 counter-set
    0 avoid-prompt
    begin
        4 ebit? if
            ['] room14.act06 goto
        else 0 2 pad? not if
            $FF panic-stage? 2 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room14.act09 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? $FE char-here? not and if
                    5 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                $FE action-end
                ['] room14.act09 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            0 0 var-set
            $8001 0 self-anim-9
            begin
                1 room14.cmd00
                yield
                0 21 var? not while
                0 var-inc
            repeat
            3 room14.cmd00
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then then
    again
;

: room14.act08 ( -- )   \ 003F9F10
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $FA $FFFF 6 self-move-tri
    self-wait-done
    $10 58.883 -8.841 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        4 ebit-set
        1 self-scripted
        yield
        $FE char-file-use
        1 wait-counter
        $8000 5 self-anim-9
        self-wait-anim
        counter-inc
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        2 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room14.act0A ( -- )   \ 003F9FD0
    self-wait-done
    -44.9 -12.9 self-turn-to-xz
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
            $247 story-flag-set
            1 effect-remove
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

' room14.enter $14 0 room-script!
' room14.char-enter $14 6 room-script!
' room14.phase1 $14 1 room-script!
' room14.phase2 $14 2 room-script!
' room14.act00 $14 $00 action-script!
' room14.act01 $14 $01 action-script!
' room14.act02 $14 $02 action-script!
' room14.act03 $14 $03 action-script!
' room14.act04 $14 $04 action-script!
' room14.act05 $14 $05 action-script!
' room14.act06 $14 $06 action-script!
' room14.act07 $14 $07 action-script!
' room14.act08 $14 $08 action-script!
' room14.act09 $14 $09 action-script!
' room14.act0A $14 $0A action-script!

\ ---- room $15 ----------------------------------------------------------------------------------

\ a lid (pstr_kousi_2, +0x24 its height, +0x34 its speed): byte 3 0 up, 1 shut; 2 falling and
\ bouncing shut (the first landing clears progress flag 0x50 and, unless the director says no,
\ thuds), 2 while moving; 3 a random rattle up, 2 while it stays below
: room15.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x15 (Room15_Cond00_ptmf): Hewie is about in room 0xF in state 0x2F or 0x52
: room15.cond00? ( -- flag )  stub-flag ;

: room15.enter ( -- )   \ 003FA090
    room-sounds
    0 0 var-set
    $80 exit-taken? if
        1 room15.cmd00
        $13 story-flag-clear
    else
        $FE char-here? $FE 0 char-in-nav-group? and if
            $FE $F0 char-to-tri
        then
        1 exit-taken? if
        else
            $310 story-flag-clear
            $13 story-flag-clear
        then
        $310 story-flag? not $13 story-flag? not and if
            1 0 $20000 nav-group
            1 room15.cmd00
        else
            $13 story-flag-set
            0 0 $20000 nav-group
            0 1 var-set
            0 room15.cmd00
        then
    then
    $14 story-flag? not if
        2 0 object-show
        0 0 $14 door-bits
        1 1 $14 door-bits
        1 1 $1000000 nav-group
    else
        2 1 object-show
        0 1 $14 door-bits
        1 0 $14 door-bits
    then
;

: room15.char-enter ( -- )   \ 003FA110
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
            0 $C7 -60 char-to-tri-facing
            1 30.0 15.0 0.0 0.0 event-camera
            0 $F0 7 action
        then
    then
    1 self-is? if
        $14 story-flag? not if
            $316 story-flag? not if
                0 1 8 action
            then
        then
    then
;

: room15.phase1 ( -- )   \ 003FA1C0
    0 ebit? not if
        6 sound-bank-loaded? if
            $14 story-flag? not if
                $40000003 6 -10.0 15.0 0.0 0 0 sound
            then
            0 ebit-set
        then
    else
        $C0000003 6 -10.0 15.0 0.0 0 0 sound
        $C0000001 6 0.0 30.0 30.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 0 0 1 chars-area-camera
    0 2 char-left-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $13 story-flag? not if
        0 0 var-set
    else room15.cond00? not if
        0 0 var-set
    then then
    \ (nop-progress-24: no effect in this game)
    $13 story-flag? if
        0 0 var? if
            0 0 char-in-nav-group? 1 0 char-in-nav-group? or $FE 0 char-in-nav-group? or if
                \ (nop-progress-24: no effect in this game)
            else
                $13 story-flag-clear
                1 0 $20000 nav-group
                0 $F1 3 action
            then
        then
    then
    $14 story-flag? not if
        0 $11 char-entered-area? 1 ebit? not and if
            1 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
;

: room15.phase2 ( -- )   \ 003FA2C0
    0 8 char-in-area? 0 -45 $2D char-heading? and if
        5 1 0 scene-change
    then
    $14 story-flag? not $13 story-flag? not and if
        0 9 char-in-area? 0 -45 $2D char-heading? and if
            5 4 0 scene-change
        then
    then
    0 $A char-in-area? 0 -45 $2D char-heading? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 90 $2D char-heading? and if
        5 5 0 scene-change
    then
    0 $C char-in-area? 0 $D char-in-area? or 0 45 $2D char-heading? and if
        5 5 0 scene-change
    then
    0 $E char-in-area? 0 0 $2D char-heading? and if
        5 5 0 scene-change
    then
    -2147483646 scene-request? if
        5 6 1 scene-change
    then
;

: room15.phase3 ( -- )   \ 003FA330
    33.0 23.0 -5.9 45.0 23.0 -5.9 33.0 11.0 -5.9 45.0 11.0 -5.9 lights-doorway
    33.0 11.0 -5.9 45.0 11.0 -5.9 33.0 0.0 -5.9 45.0 0.0 -5.9 lights-doorway
    45.0 23.0 -5.1 33.0 23.0 -5.1 45.0 0.0 -5.1 33.0 0.0 -5.1 lights-doorway
;

: room15.act00 ( -- )   \ 003FA3D0
    $2C state-flag-set
    $19 state-flag-set
    1 self-scripted
    $FE action-end
    $FE char-done
    self-frames-reset
    self-wait-16
    yield
    $F $54 fade
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
    $12 $1E movie-param
    1 action-end
    1 char-done
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
    3 6 sound-stop
    1.0 sound-volume-scale
    $1C resident-flag-set
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: room15.act01 ( -- )   \ 003FA450
    self-wait-done
    $14 story-flag? not if
        0 3 char-file-load
        $BF -39.071 35.745 -90 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 -10.0 0.0 0.0 event-camera
        0 message
        wait-message
        0 char-file-use
        0 answer? if
            2 0 object-anim
            $8000 $A self-anim-blend
            self-frames-reset
            $32 self-wait-frames
            0 6 -44.0 18.0 36.0 0 0 sound
            3 6 sound-stop
            1 0 $14 door-bits
            $14 story-flag-set
            40 hewie-trust
            \ (nop-progress-24: no effect in this game)
            0 1 $1000000 nav-group
            9 door-lock
            self-wait-anim
            $8277 item-give
        then
        self-frames-reset
        self-wait-16
        0 0.0 0.0 0.0 0.0 event-camera
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room15.act02 ( -- )   \ 0047AA28
    self-idle-or-end
;

: room15.act03 ( -- )   \ 003FA4F0
    $310 story-flag-set
    $40000001 6 0.0 30.0 30.0 0 0 sound
    2 room15.cmd00
    self-idle-or-end
;

: room15.act04 ( -- )   \ 0047AA30
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room15.act05 ( -- )   \ 003FA510
    self-wait-done
    7 0 var? if
        2 message
        wait-message
        7 1 var-set
    else 7 1 var? if
        3 message
        wait-message
        7 2 var-set
    else
        4 message
        wait-message
        7 0 var-set
    then then
    self-idle-or-end
;

: room15.act06 ( -- )   \ 003FA550
    self-wait-done
    9 self-through-door
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60A self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $608 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        6 message
        wait-message
    then
    self-idle-or-end
;

: room15.act07 ( -- )   \ 003FA580
    $17 state-flag-set
    8 state-flag-clear
    $F 1 fade
    wait-fade
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    \ (nop-progress-18: no effect in this game)
    $40000001 6 0.0 30.0 30.0 0 0 sound
    3 room15.cmd00
    2 6 0.0 30.0 30.0 0 0 sound
    self-frames-reset
    $1E self-wait-frames
    $F 0 fade
    wait-fade
    8 state-flag-set
    0 0.0 0.0 0.0 0.0 event-camera
    3 6 sound-stop
    $80 exit-check
    self-idle-or-end
;

: room15.act08 ( -- )   \ 003FA5F0
    self-wait-done
    $F5 -9.214 -23.873 -40 $FFFF $A self-move-to
    self-wait-done
    $14 story-flag? not if
        4 self-anim
        self-frames-reset
        $3C self-wait-frames
        $316 story-flag-set
        $14 story-flag? not if
            $1B03 self-anim
            self-wait-anim
            $14 story-flag? not if
                $1B03 self-anim
                self-wait-anim
                $14 story-flag? not if
                    $1B03 self-anim
                    self-wait-anim
                    $14 story-flag? not if
                        $1B03 self-anim
                        self-wait-anim
                        $14 story-flag? not if
                            $1B03 self-anim
                            self-wait-anim
                            $14 story-flag? not if
                                $1B03 self-anim
                                self-wait-anim
                                $14 story-flag? not if
                                    0 self-look-at
                                    yield
                                    4 self-anim
                                    self-frames-reset
                                    $28 self-wait-frames
                                    $FF self-look-at
                                    yield
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    self-idle-or-end
;

: room15.act09 ( -- )   \ 003FA660
    self-wait-done
    1 room15.cmd00
    2 0 object-show
    0 0 $14 door-bits
    1 1 $14 door-bits
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
    $12 $1E movie-param
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

' room15.enter $15 0 room-script!
' room15.char-enter $15 6 room-script!
' room15.phase1 $15 1 room-script!
' room15.phase2 $15 2 room-script!
' room15.phase3 $15 3 room-script!
' room15.act00 $15 $00 action-script!
' room15.act01 $15 $01 action-script!
' room15.act02 $15 $02 action-script!
' room15.act03 $15 $03 action-script!
' room15.act04 $15 $04 action-script!
' room15.act05 $15 $05 action-script!
' room15.act06 $15 $06 action-script!
' room15.act07 $15 $07 action-script!
' room15.act08 $15 $08 action-script!
' room15.act09 $15 $09 action-script!

\ ---- room $16 ----------------------------------------------------------------------------------

: room16.enter ( -- )   \ 003FA780
    1 $2300 sound-volume
    $2D3 story-flag? not if
        0 -2.0 9.0 -2.0 flicker-sprite
    then
    $AE story-flag? if
        $3E story-flag? not if
            $28 1 item-count? not if
                $3E story-flag? not if
                    1 -6.0 9.0 3.0 flicker-sprite
                then
            then
        then
    then
;

: room16.char-enter ( -- )   \ 003FA7C0
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
;

: room16.phase1 ( -- )   \ 003FA800
    0 exit-usable? if
        0 exit-check
    then
    3 0 0 1 chars-area-camera
    4 1 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
;

: room16.phase2 ( -- )   \ 003FA820
    $2D3 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 0 4 scene-change
        then
    then
    $3E story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: room16.phase3 ( -- )   \ 003FA850
    -6.7 20.0 24.7 -3.5 20.0 24.7 -6.7 0.0 24.7 -3.5 0.0 24.7 lights-doorway
    -3.5 20.0 24.8 -6.7 20.0 24.8 -3.5 0.0 24.8 -6.7 0.0 24.8 lights-doorway
    12.7 20.0 24.7 15.9 20.0 24.7 12.7 0.0 24.7 15.9 0.0 24.7 lights-doorway
    15.9 20.0 24.8 12.7 20.0 24.8 15.9 0.0 24.8 12.7 0.0 24.8 lights-doorway
;

: room16.act00 ( -- )   \ 003FA920
    $81 1 item-count? not if
        self-wait-done
        -2.0 -2.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        0 effect-remove
        $2D3 story-flag-set
        $81 message-param-room
        $81 1 item-give-count
        0 $81 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $903 self-anim
        self-wait-anim
        wait-message
    else
        self-wait-done
        -2.0 -2.0 self-turn-to-xz
        self-wait-done
        $FF panic-stage? not if
            $902 self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            $75 message-param-room
            $75 $63 item-count? if
                $8010 message
                wait-message
            else
                $2D3 story-flag-set
                0 effect-remove
                $75 1 item-give-count
                0 $75 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
            $903 self-anim
            self-wait-anim
        then
    then
    self-idle-or-end
;

: room16.act01 ( -- )   \ 003FA9C0
    self-wait-done
    -6.0 3.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $3E story-flag-set
    1 effect-remove
    $28 message-param-room
    $28 1 item-give-count
    0 $28 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

' room16.enter $16 0 room-script!
' room16.char-enter $16 6 room-script!
' room16.phase1 $16 1 room-script!
' room16.phase2 $16 2 room-script!
' room16.phase3 $16 3 room-script!
' room16.act00 $16 $00 action-script!
' room16.act01 $16 $01 action-script!

\ ---- room $17 ----------------------------------------------------------------------------------

: room17.enter ( -- )   \ 00414390
    room-sounds
    0 0 0 obstacle-place-saved
    1 1 0 obstacle-place-saved
    2 2 0 obstacle-place-saved
    3 3 0 obstacle-place-saved
    1 $2300 sound-volume
;

: room17.char-enter ( -- )   \ 004143B0
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

: room17.phase1 ( -- )   \ 00414430
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

: room17.phase2 ( -- )   \ 0047AC10
;

: room17.act00 ( -- )   \ 004144C0
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
    \ (nop-progress-18: no effect in this game)
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
    $1C door-reopen-lock
    $1D door-reopen-lock
    $38 door-close-off-unlock
    $39 door-close-off-unlock
    exits-rebuild
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    0 camera-mode? if
        $80 exit-check
    else
        $81 exit-check
    then
    self-idle-or-end
;

: room17.act01 ( -- )   \ 004145A0
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

' room17.enter $17 0 room-script!
' room17.char-enter $17 6 room-script!
' room17.phase1 $17 1 room-script!
' room17.phase2 $17 2 room-script!
' room17.act00 $17 $00 action-script!
' room17.act01 $17 $01 action-script!

\ ---- room $18 ----------------------------------------------------------------------------------

\ room 0x18 (Room18_Cmd00_ptmf): object byte 3's PlacedObject_ToDef
: room18.cmd00 ( b0 -- )  drop stub-step ;

: room18.enter ( -- )   \ 003FAA20
    5 story-flag? not if
        room-sounds
    then
    0 34.2 18.3 14.2 1 effect-86
    1 34.2 18.3 -14.2 1 effect-86
    2 -34.2 18.3 -14.2 1 effect-86
    3 -34.2 18.3 14.2 1 effect-86
    $25C story-flag? $25D story-flag? not and if
        4 -25.6 1.0 27.6 flicker-sprite
    then
    $295 story-flag? not if
        5 0.0 8.5 25.0 flicker-sprite
    then
    0 1 $14 door-bits
    1 $2300 sound-volume
;

: room18.act07 ( -- )   \ 003FB0A0
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 3 char-file-load
    then
    1 ebit-clear
    $D 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $D 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $D 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $D 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    4 stalker-alert? if
        1 ebit-clear
    then
    1 ebit? if
        0 $FE 8 action
    else
        $78 1 item-cooldown
    then
    $D pvar-inc
    exit
;

: room18.char-enter ( -- )   \ 003FAAA0
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
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    1 1 0 area-camera
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            room18.act07
        else
            2 ebit-clear
        then
    then
;

: room18.phase1 ( -- )   \ 003FAB30
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 0 1 chars-area-camera
    5 0 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 0 -1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    5 story-flag? not if
        0 2 char-left-area? if
            5 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
    $25C story-flag? not if
        0 -25.6 0.0 27.6 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 604 var-set
                $1A 605 var-set
                $1B 4 var-set
                $1C -25600 var-set
                $1D 1000 var-set
                $1E 27600 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
;

: room18.phase2 ( -- )   \ 003FACB0
    $295 story-flag? not if
        5 5 5 5 0 zone-at-effect
        0 5 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 5 5 scene-change
        else
            $8016 scene-ending
        then
    then
    5 story-flag? if
        0 $B char-in-area? 0 75 $2D char-heading? and if
            5 1 0 scene-change
        then
    then
    0 8 char-in-area? 0 45 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 9 $2D char-faces-area? if
        5 3 0 scene-change
    then
    0 $A char-in-area? 0 -67 $2D char-heading? and if
        5 4 0 scene-change
    then
    $25C story-flag? $25D story-flag? not and if
        0 4 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
;

: room18.phase3 ( -- )   \ 003FAD40
    34.0 20.0 -15.0 42.0 20.0 -15.0 34.0 0.0 -15.0 42.0 0.0 -15.0 lights-doorway
    42.0 20.0 15.0 34.0 20.0 15.0 42.0 0.0 15.0 34.0 0.0 15.0 lights-doorway
    -43.0 20.0 -15.0 -35.0 20.0 -15.0 -43.0 0.0 -15.0 -35.0 0.0 -15.0 lights-doorway
    -35.0 20.0 15.0 -43.0 20.0 15.0 -35.0 0.0 15.0 -43.0 0.0 15.0 lights-doorway
;

: room18.act00 ( -- )   \ 003FAE10
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    0 6 22.0 10.0 -24.0 0 0 sound
    $F03 $A self-anim-blend
    self-wait-anim
    $F $44 fade
    $19 3 $FF char-load
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
    wait-fade
    0 $F9 $C action
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    0 0 $14 door-bits
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
    8 state-flag-set
    0 0 char-no-shadow
    3 0 char-remove
    0 1 $14 door-bits
    4 0 object-show
    5 0 object-show
    4 room18.cmd00
    5 room18.cmd00
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $18 0 299 hewie-to-room
            1 $12B 73.05 6.36 -100 char-to-xz
        then
    then
    0 self-move-16
    0 $48 19.0 -19.0 150 char-to-xz
    hewie-controlled? not if
        0 2 -1 char-camera
        0 camera-follow
    else
        1 2 -1 char-camera
        1 camera-follow
    then
    5 story-flag-set
    $11 door-open-clear
    $11 door-unlock
    0 state-flag-set
    $FE char-activate
    $FE $18 218 2 stalker-to-room
    stalker-item-cooldown
    $FE 0 -1 char-camera
    $FE 2 stalker-mode
    0 $FE $A action
    $31 story-flag-set
    5 0 0 music
    $F $81 fade
    wait-fade
    $208 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room18.act01 ( -- )   \ 003FAF80
    self-wait-done
    $48 19.0 -19.0 150 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 1.0 10.0 0.0 0.0 event-camera
    7 message
    wait-message
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room18.act02 ( -- )   \ 0047AA48
    self-wait-done
    8 message
    wait-message
    self-idle-or-end
;

: room18.act03 ( -- )   \ 0047AA50
    self-wait-done
    9 message
    wait-message
    self-idle-or-end
;

: room18.act04 ( -- )   \ 0047AA58
    self-wait-done
    $A message
    wait-message
    self-idle-or-end
;

: room18.act06 ( -- )   \ 003FB070
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room18.act05 ( -- )   \ 003FAFD0
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 2 char-file-load
    self-wait-done
    0 char-file-use
    $141 $8004 5 -0.803 -15.664 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    0 ebit-clear
    0 avoid-prompt
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room18.act06 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? $FE char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room18.act06 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room18.act08 ( -- )   \ 003FB100
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $B2 0.866 0.417 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        0 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        0 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room18.act09 ( -- )   \ 003FB160
    self-wait-done
    -25.6 27.6 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $25D story-flag-set
            4 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
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

: room18.act0A ( -- )   \ 003FB1C0
    self-wait-done
    4 ebit-set
    $FE $DA 100 char-to-tri-facing
    0 3 self-move-slot
    self-wait-done
    4 ebit-clear
    self-idle-or-end
;

: room18.act0B ( -- )   \ 003FB1D0
    self-wait-done
    0.0 25.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $295 story-flag-set
            5 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: room18.act0C ( -- )   \ 003FB228
    begin
        8 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    self-idle-or-end
;

: room18.act0D ( -- )   \ 003FB240
    self-wait-done
    $19 3 $FF char-load
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
    0 $F9 $C action
    0 34.2 18.3 14.2 1 effect-86
    1 34.2 18.3 -14.2 1 effect-86
    2 -34.2 18.3 -14.2 1 effect-86
    3 -34.2 18.3 14.2 1 effect-86
    0 0 $14 door-bits
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
    8 state-flag-set
    0 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room18.phase5 ( -- )   \ 0047AA40
    4 ebit? if
        $FE 0 stalker-mode
    then
;

' room18.enter $18 0 room-script!
' room18.char-enter $18 6 room-script!
' room18.phase1 $18 1 room-script!
' room18.phase2 $18 2 room-script!
' room18.phase3 $18 3 room-script!
' room18.act00 $18 $00 action-script!
' room18.act01 $18 $01 action-script!
' room18.act02 $18 $02 action-script!
' room18.act03 $18 $03 action-script!
' room18.act04 $18 $04 action-script!
' room18.act05 $18 $05 action-script!
' room18.act06 $18 $06 action-script!
' room18.act07 $18 $07 action-script!
' room18.act08 $18 $08 action-script!
' room18.act09 $18 $09 action-script!
' room18.act0A $18 $0A action-script!
' room18.act0B $18 $0B action-script!
' room18.act0C $18 $0C action-script!
' room18.act0D $18 $0D action-script!
' room18.phase5 $18 5 room-script!

\ ---- room $19 ----------------------------------------------------------------------------------

\ room 0x19 (Room19_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) while
\ Hewie is controlled: in another room, or 30 or more from the player
: room19.cond00? ( -- flag )  stub-flag ;

: room19.enter ( -- )   \ 003FB3B0
    room-sounds
    $FE exit-taken? if
        0 $23F 90 char-to-tri-facing
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
        0 0 9 action
    then
    $254 story-flag? $255 story-flag? not and if
        0 34.3 1.0 -13.7 flicker-sprite
    then
    $256 story-flag? $257 story-flag? not and if
        1 56.3 51.0 -71.5 flicker-sprite
    then
    1 $2300 sound-volume
    $AF story-flag? if
        $40 story-flag? not if
            $40 story-flag-set
            \ (nop-progress-74: no effect in this game)
        then
    then
;

: room19.char-enter ( -- )   \ 003FB420
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
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    4 1 1 area-camera
    0 self-is? if
        4 exit-taken? 3 exit-taken? or if
            3 map-page
        then
    then
;

: room19.phase1 ( -- )   \ 003FB520
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 -88.0 65.0 -95.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000008 6 -88.0 65.0 -95.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $10 1 1 1 chars-area-camera
    $11 2 -1 1 chars-area-camera
    $12 1 1 1 chars-area-camera
    $13 2 -1 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? 0 7 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? 0 8 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $15 char-entered-area? 0 $16 char-entered-area? or if
        \ (nop-progress-18: no effect in this game)
    then
    0 6 char-entered-area? if
        3 map-page
    then
    0 6 char-left-area? if
        2 map-page
    then
    0 -88.0 50.0 -94.617 $1E 20 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    0 -88.0 50.0 -94.617 $1E 20 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 -9.96 0.0 43.07 $53 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $254 story-flag? not if
        5 34.3 0.0 -13.7 $A 5 0 zone
        1 5 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 596 var-set
                $1A 597 var-set
                $1B 0 var-set
                $1C 34300 var-set
                $1D 1000 var-set
                $1E -13700 var-set
                $1F 5 var-set
                0 1 $8B action
            then
        then
    then
    $256 story-flag? not if
        6 56.3 50.0 -71.5 $A 5 0 zone
        1 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 598 var-set
                $1A 599 var-set
                $1B 1 var-set
                $1C 56300 var-set
                $1D 51000 var-set
                $1E -71500 var-set
                $1F 6 var-set
                0 1 $8B action
            then
        then
    then
;

: room19.phase2 ( -- )   \ 003FB720
    0 $C char-in-area? 0 90 $2D char-heading? and if
        5 1 0 scene-change
    then
    0 $D $3C char-faces-area? if
        5 2 0 scene-change
    then
    0 $E char-in-area? 0 $F char-in-area? or 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    0 $14 char-in-area? 0 -45 $5A char-heading? and if
        5 4 0 scene-change
    then
    7 ebit? not 9 ebit? not and if
        0 $18 char-in-area? if
            2 game-mode? $FF panic-stage? not and if
                room19.cond00? if
                    1 char-here? not 1 2 char-C4? or if
                        9 ebit-set
                        $FF panic-stage? if
                            3 panic-stage
                        then
                        0 0 char-action? if
                            0 0 5 action-force
                        else
                            1 0 5 action-force
                        then
                    else
                        3 ebit-clear
                        7 0 pvar? if
                            0 chance? if
                                3 ebit-set
                            then
                        else 7 1 pvar? if
                            $32 chance? if
                                3 ebit-set
                            then
                        else 7 2 pvar? if
                            $5A chance? if
                                3 ebit-set
                            then
                        else 7 3 pvar? if
                            $5A chance? if
                                3 ebit-set
                            then
                        then then then then
                        3 ebit? not if
                            9 ebit-set
                            $FF panic-stage? if
                                3 panic-stage
                            then
                            0 0 char-action? if
                                0 0 6 action-force
                            else
                                1 0 6 action-force
                            then
                        else
                            9 ebit-set
                            $FF panic-stage? if
                                3 panic-stage
                            then
                            0 0 char-action? if
                                0 0 7 action-force
                            else
                                1 0 7 action-force
                            then
                        then
                    then
                then
            then
        then
    then
    0 $17 char-in-area? 0 -45 $32 char-heading? and if
        5 8 0 scene-change
    then
    $254 story-flag? $255 story-flag? not and if
        5 0 5 5 0 zone-at-effect
        0 5 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
    $256 story-flag? $257 story-flag? not and if
        6 1 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then
;

: room19.phase3 ( -- )   \ 003FB840
    camera-setup-changed? if
        0 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
    28.7 47.4 22.4 28.7 47.4 -29.0 74.0 47.4 22.4 74.0 47.4 -29.0 lights-doorway
    28.7 47.4 -29.0 28.7 47.4 -80.4 74.0 47.4 -29.0 74.0 47.4 -80.4 lights-doorway
    26.3 48.0 30.0 26.3 48.0 -35.9 72.1 48.0 30.0 72.1 48.0 -35.9 lights-doorway
    -55.1 48.0 -14.9 -55.1 48.0 39.6 -100.9 48.0 -14.9 -100.9 48.0 39.6 lights-doorway
    -65.1 48.0 18.6 -65.1 48.0 72.7 -110.9 48.0 18.2 -110.9 48.0 72.7 lights-doorway
    -65.1 48.0 61.0 -65.1 48.0 91.0 -110.9 48.0 61.0 -110.9 48.0 91.0 lights-doorway
;

: room19.act00 ( -- )   \ 0047AA60
    self-idle-or-end
;

: room19.act01 ( -- )   \ 003FB990
    self-wait-done
    $112 -0.195 -27.107 175 $FFFF 5 self-move-to
    self-wait-done
    $A02 self-anim
    self-wait-anim
    0 message
    wait-message
    self-idle-or-end
;

: room19.act02 ( -- )   \ 003FB9B0
    self-wait-done
    0 ebit? not if
        1 message
        wait-message
        0 ebit-set
    else
        2 message
        wait-message
    then
    self-idle-or-end
;

: room19.act03 ( -- )   \ 003FB9D0
    self-wait-done
    1 ebit? not if
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
    then
    self-idle-or-end
;

: room19.act04 ( -- )   \ 003FB9F0
    self-wait-done
    $173 33.471 79.86 -90 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 20.0 50.0 15.0 0.0 event-camera
    2 ebit? not if
        5 message
        wait-message
        2 ebit-set
    else
        6 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room19.act05 ( -- )   \ 003FBA50
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
    $F $44 fade
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
    0 $169 60.358 52.0 180 char-to-xz
    $B01 0 self-anim-blend
    camera-restart
    $FE $19 358 2 stalker-to-room
    $FE $166 55.085 70.506 180 char-to-xz
    $FE 1 1 char-camera
    1 0 char-visible
    1 action-end
    $18 state-flag-clear
    4 $1E self-anim-blend
    self-wait-anim
    $F $41 fade
    wait-fade
    $E state-flag-clear
    $64 threat-raise
    7 ebit-set
    $1F resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: room19.act06 ( -- )   \ 003FBB40
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
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
    0 $16C 63.654 67.0 -57 char-to-xz
    0 self-move-16
    camera-restart
    1 $166 56.0 62.0 -57 char-to-xz
    $FE $19 161 2 stalker-to-room
    $FE $A1 11.784 75.875 -90 char-to-xz
    $FE stalker-knock-down
    $FE 0 0 char-camera
    7 pvar-inc
    $18 state-flag-clear
    $F $41 fade
    wait-fade
    $E state-flag-clear
    10 hewie-trust
    $8278 item-give
    7 ebit-set
    $1D resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: room19.act07 ( -- )   \ 003FBC20
    $18 state-flag-set
    1 self-scripted
    $E state-flag-set
    $F $44 fade
    self-wait-done
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
    0 $16C 63.054 67.67 -57 char-to-xz
    $B01 0 self-anim-blend
    camera-restart
    1 $166 62.709 59.408 -57 char-to-xz
    $FE $19 359 2 stalker-to-room
    $FE $167 35.69 79.463 90 char-to-xz
    $FE 1 1 char-camera
    $18 state-flag-clear
    $F $41 fade
    wait-fade
    $E state-flag-clear
    7 ebit-set
    $1E resident-flag-set
    $8279 item-give
    0 self-scripted
    self-idle-or-end
;

: room19.act08 ( -- )   \ 003FBD00
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    7 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        8 state-flag-set
        $FE exit-check
    else
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room19.act09 ( -- )   \ 003FBD30
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    3 map-page
    $F 1 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room19.act0A ( -- )   \ 003FBD50
    self-wait-done
    34.3 -13.7 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $255 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room19.act0B ( -- )   \ 003FBDB0
    self-wait-done
    56.3 -71.5 self-turn-to-xz
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
            $257 story-flag-set
            1 effect-remove
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

: room19.act0C ( -- )   \ 003FBE10
    self-wait-done
    2 3 $FF char-load
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room19.act0D ( -- )   \ 003FBEA0
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room19.act0E ( -- )   \ 003FBF30
    self-wait-done
    2 3 $FF char-load
    3 char-unload
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room19.enter $19 0 room-script!
' room19.char-enter $19 6 room-script!
' room19.phase1 $19 1 room-script!
' room19.phase2 $19 2 room-script!
' room19.phase3 $19 3 room-script!
' room19.act00 $19 $00 action-script!
' room19.act01 $19 $01 action-script!
' room19.act02 $19 $02 action-script!
' room19.act03 $19 $03 action-script!
' room19.act04 $19 $04 action-script!
' room19.act05 $19 $05 action-script!
' room19.act06 $19 $06 action-script!
' room19.act07 $19 $07 action-script!
' room19.act08 $19 $08 action-script!
' room19.act09 $19 $09 action-script!
' room19.act0A $19 $0A action-script!
' room19.act0B $19 $0B action-script!
' room19.act0C $19 $0C action-script!
' room19.act0D $19 $0D action-script!
' room19.act0E $19 $0E action-script!

\ ---- room $1A ----------------------------------------------------------------------------------

\ room 0x1A (D_003FC660): the fan turns
: room1A.cmd00 ( -- )  stub-step ;
\ room 0x1A: room effect slot 0 made anew as the second TV screen (TvScreenB).
: room1A.cmd01 ( -- )  stub-step ;

: room1A.enter ( -- )   \ 003FC060
    room-sounds
    0 $F1 0 action
    room1A.cmd01
    $30A story-flag? not if
        0 1 $14 door-bits
        0 0 1 effect-string
    else
        0 0 $14 door-bits
        0 1 1 effect-string
        1 noise-level
    then
    $226 story-flag? not if
        1 -25.0 10.0 13.0 flicker-sprite
    then
;

: room1A.act07 ( -- )   \ 003FC590
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 1 char-file-load
    then
    1 ebit-clear
    8 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else 8 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else 8 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else 8 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    4 stalker-alert? if
        1 ebit-clear
    then
    1 ebit? if
        0 $FE 5 action
    else
        $78 1 item-cooldown
    then
    8 pvar-inc
    exit
;

: room1A.char-enter ( -- )   \ 003FC0A0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            4 ebit-set
            $30A story-flag? 2 creature-action? not and if
                2 self-is? 6 self-is? or 7 self-is? or if
                    $78 1 item-cooldown
                    0 $FE 6 action
                else
                    room1A.act07
                then
            else
                room1A.act07
            then
        else
            4 ebit-clear
        then
    then
;

: room1A.phase1 ( -- )   \ 003FC110
    2 ebit? not if
        6 sound-bank-loaded? if
            $30A story-flag? if
                $40000001 6 23.0 11.0 9.0 0 0 sound
            then
            2 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 27.0 15.0 -6.0 0 0 sound
            else 0 1 var? if
                $40000006 6 27.0 15.0 -6.0 0 0 sound
            else 0 2 var? if
                $40000007 6 27.0 15.0 -6.0 0 0 sound
            else 0 3 var? if
                $40000008 6 27.0 15.0 -6.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $309 story-flag? if
            $C0000001 6 23.0 11.0 9.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    5 2 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    $30A story-flag? not if
        0 1 $14 door-bits
        0 0 1 effect-string
    else
        0 0 $14 door-bits
        0 1 1 effect-string
    then
;

: room1A.phase2 ( -- )   \ 003FC220
    0 8 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 9 char-in-area? 0 60 $5A char-heading? and if
        5 2 0 scene-change
    then
    0 $A char-in-area? 0 45 $3C char-heading? and if
        $FE char-here? not if
            5 3 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 $B char-in-area? 0 -72 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 $C char-in-area? 0 0 $32 char-heading? and if
        5 9 0 scene-change
    then
    $226 story-flag? not if
        0 1 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: room1A.phase3 ( -- )   \ 003FC290
    camera-setup-changed? if
        1 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else 2 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then then
    then
;

: room1A.act00 ( -- )   \ 0047AA68
    begin
        room1A.cmd00
        yield
    again
;

: room1A.act01 ( -- )   \ 003FC2D0
    self-wait-done
    -25.0 13.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $226 story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: room1A.act02 ( -- )   \ 003FC330
    self-wait-done
    $E1 18.5 13.6 120 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $902 self-anim
    self-wait-anim
    9 0 pvar? 9 1 pvar? or if
        $30A story-flag? not if
            $30A story-flag-set
            0 0 6 char-sound
            $309 story-flag? not if
                3 $86 0.0 0.0 0.0 0 0 sound
            else
                $40000001 6 23.0 11.0 9.0 0 0 sound
            then
            $40 7 noise
            1 noise-level
        else
            0 2 6 char-sound
            1 6 sound-stop
            $30A story-flag-clear
            0 noise-level
        then
        $309 story-flag? not if
            1 self-scripted
            $5A threat-add
            1 $FF 8 rumble
            0 $43 5 char-sound
            $F04 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            $902 self-anim
            self-wait-anim
            $40000001 6 23.0 11.0 9.0 0 0 sound
            $903 self-anim
            self-wait-anim
            $309 story-flag-set
            0 self-scripted
        else
            $903 self-anim
            self-wait-anim
        then
    else
        $903 self-anim
        self-wait-anim
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room1A.act04 ( -- )   \ 003FC4C0
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room1A.act03 ( -- )   \ 003FC420
    $18 state-flag-set
    4 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $CC $8004 5 -4.7 -39.3 90 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    0 ebit-clear
    0 avoid-prompt
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room1A.act04 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                4 ebit? $FE char-here? not and if
                    4 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] room1A.act04 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room1A.act05 ( -- )   \ 003FC4F0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $CE -12.0 -37.0 90 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        0 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        0 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room1A.act06 ( -- )   \ 003FC540
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    7 14.0 18.0 120 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    $FE 2 6 char-sound
    1 6 sound-stop
    $30A story-flag-clear
    0 noise-level
    9 pvar-inc
    $FE 3 stalker-mode
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: room1A.act08 ( -- )   \ 003FC5F0
    self-wait-done
    -145 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room1A.act09 ( -- )   \ 003FC600
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $A00 self-anim
    self-frames-reset
    self-wait-16
    3 ebit? not if
        1 message
        wait-message
        3 ebit-set
    else
        2 message
        wait-message
        3 ebit-clear
    then
    self-wait-anim
    self-idle-or-end
;

' room1A.enter $1A 0 room-script!
' room1A.char-enter $1A 6 room-script!
' room1A.phase1 $1A 1 room-script!
' room1A.phase2 $1A 2 room-script!
' room1A.phase3 $1A 3 room-script!
' room1A.act00 $1A $00 action-script!
' room1A.act01 $1A $01 action-script!
' room1A.act02 $1A $02 action-script!
' room1A.act03 $1A $03 action-script!
' room1A.act04 $1A $04 action-script!
' room1A.act05 $1A $05 action-script!
' room1A.act06 $1A $06 action-script!
' room1A.act07 $1A $07 action-script!
' room1A.act08 $1A $08 action-script!
' room1A.act09 $1A $09 action-script!

\ ---- room $1B ----------------------------------------------------------------------------------

\ five pendulums (Room1B_ObjectNames[byte 3]) of their own periods and swings: byte 4 0 still at
\ a phase offset (+0x34) 60 x the index, 1 swinging on (+0x30, +0x14)
: room1B.cmd00 ( b0 b1 -- )  drop drop stub-step ;

: room1B.enter ( -- )   \ 003FC6A0
    $14 1.0 0 bgm
    $227 story-flag? not if
        0 -6.0 8.5 -5.0 flicker-sprite
    then
    0 0 room1B.cmd00
    1 0 room1B.cmd00
    3 0 room1B.cmd00
    4 0 room1B.cmd00
    0 1 room1B.cmd00
    1 1 room1B.cmd00
    3 1 room1B.cmd00
    4 1 room1B.cmd00
    0 $F1 6 action
;

: room1B.char-enter ( -- )   \ 003FC6F0
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

: room1B.phase1 ( -- )   \ 003FC770
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    1 20.5 0.0 49.99 $14 17 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 17.0 hewie-look-zone
            then
        then
    then
    2 20.64 0.0 11.01 9 10 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    2 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    3 -22.87 0.0 38.05 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    4 28.49 0.0 -29.82 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    5 1.39 0.0 -30.06 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    6 -29.89 0.0 -30.85 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    7 -29.03 0.0 -7.64 $F 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 7 9 char-zone-bits? if
                $1F 7 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: room1B.phase2 ( -- )   \ 003FC8F0
    -2147483646 scene-request? if
        5 0 1 scene-change
    then
    0 6 char-in-area? if
        5 1 0 scene-change
    then
    0 7 char-in-area? 0 0 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 90 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 $A char-in-area? 0 90 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 $B char-in-area? 0 45 $32 char-heading? and if
        5 3 0 scene-change
    then
    $227 story-flag? not if
        0 0 8 4 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    0 $C $32 char-faces-area? if
        5 4 0 scene-change
    then
;

: room1B.act00 ( -- )   \ 003FC970
    self-wait-done
    $FE self-touching? not if
        $19 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $19 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $19 door-lock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: room1B.act01 ( -- )   \ 003FC9B0
    self-wait-done
    $A01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room1B.act02 ( -- )   \ 003FC9C0
    self-wait-done
    0 self-turn-angle
    self-wait-done
    1 40.0 20.0 0.0 0.0 event-camera
    $A00 $A self-anim-blend
    1 message
    wait-message
    self-wait-anim
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room1B.act03 ( -- )   \ 003FCA00
    self-wait-done
    0 ebit? not if
        2 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
    then
    self-idle-or-end
;

: room1B.act04 ( -- )   \ 003FCA20
    self-wait-done
    1 ebit? not if
        4 message
        wait-message
        1 self-anim
        self-wait-anim
        -1 self-move-16
        6 message
        self-frames-reset
        7 self-wait-frames
        wait-message
        1 ebit-set
    else
        5 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: room1B.act05 ( -- )   \ 003FCA50
    self-wait-done
    -6.0 -5.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $227 story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: room1B.act06 ( -- )   \ 003FCAB0
    begin
        yield
        0 1 room1B.cmd00
        1 1 room1B.cmd00
        3 1 room1B.cmd00
        4 1 room1B.cmd00
    again
;

' room1B.enter $1B 0 room-script!
' room1B.char-enter $1B 6 room-script!
' room1B.phase1 $1B 1 room-script!
' room1B.phase2 $1B 2 room-script!
' room1B.act00 $1B $00 action-script!
' room1B.act01 $1B $01 action-script!
' room1B.act02 $1B $02 action-script!
' room1B.act03 $1B $03 action-script!
' room1B.act04 $1B $04 action-script!
' room1B.act05 $1B $05 action-script!
' room1B.act06 $1B $06 action-script!

\ ---- room $1C ----------------------------------------------------------------------------------

\ room 0x1C (Room1C_Cmd00_ptmf): room effect 0x1C (a depth range) with the cutscene from frame
\ 0x14A: near 1 .. 1 + 1.4 t (at most 67.6), far 48.6 + 4 t (at most 230)
: room1C.cmd00 ( -- )  stub-step ;
\ room 0x1C (D_003FD350): a sound (0xC0000000, bank 6) at the room's effect 1
: room1C.cmd01 ( -- )  stub-step ;

: room1C.act06 ( -- )   \ 003FD070
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

: room1C.enter ( -- )   \ 003FCB30
    room-sounds
    $26 door-unlocked? if
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
        room1C.act06
        1 1 $20000000 nav-group
    then
;

: room1C.char-enter ( -- )   \ 003FCBC0
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

: room1C.phase1 ( -- )   \ 003FCC40
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
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
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
        room1C.cmd01
    else
        $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    then then
;

: room1C.phase2 ( -- )   \ 003FCD70
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

: room1C.act00 ( -- )   \ 003FCDC0
    $18 state-flag-set
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
    0 self-move-16
    0 $10B -33.329 -29.383 -177 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    1 item-use
    2 item-use
    $26 door-lock
    7 story-flag-set
    room1C.act06
    0 0 $20000000 nav-group
    0 1 $1000000 nav-group
    1 1 $20000000 nav-group
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    100 hewie-trust
    self-idle-or-end
;

: room1C.act01 ( -- )   \ 003FCEA0
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

: room1C.act02 ( -- )   \ 003FCEE0
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

: room1C.act03 ( -- )   \ 003FCF30
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
        room1C.cmd00
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: room1C.act04 ( -- )   \ 0047AA70
    self-idle-or-end
;

: room1C.act05 ( -- )   \ 003FD050
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

: room1C.act07 ( -- )   \ 003FD0E0
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
    room1C.cmd01
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

: room1C.act08 ( -- )   \ 003FD1F0
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
        room1C.cmd01
        yield
    repeat
    $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    yield
    begin
        $14A cutscene-cue-reached? not while
        room1C.cmd01
        1.0 1.0 1.0 48.6 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        room1C.cmd01
        room1C.cmd00
        yield
    repeat
    wait-fade
    self-idle-or-end
;

' room1C.enter $1C 0 room-script!
' room1C.char-enter $1C 6 room-script!
' room1C.phase1 $1C 1 room-script!
' room1C.phase2 $1C 2 room-script!
' room1C.act00 $1C $00 action-script!
' room1C.act01 $1C $01 action-script!
' room1C.act02 $1C $02 action-script!
' room1C.act03 $1C $03 action-script!
' room1C.act04 $1C $04 action-script!
' room1C.act05 $1C $05 action-script!
' room1C.act06 $1C $06 action-script!
' room1C.act07 $1C $07 action-script!
' room1C.act08 $1C $08 action-script!

\ ---- room $1D ----------------------------------------------------------------------------------

\ room 0x1D (Room1D_Cmd00_ptmf): script variable 0 = 2 .. 5 at random
: room1D.cmd00 ( -- )  stub-step ;
\ room 0x1D (Room1D_Cmd01_ptmf): door 0's +0x74 (0, or -0.08 by byte 3)
: room1D.cmd01 ( b0 -- )  drop stub-step ;
\ Fiona in move 5, room 0x1D's flag 0 not set and its exit 0's door shut
: room1D.cond00? ( -- flag )  stub-flag ;

: room1D.act04 ( -- )   \ 003FD720
    $2D state-flag-clear
    1 char-here? 1 hewie-side? and if
        $1E 0 -1 hewie-to-room
        1800 2 hewie-anim
    then
    exit
;

: room1D.enter ( -- )   \ 003FD370
    room-sounds
    1 $1E char-in-room? 119 hewie-action? and if
        $2D state-flag-set
        $1D 1 252 hewie-to-room
        8 story-flag? not if
            0 1 2 action
        then
    else
        room1D.act04
    then
    0 30.0 -27.65 -9.8 0 effect-86
    8 story-flag? not if
        1 -12.0 -40.0 -25.0 flicker-sprite
    then
    $201 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $22D story-flag? not if
            2 31.0 -40.0 13.0 flicker-sprite
        then
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    $252 story-flag? $253 story-flag? not and if
        3 29.0 1.0 21.0 flicker-sprite
    then
    0 0 0 $1ED $245 obstacle-place
;

: room1D.char-enter ( -- )   \ 003FD420
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
    0 self-is? if
        0 exit-taken? if
            2 map-page
        then
    then
;

: room1D.phase1 ( -- )   \ 003FD470
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    3 1 0 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    5 3 1 1 chars-area-camera
    0 3 char-entered-area? if
        2 map-page
    then
    0 4 char-entered-area? if
        1 map-page
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    0 $1ED obstacle-on? if
        $26 door-unlocked? if
            $26 door-lock
        then
    then
    -2147483644 scene-request? not room1D.cond00? and if
        $26 door-unlocked? not if
            $26 door-unlock
        then
    then
    1 ebit? not if
        $26 door-unlocked? if
            $FE $1C char-in-room? if
                0 $F2 9 action
            then
        then
    else $26 door-unlocked? not if
        $F2 action-end
        0 room1D.cmd01
        $FE 1 stalker-mode
        $18 state-flag-clear
        1 ebit-clear
    then then
    $252 story-flag? not if
        1 29.0 0.0 21.0 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 594 var-set
                $1A 595 var-set
                $1B 3 var-set
                $1C 29000 var-set
                $1D 1000 var-set
                $1E 21000 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
;

: room1D.phase2 ( -- )   \ 003FD580
    $201 story-flag? not if
        0 31.0 -41.0 13.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $201 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            31.0 -41.0 13.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 31.0 -41.0 13.0 0 0 sound
            $40 $1B0 noise
            2 31.0 -40.0 13.0 flicker-sprite
        then
    else $22D story-flag? not if
        0 2 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then then
    0 7 $3C char-faces-area? if
        5 1 0 scene-change
    then
    1 char-here? not 1 hewie-side? not or 8 story-flag? not and 0 6 char-in-area? and 0 -12 -25 $32 char-faces-xz? and if
        5 5 0 scene-change
    then
    -2147483646 scene-request? if
        5 8 1 scene-change
    then
    0 8 char-in-area? 0 0 $32 char-heading? and if
        5 $B 0 scene-change
    then
    $252 story-flag? $253 story-flag? not and if
        1 3 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
;

: room1D.phase3 ( -- )   \ 003FD660
    camera-setup-changed? if
        1 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
;

: room1D.act00 ( -- )   \ 003FD680
    1 self-scripted
    self-wait-done
    2 wait-counter
    8 story-flag-set
    1 effect-remove
    10 hewie-trust
    3 message-param-room
    3 1 item-give-count
    0 3 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    40 hewie-trust
    $827C item-give
    counter-inc
    0 self-scripted
    self-idle-or-end
;

: room1D.act01 ( -- )   \ 0047AA88
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: room1D.act03 ( -- )   \ 003FD700
    1 wait-counter
    hewie-bark
    self-wait-done
    $92 -17.0 -30.0 45 $FFFF 5 self-move-to
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    counter-inc
    3 wait-counter
    self-idle-or-end
;

: room1D.act02 ( -- )   \ 003FD6C0
    self-wait-done
    1 $FC 0 char-to-tri-facing
    1 self-anim
    self-wait-anim
    0 self-look-at
    yield
    begin
        35 fiona-started? 44 fiona-started? or 2 game-mode? not and 0 6 char-in-area? and 0 -12 -25 $32 char-faces-xz? and if
            0 counter-set
            0 $F1 6 action
            ['] room1D.act03 goto
        then
        yield
    again
;

: room1D.act05 ( -- )   \ 003FD740
    self-wait-done
    $A01 $A self-anim-blend
    self-wait-anim
    0 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    $FF 4 -1 char-camera
    self-frames-reset
    8 self-wait-frames
    1 message
    wait-message
    self-frames-reset
    8 self-wait-frames
    hewie-controlled? not if
        0 3 1 char-camera
        0 camera-follow
    else
        1 3 1 char-camera
        1 camera-follow
    then
    camera-restart
    self-idle-or-end
;

: room1D.act06 ( -- )   \ 003FD780
    begin
        fiona-free? not while
        yield
    repeat
    counter-inc
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 0 action-force
    self-idle-or-end
;

: room1D.act07 ( -- )   \ 003FD7A0
    self-wait-done
    31.0 13.0 self-turn-to-xz
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
            $22D story-flag-set
            2 effect-remove
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

: room1D.act08 ( -- )   \ 003FD7F8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room1D.act09 ( -- )   \ 003FD810
    1 ebit-set
    $18 state-flag-set
    self-frames-reset
    $3C self-wait-frames
    room1D.cmd00
    begin
        1 6 -40.0 0.0 -20.0 0 0 sound
        1 room1D.cmd01
        yield
        0 room1D.cmd01
        $19 chance? if
            self-frames-reset
            self-wait-16
        then
        self-frames-reset
        self-wait-16
        1 6 -40.0 0.0 -20.0 0 0 sound
        1 room1D.cmd01
        yield
        0 room1D.cmd01
        $32 chance? if
            self-frames-reset
            self-wait-16
            self-frames-reset
            self-wait-16
        then
        self-frames-reset
        $3C self-wait-frames
        0 0 var? not while
        0 var-dec
        yield
    repeat
    $26 door-unlocked? if
        $31A story-flag? not if
            $31A story-flag-set
        else $4B chance? if
            $31B story-flag-set
        then then
        $FE action-end
        $FE char-done
    else
        $FE 1 stalker-mode
    then
    $18 state-flag-clear
    1 ebit-clear
    self-idle-or-end
;

: room1D.act0A ( -- )   \ 003FD8A0
    self-wait-done
    29.0 21.0 self-turn-to-xz
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
            $253 story-flag-set
            3 effect-remove
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

: room1D.act0B ( -- )   \ 003FD900
    self-wait-done
    $2FD story-flag? not if
        0 self-turn-angle
        self-wait-done
        4 message
        wait-message
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $2FD story-flag-set
            $70 1 item-give-count
            0 $70 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: room1D.phase5 ( -- )   \ 0047AA80
    room1D.act04
;

' room1D.enter $1D 0 room-script!
' room1D.char-enter $1D 6 room-script!
' room1D.phase1 $1D 1 room-script!
' room1D.phase2 $1D 2 room-script!
' room1D.phase3 $1D 3 room-script!
' room1D.act00 $1D $00 action-script!
' room1D.act01 $1D $01 action-script!
' room1D.act02 $1D $02 action-script!
' room1D.act03 $1D $03 action-script!
' room1D.act04 $1D $04 action-script!
' room1D.act05 $1D $05 action-script!
' room1D.act06 $1D $06 action-script!
' room1D.act07 $1D $07 action-script!
' room1D.act08 $1D $08 action-script!
' room1D.act09 $1D $09 action-script!
' room1D.act0A $1D $0A action-script!
' room1D.act0B $1D $0B action-script!
' room1D.phase5 $1D 5 room-script!

\ ---- room $1E ----------------------------------------------------------------------------------

: room1E.enter ( -- )   \ 003FD9C0
    room-sounds
    $1A 1.0 0 bgm
    1 char-here? 119 hewie-action? and if
        0 0 8 nav-group
        1 0 $B0 nav-group
        1 $12C char-on-tri? not if
            1 $12C -12.0 -106.0 0 char-to-xz
            1800 2 hewie-anim
        then
    else
        0 0 $30 nav-group
        1 0 $88 nav-group
        1 0 char-in-nav-group? if
            1 $47 char-to-tri
        then
    then
    $10 41.5 40.7 -137.7 $E $80 $80 $80 $40 specks
    $10 5.9 34.6 -158.4 $E $80 $80 $80 $40 specks
;

: room1E.char-enter ( -- )   \ 003FDA40
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
;

: room1E.phase1 ( -- )   \ 003FDB00
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? 0 7 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    1 char-here? if
        118 hewie-action? not if
            4 -11.187 5.0 -117.743 $1E 20 0 zone
            5 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 4 9 char-zone-bits? if
                            5 ebit-set
                            $1E chance? if
                                $1F 4 var-set
                                0 1 $88 action
                            then
                        then
                    then
                then
            then
            35 fiona-started? 1 2 char-C4? not and if
                0 8 char-in-area? $FE char-here? not and 0 -13 -110 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 0 action
                    then
                else $236 story-flag? not if
                    2 game-mode? not if
                        68 0 -274 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                            0 68 -274 $32 char-faces-xz? 1 char-busy? not and if
                                hewie-stays? if
                                    0 0 7 action
                                then
                            then
                        then
                    then
                then then
            then
        else
            44 fiona-started? if
                0 1 1 action
            then
            $FF panic-stage? not if
                0 char-busy? not $A state-flag? or if
                    2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
                        $FE 2 char-C4? not $FE 8 char-in-area? and if
                            0 counter-set
                            0 1 3 action
                            0 $FE 4 action
                            $FF panic-stage? if
                                3 panic-stage
                            then
                            0 0 char-action? if
                                0 0 2 action-force
                            else
                                1 0 2 action-force
                            then
                        then
                    then
                then
            then
        then
    then
    6 46.51 0.0 -267.37 $1C 6 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 ebit? not if
        0 46.0 0.0 -280.0 $10 4 0 zone
        0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
            0 5 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
            1 $A 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
        $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
            $FE 5 46.0 0.0 -280.0 2 $40 $50 $40 $80 splash
            1 ebit-set
        then
    then
    2 ebit? not if
        1 52.0 0.0 -240.0 $10 4 0 zone
        0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
            0 5 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
            1 $A 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
        $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
            $FE 5 52.0 0.0 -240.0 2 $40 $50 $40 $80 splash
            2 ebit-set
        then
    then
    3 ebit? not if
        2 50.0 0.0 -300.0 $10 4 0 zone
        0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
            0 5 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
        1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
            1 $A 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
        $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
            $FE 5 50.0 0.0 -300.0 2 $40 $50 $40 $80 splash
            3 ebit-set
        then
    then
    4 ebit? not if
        3 70.0 0.0 -260.0 $10 4 0 zone
        0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
            0 5 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
        1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
            1 $A 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
        $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
            $FE 5 70.0 0.0 -260.0 2 $40 $50 $40 $80 splash
            4 ebit-set
        then
    then
    6 ebit? not if
        5 73.0 0.0 -282.0 $10 4 0 zone
        0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
            0 5 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
        1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
            1 $A 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
        $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
            $FE 5 73.0 0.0 -282.0 2 $40 $50 $40 $80 splash
            6 ebit-set
        then
    then
;

: room1E.phase2 ( -- )   \ 003FDEC8
    0 9 char-in-area? 0 0 $32 char-heading? and if
        5 5 0 scene-change
    then
;

: room1E.phase5 ( -- )   \ 003FDEE0
    1 char-busy? if
        1 action-end
        1 $118 39.008 -266.892 -80 char-to-xz
        $18 state-flag-clear
    then
;

: room1E.act00 ( -- )   \ 003FDF00
    self-wait-done
    hewie-bark
    self-wait-done
    $47 -12.0 -125.0 0 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 0 8 nav-group
    1 0 $30 nav-group
    $12C -12.0 -106.0 0 $204 5 self-move-to
    self-wait-done
    3600 2 hewie-anim
    self-idle-or-end
;

: room1E.act01 ( -- )   \ 003FDF40
    1 self-scripted
    self-wait-done
    $47 -12.0 -125.0 180 $204 5 self-move-to
    self-wait-done
    -1 self-move-16
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-action
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: room1E.act02 ( -- )   \ 003FDF80
    $18 state-flag-set
    1 self-scripted
    $F $44 fade
    $FF 1.0 0 bgm
    self-wait-done
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
    $FF 1 char-visible
    $1B state-flag-set
    yield
    $1B state-flag-clear
    0 $F9 9 action
    $10 $28 movie-param
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
    $FE action-end
    3 summon-take
    0 self-move-16
    $FF 0 char-visible
    1 0 0 char-tint
    0 3 0.0 light
    counter-inc
    $1A 1.0 0 bgm
    $F $41 fade
    wait-fade
    $22E item-give
    $23 resident-flag-set
    5 hewie-trust
    $827A item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room1E.act03 ( -- )   \ 003FE060
    1 self-scripted
    1 wait-counter
    0 0 hewie-anim
    0 0 hewie-action
    1 $2F -90 char-to-tri-facing
    $11 state-flag-clear
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: room1E.act04 ( -- )   \ 0047AA90
    1 self-scripted
    1 wait-counter
    0 self-scripted
    self-idle-or-end
;

: room1E.act05 ( -- )   \ 003FE090
    self-wait-done
    $32 -10.776 -128.378 0 $FFFF 5 self-move-to
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    4 self-wait-frames
    1 char-here? 118 hewie-action? and 1 0 char-in-nav-group? and if
        2 message
        wait-message
    else 0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
        0 ebit-clear
    then then
    self-frames-reset
    4 self-wait-frames
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    self-idle-or-end
;

: room1E.act06 ( -- )   \ 0047AA98
    self-idle-or-end
;

: room1E.act07 ( -- )   \ 003FE0E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 8 action-force
    else
        $18 state-flag-clear
    then
    $C04 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    self-idle-or-end
;

: room1E.act08 ( -- )   \ 003FE110
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $118 39.008 -266.892 100 $204 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    1 self-noclip
    1 0 6 char-sound
    $13F 68.224 -274.752 100 $204 5 self-move-to
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    $A state-flag? 0 char-busy? not or if
        $73 $63 item-count? not if
            10 hewie-trust
        then
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $236 story-flag-set
            $73 1 item-give-count
            0 $73 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    then
    1 0 6 char-sound
    $118 39.008 -266.892 -80 $FFFF $A self-move-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room1E.act09 ( -- )   \ 003FE1C0
    begin
        0 cutscene-shot? not while
        yield
    repeat
    begin
        0 cutscene-shot? while
        1 $C8000000 1 char-tint
        yield
    repeat
    1 0 0 char-tint
    begin
        $1E3 cutscene-cue-reached? not while
        yield
    repeat
    2 3 0.5 light
    self-idle-or-end
;

: room1E.act0A ( -- )   \ 003FE1F0
    self-wait-done
    $10 41.5 40.7 -137.7 $E $80 $80 $80 $40 specks
    $10 5.9 34.6 -158.4 $E $80 $80 $80 $40 specks
    2 3 $FF char-load
    3 char-unload
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
    $FF 1 char-visible
    0 $F9 9 action
    $10 $28 movie-param
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
    3 0 char-remove
    $FF 0 char-visible
    1 0 0 char-tint
    0 3 0.0 light
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room1E.enter $1E 0 room-script!
' room1E.char-enter $1E 6 room-script!
' room1E.phase1 $1E 1 room-script!
' room1E.phase2 $1E 2 room-script!
' room1E.phase5 $1E 5 room-script!
' room1E.act00 $1E $00 action-script!
' room1E.act01 $1E $01 action-script!
' room1E.act02 $1E $02 action-script!
' room1E.act03 $1E $03 action-script!
' room1E.act04 $1E $04 action-script!
' room1E.act05 $1E $05 action-script!
' room1E.act06 $1E $06 action-script!
' room1E.act07 $1E $07 action-script!
' room1E.act08 $1E $08 action-script!
' room1E.act09 $1E $09 action-script!
' room1E.act0A $1E $0A action-script!

\ ---- room $1F ----------------------------------------------------------------------------------

\ two wheels (Room1F_ObjectNames) rocking 4 degrees (+0x18) through their phase +0x30, 6 degrees
\ a step (byte 3 1; 0 reset), the first one's creak (-366, 30, -25) at each turn
: room1F.cmd00 ( b0 -- )  drop stub-step ;

: room1F.enter ( -- )   \ 003FE300
    room-sounds
    0 room1F.cmd00
    0 $F1 1 action
    $A -33.7 23.2 -126.0 $C $60 $60 $60 $20 specks
    $C -92.2 23.2 -92.0 $C $60 $60 $60 $20 specks
    $A -118.0 23.2 -48.5 $C $60 $60 $60 $20 specks
    1 $2300 sound-volume
;

: room1F.char-enter ( -- )   \ 003FE350
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
;

: room1F.phase1 ( -- )   \ 003FE3D0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 -366.29 0.0 -24.86 $23 29 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 29.0 hewie-look-zone
            then
        then
    then
;

: room1F.phase2 ( -- )   \ 003FE420
    0 4 char-in-area? 0 30 $32 char-heading? and if
        5 0 0 scene-change
    then
;

: room1F.act00 ( -- )   \ 003FE430
    self-wait-done
    60 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: room1F.act01 ( -- )   \ 0047AAA8
    begin
        1 room1F.cmd00
        yield
    again
;

' room1F.enter $1F 0 room-script!
' room1F.char-enter $1F 6 room-script!
' room1F.phase1 $1F 1 room-script!
' room1F.phase2 $1F 2 room-script!
' room1F.act00 $1F $00 action-script!
' room1F.act01 $1F $01 action-script!

\ ---- room $20 ----------------------------------------------------------------------------------

\ room 0x20 (Room20_Cmd01_ptmf): the falling object back up in place
: room20.cmd01 ( -- )  stub-step ;
\ a pendulum (room object Room20_ObjectNames[byte 3]): byte 4 0 still; 1 its phase +0x30 on by 2
\ degrees (a tick sound at (-85, 30, 90) each turn), swinging 15 degrees (+0x14)
: room20.cmd02 ( b0 b1 -- )  drop drop stub-step ;
\ room 0x20 (Room20_Cmd03_ptmf): the falling object falls (gravity 0.2 a frame on its velocity
\ +0x30, spinning 1 degree a frame) along the nav mesh, events bit 1 set while it lies on open
\ floor; landing on floor that isn't 0x10000 raises dust
: room20.cmd03 ( -- )  stub-step ;
\ room 0x20 (Room20_Cmd05_ptmf): the character turns to face object byte 3
: room20.cmd05 ( b0 -- )  drop stub-step ;
\ room 0x20 (Room20_Cond00_ptmf): the player, free and within 5 of the falling object, knocks it
\ - it gets a push (0, 1, 1) turned by her facing, and its nav triangle
: room20.cond00? ( -- flag )  stub-flag ;
\ room 0x20 (Room20_Cond01_ptmf): the character's script value is at least be32 bytes 3..6
: room20.cond01? ( b0 b1 b2 b3 -- flag )  drop drop drop drop stub-flag ;
\ room 0x20 (Room20_Cond02_ptmf): object byte 3 becomes event point byte 4 (radii 5)
: room20.cond02? ( b0 b1 -- flag )  drop drop stub-flag ;

: room20.enter ( -- )   \ 003FE460
    room-sounds
    $D story-flag? not if
        0 $F2 9 action
    then
    $1C story-flag? not if
        0 0 0 $36B $3AF obstacle-place
    else
        0 0 0 $377 $3BB obstacle-place
        0 obstacle-stop
    then
    0 23.3 12.75 87.0 0 effect-86
    1 25.15 12.75 86.0 0 effect-86
    $24A story-flag? $24B story-flag? not and if
        2 -11.6 1.0 142.2 flicker-sprite
    then
    $A story-flag? if
        $2E story-flag? not if
            $2E story-flag-set
            \ (nop-progress-74: no effect in this game)
            \ (nop-progress-74: no effect in this game)
        then
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 0 room20.cmd02
    4 0 room20.cmd02
    0 $F4 $11 action
    5 1 object-show
    $349 story-flag? not if
        1 ebit-clear
        $D story-flag? $31C story-flag? not and if
            5 0 object-show
            room20.cmd01
            1 ebit-set
            7 ebit-set
        then
    then
    5 story-flag? 6 story-flag? not and if
        $80 exit-taken? if
        else
            0 $F3 $E action
        then
    then
;

: room20.char-enter ( -- )   \ 003FE540
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    3 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    4 3 3 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $142 0 char-to-tri-facing
            0 0 0 action
        then
    then
;

: room20.phase1 ( -- )   \ 003FE680
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    5 story-flag? 6 story-flag? not and if
        2 exit-usable? if
            3 3 2 char-load
            4 4 2 char-load
            2 exit-check
        then
    else 2 exit-usable? if
        2 exit-check
    then then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $D 3 3 1 chars-area-camera
    $E 1 1 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    $10 2 2 1 chars-area-camera
    $14 3 3 1 chars-area-camera
    $15 0 0 1 chars-area-camera
    $1A 1 1 1 chars-area-camera
    0 5 char-entered-area? 0 $B char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? 0 7 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-entered-area? if
        5 story-flag? 6 story-flag? not and if
            3 3 2 char-load
            4 4 2 char-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 $C char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 8 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $A char-entered-area? if
        5 story-flag? 6 story-flag? not and if
            3 0 char-remove
            4 0 char-remove
        then
        \ (nop-progress-14: no effect in this game)
    then
    $D story-flag? not if
        0 $17 char-entered-area? 8 ebit? not and if
            8 ebit-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 3 action-force
            else
                1 0 3 action-force
            then
        then
    then
    $1C story-flag? not if
        0 $377 obstacle-on? if
            $1C story-flag-set
            0 obstacle-stop
            $12 door-lock
            \ (nop-progress-24: no effect in this game)
        then
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    $24A story-flag? not if
        2 -11.6 0.0 142.2 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 586 var-set
                $1A 587 var-set
                $1B 2 var-set
                $1C -11600 var-set
                $1D 1000 var-set
                $1E 142200 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    $349 story-flag? not if
        1 ebit? if
            room20.cond00? if
                0 4 6 char-sound
                1 ebit-clear
                7 ebit-clear
                $F1 action-end
                0 $F1 $B action
            then
            5 6 room20.cond02? if
            then
        then
    then
    5 story-flag? 6 story-flag? not and if
        1 2 char-in-area? not if
            1 -144.0 0.0 109.0 $1E 20 0 zone
            3 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 1 9 char-zone-bits? if
                            3 ebit-set
                            $64 chance? if
                                $1F 1 var-set
                                0 1 $8A action
                            then
                        then
                    then
                then
            then
        then
    then
    $1C story-flag? 1 4 char-in-area? not and if
        0 33.656 0.0 111.061 $A 20 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        2 ebit-set
                        $1E chance? if
                            $1F 0 var-set
                            0 1 $89 action
                        then
                    then
                then
            then
        then
    then
    $FE $1B char-in-area? 4 stalker-alert? and $FE 2 char-C4? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 $FE $D action
        then
    then
    3 -177.49 0.0 89.35 $12 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 55.23 0.0 -184.17 $14 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    5 -84.16 0.0 90.89 $12 28 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 28.0 hewie-look-zone
            then
        then
    then
    6 sound-bank-loaded? if
        $D story-flag? not if
            $C0000002 6 -82.0 0.0 92.0 0 0 sound
        then
        $C0000001 6 -85.0 30.0 90.0 0 0 sound
    then
;

: room20.phase2 ( -- )   \ 003FE9C0
    0 2 char-group-bit4? if
        5 story-flag? 6 story-flag? not and $FE char-here? not and if
            5 2 0 scene-change
        then
        6 story-flag? -2147483646 scene-request? and if
            5 5 1 scene-change
        then
    then
    0 $11 $2D char-faces-area? if
        5 6 0 scene-change
    then
    0 $12 $2D char-faces-area? if
        5 7 0 scene-change
    then
    0 $13 $2D char-faces-area? if
        5 8 0 scene-change
    then
    $1C story-flag? not 0 $16 char-in-area? and if
        5 1 0 scene-change
    then
    0 $18 char-in-area? 0 $19 char-in-area? or 0 0 $32 char-heading? and if
        5 $A 0 scene-change
    then
    $24A story-flag? $24B story-flag? not and if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $C 4 scene-change
        then
    then
    $349 story-flag? not if
        1 ebit? if
            5 6 room20.cond02? if
                0 6 3 char-zone-bits? if
                    5 $F 0 scene-change
                then
            then
        then
    then
;

: room20.phase3 ( -- )   \ 003FEA60
    107.0 8.0 -164.5 85.8 8.3 -178.2 107.3 -25.5 -164.4 85.8 -25.5 -178.2 lights-doorway
    87.9 8.3 -176.8 50.5 8.3 -179.8 87.9 -25.5 -176.8 87.9 -25.5 -176.8 lights-doorway
;

: room20.phase5 ( -- )   \ 003FEAD0
    $FE char-busy? if
        4 ebit? if
            $FE action-end
            0 summon-take
        then
    then
    6 story-flag? $11 door-unlocked? and if
        $11 door-lock
    then
;

: room20.act00 ( -- )   \ 003FEAF0
    1 self-scripted
    self-wait-done
    $F02 0 self-anim-blend
    6 story-flag-set
    8 state-flag-clear
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    camera-restart
    $F $41 fade
    wait-fade
    $209 item-give
    0 ebit-set
    $18 state-flag-clear
    $64 threat-raise
    self-wait-anim
    0 self-scripted
    self-idle-or-end
;

: room20.act01 ( -- )   \ 0047AAC0
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: room20.act02 ( -- )   \ 003FEB30
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $11 self-through-door
    self-wait-done
    $609 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $F3 action-end
    $A02 self-anim
    self-frames-reset
    $16 self-wait-frames
    $F $44 fade
    wait-fade
    8 state-flag-set
    3 char-unload
    4 char-unload
    0 1 char-visible
    $80 exit-check
    self-idle-or-end
;

: room20.act03 ( -- )   \ 003FEB70
    self-wait-done
    $F2 action-end
    $F $54 fade
    wait-fade
    $FE $20 552 2 stalker-to-room
    $FE action-end
    $FE char-done
    0 1 char-no-shadow
    $FE 1 char-no-shadow
    1 4 movie-play
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
    8 state-flag-set
    0 0 char-no-shadow
    $FE 0 char-no-shadow
    $FE action-end
    $FE char-done
    0 2 char-file-load
    0 char-file-use
    $FE char-activate
    $FE 0 stalker-mode
    $349 story-flag? not if
        room20.cmd01
        1 ebit-set
    then
    $FE $30A -7.812 78.949 179 char-to-xz
    $FE 3 3 char-camera
    0 $FE 4 action
    0 $2E9 -8.354 49.017 -10 char-to-xz
    hewie-controlled? not if
        0 4 4 char-camera
        0 camera-follow
    else
        1 4 4 char-camera
        1 camera-follow
    then
    camera-restart
    $1B state-flag-clear
    $8001 0 self-anim-blend
    $D story-flag-set
    8 state-flag-clear
    5 0 0 music
    $F $91 fade
    $8003 self-anim
    self-wait-anim
    stalker-item-cooldown
    wait-fade
    $224 item-give
    self-idle-or-end
;

: room20.act04 ( -- )   \ 003FEC80
    self-wait-done
    0 self-look-at
    yield
    $2302 $A self-anim-blend
    self-wait-anim
    self-frames-reset
    begin
        $19 chance? if
            $1302 $A self-anim-blend
            self-wait-anim
        else $19 chance? if
            $1305 $A self-anim-blend
            self-wait-anim
        then then
        4 camera-mode? not 0 0 1 $C2 room20.cond01? or if
            self-idle-or-end
        else
            yield
        then
    again
;

: room20.act05 ( -- )   \ 003FECB8
    self-wait-done
    0 ebit? if
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room20.act06 ( -- )   \ 003FECD0
    self-wait-done
    $304 story-flag? not if
        $304 story-flag-set
        0 $43 5 char-sound
        $F00 self-anim
        $5A threat-add
        1 $FF 8 rumble
        self-frames-reset
        self-wait-16
        2 message
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        wait-message
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: room20.act07 ( -- )   \ 003FED00
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room20.act08 ( -- )   \ 003FED10
    self-wait-done
    $292 -173.0 90.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 6.0 event-camera
    self-frames-reset
    $10 self-wait-frames
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    4 message
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room20.act09 ( -- )   \ 003FED60
    0 180 var-set
    begin
        yield
        6 sound-bank-loaded? if
            0 180 var? if
                0 0 var-set
                $40000002 6 -82.0 0.0 92.0 0 0 sound
            else
                0 var-inc
            then
        then
    again
;

: room20.act0A ( -- )   \ 003FEDA0
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    7 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room20.act0B ( -- )   \ 003FEDB8
    $31C story-flag-set
    begin
        room20.cmd03
        yield
    again
;

: room20.act0C ( -- )   \ 003FEDD0
    self-wait-done
    -11.6 142.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $24B story-flag-set
            2 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
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

: room20.act0D ( -- )   \ 003FEE30
    4 ebit-set
    self-wait-done
    $FE 6 char-file-load
    $1A2 77.27 -192.48 0 $FFFF $A self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    $8000 5 self-anim-9
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        self-at-motion-event? not while
        yield
    repeat
    0 self-noclip
    0 self-scripted
    4 ebit-clear
    $FE action-end
    0 summon-take
    self-idle-or-end
;

: room20.act0E ( -- )   \ 003FEE70
    begin
        5 ebit? not if
            5 ebit-set
            $1E chance? if
                1 31 var-set
            else $32 chance? if
                1 61 var-set
            else
                1 1 var-set
            then then
        then
        yield
        1 var-dec
        1 0 var? 1 30 var? or 1 60 var? or 1 90 var? or if
            3 6 -145.0 10.0 150.0 0 0 sound
            1 0 var? if
                5 ebit-clear
            then
            $1E chance? if
                self-frames-reset
                $1E self-wait-frames
            else $32 chance? if
                self-frames-reset
                $2D self-wait-frames
            else
                self-frames-reset
                $5A self-wait-frames
            then then
        then
    again
;

: room20.act0F ( -- )   \ 003FEEF0
    self-wait-done
    5 room20.cmd05
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $93 message-param-room
    $93 $63 item-count? if
        $8010 message
        wait-message
    else
        $93 1 item-give-count
        $349 story-flag-set
        5 1 object-show
        0 $93 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8011 message
        wait-message
    then
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: room20.act10 ( -- )   \ 003FEF40
    self-wait-done
    2 3 $FF char-load
    3 char-unload
    0 0 0 $36B $3AF obstacle-place
    0 23.3 12.75 87.0 0 effect-86
    1 25.15 12.75 86.0 0 effect-86
    3 0 room20.cmd02
    4 0 room20.cmd02
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 1 char-no-shadow
    $FE 1 char-no-shadow
    1 4 movie-play
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
    8 state-flag-set
    0 0 char-no-shadow
    $FE 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room20.act11 ( -- )   \ 003FF018
    begin
        3 1 room20.cmd02
        4 1 room20.cmd02
        yield
    again
;

' room20.enter $20 0 room-script!
' room20.char-enter $20 6 room-script!
' room20.phase1 $20 1 room-script!
' room20.phase2 $20 2 room-script!
' room20.phase3 $20 3 room-script!
' room20.phase5 $20 5 room-script!
' room20.act00 $20 $00 action-script!
' room20.act01 $20 $01 action-script!
' room20.act02 $20 $02 action-script!
' room20.act03 $20 $03 action-script!
' room20.act04 $20 $04 action-script!
' room20.act05 $20 $05 action-script!
' room20.act06 $20 $06 action-script!
' room20.act07 $20 $07 action-script!
' room20.act08 $20 $08 action-script!
' room20.act09 $20 $09 action-script!
' room20.act0A $20 $0A action-script!
' room20.act0B $20 $0B action-script!
' room20.act0C $20 $0C action-script!
' room20.act0D $20 $0D action-script!
' room20.act0E $20 $0E action-script!
' room20.act0F $20 $0F action-script!
' room20.act10 $20 $10 action-script!
' room20.act11 $20 $11 action-script!

\ ---- room $21 ----------------------------------------------------------------------------------

: room21.cmd00 ( -- )  stub-step ;
\ room 0x21 (D_00400B98): the fan turns, except while a movie plays
: room21.cmd01 ( -- )  stub-step ;
\ room 0x21 (D_00400BA8)
: room21.cmd02 ( b0 -- )  drop stub-step ;
\ Sets bit 1 of the flag byte three times (inlined setter calls); returns 1.
: room21.cmd03 ( -- )  stub-step ;
\ room 0x21 (D_00400BC8)
: room21.cmd04 ( b0 -- )  drop stub-step ;
\ room 0x21 (Room21_Cmd05_ptmf): the player's model +0xC8 vector by byte 3
: room21.cmd05 ( b0 -- )  drop stub-step ;
\ room 0x21 (Room21_Cmd06_ptmf): Fiona's model +0x1570 (byte 3 0) / +0x1574 (1) = byte 4
: room21.cmd06 ( b0 b1 -- )  drop drop stub-step ;
\ room 0x21 (Room21_Cond00_ptmf): the pursuer, about and not in state 2, is in its mode 2 but in
\ another room than the current one (the player about too)
: room21.cond00? ( -- flag )  stub-flag ;

: room21.enter ( -- )   \ 003FF170
    room-sounds
    room21.cmd00
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
        1 noise-level
    then
    0 $F1 $12 action
    $25E story-flag? $25F story-flag? not and if
        2 -9.0 1.0 -11.0 flicker-sprite
    then
    0 story-flag? not if
        $3D door-unlock
    then
    2 story-flag? 3 story-flag? not and if
        $3D door-unlock
    then
    $17 story-flag? 0 story-flag? not and if
        1 1 $14 door-bits
    else
        1 0 $14 door-bits
    then
    0 0 var-set
    1 0 var-set
    1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 room21.cmd02
    0 room21.cmd04
;

: room21.act04 ( -- )   \ 003FFA90
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE $A char-file-load
    then
    5 ebit-clear
    2 0 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 1 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 2 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else 2 3 pvar? if
        $32 chance? if
            5 ebit-set
        then
    then then then then
    $C ebit? if
        5 ebit-clear
        $C ebit-clear
        $1F state-flag-clear
    else
        2 creature-action? if
            5 ebit-set
        then
        4 stalker-alert? if
            5 ebit-clear
        then
    then
    5 ebit? if
        0 $FE $B action
    else
        $78 1 item-cooldown
    then
    2 pvar-inc
    exit
;

: room21.char-enter ( -- )   \ 003FF200
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
        $1B story-flag? not $D story-flag? and if
            $3A story-flag? not if
                $3A story-flag-set
                0 exit-taken? $FF panic-stage? not and if
                    room21.cond00? if
                        0 0 $23 action
                    then
                then
            then
        then
        $80 exit-taken? if
            1 story-flag? 2 story-flag? not and if
                $1B state-flag-clear
                0.0 sound-volume-scale
                hewie-controlled? not if
                    0 2 2 char-camera
                    0 camera-follow
                else
                    1 2 2 char-camera
                    1 camera-follow
                then
                0 $A8 45 char-to-tri-facing
                8 state-flag-set
                0 0 8 action
            then
        then
        $81 exit-taken? if
            $18 story-flag? not if
                3 story-flag? $B story-flag? not and if
                    0.0 sound-volume-scale
                    0 $128 -38.951 -26.464 90 char-to-xz
                    0 0 9 action
                    $18 story-flag-set
                then
            then
        then
    then
    1 self-is? if
    then
    $FE self-is? if
        9 state-flag? if
            $B ebit-set
            $300 story-flag? 2 creature-action? not and if
                2 self-is? 6 self-is? or 7 self-is? or if
                    $78 1 item-cooldown
                    0 $FE 6 action
                else
                    room21.act04
                then
            else
                room21.act04
            then
        else
            $B ebit-clear
        then
    then
;

: room21.phase1 ( -- )   \ 003FF340
    3 ebit? not if
        6 sound-bank-loaded? if
            $300 story-flag? if
                $40000001 6 0.0 3.0 7.0 0 0 sound
            then
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            3 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000001 6 0.0 3.0 7.0 0 0 sound
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
    then
    0 story-flag? not if
        $17 story-flag? not if
            0 $E char-entered-area? $D ebit? not and if
                $D ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 7 action-force
                else
                    1 0 7 action-force
                then
            then
        then
    then
    $25E story-flag? not if
        7 -9.0 0.0 -11.0 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 606 var-set
                $1A 607 var-set
                $1B 2 var-set
                $1C -9000 var-set
                $1D 1000 var-set
                $1E -11000 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
    0 -1.41 -8.0 8.99 $D 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 -46.87 0.0 -38.5 $14 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    2 -39.23 0.0 -59.47 $14 6 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    3 -37.71 -8.0 63.24 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 46.35 0.0 -40.12 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    5 39.18 0.0 -19.5 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    6 -0.22 -8.0 59.41 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: room21.phase2 ( -- )   \ 003FF650
    0 4 char-in-area? 0 90 $5A char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 0 0 scene-change
        then
    then
    0 story-flag? not if
        $17 story-flag? if
            0 $10 char-in-area? 0 90 $32 char-heading? and if
                5 $D 0 scene-change
            then
        then
        -2147483646 scene-request? if
            5 $C 1 scene-change
        then
    else 2 story-flag? 3 story-flag? not and if
        -2147483646 scene-request? if
            5 $E 1 scene-change
        then
        0 5 char-in-area? 0 90 $3C char-heading? and if
            5 $F 0 scene-change
        then
        0 $D char-in-area? 0 22 $3C char-heading? and if
            5 $10 0 scene-change
        then
    else 0 5 char-in-area? 0 90 $3C char-heading? and if
        $D story-flag? not if
            5 $16 0 scene-change
        else $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then then
    then then then
    0 $B char-in-area? 0 -67 $3C char-heading? and if
        5 $14 0 scene-change
    then
    0 $C char-in-area? 0 45 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $15 0 scene-change
        then
    then
    0 $11 char-in-area? 0 0 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $17 0 scene-change
        then
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $18 0 scene-change
        then
    then
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $1C 0 scene-change
        then
    then
    0 $13 $32 char-faces-area? if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $1D 0 scene-change
        then
    then
    0 $14 char-in-area? 0 -45 $32 char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else
            5 $1E 0 scene-change
        then
    then
    0 $15 char-in-area? 0 90 $32 char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else $AE story-flag? not if
            5 $1F 0 scene-change
        else
            5 $26 0 scene-change
        then then
    then
    0 $D char-in-area? 0 $16 char-in-area? or 0 22 $3C char-heading? and if
        0 story-flag? not if
            5 $C 0 scene-change
        else 2 story-flag? not if
            5 $20 0 scene-change
        else
            5 $22 0 scene-change
        then then
    then
    $17 story-flag? if
        0 7 char-in-area? 0 -45 $32 char-heading? and if
            0 story-flag? not if
                5 $C 0 scene-change
            else
                5 $84 3 scene-change
            then
        then
    then
    $C ebit? -2147483646 scene-request? and if
        5 $24 1 scene-change
    then
    $25E story-flag? $25F story-flag? not and if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 $19 4 scene-change
        then
    then
;

: room21.phase3 ( -- )   \ 003FF810
    -2.0 7.6 -47.0 -2.0 7.6 -67.0 -2.0 0.0 -47.0 -2.0 0.0 -67.0 lights-doorway
;

: room21.act00 ( -- )   \ 003FF850
    self-wait-done
    $6A -1.0 13.5 160 $FFFF 5 self-move-to
    self-wait-done
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $306 story-flag? not if
        $306 story-flag-set
        $29 message
        wait-message
    then
    $902 self-anim
    self-wait-anim
    5 0 pvar? 5 1 pvar? or if
        $300 story-flag? not if
            $300 story-flag-set
            0 0 6 char-sound
            $40000001 6 0.0 3.0 7.0 0 0 sound
            $40 $6A noise
            1 noise-level
        else
            $300 story-flag-clear
            0 2 6 char-sound
            1 6 sound-stop
            0 noise-level
        then
        $903 self-anim
        self-wait-anim
    else
        $903 self-anim
        self-wait-anim
        $28 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room21.act03 ( -- )   \ 003FFA60
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room21.act01 ( -- )   \ 003FF8F0
    $18 state-flag-set
    $B ebit-clear
    1 self-scripted
    0 counter-set
    6 ebit-clear
    7 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 50 chars-within? and if
        0 1 2 action-force
        6 ebit-set
    then
    0 8 char-file-load
    self-wait-done
    6 ebit? if
        $27 14.5 -37.5 180 $FFFF 5 self-move-to
        self-wait-done
        0 char-file-use
        1 self-look-at
        yield
        begin
            7 ebit? not while
            yield
        repeat
        counter-inc
        1 self-noclip
        $FF self-look-at
        yield
        $8000 5 self-anim-9
        self-frames-reset
        $1C self-wait-frames
        0 $7E 5 char-sound
        self-wait-anim
    else
        0 char-file-use
        $27 $8004 5 14.5 -37.5 180 self-walk-anim
        self-wait-done
        counter-inc
        1 self-noclip
        0 $7E 5 char-sound
        $8005 $A self-anim-9
        self-wait-anim
    then
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    4 ebit-clear
    0 avoid-prompt
    $C ebit? if
        $FE $10 448 2 stalker-to-room
        $FE 0 stalker-mode
        $3C door-lock
        $3D door-lock
        $1F state-flag-set
    then
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room21.act03 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                $B ebit? $FE char-here? not and if
                    $B ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $C ebit? if
                $C ebit-clear
                $1F state-flag-clear
            then
            counter-inc
            $FE char-here? if
                ['] room21.act03 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room21.act02 ( -- )   \ 003FFA10
    1 self-scripted
    1 9 char-file-load
    self-wait-done
    $88 18.5 -37.5 180 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    7 ebit-set
    1 wait-counter
    1 self-noclip
    $8000 5 self-anim-blend
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                $8002 5 self-anim-9
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room21.act05 ( -- )   \ 0047AAC8
    self-wait-done
    -1 self-move-16
    self-wait-anim
    self-idle
    self-wait-done
    self-idle-or-end
;

: room21.act06 ( -- )   \ 003FFB00
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $6A -4.0 13.0 135 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    $FE 2 6 char-sound
    1 6 sound-stop
    0 noise-level
    $300 story-flag-clear
    5 pvar-inc
    $FE 3 stalker-mode
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: room21.act07 ( -- )   \ 003FFB50
    self-wait-done
    $F $54 fade
    3 3 $FF char-load
    3 char-unload
    0 1 char-no-shadow
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
    1 room21.cmd02
    1 room21.cmd04
    0 $F9 $A action
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
    0 room21.cmd02
    0 room21.cmd04
    wait-fade
    8 state-flag-set
    0 0 char-no-shadow
    3 0 char-remove
    1 1 $14 door-bits
    2 room21.cmd05
    $3C door-open-clear
    doors-room-in
    $3C door-unlock
    0 $10E -27.455 -48.354 107 char-to-xz
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    camera-restart
    $17 story-flag-set
    $F $51 fade
    wait-fade
    $201 item-give
    self-wait-done
    self-idle-or-end
;

: room21.act08 ( -- )   \ 003FFC30
    self-wait-done
    2 story-flag-set
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    \ (nop-progress-14: no effect in this game)
    0 $A8 46 char-to-tri-facing
    $3C door-open-clear
    $3D door-open-clear
    $3D door-unlock
    doors-room-in
    camera-restart
    $F $51 fade
    wait-fade
    $204 item-give
    2 30 var-set
    0 $F2 $1A action
    self-idle-or-end
;

: room21.act11 ( -- )   \ 00400020
    $8001 self-anim
    self-wait-anim
    begin
        32 hewie-action? while
        yield
    repeat
    $13 state-flag-set
    $3D door-lock
    0 counter-set
    0 $FE $13 action
    1 wait-counter
    1 self-scripted
    $13 state-flag-clear
    self-wait-done
    $FF self-look-at
    yield
    $F $54 fade
    7 2 movie-play
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
    $FE action-end
    $FE char-done
    0 self-move-16
    self-wait-anim
    $3D door-open-clear
    doors-room-in
    1 0 self-door-knock
    0 1 char-no-shadow
    $FF panic-stage? if
        3 panic-stage
    then
    $14 message-prepare
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
    0 0 char-no-shadow
    1 char-activate
    1 0 char-set-C4
    1 char-full-health
    $21 0 33 hewie-to-room
    1 $21 145 char-to-tri-facing
    $D state-flag-set
    $29 0 hewie-action
    1 hewie-wait-5
    $B story-flag-set
    0 $13 -16.325 -66.999 -8 char-to-xz
    0 self-move-16
    self-wait-anim
    \ (nop-progress-14: no effect in this game)
    $E state-flag-clear
    $FE action-end
    1 summon-take
    self-wait-done
    camera-restart
    8 state-flag-clear
    $20 state-flag-clear
    $F $51 fade
    wait-fade
    $207 item-give
    $F 6 fade
    wait-fade
    $40A7 message
    wait-message
    $F 7 fade
    wait-fade
    $A7 message-param-room
    $A7 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    $AF story-flag? if
        $F 6 fade
        wait-fade
        $32 message
        wait-message
        $F 7 fade
        wait-fade
    then
    0 hewie-wait-5
    $8274 item-give
    0 self-scripted
    self-idle-or-end
;

: room21.act09 ( -- )   \ 003FFC70
    $20 state-flag-set
    $1B state-flag-set
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
    0 $F9 $25 action
    $21 0 176 hewie-to-room
    $FE $21 20 2 stalker-to-room
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
    $12 state-flag-clear
    0 $A char-layer
    0 3 0.0 light
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    1 char-activate
    1 0 char-set-C4
    1 char-full-health
    $21 0 176 hewie-to-room
    1 $B0 -35.452 -8.852 180 char-to-xz
    $D state-flag-set
    $13 state-flag-set
    1 self-scripted
    $E state-flag-set
    $FE $21 20 2 stalker-to-room
    $FE char-activate
    $FE 2 stalker-mode
    $FE 0 stalker-search-delay
    $FE $14 -15.648 -48.68 0 char-to-xz
    $FE char-full-health
    0 $FE $1B action
    0 $B char-file-load
    0 char-file-use
    0 $A -39.575 -27.807 90 char-to-xz
    camera-restart
    $8001 self-anim
    self-wait-anim
    $1B state-flag-clear
    5 0 0 music
    $F $91 fade
    wait-fade
    self-frames-reset
    begin
        $2A message
        $F0 frames? 0 control-action? or if
            $8002 self-anim
            self-frames-reset
            $10 self-wait-frames
            0 $38 5 char-sound
            $13 state-flag-clear
            $20 0 hewie-action
            $2A message-close
            self-wait-anim
            $FE self-look-at
            yield
            ['] room21.act11 goto
        else
            yield
        then
    again
;

: room21.act0A ( -- )   \ 003FFDD0
    begin
        1 1 room21.cmd06
        8 cutscene-shot? $A cutscene-shot? or if
            1 $1E room21.cmd06
        then
        0 $14 room21.cmd06
        6 cutscene-shot? 8 cutscene-shot? or $A cutscene-shot? or $F cutscene-shot? or $11 cutscene-shot? or $13 cutscene-shot? or if
            0 $28 room21.cmd06
        then
        3 cutscene-shot? if
            1 1 $14 door-bits
        then
        $15 cutscene-shot? if
            0 room21.cmd05
        then
        $16 cutscene-shot? if
            1 room21.cmd05
        then
        $17 cutscene-shot? if
            2 room21.cmd05
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room21.act0B ( -- )   \ 003FFE30
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        4 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        4 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room21.act0C ( -- )   \ 0047AAD0
    self-wait-done
    $20 message
    wait-message
    self-idle-or-end
;

: room21.act0D ( -- )   \ 003FFE90
    self-wait-done
    $1F message
    wait-message
    0 answer? if
        $F $54 fade
        $D 3 $FF char-load
        $12 4 $FF char-load
        $18 5 $FF char-load
        3 char-unload
        4 char-unload
        5 char-unload
        $D 1 char-no-shadow
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
        0 $F9 $21 action
        $FF panic-stage? if
            3 panic-stage
        then
        8 message-prepare
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
        $D 0 char-no-shadow
        3 0 char-remove
        4 0 char-remove
        5 0 char-remove
        3 state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
        effects-arena-flip
        0 char-in
        begin
            yield
            4 sound-bank-loaded? until
        1 0 $14 door-bits
        0 $21 5.752 -41.411 178 char-to-xz
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        camera-restart
        $3C door-lock
        $3D door-lock
        0 story-flag-set
        $14 state-flag-clear
        $F $51 fade
        wait-fade
        $202 item-give
        1 $A self-anim-blend
        self-wait-anim
        10 self-move-16
        $12 message
        wait-message
        $F 6 fade
        wait-fade
        $40A6 message
        wait-message
        $AE story-flag? if
            $40AB message
            wait-message
        then
        $F 7 fade
        wait-fade
        $A6 message-param-room
        $A6 1 item-give-count
        $83 $85 0.0 0.0 0.0 0 0 sound
        $801B message
        wait-message
        $AE story-flag? if
            $34B story-flag-set
            $AB message-param-room
            $AB 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message
            wait-message
        then
    then
    self-wait-done
    self-idle-or-end
;

: room21.act0E ( -- )   \ 0047AAD8
    self-wait-done
    $C message
    wait-message
    self-wait-done
    self-idle-or-end
;

: room21.act0F ( -- )   \ 0047AAE0
    self-wait-done
    $22 message
    wait-message
    self-wait-done
    self-idle-or-end
;

: room21.act10 ( -- )   \ 00400000
    self-wait-done
    $80 -1.546 59.728 45 $FFFF 5 self-move-to
    self-wait-done
    $902 self-anim
    self-wait-anim
    $D message
    wait-message
    $903 self-anim
    self-wait-anim
    self-wait-done
    self-idle-or-end
;

: room21.act12 ( -- )   \ 0047AAE8
    begin
        room21.cmd01
        yield
    again
;

: room21.act13 ( -- )   \ 00400180
    1 self-scripted
    self-wait-done
    $FF self-look-at
    yield
    $C4 $203 6 self-move-tri
    self-wait-done
    0 exit-door-open? not if
        0 3 self-move-slot
        self-wait-done
    then
    $76 -17.0 -83.0 180 $203 5 self-move-to
    self-wait-done
    1 counter-set
    0 self-scripted
    begin
        yield
    again
;

: room21.act14 ( -- )   \ 004001B0
    self-wait-done
    -40.0 -60.0 self-turn-to-xz
    self-wait-done
    $23 message
    wait-message
    self-idle-or-end
;

: room21.act15 ( -- )   \ 004001C0
    self-wait-done
    $B5 43.571 -39.929 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $312 story-flag-set
    0 ebit? not if
        $24 message
        wait-message
        0 ebit-set
    else
        $25 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room21.act16 ( -- )   \ 00400220
    self-wait-done
    $26 message
    wait-message
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    $800 self-anim
    self-wait-anim
    $801 self-anim
    self-frames-reset
    self-wait-16
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    $27 message
    wait-message
    self-idle-or-end
;

: room21.act17 ( -- )   \ 00400250
    self-wait-done
    $9F -37.0 63.0 0 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $314 story-flag-set
    1 ebit? not if
        $2D message
        wait-message
        1 ebit-set
    else $312 story-flag? $313 story-flag? or if
        $2E message
        wait-message
    else
        $2D message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room21.act18 ( -- )   \ 004002C0
    self-wait-done
    9 -40.418 -39.428 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $313 story-flag-set
    2 ebit? not if
        $2B message
        wait-message
        2 ebit-set
    else $312 story-flag? $314 story-flag? and if
        $2C message
        wait-message
    else
        $2B message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room21.act19 ( -- )   \ 00400330
    self-wait-done
    -9.0 -11.0 self-turn-to-xz
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
            $25F story-flag-set
            2 effect-remove
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

: room21.act1A ( -- )   \ 00400390
    begin
        yield
        2 var-dec
        2 0 var? if
            $1E chance? if
                $F 6 228.0 5.0 78.0 0 0 sound
                2 180 var-set
            else
                2 60 var-set
            then
        then
    again
;

: room21.act1B ( -- )   \ 004003D0
    self-wait-done
    0 self-look-at
    yield
    begin
        $19 chance? if
            $1302 $A self-anim-blend
            self-wait-anim
        else $19 chance? if
            $1305 $A self-anim-blend
            self-wait-anim
        then then
        yield
    again
;

: room21.act1C ( -- )   \ 004003F0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $E8 -25.0 63.0 -5 char-to-xz
    1 20.0 10.0 40.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 ebit? not if
        1 message
        wait-message
        9 ebit-set
    else
        2 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room21.act1D ( -- )   \ 00400440
    self-wait-done
    5.0 40.5 self-turn-to-xz
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room21.act1E ( -- )   \ 00400450
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $55 -40.0 12.0 -90 char-to-xz
    1 30.0 20.0 -60.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room21.act1F ( -- )   \ 00400498
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $40 message
    wait-message
    self-idle-or-end
;

: room21.act20 ( -- )   \ 004004B0
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $A ebit? not if
        6 message
        wait-message
        $A ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: room21.act21 ( -- )   \ 004004D0
    begin
        2 cutscene-shot? if
            room21.cmd03
        then
        5 cutscene-shot? if
            1 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: room21.act22 ( -- )   \ 004004E8
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $31 message
    wait-message
    self-idle-or-end
;

: room21.act23 ( -- )   \ 00400500
    $18 state-flag-set
    self-wait-done
    0 4 self-move-slot
    self-wait-done
    1 $A self-anim-blend
    $F message
    self-wait-anim
    10 self-move-16
    wait-message
    $F 6 fade
    wait-fade
    $40A5 message
    wait-message
    $F 7 fade
    wait-fade
    $A5 message-param-room
    $A5 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    $3C door-open-clear
    $3C door-unlock
    $3D door-unlock
    doors-room-in
    $1F state-flag-set
    $C ebit-set
    self-idle-or-end
;

: room21.act24 ( -- )   \ 0047AAF0
    self-wait-done
    $11 message
    wait-message
    self-idle-or-end
;

: room21.act25 ( -- )   \ 00400560
    begin
        0 cutscene-shot? not while
        yield
    repeat
    0 $1E char-layer
    begin
        1 cutscene-shot? not while
        yield
    repeat
    0 $A char-layer
    begin
        6 cutscene-shot? not while
        yield
    repeat
    2 3 1.5 light
    begin
        7 cutscene-shot? not while
        yield
    repeat
    0 3 0.0 light
    self-idle-or-end
;

: room21.act26 ( -- )   \ 004005A0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message
    wait-message
    0 answer? if
        7 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            0 $10 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                3 state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                3 state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                3 state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                3 state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                3 state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                3 state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                3 state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            1 char-here? if
                1 2 char-C4? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room21.act27 ( -- )   \ 004006F0
    self-wait-done
    1 1 $14 door-bits
    1 fiona-costume
    0 char-in
    3 3 $FF char-load
    3 char-unload
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 $F1 $12 action
    0 room21.cmd02
    0 room21.cmd04
    1 1 $14 door-bits
    0 1 char-no-shadow
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
    1 room21.cmd02
    1 room21.cmd04
    0 $F9 $A action
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
    0 room21.cmd02
    0 room21.cmd04
    wait-fade
    8 state-flag-set
    0 0 char-no-shadow
    $26 1 pvar? if
        3 state-flag-set
        1 sound-set
        1 fiona-costume
        $26 1 pvar-set
    else $26 0 pvar? if
        3 state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
    else $26 2 pvar? if
        3 state-flag-set
        1 sound-set
        2 fiona-costume
        $26 2 pvar-set
    else $26 3 pvar? if
        3 state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
    else $26 6 pvar? if
        3 state-flag-clear
        0 sound-set
        6 fiona-costume
        $26 6 pvar-set
    else $26 7 pvar? if
        3 state-flag-clear
        0 sound-set
        7 fiona-costume
        $26 7 pvar-set
    else $26 8 pvar? if
        3 state-flag-set
        1 sound-set
        8 fiona-costume
        $26 8 pvar-set
    then then then then then then then
    0 char-in
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room21.act28 ( -- )   \ 00400840
    self-wait-done
    1 fiona-costume
    0 char-in
    $D 3 $FF char-load
    $12 4 $FF char-load
    $18 5 $FF char-load
    3 char-unload
    4 char-unload
    5 char-unload
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 $F1 $12 action
    0 room21.cmd02
    0 room21.cmd04
    1 1 $14 door-bits
    $D 1 char-no-shadow
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
    0 $F9 $21 action
    $FF panic-stage? if
        3 panic-stage
    then
    8 message-prepare
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
    $D 0 char-no-shadow
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $26 1 pvar? if
        3 state-flag-set
        1 sound-set
        1 fiona-costume
        $26 1 pvar-set
    else $26 0 pvar? if
        3 state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
    else $26 2 pvar? if
        3 state-flag-set
        1 sound-set
        2 fiona-costume
        $26 2 pvar-set
    else $26 3 pvar? if
        3 state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
    else $26 6 pvar? if
        3 state-flag-clear
        0 sound-set
        6 fiona-costume
        $26 6 pvar-set
    else $26 7 pvar? if
        3 state-flag-clear
        0 sound-set
        7 fiona-costume
        $26 7 pvar-set
    else $26 8 pvar? if
        3 state-flag-set
        1 sound-set
        8 fiona-costume
        $26 8 pvar-set
    then then then then then then then
    0 char-in
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room21.act29 ( -- )   \ 00400980
    self-wait-done
    0 $F1 $12 action
    0 room21.cmd02
    0 room21.cmd04
    2 3 $FF char-load
    3 char-unload
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
    0 $F9 $25 action
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
    0 $A char-layer
    0 3 0.0 light
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room21.act2A ( -- )   \ 00400A30
    self-wait-done
    0 $F1 $12 action
    0 room21.cmd02
    0 room21.cmd04
    7 2 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 char-no-shadow
    0 self-scripted
    $FF panic-stage? if
        3 panic-stage
    then
    $14 message-prepare
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
    0 0 char-no-shadow
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room21.enter $21 0 room-script!
' room21.char-enter $21 6 room-script!
' room21.phase1 $21 1 room-script!
' room21.phase2 $21 2 room-script!
' room21.phase3 $21 3 room-script!
' room21.act00 $21 $00 action-script!
' room21.act01 $21 $01 action-script!
' room21.act02 $21 $02 action-script!
' room21.act03 $21 $03 action-script!
' room21.act04 $21 $04 action-script!
' room21.act05 $21 $05 action-script!
' room21.act06 $21 $06 action-script!
' room21.act07 $21 $07 action-script!
' room21.act08 $21 $08 action-script!
' room21.act09 $21 $09 action-script!
' room21.act0A $21 $0A action-script!
' room21.act0B $21 $0B action-script!
' room21.act0C $21 $0C action-script!
' room21.act0D $21 $0D action-script!
' room21.act0E $21 $0E action-script!
' room21.act0F $21 $0F action-script!
' room21.act10 $21 $10 action-script!
' room21.act11 $21 $11 action-script!
' room21.act12 $21 $12 action-script!
' room21.act13 $21 $13 action-script!
' room21.act14 $21 $14 action-script!
' room21.act15 $21 $15 action-script!
' room21.act16 $21 $16 action-script!
' room21.act17 $21 $17 action-script!
' room21.act18 $21 $18 action-script!
' room21.act19 $21 $19 action-script!
' room21.act1A $21 $1A action-script!
' room21.act1B $21 $1B action-script!
' room21.act1C $21 $1C action-script!
' room21.act1D $21 $1D action-script!
' room21.act1E $21 $1E action-script!
' room21.act1F $21 $1F action-script!
' room21.act20 $21 $20 action-script!
' room21.act21 $21 $21 action-script!
' room21.act22 $21 $22 action-script!
' room21.act23 $21 $23 action-script!
' room21.act24 $21 $24 action-script!
' room21.act25 $21 $25 action-script!
' room21.act26 $21 $26 action-script!
' room21.act27 $21 $27 action-script!
' room21.act28 $21 $28 action-script!
' room21.act29 $21 $29 action-script!
' room21.act2A $21 $2A action-script!

\ ---- room $22 ----------------------------------------------------------------------------------

: room22.enter ( -- )   \ 00400C60
    $A story-flag? not if
        room-sounds
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    $296 story-flag? not if
        0 -22.26 8.5 -41.95 flicker-sprite
    then
;

: room22.char-enter ( -- )   \ 00400C90
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
;

: room22.phase1 ( -- )   \ 00400CD0
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    $A story-flag? not if
        0 3.5 0.0 33.0 $1E 20 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    1 3.07 0.0 30.96 $D 35 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 35.0 hewie-look-zone
            then
        then
    then
    2 -28.72 0.0 -20.26 $D 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
;

: room22.phase2 ( -- )   \ 00400D90
    $296 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    $A story-flag? not if
        0 5 char-in-area? 0 0 $2D char-heading? and if
            5 3 0 scene-change
        then
    then
    0 6 char-in-area? 0 -45 $2D char-heading? and if
        5 1 0 scene-change
    then
    0 $F char-in-area? 0 -67 $32 char-heading? and if
        5 6 0 scene-change
    then
    0 7 char-in-area? 0 9 char-in-area? or 0 -45 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 8 char-in-area? 0 $B char-in-area? or 0 0 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 $A char-in-area? 0 $D char-in-area? or 0 45 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 $C char-in-area? 0 90 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 $E $32 char-faces-area? if
        5 4 0 scene-change
    then
;

: room22.phase3 ( -- )   \ 00400E30
    camera-setup-changed? if
        0 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
;

: room22.act00 ( -- )   \ 00400E50
    2 0 char-remove
    7 partner-load
    $902 self-anim
    self-wait-anim
    0 3 6 char-sound
    $903 self-anim
    self-wait-anim
    3 message-param-room
    $8019 message
    wait-message
    0 $55 3.622 32.279 0 char-to-xz
    1 -17.0 -10.0 30.0 0.0 event-camera
    self-frames-reset
    8 self-wait-frames
    0 0 object-anim
    $8000 $A self-anim-blend
    self-frames-reset
    $14 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    self-frames-reset
    self-wait-16
    2 char-unload
    $A story-flag-set
    3 item-use
    4 message-param-room
    4 1 item-give-count
    0 4 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    1 0 0 music
    0 1 6 char-sound
    0 1 object-anim
    $8001 0 self-anim-blend
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    0 state-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room22.act01 ( -- )   \ 00400F10
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C message
    wait-message
    $F 6 fade
    wait-fade
    $F message
    wait-message
    $F 7 fade
    wait-fade
    $25 story-flag? not if
        $1D01 $A self-anim-blend
        $D message
        wait-message
        self-wait-anim
    else
        $E message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room22.act02 ( -- )   \ 00400F50
    self-wait-done
    1 ebit? not if
        8 message
        wait-message
        1 ebit-set
    else
        9 message
        wait-message
    then
    self-idle-or-end
;

: room22.act03 ( -- )   \ 00400F70
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 1 char-file-load
    $55 3.622 32.279 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    $8002 $A self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    0 2 6 char-sound
    self-frames-reset
    $B self-wait-frames
    0 2 6 char-sound
    self-frames-reset
    8 self-wait-frames
    0 2 6 char-sound
    self-wait-anim
    3 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room22.act04 ( -- )   \ 00400FC0
    self-wait-done
    0 ebit? not if
        $A message
        wait-message
        0 ebit-set
    else
        $A01 $A self-anim-blend
        $B message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: room22.act05 ( -- )   \ 00400FE0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 1 char-file-load
    $55 3.622 31.0 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    ['] room22.act00 goto
;

: room22.act06 ( -- )   \ 00401000
    self-wait-done
    -135 self-turn-angle
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: room22.act07 ( -- )   \ 00401010
    self-wait-done
    -22.26 -41.95 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $296 story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

' room22.enter $22 0 room-script!
' room22.char-enter $22 6 room-script!
' room22.phase1 $22 1 room-script!
' room22.phase2 $22 2 room-script!
' room22.phase3 $22 3 room-script!
' room22.act00 $22 $00 action-script!
' room22.act01 $22 $01 action-script!
' room22.act02 $22 $02 action-script!
' room22.act03 $22 $03 action-script!
' room22.act04 $22 $04 action-script!
' room22.act05 $22 $05 action-script!
' room22.act06 $22 $06 action-script!
' room22.act07 $22 $07 action-script!

\ ---- room $23 ----------------------------------------------------------------------------------

\ room 0x23 (Room23_Cmd00_ptmf): Fiona's model +0x9A0 / +0x9A8: 0 (byte 3 1) or 0.12 / 0.2
: room23.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x23 (Room23_Cmd01_ptmf): byte 3 0 a progress name, 1 wait for character 3 (2 while not),
\ else done
: room23.cmd01 ( b0 -- )  drop stub-step ;
\ room 0x23 (Room23_Cond00_ptmf): none of the six slots' PursuerGroup_Fields bits 0..3, and the
\ stalker is about but not active, in mode 2, 6 or 7
: room23.cond00? ( -- flag )  stub-flag ;

: room23.enter ( -- )   \ 004010A0
    room-sounds
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 scene-effect-71000
    6 1 object-show
;

: room23.char-enter ( -- )   \ 004010E0
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
    0 self-is? if
        $80 exit-taken? if
            0 0 6 action
            0 1 $14 door-bits
            1 0 $14 door-bits
        else
            0 0 $14 door-bits
            1 1 $14 door-bits
        then
    then
;

: room23.phase1 ( -- )   \ 00401140
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    0 -35.84 0.0 32.47 $12 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 36.32 0.0 24.37 $F 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
;

: room23.phase2 ( -- )   \ 004011B0
    0 3 $2D char-faces-area? if
        $1F story-flag? not if
            5 0 0 scene-change
        else
            5 3 0 scene-change
        then
    then
    0 4 $2D char-faces-area? if
        5 1 0 scene-change
    then
    0 5 $2D char-faces-area? if
        5 2 0 scene-change
    then
;

: room23.act00 ( -- )   \ 004011E0
    $18 state-flag-set
    1 self-scripted
    $F 4 fade
    self-wait-done
    2 0 movie-play
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
    6 message-prepare
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
    $1F story-flag-set
    0 ebit-set
    2 subscreen-open
    begin
        4 state-flag? while
        yield
    repeat
    yield
    $20 story-flag? if
        3 0 movie-play
        1 cutscene-start
        yield
        yield
        2 cutscene-control
        begin
            3 cutscene-control
            2 cutscene-mode? not while
            yield
        repeat
        0 $F9 4 action
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
        6 1 object-show
    then
    0 $7A 31.61 18.93 33 char-to-xz
    0 self-move-16
    camera-restart
    $F 1 fade
    wait-fade
    $20 story-flag? if
        $3F message-param-room
        0 $3F item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        begin
            $F9 char-busy? while
            yield
        repeat
        $225 item-give
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room23.act01 ( -- )   \ 00401350
    self-wait-done
    0 0 var? if
        -36.2 33.3 self-turn-to-xz
        self-wait-done
        $A00 self-anim
        self-frames-reset
        $32 self-wait-frames
        $D message
        wait-message
        0 1 var-set
    else
        $E message
        wait-message
    then
    self-idle-or-end
;

: room23.act02 ( -- )   \ 0047AB00
    self-wait-done
    $C message
    wait-message
    self-idle-or-end
;

: room23.act03 ( -- )   \ 00401380
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $1B story-flag? $307 story-flag? not and room23.cond00? and 0 exit-door-open? and if
        $11 door-open-clear
        doors-room-in
        0 0 self-door-knock
        self-frames-reset
        5 self-wait-frames
        0 $43 5 char-sound
        $F04 5 self-anim-blend
        1 $FF 8 rumble
        self-frames-reset
        $10 self-wait-frames
        $307 story-flag-set
        $5A threat-raise
        self-wait-anim
    else item-3F-under-10? if
        0 ebit? not if
            $11 message
            wait-message
            0 ebit-set
        then
        $20 story-flag? if
            $20 story-flag-clear
            2 subscreen-open
            begin
                4 state-flag? while
                yield
            repeat
            yield
            $20 story-flag? if
                $3F message-param-room
                0 $3F item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
            then
            $20 story-flag-set
        else
            2 subscreen-open
            begin
                4 state-flag? while
                yield
            repeat
            yield
            $20 story-flag? if
                $F 4 fade
                3 0 movie-play
                1 cutscene-start
                yield
                yield
                2 cutscene-control
                begin
                    3 cutscene-control
                    2 cutscene-mode? not while
                    yield
                repeat
                wait-fade
                0 $F9 4 action
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
                0 $7A 31.61 18.93 33 char-to-xz
                0 self-move-16
                camera-restart
                6 1 object-show
                $F 1 fade
                wait-fade
                $3F message-param-room
                0 $3F item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
                begin
                    $F9 char-busy? while
                    yield
                repeat
                $225 item-give
            then
        then
    else 0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $12 message
        wait-message
    then then then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room23.act04 ( -- )   \ 00401500
    begin
        $69 cutscene-cue-reached? not while
        yield
    repeat
    1 6 35.0 10.0 25.0 0 0 sound
    self-idle-or-end
;

: room23.act05 ( -- )   \ 0047AB08
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: room23.act06 ( -- )   \ 00401520
    self-wait-done
    0 $7A 31.61 18.93 33 char-to-xz
    0 1 char-visible
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
    0 room23.cmd01
    0 room23.cmd00
    0 $F9 7 action
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
    8 state-flag-set
    2 room23.cmd01
    1 room23.cmd00
    0 0 char-visible
    3 0 char-remove
    4 0 char-remove
    $25 resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: room23.act07 ( -- )   \ 004015D0
    begin
        3 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    1 room23.cmd01
    self-idle-or-end
;

: room23.act08 ( -- )   \ 004015E0
    room-sounds
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    self-wait-done
    6 1 object-show
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 scene-effect-71000
    2 0 movie-play
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
    6 message-prepare
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
    3 0 movie-play
    1 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 4 action
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

: room23.act09 ( -- )   \ 00401720
    self-wait-done
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 scene-effect-71000
    6 1 object-show
    $FF 1 char-visible
    0 1 $14 door-bits
    1 0 $14 door-bits
    3 3 $FF char-load
    4 4 $FF char-load
    3 char-unload
    4 char-unload
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
    0 room23.cmd01
    0 room23.cmd00
    0 $F9 7 action
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
    8 state-flag-set
    2 room23.cmd01
    1 room23.cmd00
    3 0 char-remove
    4 0 char-remove
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room23.enter $23 0 room-script!
' room23.char-enter $23 6 room-script!
' room23.phase1 $23 1 room-script!
' room23.phase2 $23 2 room-script!
' room23.act00 $23 $00 action-script!
' room23.act01 $23 $01 action-script!
' room23.act02 $23 $02 action-script!
' room23.act03 $23 $03 action-script!
' room23.act04 $23 $04 action-script!
' room23.act05 $23 $05 action-script!
' room23.act06 $23 $06 action-script!
' room23.act07 $23 $07 action-script!
' room23.act08 $23 $08 action-script!
' room23.act09 $23 $09 action-script!

\ ---- room $24 ----------------------------------------------------------------------------------

\ Room24_Cmd00
: room24.cmd00 ( b0 -- )  drop stub-step ;
\ Room24_Cmd01
: room24.cmd01 ( -- )  stub-step ;
\ Room24_Cmd02
: room24.cmd02 ( -- )  stub-step ;

: room24.enter ( -- )   \ 00401890
    room-sounds
    $1B story-flag? not if
        1 0 $20000 nav-group
        1 5 $800000 nav-group
        1 1 $4000000 nav-group
        0 1 $14 door-bits
    else
        0 0 $14 door-bits
        $1A story-flag? if
            1 2 $20000 nav-group
            1 6 $800000 nav-group
            1 3 $4000000 nav-group
            1 1 $14 door-bits
        else $19 story-flag? if
            2 1 $14 door-bits
        then then
    then
    $1E story-flag? not if
        5 1 $14 door-bits
    then
    room24.cmd01
    $209 story-flag? not if
        1 4 $8000000 nav-group
        3 1 $14 door-bits
        4 0 $14 door-bits
    else
        0 4 $8000000 nav-group
        3 0 $14 door-bits
        4 1 $14 door-bits
    then
    0 7 0.812 0.687 0.187 0.312 zone-rect
    $19 story-flag? $315 story-flag? not and if
        0 10.0 1.0 -25.0 flicker-sprite
    then
    2 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    2 2 1.5 light
;

: room24.char-enter ( -- )   \ 00401950
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
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
    0 $F1 4 action
;

: room24.phase1 ( -- )   \ 004019D0
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000009 6 57.0 0.0 -2.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000009 6 57.0 0.0 -2.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $209 story-flag? not if
        0 34.0 0.0 29.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $209 story-flag-set
            0 4 $8000000 nav-group
            3 0 $14 door-bits
            4 1 $14 door-bits
            34.0 0.0 29.0 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 34.0 0.0 29.0 0 0 sound
            $40 $FF noise
        then
    then
    1 0 4 4 0 zone-at-effect
    $1B story-flag? if
        $1A story-flag? if
            2 43.743 0.0 -29.309 $F 20 0 zone
            2 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 2 9 char-zone-bits? if
                            2 ebit-set
                            $1E chance? if
                                $1F 2 var-set
                                0 1 $89 action
                            then
                        then
                    then
                then
            then
        else $19 story-flag? if
            1 0 char-in-area? not if
                2 10.0 0.0 -25.0 $A 20 0 zone
                2 ebit? not 1 char-here? and 1 0 char-C4? and if
                    2 game-mode? not if
                        hewie-can-command? if
                            1 2 9 char-zone-bits? if
                                2 ebit-set
                                $1E chance? if
                                    $1F 2 var-set
                                    0 1 $89 action
                                then
                            then
                        then
                    then
                then
            then
        then then
    then
    3 49.99 0.0 -0.54 $12 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: room24.phase2 ( -- )   \ 00401B50
    -2147483644 scene-request? if
        0 0 8 action
    else $1B story-flag? not if
        0 6 $32 char-faces-area? if
            5 0 0 scene-change
        then
    else 0 $B $32 char-faces-area? $1A story-flag? and if
        5 9 0 scene-change
    then then then
    0 $C char-in-area? $1E story-flag? not and if
        0 0 $32 char-heading? if
            5 6 4 scene-change
        then
    else 0 7 $3C char-faces-area? if
        5 1 0 scene-change
    then then
    0 8 char-in-area? 0 45 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 9 char-in-area? 0 $A char-in-area? or 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
    0 1 2 char-zone-bits? if
        5 7 4 scene-change
    then
;

: room24.act00 ( -- )   \ 00401BD0
    self-wait-done
    $F5 10.0 -28.0 180 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 10.0 -40.0 0.0 0.0 event-camera
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        3 message
        wait-message
    then
    self-frames-reset
    self-wait-16
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room24.act01 ( -- )   \ 00401C30
    self-wait-done
    6.0 4.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    6 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room24.act02 ( -- )   \ 00401C50
    self-wait-done
    56.58 -1.5 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    7 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room24.act03 ( -- )   \ 00401C68
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    8 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room24.act04 ( -- )   \ 00401C80
    0 room24.cmd00
    begin
        1 room24.cmd00
        1 2 1.1 light
        self-frames-reset
        2 self-wait-frames
        1 room24.cmd00
        0 2 0.0 light
        2 2 1.5 light
        yield
    again
;

: room24.act05 ( -- )   \ 00401CB0
    0 0 6 char-sound
    self-wait-done
    self-frames-reset
    $1E self-wait-frames
    9 message
    wait-message
    self-idle-or-end
;

: room24.act06 ( -- )   \ 00401CD0
    self-wait-done
    $D6 7.826 -7.168 0 $FFFF 5 self-move-to
    self-wait-done
    $A message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    5 0 $14 door-bits
    $1E story-flag-set
    $A0 message-param-room
    $A0 1 item-give-count
    0 $A0 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room24.act07 ( -- )   \ 00401D30
    self-wait-done
    10.0 -25.0 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $8D 1 item-count? not if
        0 effect-remove
        $315 story-flag-set
        $8D message-param-room
        $8D 1 item-give-count
        0 $8D item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else
        $75 message-param-room
        $75 $63 item-count? if
            $8010 message
            wait-message
        else
            $315 story-flag-set
            0 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    then
    $901 self-anim
    self-wait-anim
    wait-message
    self-idle-or-end
;

: room24.act08 ( -- )   \ 00401DC0
    begin
        0 char-busy? not while
        yield
    repeat
    $1203 0 self-anim-blend
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    1 message
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: room24.act09 ( -- )   \ 00401DE0
    self-wait-done
    45.0 -30.0 self-turn-to-xz
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: room24.act0A ( -- )   \ 00401DF0
    $18 state-flag-set
    1 self-scripted
    8 3 $FF char-load
    self-wait-done
    $F $54 fade
    3 char-unload
    8 1 char-no-shadow
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
    8 char-activate
    0 0 $14 door-bits
    $FF 1 char-visible
    $F $3C movie-param
    2 0 char-remove
    6 partner-load
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
    2 char-unload
    $A 1 object-show
    3 0 char-remove
    0 $A8 -160 char-to-tri-facing
    camera-restart
    $13 door-lock
    $1B story-flag-set
    1 0 0 music
    0 0 $20000 nav-group
    0 5 $800000 nav-group
    0 1 $4000000 nav-group
    1 2 $20000 nav-group
    1 6 $800000 nav-group
    1 3 $4000000 nav-group
    1 1 $14 door-bits
    $FF 0 char-visible
    $F $51 fade
    $16 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room24.act0B ( -- )   \ 00401EF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
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
    0 0 $14 door-bits
    $FF 1 char-visible
    2 1 $14 door-bits
    0 10.0 1.0 -25.0 flicker-sprite
    $F $96 movie-param
    2 0 char-remove
    6 partner-load
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
    2 char-unload
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    $A 1 object-show
    0 $A8 -160 char-to-tri-facing
    camera-restart
    $13 door-lock
    $1B story-flag-set
    1 0 0 music
    0 0 $20000 nav-group
    0 5 $800000 nav-group
    0 1 $4000000 nav-group
    $FF 0 char-visible
    $F $51 fade
    $17 resident-flag-set
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room24.act0C ( -- )   \ 00401FF0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    8 3 $FF char-load
    3 char-unload
    8 1 char-no-shadow
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
    0 0 $14 door-bits
    $FF 1 char-visible
    $F $3C movie-param
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
    3 0 char-remove
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room24.act0D ( -- )   \ 004020A0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
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
    0 0 $14 door-bits
    $FF 1 char-visible
    2 1 $14 door-bits
    $F $96 movie-param
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
    $FF 0 char-visible
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room24.act0E ( -- )   \ 00402150
    $18 state-flag-set
    1 self-scripted
    8 3 $FF char-load
    self-wait-done
    $F $44 fade
    3 char-unload
    $37 1.0 1 bgm
    wait-fade
    $19 state-flag-set
    $FF panic-stage? if
        3 panic-stage
    then
    $F state-flag-set
    0 $EE 10.0 -20.0 180 char-to-xz
    1 23.0 -12.0 0.0 5.0 event-camera
    $FF 1 char-visible
    0 0 $14 door-bits
    8 char-activate
    0 0 $20000 nav-group
    0 5 $800000 nav-group
    0 1 $4000000 nav-group
    8 $EF 10.0 -30.0 0 char-to-xz
    1 0 $20000 nav-group
    1 5 $800000 nav-group
    1 1 $4000000 nav-group
    begin
        0 adx? not while
        yield
    repeat
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    0 0 var-set
    room24.cmd02
    self-frames-reset
    $A self-wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        room24.cmd02
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 1 $14 door-bits
    8 action-end
    8 char-done
    3 0 char-remove
    $FF 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    $F state-flag-clear
    $19 state-flag-clear
    $F $41 fade
    wait-fade
    3 ebit? not if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 $84 5 char-sound
        3 ebit-set
        self-frames-reset
        $1E self-wait-frames
    else
        $C message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room24.phase5 ( -- )   \ 0047AB10
;

' room24.enter $24 0 room-script!
' room24.char-enter $24 6 room-script!
' room24.phase1 $24 1 room-script!
' room24.phase2 $24 2 room-script!
' room24.act00 $24 $00 action-script!
' room24.act01 $24 $01 action-script!
' room24.act02 $24 $02 action-script!
' room24.act03 $24 $03 action-script!
' room24.act04 $24 $04 action-script!
' room24.act05 $24 $05 action-script!
' room24.act06 $24 $06 action-script!
' room24.act07 $24 $07 action-script!
' room24.act08 $24 $08 action-script!
' room24.act09 $24 $09 action-script!
' room24.act0A $24 $0A action-script!
' room24.act0B $24 $0B action-script!
' room24.act0C $24 $0C action-script!
' room24.act0D $24 $0D action-script!
' room24.act0E $24 $0E action-script!
' room24.phase5 $24 5 room-script!

\ ---- room $25 ----------------------------------------------------------------------------------

\ the rising motes started
: room25.cmd00 ( -- )  stub-step ;
\ room 0x25: Fiona is active and in a reaction (action 4) with no character behind it (+0x100
\ 0xFF).
: room25.cond00? ( -- flag )  stub-flag ;

: room25.enter ( -- )   \ 00402330
    room-sounds
    $20C story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    $20D story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $288 story-flag? not if
            1 -5.5 1.0 -99.0 flicker-sprite
        then
    then
    0 6 0.5 1.0 0.187 0.312 zone-rect
    $248 story-flag? $249 story-flag? not and if
        0 60.0 1.0 37.2 flicker-sprite
    then
    $230 story-flag? not if
        2 -33.0 43.5 15.0 flicker-sprite
    then
    room25.cmd00
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $10 20.0 19.7 36.4 $E $80 $80 $80 $40 specks
        $10 30.0 87.0 124.7 $E $80 $80 $80 $40 specks
        $10 -53.9 28.8 -52.0 $A $80 $80 $80 $40 specks
    then
;

: room25.char-enter ( -- )   \ 00402420
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 3 1 char-camera
                0 camera-follow
            else
                1 3 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 3 1 char-camera
            0 camera-follow
        else
            1 3 1 char-camera
            1 camera-follow
        then
    then then
    2 3 1 area-camera
    0 self-is? if
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
        2 exit-taken? if
            3 map-page
        then
    then
;

: room25.phase1 ( -- )   \ 004024F0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 1 -1 1 chars-area-camera
    4 0 0 1 chars-area-camera
    7 1 -1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    $B 1 -1 1 chars-area-camera
    $11 0 0 1 chars-area-camera
    $12 3 1 1 chars-area-camera
    $13 0 0 1 chars-area-camera
    $14 4 2 1 chars-area-camera
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? 0 3 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 4 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        2 map-page
    then
    $20C story-flag? not if
        0 -30.0 0.0 -85.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $20C story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -30.0 0.0 -85.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -30.0 0.0 -85.4 0 0 sound
            $40 $1E7 noise
        then
    then
    $248 story-flag? not if
        2 60.0 0.0 37.2 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 584 var-set
                $1A 585 var-set
                $1B 0 var-set
                $1C 60000 var-set
                $1D 1000 var-set
                $1E 37200 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        2 game-mode? not if
            1 hewie-side? 0 $100000 char-on-nav-flags? and if
                0 -37 16 $32 char-faces-xz? if
                    35 fiona-started? if
                        hewie-stays? if
                            0 0 5 action
                        then
                    then
                then
            then
        then
    then
    $2B story-flag? not if
        room25.cond00? if
            $2B story-flag-set
            0 $F1 2 action
        then
    then
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 0.0 71.0 35.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000000 6 0.0 71.0 35.0 0 0 sound
    then
    $230 story-flag? not if
        4 -33.85 42.7 15.04 $2C 30 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        3 -33.85 42.7 15.04 $2C 30 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 3 9 char-zone-bits? if
                    $1F 3 var-set
                    $1F 10.0 hewie-look-zone
                then
            then
        then
    then
;

: room25.phase2 ( -- )   \ 00402700
    $248 story-flag? $249 story-flag? not and if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    0 $C char-in-area? 0 45 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $D $32 char-faces-area? if
        5 1 0 scene-change
    then
    0 $10 char-in-area? 0 35 -45 $32 char-faces-xz? and if
        5 8 0 scene-change
    then
    0 1 char-group-bit4? if
        -2147483646 scene-request? if
            5 9 1 scene-change
        then
    then
    $20D story-flag? not if
        1 -5.5 0.0 -99.0 5 8 1 zone
        $FF 1 char-in-zone? if
            $20D story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -5.5 0.0 -99.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -5.5 0.0 -99.0 0 0 sound
            $40 $1F1 noise
            1 -5.5 1.0 -99.0 flicker-sprite
        then
    else $288 story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then then
;

: room25.phase3 ( -- )   \ 004027E0
    -37.6 70.0 29.5 -9.4 70.0 29.5 -37.6 32.2 29.5 -9.4 32.2 29.5 lights-doorway
    6.0 72.0 63.0 72.0 72.0 63.0 6.0 -2.0 63.0 72.0 -2.0 63.0 lights-doorway
    -39.0 72.0 38.0 32.0 72.0 38.0 -39.0 -2.0 38.0 32.0 -2.0 38.0 lights-doorway
;

: room25.phase5 ( -- )   \ 00402880
    $37 story-flag? if
        $2B story-flag? not if
            0 0 char-group-bit4? 0 1 char-group-bit4? or if
                $2B story-flag-set
            then
        then
    then
;

: room25.act00 ( -- )   \ 004028A0
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        4 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room25.act01 ( -- )   \ 004028C0
    self-wait-done
    0.0 -60.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    self-idle-or-end
;

: room25.act02 ( -- )   \ 004028D0
    2 message
    begin
        fiona-free? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 3 action-force
    self-idle-or-end
;

: room25.act03 ( -- )   \ 0047AB18
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room25.act04 ( -- )   \ 004028F0
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    $F $54 fade
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
    0 8 -19.086 -6.777 166 char-to-xz
    $12 state-flag-set
    $F $51 fade
    wait-fade
    $F 6 fade
    wait-fade
    $40A9 message
    wait-message
    $F 7 fade
    wait-fade
    $37 story-flag-set
    $A9 message-param-room
    $A9 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message
    wait-message
    2 char-unload
    0 state-flag-set
    $25 story-flag? not if
        $25 story-flag-set
        \ (nop-progress-74: no effect in this game)
    then
    $14 item-use
    6 door-lock
    0 self-scripted
    $18 state-flag-clear
    $12 state-flag-clear
    self-idle-or-end
;

: room25.act05 ( -- )   \ 004029F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    0 1 6 action-force
    0 counter-set
    $C04 self-anim
    0 1 char-wait-motion
    0 $2E 5 char-sound
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    begin
        1 $F char-in-area? not while
        yield
    repeat
    1 2 -1 char-camera
    1 wait-counter
    self-frames-reset
    $10 self-wait-frames
    0 self-scripted
    self-idle-or-end
;

: room25.act06 ( -- )   \ 00402A30
    self-wait-done
    hewie-bark
    self-wait-done
    1 camera-follow
    $3E -30.81 100.0 180 $FFFF $A self-move-to
    self-wait-done
    0 2 8 nav-group
    1 2 $30 nav-group
    $CF -35.0 20.3 0.2 50 hewie-go-to
    self-wait-done
    1 2 -1 char-camera
    $230 story-flag? not if
        $71 $63 item-count? not if
            10 hewie-trust
        then
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $230 story-flag-set
            $71 1 item-give-count
            0 $71 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $230 story-flag? if
            2 effect-remove
        then
    then
    $50 -35.79 15.06 100 $FFFF $A self-move-to
    self-wait-done
    0 3 8 nav-group
    1 3 $30 nav-group
    1 counter-set
    $196 18.0 6.0 0.1 100 hewie-go-to
    self-wait-done
    1 2 8 nav-group
    1 3 8 nav-group
    $18 state-flag-clear
    $25 0 406 hewie-to-room
    self-idle-or-end
;

: room25.act07 ( -- )   \ 00402B00
    self-wait-done
    60.0 37.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $249 story-flag-set
            0 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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

: room25.act08 ( -- )   \ 00402B60
    self-wait-done
    37.0 -44.0 self-turn-to-xz
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room25.act09 ( -- )   \ 00402B70
    self-wait-done
    1 self-through-exit
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60A self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $608 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        6 message
        wait-message
    then
    self-idle-or-end
;

: room25.act0A ( -- )   \ 00402BA0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 1 5 char-sound
    $14 message-param-room
    $14 item-use
    6 door-lock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room25.act0B ( -- )   \ 00402BE0
    self-wait-done
    -5.5 -99.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $288 story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: room25.act0C ( -- )   \ 00402C40
    self-wait-done
    $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
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

' room25.enter $25 0 room-script!
' room25.char-enter $25 6 room-script!
' room25.phase1 $25 1 room-script!
' room25.phase2 $25 2 room-script!
' room25.phase3 $25 3 room-script!
' room25.phase5 $25 5 room-script!
' room25.act00 $25 $00 action-script!
' room25.act01 $25 $01 action-script!
' room25.act02 $25 $02 action-script!
' room25.act03 $25 $03 action-script!
' room25.act04 $25 $04 action-script!
' room25.act05 $25 $05 action-script!
' room25.act06 $25 $06 action-script!
' room25.act07 $25 $07 action-script!
' room25.act08 $25 $08 action-script!
' room25.act09 $25 $09 action-script!
' room25.act0A $25 $0A action-script!
' room25.act0B $25 $0B action-script!
' room25.act0C $25 $0C action-script!

\ ---- room $26 ----------------------------------------------------------------------------------

\ the two doors ("left", "right") opening: byte 3 0 at once (2.25), else a step (0.075)
: room26.cmd00 ( b0 -- )  drop stub-step ;
\ the three rocking chairs ("movechair_1..3"; +0x30 the rock's phase in degrees, +0x34 its size,
\ +0x38 how fast it dies down; +0x10 the tilt), by byte 3: 0 all still; 1 a rocking step (6
\ degrees; each swing smaller, the first chair creaking at a volume by its size); 2 / 3 set
\ rocking at full size from their tilt now (swinging forward / back), with a creak
: room26.cmd01 ( b0 -- )  drop stub-step ;
\ the partner's target (+0xF35E0 on, +0xF35F0) 3 above the rocking chair (movechair_2): its seat
\ 6 ahead, tipped by its rock (90 x +0x34 x sin +0x30 degrees) and turned with it
: room26.cmd02 ( -- )  stub-step ;
\ the first rocking chair still rocking (+0x34 over 0.3)
: room26.cond00? ( -- flag )  stub-flag ;

: room26.enter ( -- )   \ 00402D50
    room-sounds
    1 1 $1000000 nav-group
    1 1 $4000000 nav-group
    $305 story-flag? if
        0 1 $14 door-bits
        1 1 $20000 nav-group
        0 1 object-show
        1 1 object-show
    else
        0 0 $14 door-bits
        0 1 $20000 nav-group
        0 0 object-show
        1 0 object-show
    then
    0 room26.cmd01
    0 $F1 $13 action
    $30E story-flag? not if
        2 room26.cmd01
        $30E story-flag-set
    then
    $202 story-flag? not if
        1 0 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        $28C story-flag? not if
            1 -17.5 1.0 47.0 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    $258 story-flag? $259 story-flag? not and if
        2 22.9 1.0 -9.7 flicker-sprite
    then
    $30C story-flag? if
        0 room26.cmd00
    then
;

: room26.act0D ( -- )   \ 00403680
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    6 0 pvar? if
        0 chance? if
            2 2 var-set
        then
    else 6 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else 6 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else 6 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    2 self-is? 6 self-is? or 7 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    4 stalker-alert? if
        2 0 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE 4 action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE 2 action
    then then then
    3 0 var-set
    6 pvar-inc
    exit
;

: room26.char-enter ( -- )   \ 00402E10
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
    $FE self-is? if
        9 state-flag? if
            7 ebit-set
            2 self-is? 6 self-is? or 7 self-is? or if
                $318 story-flag? not room26.cond00? and 2 creature-action? not and if
                    $FE camera-follow
                    0 $FE $E action
                else
                    room26.act0D
                then
            else
                room26.act0D
            then
        else
            7 ebit-clear
        then
    then
;

: room26.phase1 ( -- )   \ 00402EC0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 -1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 -4.0 0.0 4.0 4 7 0 zone
    1 6.0 0.0 -7.0 3 7 0 zone
    $258 story-flag? not if
        3 22.9 0.0 -9.7 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 600 var-set
                $1A 601 var-set
                $1B 2 var-set
                $1C 22900 var-set
                $1D 1000 var-set
                $1E -9700 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    8 ebit? not if
        1 char-here? 1 2 char-C4? not and if
            4 2.09 0.0 -2.09 $14 5 0 zone
            0 game-mode? if
                hewie-can-command? room26.cond00? and if
                    1 4 9 char-zone-bits? if
                        4 0 var-set
                        0 1 $11 action
                    then
                then
            then
        then
    then
;

: room26.phase2 ( -- )   \ 00402FA0
    0 6 char-in-area? 0 22 $3C char-heading? and if
        $FE char-here? not if
            5 0 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 7 $3C char-faces-area? if
        5 6 0 scene-change
    then
    0 8 char-in-area? 0 75 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 9 $3C char-faces-area? if
        5 7 0 scene-change
    then
    0 $A char-in-area? 0 90 $32 char-heading? and if
        5 $A 0 scene-change
    then
    0 $B $3C char-faces-area? if
        5 $10 0 scene-change
    then
    $258 story-flag? $259 story-flag? not and if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $F 4 scene-change
        then
    then
    $202 story-flag? not if
        2 -17.5 0.0 47.0 5 8 1 zone
        $FF 2 char-in-zone? if
            $202 story-flag-set
            0 0 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -17.5 0.0 47.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -17.5 0.0 47.0 0 0 sound
            $40 $48 noise
            1 -17.5 1.0 47.0 flicker-sprite
        then
    else $28C story-flag? not if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then then
;

: room26.act03 ( -- )   \ 00403350
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $305 story-flag-set
    1 1 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room26.act05 ( -- )   \ 00403400
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    $FE char-here? if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 2 object-anim
    1 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 1 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $305 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room26.act09 ( -- )   \ 004034A0
    1 self-scripted
    counter-inc
    4 ebit-set
    0 camera-follow
    9 state-flag-clear
    $FE char-here? 0 0 var? and if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $305 story-flag-set
    1 1 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: room26.act00 ( -- )   \ 004030A0
    $18 state-flag-set
    7 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $305 story-flag? not if
        3 ebit-set
    else
        3 ebit-clear
    then
    0 counter-set
    5 ebit-clear
    6 ebit-clear
    4 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 1 action-force
        5 ebit-set
        1 5 char-file-load
    then
    0 4 char-file-load
    $FE 6 char-file-load
    $B4 16.729 33.985 45 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    5 ebit? if
        1 self-look-at
        yield
        begin
            6 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    3 ebit? not if
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 0 object-anim
        1 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-show
        1 0 object-show
        0 1 object-anim
        1 1 object-anim
        self-frames-reset
        $E self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 1 $20000 nav-group
    $18 state-flag-clear
    9 state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] room26.act03 goto
                else
                    ['] room26.act05 goto
                then
            else $FF panic-stage? if
                ['] room26.act09 goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            7 ebit? $FE char-here? not and if
                7 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] room26.act09 goto
            then
            counter-inc
            0 camera-follow
            9 state-flag-clear
            $FE char-here? if
                $FE 0 stalker-mode
                $FE action-end
            then
            1 self-noclip
            0 2 object-anim
            1 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            0 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            0 1 6 char-sound
            self-wait-anim
            0 self-noclip
            $FE char-here? if
                $FE self-look-at
                yield
                $FE self-turn-to
                self-wait-done
            then
            $305 story-flag-clear
            0 1 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room26.act01 ( -- )   \ 00403230
    1 self-scripted
    self-wait-done
    $B4 17.237 27.889 45 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    6 ebit-set
    1 wait-counter
    1 self-noclip
    3 ebit? not if
        $8000 5 self-anim-9
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-wait-anim
    then
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                4 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    0 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            4 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                0 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room26.act02 ( -- )   \ 004032B0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    3 0 var? if
        $B4 15.0 29.0 45 $FFFF 5 self-move-to
        self-wait-done
    then
    3 0 var-set
    1 self-scripted
    0 1 var-set
    2 self-is? 6 self-is? or 7 self-is? or if
        0 3 object-anim
        1 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $C self-wait-frames
        $FE 0 6 char-sound
        self-frames-reset
        4 self-wait-frames
        0 2 var-set
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        9 state-flag? if
            2 avoid-prompt
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 9 action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room26.act04 ( -- )   \ 00403380
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $B4 15.0 29.0 45 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $1E self-wait-frames
    $FE $28 5 char-sound
    $1E threat-raise
    self-frames-reset
    $19 self-wait-frames
    $FE $28 5 char-sound
    $14 threat-raise
    self-frames-reset
    $17 self-wait-frames
    $FE $28 5 char-sound
    $A threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: room26.act06 ( -- )   \ 00403450
    self-wait-done
    23.3 4.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        3 message
        wait-message
        0 ebit-set
    else
        4 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: room26.act07 ( -- )   \ 0047AB28
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: room26.act08 ( -- )   \ 00403480
    self-wait-done
    1 ebit? not if
        6 message
        wait-message
        1 ebit-set
    else
        7 message
        wait-message
    then
    self-idle-or-end
;

: room26.act0A ( -- )   \ 004034F0
    self-wait-done
    $35 0.0 -31.5 180 $FFFF 5 self-move-to
    self-wait-done
    $17 state-flag-set
    1 self-scripted
    1 20.0 10.0 0.0 0.0 event-camera
    $30C story-flag? not if
        2 ebit? not if
            0 message
            wait-message
            2 ebit-set
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room26.act0B ( -- )   \ 00403550
    $18 state-flag-set
    1 self-scripted
    $86 1 item-count? $8A 1 item-count? and if
        9 ebit-clear
    else
        9 ebit-set
    then
    self-wait-done
    0 $35 0.0 -31.5 180 char-to-xz
    1 7.0 30.0 0.0 0.0 event-camera
    $17 state-flag-set
    4 6 0.0 5.0 -39.0 0 0 sound
    self-frames-reset
    $10 self-wait-frames
    9 ebit? if
        0 0.0 3.0 -39.0 flicker-sprite
    then
    self-frames-reset
    0 3 6 char-sound
    begin
        $1E frames? not while
        1 room26.cmd00
        yield
    repeat
    self-frames-reset
    self-wait-16
    9 ebit? if
        $A ebit-clear
        $86 1 item-count? not if
            $86 message-param-room
            $86 1 item-give-count
            0 $86 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
            $A ebit-set
        then
        $8A 1 item-count? not if
            $A ebit? if
                self-frames-reset
                self-wait-16
            then
            $8A message-param-room
            $8A 1 item-give-count
            0 $8A item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        0 effect-remove
    else
        $A message
        wait-message
    then
    $30C story-flag-set
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-clear
    $18 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: room26.act0C ( -- )   \ 00403660
    self-wait-done
    4 6 0.0 5.0 -39.0 0 0 sound
    self-frames-reset
    $1E self-wait-frames
    8 message
    wait-message
    self-idle-or-end
;

: room26.act0E ( -- )   \ 00403760
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $3A 3.123 11.435 -165 $FFFF 5 self-move-to
    self-wait-done
    $1305 self-anim
    self-wait-anim
    $FE 3 stalker-mode
    $318 story-flag-set
    self-idle-or-end
;

: room26.act0F ( -- )   \ 004037A0
    self-wait-done
    22.9 -9.7 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $259 story-flag-set
            2 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: room26.act10 ( -- )   \ 00403800
    self-wait-done
    -19.0 -19.0 self-turn-to-xz
    self-wait-done
    9 message
    wait-message
    self-idle-or-end
;

: room26.act11 ( -- )   \ 00403810
    self-wait-done
    2.0 -2.0 self-turn-to-xz
    self-wait-done
    8 ebit-set
    begin
        room26.cmd02
        4 var-inc
        4 180 var? not while
        yield
    repeat
    self-idle-or-end
;

: room26.act12 ( -- )   \ 00403830
    self-wait-done
    -17.5 47.0 self-turn-to-xz
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
            $28C story-flag-set
            1 effect-remove
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

: room26.act13 ( -- )   \ 00403890
    begin
        $FF 0 char-in-zone? if
            3 room26.cmd01
        then
        $FF 1 char-in-zone? if
            2 room26.cmd01
        then
        1 room26.cmd01
        yield
    again
;

' room26.enter $26 0 room-script!
' room26.char-enter $26 6 room-script!
' room26.phase1 $26 1 room-script!
' room26.phase2 $26 2 room-script!
' room26.act00 $26 $00 action-script!
' room26.act01 $26 $01 action-script!
' room26.act02 $26 $02 action-script!
' room26.act03 $26 $03 action-script!
' room26.act04 $26 $04 action-script!
' room26.act05 $26 $05 action-script!
' room26.act06 $26 $06 action-script!
' room26.act07 $26 $07 action-script!
' room26.act08 $26 $08 action-script!
' room26.act09 $26 $09 action-script!
' room26.act0A $26 $0A action-script!
' room26.act0B $26 $0B action-script!
' room26.act0C $26 $0C action-script!
' room26.act0D $26 $0D action-script!
' room26.act0E $26 $0E action-script!
' room26.act0F $26 $0F action-script!
' room26.act10 $26 $10 action-script!
' room26.act11 $26 $11 action-script!
' room26.act12 $26 $12 action-script!
' room26.act13 $26 $13 action-script!

\ ---- room $28 ----------------------------------------------------------------------------------

\ the pursuer's Pursuer_GrabHewieBehind
: room28.cond00? ( b0 -- flag )  drop stub-flag ;

: room28.enter ( -- )   \ 00403980
    $16 1.0 0 bgm
    $232 story-flag? not if
        0 -231.0 61.0 24.0 flicker-sprite
    then
    $260 story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
;

: room28.char-enter ( -- )   \ 004039B0
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
    1 self-is? if
        118 hewie-action? not if
            1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
                1 $13C -90 char-to-tri-facing
            then
        then
    then
;

: room28.phase1 ( -- )   \ 00403A10
    0 exit-usable? if
        0 exit-check
    then
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        4 map-page
    then
    $E story-flag? not 1 ebit? not and if
        0 6 char-in-area? if
            0 var-inc
            0 30 var? if
                1 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 0 action-force
                else
                    1 0 0 action-force
                then
                0 0 char-action? if
                    6 ebit-set
                then
            then
        else
            0 0 var-set
        then
        $FE 6 char-in-area? if
            1 var-inc
            1 30 var? if
                1 ebit-set
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 char-action? if
                    0 0 1 action-force
                else
                    1 0 1 action-force
                then
            then
        else
            1 0 var-set
        then
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
            44 fiona-started? if
                0 1 7 action
            else $FE 2 char-C4? not $FE 8 char-in-area? and if
                stalker-free? if
                    5 ebit? not if
                        0 0 8 nav-group
                        1 0 $30 nav-group
                        0 room28.cond00? if
                            5 ebit-set
                            0 $F1 8 action
                        else
                            1 0 8 nav-group
                            0 0 $30 nav-group
                        then
                    then
                then
            then then
        else 2 game-mode? not if
            0 control-action? if
                -163 97 -50 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                    0 -163 -50 $32 char-faces-xz? if
                        hewie-stays? if
                            0 1 5 action
                        then
                    then
                then
            then
        then then
    then
    2 -231.29 60.0 30.58 $32 20 0 zone
    3 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    3 ebit-set
                    $64 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    $260 story-flag? not 1 7 char-in-area? and if
        1 -149.92 60.0 -50.71 $36 36 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    1 -149.92 60.0 -50.71 $36 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
    3 -228.31 60.0 25.69 $23 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
;

: room28.phase2 ( -- )   \ 00403C10
    0 4 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 45 $32 char-heading? and if
        5 9 0 scene-change
    then
;

: room28.phase5 ( -- )   \ 00403C30
    1 char-here? if
        118 hewie-action? if
            1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
                1 $1EA -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else 1 0 char-in-nav-group? 1 $1C3 char-on-tri? or 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
            1 $13C char-to-tri
        then then
    then
;

: room28.act02 ( -- )   \ 00403D00
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $F $44 fade
    wait-fade
    8 state-flag-set
    $FE char-here? if
        0 $FE 3 action
    then
    $35 $36 door-copy
    $36 door-close-off-unlock
    exits-rebuild
    exit
;

: room28.act00 ( -- )   \ 00403C80
    $18 state-flag-set
    $E state-flag-set
    $13 state-flag-set
    1 self-scripted
    \ (nop-progress-18: no effect in this game)
    $E 3 $FF char-load
    $F 4 char-load-2
    self-wait-done
    6 ebit? if
        1 self-anim
    then
    room28.act02
    1 char-here? if
        8 0 -1 hewie-to-room
    then
    $E state-flag-clear
    $13 state-flag-clear
    $80 exit-check
    self-idle-or-end
;

: room28.act01 ( -- )   \ 00403CC0
    $18 state-flag-set
    $E state-flag-set
    $13 state-flag-set
    1 self-scripted
    $FE 0 0 char-camera
    $FE camera-follow
    \ (nop-progress-18: no effect in this game)
    $E 3 $FF char-load
    $F 4 char-load-2
    self-wait-done
    room28.act02
    1 char-here? if
        $2F 0 -1 hewie-to-room
    then
    $E state-flag-clear
    $13 state-flag-clear
    $81 exit-check
    self-idle-or-end
;

: room28.act03 ( -- )   \ 0047AB30
    begin
        yield
    again
;

: room28.act04 ( -- )   \ 00403D30
    self-wait-done
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room28.act05 ( -- )   \ 00403D50
    self-wait-done
    1 0 char-file-load
    hewie-bark
    self-wait-done
    $AE -152.54 -106.55 0 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    1 self-noclip
    $8000 5 self-anim-9
    self-wait-anim
    0 0 $30 nav-group
    0 self-noclip
    $260 story-flag? not if
        $260 story-flag? not if
            $A state-flag? 0 char-busy? not or if
                $70 $63 item-count? not if
                    10 hewie-trust
                then
                $70 message-param-room
                $70 $63 item-count? if
                    $8010 message
                    wait-message
                else
                    $260 story-flag-set
                    $70 1 item-give-count
                    0 $70 item-tab
                    0 $F9 $8C action-force
                    $83 $85 0.0 0.0 0.0 0 0 sound
                    $8011 message
                    wait-message
                then
                $260 story-flag? if
                    1 effect-remove
                then
            then
        then
        1 counter-set
        $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 0 8 nav-group
        1 0 $30 nav-group
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        1 0 8 nav-group
        0 0 $30 nav-group
    else
        1 counter-set
        $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        $102 self-anim
        self-wait-anim
        2 self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: room28.act06 ( -- )   \ 00403E40
    self-wait-done
    $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    5 ebit-clear
    self-idle-or-end
;

: room28.act07 ( -- )   \ 00403E90
    self-wait-done
    hewie-bark
    self-wait-done
    $1EA -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    self-idle-or-end
;

: room28.act08 ( -- )   \ 00403EE0
    $75 0 hewie-action
    begin
        117 hewie-action? while
        yield
    repeat
    1 $1C3 char-on-tri? 1 $1D0 char-on-tri? or 1 $1EA char-on-tri? or if
        0 1 6 action
    else
        1 0 8 nav-group
        0 0 $30 nav-group
        5 ebit-clear
        5 hewie-trust
    then
    self-idle-or-end
;

: room28.act09 ( -- )   \ 00403F20
    self-wait-done
    90 self-turn-angle
    self-wait-done
    4 ebit? not if
        3 message
        wait-message
        4 ebit-set
    else
        4 message
        wait-message
        4 ebit-clear
    then
    self-idle-or-end
;

' room28.enter $28 0 room-script!
' room28.char-enter $28 6 room-script!
' room28.phase1 $28 1 room-script!
' room28.phase2 $28 2 room-script!
' room28.phase5 $28 5 room-script!
' room28.act00 $28 $00 action-script!
' room28.act01 $28 $01 action-script!
' room28.act02 $28 $02 action-script!
' room28.act03 $28 $03 action-script!
' room28.act04 $28 $04 action-script!
' room28.act05 $28 $05 action-script!
' room28.act06 $28 $06 action-script!
' room28.act07 $28 $07 action-script!
' room28.act08 $28 $08 action-script!
' room28.act09 $28 $09 action-script!

\ ---- room $29 ----------------------------------------------------------------------------------

\ the stalker in play is chasing (+0x153C 2, 6 or 7, not +0xC4 2) with the progress state 2: in
\ this room, whether the camera sees it; elsewhere 1
: room29.cond00? ( -- flag )  stub-flag ;
\ no thing of kind 3 lies about
: room29.cond01? ( -- flag )  stub-flag ;

: room29.enter ( -- )   \ 00403F90
    $31D story-flag? if
        2 1 object-show
    then
    0 24.35 9.0 19.7 0 effect-86
    $228 story-flag? not if
        1 11.0 12.0 -58.5 flicker-sprite
    then
    $34 story-flag? $21 story-flag? not and if
        1 0 $1000010 nav-group
        3 char-unload
        2 char-activate
        2 $9000 1 0 char-anim-hold
        2 $83 12.913 14.864 90 char-to-xz
    then
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    0 0 $14 door-bits
    $349 story-flag? if
        $93 1 item-count? not room29.cond01? and if
            0 1 $14 door-bits
            2 ebit-set
        then
    then
;

: room29.char-enter ( -- )   \ 00404040
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    0 1 -1 area-camera
;

: room29.phase1 ( -- )   \ 00404080
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 7.42 0.0 22.15 $F 9 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 9.0 hewie-look-zone
            then
        then
    then
;

: room29.phase2 ( -- )   \ 00404100
    $34 story-flag? $21 story-flag? not and if
    else $31D story-flag? not if
        0 5 char-in-area? 0 0 $2D char-heading? and if
            2 game-mode? $FF panic-stage? not and if
                room29.cond00? if
                    5 0 6 scene-change
                else
                    $8016 scene-ending
                then
            else
                5 1 0 scene-change
            then
        then
    then then
    $228 story-flag? not if
        0 1 7 4 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $34 story-flag? $21 story-flag? not and if
        0 2 $14 0 $2D char-touching-facing? if
            $22 story-flag? not if
                5 2 0 scene-change
            else 1 ebit? not if
                5 6 0 scene-change
            then then
        then
    then
    0 6 char-in-area? 0 -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 7 char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    2 ebit? if
        0 8 $3C char-faces-area? if
            5 7 4 scene-change
        then
    then
;

: room29.phase5 ( -- )   \ 00404198
    $34 story-flag? $21 story-flag? not and if
        2 action-end
        2 char-done
    then
;

: room29.act00 ( -- )   \ 004041B0
    \ (nop-progress-24: no effect in this game)
    1 self-scripted
    $FE $29 116 2 stalker-to-room
    $FE $74 180 char-to-tri-facing
    $FE 0 -1 char-camera
    $FE 1 char-visible
    $FE action-end
    $FE char-done
    $F $44 fade
    3 0 movie-play
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
    1 char-here? if
        1 action-end
        1 char-done
        0 ebit-set
    then
    $20 door-open-set
    doors-room-in
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
    $FE action-end
    $FE char-done
    0 self-scripted
    8 state-flag-set
    \ (nop-progress-14: no effect in this game)
    $20 door-open-clear
    0 ebit? if
        $80 exit-check
    else
        $81 exit-check
    then
    \ (nop-progress-24: no effect in this game)
    $31D story-flag-set
    $22 resident-flag-set
    self-idle-or-end
;

: room29.act01 ( -- )   \ 00404280
    self-wait-done
    $98 7.28 20.316 15 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 20.0 0.0 event-camera
    $A01 $A self-anim-blend
    0 message
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room29.act02 ( -- )   \ 004042D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    4 0 movie-play
    1 cutscene-start
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
    0 $4B -13.414 -1.767 -6 char-to-xz
    0 self-move-16
    2 $83 12.913 14.864 90 char-to-xz
    2 $9000 1 0 char-anim-hold
    $22 story-flag-set
    1 0 char-visible
    1 action-end
    camera-restart
    $F $41 fade
    wait-fade
    $29 resident-flag-set
    7 message-param-room
    7 1 item-give-count
    0 7 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room29.act03 ( -- )   \ 004043D0
    self-wait-done
    11.0 -58.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $228 story-flag-set
            1 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: room29.act04 ( -- )   \ 00404428
    self-wait-done
    $A01 $A self-anim-blend
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room29.act05 ( -- )   \ 00404440
    self-wait-done
    0 $9C 12.0 15.0 90 char-to-xz
    1 10.0 10.0 0.0 2.0 event-camera
    $17 state-flag-set
    1 self-scripted
    self-frames-reset
    self-wait-16
    2 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room29.act06 ( -- )   \ 00404490
    1 self-scripted
    self-wait-done
    2 $9002 0 7 char-anim-hold
    2 wait-char-anim
    2 $9001 1 7 char-anim-hold
    1 ebit-set
    self-idle-or-end
;

: room29.act07 ( -- )   \ 004044B0
    self-wait-done
    -3.61 2.23 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $93 message-param-room
    $93 $63 item-count? if
        $8010 message
        wait-message
    else
        $93 1 item-give-count
        2 ebit-clear
        0 0 $14 door-bits
        0 $93 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8011 message
        wait-message
    then
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room29.act08 ( -- )   \ 00404500
    self-wait-done
    0 24.35 9.0 19.7 0 effect-86
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    2 3 $FF char-load
    3 char-unload
    3 0 movie-play
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: room29.act09 ( -- )   \ 004045E0
    self-wait-done
    0 24.35 9.0 19.7 0 effect-86
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    2 3 $FF char-load
    3 char-unload
    4 0 movie-play
    1 cutscene-start
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
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room29.enter $29 0 room-script!
' room29.char-enter $29 6 room-script!
' room29.phase1 $29 1 room-script!
' room29.phase2 $29 2 room-script!
' room29.phase5 $29 5 room-script!
' room29.act00 $29 $00 action-script!
' room29.act01 $29 $01 action-script!
' room29.act02 $29 $02 action-script!
' room29.act03 $29 $03 action-script!
' room29.act04 $29 $04 action-script!
' room29.act05 $29 $05 action-script!
' room29.act06 $29 $06 action-script!
' room29.act07 $29 $07 action-script!
' room29.act08 $29 $08 action-script!
' room29.act09 $29 $09 action-script!

\ ---- room $2A ----------------------------------------------------------------------------------

\ the box and the grate, by byte 3: 0 the box's +0x28 on by 0.4; the grate's +0x14 (an angle) 1
\ back 1.5 degrees, 2 on 0.5, 3 back 0.5
: room2A.cmd00 ( b0 -- )  drop stub-step ;
\ Room2A_Cmd01
: room2A.cmd01 ( b0 -- )  drop stub-step ;
\ room 0x2A: its effect (Room2AWisps_vtable) started at (220, 0, -100)
: room2A.cmd02 ( -- )  stub-step ;
\ the 0x14-byte effect (Effect78BC0_vtable) started with the command's parameters (from byte 3)
: room2A.cmd03 ( b0 -- )  drop stub-step ;

: room2A.enter ( -- )   \ 00404740
    room-sounds
    1 1 8 nav-group
    $C story-flag? not if
        0 resident-flag? if
            $AE story-flag-set
        then
        0 $275 -90 char-to-tri-facing
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
        1 action-end
        1 char-done
        5 state-flag-clear
        $D state-flag-clear
        $13 state-flag-clear
        $1B state-flag-set
        $1D state-flag-set
        $11 state-flag-set
        $14 state-flag-set
        7 door-unlock
        9 door-unlock
        $A door-unlock
        $B door-unlock
        $13 door-unlock
        $22 door-unlock
        $26 door-unlock
        $12 door-unlock
        $2F door-unlock
        2 door-unlock
        0 door-unlock
        3 door-unlock
        6 door-unlock
        8 door-unlock
        $14 door-unlock
        $15 door-unlock
        $19 door-unlock
        $1E door-unlock
        $34 door-unlock
        $35 door-close-off-unlock
        $18 door-close-off-unlock
        $1C door-close-off-unlock
        $1D door-close-off-unlock
        1 door-close-off-unlock
        $D door-close-off-unlock
        exits-rebuild
        0 pvar-inc
        0 pvar-inc
        0 pvar-inc
        1 pvar-inc
        1 pvar-inc
        1 pvar-inc
        8 state-flag-set
        0 0 1 action
    else
        $16 1.0 0 bgm
    then
    3 0 $14 door-bits
    4 0 $14 door-bits
    2 story-flag? if
        3 story-flag? not if
            0 30 var-set
            $2A 0 360 hewie-to-room
            0 1 3 action
            3 1 $14 door-bits
        else
            4 1 $14 door-bits
        then
    then
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
        room2A.cmd02
        0 room2A.cmd03
    else
        $A 128.0 56.7 13.0 $E $80 $80 $80 $40 specks
        $10 152.0 56.7 13.0 $C $80 $80 $80 $40 specks
        1 room2A.cmd03
    then
    $33 story-flag? not if
        1 2 $20000 nav-group
    else
        2 1 object-show
        3 1 object-show
        2 1 $14 door-bits
    then
    0 $A 0.812 0.687 0.187 0.312 zone-rect
    $214 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $229 story-flag? not if
            0 185.7 0.9 -35.4 flicker-sprite
        then
    then
;

: room2A.char-enter ( -- )   \ 004048E0
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
    $D story-flag? not if
        hewie-controlled? not if
            0 self-is? 1 exit-taken? and if
                0 1 char-to-exit
                hewie-controlled? not if
                    0 2 1 char-camera
                    0 camera-follow
                else
                    1 2 1 char-camera
                    1 camera-follow
                then
            then
        else 1 self-is? 1 exit-taken? and if
            1 1 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then then
        1 2 1 area-camera
    else
        hewie-controlled? not if
            0 self-is? 1 exit-taken? and if
                0 1 char-to-exit
                hewie-controlled? not if
                    0 6 5 char-camera
                    0 camera-follow
                else
                    1 6 5 char-camera
                    1 camera-follow
                then
            then
        else 1 self-is? 1 exit-taken? and if
            1 1 char-to-exit
            hewie-controlled? not if
                0 6 5 char-camera
                0 camera-follow
            else
                1 6 5 char-camera
                1 camera-follow
            then
        then then
        1 6 5 area-camera
    then
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 5 4 char-camera
            0 camera-follow
        else
            1 5 4 char-camera
            1 camera-follow
        then
    then then
    2 5 4 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 3 2 char-camera
                0 camera-follow
            else
                1 3 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 2 char-camera
            0 camera-follow
        else
            1 3 2 char-camera
            1 camera-follow
        then
    then then
    3 3 2 area-camera
;

: room2A.phase1 ( -- )   \ 00404A20
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    0 5 char-in-area? 0 char-busy? not and if
        $1A 3 3 char-load
        3 exit-check
    then
    $B 0 -1 1 chars-area-camera
    $D story-flag? not if
        $C 2 1 1 chars-area-camera
    else
        $C 6 5 1 chars-area-camera
    then
    $D 0 -1 1 chars-area-camera
    $E 1 0 1 chars-area-camera
    $F 1 0 1 chars-area-camera
    $10 3 2 1 chars-area-camera
    $14 4 3 1 chars-area-camera
    $15 0 -1 1 chars-area-camera
    $1B 1 0 1 chars-area-camera
    $1C 5 4 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? 0 7 char-entered-area? or 0 8 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        $1A 3 3 char-load
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-left-area? if
        3 0 char-remove
    then
    0 0 char-action? $FF panic-stage? not and 0 $16 char-entered-area? and if
        0 0 7 action
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        35 fiona-started? if
            0 $19 char-in-area? 1 $1A char-in-area? and 0 -16 137 $32 char-faces-xz? and $33 story-flag? not and $FE char-here? not and if
                hewie-stays? if
                    0 1 $B action
                then
            else 238 0 -135 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and 0 238 -135 $32 char-faces-xz? and $231 story-flag? not and if
                hewie-stays? if
                    0 0 $10 action
                then
            then then
        then
    then
    9 ebit? 1 char-busy? not and if
        1 ebit-clear
        $18 state-flag-clear
    then
    5 ebit? 4 ebit? not and if
        2 225.0 0.0 -44.0 $19 10 0 zone
        0 2 2 char-zone-bits? if
            4 ebit-set
            $F3 action-end
            0 $F3 $14 action
        then
    then
    4 ebit? 7 ebit? not and if
        0 $B char-entered-area? if
            7 ebit-clear
            3 0 char-remove
            4 0 char-remove
            $F1 action-end
            $F3 action-end
        then
    then
    2 story-flag? if
        3 story-flag? not if
            0 0 var? if
                $1E chance? if
                    1 1 6 char-sound
                    0 180 var-set
                else
                    0 60 var-set
                then
            else
                0 var-dec
            then
        then
    then
    1 244.4 11.0 82.19 $2C 13 0 zone
    3 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    3 ebit-set
                    $32 chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 244.4 11.0 82.19 $2C 13 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    $33 story-flag? not if
        1 $1A char-in-area? if
            3 -23.45 -10.0 138.0 $28 20 0 zone
            6 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 3 9 char-zone-bits? if
                            6 ebit-set
                            $64 chance? if
                                $1F 3 var-set
                                0 1 $88 action
                            then
                        then
                    then
                then
            then
        then
    then
    $231 story-flag? not if
        4 252.44 0.0 -156.9 $30 20 0 zone
        8 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
        4 252.44 0.0 -156.9 $30 20 0 zone
        1 char-here? 0 game-mode? and if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    $1F 4 var-set
                    $1F 0.0 hewie-look-zone
                then
            then
        then
    then
;

: room2A.phase2 ( -- )   \ 00404CE0
    2 story-flag? if
        3 story-flag? not if
            0 1 $A 0 $5A char-touching-facing? if
                5 0 0 scene-change
            then
        else
            5 228.1 8.0 77.57 $A 0 0 zone
            0 5 2 char-zone-bits? 0 228 77 $5A char-faces-xz? and if
                5 $15 0 scene-change
            then
        then
    then
    0 $A $3C char-faces-area? if
        5 2 0 scene-change
    then
    $33 story-flag? not if
        0 $17 char-in-area? 0 -45 $32 char-heading? and if
            5 9 0 scene-change
        then
    then
    0 $18 char-in-area? 0 -45 $32 char-heading? and if
        5 $A 0 scene-change
    then
    0 $11 char-in-area? $29 story-flag? not and if
        5 4 0 scene-change
    then
    0 $12 char-in-area? $2A story-flag? not and if
        5 5 0 scene-change
    then
    0 $13 $32 char-faces-area? if
        5 6 0 scene-change
    then
    $214 story-flag? not if
        0 185.7 -0.1 -35.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $214 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            185.7 -0.1 -35.4 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 185.7 -0.1 -35.4 0 0 sound
            $40 $2CF noise
            0 185.7 0.9 -35.4 flicker-sprite
        then
    else $229 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then then
;

: room2A.phase5 ( -- )   \ 00404E00
    2 story-flag? 3 story-flag? not and if
        1 action-end
        1 char-done
    then
    $29 story-flag-set
    $2A story-flag-set
    1 ebit? if
        $18 state-flag-clear
    then
    1 char-busy? if
        1 3 char-in-nav-group? if
            1 action-end
            1 $282 235.0 -126.0 0 char-to-xz
            1 3 8 nav-group
            $18 state-flag-clear
        then
    then
;

: room2A.act00 ( -- )   \ 00404E50
    $18 state-flag-set
    1 self-scripted
    3 story-flag-set
    self-wait-done
    228.1 77.57 self-turn-to-xz
    self-wait-done
    $C0A $A self-anim-blend
    self-wait-anim
    $C message
    wait-message
    $C0B $A self-anim-blend
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
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    3 0 $14 door-bits
    4 1 $14 door-bits
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
    0 $167 218.95 74.59 166 char-to-xz
    1 action-end
    1 char-done
    camera-restart
    $16 1.0 0 bgm
    $F $51 fade
    wait-fade
    $B message
    wait-message
    $205 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room2A.act01 ( -- )   \ 00404F30
    2 partner-load
    $E 3 $FF char-load
    $F 4 char-load-2
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    \ (nop-progress-24: no effect in this game)
    2 char-unload
    \ (nop-progress-48: no effect in this game)
    2 0 0 music
    4 0 0 music
    5 ebit-set
    0 $F1 $12 action
    0 $F3 $13 action
    $C story-flag-set
    $16 1.0 0 bgm
    $28 state-flag-clear
    $F 1 fade
    wait-fade
    $200 item-give
    self-idle-or-end
;

: room2A.act02 ( -- )   \ 00404FA0
    self-wait-done
    250.0 87.0 self-turn-to-xz
    self-wait-done
    2 story-flag? not if
        $A00 self-anim
        self-frames-reset
        $1E self-wait-frames
        0 ebit? not if
            3 message
            wait-message
            0 ebit-set
        else
            4 message
            wait-message
        then
        self-wait-anim
    else
        5 message
        wait-message
    then
    self-idle-or-end
;

: room2A.act03 ( -- )   \ 00404FD0
    self-wait-done
    1 $168 228.1 77.57 0 char-to-xz
    1 self-scripted
    $1004 0 self-anim-blend
    begin
        yield
    again
;

: room2A.act04 ( -- )   \ 00404FF0
    self-wait-done
    1 self-anim
    self-wait-anim
    -1 self-move-16
    6 message
    self-frames-reset
    7 self-wait-frames
    wait-message
    $29 story-flag-set
    self-idle-or-end
;

: room2A.act05 ( -- )   \ 00405008
    self-wait-done
    7 message
    wait-message
    $2A story-flag-set
    self-idle-or-end
;

: room2A.act06 ( -- )   \ 00405020
    self-wait-done
    65.64 70.94 self-turn-to-xz
    self-wait-done
    8 message
    wait-message
    self-idle-or-end
;

: room2A.act07 ( -- )   \ 00405030
    self-wait-done
    120 self-turn-angle
    self-wait-done
    $402 self-anim
    self-wait-anim
    -1 self-move-16
    2 message
    wait-message
    self-idle-or-end
;

: room2A.act08 ( -- )   \ 00405050
    self-wait-done
    185.7 -35.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $229 story-flag-set
            0 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: room2A.act0E ( -- )   \ 00405230
    1 self-scripted
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    counter-inc
    -90 self-turn-angle
    self-wait-done
    2 wait-counter
    self-frames-reset
    8 self-wait-frames
    1 0 6 char-sound
    begin
        3 counter? not while
        0 room2A.cmd00
        yield
    repeat
    4 wait-counter
    0 2 6 char-sound
    begin
        5 counter? not while
        1 room2A.cmd00
        yield
    repeat
    begin
        6 counter? not while
        2 room2A.cmd00
        yield
    repeat
    begin
        7 counter? not while
        3 room2A.cmd00
        yield
    repeat
    8 wait-counter
    $A ebit-clear
    $8275 item-give
    0 self-scripted
    self-idle-or-end
;

: room2A.act09 ( -- )   \ 004050B0
    self-wait-done
    $A ebit? if
        0 counter-set
        2 ebit-set
        ['] room2A.act0E goto
    then
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    1 self-scripted
    0 $2A7 -25.0 168.0 -80 char-to-xz
    1 13.0 20.0 0.0 0.0 event-camera
    self-frames-reset
    self-wait-16
    9 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    0 $21D -21.0 168.0 -90 char-to-xz
    $17 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room2A.act0A ( -- )   \ 00405120
    self-wait-done
    $DC -12.99 136.933 -90 $FFFF 5 self-move-to
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    $A message
    wait-message
    $802 self-anim
    self-wait-anim
    -1 self-move-16
    self-idle-or-end
;

: room2A.act0C ( -- )   \ 004051D0
    hewie-bark
    self-wait-done
    $346 -35.0 137.0 90 $FFFF 5 self-move-to
    self-wait-done
    $DC -12.99 136.933 90 $204 5 self-move-to
    self-wait-done
    0 self-noclip
    -1 self-move-16
    $18 state-flag-clear
    0 self-scripted
    1 ebit-clear
    self-idle-or-end
;

: room2A.act0F ( -- )   \ 00405290
    1 wait-counter
    0 self-doorway-fade
    1 $80808080 1 char-tint
    1 $346 -35.0 137.0 0 char-to-xz
    $FF 7 -1 char-camera
    1 self-noclip
    $345 -33.792 160.0 0 $FFFF 5 self-move-to
    self-wait-done
    counter-inc
    $194 -33.905 165.0 0 $204 5 self-move-to
    self-wait-done
    self-frames-reset
    $A self-wait-frames
    counter-inc
    $34D $FFFF 6 self-move-tri
    self-wait-done
    counter-inc
    self-frames-reset
    $40 self-wait-frames
    counter-inc
    self-frames-reset
    8 self-wait-frames
    counter-inc
    self-frames-reset
    $10 self-wait-frames
    counter-inc
    self-frames-reset
    8 self-wait-frames
    0 2 $20000 nav-group
    $2F door-lock
    hewie-controlled? not if
        0 5 4 char-camera
        0 camera-follow
    else
        1 5 4 char-camera
        1 camera-follow
    then
    $18 state-flag-clear
    1 ebit-clear
    $33 story-flag-set
    counter-inc
    1 self-doorway-fade
    30 hewie-trust
    $34E $FFFF 6 self-move-tri
    self-wait-done
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: room2A.act0B ( -- )   \ 00405150
    1 self-scripted
    9 ebit-set
    1 ebit-set
    2 ebit-clear
    $18 state-flag-set
    self-wait-done
    hewie-bark
    self-wait-done
    $DC -12.99 136.933 -90 $FFFF $A self-move-to
    self-wait-done
    9 ebit-clear
    $A ebit-set
    1 self-noclip
    $359 -30.0 137.0 -90 $204 5 self-move-to
    self-wait-done
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-anim
    begin
        0 $19 char-in-area? if
            44 fiona-started? if
                $A ebit-clear
                ['] room2A.act0C goto
            then
        then
        0 $17 char-in-area? if
            44 fiona-started? 35 fiona-started? or 45 fiona-started? or if
                0 counter-set
                0 $F2 $D action
                ['] room2A.act0F goto
            then
            2 ebit? if
                ['] room2A.act0F goto
            then
        then
        yield
    again
;

: room2A.act0D ( -- )   \ 00405210
    begin
        fiona-free? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 $E action-force
    self-idle-or-end
;

: room2A.act10 ( -- )   \ 00405330
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    1 char-busy? not if
        0 1 $11 action-force
        $C00 self-anim
        0 1 char-wait-motion
        0 $2E 5 char-sound
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $18 state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: room2A.act11 ( -- )   \ 00405360
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $1B3 232.55 -97.32 173 $FFFF $A self-move-to
    self-wait-done
    0 3 8 nav-group
    1 3 $30 nav-group
    $1A1 238.0 -135.0 1.0 35 hewie-go-to
    self-wait-done
    $231 story-flag? not if
        $37 $FFFF 6 self-move-tri
        self-wait-done
        $1C03 self-anim
        self-wait-anim
        $A state-flag? 0 char-busy? not or if
            $95 $63 item-count? not if
                10 hewie-trust
            then
            $95 message-param-room
            $95 $63 item-count? if
                $8010 message
                wait-message
            else
                $231 story-flag-set
                $95 1 item-give-count
                0 $95 item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8011 message
                wait-message
            then
        then
    then
    238.0 -135.0 self-turn-to-xz
    self-wait-done
    $1A1 238.0 -135.0 1.0 35 hewie-go-to
    self-wait-done
    1 3 8 nav-group
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room2A.act12 ( -- )   \ 00405420
    3 char-unload
    $E char-activate
    $E 266.9 75.5 -138.7 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: room2A.act13 ( -- )   \ 00405450
    4 3 char-hand-over
    $F char-activate
    $F 229.0 38.3 -55.3 0 char-to-xyz
    begin
        $32 chance? if
            $F $9000 1 0 char-anim-hold
        else
            $F $9001 1 0 char-anim-hold
        then
        $F wait-char-anim
    again
;

: room2A.act14 ( -- )   \ 00405480
    $F $A 6 char-sound
    $F $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 room2A.cmd01
    1 room2A.cmd01
    self-idle-or-end
;

: room2A.act15 ( -- )   \ 004054A0
    self-wait-done
    228.1 77.57 self-turn-to-xz
    self-wait-done
    $800 $A self-anim-blend
    self-wait-anim
    $D message
    wait-message
    $802 $A self-anim-blend
    self-wait-anim
    self-idle-or-end
;

: room2A.act16 ( -- )   \ 004054C0
    self-wait-done
    $A 128.0 56.7 13.0 $E $80 $80 $80 $40 specks
    $10 152.0 56.7 13.0 $C $80 $80 $80 $40 specks
    1 room2A.cmd03
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
    4 1 $14 door-bits
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room2A.enter $2A 0 room-script!
' room2A.char-enter $2A 6 room-script!
' room2A.phase1 $2A 1 room-script!
' room2A.phase2 $2A 2 room-script!
' room2A.phase5 $2A 5 room-script!
' room2A.act00 $2A $00 action-script!
' room2A.act01 $2A $01 action-script!
' room2A.act02 $2A $02 action-script!
' room2A.act03 $2A $03 action-script!
' room2A.act04 $2A $04 action-script!
' room2A.act05 $2A $05 action-script!
' room2A.act06 $2A $06 action-script!
' room2A.act07 $2A $07 action-script!
' room2A.act08 $2A $08 action-script!
' room2A.act09 $2A $09 action-script!
' room2A.act0A $2A $0A action-script!
' room2A.act0B $2A $0B action-script!
' room2A.act0C $2A $0C action-script!
' room2A.act0D $2A $0D action-script!
' room2A.act0E $2A $0E action-script!
' room2A.act0F $2A $0F action-script!
' room2A.act10 $2A $10 action-script!
' room2A.act11 $2A $11 action-script!
' room2A.act12 $2A $12 action-script!
' room2A.act13 $2A $13 action-script!
' room2A.act14 $2A $14 action-script!
' room2A.act15 $2A $15 action-script!
' room2A.act16 $2A $16 action-script!

\ ---- room $2B ----------------------------------------------------------------------------------

\ character kind 0x1A: byte 3 0 starts Kind26_MoveToB(2, -290, 42); else waits (2) until
\ Kind26_MoveDoneB says done
: room2B.cmd00 ( b0 -- )  drop stub-step ;

: room2B.enter ( -- )   \ 00405640
    room-sounds
    $16 1.0 0 bgm
    0 ebit-clear
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    then
    $213 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
    then
    0 5 0.812 0.687 0.187 0.312 zone-rect
    $222 story-flag? not if
        0 -202.89 1.0 -6.76 flicker-sprite
    then
    $240 story-flag? $241 story-flag? not and if
        1 -283.8 1.0 15.5 flicker-sprite
    then
    $1E chance? if
        0 $F1 6 action
    else
        $1A action-end
        $1A char-done
    then
;

: room2B.char-enter ( -- )   \ 004056D0
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
    $FE self-is? if
        $A state-flag? 0 3 char-in-area? and $30B story-flag? not and $37 exit-door-open? and if
            0 $FE 0 action
        then
    then
;

: room2B.phase1 ( -- )   \ 00405730
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    $FE char-busy? if
        $A state-flag? not if
            $FE action-end
            0 ebit-clear
        then
    then
    $213 story-flag? not if
        0 -120.0 0.0 -13.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $213 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -120.0 0.0 -13.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -120.0 0.0 -13.0 0 0 sound
            $40 $124 noise
        then
    then
    $240 story-flag? not if
        2 -283.8 0.0 15.5 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 576 var-set
                $1A 577 var-set
                $1B 1 var-set
                $1C -283800 var-set
                $1D 1000 var-set
                $1E 15500 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    $1A char-here? 0 game-mode? and if
        1 char-here? if
            $1A 1 70 chars-within? if
                $1A 0.0 hewie-look-char
            then
        then
    then
    2 ebit? not if
        3 -304.86 0.0 49.5 $14 5 0 zone
        $1A 3 8 char-zone-bits? if
            2 ebit-set
            $1A 3 6 char-sound
        then
    then
;

: room2B.phase2 ( -- )   \ 00405850
    0 4 char-in-area? 0 -140 0 $3C char-faces-xz? and if
        5 3 0 scene-change
    then
    0 5 char-in-area? 0 90 $3C char-heading? and if
        5 4 0 scene-change
    then
    0 6 char-in-area? 0 0 $3C char-heading? and if
        5 5 0 scene-change
    then
    $222 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    $240 story-flag? $241 story-flag? not and if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: room2B.act00 ( -- )   \ 004058B0
    self-wait-done
    $A8 -178.0 43.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 exit-door-open? if
        $1F -212.0 8.0 -180 $FFFF 5 self-move-to
        self-wait-done
        $1600 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 4 self-move-slot
        self-wait-done
        $FE 3 stalker-mode
        $30B story-flag-set
    then
    self-idle-or-end
;

: room2B.act01 ( -- )   \ 00405900
    self-wait-done
    -202.89 -6.76 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $222 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room2B.act02 ( -- )   \ 00405960
    self-wait-done
    -283.8 15.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $241 story-flag-set
            1 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
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

: room2B.act03 ( -- )   \ 004059C0
    self-wait-done
    -140.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        0 message
        wait-message
        1 ebit-set
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room2B.act04 ( -- )   \ 004059E0
    self-wait-done
    180 self-turn-angle
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: room2B.act05 ( -- )   \ 004059F0
    self-wait-done
    0 story-flag? not if
        0 self-turn-angle
        self-wait-done
        4 message
        wait-message
    else
        $1C -190.323 61.999 0 $FFFF 5 self-move-to
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $1200 self-anim
        self-wait-anim
        0 0 6 char-sound
        $1203 self-anim
        self-frames-reset
        self-wait-16
        $1202 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        3 message
        wait-message
    then
    self-idle-or-end
;

: room2B.act06 ( -- )   \ 00405A40
    3 char-unload
    $1A char-activate
    $1A -240.0 0.0 43.0 90 char-to-xyz
    $1A $9000 1 0 char-anim-hold
    0 room2B.cmd00
    1 room2B.cmd00
    3 0 char-remove
    self-idle-or-end
;

' room2B.enter $2B 0 room-script!
' room2B.char-enter $2B 6 room-script!
' room2B.phase1 $2B 1 room-script!
' room2B.phase2 $2B 2 room-script!
' room2B.act00 $2B $00 action-script!
' room2B.act01 $2B $01 action-script!
' room2B.act02 $2B $02 action-script!
' room2B.act03 $2B $03 action-script!
' room2B.act04 $2B $04 action-script!
' room2B.act05 $2B $05 action-script!
' room2B.act06 $2B $06 action-script!

\ ---- room $2C ----------------------------------------------------------------------------------

: room2C.char-enter ( -- )   \ 0041D140
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
;

: room2C.phase1 ( -- )   \ 0041D1C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    0 2 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
            $FE 0 char-file-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        \ (nop-progress-14: no effect in this game)
    then
    $5B story-flag? $5C story-flag? not and if
        0 4 char-left-area? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 0 action-force
        then
    then
    $5C story-flag? $71 story-flag? not and if
        0 stalker-alert? not if
            $FE 0 stalker-mode
        then
    then
;

: room2C.enter ( -- )   \ 0047ACB4
;

: room2C.act00 ( -- )   \ 0041D220
    $5C story-flag-set
    $18 state-flag-set
    1 self-scripted
    0 exit-door-open? not if
        0 1 self-door-knock
        self-frames-reset
        $10 self-wait-frames
    then
    0 0 self-door-knock
    self-wait-done
    0 counter-set
    $FE char-activate
    $FE $2C 46 2 stalker-to-room
    stalker-item-cooldown
    $FE $2E 0.0 192.0 -180 char-to-xz
    0 $FE 1 action
    $F04 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 wait-counter
    0 1 char-visible
    $FE 0 0 char-camera
    $FE camera-follow
    1 20.0 10.0 180.0 0.0 event-camera
    0 exit-door-open? if
        $34 door-open-clear
    then
    doors-room-in
    $34 door-unlock
    self-frames-reset
    $3C self-wait-frames
    1 char-here? not 1 0 char-in-area? or if
        $2C 0 43 hewie-to-room
        1 $2B 9.584 178.014 0 char-to-xz
    then
    0 0.0 0.0 0.0 0.0 event-camera
    0 0 char-visible
    0 camera-follow
    counter-inc
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room2C.act01 ( -- )   \ 0041D2D0
    1 self-scripted
    self-wait-done
    0 self-anim
    counter-inc
    2 wait-counter
    0 self-scripted
    self-idle-or-end
;

' room2C.char-enter $2C 6 room-script!
' room2C.phase1 $2C 1 room-script!
' room2C.enter $2C 0 room-script!
' room2C.act00 $2C $00 action-script!
' room2C.act01 $2C $01 action-script!

\ ---- room $2D ----------------------------------------------------------------------------------

\ a hanging thing (the room's +0x34 (byte 3 + 2) object) swinging, by byte 4: 0 / 2 set going
\ (12 degrees) away from the partner / Fiona; 1 a step (22.5 degrees of its swing, shrinking to
\ 0.4 at each end; under half a degree it stops) - 2 while it swings
: room2D.cmd00 ( b0 b1 -- )  drop drop stub-step ;
\ something dropped (effect LoopingSprite_vtable, its slot in event var 1) from (-276.5, 3,
\ 160), by byte 3: 0 started (event var 0 the frame count); 1 a frame (2 while falling): it
\ drifts 0.5 a frame in x and falls 0.05 x n(n+1)/2, gone below -10
: room2D.cmd01 ( b0 -- )  drop stub-step ;
\ room 0x2D: two hanging things (the room's objects 4 and 5) that Fiona pushes as she walks by:
\ past a step total of 5 they swing for 20 frames and the first toggles event flag 3 with a
\ sound (hangers_swing).
: room2D.cmd02 ( b0 -- )  drop stub-step ;
\ Hewie (in state 0x7F, +0xF3564) within 5 of the spot by byte 3: 0 (-259.5, 190), 1 (-276, 160)
\ (others: what the caller left)
: room2D.cond00? ( b0 -- flag )  drop stub-flag ;

: room2D.enter ( -- )   \ 00405AC0
    room-sounds
    $16 1.0 0 bgm
    0 1 object-show
    1 1 object-show
    2 1 $14 door-bits
    2 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    then
    $212 story-flag? not if
        1 2 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $284 story-flag? not if
            2 -14.0 -9.0 97.0 flicker-sprite
        then
    then
    0 5 0.562 0.5 0.25 0.5 zone-rect
    $23A story-flag? if
        $23B story-flag? not if
            0 -265.0 -9.0 160.0 flicker-sprite
        then
    then
    $23E story-flag? $23F story-flag? not and if
        1 -228.4 -9.0 103.2 flicker-sprite
    then
    0 room2D.cmd02
    $2F5 story-flag? not if
        3 -180.0 5.61 176.0 flicker-sprite
    then
;

: room2D.act09 ( -- )   \ 00406180
    $FE camera-follow
    1 ebit-clear
    $A 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $A 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $A 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $A 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    4 stalker-alert? if
        1 ebit-clear
    then
    1 ebit? if
        0 $FE $A action
    else
        $78 1 item-cooldown
    then
    $A pvar-inc
    exit
;

: room2D.char-enter ( -- )   \ 00405B80
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
    $FE self-is? if
        9 state-flag? if
            2 ebit-set
            room2D.act09
        else
            2 ebit-clear
        then
    then
;

: room2D.phase1 ( -- )   \ 00405BD0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 1 1 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    0 game-mode? if
        1 char-here? 1 2 char-C4? not and if
            0 char-unseen? not 1 char-unseen? not and if
                0 -130 115 $32 char-faces-xz? -130 -10 115 point-on-camera? not and if
                    1 3 char-in-area? if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 0 action
                            then
                        else 44 fiona-started? if
                            0 3 char-in-area? not if
                                hewie-stays? if
                                    0 1 0 action
                                then
                            then
                        then then
                    else 1 3 char-in-area? not if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 5 action
                            then
                        else 44 fiona-started? if
                            0 3 char-in-area? if
                                hewie-stays? if
                                    0 1 5 action
                                then
                            then
                        then then
                    then then
                then
                0 -180 180 $32 char-faces-xz? -180 5 180 point-on-camera? not and if
                    1 4 char-in-area? not if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 1 action
                            then
                        else 44 fiona-started? if
                            0 4 char-in-area? if
                                hewie-stays? if
                                    0 1 1 action
                                then
                            then
                        then then
                    else 1 4 char-in-area? if
                        35 fiona-started? if
                            hewie-stays? if
                                0 1 2 action
                            then
                        else 44 fiona-started? if
                            0 4 char-in-area? not if
                                hewie-stays? if
                                    0 1 2 action
                                then
                            then
                        then then
                    then then
                then
                0 -276 160 $1E char-faces-xz? -276 -10 160 point-on-camera? not and if
                    35 fiona-started? if
                        hewie-stays? if
                            0 1 3 action
                        then
                    then
                then
                0 -260 190 $1E char-faces-xz? -260 -10 190 point-on-camera? not and if
                    35 fiona-started? if
                        hewie-stays? if
                            0 1 4 action
                        then
                    then
                then
            then
        then
    then
    5 -276.0 -10.0 160.0 5 15 0 zone
    4 -259.5 -10.0 190.0 5 15 0 zone
    0 4 char-in-zone? if
        0 $F1 $D action
    then
    0 5 char-in-zone? if
        0 $F1 $E action
    then
    $23E story-flag? not if
        3 -228.4 -10.0 103.2 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 574 var-set
                $1A 575 var-set
                $1B 1 var-set
                $1C -228400 var-set
                $1D -9000 var-set
                $1E 103200 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
    2 -97.0 -4.0 82.0 $E 20 0 zone
    0 2 8 char-zone-bits? if
        1 room2D.cmd02
    then
    6 -275.96 -10.0 159.38 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    7 -259.2 -10.0 189.64 $14 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 7 9 char-zone-bits? if
                $1F 7 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
;

: room2D.phase2 ( -- )   \ 00405E30
    0 story-flag? if
        0 9 char-in-area? if
            $FE char-here? not if
                5 7 5 scene-change
            else
                $8016 scene-ending
            then
        then
    then
    $23A story-flag? if
        $23B story-flag? not if
            1 0 5 5 0 zone-at-effect
            0 1 3 char-zone-bits? if
                5 $10 4 scene-change
            then
        then
    then
    $23E story-flag? $23F story-flag? not and if
        3 1 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $11 4 scene-change
        then
    then
    0 $E char-in-area? 0 90 $3C char-heading? and if
        5 $12 0 scene-change
    then
    $212 story-flag? not if
        0 -14.0 -10.0 97.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $212 story-flag-set
            0 2 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -14.0 -10.0 97.0 0 -2143272896 0 0.0 scene-effect-8C
            3 6 -14.0 -10.0 97.0 0 0 sound
            $40 $10A noise
            2 -14.0 -9.0 97.0 flicker-sprite
        then
    else $284 story-flag? not if
        0 2 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $13 4 scene-change
        then
    then then
    $2F5 story-flag? not if
        8 3 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 $14 4 scene-change
        then
    then
;

: room2D.act00 ( -- )   \ 00405F30
    self-wait-done
    hewie-bark
    self-wait-done
    $14C -90.0 115.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 0 8 nav-group
    1 0 $30 nav-group
    $155 -130.0 115.0 1.5 35 hewie-go-to
    self-wait-done
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: room2D.act01 ( -- )   \ 00405F80
    self-wait-done
    hewie-bark
    self-wait-done
    0 1 8 nav-group
    $63 -130.0 180.0 -90 $FFFF $A self-move-to
    self-wait-done
    $137 $FFFF $B self-move-tri
    self-wait-done
    0 1 $30 nav-group
    self-idle-or-end
;

: room2D.act02 ( -- )   \ 00405FB0
    self-wait-done
    hewie-bark
    self-wait-done
    0 1 8 nav-group
    $39 -230.0 180.0 90 $FFFF $A self-move-to
    self-wait-done
    $1B9 $FFFF $B self-move-tri
    self-wait-done
    0 1 $30 nav-group
    self-idle-or-end
;

: room2D.act03 ( -- )   \ 00405FE0
    self-wait-done
    hewie-bark
    self-wait-done
    0 $F1 $B action
    $1C -236.0 160.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    $193 -276.0 160.0 1.0 35 hewie-go-to
    self-wait-done
    0 self-scripted
    self-idle-or-end
;

: room2D.act04 ( -- )   \ 00406010
    self-wait-done
    hewie-bark
    self-wait-done
    0 $F1 $C action
    $161 -220.0 190.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    $8A -259.5 190.1 1.0 35 hewie-go-to
    self-wait-done
    0 self-scripted
    self-idle-or-end
;

: room2D.act05 ( -- )   \ 00406040
    self-wait-done
    hewie-bark
    self-wait-done
    $4E -170.0 115.0 90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 0 8 nav-group
    1 0 $30 nav-group
    $155 -130.0 115.0 1.5 35 hewie-go-to
    self-wait-done
    0 0 $30 nav-group
    1 0 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: room2D.act06 ( -- )   \ 0047AB38
    self-idle-or-end
;

: room2D.act08 ( -- )   \ 00406150
    0 ebit? if
        2 avoid-prompt
    then
    9 state-flag-clear
    0 camera-follow
    $18 state-flag-set
    $C3 -67.3 79.93 89 $207 5 self-move-to
    self-wait-done
    4 $14 self-anim-blend
    self-wait-anim
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    self-idle-or-end
;

: room2D.act07 ( -- )   \ 00406090
    $18 state-flag-set
    2 ebit-clear
    1 self-scripted
    0 ebit-clear
    self-wait-done
    $E5 -74.756 82.505 -90 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    $800 self-anim
    self-wait-anim
    $D6 -100.488 79.931 -90 $803 5 self-move-to
    self-wait-done
    $D6 -100.488 79.931 90 $803 5 self-move-to
    self-wait-done
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room2D.act08 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? $FE char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            9 state-flag-clear
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] room2D.act08 goto
            then
            $18 state-flag-set
            $E5 -74.756 82.505 90 $803 5 self-move-to
            self-wait-done
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            self-idle-or-end
        then
    again
;

: room2D.act0A ( -- )   \ 004061D0
    self-wait-done
    $1E6 -45.859 82.656 -90 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room2D.act0B ( -- )   \ 00406200
    begin
        1 room2D.cond00? not while
        yield
    repeat
    1 $6C 5 char-sound
    $23A story-flag? not if
        0 $F2 $F action
    then
    1 0 room2D.cmd00
    1 1 room2D.cmd00
    self-idle-or-end
;

: room2D.act0C ( -- )   \ 00406230
    begin
        0 room2D.cond00? not while
        yield
    repeat
    1 $6C 5 char-sound
    0 0 room2D.cmd00
    0 1 room2D.cmd00
    self-idle-or-end
;

: room2D.act0D ( -- )   \ 00406250
    0 $8F 5 char-sound
    0 2 room2D.cmd00
    0 1 room2D.cmd00
    self-idle-or-end
;

: room2D.act0E ( -- )   \ 00406270
    0 $8F 5 char-sound
    1 2 room2D.cmd00
    1 1 room2D.cmd00
    self-idle-or-end
;

: room2D.act0F ( -- )   \ 00406290
    0 room2D.cmd01
    1 room2D.cmd01
    $23A story-flag-set
    $23B story-flag? not if
        0 -265.0 -9.0 160.0 flicker-sprite
    then
    self-idle-or-end
;

: room2D.act10 ( -- )   \ 004062B0
    self-wait-done
    -265.0 160.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $23B story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: room2D.act11 ( -- )   \ 00406310
    self-wait-done
    -228.4 103.2 self-turn-to-xz
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
            $23F story-flag-set
            1 effect-remove
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

: room2D.act12 ( -- )   \ 00406370
    self-wait-done
    $41 -190.101 72.001 180 $FFFF 5 self-move-to
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $1200 self-anim
    self-wait-anim
    0 0 6 char-sound
    $1203 self-anim
    self-frames-reset
    self-wait-16
    $1202 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 message
    wait-message
    self-idle-or-end
;

: room2D.act13 ( -- )   \ 004063B0
    self-wait-done
    -14.0 97.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message
            wait-message
        else
            $284 story-flag-set
            2 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
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

: room2D.act14 ( -- )   \ 00406410
    self-wait-done
    -180.0 176.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $90 message-param-room
        $90 $63 item-count? if
            $8010 message
            wait-message
        else
            $2F5 story-flag-set
            3 effect-remove
            $90 1 item-give-count
            0 $90 item-tab
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

' room2D.enter $2D 0 room-script!
' room2D.char-enter $2D 6 room-script!
' room2D.phase1 $2D 1 room-script!
' room2D.phase2 $2D 2 room-script!
' room2D.act00 $2D $00 action-script!
' room2D.act01 $2D $01 action-script!
' room2D.act02 $2D $02 action-script!
' room2D.act03 $2D $03 action-script!
' room2D.act04 $2D $04 action-script!
' room2D.act05 $2D $05 action-script!
' room2D.act06 $2D $06 action-script!
' room2D.act07 $2D $07 action-script!
' room2D.act08 $2D $08 action-script!
' room2D.act09 $2D $09 action-script!
' room2D.act0A $2D $0A action-script!
' room2D.act0B $2D $0B action-script!
' room2D.act0C $2D $0C action-script!
' room2D.act0D $2D $0D action-script!
' room2D.act0E $2D $0E action-script!
' room2D.act0F $2D $0F action-script!
' room2D.act10 $2D $10 action-script!
' room2D.act11 $2D $11 action-script!
' room2D.act12 $2D $12 action-script!
' room2D.act13 $2D $13 action-script!
' room2D.act14 $2D $14 action-script!

\ ---- room $2F ----------------------------------------------------------------------------------

\ room 0x2F (Room2F_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind
: room2F.cond00? ( b0 -- flag )  drop stub-flag ;

: room2F.enter ( -- )   \ 00412950
    room-sounds
    $16 1.0 0 bgm
    $260 story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
    1 $13 $10000000 nav-group-2
    1 $124 $10000000 nav-group-2
    1 $6B $10000000 nav-group-2
    1 $69 $10000000 nav-group-2
    1 $54 $10000000 nav-group-2
;

: room2F.char-enter ( -- )   \ 004129A0
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
        $E story-flag? not if
            0 $4D -45 char-to-tri-facing
            1 char-activate
            1 char-here? if
                1 $120 -45 char-to-tri-facing
            then
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            0 0 0 action
        then
    then
    1 self-is? if
        118 hewie-action? not if
            1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
                1 $13C -90 char-to-tri-facing
            then
        then
    then
;

: room2F.phase1 ( -- )   \ 00412A30
    0 exit-usable? if
        0 exit-check
    then
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        4 map-page
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and if
        1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
            44 fiona-started? if
                0 1 5 action
            else $FE 2 char-C4? not $FE 8 char-in-area? and if
                stalker-free? if
                    3 ebit? not if
                        0 0 8 nav-group
                        1 0 $30 nav-group
                        0 room2F.cond00? if
                            3 ebit-set
                            0 $F1 6 action
                        else
                            1 0 8 nav-group
                            0 0 $30 nav-group
                        then
                    then
                then
            then then
        else 2 game-mode? not if
            0 control-action? if
                -163 97 -50 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                    0 -163 -50 $32 char-faces-xz? if
                        hewie-stays? if
                            0 1 3 action
                        then
                    then
                then
            then
        then then
    then
    $260 story-flag? not 1 7 char-in-area? and if
        0 -149.92 60.0 -50.71 $36 36 0 zone
        1 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    0 -149.92 60.0 -50.71 $36 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: room2F.phase2 ( -- )   \ 00412B60
    $E story-flag? if
        0 9 char-in-area? if
            5 1 0 scene-change
        then
    then
    0 4 char-in-area? 0 90 $2D char-heading? and if
        5 2 0 scene-change
    then
    0 $A char-in-area? 0 45 $32 char-heading? and if
        5 7 0 scene-change
    then
;

: room2F.phase5 ( -- )   \ 00412B90
    1 char-here? if
        118 hewie-action? if
            1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
                1 $140 -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else 1 0 char-in-nav-group? 1 $2B char-on-tri? or 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
            1 $13C char-to-tri
        then then
    then
;

: room2F.act00 ( -- )   \ 00412BE0
    yield
    camera-restart
    8 state-flag-clear
    $F $41 fade
    $E story-flag-set
    \ (nop-progress-14: no effect in this game)
    wait-fade
    $227 item-give
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: room2F.act01 ( -- )   \ 00412C00
    $18 state-flag-set
    1 self-scripted
    \ (nop-progress-18: no effect in this game)
    self-wait-done
    2 message
    wait-message
    0 answer? if
        $F 4 fade
        wait-fade
        8 state-flag-set
        $12 state-flag-set
        0 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $B4 self-wait-frames
        1 char-here? if
            $2F 0 288 hewie-to-room
        then
        $82 exit-check
    else
        \ (nop-progress-14: no effect in this game)
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room2F.act02 ( -- )   \ 00412C50
    self-wait-done
    0 ebit? not if
        0 message
        wait-message
        0 ebit-set
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: room2F.act03 ( -- )   \ 00412C70
    self-wait-done
    1 0 char-file-load
    hewie-bark
    self-wait-done
    $AE -152.54 -106.55 0 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    1 self-noclip
    $8000 5 self-anim-9
    self-wait-anim
    0 0 $30 nav-group
    0 self-noclip
    $260 story-flag? not if
        $260 story-flag? not if
            $A state-flag? 0 char-busy? not or if
                $70 $63 item-count? not if
                    10 hewie-trust
                then
                $70 message-param-room
                $70 $63 item-count? if
                    $8010 message
                    wait-message
                else
                    $260 story-flag-set
                    $70 1 item-give-count
                    0 $70 item-tab
                    0 $F9 $8C action-force
                    $83 $85 0.0 0.0 0.0 0 0 sound
                    $8011 message
                    wait-message
                then
                $260 story-flag? if
                    1 effect-remove
                then
            then
        then
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 0 8 nav-group
        1 0 $30 nav-group
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        1 0 8 nav-group
        0 0 $30 nav-group
    else
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        $102 self-anim
        self-wait-anim
        2 self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: room2F.act04 ( -- )   \ 00412D60
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    3 ebit-clear
    self-idle-or-end
;

: room2F.act05 ( -- )   \ 00412DB0
    self-wait-done
    hewie-bark
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    self-idle-or-end
;

: room2F.act06 ( -- )   \ 00412E00
    $75 0 hewie-action
    begin
        117 hewie-action? while
        yield
    repeat
    1 $2B char-on-tri? 1 $140 char-on-tri? or 1 $1DF char-on-tri? or if
        0 1 4 action
    else
        1 0 8 nav-group
        0 0 $30 nav-group
        3 ebit-clear
        5 hewie-trust
    then
    self-idle-or-end
;

: room2F.act07 ( -- )   \ 00412E40
    self-wait-done
    90 self-turn-angle
    self-wait-done
    2 ebit? not if
        3 message
        wait-message
        2 ebit-set
    else
        4 message
        wait-message
        2 ebit-clear
    then
    self-idle-or-end
;

' room2F.enter $2F 0 room-script!
' room2F.char-enter $2F 6 room-script!
' room2F.phase1 $2F 1 room-script!
' room2F.phase2 $2F 2 room-script!
' room2F.phase5 $2F 5 room-script!
' room2F.act00 $2F $00 action-script!
' room2F.act01 $2F $01 action-script!
' room2F.act02 $2F $02 action-script!
' room2F.act03 $2F $03 action-script!
' room2F.act04 $2F $04 action-script!
' room2F.act05 $2F $05 action-script!
' room2F.act06 $2F $06 action-script!
' room2F.act07 $2F $07 action-script!

\ ---- room $30 ----------------------------------------------------------------------------------

\ room 0x30 (Room30_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind
: room30.cond00? ( b0 -- flag )  drop stub-flag ;

: room30.enter ( -- )   \ 00412EA0
    room-sounds
    $16 1.0 0 bgm
    $260 story-flag? not if
        1 -151.78 97.684 -50.579 flicker-sprite
    then
    $2FE story-flag? $2FF story-flag? not and if
        0 -105.0 135.0 23.0 flicker-sprite
    then
    1 $13 $10000000 nav-group-2
    1 $124 $10000000 nav-group-2
    1 $6B $10000000 nav-group-2
    1 $69 $10000000 nav-group-2
    1 $54 $10000000 nav-group-2
;

: room30.char-enter ( -- )   \ 00412F00
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
    0 self-is? if
        1 exit-taken? if
            4 map-page
        then
    then
    1 self-is? if
        118 hewie-action? not if
            1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
                1 $13C -90 char-to-tri-facing
            then
        then
    then
;

: room30.phase1 ( -- )   \ 00412FB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
            $FE 1 char-file-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 2 char-entered-area? if
        3 map-page
    then
    0 3 char-entered-area? if
        4 map-page
    then
    1 char-here? 1 2 char-C4? not and 1 char-busy? not and 0 hewie-side? and if
        1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
            44 fiona-started? if
                0 1 3 action
            else $FE 2 char-C4? not $FE 8 char-in-area? and if
                stalker-free? if
                    2 ebit? not if
                        0 0 8 nav-group
                        1 0 $30 nav-group
                        0 room30.cond00? if
                            2 ebit-set
                            0 $F1 4 action
                        else
                            1 0 8 nav-group
                            0 0 $30 nav-group
                        then
                    then
                then
            then then
        else 2 game-mode? not if
            0 control-action? if
                -163 97 -50 point-on-camera? not 0 char-unseen? not and 1 char-unseen? not and if
                    0 -163 -50 $32 char-faces-xz? if
                        hewie-stays? if
                            0 1 1 action
                        then
                    then
                then
            then
        then then
    then
    $2FE story-flag? not if
        1 -105.0 134.0 23.0 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 766 var-set
                $1A 767 var-set
                $1B 0 var-set
                $1C -105000 var-set
                $1D 135000 var-set
                $1E 23000 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    $260 story-flag? not 1 7 char-in-area? and if
        0 -149.92 60.0 -50.71 $36 36 0 zone
        0 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        0 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    0 -149.92 60.0 -50.71 $36 36 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: room30.phase2 ( -- )   \ 00413160
    $E story-flag? if
        0 9 char-in-area? if
            5 0 0 scene-change
        then
    then
    0 $A char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    $2FE story-flag? $2FF story-flag? not and if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: room30.phase5 ( -- )   \ 004131A0
    1 char-here? if
        118 hewie-action? if
            1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
                1 $140 -149.0 -51.0 -90 char-to-xz
                150 2 hewie-anim
            then
        else 1 0 char-in-nav-group? 1 $2B char-on-tri? or 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
            1 $13C char-to-tri
        then then
    then
;

: room30.act00 ( -- )   \ 004131F0
    $18 state-flag-set
    1 self-scripted
    \ (nop-progress-18: no effect in this game)
    self-wait-done
    2 message
    wait-message
    0 answer? if
        $F 4 fade
        wait-fade
        8 state-flag-set
        $12 state-flag-set
        0 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $B4 self-wait-frames
        1 char-here? if
            $30 0 288 hewie-to-room
        then
        $82 exit-check
    else
        \ (nop-progress-14: no effect in this game)
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: room30.act01 ( -- )   \ 00413240
    self-wait-done
    1 0 char-file-load
    hewie-bark
    self-wait-done
    $AE -152.54 -106.55 0 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    1 self-noclip
    $8000 5 self-anim-9
    self-wait-anim
    0 0 $30 nav-group
    0 self-noclip
    $260 story-flag? not if
        $260 story-flag? not if
            $A state-flag? 0 char-busy? not or if
                $70 $63 item-count? not if
                    10 hewie-trust
                then
                $70 message-param-room
                $70 $63 item-count? if
                    $8010 message
                    wait-message
                else
                    $260 story-flag-set
                    $70 1 item-give-count
                    0 $70 item-tab
                    0 $F9 $8C action-force
                    $83 $85 0.0 0.0 0.0 0 0 sound
                    $8011 message
                    wait-message
                then
                $260 story-flag? if
                    1 effect-remove
                then
            then
        then
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        0 0 8 nav-group
        1 0 $30 nav-group
        $139 -195.0 -51.0 0.5 100 hewie-go-to
        self-wait-done
        1 0 8 nav-group
        0 0 $30 nav-group
    else
        1 counter-set
        $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
        self-wait-done
        $102 self-anim
        self-wait-anim
        2 self-anim
        self-wait-anim
        5 2 hewie-anim
    then
    self-idle-or-end
;

: room30.act02 ( -- )   \ 00413330
    self-wait-done
    -105.0 23.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $98 message-param-room
        $98 $63 item-count? if
            $8010 message
            wait-message
        else
            $2FF story-flag-set
            0 effect-remove
            $98 1 item-give-count
            0 $98 item-tab
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

: room30.act03 ( -- )   \ 00413390
    self-wait-done
    hewie-bark
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    self-idle-or-end
;

: room30.act04 ( -- )   \ 004133E0
    $75 0 hewie-action
    begin
        117 hewie-action? while
        yield
    repeat
    1 $2B char-on-tri? 1 $F1 char-on-tri? or 1 $140 char-on-tri? or if
        0 1 6 action
    else
        1 0 8 nav-group
        0 0 $30 nav-group
        2 ebit-clear
        5 hewie-trust
    then
    self-idle-or-end
;

: room30.act05 ( -- )   \ 00413420
    self-wait-done
    90 self-turn-angle
    self-wait-done
    1 ebit? not if
        3 message
        wait-message
        1 ebit-set
    else
        4 message
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: room30.act06 ( -- )   \ 00413440
    self-wait-done
    $140 -149.0 -51.0 -90 $FFFF 5 self-move-to
    self-wait-done
    0 0 8 nav-group
    1 0 $30 nav-group
    $139 -195.0 -51.0 0.5 100 hewie-go-to
    self-wait-done
    1 0 8 nav-group
    0 0 $30 nav-group
    2 ebit-clear
    self-idle-or-end
;

' room30.enter $30 0 room-script!
' room30.char-enter $30 6 room-script!
' room30.phase1 $30 1 room-script!
' room30.phase2 $30 2 room-script!
' room30.phase5 $30 5 room-script!
' room30.act00 $30 $00 action-script!
' room30.act01 $30 $01 action-script!
' room30.act02 $30 $02 action-script!
' room30.act03 $30 $03 action-script!
' room30.act04 $30 $04 action-script!
' room30.act05 $30 $05 action-script!
' room30.act06 $30 $06 action-script!

\ ---- room $31 ----------------------------------------------------------------------------------

\ Room31_Cmd00
: room31.cmd00 ( -- )  stub-step ;
\ Room31_Cmd01
: room31.cmd01 ( -- )  stub-step ;
\ Room31_Cmd02
: room31.cmd02 ( b0 -- )  drop stub-step ;

: room31.enter ( -- )   \ 00429E40
    room31.cmd00
    0 1 $14 door-bits
    1 0 1 effect-string
    1 0 $14 door-bits
    0 $F1 2 action
;

: room31.char-enter ( -- )   \ 00429E60
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

: room31.phase1 ( -- )   \ 00429F10
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
;

: room31.act00 ( -- )   \ 00429F30
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
    \ (nop-progress-18: no effect in this game)
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

: room31.act01 ( -- )   \ 00429FC0
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
    2 room31.cmd02
    0 $1E char-layer
    begin
        $A cutscene-shot? not while
        yield
    repeat
    1 room31.cmd02
    0 $A char-layer
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: room31.act02 ( -- )   \ 0047AD60
    begin
        room31.cmd01
        yield
    again
;

: room31.act03 ( -- )   \ 0042A010
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

' room31.enter $31 0 room-script!
' room31.char-enter $31 6 room-script!
' room31.phase1 $31 1 room-script!
' room31.act00 $31 $00 action-script!
' room31.act01 $31 $01 action-script!
' room31.act02 $31 $02 action-script!
' room31.act03 $31 $03 action-script!

\ ---- room $32 ----------------------------------------------------------------------------------

\ room 0x32: room effect slot 1 made anew as the first TV screen (TvScreenA).
: room32.cmd00 ( -- )  stub-step ;
\ room 0x32 (D_0042C2A8): the fan turns, except while a movie plays
: room32.cmd01 ( -- )  stub-step ;
\ room 0x32 (D_0042C2B8)
: room32.cmd02 ( b0 -- )  drop stub-step ;
\ a room callback: byte 3 0 a progress name, 1 wait for character 3 (2 while not), else done
: room32.cmd03 ( b0 -- )  drop stub-step ;
\ room 0x32 (D_0042C2D8): byte 3 0..3 the lit quad (room effect 0x1A) as room 0x21's; 4 and up
\ the mirror fragment's reflection (room effect 0x1A, MirrorFragment_vtable) on the object
\ "a_fragment0": 1.8 across, -0.1 down, strength 1, kind 2, alpha 0xFF
: room32.cmd04 ( b0 -- )  drop stub-step ;
\ the player's model tint: white (byte 3 0) or blue halved
: room32.cmd05 ( b0 -- )  drop stub-step ;

: room32.enter ( -- )   \ 0042B0B0
    room-sounds
    room32.cmd00
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
        1 noise-level
    then
    0 $F1 $12 action
    $25E story-flag? $25F story-flag? not and if
        2 -9.0 1.0 -11.0 flicker-sprite
    then
    0 0 var-set
    1 0 var-set
    0 room32.cmd02
    0 room32.cmd04
    $42 story-flag? not if
        1 1 $14 door-bits
        2 0 $14 door-bits
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 0 $14 door-bits
        6 0 $14 door-bits
    else
        1 0 $14 door-bits
        2 0 $14 door-bits
        3 0 $14 door-bits
        4 0 $14 door-bits
        5 1 $14 door-bits
        6 1 $14 door-bits
    then
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
;

: room32.act04 ( -- )   \ 0042B850
    5 ebit-clear
    2 0 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 1 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 2 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else 2 3 pvar? if
        $32 chance? if
            5 ebit-set
        then
    then then then then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 $FE $B action
    else
        $78 1 item-cooldown
    then
    2 pvar-inc
    exit
;

: room32.char-enter ( -- )   \ 0042B160
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
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
    then
    1 self-is? if
    then
    $FE self-is? if
        9 state-flag? if
            $B ebit-set
            $300 story-flag? if
                room32.act04
            else
                room32.act04
            then
        else
            $B ebit-clear
        then
    then
;

: room32.phase1 ( -- )   \ 0042B200
    3 ebit? not if
        6 sound-bank-loaded? if
            $300 story-flag? if
                $40000001 6 0.0 3.0 7.0 0 0 sound
            then
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            3 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000001 6 0.0 3.0 7.0 0 0 sound
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
        0 $8000000C 6 char-sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    3 0 0 1 chars-area-camera
    8 1 -1 1 chars-area-camera
    9 2 -1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $300 story-flag? not if
        0 1 $14 door-bits
        1 0 1 effect-string
    else
        0 0 $14 door-bits
        1 1 1 effect-string
    then
    $41 story-flag? $42 story-flag? not and if
        1 2 pad? 2 2 pad? or 3 2 pad? or 0 control-action? or 1 control-action? or 2 control-action? or 3 control-action? or 4 control-action? or fiona-free? and if
            0 0 $19 action
        then
        99.0 fiona-fear
    then
    $25E story-flag? not if
        7 -9.0 0.0 -11.0 $A 5 0 zone
        1 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 606 var-set
                $1A 607 var-set
                $1B 2 var-set
                $1C -9000 var-set
                $1D 1000 var-set
                $1E -11000 var-set
                $1F 7 var-set
                0 1 $8B action
            then
        then
    then
    0 -1.41 -8.0 8.99 $D 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 -46.87 0.0 -38.5 $14 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    2 -39.23 0.0 -59.47 $14 6 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    3 -37.71 -8.0 63.24 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 46.35 0.0 -40.12 $F 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    5 39.18 0.0 -19.5 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    6 -0.22 -8.0 59.41 $10 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: room32.phase2 ( -- )   \ 0042B520
    0 4 char-in-area? 0 90 $5A char-heading? and if
        5 0 0 scene-change
    then
    0 5 char-in-area? 0 90 $3C char-heading? and if
        $41 story-flag? $42 story-flag? not and if
            5 $1A 0 scene-change
        else $FE char-here? not if
            5 1 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    0 $B char-in-area? 0 -67 $3C char-heading? and if
        5 $14 0 scene-change
    then
    0 $C char-in-area? 0 45 $3C char-heading? and if
        5 $15 0 scene-change
    then
    0 $11 char-in-area? 0 0 $3C char-heading? and if
        5 $17 0 scene-change
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        5 $18 0 scene-change
    then
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        5 $1C 0 scene-change
    then
    0 $13 $32 char-faces-area? if
        5 $1D 0 scene-change
    then
    0 $14 char-in-area? 0 -45 $32 char-heading? and if
        5 $1E 0 scene-change
    then
    0 $15 char-in-area? 0 90 $32 char-heading? and if
        $AE story-flag? not if
            5 $1F 0 scene-change
        else
            5 $20 0 scene-change
        then
    then
    0 $D char-in-area? 0 22 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 $16 char-in-area? 0 22 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 7 char-in-area? 0 -45 $32 char-heading? and if
        5 $84 3 scene-change
    then
    $41 story-flag? $42 story-flag? not and if
        -2147483646 scene-request? 2 scene-request? or if
            5 $19 1 scene-change
        then
    then
    $25E story-flag? $25F story-flag? not and if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            $41 story-flag? $42 story-flag? not and if
                5 $19 4 scene-change
            else
                5 $A 4 scene-change
            then
        then
    then
;

: room32.act00 ( -- )   \ 0042B640
    self-wait-done
    $6A -1.0 13.5 160 $FFFF 5 self-move-to
    self-wait-done
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $306 story-flag? not if
        $306 story-flag-set
        $29 message
        wait-message
    then
    $902 self-anim
    self-wait-anim
    5 0 pvar? 5 1 pvar? or if
        $300 story-flag? not if
            $300 story-flag-set
            0 0 6 char-sound
            $40000001 6 0.0 3.0 7.0 0 0 sound
            $40 $6A noise
            1 noise-level
        else
            $300 story-flag-clear
            0 2 6 char-sound
            1 6 sound-stop
            0 noise-level
        then
        $903 self-anim
        self-wait-anim
    else
        $903 self-anim
        self-wait-anim
        $28 message
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room32.act03 ( -- )   \ 0042B820
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: room32.act01 ( -- )   \ 0042B6E0
    $18 state-flag-set
    $B ebit-clear
    1 self-scripted
    0 counter-set
    6 ebit-clear
    7 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 50 chars-within? and if
        0 1 2 action-force
        6 ebit-set
    then
    0 8 char-file-load
    self-wait-done
    6 ebit? if
        $27 14.5 -37.5 180 $FFFF 5 self-move-to
        self-wait-done
        0 char-file-use
        1 self-look-at
        yield
        begin
            7 ebit? not while
            yield
        repeat
        counter-inc
        1 self-noclip
        $FF self-look-at
        yield
        $8000 5 self-anim-9
        self-frames-reset
        $1C self-wait-frames
        0 $7E 5 char-sound
        self-wait-anim
    else
        0 char-file-use
        $27 $8004 5 14.5 -37.5 180 self-walk-anim
        self-wait-done
        counter-inc
        1 self-noclip
        0 $7E 5 char-sound
        $8005 $A self-anim-9
        self-wait-anim
    then
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    4 ebit-clear
    0 avoid-prompt
    $FF 3 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] room32.act03 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                $B ebit? $FE char-here? not and if
                    $B ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            counter-inc
            $FE char-here? if
                ['] room32.act03 goto
            then
            0 camera-follow
            9 state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room32.act02 ( -- )   \ 0042B7D0
    1 self-scripted
    1 9 char-file-load
    self-wait-done
    $88 18.5 -37.5 180 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    7 ebit-set
    1 wait-counter
    1 self-noclip
    $8000 5 self-anim-blend
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                $8002 5 self-anim-9
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: room32.act05 ( -- )   \ 0047AD78
    self-wait-done
    -1 self-move-16
    self-wait-anim
    self-idle
    self-wait-done
    self-idle-or-end
;

: room32.act06 ( -- )   \ 0042B8A0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $6A -4.0 13.0 135 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    $FE 2 6 char-sound
    1 6 sound-stop
    0 noise-level
    $300 story-flag-clear
    5 pvar-inc
    $FE 3 stalker-mode
    0 self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: room32.act07 ( -- )   \ 0042B8F0
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $31 message
    wait-message
    self-idle-or-end
;

: room32.act08 ( -- )   \ 0042B900
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $42 story-flag? not if
        $31 message
        wait-message
    else
        $32 message
        wait-message
    then
    self-idle-or-end
;

: room32.act09 ( -- )   \ 0042B920
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    1 room32.cmd05
    begin
        2 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 room32.cmd05
    begin
        $A cutscene-shot? not while
        yield
    repeat
    2 room32.cmd04
    begin
        $A03 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 1 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    begin
        $B cutscene-shot? not while
        yield
    repeat
    0 room32.cmd04
    begin
        $A67 cutscene-cue-reached? not while
        yield
    repeat
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    begin
        $A70 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 1 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    begin
        $A89 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 1 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    begin
        $AB7 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    $E 0 object-show
    $F 0 object-show
    $10 0 object-show
    $11 0 object-show
    $12 0 object-show
    $13 0 object-show
    $14 0 object-show
    $15 0 object-show
    $16 0 object-show
    $17 0 object-show
    $18 0 object-show
    begin
        $AE7 cutscene-cue-reached? not while
        yield
    repeat
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    2 room32.cmd04
    begin
        $BF3 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 0 $14 door-bits
    0 room32.cmd04
    begin
        $CD2 cutscene-cue-reached? not while
        yield
    repeat
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    begin
        $10 cutscene-shot? not while
        4 room32.cmd04
        yield
    repeat
    0 room32.cmd04
    begin
        $11 cutscene-shot? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    1 room32.cmd03
    self-idle-or-end
;

: room32.act0A ( -- )   \ 0042BB10
    self-wait-done
    -9.0 -11.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $25F story-flag-set
            2 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
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

: room32.act0B ( -- )   \ 0042BB70
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE $A char-file-load
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        $FE char-file-use
        4 $FE 1 char-model-op
        $8000 self-anim
        self-wait-anim
        4 $FE 0 char-model-op
    else
        $1601 self-anim
        self-wait-anim
    then
    4 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: room32.act0C ( -- )   \ 0047AD80
    self-idle-or-end
;

: room32.act0D ( -- )   \ 0047AD84
    self-idle-or-end
;

: room32.act0E ( -- )   \ 0047AD88
    self-idle-or-end
;

: room32.act0F ( -- )   \ 0047AD8C
    self-idle-or-end
;

: room32.act10 ( -- )   \ 0047AD90
    self-idle-or-end
;

: room32.act11 ( -- )   \ 0047AD94
    self-idle-or-end
;

: room32.act12 ( -- )   \ 0047AD98
    begin
        room32.cmd01
        yield
    again
;

: room32.act13 ( -- )   \ 0047ADA0
    self-idle-or-end
;

: room32.act14 ( -- )   \ 0042BBD0
    self-wait-done
    -40.0 -60.0 self-turn-to-xz
    self-wait-done
    $23 message
    wait-message
    self-idle-or-end
;

: room32.act15 ( -- )   \ 0042BBE0
    self-wait-done
    $B5 43.571 -39.929 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $312 story-flag-set
    0 ebit? not if
        $24 message
        wait-message
        0 ebit-set
    else
        $25 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room32.act16 ( -- )   \ 0047ADA4
    self-idle-or-end
;

: room32.act17 ( -- )   \ 0042BC40
    self-wait-done
    $9F -37.0 63.0 0 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $314 story-flag-set
    1 ebit? not if
        $2D message
        wait-message
        1 ebit-set
    else $312 story-flag? $313 story-flag? or if
        $2E message
        wait-message
    else
        $2D message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room32.act18 ( -- )   \ 0042BCB0
    self-wait-done
    9 -40.418 -39.428 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    $313 story-flag-set
    2 ebit? not if
        $2B message
        wait-message
        2 ebit-set
    else $312 story-flag? $314 story-flag? and if
        $2C message
        wait-message
    else
        $2B message
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room32.act19 ( -- )   \ 0042BD20
    self-wait-done
    $100E self-anim
    0 $C 6 char-sound
    self-frames-reset
    $14 self-wait-frames
    8 ebit? not if
        $2F message
        8 ebit-set
    else
        $30 message
        8 ebit-clear
    then
    2 $A self-anim-blend
    wait-message
    self-wait-anim
    self-idle-or-end
;

: room32.act1A ( -- )   \ 0042BD50
    self-wait-done
    $21 message
    wait-message
    1 answer? if
        self-idle-or-end
    then
    $F $54 fade
    $D 0 movie-play
    $C cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 room32.cmd03
    wait-fade
    $300 story-flag-clear
    1 6 sound-stop
    0 noise-level
    1 action-end
    1 char-done
    $FE $32 212 2 stalker-to-room
    $FE 1 -1 char-camera
    $FE $80808080 1 char-tint
    $FE $1E char-layer
    0 $F9 9 action
    $FF panic-stage? if
        3 panic-stage
    then
    $1A message-prepare
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
    0 room32.cmd04
    $FE 0 0 char-tint
    $FE $A char-layer
    wait-fade
    8 state-flag-set
    0 counter-set
    2 room32.cmd03
    0 room32.cmd05
    1 char-activate
    $32 0 39 hewie-to-room
    0 1 $22 action
    $FE $D4 25.182 -33.041 -113 char-to-xz
    stalker-item-cooldown
    $42 story-flag-set
    fiona-calm-reset
    0 $21 4.683 -40.0 49 char-to-xz
    hewie-controlled? not if
        0 1 -1 char-camera
        0 camera-follow
    else
        1 1 -1 char-camera
        1 camera-follow
    then
    camera-restart
    1 door-lock
    3 door-lock
    $C door-lock
    $14 state-flag-clear
    $1E state-flag-clear
    1 wait-counter
    2 counter-set
    $1B state-flag-clear
    1 0 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 1 $14 door-bits
    6 1 $14 door-bits
    5 state-flag-clear
    $231 item-give
    $F $51 fade
    wait-fade
    stalker-item-cooldown
    self-idle-or-end
;

: room32.act1B ( -- )   \ 0047ADA8
    self-idle-or-end
;

: room32.act1C ( -- )   \ 0042BEA0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $E8 -25.0 63.0 -5 char-to-xz
    1 20.0 10.0 40.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 ebit? not if
        1 message
        wait-message
        9 ebit-set
    else
        2 message
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room32.act1D ( -- )   \ 0042BEF0
    self-wait-done
    5.0 40.5 self-turn-to-xz
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: room32.act1E ( -- )   \ 0042BF00
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $55 -40.0 12.0 -90 char-to-xz
    1 30.0 20.0 -60.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 message
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: room32.act1F ( -- )   \ 0042BF48
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $40 message
    wait-message
    self-idle-or-end
;

: room32.act20 ( -- )   \ 0042BF60
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message
    wait-message
    0 answer? if
        7 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            0 $10 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                3 state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                3 state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                3 state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                3 state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                3 state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                3 state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                3 state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            1 char-here? if
                1 2 char-C4? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: room32.act21 ( -- )   \ 0047ADAC
    self-idle-or-end
;

: room32.act22 ( -- )   \ 0042C0B0
    1 self-scripted
    1 self-noclip
    self-wait-done
    1 $25 13.928 -57.528 -90 char-to-xz
    1 9 char-file-load
    1 char-file-use
    $8001 self-anim
    self-wait-anim
    1 counter-set
    2 wait-counter
    1 $6A 5 char-sound
    $8002 5 self-anim-9
    self-wait-anim
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: room32.act23 ( -- )   \ 0042C0F0
    self-wait-done
    0 $F1 $12 action
    0 room32.cmd02
    0 room32.cmd04
    1 1 $14 door-bits
    2 0 $14 door-bits
    3 0 $14 door-bits
    4 0 $14 door-bits
    5 0 $14 door-bits
    6 0 $14 door-bits
    $E 1 object-show
    $F 1 object-show
    $10 1 object-show
    $11 1 object-show
    $12 1 object-show
    $13 1 object-show
    $14 1 object-show
    $15 1 object-show
    $16 1 object-show
    $17 1 object-show
    $18 1 object-show
    3 partner-load
    2 char-unload
    $D 0 movie-play
    $C cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 room32.cmd03
    $FE $32 212 2 stalker-to-room
    $FE $80808080 1 char-tint
    $FE $1E char-layer
    0 $F9 9 action
    $FF panic-stage? if
        3 panic-stage
    then
    $1A message-prepare
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
    0 room32.cmd04
    $FE 0 0 char-tint
    $FE $A char-layer
    2 room32.cmd03
    0 room32.cmd05
    2 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room32.enter $32 0 room-script!
' room32.char-enter $32 6 room-script!
' room32.phase1 $32 1 room-script!
' room32.phase2 $32 2 room-script!
' room32.act00 $32 $00 action-script!
' room32.act01 $32 $01 action-script!
' room32.act02 $32 $02 action-script!
' room32.act03 $32 $03 action-script!
' room32.act04 $32 $04 action-script!
' room32.act05 $32 $05 action-script!
' room32.act06 $32 $06 action-script!
' room32.act07 $32 $07 action-script!
' room32.act08 $32 $08 action-script!
' room32.act09 $32 $09 action-script!
' room32.act0A $32 $0A action-script!
' room32.act0B $32 $0B action-script!
' room32.act0C $32 $0C action-script!
' room32.act0D $32 $0D action-script!
' room32.act0E $32 $0E action-script!
' room32.act0F $32 $0F action-script!
' room32.act10 $32 $10 action-script!
' room32.act11 $32 $11 action-script!
' room32.act12 $32 $12 action-script!
' room32.act13 $32 $13 action-script!
' room32.act14 $32 $14 action-script!
' room32.act15 $32 $15 action-script!
' room32.act16 $32 $16 action-script!
' room32.act17 $32 $17 action-script!
' room32.act18 $32 $18 action-script!
' room32.act19 $32 $19 action-script!
' room32.act1A $32 $1A action-script!
' room32.act1B $32 $1B action-script!
' room32.act1C $32 $1C action-script!
' room32.act1D $32 $1D action-script!
' room32.act1E $32 $1E action-script!
' room32.act1F $32 $1F action-script!
' room32.act20 $32 $20 action-script!
' room32.act21 $32 $21 action-script!
' room32.act22 $32 $22 action-script!
' room32.act23 $32 $23 action-script!
