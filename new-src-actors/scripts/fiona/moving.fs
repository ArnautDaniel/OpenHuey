\ fiona/moving.fs - how Fiona moves (Fiona_StateIdleMove, the turning states, Fiona_Footsteps):
\ her animations' root motion moves her over her floor; she stands, rests, walks and runs.
IN: fiona.moving
USING: engine game-state actors messages common fiona.state fiona.model fiona.controls ;

$28020018 constant blocked-floor   \ the nav triangles she can't stand on (the game's mask)
: state-flag? ( n -- flag )   \ (the progress' state flags: a fact the story keeps)
    dup 5 rshift 4 * progress pr.state + l@  swap 31 and 1 swap lshift and 0<> ;

\ ---- her animation's root motion (Motion_RootMovement / Motion_RootRotation) ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )  her-model @ root-delta  rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;
: root-turn ( -- )  root@  her-yaw f@ rm-turn f@ f+ angle-wrap her-yaw f! ;
\ a move by (x, z) in the room over her floor, sliding along walls (Actor_Move)
: move-by ( F: x z -- )
    her-at blocked-floor v-nav-move  dup 0< if  drop  else  her-tri !  then ;
\ Model_SlopeKeep: on a slope (motion flag 4) the part of a step kept, by her facing along the fall
fvariable sk-x  fvariable sk-y  fvariable sk-z
: slope-keep ( F: -- k )
    her act.mflags l@ 4 and 0=  her-tri @ 0<  or if  1e exit  then
    her-tri @ tri-normal  sk-z f!  sk-y f!  sk-x f!
    sk-y f@ 1e f< 0= if  1e exit  then
    sk-x f@ sk-z f@ vlen fdup f0= if  fdrop 1e exit  then             ( F: l )
    sk-x f@ fover f/ her-yaw f@ fsin f*  sk-z f@ frot f/ her-yaw f@ fcos f* f+ fabs
    1e sk-y f@ fsq f- f*  1e fswap f- ;
: root-move ( -- )  root-turn  rm-x f@ rm-z f@ her-yaw f@ rotate-by move-by ;   \ Character_RootMove
: turn-to-heading ( F: step -- left )  her-yaw f@ her-heading f@ frot turn-toward fswap her-yaw f! ;

\ ---- standing, walking, running (Fiona_StateIdleMove) ----
:noname ( -- how )  settled? if  2  else  1  then ; is settled-for-stand
variable chase-mode   \ (the danger: 1 followed - F2 sets it)
: released ( group -- )   \ the stick let go
    case
        0 of  chase-mode @ 1 =  her-fear-bits @ 0= and  her-fear f@ 20e f< and  her-recovery @ 360 < and if
                  1 her-rest +!  her-rest @ 90 >= if  1 -1 play-table  else  -1 idle-anim  then
              else  stand  then  0 her-run-t !  endof
        5 of  ended? if  stand  then  0 her-run-t !  endof
        1 of  her-still @ 6 < if  walk-look  else  stand  then  0 her-run-t !  endof
        2 of  her-still @ 6 < if  run-look  else  0 her-rest !  0 her-run-t !  -1 idle-anim  then  endof
        >r  0 her-rest !  0 her-run-t !  -1 idle-anim  r>
    endcase ;
: stick-held ( -- )   \ the stick held: walk, or run with the run button (not under story flag 0x1E)
    stick-heading her-heading f!
    $1E state-flag? 0=  her-run? @ and if  1 her-run-t +!  run-look
    else  0 her-run-t !  walk-look  then ;
fvariable mv-x  fvariable mv-z  fvariable mv-dz
: idle-move ( -- )
    her-still @ 0= if  1e  else  her-stick-k f@ 0.05e f- 0e fmax  then  her-stick-k f!
    group
    settled? her-doing @ 0= and if
        her-still @ if  dup released  else  stick-held  then
    then  drop
    root-turn
    her-turn-mode @ 2 = if   \ the accelerating turn toward the old camera's heading
        her-turn-rate f@ 0.0013083e f+ 3e deg>rad fmin her-turn-rate f!
        her-yaw f@ her-turn-to f@ her-turn-rate f@ turn-toward  fswap her-yaw f!
        her-turn-rate f@ f< if  0 her-turn-mode !  then
        her-yaw f@ her-heading f!
    else
        10e deg>rad turn-to-heading fdrop
    then
    \ the root motion by her heading-to-be, less the further the stick points from her facing
    rm-z f@ slope-keep f* mv-dz f!
    rm-x f@ mv-dz f@ her-heading f@ rotate-by  mv-z f!  mv-x f!
    her-heading f@ her-yaw f@ f- fcos 1e f+ 2e f/ her-stick-k f@ f*       ( F: k )
    fdup mv-x f@ f* mv-x f!  mv-z f@ f* mv-z f!
    mv-x f@ mv-z f@ move-by
    \ moved backwards (pushed along a wall corner): she stays where she was
    her-at sf@ her-was-at sf@ f- mv-x f@ f*  her-at 8 + sf@ her-was-at 8 + sf@ f- mv-z f@ f* f+ f0< if
        her-was-at her-at 12 move  her-was-tri @ her-tri !
    then ;

\ ---- her footsteps (Fiona_Footsteps): as a foot comes down, its floor's sound (the sound set,
\ bank 4: by the triangle's material, a variant a step), louder running; heard as a noise ----
: contact ( prev? -- l r )   \ the motion's contact track: left, right down
    her-model @ swap -4 swap motion-track 0= if  fdrop fdrop fdrop 0 0 exit  then
    fdrop  f0> 1 and  f0> 1 and ;
fvariable ft-x  fvariable ft-z  create foot-at 12 allot
: foot-place ( left? -- )   \ where the foot is in the room (its place track, turned by her)
    her-model @ swap if  -3  else  -2  then  0 motion-track drop
    fswap fdrop  her-yaw f@ rotate-by  her-at 8 + sf@ f+ ft-z f!  her-at sf@ f+ ft-x f!
    ft-x f@ foot-at sf!  her-at 4 + sf@ foot-at 4 + sf!  ft-z f@ foot-at 8 + sf! ;
variable st-l  variable st-r  variable st-step  variable ft-base  variable ft-bank
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: footsteps ( -- )
    her-tri @ 0<  8 state-flag? or if  exit  then
    0 contact st-r ! st-l !
    group 0= if  her act.fade sf@ f0> if  -1 contact st-r ! st-l !  else  1 st-l !  1 st-r !  then  then
    0 st-step !
    st-l @ 1 = her-step-l @ 0= and if  1 st-step !  else  st-r @ 1 = her-step-r @ 0= and if  -1 st-step !  then  then
    st-l @ her-step-l !  st-r @ her-step-r !
    st-step @ 0= if  settled? group 0= and if  0 her-steps !  then  exit  then
    st-step @ 1 = foot-place
    0 ft-base !  4 ft-bank !
    foot-at blocked-floor v-tri-in dup 0< if  drop her-tri @  then  nav-flags
    $2018000 and case
        $2008000 of  6 sound-loaded? if  $10 ft-base !  6 ft-bank !  else  $15 ft-base !  then  endof
        $2000000 of  $10 ft-base !  endof
        $18000 of  $C ft-base !  endof
        $10000 of  8 ft-base !  endof
        $8000 of  4 ft-base !  endof
    endcase
    ft-base @ her-steps @ 3 and +  ft-bank @  1 her-steps +!               ( id bank )
    root@ rm-z f@ 0.4e f- 0.7e f/ 0e fmax 1e fmin 2e f* f>s $7F and         ( id bank pitch )
    0 swap her-pos sound-at
    her-mode @ 0= her-sub @ 2 = and if  $14  else  4  then                 ( loud )
    room-id her-tri @ -1 fiona-noise acoustics-id send noise ;
