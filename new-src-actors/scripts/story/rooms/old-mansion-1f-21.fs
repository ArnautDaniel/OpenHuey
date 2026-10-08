\ story/rooms/old-mansion-1f-21.fs - the event scripts of room old-mansion-1f-21 ($6D; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-21
USING: room-names story.words story.shared ;

: old-mansion-1f-21.enter ( -- )   \ 00439B10
    $21E story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $28E story-flag? not if
            0 20.0 1.0 -72.5 flicker-sprite
        then
    then
    $2CE story-flag? not if
        1 3 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 3 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $2CF story-flag? not if
        1 2 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    $2D0 story-flag? not if
        1 1 $8000000 nav-group
        6 1 $14 door-bits
        7 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        6 0 $14 door-bits
        7 1 $14 door-bits
    then
    0 6 0.345 0.687 0.187 0.312 zone-rect
    $56 story-flag? if
        $7D story-flag? not if
            $7D story-flag-set
            $6D 0 65 $80 3 -1 $FF80 44.0 creature-place
            $6D 0 36 $80 3 -1 $81 44.0 creature-place
        then
    then
    1 $2300 sound-volume
;

: old-mansion-1f-21.char-enter ( -- )   \ 00439C00
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
    then then
    0 2 1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    1 3 -1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    2 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    3 1 0 area-camera
    0 self-is? if
        3 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-21.phase1 ( -- )   \ 00439D00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
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
    $A 0 -1 1 chars-area-camera
    $B 1 0 1 chars-area-camera
    $C 0 -1 1 chars-area-camera
    $D 2 1 1 chars-area-camera
    $E 2 1 1 chars-area-camera
    $F 3 -1 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? 0 7 char-entered-area? or 0 8 char-entered-area? or if
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 9 char-entered-area? if
        2 map-page
    then
    0 9 char-left-area? if
        1 map-page
    then
    0 6 char-entered-area? if
        $1C 3 1 char-load
    then
    0 7 char-entered-area? if
        3 0 char-remove
    then
    0 4 char-entered-area? if
        $1C 3 0 char-load
    then
    0 5 char-entered-area? if
        3 0 char-remove
    then
    1 19.93 0.0 -73.33 $D 10 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 1 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    $2CE story-flag? not if
        2 6.4 0.0 -132.4 5 8 1 zone
        $FF 2 char-in-zone? if
            $2CE story-flag-set
            0 3 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            6.4 0.0 -132.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 6.4 0.0 -132.4 0 0 sound
            $40 $100 noise
        then
    then
    $2CF story-flag? not if
        3 -63.9 0.0 -147.4 5 8 1 zone
        $FF 3 char-in-zone? if
            $2CF story-flag-set
            0 2 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -63.9 0.0 -147.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -63.9 0.0 -147.4 0 0 sound
            $40 $112 noise
        then
    then
    $2D0 story-flag? not if
        4 -68.8 0.0 -156.4 5 8 1 zone
        $FF 4 char-in-zone? if
            $2D0 story-flag-set
            0 1 $8000000 nav-group
            6 0 $14 door-bits
            7 1 $14 door-bits
            -68.8 0.0 -156.4 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 -68.8 0.0 -156.4 0 0 sound
            $40 $115 noise
        then
    then
;

: old-mansion-1f-21.phase2 ( -- )   \ 00439F10
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    $21E story-flag? not if
        0 20.0 0.0 -72.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $21E story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            20.0 0.0 -72.5 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 20.0 0.0 -72.5 0 0 sound
            $40 $35 noise
            0 20.0 1.0 -72.5 flicker-sprite
        then
    else $28E story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
;

: old-mansion-1f-21.phase3 ( -- )   \ 00439FB0
    38.0 10.0 -99.0 68.0 30.0 -99.0 38.0 0.0 -99.0 68.0 0.0 -99.0 lights-doorway
    68.0 30.0 -99.0 90.0 50.0 -99.0 68.0 0.0 -99.0 90.0 0.0 -99.0 lights-doorway
;

: old-mansion-1f-21.act00 ( -- )   \ 0043A020
    self-wait-done
    $FE self-touching? not if
        $44 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $44 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $44 door-unlock
                    $8008 message
                    20 self-move-16
                    self-frames-reset
                    $14 self-wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: old-mansion-1f-21.act01 ( -- )   \ 0043A060
    self-wait-done
    20.0 -72.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $28E story-flag-set
            0 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-21.enter old-mansion-1f-21 0 room-script!
' old-mansion-1f-21.char-enter old-mansion-1f-21 6 room-script!
' old-mansion-1f-21.phase1 old-mansion-1f-21 1 room-script!
' old-mansion-1f-21.phase2 old-mansion-1f-21 2 room-script!
' old-mansion-1f-21.phase3 old-mansion-1f-21 3 room-script!
' old-mansion-1f-21.act00 old-mansion-1f-21 $00 action-script!
' old-mansion-1f-21.act01 old-mansion-1f-21 $01 action-script!
