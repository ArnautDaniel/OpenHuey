\ story/rooms/old-mansion-2f-10.fs - the event scripts of room old-mansion-2f-10 ($5F; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-10
USING: room-names story.words story.shared ;

: old-mansion-2f-10.enter ( -- )   \ 0041B130
    $233 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $274 story-flag? not if
            0 -10.5 1.0 -0.4 flicker-sprite
        then
    then
    0 2 0.812 0.687 0.187 0.312 zone-rect
    $50 story-flag? if
        $7A story-flag? not if
            $7A story-flag-set
            $5F 0 259 4 8 -1 0 0.0 creature-place
        then
    then
    1 12.4 17.3 -45.0 1 effect-86
    1 $2300 sound-volume
;

: old-mansion-2f-10.char-enter ( -- )   \ 0041B1B0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
;

: old-mansion-2f-10.phase1 ( -- )   \ 0041B270
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
    then
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        2 exit-prepare
    then
    1 5.8 0.0 15.45 $F 13 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 13.0 hewie-look-zone
            then
        then
    then
    2 -65.87 0.0 11.35 $B 15 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    3 -66.99 0.0 -11.47 $B 15 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    6 1 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
;

: old-mansion-2f-10.phase2 ( -- )   \ 0041B370
    $233 story-flag? not if
        0 -10.5 0.0 -0.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $233 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -10.5 0.0 -0.4 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -10.5 0.0 -0.4 0 0 sound
            $40 $7B noise
            0 -10.5 1.0 -0.4 flicker-sprite
        then
    else $274 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 0 4 scene-change
        then
    then then
    4 -67.5 0.0 -11.5 8 10 0 zone
    0 4 8 char-zone-bits? if
        0 -67 -11 $3C char-faces-xz? if
            5 1 0 scene-change
        then
    then
    5 -67.5 0.0 11.5 8 10 0 zone
    0 5 8 char-zone-bits? if
        0 -67 11 $3C char-faces-xz? if
            5 1 0 scene-change
        then
    then
;

: old-mansion-2f-10.phase3 ( -- )   \ 0041B450
    -13.7 15.0 -28.0 -13.7 15.0 -11.3 -13.7 0.0 -28.0 -13.7 0.0 -11.3 lights-doorway
    -13.7 15.0 14.0 -13.7 15.0 30.7 -13.7 0.0 14.0 -13.7 0.0 30.7 lights-doorway
;

: old-mansion-2f-10.act00 ( -- )   \ 0041B4C0
    self-wait-done
    -10.5 -0.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $9B message-param-room
        $9B $63 item-count? if
            $8010 message
            wait-message
        else
            $274 story-flag-set
            0 effect-remove
            $9B 1 item-give-count
            0 $9B item-tab
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

: old-mansion-2f-10.act01 ( -- )   \ 0041B520
    self-wait-done
    0 4 8 char-zone-bits? if
        -67.5 -11.5 self-turn-to-xz
        self-wait-done
    else
        -67.5 11.5 self-turn-to-xz
        self-wait-done
    then
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        0 ebit-set
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-10.enter old-mansion-2f-10 0 room-script!
' old-mansion-2f-10.char-enter old-mansion-2f-10 6 room-script!
' old-mansion-2f-10.phase1 old-mansion-2f-10 1 room-script!
' old-mansion-2f-10.phase2 old-mansion-2f-10 2 room-script!
' old-mansion-2f-10.phase3 old-mansion-2f-10 3 room-script!
' old-mansion-2f-10.act00 old-mansion-2f-10 $00 action-script!
' old-mansion-2f-10.act01 old-mansion-2f-10 $01 action-script!
