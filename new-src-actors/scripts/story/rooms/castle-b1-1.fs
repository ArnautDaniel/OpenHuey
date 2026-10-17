\ story/rooms/castle-b1-1.fs - the event scripts of room castle-b1-1 ($12; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-1
USING: room-names story.words story.shared flag-names story-names ;

\ room 0x12 (Room12_Cmd00_ptmf): byte 3 0: the smoke puffs (SmokePuffs_vtable), their slot in
\ script variable 0; else that slot started
: castle-b1-1.cmd00 ( b0 -- )  0 ['] effects:smoke-puffs effect-in-var ;   \ (the smoke puffs: b0 0 going - kept in variable 0 - else stopped)

: castle-b1-1.enter ( -- )   \ 003F8200
    room-sounds
    got-castle-sketch story-flag? not if
        0 1 $14 door-bits
    then
    0 -35.8 -50.5 -27.25 0 flame-in
    cb1-1-breakable1-broken story-flag? not if
        1 0 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
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
        1 1 $1000010 nav-group
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
        3 1 $14 door-bits
        4 0 $14 door-bits
        1 2 $20000 nav-group
    else
        3 0 $14 door-bits
        4 1 $14 door-bits
        0 2 $20000 nav-group
    then
    1 $2300 sound-volume
;

: castle-b1-1.act10 ( -- )   \ 003F8DE0
    6 ebit-set
    8 ebit-set
    5 ebit-clear
    $E 0 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 1 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 2 pvar? if
        $64 chance? if
            5 ebit-set
        then
    else $E 3 pvar? if
        $64 chance? if
            5 ebit-set
        then
    then then then then
    stalker-who char-unseen? not if
        5 ebit-set
    then
    2 creature-action? if
        5 ebit-set
    then
    4 stalker-alert? if
        5 ebit-clear
    then
    stalker-who camera-follow
    5 ebit? if
        0 stalker-who $11 action
    else
        $78 1 summoner-wait
        $B ebit-clear
    then
    $E pvar-inc
    exit
;

: castle-b1-1.char-enter ( -- )   \ 003F8350
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            5 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        5 -1 player-camera
    then then
    0 5 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            2 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        2 -1 player-camera
    then then
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
            8 ebit-clear
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
        fiona-who 5 char-entered-area? $A ebit? not and if
            $A ebit-set
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
                $19 cb1-1-lavender-found var-set
                $1A cb1-1-lavender var-set
                $1B 2 var-set
                $1C -18100 var-set
                $1D -59000 var-set
                $1E -8100 var-set
                $1F 3 var-set
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
        9 ebit? not if
            6 sound-bank-loaded? if
                $40000001 6 15.0 12.0 22.0 0 0 sound
                9 ebit-set
            then
        else
            $C0000001 6 15.0 12.0 22.0 0 0 sound
        then
        4 16.5 0.0 16.5 $19 10 0 zone
        7 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 4 9 char-zone-bits? if
                        7 ebit-set
                        $64 chance? if
                            $1F 4 var-set
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
                    $1F 6 var-set
                    $1F 15.0 hewie-look-zone
                then
            then
        then
    then
    stalker-who 2 char-cond? not if
        fiona-hidden state-flag? 6 ebit? not and stalker-who char-here? and if
            $B ebit-set
            castle-b1-1.act10
        then
    then
    cb1-1-breakable2-broken story-flag? not if
        5 22.0 -40.0 -32.0 6 12 0 zone
        fiona-who 5 char-in-zone? if
            fiona-who 0 6 char-sound
            1 $FF 4 rumble
            3 0 $14 door-bits
            4 1 $14 door-bits
            0 2 $20000 nav-group
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
                $1F 7 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 -46.47 -60.0 -29.37 $E 19 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 8 9 char-zone-bits? if
                $1F 8 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
;

: castle-b1-1.phase2 ( -- )   \ 003F8750
    cb1-1-breakable1-broken story-flag? not if
        1 -52.5 -60.05 -1.5 5 8 1 zone
        self-who 1 char-in-zone? if
            cb1-1-breakable1-broken story-flag-set
            0 0 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -52.5 -60.05 -1.5 0 -2146424784 0 0.0 shards-at
            3 6 -52.5 -60.05 -1.5 0 0 sound
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
                3 ebit-clear
                $E calm offer-action
            else stalker-who char-unseen? if
                3 ebit-clear
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
                3 ebit-set
                $E calm offer-action
            else stalker-who char-unseen? if
                3 ebit-set
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
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cb1-1-chicken-jerky story-flag-set
            1 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-b1-1.act02 ( -- )   \ 003F8B48
    self-wait-done
    3 self-look-at
    yield
    3 message   \ "She looks busy preparing dinner... Perhaps this isn't the best time to strike up a conv..."
    wait-message
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
                self-frames-reset
                $14 self-wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    4 door-unlock
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

: castle-b1-1.act04 ( -- )   \ 003F8BA0
    1 self-scripted
    self-wait-done
    -35.35 -34.5 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    0 0 $14 door-bits
    5 message-param-room
    5 1 item-give-count
    0 5 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
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
    $E message   \ "This plywood-paneled door is blocking my path. But it could be broken through easily e..."
    wait-message
    self-idle-or-end
;

: castle-b1-1.act07 ( -- )   \ 0047AA08
    self-idle-or-end
;

: castle-b1-1.act08 ( -- )   \ 003F8CA0
    self-wait-done
    1 ebit? not if
        8 message   \ "Varied ornate, high-class china lines the shelf."
        wait-message
        1 ebit-set
    else
        9 message   \ "They have all been polished to sparkling perfection. The building itself may be old..."
        wait-message
    then
    self-idle-or-end
;

: castle-b1-1.act09 ( -- )   \ 003F8CB8
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    $D message   \ "The box emanates no specific odor. Looks like it is full of canned goods."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-b1-1.act0A ( -- )   \ 003F8CD0
    self-wait-done
    2 ebit? not if
        $B message   \ "Numerous expensive spirits stretch along the wine rack."
        wait-message
        2 ebit-set
    else
        $C message   \ "Come to think of it, my father was quite the wine connoisseur."
        wait-message
    then
    self-idle-or-end
;

: castle-b1-1.act0B ( -- )   \ 0047AA10
    self-wait-done
    $A message   \ "The sink looks rusted in several spots... A thick, reddish-black discoloration cakes t..."
    wait-message
    self-idle-or-end
;

: castle-b1-1.act0C ( -- )   \ 0047AA18
    self-wait-done
    5 message   \ "There is nothing in the pot..."
    wait-message
    self-idle-or-end
;

: castle-b1-1.act0D ( -- )   \ 003F8CE8
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $1E self-wait-frames
    4 message   \ "Looks like meat is being served... It appears they still haven't finished preparing it."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-b1-1.act0F ( -- )   \ 003F8DB0
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    stalker-who char-busy? if
        4 ebit? not if
            stalker-who action-end
            stalker-who 0 stalker-command
        then
    then
    0 self-scripted
    self-idle-or-end
;

: castle-b1-1.act0E ( -- )   \ 003F8D00
    stalkers-stay state-flag-set
    8 ebit-clear
    1 self-scripted
    6 ebit-clear
    fiona-who 5 char-file-load
    self-wait-done
    fiona-who char-file-use
    3 ebit? not if
        $14A $8004 5 -22.548 -33.225 -90 self-walk-anim
        self-wait-done
    else
        $1FA $8004 5 -49.507 -33.083 90 self-walk-anim
        self-wait-done
    then
    1 self-noclip
    1 self-scripted
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    4 ebit-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            in-panic? 4 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] castle-b1-1.act0F goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                8 ebit? stalker-who char-here? not and if
                    8 ebit-clear
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
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-b1-1.act11 ( -- )   \ 003F8E40
    self-wait-done
    $B ebit? not if
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
    $B ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who 6 char-file-load
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        stalker-who char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        4 ebit-set
        self-wait-anim
    else
        $1FE -35.74 -5.079 -180 $FFFF 5 self-move-to
        self-wait-done
        $1601 self-anim
        self-wait-anim
        4 ebit-set
    then
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-b1-1.act12 ( -- )   \ 003F8EE0
    self-wait-done
    -18.1 -8.1 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cb1-1-lavender story-flag-set
            2 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-b1-1.act13 ( -- )   \ 003F8F40
    self-wait-done
    -13.0 10.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cb1-1-chicken-jerky-2 story-flag-set
            3 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
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
    $C ebit? not if
        6 message   \ "The pot is filled with what look like tendrils. Wait a second, this looks like... h..."
        wait-message
        $C ebit-set
    else
        7 message   \ "Looking at it again is going to make me nauseous."
        wait-message
    then
    self-idle-or-end
;

\ ---- registered ----
castle-b1-1 register-scripts   \ (its phases and actions, by their names)
