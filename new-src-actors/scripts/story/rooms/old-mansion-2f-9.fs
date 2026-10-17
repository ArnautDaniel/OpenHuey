\ story/rooms/old-mansion-2f-9.fs - the event scripts of room old-mansion-2f-9 ($5E; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-9
USING: room-names story.words story.shared story-names fiona.anims ;

: old-mansion-2f-9.enter ( -- )   \ 00412410
    got-sylvesta-esca story-flag? not if
        0 42.0 63.5 6.6 flicker-sprite
    then
;

: old-mansion-2f-9.char-enter ( -- )   \ 00412430
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 2 player-camera
    then
    1 2 2 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    hewie-who self-is? if
        2 hewie-mood
    then
    got-sylvesta-esca story-flag? not if
        0 42.0 63.5 6.6 flicker-sprite
    then
;

: old-mansion-2f-9.phase1 ( -- )   \ 00412510
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    0 43.86 50.0 6.98 $15 16 0 zone
    1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        hewie-zone-var 0 var-set
                        0 hewie-who $92 action
                    then
                then
            then
        then
    then
    0 43.86 50.0 6.98 $15 16 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 16.0 hewie-look-zone
            then
        then
    then
    1 -5.14 50.0 13.21 $44 29 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 29.43 hewie-look-zone
            then
        then
    then
;

: old-mansion-2f-9.phase2 ( -- )   \ 004125E0
    got-sylvesta-esca story-flag? not if
        2 0 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            0 calm offer-action
        then
    then
    fiona-who 7 $3C char-faces-area? if
        1 unhurried offer-action
    then
;

: old-mansion-2f-9.act00 ( -- )   \ 00412610
    self-wait-done
    42.0 6.6 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $65 message-param-room
        $65 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            got-sylvesta-esca story-flag-set
            0 effect-remove
            $65 1 item-give-count
            0 $65 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-9.act01 ( -- )   \ 00412670
    self-wait-done
    50.0 9.0 self-turn-to-xz
    self-wait-done
    look-at-anim self-anim
    $28 wait-frames
    0 say   \ "A stuffed chimera is showing off its fierce gaze. Why would anyone ever decorate a room..."
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-9 register-scripts   \ (its phases and actions, by their names)
