\ story/rooms/old-mansion-1f-2.fs - the event scripts of room old-mansion-1f-2 ($40; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-2
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ the scales ("tenbin") and their pans ("sara_l", "sara_r"): level (byte 3 0) or tipped
: old-mansion-1f-2.cmd00 ( b0 -- )   \ (the scales, "tenbin", and their pans: level (0) or tipped 25 degrees)
    {: tipped :}
    s" sara_l" object-here? if
        0e s" sara_l" turn-x object!  0e s" sara_l" turn-y object!  0e s" sara_l" turn-z object!
        tipped if  6.3504e 20.6e  else  6.2004e 19.8e  then  17.1e  s" sara_l" place-to
    then
    s" sara_r" object-here? if
        0e s" sara_r" turn-x object!  0e s" sara_r" turn-y object!  0e s" sara_r" turn-z object!
        tipped if  3.594e 19.25e  else  3.049e 19.8e  then  17.1e  s" sara_r" place-to
    then
    s" tenbin" object-here? if
        0e s" tenbin" turn-x object!  0e s" tenbin" turn-y object!
        tipped if  25e deg>rad  else  0e  then  s" tenbin" turn-z object!
        4.6193e 20.6e 17.082e s" tenbin" place-to
    then ;

: old-mansion-1f-2.enter ( -- )   \ 00406550
    room-sounds
    1 0 var-set
    2 0 var-set
    1 old-mansion-1f-2.cmd00
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    medallion-40-done story-flag? not if
        0 barred-floor floor-on
    else
        0 part-shown
        9 1 object-hidden
        $A 1 object-hidden
    then
    om1f-2-camomile story-flag? not if
        0 -72.34 59.0 -34.25 flicker-sprite
    then
    2 -19.7 18.3 22.5 0 flame-in
    3 -19.7 18.3 25.0 0 flame-in
    4 -17.2 18.3 22.4 0 flame-in
    5 -17.2 18.3 25.0 0 flame-in
    6 17.2 18.3 22.4 0 flame-in
    7 17.2 18.3 25.0 0 flame-in
    8 19.7 18.3 22.5 0 flame-in
    9 19.7 18.3 25.0 0 flame-in
    1 $2300 sound-volume
    $1C 3 $FF char-load
;

: old-mansion-1f-2.char-enter ( -- )   \ 00406630
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
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
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 4 exit-taken? and if
            fiona-who 4 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 4 exit-taken? and if
        hewie-who 4 char-to-exit
        3 3 player-camera
    then then
    4 3 3 area-camera
    hewie-controlled? not if
        fiona-who self-is? 5 exit-taken? and if
            fiona-who 5 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 5 exit-taken? and if
        hewie-who 5 char-to-exit
        2 2 player-camera
    then then
    5 2 2 area-camera
    fiona-who self-is? if
        2 exit-taken? 4 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-2.phase1 ( -- )   \ 00406770
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    hewie-controlled? not if
        fiona-who 5 char-in-area? fiona-who char-busy? not and if
            5 exit-check
        then
    else hewie-who 5 char-in-area? hewie-who char-busy? not and if
        5 exit-check
    then then
    $B 0 0 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    $10 4 -1 1 chars-area-camera
    fiona-who 6 char-entered-area? if
        5 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        2 exit-prepare
    then
    fiona-who $A char-entered-area? if
        4 exit-prepare
    then
    0 0.56 0.0 14.62 $23 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 22.0 hewie-look-zone
            then
        then
    then
    medallion-40-done story-flag? if
        4 ebit? not if
            6 sound-bank-loaded? if
                $40000000 6 0.0 0.0 130.0 0 0 sound
                4 ebit-set
            then
        else
            $C0000000 6 0.0 0.0 130.0 0 0 sound
        then
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 -109.95 50.0 -5.34 0 0 sound
                $4000000A 6 -42.0 20.0 117.0 0 0 sound
            else 1 1 var? if
                $40000006 6 -109.95 50.0 -5.34 0 0 sound
                $4000000B 6 -42.0 20.0 117.0 0 0 sound
            else 1 2 var? if
                $40000007 6 -109.95 50.0 -5.34 0 0 sound
                $4000000C 6 -42.0 20.0 117.0 0 0 sound
            else 1 3 var? if
                $40000008 6 -109.95 50.0 -5.34 0 0 sound
                $4000000D 6 -42.0 20.0 117.0 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: old-mansion-1f-2.phase2 ( -- )   \ 00406930
    fiona-who $11 char-in-area? fiona-who 90 $32 char-heading? and if
        4 unhurried offer-action
    then
    medallion-40-done story-flag? not if
        fiona-who $12 char-in-area? fiona-who 0 $32 char-heading? and if
            5 unhurried offer-action
        then
    then
    fiona-who $13 char-in-area? fiona-who 0 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    fiona-who $14 char-in-area? fiona-who 0 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    om1f-2-camomile story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: old-mansion-1f-2.act00 ( -- )   \ 00406990
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    stalker-who action-end
    stalker-who char-done
    summoner-on state-flag-clear
    $F $54 fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? if
        3 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        3 ebit-set
    then
    0 $F9 2 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    0 self-idle-anim
    fiona-who $158 3.0 24.0 83 char-to-xz
    camera-restart
    3 ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $40 1 229 hewie-to-room
            hewie-who $E5 26.55 54.18 -129 char-to-xz
        then
    then
    $F9 action-end
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    0 11 var? if
        7 0 object-hidden
        8 1 object-hidden
        $B item-use
    else
        7 1 object-hidden
        8 0 object-hidden
        $C item-use
    then
    $B 0 object-hidden
    $C 0 object-hidden
    $D 0 object-hidden
    $E 0 object-hidden
    0 old-mansion-1f-2.cmd00
    0 barred-floor floor-off
    0 part-shown
    9 1 object-hidden
    $A 1 object-hidden
    0 ebit-set
    1 6.2 18.0 17.1 flicker-sprite
    medallion-40-done story-flag-set
    $5D door-unlock
    $F $51 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-2.act01 ( -- )   \ 00406AF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    0 $F9 3 action
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    $F9 action-end
    0 8 var? if
        8 item-use
    else 0 9 var? if
        9 item-use
    else 0 10 var? if
        $A item-use
    then then then
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    $B 0 object-hidden
    $C 0 object-hidden
    $D 0 object-hidden
    $E 0 object-hidden
    1 old-mansion-1f-2.cmd00
    0 self-idle-anim
    fiona-who $158 5.733 23.039 180 char-to-xz
    camera-restart
    hewie-who 0 char-visible
    hewie-who action-end
    $F $51 fade
    wait-fade
    $38 resident-flag-set
    reach-out-anim self-anim
    self-wait-anim
    $10 message-param-room
    $10 1 item-give-count
    0 $10 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    got-handful-of-ash story-flag-set
    reach-back-anim self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-2.act02 ( -- )   \ 00406C30
    begin
        4 1 object-hidden
        5 1 object-hidden
        6 1 object-hidden
        0 11 var? if
            7 0 object-hidden
            8 1 object-hidden
        else
            7 1 object-hidden
            8 0 object-hidden
        then
        yield
    again
;

: old-mansion-1f-2.act03 ( -- )   \ 00406C60
    begin
        $1CC cutscene-cue-reached? not while
        4 1 object-hidden
        5 1 object-hidden
        6 1 object-hidden
        7 1 object-hidden
        8 1 object-hidden
        0 8 var? if
            4 0 object-hidden
        else 0 9 var? if
            5 0 object-hidden
        else 0 10 var? if
            6 0 object-hidden
        then then then
        yield
    repeat
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    7 1 object-hidden
    8 1 object-hidden
    self-idle-or-end
;

: old-mansion-1f-2.act04 ( -- )   \ 00406CC0
    self-wait-done
    0 ebit? not if
        4.0 17.0 self-turn-to-xz
        self-wait-done
        1 ebit? not if
            0 message   \ "A pair of scales hangs mysteriously from the statue's hand. One of the dishes i..."
            wait-message
            1 ebit-set
        else
            1 message   \ "There is a coin of sorts resting on one of the trays."
            wait-message
        then
    else
        2 message   \ "I may not be able to use this anymore, but who knows... Take the Godstone? YES NO"
        wait-message
        0 answer? if
            $158 4.619 23.5 180 $FFFF 5 self-move-to
            self-wait-done
            reach-out-anim self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            0 ebit-clear
            1 effect-remove
            1 old-mansion-1f-2.cmd00
            0 11 var? if
                7 1 object-hidden
                $B message-param-room
                $B 1 item-give-count
                0 $B item-tab
                item-fanfare
                $8012 message   \ "You've obtained ."
                wait-message
            else
                8 1 object-hidden
                $C message-param-room
                $C 1 item-give-count
                0 $C item-tab
                item-fanfare
                $8012 message   \ "You've obtained ."
                wait-message
            then
            reach-back-anim self-anim
            self-wait-anim
        then
    then
    self-idle-or-end
;

: old-mansion-1f-2.act05 ( -- )   \ 00406D70
    self-wait-done
    2 ebit? not if
        0 self-turn-angle
        self-wait-done
        look-at-anim self-anim
        self-wait-anim
        3 message   \ "A large door stands unnervingly still. Letters have been etched into the door."
        wait-message
        stalkers-stay state-flag-set
        1 self-scripted
        $F 6 fade
        wait-fade
        4 message   \ "Pursuer of truths, entrust the accumulation of knowledge to the judgment of the god..."
        wait-message
        2 ebit-set
        $F 7 fade
        wait-fade
        $246 item-give
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        $1B9 -3.806 112.773 0 $FFFF 5 self-move-to
        self-wait-done
        lean-anim self-anim
        self-wait-anim
        strain-anim self-anim
        self-wait-anim
        self-frames-reset
        self-wait-16
        let-go-push-anim self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        5 message   \ "Damn, it just won't open. This could be my way out. Open, damn you!"
        wait-message
        2 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-2.act06 ( -- )   \ 00406DD0
    self-wait-done
    -72.34 -34.25 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-2-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-2.act07 ( -- )   \ 00406E30
    self-wait-done
    2 -19.7 18.3 22.5 0 flame-in
    3 -19.7 18.3 25.0 0 flame-in
    4 -17.2 18.3 22.4 0 flame-in
    5 -17.2 18.3 25.0 0 flame-in
    6 17.2 18.3 22.4 0 flame-in
    7 17.2 18.3 25.0 0 flame-in
    8 19.7 18.3 22.5 0 flame-in
    9 19.7 18.3 25.0 0 flame-in
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 11 var-set
    0 $F9 2 action
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

: old-mansion-1f-2.act08 ( -- )   \ 00406F40
    self-wait-done
    2 -19.7 18.3 22.5 0 flame-in
    3 -19.7 18.3 25.0 0 flame-in
    4 -17.2 18.3 22.4 0 flame-in
    5 -17.2 18.3 25.0 0 flame-in
    6 17.2 18.3 22.4 0 flame-in
    7 17.2 18.3 25.0 0 flame-in
    8 19.7 18.3 22.5 0 flame-in
    9 19.7 18.3 25.0 0 flame-in
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 9 var-set
    0 $F9 3 action
    $10 $FF movie-param
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

\ ---- registered ----
old-mansion-1f-2 register-scripts   \ (its phases and actions, by their names)
