\ story/rooms/old-mansion-1f-5.fs - the event scripts of room old-mansion-1f-5 ($43; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-5
USING: room-names story.words story.shared ;

\ the effect BigFire_vtable (three quad drawers) started with parameter 0
: old-mansion-1f-5.cmd00 ( -- )  s" old-mansion-1f-5.cmd00" stub-step ;

: old-mansion-1f-5.enter ( -- )   \ 00407AE0
    room-sounds
    $19 1.0 0 bgm
    1 1 $300000 nav-group
    $26E story-flag? not if
        0 122.09 8.28 10.26 flicker-sprite
    then
    $26F story-flag? not if
        1 -107.57 1.0 10.1 flicker-sprite
    then
    $270 story-flag? not if
        2 -140.37 16.0 -25.97 flicker-sprite
    then
    8 1 item-count? not 9 1 item-count? not and $A 1 item-count? not and $B 1 item-count? not and $C 1 item-count? not and if
        0 ebit-set
        3 -111.0 17.0 -115.0 flicker-sprite
    then
    $2AB story-flag? $2AC story-flag? not and if
        4 -9.11 1.0 -21.78 flicker-sprite
    then
    old-mansion-1f-5.cmd00
;

: old-mansion-1f-5.char-enter ( -- )   \ 00407B70
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 4 4 char-camera
                0 camera-follow
            else
                1 4 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 4 4 char-camera
            0 camera-follow
        else
            1 4 4 char-camera
            1 camera-follow
        then
    then then
    3 4 4 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    4 2 2 area-camera
    0 self-is? if
        3 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-5.phase1 ( -- )   \ 00407C00
    1 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 -138.0 11.0 11.0 0 0 sound
            1 ebit-set
        then
    else
        $C0000000 6 -138.0 11.0 11.0 0 0 sound
    then
    3 exit-usable? if
        3 exit-check
    then
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    $2AB story-flag? not if
        3 -9.11 0.0 -21.78 $A 5 0 zone
        1 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 683 var-set
                $1A 684 var-set
                $1B 4 var-set
                $1C -9110 var-set
                $1D 1000 var-set
                $1E -21780 var-set
                $1F 3 var-set
                0 1 $8B action
            then
        then
    then
;

: old-mansion-1f-5.phase2 ( -- )   \ 00407CB0
    0 $19 char-in-area? 0 -45 $32 char-heading? and if
        5 0 0 scene-change
    then
    $26F story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    $2AB story-flag? $2AC story-flag? not and if
        3 4 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
;

: old-mansion-1f-5.act00 ( -- )   \ 00407D00
    self-wait-done
    -136.0 12.0 self-turn-to-xz
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    $21 0 pvar? if
        $21 pvar-inc
        $11 message
        wait-message
    else $21 1 pvar? if
        1 6 -138.0 5.0 5.0 0 0 sound
        self-frames-reset
        $10 self-wait-frames
        0 $43 5 char-sound
        $F00 self-anim
        $21 pvar-inc
        $FF panic-stage? not if
            3 panic-stage
        then
        $12 message
        wait-message
    else
        $21 pvar-inc
        $13 message
        wait-message
    then then
    self-idle-or-end
;

: old-mansion-1f-5.act01 ( -- )   \ 0047AB40
    self-idle-or-end
;

: old-mansion-1f-5.act02 ( -- )   \ 0047AB44
    self-idle-or-end
;

: old-mansion-1f-5.act03 ( -- )   \ 00407D60
    self-wait-done
    -107.57 10.1 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $26F story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
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

: old-mansion-1f-5.act04 ( -- )   \ 0047AB48
    self-idle-or-end
;

: old-mansion-1f-5.act05 ( -- )   \ 00407DC0
    self-wait-done
    -9.11 -21.78 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $2AC story-flag-set
            4 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: old-mansion-1f-5.phase5 ( -- )   \ 0047AB3C
;

\ ---- registered ----
' old-mansion-1f-5.enter old-mansion-1f-5 0 room-script!
' old-mansion-1f-5.char-enter old-mansion-1f-5 6 room-script!
' old-mansion-1f-5.phase1 old-mansion-1f-5 1 room-script!
' old-mansion-1f-5.phase2 old-mansion-1f-5 2 room-script!
' old-mansion-1f-5.act00 old-mansion-1f-5 $00 action-script!
' old-mansion-1f-5.act01 old-mansion-1f-5 $01 action-script!
' old-mansion-1f-5.act02 old-mansion-1f-5 $02 action-script!
' old-mansion-1f-5.act03 old-mansion-1f-5 $03 action-script!
' old-mansion-1f-5.act04 old-mansion-1f-5 $04 action-script!
' old-mansion-1f-5.act05 old-mansion-1f-5 $05 action-script!
' old-mansion-1f-5.phase5 old-mansion-1f-5 5 room-script!
