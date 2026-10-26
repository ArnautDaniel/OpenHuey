\ story/rooms/lake-pathway-1.fs - the event scripts of room lake-pathway-1 ($C8; Lake Pathway).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.lake-pathway-1
USING: room-names facts story.words story.shared story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant body-remarked               \ (bit 0) its first remark made: "His limbs are twisted into unnatural positions..."
1 constant collapsed-path-remarked     \ (bit 1) its first remark made: "The path has collapsed... So, all of this was on top of a la"
2 constant hewie-zone1-noticed         \ (bit 2) Hewie has reacted to zone 1 (once a visit)

\ an effect TurningModel_vtable (8 bytes), not started
: lake-pathway-1.cmd00 ( -- )  0 $40404040  400e 27.1e -125e  1000e 0.5e 0e  effects:caustic drop ;   \ (the water's light, turning: TurningModel)

: lake-pathway-1.enter ( -- )   \ 00441520
    $18 1.0 0 bgm
    c7-second-event story-flag? not over-music-d story-flag? or if
        0 barred-floor hole-floor or floor-on
    else
        0 part-shown
    then
    lp-1-lavender story-flag? not if
        0 19.59 39.0 -247.61 flicker-sprite
    then
    lake-pathway-1.cmd00
;

: lake-pathway-1.char-enter ( -- )   \ 00441560
    c7-second-event story-flag? not over-music-d story-flag? or if
        player-who self-is? 0 exit-taken? and if
            player-who 0 char-to-exit
            1 1 player-camera
        then
        0 1 1 area-camera
    else
        player-who self-is? 0 exit-taken? and if
            player-who 0 char-to-exit
            0 0 player-camera
        then
        0 0 0 area-camera
    then
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
;

: lake-pathway-1.phase1 ( -- )   \ 00441620
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    c7-second-event story-flag? not if
        1 -4.8 38.0 -171.89 $35 15 0 zone
        hewie-zone1-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        hewie-zone1-noticed ebit-set
                        $1E chance? if
                            hewie-zone-var 1 var-set
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
    body-remarked ebit? not if
        fiona-who 4 char-in-area? if
            0 show-message   \ "His limbs are twisted into unnatural positions. It looks so painful... But I gu..."
            look-low-anim play-out
        else
            180 self-turn-angle
            self-wait-done
            1 show-message   \ "No one could withstand a fall like that. No one."
            look-at-anim play-out
        then
        -1 self-idle-anim
        7 wait-frames
        wait-message
        body-remarked ebit-set
    else
        pick-up-anim play-out
        2 say   \ "His skin... It's so dry and leathery. This body... There is something that just doe..."
        up-from-pick-anim play-out
        body-remarked ebit-clear
    then
    self-idle-or-end
;

: lake-pathway-1.act01 ( -- )   \ 00441730
    self-wait-done
    collapsed-path-remarked ebit? not if
        collapsed-path-remarked ebit-set
        3 say   \ "The path has collapsed... So, all of this was on top of a lake."
    else
        collapsed-path-remarked ebit-clear
        look-close-anim self-anim
        $28 wait-frames
        4 say   \ "There's a light coming from the other side of the lake. I've got to find a way to g..."
        self-wait-anim
    then
    self-idle-or-end
;

: lake-pathway-1.act02 ( -- )   \ 00441750
    self-wait-done
    19.59 -247.61 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            lp-1-lavender story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
lake-pathway-1 register-scripts   \ (its phases and actions, by their names)
