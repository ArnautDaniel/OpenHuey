\ story/rooms/old-mansion-2f-7.fs - the event scripts of room old-mansion-2f-7 ($5B; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-7
USING: room-names story.words story.shared ;

: old-mansion-2f-7.enter ( -- )   \ 0041B020
    $17 1.0 0 bgm
    0 -12.3 17.6 125.0 1 flame-in
    1 -12.3 27.6 70.0 1 flame-in
    2 -12.3 27.6 10.0 1 flame-in
    3 -12.3 27.6 -50.0 1 flame-in
    4 -12.3 17.7 -106.0 1 flame-in
;

: old-mansion-2f-7.char-enter ( -- )   \ 0041B080
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
;

: old-mansion-2f-7.phase1 ( -- )   \ 0041B100
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
;

\ ---- registered ----
old-mansion-2f-7 register-scripts   \ (its phases and actions, by their names)
