\ story/rooms/chaos-forest-8.fs - the event scripts of room chaos-forest-8 ($106; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-8
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ room 0x106: starts a water drip effect (OneDrip, 0xC0 bytes).
: chaos-forest-8.cmd00 ( -- )  effects:one-drip ;   \ (a drop falling now and then)

: chaos-forest-8.enter ( -- )   \ 004182D0
    found-file-247 story-flag? if
        room-sounds
        $19 1.0 0 bgm
    then
    $1F 0 pvar? if
        deal-things
    then
    in-panic? if
        1 ebit-set
    then
    cf-8-breakable1-broken story-flag? not if
        2 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        2 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        cf-8-medallion story-flag? not if
            0 -367.0 1.0 -118.7 flicker-sprite
        then
    then
    cf-8-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
    then
    cf-8-breakable3-broken story-flag? not if
        0 keeps-people-off floor-on
        4 part-shown
        5 part-hidden
    else
        0 keeps-people-off floor-off
        4 part-hidden
        5 part-shown
    then
    cf-8-breakable4-broken story-flag? not if
        3 keeps-people-off floor-on
        7 part-shown
        6 part-hidden
    else
        3 keeps-people-off floor-off
        7 part-hidden
        6 part-shown
    then
    cf-8-breakable5-broken story-flag? not if
        4 keeps-people-off floor-on
        9 part-shown
        8 part-hidden
    else
        4 keeps-people-off floor-off
        9 part-hidden
        8 part-shown
        cf-8-chicken-jerky story-flag? not if
            1 -275.5 1.0 -96.0 flicker-sprite
        then
    then
    cf-8-breakable6-broken story-flag? not if
        5 keeps-people-off floor-on
        $B part-shown
        $A part-hidden
    else
        5 keeps-people-off floor-off
        $B part-hidden
        $A part-shown
    then
    cf-8-breakable7-broken story-flag? not if
        6 keeps-people-off floor-on
        $D part-shown
        $C part-hidden
    else
        6 keeps-people-off floor-off
        $D part-hidden
        $C part-shown
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    chaos-forest-8.cmd00
;

: chaos-forest-8.char-enter ( -- )   \ 00418430
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
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
    fiona-who self-is? if
        found-file-247 story-flag? not if
            0 fiona-who 0 action
        then
    then
;

: chaos-forest-8.phase1 ( -- )   \ 004184C0
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        $1A 4 1 char-load
        $E 5 1 char-load
        found-file-247 story-flag? if
            3 0 char-remove
        then
        1 exit-prepare
    then
    fiona-who 3 char-left-area? if
        found-file-247 story-flag? if
            $1C 3 0 char-load
        then
        4 0 char-remove
        5 0 char-remove
    then
    cf-8-breakable2-broken story-flag? not if
        1 -376.0 0.0 -115.9 5 8 1 zone
        self-who 1 char-in-zone? if
            cf-8-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            -376.0 0.0 -115.9 0 -2143272896 0 0.0 shards-at
            $88 5 -376.0 0.0 -115.9 0 0 sound
            $40 $F0 noise
        then
    then
    cf-8-breakable3-broken story-flag? not if
        2 -381.0 0.0 -106.4 5 8 1 zone
        self-who 2 char-in-zone? if
            cf-8-breakable3-broken story-flag-set
            0 keeps-people-off floor-off
            4 part-hidden
            5 part-shown
            -381.0 0.0 -106.4 0 -2143272896 0 0.0 shards-at
            $88 5 -381.0 0.0 -106.4 0 0 sound
            $40 $E6 noise
        then
    then
    cf-8-breakable4-broken story-flag? not if
        3 -275.5 0.0 -105.0 5 8 1 zone
        self-who 3 char-in-zone? if
            cf-8-breakable4-broken story-flag-set
            3 keeps-people-off floor-off
            7 part-hidden
            6 part-shown
            -275.5 0.0 -105.0 0 -2143272896 0 0.0 shards-at
            $88 5 -275.5 0.0 -105.0 0 0 sound
            $40 $84 noise
        then
    then
    cf-8-breakable6-broken story-flag? not if
        5 -244.5 0.0 -105.0 5 8 1 zone
        self-who 5 char-in-zone? if
            cf-8-breakable6-broken story-flag-set
            5 keeps-people-off floor-off
            $B part-hidden
            $A part-shown
            -244.5 0.0 -105.0 0 -2143272896 0 0.0 shards-at
            $88 5 -244.5 0.0 -105.0 0 0 sound
            $40 $8E noise
        then
    then
    cf-8-breakable7-broken story-flag? not if
        6 -244.5 0.0 -96.0 5 8 1 zone
        self-who 6 char-in-zone? if
            cf-8-breakable7-broken story-flag-set
            6 keeps-people-off floor-off
            $D part-hidden
            $C part-shown
            -244.5 0.0 -96.0 0 -2143272896 0 0.0 shards-at
            $88 5 -244.5 0.0 -96.0 0 0 sound
            $40 $F7 noise
        then
    then
    0 ebit? not if
        1 ebit? not in-panic? and if
            0 ebit-set
            deal-things
        then
    then
    in-panic? if
        1 ebit-set
    else
        1 ebit-clear
    then
;

: chaos-forest-8.phase2 ( -- )   \ 00418730
    fiona-who 6 $32 char-faces-area? if
        sundial-examined story-flag? not if
            1 unhurried offer-action
        else
            $85 calm-hewie-up offer-action
        then
    then
    cf-8-breakable1-broken story-flag? not if
        0 -367.0 0.0 -118.7 5 8 1 zone
        self-who 0 char-in-zone? if
            cf-8-breakable1-broken story-flag-set
            2 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -367.0 0.0 -118.7 0 -2143272896 0 0.0 shards-at
            $88 5 -367.0 0.0 -118.7 0 0 sound
            $40 $E8 noise
            0 -367.0 1.0 -118.7 flicker-sprite
        then
    else cf-8-medallion story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            2 calm offer-action
        then
    then then
    cf-8-breakable5-broken story-flag? not if
        4 -275.5 0.0 -96.0 5 8 1 zone
        self-who 4 char-in-zone? if
            cf-8-breakable5-broken story-flag-set
            4 keeps-people-off floor-off
            9 part-hidden
            8 part-shown
            -275.5 0.0 -96.0 0 -2143272896 0 0.0 shards-at
            $88 5 -275.5 0.0 -96.0 0 0 sound
            $40 $FE noise
            1 -275.5 1.0 -96.0 flicker-sprite
        then
    else cf-8-chicken-jerky story-flag? not if
        4 1 5 5 0 zone-at-effect
        fiona-who 4 3 char-zone-bits? if
            3 calm offer-action
        then
    then then
;

: chaos-forest-8.act00 ( -- )   \ 00418860
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $34 door-lock
    stalker-who $10B 1 room-doors-state
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    3 char-start
    4 3 char-hand-over
    room-sounds
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    50 hewie-trust
    hewie-who action-end
    hewie-who char-done
    3 0 char-remove
    4 0 char-remove
    begin
        loader-done? while
        yield
    repeat
    0 self-idle-anim
    fiona-who $BD -78.171 -25.963 -73 char-to-xz
    camera-restart
    found-file-247 story-flag-set
    $19 1.0 0 bgm
    $F $41 fade
    wait-fade
    $247 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    plain-commands state-flag-set
    self-idle-or-end
;

: chaos-forest-8.act01 ( -- )   \ 00418930
    self-wait-done
    0 message   \ "This is a sundial. I think it's used to tell the time of day depending on the length an..."
    wait-message
    sundial-examined story-flag-set
    self-idle-or-end
;

: chaos-forest-8.act02 ( -- )   \ 00418940
    self-wait-done
    -367.0 -118.7 self-turn-to-xz
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
            cf-8-medallion story-flag-set
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

: chaos-forest-8.act03 ( -- )   \ 004189A0
    self-wait-done
    -275.5 -96.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cf-8-chicken-jerky story-flag-set
            1 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: chaos-forest-8.act04 ( -- )   \ 00418A00
    self-wait-done
    chaos-forest-8.cmd00
    $E 3 $FF char-load
    $F 4 figure-load
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    3 char-start
    4 3 char-hand-over
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
chaos-forest-8 register-scripts   \ (its phases and actions, by their names)
