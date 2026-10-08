\ hewie/moving.fs - how Hewie moves: his animation's root motion, his body fitted to the floor,
\ turning (his head leading), the freest heading, strides along his way, where Fiona's commands
\ put him, and where his head looks (Hewie_TurnHead).
IN: hewie.moving
USING: engine actors common paths hewie.state hewie.body hewie.model ;

\ ---- his animation's root motion this frame (Motion_RootMovement / Motion_RootRotation), the
\ stride scaled to the level part of sloped floor ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )  model root-delta  his-floor-k f@ f* rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;

\ ---- DogModel_BodyFrames: on the floor his back (bone 0) lies along it from 5 behind him to
\ him, his shoulders (bone 16) from him to 5 ahead; his stride shrinks to the level part ----
fvariable fy0  fvariable fya  fvariable fyb  create fl-at 12 allot
: floor-along ( F: d -- y )   \ the floor d along his heading
    his-yaw fswap  fl-at his-at vec-ahead  fl-at vec@ nav-floor drop ;
: fit-floor ( reach? -- )
    0e his-back-pitch f!  0e his-front-pitch f!  1e his-floor-k f!
    his-tri 0< if  drop exit  then
    his-at vec@ nav-floor 0= if  fdrop drop exit  then
    his-at 4 + sf@ fswap f- 0.01e f> if  drop exit  then   \ (off the floor)
    0= if  exit  then
    his-at 4 + sf@ fy0 f!  5e floor-along fya f!  -5e floor-along fyb f!
    fy0 f@ fyb f@ f- 5e fatan2 fnegate his-back-pitch f!
    fya f@ fy0 f@ f- 5e fatan2 fnegate his-front-pitch f!
    10e fya f@ fyb f@ f- fsq 100e f+ fsqrt f/ his-floor-k f! ;
: turn-by-anim ( -- )  root@  his-yaw rm-turn f@ f+ angle-wrap his-yaw! ;
: root-move ( -- )  root@  rm-x f@ rm-z f@ body-move-local ;     \ moved by the step, along walls
: slide-root ( -- )  root-move  1 his-no-root ! ;                \ ... and the frame's move done
: stride-len ( F: -- s )  root@ rm-z f@ ;                        \ his stride this frame
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
fvariable tw-a  fvariable tw-s
: turn-to ( F: a step -- left )
    tw-s f!  tw-a f!
    tw-a f@ his-yaw f- angle-wrap                                    ( F: d )
    fdup fabs 30e deg>rad f<  his-head-yaw f@ fover f* f0< 0=  or if
        fdrop  tw-a f@ tw-s f@ body-turn-toward exit
    then
    f0< if  his-yaw tw-s f@ f+  else  his-yaw tw-s f@ f-  then  angle-wrap his-yaw!
    tw-a f@ his-yaw f- angle-wrap fabs ;
\ his head held level toward heading a (look mode 8: now, or in 10 frames)
: head-toward ( F: a -- )  8 look-now!  0e his-look-pitch f!  his-yaw f- angle-wrap his-look-yaw f! ;
: head-toward-soon ( F: a -- )  8 look!  0e his-look-pitch f!  his-yaw f- angle-wrap his-look-yaw f! ;

\ ---- Hewie_BestHeading: the heading near `yaw` freest for `dist` - tried from `from` to `to`
\ degrees off by `step`, either side first at random; the first quite free wins ----
variable bh-from  variable bh-to  variable bh-step
fvariable bh-yaw  fvariable bh-dist  fvariable bh-best  fvariable bh-res  fvariable bh-sign
: bh-try ( deg -- done? )
    s>f deg>rad bh-sign f@ f* bh-yaw f@ f+ angle-wrap              ( F: a )
    fdup bh-dist f@ free-ahead                                     ( F: a r )
    fdup bh-best f@ f> if  bh-best f!  bh-res f!  bh-best f@ bh-dist f@ f= exit  then
    fdrop fdrop false ;
: bh-flip ( -- )  bh-sign f@ fnegate bh-sign f! ;
: bh-more? ( deg -- deg flag )  dup  bh-from @ bh-to @ < if  bh-to @ <=  else  bh-to @ >=  then ;
: best-heading ( from to step -- ) ( F: yaw dist -- yaw' )
    bh-step !  bh-to !  bh-from !  bh-dist f!  fdup bh-yaw f!  bh-res f!
    bh-yaw f@ bh-dist f@ free-ahead fdup bh-best f!  bh-dist f@ f= if  bh-res f@ exit  then
    rnd 0.5e f< if  1e  else  -1e  then  bh-sign f!
    bh-from @
    begin  bh-more? while
        dup bh-try if  drop bh-res f@ exit  then  bh-flip
        dup bh-try if  drop bh-res f@ exit  then  bh-flip
        bh-from @ bh-to @ < if  bh-step @ +  else  bh-step @ -  then
    repeat  drop bh-res f@ ;

\ ---- Hewie_Stride: one stride along his way: turned toward the way 12 strides on; facing
\ within ~41 degrees the way to the stride's point, stepped there ----
create sd-at 12 allot  create sd-ahead 12 allot  fvariable sd-yaw  fvariable sd-rate  variable sd-next
: stride ( -- stepped? )
    stride-len 0e fmax                                               ( F: s )
    fdup 12e f* ahead@ drop  sd-ahead ahead vec-copy
    sd-ahead heading-to sd-yaw f!
    fdup ahead@ sd-next !  sd-at ahead vec-copy
    his-head-yaw f@ fabs 0.5e f* 12e f* f*  deg>rad sd-rate f!
    sd-yaw f@ his-yaw f- angle-wrap                                  ( F: off )
    fdup fabs 30e deg>rad f<  his-head-yaw f@ fover f* f0< 0=  or if
        fdrop  sd-yaw f@ sd-rate f@ body-turn-toward fdrop
    else
        f0< if  his-yaw sd-rate f@ f+  else  his-yaw sd-rate f@ f-  then  angle-wrap his-yaw!
    then
    sd-yaw f@ head-toward
    sd-at heading-to his-yaw f- fcos 0.75e f>  his-at sd-at vec-dist-xz f0= 0= and if
        sd-at vec@ his-place  sd-next @ his-path path-i!  1 his-no-root !  true exit
    then
    slide-root  path-stop  false ;
\ the last stretch to v, straight (run_straight): turning to it as he runs, head on it, by his
\ root motion unless he already moved
: aim-run ( v -- )  heading-to  fdup run-turn turn-to fdrop  head-toward ;
: run-straight ( v -- )  aim-run  his-no-root @ 0= if  slide-root  path-stop  then ;

\ ---- Hewie_CommandPlace: where Fiona's command puts him, into cp-pos; its triangle ----
create cp-pos 12 allot  create cp-try 12 allot
: free-at? ( v -- tri | -1 )   \ on the floor, not blocked for him
    v-tri dup 0< if  exit  then  dup nav-flags self body-mask and if  drop -1  then ;
: her-spot ( -- tri )  cp-pos fiona-at vec-copy  her body-tri ;
fvariable bs-x  fvariable bs-z
: at-her-side ( F: x -- )   \ cp-try = her place + x along his right
    fdup his-yaw fcos f* bs-x f!  his-yaw fsin fnegate f* bs-z f!
    fiona-at vec@  bs-z f@ f+  frot bs-x f@ f+  frot frot  cp-try vec! ;
: command-place ( act -- tri )
    her-with? 0= if  drop  cp-pos his-at vec-copy  his-tri exit  then
    $64 = if   \ 15 in front of the spot she showed, facing her way
        his-to vec@  his-to-yaw f@ fsin 15e f* bs-x f!  his-to-yaw f@ fcos 15e f* bs-z f!
        bs-z f@ f+  frot bs-x f@ f+  frot frot  cp-try vec!
        cp-try free-at? dup 0< if  drop her-spot exit  then
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
fvariable fs-up  fvariable fs-down  variable fs-there
: gait ( F: d -- ) ( -- there? )
    his-t1 @ case
        0 of  fdup 12e f< if  fdrop path-stop true exit  then
              30e f<= if  7 keep-pose  else  1 his-t1 !  8 keep-pose  then  endof
        1 of  fdup 20e f< if  fdrop 0 his-t1 !  7 keep-pose
              else  fs-up f@ f<= if  8 keep-pose  else  2 his-t1 !  9 keep-pose  then  then  endof
        2 of  fs-down f@ f< if  1 his-t1 !  8 keep-pose  else  9 keep-pose  then  endof
        >r fdrop r>
    endcase  false ;
: follow-step ( F: up down -- ) ( -- -1 | 0 | 1 )
    fs-down f!  fs-up f!
    his-action @ command-place                                     ( tri )
    dup cp-pos -1 body-tri-to = dup fs-there !
    0= path-done? and if
        cp-pos plan-and-go if  to-default -1 exit  then
    else  drop  then
    fs-there @ if  his-at cp-pos vec-dist  else  rest  then
    gait if  1 exit  then
    fs-there @ 0= if
        stride 0= his-t1 @ 2 = and if  8 keep-pose  then  0 exit
    then
    cp-pos aim-run  his-no-root @ 0= if  path-stop  then  0 ;

\ ---- where his head looks (Hewie_TurnHead) ----
\ Motion_LookAt from his eyes (the model's +0x860: 5.6 up, 3 ahead): the pitch, and the turn
\ from his heading
create eye-at 12 allot
: look-at ( v -- ) ( F: -- pitch turn )
    his-yaw 3e eye-at his-at vec-ahead  eye-at 4 + dup sf@ 5.6e f+ sf!
    dup 4 + sf@  eye-at 4 + sf@ f-                                  ( F: rise )
    dup eye-at swap vec-dist-xz  fatan2
    eye-at swap vec-heading his-yaw f- angle-wrap ;
: char-look ( id -- ) ( F: -- pitch turn )   \ at someone's head (about 11 up)
    body-at cp-try swap vec-copy  cp-try 4 + dup sf@ 11e f+ sf!  cp-try look-at ;
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
        0 of  his-target @ dup with? if  char-look  else  drop 4 his-look-now ! 0e 0e  then  endof
        5 of  his-target @ dup with? if  char-look fswap fdrop 0e fswap  else  drop 4 his-look-now ! 0e 0e  then  endof
        8 of  his-look-pitch f@ his-look-yaw f@  endof
        1 of  90 glance  his-look-pitch f@ his-look-yaw f@  endof
        2 of  60 glance  his-look-pitch f@ his-look-yaw f@  endof
        3 of  his-look-t @ if  -1 his-look-t +!  else
                  128 roll 20 + his-look-t !
                  rnd -0.2e f* pi f* his-look-pitch f!  rnd 0.2e f* 0.1e f- pi f* his-look-yaw f!
              then  his-look-pitch f@ his-look-yaw f@  endof
        6 of  0e pi f2/ fnegate  endof
        7 of  0e pi f2/  endof
        9 of  0e pi 0.75e f* fnegate  endof
        10 of  0e pi 0.75e f*  endof
        11 of  0.9424779e 0e  endof
        12 of  0.3e -1.2566371e  endof
        13 of  0.3e 1.2566371e  endof
        >r 0e 0e r>
    endcase ;
fvariable hd-p  fvariable hd-y  fvariable hd-s
: turn-head ( -- )
    his-cond @ 2 = if  0e 0e
    else his-look-char @ 0< 0= if
        his-look-char @ dup with? if  char-look  else  drop -1 his-look-char !  0e 0e  then
    else his-look-pt? @ if  his-look-pt look-at
    else  look-mode  then then then                               ( F: pitch turn )
    -2.8274333e fmax 2.8274333e fmin  hd-y f!
    -0.7853982e fmax 2.3561945e fmin  hd-p f!
    \ eased 0.15 of the way (the turn faster by how far his heading changed)
    hd-p f@ his-head-pitch f@ f- 0.15e f*  his-head-pitch f@ f+ his-head-pitch f!
    hd-y f@ his-head-yaw f@ f-  fdup fabs 0.15e f*
    his-look-now @ dup 8 = over 5 = or swap 0= or if  his-yaw his-yaw-was f@ f- angle-wrap fabs f+  then
    hd-s f!                                                         ( F: dy )
    fdup fabs hd-s f@ f<= if  fdrop hd-y f@ his-head-yaw f!  exit  then
    f0< if  hd-s f@ fnegate  else  hd-s f@  then  his-head-yaw f@ f+ his-head-yaw f! ;
