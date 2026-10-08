\ story/rooms/old-mansion-b1-2.fs - the event scripts of room old-mansion-b1-2 ($4B; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-2
USING: room-names story.words story.shared ;

\ room 0x4B (D_00409910): a lit quad at x -63.65 .. -55.65, z 104.5, from 4 to 21
: old-mansion-b1-2.cmd00 ( b0 -- )  drop s" old-mansion-b1-2.cmd00" stub-step ;
\ room 0x4B: the curtain ("Curtain") animated by event variable 0 (var0_anim: byte 3 0 forward,
\ 1 back, 2 / 3 back at rest).
: old-mansion-b1-2.cmd01 ( b0 -- )  drop s" old-mansion-b1-2.cmd01" stub-step ;
\ 1 unless the object at +0x18 exists and its byte +0x28 is 1.
: old-mansion-b1-2.cond00? ( -- flag )  s" old-mansion-b1-2.cond00?" stub-flag ;

: old-mansion-b1-2.enter ( -- )   \ 00408B90
    room-sounds
    $70 story-flag? not if
        old-mansion-b1-2.cond00? if
            $4B 0 422 2 8 6 0 0.0 creature-place
        then
    then
    $237 story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $290 story-flag? not if
            0 13.5 -9.0 -75.0 flicker-sprite
        then
    then
    $238 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
        $291 story-flag? not if
            1 13.5 -9.0 -59.5 flicker-sprite
        then
    then
    0 4 0.5 0.5 0.25 0.5 zone-rect
    1 $B 0.4 0.0 0.2 0.5 zone-rect
    $329 story-flag? not if
        4 0 $14 door-bits
        5 1 $14 door-bits
        $32A story-flag? not if
            3 old-mansion-b1-2.cmd01
            1 old-mansion-b1-2.cmd00
        else
            2 old-mansion-b1-2.cmd01
            0 old-mansion-b1-2.cmd00
        then
    else
        4 1 $14 door-bits
        5 0 $14 door-bits
        1 old-mansion-b1-2.cmd00
        1 2 $2008000 nav-group
    then
    $269 story-flag? not if
        2 -109.83 12.0 72.62 flicker-sprite
    then
    3 132.4 -16.6 -76.8 1 effect-86
    4 85.7 -5.3 -16.8 1 effect-86
    5 85.7 -5.3 16.8 1 effect-86
    6 132.4 -16.6 76.8 1 effect-86
    1 $2300 sound-volume
;

: old-mansion-b1-2.char-enter ( -- )   \ 00408CD0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 1 char-camera
                0 camera-follow
            else
                1 2 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 1 char-camera
            0 camera-follow
        else
            1 2 1 char-camera
            1 camera-follow
        then
    then then
    1 2 1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 5 3 char-camera
                0 camera-follow
            else
                1 5 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 5 3 char-camera
            0 camera-follow
        else
            1 5 3 char-camera
            1 camera-follow
        then
    then then
    2 5 3 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 4 2 char-camera
                0 camera-follow
            else
                1 4 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 4 2 char-camera
            0 camera-follow
        else
            1 4 2 char-camera
            1 camera-follow
        then
    then then
    3 4 2 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    4 1 0 area-camera
    hewie-controlled? not if
        0 self-is? 5 exit-taken? and if
            0 5 char-to-exit
            hewie-controlled? not if
                0 6 4 char-camera
                0 camera-follow
            else
                1 6 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 5 exit-taken? and if
        1 5 char-to-exit
        hewie-controlled? not if
            0 6 4 char-camera
            0 camera-follow
        else
            1 6 4 char-camera
            1 camera-follow
        then
    then then
    5 6 4 area-camera
    0 self-is? if
        0 exit-taken? 1 exit-taken? or 4 exit-taken? or 5 exit-taken? or if
            1 map-page
        then
    then
;

: old-mansion-b1-2.phase1 ( -- )   \ 00408E50
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
    4 exit-usable? if
        4 exit-check
    then
    5 exit-usable? if
        5 exit-check
    then
    $D 0 -1 1 chars-area-camera
    $E 1 0 1 chars-area-camera
    $F 1 0 1 chars-area-camera
    $10 2 1 1 chars-area-camera
    $11 1 0 1 chars-area-camera
    $12 3 -1 1 chars-area-camera
    $13 3 -1 1 chars-area-camera
    $14 4 2 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 5 3 1 chars-area-camera
    $17 1 0 1 chars-area-camera
    $18 6 4 1 chars-area-camera
    0 6 char-entered-area? if
        0 exit-prepare
    then
    0 7 char-entered-area? if
        1 exit-prepare
    then
    0 8 char-entered-area? if
        2 exit-prepare
    then
    0 9 char-entered-area? if
        3 exit-prepare
    then
    0 $A char-entered-area? 0 $B char-entered-area? or if
        4 exit-prepare
    then
    0 $C char-entered-area? if
        5 exit-prepare
    then
    0 $12 char-entered-area? if
        0 map-page
    then
    0 $12 char-left-area? if
        1 map-page
    then
    2 ebit? not if
        $329 story-flag? not $32A story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                $FE $1A char-entered-area? $FE 0 char-action? and if
                    0 $FE 7 action
                then
            then
        then
    then
    2 85.21 -24.0 1.06 $14 19 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 19.0 hewie-look-zone
            then
        then
    then
    $329 story-flag? not $32A story-flag? and if
        3 -60.85 0.0 104.12 $10 6 0 zone
        4 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        4 ebit-set
                        $32 chance? if
                            $1F 3 var-set
                            0 1 $93 action
                        then
                    then
                then
            then
        then
    then
    4 -101.62 0.0 100.21 $14 6 0 zone
    5 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    5 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    4 -101.62 0.0 100.21 $14 6 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    5 -132.75 0.0 71.15 $14 12 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 5 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 5 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    5 -132.75 0.0 71.15 $14 12 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    6 -29.87 0.0 75.83 $14 12 0 zone
    7 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 6 9 char-zone-bits? if
                    7 ebit-set
                    $1E chance? if
                        $1F 6 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    6 -29.87 0.0 75.83 $14 12 0 zone
    1 char-here? 2 game-mode? not and if
        hewie-can-command? if
            1 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-b1-2.phase2 ( -- )   \ 004090C0
    -2147483646 scene-request? if
        0 0 char-group-bit4? if
            5 4 1 scene-change
        then
        0 2 char-group-bit4? if
            5 0 1 scene-change
        then
        0 3 char-group-bit4? if
            5 2 1 scene-change
        then
    then
    0 $19 char-in-area? 0 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    3 ebit? not if
        0 $1A char-in-area? 0 0 $3C char-heading? and if
            $329 story-flag? not if
                $FE char-here? not if
                    5 6 5 scene-change
                else $FE $1A char-in-area? $FE 0 40 chars-within? or if
                    $8016 scene-ending
                else
                    5 6 5 scene-change
                then then
            else
                5 8 0 scene-change
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        $FE char-busy? not if
            3 ebit-clear
        then
    else
        3 ebit-clear
    then then
    $269 story-flag? not if
        7 2 5 5 0 zone-at-effect
        0 7 3 char-zone-bits? if
            5 9 4 scene-change
        then
    then
    8 -29.58 0.0 75.55 7 10 0 zone
    0 8 8 char-zone-bits? if
        0 -30 76 $3C char-faces-xz? if
            5 $C 0 scene-change
        then
    then
    9 -133.09 0.0 71.35 7 10 0 zone
    0 9 8 char-zone-bits? if
        0 -133 71 $3C char-faces-xz? if
            5 $C 0 scene-change
        then
    then
    0 $1D char-in-area? 0 -45 $32 char-heading? and if
        5 $D 0 scene-change
    then
    $237 story-flag? not if
        0 13.5 -10.0 -75.0 5 8 1 zone
        $FF 0 char-in-zone? if
            $237 story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            13.5 -10.0 -75.0 0 -2145378272 0 0.0 scene-effect-8C
            $88 5 13.5 -10.0 -75.0 0 0 sound
            $40 $23A noise
            0 13.5 -9.0 -75.0 flicker-sprite
        then
    else $290 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then then
    $238 story-flag? not if
        1 13.5 -10.0 -59.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $238 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            13.5 -10.0 -59.5 0 -2146430960 0 0.0 scene-effect-8C
            $88 5 13.5 -10.0 -59.5 0 0 sound
            $40 $234 noise
            1 13.5 -9.0 -59.5 flicker-sprite
        then
    else $291 story-flag? not if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then then
;

: old-mansion-b1-2.phase5 ( -- )   \ 004092C0
    3 ebit? if
        $32A story-flag-set
        $329 story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            $FE 1 stalker-rage
        then
    then
;

: old-mansion-b1-2.phase3 ( -- )   \ 004092E0
    58.0 5.0 19.5 58.0 5.0 15.0 58.0 -24.0 19.5 58.0 -24.0 15.0 lights-doorway
    58.0 5.0 -15.0 58.0 5.0 -19.5 58.0 -24.0 -15.0 58.0 -24.0 -19.5 lights-doorway
;

: old-mansion-b1-2.act00 ( -- )   \ 00409350
    self-wait-done
    2 self-through-exit
    self-wait-done
    0 ebit? not 2 game-mode? or $FF panic-stage? or if
        $FF panic-stage? 2 game-mode? or if
            $60B self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $609 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            1 message
            wait-message
        then
        0 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $8F 145.0 -75.0 180 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 3.0 event-camera
        self-frames-reset
        4 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $8F 143.75 -75.3 180 char-to-xz
        $17 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-b1-2.act01 ( -- )   \ 004093E0
    1 self-scripted
    self-wait-done
    2 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 2 6 char-sound
    $D message-param-room
    $52 door-locked? not if
        $D item-use
    then
    $51 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-2.act02 ( -- )   \ 00409420
    self-wait-done
    3 self-through-exit
    self-wait-done
    1 ebit? not 2 game-mode? or $FF panic-stage? or if
        $FF panic-stage? 2 game-mode? or if
            $60B self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
        else
            $609 self-anim
            self-wait-anim
            -1 self-move-16
            self-frames-reset
            7 self-wait-frames
            1 message
            wait-message
        then
        1 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        0 $29 145.0 75.0 0 char-to-xz
        $17 state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 3.0 event-camera
        self-frames-reset
        4 self-wait-frames
        2 message
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        0 $29 146.25 75.3 0 char-to-xz
        $17 state-flag-clear
        1 self-scripted
    then
    self-idle-or-end
;

: old-mansion-b1-2.act03 ( -- )   \ 004094B0
    1 self-scripted
    self-wait-done
    3 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    0 2 6 char-sound
    $D message-param-room
    $51 door-locked? not if
        $D item-use
    then
    $52 door-unlock
    20 self-move-16
    self-frames-reset
    $14 self-wait-frames
    $8019 message
    wait-message
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-2.act04 ( -- )   \ 004094F0
    self-wait-done
    $FE self-touching? not if
        $4F self-through-door
        self-wait-done
        $FE self-touching? not if
            $FF panic-stage? not if
                0 $4F char-not-at-door? if
                    $609 self-anim
                else
                    $608 self-anim
                then
                self-frames-reset
                $14 self-wait-frames
                $FF panic-stage? not if
                    0 $72 5 char-sound
                    $4F door-unlock
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

: old-mansion-b1-2.act05 ( -- )   \ 00409530
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    90.0 0.0 self-turn-to-xz
    self-wait-done
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-2.act06 ( -- )   \ 00409560
    1 self-scripted
    2 ebit-set
    self-wait-done
    0 0 char-file-load
    $21D -59.65 99.14 0 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $32A story-flag? not if
        0 old-mansion-b1-2.cmd00
        0 0 var-set
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            0 old-mansion-b1-2.cmd01
            yield
            0 30 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        2 old-mansion-b1-2.cmd01
        $32A story-flag-set
    else
        0 0 var-set
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        begin
            1 old-mansion-b1-2.cmd01
            yield
            0 28 var? not while
            0 12 var? if
                0 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        3 old-mansion-b1-2.cmd01
        $32A story-flag-clear
        1 old-mansion-b1-2.cmd00
    then
    2 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-2.act07 ( -- )   \ 00409610
    3 ebit-set
    self-wait-done
    $FE 1 char-file-load
    $21D -59.65 99.14 0 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    $FE 5 6 char-sound
    $8000 5 self-anim-blend
    self-wait-anim
    $E00 5 self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    $329 story-flag-set
    $FE 1 6 char-sound
    $FE 1 stalker-rage
    4 1 $14 door-bits
    5 0 $14 door-bits
    1 2 $2008000 nav-group
    1 old-mansion-b1-2.cmd00
    -57.0 16.0 104.5 1 404232216 4 -15.5 scene-effect-8C
    -61.0 16.0 104.5 1 404232216 4 -15.5 scene-effect-8C
    -57.0 10.0 104.5 1 404232216 4 -9.5 scene-effect-8C
    -61.0 10.0 104.5 1 404232216 4 -9.5 scene-effect-8C
    -57.0 4.0 104.5 1 404232216 4 -3.5 scene-effect-8C
    -61.0 4.0 104.5 1 404232216 4 -3.5 scene-effect-8C
    self-wait-anim
    3 ebit-clear
    self-idle-or-end
;

: old-mansion-b1-2.act08 ( -- )   \ 004096F8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-2.act09 ( -- )   \ 00409710
    self-wait-done
    -109.83 72.62 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message
            wait-message
        else
            $269 story-flag-set
            2 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
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

: old-mansion-b1-2.act0A ( -- )   \ 00409770
    self-wait-done
    13.5 -75.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $290 story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: old-mansion-b1-2.act0B ( -- )   \ 004097D0
    self-wait-done
    13.5 -59.5 self-turn-to-xz
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
            $291 story-flag-set
            1 effect-remove
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

: old-mansion-b1-2.act0C ( -- )   \ 00409830
    self-wait-done
    0 8 8 char-zone-bits? if
        -29.58 75.55 self-turn-to-xz
        self-wait-done
    else
        -133.09 71.35 self-turn-to-xz
        self-wait-done
    then
    4 message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-2.act0D ( -- )   \ 00409860
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $F4 -53.0 -16.0 -90 char-to-xz
    $17 state-flag-set
    1 12.0 0.0 0.0 5.0 event-camera
    5 message
    wait-message
    $17 state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    $1D01 $A self-anim-blend
    6 message
    wait-message
    self-wait-anim
    1 var-inc
    1 2 var? if
        $236 item-give
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-b1-2.enter old-mansion-b1-2 0 room-script!
' old-mansion-b1-2.char-enter old-mansion-b1-2 6 room-script!
' old-mansion-b1-2.phase1 old-mansion-b1-2 1 room-script!
' old-mansion-b1-2.phase2 old-mansion-b1-2 2 room-script!
' old-mansion-b1-2.phase5 old-mansion-b1-2 5 room-script!
' old-mansion-b1-2.phase3 old-mansion-b1-2 3 room-script!
' old-mansion-b1-2.act00 old-mansion-b1-2 $00 action-script!
' old-mansion-b1-2.act01 old-mansion-b1-2 $01 action-script!
' old-mansion-b1-2.act02 old-mansion-b1-2 $02 action-script!
' old-mansion-b1-2.act03 old-mansion-b1-2 $03 action-script!
' old-mansion-b1-2.act04 old-mansion-b1-2 $04 action-script!
' old-mansion-b1-2.act05 old-mansion-b1-2 $05 action-script!
' old-mansion-b1-2.act06 old-mansion-b1-2 $06 action-script!
' old-mansion-b1-2.act07 old-mansion-b1-2 $07 action-script!
' old-mansion-b1-2.act08 old-mansion-b1-2 $08 action-script!
' old-mansion-b1-2.act09 old-mansion-b1-2 $09 action-script!
' old-mansion-b1-2.act0A old-mansion-b1-2 $0A action-script!
' old-mansion-b1-2.act0B old-mansion-b1-2 $0B action-script!
' old-mansion-b1-2.act0C old-mansion-b1-2 $0C action-script!
' old-mansion-b1-2.act0D old-mansion-b1-2 $0D action-script!
