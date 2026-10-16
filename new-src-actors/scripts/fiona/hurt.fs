\ fiona/hurt.fs - Fiona struck or caught (F4; src/game/fiona.c Fiona_Reaction, Fiona_React and
\ the states after: Fiona_StateKnockedDown, Fiona_StateThrown, Fiona_StateCaught). A `hit` from
\ a stalker: what it makes of her by what she is doing - knocked down (a blow; on a step or in a
\ doorway the short falls), thrown (a hard blow: forwards or backwards by where it came from),
\ caught (a hold: she flinches in his grip and pulls free) - the fright it carries, and getting
\ up again. How she stands for an attack (`fiona-plight`) is told each frame.
IN: fiona.hurt
USING: engine game-state actors messages common facts flag-names rumble fiona.state fiona.model fiona.moving fiona.fear ;

defer to-idle ( -- )
defer grabbed ( -- )   \ (seized outright: fiona.seized)
defer held-down ( -- )   \ (held by homunculi: fiona.seized)
defer knocked-off ( -- )   \ (struck on a ladder: fiona.ladder)
defer drop-door ( -- )   ' noop is drop-door   \ (a door she held let go: fiona.doors)
: danger-id ( -- id )  s" danger" actor-named ;

\ ---- how she stands for a stalker's attack (Pursuer_FionaState): out of reach while struck
\ (her reactions, mode 4) or seized; the panic's stages 4 / 5; fleeing (mode $B); panicking
\ (fear over 90, or the panic attack) ----
: plight ( -- n )
    her-mode @ 4 =  her-sub @ $10 = or  stalkers-blind state-flag? or  her-aside @ or if  3 exit  then
    her-panic-stage @ dup 4 = swap 5 = or if  her-panic-stage @ exit  then
    her-mode @ $B = if  1 exit  then
    her-fear f@ 90e f>  her-doing @ $E = or if  2 exit  then
    0 ;

\ ---- the reaction to a blow of `kind` (Fiona_Reaction): none while one is under way (mode 4,
\ floored $A) or the world is held; on a ladder (mode 3) her fall from it comes with the
\ ladders (F5) ----
\ (the short falls: her floor a step or a doorway's, or she is at a door of the room - within 20
\ of where it stands - or in a ladder's region: Progress_WhoIsWhere's door and link bits)
: at-a-door? ( -- flag )
    8 0 do  i door-here? if  i her-pos door-near? if  true unloop exit  then  then  loop  false ;
: by-a-ladder? ( -- flag )
    nav-links 0 ?do  her-at i nav-link-at? if  true unloop exit  then  loop  false ;
: on-step? ( -- flag )
    her-tri @ dup 0< 0= if  nav-flags $80003 and if  true exit  then  else  drop  then
    at-a-door?  by-a-ladder? or ;
: mode-in? ( m1 m2 -- flag )  her-mode @ tuck = >r = r> or ;   \ (her mode is one of the two)
: on-rungs? ( -- flag )  her-mode @ 3 =  her-sub @ 7 = and ;    \ (on the ladder itself: knocked off)
: charmed? ( -- flag )  progress pr.equipped 4 + sl@ $88 = ;   \ (the charm, item $88, in slot 1)
\ Fiona_Reaction: her reaction to a request by its kind (-1 none)
: reaction ( kind -- r | -1 )
    world-held state-flag? if  drop -1 exit  then
    case
        $D of  4 $A mode-in? her-mode @ 3 = or if  -1  else  $13  then  endof   \ (a spirit)
        1 of  4 $A mode-in? if  -1  else on-rungs? if  8  else on-step? if  $C  else  $E  then then then  endof
        2 of  4 $A mode-in? if  -1  else on-rungs? if  8  else on-step? if  $D  else  $F  then then then  endof
        4 of  4 $A mode-in? if  -1  else on-rungs? if  8  else on-step? if  $D  else  $A  then then then  endof
        3 of  her-mode @ 4 = her-sub @ $10 = and if  $FF 16 fiona-jolt  -1  else  $10  then  endof   \ (seized: again, only felt)
        5 of  her-mode @ 4 = her-sub @ $B = and  her-hit-how @ her-door-hit @ = and if  -1  else  $B  then  endof   \ (not again by the same door)
        6 of  4 $A mode-in?  her-mode @ 0= her-doing @ $F = and  or if  -1   \ (not while out of breath)
              else charmed? if  $20  else on-rungs? if  8  else  $20  then then then  endof
        \ ($A, reaction 9 - led away by the hand or walking: the stalker's `seize`, fiona/seized.fs)
        $C of  her-mode @ 4 = if  her-sub @ $12 = if  $12  else  -1  then   \ (homunculi: in a reaction only theirs again)
               else  3 $A mode-in? if  -1  else  $12  then  then  endof
        >r -1 r>
    endcase ;

\ ---- the way the blow sends her: from whoever struck to her (nobody: from where it was asked
\ from - her request's point), turned from her heading ----
fvariable side
create from-pt 12 allot
: striker? ( -- flag )  her-hit-who @ dup 0< if  drop false exit  then  dup alive? if  body?  else  drop false  then ;
: blow-side ( -- )
    striker? if  her-hit-who @ body-at her-at vec-heading  else  from-pt her-at vec-heading  then
    her-yaw f@ f- angle-wrap side f! ;
\ a fall by side: sent along her heading - struck from behind - (within 60 degrees) `front`, sent
\ back - struck from in front - (past 120) +1, to her left +3, her right +2
: fall-by-side ( front -- )
    side f@ fabs 60e deg>rad f< if  -1 play-table exit  then
    side f@ fabs 120e deg>rad f> if  1+ -1 play-table exit  then
    side f@ f0< if  3 +  else  2 +  then  -1 play-table ;

\ ---- knocked down (Fiona_StateKnockedDown): her cry, the fall, then up - a stumble's while
\ before she may fall again ----
: up-again ( -- )  ended? if  30 her-stumble-t !  to-idle exit  then  root-move ;
: knocked ( -- )
    settled? if
        blow-side
        her-sub @ case
            $C of  $3E 0 her-sound  $100E -1 play-table  endof
            $E of  $3E 0 her-sound  $1000 fall-by-side  endof
            $D of  $3F 0 her-sound  $100F -1 play-table  endof
            $F of  $3F 0 her-sound  $1004 fall-by-side  endof
        endcase
        $5F noise-here  ['] up-again her-act !
    then
    root-move ;

\ ---- thrown (Fiona_StateThrown): away from him - her back to him, forwards ($1008); facing
\ him, backwards ($100B; panicking she is flung down $B04, turned to him). Moved by the motion
\ turned to that way, turning there 20 degrees a frame (pull_back_move); then up ($100A / $100D), or
\ panicking she stays down and gets up as from a panic fall ----
: pull-back ( -- )
    her-fall-turn @ -1 = if  root-move exit  then
    root@ rm-x f@ rm-z f@ her-fall-yaw f@ rotate-by move-by
    her-fall-turn @ 0= if
        her-yaw f@ her-fall-yaw f@ 20e deg>rad turn-toward f0= if  1 her-fall-turn !  then  her-yaw f!
    then ;
: got-up ( -- )  ended? if  to-idle exit  then  root-move ;
: floored ( -- )  $B01 -1 play-table  $A her-mode !  $B her-doing !  ['] getting-up her-act ! ;
: after-throw ( -- )
    ended? if
        her-throw-back @ 0= if
            anim@ $1008 = if
                her-fear-bits @ 2 and if  floored exit  then
                $100A -1 play-table  ['] got-up her-act ! exit
            then
        else
            anim@ $B04 = if  floored exit  then
            anim@ $100B = if  $100D -1 play-table  ['] got-up her-act ! exit  then
        then
    then
    pull-back  along-wall ;
: thrown ( -- )
    settled? if
        striker? if  0 her-fall-turn !  her-hit-who @ body-at her-at vec-heading her-fall-yaw f!   \ (away from him)
        else  -1 her-fall-turn !  her-yaw f@ her-fall-yaw f!  then
        her-fall-yaw f@ her-yaw f@ f- angle-wrap fabs pi f2/ f< if
            0 her-throw-back !  $1008 -1 play-table
        else
            -1 her-throw-back !  her-fall-yaw f@ pi f+ angle-wrap her-fall-yaw f!
            her-fear-bits @ 2 and if  -1 her-fall-turn !  her-fall-yaw f@ her-yaw f!  $B04  else  $100B  then
            -1 play-table
        then
        her-fear-bits @ 2 and if  $42 0 her-sound  $6F noise-here  1000e f>cell s" panic" actor-named send fear-set
        else  $40 0 her-sound  $5F noise-here  then
        ['] after-throw her-act !
    then
    pull-back ;

\ ---- caught (Fiona_StateCaught: a hold - her gasp, a flinch in his grip by how he holds her:
\ an arm $F02 looking at him, $F04, from behind $F03, $F05; then free) ----
: watch-him ( -- )   \ (her head on him while caught by the arm or from behind)
    her-hit-how @ dup 1 = swap 3 = or striker? and if  1 her-look-on !  her-hit-who @ her-look-who !  then ;
: after-caught ( -- )  ended? if  to-idle exit  then  watch-him  root-move ;
: flinching ( -- )
    settled? if
        $80 8 fiona-jolt  $43 0 her-sound
        her-hit-how @ case  4 of $F05 endof  1 of $F02 endof  2 of $F04 endof  >r $F03 r>  endcase
        -1 play-table  ['] after-caught her-act !
    then
    root-move ;
\ a flinch with nobody holding her (the story's action $B / $21 - water-tower-4f-1): by the arm
\ on open floor, else as from behind
: flinch ( -- )
    her-led @ if  exit  then
    -1 her-hit-who !  1  her-tri @ dup 0< 0= if  nav-flags $80001 and if  drop 3  then  else  drop  then  her-hit-how !
    $B her-mode !  8 her-doing !  ['] flinching her-act ! ;
\ his grip: by the arm (1) only on open floor, facing him (else from behind, 3)
: grip ( how -- how' )
    dup 1 <> if  exit  then  drop
    her-tri @ dup 0< 0= if  nav-flags $80001 and if  3 exit  then  else  drop  then
    striker? 0= if  3 exit  then
    her-at her-hit-who @ body-at vec-heading her-yaw f@ f- angle-wrap fabs pi f2/ f< if  1  else  3  then ;

\ ---- a spirit's fall (Fiona_StateFall13): away from where it came from (her request's
\ point) - on open floor by the side it came from ($1004 ahead .. as a blow), on a step or in a
\ doorway straight down ($100F); her cry, a noise; at the fall's last frame the threat up 75,
\ then up (MOTION_EVENTS $400 / $20: the motion's own flags, act.mflags) ----
: at-end? ( -- flag )  her-model @ actor act.mflags l@ $400 and 0<> ;   \ (its last frame: the motion's flag $400)
: risen ( -- )  ended? if  30 her-stumble-t !  to-idle exit  then  root-move ;
: after-fall ( -- )
    at-end? if  75e f>cell s" panic" actor-named send threat  ['] risen her-act !  then
    root-move ;
: falling ( -- )
    settled? if
        $3F 0 her-sound
        on-step? if  $100F -1 play-table
        else  from-pt her-at vec-heading her-yaw f@ f- angle-wrap side f!  $1004 fall-by-side  then
        $5F noise-here  ['] after-fall her-act !
    then
    root-move ;

\ ---- hit by a door (Fiona_StateHitByDoor: request 5, the stalker pushing a door she is behind):
\ thrown along its swing, away from it - forward ($1008) or backward ($100B; panicking she is
\ flung down, $B04, with a scream and her panic full); then as thrown ----
create door-pt 12 allot
: door-way ( F: -- a )   \ (the door's turn at rest, or round from it: whichever points from beyond it to her)
    her-hit-how @ door-turn
    her-hit-how @ 1 exit-spot drop  door-pt vec!  door-pt her-at vec-heading      ( F: turn away )
    fover f- angle-wrap fabs pi f2/ f> if  pi f+ angle-wrap  then ;
: door-thrown ( -- )
    door-way  -1 her-fall-turn !  fdup her-fall-yaw f!
    her-yaw f@ f- angle-wrap fabs pi f2/ f< if
        0 her-throw-back !  $1008 -1 play-table
    else
        -1 her-throw-back !  her-fall-yaw f@ pi f+ angle-wrap her-fall-yaw f!
        her-fear-bits @ 2 and if  her-fall-yaw f@ her-yaw f!  $B04  else  $100B  then  -1 play-table
    then
    her-fear-bits @ 2 and if
        $42 0 her-sound  $6F noise-here  1000e f>cell s" panic" actor-named send fear-set
    else  $40 0 her-sound  $5F noise-here  then
    ['] after-throw her-act ! ;

\ the jolt each reaction gives the pad (Fiona_React)
: reaction-jolt ( r -- )
    case
        $13 of  $A0 8  endof               \ (a spirit's fall)
        $C of  $80 8  endof   $E of  $80 8  endof   \ (knocked down: the lighter blows)
        $D of  $A0 8  endof   $F of  $A0 8  endof
        $A of  $D0 12  endof  $B of  $D0 12  endof   \ (thrown, a door into her)
        $10 of  $FF 16  endof               \ (seized)
        >r 0 0 r>
    endcase  ?dup if  fiona-jolt  else  drop  then ;

: knocked-or-thrown? ( r -- flag )  dup $A = swap $C $10 within or ;
: by-somebody? ( -- flag )  her-hit-who @ dup 0< if  drop false exit  then  s" hewie" actor-named <> ;
: room-to-throw? ( -- flag )   \ (Actor_TriFreeFor: not on a floor nothing stands on, nor at a door or a ladder)
    her-tri @ dup 0< 0= if  nav-flags $80001 and if  false exit  then  else  drop  then
    at-a-door? by-a-ladder? or 0= ;

\ ---- Fiona_React: the blow taken - its fright to her panic, what she was doing dropped, the
\ reaction started; struck, the danger is told (a stumble resets her recovery: 1800 frames to calm) ----
: struck ( kind how who -- taken? ) ( F: fright -- )
    her-hit-who !  her-hit-how !
    reaction dup 0< if  drop fdrop false exit  then
    f>cell s" panic" actor-named send fright
    dup $20 = charmed? and if  drop false exit  then   \ (wearing the charm: she can't be caught)
    her-doing @ 3 = if  drop-door  then
    -1 her-fall-turn !  0 her-busy-t !
    4 her-mode !  $A her-doing !  dup her-sub !
    dup knocked-or-thrown? if   \ (a blow: the danger told only of one by somebody other than Hewie;
        by-somebody? if  1 danger-id send danger-signal  then   \ a stumble resets her recovery)
        her-hit-how @ $8000 and if  reset-recovery  then
    then
    dup $B = over 8 = or if  1 danger-id send danger-signal  then   \ (Progress_SetCondBit 1: she's been struck)
    dup reaction-jolt
    dup $10 = if  drop  fiona-occupied state-flag-set  grabbed  true exit  then
    dup $12 = if  drop  held-down  true exit  then
    case
        $13 of  ['] falling her-act !  endof
        $20 of
            her-hit-how @ 5 = if   \ (the door's hold: thrown where there's room, else knocked down by him)
                room-to-throw? if  -1 her-hit-who !  $D0 12 fiona-jolt  ['] thrown her-act !
                else  fiona.slam:stalker-in-room her-hit-who !  $80 8 fiona-jolt  $C her-sub !  ['] knocked her-act !  then
            else  $B her-mode !  8 her-doing !  her-hit-how @ grip her-hit-how !  ['] flinching her-act !  then   \ (its rumble as she flinches)
        endof
        $A of  ['] thrown her-act !  endof
        $B of  her-hit-how @ her-door-hit !  ['] door-thrown her-act !  endof
        8 of  knocked-off  endof
        >r ['] knocked her-act ! r>
    endcase
    true ;

\ ---- thrown down by a script (the story's 0xB2, Fiona_StartAction4: sub $A, no choosing): by
\ `who` (-1 nobody); `recover` her recovery reset too ----
: thrown-down ( who recover -- )
    if  reset-recovery  then  her-hit-who !  0 her-hit-how !
    -1 her-fall-turn !  0 her-busy-t !  4 her-mode !  $A her-doing !  $A her-sub !
    $D0 12 fiona-jolt  1 danger-id send danger-signal  ['] thrown her-act ! ;
