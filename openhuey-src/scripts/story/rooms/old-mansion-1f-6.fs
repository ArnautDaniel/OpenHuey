\ story/rooms/old-mansion-1f-6.fs - the event scripts of room old-mansion-1f-6 ($46; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-6
USING: engine room-names facts story.words story.shared flag-names story-names fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant in-zone0                    \ (bit 0) someone in zone 0 (the figure's hops count)
1 constant hewie-zone1-noticed         \ (bit 1) Hewie has reacted to zone 1 (once a visit)
2 constant area11-entered              \ (bit 2) she entered area $B (the figure's scene goes on)
3 constant turned-round                \ (bit 3) the figure turned round (cmd00)
4 constant found-her                   \ (bit 4) he has found her hiding: she must flee (the avoid prompt)
5 constant will-find-her               \ (bit 5) he will find her hiding (by chance, as he searches: more likely by the progress variable)
6 constant came-while-hidden           \ (bit 6) the stalker came in while she was hiding
7 constant searched-hiding             \ (bit 7) he has searched her hiding place
8 constant search-started              \ (bit 8) his search begun (phase 1 starts act07)
0 constant hops-var                    \ (var 0) the figure's hops (5: away)
1 constant turned-var                  \ (var 1) the figure turned (1)
2 constant turn-var                    \ (var 2) the figure's turn so far
3 constant still-var                   \ (var 3) the figure still (0) or hopping
4 constant call-frames-var             \ (var 4) frames to the figure's next call (60)
0 constant figure-hit1-sound           \ (snd 0) the figure as it strikes her / is struck
1 constant figure-hit2-sound           \ (snd 1) the figure as it strikes her / is struck
2 constant figure-call1-sound          \ (snd 2) the figure's call (now and then)
3 constant figure-call2-sound          \ (snd 3) the figure's call (now and then)
4 constant figure-call3-sound          \ (snd 4) the figure's call (now and then)

\ Room46_Cmd00
: old-mansion-1f-6.cmd00 ( b0 -- )   \ (Room46_CharHook, for the script's character ($21): b0 1 turned from pi by
    \ 0.1 for each of variable 2 (on by 2 a call), to 1.57 at least - event bit 3 there; 2 moved by
    \ its animation - the cast's own; 0 its move stopped. Then while $21 is right of x -84,
    \ variable 1 is 1)
    1 = if
        self-char dup char-here if
            dup char-at vec@  pi turn-var var @ s>f 0.1e f* f-
            fdup 1.57e f<= if  fdrop 1.57e  turned-round ebit-set  then  angle-wrap  place-char
        else  drop  then
        2  turn-var var @ turn-var +  var-set
    then
    $21 who>cs dup 0< if  drop exit  then  dup char-here 0= if  drop exit  then
    char-at sf@ -84e f> if  turned-var 1 var-set  then ;
\ Room46_Cond00
: old-mansion-1f-6.cond00? ( -- flag )   \ (Room46_PursuerBusy: about, and moving on his own (0 / 1) or in view)
    stalker-about? 0= if  false exit  then  stalker-move-mode 2 u< stalker-in-view? or ;

: old-mansion-1f-6.enter ( -- )   \ 00416A50
    room-sounds
    $1C $BB char-to-tri
    still-var 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 20.0 0.0 -105.0 0 char-to-xyz
        $1C $9006 1 0 char-anim-hold
    then
    0 1 track 0 action
    hunted state-flag-set
    om1f-6-medallion story-flag? not if
        0 -119.67 9.0 151.64 flicker-sprite
    then
    om3f-1-flag2 story-flag? not om1f-6-flag1 story-flag? not and if
        1 exit-taken? if
            4 char-start
            $21 $FB char-to-tri
            $21 char-activate
            $21 -97.0 0.0 84.0 180 char-to-xyz
            $21 0 1 0 char-anim-hold
            0 $21 2 action-force
        else
            om1f-6-flag1 story-flag-set
        then
    then
    1 -122.2 13.1 139.0 0 flame-in
    2 -120.5 12.8 140.35 0 flame-in
    1 $2300 sound-volume
;

: old-mansion-1f-6.act07 ( -- )   \ 00417050
    searched-hiding ebit-set
    came-while-hidden ebit-set
    will-find-her ebit-clear
    $22 0 pvar? if
        $A chance? if
            will-find-her ebit-set
        then
    else $22 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $22 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $22 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who 8 action
    else
        $78 1 summoner-wait
        search-started ebit-clear
    then
    0 stalker-alert? if
        stalker-who 2 stalker-command
    then
    $22 pvar-inc
    exit
;

: old-mansion-1f-6.char-enter ( -- )   \ 00416AF0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        3 -1 player-camera
    then
    1 3 -1 area-camera
    fiona-who self-is? if
        1 exit-taken? if
            2 map-page
        then
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            old-mansion-1f-6.act07
        else
            came-while-hidden ebit-clear
        then
    then
;

: old-mansion-1f-6.phase1 ( -- )   \ 00416B90
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 1 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    9 3 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who $E char-entered-area? if
        1 map-page
    then
    fiona-who $E char-left-area? if
        2 map-page
    then
    om3f-1-flag2 story-flag? not om1f-6-flag1 story-flag? not and if
        fiona-who $B char-entered-area? if
            area11-entered ebit-set
        then
    then
    fiona-who $A char-entered-area? if
        om1f-6-flag1 story-flag? not if
            4 0 char-remove
            $46 0 82 $80 8 -1 $B4 0.0 creature-place
            om1f-6-flag1 story-flag-set
        then
    then
    stalker-who 2 char-cond? not if
        fiona-hidden state-flag? searched-hiding ebit? not and stalker-who char-here? and if
            stalker-who char-busy? not if
                search-started ebit-set
                old-mansion-1f-6.act07
            then
        then
    then
    0 20.0 0.0 -105.0 5 10 1 zone
    self-who 0 char-in-zone? if
        in-zone0 ebit-set
    else
        in-zone0 ebit-clear
    then
    still-var 0 var? not if
        call-frames-var var-inc
        call-frames-var 60 var? if
            $21 chance? if
                $1C figure-call1-sound 6 char-sound
            else $32 chance? if
                $1C figure-call2-sound 6 char-sound
            else
                $1C figure-call3-sound 6 char-sound
            then then
            call-frames-var 0 var-set
        then
    then
    1 20.8 0.0 -104.26 $1E 15 0 zone
    hewie-zone1-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    hewie-zone1-noticed ebit-set
                    $50 chance? if
                        hewie-zone-var 1 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    1 20.8 0.0 -104.26 $1E 15 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 15.0 hewie-look-zone
            then
        then
    then
    3 1 8 -4 0 zone-at-effect
    fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    4 2 8 -4 0 zone-at-effect
    fiona-who 4 3 char-zone-bits? fiona-who 4 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 4 3 char-zone-bits? hewie-who 4 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 4 3 char-zone-bits? stalker-who 4 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
;

: old-mansion-1f-6.phase2 ( -- )   \ 00416D60
    om1f-6-medallion story-flag? not if
        2 0 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            1 calm offer-action
        then
    then
    fiona-who $F char-in-area? fiona-who -45 $3C char-heading? and if
        stalker-who char-here? not if
            5 calm offer-action
        else old-mansion-1f-6.cond00? not if
            5 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then then
    then
    fiona-who $C $3C char-faces-area? if
        3 unhurried offer-action
    then
    fiona-who $D char-in-area? fiona-who 45 $3C char-heading? and if
        4 unhurried offer-action
    then
;

: old-mansion-1f-6.phase3 ( -- )   \ 00416DB0
    4 camera-mode? not if
        -111.5 18.0 114.0 -94.0 18.0 93.0 -111.5 0.0 114.0 -94.0 0.0 93.0 lights-doorway
    then
;

: old-mansion-1f-6.act00 ( -- )   \ 00416DF0
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 20.0 0.0 -105.0 0 char-to-xyz
        $1C $9006 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        in-zone0 ebit? if
            still-var 0 var-set
            hops-var var-inc
            hops-var 3 var? if
                $21 chance? if
                    hops-var 5 var-set
                then
            then
            hops-var 4 var? if
                $32 chance? if
                    hops-var 5 var-set
                then
            then
            hops-var 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                stalker-who 0 stalker-command
                $1C figure-hit1-sound 6 char-sound
            else
                $1C figure-hit2-sound 6 char-sound
            then
            fiona-who shove-hit-sound 5 char-sound
            $1C $9007 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9006 0 3 char-anim-hold
            still-var 0 var-set
            hops-var 5 var? if
                $1C wait-char-anim
                hops-var 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9006 0 0 char-anim-hold
            still-var 0 var-set
        else
            still-var var-inc
        then
        1 wait-frames
    again
;

: old-mansion-1f-6.act01 ( -- )   \ 00416EB0
    self-wait-done
    -119.67 151.64 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-6-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-6.act02 ( -- )   \ 00416F10
    0 old-mansion-1f-6.cmd00
    [: area11-entered ebit? ;] wait-until
    $21 $200 1 4 char-anim-hold
    turned-var 0 var-set
    turn-var 0 var-set
    [: 1 old-mansion-1f-6.cmd00 turned-round ebit? ;] wait-until
    [: 2 old-mansion-1f-6.cmd00 turned-var 0 var? ;] wait-while
    self-idle-or-end
;

: old-mansion-1f-6.act03 ( -- )   \ 00416F50
    self-wait-done
    20.0 -120.0 self-turn-to-xz
    self-wait-done
    0 say   \ "There's a sheet draped over this... thing. Why are they trying to hide everything away..."
    self-idle-or-end
;

: old-mansion-1f-6.act04 ( -- )   \ 00416F60
    self-wait-done
    90 self-turn-angle
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    1 say   \ "It's an enormous cage. What in the world could they have been keeping in here?"
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-6.act06 ( -- )   \ 00417020
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A blend-out
    0 self-noclip
    found-her ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-6.act05 ( -- )   \ 00416F80
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    searched-hiding ebit-clear
    fiona-who 0 char-file-load
    self-wait-done
    self-who self-look-at
    yield
    fiona-who char-file-use
    $92 $8004 5 -110.16 146.09 -90 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who slide-sound 5 char-sound
    5 room-motion $A self-anim-blend8
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    found-her ebit-clear
    0 avoid-prompt
    self-who 4 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] old-mansion-1f-6.act06 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                came-while-hidden ebit? stalker-who char-here? not and if
                    came-while-hidden ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] old-mansion-1f-6.act06 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            5 wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-1f-6.act08 ( -- )   \ 004170B0
    self-wait-done
    search-started ebit? not if
        stalker-who 0 char-heading-for? 0 exit-door-open? not and if
            fiona-who 3 self-move-slot
            self-wait-done
        then
    else stalker-who 0 char-heading-for? 0 exit-door-open? not and stalker-who 0 char-in-area? and if
        fiona-who 3 self-move-slot
        self-wait-done
    then then
    search-started ebit-clear
    3 self-is? $22 self-is? or if
        stalker-who 1 char-file-load
    then
    $20 -100.0 146.09 -90 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
        4 stalker-who 1 char-model-op
        0 room-motion play-out
        4 stalker-who 0 char-model-op
    else
        there-anim play-out
    then
    found-her ebit-set
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-1f-6.phase5 ( -- )   \ 0047AC40
    $1C action-end
    $1C char-done
;

\ ---- registered ----
old-mansion-1f-6 register-scripts   \ (its phases and actions, by their names)
