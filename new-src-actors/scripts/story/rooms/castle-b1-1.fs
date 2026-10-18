\ story/rooms/castle-b1-1.fs - the event scripts of room castle-b1-1 ($12; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
1 constant china-remarked              \ (bit 1) its first remark made: "Varied ornate, high-class china lines the shelf."
2 constant wine-rack-remarked          \ (bit 2) its first remark made: "Numerous expensive spirits stretch along the wine rack."
3 constant may-hide                    \ (bit 3) the stalker not here or not seeing: her hiding offered
4 constant found-her                   \ (bit 4) he has found her hiding: she must flee (the avoid prompt)
5 constant will-find-her               \ (bit 5) he will find her hiding (by chance, as he searches: more likely by the progress variable)
6 constant searched-hiding             \ (bit 6) he has searched her hiding place
7 constant hewie-zone4-noticed         \ (bit 7) Hewie has reacted to zone 4 (once a visit)
8 constant searching                   \ (bit 8) he is searching (act10)
9 constant sound-started               \ (bit 9) its sound started (then kept up)
10 constant area5-reacted              \ (bit 10) her reaction on entering the area made (once a visit)
11 constant search-started             \ (bit 11) his search begun (phase 1 starts act10)
12 constant pot-remarked               \ (bit 12) its first remark made: "The pot is filled with what look like tendrils. Wait a secon"
0 constant break-sound                 \ (snd 0) the breakable giving way (rumble)
1 constant room-loop-sound             \ (snd 1) its steady sound (started as it comes in, then kept up)
3 constant shatter-sound               \ (snd 3) a breakable shattering

\ room 0x12 (Room12_Cmd00_ptmf): byte 3 0: the smoke puffs (SmokePuffs_vtable), their slot in
\ script variable 0; else that slot started
: castle-b1-1.cmd00 ( b0 -- )  0 ['] effects:smoke-puffs effect-in-var ;   \ (the smoke puffs: b0 0 going - kept in variable 0 - else stopped)

: castle-b1-1.enter ( -- )   \ 003F8200
    room-sounds
    got-castle-sketch story-flag? not if
        0 part-shown
    then
    0 -35.8 -50.5 -27.25 0 flame-in
    cb1-1-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        1 part-shown
        2 part-hidden
    else
        0 keeps-people-off floor-off
        1 part-hidden
        2 part-shown
        cb1-1-chicken-jerky story-flag? not if
            1 -52.5 -59.05 -1.5 flicker-sprite
        then
    then
    0 8 0.562 0.5 0.25 0.5 zone-rect
    cb1-1-lavender-found story-flag? cb1-1-lavender story-flag? not and if
        2 -18.1 -59.0 -8.1 flicker-sprite
    then
    cb1-1-chicken-jerky-2 story-flag? not if
        3 -13.0 9.0 10.0 flicker-sprite
    then
    hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
        force-followed state-flag-set
        1 keeps-fiona-off keeps-hewie-off or floor-on
        3 char-start
        3 char-activate
        0 3 0 char-model-op
        3 $9000 1 0 char-anim-hold
        3 $2A 14.85 17.65 0 char-to-xz
        4 0 object-anim-loop
        0 castle-b1-1.cmd00
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    1 0 -19.5 -26.0 -31.0 4.0 4.0 100.0 0.0 10.0 wanderer
    2 0 -53.0 -22.0 -11.0 4.0 5.0 80.0 0.0 30.0 wanderer
    cb1-1-breakable2-broken story-flag? not if
        3 part-shown
        4 part-hidden
        2 barred-floor floor-on
    else
        3 part-hidden
        4 part-shown
        2 barred-floor floor-off
    then
    1 $2300 sound-volume
;

: castle-b1-1.act10 ( -- )   \ 003F8DE0
    searched-hiding ebit-set
    searching ebit-set
    will-find-her ebit-clear
    $E 0 pvar? if
        $64 chance? if
            will-find-her ebit-set
        then
    else $E 1 pvar? if
        $64 chance? if
            will-find-her ebit-set
        then
    else $E 2 pvar? if
        $64 chance? if
            will-find-her ebit-set
        then
    else $E 3 pvar? if
        $64 chance? if
            will-find-her ebit-set
        then
    then then then then
    stalker-who char-unseen? not if
        will-find-her ebit-set
    then
    2 creature-action? if
        will-find-her ebit-set
    then
    4 stalker-alert? if
        will-find-her ebit-clear
    then
    stalker-who camera-follow
    will-find-her ebit? if
        0 stalker-who $11 action
    else
        $78 1 summoner-wait
        search-started ebit-clear
    then
    $E pvar-inc
    exit
;

: castle-b1-1.char-enter ( -- )   \ 003F8350
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        5 -1 player-camera
    then
    0 5 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 -1 player-camera
    then
    1 2 -1 area-camera
    fiona-who self-is? if
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
    hewie-who self-is? if
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            castle-b1-1.act10
        else
            searching ebit-clear
        then
    then
;

: castle-b1-1.phase1 ( -- )   \ 003F83F0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 1 0 1 chars-area-camera
    3 2 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 3 1 1 chars-area-camera
    7 0 -1 1 chars-area-camera
    8 3 1 1 chars-area-camera
    $10 1 0 1 chars-area-camera
    $11 5 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who $13 char-entered-area? if
        2 map-page
    then
    fiona-who $13 char-left-area? if
        1 map-page
    then
    got-castle-sketch story-flag? c2f-3-flag1 story-flag? not and if
        fiona-who 5 char-entered-area? area5-reacted ebit? not and if
            area5-reacted ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 5 action-force
            else
                1 fiona-who 5 action-force
            then
        then
    then
    cb1-1-lavender-found story-flag? not if
        3 -18.1 -60.0 -8.1 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var cb1-1-lavender-found var-set
                taken-flag-var cb1-1-lavender var-set
                sparkle-fx-var 2 var-set
                sparkle-x-var -18100 var-set
                sparkle-y-var -59000 var-set
                sparkle-z-var -8100 var-set
                hewie-zone-var 3 var-set
                0 hewie-who $8B action
            then
        then
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
        cb1-1-flag1 story-flag? not if
            2 16.5 0.0 16.5 $1E 10 0 zone
        else
            2 16.5 0.0 16.5 $A 10 0 zone
        then
        cb1-1-flag1 story-flag? not if
            fiona-who 2 3 char-zone-bits? 2 camera-mode? and if
                cb1-1-flag1 story-flag-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 0 action-force
                else
                    1 fiona-who 0 action-force
                then
            then
        else fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
            fiona-who 0 char-action? in-panic? not and if
                panic-eased
                0 fiona-who 2 action-force
            then
        then then
        sound-started ebit? not if
            6 sound-bank-loaded? if
                room-loop-sound volume-scaled 6 15.0 12.0 22.0 0 0 sound
                sound-started ebit-set
            then
        else
            room-loop-sound kept-going volume-scaled 6 15.0 12.0 22.0 0 0 sound
        then
        4 16.5 0.0 16.5 $19 10 0 zone
        hewie-zone4-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        hewie-zone4-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 4 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
        6 16.5 0.0 16.5 $32 10 0 zone
        hewie-who char-here? 1 game-mode? and if
            hewie-can-command? if
                hewie-who 6 9 char-zone-bits? if
                    hewie-zone-var 6 var-set
                    hewie-zone-var 15.0 hewie-look-zone
                then
            then
        then
    then
    stalker-who 2 char-cond? not if
        fiona-hidden state-flag? searched-hiding ebit? not and stalker-who char-here? and if
            search-started ebit-set
            castle-b1-1.act10
        then
    then
    cb1-1-breakable2-broken story-flag? not if
        5 22.0 -40.0 -32.0 6 12 0 zone
        fiona-who 5 char-in-zone? if
            fiona-who break-sound 6 char-sound
            1 $FF 4 rumble
            3 part-hidden
            4 part-shown
            2 barred-floor floor-off
            cb1-1-breakable2-broken story-flag-set
            0 8 0.234 0.5 0.328 0.5 zone-rect
            19.0 -40.0 -32.0 0 -2144325584 2 0.0 shards-at
            22.0 -40.0 -32.0 0 -2143272896 2 0.0 shards-at
            25.0 -40.0 -32.0 0 -2144325584 2 0.0 shards-at
            0 18.0 -36.0 -33.0 0 0 0 0 dust
            0 23.0 -38.0 -33.0 0 0 0 0 dust
            0 28.0 -36.0 -33.0 0 0 0 0 dust
        then
    then
    7 0.6 0.0 -3.32 $F 14 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 7 9 char-zone-bits? if
                hewie-zone-var 7 var-set
                hewie-zone-var 14.0 hewie-look-zone
            then
        then
    then
    8 -46.47 -60.0 -29.37 $E 19 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 8 9 char-zone-bits? if
                hewie-zone-var 8 var-set
                hewie-zone-var 19.0 hewie-look-zone
            then
        then
    then
;

: castle-b1-1.phase2 ( -- )   \ 003F8750
    cb1-1-breakable1-broken story-flag? not if
        1 -52.5 -60.05 -1.5 5 8 1 zone
        self-who 1 char-in-zone? if
            cb1-1-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            1 part-hidden
            2 part-shown
            -52.5 -60.05 -1.5 0 -2146424784 0 0.0 shards-at
            shatter-sound 6 -52.5 -60.05 -1.5 0 0 sound
            $40 $209 noise
            1 -52.5 -59.05 -1.5 flicker-sprite
        then
    else cb1-1-chicken-jerky story-flag? not if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            1 calm offer-action
        then
    then then
    cb1-1-lavender-found story-flag? cb1-1-lavender story-flag? not and if
        3 2 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            $12 calm offer-action
        then
    then
    cb1-1-chicken-jerky-2 story-flag? not if
        9 3 5 5 0 zone-at-effect
        fiona-who 9 3 char-zone-bits? if
            $13 calm offer-action
        then
    then
    fiona-who $12 $2D char-faces-area? if
        fiona-who -45 $3C char-heading? if
            got-castle-sketch story-flag? not if
                fiona-who $A $2D char-faces-area? if
                    4 calm offer-action
                then
            else stalker-who char-here? not if
                may-hide ebit-clear
                $E calm offer-action
            else stalker-who char-unseen? if
                may-hide ebit-clear
                $E calm offer-action
            else
                $8016 examine-message   \ "I have to get away! I don't have time for this!"
            then then then
        then
        fiona-who 45 $3C char-heading? if
            got-castle-sketch story-flag? not if
                fiona-who $A $2D char-faces-area? if
                    4 calm offer-action
                then
            else stalker-who char-here? not if
                may-hide ebit-set
                $E calm offer-action
            else stalker-who char-unseen? if
                may-hide ebit-set
                $E calm offer-action
            else
                $8016 examine-message   \ "I have to get away! I don't have time for this!"
            then then then
        then
    then
    5 scene-request? not if
        fiona-who 9 char-in-area? fiona-who 45 $32 char-heading? and if
            8 unhurried offer-action
        then
        fiona-who $B char-in-area? fiona-who 45 $32 char-heading? and if
            9 unhurried offer-action
        then
        fiona-who $C char-in-area? fiona-who 90 $32 char-heading? and if
            $A unhurried offer-action
        then
        fiona-who $D char-in-area? fiona-who 0 $32 char-heading? and if
            $B unhurried offer-action
        then
        hewie-found-in-wire story-flag? not if
            fiona-who $E char-in-area? fiona-who 0 $32 char-heading? and if
                $C unhurried offer-action
            then
        else found-file-208 story-flag? if
            fiona-who $E char-in-area? fiona-who 0 $32 char-heading? and if
                $16 unhurried offer-action
            then
        then then
        fiona-who 3 -2 $32 char-faces-xz? fiona-who -3 -2 $32 char-faces-xz? or fiona-who $F char-in-area? and if
            $D unhurried offer-action
        then
    then
    fiona-who 5 3 char-zone-bits? fiona-who 0 $32 char-heading? and if
        6 unhurried offer-action
    then
;

: castle-b1-1.phase3 ( -- )   \ 003F8900
    -19.0 8.0 0.0 13.0 8.0 0.0 -19.0 0.0 0.0 13.0 0.0 0.0 lights-doorway
    -18.1 13.0 -2.7 -16.2 13.0 -2.7 -18.1 0.0 -2.7 -16.2 0.0 -2.7 lights-doorway
    -18.1 13.0 -1.0 -18.1 13.0 -2.7 -18.1 0.0 -1.0 -18.1 0.0 -2.7 lights-doorway
    -19.0 8.0 15.0 -19.0 8.0 0.0 -19.0 0.0 15.0 -19.0 0.0 0.0 lights-doorway
;

: castle-b1-1.phase5 ( -- )   \ 003F89D0
    hewie-found-in-wire story-flag? found-file-208 story-flag? not and if
        force-followed state-flag-clear
        1 stalker-who 0 char-model-op
        3 action-end
        3 char-done
    then
;

: castle-b1-1.act00 ( -- )   \ 003F89F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    4 object-anim-reset
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    1 castle-b1-1.cmd00
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    $50 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 $9000 1 0 char-anim-hold
    4 0 object-anim-loop
    4 0 object-hidden
    3 $2A 14.85 17.65 0 char-to-xz
    0 3 0 char-model-op
    hewie-who 0 char-visible
    hewie-who action-end
    wait-fade
    0 self-idle-anim
    fiona-who $18B 43.7 11.8 -75 char-to-xz
    2 -1 player-camera
    camera-restart
    0 castle-b1-1.cmd00
    $F $51 fade
    wait-fade
    $226 item-give
    $1B resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act01 ( -- )   \ 003F8AF0
    self-wait-done
    -52.5 -1.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            cb1-1-chicken-jerky story-flag-set
            1 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-b1-1.act02 ( -- )   \ 003F8B48
    self-wait-done
    3 self-look-at
    yield
    3 say   \ "She looks busy preparing dinner... Perhaps this isn't the best time to strike up a conv..."
    self-who self-look-at
    yield
    self-idle-or-end
;

: castle-b1-1.act03 ( -- )   \ 003F8B60
    self-wait-done
    stalker-who self-touching? not if
        4 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who 4 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                $14 wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    4 door-unlock
                    $8008 message   \ "You unlocked the door."
                    20 self-idle-anim
                    $14 wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: castle-b1-1.act04 ( -- )   \ 003F8BA0
    1 self-scripted
    self-wait-done
    -35.35 -34.5 self-turn-to-xz
    self-wait-done
    reach-out-anim play-out
    0 part-hidden
    5 message-param-room
    5 1 item-give-count
    0 5 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    got-castle-sketch story-flag-set
    summoner-on state-flag-clear
    5 door-open-clear
    doors-room-in
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act05 ( -- )   \ 003F8BF0
    stalkers-stay state-flag-set
    force-calm state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    $FE $12 25 2 stalker-to-room
    stalker-who action-end
    stalker-who char-done
    4 3 $FF char-load
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    stalker-who action-end
    stalker-who char-done
    scene-locked state-flag-set
    fiona-who 1 char-silent
    camera-restart
    stalker-who $12 0 room-doors-state
    stalkers-stay state-flag-clear
    0 self-scripted
    0 exit-check
    self-idle-or-end
;

: castle-b1-1.act06 ( -- )   \ 0047AA00
    self-wait-done
    $E say   \ "This plywood-paneled door is blocking my path. But it could be broken through easily e..."
    self-idle-or-end
;

: castle-b1-1.act07 ( -- )   \ 0047AA08
    self-idle-or-end
;

: castle-b1-1.act08 ( -- )   \ 003F8CA0
    self-wait-done
    china-remarked ebit? not if
        8 say   \ "Varied ornate, high-class china lines the shelf."
        china-remarked ebit-set
    else
        9 say   \ "They have all been polished to sparkling perfection. The building itself may be old..."
    then
    self-idle-or-end
;

: castle-b1-1.act09 ( -- )   \ 003F8CB8
    self-wait-done
    look-at-anim self-anim
    16 wait-frames
    $D say   \ "The box emanates no specific odor. Looks like it is full of canned goods."
    self-wait-anim
    self-idle-or-end
;

: castle-b1-1.act0A ( -- )   \ 003F8CD0
    self-wait-done
    wine-rack-remarked ebit? not if
        $B say   \ "Numerous expensive spirits stretch along the wine rack."
        wine-rack-remarked ebit-set
    else
        $C say   \ "Come to think of it, my father was quite the wine connoisseur."
    then
    self-idle-or-end
;

: castle-b1-1.act0B ( -- )   \ 0047AA10
    self-wait-done
    $A say   \ "The sink looks rusted in several spots... A thick, reddish-black discoloration cakes t..."
    self-idle-or-end
;

: castle-b1-1.act0C ( -- )   \ 0047AA18
    self-wait-done
    5 say   \ "There is nothing in the pot..."
    self-idle-or-end
;

: castle-b1-1.act0D ( -- )   \ 003F8CE8
    self-wait-done
    look-low-anim self-anim
    $1E wait-frames
    4 say   \ "Looks like meat is being served... It appears they still haven't finished preparing it."
    self-wait-anim
    self-idle-or-end
;

: castle-b1-1.act0F ( -- )   \ 003F8DB0
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A blend-out
    0 self-noclip
    stalker-who char-busy? if
        found-her ebit? not if
            stalker-who action-end
            stalker-who 0 stalker-command
        then
    then
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act0E ( -- )   \ 003F8D00
    stalkers-stay state-flag-set
    searching ebit-clear
    1 self-scripted
    searched-hiding ebit-clear
    fiona-who 5 char-file-load
    self-wait-done
    fiona-who char-file-use
    may-hide ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
    1 self-scripted
    fiona-who slide-sound 5 char-sound
    5 room-motion $A self-anim-blend8
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    found-her ebit-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] castle-b1-1.act0F goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                searching ebit? stalker-who char-here? not and if
                    searching ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] castle-b1-1.act0F goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            5 wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-b1-1.act11 ( -- )   \ 003F8E40
    self-wait-done
    search-started ebit? not if
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    else
        stalker-who 0 char-heading-for? 0 exit-door-open? not and stalker-who 0 char-in-area? and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and stalker-who 1 char-in-area? and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    then
    search-started ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who 6 char-file-load
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        stalker-who char-file-use
        0 room-motion $A self-anim-blend
        $1E wait-frames
        found-her ebit-set
        self-wait-anim
    else
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        there-anim play-out
        found-her ebit-set
    then
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-b1-1.act12 ( -- )   \ 003F8EE0
    self-wait-done
    -18.1 -8.1 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            cb1-1-lavender story-flag-set
            2 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-b1-1.act13 ( -- )   \ 003F8F40
    self-wait-done
    -13.0 10.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            cb1-1-chicken-jerky-2 story-flag-set
            3 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: castle-b1-1.act14 ( -- )   \ 003F8FA0
    self-wait-done
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    2 3 $FF char-load
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

: castle-b1-1.act15 ( -- )   \ 003F9040
    self-wait-done
    3 3 $FF char-load
    3 char-start
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    $50 $FF movie-param
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

: castle-b1-1.act16 ( -- )   \ 003F90D0
    self-wait-done
    pot-remarked ebit? not if
        6 say   \ "The pot is filled with what look like tendrils. Wait a second, this looks like... h..."
        pot-remarked ebit-set
    else
        7 say   \ "Looking at it again is going to make me nauseous."
    then
    self-idle-or-end
;

\ ---- registered ----
castle-b1-1 register-scripts   \ (its phases and actions, by their names)
