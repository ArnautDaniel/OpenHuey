\ story/rooms/lake-pathway-1.fs - the event scripts of room lake-pathway-1 ($C8; Lake Pathway).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.lake-pathway-1
USING: room-names story.words story.shared story-names ;

\ an effect TurningModel_vtable (8 bytes), not started
: lake-pathway-1.cmd00 ( -- )  0 $40404040  400e 27.1e -125e  1000e 0.5e 0e  effects:caustic drop ;   \ (the water's light, turning: TurningModel)

: lake-pathway-1.enter ( -- )   \ 00441520
    $18 1.0 0 bgm
    c7-second-event story-flag? not over-music-d story-flag? or if
        1 0 $10020000 nav-group
    else
        0 1 $14 door-bits
    then
    lp-1-lavender story-flag? not if
        0 19.59 39.0 -247.61 flicker-sprite
    then
    lake-pathway-1.cmd00
;

: lake-pathway-1.char-enter ( -- )   \ 00441560
    c7-second-event story-flag? not over-music-d story-flag? or if
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
    else
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
    then
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
;

: lake-pathway-1.phase1 ( -- )   \ 00441620
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            0 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    c7-second-event story-flag? not if
        1 -4.8 38.0 -171.89 $35 15 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        2 ebit-set
                        $1E chance? if
                            $1F 1 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
;

: lake-pathway-1.phase2 ( -- )   \ 004416A0
    c7-second-event story-flag? if
        fiona-who 4 char-in-area? fiona-who 0 $32 char-heading? and if
            0 unhurried offer-action
        then
        fiona-who 5 char-in-area? fiona-who 90 $32 char-heading? and if
            0 unhurried offer-action
        then
    else fiona-who 6 char-in-area? fiona-who 0 $32 char-heading? and if
        1 unhurried offer-action
    then then
    lp-1-lavender story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            2 calm offer-action
        then
    then
;

: lake-pathway-1.act00 ( -- )   \ 004416F0
    self-wait-done
    0 ebit? not if
        fiona-who 4 char-in-area? if
            0 message   \ "His limbs are twisted into unnatural positions. It looks so painful... But I gu..."
            $A01 self-anim
            self-wait-anim
        else
            180 self-turn-angle
            self-wait-done
            1 message   \ "No one could withstand a fall like that. No one."
            $A00 self-anim
            self-wait-anim
        then
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        wait-message
        0 ebit-set
    else
        $900 self-anim
        self-wait-anim
        2 message   \ "His skin... It's so dry and leathery. This body... There is something that just doe..."
        wait-message
        $901 self-anim
        self-wait-anim
        0 ebit-clear
    then
    self-idle-or-end
;

: lake-pathway-1.act01 ( -- )   \ 00441730
    self-wait-done
    1 ebit? not if
        1 ebit-set
        3 message   \ "The path has collapsed... So, all of this was on top of a lake."
        wait-message
    else
        1 ebit-clear
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        4 message   \ "There's a light coming from the other side of the lake. I've got to find a way to g..."
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: lake-pathway-1.act02 ( -- )   \ 00441750
    self-wait-done
    19.59 -247.61 self-turn-to-xz
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
            lp-1-lavender story-flag-set
            0 effect-remove
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

\ ---- registered ----
lake-pathway-1 register-scripts   \ (its phases and actions, by their names)
