\ story/rooms/castle-2f-3.fs - the event scripts of room castle-2f-3 ($21; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-3
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

: castle-2f-3.cmd00 ( -- )  1 0 tv-in ;   \ (room effect 1 the TV showing the screen)
\ room 0x21 (D_00400B98): the fan turns, except while a movie plays
: castle-2f-3.cmd01 ( -- )  fan-turn ;
\ room 0x21 (D_00400BA8)
: castle-2f-3.cmd02 ( b0 -- )  $1B $10000040  0.491e 22.0e 69.502e  9.502e 22.0e 60.491e  0.491e 0.0e 69.502e  9.502e 0.0e 60.491e  lit-quad ;   \ (a reflecting window: b0 1 removed, 2 over the whole picture)
\ Sets bit 1 of the flag byte three times (inlined setter calls); returns 1.
: castle-2f-3.cmd03 ( -- )  fiona-who 10 hide-part ;   \ (Fiona's model part 10 hidden - the original's flag 2 at +$AC)
\ room 0x21 (D_00400BC8)
: castle-2f-3.cmd04 ( b0 -- )  $1A $20000040  40.491e 30.0e -10.498e  49.502e 30.0e -19.509e  40.491e 1.5e -10.498e  49.502e 1.5e -19.509e  lit-quad ;   \ (a reflecting window: b0 1 removed, 2 over the whole picture)
\ room 0x21 (Room21_Cmd05_ptmf): the player's model +0xC8 vector by byte 3
: castle-2f-3.cmd05 ( b0 -- )   \ (Fiona's hair's force (FionaModel_SetVector): 0 back 0.01, 1 aside 0.02, else straight; 0.1 down)
    case
        0 of  0e 0.1e -0.01e  endof
        1 of  0.02e 0.1e 0e  endof
        >r 0e 0.1e 0e r>
    endcase
    0 char-model dup 0< if  drop fdrop fdrop fdrop exit  then  2 cloth-force! ;
\ room 0x21 (Room21_Cmd06_ptmf): Fiona's model +0x1570 (byte 3 0) / +0x1574 (1) = byte 4
: castle-2f-3.cmd06 ( b0 b1 -- )  drop drop ;   \ (Daniella's hair settling steps (her model's springs: not simulated here yet))
\ room 0x21 (Room21_Cond00_ptmf): the pursuer, about and not in state 2, is in its mode 2 but in
\ another room than the current one (the player about too)
: castle-2f-3.cond00? ( -- flag )
    stalker-about? stalker-down? 0= and slot2-kind @ 2 = and  stalker-in-this-room? 0= and ;

: castle-2f-3.enter ( -- )   \ 003FF170
    room-sounds
    castle-2f-3.cmd00
    c2f-3-tv-examined story-flag? not if
        0 part-shown
        1 0 1 effect-string
    else
        0 part-hidden
        1 1 1 effect-string
        1 noise-level
    then
    0 $F1 $12 action
    c2f-3-medallion-found story-flag? c2f-3-medallion story-flag? not and if
        2 -9.0 1.0 -11.0 flicker-sprite
    then
    found-file-202 story-flag? not if
        $3D door-lock
    then
    c2f-3-flag1 story-flag? hewie-found-in-wire story-flag? not and if
        $3D door-lock
    then
    found-file-201 story-flag? found-file-202 story-flag? not and if
        1 part-shown
    else
        1 part-hidden
    then
    0 0 var-set
    1 0 var-set
    got-castle-sketch story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
;

: castle-2f-3.act04 ( -- )   \ 003FFA90
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who $A char-file-load
    then
    5 ebit-clear
    2 0 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 1 pvar? if
        0 chance? if
            5 ebit-set
        then
    else 2 2 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else 2 3 pvar? if
        $32 chance? if
            5 ebit-set
        then
    then then then then
    $C ebit? if
        5 ebit-clear
        $C ebit-clear
        force-chased state-flag-clear
    else
        2 creature-action? if
            5 ebit-set
        then
        4 stalker-alert? if
            5 ebit-clear
        then
    then
    5 ebit? if
        0 stalker-who $B action
    else
        $78 1 summoner-wait
    then
    2 pvar-inc
    exit
;

: castle-2f-3.char-enter ( -- )   \ 003FF200
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    fiona-who self-is? if
        c2f-8-flag3 story-flag? not c2f-2-flag1 story-flag? and if
            c2f-3-flag3 story-flag? not if
                c2f-3-flag3 story-flag-set
                0 exit-taken? in-panic? not and if
                    castle-2f-3.cond00? if
                        0 fiona-who $23 action
                    then
                then
            then
        then
        $80 exit-taken? if
            got-castle-sketch story-flag? c2f-3-flag1 story-flag? not and if
                force-calm state-flag-clear
                0.0 sound-volume-scale
                2 2 player-camera
                fiona-who $A8 45 char-to-tri-facing
                world-held state-flag-set
                0 fiona-who 8 action
            then
        then
        $81 exit-taken? if
            c2f-3-flag2 story-flag? not if
                hewie-found-in-wire story-flag? hewie-freed story-flag? not and if
                    0.0 sound-volume-scale
                    fiona-who $128 -38.951 -26.464 90 char-to-xz
                    0 fiona-who 9 action
                    c2f-3-flag2 story-flag-set
                then
            then
        then
    then
    hewie-who self-is? if
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            $B ebit-set
            c2f-3-tv-examined story-flag? 2 creature-action? not and if
                2 self-is? 6 self-is? or 7 self-is? or if
                    $78 1 summoner-wait
                    0 stalker-who 6 action
                else
                    castle-2f-3.act04
                then
            else
                castle-2f-3.act04
            then
        else
            $B ebit-clear
        then
    then
;

: castle-2f-3.phase1 ( -- )   \ 003FF340
    3 ebit? not if
        6 sound-bank-loaded? if
            c2f-3-tv-examined story-flag? if
                $40000001 6 0.0 3.0 7.0 0 0 sound
            then
            $40000009 6 -42.0 4.0 -63.0 0 0 sound
            3 ebit-set
        then
    else
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000005 6 -47.0 12.0 -28.0 0 0 sound
            else 0 1 var? if
                $40000006 6 -47.0 12.0 -28.0 0 0 sound
            else 0 2 var? if
                $40000007 6 -47.0 12.0 -28.0 0 0 sound
            else 0 3 var? if
                $40000008 6 -47.0 12.0 -28.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
        $C0000001 6 0.0 3.0 7.0 0 0 sound
        $C0000009 6 -42.0 4.0 -63.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    c2f-3-tv-examined story-flag? not if
        0 part-shown
        1 0 1 effect-string
    else
        0 part-hidden
        1 1 1 effect-string
    then
    found-file-202 story-flag? not if
        found-file-201 story-flag? not if
            fiona-who $E char-entered-area? $D ebit? not and if
                $D ebit-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 7 action-force
                else
                    1 fiona-who 7 action-force
                then
            then
        then
    then
    c2f-3-medallion-found story-flag? not if
        7 -9.0 0.0 -11.0 $A 5 0 zone
        hewie-who 7 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 c2f-3-medallion-found var-set
                $1A c2f-3-medallion var-set
                $1B 2 var-set
                $1C -9000 var-set
                $1D 1000 var-set
                $1E -11000 var-set
                $1F 7 var-set
                0 hewie-who $8B action
            then
        then
    then
    0 -1.41 -8.0 8.99 $D 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 -46.87 0.0 -38.5 $14 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    2 -39.23 0.0 -59.47 $14 6 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 6.0 hewie-look-zone
            then
        then
    then
    3 -37.71 -8.0 63.24 $F 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    4 46.35 0.0 -40.12 $F 22 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    5 39.18 0.0 -19.5 $10 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    6 -0.22 -8.0 59.41 $10 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 6 9 char-zone-bits? if
                $1F 6 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-3.phase2 ( -- )   \ 003FF650
    fiona-who 4 char-in-area? fiona-who 90 $5A char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            0 unhurried offer-action
        then
    then
    found-file-202 story-flag? not if
        found-file-201 story-flag? if
            fiona-who $10 char-in-area? fiona-who 90 $32 char-heading? and if
                $D unhurried offer-action
            then
        then
        -2147483646 scene-request? if
            $C always offer-action
        then
    else c2f-3-flag1 story-flag? hewie-found-in-wire story-flag? not and if
        -2147483646 scene-request? if
            $E always offer-action
        then
        fiona-who 5 char-in-area? fiona-who 90 $3C char-heading? and if
            $F unhurried offer-action
        then
        fiona-who $D char-in-area? fiona-who 22 $3C char-heading? and if
            $10 unhurried offer-action
        then
    else fiona-who 5 char-in-area? fiona-who 90 $3C char-heading? and if
        c2f-2-flag1 story-flag? not if
            $16 unhurried offer-action
        else stalker-who char-here? not if
            1 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then then
    then then then
    fiona-who $B char-in-area? fiona-who -67 $3C char-heading? and if
        $14 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 45 $3C char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            $15 unhurried offer-action
        then
    then
    fiona-who $11 char-in-area? fiona-who 0 $3C char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            $17 unhurried offer-action
        then
    then
    fiona-who 6 char-in-area? fiona-who -45 $3C char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            $18 unhurried offer-action
        then
    then
    fiona-who $12 char-in-area? fiona-who 0 $32 char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            $1C unhurried offer-action
        then
    then
    fiona-who $13 $32 char-faces-area? if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            $1D unhurried offer-action
        then
    then
    fiona-who $14 char-in-area? fiona-who -45 $32 char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else
            $1E unhurried offer-action
        then
    then
    fiona-who $15 char-in-area? fiona-who 90 $32 char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else fg-3-flag3 story-flag? not if
            $1F unhurried offer-action
        else
            $26 unhurried offer-action
        then then
    then
    fiona-who $D char-in-area? fiona-who $16 char-in-area? or fiona-who 22 $3C char-heading? and if
        found-file-202 story-flag? not if
            $C unhurried offer-action
        else c2f-3-flag1 story-flag? not if
            $20 unhurried offer-action
        else
            $22 unhurried offer-action
        then then
    then
    found-file-201 story-flag? if
        fiona-who 7 char-in-area? fiona-who -45 $32 char-heading? and if
            found-file-202 story-flag? not if
                $C unhurried offer-action
            else
                $84 calm-hewie-up offer-action
            then
        then
    then
    $C ebit? -2147483646 scene-request? and if
        $24 always offer-action
    then
    c2f-3-medallion-found story-flag? c2f-3-medallion story-flag? not and if
        7 2 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            $19 calm offer-action
        then
    then
;

: castle-2f-3.phase3 ( -- )   \ 003FF810
    -2.0 7.6 -47.0 -2.0 7.6 -67.0 -2.0 0.0 -47.0 -2.0 0.0 -67.0 lights-doorway
;

: castle-2f-3.act00 ( -- )   \ 003FF850
    self-wait-done
    $6A -1.0 13.5 160 $FFFF 5 self-move-to
    self-wait-done
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    c2f-3-tv-examined-2 story-flag? not if
        c2f-3-tv-examined-2 story-flag-set
        $29 message   \ "I hope this TV works..."
        wait-message
    then
    reach-out-anim self-anim
    self-wait-anim
    5 0 pvar? 5 1 pvar? or if
        c2f-3-tv-examined story-flag? not if
            c2f-3-tv-examined story-flag-set
            fiona-who 0 6 char-sound
            $40000001 6 0.0 3.0 7.0 0 0 sound
            $40 $6A noise
            1 noise-level
        else
            c2f-3-tv-examined story-flag-clear
            fiona-who 2 6 char-sound
            1 6 sound-stop
            0 noise-level
        then
        reach-back-anim self-anim
        self-wait-anim
    else
        reach-back-anim self-anim
        self-wait-anim
        $28 message   \ "It won't turn on. Maybe it's broken?"
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act03 ( -- )   \ 003FFA60
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    counter-inc
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: castle-2f-3.act01 ( -- )   \ 003FF8F0
    stalkers-stay state-flag-set
    $B ebit-clear
    1 self-scripted
    0 counter-set
    6 ebit-clear
    7 ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 50 chars-within? and if
        0 hewie-who 2 action-force
        6 ebit-set
    then
    fiona-who 8 char-file-load
    self-wait-done
    6 ebit? if
        $27 14.5 -37.5 180 $FFFF 5 self-move-to
        self-wait-done
        fiona-who char-file-use
        hewie-who self-look-at
        yield
        begin
            7 ebit? not while
            yield
        repeat
        counter-inc
        1 self-noclip
        self-who self-look-at
        yield
        0 room-motion 5 self-anim-blend8
        self-frames-reset
        $1C self-wait-frames
        fiona-who slide-sound 5 char-sound
        self-wait-anim
    else
        fiona-who char-file-use
        $27 $8004 5 14.5 -37.5 180 self-walk-anim
        self-wait-done
        counter-inc
        1 self-noclip
        fiona-who slide-sound 5 char-sound
        5 room-motion $A self-anim-blend8
        self-wait-anim
    then
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    4 ebit-clear
    0 avoid-prompt
    $C ebit? if
        $FE $10 448 2 stalker-to-room
        stalker-who 0 stalker-command
        $3C door-unlock
        $3D door-unlock
        force-chased state-flag-set
    then
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? 4 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] castle-2f-3.act03 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                $B ebit? stalker-who char-here? not and if
                    $B ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $C ebit? if
                $C ebit-clear
                force-chased state-flag-clear
            then
            counter-inc
            stalker-who char-here? if
                ['] castle-2f-3.act03 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-3.act02 ( -- )   \ 003FFA10
    1 self-scripted
    hewie-who 9 char-file-load
    self-wait-done
    $88 18.5 -37.5 180 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    7 ebit-set
    1 wait-counter
    1 self-noclip
    0 room-motion 5 self-anim-blend
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        fiona-who char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                2 room-motion 5 self-anim-blend8
                self-wait-anim
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-3.act05 ( -- )   \ 0047AAC8
    self-wait-done
    -1 self-idle-anim
    self-wait-anim
    self-idle
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act06 ( -- )   \ 003FFB00
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $6A -4.0 13.0 135 $FFFF 5 self-move-to
    self-wait-done
    $602 self-anim
    self-frames-reset
    $12 self-wait-frames
    stalker-who 2 6 char-sound
    1 6 sound-stop
    0 noise-level
    c2f-3-tv-examined story-flag-clear
    5 pvar-inc
    stalker-who 3 stalker-command
    stalker.anims:stand-anim self-anim
    self-wait-anim
    self-frames-reset
    self-wait-16
    self-idle-or-end
;

: castle-2f-3.act07 ( -- )   \ 003FFB50
    self-wait-done
    $F $54 fade
    3 3 $FF char-load
    3 char-start
    fiona-who 1 char-no-shadow
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    1 castle-2f-3.cmd02
    1 castle-2f-3.cmd04
    0 $F9 $A action
    panic-eased
    0 message-prepare   \ "I've gathered some clothes for you. Wait. Don't go. Um, excuse me, but where ar..."
    1 result? if
        scene-play
    then
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    wait-fade
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    3 0 char-remove
    1 part-shown
    2 castle-2f-3.cmd05
    $3C door-open-clear
    doors-room-in
    $3C door-lock
    fiona-who $10E -27.455 -48.354 107 char-to-xz
    1 1 player-camera
    camera-restart
    found-file-201 story-flag-set
    $F $51 fade
    wait-fade
    $201 item-give
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act08 ( -- )   \ 003FFC30
    self-wait-done
    c2f-3-flag1 story-flag-set
    2 2 player-camera
    1 exit-prepare
    fiona-who $A8 46 char-to-tri-facing
    $3C door-open-clear
    $3D door-open-clear
    $3D door-lock
    doors-room-in
    camera-restart
    $F $51 fade
    wait-fade
    $204 item-give
    2 30 var-set
    0 $F2 $1A action
    self-idle-or-end
;

: castle-2f-3.act11 ( -- )   \ 00400020
    1 room-motion self-anim
    self-wait-anim
    begin
        32 hewie-action? while
        yield
    repeat
    hewie-no-attack state-flag-set
    $3D door-unlock
    0 counter-set
    0 stalker-who $13 action
    1 wait-counter
    1 self-scripted
    hewie-no-attack state-flag-clear
    self-wait-done
    self-who self-look-at
    yield
    $F $54 fade
    7 2 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    0 self-idle-anim
    self-wait-anim
    $3D door-open-clear
    doors-room-in
    1 0 self-door-knock
    fiona-who 1 char-no-shadow
    panic-eased
    $14 message-prepare   \ "Thank you. You saved me. Come here... Come on, boy. What's your name? Hewie?..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    hewie-who char-activate
    hewie-who 0 char-cond!
    hewie-who char-full-health
    $21 0 33 hewie-to-room
    hewie-who $21 145 char-to-tri-facing
    hewie-commandable state-flag-set
    $29 0 hewie-do
    1 hewie-pause
    hewie-freed story-flag-set
    fiona-who $13 -16.325 -66.999 -8 char-to-xz
    0 self-idle-anim
    self-wait-anim
    0 exit-prepare
    stalkers-blind state-flag-clear
    stalker-who action-end
    1 summon-take
    self-wait-done
    camera-restart
    world-held state-flag-clear
    traps-off state-flag-clear
    $F $51 fade
    wait-fade
    $207 item-give
    $F 6 fade
    wait-fade
    $40A7 message   \ "'Additional Dog Commands' Hewie has befriended you. Give him various commands via t..."
    wait-message
    $F 7 fade
    wait-fade
    $A7 message-param-room
    $A7 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message   \ "has been saved to your file."
    wait-message
    items-40-as-70 story-flag? if
        $F 6 fade
        wait-fade
        $32 message   \ "WARNING!! In HARD MODE, if your dog is ever 'critically wounded,' the game is ove..."
        wait-message
        $F 7 fade
        wait-fade
    then
    0 hewie-pause
    $8274 item-give
    0 self-scripted
    self-idle-or-end
;

: castle-2f-3.act09 ( -- )   \ 003FFC70
    traps-off state-flag-set
    force-calm state-flag-set
    self-wait-done
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $25 action
    $21 0 176 hewie-to-room
    $FE $21 20 2 stalker-to-room
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    scene-locked state-flag-clear
    fiona-who $A char-layer
    0 3 0.0 light
    1 1 player-camera
    hewie-who char-activate
    hewie-who 0 char-cond!
    hewie-who char-full-health
    $21 0 176 hewie-to-room
    hewie-who $B0 -35.452 -8.852 180 char-to-xz
    hewie-commandable state-flag-set
    hewie-no-attack state-flag-set
    1 self-scripted
    stalkers-blind state-flag-set
    $FE $21 20 2 stalker-to-room
    stalker-who char-activate
    stalker-who 2 stalker-command
    stalker-who 0 stalker-search-delay
    stalker-who $14 -15.648 -48.68 0 char-to-xz
    stalker-who char-full-health
    0 stalker-who $1B action
    fiona-who $B char-file-load
    fiona-who char-file-use
    fiona-who $A -39.575 -27.807 90 char-to-xz
    camera-restart
    1 room-motion self-anim
    self-wait-anim
    force-calm state-flag-clear
    5 0 0 music
    $F $91 fade
    wait-fade
    self-frames-reset
    begin
        $2A message   \ "Push up on the right analog stick to command the dog to attack."
        $F0 frames? 0 control-action? or if
            2 room-motion self-anim
            self-frames-reset
            $10 self-wait-frames
            fiona-who $38 5 char-sound
            hewie-no-attack state-flag-clear
            $20 0 hewie-do
            $2A message-close
            self-wait-anim
            stalker-who self-look-at
            yield
            ['] castle-2f-3.act11 goto
        else
            yield
        then
    again
;

: castle-2f-3.act0A ( -- )   \ 003FFDD0
    begin
        1 1 castle-2f-3.cmd06
        8 cutscene-shot? $A cutscene-shot? or if
            1 $1E castle-2f-3.cmd06
        then
        0 $14 castle-2f-3.cmd06
        6 cutscene-shot? 8 cutscene-shot? or $A cutscene-shot? or $F cutscene-shot? or $11 cutscene-shot? or $13 cutscene-shot? or if
            0 $28 castle-2f-3.cmd06
        then
        3 cutscene-shot? if
            1 part-shown
        then
        $15 cutscene-shot? if
            0 castle-2f-3.cmd05
        then
        $16 cutscene-shot? if
            1 castle-2f-3.cmd05
        then
        $17 cutscene-shot? if
            2 castle-2f-3.cmd05
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-2f-3.act0B ( -- )   \ 003FFE30
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $1C -0.663 -24.791 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who char-file-use
        0 room-motion $A self-anim-blend
        self-frames-reset
        $1E self-wait-frames
        4 ebit-set
        self-wait-anim
    else
        there-anim self-anim
        self-wait-anim
        4 ebit-set
    then
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-2f-3.act0C ( -- )   \ 0047AAD0
    self-wait-done
    $20 message   \ "I can't just walk around all day holding these sheets around me. Call me old-fashione..."
    wait-message
    self-idle-or-end
;

: castle-2f-3.act0D ( -- )   \ 003FFE90
    self-wait-done
    $1F message   \ "There is a change of clothes that maid has apparently laid out. Change clothes? YES NO"
    wait-message
    0 answer? if
        $F $54 fade
        $D 3 $FF char-load
        $12 4 $FF char-load
        $18 5 $FF char-load
        3 char-start
        4 char-start
        5 char-start
        $D 1 char-no-shadow
        3 0 movie-play
        2 cutscene-start
        yield
        yield
        scene-wait-ready
        wait-fade
        0 $F9 $21 action
        panic-eased
        8 message-prepare   \ "Wait a minute... I remember being in the car with..."
        1 result? if
            scene-play
        then
        world-held state-flag-set
        $D 0 char-no-shadow
        3 0 char-remove
        4 0 char-remove
        5 0 char-remove
        new-game-sounds state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
        effects-arena-flip
        0 char-in
        begin
            yield
            4 sound-bank-loaded? until
        1 part-hidden
        fiona-who $21 5.752 -41.411 178 char-to-xz
        1 1 player-camera
        camera-restart
        $3C door-unlock
        $3D door-unlock
        found-file-202 story-flag-set
        no-flee state-flag-clear
        $F $51 fade
        wait-fade
        $202 item-give
        rest-anim $A self-anim-blend
        self-wait-anim
        10 self-idle-anim
        $12 message   \ "That worked... but now what should I do? If I could just find a map of this place..."
        wait-message
        $F 6 fade
        wait-fade
        $40A6 message   \ "'Additional Actions' You are now able to perform the following new actions in t..."
        wait-message
        fg-3-flag3 story-flag? if
            $40AB message   \ "'Costumes' You are now able to change costumes. Change into alternate costu..."
            wait-message
        then
        $F 7 fade
        wait-fade
        $A6 message-param-room
        $A6 1 item-give-count
        $83 $85 0.0 0.0 0.0 0 0 sound
        $801B message   \ "has been saved to your file."
        wait-message
        fg-3-flag3 story-flag? if
            got-doc-action story-flag-set
            $AB message-param-room
            $AB 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message   \ "has been saved to your file."
            wait-message
        then
    then
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act0E ( -- )   \ 0047AAD8
    self-wait-done
    $C message   \ "I wonder if that dog will be alright?"
    wait-message
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act0F ( -- )   \ 0047AAE0
    self-wait-done
    $22 message   \ "I can't fall asleep. I am too worried about that dog."
    wait-message
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act10 ( -- )   \ 00400000
    self-wait-done
    $80 -1.546 59.728 45 $FFFF 5 self-move-to
    self-wait-done
    reach-out-anim self-anim
    self-wait-anim
    $D message   \ "If I just leave it like this, it may not make it through the night."
    wait-message
    reach-back-anim self-anim
    self-wait-anim
    self-wait-done
    self-idle-or-end
;

: castle-2f-3.act12 ( -- )   \ 0047AAE8
    begin
        castle-2f-3.cmd01
        yield
    again
;

: castle-2f-3.act13 ( -- )   \ 00400180
    1 self-scripted
    self-wait-done
    self-who self-look-at
    yield
    $C4 $203 6 self-move-tri
    self-wait-done
    0 exit-door-open? not if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $76 -17.0 -83.0 180 $203 5 self-move-to
    self-wait-done
    1 counter-set
    0 self-scripted
    begin
        yield
    again
;

: castle-2f-3.act14 ( -- )   \ 004001B0
    self-wait-done
    -40.0 -60.0 self-turn-to-xz
    self-wait-done
    $23 message   \ "The fire burns, emitting a red glow. The room is heated to a comfortable temperature."
    wait-message
    self-idle-or-end
;

: castle-2f-3.act15 ( -- )   \ 004001C0
    self-wait-done
    $B5 43.571 -39.929 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 10.0 event-camera
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    c2f-3-portrait-1-examined story-flag-set
    0 ebit? not if
        $24 message   \ "I wonder whose portrait this is?"
        wait-message
        0 ebit-set
    else
        $25 message   \ "This sounds really weird, but for some reason, I feel like I've seen this person..."
        wait-message
    then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act16 ( -- )   \ 00400220
    self-wait-done
    $26 message   \ "Freshly starched and pressed sheets and bed covers now line the bed."
    wait-message
    turn-round-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    crouch-down-anim self-anim
    self-wait-anim
    crouching-anim self-anim
    self-frames-reset
    self-wait-16
    crouch-up-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    $27 message   \ "Oh come on, Fiona... It's not like you are going to find the Boogieman under there."
    wait-message
    self-idle-or-end
;

: castle-2f-3.act17 ( -- )   \ 00400250
    self-wait-done
    $9F -37.0 63.0 0 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    c2f-3-portrait-2-examined story-flag-set
    1 ebit? not if
        $2D message   \ "The man pictured here has a noble air about him. I wonder how long ago this portr..."
        wait-message
        1 ebit-set
    else c2f-3-portrait-1-examined story-flag? c2f-3-portrait-3-examined story-flag? or if
        $2E message   \ "In this painting, the man looks quite young. There is no mistaking that all the p..."
        wait-message
    else
        $2D message   \ "The man pictured here has a noble air about him. I wonder how long ago this portr..."
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act18 ( -- )   \ 004002C0
    self-wait-done
    9 -40.418 -39.428 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -5.0 0.0 10.0 event-camera
    look-at-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    c2f-3-portrait-3-examined story-flag-set
    2 ebit? not if
        $2B message   \ "What a powerful piece of art. The eyes really feel like they are peering deep int..."
        wait-message
        2 ebit-set
    else c2f-3-portrait-1-examined story-flag? c2f-3-portrait-2-examined story-flag? and if
        $2C message   \ "Every painting features the same person. Perhaps it's the lord of this castle?"
        wait-message
    else
        $2B message   \ "What a powerful piece of art. The eyes really feel like they are peering deep int..."
        wait-message
    then then
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act19 ( -- )   \ 00400330
    self-wait-done
    -9.0 -11.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-3-medallion story-flag-set
            2 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-3.act1A ( -- )   \ 00400390
    begin
        yield
        2 var-dec
        2 0 var? if
            $1E chance? if
                $F 6 228.0 5.0 78.0 0 0 sound
                2 180 var-set
            else
                2 60 var-set
            then
        then
    again
;

: castle-2f-3.act1B ( -- )   \ 004003D0
    self-wait-done
    fiona-who self-look-at
    yield
    begin
        $19 chance? if
            look-about-anim $A self-anim-blend
            self-wait-anim
        else $19 chance? if
            look-about-3-anim $A self-anim-blend
            self-wait-anim
        then then
        yield
    again
;

: castle-2f-3.act1C ( -- )   \ 004003F0
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $E8 -25.0 63.0 -5 char-to-xz
    1 20.0 10.0 40.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    9 ebit? not if
        1 message   \ "There's an antique doll lying here."
        wait-message
        9 ebit-set
    else
        2 message   \ "They say that bisque dolls are supposedly designed with a specific model in mind......"
        wait-message
    then
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act1D ( -- )   \ 00400440
    self-wait-done
    5.0 40.5 self-turn-to-xz
    self-wait-done
    3 message   \ "A birdcage... Right now there is nothing in it."
    wait-message
    self-idle-or-end
;

: castle-2f-3.act1E ( -- )   \ 00400450
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $55 -40.0 12.0 -90 char-to-xz
    1 30.0 20.0 -60.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    4 message   \ "There is a pair of scales lying here. It looks like their purpose is more one of decora..."
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-3.act1F ( -- )   \ 00400498
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $40 message   \ "A chest used to store clothes."
    wait-message
    self-idle-or-end
;

: castle-2f-3.act20 ( -- )   \ 004004B0
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $A ebit? not if
        6 message   \ "The sun is starting to set... And with it will come night..."
        wait-message
        $A ebit-set
    else
        7 message   \ "I should cover as much of this castle as I can while there is still some light left."
        wait-message
    then
    self-idle-or-end
;

: castle-2f-3.act21 ( -- )   \ 004004D0
    begin
        2 cutscene-shot? if
            castle-2f-3.cmd03
        then
        5 cutscene-shot? if
            1 part-hidden
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-2f-3.act22 ( -- )   \ 004004E8
    self-wait-done
    45 self-turn-angle
    self-wait-done
    $31 message   \ "The sun has completely set... It's pitch black out now. I can't see 10 feet out the w..."
    wait-message
    self-idle-or-end
;

: castle-2f-3.act23 ( -- )   \ 00400500
    stalkers-stay state-flag-set
    self-wait-done
    fiona-who 4 self-move-slot
    self-wait-done
    rest-anim $A self-anim-blend
    $F message   \ "Oh no! What should I do?! If I don't do something, that... thing will get me!"
    self-wait-anim
    10 self-idle-anim
    wait-message
    $F 6 fade
    wait-fade
    $40A5 message   \ "'Evasion Points' Press the ? button when near closets or beds to hide and escape en..."
    wait-message
    $F 7 fade
    wait-fade
    $A5 message-param-room
    $A5 1 item-give-count
    $83 $85 0.0 0.0 0.0 0 0 sound
    $801B message   \ "has been saved to your file."
    wait-message
    $3C door-open-clear
    $3C door-lock
    $3D door-lock
    doors-room-in
    force-chased state-flag-set
    $C ebit-set
    self-idle-or-end
;

: castle-2f-3.act24 ( -- )   \ 0047AAF0
    self-wait-done
    $11 message   \ "I have to find a place to hide!"
    wait-message
    self-idle-or-end
;

: castle-2f-3.act25 ( -- )   \ 00400560
    begin
        0 cutscene-shot? not while
        yield
    repeat
    fiona-who $1E char-layer
    begin
        1 cutscene-shot? not while
        yield
    repeat
    fiona-who $A char-layer
    begin
        6 cutscene-shot? not while
        yield
    repeat
    2 3 1.5 light
    begin
        7 cutscene-shot? not while
        yield
    repeat
    0 3 0.0 light
    self-idle-or-end
;

: castle-2f-3.act26 ( -- )   \ 004005A0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $41 message   \ "A chest used to store clothes. Change clothes? YES NO"
    wait-message
    0 answer? if
        7 subscreen-open
        begin
            subscreen-wanted state-flag? while
            yield
        repeat
        0 0.0 0.0 0.0 0.0 event-camera
        $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
            fiona-who $10 6 char-sound
        then
        $26 $28 pvars-equal? not if
            $26 1 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                1 fiona-costume
                $26 1 pvar-set
            else $26 0 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                0 fiona-costume
                $26 0 pvar-set
            else $26 2 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                2 fiona-costume
                $26 2 pvar-set
            else $26 3 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                3 fiona-costume
                $26 3 pvar-set
            else $26 6 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                6 fiona-costume
                $26 6 pvar-set
            else $26 7 pvar? if
                new-game-sounds state-flag-clear
                0 sound-set
                7 fiona-costume
                $26 7 pvar-set
            else $26 8 pvar? if
                new-game-sounds state-flag-set
                1 sound-set
                8 fiona-costume
                $26 8 pvar-set
            then then then then then then then
            effects-arena-flip
        then
        $27 $29 pvars-equal? not if
            $27 0 pvar? if
                0 hewie-model
                $27 0 pvar-set
            else $27 1 pvar? if
                1 hewie-model
                $27 1 pvar-set
            else $27 2 pvar? if
                2 hewie-model
                $27 2 pvar-set
            then then then
            effects-arena-flip
        then
        $26 $28 pvars-equal? not if
            0 char-in
            begin
                yield
                4 sound-bank-loaded? until
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
        then
        $27 $29 pvars-equal? not if
            1 char-in
            hewie-who char-here? if
                hewie-who 2 char-cond? if
                    $1002 hewie-anim-set
                else
                    1 hewie-anim-set
                    $2100 hewie-anim-set
                    $1F00 hewie-anim-set
                    $2000 hewie-anim-set
                then
            then
        then
        $F $41 fade
        wait-fade
    else
        self-frames-reset
        $10 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-2f-3.act27 ( -- )   \ 004006F0
    self-wait-done
    1 part-shown
    1 fiona-costume
    0 char-in
    3 3 $FF char-load
    3 char-start
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    1 part-shown
    fiona-who 1 char-no-shadow
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    1 castle-2f-3.cmd02
    1 castle-2f-3.cmd04
    0 $F9 $A action
    panic-eased
    0 message-prepare   \ "I've gathered some clothes for you. Wait. Don't go. Um, excuse me, but where ar..."
    1 result? if
        scene-play
    then
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    wait-fade
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    $26 1 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        1 fiona-costume
        $26 1 pvar-set
    else $26 0 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
    else $26 2 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        2 fiona-costume
        $26 2 pvar-set
    else $26 3 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
    else $26 6 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        6 fiona-costume
        $26 6 pvar-set
    else $26 7 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        7 fiona-costume
        $26 7 pvar-set
    else $26 8 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        8 fiona-costume
        $26 8 pvar-set
    then then then then then then then
    0 char-in
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-2f-3.act28 ( -- )   \ 00400840
    self-wait-done
    1 fiona-costume
    0 char-in
    $D 3 $FF char-load
    $12 4 $FF char-load
    $18 5 $FF char-load
    3 char-start
    4 char-start
    5 char-start
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    1 part-shown
    $D 1 char-no-shadow
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 $F9 $21 action
    panic-eased
    8 message-prepare   \ "Wait a minute... I remember being in the car with..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    $D 0 char-no-shadow
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $26 1 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        1 fiona-costume
        $26 1 pvar-set
    else $26 0 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        0 fiona-costume
        $26 0 pvar-set
    else $26 2 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        2 fiona-costume
        $26 2 pvar-set
    else $26 3 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
    else $26 6 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        6 fiona-costume
        $26 6 pvar-set
    else $26 7 pvar? if
        new-game-sounds state-flag-clear
        0 sound-set
        7 fiona-costume
        $26 7 pvar-set
    else $26 8 pvar? if
        new-game-sounds state-flag-set
        1 sound-set
        8 fiona-costume
        $26 8 pvar-set
    then then then then then then then
    0 char-in
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-2f-3.act29 ( -- )   \ 00400980
    self-wait-done
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    2 3 $FF char-load
    3 char-start
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $25 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who $A char-layer
    0 3 0.0 light
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-2f-3.act2A ( -- )   \ 00400A30
    self-wait-done
    0 $F1 $12 action
    0 castle-2f-3.cmd02
    0 castle-2f-3.cmd04
    7 2 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    fiona-who 1 char-no-shadow
    0 self-scripted
    panic-eased
    $14 message-prepare   \ "Thank you. You saved me. Come here... Come on, boy. What's your name? Hewie?..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-2f-3 register-scripts   \ (its phases and actions, by their names)
