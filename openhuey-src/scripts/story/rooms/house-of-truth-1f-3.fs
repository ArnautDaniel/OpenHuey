\ story/rooms/house-of-truth-1f-3.fs - the event scripts of room house-of-truth-1f-3 ($8A; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-3
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant hewie-sent-off              \ (bit 0) Hewie was here and sent off for the scene

: house-of-truth-1f-3.enter ( -- )   \ 00433580
    got-golden-candlestick story-flag? not if
        0 part-shown
    then
    0 -42.5 45.4 -36.9 1 flame-in
    1 -72.4 17.2 -7.1 1 flame-in
    2 -42.5 45.4 87.3 1 flame-in
    3 -72.4 17.2 117.2 1 flame-in
    ht1f-3-rubrum-medallion story-flag? not if
        4 -23.41 1.0 -14.78 flicker-sprite
    then
    ht1f-3-viola-medallion-found story-flag? ht1f-3-viola-medallion story-flag? not and if
        5 3.0 1.0 168.0 flicker-sprite
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-3.char-enter ( -- )   \ 00433600
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        1 1 player-camera
    then
    2 1 1 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        2 2 player-camera
    then
    3 2 2 area-camera
;

: house-of-truth-1f-3.phase1 ( -- )   \ 00433700
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    ht1f-3-flag1 story-flag? not if
        fiona-who 9 char-left-area? if
            ht1f-3-flag1 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    fiona-who 4 3 char-zone-bits? fiona-who 4 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 4 3 char-zone-bits? hewie-who 4 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 4 3 char-zone-bits? stalker-who 4 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
    got-golden-candlestick story-flag? not if
        0 -61.25 0.0 -4.27 $11 15 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    hewie-zone-var 0 var-set
                    hewie-zone-var 15.0 hewie-look-zone
                then
            then
        then
    then
    ht1f-3-viola-medallion-found story-flag? not if
        2 3.0 0.0 168.0 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var ht1f-3-viola-medallion-found var-set
                taken-flag-var ht1f-3-viola-medallion var-set
                sparkle-fx-var 5 var-set
                sparkle-x-var 3000 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 168000 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: house-of-truth-1f-3.phase2 ( -- )   \ 004338F0
    got-golden-candlestick story-flag? not if
        fiona-who $A char-in-area? fiona-who 0 $32 char-heading? and if
            1 calm offer-action
        then
    then
    fiona-who $B char-in-area? fiona-who 45 $32 char-heading? and if
        2 unhurried offer-action
    then
    ht1f-3-rubrum-medallion story-flag? not if
        1 4 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            3 calm offer-action
        then
    then
    ht1f-3-viola-medallion-found story-flag? ht1f-3-viola-medallion story-flag? not and if
        2 5 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            4 calm offer-action
        then
    then
;

: house-of-truth-1f-3.act00 ( -- )   \ 00433950
    stalkers-stay state-flag-set
    stalker-who action-end
    stalker-who char-done
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    hewie-who char-here? if
        hewie-sent-off ebit-set
        hewie-who action-end
        hewie-who char-done
    then
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who $BC -12.31 49.96 0 char-to-xz
    hewie-sent-off ebit? if
        hewie-who char-activate
        hewie-who $1E4 -15.99 63.61 0 char-to-xz
        hewie-sent-off ebit-clear
    then
    stalker-who char-activate
    $FE $8A 480 2 stalker-to-room
    stalker-who $1E0 -11.74 29.25 0 char-to-xz
    stalker-who 0 0 char-camera
    summoner-on state-flag-set
    summoner-own-cooldown
    0 0 player-camera
    camera-restart
    $F $41 fade
    wait-fade
    $45 resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    ht1f-3-flag1 story-flag-set
    self-idle-or-end
;

: house-of-truth-1f-3.act01 ( -- )   \ 00433A50
    self-wait-done
    -62.5 -1.0 self-turn-to-xz
    self-wait-done
    reach-out-anim play-out
    0 part-hidden
    got-golden-candlestick story-flag-set
    items-40-as-70 story-flag? not if
        $24E item-add
    then
    $24 message-param-room
    $24 1 item-give-count
    0 $24 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    items-40-as-70 story-flag? not if
        $C $85 0.0 0.0 0.0 0 0 sound
    then
    self-idle-or-end
;

: house-of-truth-1f-3.act02 ( -- )   \ 00433AC0
    self-wait-done
    fiona-who $4F -33.0 -24.0 90 char-to-xz
    1 42.0 10.0 -160.0 0.0 event-camera
    0 say   \ "This is connected to another corridor but... these foreboding bars seem to keep out all..."
    16 wait-frames
    self-idle-or-end
;

: house-of-truth-1f-3.act03 ( -- )   \ 00433AF0
    self-wait-done
    -23.41 -14.78 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht1f-3-rubrum-medallion story-flag-set
            4 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-1f-3.act04 ( -- )   \ 00433B50
    self-wait-done
    3.0 168.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht1f-3-viola-medallion story-flag-set
            5 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-1f-3.act05 ( -- )   \ 00433BB0
    self-wait-done
    0 -42.5 45.4 -36.9 1 flame-in
    1 -72.4 17.2 -7.1 1 flame-in
    2 -42.5 45.4 87.3 1 flame-in
    3 -72.4 17.2 117.2 1 flame-in
    $B 3 $FF char-load
    3 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-3 register-scripts   \ (its phases and actions, by their names)
