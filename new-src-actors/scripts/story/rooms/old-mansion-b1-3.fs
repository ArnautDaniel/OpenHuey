\ story/rooms/old-mansion-b1-3.fs - the event scripts of room old-mansion-b1-3 ($4C; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-3
USING: engine actors messages story.state room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant on-plate1                   \ (bit 0) someone on the first plate
1 constant on-plate2                   \ (bit 1) someone on the second plate
2 constant on-plate3                   \ (bit 2) someone on the third plate
3 constant on-plate4                   \ (bit 3) someone on the fourth plate
4 constant flame1-out                  \ (bit 4) the first flame put out (plates 2 and 3)
5 constant flame2-out                  \ (bit 5) the second flame put out (plates 1 and 4)
6 constant flame3-out                  \ (bit 6) the third flame put out (plates 2 and 4)
7 constant flame4-out                  \ (bit 7) the fourth flame put out (plates 1 and 3)
8 constant plates23-held               \ (bit 8) plates 2 and 3 held
9 constant plates14-held               \ (bit 9) plates 1 and 4 held
10 constant plates24-held              \ (bit 10) plates 2 and 4 held
11 constant plates13-held              \ (bit 11) plates 1 and 3 held
12 constant birdcage-remarked          \ (bit 12) its first remark made: "Is this a birdcage? It just seems too large to be a cage for"
13 constant drained-remarked           \ (bit 13) its first remark made: "Any remaining life energy has been totally drained away. Did"
14 constant hewie-in-zone0             \ (bit 14) Hewie in zone 0 (his action there made)
15 constant hewie-in-zone1             \ (bit 15) Hewie in zone 1 (his action there made)
16 constant hewie-in-zone2             \ (bit 16) Hewie in zone 2 (his action there made)
17 constant hewie-in-zone3             \ (bit 17) Hewie in zone 3 (his action there made)
18 constant breaker-remarked           \ (bit 18) its first remark made: "A circuit breaker."
19 constant shaken-free                \ (bit 19) she shook herself free (35 shakes, or near panic)
20 constant panicked                   \ (bit 20) she was near panic when held (the panic's stage 4 after)
21 constant held-scene-on              \ (bit 21) her being held (act02) under way
22 constant hewie-came                 \ (bit 22) Hewie came into zone 4 to help
23 constant found-her                  \ (bit 23) he has found her hiding: she must flee (the avoid prompt)
24 constant will-find-her              \ (bit 24) he will find her hiding (by chance, as he searches: more likely by the progress variable)
25 constant came-while-hidden          \ (bit 25) the stalker came in while she was hiding
26 constant hewie-zone5-noticed        \ (bit 26) Hewie has reacted to zone 5 (once a visit)
27 constant hewie-zone6-noticed        \ (bit 27) Hewie has reacted to zone 6 (once a visit)
28 constant plates12-held              \ (bit 28) plates 1 and 2 held
29 constant plates34-held              \ (bit 29) plates 3 and 4 held
30 constant sound-started              \ (bit 30) its sound started (then kept up)
31 constant cried-out                  \ (bit 31) she cried out (her command; again after 60 frames)
0 constant flames-out-var              \ (var 0) flames put out so far
1 constant threat-frames-var           \ (var 1) frames to the next threat (12)
2 constant cry-frames-var              \ (var 2) frames since her cry (60)
0 constant shut-sound                  \ (snd 0) it closing
1 constant open-sound                  \ (snd 1) it opening
2 constant climb-sound                 \ (snd 2) her climbing
3 constant hide-sound                  \ (snd 3) her hiding
4 constant room-loop-sound             \ (snd 4) its steady sound (started as it comes in, then kept up)
5 constant step-sound                  \ (snd 5) ?
7 constant shut2-sound                 \ (snd 7) ?
8 constant plate-sound                 \ (snd 8) a plate held while a flame is out
9 constant flame-out-sound             \ (snd 9) a flame going out

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
        1 of  fiona-shook @ 35 >= if  shaken-free ebit-set  false fiona-id send count-shakes  then  endof
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
    will-find-her ebit-clear
    $17 0 pvar? if
        0 chance? if
            will-find-her ebit-set
        then
    else $17 1 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $17 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $17 3 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who $10 action
    else
        $78 1 summoner-wait
    then
    $17 pvar-inc
    exit
;

: old-mansion-b1-3.char-enter ( -- )   \ 00409A80
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            old-mansion-b1-3.act0F
        else
            came-while-hidden ebit-clear
        then
    then
;

: old-mansion-b1-3.phase1 ( -- )   \ 00409AD0
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    on-plate1 ebit-clear
    fiona-who $89 char-on-tri? fiona-who $B1 char-on-tri? or if
        on-plate1 ebit-set
    then
    hewie-who $89 char-on-tri? hewie-who $B1 char-on-tri? or hewie-who char-here? and if
        on-plate1 ebit-set
    then
    stalker-who $89 char-on-tri? stalker-who $B1 char-on-tri? or stalker-who char-here? and if
        on-plate1 ebit-set
    then
    on-plate2 ebit-clear
    fiona-who $90 char-on-tri? fiona-who $B2 char-on-tri? or if
        on-plate2 ebit-set
    then
    hewie-who $90 char-on-tri? hewie-who $B2 char-on-tri? or hewie-who char-here? and if
        on-plate2 ebit-set
    then
    stalker-who $90 char-on-tri? stalker-who $B2 char-on-tri? or stalker-who char-here? and if
        on-plate2 ebit-set
    then
    on-plate3 ebit-clear
    fiona-who $97 char-on-tri? fiona-who $B3 char-on-tri? or if
        on-plate3 ebit-set
    then
    hewie-who $97 char-on-tri? hewie-who $B3 char-on-tri? or hewie-who char-here? and if
        on-plate3 ebit-set
    then
    stalker-who $97 char-on-tri? stalker-who $B3 char-on-tri? or stalker-who char-here? and if
        on-plate3 ebit-set
    then
    on-plate4 ebit-clear
    fiona-who $83 char-on-tri? fiona-who $B0 char-on-tri? or if
        on-plate4 ebit-set
    then
    hewie-who $83 char-on-tri? hewie-who $B0 char-on-tri? or hewie-who char-here? and if
        on-plate4 ebit-set
    then
    stalker-who $83 char-on-tri? stalker-who $B0 char-on-tri? or stalker-who char-here? and if
        on-plate4 ebit-set
    then
    1 old-mansion-b1-3.cmd00
    omb1-3-flag2 story-flag? not if
        on-plate2 ebit? on-plate3 ebit? and if
            plates23-held ebit? not flame1-out ebit? not and if
                flames-out-var 0 var? if
                    flames-out-var var-inc
                then
                flame1-out ebit-set
                flame-out-sound 6 -0.5 18.5 -35.5 0 0 sound
                0 effect-remove
            then
            plates23-held ebit-set
        else
            plates23-held ebit-clear
        then
        on-plate1 ebit? on-plate4 ebit? and if
            plates14-held ebit? not flame2-out ebit? not and if
                flames-out-var 1 var? if
                    flames-out-var var-inc
                then
                flame2-out ebit-set
                flame-out-sound 6 9.5 18.5 -35.0 0 0 sound
                1 effect-remove
            then
            plates14-held ebit-set
        else
            plates14-held ebit-clear
        then
        on-plate2 ebit? on-plate4 ebit? and if
            plates24-held ebit? not flame3-out ebit? not and if
                flames-out-var 2 var? if
                    flames-out-var var-inc
                then
                flame3-out ebit-set
                flame-out-sound 6 4.8 25.5 -37.5 0 0 sound
                2 effect-remove
            then
            plates24-held ebit-set
        else
            plates24-held ebit-clear
        then
        on-plate1 ebit? on-plate3 ebit? and if
            plates13-held ebit? not flame4-out ebit? not and if
                flames-out-var 3 var? if
                    flames-out-var var-inc
                then
                flame4-out ebit-set
                flame-out-sound 6 4.8 6.2 -33.4 0 0 sound
                3 effect-remove
            then
            plates13-held ebit-set
        else
            plates13-held ebit-clear
        then
        on-plate1 ebit? on-plate2 ebit? and if
            plates12-held ebit? not if
                flame1-out ebit? flame2-out ebit? or flame3-out ebit? or flame4-out ebit? or if
                    plate-sound 6 4.8 6.2 -33.4 0 0 sound
                then
                flame1-out ebit-clear
                flame2-out ebit-clear
                flame3-out ebit-clear
                flame4-out ebit-clear
                0 -0.5 18.5 -35.5 2 flame-in
                1 9.5 18.5 -35.0 2 flame-in
                2 4.8 25.5 -37.5 2 flame-in
                3 4.8 6.2 -33.4 2 flame-in
                flames-out-var 0 var-set
            then
            plates12-held ebit-set
        else
            plates12-held ebit-clear
        then
        on-plate3 ebit? on-plate4 ebit? and if
            plates34-held ebit? not if
                flame1-out ebit? flame2-out ebit? or flame3-out ebit? or flame4-out ebit? or if
                    plate-sound 6 4.8 6.2 -33.4 0 0 sound
                then
                flame1-out ebit-clear
                flame2-out ebit-clear
                flame3-out ebit-clear
                flame4-out ebit-clear
                0 -0.5 18.5 -35.5 2 flame-in
                1 9.5 18.5 -35.0 2 flame-in
                2 4.8 25.5 -37.5 2 flame-in
                3 4.8 6.2 -33.4 2 flame-in
                flames-out-var 0 var-set
            then
            plates34-held ebit-set
        else
            plates34-held ebit-clear
        then
        flame1-out ebit? flame2-out ebit? and flame3-out ebit? and flame4-out ebit? and if
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
        hewie-in-zone0 ebit? not if
            0 23.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        hewie-in-zone0 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            0 23.0 0.0 -22.5 7 5 0 zone
            hewie-who 0 9 char-zone-bits? not if
                hewie-in-zone0 ebit-clear
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        hewie-in-zone1 ebit? not if
            1 11.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        hewie-in-zone1 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            1 11.0 0.0 -22.5 7 5 0 zone
            hewie-who 1 9 char-zone-bits? not if
                hewie-in-zone1 ebit-clear
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        hewie-in-zone2 ebit? not if
            2 -1.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 2 9 char-zone-bits? if
                        hewie-in-zone2 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            2 -1.0 0.0 -22.5 7 5 0 zone
            hewie-who 2 9 char-zone-bits? not if
                hewie-in-zone2 ebit-clear
            then
        then
    then
    hewie-who char-here? hewie-who 0 char-cond? and if
        hewie-in-zone3 ebit? not if
            3 -13.0 0.0 -22.5 5 5 0 zone
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        hewie-in-zone3 ebit-set
                        $80 0 hewie-do
                    then
                then
            then
        else
            3 -13.0 0.0 -22.5 7 5 0 zone
            hewie-who 3 9 char-zone-bits? not if
                hewie-in-zone3 ebit-clear
            then
        then
    then
    held-scene-on ebit? hewie-came ebit? not and if
        4 10.8 0.0 14.07 $A 200 0 zone
        hewie-who 4 char-in-zone? if
            hewie-came ebit-set
        then
    then
    held-scene-on ebit? not if
        5 37.99 0.0 24.16 $14 14 0 zone
        hewie-zone5-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 5 9 char-zone-bits? if
                        hewie-zone5-noticed ebit-set
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
            hewie-zone6-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 6 9 char-zone-bits? if
                            hewie-zone6-noticed ebit-set
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
    sound-started ebit? not if
        6 sound-bank-loaded? if
            room-loop-sound volume-scaled 6 40.0 5.0 -26.0 0 0 sound
            sound-started ebit-set
        then
    else
        room-loop-sound kept-going volume-scaled 6 40.0 5.0 -26.0 0 0 sound
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
    [: hewie-who char-busy? ;] wait-until
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
        0 say   \ "A corpse. There are traces that suggest this person was heavily tortured... There i..."
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
            $17 wait-frames
            fiona-who step-sound 6 char-sound
            self-wait-anim
        else
            1 27.0 10.0 -40.0 0.0 event-camera
            fiona-who $2F 12.34 13.19 180 char-to-xz
            $11 9.97 0.0 -0.59 0 char-to-xyz
            1 self-scripted
            $11 $9001 0 0 char-anim-hold
            3 room-motion self-anim
        then
        held-scene-on ebit-set
        hewie-came ebit-clear
        shaken-free ebit-clear
        panicked ebit-clear
        0 old-mansion-b1-3.cmd01
        begin
            hewie-can-command? if
                $61 0 hewie-do
            then
            0 control-action? 1 control-action? or if
                cried-out ebit? not if
                    cried-out ebit-set
                    fiona-who $38 5 char-sound
                then
            then
            cried-out ebit? if
                cry-frames-var var-inc
                cry-frames-var 60 var? if
                    cry-frames-var 0 var-set
                    cried-out ebit-clear
                then
            then
            1 old-mansion-b1-3.cmd01
            threat-frames-var var-inc
            threat-frames-var 12 var? if
                $A threat-add
                threat-frames-var 0 var-set
            then
            panic-near-full? if
                shaken-free ebit-set
                panicked ebit-set
            then
            shaken-free ebit? not if
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
            hewie-came ebit? not if
                self-at-motion-event? not if
                    1 wait-frames
                    false
                else shaken-free ebit? not if
                    1 wait-frames
                    false
                else
                    true
                then then
            else
                true
            then
        until
        held-scene-on ebit-clear
        0 0.0 0.0 0.0 0.0 event-camera
        hewie-can-command? if
            0 0 hewie-do
        then
        $11 $9003 0 5 char-anim-hold
        5 room-motion 3 blend-out
        $11 $9005 0 5 char-anim-hold
        panicked ebit? if
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
    4 wait-frames
    3 say   \ "Is this a tool used to torture innocent victims? Tiny letters are engraved..."
    $F 6 fade
    wait-fade
    9 say   \ "Fire is essentially composed of hotness and dryness. Air differs only from fire in that..."
    $F 7 fade
    wait-fade
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    4 say   \ "'Hotness and dryness form the element of fire' What could it mean?"
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-3.act04 ( -- )   \ 0040A4F0
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    drained-remarked ebit? not if
        1 say   \ "Any remaining life energy has been totally drained away. Did the statue suck its li..."
        drained-remarked ebit-set
    else
        2 say   \ "Strange, but the expression on its face looks somehow... peaceful. Maybe it was her..."
        drained-remarked ebit-clear
    then
    self-idle-or-end
;

: old-mansion-b1-3.act05 ( -- )   \ 0040A510
    self-wait-done
    10.0 0.0 self-turn-to-xz
    self-wait-done
    reach-out-anim play-out
    4 wait-frames
    got-venus-key story-flag-set
    0 old-mansion-b1-3.cmd02
    $F message-param-room
    $F 1 item-give-count
    0 $F item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: old-mansion-b1-3.act0E ( -- )   \ 0040A790
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    2 room-motion 5 self-anim-blend8
    5 wait-frames
    fiona-who climb-sound 6 char-sound
    $E 2 object-anim
    self-wait-anim
    0 self-noclip
    found-her ebit? not if
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
        $13 say   \ "Huge spikes jut out from inside the iron maiden, rendering the device unusable as..."
    else
        stalkers-stay state-flag-set
        came-while-hidden ebit-clear
        1 self-scripted
        fiona-who 9 char-file-load
        self-wait-done
        $3C 32.0 20.63 90 $FFFF 5 self-move-to
        self-wait-done
        1 self-noclip
        fiona-who char-file-use
        0 room-motion 5 self-anim-blend8
        5 wait-frames
        2 part-hidden
        $E 0 object-hidden
        $E 0 object-anim
        $24 wait-frames
        fiona-who climb-sound 6 char-sound
        self-wait-anim
        -1 self-idle-anim
        0 self-noclip
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-set
        found-her ebit-clear
        0 avoid-prompt
        $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
            fiona-who hide-sound 6 char-sound
        then
        self-who 3 -1 char-camera
        begin
            0 2 pad? not if
                in-panic? found-her ebit? or if
                    in-panic? not if
                        fiona-who seized-sound 5 char-sound
                    then
                    ['] old-mansion-b1-3.act0E goto
                else
                    2 panic-calm-in
                    6 fiona-calm
                    $1E fiona-recovery-lower
                    came-while-hidden ebit? stalker-who char-here? not and if
                        came-while-hidden ebit-clear
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
        5 wait-frames
        fiona-who climb-sound 6 char-sound
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
        5 wait-frames
        $A 1 object-hidden
        $B 0 object-hidden
        $F 1 object-hidden
        $10 0 object-hidden
        $B 0 object-anim
        $10 0 object-anim
        $19 wait-frames
        fiona-who open-sound 6 char-sound
        $37 wait-frames
        fiona-who shut-sound 6 char-sound
        self-wait-anim
        omb1-3-flag3 story-flag-set
    else
        3 room-motion 5 self-anim-blend8
        5 wait-frames
        $A 0 object-hidden
        $B 1 object-hidden
        $F 0 object-hidden
        $10 1 object-hidden
        $A 0 object-anim
        $F 0 object-anim
        $19 wait-frames
        fiona-who open-sound 6 char-sound
        $37 wait-frames
        fiona-who shut-sound 6 char-sound
        5 wait-frames
        fiona-who shut2-sound 6 char-sound
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
    $A say   \ "The oven burns an angry red. I'd rather not even think about the kind of things they b..."
    self-idle-or-end
;

: old-mansion-b1-3.act09 ( -- )   \ 0040A710
    self-wait-done
    90 self-turn-angle
    self-wait-done
    birdcage-remarked ebit? not if
        $B say   \ "Is this a birdcage? It just seems too large to be a cage for birds."
        birdcage-remarked ebit-set
    else
        $C say   \ "Could it have housed a person? No. No one could fit inside it."
        birdcage-remarked ebit-clear
    then
    self-idle-or-end
;

: old-mansion-b1-3.act0A ( -- )   \ 0040A728
    self-wait-done
    0 self-turn-angle
    self-wait-done
    $D say   \ "There's a handle that winds the attached rope up. Looks like this was used as a stretc..."
    self-idle-or-end
;

: old-mansion-b1-3.act0B ( -- )   \ 0040A738
    self-wait-done
    -45 self-turn-angle
    self-wait-done
    $E say   \ "Thin-bladed sabers line the wall. Several of them appear to be coated with rust and ca..."
    self-idle-or-end
;

: old-mansion-b1-3.act0C ( -- )   \ 0040A750
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    breaker-remarked ebit? not if
        $F say   \ "A circuit breaker."
        breaker-remarked ebit-set
    else
        10.0 10.0 0.0 self-look-at-point
        yield
        $10 say   \ "The end of the cord connects to that chair... Which can mean only one thing. This..."
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
    16 wait-frames
    $12 say   \ "Restraints, a whip, and various surgical instruments. Dried blood covers all of the s..."
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
        there-anim play-out
        found-her ebit-set
        10 self-idle-anim
        [: fiona-who char-busy? ;] wait-while
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
        reach-out-anim play-out
        4 wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-3-camomile story-flag-set
            5 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-3.act13 ( -- )   \ 0040A968
    $11 1 char-no-shadow
    [: 2 cutscene-shot? ;] wait-until
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
