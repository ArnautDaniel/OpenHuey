\ story/rooms/old-mansion-1f-21.fs - the event scripts of room old-mansion-1f-21 ($6D; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-21
USING: room-names story.words story.shared story-names floor-flags fiona.anims ;

: old-mansion-1f-21.enter ( -- )   \ 00439B10
    om1f-21-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        om1f-21-album-medallion story-flag? not if
            0 20.0 1.0 -72.5 flicker-sprite
        then
    then
    om1f-21-breakable2-broken story-flag? not if
        3 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        3 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
    then
    om1f-21-breakable3-broken story-flag? not if
        2 keeps-people-off floor-on
        4 part-shown
        5 part-hidden
    else
        2 keeps-people-off floor-off
        4 part-hidden
        5 part-shown
    then
    om1f-21-breakable4-broken story-flag? not if
        1 keeps-people-off floor-on
        6 part-shown
        7 part-hidden
    else
        1 keeps-people-off floor-off
        6 part-hidden
        7 part-shown
    then
    0 6 0.345 0.687 0.187 0.312 zone-rect
    om3f-1-flag2 story-flag? if
        om1f-21-flag1 story-flag? not if
            om1f-21-flag1 story-flag-set
            $6D 0 65 $80 3 -1 $FF80 44.0 creature-place
            $6D 0 36 $80 3 -1 $81 44.0 creature-place
        then
    then
    1 $2300 sound-volume
;

: old-mansion-1f-21.char-enter ( -- )   \ 00439C00
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 1 player-camera
    then
    0 2 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        3 -1 player-camera
    then
    1 3 -1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 -1 player-camera
    then
    2 0 -1 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        1 0 player-camera
    then
    3 1 0 area-camera
    fiona-who self-is? if
        3 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-21.phase1 ( -- )   \ 00439D00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    $A 0 -1 1 chars-area-camera
    $B 1 0 1 chars-area-camera
    $C 0 -1 1 chars-area-camera
    $D 2 1 1 chars-area-camera
    $E 2 1 1 chars-area-camera
    $F 3 -1 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 7 char-entered-area? or fiona-who 8 char-entered-area? or if
        2 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        2 map-page
    then
    fiona-who 9 char-left-area? if
        1 map-page
    then
    fiona-who 6 char-entered-area? if
        $1C 3 1 char-load
    then
    fiona-who 7 char-entered-area? if
        3 0 char-remove
    then
    fiona-who 4 char-entered-area? if
        $1C 3 0 char-load
    then
    fiona-who 5 char-entered-area? if
        3 0 char-remove
    then
    1 19.93 0.0 -73.33 $D 10 0 zone
    6 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        hewie-zone-var 1 var-set
                        0 hewie-who $89 action
                    then
                then
            then
        then
    then
    om1f-21-breakable2-broken story-flag? not if
        2 6.4 0.0 -132.4 5 8 1 zone
        self-who 2 char-in-zone? if
            om1f-21-breakable2-broken story-flag-set
            3 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            6.4 0.0 -132.4 0 -2145378272 0 0.0 shards-at
            $88 5 6.4 0.0 -132.4 0 0 sound
            $40 $100 noise
        then
    then
    om1f-21-breakable3-broken story-flag? not if
        3 -63.9 0.0 -147.4 5 8 1 zone
        self-who 3 char-in-zone? if
            om1f-21-breakable3-broken story-flag-set
            2 keeps-people-off floor-off
            4 part-hidden
            5 part-shown
            -63.9 0.0 -147.4 0 -2145378272 0 0.0 shards-at
            $88 5 -63.9 0.0 -147.4 0 0 sound
            $40 $112 noise
        then
    then
    om1f-21-breakable4-broken story-flag? not if
        4 -68.8 0.0 -156.4 5 8 1 zone
        self-who 4 char-in-zone? if
            om1f-21-breakable4-broken story-flag-set
            1 keeps-people-off floor-off
            6 part-hidden
            7 part-shown
            -68.8 0.0 -156.4 0 -2145378272 0 0.0 shards-at
            $88 5 -68.8 0.0 -156.4 0 0 sound
            $40 $115 noise
        then
    then
;

: old-mansion-1f-21.phase2 ( -- )   \ 00439F10
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            0 always offer-action
        then
    then
    om1f-21-breakable1-broken story-flag? not if
        0 20.0 0.0 -72.5 5 8 1 zone
        self-who 0 char-in-zone? if
            om1f-21-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            20.0 0.0 -72.5 0 -2145378272 0 0.0 shards-at
            $88 5 20.0 0.0 -72.5 0 0 sound
            $40 $35 noise
            0 20.0 1.0 -72.5 flicker-sprite
        then
    else om1f-21-album-medallion story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            1 calm offer-action
        then
    then then
;

: old-mansion-1f-21.phase3 ( -- )   \ 00439FB0
    38.0 10.0 -99.0 68.0 30.0 -99.0 38.0 0.0 -99.0 68.0 0.0 -99.0 lights-doorway
    68.0 30.0 -99.0 90.0 50.0 -99.0 68.0 0.0 -99.0 90.0 0.0 -99.0 lights-doorway
;

: old-mansion-1f-21.act00 ( -- )   \ 0043A020
    self-wait-done
    stalker-who self-touching? not if
        $44 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $44 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                $14 wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $44 door-unlock
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

: old-mansion-1f-21.act01 ( -- )   \ 0043A060
    self-wait-done
    20.0 -72.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-21-album-medallion story-flag-set
            0 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-21 register-scripts   \ (its phases and actions, by their names)
