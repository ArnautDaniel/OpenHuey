\ story/rooms/old-mansion-2f-9.fs - the event scripts of room old-mansion-2f-9 ($5E; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-9
USING: room-names story.words story.shared ;

: old-mansion-2f-9.enter ( -- )   \ 00412410
    $26B story-flag? not if
        0 42.0 63.5 6.6 flicker-sprite
    then
;

: old-mansion-2f-9.char-enter ( -- )   \ 00412430
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
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
    1 self-is? if
        2 hewie-mode
    then
    $26B story-flag? not if
        0 42.0 63.5 6.6 flicker-sprite
    then
;

: old-mansion-2f-9.phase1 ( -- )   \ 00412510
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 43.86 50.0 6.98 $15 16 0 zone
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
    0 43.86 50.0 6.98 $15 16 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
    1 -5.14 50.0 13.21 $44 29 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 29.43 hewie-look-zone
            then
        then
    then
;

: old-mansion-2f-9.phase2 ( -- )   \ 004125E0
    $26B story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 0 4 scene-change
        then
    then
    0 7 $3C char-faces-area? if
        5 1 0 scene-change
    then
;

: old-mansion-2f-9.act00 ( -- )   \ 00412610
    self-wait-done
    42.0 6.6 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $65 message-param-room
        $65 $63 item-count? if
            $8010 message
            wait-message
        else
            $26B story-flag-set
            0 effect-remove
            $65 1 item-give-count
            0 $65 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-9.act01 ( -- )   \ 00412670
    self-wait-done
    50.0 9.0 self-turn-to-xz
    self-wait-done
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-9.enter old-mansion-2f-9 0 room-script!
' old-mansion-2f-9.char-enter old-mansion-2f-9 6 room-script!
' old-mansion-2f-9.phase1 old-mansion-2f-9 1 room-script!
' old-mansion-2f-9.phase2 old-mansion-2f-9 2 room-script!
' old-mansion-2f-9.act00 old-mansion-2f-9 $00 action-script!
' old-mansion-2f-9.act01 old-mansion-2f-9 $01 action-script!
