\ story/rooms/old-mansion-1f-17.fs - the event scripts of room old-mansion-1f-17 ($5D; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-17
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ room 0x5D (Room5D_Cmd00_ptmf): four objects 60 to the left
: old-mansion-1f-17.cmd00 ( -- )   \ (four objects - the room's strings 2..5 - 60 to the left)
    6 2 do  i room-object 2dup object-here? if  -60e place-x object+!  else  2drop  then  loop ;
\ room 0x5D (Room5D_Cmd01_ptmf): the lever at -60 / 0 / 60 degrees by byte 3
: old-mansion-1f-17.cmd01 ( b0 -- )   \ (the lever, "doll": 0 -60 degrees, 2 60, else 0)
    case  0 of  -60e  endof  2 of  60e  endof  >r 0e r>  endcase  deg>rad s" doll" turn-y object! ;

: old-mansion-1f-17.enter ( -- )   \ 00411B80
    room-sounds
    $5A door-not-closed-off? not if
        4 part-shown
        3 solid-floor floor-on
    then
    om2f-8-flag1 story-flag? not if
        0 barred-floor floor-on
    else
        old-mansion-1f-17.cmd00
    then
    om1f-17-breakable1-broken story-flag? not if
        1 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        1 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        om1f-17-magnesia story-flag? not if
            1 43.0 1.0 -40.0 flicker-sprite
        then
    then
    om1f-17-breakable2-broken story-flag? not if
        2 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        2 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
    then
    om1f-17-chicken-jerky story-flag? not if
        0 -137.37 -41.0 -14.57 flicker-sprite
    then
    om1f-17-medallion-found story-flag? om1f-17-medallion story-flag? not and if
        2 68.99 1.0 19.99 flicker-sprite
    then
    0 8 0.562 0.5 0.25 0.5 zone-rect
    1 8 0.812 0.687 0.187 0.312 zone-rect
    3 -84.0 -19.0 13.7 1 flame-in
    1 $2300 sound-volume
;

: old-mansion-1f-17.char-enter ( -- )   \ 00411C60
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
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        1 1 player-camera
    then then
    3 1 1 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $12C char-to-tri
            1 -1 player-camera
            0 fiona-who 0 action
        then
        0 exit-taken? if
            0 old-mansion-1f-17.cmd01
        else 3 exit-taken? if
            1 old-mansion-1f-17.cmd01
        else 1 exit-taken? if
            2 old-mansion-1f-17.cmd01
        else 2 exit-taken? if
            2 old-mansion-1f-17.cmd01
        then then then then
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-17.phase1 ( -- )   \ 00411D60
    $5A door-not-closed-off? if
        0 exit-usable? if
            0 exit-check
        then
    then
    1 exit-usable? if
        1 exit-check
    then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    $A 0 0 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    fiona-who 4 char-entered-area? fiona-who 6 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        3 exit-prepare
    then
    fiona-who $11 char-entered-area? if
        2 map-page
    then
    fiona-who $11 char-left-area? if
        1 map-page
    then
    om1f-17-breakable2-broken story-flag? not if
        1 -138.5 -50.0 -7.5 5 8 1 zone
        self-who 1 char-in-zone? if
            om1f-17-breakable2-broken story-flag-set
            2 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            -138.5 -50.0 -7.5 1 -2144851928 0 0.0 shards-at
            $88 5 -138.5 -50.0 -7.5 0 0 sound
            $40 5 noise
        then
    then
    om1f-17-medallion-found story-flag? not if
        7 68.99 0.0 19.99 $A 5 0 zone
        hewie-who 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-17-medallion-found var-set
                taken-flag-var om1f-17-medallion var-set
                sparkle-fx-var 2 var-set
                sparkle-x-var 68990 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 19990 var-set
                hewie-zone-var 7 var-set
                0 hewie-who $8B action
            then
        then
    then
    om2f-8-flag1 story-flag? not if
        2 -15.99 0.0 0.03 $19 22 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    hewie-zone-var 2 var-set
                    hewie-zone-var 22.0 hewie-look-zone
                then
            then
        then
    then
    3 54.73 0.0 -58.43 $13 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 22.0 hewie-look-zone
            then
        then
    then
    4 68.2 0.0 31.14 $14 12 0 zone
    1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    1 ebit-set
                    $32 chance? if
                        hewie-zone-var 4 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    4 68.2 0.0 31.14 $14 12 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    5 -125.41 -50.0 1.03 $14 14 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                hewie-zone-var 5 var-set
                hewie-zone-var 14.0 hewie-look-zone
            then
        then
    then
    8 3 8 -4 0 zone-at-effect
    fiona-who 8 3 char-zone-bits? fiona-who 8 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 8 3 char-zone-bits? hewie-who 8 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 8 3 char-zone-bits? stalker-who 8 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
;

: old-mansion-1f-17.phase2 ( -- )   \ 00411FB0
    om2f-8-flag1 story-flag? not if
        fiona-who $E char-in-area? fiona-who -45 $32 char-heading? and if
            1 unhurried offer-action
        then
    then
    fiona-who $10 char-in-area? fiona-who 45 $3C char-heading? and if
        5 unhurried offer-action
    then
    om1f-17-chicken-jerky story-flag? not if
        6 0 5 5 0 zone-at-effect
        fiona-who 6 3 char-zone-bits? if
            2 calm offer-action
        then
    then
    om1f-17-medallion-found story-flag? om1f-17-medallion story-flag? not and if
        7 2 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    om1f-17-breakable1-broken story-flag? not if
        0 43.0 0.0 -40.0 5 8 1 zone
        self-who 0 char-in-zone? if
            om1f-17-breakable1-broken story-flag-set
            1 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            43.0 0.0 -40.0 0 -2145904616 0 0.0 shards-at
            3 6 43.0 0.0 -40.0 0 0 sound
            $40 $126 noise
            1 43.0 1.0 -40.0 flicker-sprite
        then
    else om1f-17-magnesia story-flag? not if
        0 1 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then then
;

: old-mansion-1f-17.act00 ( -- )   \ 00412090
    1 self-scripted
    self-wait-done
    world-frozen state-flag-set
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $14 $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    world-held state-flag-set
    world-frozen state-flag-clear
    $80 exit-check
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-17.act01 ( -- )   \ 00412120
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    0 ebit? not if
        0 message   \ "'Blue Flame, Secret Ways' Strange title for a picture. Unless it could be some sort..."
        wait-message
        0 ebit-set
    else
        1 message   \ "A female is raising an oil lamp. A blue flame burns brightly from its tip. It's alm..."
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-17.act02 ( -- )   \ 00412140
    self-wait-done
    -137.37 -14.57 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-17-chicken-jerky story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-17.act03 ( -- )   \ 004121A0
    self-wait-done
    43.0 -40.0 self-turn-to-xz
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
            om1f-17-magnesia story-flag-set
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

: old-mansion-1f-17.act04 ( -- )   \ 00412200
    self-wait-done
    68.99 19.99 self-turn-to-xz
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
            om1f-17-medallion story-flag-set
            2 effect-remove
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

: old-mansion-1f-17.act05 ( -- )   \ 00412260
    self-wait-done
    $137 57.06 30.88 90 $FFFF 5 self-move-to
    self-wait-done
    1 self-scripted
    mummy-examined story-flag? not if
        fiona-who seized-sound 5 char-sound
        pull-free-anim $A self-anim-blend
        $5A threat-add
        1 $FF 8 rumble
        self-wait-anim
        world-frozen state-flag-set
        1 5.0 10.0 0.0 0.0 event-camera
        mummy-examined story-flag-set
        2 message   \ "This... isn't a doll! It's a mummy!"
        wait-message
    else
        world-frozen state-flag-set
        1 5.0 10.0 0.0 0.0 event-camera
        3 message   \ "Why in the world is this here!?"
        wait-message
    then
    self-frames-reset
    self-wait-16
    0 self-scripted
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: old-mansion-1f-17.act06 ( -- )   \ 004122F0
    self-wait-done
    3 -84.0 -19.0 13.7 1 flame-in
    self-who 1 char-visible
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $14 $C8 movie-param
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
old-mansion-1f-17 register-scripts   \ (its phases and actions, by their names)
