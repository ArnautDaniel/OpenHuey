\ story/rooms/off-map-0d.fs - the event scripts of room off-map-0d ($D; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-0d
USING: room-names story.words story.shared ;

: off-map-0d.char-enter ( -- )   \ 003F5490
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            5 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        5 -1 player-camera
    then then
    1 5 -1 area-camera
;

: off-map-0d.phase1 ( -- )   \ 003F54D0
    hewie-controlled? not if
        fiona-who 0 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 0 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    fiona-who $BB char-on-tri? fiona-who $1C7 char-on-tri? or if
        2 -1 player-camera
    then
    fiona-who $1D0 char-on-tri? fiona-who $E0 char-on-tri? or if
        4 -1 player-camera
    then
    fiona-who $1F1 char-on-tri? fiona-who $1ED char-on-tri? or fiona-who $EA char-on-tri? or if
        5 -1 player-camera
    then
;

: off-map-0d.enter ( -- )   \ 0047A9D0
    1 $3FFF sound-volume
;

: off-map-0d.act00 ( -- )   \ 0047A9D8
    self-idle-or-end
;

\ ---- registered ----
off-map-0d register-scripts   \ (its phases and actions, by their names)
