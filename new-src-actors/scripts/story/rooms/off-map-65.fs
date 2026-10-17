\ story/rooms/off-map-65.fs - the event scripts of room off-map-65 ($65; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-65
USING: room-names story.words story.shared ;

: off-map-65.char-enter ( -- )   \ 00429640
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
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
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 -1 player-camera
    then then
    2 0 -1 area-camera
;

: off-map-65.phase1 ( -- )   \ 00429700
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        2 exit-prepare
    then
;

: off-map-65.enter ( -- )   \ 0047AD50
;

\ ---- registered ----
off-map-65 register-scripts   \ (its phases and actions, by their names)
