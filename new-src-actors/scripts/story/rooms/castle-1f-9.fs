\ story/rooms/castle-1f-9.fs - the event scripts of room castle-1f-9 ($25; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-9
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ the rising motes started
: castle-1f-9.cmd00 ( -- )  effects:rising-motes ;
\ room 0x25: Fiona is active and in a reaction (action 4) with no character behind it (+0x100
\ 0xFF).
: castle-1f-9.cond00? ( -- flag )  fiona-move 4 =  fiona-struck? 0= and ;

: castle-1f-9.enter ( -- )   \ 00402330
    room-sounds
    c1f-9-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    c1f-9-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
        c1f-9-magnesia story-flag? not if
            1 -5.5 1.0 -99.0 flicker-sprite
        then
    then
    0 6 0.5 1.0 0.187 0.312 zone-rect
    c1f-9-rubrum-medallion-found story-flag? c1f-9-rubrum-medallion story-flag? not and if
        0 60.0 1.0 37.2 flicker-sprite
    then
    c1f-9-album-medallion story-flag? not if
        2 -33.0 43.5 15.0 flicker-sprite
    then
    castle-1f-9.cmd00
    c2f-3-flag1 story-flag? not if
        $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    else
        $10 20.0 19.7 36.4 $E $80 $80 $80 $40 specks
        $10 30.0 87.0 124.7 $E $80 $80 $80 $40 specks
        $10 -53.9 28.8 -52.0 $A $80 $80 $80 $40 specks
    then
;

: castle-1f-9.char-enter ( -- )   \ 00402420
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 -1 player-camera
    then then
    0 1 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 -1 player-camera
    then then
    1 1 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            3 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        3 1 player-camera
    then then
    2 3 1 area-camera
    fiona-who self-is? if
        saturnus-hint-on story-flag? c1f-14-flag2 story-flag? not and if
            c1f-14-flag2 story-flag-set
            stalker-who action-end
            stalker-who char-done
        then
        2 exit-taken? if
            3 map-page
        then
    then
;

: castle-1f-9.phase1 ( -- )   \ 004024F0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 1 -1 1 chars-area-camera
    4 0 0 1 chars-area-camera
    7 1 -1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    $B 1 -1 1 chars-area-camera
    $11 0 0 1 chars-area-camera
    $12 3 1 1 chars-area-camera
    $13 0 0 1 chars-area-camera
    $14 4 2 1 chars-area-camera
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? fiona-who 3 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        3 map-page
    then
    fiona-who 3 char-entered-area? if
        2 map-page
    then
    c1f-9-breakable1-broken story-flag? not if
        0 -30.0 0.0 -85.4 5 8 1 zone
        self-who 0 char-in-zone? if
            c1f-9-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -30.0 0.0 -85.4 0 -2145378272 0 0.0 shards-at
            $88 5 -30.0 0.0 -85.4 0 0 sound
            $40 $1E7 noise
        then
    then
    c1f-9-rubrum-medallion-found story-flag? not if
        2 60.0 0.0 37.2 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var c1f-9-rubrum-medallion-found var-set
                taken-flag-var c1f-9-rubrum-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 60000 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 37200 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
        2 game-mode? not if
            1 hewie-side? fiona-who $100000 char-on-nav-flags? and if
                fiona-who -37 16 $32 char-faces-xz? if
                    35 fiona-started? if
                        hewie-may-break-off? if
                            0 fiona-who 5 action
                        then
                    then
                then
            then
        then
    then
    c1f-9-flag1 story-flag? not if
        castle-1f-9.cond00? if
            c1f-9-flag1 story-flag-set
            0 $F1 2 action
        then
    then
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 0.0 71.0 35.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000000 6 0.0 71.0 35.0 0 0 sound
    then
    c1f-9-album-medallion story-flag? not if
        4 -33.85 42.7 15.04 $2C 30 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            hewie-zone-var 4 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
        3 -33.85 42.7 15.04 $2C 30 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 3 9 char-zone-bits? if
                    hewie-zone-var 3 var-set
                    hewie-zone-var 10.0 hewie-look-zone
                then
            then
        then
    then
;

: castle-1f-9.phase2 ( -- )   \ 00402700
    c1f-9-rubrum-medallion-found story-flag? c1f-9-rubrum-medallion story-flag? not and if
        2 0 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            7 calm offer-action
        then
    then
    fiona-who $C char-in-area? fiona-who 45 $32 char-heading? and if
        0 unhurried offer-action
    then
    fiona-who $D $32 char-faces-area? if
        1 unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who 35 -45 $32 char-faces-xz? and if
        8 unhurried offer-action
    then
    fiona-who 1 char-before-door? if
        -2147483646 scene-request? if
            9 always offer-action
        then
    then
    c1f-9-breakable2-broken story-flag? not if
        1 -5.5 0.0 -99.0 5 8 1 zone
        self-who 1 char-in-zone? if
            c1f-9-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            -5.5 0.0 -99.0 0 -2145378272 0 0.0 shards-at
            $88 5 -5.5 0.0 -99.0 0 0 sound
            $40 $1F1 noise
            1 -5.5 1.0 -99.0 flicker-sprite
        then
    else c1f-9-magnesia story-flag? not if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $B calm offer-action
        then
    then then
;

: castle-1f-9.phase3 ( -- )   \ 004027E0
    -37.6 70.0 29.5 -9.4 70.0 29.5 -37.6 32.2 29.5 -9.4 32.2 29.5 lights-doorway
    6.0 72.0 63.0 72.0 72.0 63.0 6.0 -2.0 63.0 72.0 -2.0 63.0 lights-doorway
    -39.0 72.0 38.0 32.0 72.0 38.0 -39.0 -2.0 38.0 32.0 -2.0 38.0 lights-doorway
;

: castle-1f-9.phase5 ( -- )   \ 00402880
    garden-door-done story-flag? if
        c1f-9-flag1 story-flag? not if
            fiona-who 0 char-before-door? fiona-who 1 char-before-door? or if
                c1f-9-flag1 story-flag-set
            then
        then
    then
;

: castle-1f-9.act00 ( -- )   \ 004028A0
    self-wait-done
    90 self-turn-angle
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 message   \ "Why do the stairs stop halfway?"
        wait-message
        0 ebit-set
    else
        4 message   \ "The longer I look over the edge, the more I feel like something is going to suck me..."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: castle-1f-9.act01 ( -- )   \ 004028C0
    self-wait-done
    0.0 -60.0 self-turn-to-xz
    self-wait-done
    1 message   \ "This looks like an old well or something. The opening has been sealed up with a wooden..."
    wait-message
    self-idle-or-end
;

: castle-1f-9.act02 ( -- )   \ 004028D0
    2 message   \ "What the!"
    begin
        fiona-free? not while
        yield
    repeat
    panic-eased
    0 fiona-who 3 action-force
    self-idle-or-end
;

: castle-1f-9.act03 ( -- )   \ 0047AB18
    self-wait-done
    3 message   \ "What was that thing?!"
    wait-message
    self-idle-or-end
;

: castle-1f-9.act04 ( -- )   \ 004028F0
    1 self-scripted
    stalkers-stay state-flag-set
    self-wait-done
    $F $54 fade
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    $28 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who 8 -19.086 -6.777 166 char-to-xz
    scene-locked state-flag-set
    $F $51 fade
    wait-fade
    $F 6 fade
    wait-fade
    $40A9 message   \ "'Luminessants' Luminessants are tiny creatures that feverishly pursue Fiona once th..."
    wait-message
    $F 7 fade
    wait-fade
    garden-door-done story-flag-set
    $A9 message-param-room
    $A9 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message   \ "has been saved to your file."
    wait-message
    2 char-start
    summoner-on state-flag-set
    got-doc-luminessants story-flag? not if
        got-doc-luminessants story-flag-set
        $25 0 217 3 6 -1 0 0.0 creature-place
    then
    $14 item-use
    6 door-unlock
    0 self-scripted
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    self-idle-or-end
;

: castle-1f-9.act05 ( -- )   \ 004029F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0 hewie-who 6 action-force
    0 counter-set
    go-there-gesture-anim self-anim
    fiona-who 1 char-wait-motion
    fiona-who $2E 5 char-sound
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    begin
        hewie-who $F char-in-area? not while
        yield
    repeat
    hewie-who 2 -1 char-camera
    1 wait-counter
    self-frames-reset
    $10 self-wait-frames
    0 self-scripted
    self-idle-or-end
;

: castle-1f-9.act06 ( -- )   \ 00402A30
    self-wait-done
    hewie-bark
    self-wait-done
    hewie-who camera-follow
    $3E -30.81 100.0 180 $FFFF $A self-move-to
    self-wait-done
    2 solid-floor floor-off
    2 keeps-fiona-off keeps-pursuers-off or floor-on
    $CF -35.0 20.3 0.2 50 hewie-go-to
    self-wait-done
    hewie-who 2 -1 char-camera
    c1f-9-album-medallion story-flag? not if
        $71 $63 item-count? not if
            10 hewie-trust
        then
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-9-album-medallion story-flag-set
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        c1f-9-album-medallion story-flag? if
            2 effect-remove
        then
    then
    $50 -35.79 15.06 100 $FFFF $A self-move-to
    self-wait-done
    3 solid-floor floor-off
    3 keeps-fiona-off keeps-pursuers-off or floor-on
    1 counter-set
    $196 18.0 6.0 0.1 100 hewie-go-to
    self-wait-done
    2 solid-floor floor-on
    3 solid-floor floor-on
    stalkers-stay state-flag-clear
    $25 0 406 hewie-to-room
    self-idle-or-end
;

: castle-1f-9.act07 ( -- )   \ 00402B00
    self-wait-done
    60.0 37.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-9-rubrum-medallion story-flag-set
            0 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-9.act08 ( -- )   \ 00402B60
    self-wait-done
    37.0 -44.0 self-turn-to-xz
    self-wait-done
    5 message   \ "The bench has been situated so it is facing the wall. No one can use it like this..."
    wait-message
    self-idle-or-end
;

: castle-1f-9.act09 ( -- )   \ 00402B70
    self-wait-done
    1 self-through-exit
    self-wait-done
    in-panic? 2 game-mode? or if
        rattle-front-slow-anim self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
    else
        rattle-front-anim self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        6 message   \ "The door is locked. I will need to find a key if I am going to get through this doo..."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-9.act0A ( -- )   \ 00402BA0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $608 self-anim
    self-frames-reset
    $14 self-wait-frames
    fiona-who 1 5 char-sound
    $14 message-param-room
    $14 item-use
    6 door-unlock
    20 self-idle-anim
    self-frames-reset
    $14 self-wait-frames
    $8019 message   \ "Used ."
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-9.act0B ( -- )   \ 00402BE0
    self-wait-done
    -5.5 -99.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-9-magnesia story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-9.act0C ( -- )   \ 00402C40
    self-wait-done
    $1F $AC $60 $26 $50 $19 $1F $12 $80 0 9 effect-string
    1 1 movie-play
    0 cutscene-start
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

\ ---- registered ----
castle-1f-9 register-scripts   \ (its phases and actions, by their names)
