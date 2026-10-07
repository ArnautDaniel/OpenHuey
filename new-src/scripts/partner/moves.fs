\ partner/moves.fs - how Hewie moves (src/game/hewie.c): his animation's root motion, turning
\ (with his head leading), the freest heading, strides along a planned way, where Fiona's
\ commands put him, and where his head looks (Hewie_TurnHead).
IN: partner.moves
USING: engine game-state events.core chars partner.core ;

\ ---- his animation's root motion this frame (Motion_RootMovement / Motion_RootRotation; the
\ model's ground speed factor, motion +0x48, taken as 1) ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
\ his stride's share on sloped floor (DogModel_FloorLevel: the level part of the way from the
\ floor 5 behind him to the floor 5 ahead; the motion's +0x48)
fvariable floor-k  1e floor-k f!
: root@ ( -- )
    h-actor dup 0< if  drop 0e rm-turn f!  0e rm-x f!  0e rm-y f!  0e rm-z f!  exit  then
    root-delta  floor-k f@ f* rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;

\ ---- DogModel_BodyFrames: on the floor his back (bone 0) lies along it from 5 behind him to
\ him, his shoulders (bone 16) from him to 5 ahead ----
fvariable back-pitch  fvariable front-pitch  fvariable fy0  fvariable fya  fvariable fyb
create fl-at 12 allot
: floor-along ( F: d -- y )   \ the walk mesh's floor d along his heading
    h-yaw fswap  fl-at h-pos vec-ahead  fl-at vec@ nav-floor drop ;
: fit-floor ( reach? -- )
    0e back-pitch f!  0e front-pitch f!  1e floor-k f!
    h-actor 0< h-tri 0< or if  drop exit  then
    h-pos vec@ nav-floor 0= if  fdrop drop exit  then
    h-pos 4 + sf@ fswap f- 0.01e f> if  drop exit  then   \ (off the floor)
    0= if  exit  then
    h-pos 4 + sf@ fy0 f!  5e floor-along fya f!  -5e floor-along fyb f!
    fy0 f@ fyb f@ f- 5e fatan2 fnegate back-pitch f!
    fya f@ fy0 f@ f- 5e fatan2 fnegate front-pitch f!
    10e fya f@ fyb f@ f- fsq 100e f+ fsqrt f/ floor-k f! ;
\ turn_by_anim: the animation's turn added to his heading
: turn-by-anim ( -- )  root@  h-yaw rm-turn f@ f+ angle-wrap h-yaw! ;
\ root_motion / slide_root: moved by the step (level), sliding along walls; the frame's move done
: slide-root ( -- )  root@  him rm-x f@ rm-z f@ c-move-local  1 h-no-root ! ;
\ where his head is (bone 0x1F as last drawn; not drawn: 5 ahead of him)
: head-at ( v -- )
    h-actor dup 0< if  drop  h-pos h-yaw 5e vec-ahead exit  then
    dup actor act.visible l@ 0= if  drop  h-pos h-yaw 5e vec-ahead exit  then
    $1F bone-pos vec! ;
\ his stride this frame: the root motion's forward step
: stride-len ( F: -- s )  root@ rm-z f@ ;

\ run_turn: how fast he turns as he runs - by his stride and how far his head is turned
: run-turn ( F: -- step )
    stride-len fdup f0< if  fdrop 0e exit  then
    h-head-yaw f@ fabs 0.5e f* 12e f* f*  deg>rad ;

\ turn_toward: toward heading a by step (how far is left); more than 30 degrees off with his
\ head turned the other way, his body steps round the other way to follow it
fvariable tw-a  fvariable tw-s
: turn-to ( F: a step -- left )
    tw-s f!  tw-a f!
    tw-a f@ h-yaw f- angle-wrap                                  ( F: d )
    fdup fabs 30e deg>rad f<  h-head-yaw f@ fover f* f0< 0=  or if
        fdrop  him tw-a f@ tw-s f@ c-turn-toward exit
    then
    f0< if  h-yaw tw-s f@ f+  else  h-yaw tw-s f@ f-  then  angle-wrap h-yaw!
    tw-a f@ h-yaw f- angle-wrap fabs ;

\ his head held level toward heading a (look mode 8 at once)
: head-toward ( F: a -- )  8 look-now!  0e h-look-pitch f!  h-yaw f- angle-wrap h-look-yaw f! ;
\ the same, 10 frames to take it up
: head-toward-soon ( F: a -- )  8 look!  0e h-look-pitch f!  h-yaw f- angle-wrap h-look-yaw f! ;

\ ---- Hewie_BestHeading: the heading near `yaw` freest for `dist` - tried from `from` to `to`
\ degrees off by `step`, either side first at random; the first quite free wins ----
variable bh-from  variable bh-to  variable bh-step
fvariable bh-yaw  fvariable bh-dist  fvariable bh-best  fvariable bh-res  fvariable bh-sign
: free-ahead ( F: yaw d -- free )  him -1 c-free ;
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
    rnd01 0.5e f< if  1e  else  -1e  then  bh-sign f!
    bh-from @
    begin  bh-more? while
        dup bh-try if  drop bh-res f@ exit  then  bh-flip
        dup bh-try if  drop bh-res f@ exit  then  bh-flip
        bh-from @ bh-to @ < if  bh-step @ +  else  bh-step @ -  then
    repeat  drop bh-res f@ ;

\ ---- his planned way (Hewie_PlanTo / Hewie_PlanAndGo) ----
\ triangles on either side of a room's divider (flags 0x100000 / 0x200000)
: level-of ( tri -- bits )  nav-flags $300000 and ;
: across? ( tri -- flag )
    level-of h-tri level-of
    2dup $100000 = swap $200000 = and >r  $200000 = swap $100000 = and  r> or ;
\ a way to v on `tri` (not across the divider): true if there is one
: plan-to ( tri v -- flag )
    over across? if  2drop false exit  then
    him -rot -1 c-plan 0> ;
\ the same, the way to be followed: 0, -1 none
: plan-and-go ( tri v -- 0 | -1 )  plan-to if  0  else  -1  then ;
: path-done? ( -- flag )  him path-left? 0= ;
: path-end ( -- )  him c-path-end ;
: rest ( F: -- d )  him path-rest ;

\ ---- Hewie_Stride: one stride along his way ----
create sd-at 12 allot  create sd-ahead 12 allot  fvariable sd-yaw  fvariable sd-rate  variable sd-next
: stride ( -- stepped? )
    stride-len 0e fmax                                               ( F: s )
    fdup 12e f* him path-ahead drop  sd-ahead pa-pos vec-copy
    him sd-ahead c-heading-to sd-yaw f!
    fdup him path-ahead sd-next !  sd-at pa-pos vec-copy
    h-head-yaw f@ fabs 0.5e f* 12e f* f*  deg>rad sd-rate f!
    sd-yaw f@ h-yaw f- angle-wrap                                    ( F: off )
    fdup fabs 30e deg>rad f<  h-head-yaw f@ fover f* f0< 0=  or if
        fdrop  him sd-yaw f@ sd-rate f@ c-turn-toward fdrop
    else
        f0< if  h-yaw sd-rate f@ f+  else  h-yaw sd-rate f@ f-  then  angle-wrap h-yaw!
    then
    sd-yaw f@ head-toward
    \ facing within ~41 degrees the way to the stride's point: stepped there
    him sd-at c-heading-to h-yaw f- fcos 0.75e f>  him c-pos sd-at vec-dist-xz f0= 0= and if
        sd-at vec@ him c-place!  sd-next @ him path-i!  1 h-no-root !  true exit
    then
    slide-root  path-end  false ;

\ the last stretch to v, straight (run_straight): turning to it as he runs, head on it, by his
\ root motion unless he already moved
: aim-run ( v -- )  him swap c-heading-to  fdup run-turn turn-to fdrop  head-toward ;
: run-straight ( v -- )  aim-run  h-no-root @ 0= if  slide-root  path-end  then ;

\ ---- where Fiona's command puts him (Hewie_CommandPlace): into cp-pos, its triangle ----
create cp-pos 12 allot  create cp-try 12 allot
\ free_at: the point is on the mesh where she is, not blocked for him
: free-at? ( v -- tri | -1 )
    v-tri dup 0< if  exit  then  dup nav-flags him c-mask and if  drop -1  then ;
: her-spot ( -- tri )  cp-pos her c-pos vec-copy  her c-tri ;
fvariable bs-x  fvariable bs-z
: at-her-side ( F: x -- )   \ cp-try = her place + x along his right
    fdup h-yaw fcos f* bs-x f!  h-yaw fsin fnegate f* bs-z f!
    her c-pos vec@  bs-z f@ f+  frot bs-x f@ f+  frot frot  cp-try vec! ;
: command-place ( cmd -- tri )
    her with? 0= if  drop  cp-pos h-pos vec-copy  h-tri exit  then
    $64 = if   \ 15 in front of the spot she showed, facing her way
        h-to vec@  h-to-yaw f@ fsin 15e f* bs-x f!  h-to-yaw f@ fcos 15e f* bs-z f!
        bs-z f@ f+  frot bs-x f@ f+  frot frot  cp-try vec!
        cp-try free-at? dup 0< if  drop her-spot exit  then
        cp-pos cp-try vec-copy exit
    then
    \ beside her: drifting out to 5 on one side, else 5 on the other
    h-side f@ fdup fabs 5e f< if
        h-side-dir @ 1 and if  0.3e f+  else  0.3e f-  then
    then  fdup h-side f!
    at-her-side  cp-try free-at? dup 0< 0= if  cp-pos cp-try vec-copy exit  then  drop
    h-side f@ 0e f> if  -5e  else  5e  then  at-her-side
    cp-try free-at? dup 0< if  drop her-spot exit  then
    h-side f@ f0< if  1  else  0  then  h-side-dir !  0e h-side f!
    cp-pos cp-try vec-copy ;

\ ---- follow_step: one step toward where her command puts him, the gait (h-t1: 0 walk, 1 trot,
\ 2 run) kept by the distance left with some give; -1 no way (the default action taken), 1
\ there (walking within 12) ----
fvariable fs-up  fvariable fs-down  variable fs-there
: gait ( F: d -- ) ( -- there? )
    h-t1 @ case
        0 of  fdup 12e f< if  fdrop path-end true exit  then
              30e f<= if  7 keep-pose  else  1 h-t1 !  8 keep-pose  then  endof
        1 of  fdup 20e f< if  fdrop 0 h-t1 !  7 keep-pose
              else  fs-up f@ f<= if  8 keep-pose  else  2 h-t1 !  9 keep-pose  then  then  endof
        2 of  fs-down f@ f< if  1 h-t1 !  8 keep-pose  else  9 keep-pose  then  endof
        >r fdrop r>
    endcase  false ;
: follow-step ( F: up down -- ) ( -- -1 | 0 | 1 )
    fs-down f!  fs-up f!
    h-action @ command-place                                       ( tri )
    dup him cp-pos -1 c-tri-to = dup fs-there !
    0= path-done? and if
        cp-pos plan-and-go if  to-default -1 exit  then
    else  drop  then
    fs-there @ if  h-pos cp-pos vec-dist  else  rest  then
    gait if  1 exit  then
    fs-there @ 0= if
        stride 0= h-t1 @ 2 = and if  8 keep-pose  then  0 exit
    then
    cp-pos aim-run  h-no-root @ 0= if  path-end  then  0 ;

\ ---- where his head looks (Hewie_TurnHead) ----
\ Motion_LookAt from his eyes (about 7 above him): the pitch and the turn from his heading
create eye-at 12 allot
: look-at ( v -- ) ( F: -- pitch turn )   \ from his eyes (the model's +0x860: 5.6 up, 3 ahead)
    eye-at h-pos h-yaw 3e vec-ahead  eye-at 4 + dup sf@ 5.6e f+ sf!
    dup 4 + sf@  eye-at 4 + sf@ f-                                  ( F: rise )
    dup eye-at swap vec-dist-xz  fatan2
    eye-at swap vec-heading h-yaw f- angle-wrap ;
: char-look ( cs -- ) ( F: -- pitch turn )   \ at a character's head (about 11 up)
    c-pos cp-try swap vec-copy  cp-try 4 + dup sf@ 11e f+ sf!  cp-try look-at ;
fvariable hd-p  fvariable hd-y  fvariable hd-s
: glance ( n -- )   \ looking about at random: a new look every 20 .. 20+n frames
    h-look-t @ if  -1 h-look-t +!  drop exit  then
    s>f rnd01 f* f>s 20 + h-look-t !
    rnd01 0.1e f* 0.05e f- pi f* h-look-pitch f!
    rnd01 0.8e f* 0.3e f- pi f*  h-head-yaw f@ f0< if  h-head-yaw f@ f+  else  fnegate h-head-yaw f@ f+  then
    h-look-yaw f! ;
: look-mode ( -- ) ( F: -- pitch turn )
    h-look-delay @ 0= h-look-now @ h-look-to @ <> and if
        h-look-to @ h-look-now !  0 h-look-t !
    else  h-look-delay @ if  -1 h-look-delay +!  then  then
    h-look-now @ case
        0 of  h-target @ dup with? if  char-look  else  drop 4 h-look-now ! 0e 0e  then  endof
        5 of  h-target @ dup with? if  char-look fswap fdrop 0e fswap  else  drop 4 h-look-now ! 0e 0e  then  endof
        8 of  h-look-pitch f@ h-look-yaw f@  endof
        1 of  90 glance  h-look-pitch f@ h-look-yaw f@  endof
        2 of  60 glance  h-look-pitch f@ h-look-yaw f@  endof
        3 of  h-look-t @ if  -1 h-look-t +!  else
                  128 s>f rnd01 f* f>s 20 + h-look-t !
                  rnd01 -0.2e f* pi f* h-look-pitch f!  rnd01 0.2e f* 0.1e f- pi f* h-look-yaw f!
              then  h-look-pitch f@ h-look-yaw f@  endof
        6 of  0e pi f2/ fnegate  endof
        7 of  0e pi f2/  endof
        9 of  0e pi 0.75e f* fnegate  endof
        10 of  0e pi 0.75e f*  endof
        11 of  0.9424779e 0e  endof
        12 of  0.3e -1.2566371e  endof
        13 of  0.3e 1.2566371e  endof
        >r 0e 0e r>
    endcase ;
: turn-head ( -- )
    h-cond 2 = if  0e 0e
    else h-look-char @ $FF <> if
        h-look-char @ dup with? if  char-look  else  drop $FF h-look-char !  0e 0e  then
    else h-look-pt? @ if  h-look-pt look-at
    else  look-mode  then then then                               ( F: pitch turn )
    -2.8274333e fmax 2.8274333e fmin  hd-y f!
    -0.7853982e fmax 2.3561945e fmin  hd-p f!
    \ eased 0.15 of the way (the turn faster by how far his heading changed)
    hd-p f@ h-head-pitch f@ f- 0.15e f*  h-head-pitch f@ f+ h-head-pitch f!
    hd-y f@ h-head-yaw f@ f-  fdup fabs 0.15e f*
    h-look-now @ dup 8 = over 5 = or swap 0= or if  h-yaw h-yaw-was f@ f- angle-wrap fabs f+  then
    hd-s f!                                                         ( F: dy )
    fdup fabs hd-s f@ f<= if  fdrop hd-y f@ h-head-yaw f!  exit  then
    f0< if  hd-s f@ fnegate  else  hd-s f@  then  h-head-yaw f@ f+ h-head-yaw f! ;
