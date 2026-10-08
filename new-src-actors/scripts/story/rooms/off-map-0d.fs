\ story/rooms/off-map-0d.fs - the event scripts of room off-map-0d ($D; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-0d
USING: room-names story.words story.shared ;

: off-map-0d.char-enter ( -- )   \ 003F5490
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 5 -1 char-camera
                0 camera-follow
            else
                1 5 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then then
    1 5 -1 area-camera
;

: off-map-0d.phase1 ( -- )   \ 003F54D0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    0 $BB char-on-tri? 0 $1C7 char-on-tri? or if
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then
    0 $1D0 char-on-tri? 0 $E0 char-on-tri? or if
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then
    0 $1F1 char-on-tri? 0 $1ED char-on-tri? or 0 $EA char-on-tri? or if
        hewie-controlled? not if
            0 5 -1 char-camera
            0 camera-follow
        else
            1 5 -1 char-camera
            1 camera-follow
        then
    then
;

: off-map-0d.enter ( -- )   \ 0047A9D0
    1 $3FFF sound-volume
;

: off-map-0d.act00 ( -- )   \ 0047A9D8
    self-idle-or-end
;

\ ---- registered ----
' off-map-0d.char-enter off-map-0d 6 room-script!
' off-map-0d.phase1 off-map-0d 1 room-script!
' off-map-0d.enter off-map-0d 0 room-script!
' off-map-0d.act00 off-map-0d $00 action-script!
