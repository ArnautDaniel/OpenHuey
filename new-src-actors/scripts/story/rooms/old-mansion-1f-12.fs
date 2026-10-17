\ story/rooms/old-mansion-1f-12.fs - the event scripts of room old-mansion-1f-12 ($4F; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-12
USING: room-names story.words story.shared flag-names story-names floor-flags ;

\ room 0x4F (Room4F_Cmd00_ptmf): object byte 3 by byte 4: 0 up (+0x10 0), 1 down (-0.65), 2
\ lowered a step-floor (0.02, not during a movie); once down, events bit 3
: old-mansion-1f-12.cmd00 ( b0 b1 -- )   \ (object b0 - the room's string - tipped by b1: 0 up, 1 down (-0.65), 2 lowered a step-floor
    \ (0.02; not while a movie plays), down - event flag 3)
    {: k op | a n :}  k room-object to n to a  a n object-here? 0= if  exit  then
    op case
        0 of  0e a n turn-x object!  endof
        1 of  -0.65e a n turn-x object!  endof
        2 of  movie-playing? 0= if
                  -0.02e a n turn-x object+!
                  a n turn-x object@ -0.65e f<= if  -0.65e a n turn-x object!  3 ebit-set  then
              then  endof
    endcase ;
\ room 0x4F (Room4F_Cmd01_ptmf): an effect (DustShaft_vtable, 0x7460 bytes), not started
: old-mansion-1f-12.cmd01 ( -- )  effects:dust-shaft ;   \ (dust drifting down the shaft of light)
\ byte 3 0: a SpiralSmoke_vtable effect (0x36C0 bytes) spawned, its slot kept in event var 0;
\ else that slot's effect removed
: old-mansion-1f-12.cmd02 ( b0 -- )  0 ['] effects:spiral-smoke effect-in-var ;   \ (the smoke spiralling up the fire: b0 0 lit, else out)
\ lower the object (pstr_sikakebox)'s +0x14 by 0.025 a frame down to -0.78, then event 6 (+0x5C)
: old-mansion-1f-12.cmd03 ( -- )   \ (the box, "sikakebox", lowered 0.025 a frame to -0.78; there event flag 6)
    s" sikakebox" object-here? 0= if  exit  then
    -0.025e s" sikakebox" turn-y object+!
    s" sikakebox" turn-y object@ -0.78e f<= if  -0.78e s" sikakebox" turn-y object!  6 ebit-set  then ;
\ Room4F_Cond00
: old-mansion-1f-12.cond00? ( -- flag )  -35.7e -7.5e 4e put-away-near ;   \ (Room4F_CreatureVanish: one within 4 put away)

: old-mansion-1f-12.enter ( -- )   \ 0040B520
    room-sounds
    om1f-12-flag3 story-flag? if
        0 1 old-mansion-1f-12.cmd00
    then
    om1f-12-flag1 story-flag? if
        1 1 old-mansion-1f-12.cmd00
    then
    om1f-12-flag2 story-flag? if
        2 1 old-mansion-1f-12.cmd00
    then
    om1f-12-flag4 story-flag? if
        0 part-shown
        3 part-shown
        8 1 object-hidden
    else
        0 part-hidden
        3 part-hidden
        8 0 object-hidden
    then
    got-film-reel story-flag? if
        4 part-hidden
    else
        4 part-shown
    then
    om1f-12-flag1 story-flag? if
        5 part-hidden
        6 part-shown
        1 part-shown
        old-mansion-1f-12.cmd01
    else
        5 part-shown
        6 part-hidden
        2 part-shown
        0 old-mansion-1f-12.cmd02
    then
    got-torn-letter story-flag? not om1f-12-flag2 story-flag? not and if
        0 12.0 13.0 -24.0 flicker-sprite
    then
    om1f-12-rubrum-medallion-found story-flag? om1f-12-rubrum-medallion story-flag? not and if
        1 52.04 1.0 5.68 flicker-sprite
    then
;

: old-mansion-1f-12.char-enter ( -- )   \ 0040B5C0
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
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
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
    fiona-who self-is? if
        2 exit-taken? if
            2 map-page
        then
    then
;

: old-mansion-1f-12.phase1 ( -- )   \ 0040B680
    om1f-12-flag1 story-flag? not if
        7 ebit? not if
            6 sound-bank-loaded? if
                $40000004 6 38.0 5.0 -10.0 0 0 sound
                7 ebit-set
            then
        else
            $C0000004 6 38.0 5.0 -10.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
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
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    4 ebit? not if
        om1f-12-flag4 story-flag? not if
            0 camera-mode? 1 camera-mode? or if
                old-mansion-1f-12.cond00? if
                    4 ebit-set
                    om1f-12-flag4 story-flag-set
                    0 $F1 $C action
                then
            then
        then
    then
    got-torn-letter story-flag? not om1f-12-flag2 story-flag? not and if
        0 0 4 4 0 zone-at-effect
    then
    om1f-12-flag1 story-flag? not if
        1 0.32 0.0 -28.08 $19 31 0 zone
        5 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 hewie-who $92 action
                        then
                    then
                then
            then
        then
    then
    2 -12.29 0.0 -26.2 $18 15 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    1 0.32 0.0 -28.08 $19 31 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 31.0 hewie-look-zone
            then
        then
    then
    3 12.19 0.0 -23.96 $18 15 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    4 38.13 0.0 -9.78 $1E 30 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 30.0 hewie-look-zone
            then
        then
    then
    5 -35.94 0.0 -7.54 $10 11 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    om1f-12-rubrum-medallion-found story-flag? not if
        6 52.04 0.0 5.68 $A 5 0 zone
        hewie-who 6 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-12-rubrum-medallion-found var-set
                $1A om1f-12-rubrum-medallion var-set
                $1B 1 var-set
                $1C 52040 var-set
                $1D 1000 var-set
                $1E 5680 var-set
                $1F 6 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: old-mansion-1f-12.phase2 ( -- )   \ 0040B8A0
    -2147483646 scene-request? if
        0 always offer-action
    then
    fiona-who 5 char-in-area? fiona-who 90 $32 char-heading? and if
        om1f-12-flag4 story-flag? got-film-reel story-flag? not and if
            2 calm offer-action
        else
            2 unhurried offer-action
        then
    then
    fiona-who 6 $32 char-faces-area? if
        3 unhurried offer-action
    then
    fiona-who 7 char-in-area? fiona-who 0 $32 char-heading? and if
        4 unhurried offer-action
    then
    got-torn-letter story-flag? not om1f-12-flag2 story-flag? not and if
        fiona-who 0 2 char-zone-bits? if
            $B calm offer-action
        then
    else fiona-who 8 char-in-area? fiona-who 90 $32 char-heading? and if
        9 unhurried offer-action
    then then
    fiona-who 9 char-in-area? fiona-who 90 $32 char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $F char-in-area? fiona-who -45 $3C char-heading? and if
        $E unhurried offer-action
    then
    om1f-12-rubrum-medallion-found story-flag? om1f-12-rubrum-medallion story-flag? not and if
        6 1 5 5 0 zone-at-effect
        fiona-who 6 3 char-zone-bits? if
            $D calm offer-action
        then
    then
;

: old-mansion-1f-12.act00 ( -- )   \ 0040B940
    self-wait-done
    1 ebit? not 2 game-mode? or in-panic? or if
        0 self-through-exit
        self-wait-done
        in-panic? 2 game-mode? or if
            $60B self-anim
            self-wait-anim
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
        else
            $609 self-anim
            self-wait-anim
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
            8 message   \ "The door is locked..."
            wait-message
        then
        1 ebit-set
    else
        self-frames-reset
        4 self-wait-frames
        fiona-who $150 -60.0 15.0 -90 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        self-frames-reset
        4 self-wait-frames
        9 message   \ "A strange symbol is written on the door. I wonder what it could mean?"
        wait-message
        self-frames-reset
        4 self-wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        fiona-who $150 -60.3 16.25 -90 char-to-xz
        world-frozen state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-12.act01 ( -- )   \ 0040B9D0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $609 self-anim
    self-frames-reset
    $14 self-wait-frames
    fiona-who 3 6 char-sound
    $F message-param-room
    $F item-use
    $6C door-unlock
    20 self-idle-anim
    self-frames-reset
    $14 self-wait-frames
    $8019 message   \ "Used ."
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act02 ( -- )   \ 0040BA10
    self-wait-done
    om1f-12-flag4 story-flag? got-film-reel story-flag? not and if
        $14E -33.036 -2.992 -130 $FFFF 5 self-move-to
        self-wait-done
        $900 self-anim
        self-wait-anim
        4 part-hidden
        got-film-reel story-flag-set
        $E message-param-room
        $E 1 item-give-count
        0 $E item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        $901 self-anim
        self-wait-anim
    else
        world-frozen state-flag-set
        1 self-scripted
        fiona-who $14E -38.0 -2.5 180 char-to-xz
        1 10.0 -10.0 0.0 0.0 event-camera
        self-frames-reset
        4 self-wait-frames
        om1f-12-flag4 story-flag? not if
            $B message   \ "A statue grasping firmly onto an oil lamp. I know I've seen this somewhere bef..."
            wait-message
        else
            $C message   \ "I knew there was some trick to solving it!"
            wait-message
        then
        0 0.0 0.0 0.0 0.0 event-camera
        world-frozen state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-12.act03 ( -- )   \ 0040BAC0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    om1f-12-flag1 story-flag? not if
        38.0 -10.0 self-turn-to-xz
        self-wait-done
        $402 self-anim
        self-wait-anim
        5 message   \ "A pillar of seething flame is spewing forth from the floor. There is something up a..."
        wait-message
    else
        6 message   \ "The flame has been frozen in place, creating a crystalline ice pillar. This could b..."
        wait-message
        0 answer? if
            $F 4 fade
            wait-fade
            fiona-who 5 6 char-sound
            fiona-who $10C 180 char-to-tri-facing
            0 0 player-camera
            camera-restart
            2 exit-prepare
            self-frames-reset
            $5A self-wait-frames
            2 map-page
            $F 1 fade
            wait-fade
        then
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act04 ( -- )   \ 0040BB20
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A message   \ "I can use this to get down. Climb down the ice pillar? YES NO"
    wait-message
    0 answer? if
        $F 4 fade
        wait-fade
        fiona-who 5 6 char-sound
        fiona-who $141 0 char-to-tri-facing
        1 1 player-camera
        camera-restart
        1 exit-prepare
        self-frames-reset
        $5A self-wait-frames
        1 map-page
        $F 1 fade
        wait-fade
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act05 ( -- )   \ 0040BB70
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    4 1 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    2 part-hidden
    1 old-mansion-1f-12.cmd02
    0 ebit? if
        0 $F9 8 action
    then
    $10 $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    world-held state-flag-set
    0 self-idle-anim
    fiona-who $11E -5.7 17.0 180 char-to-xz
    hewie-who $A3 -22.21 -11.73 -45 char-to-xz
    camera-restart
    0 ebit? if
        1 1 old-mansion-1f-12.cmd00
        5 part-hidden
        6 1 movie-play
        5 cutscene-start
        yield
        yield
        scene-wait-ready
        wait-fade
        hewie-who action-end
        hewie-who char-done
        $10 $C8 movie-param
        panic-eased
        $FFFF message-prepare
        1 result? if
            scene-play
        then
        wait-fade
        hewie-who char-activate
        $4F 0 163 hewie-to-room
        hewie-who $A3 -22.21 -11.73 -45 char-to-xz
        50 hewie-trust
        1 part-shown
        6 part-shown
        old-mansion-1f-12.cmd01
        4 6 sound-stop
        om1f-12-flag1 story-flag-set
    else
        2 part-shown
        5 part-shown
        0 old-mansion-1f-12.cmd02
    then
    $F $51 fade
    wait-fade
    0 ebit? not if
        self-frames-reset
        self-wait-16
        3 message   \ "Nothing happened. This must not be the correct plate..."
        wait-message
    then
    $827D item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act06 ( -- )   \ 0040BD10
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    12.0 -24.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    2 6 12.0 10.0 -25.0 0 0 sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    0 ebit? if
        0 effect-remove
        3 ebit-clear
        1 6 12.0 10.0 -25.0 0 0 sound
        begin
            2 2 old-mansion-1f-12.cmd00
            3 ebit? not while
            yield
        repeat
        om1f-12-flag2 story-flag-set
    else
        self-frames-reset
        $1E self-wait-frames
        3 message   \ "Nothing happened. This must not be the correct plate..."
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act07 ( -- )   \ 0040BD80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -12.0 -24.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    2 6 -12.0 10.0 -25.0 0 0 sound
    self-frames-reset
    self-wait-16
    $903 self-anim
    self-wait-anim
    0 ebit? if
        3 ebit-clear
        1 6 -12.0 10.0 -25.0 0 0 sound
        begin
            0 2 old-mansion-1f-12.cmd00
            3 ebit? not while
            yield
        repeat
        om1f-12-flag3 story-flag-set
    else
        self-frames-reset
        $1E self-wait-frames
        3 message   \ "Nothing happened. This must not be the correct plate..."
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-12.act08 ( -- )   \ 0040BDF0
    begin
        $44C cutscene-cue-reached? not while
        yield
    repeat
    3 ebit-clear
    1 6 0.0 30.0 -27.0 0 0 sound
    begin
        1 2 old-mansion-1f-12.cmd00
        3 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-1f-12.act09 ( -- )   \ 0040BE20
    self-wait-done
    om1f-12-flag1 story-flag? not if
        om1f-12-flag3 story-flag? not om1f-12-flag2 story-flag? not or if
            0 message   \ "A 3-headed monster. Is this some kind of dragon? Its wide-open mouth is very un..."
            wait-message
        else
            1 message   \ "The highest dragon head's mouth is still open. I'll need to insert one more plate."
            wait-message
        then
    else
        2 message   \ "All 3 mouths are now tightly shut. Looks like I've completely solved this puzzle..."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-12.act0A ( -- )   \ 0040BE40
    self-wait-done
    0.0 -27.0 self-turn-to-xz
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    4 message   \ "Darn it! I'm just not tall enough. If only Hewie were here, I bet it would be doable."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-12.act0B ( -- )   \ 0040BE60
    self-wait-done
    12.0 -24.0 self-turn-to-xz
    self-wait-done
    $D message   \ "It's holding something in its mouth."
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-torn-letter story-flag-set
    0 effect-remove
    $A3 message-param-room
    $A3 1 item-give-count
    0 $A3 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-12.act0C ( -- )   \ 0040BEB0
    self-frames-reset
    self-wait-16
    0 part-shown
    0 6 -38.0 5.0 -8.0 0 0 sound
    6 ebit-clear
    begin
        old-mansion-1f-12.cmd03
        6 ebit? not while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-1f-12.act0D ( -- )   \ 0040BEE0
    self-wait-done
    52.04 5.68 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-12-rubrum-medallion story-flag-set
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

: old-mansion-1f-12.act0E ( -- )   \ 0040BF40
    self-wait-done
    -63.0 -10.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $E message   \ "Ivy has grown in through the collapsed area where the floor used to be... Looks like i..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-12.act0F ( -- )   \ 0040BF60
    room-sounds
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    self-wait-done
    0 1 old-mansion-1f-12.cmd00
    2 1 old-mansion-1f-12.cmd00
    5 part-shown
    4 1 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 8 action
    $10 $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-1f-12.act10 ( -- )   \ 0040C000
    0 1 old-mansion-1f-12.cmd00
    1 1 old-mansion-1f-12.cmd00
    2 1 old-mansion-1f-12.cmd00
    6 1 movie-play
    5 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-12 register-scripts   \ (its phases and actions, by their names)
