\ story/rooms/old-mansion-2f-10.fs - the event scripts of room old-mansion-2f-10 ($5F; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-10
USING: room-names story.words story.shared story-names floor-flags fiona.anims ;

: old-mansion-2f-10.enter ( -- )   \ 0041B130
    om2f-10-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        om2f-10-weeping-stone story-flag? not if
            0 -10.5 1.0 -0.4 flicker-sprite
        then
    then
    0 2 0.812 0.687 0.187 0.312 zone-rect
    om1f-12-flag1 story-flag? if
        om2f-10-flag1 story-flag? not if
            om2f-10-flag1 story-flag-set
            $5F 0 259 4 8 -1 0 0.0 creature-place
        then
    then
    1 12.4 17.3 -45.0 1 flame-in
    1 $2300 sound-volume
;

: old-mansion-2f-10.char-enter ( -- )   \ 0041B1B0
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
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
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
;

: old-mansion-2f-10.phase1 ( -- )   \ 0041B270
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        2 exit-prepare
    then
    1 5.8 0.0 15.45 $F 13 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    2 -65.87 0.0 11.35 $B 15 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    3 -66.99 0.0 -11.47 $B 15 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    6 1 8 -4 0 zone-at-effect
    fiona-who 6 3 char-zone-bits? fiona-who 6 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 6 3 char-zone-bits? hewie-who 6 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 6 3 char-zone-bits? stalker-who 6 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
;

: old-mansion-2f-10.phase2 ( -- )   \ 0041B370
    om2f-10-breakable-broken story-flag? not if
        0 -10.5 0.0 -0.4 5 8 1 zone
        self-who 0 char-in-zone? if
            om2f-10-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -10.5 0.0 -0.4 0 -2143272896 0 0.0 shards-at
            $88 5 -10.5 0.0 -0.4 0 0 sound
            $40 $7B noise
            0 -10.5 1.0 -0.4 flicker-sprite
        then
    else om2f-10-weeping-stone story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            0 calm offer-action
        then
    then then
    4 -67.5 0.0 -11.5 8 10 0 zone
    fiona-who 4 8 char-zone-bits? if
        fiona-who -67 -11 $3C char-faces-xz? if
            1 unhurried offer-action
        then
    then
    5 -67.5 0.0 11.5 8 10 0 zone
    fiona-who 5 8 char-zone-bits? if
        fiona-who -67 11 $3C char-faces-xz? if
            1 unhurried offer-action
        then
    then
;

: old-mansion-2f-10.phase3 ( -- )   \ 0041B450
    -13.7 15.0 -28.0 -13.7 15.0 -11.3 -13.7 0.0 -28.0 -13.7 0.0 -11.3 lights-doorway
    -13.7 15.0 14.0 -13.7 15.0 30.7 -13.7 0.0 14.0 -13.7 0.0 30.7 lights-doorway
;

: old-mansion-2f-10.act00 ( -- )   \ 0041B4C0
    self-wait-done
    -10.5 -0.4 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $9B message-param-room
        $9B $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-10-weeping-stone story-flag-set
            0 effect-remove
            $9B 1 item-give-count
            0 $9B item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-10.act01 ( -- )   \ 0041B520
    self-wait-done
    fiona-who 4 8 char-zone-bits? if
        -67.5 -11.5 self-turn-to-xz
        self-wait-done
    else
        -67.5 11.5 self-turn-to-xz
        self-wait-done
    then
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 ebit-set
        0 message   \ "A bronze statue with a cow-like head. My Greek mythology is kind of rusty, but I th..."
        wait-message
    else
        1 message   \ "It looks like it could come to life at any second."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-10 register-scripts   \ (its phases and actions, by their names)
