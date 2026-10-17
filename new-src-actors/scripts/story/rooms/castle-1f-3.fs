\ story/rooms/castle-1f-3.fs - the event scripts of room castle-1f-3 ($9; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-3
USING: room-names story.words story.shared flag-names story-names ;

\ room 0x09 (Room09_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in
\ another room than 9 (the player about too)
: castle-1f-3.cond00? ( -- flag )  hunting-debilitas? stalker-room 9 <> and ;

: castle-1f-3.enter ( -- )   \ 003F3240
    c1f-3-flag1 story-flag? not if
        4 1.0 0 bgm
    then
    0 4 $1000000 nav-group
    c1f-3-bookshelf-examined story-flag? not if
        1 0 $800088 nav-group
        1 1 $40 nav-group
    else
        2 1 object-hidden
        0 0 $800088 nav-group
        0 1 $40 nav-group
        0 1 $14 door-bits
        1 2 8 nav-group
        1 3 $40 nav-group
    then
    c1f-3-camomile story-flag? not if
        0 -6.0 11.5 -46.0 flicker-sprite
    then
    got-garden-key story-flag? not if
        3 1 $14 door-bits
    then
    c1f-3-flag1 story-flag? not if
        1 1 $14 door-bits
    else
        2 1 $14 door-bits
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 $2300 sound-volume
;

: castle-1f-3.char-enter ( -- )   \ 003F32D0
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
    fiona-who self-is? if
        c1f-3-bookshelf-examined story-flag? not if
            c1f-3-bookshelf-hint story-flag? not if
                2 game-mode? in-panic? not and if
                    castle-1f-3.cond00? if
                        stalker-who char-here? not if
                            0 fiona-who $C action
                        then
                    then
                then
            then
        then
    then
;

: castle-1f-3.phase1 ( -- )   \ 003F3330
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 1 1 1 chars-area-camera
    c1f-3-bookshelf-examined story-flag? not if
        fiona-who 8 char-in-area? if
            2 game-mode? in-panic? not and if
                castle-1f-3.cond00? if
                    0 fiona-who 0 action
                then
            then
        then
    then
    3 13.13 0.0 -30.63 $1C 11 0 zone
    4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 3 9 char-zone-bits? if
                    4 ebit-set
                    $32 chance? if
                        $1F 3 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    3 13.13 0.0 -30.63 $1C 11 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    2 26.99 0.0 27.94 $18 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
;

: castle-1f-3.phase2 ( -- )   \ 003F3400
    -2147483644 scene-request? if
        0 fiona-who 6 action
    else c1f-3-bookshelf-examined story-flag? not if
        fiona-who 9 char-in-area? fiona-who 90 $2D char-heading? and if
            7 unhurried offer-action
        then
        fiona-who 8 char-in-area? fiona-who 0 $2D char-heading? and if
            8 unhurried offer-action
        then
    then then
    fiona-who $A char-in-area? fiona-who -67 $5A char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 45 $2D char-heading? and if
        $B unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who -45 $2D char-heading? and if
        $F unhurried offer-action
    then
    c1f-3-camomile story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            9 calm offer-action
        then
    then
    got-garden-key story-flag? not if
        1 10.0 6.0 -17.0 5 5 0 zone
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then
;

: castle-1f-3.phase3 ( -- )   \ 003F34A0
    -0.9 11.8 -26.5 -2.9 11.7 -22.0 0.2 -1.0 -26.0 -1.9 -1.0 -22.0 lights-doorway
;

: castle-1f-3.phase5 ( -- )   \ 003F34D8
    2 ebit? if
        stalkers-stay state-flag-clear
        force-chased state-flag-clear
    then
;

: castle-1f-3.act00 ( -- )   \ 003F34F0
    2 ebit? if
        stalkers-stay state-flag-clear
        $F1 action-end
    then
    1 self-scripted
    c1f-3-bookshelf-examined story-flag-set
    hewie-who 9 1 room-doors-state
    $FE 9 103 2 stalker-to-room
    stalker-who $67 180 char-to-tri-facing
    stalker-who 1 char-visible
    stalker-who action-end
    stalker-who char-done
    $F $54 fade
    c2f-3-flag1 story-flag? not if
        6 4 movie-play
        5 cutscene-start
        yield
        yield
        scene-wait-ready
    else
        1 4 movie-play
        0 cutscene-start
        yield
        yield
        scene-wait-ready
    then
    wait-fade
    0 $F9 3 action
    hewie-who char-here? if
        0 hewie-who 2 action-force
        c1f-3-flag2 story-flag-set
    then
    fiona-who 1 char-no-shadow
    stalker-who 1 char-no-shadow
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    hewie-who char-here? if
        hewie-who action-end
    then
    camera-restart
    $FE 9 143 2 stalker-to-room
    stalker-who $8F -34.144 33.682 90 char-to-xz
    stalker-who 0 0 char-camera
    stalker-who char-activate
    stalker-who stalker-knock-down
    c2f-3-flag1 story-flag? not if
        $18 resident-flag-set
    else
        $19 resident-flag-set
    then
    0 self-scripted
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    stalker-who 0 char-no-shadow
    $E door-open-clear
    0 exit-prepare
    0 exit-check
    hewie-who 9 0 room-doors-state
    c1f-3-bookshelf-examined story-flag-set
    c1f-1-flag2 story-flag-clear
    self-idle-or-end
;

: castle-1f-3.act01 ( -- )   \ 003F3618
    castle-1f-3.cond00? not if
        0 message   \ "A book shelf of fine design. It has a rather sturdy look to it."
        wait-message
    else
        ['] castle-1f-3.act00 goto
    then
    self-idle-or-end
;

: castle-1f-3.act02 ( -- )   \ 003F3630
    self-wait-done
    $D 4.0 13.0 0 $FFFF $A self-move-to
    self-wait-done
    1 4 $1000000 nav-group
    self-idle-or-end
;

: castle-1f-3.act03 ( -- )   \ 003F3650
    begin
        4 0 cutscene-passed? if
            0 exit-door-open? not if
                $E door-open-set
                doors-room-in
                stalker-who $28 5 char-sound
            then
        then
        4 1 cutscene-passed? if
            stalker-who $13 7 char-sound
        then
        4 2 cutscene-passed? if
            hewie-who char-here? if
                0 hewie-who 4 action-force
            then
        then
        4 3 cutscene-passed? not while
        yield
    repeat
    self-idle-or-end
;

: castle-1f-3.act04 ( -- )   \ 003F3690
    self-wait-done
    $A1 6.0 33.0 0 $FFFF $A self-move-to
    self-wait-done
    self-idle-or-end
;

: castle-1f-3.act05 ( -- )   \ 003F36B0
    3 ebit-set
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    c1f-3-flag1 story-flag? not if
        $F $44 fade
        4 4 movie-play
        3 cutscene-start
        yield
        yield
        scene-wait-ready
        wait-fade
        fiona-who 1 char-no-shadow
        0 $F2 $E action
        panic-eased
        7 message-prepare   \ "Fiona. I see you've finally awakened... There's something that I've wanted..."
        1 result? if
            scene-play
        then
        $FF 1.0 0 bgm
        wait-fade
        fiona-who 0 char-no-shadow
        $F2 action-end
        1 0 $14 door-bits
        2 1 $14 door-bits
        2 0 object-hidden
        0 self-idle-anim
        fiona-who $13C 23.771 -27.607 -73 char-to-xz
        1 1 player-camera
        camera-restart
        c1f-3-flag1 story-flag-set
        $F $41 fade
        wait-fade
    else
        10.0 -17.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        got-garden-key story-flag-set
        3 0 $14 door-bits
        $14 message-param-room
        $14 1 item-give-count
        0 $14 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        $903 self-anim
        self-wait-anim
    then
    2 ebit? not if
        stalkers-stay state-flag-clear
    then
    0 self-scripted
    3 ebit-clear
    self-idle-or-end
;

: castle-1f-3.act06 ( -- )   \ 003F37E0
    begin
        fiona-who char-busy? not while
        yield
    repeat
    1 message   \ "If I just apply enough force, I might be able to knock it over..."
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

: castle-1f-3.act07 ( -- )   \ 0047A9B8
    0 message   \ "A book shelf of fine design. It has a rather sturdy look to it."
    wait-message
    self-idle-or-end
;

: castle-1f-3.act08 ( -- )   \ 003F3800
    self-wait-done
    stalkers-stay state-flag-set
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $DA -26.45 -13.04 0 char-to-xz
    1 20.0 -18.0 8.0 0.0 event-camera
    0 message   \ "A book shelf of fine design. It has a rather sturdy look to it."
    wait-message
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-3.act09 ( -- )   \ 003F3850
    self-wait-done
    -6.0 -46.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-3-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-3.act0A ( -- )   \ 003F38B0
    self-wait-done
    0 ebit? not if
        2 message   \ "A wealth of information, right before your fingertips... A lot of the books seem to..."
        wait-message
        0 ebit-set
    else
        3 message   \ "Maybe they wouldn't mind if I took a few of these books... Nah, I'm sure they would..."
        wait-message
    then
    self-idle-or-end
;

: castle-1f-3.act0B ( -- )   \ 003F38D0
    self-wait-done
    $77 24.585 26.685 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 60.0 -45.0 30.0 event-camera
    self-frames-reset
    $10 self-wait-frames
    $A00 self-anim
    self-frames-reset
    $37 self-wait-frames
    1 ebit? not if
        4 message   \ "This harp is so large and bulky... It's hard to imagine people actually play these..."
        wait-message
        1 ebit-set
    else
        5 message   \ "Trying to play that for long stints of time would probably give even the most exper..."
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-3.act0C ( -- )   \ 003F3930
    stalkers-stay state-flag-set
    force-chased state-flag-set
    1 self-scripted
    c1f-3-bookshelf-hint story-flag-set
    2 ebit-set
    self-wait-done
    1 $A self-anim-blend
    6 message   \ "That bookshelf... I wonder if I can hide behind it?"
    self-wait-anim
    10 self-idle-anim
    wait-message
    0 self-scripted
    0 $F1 $D action
    self-idle-or-end
;

: castle-1f-3.act0D ( -- )   \ 003F3960
    self-frames-reset
    $F0 self-wait-frames
    begin
        3 ebit? while
        yield
    repeat
    stalkers-stay state-flag-clear
    force-chased state-flag-clear
    2 ebit-clear
    self-idle-or-end
;

: castle-1f-3.act0E ( -- )   \ 003F3980
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    $FF 1.0 0 bgm
    begin
        4 cutscene-shot? not while
        yield
    repeat
    1 0 $14 door-bits
    2 1 $14 door-bits
    self-idle-or-end
;

: castle-1f-3.act0F ( -- )   \ 003F39A0
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $13B 20.181 -28.363 -90 char-to-xz
    1 10.0 20.0 -30.0 0.0 event-camera
    c1f-3-flag1 story-flag? not if
        8 message   \ "What... is this? I probably shouldn't go snooping around things I know nothing about."
        wait-message
    else 5 ebit? not if
        9 message   \ "W-What is this thing? It is really giving me the creeps. Why would anyone in their..."
        wait-message
        5 ebit-set
    else
        $A message   \ "I think I'll refrain from touching it... Heck, even looking at it makes my skin cr..."
        wait-message
    then then
    self-frames-reset
    $10 self-wait-frames
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-3.act10 ( -- )   \ 003F3A00
    self-wait-done
    4 1.0 0 bgm
    3 1 $14 door-bits
    1 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    4 4 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    fiona-who 1 char-no-shadow
    0 $F2 $E action
    panic-eased
    7 message-prepare   \ "Fiona. I see you've finally awakened... There's something that I've wanted to s..."
    1 result? if
        scene-play
    then
    $FF 1.0 0 bgm
    fiona-who 0 char-no-shadow
    $F2 action-end
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-3.act11 ( -- )   \ 003F3AB0
    self-wait-done
    2 1 $14 door-bits
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    2 3 $FF char-load
    3 char-start
    6 4 movie-play
    5 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 3 action
    fiona-who 1 char-no-shadow
    stalker-who 1 char-no-shadow
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

: castle-1f-3.act12 ( -- )   \ 003F3B60
    self-wait-done
    2 1 $14 door-bits
    2 3 $FF char-load
    3 char-start
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 3 action
    fiona-who 1 char-no-shadow
    stalker-who 1 char-no-shadow
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
castle-1f-3 register-scripts   \ (its phases and actions, by their names)
