\ story/rooms/house-of-truth-2f-1.fs - the event scripts of room house-of-truth-2f-1 ($82; House of Truth: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-2f-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

: house-of-truth-2f-1.enter ( -- )   \ 004314B0
    ht2f-1-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    ht2f-1-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
        ht2f-1-viola-medallion story-flag? not if
            7 -17.0 71.0 -41.0 flicker-sprite
        then
    then
    ht2f-1-breakable3-broken story-flag? not if
        2 keeps-people-off floor-on
        4 part-shown
        5 part-hidden
    else
        2 keeps-people-off floor-off
        4 part-hidden
        5 part-shown
    then
    0 5 0.812 0.687 0.187 0.312 zone-rect
    0 -102.0 65.4 -31.4 4 flame-in
    1 -43.2 46.0 -31.2 1 flame-in
    2 -72.4 17.2 -1.75 1 flame-in
    3 -102.0 66.0 87.6 4 flame-in
    4 -43.0 46.0 87.7 1 flame-in
    5 -72.4 17.2 117.2 1 flame-in
    8 -101.8 66.0 -31.0 4 flame-in
    9 -101.8 66.0 88.0 4 flame-in
    ht2f-1-medallion-found story-flag? ht2f-1-medallion story-flag? not and if
        6 -50.0 71.0 -15.0 flicker-sprite
    then
;

: house-of-truth-2f-1.char-enter ( -- )   \ 004315E0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        6 6 player-camera
    then
    3 6 6 area-camera
    player-who self-is? 4 exit-taken? and if
        player-who 4 char-to-exit
        5 5 player-camera
    then
    4 5 5 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $C3 -9.0 95.0 180 char-to-xz
            1 1 player-camera
            $82 1 194 hewie-to-room
            hewie-who $C2 0 char-to-tri-facing
            0 fiona-who 3 action
        else
            room-sounds
        then
    then
;

: house-of-truth-2f-1.phase1 ( -- )   \ 00431750
    over-music-d story-flag? if
        0 exit-usable? if
            0 exit-check
        then
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    player-who 4 char-in-area? player-who char-busy? not and if
        4 exit-check
    then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 4 4 1 chars-area-camera
    $F 5 5 1 chars-area-camera
    $10 6 6 1 chars-area-camera
    $11 7 7 1 chars-area-camera
    $12 3 3 1 chars-area-camera
    $13 7 7 1 chars-area-camera
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        4 exit-prepare
    then
    over-music-d story-flag? not if
        fiona-who 0 char-entered-area? 2 ebit? not and if
            2 ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    ht2f-1-breakable1-broken story-flag? not if
        0 -57.4 70.0 114.5 5 8 1 zone
        self-who 0 char-in-zone? if
            ht2f-1-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -57.4 70.0 114.5 0 -2145378272 0 0.0 shards-at
            $88 5 -57.4 70.0 114.5 0 0 sound
            $40 $103 noise
        then
    then
    ht2f-1-breakable3-broken story-flag? not if
        9 -3.0 70.0 -41.0 5 8 1 zone
        self-who 9 char-in-zone? if
            ht2f-1-breakable3-broken story-flag-set
            2 keeps-people-off floor-off
            4 part-hidden
            5 part-shown
            -3.0 70.0 -41.0 0 -2145378272 0 0.0 shards-at
            $88 5 -3.0 70.0 -41.0 0 0 sound
            $40 $86 noise
            $82 1 147 $24 5 -1 0 0.0 creature-place
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
    5 4 8 -4 0 zone-at-effect
    fiona-who 5 3 char-zone-bits? fiona-who 5 3 char-zone-bits-before? not and if
        fiona-who 4 char-effect-moving
    then
    hewie-who 5 3 char-zone-bits? hewie-who 5 3 char-zone-bits-before? not and if
        hewie-who 4 char-effect-moving
    then
    stalker-who 5 3 char-zone-bits? stalker-who 5 3 char-zone-bits-before? not and if
        stalker-who 4 char-effect-moving
    then
    6 5 8 -4 0 zone-at-effect
    fiona-who 6 3 char-zone-bits? fiona-who 6 3 char-zone-bits-before? not and if
        fiona-who 5 char-effect-moving
    then
    hewie-who 6 3 char-zone-bits? hewie-who 6 3 char-zone-bits-before? not and if
        hewie-who 5 char-effect-moving
    then
    stalker-who 6 3 char-zone-bits? stalker-who 6 3 char-zone-bits-before? not and if
        stalker-who 5 char-effect-moving
    then
    ht2f-1-medallion-found story-flag? not if
        7 -50.0 70.0 -15.0 $A 5 0 zone
        hewie-who 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var ht2f-1-medallion-found var-set
                taken-flag-var ht2f-1-medallion var-set
                sparkle-fx-var 6 var-set
                sparkle-x-var -50000 var-set
                sparkle-y-var 71000 var-set
                sparkle-z-var -15000 var-set
                hewie-zone-var 7 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: house-of-truth-2f-1.phase2 ( -- )   \ 00431A60
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            1 always offer-action
        then
    then
    fiona-who $14 char-in-area? fiona-who 45 $32 char-heading? and if
        4 unhurried offer-action
    then
    ht2f-1-medallion-found story-flag? ht2f-1-medallion story-flag? not and if
        7 6 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    ht2f-1-breakable2-broken story-flag? not if
        8 -17.0 70.0 -41.0 5 8 1 zone
        self-who 8 char-in-zone? if
            ht2f-1-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            -17.0 70.0 -41.0 0 -2145378272 0 0.0 shards-at
            $88 5 -17.0 70.0 -41.0 0 0 sound
            $40 $95 noise
            7 -17.0 71.0 -41.0 flicker-sprite
        then
    else ht2f-1-viola-medallion story-flag? not if
        8 7 5 5 0 zone-at-effect
        fiona-who 8 3 char-zone-bits? if
            6 calm offer-action
        then
    then then
;

: house-of-truth-2f-1.act00 ( -- )   \ 00431B20
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    0 creatures-clear
    scene-locked state-flag-set
    0 exit-prepare
    0 self-scripted
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-2f-1.act01 ( -- )   \ 00431B40
    self-wait-done
    0 self-through-exit
    self-wait-done
    1 ebit? not 2 game-mode? or in-panic? or if
        in-panic? 2 game-mode? or if
            rattle-behind-slow-anim play-out
            -1 self-idle-anim
            7 wait-frames
        else
            rattle-behind-anim play-out
            -1 self-idle-anim
            7 wait-frames
            0 say   \ "The door is locked."
        then
        1 ebit-set
    else
        4 wait-frames
        fiona-who $26E -10.0 135.3 0 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 0.0 0.0 2.0 event-camera
        4 wait-frames
        1 say   \ "A strange symbol is carved into the door. The symbol looks like a sun."
        4 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        1 ebit-clear
        world-frozen state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: house-of-truth-2f-1.act02 ( -- )   \ 00431BD0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $609 self-anim
    $14 wait-frames
    fiona-who 3 6 char-sound
    $25 message-param-room
    $E6 door-locked? not if
        $25 item-use
    then
    $E2 door-unlock
    20 self-idle-anim
    $14 wait-frames
    $8019 say   \ "Used ."
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-1.act03 ( -- )   \ 00431C10
    1 self-scripted
    self-wait-done
    camera-restart
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
    $E6 door-lock
    $EA door-lock
    $103 door-lock
    hewie-who $8E 1 room-doors-state
    stalker-who $8E 1 room-doors-state
    stalker-who $8C 1 room-doors-state
    stalker-who char-activate
    $FE $82 622 2 stalker-to-room
    stalker-who $26E 180 char-to-tri-facing
    stalker-who 1 1 char-camera
    summoner-on state-flag-set
    summoner-own-cooldown
    room-sounds
    $F $41 fade
    wait-fade
    $24F item-give
    scene-locked state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-2f-1.act04 ( -- )   \ 00431CC0
    self-wait-done
    fiona-who $FB -34.0 99.0 90 char-to-xz
    1 46.0 10.0 150.0 0.0 event-camera
    2 say   \ "This room connects to another corridor... But it doesn't seem like I can get through."
    16 wait-frames
    self-idle-or-end
;

: house-of-truth-2f-1.act05 ( -- )   \ 00431CF0
    self-wait-done
    -50.0 -15.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht2f-1-medallion story-flag-set
            6 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-2f-1.act06 ( -- )   \ 00431D50
    self-wait-done
    -17.0 -41.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht2f-1-viola-medallion story-flag-set
            7 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-2f-1 register-scripts   \ (its phases and actions, by their names)
