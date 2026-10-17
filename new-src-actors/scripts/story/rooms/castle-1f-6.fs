\ story/rooms/castle-1f-6.fs - the event scripts of room castle-1f-6 ($11; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-6
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

: castle-1f-6.enter ( -- )   \ 003F7920
    0 1.2 15.55 -24.813 0 flame-in
    1 -1.26 15.55 -24.365 0 flame-in
    2 4.387 15.55 -6.264 0 flame-in
    3 2.327 15.55 -4.824 0 flame-in
    4 -3.559 15.55 -5.985 0 flame-in
    5 -1.887 15.55 -0.304 0 flame-in
    6 1.651 15.55 3.417 0 flame-in
    7 2.473 15.55 5.786 0 flame-in
    8 -2.85 15.55 13.116 0 flame-in
    9 -0.431 15.55 13.794 0 flame-in
    $A 3.347 15.55 19.167 0 flame-in
    $B 1.908 15.55 24.905 0 flame-in
    $C -0.597 15.55 25.104 0 flame-in
    got-cracked-plate-key-2 story-flag? not if
        1 part-shown
    else
        1 part-hidden
    then
    c1f-6-medallion-found story-flag? c1f-6-medallion story-flag? not and if
        $E 34.2 1.0 60.5 flicker-sprite
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
;

: castle-1f-6.char-enter ( -- )   \ 003F7A20
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
        $80 exit-taken? if
            fiona-who $E8 char-to-tri
            0 0 player-camera
            0 fiona-who 2 action
        then
    then
;

: castle-1f-6.phase1 ( -- )   \ 003F7AC0
    0 exit-usable? if
        0 exit-check
    then
    hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
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
    fiona-who 2 char-entered-area? if
        hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
            3 0 char-remove
        then
        0 exit-prepare
    then
    fiona-who 3 char-left-area? if
        hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
            3 3 1 char-load
        then
        1 exit-prepare
    then
    c1f-6-medallion-found story-flag? not if
        0 34.2 0.0 60.5 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var c1f-6-medallion-found var-set
                taken-flag-var c1f-6-medallion var-set
                sparkle-fx-var 14 var-set
                sparkle-x-var 34200 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 60500 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
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
    7 6 8 -4 0 zone-at-effect
    fiona-who 7 3 char-zone-bits? fiona-who 7 3 char-zone-bits-before? not and if
        fiona-who 6 char-effect-moving
    then
    hewie-who 7 3 char-zone-bits? hewie-who 7 3 char-zone-bits-before? not and if
        hewie-who 6 char-effect-moving
    then
    stalker-who 7 3 char-zone-bits? stalker-who 7 3 char-zone-bits-before? not and if
        stalker-who 6 char-effect-moving
    then
    8 7 8 -4 0 zone-at-effect
    fiona-who 8 3 char-zone-bits? fiona-who 8 3 char-zone-bits-before? not and if
        fiona-who 7 char-effect-moving
    then
    hewie-who 8 3 char-zone-bits? hewie-who 8 3 char-zone-bits-before? not and if
        hewie-who 7 char-effect-moving
    then
    stalker-who 8 3 char-zone-bits? stalker-who 8 3 char-zone-bits-before? not and if
        stalker-who 7 char-effect-moving
    then
    9 8 8 -4 0 zone-at-effect
    fiona-who 9 3 char-zone-bits? fiona-who 9 3 char-zone-bits-before? not and if
        fiona-who 8 char-effect-moving
    then
    hewie-who 9 3 char-zone-bits? hewie-who 9 3 char-zone-bits-before? not and if
        hewie-who 8 char-effect-moving
    then
    stalker-who 9 3 char-zone-bits? stalker-who 9 3 char-zone-bits-before? not and if
        stalker-who 8 char-effect-moving
    then
    $A 9 8 -4 0 zone-at-effect
    fiona-who $A 3 char-zone-bits? fiona-who $A 3 char-zone-bits-before? not and if
        fiona-who 9 char-effect-moving
    then
    hewie-who $A 3 char-zone-bits? hewie-who $A 3 char-zone-bits-before? not and if
        hewie-who 9 char-effect-moving
    then
    stalker-who $A 3 char-zone-bits? stalker-who $A 3 char-zone-bits-before? not and if
        stalker-who 9 char-effect-moving
    then
    $B $A 8 -4 0 zone-at-effect
    fiona-who $B 3 char-zone-bits? fiona-who $B 3 char-zone-bits-before? not and if
        fiona-who $A char-effect-moving
    then
    hewie-who $B 3 char-zone-bits? hewie-who $B 3 char-zone-bits-before? not and if
        hewie-who $A char-effect-moving
    then
    stalker-who $B 3 char-zone-bits? stalker-who $B 3 char-zone-bits-before? not and if
        stalker-who $A char-effect-moving
    then
    $C $B 8 -4 0 zone-at-effect
    fiona-who $C 3 char-zone-bits? fiona-who $C 3 char-zone-bits-before? not and if
        fiona-who $B char-effect-moving
    then
    hewie-who $C 3 char-zone-bits? hewie-who $C 3 char-zone-bits-before? not and if
        hewie-who $B char-effect-moving
    then
    stalker-who $C 3 char-zone-bits? stalker-who $C 3 char-zone-bits-before? not and if
        stalker-who $B char-effect-moving
    then
    $D $C 8 -4 0 zone-at-effect
    fiona-who $D 3 char-zone-bits? fiona-who $D 3 char-zone-bits-before? not and if
        fiona-who $C char-effect-moving
    then
    hewie-who $D 3 char-zone-bits? hewie-who $D 3 char-zone-bits-before? not and if
        hewie-who $C char-effect-moving
    then
    stalker-who $D 3 char-zone-bits? stalker-who $D 3 char-zone-bits-before? not and if
        stalker-who $C char-effect-moving
    then
;

: castle-1f-6.phase2 ( -- )   \ 003F7E00
    got-cracked-plate-key-2 story-flag? not if
        $E 4.0 8.5 -20.0 5 5 0 zone
        fiona-who $E 3 char-zone-bits? if
            1 calm offer-action
        then
    then
    c1f-6-medallion-found story-flag? c1f-6-medallion story-flag? not and if
        0 $E 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then
    fiona-who 5 char-in-area? fiona-who -45 $3C char-heading? and if
        0 unhurried offer-action
    then
    fiona-who 6 char-in-area? fiona-who 45 $3C char-heading? and if
        0 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 90 $32 char-heading? and if
        4 unhurried offer-action
    then
    -2147483646 scene-request? if
        5 always offer-action
    then
;

: castle-1f-6.act00 ( -- )   \ 003F7E80
    self-wait-done
    c2f-3-flag1 story-flag? not if
        6 message   \ "Why are there so many candlesticks on this table?"
        wait-message
    else
        7 message   \ "There are just so many candlesticks on this table... I don't think I'm imagining th..."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-6.act01 ( -- )   \ 003F7E90
    self-wait-done
    4.0 -20.0 self-turn-to-xz
    self-wait-done
    reach-out-anim self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-cracked-plate-key-2 story-flag-set
    1 part-hidden
    $13 message-param-room
    $13 1 item-give-count
    0 $13 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    reach-back-anim self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-1f-6.act02 ( -- )   \ 003F7EE0
    self-wait-done
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $FFFF message-close
    3 door-open-clear
    doors-room-in
    world-held state-flag-set
    $1D 3 $FF char-load
    3 char-start
    0 exit-prepare
    $32 $FF movie-param
    0 part-shown
    panic-eased
    2 message-prepare   \ "My creator said he made me the perfect woman... But I cannot taste or experienc..."
    1 result? if
        scene-play
    then
    3 0 char-remove
    wait-fade
    world-held state-flag-set
    3 action-end
    3 char-done
    fiona-who $E8 char-to-tri
    0 0 player-camera
    camera-restart
    c1f-6-flag1 story-flag-set
    $80 exit-check
    0 part-hidden
    self-idle-or-end
;

: castle-1f-6.act03 ( -- )   \ 003F7FA0
    self-wait-done
    34.2 60.5 self-turn-to-xz
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
            c1f-6-medallion story-flag-set
            $E effect-remove
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

: castle-1f-6.act04 ( -- )   \ 003F8000
    self-wait-done
    180 self-turn-angle
    self-wait-done
    turn-round-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    look-at-anim self-anim
    self-wait-anim
    8 message   \ "A large tapestry is suspended here. I wonder if this is an embroidery of one of the cas..."
    wait-message
    self-idle-or-end
;

: castle-1f-6.act05 ( -- )   \ 003F8020
    self-wait-done
    stalker-who self-touching? not if
        3 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who 3 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    3 door-unlock
                    $8008 message   \ "You unlocked the door."
                    20 self-idle-anim
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: castle-1f-6.act06 ( -- )   \ 003F8060
    self-wait-done
    0 1.2 15.55 -24.813 0 flame-in
    1 -1.26 15.55 -24.365 0 flame-in
    2 4.387 15.55 -6.264 0 flame-in
    3 2.327 15.55 -4.824 0 flame-in
    4 -3.559 15.55 -5.985 0 flame-in
    5 -1.887 15.55 -0.304 0 flame-in
    6 1.651 15.55 3.417 0 flame-in
    7 2.473 15.55 5.786 0 flame-in
    8 -2.85 15.55 13.116 0 flame-in
    9 -0.431 15.55 13.794 0 flame-in
    $A 3.347 15.55 19.167 0 flame-in
    $B 1.908 15.55 24.905 0 flame-in
    $C -0.597 15.55 25.104 0 flame-in
    3 partner-load
    2 char-start
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $1D 3 $FF char-load
    3 char-start
    $32 $FF movie-param
    0 part-shown
    panic-eased
    2 message-prepare   \ "My creator said he made me the perfect woman... But I cannot taste or experienc..."
    1 result? if
        scene-play
    then
    3 0 char-remove
    3 action-end
    3 char-done
    0 part-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-1f-6 register-scripts   \ (its phases and actions, by their names)
