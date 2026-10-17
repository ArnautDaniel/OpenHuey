\ story/rooms/castle-1f-14.fs - the event scripts of room castle-1f-14 ($E; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-14
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

: castle-1f-14.enter ( -- )   \ 003F5540
    c1f-14-viride-medallion-found story-flag? c1f-14-viride-medallion story-flag? not and if
        1 -70.0 61.0 54.1 flicker-sprite
    then
    c1f-14-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    c1f-14-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
        c1f-14-chicken-jerky story-flag? not if
            0 -68.0 1.0 -58.0 flicker-sprite
        then
    then
    c1f-14-breakable3-broken story-flag? not if
        2 keeps-people-off floor-on
        4 part-shown
        5 part-hidden
    else
        2 keeps-people-off floor-off
        4 part-hidden
        5 part-shown
        c1f-14-antimony-powder story-flag? not if
            2 -66.0 1.0 55.0 flicker-sprite
        then
    then
    1 $2300 sound-volume
    items-40-as-70 story-flag? if
        c1f-14-flag1 story-flag? not if
            c1f-14-flag1 story-flag-set
            $E 0 829 $80 10 -1 $FF80 0.0 creature-place
        then
    then
;

: castle-1f-14.char-enter ( -- )   \ 003F5610
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 2 player-camera
    then then
    0 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
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
    fiona-who self-is? if
        saturnus-hint-on story-flag? c1f-14-flag2 story-flag? not and if
            c1f-14-flag2 story-flag-set
            stalker-who action-end
            stalker-who char-done
        then
        0 exit-taken? if
            3 map-page
        then
        $80 exit-taken? if
            panic-eased
            0 fiona-who 4 action-force
        then
    then
    0 4 0.0 0.687 0.187 0.312 zone-rect
;

: castle-1f-14.phase1 ( -- )   \ 003F5710
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
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        2 exit-prepare
    then
    fiona-who $D char-entered-area? if
        3 map-page
    then
    fiona-who $D char-left-area? if
        2 map-page
    then
    c1f-14-viride-medallion-found story-flag? not if
        3 -70.0 60.0 54.1 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 c1f-14-viride-medallion-found var-set
                $1A c1f-14-viride-medallion var-set
                $1B 1 var-set
                $1C -70000 var-set
                $1D 61000 var-set
                $1E 54100 var-set
                $1F 3 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: castle-1f-14.phase2 ( -- )   \ 003F57E0
    c1f-14-breakable1-broken story-flag? not if
        0 -61.0 0.0 -52.0 5 8 1 zone
        self-who 0 char-in-zone? if
            c1f-14-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -61.0 0.0 -52.0 0 -2144325584 0 0.0 shards-at
            $88 5 -61.0 0.0 -52.0 0 0 sound
            $40 $55 noise
        then
    then
    c1f-14-breakable2-broken story-flag? not if
        1 -68.0 0.0 -58.0 5 8 1 zone
        self-who 1 char-in-zone? if
            c1f-14-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            -68.0 0.0 -58.0 0 -2144325584 0 0.0 shards-at
            $88 5 -68.0 0.0 -58.0 0 0 sound
            $40 $2BF noise
            0 -68.0 1.0 -58.0 flicker-sprite
        then
    else c1f-14-chicken-jerky story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            1 calm offer-action
        then
    then then
    c1f-14-breakable3-broken story-flag? not if
        2 -66.0 0.0 55.0 5 8 1 zone
        self-who 2 char-in-zone? if
            c1f-14-breakable3-broken story-flag-set
            2 keeps-people-off floor-off
            4 part-hidden
            5 part-shown
            -66.0 0.0 55.0 0 -2145378272 0 0.0 shards-at
            $88 5 -66.0 0.0 55.0 0 0 sound
            $40 $295 noise
            2 -66.0 1.0 55.0 flicker-sprite
        then
    else c1f-14-antimony-powder story-flag? not if
        2 2 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            3 calm offer-action
        then
    then then
    -2147483646 scene-request? if
        0 always offer-action
    then
    c1f-14-viride-medallion-found story-flag? c1f-14-viride-medallion story-flag? not and if
        3 1 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            2 calm offer-action
        then
    then
;

: castle-1f-14.phase3 ( -- )   \ 003F5980
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

: castle-1f-14.act00 ( -- )   \ 003F5BD0
    self-wait-done
    stalker-who self-touching? not if
        0 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who 0 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    0 door-unlock
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

: castle-1f-14.act01 ( -- )   \ 003F5C10
    self-wait-done
    -68.0 -58.0 self-turn-to-xz
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
            c1f-14-chicken-jerky story-flag-set
            0 effect-remove
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

: castle-1f-14.act02 ( -- )   \ 003F5C70
    self-wait-done
    -70.0 54.1 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-14-viride-medallion story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-14.act03 ( -- )   \ 003F5CD0
    self-wait-done
    -66.0 55.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-14-antimony-powder story-flag-set
            2 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-14.act04 ( -- )   \ 003F5D30
    3 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    self-wait-done
    fiona-who 1 char-visible
    stalker-who action-end
    stalker-who char-done
    hewie-who action-end
    hewie-who char-done
    $B 3 $FF char-load
    $10 4 $FF char-load
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    3 char-start
    4 char-start
    panic-eased
    0 message-prepare   \ "Fiona! Fiona! Fio...! Don't go, Fiona... My life... My Azoth... Fiona..."
    1 result? if
        scene-play
    then
    3 0 char-remove
    4 0 char-remove
    $2D 2 pvar-set
    2 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: castle-1f-14.act05 ( -- )   \ 003F5DE0
    self-wait-done
    fiona-who 1 char-visible
    $B 3 $FF char-load
    $10 4 $FF char-load
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    $37 room-preload
    3 char-start
    4 char-start
    panic-eased
    0 message-prepare   \ "Fiona! Fiona! Fio...! Don't go, Fiona... My life... My Azoth... Fiona..."
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
castle-1f-14 register-scripts   \ (its phases and actions, by their names)
