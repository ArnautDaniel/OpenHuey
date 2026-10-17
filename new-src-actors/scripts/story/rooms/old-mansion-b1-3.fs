\ story/rooms/old-mansion-b1-3.fs - the event scripts of room old-mansion-b1-3 ($4C; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-3
USING: engine actors messages story.state room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ four room objects (Room4C_ObjectNames) pressed in (+0x24 down 0.2 to -0.7, a sound as each
\ starts) while event flag i is set, else back up 0.2 to 0; byte 3 0 all reset
: old-mansion-b1-3.cmd00 ( b0 -- )   \ (the plates "fumi0".."fumi3": each pressed down to -0.7 while its event bit
    \ is set - a sound as it starts - else back up to 0; b0 0 all up at once)
    {: b0 | a n :}
    4 0 do
        i room-object to n to a
        a n object-here? if
            b0 0= if  0e a n place-y object!
            else i ebit? if
                a n place-y object@ f0= if  6 6 0 0  a n place-x object@ a n place-y object@ a n place-z object@  sound  then
                -0.2e a n place-y object+!  a n place-y object@ -0.7e fmax a n place-y object!
            else
                0.2e a n place-y object+!  a n place-y object@ 0e fmin a n place-y object!
            then then
        then
    loop ;
\ room 0x4C (Room4C_Cmd01_ptmf): byte 3 0 starts counting what the player does (her +0x1AD710
\ on), 1 adds this frame's (Fiona_Shakes); at 35 events bit 0x13
: old-mansion-b1-3.cmd01 ( b0 -- )   \ (her shakes counted: b0 0 from now; 1 at 35 of them, event bit $13)
    case
        0 of  0 fiona-shook !  true fiona-id send count-shakes  endof
        1 of  fiona-shook @ 35 >= if  $13 ebit-set  false fiona-id send count-shakes  then  endof
    endcase ;
\ room 0x4C (D_0040AC08): character 0x11's model parts 0xA2 / 0xA4 / 0xAC get bit 2 when byte 3
\ is 0, else lose it
: old-mansion-b1-3.cmd02 ( b0 -- )   \ (character $11's model parts 5, 6 and 10 hidden (b0 0) or shown - the original's +$A2 / +$A4 / +$AC)
    0<> {: on :}  $11 5 on show-part  $11 6 on show-part  $11 10 on show-part ;

: old-mansion-b1-3.enter ( -- )   \ 00409980
    room-sounds
    0 old-mansion-b1-3.cmd00
    omb1-3-flag2 story-flag? not if
        0 -0.5 18.5 -35.5 2 flame-in
        1 9.5 18.5 -35.0 2 flame-in
        2 4.8 25.5 -37.5 2 flame-in
        3 4.8 6.2 -33.4 2 flame-in
    else
        0 -0.5 18.5 -35.5 3 flame-in
        1 9.5 18.5 -35.0 3 flame-in
        2 4.8 25.5 -37.5 3 flame-in
        3 4.8 6.2 -33.4 3 flame-in
    then
    3 char-start
    $11 char-activate
    got-venus-key story-flag? if
        0 old-mansion-b1-3.cmd02
    else
        1 old-mansion-b1-3.cmd02
    then
    $11 $9005 1 0 char-anim-hold
    $11 9.97 0.0 -0.59 0 char-to-xyz
    omb1-3-flag3 story-flag? not if
        $A 1 object-hidden
        $B 0 object-hidden
        $F 1 object-hidden
        $10 0 object-hidden
    else
        $A 0 object-hidden
        $B 1 object-hidden
        $F 0 object-hidden
        $10 1 object-hidden
    then
    2 part-shown
    $E 1 object-hidden
    5 part-hidden
    omb1-3-camomile story-flag? not if
        5 -20.96 8.5 5.57 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-b1-3.act0F ( -- )   \ 0040A7D0
    $18 ebit-clear
    $17 0 pvar? if
        0 chance? if
            $18 ebit-set
        then
    else $17 1 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    else $17 2 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    else $17 3 pvar? if
        $32 chance? if
            $18 ebit-set
        then
    then then then then
    2 creature-action? if
        $18 ebit-set
    then
    $18 ebit? if
        0 stalker-who $10 action
    else
        $78 1 summoner-wait
    then
    $17 pvar-inc
    exit
;

: old-mansion-b1-3.char-enter ( -- )   \ 00409A80
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            $19 ebit-set
            old-mansion-b1-3.act0F
        else
            $19 ebit-clear
        then
    then
;

: old-mansion-b1-3.phase1 ( -- )   \ 00409AD0
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    0 ebit-clear
    fiona-who $89 char-on-tri? fiona-who $B1 char-on-tri? or if
        0 ebit-set
    then
    hewie-who $89 char-on-tri? hewie-who $B1 char-on-tri? or hewie-who char-here? and if
        0 ebit-set
    then
    stalker-who $89 char-on-tri? stalker-who $B1 char-on-tri? or stalker-who char-here? and if
        0 ebit-set
    then
    1 ebit-clear
    fiona-who $90 char-on-tri? fiona-who $B2 char-on-tri? or if
        1 ebit-set
    then
    hewie-who $90 char-on-tri? hewie-who $B2 char-on-tri? or hewie-who char-here? and if
        1 ebit-set
    then
    stalker-who $90 char-on-tri? stalker-who $B2 char-on-tri? or stalker-who char-here? and if
        1 ebit-set
    then
    2 ebit-clear
    fiona-who $97 char-on-tri? fiona-who $B3 char-on-tri? or if
        2 ebit-set
    then
    hewie-who $97 char-on-tri? hewie-who $B3 char-on-tri? or hewie-who char-here? and if
        2 ebit-set
    then
    stalker-who $97 char-on-tri? stalker-who $B3 char-on-tri? or stalker-who char-here? and if
        2 ebit-set
    then
    3 ebit-clear
    fiona-who $83 char-on-tri? fiona-who $B0 char-on-tri? or if
        3 ebit-set
    then
    hewie-who $83 char-on-tri? hewie-who $B0 char-on-tri? or hewie-who char-here? and if
        3 ebit-set
    then
    stalker-who $83 char-on-tri? stalker-who $B0 char-on-tri? or stalker-who char-here? and if
        3 ebit-set
    then
    1 old-mansion-b1-3.cmd00
    omb1-3-flag2 story-flag? not if
        1 ebit? 2 ebit? and if
            8 ebit? not 4 ebit? not and if
                0 0 var? if
                    0 var-inc
                then
                4 ebit-set
                9 6 -0.5 18.5 -35.5 0 0 sound
                0 effect-remove
            then
            8 ebit-set
        else
            8 ebit-clear
        then
        0 ebit? 3 ebit? and if
            9 ebit? not 5 ebit? not and if
                0 1 var? if
                    0 var-inc
                then
                5 ebit-set
                9 6 9.5 18.5 -35.0 0 0 sound
                1 effect-remove
            then
            9 ebit-set
        else
            9 ebit-clear
        then
        1 ebit? 3 ebit? and if
            $A ebit? not 6 ebit? not and if
                0 2 var? if
                    0 var-inc
                then
                6 ebit-set
                9 6 4.8 25.5 -37.5 0 0 sound
                2 effect-remove
            then
            $A ebit-set
        else
            $A ebit-clear
        then
        0 ebit? 2 ebit? and if
            $B ebit? not 7 ebit? not and if
                0 3 var? if
                    0 var-inc
                then
                7 ebit-set
                9 6 4.8 6.2 -33.4 0 0 sound
                3 effect-remove
            then
            $B ebit-set
        else
            $B ebit-clear
        then
        0 ebit? 1 ebit? and if
            $1C ebit? not if
                4 ebit? 5 ebit? or 6 ebit? or 7 ebit? or if
                    8 6 4.8 6.2 -33.4 0 0 sound
                then
                4 ebit-clear
                5 ebit-clear
                6 ebit-clear
                7 ebit-clear
                0 -0.5 18.5 -35.5 2 flame-in
                1 9.5 18.5 -35.0 2 flame-in
                2 4.8 25.5 -37.5 2 flame-in
                3 4.8 6.2 -33.4 2 flame-in
                0 0 var-set
            then
            $1C ebit-set
        else
            $1C ebit-clear
        then
        2 ebit? 3 ebit? and if
            $1D ebit? not if
                4 ebit? 5 ebit? or 6 ebit? or 7 ebit? or if
                    8 6 4.8 6.2 -33.4 0 0 sound
                then
                4 ebit-clear
                5 ebit-clear
                6 ebit-clear
                7 ebit-clear
                0 -0.5 18.5 -35.5 2 flame-in
                1 9.5 18.5 -35.0 2 flame-in
                2 4.8 25.5 -37.5 2 flame-in
                3 4.8 6.2 -33.4 2 flame-in
                0 0 var-set
            then
            $1D ebit-set
        else
            $1D ebit-clear
        then
        4 ebit? 5 ebit? and 6 ebit? and 7 ebit? and if
            fiona-who char-busy? not if
                omb1-3-flag2 story-flag-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 0 action-force
                else
                    1 fiona-who 0 action-force
                then
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        $E ebit? not if
            0 23.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        $E ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            0 23.0 0.0 -22.5 7 5 0 zone
            hewie-who 0 9 char-zone-bits? not if
                $E ebit-clear
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        $F ebit? not if
            1 11.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        $F ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            1 11.0 0.0 -22.5 7 5 0 zone
            hewie-who 1 9 char-zone-bits? not if
                $F ebit-clear
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        $10 ebit? not if
            2 -1.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 2 9 char-zone-bits? if
                        $10 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            2 -1.0 0.0 -22.5 7 5 0 zone
            hewie-who 2 9 char-zone-bits? not if
                $10 ebit-clear
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        $11 ebit? not if
            3 -13.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        $11 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            3 -13.0 0.0 -22.5 7 5 0 zone
            hewie-who 3 9 char-zone-bits? not if
                $11 ebit-clear
            then
        then
    then
    $15 ebit? $16 ebit? not and if
        4 10.8 0.0 14.07 $A 200 0 zone
        hewie-who 4 char-in-zone? if
            $16 ebit-set
        then
    then
    $15 ebit? not if
        5 37.99 0.0 24.16 $14 14 0 zone
        $1A ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 5 9 char-zone-bits? if
                        $1A ebit-set
                        $32 chance? if
                            hewie-zone-var 5 var-set
                            0 hewie-who $92 action
                        then
                    then
                then
            then
        then
        5 37.99 0.0 24.16 $14 14 0 zone
        hewie-who char-here? 2 game-mode? not and if
            hewie-can-command? if
                hewie-who 5 9 char-zone-bits? if
                    hewie-zone-var 5 var-set
                    hewie-zone-var 14.0 hewie-look-zone
                then
            then
        then
        omb1-3-flag2 story-flag? not if
            6 11.81 0.0 -0.03 $14 6 0 zone
            $1B ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 6 9 char-zone-bits? if
                            $1B ebit-set
                            $64 chance? if
                                hewie-zone-var 6 var-set
                                0 hewie-who $8A action
                            then
                        then
                    then
                then
            then
            6 11.81 0.0 -0.03 $14 6 0 zone
            hewie-who char-here? 1 game-mode? and if
                hewie-can-command? if
                    hewie-who 6 9 char-zone-bits? if
                        hewie-zone-var 6 var-set
                        hewie-zone-var 6.0 hewie-look-zone
                    then
                then
            then
        then
    then
    $1E ebit? not if
        6 sound-bank-loaded? if
            $40000004 6 40.0 5.0 -26.0 0 0 sound
            $1E ebit-set
        then
    else
        $C0000004 6 40.0 5.0 -26.0 0 0 sound
    then
;

: old-mansion-b1-3.phase2 ( -- )   \ 0040A020
    fiona-who 3 char-in-area? fiona-who 90 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who 4 char-in-area? fiona-who 10 0 $32 char-faces-xz? and if
        omb1-3-flag2 story-flag? not if
            2 unhurried offer-action
        else got-venus-key story-flag? not if
            5 calm offer-action
        else
            4 unhurried offer-action
        then then
    then
    fiona-who 5 char-in-area? fiona-who 45 $32 char-heading? and if
        omb1-3-flag3 story-flag? not if
            6 unhurried offer-action
        else stalker-who char-here? not if
            6 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then then
    then
    fiona-who 6 char-in-area? fiona-who 0 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 55 $32 char-heading? and if
        8 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who 45 $32 char-heading? and if
        9 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who 0 $32 char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who -22 $32 char-heading? and if
        $B unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who -45 $32 char-heading? and if
        $C unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who -45 $32 char-heading? and if
        $D unhurried offer-action
    then
    omb1-3-camomile story-flag? not if
        7 5 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            $12 calm offer-action
        then
    then
;

: old-mansion-b1-3.act00 ( -- )   \ 0040A0F0
    0 counter-set
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 cutscene-track $13 action
    self-who 1 char-visible
    hewie-who char-here? if
        0 hewie-who 1 action-force
        1 wait-counter
    then
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    50 hewie-trust
    0 self-idle-anim
    camera-restart
    self-who 0 char-visible
    counter-inc
    0 -0.5 18.5 -35.5 3 flame-in
    1 9.5 18.5 -35.0 3 flame-in
    2 4.8 25.5 -37.5 3 flame-in
    3 4.8 6.2 -33.4 3 flame-in
    $11 $9005 1 0 char-anim-hold
    $11 9.97 0.0 -0.59 0 char-to-xyz
    $11 0 char-no-shadow
    stalker-who $4C 0 room-doors-state
    $F $41 fade
    wait-fade
    $827E item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act01 ( -- )   \ 0040A200
    begin
        hewie-who char-busy? not while
        yield
    repeat
    self-who 1 char-visible
    counter-inc
    2 wait-counter
    self-who 0 char-visible
    self-idle-or-end
;

: old-mansion-b1-3.act02 ( -- )   \ 0040A220
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    omb1-3-flag1 story-flag? not if
        $8D 9.0 10.0 180 $FFFF 5 self-move-to
        self-wait-done
        0 message   \ "A corpse. There are traces that suggest this person was heavily tortured... There i..."
        wait-message
        0 answer? if
            $F $44 fade
            5 0 movie-play
            4 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            hewie-who char-here? hewie-who 2 char-cond? and if
                0 hewie-who $86 action
            then
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            wait-fade
            0 self-idle-anim
            fiona-who $2F 10.8 14.07 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            $11 $9005 0 0 char-anim-hold
            camera-restart
            hewie-who 0 char-visible
            hewie-who action-end
            world-held state-flag-set
            fiona-who 8 char-file-load
            fiona-who char-file-use
            world-held state-flag-clear
            $F $41 fade
        then
    then
    0 answer? if
        self-wait-done
        omb1-3-flag1 story-flag? if
            fiona-who 8 char-file-load
            fiona-who char-file-use
            1 27.0 10.0 -40.0 0.0 event-camera
            fiona-who $2F 10.8 14.07 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9000 0 0 char-anim-hold
            2 room-motion 5 self-anim-blend
            self-frames-reset
            $17 self-wait-frames
            fiona-who 5 6 char-sound
            self-wait-anim
        else
            1 27.0 10.0 -40.0 0.0 event-camera
            fiona-who $2F 12.34 13.19 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9001 0 0 char-anim-hold
            3 room-motion self-anim
        then
        $15 ebit-set
        $16 ebit-clear
        $13 ebit-clear
        $14 ebit-clear
        0 old-mansion-b1-3.cmd01
        begin
            hewie-can-command? if
                $61 0 hewie-do
            then
            0 control-action? 1 control-action? or if
                $1F ebit? not if
                    $1F ebit-set
                    fiona-who $38 5 char-sound
                then
            then
            $1F ebit? if
                2 var-inc
                2 60 var? if
                    2 0 var-set
                    $1F ebit-clear
                then
            then
            1 old-mansion-b1-3.cmd01
            1 var-inc
            1 12 var? if
                $A threat-add
                1 0 var-set
            then
            panic-near-full? if
                $13 ebit-set
                $14 ebit-set
            then
            $13 ebit? not if
                self-at-motion-event? if
                    0 panic-stage? 1 panic-stage? or if
                        $11 $9001 0 5 char-anim-hold
                        3 room-motion 5 self-anim-blend
                    then
                    2 panic-stage? 3 panic-stage? or if
                        $11 $9002 0 5 char-anim-hold
                        4 room-motion 5 self-anim-blend
                    then
                then
            then
            $16 ebit? not if
                self-at-motion-event? not if
                    self-frames-reset
                    1 self-wait-frames
                    false
                else $13 ebit? not if
                    self-frames-reset
                    1 self-wait-frames
                    false
                else
                    true
                then then
            else
                true
            then
        until
        $15 ebit-clear
        0 0.0 0.0 0.0 0.0 event-camera
        hewie-can-command? if
            0 0 hewie-do
        then
        $11 $9003 0 5 char-anim-hold
        5 room-motion 3 self-anim-blend
        self-wait-anim
        $11 $9005 0 5 char-anim-hold
        $14 ebit? if
            4 panic-stage
        then
        omb1-3-flag1 story-flag-set
        $2F resident-flag-set
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act03 ( -- )   \ 0040A490
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    world-frozen state-flag-set
    1 20.0 -15.0 0.0 0.0 event-camera
    fiona-who $97 5.0 -27.0 180 char-to-xz
    self-frames-reset
    4 self-wait-frames
    3 message   \ "Is this a tool used to torture innocent victims? Tiny letters are engraved..."
    wait-message
    $F 6 fade
    wait-fade
    9 message   \ "Fire is essentially composed of hotness and dryness. Air differs only from fire in that..."
    wait-message
    $F 7 fade
    wait-fade
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    4 message   \ "'Hotness and dryness form the element of fire' What could it mean?"
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act04 ( -- )   \ 0040A4F0
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    $D ebit? not if
        1 message   \ "Any remaining life energy has been totally drained away. Did the statue suck its li..."
        wait-message
        $D ebit-set
    else
        2 message   \ "Strange, but the expression on its face looks somehow... peaceful. Maybe it was her..."
        wait-message
        $D ebit-clear
    then
    self-idle-or-end
;

: old-mansion-b1-3.act05 ( -- )   \ 0040A510
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    reach-out-anim self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-venus-key story-flag-set
    0 old-mansion-b1-3.cmd02
    $F message-param-room
    $F 1 item-give-count
    0 $F item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    reach-back-anim self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-3.act0E ( -- )   \ 0040A790
    $17 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    2 room-motion 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    fiona-who 2 6 char-sound
    $E 2 object-anim
    self-wait-anim
    0 self-noclip
    $17 ebit? not if
        stalker-who action-end
        stalker-who char-here? if
            stalker-who 0 stalker-command
        then
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act06 ( -- )   \ 0040A560
    omb1-3-flag3 story-flag? not if
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        $13 message   \ "Huge spikes jut out from inside the iron maiden, rendering the device unusable as..."
        wait-message
    else
        stalkers-stay state-flag-set
        $19 ebit-clear
        1 self-scripted
        fiona-who 9 char-file-load
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        1 self-noclip
        fiona-who char-file-use
        0 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        2 part-hidden
        $E 0 object-hidden
        $E 0 object-anim
        self-frames-reset
        $24 self-wait-frames
        fiona-who 2 6 char-sound
        self-wait-anim
        -1 self-idle-anim
        0 self-noclip
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-set
        $17 ebit-clear
        0 avoid-prompt
        $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
            fiona-who 3 6 char-sound
        then
        self-who 3 -1 char-camera
        begin
            0 2 pad? not if
                in-panic? $17 ebit? or if
                    in-panic? not if
                        fiona-who seized-sound 5 char-sound
                    then
                    ['] old-mansion-b1-3.act0E goto
                else
                    2 panic-calm-in
                    6 fiona-calm
                    $1E fiona-recovery-lower
                    $19 ebit? stalker-who char-here? not and if
                        $19 ebit-clear
                        1 avoid-prompt
                    then
                    yield
                    false
                then
            else
                true
            then
        until
        stalker-who char-here? if
            ['] old-mansion-b1-3.act0E goto
        then
        fiona-who camera-follow
        fiona-hidden state-flag-clear
        1 self-noclip
        1 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        fiona-who 2 6 char-sound
        $E 1 object-anim
        self-wait-anim
        0 self-noclip
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-b1-3.act07 ( -- )   \ 0040A650
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    fiona-who 9 char-file-load
    5 9.83 27.45 0 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    omb1-3-flag3 story-flag? not if
        4 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        $A 1 object-hidden
        $B 0 object-hidden
        $F 1 object-hidden
        $10 0 object-hidden
        $B 0 object-anim
        $10 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        fiona-who 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        fiona-who 0 6 char-sound
        self-wait-anim
        omb1-3-flag3 story-flag-set
    else
        3 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        $A 0 object-hidden
        $B 1 object-hidden
        $F 0 object-hidden
        $10 1 object-hidden
        $A 0 object-anim
        $F 0 object-anim
        self-frames-reset
        $19 self-wait-frames
        fiona-who 1 6 char-sound
        self-frames-reset
        $37 self-wait-frames
        fiona-who 0 6 char-sound
        self-frames-reset
        5 self-wait-frames
        fiona-who 7 6 char-sound
        self-wait-anim
        omb1-3-flag3 story-flag-clear
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-3.act08 ( -- )   \ 0040A700
    self-wait-done
    110 self-turn-angle
    self-wait-done
    $A message   \ "The oven burns an angry red. I'd rather not even think about the kind of things they b..."
    wait-message
    self-idle-or-end
;

: old-mansion-b1-3.act09 ( -- )   \ 0040A710
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $C ebit? not if
        $B message   \ "Is this a birdcage? It just seems too large to be a cage for birds."
        wait-message
        $C ebit-set
    else
        $C message   \ "Could it have housed a person? No. No one could fit inside it."
        wait-message
        $C ebit-clear
    then
    self-idle-or-end
;

: old-mansion-b1-3.act0A ( -- )   \ 0040A728
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $D message   \ "There's a handle that winds the attached rope up. Looks like this was used as a stretc..."
    wait-message
    self-idle-or-end
;

: old-mansion-b1-3.act0B ( -- )   \ 0040A738
    self-wait-done
    -45 self-turn-angle
    self-wait-done
    $E message   \ "Thin-bladed sabers line the wall. Several of them appear to be coated with rust and ca..."
    wait-message
    self-idle-or-end
;

: old-mansion-b1-3.act0C ( -- )   \ 0040A750
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    $12 ebit? not if
        $F message   \ "A circuit breaker."
        wait-message
        $12 ebit-set
    else
        10.0 10.0 0.0 self-look-at-point
        yield
        $10 message   \ "The end of the cord connects to that chair... Which can mean only one thing. This..."
        wait-message
        self-who self-look-at
        yield
    then
    self-idle-or-end
;

: old-mansion-b1-3.act0D ( -- )   \ 0040A780
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    self-wait-16
    $12 message   \ "Restraints, a whip, and various surgical instruments. Dried blood covers all of the s..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-3.act10 ( -- )   \ 0040A820
    3 self-is? $22 self-is? or if
        1 self-scripted
        things-clear
        0 counter-set
        panic-eased
        0 fiona-who $11 action-force
        1 self-scripted
        self-wait-done
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        counter-inc
        begin
            yield
        again
    else
        self-wait-done
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
        7 26.16 27.36 118 $FFFF 5 self-move-to
        self-wait-done
        there-anim self-anim
        self-wait-anim
        $17 ebit-set
        10 self-idle-anim
        begin
            fiona-who char-busy? while
            yield
        repeat
        stalker-who 0 stalker-command
    then
    self-idle-or-end
;

: old-mansion-b1-3.act11 ( -- )   \ 0040A890
    capture-no-end state-flag-set
    no-pause state-flag-set
    1 wait-counter
    $F $44 fade
    $12 1 movie-play
    $11 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    $A 1 object-hidden
    $B 1 object-hidden
    3 stalker-kind? $22 stalker-kind? or if
        3 4 0 char-model-op
    then
    $10 $50 movie-param
    fiona-who 1 char-visible
    panic-eased
    $FFFF message-prepare
    1 result? if
        movie-loop
        scene-play-through
    then
    $2E resident-flag-set
    0 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: old-mansion-b1-3.act12 ( -- )   \ 0040A910
    self-wait-done
    -20.96 5.57 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            omb1-3-camomile story-flag-set
            5 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-b1-3.act13 ( -- )   \ 0040A968
    $11 1 char-no-shadow
    begin
        2 cutscene-shot? not while
        yield
    repeat
    $11 0 char-no-shadow
    self-idle-or-end
;

: old-mansion-b1-3.act14 ( -- )   \ 0040A980
    self-wait-done
    3 partner-load
    2 char-start
    $12 1 movie-play
    $11 cutscene-start
    yield
    yield
    scene-wait-ready
    $A 1 object-hidden
    $B 1 object-hidden
    $F 0 object-hidden
    $10 1 object-hidden
    $E 1 object-hidden
    3 4 0 char-model-op
    $10 $50 movie-param
    fiona-who 1 char-visible
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who 0 char-visible
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-b1-3.act15 ( -- )   \ 0040AA30
    self-wait-done
    $A 1 object-hidden
    $B 0 object-hidden
    $F 1 object-hidden
    $10 0 object-hidden
    $E 1 object-hidden
    $11 3 $FF char-load
    3 char-start
    $F $44 fade
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

: old-mansion-b1-3.act16 ( -- )   \ 0040AAD0
    self-wait-done
    $A 1 object-hidden
    $B 0 object-hidden
    $F 1 object-hidden
    $10 0 object-hidden
    $E 1 object-hidden
    $11 3 $FF char-load
    3 char-start
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $13 action
    self-who 1 char-visible
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    self-who 0 char-visible
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-b1-3 register-scripts   \ (its phases and actions, by their names)
