\ story/rooms/castle-2f-1.fs - the event scripts of room castle-2f-1 ($A; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-1
USING: room-names story.words story.shared flag-names story-names fiona.anims ;

\ the dial pstr_syuukouki on progress var 3
: castle-2f-1.cmd00 ( b0 -- )  s" syuukouki" 3 0 0 dial-step ;   \ (the dial on progress variable 3, from 0 degrees)
\ room 0x0A (Room0A_Cmd01_ptmf): an effect on its object at -2.88
: castle-2f-1.cmd01 ( -- )  s" syuukouki" -2.88e lens-flash ;   \ (the dial's lens catching the light)

: castle-2f-1.enter ( -- )   \ 003F3C90
    c1f-12-flag1 story-flag? not if
        room-sounds
    then
    got-lunar-refractor story-flag? if
        c2f-1-flag1 story-flag? not if
            c2f-1-flag1 story-flag-set
            $A 0 43 4 15 -1 0 0.0 creature-place
        then
    then
    c2f-1-refined-magnesia-found story-flag? c2f-1-refined-magnesia story-flag? not and if
        0 39.5 81.0 -38.8 flicker-sprite
    then
    c2f-1-lavender story-flag? not if
        1 41.5 69.0 31.0 flicker-sprite
    then
    0 castle-2f-1.cmd00
    1 $2300 sound-volume
;

: castle-2f-1.char-enter ( -- )   \ 003F3CF0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
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
    fiona-who self-is? if
        $80 exit-taken? $81 exit-taken? or if
            fiona-who $EB -41.573 -3.787 50 char-to-xz
            1 1 player-camera
            $80 exit-taken? if
                0 fiona-who 2 action
            else
                0 fiona-who 3 action
            then
        then
    then
;

: castle-2f-1.phase1 ( -- )   \ 003F3DA0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 2 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    c2f-1-refined-magnesia-found story-flag? not if
        0 39.5 80.0 -38.8 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var c2f-1-refined-magnesia-found var-set
                taken-flag-var c2f-1-refined-magnesia var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 39500 var-set
                sparkle-y-var 81000 var-set
                sparkle-z-var -38800 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    1 -35.43 90.0 3.42 $10 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    2 16.78 80.0 -16.27 $17 8 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 8.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-1.phase2 ( -- )   \ 003F3E90
    -2147483646 scene-request? if
        0 always offer-action
    then
    fiona-who 6 $32 char-faces-area? if
        1 unhurried offer-action
    then
    fiona-who 7 $32 char-faces-area? if
        5 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who 90 $32 char-heading? and if
        6 unhurried offer-action
    then
    c2f-1-refined-magnesia-found story-flag? c2f-1-refined-magnesia story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    c2f-1-lavender story-flag? not if
        3 1 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            8 calm offer-action
        then
    then
;

: castle-2f-1.phase3 ( -- )   \ 003F3EF0
    -3.0 80.0 3.0 -10.0 80.0 -10.0 -2.1 65.0 3.0 -10.0 65.0 -10.0 lights-doorway
    10.0 80.0 10.0 -3.0 80.0 3.0 10.0 65.0 10.0 -3.0 65.0 3.0 lights-doorway
    50.0 100.0 10.0 10.0 100.0 10.0 50.0 40.0 10.0 10.0 40.0 10.0 lights-doorway
    -10.0 80.0 -10.0 -10.0 80.0 -30.0 -10.0 30.0 -10.0 -10.0 30.0 -30.0 lights-doorway
    -26.0 120.0 8.0 -26.0 120.0 13.0 -26.0 90.0 8.0 -26.0 90.0 13.0 lights-doorway
;

: castle-2f-1.act00 ( -- )   \ 003F3FF0
    self-wait-done
    stalker-who self-touching? not if
        $15 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $15 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $15 door-unlock
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

: castle-2f-1.act02 ( -- )   \ 003F4110
    1 self-scripted
    self-wait-done
    $80 exit-taken? if
        stalker-who char-here? if
            0 stalker-who 7 action
            yield
        then
        world-frozen state-flag-set
    then
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    4 room-preload
    wait-fade
    world-frozen state-flag-set
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    world-frozen state-flag-clear
    3 ebit? if
        $80 exit-check
    else
        $81 exit-check
    then
    0 self-scripted
    self-idle-or-end
;

: castle-2f-1.act01 ( -- )   \ 003F4030
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    c1f-12-flag1 story-flag? if
        1 message   \ "A device that attracts moonlight? How bizarre."
        wait-message
    else
        2 ebit? not if
            0 message   \ "Huh? What is this? Looks like it pivots at the base..."
            wait-message
            2 ebit-set
            self-frames-reset
            8 self-wait-frames
        then
        world-frozen state-flag-set
        fiona-who $E6 -43.61 1.45 62 char-to-xz
        1 11.0 0.0 0.0 0.0 event-camera
        2 message   \ "Rotate by pressing Right or Left on the left analog stick."
        0 ebit-set
        1 ebit-set
        begin
            0 ebit? while
            1 ebit? if
                1 castle-2f-1.cmd00
                1 ebit? not if
                    0 6 -36.0 -102.0 5.0 0 0 sound
                then
            else
                2 castle-2f-1.cmd00
                1 ebit? 3 3 pvar? and if
                    1 6 -36.0 -102.0 5.0 0 0 sound
                    castle-2f-1.cmd01
                then
            then
            yield
        repeat
        fiona-who $EB -42.2 -4.35 52 char-to-xz
        2 message-close
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 5 pvar? 1 2 pvar? and 3 3 pvar? and if
            3 ebit-set
            force-calm state-flag-set
            $F $44 fade
            ['] castle-2f-1.act02 goto
        then
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-1.act03 ( -- )   \ 003F41C0
    1 self-scripted
    self-wait-done
    $22 door-unlock
    c1f-12-flag1 story-flag-set
    5 0 creature-count
    force-calm state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    camera-restart
    0 exit-prepare
    $F $41 fade
    self-idle-or-end
;

: castle-2f-1.act04 ( -- )   \ 003F41E0
    self-wait-done
    39.5 -38.8 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-1-refined-magnesia story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-1.act05 ( -- )   \ 003F4240
    self-wait-done
    18.0 -16.0 self-turn-to-xz
    self-wait-done
    4 message   \ "Tinkering out a few chords may soothe my nerves a bit... But now is hardly the time or..."
    wait-message
    self-idle-or-end
;

: castle-2f-1.act06 ( -- )   \ 003F4250
    self-wait-done
    180 self-turn-angle
    self-wait-done
    3 message   \ "Loads of speakers line the walls in a haphazard fashion. I can feel a piercing stare co..."
    wait-message
    self-idle-or-end
;

: castle-2f-1.act07 ( -- )   \ 0047A9C0
    self-wait-done
    begin
        yield
    again
;

: castle-2f-1.act08 ( -- )   \ 003F4260
    self-wait-done
    41.5 31.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-1-lavender story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-1.act09 ( -- )   \ 003F42C0
    self-wait-done
    self-who 1 char-visible
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    4 room-preload
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    self-who 0 char-visible
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

\ ---- registered ----
castle-2f-1 register-scripts   \ (its phases and actions, by their names)
