\ fiona/defend.fs - Fiona defending herself (F4; src/game/fiona.c Fiona_StateShoveStart /
\ StateStrike / StateShove, Fiona_StateKickStart / StateKick; Progress_PlayerButtons'
\ square, Fiona_StateBlock 8): the square button, while she's free and not past the panic's
\ stage 3 - standing, walking, resting, or just set off running, a shove ($E00: her hand, damage
\ 1); running or turning to flee, a kick ($E01: her body, damage 5, and it may make him
\ stumble). A blow is a `blow` to whom it reaches; struck home she's held still 5 frames.
\ Hewie is told when she shoves or kicks without him being struck (Fiona_HewieReact 10 / 11).
IN: fiona.defend
USING: engine game-state actors messages common keys facts flag-names fiona.state fiona.model fiona.moving fiona.slam fiona.fear fiona.controls fiona.looks ;

defer to-idle ( -- )
\ (the stalker and Hewie in her room, held still by a blow: fiona/slam.fs)
: moving ( -- )  her-recoil @ 0= if  root-move  then ;

\ ---- whom a blow at a point reaches (Progress_CharNear: within their height and radius,
\ widened by the margin) ----
fvariable cn-m  variable cn-v  variable cn-id
: near-point? ( v id -- flag ) ( F: margin -- )
    cn-m f!  cn-id !  cn-v !  cn-id @ 0< if  false exit  then
    cn-v @ 4 + sf@  cn-id @ body-at 4 + sf@ cn-m f@ f-  f> 0= if  false exit  then
    cn-v @ 4 + sf@  cn-id @ body-at 4 + sf@  cn-id @ body-dims fswap fdrop f+ cn-m f@ f+  f< 0= if  false exit  then
    cn-v @ cn-id @ body-at vec-dist-xz  cn-id @ body-dims fdrop cn-m f@ f+  f< ;
: touching? ( id -- flag )  dup 0< if  drop false exit  then  self swap 0e 0e bodies-touching? ;

\ a blow sent: to the stalker (1) and Hewie (2) it reaches, each once a blow
: strike ( kind damage how mask -- )
    her-struck @ invert and  dup her-struck @ or her-struck !
    dup 1 and if  stalker-in-room >r  2over 2over drop r> send blow  then
    2 and if  dog-in-room send blow  else  2drop drop  then ;

\ ---- the shove: at its pace (slower the longer she's been shaken, the more frightened), her
\ hand at its key reaching 2 round; into a wall it ends ($101) ----
create hand 12 allot
: shove-end ( -- )  ended? if  0 her-run-t !  to-idle exit  then  moving ;
: shoving ( -- )
    recoil-down
    ended? if  her-struck @ 2 and 0= if  10 hewie-id send reaction  then  0 her-run-t !  to-idle exit  then
    moving
    events 2 and 0= if  exit  then
    her-model @ 9 bone-pos hand vec!
    hand true slam-door if  exit  then                     \ (a door there slammed)
    hand blocked-floor v-tri-in 0< if  $101 10 -1 play-blend  ['] shove-end her-act ! exit  then
    0  hand stalker-in-room 2e near-point? 1 and or  hand dog-in-room 2e near-point? 2 and or
    ?dup if  >r 1 1 0 r> strike  then ;
fvariable pace
: shove-start ( -- )
    moving
    settled? 0= if  exit  then
    1e  her-recovery @ $1C3 < 0= if  fdrop 3150 her-recovery @ - s>f 1800e f/ 1.5e f*  then
    1e  her-fear f@ 40e f> if  fdrop 160e her-fear f@ f- 120e f/  then
    fmin 0.1e fmax pace f!
    $E00 2 play-table  pace f@ weight!
    0 her-struck !  ['] shoving her-act ! ;

\ ---- the kick: a step ahead while it fades in; while its key is clear it meets whom it
\ touches (a kick, damage 5, he may stumble); at its end her fear up 10 ----
create ahead-pt 12 allot   \ (her radius ahead)
: ahead-pt! ( -- )  her-yaw f@ self body-dims fdrop ahead-pt her-at vec-ahead ;
: kicking ( -- )
    recoil-down
    her-recoil @ 0= if  settled? 0= if  0e 1.0756e her-yaw f@ rotate-by move-by  else  root-move  then  then
    ended? if
        her-struck @ 2 and 0= if  11 hewie-id send reaction  then
        10e add-fear  0 her-run-t !  to-idle exit
    then
    events 2 and if  exit  then
    ahead-pt! ahead-pt true slam-door if  exit  then                 \ (a door ahead slammed)
    0  stalker-in-room touching? 1 and or  dog-in-room touching? 2 and or
    ?dup if  >r 2 5 1 r> strike  then ;
: kick-start ( -- )
    root-move
    settled? 0= if  exit  then
    $3D 0 her-sound  $E01 -1 play-table  0 her-struck !  ['] kicking her-act ! ;

\ ---- the square button (Progress_PlayerButtons, Fiona_StateBlock 8) ----
: try-defend ( -- )
    square button-pressed? 0= if  exit  then
    hands-off?  her-mode @ 0<> or  her-panic-stage @ 4 >= or  no-flee state-flag? or if  exit  then
    her-doing @ $D = if  6 her-doing !  8 her-mode !  $1B her-sub !  ['] kick-start her-act ! exit  then
    group dup 5 = over 1 = or over 0= or
    over 2 = her-recovery @ 0<> her-run-t @ $3D < or and or if
        drop  5 her-doing !  8 her-mode !  $1A her-sub !  ['] shove-start her-act ! exit
    then
    2 = if  6 her-doing !  8 her-mode !  $1B her-sub !  ['] kick-start her-act !  then ;
\ her blow struck home (`blow-taken`): held still; her sound ($90 a kick, $8F a shove; Hewie
\ counted as kicked - with his trust, H3)
: struck-home ( -- )  her-sub @ $1B = if  $90  else  $8F  then  0 her-sound  recoil ;
