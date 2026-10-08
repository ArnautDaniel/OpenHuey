\ story/rooms/water-tower-7f-2.fs - the event scripts of room water-tower-7f-2 ($C6; Water Tower: 7F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.water-tower-7f-2
USING: room-names story.words story.shared flag-names ;

: water-tower-7f-2.enter ( -- )   \ 00441170
    room-sounds
    hunted state-flag-set
    $30 1.0 0 bgm
    $2DA story-flag? $2DB story-flag? not and if
        0 -9.0 140.97 -10.23 flicker-sprite
    then
;

: water-tower-7f-2.char-enter ( -- )   \ 004411A0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 1 char-camera
                0 camera-follow
            else
                1 3 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 1 char-camera
            0 camera-follow
        else
            1 3 1 char-camera
            1 camera-follow
        then
    then then
    0 3 1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    1 0 -1 area-camera
    0 self-is? if
        $80 exit-taken? if
            0 $1C2 -45.44 -52.11 105 char-to-xz
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
            0.0 sound-volume-scale
            0 0 0 action
        then
        $81 exit-taken? if
            0 char-activate
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 1 char-camera
                0 camera-follow
            else
                1 3 1 char-camera
                1 camera-follow
            then
            1 char-activate
            $C6 0 836 hewie-to-room
            1 $344 6.81 -30.23 0 char-to-xz
            0.0 sound-volume-scale
            0 0 1 action
        then
        0 exit-taken? if
            8 map-page
        then
    then
    1 self-is? if
        $80 exit-taken? if
            1 $6D -14.34 -72.11 -30 char-to-xz
        then
    then
;

: water-tower-7f-2.phase1 ( -- )   \ 004412A0
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 1 -1 1 chars-area-camera
    8 1 -1 1 chars-area-camera
    9 2 0 1 chars-area-camera
    $A 2 0 1 chars-area-camera
    $B 3 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $F char-entered-area? if
        7 map-page
    then
    0 $F char-left-area? if
        8 map-page
    then
    $2DA story-flag? not if
        0 -9.0 139.97 -10.23 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 730 var-set
                $1A 731 var-set
                $1B 0 var-set
                $1C -9000 var-set
                $1D 140970 var-set
                $1E -10230 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -127.0 1.7 10.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -127.0 1.7 10.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -127.0 1.7 10.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -127.0 1.7 10.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: water-tower-7f-2.phase2 ( -- )   \ 00441400
    0 $C $32 char-faces-area? if
        5 $84 3 scene-change
    then
    0 $E $32 char-faces-area? if
        5 3 0 scene-change
    then
    $2DA story-flag? $2DB story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: water-tower-7f-2.act00 ( -- )   \ 00441430
    yield
    camera-restart
    1 exit-prepare
    self-frames-reset
    self-wait-16
    world-held state-flag-clear
    $F $51 fade
    $828A item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-2.act01 ( -- )   \ 00441450
    world-held state-flag-clear
    $F $51 fade
    wait-fade
    stalkers-stay state-flag-clear
    $FF 0 char-visible
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-2.act02 ( -- )   \ 00441470
    self-wait-done
    -9.0 -10.23 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2DB story-flag-set
            0 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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

: water-tower-7f-2.act03 ( -- )   \ 004414D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0.0 -105.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: water-tower-7f-2.phase5 ( -- )   \ 0047AF30
    hunted state-flag-clear
;

\ ---- registered ----
' water-tower-7f-2.enter water-tower-7f-2 0 room-script!
' water-tower-7f-2.char-enter water-tower-7f-2 6 room-script!
' water-tower-7f-2.phase1 water-tower-7f-2 1 room-script!
' water-tower-7f-2.phase2 water-tower-7f-2 2 room-script!
' water-tower-7f-2.act00 water-tower-7f-2 $00 action-script!
' water-tower-7f-2.act01 water-tower-7f-2 $01 action-script!
' water-tower-7f-2.act02 water-tower-7f-2 $02 action-script!
' water-tower-7f-2.act03 water-tower-7f-2 $03 action-script!
' water-tower-7f-2.phase5 water-tower-7f-2 5 room-script!
