\ fiona/moving.fs - how Fiona moves (Fiona_StateIdleMove, the turning states, Fiona_Footsteps):
\ her animations' root motion moves her over her floor; she stands, rests, walks and runs.
IN: fiona.moving
USING: engine game-state actors messages common vectors facts items.names fiona.state fiona.anims fiona.model fiona.controls flag-names floor-flags ;

: blocked-floor ( -- mask )   \ (none - the story's 0x20: through anything)
    her-noclip @ if  0  else  fiona-floor  then ;

\ ---- her animation's root motion this frame (Motion_RootMovement / Motion_RootRotation): its
\ turn, and its step across, up, ahead ----
: root@ ( F: -- turn x y z )  her-model @ root-delta ;
: root-step ( F: -- x z )  root@ {: F: turn x y z :}  x z ;
: root-turn ( F: -- x y z )   \ (the turn taken: the step left)
    root@ {: F: turn x y z :}  her-yaw f@ turn f+ angle-wrap her-yaw f!  x y z ;
\ a move by (x, z) in the room over the floor the mask lets her on, sliding along walls
\ (Actor_Move); over her own floor
: move-within ( mask -- ) ( F: x z -- )
    her-at swap v-nav-move  dup 0< if  drop  else  her-tri !  then ;
: move-by ( F: x z -- )  blocked-floor move-within ;
\ Model_SlopeKeep: on a slope (motion flag 4) the part of a step kept, by her facing along the fall
4 constant on-slope-flag
: slope-keep ( F: -- k )
    her act.mflags l@ on-slope-flag and 0=  her-tri @ 0<  or if  1e exit  then
    her-tri @ tri-normal  {: F: nx ny nz :}
    ny 1e f< 0= if  1e exit  then
    nx nz vlen {: F: l :}  l f0= if  1e exit  then
    nx l f/ her-yaw f@ fsin f*  nz l f/ her-yaw f@ fcos f* f+ fabs
    1e ny fsq f- f*  1e fswap f- ;
: root-move ( -- )  root-turn {: F: x y z :}  x z her-yaw f@ rotate-by move-by ;   \ Character_RootMove
: root-move-within ( mask -- )  root-turn {: F: x y z :}  x z her-yaw f@ rotate-by move-within ;
: turn-to-heading ( F: step -- left )  her-yaw f@ her-heading f@ frot turn-toward fswap her-yaw f! ;

\ ---- standing, walking, running (Fiona_StateIdleMove) ----
:noname ( -- how )  settled? if  by-none  else  by-facing  then ; is settled-for-stand
: released ( group -- )   \ the stick let go
    case
        standing-group of   \ (followed, calm enough: after 3 s she rests)
              her-danger @ followed =  her-fear-bits @ 0= and  her-fear f@ 20e f< and  her-recovery @ 360 < and if
                  1 her-rest +!  her-rest @ 90 >= if  rest-anim -1 play-table  else  -1 idle-anim  then
              else  stand  then  0 her-run-t !  endof
        resting-group of  ended? if  stand  then  0 her-run-t !  endof
        walking-group of  her-still @ 6 < if  walk-look  else  stand  then  0 her-run-t !  endof
        running-group of  her-still @ 6 < if  run-look  else  0 her-rest !  0 her-run-t !  -1 idle-anim  then  endof
        >r  0 her-rest !  0 her-run-t !  -1 idle-anim  r>
    endcase ;
defer exhausted   ' noop is exhausted   \ (Fiona_Exhausted: fiona.fear)
: stick-held ( -- )   \ the stick held: walk, or run with the run button (not under story flag 0x1E)
    stick-heading her-heading f!
    no-running state-flag? 0=  her-run? @ and if
        her-fear-bits @ 1 and if   \ out of breath: running only while it lasts
            -1 her-panic-t +!  her-panic-t @ 0< 0= if  1 her-run-t +!  run-look  else  exhausted  0 her-run-t !  then
        else  1 her-run-t +!  run-look  then
    else  0 her-run-t !  walk-look  then ;
defer push-check ( F: reach -- )   ' fdrop is push-check   \ (something to push that far ahead: fiona/push.fs)
: turn-to-stick ( -- )   \ toward the stick's heading 10 degrees a frame - or, after a cut, the
    her-turn-mode @ turning-round = if   \ accelerating turn toward the old camera's heading
        her-turn-rate f@ 0.0013083e f+ 3e deg>rad fmin her-turn-rate f!
        her-yaw f@ her-turn-to f@ her-turn-rate f@ turn-toward  fswap her-yaw f!
        her-turn-rate f@ f< if  camera-relative her-turn-mode !  then
        her-yaw f@ her-heading f!
    else
        10e deg>rad turn-to-heading fdrop
    then ;
: idle-move ( -- )
    her-still @ 0= if  1e  else  her-stick-k f@ 0.05e f- 0e fmax  then  her-stick-k f!
    group
    settled? if
        her-doing @ free-doing = if
            her-fear-bits @ 2 and 0= if
                her-still @ if  dup released  else  stick-held  then
            else   \ panicking: running blindly until out of breath
                1e her-stick-k f!
                -1 her-panic-t +!  her-panic-t @ 0< 0= if
                    run-look  her-still @ 0= if  stick-heading her-heading f!  then
                else  exhausted  then
                0 her-run-t !
            then
        else   \ (out of breath: at its end free again)
            1e her-stick-k f!
            ended? if
                free-doing her-doing !
                her-fear-bits @ 2 and 0= if
                    her-still @ if  her-still @ 6 >= if  stand  then  else  run-look  then
                else  run-look  then
            then
        then
    then  drop
    root-turn {: F: rx ry rz :}
    turn-to-stick
    \ the root motion by her heading-to-be, less the further the stick points from her facing
    rz slope-keep f* {: F: ahead :}
    rx ahead her-heading f@ rotate-by  {: F: dx dz :}
    her-heading f@ her-yaw f@ f- fcos 1e f+ 2e f/ her-stick-k f@ f*  {: F: k :}
    dx k f* to dx  dz k f* to dz
    dx dz move-by
    \ moved backwards (pushed along a wall corner): she stays where she was
    her-at v.x sf@ her-was-at v.x sf@ f- dx f*  her-at v.z sf@ her-was-at v.z sf@ f- dz f* f+ f0< if
        her-was-at her-at /vec move  her-was-tri @ her-tri !
    then
    ahead push-check ;

\ ---- what she wears (the equipment): her boots (slot 0), her choker (slot 3) ----
: worn ( slot -- item )  4 * progress pr.equipped + sl@ ;
: her-boots ( -- item )  0 worn ;
: her-choker ( -- item )  3 worn ;
feather-boots constant soft-boots
\ the chokers against the panic (Fiona_AddPanic, Fiona_ResetRecovery): the jade takes a quarter
\ off each fright; the ruby a quarter off, her calming half again; the diamond half off, her
\ calming doubled
: choker-fear ( F: amount -- amount' )
    fdup f0> if
        her-choker case  jade-choker of  0.75e f*  endof  ruby-choker of  0.75e f*  endof  diamond-choker of  0.5e f*  endof  endcase
    else
        her-choker case  ruby-choker of  1.5e f*  endof  diamond-choker of  2e f*  endof  endcase
    then ;
\ her recovery's count back to 1800 frames after a fright - never with the diamond choker, half
\ the time with the ruby, three times in four with the jade
: reset-recovery ( -- )
    her-choker case
        diamond-choker of  exit  endof
        ruby-choker of  rnd 0.5e f< 0= if  exit  then  endof
        jade-choker of  rnd 0.75e f< 0= if  exit  then  endof
    endcase  1800 her-recovery ! ;

\ ---- her footsteps (Fiona_Footsteps): as a foot comes down, its floor's sound (the sound set,
\ bank 4: by the triangle's material, a variant a step), louder running; heard as a noise ----
: contact ( prev? -- l r )   \ the motion's contact track: left, right down
    her-model @ swap -4 swap motion-track 0= if  fdrop fdrop fdrop 0 0 exit  then
    fdrop  f0> 1 and  f0> 1 and ;
create foot-at /vec allot
: foot-place ( left? -- )   \ where the foot is in the room (Motion_FootPos: its place track, as she is)
    her-model @ swap 0e engine:foot-at drop  foot-at vec! ;
\ on a ladder (her mode 3): the rungs' sound ($14) - but getting on or off at its foot ($704 /
\ $703) with the foot over the floor out from it, the floor's (off there: under the foot from the
\ foot's spot); off at its top ($707) the floor's from the top's spot. -1: the floor under the foot
-2 constant rungs
create lf-at /vec allot
: from-end ( v side -- flags | -1 )   \ (Actor_TriFrom: walked straight to the foot from that end's spot)
    {: v side :}  her-ladder @ side nav-link-tri  her-ladder @ side nav-link-spot lf-at vec!  {: tri :}
    tri lf-at v 0 v-walk  dup 0< if  exit  then  nav-flags ;
: over-foot-floor? ( v -- flag )   \ (out from its foot as she gets off there: Actor_DoorFront)
    {: v :}  her-ladder @ 0  0e -7.1132e nav-link-front  dup 0< if  drop fdrop fdrop fdrop false exit  then
    {: tri :}  lf-at vec!  tri lf-at v 0 v-walk 0< 0= ;
: ladder-floor ( v -- flags | -1 | rungs )
    anim@ case
        on-ladder-top-anim of  drop settled? if  rungs  else  -1  then  endof
        rung-down-anim of  drop rungs  endof   rung-down-end-anim of  drop rungs  endof   rung-up-anim of  drop rungs  endof
        rung-up-end-anim of  drop rungs  endof   ladder-hold-anim of  drop rungs  endof   ladder-hold-2-anim of  drop rungs  endof
        off-ladder-foot-anim of  dup over-foot-floor? if  0 from-end  else  drop rungs  then  endof
        on-ladder-foot-anim of  over-foot-floor? if  -1  else  rungs  then  endof
        off-ladder-top-anim of  1 from-end  endof
        >r drop -1 r>
    endcase ;
: acoustics-id ( -- id )  s" acoustics" actor-named ;
\ the step's sound by the floor's material (its nav flags; the sound set's, bank 4 - a room's own,
\ bank 6, for some): its first of four variants, and its bank
$2018000 constant material-bits
: step-sound ( flags -- id bank )
    material-bits and case
        $2008000 of  6 sound-loaded? if  $10 6  else  $15 4  then  endof   \ (the room's own when it has them)
        $2000000 of  $10 4  endof
        $18000 of  $C 4  endof
        $10000 of  8 4  endof
        $8000 of  4 4  endof
        >r 0 4 r>
    endcase ;
$14 constant rungs-sound
: step-noise ( loud -- )  room-id her-tri @ -1 fiona-noise acoustics-id send noise ;
: running-own? ( -- flag )  her-mode @ own-mode = her-sub @ run-sub = and ;
: footsteps ( -- )
    her-tri @ 0<  world-held state-flag? or if  exit  then
    {: | left right step :}
    0 contact to right to left
    group standing-group = if  her act.fade sf@ f0> if  -1 contact to right to left  else  1 to left  1 to right  then  then
    left 1 = her-step-l @ 0= and if  1 to step  else  right 1 = her-step-r @ 0= and if  -1 to step  then  then   \ (a foot just down)
    left her-step-l !  right her-step-r !
    step 0= if  settled? group standing-group = and if  0 her-steps !  then  exit  then
    step 1 = foot-place
    wading? if  running-own?  foot-at water-step  then   \ (running: the bigger ripple)
    her-mode @ ladder-mode = if  foot-at ladder-floor  else  -1  then
    dup rungs = if  drop  rungs-sound 4 0 0 her-pos sound-at  4 step-noise  exit  then
    dup 0< if  drop  foot-at blocked-floor v-tri-in dup 0< if  drop her-tri @  then  nav-flags  then
    step-sound {: base bank :}
    base her-steps @ 3 and +  bank  1 her-steps +!                          ( id bank )
    root-step fnip 0.4e f- 0.7e f/ 0e fmax 1e fmin 2e f* f>s $7F and         ( id bank pitch )
    her-boots soft-boots = if  -48  else  0  then  swap her-pos sound-at   \ (soft boots: softer, heard less)
    running-own?  her-boots soft-boots = if  5 1  else  $14 4  then  ?  step-noise ;
