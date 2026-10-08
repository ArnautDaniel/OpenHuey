\ story/rooms/old-mansion-2f-4.fs - the event scripts of room old-mansion-2f-4 ($57; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-4
USING: room-names story.words story.shared flag-names ;

: old-mansion-2f-4.enter ( -- )   \ 0040F490
    room-sounds
    1 0 $300000 nav-group
    $84 story-flag? if
        0 1 $14 door-bits
        2 1 object-show
    then
    $27C story-flag? not if
        0 5.64 9.5 5.58 flicker-sprite
    then
;

: old-mansion-2f-4.char-enter ( -- )   \ 0040F4C0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 -1 char-camera
                0 camera-follow
            else
                1 2 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 -1 char-camera
            0 camera-follow
        else
            1 2 -1 char-camera
            1 camera-follow
        then
    then then
    0 2 -1 area-camera
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
;

: old-mansion-2f-4.phase1 ( -- )   \ 0040F540
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    5 0 0 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    0 3 char-entered-area? if
        0 exit-prepare
        3 0 char-remove
    then
    0 4 char-entered-area? if
        1 exit-prepare
        $1C 3 1 char-load
    then
    0 -44.47 0.0 54.87 $1E 16 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-2f-4.phase2 ( -- )   \ 0040F5B0
    0 $D char-in-area? 0 -15 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 $F char-in-area? 0 20 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 $10 char-in-area? 0 45 $3C char-heading? and if
        5 9 0 scene-change
    then
    $27C story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 7 4 scene-change
        then
    then
    0 $11 char-in-area? 0 90 $32 char-heading? and if
        5 1 0 scene-change
    then
;

: old-mansion-2f-4.act00 ( -- )   \ 0040F600
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    6 message
    wait-message
    $902 self-anim
    self-wait-anim
    9 item-use
    $A message-param-room
    $A 1 item-give-count
    0 $A item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act01 ( -- )   \ 0040F670
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    9 message
    wait-message
    $F 6 fade
    wait-fade
    $A message
    wait-message
    $F 7 fade
    wait-fade
    $48 subscreen-bit? not $49 subscreen-bit? not or if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $48 subscreen-bit
        $49 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act02 ( -- )   \ 0040F6B0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        3 message
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $243 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act03 ( -- )   \ 0040F700
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    5 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act04 ( -- )   \ 0047ABC4
    self-idle-or-end
;

: old-mansion-2f-4.act05 ( -- )   \ 0047ABC8
    self-idle-or-end
;

: old-mansion-2f-4.act06 ( -- )   \ 0047ABCC
    self-idle-or-end
;

: old-mansion-2f-4.act07 ( -- )   \ 0040F820
    self-wait-done
    5.64 5.58 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message
            wait-message
        else
            $27C story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
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

: old-mansion-2f-4.act08 ( -- )   \ 0040F880
    self-wait-done
    0.6 5.2 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    7 message
    wait-message
    self-idle-or-end
;

: old-mansion-2f-4.act09 ( -- )   \ 0040F8A0
    self-wait-done
    46.8 0.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    8 message
    wait-message
    self-idle-or-end
;

: old-mansion-2f-4.phase5 ( -- )   \ 0047ABC0
;

\ ---- registered ----
' old-mansion-2f-4.enter old-mansion-2f-4 0 room-script!
' old-mansion-2f-4.char-enter old-mansion-2f-4 6 room-script!
' old-mansion-2f-4.phase1 old-mansion-2f-4 1 room-script!
' old-mansion-2f-4.phase2 old-mansion-2f-4 2 room-script!
' old-mansion-2f-4.act00 old-mansion-2f-4 $00 action-script!
' old-mansion-2f-4.act01 old-mansion-2f-4 $01 action-script!
' old-mansion-2f-4.act02 old-mansion-2f-4 $02 action-script!
' old-mansion-2f-4.act03 old-mansion-2f-4 $03 action-script!
' old-mansion-2f-4.act04 old-mansion-2f-4 $04 action-script!
' old-mansion-2f-4.act05 old-mansion-2f-4 $05 action-script!
' old-mansion-2f-4.act06 old-mansion-2f-4 $06 action-script!
' old-mansion-2f-4.act07 old-mansion-2f-4 $07 action-script!
' old-mansion-2f-4.act08 old-mansion-2f-4 $08 action-script!
' old-mansion-2f-4.act09 old-mansion-2f-4 $09 action-script!
' old-mansion-2f-4.phase5 old-mansion-2f-4 5 room-script!
