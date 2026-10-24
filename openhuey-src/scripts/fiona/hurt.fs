\ fiona/hurt.fs - Fiona struck or caught (F4; src/game/fiona.c Fiona_Reaction, Fiona_React and
\ the states after: Fiona_StateKnockedDown, Fiona_StateThrown, Fiona_StateCaught). A `hit` from
\ a stalker: what it makes of her by what she is doing - knocked down (a blow; on a step or in a
\ doorway the short falls), thrown (a hard blow: forwards or backwards by where it came from),
\ caught (a hold: she flinches in his grip and pulls free) - the fright it carries, and getting
\ up again. How she stands for an attack (`fiona-plight`) is told each frame.
IN: fiona.hurt
USING: engine vectors game-state actors messages common facts flag-names rumble items.names fiona.state fiona.anims fiona.sounds fiona.model fiona.moving fiona.fear floor-flags ;

defer to-idle ( -- )
\ how he holds her (her-hit-how): by the arm, from the side, from behind, from the front; a door's
1 constant by-arm   2 constant by-side   3 constant from-behind   4 constant from-front   5 constant by-door
$8000 constant stumbles   \ (a blow's flag: she stumbles)
defer grabbed ( -- )   \ (seized outright: fiona.seized)
defer held-down ( -- )   \ (held by homunculi: fiona.seized)
defer knocked-off ( -- )   \ (struck on a ladder: fiona.ladder)
defer drop-door ( -- )   ' noop is drop-door   \ (a door she held let go: fiona.doors)
: danger-id ( -- id )  s" danger" actor-named ;

\ ---- how she stands for a stalker's attack (Pursuer_FionaState): out of reach while struck
\ (her reactions, mode 4) or seized; the panic's stages 4 / 5; fleeing (mode $B); panicking
\ (fear over 90, or the panic attack) ----
: plight ( -- n )
    her-mode @ thrown-mode =  her-sub @ grabbed-r = or  stalkers-blind state-flag? or  her-aside @ or if  unreachable-plight exit  then
    her-panic-stage @ panicking = if  panicking-plight exit  then
    her-panic-stage @ calming-down = if  calming-plight exit  then
    her-mode @ react-mode = if  fleeing-plight exit  then
    her-fear f@ 90e f>  her-doing @ attack-doing = or if  frightened-plight exit  then
    free-plight ;

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
    her-tri @ dup 0< 0= if  nav-flags doorway-or-step 2 or and if  true exit  then  else  drop  then
    at-a-door?  by-a-ladder? or ;
: mode-in? ( m1 m2 -- flag )  her-mode @ tuck = >r = r> or ;   \ (her mode is one of the two)
7 constant on-rungs-sub
: on-rungs? ( -- flag )  her-mode @ ladder-mode =  her-sub @ on-rungs-sub = and ;    \ (on the ladder itself: knocked off)
: charmed? ( -- flag )  progress pr.equipped 4 + sl@ alchemist-earrings = ;   \ (the Alchemist Earrings: she can't be caught)
: reacting? ( -- flag )  thrown-mode down-mode mode-in? ;   \ (one under way, or down)
: knock-for ( light heavy -- r )   \ (on the rungs knocked off; on a step or in a doorway the short falls)
    {: light heavy :}  on-rungs? if  off-ladder-r  else on-step? if  light  else  heavy  then then ;
\ Fiona_Reaction: her reaction to a request by its kind (-1 none)
: reaction ( kind -- r | -1 )
    world-held state-flag? if  drop -1 exit  then
    case
        spirit-request of  reacting? her-mode @ ladder-mode = or if  no-reaction  else  spirit-fall-r  then  endof
        blow-request of  reacting? if  no-reaction  else  step-knock-r knock-r knock-for  then  endof
        heavy-blow-request of  reacting? if  no-reaction  else  step-knock-hard-r knock-hard-r knock-for  then  endof
        throw-request of  reacting? if  no-reaction  else  step-knock-hard-r thrown-r knock-for  then  endof
        grab-request of  her-mode @ thrown-mode = her-sub @ grabbed-r = and if  $FF 16 fiona-jolt  no-reaction  else  grabbed-r  then  endof   \ (seized: again, only felt)
        door-request of  her-mode @ thrown-mode = her-sub @ door-thrown-r = and  her-hit-how @ her-door-hit @ = and if  no-reaction  else  door-thrown-r  then  endof   \ (not again by the same door)
        hold-request of  reacting?  her-mode @ own-mode = her-doing @ breathless-doing = and  or if  no-reaction   \ (not while out of breath)
              else charmed? if  caught-r  else on-rungs? if  off-ladder-r  else  caught-r  then then then  endof
        \ (lead-request, led away by the hand or walking: the stalker's `seize`, fiona/seized.fs)
        homunculi-request of  her-mode @ thrown-mode = if  her-sub @ held-down-r = if  held-down-r  else  no-reaction  then   \ (homunculi: in a reaction only theirs again)
               else  ladder-mode down-mode mode-in? if  no-reaction  else  held-down-r  then  then  endof
        >r no-reaction r>
    endcase ;

\ ---- the way the blow sends her: from whoever struck to her (nobody: from where it was asked
\ from - her request's point, the original's noise point of her slot), turned from her heading;
\ a striker gone meanwhile, none - as from behind (Fiona_StateKnockedDown) ----
fvariable side
create from-pt /vec allot
: striker? ( -- flag )  her-hit-who @ dup 0< if  drop false exit  then  dup alive? if  body?  else  drop false  then ;
: blow-side ( -- )
    her-hit-who @ 0< if  from-pt her-at vec-heading  her-yaw f@ f- angle-wrap side f! exit  then
    striker? 0= if  0e side f! exit  then
    her-hit-who @ body-at her-at vec-heading  her-yaw f@ f- angle-wrap side f! ;
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
            step-knock-r of  knocked-sound 0 her-sound  knocked-anim -1 play-table  endof
            knock-r of  knocked-sound 0 her-sound  $1000 fall-by-side  endof
            step-knock-hard-r of  struck-hard-sound 0 her-sound  fall-through-anim -1 play-table  endof
            knock-hard-r of  struck-hard-sound 0 her-sound  $1004 fall-by-side  endof
        endcase
        $5F noise-here  ['] up-again her-act !
    then
    root-move ;

\ ---- thrown (Fiona_StateThrown): away from him - her back to him, forwards ($1008); facing
\ him, backwards ($100B; panicking she is flung down $B04, turned to him). Moved by the motion
\ turned to that way, turning there 20 degrees a frame (pull_back_move), along walls - not while
\ thrown (sub $A) in an exit's area; then up ($100A / $100D, cut in), or panicking she stays down
\ and gets up as from a panic fall. Thrown, after and getting up, nav flag 1 blocks her too; by
\ a door (sub $B) her floor is the door's ($8000018) ----
$8000018 constant door-hit-floor
: thrown-floor ( -- mask )   \ (navMask |= 1)
    her-sub @ door-thrown-r = if  door-hit-floor  else  blocked-floor  then  1 or ;
: pull-back ( -- )
    her-fall-turn @ -1 = if  thrown-floor root-move-within exit  then
    root-step her-fall-yaw f@ rotate-by  thrown-floor move-within
    her-fall-turn @ 0= if
        her-yaw f@ her-fall-yaw f@ 20e deg>rad turn-toward f0= if  1 her-fall-turn !  then  her-yaw f!
    then ;
: got-up ( -- )  ended? if  to-idle exit  then  blocked-floor 1 or root-move-within ;   \ (getting_up)
: get-up ( anim -- )  play-cut  ['] got-up her-act ! ;
: thrown-down? ( -- anim | 0 )   \ (the fall that came to its end: forwards, backwards, flung down; 0 none)
    ended? 0= if  0 exit  then
    anim@ her-throw-back @ if
        dup flung-down-anim =  over thrown-back-anim = or 0= if  drop 0  then
    else  dup thrown-forward-anim <> if  drop 0  then  then ;
: after-throw ( -- )
    thrown-down? case
        thrown-forward-anim of  her-fear-bits @ 2 and if  lie-down  else  up-from-forward-anim get-up  then  exit  endof
        thrown-back-anim of  up-from-back-anim get-up  exit  endof
        flung-down-anim of  lie-down  exit  endof
    endcase
    pull-back
    her-sub @ thrown-r =  in-exit-area? and 0= if  along-wall  then ;
: thrown ( -- )
    settled? if
        striker? if  0 her-fall-turn !  her-hit-who @ body-at her-at vec-heading her-fall-yaw f!   \ (away from him)
        else  -1 her-fall-turn !  her-yaw f@ her-fall-yaw f!  then
        her-fall-yaw f@ her-yaw f@ f- angle-wrap fabs pi f2/ f< if
            0 her-throw-back !  thrown-forward-anim -1 play-table
        else
            -1 her-throw-back !  her-fall-yaw f@ pi f+ angle-wrap her-fall-yaw f!
            her-fear-bits @ 2 and if  -1 her-fall-turn !  her-fall-yaw f@ her-yaw f!  flung-down-anim  else  thrown-back-anim  then
            -1 play-table
        then
        her-fear-bits @ 2 and if  panic-scream-sound 0 her-sound  $6F noise-here  1000e f>cell s" panic" actor-named send fear-set
        else  fall-sound 0 her-sound  $5F noise-here  then
        ['] after-throw her-act !
    then
    blocked-floor 1 or root-move-within ;

\ ---- caught (Fiona_StateCaught: a hold - her gasp, a flinch in his grip by how he holds her:
\ an arm $F02 looking at him, $F04, from behind $F03, $F05; then free) ----
: watch-him ( -- )   \ (her head on him while caught by the arm or from behind)
    her-hit-how @ dup by-arm = swap from-behind = or striker? and if  1 her-look-on !  her-hit-who @ her-look-who !  then ;
: after-caught ( -- )  ended? if  to-idle exit  then  watch-him  root-move ;
: flinching ( -- )
    settled? if
        $80 8 fiona-jolt  seized-sound 0 her-sound
        her-hit-how @ case  from-front of $F05 endof  by-arm of pull-free-anim endof  by-side of $F04 endof  >r $F03 r>  endcase
        -1 play-table  ['] after-caught her-act !
    then
    root-move ;
\ a flinch with nobody holding her (the story's action $B / $21 - water-tower-4f-1): by the arm
\ on open floor, else as from behind
: flinch ( -- )
    her-led @ if  exit  then
    -1 her-hit-who !  by-arm  her-tri @ dup 0< 0= if  nav-flags doorway-or-step and if  drop from-behind  then  else  drop  then  her-hit-how !
    react-mode her-mode !  flinch-doing her-doing !  ['] flinching her-act ! ;
\ his grip: by the arm (1) only on open floor, facing him (else from behind, 3)
: grip ( how -- how' )
    dup by-arm <> if  exit  then  drop
    her-tri @ dup 0< 0= if  nav-flags doorway-or-step and if  from-behind exit  then  else  drop  then
    striker? 0= if  from-behind exit  then
    her-at her-hit-who @ body-at vec-heading her-yaw f@ f- angle-wrap fabs pi f2/ f< by-arm from-behind ? ;

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
        struck-hard-sound 0 her-sound
        on-step? if  fall-through-anim -1 play-table
        else  from-pt her-at vec-heading her-yaw f@ f- angle-wrap side f!  $1004 fall-by-side  then
        $5F noise-here  ['] after-fall her-act !
    then
    root-move ;

\ ---- hit by a door (Fiona_StateHitByDoor: request 5, the stalker pushing a door she is behind):
\ thrown along its swing, away from it - forward ($1008) or backward ($100B; panicking she is
\ flung down, $B04, with a scream and her panic full); then as thrown ----
create door-pt /vec allot
: door-way ( F: -- a )   \ (the door's turn at rest, or round from it: whichever points from beyond it to her)
    her-hit-how @ door-turn
    her-hit-how @ 1 exit-spot drop  door-pt vec!  door-pt her-at vec-heading      ( F: turn away )
    fover f- angle-wrap fabs pi f2/ f> if  pi f+ angle-wrap  then ;
: door-thrown ( -- )
    door-way  0 her-fall-turn !  fdup her-fall-yaw f!   \ (turned to the swing as she slides)
    her-yaw f@ f- angle-wrap fabs pi f2/ f< if
        0 her-throw-back !  thrown-forward-anim play-cut
    else
        -1 her-throw-back !  her-fall-yaw f@ pi f+ angle-wrap her-fall-yaw f!
        her-fear-bits @ 2 and if  -1 her-fall-turn !  her-fall-yaw f@ her-yaw f!  flung-down-anim  else  thrown-back-anim  then  play-cut
    then
    blocked-floor 1 or root-move-within
    her-fear-bits @ 2 and if
        panic-scream-sound 0 her-sound  $6F noise-here  1000e f>cell s" panic" actor-named send fear-set
    else  fall-sound 0 her-sound  $5F noise-here  then
    ['] after-throw her-act ! ;

\ the jolt each reaction gives the pad (Fiona_React)
: reaction-jolt ( r -- )
    case
        spirit-fall-r of  $A0 8  endof
        step-knock-r of  $80 8  endof   knock-r of  $80 8  endof   \ (knocked down: the lighter blows)
        step-knock-hard-r of  $A0 8  endof   knock-hard-r of  $A0 8  endof
        thrown-r of  $D0 12  endof  door-thrown-r of  $D0 12  endof   \ (thrown, a door into her)
        grabbed-r of  $FF 16  endof
        >r 0 0 r>
    endcase  ?dup if  fiona-jolt  else  drop  then ;

: knocked-or-thrown? ( r -- flag )  dup thrown-r = swap step-knock-r grabbed-r within or ;
: by-somebody? ( -- flag )  her-hit-who @ dup 0< if  drop false exit  then  s" hewie" actor-named <> ;
: room-to-throw? ( -- flag )   \ (Actor_TriFreeFor: not on a floor nothing stands on, nor at a door or a ladder)
    her-tri @ dup 0< 0= if  nav-flags doorway-or-step and if  false exit  then  else  drop  then
    at-a-door? by-a-ladder? or 0= ;

\ ---- Fiona_React: the blow taken - its fright to her panic, what she was doing dropped, the
\ reaction started; struck, the danger is told (a stumble resets her recovery: 1800 frames to calm) ----
: struck ( kind how who -- taken? ) ( F: fright -- )
    her-hit-who !  her-hit-how !
    reaction dup 0< if  drop fdrop false exit  then
    f>cell s" panic" actor-named send fright
    dup caught-r = charmed? and if  drop false exit  then   \ (wearing the charm: she can't be caught)
    her-doing @ door-doing = if  drop-door  then
    -1 her-fall-turn !  0 her-busy-t !
    thrown-mode her-mode !  thrown-doing her-doing !  dup her-sub !
    dup knocked-or-thrown? if   \ (a blow: the danger told only of one by somebody other than Hewie;
        by-somebody? if  fiona-struck danger-id send danger-signal  then   \ a stumble resets her recovery)
        her-hit-how @ stumbles and if  reset-recovery  then
    then
    dup door-thrown-r = over off-ladder-r = or if  fiona-struck danger-id send danger-signal  then   \ (Progress_SetCondBit 1: she's been struck)
    dup reaction-jolt
    dup grabbed-r = if  drop  fiona-occupied state-flag-set  grabbed  true exit  then
    dup held-down-r = if  drop  held-down  true exit  then
    case
        spirit-fall-r of  ['] falling her-act !  endof
        caught-r of
            her-hit-how @ by-door = if   \ (the door's hold: thrown where there's room, else knocked down by him)
                room-to-throw? if  -1 her-hit-who !  $D0 12 fiona-jolt  ['] thrown her-act !
                else  fiona.slam:stalker-in-room her-hit-who !  $80 8 fiona-jolt  step-knock-r her-sub !  ['] knocked her-act !  then
            else  react-mode her-mode !  flinch-doing her-doing !  her-hit-how @ grip her-hit-how !  ['] flinching her-act !  then   \ (its rumble as she flinches)
        endof
        thrown-r of  ['] thrown her-act !  endof
        door-thrown-r of  her-hit-how @ her-door-hit !  ['] door-thrown her-act !  endof
        off-ladder-r of  knocked-off  endof
        >r ['] knocked her-act ! r>
    endcase
    true ;

\ ---- thrown down by a script (the story's 0xB2, Fiona_StartAction4: sub $A, no choosing): by
\ `who` (-1 nobody); `recover` her recovery reset too ----
: thrown-down ( who recover -- )
    if  reset-recovery  then  her-hit-who !  0 her-hit-how !
    -1 her-fall-turn !  0 her-busy-t !  thrown-mode her-mode !  thrown-doing her-doing !  thrown-r her-sub !
    $D0 12 fiona-jolt  fiona-struck danger-id send danger-signal  ['] thrown her-act ! ;
