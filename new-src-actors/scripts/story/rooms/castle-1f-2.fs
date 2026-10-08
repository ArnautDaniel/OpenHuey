\ story/rooms/castle-1f-2.fs - the event scripts of room castle-1f-2 ($2; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-2
USING: room-names story.words story.shared flag-names ;

\ the room object pstr_ori's +0x24 by byte 3: 0 set (37.978 with progress flag 0x12, else 11), 1
\ up 0.25 to 37.978 (then event +0x5C (2)), 2 up 0.25, else down 0.25
: castle-1f-2.cmd00 ( b0 -- )  drop s" castle-1f-2.cmd00" stub-step ;
\ room 0x02 (Room02_Cmd01_ptmf): the panic (progress +0x7B8) raised to 80
: castle-1f-2.cmd01 ( -- )  s" castle-1f-2.cmd01" stub-step ;
\ room 0x02 (D_003F03A0): the 0xE60-byte effect OrangeSparks_vtable by byte 3 - 0 made (its slot
\ kept in event variable 0), 1 that one sent 0 (stop), else one more made and sent 1
: castle-1f-2.cmd02 ( b0 -- )  drop s" castle-1f-2.cmd02" stub-step ;
\ room 0x02 (D_003F03B0): the drum can's wobble by byte 3 - 0 still (rest height +0x38 = its
\ height), 2 struck (+0x34 strength 1), 1 each frame: the strength fades by 0.2 while it bobs
\ 0.2 x strength x sin(phase +0x30, on by 90 degrees) about the rest height
: castle-1f-2.cmd03 ( b0 -- )  drop s" castle-1f-2.cmd03" stub-step ;
\ room 0x02 (D_003F03C0): the player is about and down at floor level (y <= 0)
: castle-1f-2.cond00? ( -- flag )  s" castle-1f-2.cond00?" stub-flag ;
\ room 0x02 (Room02_Cond01_ptmf): the pursuer is about, in a mode other than 0, 1 or 5, and
\ progress +0x1130 isn't 0xFE
: castle-1f-2.cond01? ( -- flag )  s" castle-1f-2.cond01?" stub-flag ;

: castle-1f-2.enter ( -- )   \ 003EEDF0
    room-sounds
    0 1 $14 door-bits
    5 1 $14 door-bits
    1 3 $38 nav-group
    0 castle-1f-2.cmd00
    $12 story-flag? not if
        1 4 8 nav-group
        2 0 object-show
        6 0 $14 door-bits
    else
        2 1 object-show
        6 1 $14 door-bits
    then
    $20A story-flag? not if
        1 1 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        $285 story-flag? not if
            0 215.5 0.0 2.6 flicker-sprite
        then
    then
    $20B story-flag? not if
        1 2 $8000000 nav-group
        3 1 $14 door-bits
        4 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        3 0 $14 door-bits
        4 1 $14 door-bits
        $286 story-flag? not if
            2 290.5 0.0 -99.5 flicker-sprite
        then
    then
    0 $C 0.562 0.5 0.25 0.5 zone-rect
    1 $C 0.812 0.687 0.187 0.312 zone-rect
    $24E story-flag? $24F story-flag? not and if
        1 292.2 0.0 -61.8 flicker-sprite
    then
    0 castle-1f-2.cmd02
    3 0 4 $FF $FC 0 $50 1 butterflies
    0 castle-1f-2.cmd03
    8 229.9 29.0 -63.9 $B $80 $80 $80 $30 specks
    $A 111.4 25.0 -58.5 $B $80 $80 $80 $30 specks
    $E 122.8 19.0 103.2 $E $80 $80 $80 $40 specks
    $C 63.3 19.0 151.6 $10 $80 $80 $80 $40 specks
    8 34.2 19.0 92.1 $C $80 $80 $80 $40 specks
    1 $38B $10000000 nav-tri-flags
    1 $1B9 $10000000 nav-tri-flags
    1 $38C $10000000 nav-tri-flags
    1 $38D $10000000 nav-tri-flags
    1 $386 $10000000 nav-tri-flags
    1 $38E $10000000 nav-tri-flags
;

: castle-1f-2.act0E ( -- )   \ 003EFED0
    6 ebit-set
    $12 ebit-set
    4 ebit-clear
    $C 0 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $C 1 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $C 2 pvar? if
        $4B chance? if
            4 ebit-set
        then
    else $C 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    $FE char-unseen? not if
        4 ebit-set
    then
    2 creature-action? if
        4 ebit-set
    then
    4 stalker-alert? if
        4 ebit-clear
    then
    castle-1f-2.cond01? not if
        $FE camera-follow
    then
    4 ebit? if
        0 $FE $F action
    else
        $78 1 item-cooldown
        $16 ebit-clear
    then
    0 stalker-alert? if
        $FE 2 stalker-mode
    then
    $C pvar-inc
    exit
;

: castle-1f-2.char-enter ( -- )   \ 003EEF70
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
                0 5 4 char-camera
                0 camera-follow
            else
                1 5 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 4 char-camera
            0 camera-follow
        else
            1 5 4 char-camera
            1 camera-follow
        then
    then then
    1 5 4 area-camera
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
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    3 3 3 area-camera
    0 self-is? if
        $80 exit-taken? $81 exit-taken? or if
            5 ebit-set
            0 $324 180 char-to-tri-facing
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            0 0 2 action
            $80 exit-taken? if
                1 char-activate
                2 0 691 hewie-to-room
                1 $2B3 50 char-to-tri-facing
            then
        then
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            $FE action-end
            $FE char-done
        then
        1 exit-taken? if
            3 map-page
        then
    then
    $FE self-is? if
        $D ebit-clear
        fiona-hidden state-flag? if
            castle-1f-2.act0E
        else
            $12 ebit-clear
        then
    then
;

: castle-1f-2.phase1 ( -- )   \ 003EF0D0
    $13 ebit? not if
        6 sound-bank-loaded? if
            $40000006 6 276.0 5.0 -146.0 0 0 sound
            $13 ebit-set
        then
    else
        $C0000001 6 110.0 -10.0 142.0 0 0 sound
        $C0000006 6 276.0 5.0 -146.0 0 0 sound
    then
    $34 story-flag? $21 story-flag? not and if
        0 exit-usable? if
            2 3 0 char-load
            0 exit-check
        then
    else 0 exit-usable? if
        0 exit-check
    then then
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
    5 1 1 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 3 3 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 3 3 1 chars-area-camera
    $11 4 -1 1 chars-area-camera
    $16 5 4 1 chars-area-camera
    0 4 char-entered-area? if
        $34 story-flag? $21 story-flag? not and if
            2 3 0 char-load
        then
        0 exit-prepare
    then
    0 5 char-entered-area? 0 6 char-entered-area? or if
        $34 story-flag? $21 story-flag? not and if
            3 0 char-remove
        then
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? 0 7 char-entered-area? or if
        2 exit-prepare
    then
    0 8 char-entered-area? if
        3 exit-prepare
    then
    0 5 char-entered-area? if
        2 map-page
    then
    0 $16 char-entered-area? if
        3 map-page
    then
    $24E story-flag? not if
        8 292.2 -1.0 -61.8 $A 5 0 zone
        1 8 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 590 var-set
                $1A 591 var-set
                $1B 1 var-set
                $1C 292200 var-set
                $1D 0 var-set
                $1E -61800 var-set
                $1F 8 var-set
                0 1 $8B action
            then
        then
    then
    2 276.5 -1.0 -145.9 $E 4 0 zone
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        1 castle-1f-2.cmd02
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        1 castle-1f-2.cmd02
    then
    4 276.4 -1.0 -145.7 5 10 0 zone
    1 castle-1f-2.cmd03
    $14 ebit? not if
        $FF 4 char-in-zone? if
            $14 ebit-set
            $40000007 6 276.0 5.0 -146.0 0 0 sound
            2 castle-1f-2.cmd03
            1 castle-1f-2.cmd02
            2 castle-1f-2.cmd02
            2 castle-1f-2.cmd02
            2 castle-1f-2.cmd02
        then
    else 0 8 char-action? not if
        $14 ebit-clear
    then then
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        0 7 char-left-area? 0 8 char-left-area? or 0 $11 char-left-area? or if
            $17 ebit? not if
                0 $F1 $18 action
            then
        then
    then
    5 ebit? if
        $FE char-here? if
            5 ebit-clear
        then
    then
    $FE 2 char-C4? not if
        fiona-hidden state-flag? 6 ebit? not and $FE char-here? and if
            $FE char-busy? not if
                $16 ebit-set
                castle-1f-2.act0E
            then
        then
    then
    $FE 5 char-in-area? castle-1f-2.cond00? and if
        $D ebit? not $FE 2 char-C4? not and if
            2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
                0 stalker-alert? 2 stalker-alert? or if
                    $FE char-busy? 4 ebit? and fiona-hidden state-flag? and $B ebit? not and if
                        $FE action-end
                    then
                    0 $FE $10 action
                then
            then
        then
    then
    $FE $1A char-in-area? 4 stalker-alert? and $FE 2 char-C4? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 $FE $13 action
        then
    then
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        7 -142.16 0.0 179.0 $4B 20 0 zone
        $A ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 7 9 char-zone-bits? if
                        $A ebit-set
                        $64 chance? if
                            $1F 7 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
    then
    0 ebit? not if
        1 $19 char-in-area? if
            6 5.346 -9.5 80.917 $28 20 0 zone
            9 ebit? not 1 char-here? and 1 0 char-C4? and if
                2 game-mode? not if
                    hewie-can-command? if
                        1 6 9 char-zone-bits? if
                            9 ebit-set
                            $1E chance? if
                                $1F 6 var-set
                                0 1 $88 action
                            then
                        then
                    then
                then
            then
        then
    then
    $12 story-flag? not if
        5 50.0 -10.0 164.549 $1E 20 0 zone
        8 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 5 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 5 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    $C 274.93 -1.0 -145.73 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 $C 9 char-zone-bits? if
                $1F 12 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    $F ebit? not if
        $A 28.0 -9.0 72.0 $10 4 0 zone
        0 $A 3 char-zone-bits? 0 $A 3 char-zone-bits-before? not and if
            0 5 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
        1 $A 3 char-zone-bits? 1 $A 3 char-zone-bits-before? not and if
            1 $A 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
        $FE $A 3 char-zone-bits? $FE $A 3 char-zone-bits-before? not and if
            $FE 5 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
    then
    $10 ebit? not if
        $B 8.0 -9.0 70.0 $10 4 0 zone
        0 $B 3 char-zone-bits? 0 $B 3 char-zone-bits-before? not and if
            0 5 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
        1 $B 3 char-zone-bits? 1 $B 3 char-zone-bits-before? not and if
            1 $A 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
        $FE $B 3 char-zone-bits? $FE $B 3 char-zone-bits-before? not and if
            $FE 5 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
    then
    fiona-hidden state-flag? if
        castle-1f-2.cond01? if
            0 camera-follow
        else
            $FE camera-follow
        then
    then
    35 fiona-started? 45 fiona-started? or 1 2 char-C4? not and $FE char-here? not and 0 $13 char-in-area? and 1 $17 char-in-area? and 0 45 69 $32 char-faces-xz? and 0 ebit? not and if
        hewie-stays? if
            0 1 3 action
        then
    then
    44 fiona-started? 0 $13 char-in-area? and 1 3 char-in-nav-group? and if
        0 1 4 action
    then
    $15 ebit? 1 char-busy? not and if
        $15 ebit-clear
        0 ebit-clear
        stalkers-stay state-flag-clear
    then
;

: castle-1f-2.phase2 ( -- )   \ 003EF5B0
    0 $18 char-in-area? 0 0 $3C char-heading? and if
        $FE char-here? not if
            5 $C 5 scene-change
        else $FE char-unseen? if
            5 $C 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    $12 story-flag? not if
        0 9 char-in-area? 0 22 $32 char-heading? and if
        then
    then
    0 $12 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 $15 char-in-area? 0 0 $32 char-heading? and if
        5 6 0 scene-change
    then
    0 $A char-in-area? 0 -45 $32 char-heading? and if
        5 8 0 scene-change
    then
    0 $14 char-in-area? 0 90 $32 char-heading? and if
        5 9 0 scene-change
    then
    9 276.5 -1.0 -145.9 7 10 0 zone
    0 9 2 char-zone-bits? 0 276 -146 $32 char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 exit-door-open? not 5 ebit? and if
        0 0 char-group-bit4? if
            7 scene-ending
        then
    then
    $24E story-flag? $24F story-flag? not and if
        8 1 5 5 0 zone-at-effect
        0 8 3 char-zone-bits? if
            5 $11 4 scene-change
        then
    then
    $20A story-flag? not if
        0 215.5 -1.0 2.6 5 8 1 zone
        $FF 0 char-in-zone? if
            $20A story-flag-set
            0 1 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            215.5 -1.0 2.6 0 -2145378272 0 0.0 scene-effect-8C
            3 6 215.5 -1.0 2.6 0 0 sound
            $40 $377 noise
            0 215.5 0.0 2.6 flicker-sprite
        then
    else $285 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $14 4 scene-change
        then
    then then
    $20B story-flag? not if
        1 290.5 -1.0 -99.5 5 8 1 zone
        $FF 1 char-in-zone? if
            $20B story-flag-set
            0 2 $8000000 nav-group
            3 0 $14 door-bits
            4 1 $14 door-bits
            290.5 -1.0 -99.5 1 -2145378272 0 0.0 scene-effect-8C
            $88 5 290.5 -1.0 -99.5 0 0 sound
            $40 $380 noise
            2 290.5 0.0 -99.5 flicker-sprite
        then
    else $286 story-flag? not if
        1 2 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $15 4 scene-change
        then
    then then
;

: castle-1f-2.phase3 ( -- )   \ 003EF780
    263.8 8.9 -109.2 263.8 8.9 -145.2 259.799 -39.1 -119.2 258.799 -39.1 -147.2 lights-doorway
    264.8 9.1 -68.9 264.8 9.1 -104.9 261.799 -30.9 -79.9 261.799 -30.9 -90.3 lights-doorway
    210.0 8.0 -71.0 267.0 8.0 -71.0 210.0 -40.0 -75.0 266.0 -40.0 -75.0 lights-doorway
    153.0 10.0 -71.0 211.0 10.0 -71.0 153.0 -38.0 -75.0 210.0 -38.0 -75.0 lights-doorway
    60.0 5.0 57.0 90.0 5.0 57.0 60.0 -40.0 53.0 89.0 -39.0 53.0 lights-doorway
    89.0 8.0 58.0 99.0 5.0 46.0 91.3 -40.0 53.0 99.0 -40.0 42.0 lights-doorway
    -162.0 25.5 122.8 -162.0 25.5 160.5 -162.0 -10.0 123.0 -162.0 -10.0 160.0 lights-doorway
;

: castle-1f-2.phase5 ( -- )   \ 003EF8E0
    room-flag-2d state-flag-clear
    1 char-here? 1 3 char-in-nav-group? and if
        1 $101 char-to-tri
        stalkers-stay state-flag-clear
    then
    0 ebit? if
        stalkers-stay state-flag-clear
    then
    5 ebit? $FE char-here? not and $FE 2 char-C4? and if
        $FE action-end
        2 summon-take
    then
    $FE char-busy? if
        $C ebit? if
            $FE action-end
            $FE $192 180 char-to-tri-facing
            $C ebit-clear
            $B ebit-clear
        else $11 ebit? if
            $FE action-end
            0 summon-take
        then then
    then
;

: castle-1f-2.act00 ( -- )   \ 003EF930
    $12 story-flag? not if
        stalkers-stay state-flag-set
        self-wait-done
        0 6 char-file-load
        $8C 49.9 164.0 0 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 20.0 0.0 0.0 event-camera
        0 message
        wait-message
        0 char-file-use
        begin
            6 sound-bank-loaded? not while
            yield
        repeat
        self-frames-reset
        self-wait-16
        0 0.0 0.0 0.0 0.0 event-camera
        1 self-scripted
        0 answer? if
            2 0 object-anim
            $8000 $A self-anim-blend
            self-frames-reset
            $20 self-wait-frames
            $40000005 6 50.0 -5.0 171.0 0 0 sound
            self-frames-reset
            $3E self-wait-frames
            $40000000 6 50.0 -5.0 171.0 0 0 sound
            self-wait-anim
            $14 door-unlock
            -1 self-move-16
            self-wait-anim
            $12 story-flag-set
            $5F $1F9 noise
            0 $F1 $A action
            $23 story-flag? not if
                1 0 443 4 0 -1 0 0.0 creature-place
                $25 0 217 4 0 -1 0 0.0 creature-place
            then
        then
        0 self-scripted
        0 ebit? not if
            stalkers-stay state-flag-clear
        then
    else
        self-wait-done
        1 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-2.act01 ( -- )   \ 003EFA10
    1 self-scripted
    self-wait-done
    2 wait-counter
    1 item-use
    2 item-use
    10 hewie-trust
    1 message-param-room
    1 1 item-give-count
    0 1 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $827B item-give
    counter-inc
    0 self-scripted
    self-idle-or-end
;

: castle-1f-2.act02 ( -- )   \ 003EFA50
    1 self-scripted
    force-followed state-flag-set
    self-frames-reset
    self-wait-16
    force-followed state-flag-clear
    0 $28 5 char-sound
    world-held state-flag-clear
    $FE $29 129 2 stalker-to-room
    $FE stalker-knock-down
    $F $41 fade
    wait-fade
    $22C item-give
    0 self-scripted
    self-idle-or-end
;

: castle-1f-2.act04 ( -- )   \ 003EFB20
    1 self-scripted
    self-wait-done
    $353 37.0 69.0 90 $FFFF $A self-move-to
    self-wait-done
    0 0 8 nav-group
    $267 52.0 69.0 90 $204 5 self-move-to
    self-wait-done
    1 3 8 nav-group
    -1 self-move-16
    0 self-scripted
    room-flag-2d state-flag-clear
    stalkers-stay state-flag-clear
    0 ebit-clear
    self-idle-or-end
;

: castle-1f-2.act05 ( -- )   \ 003EFB70
    1 wait-counter
    hewie-bark
    self-wait-done
    $1DF $FFFF 6 self-move-tri
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    counter-inc
    3 wait-counter
    0 self-scripted
    self-idle-or-end
;

: castle-1f-2.act03 ( -- )   \ 003EFA80
    stalkers-stay state-flag-set
    0 ebit-set
    $15 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $267 52.0 69.0 -90 $FFFF $A self-move-to
    self-wait-done
    room-flag-2d state-flag-set
    $15 ebit-clear
    1 self-scripted
    0 3 8 nav-group
    $353 40.0 69.0 -90 $204 5 self-move-to
    self-wait-done
    1 0 8 nav-group
    $100 self-anim
    self-wait-anim
    1 self-anim
    self-wait-anim
    begin
        0 $13 char-in-area? if
            44 fiona-started? if
                ['] castle-1f-2.act04 goto
            then
            35 fiona-started? 45 fiona-started? or if
                hewie-stays? if
                    0 counter-set
                    0 $F2 $B action
                    ['] castle-1f-2.act05 goto
                then
            then
        then
        0 $14 char-in-area? if
            44 fiona-started? 35 fiona-started? or 45 fiona-started? or if
                0 counter-set
                0 $F2 $B action
                ['] castle-1f-2.act05 goto
            then
        then
        yield
    again
;

: castle-1f-2.act06 ( -- )   \ 003EFB90
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        stalkers-stay state-flag-set
        self-wait-done
        6 message
        wait-message
        1 self-scripted
        0 answer? if
            $F $44 fade
            $B 3 $FF char-load
            3 char-unload
            $B char-activate
            self-wait-done
            3 0 movie-play
            0 cutscene-start
            yield
            yield
            2 cutscene-control
            begin
                3 cutscene-control
                2 cutscene-mode? not while
                yield
            repeat
            wait-fade
            0 $F9 $16 action
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $B 1 char-no-shadow
            1 0.9 camera-value
            $FF panic-stage? if
                3 panic-stage
            then
            $FFFF message-prepare
            1 result? if
                6 cutscene-control
                1.0 movie-volume
                effects-arena-flip
                yield
                movie-skipped state-flag-clear
                $F 1 fade
                $A cutscene-control
                begin
                    movie-skipped state-flag? not if
                        7 cutscene-control
                        cutscene-near-end? if
                            $F 0 fade
                        then
                        $C cutscene-control
                        0 cutscene-control
                        4 cutscene-control
                        -1 result? not if
                            5 cutscene-mode? not 4 cutscene-mode? not and if
                                yield
                                false
                            else
                                true
                            then
                        else
                            true
                        then
                    else
                        true
                    then
                until
                $FFFF message-close
                $F9 action-end
                $B cutscene-control
                wait-fade
                1 cutscene-control
                world-held state-flag-set
                effects-arena-flip
                begin
                    movie-playing? while
                    yield
                repeat
                yield
                pause-wanted state-flag-set
                8 cutscene-control
            then
            1 1.0 camera-value
            0 self-move-16
            0 $EE -141.823 160.482 -2 char-to-xz
            camera-restart
            0 0 char-no-shadow
            $B 0 char-no-shadow
            1 0 char-visible
            1 action-end
            3 0 char-remove
            1 1 object-show
            castle-1f-2.cmd01
            $F $41 fade
            wait-fade
            $27 story-flag-set
            $24 resident-flag-set
            $A1 message-param-room
            $A1 1 item-give-count
            0 $A1 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        0 ebit? not if
            stalkers-stay state-flag-clear
        then
        0 self-scripted
    else
        self-wait-done
        $27 story-flag? not if
            2 message
            wait-message
        else
            $1D02 $A self-anim-blend
            3 message
            wait-message
            self-wait-anim
        then
    then
    self-idle-or-end
;

: castle-1f-2.act07 ( -- )   \ 0047A988
    self-idle-or-end
;

: castle-1f-2.act08 ( -- )   \ 003EFCD0
    self-wait-done
    1 ebit? not if
        $A02 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        1 ebit-set
    else
        $A01 $A self-anim-blend
        5 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-2.act09 ( -- )   \ 003EFCF0
    self-wait-done
    $7D 6.874 90.0 180 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 10.0 20.0 0.0 0.0 event-camera
    5 0 $14 door-bits
    4 message
    wait-message
    self-frames-reset
    self-wait-16
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    5 1 $14 door-bits
    0 self-scripted
    self-idle-or-end
;

: castle-1f-2.act0A ( -- )   \ 003EFD50
    $40000001 6 110.0 -10.0 142.0 0 0 sound
    self-frames-reset
    begin
        $40 frames? not while
        1 castle-1f-2.cmd00
        yield
    repeat
    0 4 8 nav-group
    begin
        2 ebit? not while
        1 castle-1f-2.cmd00
        yield
    repeat
    $40000002 6 110.0 -10.0 142.0 0 0 sound
    2 castle-1f-2.cmd00
    self-frames-reset
    4 self-wait-frames
    3 castle-1f-2.cmd00
    self-frames-reset
    4 self-wait-frames
    2 castle-1f-2.cmd00
    self-frames-reset
    4 self-wait-frames
    3 castle-1f-2.cmd00
    self-frames-reset
    4 self-wait-frames
    2 castle-1f-2.cmd00
    self-frames-reset
    4 self-wait-frames
    3 castle-1f-2.cmd00
    self-idle-or-end
;

: castle-1f-2.act0B ( -- )   \ 003EFDD0
    begin
        fiona-free? not while
        yield
    repeat
    counter-inc
    $FF panic-stage? if
        3 panic-stage
    then
    0 0 1 action-force
    self-idle-or-end
;

: castle-1f-2.act0D ( -- )   \ 003EFE90
    3 ebit? if
        2 avoid-prompt
    then
    $FF panic-stage? not if
        0 $43 5 char-sound
    then
    1 self-scripted
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    $FE char-busy? $B ebit? not and $11 ebit? not and if
        3 ebit? not if
            $FE action-end
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: castle-1f-2.act0C ( -- )   \ 003EFDF0
    stalkers-stay state-flag-set
    $12 ebit-clear
    1 self-scripted
    6 ebit-clear
    0 4 char-file-load
    self-wait-done
    $FF self-look-at
    yield
    0 char-file-use
    $132 $8004 5 240.0 -17.59 0 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    0 ebit? not if
        stalkers-stay state-flag-clear
    then
    fiona-hidden state-flag-set
    3 ebit-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] castle-1f-2.act0D goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                $12 ebit? $FE char-here? not and if
                    $12 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] castle-1f-2.act0D goto
            then
            0 camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-1f-2.act0F ( -- )   \ 003EFF40
    self-wait-done
    $16 ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and if
            1 3 self-move-slot
            self-wait-done
        then
    else
        $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
            0 3 self-move-slot
            self-wait-done
        then
        $FE 1 char-heading-for? 1 exit-door-open? not and $FE 1 char-in-area? and if
            1 3 self-move-slot
            self-wait-done
        then
    then
    $16 ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE 5 char-file-load
    then
    $132 237.262 -39.055 0 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        $FE char-file-use
        $8000 $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        3 ebit-set
        self-wait-anim
    else
        $1601 self-anim
        self-wait-anim
        3 ebit-set
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: castle-1f-2.act10 ( -- )   \ 003EFFD0
    $B ebit-set
    self-wait-done
    $FE 8 char-file-load
    $11 151.535 -15.151 180 $FFFF 5 self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    $C ebit-set
    $8000 5 self-anim-9
    1 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        1 49 var? if
            0 153.0 -3.7 -51.0 0 0 0 0 dust
            0 148.0 -4.0 -51.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $FE 0 0 char-camera
    0 5 self-anim-blend
    self-wait-anim
    0 self-noclip
    0 self-scripted
    $C ebit-clear
    $B ebit-clear
    6 ebit-clear
    $D ebit-set
    self-idle-or-end
;

: castle-1f-2.act11 ( -- )   \ 003F0060
    self-wait-done
    292.2 -61.8 self-turn-to-xz
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
            $24F story-flag-set
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

: castle-1f-2.act12 ( -- )   \ 003F00C0
    self-wait-done
    276.5 -145.9 self-turn-to-xz
    self-wait-done
    $E ebit? not if
        8 message
        wait-message
        $E ebit-set
    else
        9 message
        wait-message
    then
    self-idle-or-end
;

: castle-1f-2.act13 ( -- )   \ 003F00E0
    $11 ebit-set
    self-wait-done
    $FE 8 char-file-load
    $381 279.332 -83.7 90 $FFFF $A self-move-to
    self-wait-done
    $FE char-file-use
    1 self-noclip
    1 self-scripted
    $8001 5 self-anim-9
    1 0 var-set
    begin
        0 self-touching? if
            $FE 0 fiona-thrown
        then
        1 54 var? if
            0 306.0 23.0 -79.0 0 0 0 0 dust
            0 306.0 23.0 -85.0 0 0 0 0 dust
            $FE $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    $395 341.992 -29.2 90 $FFFF $A self-move-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    $11 ebit-clear
    $FE action-end
    0 summon-take
    self-idle-or-end
;

: castle-1f-2.act14 ( -- )   \ 003F0170
    self-wait-done
    215.5 2.6 self-turn-to-xz
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
            $285 story-flag-set
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

: castle-1f-2.act15 ( -- )   \ 003F01D0
    self-wait-done
    290.5 -99.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $62 message-param-room
        $62 $63 item-count? if
            $8010 message
            wait-message
        else
            $286 story-flag-set
            2 effect-remove
            $62 1 item-give-count
            0 $62 item-tab
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

: castle-1f-2.act16 ( -- )   \ 003F0230
    begin
        2 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        8 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: castle-1f-2.act17 ( -- )   \ 003F0250
    self-wait-done
    $B 3 $FF char-load
    3 char-unload
    3 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $16 action
    1 0.9 camera-value
    $B 1 char-no-shadow
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    1 1.0 camera-value
    0 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: castle-1f-2.act18 ( -- )   \ 003F02F0
    $17 ebit-set
    $40000004 6 -142.0 12.0 179.0 0 0 sound
    self-frames-reset
    $4B self-wait-frames
    $17 ebit-clear
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-2.enter castle-1f-2 0 room-script!
' castle-1f-2.char-enter castle-1f-2 6 room-script!
' castle-1f-2.phase1 castle-1f-2 1 room-script!
' castle-1f-2.phase2 castle-1f-2 2 room-script!
' castle-1f-2.phase3 castle-1f-2 3 room-script!
' castle-1f-2.phase5 castle-1f-2 5 room-script!
' castle-1f-2.act00 castle-1f-2 $00 action-script!
' castle-1f-2.act01 castle-1f-2 $01 action-script!
' castle-1f-2.act02 castle-1f-2 $02 action-script!
' castle-1f-2.act03 castle-1f-2 $03 action-script!
' castle-1f-2.act04 castle-1f-2 $04 action-script!
' castle-1f-2.act05 castle-1f-2 $05 action-script!
' castle-1f-2.act06 castle-1f-2 $06 action-script!
' castle-1f-2.act07 castle-1f-2 $07 action-script!
' castle-1f-2.act08 castle-1f-2 $08 action-script!
' castle-1f-2.act09 castle-1f-2 $09 action-script!
' castle-1f-2.act0A castle-1f-2 $0A action-script!
' castle-1f-2.act0B castle-1f-2 $0B action-script!
' castle-1f-2.act0C castle-1f-2 $0C action-script!
' castle-1f-2.act0D castle-1f-2 $0D action-script!
' castle-1f-2.act0E castle-1f-2 $0E action-script!
' castle-1f-2.act0F castle-1f-2 $0F action-script!
' castle-1f-2.act10 castle-1f-2 $10 action-script!
' castle-1f-2.act11 castle-1f-2 $11 action-script!
' castle-1f-2.act12 castle-1f-2 $12 action-script!
' castle-1f-2.act13 castle-1f-2 $13 action-script!
' castle-1f-2.act14 castle-1f-2 $14 action-script!
' castle-1f-2.act15 castle-1f-2 $15 action-script!
' castle-1f-2.act16 castle-1f-2 $16 action-script!
' castle-1f-2.act17 castle-1f-2 $17 action-script!
' castle-1f-2.act18 castle-1f-2 $18 action-script!
