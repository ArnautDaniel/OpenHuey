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
USING: engine vectors game-state actors messages common keys facts flag-names rumble fiona.state fiona.anims fiona.model fiona.moving fiona.spots fiona.controls fiona.slam fiona.fear fiona.voice ;

defer to-idle   ' noop is to-idle        \ (fiona.fs)
: events ( -- bits )  her act.mflags l@ ;   \ (MOTION_EVENTS: the motion's flags; $20 come round)
: play-own ( anim -- )  dup entry 8 or >r >r -1 r> r> start ;   \ (Motion_PlayOwnBlend)
: play-cut ( anim -- )  dup entry >r >r -1 r> r> start ;        \ (Motion_Play)
: off-ladder ( -- )  -1 her-ladder !  to-idle ;

\ ---- moved by her animation's root motion, free of the floor (up and down too) ----
: climb-move ( -- )
    root-turn {: F: x y z :}  x z her-yaw f@ rotate-by {: F: dx dz :}
    her-at v.x sf@ dx f+ her-at v.x sf!  her-at v.z sf@ dz f+ her-at v.z sf!
    her-at v.y sf@ y f+ her-at v.y sf! ;
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
    stalker-in-room {: side him :}  him 0< if  false exit  then
    him body-at her-ladder @ nav-link-at? 0= if  false exit  then
    him body-at him body-tri her-ladder @ nav-link-side side =
    side 1 = if  her-at v.y sf@ him body-at v.y sf@ f-  him body-dims fswap fdrop f< and  then ;

\ ---- off (Fiona_StateLadderOff): at the event $20 put by that end, free ----
: ladder-off ( -- )
    events $20 and 0= if  climb-move exit  then
    anim@ off-ladder-top-anim = 1 0 ?  {: side :}
    her-ladder @ side off-offset nav-link-front                      ( tri ) ( F: x y z )
    dup 0< if  drop fdrop fdrop fdrop  else  her-tri !  her-at vec!  then
    own-mode her-mode !  off-ladder ;

\ ---- on it (Fiona_StateOnLadder): as each move comes round, the stick picks the next ----
: stick-up ( F: -- v )  sz fnegate ;   \ (the stick's up; W)
: from-end? ( -- flag )  anim@ dup on-ladder-top-anim = swap rung-down-end-anim = or ;   \ (a move's end: cut in)
: rung-down ( -- )   \ (from the top, a rung's end or holding: off at the foot within 18, else the next rung)
    0 end-y her-at v.y sf@ f- 18e f< if
        0 stalker-on if  anim@ ladder-hold-anim <> if  ladder-hold-anim play-own  then  exit  then
        from-end? if  off-ladder-foot-anim play-cut  else  off-ladder-foot-anim play-own  then
        ['] ladder-off her-act !  exit
    then
    from-end? if  rung-down-anim play-cut  else  rung-down-anim play-own  then ;
: going-down ( anim -- moved? )
    case
        on-ladder-top-anim of  rung-down  endof   rung-down-end-anim of  rung-down  endof
        rung-up-end-anim of  rung-down  endof     ladder-hold-anim of  rung-down  endof
        rung-down-anim of  rung-down-end-anim play-cut  endof
        on-ladder-foot-anim of  0 stalker-on if  ladder-hold-anim play-own  else  off-ladder-foot-anim play-own  ['] ladder-off her-act !  then  endof
        rung-up-anim of  rung-down-end-anim play-own  endof   ladder-hold-2-anim of  rung-down-end-anim play-own  endof
        drop false exit
    endcase  true ;
: on-from-foot? ( -- flag )  anim@ dup on-ladder-foot-anim = swap rung-up-end-anim = or ;
: rung-up ( -- )   \ (from the foot, a rung's end or holding: off at the top within 3, else the next rung)
    her-at v.y sf@ 1 end-y f- 3e f< if
        on-from-foot? if  off-ladder-top-anim play-cut  else  off-ladder-top-anim play-own  then
        ['] ladder-off her-act !  exit
    then
    on-from-foot? if  rung-up-anim play-cut  else  rung-up-anim play-own  then ;
: going-up ( anim -- moved? )
    case
        on-ladder-top-anim of  off-ladder-top-anim play-own  ['] ladder-off her-act !  endof
        rung-down-anim of  rung-up-end-anim play-own  endof   ladder-hold-2-anim of  rung-up-end-anim play-own  endof
        rung-up-anim of  rung-up-end-anim play-cut  endof
        rung-down-end-anim of  rung-up  endof   on-ladder-foot-anim of  rung-up  endof
        rung-up-end-anim of  rung-up  endof     ladder-hold-anim of  rung-up  endof
        drop false exit
    endcase  true ;
: holding ( anim -- )
    case
        on-ladder-top-anim of  ladder-hold-anim play-own  endof   rung-down-end-anim of  ladder-hold-anim play-own  endof
        on-ladder-foot-anim of  ladder-hold-anim play-own  endof   rung-up-end-anim of  ladder-hold-anim play-own  endof
        rung-down-anim of  ladder-hold-2-anim play-own  endof   rung-up-anim of  ladder-hold-2-anim play-own  endof
    endcase ;
: on-ladder ( -- )   \ (until the move has faded in: still)
    settled? 0= if  exit  then
    anim@ events $20 and 0<>  over ladder-hold-anim ladder-slip-anim within or 0= if  drop climb-move exit  then   ( anim )
    stick-up fdup fabs 0.5e f> if
        f0< if  going-down  else  1 stalker-on if  holding true  else  going-up  then  then
    else  fdrop holding true  then
    if  anim@ dup off-ladder-foot-anim = swap on-ladder-foot-anim = or 0 1 ?  end-tri  then
    climb-move ;

\ ---- getting on (Fiona_StateLadder, LadderWalk): to its end's spot, facing it; there, on ----
6 constant getting-on   7 constant on-rungs   \ (her sub on the ladder)
: ladder-walk ( -- )
    walk-to-spot dup 0< if  drop off-ladder exit  then  if  exit  then
    on-rungs her-sub !
    her-ladder-side @ if  on-ladder-top-anim  else  on-ladder-foot-anim  then  play-own  ['] on-ladder her-act ! ;
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
    her-mode @ own-mode <>  her-doing @ free-doing <> or  her-panic-stage @ panicking >= or if  exit  then
    circle button-pressed? 0= if  exit  then
    ladder-here dup 0< if  drop exit  then                              ( link side )
    stalker-in-room dup 0< 0= if  self swap 0e 0e bodies-touching? if  2drop exit  then  else  drop  then
    her-ladder-side !  her-ladder !
    getting-on her-sub !  ladder-doing her-doing !  ladder-mode her-mode !  ladder-start ;

\ ---- knocked off it (a blow while on it, Fiona_Reaction 8: Fiona_StateCaughtCrawling): to the
\ spot 10 out from its top - near it (under 20 below) pulled off ($F02), else dropped there
\ ($70A, then $70B as it comes round), falling 0.5 faster each frame over as many frames as that
\ takes; there her cry ($7D pulled, $81 fallen); fallen, down ($70C) and up ($100D) - her
\ recovery reset (CrawlDrop / CrawlPull / CrawlFall / CrawlDown) ----
: crawl-up ( -- )   \ (standing again: out of the ladder)
    ended? if  her-drop-far @ if  reset-recovery  then  own-mode her-mode !  off-ladder exit  then  root-move ;
: crawl-down ( -- )  ended? if  up-from-back-anim play-cut  ['] crawl-up her-act !  then  root-move ;
: crawl-fall ( -- )  settled? if  ladder-fallen-anim -1 play-table  ['] crawl-down her-act !  then ;
: crawl-drop ( -- )
    -1 her-drop-n +!
    her-drop-n @ if
        her-drop-far @  anim@ ladder-fall-anim <> and  ended? and if  ladder-fall-anim -1 play-table  then
        her-at v.x sf@ her-drop-dx f@ f- her-at v.x sf!
        her-at v.y sf@ her-drop-dy f@ f- her-at v.y sf!
        her-at v.z sf@ her-drop-dz f@ f- her-at v.z sf!
        her-drop-dy f@ 0.5e f+ her-drop-dy f!  exit   \ (falling faster)
    then
    her-at her-drop-at vec-copy  her-drop-tri @ dup 0< if  drop  else  her-tri !  then
    $D0 12 fiona-jolt   \ (landed)
    her-drop-far @ if  $81 voice  ['] crawl-fall  else  $7D voice  ['] crawl-up  then  her-act ! ;
: frames-to-fall ( F: h -- ) ( -- n )   \ (0, 0.5, 1 ... a frame: how many frames to fall h)
    {: F: h | F: step :}  1
    begin  h step f- to h  h f0< 0= while  step 0.5e f+ to step  1+  repeat ;
: knocked-off ( -- )
    her-ladder @ 1 0e 10e nav-link-front her-drop-tri !  her-drop-at vec!
    her-at v.y sf@ her-drop-at v.y sf@ f- fdup 20e f< if
        0 her-drop-far !  react-mode her-mode !  pull-free-anim -1 play-table  $3F 0 her-sound
    else
        -1 her-drop-far !  ladder-slip-anim -1 play-table  $40 0 her-sound
    then
    frames-to-fall dup her-drop-n !  s>f  {: F: n :}
    her-at v.x sf@ her-drop-at v.x sf@ f- n f/ her-drop-dx f!
    her-at v.z sf@ her-drop-at v.z sf@ f- n f/ her-drop-dz f!
    0e her-drop-dy f!  ['] crawl-drop her-act ! ;
