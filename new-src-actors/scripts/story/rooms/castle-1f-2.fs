\ story/rooms/castle-1f-2.fs - the event scripts of room castle-1f-2 ($2; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-2
USING: actors messages room-names story.words story.shared flag-names ;

\ the room object pstr_ori's +0x24 by byte 3: 0 set (37.978 with progress flag 0x12, else 11), 1
\ up 0.25 to 37.978 (then event +0x5C (2)), 2 up 0.25, else down 0.25
: castle-1f-2.cmd00 ( b0 -- )   \ (the cage, "ori": 0 set - up (37.978) once story flag $12, else 11; 1 up 0.25 to the
    \ top, there event flag 2; 2 up 0.25; else down 0.25)
    s" ori" object-here? 0= if  drop exit  then
    case
        0 of  $12 story-flag? if  37.978e  else  11e  then  s" ori" place-y object!  endof
        1 of  0.25e s" ori" place-y object+!
              s" ori" place-y object@ 37.978e f< 0= if  2 ebit-set  37.978e s" ori" place-y object!  then  endof
        2 of  0.25e s" ori" place-y object+!  endof
        >r -0.25e s" ori" place-y object+! r>
    endcase ;
\ room 0x02 (Room02_Cmd01_ptmf): the panic (progress +0x7B8) raised to 80
: castle-1f-2.cmd01 ( -- )  80 panic-raise-to ;   \ (her panic up to 80: Panic_FrightRaw)
\ room 0x02 (D_003F03A0): the 0xE60-byte effect OrangeSparks_vtable by byte 3 - 0 made (its slot
\ kept in event variable 0), 1 that one sent 0 (stop), else one more made and sent 1
: castle-1f-2.cmd02 ( b0 -- )   \ (the fireplace's embers: b0 0 lit - kept in variable 0; 1 a gust blows them back;
    \ 2 another lot flung away as it goes out)
    case
        0 of  0  -1 effects:embers  var-set  endof
        1 of  0 var @ dup alive? if  0 swap send effect-command  else  drop  then  endof
        >r  1 effects:embers drop  r>
    endcase ;
\ room 0x02 (D_003F03B0): the drum can's wobble by byte 3 - 0 still (rest height +0x38 = its
\ height), 2 struck (+0x34 strength 1), 1 each frame: the strength fades by 0.2 while it bobs
\ 0.2 x strength x sin(phase +0x30, on by 90 degrees) about the rest height
: castle-1f-2.cmd03 ( b0 -- )   \ (the drum can, "doramukan": 0 still, its rest height kept (+$38); 2 struck (strength 1);
    \ 1 a frame of its wobble - strength down 0.2, phase on 90 degrees, height rest + 0.2 x strength x sin)
    s" doramukan" object-here? 0= if  drop exit  then
    case
        2 of  0 s" doramukan" $30 object-n!  1e s" doramukan" $34 object!  endof
        1 of
            -0.2e s" doramukan" $34 object+!
            s" doramukan" $34 object@ f0> 0= if
                0e s" doramukan" $34 object!  0e s" doramukan" $30 object!
                s" doramukan" $38 object@ s" doramukan" place-y object!
            else
                90e s" doramukan" $30 object+!
                s" doramukan" $30 object@ 360e f< 0= if  -360e s" doramukan" $30 object+!  then
                s" doramukan" $34 object@ s" doramukan" $30 object@ deg>rad fsin f* 0.2e f*
                s" doramukan" $38 object@ f+ s" doramukan" place-y object!
            then  endof
        0 of  0 s" doramukan" $34 object-n!  0 s" doramukan" $30 object-n!
              s" doramukan" place-y object@ s" doramukan" $38 object!  endof
    endcase ;
\ room 0x02 (D_003F03C0): the player is about and down at floor level (y <= 0)
: castle-1f-2.cond00? ( -- flag )  fiona-at 4 + sf@ 0e f<= ;
\ room 0x02 (Room02_Cond01_ptmf): the pursuer is about, in a mode other than 0, 1 or 5, and
\ progress +0x1130 isn't 0xFE
: castle-1f-2.cond01? ( -- flag )
    stalker-about? 0= if  false exit  then
    stalker-move-mode dup 0= over 1 = or swap 5 = or if  false exit  then
    camera-on $FE <> ;

: castle-1f-2.enter ( -- )   \ 003EEDF0
    room-sounds
    0 1 $14 door-bits
    5 1 $14 door-bits
    1 3 $38 nav-group
    0 castle-1f-2.cmd00
    $12 story-flag? not if
        1 4 8 nav-group
        2 0 object-hidden
        6 0 $14 door-bits
    else
        2 1 object-hidden
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
    stalker-who char-unseen? not if
        4 ebit-set
    then
    2 creature-action? if
        4 ebit-set
    then
    4 stalker-alert? if
        4 ebit-clear
    then
    castle-1f-2.cond01? not if
        stalker-who camera-follow
    then
    4 ebit? if
        0 stalker-who $F action
    else
        $78 1 summoner-wait
        $16 ebit-clear
    then
    0 stalker-alert? if
        stalker-who 2 stalker-command
    then
    $C pvar-inc
    exit
;

: castle-1f-2.char-enter ( -- )   \ 003EEF70
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            5 4 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        5 4 player-camera
    then then
    1 5 4 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        3 3 player-camera
    then then
    3 3 3 area-camera
    fiona-who self-is? if
        $80 exit-taken? $81 exit-taken? or if
            5 ebit-set
            fiona-who $324 180 char-to-tri-facing
            fiona-who 0 char-to-exit
            0 0 player-camera
            camera-restart
            0 fiona-who 2 action
            $80 exit-taken? if
                hewie-who char-activate
                2 0 691 hewie-to-room
                hewie-who $2B3 50 char-to-tri-facing
            then
        then
        $42 story-flag? $5B story-flag? not and if
            $5B story-flag-set
            stalker-who action-end
            stalker-who char-done
        then
        1 exit-taken? if
            3 map-page
        then
    then
    stalker-who self-is? if
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
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
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
    fiona-who 4 char-entered-area? if
        $34 story-flag? $21 story-flag? not and if
            2 3 0 char-load
        then
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 6 char-entered-area? or if
        $34 story-flag? $21 story-flag? not and if
            3 0 char-remove
        then
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? fiona-who 7 char-entered-area? or if
        2 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        2 map-page
    then
    fiona-who $16 char-entered-area? if
        3 map-page
    then
    $24E story-flag? not if
        8 292.2 -1.0 -61.8 $A 5 0 zone
        hewie-who 8 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 590 var-set
                $1A 591 var-set
                $1B 1 var-set
                $1C 292200 var-set
                $1D 0 var-set
                $1E -61800 var-set
                $1F 8 var-set
                0 hewie-who $8B action
            then
        then
    then
    2 276.5 -1.0 -145.9 $E 4 0 zone
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        1 castle-1f-2.cmd02
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        1 castle-1f-2.cmd02
    then
    4 276.4 -1.0 -145.7 5 10 0 zone
    1 castle-1f-2.cmd03
    $14 ebit? not if
        self-who 4 char-in-zone? if
            $14 ebit-set
            $40000007 6 276.0 5.0 -146.0 0 0 sound
            2 castle-1f-2.cmd03
            1 castle-1f-2.cmd02
            2 castle-1f-2.cmd02
            2 castle-1f-2.cmd02
            2 castle-1f-2.cmd02
        then
    else fiona-who 8 char-action? not if
        $14 ebit-clear
    then then
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        fiona-who 7 char-left-area? fiona-who 8 char-left-area? or fiona-who $11 char-left-area? or if
            $17 ebit? not if
                0 $F1 $18 action
            then
        then
    then
    5 ebit? if
        stalker-who char-here? if
            5 ebit-clear
        then
    then
    stalker-who 2 char-cond? not if
        fiona-hidden state-flag? 6 ebit? not and stalker-who char-here? and if
            stalker-who char-busy? not if
                $16 ebit-set
                castle-1f-2.act0E
            then
        then
    then
    stalker-who 5 char-in-area? castle-1f-2.cond00? and if
        $D ebit? not stalker-who 2 char-cond? not and if
            2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
                0 stalker-alert? 2 stalker-alert? or if
                    stalker-who char-busy? 4 ebit? and fiona-hidden state-flag? and $B ebit? not and if
                        stalker-who action-end
                    then
                    0 stalker-who $10 action
                then
            then
        then
    then
    stalker-who $1A char-in-area? 4 stalker-alert? and stalker-who 2 char-cond? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 stalker-who $13 action
        then
    then
    8 story-flag? $27 story-flag? not and $26 story-flag? not and if
        7 -142.16 0.0 179.0 $4B 20 0 zone
        $A ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 7 9 char-zone-bits? if
                        $A ebit-set
                        $64 chance? if
                            $1F 7 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
    then
    0 ebit? not if
        hewie-who $19 char-in-area? if
            6 5.346 -9.5 80.917 $28 20 0 zone
            9 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 6 9 char-zone-bits? if
                            9 ebit-set
                            $1E chance? if
                                $1F 6 var-set
                                0 hewie-who $88 action
                            then
                        then
                    then
                then
            then
        then
    then
    $12 story-flag? not if
        5 50.0 -10.0 164.549 $1E 20 0 zone
        8 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 5 9 char-zone-bits? if
                        8 ebit-set
                        $64 chance? if
                            $1F 5 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    $C 274.93 -1.0 -145.73 $14 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who $C 9 char-zone-bits? if
                $1F 12 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    $F ebit? not if
        $A 28.0 -9.0 72.0 $10 4 0 zone
        fiona-who $A 3 char-zone-bits? fiona-who $A 3 char-zone-bits-before? not and if
            fiona-who 5 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
        hewie-who $A 3 char-zone-bits? hewie-who $A 3 char-zone-bits-before? not and if
            hewie-who $A 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
        stalker-who $A 3 char-zone-bits? stalker-who $A 3 char-zone-bits-before? not and if
            stalker-who 5 28.0 -9.0 72.0 2 $40 $50 $40 $80 splash
            $F ebit-set
        then
    then
    $10 ebit? not if
        $B 8.0 -9.0 70.0 $10 4 0 zone
        fiona-who $B 3 char-zone-bits? fiona-who $B 3 char-zone-bits-before? not and if
            fiona-who 5 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
        hewie-who $B 3 char-zone-bits? hewie-who $B 3 char-zone-bits-before? not and if
            hewie-who $A 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
        stalker-who $B 3 char-zone-bits? stalker-who $B 3 char-zone-bits-before? not and if
            stalker-who 5 8.0 -9.0 70.0 2 $40 $50 $40 $80 splash
            $10 ebit-set
        then
    then
    fiona-hidden state-flag? if
        castle-1f-2.cond01? if
            fiona-who camera-follow
        else
            stalker-who camera-follow
        then
    then
    35 fiona-started? 45 fiona-started? or hewie-who 2 char-cond? not and stalker-who char-here? not and fiona-who $13 char-in-area? and hewie-who $17 char-in-area? and fiona-who 45 69 $32 char-faces-xz? and 0 ebit? not and if
        hewie-may-break-off? if
            0 hewie-who 3 action
        then
    then
    44 fiona-started? fiona-who $13 char-in-area? and hewie-who 3 char-in-nav-group? and if
        0 hewie-who 4 action
    then
    $15 ebit? hewie-who char-busy? not and if
        $15 ebit-clear
        0 ebit-clear
        stalkers-stay state-flag-clear
    then
;

: castle-1f-2.phase2 ( -- )   \ 003EF5B0
    fiona-who $18 char-in-area? fiona-who 0 $3C char-heading? and if
        stalker-who char-here? not if
            5 $C 5 scene-change
        else stalker-who char-unseen? if
            5 $C 5 scene-change
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then then
    then
    $12 story-flag? not if
        fiona-who 9 char-in-area? fiona-who 22 $32 char-heading? and if
        then
    then
    fiona-who $12 char-in-area? fiona-who 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    fiona-who $15 char-in-area? fiona-who 0 $32 char-heading? and if
        5 6 0 scene-change
    then
    fiona-who $A char-in-area? fiona-who -45 $32 char-heading? and if
        5 8 0 scene-change
    then
    fiona-who $14 char-in-area? fiona-who 90 $32 char-heading? and if
        5 9 0 scene-change
    then
    9 276.5 -1.0 -145.9 7 10 0 zone
    fiona-who 9 2 char-zone-bits? fiona-who 276 -146 $32 char-faces-xz? and if
        5 $12 0 scene-change
    then
    0 exit-door-open? not 5 ebit? and if
        fiona-who 0 char-before-door? if
            7 examine-message   \ "There is no way I am going back there now!"
        then
    then
    $24E story-flag? $24F story-flag? not and if
        8 1 5 5 0 zone-at-effect
        fiona-who 8 3 char-zone-bits? if
            5 $11 4 scene-change
        then
    then
    $20A story-flag? not if
        0 215.5 -1.0 2.6 5 8 1 zone
        self-who 0 char-in-zone? if
            $20A story-flag-set
            0 1 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            215.5 -1.0 2.6 0 -2145378272 0 0.0 shards-at
            3 6 215.5 -1.0 2.6 0 0 sound
            $40 $377 noise
            0 215.5 0.0 2.6 flicker-sprite
        then
    else $285 story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 $14 4 scene-change
        then
    then then
    $20B story-flag? not if
        1 290.5 -1.0 -99.5 5 8 1 zone
        self-who 1 char-in-zone? if
            $20B story-flag-set
            0 2 $8000000 nav-group
            3 0 $14 door-bits
            4 1 $14 door-bits
            290.5 -1.0 -99.5 1 -2145378272 0 0.0 shards-at
            $88 5 290.5 -1.0 -99.5 0 0 sound
            $40 $380 noise
            2 290.5 0.0 -99.5 flicker-sprite
        then
    else $286 story-flag? not if
        1 2 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
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
    hewie-who char-here? hewie-who 3 char-in-nav-group? and if
        hewie-who $101 char-to-tri
        stalkers-stay state-flag-clear
    then
    0 ebit? if
        stalkers-stay state-flag-clear
    then
    5 ebit? stalker-who char-here? not and stalker-who 2 char-cond? and if
        stalker-who action-end
        2 summon-take
    then
    stalker-who char-busy? if
        $C ebit? if
            stalker-who action-end
            stalker-who $192 180 char-to-tri-facing
            $C ebit-clear
            $B ebit-clear
        else $11 ebit? if
            stalker-who action-end
            0 summon-take
        then then
    then
;

: castle-1f-2.act00 ( -- )   \ 003EF930
    $12 story-flag? not if
        stalkers-stay state-flag-set
        self-wait-done
        fiona-who 6 char-file-load
        $8C 49.9 164.0 0 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 20.0 0.0 0.0 event-camera
        0 message   \ "Looks like this is used to open the gate. Pull the lever? YES NO"
        wait-message
        fiona-who char-file-use
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
            -1 self-idle-anim
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
        1 message   \ "The lever has already been pulled."
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
    item-fanfare
    $8012 message   \ "You've obtained ."
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
    fiona-who $28 5 char-sound
    world-held state-flag-clear
    $FE $29 129 2 stalker-to-room
    stalker-who stalker-knock-down
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
    -1 self-idle-anim
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
        fiona-who $13 char-in-area? if
            44 fiona-started? if
                ['] castle-1f-2.act04 goto
            then
            35 fiona-started? 45 fiona-started? or if
                hewie-may-break-off? if
                    0 counter-set
                    0 $F2 $B action
                    ['] castle-1f-2.act05 goto
                then
            then
        then
        fiona-who $14 char-in-area? if
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
        6 message   \ "Maybe I am hearing things, but I'd swear there are voices coming from the hole. Sea..."
        wait-message
        1 self-scripted
        0 answer? if
            $F $44 fade
            $B 3 $FF char-load
            3 char-start
            $B char-activate
            self-wait-done
            3 0 movie-play
            0 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            0 $F9 $16 action
            hewie-who char-here? hewie-who 2 char-cond? and if
                0 hewie-who $86 action
            then
            $B 1 char-no-shadow
            1 0.9 camera-value
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            1 1.0 camera-value
            0 self-idle-anim
            fiona-who $EE -141.823 160.482 -2 char-to-xz
            camera-restart
            fiona-who 0 char-no-shadow
            $B 0 char-no-shadow
            hewie-who 0 char-visible
            hewie-who action-end
            3 0 char-remove
            1 1 object-hidden
            castle-1f-2.cmd01
            $F $41 fade
            wait-fade
            $27 story-flag-set
            $24 resident-flag-set
            $A1 message-param-room
            $A1 1 item-give-count
            0 $A1 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        0 ebit? not if
            stalkers-stay state-flag-clear
        then
        0 self-scripted
    else
        self-wait-done
        $27 story-flag? not if
            2 message   \ "There's a breeze coming from the other side. I wonder where it leads..."
            wait-message
        else
            $1D02 $A self-anim-blend
            3 message   \ "That is the last time I put my hand in there."
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
        4 message   \ "There are beautiful flowers blooming here."
        wait-message
        self-wait-anim
        1 ebit-set
    else
        $A01 $A self-anim-blend
        5 message   \ "A section of the fence is broken. There is a hole..."
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
    4 message   \ "There are beautiful flowers blooming here."
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
    panic-eased
    0 fiona-who 1 action-force
    self-idle-or-end
;

: castle-1f-2.act0D ( -- )   \ 003EFE90
    3 ebit? if
        2 avoid-prompt
    then
    $FF panic-stage? not if
        fiona-who $43 5 char-sound
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    stalker-who char-busy? $B ebit? not and $11 ebit? not and if
        3 ebit? not if
            stalker-who action-end
            stalker-who 0 stalker-command
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
    fiona-who 4 char-file-load
    self-wait-done
    self-who self-look-at
    yield
    fiona-who char-file-use
    $132 $8004 5 240.0 -17.59 0 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
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
                    fiona-who $43 5 char-sound
                then
                ['] castle-1f-2.act0D goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                $12 ebit? stalker-who char-here? not and if
                    $12 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] castle-1f-2.act0D goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
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
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    else
        stalker-who 0 char-heading-for? 0 exit-door-open? not and stalker-who 0 char-in-area? and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        stalker-who 1 char-heading-for? 1 exit-door-open? not and stalker-who 1 char-in-area? and if
            hewie-who 3 self-move-slot
            self-wait-done
        then
    then
    $16 ebit-clear
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who 5 char-file-load
    then
    $132 237.262 -39.055 0 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who char-file-use
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
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-1f-2.act10 ( -- )   \ 003EFFD0
    $B ebit-set
    self-wait-done
    stalker-who 8 char-file-load
    $11 151.535 -15.151 180 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-file-use
    1 self-noclip
    1 self-scripted
    $C ebit-set
    $8000 5 self-anim-blend8
    1 0 var-set
    begin
        fiona-who self-touching? if
            stalker-who 0 fiona-thrown
        then
        1 49 var? if
            0 153.0 -3.7 -51.0 0 0 0 0 dust
            0 148.0 -4.0 -51.0 0 0 0 0 dust
            stalker-who $B 6 char-sound
        then
        1 var-inc
        self-at-motion-event? not while
        yield
    repeat
    stalker-who 0 0 char-camera
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
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $24F story-flag-set
            1 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
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
        8 message   \ "A fire is crisply burning."
        wait-message
        $E ebit-set
    else
        9 message   \ "Something's inside. It's a... ripped apart doll?!"
        wait-message
    then
    self-idle-or-end
;

: castle-1f-2.act13 ( -- )   \ 003F00E0
    $11 ebit-set
    self-wait-done
    stalker-who 8 char-file-load
    $381 279.332 -83.7 90 $FFFF $A self-move-to
    self-wait-done
    stalker-who char-file-use
    1 self-noclip
    1 self-scripted
    $8001 5 self-anim-blend8
    1 0 var-set
    begin
        fiona-who self-touching? if
            stalker-who 0 fiona-thrown
        then
        1 54 var? if
            0 306.0 23.0 -79.0 0 0 0 0 dust
            0 306.0 23.0 -85.0 0 0 0 0 dust
            stalker-who $B 6 char-sound
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
    stalker-who action-end
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
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $285 story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
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
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $286 story-flag-set
            2 effect-remove
            $62 1 item-give-count
            0 $62 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
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
    fiona-who 1 char-no-shadow
    begin
        8 cutscene-shot? not while
        yield
    repeat
    fiona-who 0 char-no-shadow
    self-idle-or-end
;

: castle-1f-2.act17 ( -- )   \ 003F0250
    self-wait-done
    $B 3 $FF char-load
    3 char-start
    3 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $16 action
    1 0.9 camera-value
    $B 1 char-no-shadow
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 1.0 camera-value
    fiona-who 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
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
