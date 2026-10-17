\ hewie/moving.fs - how Hewie moves: his animation's root motion, his body fitted to the floor,
\ turning (his head leading), the freest heading, strides along his way, where Fiona's commands
\ put him, and where his head looks (Hewie_TurnHead).
IN: hewie.moving
USING: engine actors common paths vectors messages hewie-names hewie.state hewie.body hewie.model ;

\ ---- his animation's root motion this frame (Motion_RootMovement / Motion_RootRotation): its
\ turn, and its step across, up, along - the stride scaled to the level part of sloped floor ----
: root@ ( F: -- turn x y z )  model root-delta  his-floor-k f@ f* ;

\ ---- DogModel_BodyFrames: on the floor his back (bone 0) lies along it from 5 behind him to
\ him, his shoulders (bone 16) from him to 5 ahead; his stride shrinks to the level part ----
create fl-at /vec allot
: floor-along ( F: d -- y )   \ the floor d along his heading
    his-yaw fswap  fl-at his-at vec-ahead  fl-at vec@ nav-floor drop ;
: fit-floor ( reach? -- )
    0e his-back-pitch f!  0e his-front-pitch f!  1e his-floor-k f!
    his-tri 0< if  drop exit  then
    his-at vec@ nav-floor 0= if  fdrop drop exit  then                ( F: floor )
    his-at v.y sf@ fswap f- 0.01e f> if  drop exit  then   \ (off the floor)
    0= if  exit  then
    his-at v.y sf@  5e floor-along  -5e floor-along  {: F: y ahead behind :}
    y behind f- 5e fatan2 fnegate his-back-pitch f!
    ahead y f- 5e fatan2 fnegate his-front-pitch f!
    10e  ahead behind f- fsq 100e f+ fsqrt  f/ his-floor-k f! ;
: turn-by-anim ( -- )  root@ {: F: turn x y z :}  his-yaw turn f+ angle-wrap his-yaw! ;
: root-move ( -- )  root@ {: F: turn x y z :}  x z body-move-local ;     \ moved by the step, along walls
: slide-root ( -- )  root-move  1 his-no-root ! ;                        \ ... and the frame's move done
: stride-len ( F: -- s )  root@ {: F: turn x y z :}  z ;                 \ his stride this frame
\ where his head is (bone 0x1F as last drawn; not drawn: 5 ahead of him)
: head-at ( v -- )
    him-model act.visible l@ 0= if  his-yaw 5e his-at vec-ahead exit  then
    model $1F bone-pos vec! ;

\ run_turn: how fast he turns as he runs - by his stride and how far his head is turned
: run-turn ( F: -- step )
    stride-len fdup f0< if  fdrop 0e exit  then
    his-head-yaw f@ fabs 0.5e f* 12e f* f*  deg>rad ;
\ toward heading a by step (how far is left); more than 30 degrees off with his head turned the
\ other way, his body steps round the other way to follow it
: turn-to ( F: a step -- left )
    {: F: a step :}  a his-yaw f- angle-wrap {: F: d :}
    d fabs 30e deg>rad f<  his-head-yaw f@ d f* f0< 0= or if  a step body-turn-toward exit  then
    d f0< if  his-yaw step f+  else  his-yaw step f-  then  angle-wrap his-yaw!
    a his-yaw f- angle-wrap fabs ;
\ his head held level toward heading a (now, or in 10 frames)
: head-toward ( F: a -- )  held-look look-now!  0e his-look-pitch f!  his-yaw f- angle-wrap his-look-yaw f! ;
: head-toward-soon ( F: a -- )  held-look look!  0e his-look-pitch f!  his-yaw f- angle-wrap his-look-yaw f! ;

\ ---- Hewie_BestHeading: the heading near `yaw` freest for `dist` - tried from `first` to `last`
\ degrees off by `step`, either side first at random; the first quite free wins ----
: best-heading ( first last step -- ) ( F: yaw dist -- yaw' )
    {: first last step F: yaw dist | F: best res sign :}
    yaw to res  yaw dist free-ahead to best
    best dist f= if  res exit  then
    rnd 0.5e f< if  1e  else  -1e  then  to sign
    first
    begin  dup  first last < if  last <=  else  last >=  then  while
        2 0 do   \ (one side, then the other)
            dup s>f deg>rad sign f* yaw f+ angle-wrap  fdup dist free-ahead      ( F: a free )
            fdup best f> if
                to best  to res  best dist f= if  drop res unloop exit  then
            else  fdrop fdrop  then
            sign fnegate to sign
        loop
        first last < if  step +  else  step -  then
    repeat  drop res ;

\ ---- Hewie_Stride: one stride along his way: turned toward the way 12 strides on; facing
\ within ~41 degrees the way to the stride's point, stepped there ----
create sd-at /vec allot  create sd-ahead /vec allot
: stride ( -- stepped? )
    stride-len 0e fmax {: F: s :}
    s 12e f* ahead@ drop  sd-ahead ahead vec-copy
    sd-ahead heading-to {: F: way :}
    s ahead@ {: next :}  sd-at ahead vec-copy
    his-head-yaw f@ fabs 0.5e f* 12e f* s f* deg>rad {: F: rate :}
    way his-yaw f- angle-wrap {: F: off :}
    off fabs 30e deg>rad f<  his-head-yaw f@ off f* f0< 0= or if
        way rate body-turn-toward fdrop
    else
        off f0< if  his-yaw rate f+  else  his-yaw rate f-  then  angle-wrap his-yaw!
    then
    way head-toward
    sd-at heading-to his-yaw f- fcos 0.75e f>  his-at sd-at vec-dist-xz f0= 0= and if
        sd-at vec@ his-place  next his-path path-i!  1 his-no-root !  true exit
    then
    slide-root  path-stop  false ;
\ the last stretch to v, straight (run_straight): turning to it as he runs, head on it, by his
\ root motion unless he already moved
: aim-run ( v -- )  heading-to  fdup run-turn turn-to fdrop  head-toward ;
: run-straight ( v -- )  aim-run  his-no-root @ 0= if  slide-root  path-stop  then ;

\ ---- Hewie_CommandPlace: where Fiona's command puts him, into cp-pos; its triangle ----
create cp-pos /vec allot  create cp-try /vec allot
: free-at? ( v -- tri | -1 )   \ on the floor, not blocked for him
    v-tri dup 0< if  exit  then  dup nav-flags self body-mask and if  drop -1  then ;
: her-spot ( -- tri )  cp-pos fiona-at vec-copy  her body-tri ;
: at-her-side ( F: x -- )   \ cp-try = her place + x along his right
    {: F: x :}  fiona-at vec@ {: F: fx fy fz :}
    fx  x his-yaw fcos f* f+  fy  fz  x his-yaw fsin fnegate f* f+  cp-try vec! ;
: before-spot ( -- )   \ cp-try = 15 in front of the spot she showed, along her heading then
    his-to vec@ {: F: tx ty tz :}
    tx  his-to-yaw f@ fsin 15e f* f+  ty  tz  his-to-yaw f@ fcos 15e f* f+  cp-try vec! ;
: command-place ( act -- tri )
    her-with? 0= if  drop  cp-pos his-at vec-copy  his-tri exit  then
    scramble-there-act = if   \ 15 in front of the spot she showed, facing her way
        before-spot  cp-try free-at? dup 0< if  drop her-spot exit  then
        cp-pos cp-try vec-copy exit
    then
    \ beside her: drifting out to 5 on one side, else 5 on the other
    his-side f@ fdup fabs 5e f< if
        his-side-dir @ 1 and if  0.3e f+  else  0.3e f-  then
    then  fdup his-side f!
    at-her-side  cp-try free-at? dup 0< 0= if  cp-pos cp-try vec-copy exit  then  drop
    his-side f@ 0e f> if  -5e  else  5e  then  at-her-side
    cp-try free-at? dup 0< if  drop her-spot exit  then
    his-side f@ f0< if  1  else  0  then  his-side-dir !  0e his-side f!
    cp-pos cp-try vec-copy ;

\ ---- follow_step: a step toward where her command puts him, the gait (his-t1: 0 walk, 1 trot,
\ 2 run) kept by the distance left with some give; -1 no way (the default action taken), 1
\ there (walking within 12) ----
: gait ( F: d up down -- ) ( -- there? )
    {: F: d up down :}
    his-t1 @ case
        0 of  d 12e f< if  path-stop true exit  then
              d 30e f<= if  walk-pose keep-pose  else  1 his-t1 !  trot-pose keep-pose  then  endof
        1 of  d 20e f< if  0 his-t1 !  walk-pose keep-pose
              else  d up f<= if  trot-pose keep-pose  else  2 his-t1 !  run-pose keep-pose  then  then  endof
        2 of  d down f< if  1 his-t1 !  trot-pose keep-pose  else  run-pose keep-pose  then  endof
    endcase  false ;
: follow-step ( F: up down -- ) ( -- -1 | 0 | 1 )
    {: F: up down :}
    his-action @ command-place {: tri :}
    cp-pos -1 body-tri-to tri = {: there :}   \ (on its triangle, straight from him)
    there 0= path-done? and if  tri cp-pos plan-and-go if  to-default -1 exit  then  then
    there if  his-at cp-pos vec-dist  else  rest  then  up down gait if  1 exit  then
    there 0= if  stride 0= his-t1 @ 2 = and if  trot-pose keep-pose  then  0 exit  then
    cp-pos aim-run  his-no-root @ 0= if  path-stop  then  0 ;

\ ---- where his head looks (Hewie_TurnHead) ----
\ Motion_LookAt from his eyes (the model's +0x860: 5.6 up, 3 ahead): the pitch, and the turn
\ from his heading
create eye-at /vec allot
: look-at ( v -- ) ( F: -- pitch turn )
    {: v :}  his-yaw 3e eye-at his-at vec-ahead  eye-at v.y sf@ 5.6e f+ eye-at v.y sf!
    v v.y sf@ eye-at v.y sf@ f-  eye-at v vec-dist-xz fatan2
    eye-at v vec-heading his-yaw f- angle-wrap ;
: char-look ( id -- ) ( F: -- pitch turn )   \ at someone's head (about 11 up)
    body-at cp-try swap vec-copy  cp-try v.y sf@ 11e f+ cp-try v.y sf!  cp-try look-at ;
: glance ( n -- )   \ looking about at random: a new look every 20 .. 20+n frames
    his-look-t @ if  -1 his-look-t +!  drop exit  then
    roll 20 + his-look-t !
    rnd 0.1e f* 0.05e f- pi f* his-look-pitch f!
    rnd 0.8e f* 0.3e f- pi f*  his-head-yaw f@ f0< if  his-head-yaw f@ f+  else  fnegate his-head-yaw f@ f+  then
    his-look-yaw f! ;
: look-mode ( -- ) ( F: -- pitch turn )
    his-look-delay @ 0= his-look-now @ his-look-to @ <> and if
        his-look-to @ his-look-now !  0 his-look-t !
    else  his-look-delay @ if  -1 his-look-delay +!  then  then
    his-look-now @ case
        target-look of  his-target @ dup with? if  char-look  else  drop ahead-look his-look-now !  0e 0e  then  endof
        target-level-look of
            his-target @ dup with? if  char-look fswap fdrop 0e fswap  else  drop ahead-look his-look-now !  0e 0e  then  endof
        held-look of  his-look-pitch f@ his-look-yaw f@  endof
        slow-glance-look of  90 glance  his-look-pitch f@ his-look-yaw f@  endof
        glance-look of  60 glance  his-look-pitch f@ his-look-yaw f@  endof
        about-look of
            his-look-t @ if  -1 his-look-t +!  else
                128 roll 20 + his-look-t !
                rnd -0.2e f* pi f* his-look-pitch f!  rnd 0.2e f* 0.1e f- pi f* his-look-yaw f!
            then  his-look-pitch f@ his-look-yaw f@  endof
        left-look of  0e pi f2/ fnegate  endof
        right-look of  0e pi f2/  endof
        back-left-look of  0e pi 0.75e f* fnegate  endof
        back-right-look of  0e pi 0.75e f*  endof
        low-look of  0.9424779e 0e  endof                  \ (54 degrees)
        aside-left-look of  0.3e -1.2566371e  endof        \ (72 degrees)
        aside-right-look of  0.3e 1.2566371e  endof
        >r 0e 0e r>
    endcase ;
: turn-head ( -- )
    his-cond @ down-cond = if  0e 0e
    else his-look-char @ 0< 0= if
        his-look-char @ dup with? if  char-look  else  drop -1 his-look-char !  0e 0e  then
    else his-look-pt? @ if  his-look-pt look-at
    else  look-mode  then then then                               ( F: pitch turn )
    -2.8274333e fmax 2.8274333e fmin {: F: yaw :}                 \ (162 degrees)
    -0.7853982e fmax 2.3561945e fmin {: F: pitch :}               \ (-45 .. 135)
    \ eased 0.15 of the way (the turn faster by how far his heading changed)
    pitch his-head-pitch f@ f- 0.15e f*  his-head-pitch f@ f+ his-head-pitch f!
    yaw his-head-yaw f@ f- {: F: dy :}
    dy fabs 0.15e f*
    his-look-now @ dup held-look = over target-level-look = or swap target-look = or if
        his-yaw his-yaw-was f@ f- angle-wrap fabs f+
    then  {: F: step :}
    dy fabs step f<= if  yaw his-head-yaw f!  exit  then
    dy f0< if  step fnegate  else  step  then  his-head-yaw f@ f+ his-head-yaw f! ;
