\ story/rooms/water-tower-7f-2.fs - the event scripts of room water-tower-7f-2 ($C6; Water Tower: 7F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-7f-2
USING: room-names story.words story.shared flag-names story-names fiona.anims ;

: water-tower-7f-2.enter ( -- )   \ 00441170
    room-sounds
    hunted state-flag-set
    $30 1.0 0 bgm
    wt7f-2-rubrum-medallion-found story-flag? wt7f-2-rubrum-medallion story-flag? not and if
        0 -9.0 140.97 -10.23 flicker-sprite
    then
;

: water-tower-7f-2.char-enter ( -- )   \ 004411A0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            3 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        3 1 player-camera
    then then
    0 3 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 -1 player-camera
    then then
    1 0 -1 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $1C2 -45.44 -52.11 105 char-to-xz
            1 -1 player-camera
            0.0 sound-volume-scale
            0 fiona-who 0 action
        then
        $81 exit-taken? if
            fiona-who char-activate
            fiona-who 0 char-to-exit
            3 1 player-camera
            hewie-who char-activate
            $C6 0 836 hewie-to-room
            hewie-who $344 6.81 -30.23 0 char-to-xz
            0.0 sound-volume-scale
            0 fiona-who 1 action
        then
        0 exit-taken? if
            8 map-page
        then
    then
    hewie-who self-is? if
        $80 exit-taken? if
            hewie-who $6D -14.34 -72.11 -30 char-to-xz
        then
    then
;

: water-tower-7f-2.phase1 ( -- )   \ 004412A0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 1 -1 1 chars-area-camera
    8 1 -1 1 chars-area-camera
    9 2 0 1 chars-area-camera
    $A 2 0 1 chars-area-camera
    $B 3 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who $F char-entered-area? if
        7 map-page
    then
    fiona-who $F char-left-area? if
        8 map-page
    then
    wt7f-2-rubrum-medallion-found story-flag? not if
        0 -9.0 139.97 -10.23 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var wt7f-2-rubrum-medallion-found var-set
                taken-flag-var wt7f-2-rubrum-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var -9000 var-set
                sparkle-y-var 140970 var-set
                sparkle-z-var -10230 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -127.0 1.7 10.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -127.0 1.7 10.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -127.0 1.7 10.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -127.0 1.7 10.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: water-tower-7f-2.phase2 ( -- )   \ 00441400
    fiona-who $C $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    fiona-who $E $32 char-faces-area? if
        3 unhurried offer-action
    then
    wt7f-2-rubrum-medallion-found story-flag? wt7f-2-rubrum-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            2 calm offer-action
        then
    then
;

: water-tower-7f-2.act00 ( -- )   \ 00441430
    yield
    camera-restart
    1 exit-prepare
    self-frames-reset
    self-wait-16
    world-held state-flag-clear
    $F $51 fade
    $828A item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-2.act01 ( -- )   \ 00441450
    world-held state-flag-clear
    $F $51 fade
    wait-fade
    stalkers-stay state-flag-clear
    self-who 0 char-visible
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-2.act02 ( -- )   \ 00441470
    self-wait-done
    -9.0 -10.23 self-turn-to-xz
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
            wt7f-2-rubrum-medallion story-flag-set
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

: water-tower-7f-2.act03 ( -- )   \ 004414D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0.0 -105.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    $F 6 fade
    wait-fade
    0 message   \ "All life on the surface of the planet stretches out below the stars' radiant light. All..."
    wait-message
    $F 7 fade
    wait-fade
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-2.phase5 ( -- )   \ 0047AF30
    hunted state-flag-clear
;

\ ---- registered ----
water-tower-7f-2 register-scripts   \ (its phases and actions, by their names)
