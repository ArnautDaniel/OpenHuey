\ story/rooms/old-mansion-2f-11.fs - the event scripts of room old-mansion-2f-11 ($62; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-11
USING: room-names story.words story.shared flag-names ;

\ room 0x62 (Room62_Cmd00_ptmf): object byte 4 swings: byte 3 0 starts it (phase +0x30 0, size
\ +0x34 0.01; object 0 with sound 7), else a step (+0x10 = size x sin(phase), phase on 60
\ degrees, the size down 0.001); 2 until it is still
: old-mansion-2f-11.cmd00 ( b0 b1 -- )  drop drop s" old-mansion-2f-11.cmd00" stub-step ;
\ room 0x62 (D_00422318): the room's effect 0 dropped by byte 3 - 0 at rest (speed 0), 1 raised
\ by 0.5; else it falls (gravity 0.5 a frame, turning 0.16) and bounces off 0.7 losing 70% (a
\ sound each bounce) until slower than 0.2 (+0x74 set: landed; 1), else still going (2)
: old-mansion-2f-11.cmd01 ( b0 -- )  drop s" old-mansion-2f-11.cmd01" stub-step ;

: old-mansion-2f-11.enter ( -- )   \ 00421C60
    room-sounds
    2 0 var-set
    3 0 var-set
    $72 story-flag? not if
        0 5.19 35.3 -34.0 flicker-sprite
        0 old-mansion-2f-11.cmd01
    else $53 story-flag? not if
        0 7.3 0.7 -28.3 flicker-sprite
    then then
    $26C story-flag? not if
        1 -6.82 1.0 -10.25 flicker-sprite
    then
    $AF story-flag? if
        $81 story-flag? not if
            $81 story-flag-set
            $62 0 15 $80 8 -1 $5A 0.0 creature-place
        then
    then
;

: old-mansion-2f-11.char-enter ( -- )   \ 00421CD0
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
;

: old-mansion-2f-11.phase1 ( -- )   \ 00421D50
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
    4 1 1 -1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 3.0 0.0 -35.5 9 15 0 zone
    1 -5.75 0.0 -33.25 9 15 0 zone
    2 -14.5 0.0 -31.0 9 15 0 zone
    $72 story-flag? not if
        0 0 char-in-zone? 0 1 char-in-zone? or 0 2 char-in-zone? or if
            0 $F1 $B action
        then
    then
    4 21.19 0.0 -20.13 $C 10 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    1 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    5 19.29 0.0 -1.64 $14 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    6 sound-bank-loaded? if
        3 var-inc
        3 30 var? if
            3 0 var-set
            2 0 var? if
                $40000003 6 37.86 0.0 -38.87 0 0 sound
            else 2 1 var? if
                $40000004 6 37.86 0.0 -38.87 0 0 sound
            else 2 2 var? if
                $40000005 6 37.86 0.0 -38.87 0 0 sound
            else 2 3 var? if
                $40000006 6 37.86 0.0 -38.87 0 0 sound
            then then then then
            2 var-inc
            2 4 var? if
                2 0 var-set
            then
        then
    then
;

: old-mansion-2f-11.phase2 ( -- )   \ 00421EF0
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 0 1 scene-change
        then
    then
    $72 story-flag? $53 story-flag? not and if
        3 0 5 5 0 zone-at-effect
        0 3 2 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    0 8 char-in-area? 0 90 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 9 char-in-area? 0 60 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 -10 $32 char-heading? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 90 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $C char-in-area? 0 0 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $D char-in-area? 0 -80 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $E char-in-area? 0 90 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 $10 char-in-area? 0 -45 $32 char-heading? and if
        5 7 0 scene-change
    then
    $26C story-flag? not if
        6 1 5 5 0 zone-at-effect
        0 6 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
    0 $11 char-in-area? 0 -45 $32 char-heading? and if
        5 9 0 scene-change
    then
    0 $12 char-in-area? 0 -45 $32 char-heading? and if
        5 $A 0 scene-change
    then
;

: old-mansion-2f-11.phase3 ( -- )   \ 00421FC0
    -7.0 30.0 42.0 -10.8 30.0 29.2 -7.0 0.0 42.0 -10.8 0.0 29.2 lights-doorway
;

: old-mansion-2f-11.act00 ( -- )   \ 00422000
    self-wait-done
    $FE self-touching? not if
        $67 self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $67 char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $67 door-unlock
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

: old-mansion-2f-11.act01 ( -- )   \ 00422040
    self-wait-done
    7.3 -28.3 self-turn-to-xz
    self-wait-done
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $53 story-flag-set
    0 effect-remove
    $D message-param-room
    $D 1 item-give-count
    0 $D item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-11.act02 ( -- )   \ 00422090
    $72 story-flag-set
    2 old-mansion-2f-11.cmd01
    0 7.3 0.7 -28.3 flicker-sprite
    self-idle-or-end
;

: old-mansion-2f-11.act03 ( -- )   \ 004220A8
    0 0 old-mansion-2f-11.cmd00
    1 0 old-mansion-2f-11.cmd00
    self-idle-or-end
;

: old-mansion-2f-11.act04 ( -- )   \ 004220C0
    self-wait-done
    5.19 -34.0 self-turn-to-xz
    self-wait-done
    0 ebit? not $72 story-flag? or if
        0 message
        wait-message
        0 ebit-set
    else
        $A00 self-anim
        self-wait-anim
        1 message
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-11.act05 ( -- )   \ 004220F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -20 self-turn-angle
    self-wait-done
    $331 story-flag? not if
        2 message
        wait-message
        $331 story-flag-set
    else
        3 message
        wait-message
    then
    $F 6 fade
    wait-fade
    4 message
    wait-message
    $F 7 fade
    wait-fade
    $237 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-11.act06 ( -- )   \ 00422130
    self-wait-done
    -6.82 -10.25 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 message
            wait-message
        else
            $26C story-flag-set
            1 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
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

: old-mansion-2f-11.act07 ( -- )   \ 00422190
    self-wait-done
    0 $B char-in-area? if
        180 self-turn-angle
        self-wait-done
    else 0 $C char-in-area? if
        0 self-turn-angle
        self-wait-done
    else 0 $D char-in-area? if
        160 self-turn-angle
        self-wait-done
    else 0 $E char-in-area? if
        180 self-turn-angle
        self-wait-done
    else 0 $F char-in-area? if
        0 self-turn-angle
        self-wait-done
    else 0 $10 char-in-area? if
        -90 self-turn-angle
        self-wait-done
    then then then then then then
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 0 var? if
        5 message
        wait-message
        1 var-inc
    else 1 1 var? if
        6 message
        wait-message
        1 var-inc
    else 1 2 var? if
        7 message
        wait-message
        1 0 var-set
    then then then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-11.act08 ( -- )   \ 00422208
    0 1 old-mansion-2f-11.cmd00
    1 1 old-mansion-2f-11.cmd00
    self-idle-or-end
;

: old-mansion-2f-11.act09 ( -- )   \ 00422220
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    $A message
    wait-message
    $F 7 fade
    wait-fade
    $63 subscreen-bit? not $65 subscreen-bit? not or if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $63 subscreen-bit
        $65 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-11.act0A ( -- )   \ 00422260
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    $46 subscreen-bit? not $47 subscreen-bit? not or if
        $1D01 self-anim
        $B message
        wait-message
        self-wait-anim
        $46 subscreen-bit
        $47 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-11.act0B ( -- )   \ 004222A0
    0 $F2 3 action
    0 $F4 8 action
    0 var-inc
    0 3 var? if
        0 $F3 2 action
    else
        1 old-mansion-2f-11.cmd01
    then
    begin
        fiona-free? not while
        yield
    repeat
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-11.enter old-mansion-2f-11 0 room-script!
' old-mansion-2f-11.char-enter old-mansion-2f-11 6 room-script!
' old-mansion-2f-11.phase1 old-mansion-2f-11 1 room-script!
' old-mansion-2f-11.phase2 old-mansion-2f-11 2 room-script!
' old-mansion-2f-11.phase3 old-mansion-2f-11 3 room-script!
' old-mansion-2f-11.act00 old-mansion-2f-11 $00 action-script!
' old-mansion-2f-11.act01 old-mansion-2f-11 $01 action-script!
' old-mansion-2f-11.act02 old-mansion-2f-11 $02 action-script!
' old-mansion-2f-11.act03 old-mansion-2f-11 $03 action-script!
' old-mansion-2f-11.act04 old-mansion-2f-11 $04 action-script!
' old-mansion-2f-11.act05 old-mansion-2f-11 $05 action-script!
' old-mansion-2f-11.act06 old-mansion-2f-11 $06 action-script!
' old-mansion-2f-11.act07 old-mansion-2f-11 $07 action-script!
' old-mansion-2f-11.act08 old-mansion-2f-11 $08 action-script!
' old-mansion-2f-11.act09 old-mansion-2f-11 $09 action-script!
' old-mansion-2f-11.act0A old-mansion-2f-11 $0A action-script!
' old-mansion-2f-11.act0B old-mansion-2f-11 $0B action-script!
