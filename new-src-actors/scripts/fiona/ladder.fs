\ fiona/ladder.fs - Fiona on a ladder (src/game/fiona.c Fiona_StateLadder / LadderWalk / OnLadder /
\ LadderOff; Progress_CharRequests' nav door regions, Actor_FacingDoor, Actor_DoorFront). A
\ ladder is a nav link of the room (`nav-link-*`: its two ends' triangles, facings and points;
\ side 0 its foot, 1 its top). Free, in a ladder's region on one of its sides, nobody else there
\ but Hewie, facing along that side - the action button: she walks to its end's spot (at the top
\ 1.7 back from it, at the foot 2.1 out), faces the ladder and gets on ($700 from the top, $704 from the
\ foot). On it, as each move comes round, the stick: down - the next rung ($701, its end $702),
\ off at the foot within 18 of it ($703); up - the next rung ($705, its end $706), off at the top
\ within 3 of it ($707); let go - holding ($708 / $709). The stalker at its foot keeps her from
\ getting off there, at its top (within his height above her) from going up. Off: at the move's
\ event $20 she is put by that end and free. Her moves on it are her animations' own,
\ up and down too.
IN: fiona.ladder
USING: engine game-state actors messages common keys facts flag-names fiona.state fiona.model fiona.moving fiona.spots fiona.controls fiona.slam ;

defer to-idle   ' noop is to-idle        \ (fiona.fs)
: events ( -- bits )  her act.mflags l@ ;   \ (MOTION_EVENTS: the motion's flags; $20 come round)
: play-own ( anim -- )  dup entry 8 or >r >r -1 r> r> start ;   \ (Motion_PlayOwnBlend)
: play-cut ( anim -- )  dup entry >r >r -1 r> r> start ;        \ (Motion_Play)
: off-ladder ( -- )  -1 her-ladder !  to-idle ;

\ ---- moved by her animation's root motion, free of the floor (up and down too) ----
: climb-move ( -- )
    root-turn  rm-x f@ rm-z f@ her-yaw f@ rotate-by                    ( F: dx dz )
    her-at 8 + sf@ f+ her-at 8 + sf!  her-at sf@ f+ her-at sf!
    her-at 4 + sf@ rm-y f@ f+ her-at 4 + sf! ;
\ where she stands off it at an end (kFionaMeetOffsets: the foot 2.1 out, the top 1.7 back from it)
: off-offset ( side -- side ) ( F: -- ox oz )  0e  dup if  6.6709e  else  -7.1132e  then ;
\ her triangle at an end (after each move: off at the foot or on from it - the foot's; else the top's)
: end-tri ( side -- )
    her-ladder @ swap off-offset nav-link-front  fdrop fdrop fdrop
    dup 0< if  drop  else  her-tri !  then ;
\ the end's point (Rooms' nav +0x5C: 0 the foot, 1 the top)
: end-y ( side -- ) ( F: -- y )  her-ladder @ swap nav-link-spot  fdrop fswap fdrop ;

\ ---- the stalker in this ladder's region on that side (Progress_WhoIsWhere's region bits: 4
\ side 0, its foot; 2 side 1, its top): at the foot he keeps her from getting off there; at the
\ top, within his height above her, from going up ----
: stalker-on ( side -- flag )
    stalker-in-room dup 0< if  2drop false exit  then  >r
    r@ body-at her-ladder @ nav-link-at? 0= if  r> 2drop false exit  then
    dup  r@ body-at r@ body-tri her-ladder @ nav-link-side =        ( side flag )
    swap 1 = if  her-at 4 + sf@ r@ body-at 4 + sf@ f-  r@ body-dims fswap fdrop f< and  then
    r> drop ;

\ ---- off (Fiona_StateLadderOff): at the event $20 put by that end, free ----
: ladder-off ( -- )
    events $20 and 0= if  climb-move exit  then
    anim@ $707 = if  1  else  0  then
    her-ladder @ swap off-offset nav-link-front                      ( tri ) ( F: x y z )
    dup 0< if  drop fdrop fdrop fdrop  else  her-tri !  her-at 8 + sf! her-at 4 + sf! her-at sf!  then
    0 her-mode !  off-ladder ;

\ ---- on it (Fiona_StateOnLadder): as each move comes round, the stick picks the next ----
: stick-up ( F: -- v )  sz f@ fnegate ;   \ (the stick's up; W)
: rung-down ( -- )   \ (from $700 / $702 / $706 / $708: off at the foot within 18, else the next rung)
    0 end-y her-at 4 + sf@ f- 18e f< if
        0 stalker-on if  anim@ $708 <> if  $708 play-own  then  exit  then
        anim@ dup $700 = swap $702 = or if  $703 play-cut  else  $703 play-own  then
        ['] ladder-off her-act !  exit
    then
    anim@ dup $700 = swap $702 = or if  $701 play-cut  else  $701 play-own  then ;
: going-down ( anim -- moved? )
    $700 - case
        0 of  rung-down  endof  2 of  rung-down  endof  6 of  rung-down  endof  8 of  rung-down  endof
        1 of  $702 play-cut  endof
        4 of  0 stalker-on if  $708 play-own  else  $703 play-own  ['] ladder-off her-act !  then  endof
        5 of  $702 play-own  endof  9 of  $702 play-own  endof
        drop false exit
    endcase  true ;
: rung-up ( -- )   \ (from $702 / $704 / $706 / $708: off at the top within 3, else the next rung)
    her-at 4 + sf@ 1 end-y f- 3e f< if
        anim@ dup $704 = swap $706 = or if  $707 play-cut  else  $707 play-own  then
        ['] ladder-off her-act !  exit
    then
    anim@ dup $704 = swap $706 = or if  $705 play-cut  else  $705 play-own  then ;
: going-up ( anim -- moved? )
    $700 - case
        0 of  $707 play-own  ['] ladder-off her-act !  endof
        1 of  $706 play-own  endof  9 of  $706 play-own  endof
        5 of  $706 play-cut  endof
        2 of  rung-up  endof  4 of  rung-up  endof  6 of  rung-up  endof  8 of  rung-up  endof
        drop false exit
    endcase  true ;
: holding ( anim -- )
    $700 - case
        0 of  $708 play-own  endof  2 of  $708 play-own  endof  4 of  $708 play-own  endof  6 of  $708 play-own  endof
        1 of  $709 play-own  endof  5 of  $709 play-own  endof
    endcase ;
: on-ladder ( -- )   \ (until the move has faded in: still)
    settled? 0= if  exit  then
    anim@ events $20 and 0<>  over $708 $70A within or 0= if  drop climb-move exit  then   ( anim )
    stick-up fdup fabs 0.5e f> if
        f0< if  going-down  else  1 stalker-on if  holding true  else  going-up  then  then
    else  fdrop holding true  then
    if  anim@ dup $703 = swap $704 = or if  0  else  1  then  end-tri  then
    climb-move ;

\ ---- getting on (Fiona_StateLadder, LadderWalk): to its end's spot, facing it; there, on ----
: ladder-walk ( -- )
    walk-to-spot dup 0< if  drop off-ladder exit  then  if  exit  then
    7 her-sub !
    her-ladder-side @ if  $700  else  $704  then  play-own  ['] on-ladder her-act ! ;
: ladder-start ( -- )
    her-ladder @ her-ladder-side @  dup if  0e 6.7e  else  0e -7.1068e  then  nav-link-front   ( tri ) ( F: x y z )
    dup 0< if  drop fdrop fdrop fdrop off-ladder exit  then
    her-ladder @ her-ladder-side @ nav-link-yaw  walk-spot
    ['] ladder-walk her-act ! ;

\ ---- the action button by a ladder (Progress_CharRequests' regions, Progress_PlayerButtons) ----
: facing-along? ( link side -- flag )   \ (Actor_FacingDoor: her forward against its facing)
    nav-link-yaw her-yaw f@ f- fcos f0> ;
: ladder-here ( -- link side | -1 )
    nav-links 0 ?do
        her-at i nav-link-at? if
            her-at her-tri @ i nav-link-side dup 0 2 within if
                stalker-in-room dup 0< 0= if
                    dup body-at i nav-link-at? if  drop  drop unloop -1 exit  then
                then  drop
                i over facing-along? if  i swap unloop exit  then
            then  drop
        then
    loop  -1 ;
: try-ladders ( -- )
    her-mode @ 0<>  her-doing @ 0<> or  her-panic-stage @ 4 >= or if  exit  then
    circle button-pressed? 0= if  exit  then
    ladder-here dup 0< if  drop exit  then                              ( link side )
    stalker-in-room dup 0< 0= if  self swap 0e 0e bodies-touching? if  2drop exit  then  else  drop  then
    her-ladder-side !  her-ladder !
    6 her-sub !  2 her-doing !  3 her-mode !  ladder-start ;
