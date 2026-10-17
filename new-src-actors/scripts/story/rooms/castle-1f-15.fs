\ story/rooms/castle-1f-15.fs - the event scripts of room castle-1f-15 ($19; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-15
USING: engine room-names story.words story.shared flag-names story-names fiona.anims fiona.sounds ;

\ room 0x19 (Room19_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) while
\ Hewie is controlled: in another room, or 30 or more from the player
: castle-1f-15.cond00? ( -- flag )
    hunting-debilitas? 0= if  false exit  then  2 game-mode? 0= if  false exit  then
    stalker-in-this-room? 0= if  true exit  then
    stalker-at fiona-at vec-dist 30e f< 0= ;

: castle-1f-15.enter ( -- )   \ 003FB3B0
    room-sounds
    $FE exit-taken? if
        fiona-who $23F 90 char-to-tri-facing
        2 -1 player-camera
        0 fiona-who 9 action
    then
    c1f-15-camomile-found story-flag? c1f-15-camomile story-flag? not and if
        0 34.3 1.0 -13.7 flicker-sprite
    then
    c1f-15-medallion-found story-flag? c1f-15-medallion story-flag? not and if
        1 56.3 51.0 -71.5 flicker-sprite
    then
    1 $2300 sound-volume
    items-40-as-70 story-flag? if
        c1f-15-flag1 story-flag? not if
            c1f-15-flag1 story-flag-set
            $19 0 389 $80 10 -1 $FFDC 0.0 creature-place
        then
    then
;

: castle-1f-15.char-enter ( -- )   \ 003FB420
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
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
    then then
    3 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 4 exit-taken? and if
            fiona-who 4 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 4 exit-taken? and if
        hewie-who 4 char-to-exit
        1 1 player-camera
    then then
    4 1 1 area-camera
    fiona-who self-is? if
        4 exit-taken? 3 exit-taken? or if
            3 map-page
        then
    then
;

: castle-1f-15.phase1 ( -- )   \ 003FB520
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 -88.0 65.0 -95.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000008 6 -88.0 65.0 -95.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $10 1 1 1 chars-area-camera
    $11 2 -1 1 chars-area-camera
    $12 1 1 1 chars-area-camera
    $13 2 -1 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 7 char-entered-area? or if
        2 exit-prepare
    then
    fiona-who 6 char-entered-area? fiona-who 8 char-entered-area? or if
        3 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        4 exit-prepare
    then
    fiona-who $15 char-entered-area? fiona-who $16 char-entered-area? or if
        $27 room-preload
    then
    fiona-who 6 char-entered-area? if
        3 map-page
    then
    fiona-who 6 char-left-area? if
        2 map-page
    then
    0 -88.0 50.0 -94.617 $1E 20 0 zone
    4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 hewie-who $88 action
                    then
                then
            then
        then
    then
    0 -88.0 50.0 -94.617 $1E 20 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 -9.96 0.0 43.07 $53 16 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    c1f-15-camomile-found story-flag? not if
        5 34.3 0.0 -13.7 $A 5 0 zone
        hewie-who 5 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 c1f-15-camomile-found var-set
                $1A c1f-15-camomile var-set
                $1B 0 var-set
                $1C 34300 var-set
                $1D 1000 var-set
                $1E -13700 var-set
                $1F 5 var-set
                0 hewie-who $8B action
            then
        then
    then
    c1f-15-medallion-found story-flag? not if
        6 56.3 50.0 -71.5 $A 5 0 zone
        hewie-who 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 c1f-15-medallion-found var-set
                $1A c1f-15-medallion var-set
                $1B 1 var-set
                $1C 56300 var-set
                $1D 51000 var-set
                $1E -71500 var-set
                $1F 6 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: castle-1f-15.phase2 ( -- )   \ 003FB720
    fiona-who $C char-in-area? fiona-who 90 $2D char-heading? and if
        1 unhurried offer-action
    then
    fiona-who $D $3C char-faces-area? if
        2 unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who $F char-in-area? or fiona-who 90 $2D char-heading? and if
        3 unhurried offer-action
    then
    fiona-who $14 char-in-area? fiona-who -45 $5A char-heading? and if
        4 unhurried offer-action
    then
    7 ebit? not 9 ebit? not and if
        fiona-who $18 char-in-area? if
            2 game-mode? in-panic? not and if
                castle-1f-15.cond00? if
                    hewie-who char-here? not hewie-who 2 char-cond? or if
                        9 ebit-set
                        panic-eased
                        fiona-who 0 char-action? if
                            0 fiona-who 5 action-force
                        else
                            1 fiona-who 5 action-force
                        then
                    else
                        3 ebit-clear
                        7 0 pvar? if
                            0 chance? if
                                3 ebit-set
                            then
                        else 7 1 pvar? if
                            $32 chance? if
                                3 ebit-set
                            then
                        else 7 2 pvar? if
                            $5A chance? if
                                3 ebit-set
                            then
                        else 7 3 pvar? if
                            $5A chance? if
                                3 ebit-set
                            then
                        then then then then
                        3 ebit? not if
                            9 ebit-set
                            panic-eased
                            fiona-who 0 char-action? if
                                0 fiona-who 6 action-force
                            else
                                1 fiona-who 6 action-force
                            then
                        else
                            9 ebit-set
                            panic-eased
                            fiona-who 0 char-action? if
                                0 fiona-who 7 action-force
                            else
                                1 fiona-who 7 action-force
                            then
                        then
                    then
                then
            then
        then
    then
    fiona-who $17 char-in-area? fiona-who -45 $32 char-heading? and if
        8 unhurried offer-action
    then
    c1f-15-camomile-found story-flag? c1f-15-camomile story-flag? not and if
        5 0 5 5 0 zone-at-effect
        fiona-who 5 3 char-zone-bits? if
            $A calm offer-action
        then
    then
    c1f-15-medallion-found story-flag? c1f-15-medallion story-flag? not and if
        6 1 5 5 0 zone-at-effect
        fiona-who 6 3 char-zone-bits? if
            $B calm offer-action
        then
    then
;

: castle-1f-15.phase3 ( -- )   \ 003FB840
    camera-setup-changed? if
        0 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
    28.7 47.4 22.4 28.7 47.4 -29.0 74.0 47.4 22.4 74.0 47.4 -29.0 lights-doorway
    28.7 47.4 -29.0 28.7 47.4 -80.4 74.0 47.4 -29.0 74.0 47.4 -80.4 lights-doorway
    26.3 48.0 30.0 26.3 48.0 -35.9 72.1 48.0 30.0 72.1 48.0 -35.9 lights-doorway
    -55.1 48.0 -14.9 -55.1 48.0 39.6 -100.9 48.0 -14.9 -100.9 48.0 39.6 lights-doorway
    -65.1 48.0 18.6 -65.1 48.0 72.7 -110.9 48.0 18.2 -110.9 48.0 72.7 lights-doorway
    -65.1 48.0 61.0 -65.1 48.0 91.0 -110.9 48.0 61.0 -110.9 48.0 91.0 lights-doorway
;

: castle-1f-15.act00 ( -- )   \ 0047AA60
    self-idle-or-end
;

: castle-1f-15.act01 ( -- )   \ 003FB990
    self-wait-done
    $112 -0.195 -27.107 175 $FFFF 5 self-move-to
    self-wait-done
    look-close-anim self-anim
    self-wait-anim
    0 message   \ "'Agnus Dei' Looks like that's the title of this diorama."
    wait-message
    self-idle-or-end
;

: castle-1f-15.act02 ( -- )   \ 003FB9B0
    self-wait-done
    0 ebit? not if
        1 message   \ "This spear decoration serves as an obvious conversation piece. Hard to believe peop..."
        wait-message
        0 ebit-set
    else
        2 message   \ "I could try using this to protect myself... Damn! No good, it's just too heavy."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-15.act03 ( -- )   \ 003FB9D0
    self-wait-done
    1 ebit? not if
        3 message   \ "A suit of armor forged from steel."
        wait-message
        1 ebit-set
    else
        4 message   \ "I guess this room serves as a museum of sorts."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-15.act04 ( -- )   \ 003FB9F0
    self-wait-done
    $173 33.471 79.86 -90 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 50.0 15.0 0.0 event-camera
    2 ebit? not if
        5 message   \ "This section of handrail seems to be... gone. They always say 'never look down' in..."
        wait-message
        2 ebit-set
    else
        6 message   \ "If I fell here, there would be no coming back."
        wait-message
    then
    self-frames-reset
    self-wait-16
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act05 ( -- )   \ 003FBA50
    stalkers-stay state-flag-set
    1 self-scripted
    stalkers-blind state-flag-set
    $F $44 fade
    self-wait-done
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who $169 60.358 52.0 180 char-to-xz
    lying-anim 0 self-anim-blend
    camera-restart
    $FE $19 358 2 stalker-to-room
    stalker-who $166 55.085 70.506 180 char-to-xz
    stalker-who 1 1 char-camera
    hewie-who 0 char-visible
    hewie-who action-end
    stalkers-stay state-flag-clear
    hug-self-anim $1E self-anim-blend
    self-wait-anim
    $F $41 fade
    wait-fade
    stalkers-blind state-flag-clear
    $64 threat-raise
    7 ebit-set
    $1F resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act06 ( -- )   \ 003FBB40
    stalkers-stay state-flag-set
    1 self-scripted
    stalkers-blind state-flag-set
    $F $44 fade
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who $16C 63.654 67.0 -57 char-to-xz
    0 self-idle-anim
    camera-restart
    hewie-who $166 56.0 62.0 -57 char-to-xz
    $FE $19 161 2 stalker-to-room
    stalker-who $A1 11.784 75.875 -90 char-to-xz
    stalker-who stalker-knock-down
    stalker-who 0 0 char-camera
    7 pvar-inc
    stalkers-stay state-flag-clear
    $F $41 fade
    wait-fade
    stalkers-blind state-flag-clear
    10 hewie-trust
    $8278 item-give
    7 ebit-set
    $1D resident-flag-set
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act07 ( -- )   \ 003FBC20
    stalkers-stay state-flag-set
    1 self-scripted
    stalkers-blind state-flag-set
    $F $44 fade
    self-wait-done
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who $16C 63.054 67.67 -57 char-to-xz
    lying-anim 0 self-anim-blend
    camera-restart
    hewie-who $166 62.709 59.408 -57 char-to-xz
    $FE $19 359 2 stalker-to-room
    stalker-who $167 35.69 79.463 90 char-to-xz
    stalker-who 1 1 char-camera
    stalkers-stay state-flag-clear
    $F $41 fade
    wait-fade
    stalkers-blind state-flag-clear
    7 ebit-set
    $1E resident-flag-set
    $8279 item-give
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act08 ( -- )   \ 003FBD00
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    look-close-anim $A self-anim-blend
    self-wait-anim
    7 message   \ "Creaks and pops crackle out of the dubious-looking hole. Everything about the hole-floor scre..."
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        fiona-who slide-sound 5 char-sound
        world-held state-flag-set
        $FE exit-check
    else
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: castle-1f-15.act09 ( -- )   \ 003FBD30
    1 self-scripted
    self-wait-done
    camera-restart
    fiona-who slide-sound 5 char-sound
    3 map-page
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-15.act0A ( -- )   \ 003FBD50
    self-wait-done
    34.3 -13.7 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-15-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-15.act0B ( -- )   \ 003FBDB0
    self-wait-done
    56.3 -71.5 self-turn-to-xz
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
            c1f-15-medallion story-flag-set
            1 effect-remove
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

: castle-1f-15.act0C ( -- )   \ 003FBE10
    self-wait-done
    2 3 $FF char-load
    3 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
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

: castle-1f-15.act0D ( -- )   \ 003FBEA0
    self-wait-done
    2 3 $FF char-load
    3 char-start
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
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

: castle-1f-15.act0E ( -- )   \ 003FBF30
    self-wait-done
    2 3 $FF char-load
    3 char-start
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
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
castle-1f-15 register-scripts   \ (its phases and actions, by their names)
