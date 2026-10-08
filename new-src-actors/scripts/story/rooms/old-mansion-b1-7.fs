\ story/rooms/old-mansion-b1-7.fs - the event scripts of room old-mansion-b1-7 ($61; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-7
USING: room-names story.words story.shared ;

\ room 0x61 (byte 3): 0 / 2 / 4 set up the paths of characters 0x14 (2 x 3 x 7 cells from (25,
\ 0, 40), 480 frames a stretch; with the glint Glint_vtable) / 0x15 (2 x 3 x 5 from (30, 0, 50),
\ 240) / 0x16 (the same box, 280); 1 / 3 / 5 move them a frame (returning 2: again next frame)
: old-mansion-b1-7.cmd00 ( b0 -- )  drop s" old-mansion-b1-7.cmd00" stub-step ;
\ room 0x61: the light shaft: byte 3 0 starts it (LightShaft, with its motes from (30, 0, 70)),
\ its slot in event variable 3; 1 its haze on; 2 off.
: old-mansion-b1-7.cmd01 ( b0 -- )  drop s" old-mansion-b1-7.cmd01" stub-step ;

: old-mansion-b1-7.enter ( -- )   \ 004291C0
    0 $F1 0 action
    0 $F2 1 action
    0 $F3 2 action
    0 0 var-set
    1 20 var-set
    2 90 var-set
    $2A5 story-flag? $2A6 story-flag? not and if
        0 8.95 1.0 11.89 flicker-sprite
    then
    1 $2300 sound-volume
    0 old-mansion-b1-7.cmd01
    1 2 $10000000 nav-group
;

: old-mansion-b1-7.char-enter ( -- )   \ 00429210
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
    0 self-is? if
        0 exit-taken? if
            1 map-page
        then
    then
;

: old-mansion-b1-7.phase1 ( -- )   \ 004292A0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $B char-entered-area? if
        1 map-page
    then
    0 $B char-left-area? if
        0 map-page
    then
    0 3 var? if
        0 game-mode? if
            2 var-dec
            2 0 var? if
                2 90 var-set
                1 20 var? if
                    1 21 var-set
                else 1 21 var? if
                    1 22 var-set
                else 1 22 var? if
                    1 20 var-set
                then then then
            then
            1 20 var? if
                1 char-here? if
                    $14 1 100 chars-within? if
                        $14 0.0 hewie-look-char
                    then
                then
            else 1 21 var? if
                1 char-here? if
                    $15 1 100 chars-within? if
                        $15 0.0 hewie-look-char
                    then
                then
            else 1 22 var? if
                1 char-here? if
                    $16 1 100 chars-within? if
                        $16 0.0 hewie-look-char
                    then
                then
            then then then
        then
    then
    $2A5 story-flag? not if
        0 8.95 0.0 11.89 $A 5 0 zone
        1 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 677 var-set
                $1A 678 var-set
                $1B 0 var-set
                $1C 8950 var-set
                $1D 1000 var-set
                $1E 11890 var-set
                $1F 0 var-set
                0 1 $8B action
            then
        then
    then
;

: old-mansion-b1-7.phase2 ( -- )   \ 004293E0
    0 $A char-in-area? if
        5 4 0 scene-change
    then
    $2A5 story-flag? $2A6 story-flag? not and if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
;

: old-mansion-b1-7.phase3 ( -- )   \ 00429408
    2 camera-mode? if
        1 old-mansion-b1-7.cmd01
    else
        2 old-mansion-b1-7.cmd01
    then
;

: old-mansion-b1-7.phase5 ( -- )   \ 00429418
    3 action-end
    3 char-done
    4 action-end
    4 char-done
    5 action-end
    5 char-done
;

: old-mansion-b1-7.act00 ( -- )   \ 00429430
    $14 3 $FF char-load
    3 char-unload
    $14 char-activate
    $14 $9000 1 0 char-anim-hold
    $14 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    0 old-mansion-b1-7.cmd00
    1 old-mansion-b1-7.cmd00
    self-idle-or-end
;

: old-mansion-b1-7.act01 ( -- )   \ 00429460
    $15 4 $FF char-load
    4 char-unload
    $15 char-activate
    $15 $9000 1 0 char-anim-hold
    $15 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    2 old-mansion-b1-7.cmd00
    3 old-mansion-b1-7.cmd00
    self-idle-or-end
;

: old-mansion-b1-7.act02 ( -- )   \ 00429490
    $16 5 $FF char-load
    5 char-unload
    self-frames-reset
    $A self-wait-frames
    $16 char-activate
    $16 $9000 1 0 char-anim-hold
    $16 37.5 12.5 72.5 0 char-to-xyz
    0 var-inc
    4 old-mansion-b1-7.cmd00
    5 old-mansion-b1-7.cmd00
    self-idle-or-end
;

: old-mansion-b1-7.act03 ( -- )   \ 004294C0
    self-wait-done
    8.95 11.89 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $2A6 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: old-mansion-b1-7.act04 ( -- )   \ 00429520
    self-wait-done
    1 self-anim
    self-wait-anim
    -1 self-move-16
    0 message
    self-frames-reset
    7 self-wait-frames
    wait-message
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-b1-7.enter old-mansion-b1-7 0 room-script!
' old-mansion-b1-7.char-enter old-mansion-b1-7 6 room-script!
' old-mansion-b1-7.phase1 old-mansion-b1-7 1 room-script!
' old-mansion-b1-7.phase2 old-mansion-b1-7 2 room-script!
' old-mansion-b1-7.phase3 old-mansion-b1-7 3 room-script!
' old-mansion-b1-7.phase5 old-mansion-b1-7 5 room-script!
' old-mansion-b1-7.act00 old-mansion-b1-7 $00 action-script!
' old-mansion-b1-7.act01 old-mansion-b1-7 $01 action-script!
' old-mansion-b1-7.act02 old-mansion-b1-7 $02 action-script!
' old-mansion-b1-7.act03 old-mansion-b1-7 $03 action-script!
' old-mansion-b1-7.act04 old-mansion-b1-7 $04 action-script!
