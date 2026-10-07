\ events/builtin.fs - the shared event scripts (ids 0x80..) and the built-in ones.
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.builtin
USING: events.words ;

\ ---- the shared scripts (ids 0x80..) -----------------------------------------------------------

defer builtin.act96
defer builtin.act97
: builtin.act80 ( -- )   \ 003D6300
    self-wait-done
    $FE self-turn-to
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 counter-set
    stalker-gift
    0 counter? not if
        $83 $85 0.0 0.0 0.0 0 0 sound
        0 $F9 $8C action-force
    then
    self-frames-reset
    8 self-wait-frames
    $901 self-anim
    self-wait-anim
    wait-message
    self-idle-or-end
;

: builtin.act82 ( -- )   \ 003D63A0
    $A state-flag-clear
    0 camera-follow
    self-frames-reset
    self-wait-16
    $FE self-look-at
    yield
    $802 5 self-anim-blend
    self-frames-reset
    8 self-wait-frames
    0 $43 5 char-sound
    $F03 $A self-anim-blend
    self-wait-anim
    1 $7F $F rumble
    $FE self-turn-to
    self-wait-done
    self-idle-or-end
;

: builtin.act81 ( -- )   \ 003D6340
    self-wait-done
    $800 self-anim
    self-wait-anim
    $A state-flag-set
    fiona-hewie-react
    $801 self-anim
    begin
        $FF $FF pad? not if
            fiona-caught? if
                $FE self-touching? $FE 2 char-C4? not and if
                    ['] builtin.act82 goto
                else self-near-creature? $FF panic-stage? or if
                    ['] builtin.act82 goto
                then then
            else 2 game-mode? $FE char-here? and 0 $FE 70 chars-within? and if
                ['] builtin.act82 goto
            else $FF panic-stage? if
                ['] builtin.act82 goto
            then then then
            1 panic-grow
            2 game-mode? if
                4 fiona-calm
            else
                5 fiona-calm
            then
            $2D fiona-recovery-lower
            yield
        else
            $A state-flag-clear
            0 camera-follow
            $802 self-anim
            self-wait-anim
            self-idle-or-end
        then
    again
;

: builtin.act83 ( -- )   \ 0047A918
    5 $41 fade
    wait-fade
    self-idle-or-end
;

: builtin.act84 ( -- )   \ 003D63D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    hewie-reachable? not if
        $8024 message
    else
        0 counter-set
        1 char-here? if
            0 1 $94 action-force
        then
        $8014 message
        wait-message
        0 answer? if
            1 subscreen-open
            begin
                4 state-flag? while
                yield
            repeat
            yield
        then
        counter-inc
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: builtin.act85 ( -- )   \ 003D6410
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    hewie-reachable? not if
        $8024 message
    else
        0 counter-set
        1 char-here? if
            0 1 $94 action-force
        then
        $8017 message
        wait-message
        0 answer? if
            1 subscreen-open
            begin
                4 state-flag? while
                yield
            repeat
            yield
        then
        counter-inc
    then
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: builtin.act86 ( -- )   \ 003D6448
    begin
        1 char-busy? not while
        yield
    repeat
    $FF 1 char-visible
    begin
        yield
    again
;

: builtin.act87 ( -- )   \ 003D6458
    self-wait-done
    $800 self-anim
    self-frames-reset
    5 self-wait-frames
    $802 $A self-anim-blend
    self-wait-anim
    self-idle-or-end
;

: builtin.act88 ( -- )   \ 003D6470
    self-wait-done
    $1F self-face-zone
    self-wait-done
    1 1 char-silent
    $1B00 $A self-anim-blend
    1 1 char-wait-motion
    1 0 char-silent
    1 $5B 5 char-sound
    1 1 char-silent
    self-wait-anim
    10 self-move-16
    1 0 char-silent
    self-frames-reset
    $A self-wait-frames
    hewie-stay-300
    self-idle-or-end
;

: builtin.act89 ( -- )   \ 003D64A0
    self-wait-done
    $1F self-face-zone
    self-wait-done
    $1C03 $A self-anim-blend
    self-wait-anim
    10 self-move-16
    self-frames-reset
    $A self-wait-frames
    hewie-stay-300
    self-idle-or-end
;

: builtin.act8A ( -- )   \ 003D64C0
    self-wait-done
    $1F 0 hewie-go-to-zone
    self-wait-done
    4 self-anim
    yield
    $1F 1 hewie-go-to-zone
    self-frames-reset
    $78 self-wait-frames
    $FF self-look-at
    yield
    10 self-move-16
    self-frames-reset
    $A self-wait-frames
    hewie-stay-300
    self-idle-or-end
;

: builtin.act8B ( -- )   \ 003D64E0
    self-wait-done
    $1F self-face-zone
    self-wait-done
    1 1 char-silent
    $1B00 $A self-anim-blend
    1 1 char-wait-motion
    1 0 char-silent
    1 $5B 5 char-sound
    1 1 char-silent
    self-wait-anim
    1 0 char-silent
    $100 self-anim
    self-wait-anim
    1 self-anim
    yield
    0 self-look-at
    yield
    $19 story-flag-set-var
    5 hewie-trust
    $1B $1C $1D $1E flicker-sprite-var
    $1F 0 var-set
    begin
        yield
        $1F var-inc
        $1A story-flag-var? not 2 game-mode? not and -1 control-action? and $1F 300 var? not and not until
    $FF self-look-at
    yield
    hewie-stay-300
    self-idle-or-end
;

: builtin.act8C ( -- )   \ 003D6540
    1 0 item-tab
    2 0 item-tab
    begin
        message-closed? not while
        3 0 item-tab
        yield
    repeat
    4 0 item-tab
    self-idle-or-end
;

: builtin.act8D ( -- )   \ 003D6560
    self-wait-done
    $1D00 $A self-anim-blend
    0 $23 5 char-sound
    0 2 char-wait-motion
    1 $60 8 rumble
    yield
    0 2 char-wait-motion
    1 $60 8 rumble
    self-wait-anim
    10 self-move-16
    self-frames-reset
    9 self-wait-frames
    self-idle-or-end
;

: builtin.act8E ( -- )   \ 003D6590
    0 counter-set
    self-wait-done
    begin
        1 char-busy? 0 counter? and while
        yield
    repeat
    1 self-turn-to
    self-wait-done
    $900 self-anim
    self-wait-anim
    2 counter-set
    $901 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 self-look-at
    yield
    begin
        1 char-busy? 2 counter? and while
        yield
    repeat
    $FF self-look-at
    yield
    self-idle-or-end
;

: builtin.act8F ( -- )   \ 003D65D0
    1 2 char-C4? if
        1 self-scripted
    then
    self-wait-done
    1 2 char-C4? if
        1 counter-set
        begin
            0 char-busy? 1 counter? and while
            yield
        repeat
        1 $5A 5 char-sound
        $1003 self-anim
        self-wait-anim
        0 self-scripted
        10 self-move-16
        self-frames-reset
        $A self-wait-frames
        1 char-full-health? if
            1 hewie-mode
        then
        3 counter-set
    else
        1 counter-set
        0 self-turn-to
        self-wait-done
        begin
            0 char-busy? 1 counter? and while
            yield
        repeat
        $1C03 self-anim
        self-wait-anim
        0 self-look-at
        yield
        1 1 char-C4? if
            $1B05 self-anim
            self-wait-anim
        else
            $1B00 self-anim
            self-wait-anim
        then
        1 char-full-health? if
            1 hewie-mode
        then
        3 counter-set
        $FF self-look-at
        yield
    then
    self-idle-or-end
;

: builtin.act90 ( -- )   \ 003D6640
    self-wait-done
    1 2 char-C4? if
        1 counter-set
        begin
            0 char-busy? 1 counter? and while
            yield
        repeat
        1 $66 5 char-sound
        $2213 0 hewie-anim-root
        self-wait-anim
        $1002 5 self-anim-blend
        self-frames-reset
        5 self-wait-frames
        3 counter-set
    else
        1 counter-set
        0 self-turn-to
        self-wait-done
        begin
            0 char-busy? 0 counter? and while
            yield
        repeat
        $1C03 self-anim
        self-wait-anim
        0 self-look-at
        yield
        1 char-dead? 1 char-tri-free? and if
            $1001 self-anim
            self-wait-anim
            1 2 char-set-C4
        else
            1 $65 5 char-sound
            $1000 self-anim
            self-wait-anim
        then
        3 counter-set
        $FF self-look-at
        yield
    then
    self-idle-or-end
;

: builtin.act91 ( -- )   \ 003D66B0
    self-wait-done
    1 2 char-C4? if
        1 counter-set
        begin
            0 char-busy? 1 counter? and while
            yield
        repeat
        1 $66 5 char-sound
        $2213 0 hewie-anim-root
        self-wait-anim
        $1002 5 self-anim-blend
        self-frames-reset
        5 self-wait-frames
        3 counter-set
    else
        1 counter-set
        0 self-turn-to
        self-wait-done
        begin
            0 char-busy? 0 counter? and while
            yield
        repeat
        $1C03 self-anim
        self-wait-anim
        0 self-look-at
        yield
        1 1 char-C4? if
            $1B05 self-anim
            self-wait-anim
        else
            $1B00 self-anim
            self-wait-anim
        then
        3 counter-set
        $FF self-look-at
        yield
    then
    self-idle-or-end
;

: builtin.act92 ( -- )   \ 003D6710
    self-wait-done
    $1F self-face-zone
    self-wait-done
    $1B03 $A self-anim-blend
    self-wait-anim
    $1B03 0 self-anim-blend
    self-wait-anim
    $1B03 0 self-anim-blend
    self-wait-anim
    10 self-move-16
    self-frames-reset
    $A self-wait-frames
    hewie-stay-300
    self-idle-or-end
;

: builtin.act93 ( -- )   \ 003D6730
    self-wait-done
    $1F self-face-zone
    self-wait-done
    $100 $A self-anim-blend
    self-wait-anim
    $1C01 0 self-anim-blend
    self-wait-anim
    1 0 self-anim-blend
    self-frames-reset
    $A self-wait-frames
    hewie-stay-300
    self-idle-or-end
;

: builtin.act94 ( -- )   \ 0047A920
    self-wait-done
    1 wait-counter
    self-idle-or-end
;

: builtin.act95 ( -- )   \ 003D6750
    $A3 story-flag? if
        $A4 story-flag? $96 story-flag? not and if
            0 $F8 $97 action
        else
            0 $F8 $96 action
        then
    then
    exit
;

:noname   \ builtin.act96 (003D6770; deferred: used before it is defined)
    self-frames-reset
    $3C self-wait-frames
    $19 chance? if
        self-frames-reset
        $1E self-wait-frames
    then
    $19 chance? if
        self-frames-reset
        $1E self-wait-frames
    then
    self-frames-reset
    begin
        $1E frames? not while
        0.1 camera-shake
        yield
    repeat
    ['] builtin.act96 goto
; is builtin.act96

:noname   \ builtin.act97 (003D67A0; deferred: used before it is defined)
    self-frames-reset
    begin
        $5A frames? not while
        0.1 camera-shake
        yield
    repeat
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    $1B ebit-clear
    0 0 $1E rumble
    1 $FF $1E rumble
    $31 $87 0.0 0.0 0.0 0 0 sound
    self-frames-reset
    begin
        $1E frames? not while
        0 fiona-action? 5 fiona-action? or 0 char-busy? not and $1B ebit? not and if
            $FF 1 fiona-thrown
            $1B ebit-set
        then
        1.0 camera-shake
        yield
    repeat
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    ['] builtin.act97 goto
; is builtin.act97

: builtin.act98 ( -- )   \ 003D6820
    fading? not 8 state-flag? not and $F8 char-busy? not and if
        0 char-busy? not $A state-flag? or if
            $351 story-flag? not if
                $708 event-704-reached? 6 sound-bank-loaded? and if
                    0 $F8 $99 action
                    $351 story-flag-set
                then
            else $352 story-flag? not if
                $1194 event-704-reached? if
                    0 $F8 $9A action
                    $352 story-flag-set
                then
            else $353 story-flag? not if
                $1C20 event-704-reached? if
                    0 $F8 $9B action
                    $353 story-flag-set
                then
            else $354 story-flag? not if
                $1D4C event-704-reached? if
                    $354 story-flag-set
                then
            then then then then
        then
    then
    exit
;

: builtin.act99 ( -- )   \ 003D6890
    $12 state-flag-set
    0 $86 0.0 0.0 0.0 0 0 sound
    5 message
    wait-message
    $12 state-flag-clear
    self-idle-or-end
;

: builtin.act9A ( -- )   \ 003D68C0
    $12 state-flag-set
    1 $86 0.0 0.0 0.0 0 0 sound
    6 message
    wait-message
    $12 state-flag-clear
    self-idle-or-end
;

: builtin.act9B ( -- )   \ 003D68F0
    $12 state-flag-set
    2 $86 0.0 0.0 0.0 0 0 sound
    7 message
    wait-message
    $12 state-flag-clear
    self-idle-or-end
;

: builtin.act9C ( -- )   \ 003D6920
    $AF story-flag? if
        1 hewie-trust-level? if
            $828F item-give
        else 2 hewie-trust-level? if
            $8290 item-give
        else 3 hewie-trust-level? if
            $8291 item-give
        else 4 hewie-trust-level? if
            $8292 item-give
        else 5 hewie-trust-level? if
            $8293 item-give
        else 6 hewie-trust-level? if
            $8294 item-give
        else 7 hewie-trust-level? if
            $8295 item-give
        then then then then then then then
    then
    exit
;

: builtin.actB8 ( -- )   \ 003D69D8
    begin
        fade-finish
        yield
        fade-past-40? not until
    fade-over
    begin
        fade-finish
        yield
    again
;

: builtin.actB9 ( -- )   \ 003D69E8
    8 state-flag-clear
    begin
        fade-finish
        yield
        fade-past-40? not until
    fade-over
    self-idle-or-end
;

: builtin.actBC ( -- )   \ 003D6A00
    begin
        fade-finish
        yield
        fade-past-40? not until
    self-frames-reset
    begin
        $F frames? not while
        fade-finish
        yield
    repeat
    fade-over
    begin
        fade-finish
        yield
    again
;

: builtin.actBD ( -- )   \ 003D6A20
    8 state-flag-clear
    self-frames-reset
    begin
        $F frames? not while
        fade-finish
        yield
    repeat
    begin
        fade-finish
        yield
        fade-past-40? not until
    fade-over
    self-idle-or-end
;

: builtin.after-phase1 ( -- )   \ 003D6240
    hewie-controlled? not if
        $14 state-flag? not if
            noise-slot-D? not if
                $D fiona-action? not if
                    2 2 pad? fiona-free? and if
                        fading? not 8 state-flag? not and $17 state-flag? not and if
                            2 game-mode? $34F story-flag? not and if
                                0 $FE 70 chars-within? not if
                                    0 1 char-on-nav-flags? not if
                                        0 0 $81 action
                                    else
                                        0 0 $87 action
                                    then
                                else
                                    $FE -1 fiona-target
                                then
                            else 0 1 char-on-nav-flags? not if
                                0 0 $81 action
                            else
                                0 0 $87 action
                            then then
                        then
                    then
                then
            then
        then
    then
    $A state-flag? $23 state-flag? not and $34F story-flag? not and if
        $FE camera-follow
    then
    $75 story-flag? $76 story-flag? not and if
        $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
            $3C panic-level
            $FF panic-stage? not 9 state-flag? and if
                4 panic-stage
            then
        else
            \ (nop-progress-24: no effect in this game)
            \ (nop-progress-24: no effect in this game)
        then
    then
    $AF story-flag? if
        0 char-busy? not 1 char-busy? not and 1 0 char-action? and if
            reward-item
        then
    then
;

: builtin.after-phase2 ( -- )   \ 003D6230
    0 $FE 2 0 $32 char-touching-facing? stalker-stance-2? and if
        5 $80 4 scene-change
    then
;

: builtin.char-enter-26 ( -- )   \ 003D6C10
    0 self-is? if
        1 action-end
        1 char-done
        $2A 0 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $27 action-force
        else $2A 1 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $28 action-force
        else $2A 2 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A 3 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A 4 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        else $2A 5 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $D action-force
        else $2A 6 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A 7 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        else $2A 8 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $11 action-force
        else $2A 9 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $12 action-force
        else $2A $A pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $14 action-force
        else $2A $B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $19 action-force
        else $2A $C pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 3 action-force
        else $2A $D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $16 action-force
        else $2A $E pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 7 action-force
        else $2A $F pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $29 action-force
        else $2A $10 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $2A action-force
        else $2A $11 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $11 action-force
        else $2A $12 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $12 action-force
        else $2A $13 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $15 action-force
        else $2A $14 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $1A action-force
        else $2A $15 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 9 action-force
        else $2A $16 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $D action-force
        else $2A $17 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        else $2A $18 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $D action-force
        else $2A $19 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $E action-force
        else $2A $1A pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $E action-force
        else $2A $1B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        else $2A $1C pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 7 action-force
        else $2A $1D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A $1E pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
        else $2A $1F pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $17 action-force
        else $2A $20 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 9 action-force
        else $2A $21 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 1 action-force
        else $2A $22 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 9 action-force
        else $2A $23 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
        else $2A $24 pvar? if
            $2C 0 pvar? if
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 9 action-force
            else
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 $B action-force
            then
        else $2A $25 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        else $2A $26 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $D action-force
        else $2A $27 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $E action-force
        else $2A $28 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        else $2A $29 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A $2B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 3 action-force
        else $2A $2C pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 6 action-force
        else $2A $2D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $23 action-force
        else $2A $2A pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 9 action-force
        else $2A $31 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 9 action-force
        else $2A $2E pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        else $2A $2F pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 6 action-force
        else $2A $30 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 6 action-force
        else $2A $32 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        else $2A $33 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A $34 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $14 action-force
        else $2A $35 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $15 action-force
        else $2A $36 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $16 action-force
        else $2A $37 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $17 action-force
        else $2A $38 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $13 action-force
        else $2A $39 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $14 action-force
        else $2A $3A pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $15 action-force
        else $2A $3B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $16 action-force
        else $2A $45 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
        else $2A $46 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $B action-force
        else $2A $3C pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $20 action-force
        else $2A $3D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $E action-force
        else $2A $3E pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $21 action-force
        else $2A $3F pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $22 action-force
        else $2A $40 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $1B action-force
        else $2A $41 pvar? if
        else $2A $42 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A $43 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $E action-force
        else $2A $44 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        else $2A $47 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
        else $2A $48 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $B action-force
        else $2A $49 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A $4A pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A $4B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $11 action-force
        else $2A $4C pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A $4D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 7 action-force
        else $2A $4E pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A $4F pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        else $2A $50 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        else $2A $51 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 2 action-force
        else $2A $52 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $12 action-force
        else $2A $53 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $1A action-force
        else $2A $54 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $1B action-force
        else $2A $55 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $1C action-force
        else $2A $56 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        else $2A $57 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $13 action-force
        else $2A $58 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $14 action-force
        else $2A $59 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $15 action-force
        else $2A $5A pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $16 action-force
        else $2A $5B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        else $2A $5C pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 8 action-force
        else $2A $5D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 5 action-force
        else $2A $5E pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $B action-force
        else $2A $5F pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        else $2A $60 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $D action-force
        else $2A $61 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $B action-force
        else $2A $62 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 4 action-force
        else $2A $63 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 1 action-force
        else $2A $64 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        else $2A $65 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A $66 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $11 action-force
        else $2A $67 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $12 action-force
        else $2A $68 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $10 action-force
        else $2A $69 pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 7 action-force
        else $2A $6A pvar? if
            $2C 0 pvar? if
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 4 action-force
            else
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 1 action-force
            then
        else $2A $6B pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 5 action-force
        else $2A $6C pvar? if
            $2C 0 pvar? if
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 6 action-force
            else
                $FF panic-stage? if
                    3 panic-stage
                then
                0 0 5 action-force
            then
        else $2A $6D pvar? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 2 action-force
        then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then
    then
;

' builtin.act80 $80 builtin-script!
' builtin.act81 $81 builtin-script!
' builtin.act82 $82 builtin-script!
' builtin.act83 $83 builtin-script!
' builtin.act84 $84 builtin-script!
' builtin.act85 $85 builtin-script!
' builtin.act86 $86 builtin-script!
' builtin.act87 $87 builtin-script!
' builtin.act88 $88 builtin-script!
' builtin.act89 $89 builtin-script!
' builtin.act8A $8A builtin-script!
' builtin.act8B $8B builtin-script!
' builtin.act8C $8C builtin-script!
' builtin.act8D $8D builtin-script!
' builtin.act8E $8E builtin-script!
' builtin.act8F $8F builtin-script!
' builtin.act90 $90 builtin-script!
' builtin.act91 $91 builtin-script!
' builtin.act92 $92 builtin-script!
' builtin.act93 $93 builtin-script!
' builtin.act94 $94 builtin-script!
' builtin.act95 $95 builtin-script!
' builtin.act96 $96 builtin-script!
' builtin.act97 $97 builtin-script!
' builtin.act98 $98 builtin-script!
' builtin.act99 $99 builtin-script!
' builtin.act9A $9A builtin-script!
' builtin.act9B $9B builtin-script!
' builtin.act9C $9C builtin-script!
' builtin.actB8 $B8 builtin-script!
' builtin.actB9 $B9 builtin-script!
' builtin.actB8 $BA builtin-script!
' builtin.actB9 $BB builtin-script!
' builtin.actBC $BC builtin-script!
' builtin.actBD $BD builtin-script!
' builtin.actB8 $BE builtin-script!
' builtin.actB9 $BF builtin-script!
