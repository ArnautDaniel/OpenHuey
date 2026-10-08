\ story/rooms/old-mansion-1f-10.fs - the event scripts of room old-mansion-1f-10 ($4D; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-10
USING: room-names story.words story.shared ;

: old-mansion-1f-10.char-enter ( -- )   \ 0040AC70
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
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
    0 -24.64 2.5 27.03 $14 6 0 zone
    0 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    0 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    0 -24.64 2.5 27.03 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 25.74 2.5 -26.39 $14 6 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 25.74 2.5 -26.39 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-10.enter ( -- )   \ 0047AB90
    2 1.0 0 bgm
;

\ ---- registered ----
' old-mansion-1f-10.char-enter old-mansion-1f-10 6 room-script!
' old-mansion-1f-10.phase1 old-mansion-1f-10 1 room-script!
' old-mansion-1f-10.enter old-mansion-1f-10 0 room-script!
