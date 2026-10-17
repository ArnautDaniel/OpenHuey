\ story/rooms/house-of-truth-1f-9.fs - the event scripts of room house-of-truth-1f-9 ($92; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-9
USING: engine common actors messages story.state room-names story.words story.shared flag-names story-names ;

\ ---- the debris falling from above ("keri0".."keri5", each carried by a kind-9 thing: free
\ while hidden) and the traps set round the middle (kind 10) - the things actor's ----
: keri$ ( k -- addr len )
    case  0 of  s" keri0"  endof  1 of  s" keri1"  endof  2 of  s" keri2"  endof
          3 of  s" keri3"  endof  4 of  s" keri4"  endof  >r s" keri5" r>  endcase ;
: free-keri ( -- k | -1 )  6 0 do  i keri$ placed-shown? 0= if  i unloop exit  then  loop  -1 ;
fvariable dx  fvariable dz  fvariable dy
: drop-spot ( -- tri ) ( F: -- x y z )   \ (40..50 up, 25..40 out in any direction, over the floor)
    begin
        pi 2e pi f* rnd f* f-  ( F: ang )  40e 10e rnd f* f+ dy f!
        25e 15e rnd f* f+  fover fsin fover f* dx f!  fswap fcos f* dz f!
        dx f@ 0e dz f@ nav-tri  dup 0< while  drop
    repeat  dx f@ dy f@ dz f@ ;
: things-id ( -- id )  s" things" actor-named ;
\ room objects 4 / 5 spinning (+0x10) at a speed (+0x30) eased by script variable 4 / 5: byte 3
\ 0 sets them up (speed and base 4 degrees, top 20); else each frame (unless the progress' +0x54
\ says no): state 0 slows by 5% of the base to 0, 1 speeds by 2% up to the base, 2 by 10% up to
\ the top, 3 jumps to the base
: house-of-truth-1f-9.cmd00 ( b0 -- )   \ (the fans "fan0" / "fan1" turning (+0x10) at their speed (+0x30), by variables
    \ 4 / 5: 0 slowing by 5% of the base (+0x34) to a stop, 1 speeding up by 2% to the base, 2 by
    \ 10% to the top (+0x38), 3 at the base. b0 0 sets them up: speed and base 4 degrees, top 20)
    {: b0 | a n :}
    2 0 do
        i if  s" fan1"  else  s" fan0"  then  to n to a
        a n object-here? if
            b0 0= if
                4e deg>rad fdup a n $30 object!  a n $34 object!  20e deg>rad a n $38 object!
            else
                a n $30 object@  a n $34 object@                       ( F: speed base )
                4 i + var @ case
                    0 of  0.05e f* f-  0e fmax  endof
                    1 of  0.02e f* f+  a n $34 object@ fmin  endof
                    2 of  0.1e f* f+  a n $38 object@ fmin  endof
                    3 of  fswap fdrop  endof
                    >r fdrop r>
                endcase
                fdup a n $30 object!
                a n turn-x object@ f+  fdup pi f> if  2e pi f* f-  then  a n turn-x object!
            then
        then
    loop ;
\ room 0x92: three grey smoke effects (Effect79B00, size 50) at the room's spots 2, 7, 5
\ (grey_three).
: house-of-truth-1f-9.cmd01 ( -- )   \ (the ceiling coming down: stones of three kinds falling from 50)
    2 50e 0e 0e 0.5e stones-falling  7 50e 0e 0e 0.5e stones-falling  5 50e 0.5e 0e 0.5e stones-falling ;
\ up to 6 things out (script variable 6 counts them): 1..3 more placed things of kind 9 (+0x8),
\ each tied to the first room object 10..15 flagged (+0 = 1) (+0x122 its index), set up (+0xC,
\ +0x28 on) and dropped at a random spot 40..50 out and 25..40 up in any direction that lands on
\ the nav mesh (+0x3C)
: house-of-truth-1f-9.cmd02 ( -- )   \ (more debris falling: 1..3 more, 6 out at most - variable 6 counts them - each
    \ carrying the first "keri" free)
    6 var @ 6 >= if  exit  then
    3 random 1+  6 6 var @ - min  0 ?do
        free-keri dup 0< if  drop leave  then
        6 var-inc
        drop-spot  xyz>cells  things-id send keri-drop
    loop ;
\ the placed thing 10 brought back (list +0x14 / +0x8, its +0xC, +0x28 on) and put on one of
\ five spots round a circle (script variable 1, then on by 2 of 5): turned to the spot's angle
\ (with a little random), 2.1 up and 1.5..2 out on triangle 0x3B; then effect SmokeTrail_vtable
\ on it
: house-of-truth-1f-9.cmd03 ( -- )   \ (a trap set down on one of five spots round the middle - variable 1, then on by
    \ 2 of 5: 72 degrees a spot, with a little random - 1.5..2 out, 2.1 up, on triangle $3B, smoke
    \ rising from it)
    1.0053e rnd f*  1.2566e 1 var @ s>f f* f+  0.5027e f-  angle-wrap  ( F: a )
    1.5e 0.5e rnd f* f+  fover fsin fover f* dx f!  fswap fcos f* dz f!
    $3B  dx f@ f>cell  2.1e f>cell  dz f@ f>cell  things-id send trap-place
    1  1 var @ 2 + 5 mod  var-set ;
\ (as Room49_Cmd02) byte 3 0: the effect WispColumn_vtable spawned (told 1), its slot in event
\ var 0; 1: it is told 0
: house-of-truth-1f-9.cmd04 ( b0 -- )   \ (the column of wisps: b0 0 made - kept in variable 0; 1 fading out)
    0= if  0 effects:wisp-column var-set  else  0 var @ dup alive? if  0 swap send effect-command  else  drop  then  then ;
\ the things that fell below -30: placed things of kind 9 are reset (+0x28) and each counts down
\ script variable 6 (and the event manager's +0x5C); room objects 10..15 that did get +0 set
: house-of-truth-1f-9.cmd05 ( -- )   \ (the debris fallen through since: each counts variable 6 down, event bit 9)
    keri-fallen @ 0 ?do  6 var-dec  9 ebit-set  loop  0 keri-fallen ! ;
\ the 0x20E0-byte effect SparkSpray_vtable (sent byte 3): byte 3 0 / 1 one made, its slot in
\ script variable 2 / 3; 2 / 3 the one in variable 2 / 3 (if any) sent nothing
: house-of-truth-1f-9.cmd06 ( b0 -- )   \ (the sparks sprayed across: b0 0 / 1 from the left / right - kept in variable 2 / 3;
    \ 2 / 3 that one off)
    dup 2 < if  dup 2 +  swap effects:spark-spray  var-set  exit  then
    var @ dup alive? if  0 swap send effect-command  else  drop  then ;
\ (as slam_shake, both of Lorenzo's forms: 0xA and 0x27)
: house-of-truth-1f-9.cmd07 ( -- )  $A slam-landed?  $27 slam-landed? or if  0.5e camera-shake  then ;
\ room objects 8 / 9 raised (+0x24 down 0.2 a call to 0) while the event manager's +0x58 test 4
\ / 5 holds, else lowered back (up 0.4 a call to 0.7)
: house-of-truth-1f-9.cmd08 ( -- )   \ (the plates "fumi0" / "fumi1" up - +0x24 down 0.2 a call to 0 - while event
    \ bits 4 / 5 are set, else back down - up 0.4 a call to 0.7)
    {: | a n :}
    2 0 do
        i if  s" fumi1"  else  s" fumi0"  then  to n to a
        a n object-here? if
            4 i + ebit? if
                a n place-y object@ f0> if  -0.2e a n place-y object+!  a n place-y object@ 0e fmax a n place-y object!  then
            else
                a n place-y object@ 0.7e f< if  0.4e a n place-y object+!  a n place-y object@ 0.7e fmin a n place-y object!  then
            then
        then
    loop ;
\ a noise at (-70 or 70 by byte 3, 14, 0): byte 4 0 / 1 / 2 kind 1 / 2 / 4
: house-of-truth-1f-9.cmd09 ( b0 b1 -- )   \ (a blow of kind 2 from (-70 or 70 by b0, 14, 0) at b1 0 Fiona, 1 Hewie,
    \ 2 the stalker - Progress_Noise: kind 2, damage 5, threat 10)
    swap if  70e  else  -70e  then  14e 0e                                  ( who ) ( F: x y z )
    case
        0 of  2 xyz>cells 10e f>cell  fiona-id send fiona-request  endof
        1 of  2 5 0 xyz>cells  hewie-id send blow-from  endof
        2 of  2 5 0 xyz>cells  slot2 @ dup 0< if  drop 2drop 2drop 2drop  else  send blow-from  then  endof
        >r fdrop fdrop fdrop r>
    endcase ;
\ Room92_Cmd0A
: house-of-truth-1f-9.cmd0A ( -- )  things-id send story-things-off ;   \ (the debris and the traps all gone)
\ Room92_Cmd0B
: house-of-truth-1f-9.cmd0B ( -- )   \ (from the scene's frame 825: 1 / 1 on 0.3 a frame to 21 / 80 on 0.7 a frame to 120 / 160)
    cutscene-frame 825 - s>f  1e  fover 0.3e f* 1e f+ 21e fmin  frot 0.7e f* 80e f+ 120e fmin  160e  depth-range ;
\ Room92_Cmd0C
: house-of-truth-1f-9.cmd0C ( b0 b1 -- )   \ (the heat haze: b0 0 made - kept in variable 10; 1 its wave easing to b1, 2 at once)
    swap case
        0 of  drop  10 effects:heat-haze var-set  endof
        1 of  10 var @ dup alive? if  send effect-command  else  2drop  then  endof
        2 of  256 +  10 var @ dup alive? if  send effect-command  else  2drop  then  endof
        >r drop r>
    endcase ;
\ Room92_Cmd0D
: house-of-truth-1f-9.cmd0D ( b0 -- )   \ (the pit's embers: b0 0 with flames, 1 without - kept in variable 11; 2 off)
    dup 2 < if  11 swap 0= effects:pit-embers var-set  exit  then
    drop  11 var @ dup alive? if  0 swap send effect-command  else  drop  then ;

defer house-of-truth-1f-9.act0F
: house-of-truth-1f-9.enter ( -- )   \ 00436CF0
    room-sounds
    0 -36.4 13.4 54.4 1 flame-in
    1 58.7 10.0 -29.9 1 flame-in
    ht1f-9-flag1 story-flag? if
        0 1 $14 door-bits
        0 $F1 9 action
        4 3 var-set
        5 3 var-set
    then
    tremor-warned story-flag? if
        5 1.0 0 bgm
    then
    $80 exit-taken? not if
        shared.act95
        ht1f-9-flag2 story-flag? if
            house-of-truth-1f-9.cmd01
            0 0 house-of-truth-1f-9.cmd0C
            0 house-of-truth-1f-9.cmd0D
        then
    else
        $B ebit-set
    then
    1 0 $18000020 nav-group
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
;

: house-of-truth-1f-9.char-enter ( -- )   \ 00436D70
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
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $85 char-to-tri
            0 0 player-camera
            0 fiona-who $A action
            0.0 sound-volume-scale
        then
    then
;

: house-of-truth-1f-9.phase1 ( -- )   \ 00436DE0
    ht1f-9-flag1 story-flag? if
        8 ebit? not if
            6 sound-bank-loaded? if
                $40000000 6 0.0 50.0 0.0 0 0 sound
                $40000001 6 0.0 0.0 0.0 0 0 sound
                8 ebit-set
            then
        else
            $C0000000 6 0.0 50.0 0.0 0 0 sound
            $C0000001 6 0.0 0.0 0.0 0 0 sound
            $C0000003 6 0.0 0.0 0.0 0 0 sound
            $C0000002 6 -70.0 15.0 0.0 0 0 sound
            $C0000006 6 70.0 15.0 0.0 0 0 sound
        then
    then
    $B ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    found-file-255 story-flag? not if
        0 control-action? hewie-who char-here? and hewie-who 2 char-cond? not and stalker-who char-here? not and fiona-who 4 char-in-area? and fiona-who 0 65 $32 char-faces-xz? and if
            hewie-may-break-off? if
                0 fiona-who 3 action
            then
        then
    then
    ht1f-9-flag1 story-flag? ht1f-9-flag2 story-flag? not and if
        stalker-who 2 char-cond? if
            ht1f-9-flag2 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 5 action-force
            else
                1 fiona-who 5 action-force
            then
        then
    then
    ht1f-9-flag1 story-flag? ht1f-9-flag2 story-flag? not and if
        house-of-truth-1f-9.cmd05
        3 ebit? not if
            9 ebit? if
                3 ebit-set
                0 $F2 $B action
            then
        then
        4 ebit? not if
            fiona-who 5 char-in-area? hewie-who 5 char-in-area? or stalker-who 5 char-in-area? or if
                4 ebit-set
                0 $F3 $C action
            then
        else 6 ebit? if
            fiona-who 7 char-in-area? fiona-who 0 char-action? and if
                0 0 house-of-truth-1f-9.cmd09
            then
            hewie-who 7 char-in-area? hewie-who 0 char-action? and if
                0 1 house-of-truth-1f-9.cmd09
            then
            stalker-who 7 char-in-area? stalker-who 0 char-action? and if
                0 2 house-of-truth-1f-9.cmd09
            then
        then then
        5 ebit? not if
            fiona-who 6 char-in-area? hewie-who 6 char-in-area? or stalker-who 6 char-in-area? or if
                5 ebit-set
                0 $F4 $D action
            then
        else 7 ebit? if
            fiona-who 8 char-in-area? fiona-who 0 char-action? and if
                1 0 house-of-truth-1f-9.cmd09
            then
            hewie-who 8 char-in-area? hewie-who 0 char-action? and if
                1 1 house-of-truth-1f-9.cmd09
            then
            stalker-who 8 char-in-area? stalker-who 0 char-action? and if
                1 2 house-of-truth-1f-9.cmd09
            then
        then then
    else
        4 ebit? not if
            fiona-who 5 char-in-area? hewie-who 5 char-in-area? or stalker-who 5 char-in-area? or if
                5 6 -40.0 0.0 -40.0 0 0 sound
                4 ebit-set
            then
        else fiona-who 5 char-in-area? not hewie-who 5 char-in-area? not and stalker-who 5 char-in-area? not and if
            4 ebit-clear
        then then
        5 ebit? not if
            fiona-who 6 char-in-area? hewie-who 6 char-in-area? or stalker-who 6 char-in-area? or if
                5 6 44.0 0.0 37.0 0 0 sound
                5 ebit-set
            then
        else fiona-who 6 char-in-area? not hewie-who 6 char-in-area? not and stalker-who 6 char-in-area? not and if
            5 ebit-clear
        then then
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    found-file-255 story-flag? not if
        3 1.06 0.0 59.99 $1A 14 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 hewie-who $88 action
                        then
                    then
                then
            then
        then
    then
    4 0.21 0.0 0.03 $21 10 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
    house-of-truth-1f-9.cmd07
    house-of-truth-1f-9.cmd08
;

: house-of-truth-1f-9.phase2 ( -- )   \ 004370F0
    fiona-who 3 char-in-area? fiona-who 0 $32 char-heading? and if
        0 unhurried offer-action
    then
    scene-5-pending state-flag? if
        -2147483646 scene-request? if
            6 always offer-action
        then
    then
    ht1f-9-flag2 story-flag? if
        2 0.0 0.0 0.0 $17 4 0 zone
        fiona-who 2 3 char-zone-bits? fiona-who 0 0 $32 char-faces-xz? and if
            7 unhurried offer-action
        then
    then
;

: house-of-truth-1f-9.phase3 ( -- )   \ 00437140
    ht1f-9-flag2 story-flag? $B ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        $B ebit-set
    then
;

: house-of-truth-1f-9.act02 ( -- )   \ 00437320
    hewie-who self-look-at
    yield
    0 counter-set
    0 hewie-who 1 action-force
    1 wait-counter
    found-file-255 story-flag-set
    50 hewie-trust
    3 message   \ "Hewie? Have you picked up the scent boy?"
    wait-message
    self-who self-look-at
    yield
    $255 item-give
    $828C item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-9.act00 ( -- )   \ 00437160
    self-wait-done
    ht1f-9-flag2 story-flag? not if
        found-file-255 story-flag? not if
            hewie-who char-here? hewie-who 2 char-cond? not and stalker-who char-here? not and if
                stalkers-stay state-flag-set
                1 self-scripted
                self-frames-reset
                8 self-wait-frames
                fiona-who $31 0.0 58.0 0 char-to-xz
                1 15.0 10.0 0.0 5.0 event-camera
                world-frozen state-flag-set
                self-frames-reset
                8 self-wait-frames
                2 message   \ "I wonder if there used to be something inserted here before? There is this..."
                wait-message
                self-frames-reset
                8 self-wait-frames
                world-frozen state-flag-clear
                0 0.0 0.0 0.0 0.0 event-camera
                fiona-who $62 -10.0 54.0 140 char-to-xz
                ['] house-of-truth-1f-9.act02 goto
            else 0 ebit? not if
                self-frames-reset
                8 self-wait-frames
                fiona-who $31 0.0 58.0 0 char-to-xz
                1 15.0 10.0 0.0 5.0 event-camera
                world-frozen state-flag-set
                self-frames-reset
                8 self-wait-frames
                0 message   \ "A piece of a broken branch... Looks like something can be inserted here."
                wait-message
                self-frames-reset
                8 self-wait-frames
                world-frozen state-flag-clear
                0 0.0 0.0 0.0 0.0 event-camera
                0 ebit-set
            else
                1 message   \ "There is an odd smell here. I can't put my finger on it."
                wait-message
            then then
        else
            0.0 65.0 self-turn-to-xz
            self-wait-done
            self-frames-reset
            8 self-wait-frames
            fiona-who $31 0.0 58.0 0 char-to-xz
            1 15.0 10.0 0.0 5.0 event-camera
            world-frozen state-flag-set
            self-frames-reset
            8 self-wait-frames
            4 message   \ "So there was something inserted here before. Can you sniff it out for me?"
            wait-message
            self-frames-reset
            8 self-wait-frames
            world-frozen state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
    else 0 ebit? not if
        self-frames-reset
        8 self-wait-frames
        fiona-who $31 0.0 58.0 0 char-to-xz
        1 15.0 10.0 0.0 5.0 event-camera
        world-frozen state-flag-set
        self-frames-reset
        8 self-wait-frames
        5 message   \ "The whole building is moaning and groaning as if it were alive. This cane acted as..."
        wait-message
        self-frames-reset
        8 self-wait-frames
        world-frozen state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
        0 ebit-set
    else
        0.0 65.0 self-turn-to-xz
        self-wait-done
        7 message   \ "Something just doesn't seem right here. I'd better get out of here quick."
        wait-message
    then then
    self-idle-or-end
;

: house-of-truth-1f-9.act01 ( -- )   \ 004372F0
    1 self-scripted
    self-wait-done
    $31 3.0 55.0 -10 $204 5 self-move-to
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    fiona-who self-look-at
    yield
    hewie-bark
    self-wait-done
    counter-inc
    self-who self-look-at
    yield
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-9.act03 ( -- )   \ 00437350
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C04 self-anim
    fiona-who 1 char-wait-motion
    fiona-who $2E 5 char-sound
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    ['] house-of-truth-1f-9.act02 goto
;

: house-of-truth-1f-9.act04 ( -- )   \ 00437370
    summoner-on state-flag-clear
    stalker-who action-end
    stalker-who char-done
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    $F $44 fade
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    0 1 $14 door-bits
    $26 item-use
    0 creatures-clear
    $A ebit-clear
    $10 $C8 movie-param
    0 $F9 8 action
    $FF 1.0 0 bgm
    panic-eased
    9 message-prepare   \ "Your effort is for naught. How long do you plan to keep this up? Let's finish t..."
    1 result? if
        scene-play
    then
    wait-fade
    $F9 action-end
    $1F $7B $32 $2A $64 $24 $2C $36 $4E 0 9 effect-string
    $8080805E 0 1 screen-blend
    0 self-idle-anim
    fiona-who $7E 0.023 24.438 174 char-to-xz
    stalker-who action-end
    stalker-who char-done
    2 0 char-remove
    $27 partner-load
    2 char-start
    stalker-who char-activate
    $FE $92 133 2 stalker-to-room
    stalker-who $85 -0.009 -44.982 30 char-to-xz
    stalker-who 0 0 char-camera
    $92 0 151 hewie-to-room
    hewie-who $97 180 char-to-tri-facing
    4 0 object-hidden
    5 0 object-hidden
    0 $F1 9 action
    4 3 var-set
    5 3 var-set
    $ED door-open-clear
    doors-room-in
    $ED door-lock
    ht1f-9-flag1 story-flag-set
    scene-5-pending state-flag-set
    4 ebit-clear
    5 ebit-clear
    7 20 var-set
    0 $F5 $F action
    $A ebit? not if
        0 0 house-of-truth-1f-9.cmd0C
    then
    2 2 house-of-truth-1f-9.cmd0C
    1 house-of-truth-1f-9.cmd0D
    7 1.0 0 bgm
    $F 1 fade
    wait-fade
    $4D resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-9.act05 ( -- )   \ 004374E0
    $FF 1.0 0 bgm
    $F2 action-end
    $F3 action-end
    $F4 action-end
    $F5 action-end
    4 ebit-clear
    5 ebit-clear
    6 ebit-clear
    7 ebit-clear
    3 ebit-clear
    9 ebit-clear
    stalkers-stay state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    house-of-truth-1f-9.cmd0A
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    4 3 var-set
    5 3 var-set
    2 house-of-truth-1f-9.cmd0D
    $10 $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $92 0 151 hewie-to-room
    hewie-who $97 180 char-to-tri-facing
    50 hewie-trust
    0 self-idle-anim
    fiona-who $2B 54.575 -2.153 -90 char-to-xz
    camera-restart
    scene-5-pending state-flag-clear
    stalker-who action-end
    stalker-who char-done
    2 0 char-remove
    $C partner-load
    2 char-start
    $ED door-unlock
    4 0 object-hidden
    5 0 object-hidden
    $E6 door-lock
    $E7 door-lock
    $EB door-open-clear
    $EB door-lock
    house-of-truth-1f-9.cmd01
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    0 house-of-truth-1f-9.cmd0D
    $F $51 fade
    wait-fade
    $4E resident-flag-set
    $256 item-give
    $828D item-give
    shared.act9C
    stalkers-stay state-flag-clear
    0 self-scripted
    shared.act95
    self-idle-or-end
;

: house-of-truth-1f-9.act06 ( -- )   \ 0047AE6C
    8 message   \ "Oh no! It's locked!"
    self-idle-or-end
;

: house-of-truth-1f-9.act07 ( -- )   \ 00437630
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 ebit? not over-music-f story-flag? not and if
        $A01 self-anim
        self-frames-reset
        $1E self-wait-frames
        6 message   \ "If I fell in from here, I would be toast... Quite literally."
        wait-message
        self-wait-anim
        1 ebit-set
    else
        7 message   \ "Something just doesn't seem right here. I'd better get out of here quick."
        wait-message
    then
    self-idle-or-end
;

: house-of-truth-1f-9.act08 ( -- )   \ 00437660
    depth-range-off
    begin
        $21C cutscene-cue-reached? not while
        yield
    repeat
    begin
        $2ED cutscene-cue-reached? not while
        1.0 1.0 80.0 160.0 depth-range
        yield
    repeat
    begin
        $348 cutscene-cue-reached? not while
        house-of-truth-1f-9.cmd0B
        yield
    repeat
    depth-range-off
    begin
        4 cutscene-shot? not while
        yield
    repeat
    depth-range-off
    $ED door-open-clear
    doors-room-in
    $1F $72 $96 $94 $6E $60 $56 $70 $3C 0 9 effect-string
    $9EEAEAFF 0 1 screen-blend
    begin
        5 cutscene-shot? not while
        yield
    repeat
    $1F $7B $32 $2A $64 $24 $2C $36 $4E 0 9 effect-string
    $8080805E 0 1 screen-blend
    begin
        $47E cutscene-cue-reached? not while
        yield
    repeat
    0 0 house-of-truth-1f-9.cmd0C
    $A ebit-set
    2 6 house-of-truth-1f-9.cmd0C
    begin
        $492 cutscene-cue-reached? not while
        yield
    repeat
    1 2 house-of-truth-1f-9.cmd0C
    begin
        8 cutscene-shot? not while
        yield
    repeat
    1 0 house-of-truth-1f-9.cmd0C
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    self-idle-or-end
;

: house-of-truth-1f-9.act09 ( -- )   \ 00437700
    0 house-of-truth-1f-9.cmd00
    begin
        1 house-of-truth-1f-9.cmd00
        yield
    again
;

: house-of-truth-1f-9.act0A ( -- )   \ 00437710
    1 self-scripted
    hewie-who char-here? if
        $91 0 -1 hewie-to-room
    then
    self-wait-done
    self-who 1 char-visible
    house-of-truth-1f-9.cmd01
    0 0 house-of-truth-1f-9.cmd0C
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    world-held state-flag-set
    stalker-who action-end
    stalker-who char-done
    self-who 0 char-visible
    1 house-of-truth-1f-9.cmd04
    $4F resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-1f-9.act0B ( -- )   \ 004377C0
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        0.3 camera-shake
        yield
    repeat
    0 house-of-truth-1f-9.cmd04
    $40000003 6 0.0 0.0 0.0 0 0 sound
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    house-of-truth-1f-9.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    house-of-truth-1f-9.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    house-of-truth-1f-9.cmd03
    9 30 var-set
    begin
        9 var-dec
        9 0 var? not while
        1.0 camera-shake
        yield
    repeat
    1 house-of-truth-1f-9.cmd04
    9 ebit-clear
    3 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-9.act0C ( -- )   \ 00437870
    $40000002 6 -70.0 15.0 0.0 0 0 sound
    4 2 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    0 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    6 ebit-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    4 0 var-set
    2 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    6 ebit-clear
    self-frames-reset
    $384 self-wait-frames
    4 1 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    4 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-9.act0D ( -- )   \ 004378D0
    $40000006 6 70.0 15.0 0.0 0 0 sound
    5 2 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    1 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    7 ebit-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    5 0 var-set
    3 house-of-truth-1f-9.cmd06
    self-frames-reset
    $F self-wait-frames
    7 ebit-clear
    self-frames-reset
    $384 self-wait-frames
    5 1 var-set
    self-frames-reset
    $1E self-wait-frames
    self-frames-reset
    $1E self-wait-frames
    5 ebit-clear
    self-idle-or-end
;

: house-of-truth-1f-9.act0E ( -- )   \ 0047AE70
    self-idle-or-end
;

:noname   \ house-of-truth-1f-9.act0F (00437930; deferred: used before it is defined)
    7 0 var? 7 1 var? or 7 2 var? or if
        8 10 var-set
        begin
            0.5 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        house-of-truth-1f-9.cmd02
        8 10 var-set
        begin
            1.0 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        8 10 var-set
        begin
            0.5 camera-shake
            8 var-dec
            8 0 var? not while
            yield
        repeat
        7 150 var-set
    then
    self-frames-reset
    $1E self-wait-frames
    7 var-dec
    7 var-dec
    $32 chance? if
        7 var-dec
    then
    ['] house-of-truth-1f-9.act0F goto
; is house-of-truth-1f-9.act0F

: house-of-truth-1f-9.act10 ( -- )   \ 004379B0
    self-wait-done
    0 -36.4 13.4 54.4 1 flame-in
    1 58.7 10.0 -29.9 1 flame-in
    $A 3 $FF char-load
    3 char-start
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 1 $14 door-bits
    $10 $C8 movie-param
    0 $F9 8 action
    panic-eased
    9 message-prepare   \ "Your effort is for naught. How long do you plan to keep this up? Let's finish t..."
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

: house-of-truth-1f-9.act11 ( -- )   \ 00437A70
    self-wait-done
    $A 3 $FF char-load
    3 char-start
    0 0 house-of-truth-1f-9.cmd0C
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 $C8 movie-param
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

: house-of-truth-1f-9.act12 ( -- )   \ 00437B20
    self-wait-done
    0 -36.4 13.4 54.4 1 flame-in
    1 58.7 10.0 -29.9 1 flame-in
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    self-who 1 char-visible
    house-of-truth-1f-9.cmd01
    0 0 house-of-truth-1f-9.cmd0C
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-9 register-scripts   \ (its phases and actions, by their names)
