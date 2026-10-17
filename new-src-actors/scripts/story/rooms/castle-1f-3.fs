\ story/rooms/castle-1f-3.fs - the event scripts of room castle-1f-3 ($9; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-3
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant bookshelf-remarked          \ (bit 0) its first remark made: "A wealth of information, right before your fingertips... A l"
1 constant harp-remarked               \ (bit 1) its first remark made: "This harp is so large and bulky... It's hard to imagine peop"
2 constant chase-scene-on              \ (bit 2) the chase scene under way: at the room's end the stalkers let go
3 constant item-scene-on               \ (bit 3) the item's scene (act05) under way
4 constant hewie-zone3-noticed         \ (bit 4) Hewie has reacted to zone 3 (once a visit)
5 constant creepy-thing-remarked       \ (bit 5) its first remark made: "W-What is this thing? It is really giving me the creeps. Why"

\ room 0x09 (Room09_Cond00_ptmf): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in
\ another room than 9 (the player about too)
: castle-1f-3.cond00? ( -- flag )  hunting-debilitas? stalker-room 9 <> and ;

: castle-1f-3.enter ( -- )   \ 003F3240
    c1f-3-flag1 story-flag? not if
        4 1.0 0 bgm
    then
    4 keeps-hewie-off floor-off
    c1f-3-bookshelf-examined story-flag? not if
        0 solid-floor floor-edge or pushable or floor-on
        1 keeps-debilitas-off floor-on
    else
        2 1 object-hidden
        0 solid-floor floor-edge or pushable or floor-off
        1 keeps-debilitas-off floor-off
        0 part-shown
        2 solid-floor floor-on
        3 keeps-debilitas-off floor-on
    then
    c1f-3-camomile story-flag? not if
        0 -6.0 11.5 -46.0 flicker-sprite
    then
    got-garden-key story-flag? not if
        3 part-shown
    then
    c1f-3-flag1 story-flag? not if
        1 part-shown
    else
        2 part-shown
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 $2300 sound-volume
;

: castle-1f-3.char-enter ( -- )   \ 003F32D0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
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
    hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 3 9 char-zone-bits? if
                    hewie-zone3-noticed ebit-set
                    $32 chance? if
                        hewie-zone-var 3 var-set
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
                hewie-zone-var 3 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    2 26.99 0.0 27.94 $18 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 22.0 hewie-look-zone
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
    chase-scene-on ebit? if
        stalkers-stay state-flag-clear
        force-chased state-flag-clear
    then
;

: castle-1f-3.act00 ( -- )   \ 003F34F0
    chase-scene-on ebit? if
        stalkers-stay state-flag-clear
        1 track action-end
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
    0 cutscene-track 3 action
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
        0 say   \ "A book shelf of fine design. It has a rather sturdy look to it."
    else
        ['] castle-1f-3.act00 goto
    then
    self-idle-or-end
;

: castle-1f-3.act02 ( -- )   \ 003F3630
    self-wait-done
    $D 4.0 13.0 0 $FFFF $A self-move-to
    self-wait-done
    4 keeps-hewie-off floor-on
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
    item-scene-on ebit-set
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
        0 2 track $E action
        panic-eased
        7 message-prepare   \ "Fiona. I see you've finally awakened... There's something that I've wanted..."
        1 result? if
            scene-play
        then
        $FF 1.0 0 bgm
        wait-fade
        fiona-who 0 char-no-shadow
        2 track action-end
        1 part-hidden
        2 part-shown
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
        reach-out-anim play-out
        4 wait-frames
        got-garden-key story-flag-set
        3 part-hidden
        $14 message-param-room
        $14 1 item-give-count
        0 $14 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
        reach-back-anim play-out
    then
    chase-scene-on ebit? not if
        stalkers-stay state-flag-clear
    then
    0 self-scripted
    item-scene-on ebit-clear
    self-idle-or-end
;

: castle-1f-3.act06 ( -- )   \ 003F37E0
    [: fiona-who char-busy? ;] wait-until
    1 message   \ "If I just apply enough force, I might be able to knock it over..."
    -1 self-idle-anim
    7 wait-frames
    wait-message
    self-idle-or-end
;

: castle-1f-3.act07 ( -- )   \ 0047A9B8
    0 say   \ "A book shelf of fine design. It has a rather sturdy look to it."
    self-idle-or-end
;

: castle-1f-3.act08 ( -- )   \ 003F3800
    self-wait-done
    stalkers-stay state-flag-set
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $DA -26.45 -13.04 0 char-to-xz
    1 20.0 -18.0 8.0 0.0 event-camera
    0 say   \ "A book shelf of fine design. It has a rather sturdy look to it."
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
        reach-out-anim play-out
        4 wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-3-camomile story-flag-set
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

: castle-1f-3.act0A ( -- )   \ 003F38B0
    self-wait-done
    bookshelf-remarked ebit? not if
        2 say   \ "A wealth of information, right before your fingertips... A lot of the books seem to..."
        bookshelf-remarked ebit-set
    else
        3 say   \ "Maybe they wouldn't mind if I took a few of these books... Nah, I'm sure they would..."
    then
    self-idle-or-end
;

: castle-1f-3.act0B ( -- )   \ 003F38D0
    self-wait-done
    $77 24.585 26.685 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 60.0 -45.0 30.0 event-camera
    $10 wait-frames
    look-at-anim self-anim
    $37 wait-frames
    harp-remarked ebit? not if
        4 say   \ "This harp is so large and bulky... It's hard to imagine people actually play these..."
        harp-remarked ebit-set
    else
        5 say   \ "Trying to play that for long stints of time would probably give even the most exper..."
    then
    self-wait-anim
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-3.act0C ( -- )   \ 003F3930
    stalkers-stay state-flag-set
    force-chased state-flag-set
    1 self-scripted
    c1f-3-bookshelf-hint story-flag-set
    chase-scene-on ebit-set
    self-wait-done
    rest-anim $A self-anim-blend
    6 message   \ "That bookshelf... I wonder if I can hide behind it?"
    self-wait-anim
    10 self-idle-anim
    wait-message
    0 self-scripted
    0 1 track $D action
    self-idle-or-end
;

: castle-1f-3.act0D ( -- )   \ 003F3960
    $F0 wait-frames
    [: item-scene-on ebit? ;] wait-while
    stalkers-stay state-flag-clear
    force-chased state-flag-clear
    chase-scene-on ebit-clear
    self-idle-or-end
;

: castle-1f-3.act0E ( -- )   \ 003F3980
    [: $64 cutscene-cue-reached? ;] wait-until
    $FF 1.0 0 bgm
    [: 4 cutscene-shot? ;] wait-until
    1 part-hidden
    2 part-shown
    self-idle-or-end
;

: castle-1f-3.act0F ( -- )   \ 003F39A0
    world-frozen state-flag-set
    1 self-scripted
    fiona-who $13B 20.181 -28.363 -90 char-to-xz
    1 10.0 20.0 -30.0 0.0 event-camera
    c1f-3-flag1 story-flag? not if
        8 say   \ "What... is this? I probably shouldn't go snooping around things I know nothing about."
    else creepy-thing-remarked ebit? not if
        9 say   \ "W-What is this thing? It is really giving me the creeps. Why would anyone in their..."
        creepy-thing-remarked ebit-set
    else
        $A say   \ "I think I'll refrain from touching it... Heck, even looking at it makes my skin cr..."
    then then
    $10 wait-frames
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-3.act10 ( -- )   \ 003F3A00
    self-wait-done
    4 1.0 0 bgm
    3 part-shown
    1 part-shown
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    4 4 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    fiona-who 1 char-no-shadow
    0 2 track $E action
    panic-eased
    7 message-prepare   \ "Fiona. I see you've finally awakened... There's something that I've wanted to s..."
    1 result? if
        scene-play
    then
    $FF 1.0 0 bgm
    fiona-who 0 char-no-shadow
    2 track action-end
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-3.act11 ( -- )   \ 003F3AB0
    self-wait-done
    2 part-shown
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    2 3 $FF char-load
    3 char-start
    6 4 movie-play
    5 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 3 action
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
    2 part-shown
    2 3 $FF char-load
    3 char-start
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 3 action
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
