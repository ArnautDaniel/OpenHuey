\ story/rooms/off-map-c4.fs - the event scripts of room off-map-c4 ($C4; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-c4
USING: room-names story.words story.shared ;

: off-map-c4.char-enter ( -- )   \ 00440100
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        5 5 player-camera
    then
    1 5 5 area-camera
;

: off-map-c4.phase1 ( -- )   \ 00440140
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    2 0 0 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 1 1 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 3 3 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 4 4 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 5 5 1 chars-area-camera
    $C 4 4 1 chars-area-camera
    $D 5 5 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 6 6 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
;

: off-map-c4.enter ( -- )   \ 0047AF0C
;

: off-map-c4.act00 ( -- )   \ 0047AF10
    self-idle-or-end
;

\ ---- registered ----
off-map-c4 register-scripts   \ (its phases and actions, by their names)
