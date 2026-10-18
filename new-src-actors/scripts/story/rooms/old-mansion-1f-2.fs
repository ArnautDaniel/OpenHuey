\ story/rooms/old-mansion-1f-2.fs - the event scripts of room old-mansion-1f-2 ($40; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-2
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant medallion-placed            \ (bit 0) the medallion placed (act00): the door unlocked, its sparkle shown
1 constant scales-remarked             \ (bit 1) its first remark made: "A pair of scales hangs mysteriously from the statue's hand. "
2 constant inscription-read            \ (bit 2) the inscription read (act05: a file given)
3 constant hewie-away                  \ (bit 3) Hewie not here (or sent off) as the scene ends
4 constant sound-started               \ (bit 4) its sound started (then kept up)
0 constant used-item-var               \ (var 0) the item she used (8 .. 11)
1 constant sound-step-var              \ (var 1) which of its sounds next (0..3)
2 constant sound-frames-var            \ (var 2) frames to the next sound (30)
0 constant room-loop-sound             \ (snd 0) its steady sound (started as it comes in, then kept up)
5 constant ambient1-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the first
6 constant ambient2-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the second
7 constant ambient3-sound              \ (snd 7) its ambient sounds in turn (every 30 frames): the third
8 constant ambient4-sound              \ (snd 8) its ambient sounds in turn (every 30 frames): the fourth
10 constant ambient12-sound           \ (snd 10) its ambient sounds in turn (every 30 frames): the first
11 constant ambient22-sound           \ (snd 11) its ambient sounds in turn (every 30 frames): the second
12 constant ambient32-sound           \ (snd 12) its ambient sounds in turn (every 30 frames): the third
13 constant ambient42-sound           \ (snd 13) its ambient sounds in turn (every 30 frames): the fourth

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
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
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
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    player-who self-is? 4 exit-taken? and if
        player-who 4 char-to-exit
        3 3 player-camera
    then
    4 3 3 area-camera
    player-who self-is? 5 exit-taken? and if
        player-who 5 char-to-exit
        2 2 player-camera
    then
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
    player-who 5 char-in-area? player-who char-busy? not and if
        5 exit-check
    then
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
        sound-started ebit? not if
            6 sound-bank-loaded? if
                room-loop-sound volume-scaled 6 0.0 0.0 130.0 0 0 sound
                sound-started ebit-set
            then
        else
            room-loop-sound kept-going volume-scaled 6 0.0 0.0 130.0 0 0 sound
        then
    then
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 -109.95 50.0 -5.34 0 0 sound
                ambient12-sound volume-scaled 6 -42.0 20.0 117.0 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 -109.95 50.0 -5.34 0 0 sound
                ambient22-sound volume-scaled 6 -42.0 20.0 117.0 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 -109.95 50.0 -5.34 0 0 sound
                ambient32-sound volume-scaled 6 -42.0 20.0 117.0 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 -109.95 50.0 -5.34 0 0 sound
                ambient42-sound volume-scaled 6 -42.0 20.0 117.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
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
        hewie-away ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        hewie-away ebit-set
    then
    0 cutscene-track 2 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    0 self-idle-anim
    fiona-who $158 3.0 24.0 83 char-to-xz
    camera-restart
    hewie-away ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $40 1 229 hewie-to-room
            hewie-who $E5 26.55 54.18 -129 char-to-xz
        then
    then
    cutscene-track action-end
    4 1 object-hidden
    5 1 object-hidden
    6 1 object-hidden
    used-item-var 11 var? if
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
    medallion-placed ebit-set
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
    0 cutscene-track 3 action
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    cutscene-track action-end
    used-item-var 8 var? if
        8 item-use
    else used-item-var 9 var? if
        9 item-use
    else used-item-var 10 var? if
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
    reach-out-anim play-out
    $10 message-param-room
    $10 1 item-give-count
    0 $10 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    got-handful-of-ash story-flag-set
    reach-back-anim play-out
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-2.act02 ( -- )   \ 00406C30
    begin
        4 1 object-hidden
        5 1 object-hidden
        6 1 object-hidden
        used-item-var 11 var? if
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
        used-item-var 8 var? if
            4 0 object-hidden
        else used-item-var 9 var? if
            5 0 object-hidden
        else used-item-var 10 var? if
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
    medallion-placed ebit? not if
        4.0 17.0 self-turn-to-xz
        self-wait-done
        scales-remarked ebit? not if
            0 say   \ "A pair of scales hangs mysteriously from the statue's hand. One of the dishes i..."
            scales-remarked ebit-set
        else
            1 say   \ "There is a coin of sorts resting on one of the trays."
        then
    else
        2 say   \ "I may not be able to use this anymore, but who knows... Take the Godstone? YES NO"
        0 answer? if
            $158 4.619 23.5 180 $FFFF 5 self-move-to
            self-wait-done
            reach-out-anim play-out
            4 wait-frames
            medallion-placed ebit-clear
            1 effect-remove
            1 old-mansion-1f-2.cmd00
            used-item-var 11 var? if
                7 1 object-hidden
                $B message-param-room
                $B 1 item-give-count
                0 $B item-tab
                item-fanfare
                $8012 say   \ "You've obtained ."
            else
                8 1 object-hidden
                $C message-param-room
                $C 1 item-give-count
                0 $C item-tab
                item-fanfare
                $8012 say   \ "You've obtained ."
            then
            reach-back-anim play-out
        then
    then
    self-idle-or-end
;

: old-mansion-1f-2.act05 ( -- )   \ 00406D70
    self-wait-done
    inscription-read ebit? not if
        0 self-turn-angle
        self-wait-done
        look-at-anim play-out
        3 say   \ "A large door stands unnervingly still. Letters have been etched into the door."
        stalkers-stay state-flag-set
        1 self-scripted
        $F 6 fade
        wait-fade
        4 say   \ "Pursuer of truths, entrust the accumulation of knowledge to the judgment of the god..."
        inscription-read ebit-set
        $F 7 fade
        wait-fade
        $246 item-give
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        $1B9 -3.806 112.773 0 $FFFF 5 self-move-to
        self-wait-done
        lean-anim play-out
        strain-anim play-out
        16 wait-frames
        let-go-push-anim play-out
        -1 self-idle-anim
        7 wait-frames
        5 say   \ "Damn, it just won't open. This could be my way out. Open, damn you!"
        inscription-read ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-2.act06 ( -- )   \ 00406DD0
    self-wait-done
    -72.34 -34.25 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-2-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
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
    used-item-var 11 var-set
    0 cutscene-track 2 action
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
    used-item-var 9 var-set
    0 cutscene-track 3 action
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
