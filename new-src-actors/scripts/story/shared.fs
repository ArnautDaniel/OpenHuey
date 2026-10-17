\ story/shared.fs - the shared event scripts (ids 0x80..) and the built-in ones.
\ Converted from the game's bytecode (new-src's tools/events2forth.py), renamed by
\ tools/story_convert.py, once: edit by hand.
IN: story.shared
USING: story.words flag-names story-names fiona.anims fiona.sounds hewie.anims hewie.sounds ;

\ ---- the shared scripts (ids 0x80..) -----------------------------------------------------------

defer shared.act96
defer shared.act97
: shared.act80 ( -- )   \ 003D6300
    self-wait-done
    stalker-who self-turn-to
    self-wait-done
    pick-up-anim self-anim
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
    up-from-pick-anim self-anim
    self-wait-anim
    wait-message
    self-idle-or-end
;

: shared.act82 ( -- )   \ 003D63A0
    fiona-half-hidden state-flag-clear
    fiona-who camera-follow
    self-frames-reset
    self-wait-16
    stalker-who self-look-at
    yield
    crouch-up-anim 5 self-anim-blend
    self-frames-reset
    8 self-wait-frames
    fiona-who seized-sound 5 char-sound
    uneasy-anim $A self-anim-blend
    self-wait-anim
    1 $7F $F rumble
    stalker-who self-turn-to
    self-wait-done
    self-idle-or-end
;

: shared.act81 ( -- )   \ 003D6340
    self-wait-done
    crouch-down-anim self-anim
    self-wait-anim
    fiona-half-hidden state-flag-set
    fiona-hewie-react
    crouching-anim self-anim
    begin   \ (crouched till she shakes - Fiona_Shakes - or L1 is let go, as the PS2 plays it)
        $FF $FF pad?  2 1 pad? not or  not if
            fiona-unseen? if
                stalker-who self-touching? stalker-who 2 char-cond? not and if
                    ['] shared.act82 goto
                else self-near-creature? in-panic? or if
                    ['] shared.act82 goto
                then then
            else 2 game-mode? stalker-who char-here? and fiona-who stalker-who 70 chars-within? and if
                ['] shared.act82 goto
            else in-panic? if
                ['] shared.act82 goto
            then then then
            1 panic-calm-in
            2 game-mode? if
                4 fiona-calm
            else
                5 fiona-calm
            then
            $2D fiona-recovery-lower
            yield
        else
            fiona-half-hidden state-flag-clear
            fiona-who camera-follow
            crouch-up-anim self-anim
            self-wait-anim
            self-idle-or-end
        then
    again
;

: shared.act83 ( -- )   \ 0047A918
    5 $41 fade
    wait-fade
    self-idle-or-end
;

: shared.act84 ( -- )   \ 003D63D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-reachable? not if
        $8024 message
    else
        0 counter-set
        hewie-who char-here? if
            0 hewie-who $94 action-force
        then
        $8014 message   \ "The clock etches out moments in time... Save current game? YES NO"
        wait-message
        0 answer? if
            1 subscreen-open
            begin
                subscreen-wanted state-flag? while
                yield
            repeat
            yield
        then
        counter-inc
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: shared.act85 ( -- )   \ 003D6410
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-reachable? not if
        $8024 message
    else
        0 counter-set
        hewie-who char-here? if
            0 hewie-who $94 action-force
        then
        $8017 message   \ "It's another clock... Save current game? YES NO"
        wait-message
        0 answer? if
            1 subscreen-open
            begin
                subscreen-wanted state-flag? while
                yield
            repeat
            yield
        then
        counter-inc
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: shared.act86 ( -- )   \ 003D6448
    begin
        hewie-who char-busy? not while
        yield
    repeat
    self-who 1 char-visible
    begin
        yield
    again
;

: shared.act87 ( -- )   \ 003D6458
    self-wait-done
    crouch-down-anim self-anim
    self-frames-reset
    5 self-wait-frames
    crouch-up-anim $A self-anim-blend
    self-wait-anim
    self-idle-or-end
;

: shared.act88 ( -- )   \ 003D6470
    self-wait-done
    $1F self-face-zone
    self-wait-done
    hewie-who 1 char-silent
    bark-anim $A self-anim-blend
    hewie-who 1 char-wait-motion
    hewie-who 0 char-silent
    hewie-who $5B 5 char-sound
    hewie-who 1 char-silent
    self-wait-anim
    10 self-idle-anim
    hewie-who 0 char-silent
    self-frames-reset
    $A self-wait-frames
    hewie-stay-put
    self-idle-or-end
;

: shared.act89 ( -- )   \ 003D64A0
    self-wait-done
    $1F self-face-zone
    self-wait-done
    sit-a-while-anim $A self-anim-blend
    self-wait-anim
    10 self-idle-anim
    self-frames-reset
    $A self-wait-frames
    hewie-stay-put
    self-idle-or-end
;

: shared.act8A ( -- )   \ 003D64C0
    self-wait-done
    $1F 0 hewie-go-to-zone
    self-wait-done
    stance-anim self-anim
    yield
    $1F 1 hewie-go-to-zone
    self-frames-reset
    $78 self-wait-frames
    self-who self-look-at
    yield
    10 self-idle-anim
    self-frames-reset
    $A self-wait-frames
    hewie-stay-put
    self-idle-or-end
;

: shared.act8B ( -- )   \ 003D64E0
    self-wait-done
    $1F self-face-zone
    self-wait-done
    hewie-who 1 char-silent
    bark-anim $A self-anim-blend
    hewie-who 1 char-wait-motion
    hewie-who 0 char-silent
    hewie-who $5B 5 char-sound
    hewie-who 1 char-silent
    self-wait-anim
    hewie-who 0 char-silent
    sit-down-anim self-anim
    self-wait-anim
    sit-anim self-anim
    yield
    fiona-who self-look-at
    yield
    $19 story-flag-set-var
    5 hewie-trust
    $1B $1C $1D $1E flicker-sprite-var
    $1F 0 var-set
    begin
        yield
        $1F var-inc
        $1A story-flag-var? not 2 game-mode? not and -1 control-action? and $1F 300 var? not and not until
    self-who self-look-at
    yield
    hewie-stay-put
    self-idle-or-end
;

: shared.act8C ( -- )   \ 003D6540
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

: shared.act8D ( -- )   \ 003D6560
    self-wait-done
    bent-hand-mouth-anim $A self-anim-blend
    fiona-who $23 5 char-sound
    fiona-who 2 char-wait-motion
    1 $60 8 rumble
    yield
    fiona-who 2 char-wait-motion
    1 $60 8 rumble
    self-wait-anim
    10 self-idle-anim
    self-frames-reset
    9 self-wait-frames
    self-idle-or-end
;

: shared.act8E ( -- )   \ 003D6590
    0 counter-set
    self-wait-done
    begin
        hewie-who char-busy? 0 counter? and while
        yield
    repeat
    hewie-who self-turn-to
    self-wait-done
    pick-up-anim self-anim
    self-wait-anim
    2 counter-set
    up-from-pick-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    hewie-who self-look-at
    yield
    begin
        hewie-who char-busy? 2 counter? and while
        yield
    repeat
    self-who self-look-at
    yield
    self-idle-or-end
;

: shared.act8F ( -- )   \ 003D65D0
    hewie-who 2 char-cond? if
        1 self-scripted
    then
    self-wait-done
    hewie-who 2 char-cond? if
        1 counter-set
        begin
            fiona-who char-busy? 1 counter? and while
            yield
        repeat
        hewie-who bark-sound 5 char-sound
        doubled-over-anim self-anim
        self-wait-anim
        0 self-scripted
        10 self-idle-anim
        self-frames-reset
        $A self-wait-frames
        hewie-who char-full-health? if
            1 hewie-mood
        then
        3 counter-set
    else
        1 counter-set
        fiona-who self-turn-to
        self-wait-done
        begin
            fiona-who char-busy? 1 counter? and while
            yield
        repeat
        $1C03 self-anim
        self-wait-anim
        fiona-who self-look-at
        yield
        hewie-who 1 char-cond? if
            $1B05 self-anim
            self-wait-anim
        else
            $1B00 self-anim
            self-wait-anim
        then
        hewie-who char-full-health? if
            1 hewie-mood
        then
        3 counter-set
        self-who self-look-at
        yield
    then
    self-idle-or-end
;

: shared.act90 ( -- )   \ 003D6640
    self-wait-done
    hewie-who 2 char-cond? if
        1 counter-set
        begin
            fiona-who char-busy? 1 counter? and while
            yield
        repeat
        hewie-who yelp-sound 5 char-sound
        $2213 0 hewie-anim-root
        self-wait-anim
        stagger-anim 5 self-anim-blend
        self-frames-reset
        5 self-wait-frames
        3 counter-set
    else
        1 counter-set
        fiona-who self-turn-to
        self-wait-done
        begin
            fiona-who char-busy? 0 counter? and while
            yield
        repeat
        $1C03 self-anim
        self-wait-anim
        fiona-who self-look-at
        yield
        hewie-who char-dead? hewie-who char-tri-free? and if
            $1001 self-anim
            self-wait-anim
            hewie-who 2 char-cond!
        else
            hewie-who shoved-yelp 5 char-sound
            $1000 self-anim
            self-wait-anim
        then
        3 counter-set
        self-who self-look-at
        yield
    then
    self-idle-or-end
;

: shared.act91 ( -- )   \ 003D66B0
    self-wait-done
    hewie-who 2 char-cond? if
        1 counter-set
        begin
            fiona-who char-busy? 1 counter? and while
            yield
        repeat
        hewie-who yelp-sound 5 char-sound
        $2213 0 hewie-anim-root
        self-wait-anim
        stagger-anim 5 self-anim-blend
        self-frames-reset
        5 self-wait-frames
        3 counter-set
    else
        1 counter-set
        fiona-who self-turn-to
        self-wait-done
        begin
            fiona-who char-busy? 0 counter? and while
            yield
        repeat
        $1C03 self-anim
        self-wait-anim
        fiona-who self-look-at
        yield
        hewie-who 1 char-cond? if
            $1B05 self-anim
            self-wait-anim
        else
            $1B00 self-anim
            self-wait-anim
        then
        3 counter-set
        self-who self-look-at
        yield
    then
    self-idle-or-end
;

: shared.act92 ( -- )   \ 003D6710
    self-wait-done
    $1F self-face-zone
    self-wait-done
    bark-tense-anim $A self-anim-blend
    self-wait-anim
    bark-tense-anim 0 self-anim-blend
    self-wait-anim
    bark-tense-anim 0 self-anim-blend
    self-wait-anim
    10 self-idle-anim
    self-frames-reset
    $A self-wait-frames
    hewie-stay-put
    self-idle-or-end
;

: shared.act93 ( -- )   \ 003D6730
    self-wait-done
    $1F self-face-zone
    self-wait-done
    sit-down-anim $A self-anim-blend
    self-wait-anim
    refuse-anim 0 self-anim-blend
    self-wait-anim
    sit-anim 0 self-anim-blend
    self-frames-reset
    $A self-wait-frames
    hewie-stay-put
    self-idle-or-end
;

: shared.act94 ( -- )   \ 0047A920
    self-wait-done
    1 wait-counter
    self-idle-or-end
;

: shared.act95 ( -- )   \ 003D6750
    ht1f-9-flag2 story-flag? if
        over-music-f story-flag? found-file-257 story-flag? not and if
            0 $F8 $97 action
        else
            0 $F8 $96 action
        then
    then
    exit
;

:noname   \ shared.act96 (003D6770; deferred: used before it is defined)
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
    ['] shared.act96 goto
; is shared.act96

:noname   \ shared.act97 (003D67A0; deferred: used before it is defined)
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
        0 fiona-action? 5 fiona-action? or fiona-who char-busy? not and $1B ebit? not and if
            self-who 1 fiona-thrown
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
    ['] shared.act97 goto
; is shared.act97

: shared.act98 ( -- )   \ 003D6820
    fading? not world-held state-flag? not and $F8 char-busy? not and if
        fiona-who char-busy? not fiona-half-hidden state-flag? or if
            shared-flag1 story-flag? not if
                $708 room-frames-reached? 6 sound-bank-loaded? and if
                    0 $F8 $99 action
                    shared-flag1 story-flag-set
                then
            else shared-flag2 story-flag? not if
                $1194 room-frames-reached? if
                    0 $F8 $9A action
                    shared-flag2 story-flag-set
                then
            else shared-flag3 story-flag? not if
                $1C20 room-frames-reached? if
                    0 $F8 $9B action
                    shared-flag3 story-flag-set
                then
            else shared-flag4 story-flag? not if
                $1D4C room-frames-reached? if
                    shared-flag4 story-flag-set
                then
            then then then then
        then
    then
    exit
;

: shared.act99 ( -- )   \ 003D6890
    scene-locked state-flag-set
    0 $86 0.0 0.0 0.0 0 0 sound
    5 message
    wait-message
    scene-locked state-flag-clear
    self-idle-or-end
;

: shared.act9A ( -- )   \ 003D68C0
    scene-locked state-flag-set
    1 $86 0.0 0.0 0.0 0 0 sound
    6 message
    wait-message
    scene-locked state-flag-clear
    self-idle-or-end
;

: shared.act9B ( -- )   \ 003D68F0
    scene-locked state-flag-set
    2 $86 0.0 0.0 0.0 0 0 sound
    7 message
    wait-message
    scene-locked state-flag-clear
    self-idle-or-end
;

: shared.act9C ( -- )   \ 003D6920
    items-40-as-70 story-flag? if
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

: shared.actB8 ( -- )   \ 003D69D8
    begin
        fade-finish
        yield
        fade-well-on? not until
    fade-over
    begin
        fade-finish
        yield
    again
;

: shared.actB9 ( -- )   \ 003D69E8
    world-held state-flag-clear
    begin
        fade-finish
        yield
        fade-well-on? not until
    fade-over
    self-idle-or-end
;

: shared.actBC ( -- )   \ 003D6A00
    begin
        fade-finish
        yield
        fade-well-on? not until
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

: shared.actBD ( -- )   \ 003D6A20
    world-held state-flag-clear
    self-frames-reset
    begin
        $F frames? not while
        fade-finish
        yield
    repeat
    begin
        fade-finish
        yield
        fade-well-on? not until
    fade-over
    self-idle-or-end
;

: shared.after-phase1 ( -- )   \ 003D6240
    hewie-controlled? not if
        no-flee state-flag? not if
            spirit-request-pending? not if
                $D fiona-action? not if
                    2 2 pad? fiona-free? and if
                        fading? not world-held state-flag? not and world-frozen state-flag? not and if
                            2 game-mode? tremor-warned-2 story-flag? not and if
                                fiona-who stalker-who 70 chars-within? not if
                                    fiona-who 1 char-on-nav-flags? not if
                                        0 fiona-who $81 action
                                    else
                                        0 fiona-who $87 action
                                    then
                                else
                                    stalker-who -1 fiona-target
                                then
                            else fiona-who 1 char-on-nav-flags? not if
                                0 fiona-who $81 action
                            else
                                0 fiona-who $87 action
                            then then
                        then
                    then
                then
            then
        then
    then
    fiona-half-hidden state-flag? no-stalker-camera state-flag? not and tremor-warned-2 story-flag? not and if
        stalker-who camera-follow
    then
    om1f-13-flag2 story-flag? white-mandragora-used story-flag? not and if
        $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
            $3C panic-level
            in-panic? not fiona-hidden state-flag? and if
                4 panic-stage
            then
        else
            stalker-who $48 0 room-doors-state
            stalker-who $66 1 room-doors-state
        then
    then
    items-40-as-70 story-flag? if
        fiona-who char-busy? not hewie-who char-busy? not and hewie-who 0 char-action? and if
            reward-item
        then
    then
;

: shared.after-phase2 ( -- )   \ 003D6230
    fiona-who stalker-who 2 0 $32 char-touching-facing? stalker-second-stance? and if
        $80 calm offer-action
    then
;

: shared.char-enter-26 ( -- )   \ 003D6C10
    fiona-who self-is? if
        hewie-who action-end
        hewie-who char-done
        $2A 0 pvar? if
            panic-eased
            0 fiona-who $27 action-force
        else $2A 1 pvar? if
            panic-eased
            0 fiona-who $28 action-force
        else $2A 2 pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A 3 pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A 4 pvar? if
            panic-eased
            0 fiona-who $C action-force
        else $2A 5 pvar? if
            panic-eased
            0 fiona-who $D action-force
        else $2A 6 pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A 7 pvar? if
            panic-eased
            0 fiona-who $C action-force
        else $2A 8 pvar? if
            panic-eased
            0 fiona-who $11 action-force
        else $2A 9 pvar? if
            panic-eased
            0 fiona-who $12 action-force
        else $2A $A pvar? if
            panic-eased
            0 fiona-who $14 action-force
        else $2A $B pvar? if
            panic-eased
            0 fiona-who $19 action-force
        else $2A $C pvar? if
            panic-eased
            0 fiona-who 3 action-force
        else $2A $D pvar? if
            panic-eased
            0 fiona-who $16 action-force
        else $2A $E pvar? if
            panic-eased
            0 fiona-who 7 action-force
        else $2A $F pvar? if
            panic-eased
            0 fiona-who $29 action-force
        else $2A $10 pvar? if
            panic-eased
            0 fiona-who $2A action-force
        else $2A $11 pvar? if
            panic-eased
            0 fiona-who $11 action-force
        else $2A $12 pvar? if
            panic-eased
            0 fiona-who $12 action-force
        else $2A $13 pvar? if
            panic-eased
            0 fiona-who $15 action-force
        else $2A $14 pvar? if
            panic-eased
            0 fiona-who $1A action-force
        else $2A $15 pvar? if
            panic-eased
            0 fiona-who 9 action-force
        else $2A $16 pvar? if
            panic-eased
            0 fiona-who $D action-force
        else $2A $17 pvar? if
            panic-eased
            0 fiona-who $C action-force
        else $2A $18 pvar? if
            panic-eased
            0 fiona-who $D action-force
        else $2A $19 pvar? if
            panic-eased
            0 fiona-who $E action-force
        else $2A $1A pvar? if
            panic-eased
            0 fiona-who $E action-force
        else $2A $1B pvar? if
            panic-eased
            0 fiona-who $F action-force
        else $2A $1C pvar? if
            panic-eased
            0 fiona-who 7 action-force
        else $2A $1D pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A $1E pvar? if
            panic-eased
            0 fiona-who $A action-force
        else $2A $1F pvar? if
            panic-eased
            0 fiona-who $17 action-force
        else $2A $20 pvar? if
            panic-eased
            0 fiona-who 9 action-force
        else $2A $21 pvar? if
            panic-eased
            0 fiona-who 1 action-force
        else $2A $22 pvar? if
            panic-eased
            0 fiona-who 9 action-force
        else $2A $23 pvar? if
            panic-eased
            0 fiona-who $A action-force
        else $2A $24 pvar? if
            $2C 0 pvar? if
                panic-eased
                0 fiona-who 9 action-force
            else
                panic-eased
                0 fiona-who $B action-force
            then
        else $2A $25 pvar? if
            panic-eased
            0 fiona-who $C action-force
        else $2A $26 pvar? if
            panic-eased
            0 fiona-who $D action-force
        else $2A $27 pvar? if
            panic-eased
            0 fiona-who $E action-force
        else $2A $28 pvar? if
            panic-eased
            0 fiona-who $F action-force
        else $2A $29 pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A $2B pvar? if
            panic-eased
            0 fiona-who 3 action-force
        else $2A $2C pvar? if
            panic-eased
            0 fiona-who 6 action-force
        else $2A $2D pvar? if
            panic-eased
            0 fiona-who $23 action-force
        else $2A $2A pvar? if
            panic-eased
            0 fiona-who 9 action-force
        else $2A $31 pvar? if
            panic-eased
            0 fiona-who 9 action-force
        else $2A $2E pvar? if
            panic-eased
            0 fiona-who 4 action-force
        else $2A $2F pvar? if
            panic-eased
            0 fiona-who 6 action-force
        else $2A $30 pvar? if
            panic-eased
            0 fiona-who 6 action-force
        else $2A $32 pvar? if
            panic-eased
            0 fiona-who $F action-force
        else $2A $33 pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A $34 pvar? if
            panic-eased
            0 fiona-who $14 action-force
        else $2A $35 pvar? if
            panic-eased
            0 fiona-who $15 action-force
        else $2A $36 pvar? if
            panic-eased
            0 fiona-who $16 action-force
        else $2A $37 pvar? if
            panic-eased
            0 fiona-who $17 action-force
        else $2A $38 pvar? if
            panic-eased
            0 fiona-who $13 action-force
        else $2A $39 pvar? if
            panic-eased
            0 fiona-who $14 action-force
        else $2A $3A pvar? if
            panic-eased
            0 fiona-who $15 action-force
        else $2A $3B pvar? if
            panic-eased
            0 fiona-who $16 action-force
        else $2A $45 pvar? if
            panic-eased
            0 fiona-who $A action-force
        else $2A $46 pvar? if
            panic-eased
            0 fiona-who $B action-force
        else $2A $3C pvar? if
            panic-eased
            0 fiona-who $20 action-force
        else $2A $3D pvar? if
            panic-eased
            0 fiona-who $E action-force
        else $2A $3E pvar? if
            panic-eased
            0 fiona-who $21 action-force
        else $2A $3F pvar? if
            panic-eased
            0 fiona-who $22 action-force
        else $2A $40 pvar? if
            panic-eased
            0 fiona-who $1B action-force
        else $2A $41 pvar? if
        else $2A $42 pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A $43 pvar? if
            panic-eased
            0 fiona-who $E action-force
        else $2A $44 pvar? if
            panic-eased
            0 fiona-who $F action-force
        else $2A $47 pvar? if
            panic-eased
            0 fiona-who $A action-force
        else $2A $48 pvar? if
            panic-eased
            0 fiona-who $B action-force
        else $2A $49 pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A $4A pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A $4B pvar? if
            panic-eased
            0 fiona-who $11 action-force
        else $2A $4C pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A $4D pvar? if
            panic-eased
            0 fiona-who 7 action-force
        else $2A $4E pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A $4F pvar? if
            panic-eased
            0 fiona-who 4 action-force
        else $2A $50 pvar? if
            panic-eased
            0 fiona-who 4 action-force
        else $2A $51 pvar? if
            panic-eased
            0 fiona-who 2 action-force
        else $2A $52 pvar? if
            panic-eased
            0 fiona-who $12 action-force
        else $2A $53 pvar? if
            panic-eased
            0 fiona-who $1A action-force
        else $2A $54 pvar? if
            panic-eased
            0 fiona-who $1B action-force
        else $2A $55 pvar? if
            panic-eased
            0 fiona-who $1C action-force
        else $2A $56 pvar? if
            panic-eased
            0 fiona-who 4 action-force
        else $2A $57 pvar? if
            panic-eased
            0 fiona-who $13 action-force
        else $2A $58 pvar? if
            panic-eased
            0 fiona-who $14 action-force
        else $2A $59 pvar? if
            panic-eased
            0 fiona-who $15 action-force
        else $2A $5A pvar? if
            panic-eased
            0 fiona-who $16 action-force
        else $2A $5B pvar? if
            panic-eased
            0 fiona-who $C action-force
        else $2A $5C pvar? if
            panic-eased
            0 fiona-who 8 action-force
        else $2A $5D pvar? if
            panic-eased
            0 fiona-who 5 action-force
        else $2A $5E pvar? if
            panic-eased
            0 fiona-who $B action-force
        else $2A $5F pvar? if
            panic-eased
            0 fiona-who $C action-force
        else $2A $60 pvar? if
            panic-eased
            0 fiona-who $D action-force
        else $2A $61 pvar? if
            panic-eased
            0 fiona-who $B action-force
        else $2A $62 pvar? if
            panic-eased
            0 fiona-who 4 action-force
        else $2A $63 pvar? if
            panic-eased
            0 fiona-who 1 action-force
        else $2A $64 pvar? if
            panic-eased
            0 fiona-who $F action-force
        else $2A $65 pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A $66 pvar? if
            panic-eased
            0 fiona-who $11 action-force
        else $2A $67 pvar? if
            panic-eased
            0 fiona-who $12 action-force
        else $2A $68 pvar? if
            panic-eased
            0 fiona-who $10 action-force
        else $2A $69 pvar? if
            panic-eased
            0 fiona-who 7 action-force
        else $2A $6A pvar? if
            $2C 0 pvar? if
                panic-eased
                0 fiona-who 4 action-force
            else
                panic-eased
                0 fiona-who 1 action-force
            then
        else $2A $6B pvar? if
            panic-eased
            0 fiona-who 5 action-force
        else $2A $6C pvar? if
            $2C 0 pvar? if
                panic-eased
                0 fiona-who 6 action-force
            else
                panic-eased
                0 fiona-who 5 action-force
            then
        else $2A $6D pvar? if
            panic-eased
            0 fiona-who 2 action-force
        then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then
    then
;

register-shared   \ (shared.act80 .., by their names; the four below share a script)
' shared.actB8 $BA builtin-script!
' shared.actB9 $BB builtin-script!
' shared.actB8 $BE builtin-script!
' shared.actB9 $BF builtin-script!
