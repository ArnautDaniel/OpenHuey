\ story/rooms/old-mansion-1f-10.fs - the event scripts of room old-mansion-1f-10 ($4D; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-10
USING: room-names story.words story.shared ;

: old-mansion-1f-10.char-enter ( -- )   \ 0040AC70
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
;

: old-mansion-1f-10.phase1 ( -- )   \ 0040AD70
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
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    0 -24.64 2.5 27.03 $14 6 0 zone
    0 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    0 ebit-set
                    $1E chance? if
                        hewie-zone-var 0 var-set
                        0 hewie-who $92 action
                    then
                then
            then
        then
    then
    0 -24.64 2.5 27.03 $14 6 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 6.0 hewie-look-zone
            then
        then
    then
    1 25.74 2.5 -26.39 $14 6 0 zone
    1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        hewie-zone-var 1 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    1 25.74 2.5 -26.39 $14 6 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 6.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-10.enter ( -- )   \ 0047AB90
    2 1.0 0 bgm
;

\ ---- registered ----
old-mansion-1f-10 register-scripts   \ (its phases and actions, by their names)
