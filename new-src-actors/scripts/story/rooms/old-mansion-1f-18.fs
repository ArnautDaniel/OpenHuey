\ story/rooms/old-mansion-1f-18.fs - the event scripts of room old-mansion-1f-18 ($68; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-18
USING: room-names story.words story.shared story-names ;

: old-mansion-1f-18.enter ( -- )   \ 00424880
    3 1.0 0 bgm
    0 exit-taken? if
        $11 3 $FF char-load
    then
;

: old-mansion-1f-18.char-enter ( -- )   \ 00424890
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
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
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
;

: old-mansion-1f-18.phase1 ( -- )   \ 00424990
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        white-mandragora-used story-flag? not if
            3 stalker-kind? $22 stalker-kind? or if
            then
        then
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        white-mandragora-used story-flag? not if
            3 stalker-kind? $22 stalker-kind? or if
                stalker-who 0 char-file-load
            then
        then
        3 exit-prepare
    then
    0 -26.02 2.5 -26.23 $14 6 0 zone
    1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 hewie-who $92 action
                    then
                then
            then
        then
    then
    0 -26.02 2.5 -26.23 $14 6 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 26.56 2.5 25.45 $14 6 0 zone
    2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    2 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    1 26.56 2.5 25.45 $14 6 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-18.phase2 ( -- )   \ 00424AC0
    fiona-who 8 char-in-area? fiona-who 8 $3C char-faces-area? and if
        0 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who 9 $3C char-faces-area? and if
        0 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who $A $3C char-faces-area? and if
        0 unhurried offer-action
    then
;

: old-mansion-1f-18.phase4 ( -- )   \ 00424AF0
    fiona-who 0 char-in-area? if
        3 0 char-remove
    then
;

: old-mansion-1f-18.act00 ( -- )   \ 00424B00
    self-wait-done
    fiona-who 8 char-in-area? if
        6.92 -26.68 self-turn-to-xz
        self-wait-done
    else fiona-who 9 char-in-area? if
        -20.57 -19.02 self-turn-to-xz
        self-wait-done
    else fiona-who $A char-in-area? if
        -26.62 7.92 self-turn-to-xz
        self-wait-done
    then then then
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    red-painting-examined story-flag? not if
        red-painting-examined story-flag-set
        0 message   \ "Why would someone intentionally paint this completely red?"
        wait-message
    else
        1 message   \ "Something smells... like something living. Certainly that is really just red paint..."
        wait-message
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-18 register-scripts   \ (its phases and actions, by their names)
