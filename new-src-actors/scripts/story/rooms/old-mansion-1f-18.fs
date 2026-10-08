\ story/rooms/old-mansion-1f-18.fs - the event scripts of room old-mansion-1f-18 ($68; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-18
USING: room-names story.words story.shared ;

: old-mansion-1f-18.enter ( -- )   \ 00424880
    3 1.0 0 bgm
    0 exit-taken? if
        $11 3 $FF char-load
    then
;

: old-mansion-1f-18.char-enter ( -- )   \ 00424890
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
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
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        $76 story-flag? not if
            3 stalker-kind? $22 stalker-kind? or if
            then
        then
        2 exit-prepare
    then
    0 7 char-entered-area? if
        $76 story-flag? not if
            3 stalker-kind? $22 stalker-kind? or if
                $FE 0 char-file-load
            then
        then
        3 exit-prepare
    then
    0 -26.02 2.5 -26.23 $14 6 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 0 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    0 -26.02 2.5 -26.23 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    1 26.56 2.5 25.45 $14 6 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    2 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 26.56 2.5 25.45 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-18.phase2 ( -- )   \ 00424AC0
    0 8 char-in-area? 0 8 $3C char-faces-area? and if
        5 0 0 scene-change
    then
    0 9 char-in-area? 0 9 $3C char-faces-area? and if
        5 0 0 scene-change
    then
    0 $A char-in-area? 0 $A $3C char-faces-area? and if
        5 0 0 scene-change
    then
;

: old-mansion-1f-18.phase4 ( -- )   \ 00424AF0
    0 0 char-in-area? if
        3 0 char-remove
    then
;

: old-mansion-1f-18.act00 ( -- )   \ 00424B00
    self-wait-done
    0 8 char-in-area? if
        6.92 -26.68 self-turn-to-xz
        self-wait-done
    else 0 9 char-in-area? if
        -20.57 -19.02 self-turn-to-xz
        self-wait-done
    else 0 $A char-in-area? if
        -26.62 7.92 self-turn-to-xz
        self-wait-done
    then then then
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $332 story-flag? not if
        $332 story-flag-set
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-18.enter old-mansion-1f-18 0 room-script!
' old-mansion-1f-18.char-enter old-mansion-1f-18 6 room-script!
' old-mansion-1f-18.phase1 old-mansion-1f-18 1 room-script!
' old-mansion-1f-18.phase2 old-mansion-1f-18 2 room-script!
' old-mansion-1f-18.phase4 old-mansion-1f-18 4 room-script!
' old-mansion-1f-18.act00 old-mansion-1f-18 $00 action-script!
