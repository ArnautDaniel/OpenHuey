\ story/rooms/old-mansion-1f-8.fs - the event scripts of room old-mansion-1f-8 ($48; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-8
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds hewie.anims ;

\ room 0x48 (D_004267F8): the handler's objects 2 and 3 swing (phases 60 degrees apart) - byte 3
\ 0 sets them still; 1: while slower than 5, Hewie's movement (the squared length of his last
\ step, +0x3C) past 1 makes them swing for 20 frames (the first also creaks: sounds 4 / 5 by
\ turns, event bit 0x11); the tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36
: old-mansion-1f-8.cmd00 ( b0 -- )   \ (the room's things 2 and 3, 60 degrees apart, swinging as Hewie brushes past: 0 still; 1
    \ while slower than 5, his step's square past 1 swings them 20 frames, the first creaking - her
    \ sounds 4 / 5 by turns, event flag $11)
    {: op | a n :}
    2 0 do  i 2 + room-object to n to a  a n object-here? if
        op 0= if  0e a n $30 object!  60e i s>f f* a n $34 object!  0e a n $38 object!  0e a n $3C object!  then
        op 1 = if
            a n $38 object@ 5e f< if
                1 char-step fdup f* a n $3C object+!
                a n $3C object@ 1e f> if
                    20e a n $38 object!  0e a n $3C object!
                    i 0= if  $11 ebit? if  $11 ebit-clear  fiona-who 4 6 char-sound  else  $11 ebit-set  fiona-who 5 6 char-sound  then  then
                then
            then
            a n $38 object@ f0> if
                a n $38 object@ 1e f- 0e fmax a n $38 object!
                36e a n $30 object+!
                a n $30 object@ a n $34 object@ f+ 360e f< 0= if  -360e a n $30 object+!  then
                a n $30 object@ a n $34 object@ f+ deg>rad fsin 1e f+ deg>rad
                fdup pi f> if  2e pi f* f-  then  a n turn-x object!
            then
        then
    then  loop ;
\ room 0x48 (Room48_Cmd01_ptmf): character 0x26's +0xE4 cleared
: old-mansion-1f-8.cmd01 ( -- )  $26 false doorway-fade! ;   \ (character $26 no longer fading in doorways)
\ room 0x48 (D_00426818): byte 3 0 starts the wall shadow (WallShadow_vtable, its slot in event
\ variable 2); else that one is ended
: old-mansion-1f-8.cmd02 ( b0 -- )  2 ['] effects:wall-shadow effect-in-var ;   \ (a shadow on the wall: b0 0 cast - kept in variable 2 - else gone)
\ room 0x48 (Room48_Cmd03_ptmf): character 0xFE's model +0x9E8 = -0.15 (byte 3 0) or 0
: old-mansion-1f-8.cmd03 ( b0 -- )  drop ;   \ (the stalker's model's springs (+0x9E8): not simulated here yet)
\ room 0x48 (Room48_Cmd05_ptmf): the creatures (10) in play in the current room: the list's
\ +0x2C
: old-mansion-1f-8.cmd05 ( -- )  creatures-to-level0 ;
\ room 0x48 (Room48_Cond00_ptmf): door 0 of room 0x48 (Progress_CurRoomFlag)
: old-mansion-1f-8.cond00? ( -- flag )  $48 0 room-door-flag? ;

: old-mansion-1f-8.enter ( -- )   \ 004252D0
    room-sounds
    $19 1.0 0 bgm
    0 both-halves floor-on
    $7D door-locked? if
        4 other-half floor-off
        4 one-half floor-on
    then
    om1f-8-beef-jerky story-flag? not if
        0 122.09 8.28 10.26 flicker-sprite
    then
    om1f-5-lavender story-flag? not if
        1 -131.47 1.0 9.32 flicker-sprite
    then
    om1f-8-album-medallion story-flag? not if
        2 -140.37 16.0 -25.97 flicker-sprite
    then
    8 1 item-count? not 9 1 item-count? not and $A 1 item-count? not and $B 1 item-count? not and $C 1 item-count? not and $10 1 item-count? not and if
        0 ebit-set
        3 -111.0 17.0 -115.0 flicker-sprite
    then
    om1f-12-flag1 story-flag? if
        om1f-8-flag1 story-flag? not if
            om1f-8-flag1 story-flag-set
            $48 0 97 4 8 -1 0 0.0 creature-place
            $48 0 488 4 8 -1 0 0.0 creature-place
        then
    then
    hewie-who char-here? 119 hewie-action? and if
        2 solid-floor floor-off
        2 keeps-fiona-off keeps-pursuers-off or floor-edge or floor-on
        hewie-who 8 char-on-tri? not if
            hewie-who 8 76.218 -13.075 90 char-to-xz
            1800 2 hewie-anim
        then
    else
        2 keeps-fiona-off keeps-pursuers-off or floor-off
        2 solid-floor floor-edge or floor-on
        hewie-who 2 char-in-nav-group? if
            hewie-who $175 char-to-tri
        then
    then
    0 old-mansion-1f-8.cmd00
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    om1f-13-flag2 story-flag? white-mandragora-used story-flag? not and if
        stalker-who action-end
        stalker-who char-done
    then
    white-mandragora-used story-flag? not if
        3 barred-floor floor-on
        $26 3 $FF char-load
        om1f-8-flag2 story-flag? if
            0 1 track $1C action
        then
        0 old-mansion-1f-8.cmd02
    then
;

: old-mansion-1f-8.char-enter ( -- )   \ 00425460
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
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        3 3 player-camera
    then then
    2 3 3 area-camera
    hewie-controlled? not if
        fiona-who self-is? 5 exit-taken? and if
            fiona-who 5 char-to-exit
            5 5 player-camera
        then
    else hewie-who self-is? 5 exit-taken? and if
        hewie-who 5 char-to-exit
        5 5 player-camera
    then then
    5 5 5 area-camera
    fiona-who self-is? if
        0 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-8.act23 ( -- )   \ 00426720
    $79 $89 door-copy
    $79 door-reopen-unlock
    exits-rebuild
    4 one-half floor-off
    4 other-half floor-on
    hewie-who char-here? if
        0 hewie-side
    then
    stalker-who char-here? if
        stalker-who 0 stalker-command
    else stalker-active? stalker-who 2 char-cond? not and if
        $FE $FFFF -1 2 stalker-to-room
    then then
    old-mansion-1f-8.cmd05
    $12 ebit-clear
    exit
;

: old-mansion-1f-8.phase1 ( -- )   \ 00425520
    $10 ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 -138.0 8.0 12.0 0 0 sound
            $10 ebit-set
        then
    else
        $C0000000 6 -138.0 8.0 12.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    hewie-controlled? not if
        fiona-who 5 char-in-area? fiona-who char-busy? not and if
            5 exit-check
        then
    else hewie-who 5 char-in-area? hewie-who char-busy? not and if
        5 exit-check
    then then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 3 3 1 chars-area-camera
    $B 5 5 1 chars-area-camera
    fiona-who $C char-entered-area? if
        0 exit-prepare
    then
    fiona-who $D char-entered-area? if
        2 exit-prepare
    then
    fiona-who $E char-entered-area? if
        5 exit-prepare
    then
    fiona-who $C char-entered-area? if
        $1C 3 0 char-load
    then
    fiona-who $C char-left-area? if
        3 0 char-remove
    then
    fiona-who 6 char-entered-area? if
        2 map-page
    then
    fiona-who 6 char-left-area? if
        1 map-page
    then
    3 75.003 7.0 -9.266 $E 20 0 zone
    hewie-who 3 8 char-zone-bits? if
        1 old-mansion-1f-8.cmd00
    then
    hewie-who char-here? if
        118 hewie-action? not if
            35 fiona-started? hewie-who 2 char-cond? not and if
                75 7 -9 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and stalker-who char-here? not and fiona-who 75 -9 $32 char-faces-xz? and if
                    hewie-may-break-off? if
                        0 hewie-who 9 action
                    then
                then
            then
        else
            44 fiona-started? if
                0 hewie-who $A action
            then
            in-panic? not if
                fiona-who char-busy? not fiona-half-hidden state-flag? or if
                    3 stalker-kind-here? $22 stalker-kind-here? or if
                        stalker-who 2 char-cond? not if
                            75 7 -9 point-on-camera? not fiona-who char-unseen? not and stalker-who char-unseen? not and if
                                0 counter-set
                                0 hewie-who $C action
                                0 stalker-who $D action
                                panic-eased
                                fiona-who 0 char-action? if
                                    0 fiona-who $B action-force
                                else
                                    1 fiona-who $B action-force
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    $13 ebit? hewie-who char-busy? not and if
        1 ebit-clear
        $E ebit-clear
    then
    0 ebit? if
        hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
            2 game-mode? not stalker-who char-here? not and if
                0 control-action? if
                    -111 17 -115 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                        fiona-who -111 -115 $32 char-faces-xz? if
                            hewie-may-break-off? if
                                0 fiona-who 5 action
                            then
                        then
                    then
                then
            then
        then
    then
    om1f-8-antimony-tube story-flag? not if
        hewie-who char-here? hewie-who 2 char-cond? not and hewie-who char-busy? not and if
            2 game-mode? not if
                0 control-action? if
                    75 7 95 point-on-camera? not fiona-who char-unseen? not and hewie-who char-unseen? not and if
                        fiona-who 75 95 $32 char-faces-xz? if
                            hewie-may-break-off? if
                                0 fiona-who 7 action
                            then
                        then
                    then
                then
            then
        then
    then
    9 ebit? not if
        4 80.25 7.0 -6.0 $10 4 0 zone
        fiona-who 4 3 char-zone-bits? fiona-who 4 3 char-zone-bits-before? not and if
            fiona-who 5 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
        hewie-who 4 3 char-zone-bits? hewie-who 4 3 char-zone-bits-before? not and if
            hewie-who $A 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
        stalker-who 4 3 char-zone-bits? stalker-who 4 3 char-zone-bits-before? not and if
            stalker-who 5 80.25 7.0 -6.0 2 $40 $50 $40 $80 splash
            9 ebit-set
        then
    then
    $A ebit? not if
        5 67.4 7.0 -6.7 $10 4 0 zone
        fiona-who 5 3 char-zone-bits? fiona-who 5 3 char-zone-bits-before? not and if
            fiona-who 5 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
        hewie-who 5 3 char-zone-bits? hewie-who 5 3 char-zone-bits-before? not and if
            hewie-who $A 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
        stalker-who 5 3 char-zone-bits? stalker-who 5 3 char-zone-bits-before? not and if
            stalker-who 5 67.4 7.0 -6.7 2 $40 $50 $40 $80 splash
            $A ebit-set
        then
    then
    0 ebit? if
        6 -111.73 15.0 -115.37 $3C 6 0 zone
        $B ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 6 9 char-zone-bits? if
                        $B ebit-set
                        $64 chance? if
                            hewie-zone-var 6 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
        6 -111.73 15.0 -115.37 $3C 6 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 6 9 char-zone-bits? if
                    hewie-zone-var 6 var-set
                    hewie-zone-var 0.0 hewie-look-zone
                then
            then
        then
    then
    om1f-8-antimony-tube story-flag? not if
        7 77.32 0.0 95.28 $3C 10 0 zone
        $C ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 7 9 char-zone-bits? if
                        $C ebit-set
                        $64 chance? if
                            hewie-zone-var 7 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
        7 77.32 0.0 95.28 $3C 10 0 zone
        hewie-who char-here? 0 game-mode? and if
            hewie-can-command? if
                hewie-who 7 9 char-zone-bits? if
                    hewie-zone-var 7 var-set
                    hewie-zone-var 8.0 hewie-look-zone
                then
            then
        then
    then
    $12 ebit? if
        old-mansion-1f-8.cond00? not if
            old-mansion-1f-8.act23
        then
    then
;

: old-mansion-1f-8.phase2 ( -- )   \ 004258D0
    -2147483646 scene-request? if
        fiona-who 1 char-before-door? fiona-who 1 char-in-area? and if
            1 always offer-action
        then
    then
    0 ebit? if
        fiona-who $F char-in-area? fiona-who -60 $3C char-heading? and if
            $1D unhurried offer-action
        then
    then
    om1f-8-beef-jerky story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            2 calm offer-action
        then
    then
    om1f-8-album-medallion story-flag? not if
        2 2 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    fiona-who $18 char-in-area? fiona-who -40 $3C char-heading? and if
        $E unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who 0 $3C char-heading? and if
        $F unhurried offer-action
    then
    fiona-who $11 char-in-area? fiona-who -40 $3C char-heading? and if
        $10 unhurried offer-action
    then
    fiona-who $12 char-in-area? fiona-who -60 $3C char-heading? and if
        $11 unhurried offer-action
    then
    fiona-who $13 char-in-area? fiona-who 45 $3C char-heading? and if
        $12 unhurried offer-action
    then
    fiona-who $14 char-in-area? fiona-who 62 $3C char-heading? and if
        $13 unhurried offer-action
    then
    fiona-who $15 char-in-area? fiona-who 90 $3C char-heading? and if
        $14 unhurried offer-action
    then
    fiona-who $16 char-in-area? fiona-who -30 $3C char-heading? and if
        $15 unhurried offer-action
    then
    fiona-who $17 char-in-area? fiona-who 45 $3C char-heading? and if
        $16 unhurried offer-action
    then
;

: old-mansion-1f-8.phase5 ( -- )   \ 004259B0
    hewie-who char-busy? if
        hewie-who action-end
        1 ebit? if
            hewie-who $1E7 94.1 -5.96 90 char-to-xz
        else $D ebit? if
            hewie-who $BC char-to-tri
            stalkers-stay state-flag-clear
        else $E ebit? if
            hewie-who $64 char-to-tri
            stalkers-stay state-flag-clear
        then then then
    then
    $12 ebit? if
        old-mansion-1f-8.act23
    then
;

: old-mansion-1f-8.act00 ( -- )   \ 004259F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $18 message-param-room
    $18 item-use
    $7C door-unlock
    white-mandragora-used story-flag-set
    3 barred-floor floor-off
    $F $54 fade
    $FF 1.0 0 bgm
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    world-held state-flag-set
    hewie-who char-here? if
        $F ebit-set
        hewie-who action-end
        hewie-who char-done
    then
    1 track action-end
    3 char-start
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $F $51 fade
    $19 1.0 0 bgm
    $F ebit? if
        hewie-who char-activate
        $F ebit-clear
        hewie-who $19F 140 char-to-tri-facing
    then
    3 0 char-remove
    1 old-mansion-1f-8.cmd02
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    summoner-on state-flag-set
    stalker-who $48 0 room-doors-state
    stalker-who $66 1 room-doors-state
    self-idle-or-end
;

: old-mansion-1f-8.act01 ( -- )   \ 00425AE0
    self-wait-done
    $7D self-through-door
    self-wait-done
    fiona-who $7D char-not-at-door? if
        rattle-behind-anim self-anim
    else
        rattle-front-anim self-anim
    then
    self-frames-reset
    $14 self-wait-frames
    fiona-who $72 5 char-sound
    $7D door-unlock
    $12 ebit-set
    20 self-idle-anim
    self-frames-reset
    $14 self-wait-frames
    $8008 message   \ "You unlocked the door."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-8.act02 ( -- )   \ 00425B10
    self-wait-done
    122.09 10.26 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-8-beef-jerky story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-8.act03 ( -- )   \ 00425B70
    self-wait-done
    -131.47 9.32 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-5-lavender story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-8.act04 ( -- )   \ 00425BD0
    self-wait-done
    -140.37 -25.97 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-8-album-medallion story-flag-set
            2 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-8.act05 ( -- )   \ 00425C30
    0 counter-set
    0 hewie-who 6 action-force
    $D ebit-set
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    go-for-it-gesture-anim self-anim
    fiona-who 1 char-wait-motion
    fiona-who $2E 5 char-sound
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    1 wait-counter
    self-frames-reset
    self-wait-16
    self-frames-reset
    self-wait-16
    $8284 item-give
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act06 ( -- )   \ 00425C70
    1 self-scripted
    self-wait-done
    hewie-bark
    self-wait-done
    $130 -83.0 -83.0 -120 $FFFF $A self-move-to
    self-wait-done
    1 self-noclip
    $133 -111.0 -108.0 180 $204 5 self-move-to
    self-wait-done
    180 self-turn-angle
    self-wait-done
    sit-a-while-anim $A self-anim-blend
    self-wait-anim
    3 effect-remove
    0 ebit-clear
    10 hewie-trust
    8 message-param-room
    8 1 item-give-count
    0 8 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    counter-inc
    0 self-turn-angle
    self-wait-done
    $130 -83.0 -83.0 60 $FFFF $A self-move-to
    self-wait-done
    items-40-as-70 story-flag? if
        5 0 creature-count
    then
    0 self-noclip
    stalkers-stay state-flag-clear
    $D ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act07 ( -- )   \ 00425D00
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-who char-busy? not if
        0 hewie-who 8 action-force
        go-for-it-gesture-anim self-anim
        fiona-who 1 char-wait-motion
        fiona-who $2E 5 char-sound
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
    else
        stalkers-stay state-flag-clear
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act08 ( -- )   \ 00425D30
    1 self-scripted
    $E ebit-set
    $13 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $67 149.0 91.0 64 $FFFF $A self-move-to
    self-wait-done
    -84 self-turn-angle
    self-wait-done
    $13 ebit-clear
    1 self-noclip
    $66 129.619 91.48 1.7 10 hewie-go-to
    self-wait-done
    sit-a-while-anim $A self-anim-blend
    self-wait-anim
    om1f-8-antimony-tube story-flag? not if
        fiona-half-hidden state-flag? fiona-who char-busy? not or if
            $97 $63 item-count? not if
                10 hewie-trust
            then
            $97 message-param-room
            $97 $63 item-count? if
                $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
                wait-message
            else
                om1f-8-antimony-tube story-flag-set
                $97 1 item-give-count
                0 $97 item-tab
                item-fanfare
                $8011 message   \ "You've obtained ."
                wait-message
            then
        then
    then
    $1ED 43.7 110.48 -90 $FFFF $A self-move-to
    self-wait-done
    110 self-turn-angle
    self-wait-done
    $1EE 76.12 104.76 0.5 5 hewie-go-to
    self-wait-done
    0 self-noclip
    0 self-scripted
    stalkers-stay state-flag-clear
    $E ebit-clear
    self-idle-or-end
;

: old-mansion-1f-8.act09 ( -- )   \ 00425DF0
    $13 ebit-set
    1 ebit-set
    self-wait-done
    hewie-bark
    self-wait-done
    $175 92.855 -13.871 -90 $FFFF $A self-move-to
    self-wait-done
    $13 ebit-clear
    1 self-scripted
    2 solid-floor floor-off
    2 keeps-fiona-off keeps-pursuers-off or floor-on
    8 76.218 -13.075 -90 $204 5 self-move-to
    self-wait-done
    90 self-turn-angle
    self-wait-done
    lie-down-anim $A self-anim-blend
    self-wait-anim
    3600 2 hewie-anim
    1 ebit-clear
    self-idle-or-end
;

: old-mansion-1f-8.act0A ( -- )   \ 00425E40
    1 self-scripted
    1 ebit-set
    self-wait-done
    9 ebit-clear
    $A ebit-clear
    $175 92.855 -13.871 90 $204 5 self-move-to
    self-wait-done
    -1 self-idle-anim
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-do
    2 keeps-fiona-off keeps-pursuers-off or floor-off
    2 solid-floor floor-on
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act0B ( -- )   \ 00425E80
    stalkers-stay state-flag-set
    1 self-noclip
    1 self-scripted
    $F $54 fade
    $FF 1.0 0 bgm
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    3 4 0 char-model-op
    force-calm state-flag-set
    yield
    force-calm state-flag-clear
    fiona-who 1 char-no-shadow
    0 cutscene-track $1F action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 old-mansion-1f-8.cmd03
    stalker-who action-end
    3 summon-take
    5 hewie-trust
    world-held state-flag-set
    fiona-who $1BB 112.81 45.98 32 char-to-xz
    3 3 player-camera
    fiona-who 0 char-no-shadow
    yield
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    $23A item-give
    $8280 item-give
    $31 resident-flag-set
    counter-inc
    0 self-noclip
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: old-mansion-1f-8.act0C ( -- )   \ 00425F70
    1 wait-counter
    0 0 hewie-anim
    0 0 hewie-do
    hewie-who $1DE 100.17 -23.46 -120 char-to-xz
    hewie-no-dodge state-flag-clear
    self-idle-or-end
;

: old-mansion-1f-8.act0D ( -- )   \ 0047AD30
    1 wait-counter
    self-idle-or-end
;

: old-mansion-1f-8.act0E ( -- )   \ 00425FA0
    self-wait-done
    -154.0 -28.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    2 ebit? 0 0 var? not and 0 1 var? not and if
        7 message   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
        wait-message
    else
        1 message   \ "Aureolus Belli (1698-1763)"
        wait-message
    then
    2 ebit? not if
        0 var-inc
    then
    2 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act0F ( -- )   \ 00425FE0
    self-wait-done
    -86.29 -39.06 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? 0 0 var? not and 0 1 var? not and if
        7 message   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
        wait-message
    else
        2 message   \ "Aureolus Belli (1537-1584)"
        wait-message
    then
    3 ebit? not if
        0 var-inc
    then
    3 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act10 ( -- )   \ 00426020
    self-wait-done
    40.56 -52.11 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    4 ebit? 0 0 var? not and 0 1 var? not and if
        7 message   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
        wait-message
    else
        3 message   \ "Aureolus Belli (1870-1974)"
        wait-message
    then
    4 ebit? not if
        0 var-inc
    then
    4 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act11 ( -- )   \ 00426060
    self-wait-done
    77.34 6.2 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    5 ebit? 0 0 var? not and 0 1 var? not and if
        7 message   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
        wait-message
    else
        4 message   \ "Aureolus Belli (1755-1830)"
        wait-message
    then
    5 ebit? not if
        0 var-inc
    then
    5 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act12 ( -- )   \ 004260A0
    self-wait-done
    127.66 -13.38 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    6 ebit? 0 0 var? not and 0 1 var? not and if
        7 message   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
        wait-message
    else
        5 message   \ "Aureolus Belli (1508-1583)"
        wait-message
    then
    6 ebit? not if
        0 var-inc
    then
    6 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act13 ( -- )   \ 004260E0
    self-wait-done
    144.96 39.88 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    7 ebit? 0 0 var? not and 0 1 var? not and if
        7 message   \ "There are multiple gravestones all with the same name... Now what in the world coul..."
        wait-message
    else
        6 message   \ "Aureolus Belli (1832-1905)"
        wait-message
    then
    7 ebit? not if
        0 var-inc
    then
    7 ebit-set
    self-idle-or-end
;

: old-mansion-1f-8.act14 ( -- )   \ 00426120
    self-wait-done
    $47 138.0 -98.0 180 $FFFF 5 self-move-to
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    8 ebit? not if
        8 message   \ "Looks to be a mausoleum... There are words written here."
        wait-message
        $F 6 fade
        wait-fade
        9 message   \ "Dead spirit craving for life, crawling back from the underworld... Sip of the fresh..."
        wait-message
        $F 7 fade
        wait-fade
        8 ebit-set
    else
        $A message   \ "Spirits? Life-blood? No matter how you cut it, these don't sound like the words of..."
        wait-message
        8 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-8.act15 ( -- )   \ 00426160
    self-wait-done
    73.11 -9.81 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    hewie-who char-here? 118 hewie-action? and hewie-who 8 char-on-tri? and if
        $C message   \ "Hewie's eyes flash from inside the thicket."
        wait-message
    else
        $B message   \ "There is an opening in the thicket... It looks just big enough for an animal to ge..."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-8.act16 ( -- )   \ 00426190
    self-wait-done
    $1A2 113.0 -122.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 30.0 0.0 event-camera
    1 self-scripted
    white-mandragora-used story-flag? not if
        om1f-8-flag2 story-flag? not if
            stalkers-stay state-flag-set
            fiona-who 8 char-file-load
            $14 message   \ "It's too dark to see... It looks like there could be something in the back. F..."
            wait-message
            0 answer? if
                fiona-who char-file-use
                3 char-start
                $26 char-activate
                $26 127.567 0.0 -123.393 -90 char-to-xyz
                old-mansion-1f-8.cmd01
                look-close-anim self-anim
                self-wait-anim
                0 1 track $1E action
                self-frames-reset
                $F self-wait-frames
                fiona-who seized-sound 5 char-sound
                0 room-motion self-anim
                self-wait-anim
                3 panic-stage? not if
                    3 panic-stage
                then
                1 room-motion self-anim
                self-wait-anim
                3 room-motion self-anim
                self-wait-anim
                om1f-8-flag2 story-flag-set
            then
            stalkers-stay state-flag-clear
        else
            1 0 var? if
                $D message   \ "It doesn't seem too terribly dangerous... But I get the feeling that if I..."
                wait-message
            then
            1 1 var? if
                $E message   \ "It's almost as if... it wants something..."
                wait-message
            then
            1 2 var? if
                crouch-down-anim self-anim
                self-wait-anim
                $F message   \ "There are traces as if something has recently been ripped to shreds here...."
                wait-message
                crouch-up-anim self-anim
                self-wait-anim
            then
            1 var-inc
            1 3 var? if
                1 0 var-set
            then
        then
    else
        crouch-down-anim self-anim
        self-wait-anim
        $10 message   \ "Mandragora... So, this is used as a substitute for life-blood."
        wait-message
        crouch-up-anim self-anim
        self-wait-anim
    then
    0 self-scripted
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: old-mansion-1f-8.act1B ( -- )   \ 00426300
    $F $54 fade
    $FF 1.0 0 bgm
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    world-held state-flag-set
    1 track action-end
    hewie-who char-here? if
        $F ebit-set
        hewie-who action-end
        hewie-who char-done
    then
    3 char-start
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who $1A2 108.0 -122.0 90 char-to-xz
    $F ebit? if
        hewie-who char-activate
        $F ebit-clear
        hewie-who $19F 140 char-to-tri-facing
    then
    om1f-8-flag2 story-flag-set
    0 1 track $1C action
    fiona-who 8 char-file-load
    fiona-who char-file-use
    3 room-motion self-anim
    3 panic-stage? not if
        3 panic-stage
    then
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    self-wait-anim
    $32 resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-8.act17 ( -- )   \ 00426280
    self-wait-done
    stalkers-stay state-flag-set
    1 self-scripted
    $19 message-param-room
    $19 item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act18 ( -- )   \ 004262A0
    self-wait-done
    stalkers-stay state-flag-set
    1 self-scripted
    $1A message-param-room
    $1A item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act19 ( -- )   \ 004262C0
    self-wait-done
    stalkers-stay state-flag-set
    1 self-scripted
    $1B message-param-room
    $1B item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act1A ( -- )   \ 004262E0
    self-wait-done
    stalkers-stay state-flag-set
    1 self-scripted
    $1C message-param-room
    $1C item-use
    ['] old-mansion-1f-8.act1B goto
;

: old-mansion-1f-8.act1C ( -- )   \ 004263E0
    3 char-start
    $26 char-activate
    $26 127.567 0.0 -123.393 -90 char-to-xyz
    old-mansion-1f-8.cmd01
    $26 $9001 1 0 char-anim-hold
    self-idle-or-end
;

: old-mansion-1f-8.act1D ( -- )   \ 00426400
    self-wait-done
    -112.26 -123.08 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    0 message   \ "There is something glowing from the rear of the graveyard... I just can't make it out f..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-8.act1E ( -- )   \ 00426418
    $26 $9000 0 0 char-anim-hold
    $26 wait-char-anim
    $26 $9001 1 0 char-anim-hold
    self-idle-or-end
;

: old-mansion-1f-8.act1F ( -- )   \ 00426430
    0 old-mansion-1f-8.cmd03
    begin
        2 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-1f-8.cmd03
    self-idle-or-end
;

: old-mansion-1f-8.act20 ( -- )   \ 00426440
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    3 partner-load
    2 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    3 4 0 char-model-op
    fiona-who 1 char-no-shadow
    0 cutscene-track $1F action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 old-mansion-1f-8.cmd03
    fiona-who 0 char-no-shadow
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-1f-8.act21 ( -- )   \ 00426540
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    0 old-mansion-1f-8.cmd02
    $26 3 $FF char-load
    3 char-start
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-1f-8.act22 ( -- )   \ 00426630
    self-wait-done
    $10 -140.0 65.6 -130.0 $E $80 $80 $80 $40 specks
    $10 42.0 45.2 -118.0 $E $80 $80 $80 $40 specks
    $10 -100.0 53.3 -25.0 $E $80 $80 $80 $40 specks
    $10 -42.0 53.2 -118.0 $E $80 $80 $80 $40 specks
    $10 149.5 38.0 61.0 $E $80 $80 $80 $40 specks
    0 old-mansion-1f-8.cmd02
    $26 3 $FF char-load
    3 char-start
    7 0 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-8 register-scripts   \ (its phases and actions, by their names)
