\ story/rooms/front-garden-1.fs - the event scripts of room front-garden-1 ($0; Belli Castle: Front Garden).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.front-garden-1
USING: actors room-names story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ room 0x00: a grey glow (Effect737D0, size 30) at one of four spots picked by byte 3
\ (glow4_spot from spot 0): byte 4 0 starts it, its slot kept in event variable byte 3; else it
\ is removed.
: front-garden-1.cmd00 ( b0 b1 -- )  0 grey-glow ;   \ (a grey glow at spot b0, kept in variable b0: b1 0 lit, else out)
\ Fiona's model +0xD0 (0, 1.5, -2.5) and +0xCC(1) when byte 3 is 0, else (0, 1.5, -1.5) and
\ +0xCC(0)
: front-garden-1.cmd01 ( b0 -- )  drop ;   \ (the model hooks it calls are empty on PC and PS2 alike: CharModel_SetFlag / _VtCC / _VtD0)

: front-garden-1.enter ( -- )   \ 003ED800
    c2f-3-flag1 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $80C0F2AA 0 1 screen-blend
        3 part-shown
        4 part-shown
        5 part-shown
        6 part-shown
        7 part-shown
        8 part-shown
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
    got-broken-marionette story-flag? not if
        0 part-shown
    else
        0 part-hidden
    then
    fg-1-breakable-broken story-flag? not if
        1 keeps-people-off floor-on
        1 part-shown
        2 part-hidden
    else
        1 keeps-people-off floor-off
        1 part-hidden
        2 part-shown
    then
    0 4 0.0 0.687 0.187 0.312 zone-rect
    fg-1-medallion-found story-flag? fg-1-medallion story-flag? not and if
        0 -331.4 61.0 239.2 flicker-sprite
    then
    0 door-locked? not if
        cb1-6-flag3 story-flag? not if
            cb1-6-flag3 story-flag-set
            0 0 338 4 15 -1 0 0.0 creature-place
            0 0 320 4 15 -1 0 0.0 creature-place
        then
    then
    fg-1-medallion-2 story-flag? not if
        1 -366.0 72.0 26.0 flicker-sprite
    then
;

: front-garden-1.char-enter ( -- )   \ 003ED960
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
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
    fiona-who 1 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 2 char-entered-area? if
        1 exit-prepare
    then
    got-broken-marionette story-flag? not if
        hewie-who char-here? hewie-who 2 char-cond? not and if
            fiona-who $A char-in-area? hewie-who $B char-in-area? and if
                fiona-who -204 -117 $32 char-faces-xz? if
                    35 fiona-started? if
                        0 counter-set
                        0 $F1 $C action
                        0 hewie-who $B action-force
                    then
                then
            then
        then
    then
    fg-1-breakable-broken story-flag? not if
        0 -284.0 60.0 -97.0 5 8 1 zone
        self-who 0 char-in-zone? if
            fg-1-breakable-broken story-flag-set
            1 keeps-people-off floor-off
            1 part-hidden
            2 part-shown
            -284.0 60.0 -97.0 0 -2143272896 0 0.0 shards-at
            $88 5 -284.0 60.0 -97.0 0 0 sound
            $40 $185 noise
        then
    then
    fg-1-medallion-found story-flag? not if
        1 -331.4 60.0 239.2 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var fg-1-medallion-found var-set
                taken-flag-var fg-1-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var -331400 var-set
                sparkle-y-var 61000 var-set
                sparkle-z-var 239200 var-set
                hewie-zone-var 1 var-set
                0 hewie-who $8B action
            then
        then
    then
    c2f-3-flag1 story-flag? if
        fiona-who $C char-entered-area? if
            $1E chance? if
                0 $F8 8 action
            then
        then
        fiona-who $C char-left-area? if
            $1E chance? if
                0 $F8 8 action
            then
        then
        fiona-who $D char-entered-area? if
            $1E chance? if
                0 $F7 8 action
            then
        then
        fiona-who $D char-left-area? if
            $1E chance? if
                0 $F7 8 action
            then
        then
        fiona-who $E char-entered-area? if
            $1E chance? if
                0 $F6 8 action
            then
        then
        fiona-who $E char-left-area? if
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
    got-broken-marionette story-flag? not if
        hewie-who $F char-in-area? if
            2 -210.0 60.0 -114.0 $4B 20 0 zone
            0 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 2 9 char-zone-bits? if
                            0 ebit-set
                            $64 chance? if
                                hewie-zone-var 2 var-set
                                0 hewie-who $88 action
                            then
                        then
                    then
                then
            then
        then
    then
;

: front-garden-1.phase2 ( -- )   \ 003EDBC0
    fiona-who 5 char-in-area? fiona-who 0 $32 char-heading? and if
        found-file-222 story-flag? not if
            9 unhurried offer-action
        else
            4 unhurried offer-action
        then
    then
    fiona-who char-busy? not got-broken-marionette story-flag? not and if
        fiona-who 4 $2D char-faces-area? if
            found-file-202 story-flag? not if
                7 unhurried offer-action
            else found-file-223 story-flag? not if
                0 unhurried offer-action
            else
                5 unhurried offer-action
            then then
        then
    then
    fg-1-medallion-found story-flag? fg-1-medallion story-flag? not and if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $D calm offer-action
        then
    then
    fg-1-medallion-2 story-flag? not if
        3 1 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            $E calm offer-action
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
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    $E message   \ "It looks like there is something up there..."
    c2f-3-flag1 story-flag? not if
        4 1 movie-play
        2 cutscene-start
        yield
        yield
        scene-wait-ready
    else
        3 1 movie-play
        2 cutscene-start
        yield
        yield
        scene-wait-ready
    then
    wait-message
    $F $44 fade
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    $28 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    0 self-idle-anim
    fiona-who $1B0 -242.807 -79.976 -173 char-to-xz
    camera-restart
    hewie-who 0 char-visible
    hewie-who action-end
    found-file-223 story-flag-set
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
    scene-wait-ready
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    0 $F9 $10 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 front-garden-1.cmd01
    scene-locked state-flag-clear
    0 self-idle-anim
    fiona-who $148 -350.123 232.183 -11 char-to-xz
    camera-restart
    hewie-who 0 char-visible
    hewie-who action-end
    found-file-222 story-flag-set
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
    hands-clasped-anim $A self-anim-blend
    $C message   \ "The gate has been shut and locked solidly. It will be impossible to open it without a..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: front-garden-1.act05 ( -- )   \ 003EDEF0
    self-wait-done
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    $F message   \ "It's going to be too hard for me to get up there."
    wait-message
    self-idle-or-end
;

: front-garden-1.act06 ( -- )   \ 003EDF00
    1 self-scripted
    self-wait-done
    hewie-who self-look-at
    yield
    begin
        hewie-who $A char-in-area? not while
        yield
    repeat
    self-who 3 -1 char-camera
    self-who self-look-at
    yield
    -201.5 -121.8 self-turn-to-xz
    self-wait-done
    look-at-anim $A self-anim-blend
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    hewie-who self-look-at
    yield
    1 wait-counter
    self-who self-look-at
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
        3 part-hidden
        0 1 front-garden-1.cmd00
        yield
        3 part-shown
        0 0 front-garden-1.cmd00
        yield
        3 part-hidden
        0 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            3 part-shown
            0 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            3 part-hidden
            0 1 front-garden-1.cmd00
            yield
            3 part-shown
            0 0 front-garden-1.cmd00
            yield
            3 part-hidden
            0 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            3 part-shown
            0 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            3 part-hidden
            0 1 front-garden-1.cmd00
            yield
            3 part-shown
            0 0 front-garden-1.cmd00
            yield
            3 part-hidden
            0 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            3 part-shown
            0 0 front-garden-1.cmd00
            yield
            3 part-hidden
            0 1 front-garden-1.cmd00
            yield
            3 part-shown
            0 0 front-garden-1.cmd00
            yield
            3 part-hidden
            0 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        3 part-shown
        0 0 front-garden-1.cmd00
        yield
        3 part-hidden
        0 1 front-garden-1.cmd00
        yield
        3 part-shown
        0 0 front-garden-1.cmd00
    else $F7 self-is? if
        4 part-hidden
        1 1 front-garden-1.cmd00
        yield
        4 part-shown
        1 0 front-garden-1.cmd00
        yield
        4 part-hidden
        1 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            4 part-shown
            1 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            4 part-hidden
            1 1 front-garden-1.cmd00
            yield
            4 part-shown
            1 0 front-garden-1.cmd00
            yield
            4 part-hidden
            1 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            4 part-shown
            1 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            4 part-hidden
            1 1 front-garden-1.cmd00
            yield
            4 part-shown
            1 0 front-garden-1.cmd00
            yield
            4 part-hidden
            1 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            4 part-shown
            1 0 front-garden-1.cmd00
            yield
            4 part-hidden
            1 1 front-garden-1.cmd00
            yield
            4 part-shown
            1 0 front-garden-1.cmd00
            yield
            4 part-hidden
            1 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        4 part-shown
        1 0 front-garden-1.cmd00
        yield
        4 part-hidden
        1 1 front-garden-1.cmd00
        yield
        4 part-shown
        1 0 front-garden-1.cmd00
    else $F6 self-is? if
        5 part-hidden
        2 1 front-garden-1.cmd00
        yield
        5 part-shown
        2 0 front-garden-1.cmd00
        yield
        5 part-hidden
        2 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            5 part-shown
            2 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            5 part-hidden
            2 1 front-garden-1.cmd00
            yield
            5 part-shown
            2 0 front-garden-1.cmd00
            yield
            5 part-hidden
            2 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            5 part-shown
            2 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            5 part-hidden
            2 1 front-garden-1.cmd00
            yield
            5 part-shown
            2 0 front-garden-1.cmd00
            yield
            5 part-hidden
            2 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            5 part-shown
            2 0 front-garden-1.cmd00
            yield
            5 part-hidden
            2 1 front-garden-1.cmd00
            yield
            5 part-shown
            2 0 front-garden-1.cmd00
            yield
            5 part-hidden
            2 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        5 part-shown
        2 0 front-garden-1.cmd00
        yield
        5 part-hidden
        2 1 front-garden-1.cmd00
        yield
        5 part-shown
        2 0 front-garden-1.cmd00
    else $F5 self-is? if
        6 part-hidden
        3 1 front-garden-1.cmd00
        yield
        6 part-shown
        3 0 front-garden-1.cmd00
        yield
        6 part-hidden
        3 1 front-garden-1.cmd00
        self-frames-reset
        4 self-wait-frames
        $32 chance? if
            6 part-shown
            3 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            6 part-hidden
            3 1 front-garden-1.cmd00
            yield
            6 part-shown
            3 0 front-garden-1.cmd00
            yield
            6 part-hidden
            3 1 front-garden-1.cmd00
            self-frames-reset
            8 self-wait-frames
        then
        $19 chance? if
            6 part-shown
            3 0 front-garden-1.cmd00
            self-frames-reset
            2 self-wait-frames
            6 part-hidden
            3 1 front-garden-1.cmd00
            yield
            6 part-shown
            3 0 front-garden-1.cmd00
            yield
            6 part-hidden
            3 1 front-garden-1.cmd00
            self-frames-reset
            self-wait-16
        then
        5 chance? if
            6 part-shown
            3 0 front-garden-1.cmd00
            yield
            6 part-hidden
            3 1 front-garden-1.cmd00
            yield
            6 part-shown
            3 0 front-garden-1.cmd00
            yield
            6 part-hidden
            3 1 front-garden-1.cmd00
            self-frames-reset
            $20 self-wait-frames
        then
        6 part-shown
        3 0 front-garden-1.cmd00
        yield
        6 part-hidden
        3 1 front-garden-1.cmd00
        yield
        6 part-shown
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
    c2f-3-flag1 story-flag? not if
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
    0 solid-floor floor-off
    0 keeps-fiona-off floor-on
    0 keeps-pursuers-off floor-on
    $208 -206.0 -116.0 138 $FFFF $A self-move-to
    self-wait-done
    sit-a-while-anim self-anim
    self-wait-anim
    got-broken-marionette story-flag-set
    10 hewie-trust
    0 message-param-room
    0 1 item-give-count
    0 0 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    self-wait-done
    0 part-hidden
    40 hewie-trust
    fiona-who camera-follow
    $54 $FFFF $B self-move-tri
    self-wait-done
    0 solid-floor floor-on
    1 counter-set
    0 self-scripted
    self-idle-or-end
;

: front-garden-1.act0C ( -- )   \ 003EE340
    begin
        fiona-free? not while
        yield
    repeat
    panic-eased
    0 fiona-who 6 action-force
    self-idle-or-end
;

: front-garden-1.act0D ( -- )   \ 003EE360
    self-wait-done
    -331.4 239.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            fg-1-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: front-garden-1.act0E ( -- )   \ 003EE3C0
    self-wait-done
    -366.0 26.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            fg-1-medallion-2 story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
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
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
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
    3 part-shown
    4 part-shown
    5 part-shown
    6 part-shown
    7 part-shown
    8 part-shown
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
    0 part-shown
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    $28 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: front-garden-1.act12 ( -- )   \ 003EE580
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $80C0F2AA 0 1 screen-blend
    3 part-shown
    4 part-shown
    5 part-shown
    6 part-shown
    7 part-shown
    8 part-shown
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
    0 part-shown
    0 $F9 $10 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 front-garden-1.cmd01
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
front-garden-1 register-scripts   \ (its phases and actions, by their names)
