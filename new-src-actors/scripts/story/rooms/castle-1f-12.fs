\ story/rooms/castle-1f-12.fs - the event scripts of room castle-1f-12 ($4; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-12
USING: room-names story.words story.shared flag-names story-names ;

\ two dials (byte 3: pstr_syuukouki2 on var 0, pstr_syuukouki1 on var 1, from -90 degrees), byte
\ 4 the step
: castle-1f-12.cmd00 ( b0 b1 -- )   \ (the two dials: b0 0 "syuukouki2" on progress variable 0, else "syuukouki1" on 1; b1 the step)
    swap if  s" syuukouki1" 1  else  s" syuukouki2" 0  then  -90 1 dial-step ;
\ room 0x04 (Room04_Cmd01_ptmf): five objects turned -75 / 75 degrees in turn
: castle-1f-12.cmd01 ( -- )   \ (the five doors turned -75 / 75 degrees in turn: open)
    -75e deg>rad s" autodoor0" turn-y object!  75e deg>rad s" autodoor1" turn-y object!
    -75e deg>rad s" autodoor2" turn-y object!  75e deg>rad s" autodoor3" turn-y object!
    -75e deg>rad s" autodoor4" turn-y object! ;
\ room 0x04 (Room04_Cmd02_ptmf): byte 3 0: room effect 0x1B (MirrorFragment_vtable) on its
\ object, a box (640, -560, 1000, 0, 0x60); else the effect gone
: castle-1f-12.cmd02 ( b0 -- )  $1B s" lens" $60 0  640e -560e 1000e  mirror-fragment ;   \ (the lens reflecting: b0 0 made, else gone)
\ room 0x04 (Room04_Cmd03_ptmf): an effect on one object (byte 3 0: at 90 degrees) or the other
\ (0)
: castle-1f-12.cmd03 ( b0 -- )   \ (a dial's lens catching the light: b0 0 "syuukouki2", turned 90 degrees more, else "syuukouki1")
    0= if  s" syuukouki2" pi f2/  else  s" syuukouki1" 0e  then  lens-flash ;

: castle-1f-12.enter ( -- )   \ 003F0DF0
    c1f-12-flag1 story-flag? not if
        room-sounds
        1 1 $20000 nav-group
    else
        castle-1f-12.cmd01
        0 1 $20000 nav-group
    then
    lunar-refractor-used story-flag? not if
        2 1 $14 door-bits
        2 1 object-hidden
    else
        2 0 $14 door-bits
        0 0 castle-1f-12.cmd00
    then
    1 0 castle-1f-12.cmd00
    c1f-12-breakable-broken story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        c1f-12-medallion story-flag? not if
            0 -73.5 -102.0 55.5 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    1 $2300 sound-volume
;

: castle-1f-12.char-enter ( -- )   \ 003F0E80
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
            2 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        2 -1 player-camera
    then then
    1 2 -1 area-camera
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
        $80 exit-taken? $81 exit-taken? or if
            fiona-who $56 -68.279 -15.589 -90 char-to-xz
            2 -1 player-camera
            0 fiona-who 5 action
        then
    then
;

: castle-1f-12.phase1 ( -- )   \ 003F0F70
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    $B 2 -1 1 chars-area-camera
    1 52.29 -103.0 49.3 $1E 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 30.64 -103.0 -51.68 $1E 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    fiona-who 3 char-entered-area? if
        c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        then
        1 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        then
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
            stalker-who $C char-file-load
        then
        2 exit-prepare
    then
;

: castle-1f-12.phase2 ( -- )   \ 003F1060
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            0 always offer-action
        then
    then
    c1f-12-flag1 story-flag? not if
        fiona-who $E char-in-area? fiona-who -45 $32 char-heading? and if
            2 unhurried offer-action
        then
    then
    fiona-who $C char-in-area? fiona-who 54 49 $32 char-faces-xz? and if
        3 unhurried offer-action
    then
    fiona-who $D char-in-area? fiona-who 30 -51 $32 char-faces-xz? and if
        6 unhurried offer-action
    then
    c1f-12-breakable-broken story-flag? not if
        0 -73.5 -103.0 55.5 5 8 1 zone
        self-who 0 char-in-zone? if
            c1f-12-breakable-broken story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -73.5 -103.0 55.5 0 -2144325584 0 0.0 shards-at
            $88 5 -73.5 -103.0 55.5 0 0 sound
            $40 $1E3 noise
            0 -73.5 -102.0 55.5 flicker-sprite
        then
    else c1f-12-medallion story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            8 calm offer-action
        then
    then then
;

: castle-1f-12.act00 ( -- )   \ 003F1130
    self-wait-done
    stalker-who self-touching? not if
        $1E self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $1E char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $1E door-unlock
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

: castle-1f-12.act01 ( -- )   \ 003F1170
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    $A room-preload
    self-wait-done
    wait-fade
    force-calm state-flag-set
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-1f-12.act02 ( -- )   \ 003F1190
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $A0 -76.0 -15.0 -90 char-to-xz
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 -10.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    3 ebit? not if
        3 message   \ "This part of the wall has been designed differently from the rest of the wall."
        wait-message
        3 ebit-set
    else
        4 message   \ "It's a picture of the moon... What could this mean?"
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-clear
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-12.act03 ( -- )   \ 003F11F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    c1f-12-flag1 story-flag? if
        1 message   \ "A device that refracts moonlight... I've never seen anything like this before."
        wait-message
    else lunar-refractor-used story-flag? not if
        self-frames-reset
        8 self-wait-frames
        world-frozen state-flag-set
        fiona-who $73 61.0 50.0 -90 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        self-frames-reset
        8 self-wait-frames
        2 message   \ "I wonder what this does? Looks like there was something inserted here until just re..."
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
    else
        2 ebit? not if
            0 message   \ "Hmm, wonder what this device is for? Looks like it pivots at the base..."
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        world-frozen state-flag-set
        fiona-who $73 61.0 50.0 -90 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        5 message   \ "Rotate by pressing Right or Left on the left analog stick."
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                0 1 castle-1f-12.cmd00
                1 ebit? not if
                    0 6 54.0 -90.0 49.0 0 0 sound
                then
            else
                0 2 castle-1f-12.cmd00
                1 ebit? 0 5 pvar? and if
                    1 6 54.0 -90.0 49.0 0 0 sound
                    0 castle-1f-12.cmd03
                then
            then
            yield
        repeat
        5 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            ['] castle-1f-12.act01 goto
        then
    then then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act04 ( -- )   \ 003F1310
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    3 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    0 $F9 9 action
    $1E $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 castle-1f-12.cmd02
    2 0 $14 door-bits
    1 1 object-hidden
    2 0 object-hidden
    0 self-idle-anim
    fiona-who $73 63.3 50.28 -68 char-to-xz
    hewie-who 0 char-visible
    hewie-who action-end
    camera-restart
    4 item-use
    lunar-refractor-used story-flag-set
    $F $41 fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act05 ( -- )   \ 003F13E0
    1 self-scripted
    self-wait-done
    stalker-who char-here? if
        0 stalker-who 7 action
        yield
    then
    world-frozen state-flag-set
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $80 exit-taken? if
        $A room-preload
    else
        7 room-preload
    then
    wait-fade
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-taken? if
        world-held state-flag-set
        world-frozen state-flag-clear
        $81 exit-check
    else
        $22 door-unlock
        c1f-12-flag1 story-flag-set
        0 0 castle-1f-12.cmd00
        1 0 castle-1f-12.cmd00
        castle-1f-12.cmd01
        0 1 $20000 nav-group
        5 0 creature-count
        camera-restart
        force-calm state-flag-clear
        stalkers-stay state-flag-clear
        world-frozen state-flag-clear
        0 self-scripted
        $F $41 fade
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act06 ( -- )   \ 003F14C0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    c1f-12-flag1 story-flag? if
        1 message   \ "A device that refracts moonlight... I've never seen anything like this before."
        wait-message
    else
        2 ebit? not if
            0 message   \ "Hmm, wonder what this device is for? Looks like it pivots at the base..."
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        world-frozen state-flag-set
        fiona-who $BE 30.0 -44.0 180 char-to-xz
        1 13.0 0.0 0.0 0.0 event-camera
        5 message   \ "Rotate by pressing Right or Left on the left analog stick."
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                1 1 castle-1f-12.cmd00
                1 ebit? not if
                    0 6 30.0 -90.0 -51.0 0 0 sound
                then
            else
                1 2 castle-1f-12.cmd00
                1 ebit? 1 2 pvar? and if
                    1 6 30.0 -90.0 -51.0 0 0 sound
                    1 castle-1f-12.cmd03
                then
            then
            yield
        repeat
        5 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            ['] castle-1f-12.act01 goto
        then
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-12.act07 ( -- )   \ 0047A990
    self-wait-done
    begin
        yield
    again
;

: castle-1f-12.act08 ( -- )   \ 003F1590
    self-wait-done
    -73.5 55.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-12-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-12.act09 ( -- )   \ 003F15F0
    begin
        $64 cutscene-cue-reached? not while
        yield
    repeat
    begin
        cutscene-near-end? not while
        0 castle-1f-12.cmd02
        yield
    repeat
    self-idle-or-end
;

: castle-1f-12.act0A ( -- )   \ 003F1610
    self-wait-done
    2 1 $14 door-bits
    2 1 object-hidden
    3 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 9 action
    $1E $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 castle-1f-12.cmd02
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-12.act0B ( -- )   \ 003F16B0
    self-wait-done
    self-who 1 char-visible
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $37 room-preload
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-1f-12 register-scripts   \ (its phases and actions, by their names)
