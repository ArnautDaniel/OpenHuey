\ story/rooms/chaos-forest-3.fs - the event scripts of room chaos-forest-3 ($101; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-3
USING: room-names story.words story.shared ;

: chaos-forest-3.char-enter ( -- )   \ 00417320
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
;

: chaos-forest-3.phase1 ( -- )   \ 00417420
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
;

: chaos-forest-3.enter ( -- )   \ 0047AC58
    $19 1.0 0 bgm
;

\ ---- registered ----
' chaos-forest-3.char-enter chaos-forest-3 6 room-script!
' chaos-forest-3.phase1 chaos-forest-3 1 room-script!
' chaos-forest-3.enter chaos-forest-3 0 room-script!
