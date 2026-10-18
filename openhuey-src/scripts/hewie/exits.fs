\ hewie/exits.fs - out of the room by an exit, on screen: away from her or anywhere (actions
\ 0x24 / 0x25: Hewie_FleeExit, StateGoToExit), and drawing the stalker off while she hides
\ (0x7B: StatePlanPursuer, RunSpot, PursuerNear, Guard, Flee) - he makes for a way out, gets
\ ahead of the stalker on its way there and stands his ground barking at it; the second time
\ round he runs for the exit and out, and waits there a while (0x31) for his trust's time.
IN: hewie.exits
USING: engine actors common facts paths vectors flag-names messages doors hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.model hewie.moving hewie.states hewie.offscreen ;

\ ---- the exits: open to him, and where ----
: exit-open? ( e -- flag )  room-id swap room-exit-door  dup 0< if  drop false exit  then  door-usable? ;
: exit-in ( e -- tri )  1 exit-tri ;                       \ (Rooms_ExitPointIn: the spot this side)
create door-pt /vec allot
: door-pt! ( e -- found? )   \ (where its door stands; a doorway: the spot just out)
    dup room-door-at if  door-pt vec!  drop true exit  then  fdrop fdrop fdrop
    0 exit-tri dup 0< if  drop false exit  then  tri-center door-pt vec!  true ;

\ ---- Hewie_FleeExit: the exit to leave by - open, nearest first, those within 20 of `from`
\ last; the first he can plan a way to (-1 none, or `from` not in his room) ----
create ex-dist  8 floats allot   \ each exit's distance (-1: not one to take)
create ex-at /vec allot
: ex-d ( e -- addr )  floats ex-dist + ;
: measure ( from e -- )
    dup ex-d -1e f!
    dup exit-open? 0= if  2drop exit  then
    dup door-pt! 0= if  2drop exit  then
    door-pt rot body-at vec-dist-xz 20e f< if  ex-d 100000e f! exit  then
    door-pt his-at vec-dist-xz ex-d f! ;
: nearest ( -- e | -1 )   \ (the nearest still to try; ties: the later exit)
    -1  8 0 do
        i ex-d f@ f0< 0= if
            dup 0< if  drop i  else  i ex-d f@ dup ex-d f@ f<= if  drop i  then  then
        then
    loop ;
: flee-exit ( from both? -- e | -1 )
    drop   \ (passable for `from` too: the doors here don't tell one passer from another)
    dup self <> over with? 0= and if  drop -1 exit  then
    8 0 do  dup i measure  loop  drop
    8 0 do
        nearest dup 0< if  unloop exit  then
        dup exit-in dup 0< 0= if
            dup tri-center ex-at vec!  ex-at plan-to if  unloop exit  then
        else  drop  then
        -1e ex-d f!
    loop  -1 ;

\ ---- away-from-her-act / away-act: to an exit and out (Hewie_StateGoToExit) ----
: to-place ( e -- )   \ (his-spot the spot by it, his-t1 its triangle, his-t3 his gait)
    free-mode his-mode !  dup his-exit !  exit-in dup his-t1 !  tri-center his-spot vec!
    0 his-t2 !  random-level his-t3 !  his-path path-clear  breaks-any on-waiting on-footing or but his-cmd ! ;
: on-spot? ( -- flag )  his-spot -1 body-tri-to his-t1 @ = ;
: making-for? ( -- ok? )   \ (no way to the spot: himself again)
    on-spot? 0= path-done? and if  his-t1 @ his-spot plan-and-go if  to-default false exit  then  then  true ;
: toward-spot ( -- )  on-spot? if  his-spot run-straight  else  stride 0= if  trot-pose keep-pose  then  then ;
: past-spot? ( -- flag )   \ the spot nearer its door's way out than he is
    his-exit @ 0 exit-tri dup 0< if  drop false exit  then
    tri-center door-pt vec!  his-spot door-pt vec-dist  his-at door-pt vec-dist f< ;
: out-door ( -- door | -1 )  room-id his-exit @ room-exit-door ;
: st-go-to-exit ( -- )
    making-for? 0= if  exit  then
    his-t3 @ walk-pose + keep-pose  toward-spot   \ (at his gait)
    his-exit @ exit-open? 0= past-spot? 0= or if  exit  then
    out-door dup 0< 0= if
        her-with? if  her body-tri fiona-at plan-to 0= if  dup avoid-door  then  then   \ (she can't follow)
        his-exit @ through-exit drop
    then  drop  to-default ;
: leave-by ( e -- )  to-place  ['] st-go-to-exit behave ;
: away-from-her ( -- )
    her-with? if  her  else  self  then  0 flee-exit  dup 0< if  drop to-default exit  then  leave-by ;
: away ( -- )
    self 0 flee-exit  dup 0< if  drop keep-off-her-act 0 want exit  then  leave-by ;

\ ---- draw-off-act: while she hides (fiona-hidden / -half-hidden), drawing the stalker off ----
: hiding? ( -- flag )  fiona-hidden state-flag?  fiona-half-hidden state-flag? or ;
: still-on? ( -- flag )   \ (the stalker here and she hidden; else himself again)
    pursuer with? hiding? and dup 0= if  to-default  then ;
\ Hewie_StateFlee: for the exit and out; then a wait for his trust's time
: st-flee ( -- )
    still-on? 0= if  exit  then
    making-for? 0= if  exit  then
    run-pose keep-pose
    on-spot? if  his-spot run-straight  else  stride 0= his-danger @ calm <> and if  trot-pose keep-pose  then  then
    his-exit @ exit-open? 0= past-spot? 0= or if  exit  then
    out-door 0< if  to-default exit  then
    his-exit @ through-exit drop  flee-wait trust-of his-wanted !  wait-after-flight-act 0 want ;
\ Hewie_StateGuard: barking at the stalker while it is 60..100 away by foot
defer plan-cut
: st-guard ( -- )
    still-on? 0= if  exit  then
    stand-pose step-to-pose 0= if
        pursuer body-tri pursuer body-at plan-to 0= if  to-default exit  then
        rest  fdup 60e f<  100e f> or if  ['] plan-cut behave exit  then
        bark  snap-at
    then
    target-look look!  pursuer his-target ! ;
\ Hewie_StatePursuerNear: turned to it, the stance, then on guard; within 50, again
: st-pursuer-near ( -- )
    still-on? 0= if  exit  then
    pursuer body-at his-at vec-dist-xz 50e f< if  ['] plan-cut behave exit  then
    pursuer body-at heading-to  run-turn turn-to
    pursuer body-at look-at his-look-yaw f! his-look-pitch f!  held-look look-now!
    60e deg>rad f> if  walk-pose keep-pose exit  then
    settled? 0= if  exit  then
    anim@ stance-anim = if  ['] st-guard behave exit  then
    stand-pose step-to-pose 0= if  stance-anim play  then ;
\ Hewie_StateRunSpot: for the spot on its way (his-cut), there first
: on-cut? ( -- flag )  his-cut -1 body-tri-to his-cut-tri @ = ;
: st-run-spot ( -- )
    still-on? 0= if  exit  then
    on-cut? if  his-at his-cut vec-dist
    else
        path-done? if  his-cut-tri @ his-cut plan-and-go if  to-default exit  then  then  rest
    then
    10e f< if  ['] st-pursuer-near behave exit  then
    run-pose keep-pose
    on-cut? if  his-cut run-straight  else  stride 0= if  trot-pose keep-pose  then  then ;
\ Hewie_StatePlanPursuer: the stalker's way to the exit, and the spot 80 along it; the end of
\ it already: on guard there (the first time), else for the exit and out
: st-plan-pursuer ( -- )
    still-on? 0= if  exit  then
    his-path  pursuer body-tri  pursuer body-at  his-t1 @  his-spot  self body-mask  path-plan
    0> 0= if  to-default exit  then
    his-path pursuer body-at 80e path-ahead  his-path path-n =       ( end? )
    his-cut ahead vec-copy  his-cut -1 body-tri-to his-cut-tri !
    his-path path-clear
    if  his-t3 @ if  ['] st-flee  else  ['] st-guard  then  else  ['] st-run-spot  then  behave
    1 his-t3 ! ;
' st-plan-pursuer is plan-cut
: draw-off ( -- )
    pursuer dup with? 0= if  drop sniff-act 0 want exit  then
    dup mode-of dup unreachable-mode = swap struck-mode = or if  drop sniff-act 0 want exit  then
    1 flee-exit  dup 0< if  drop sniff-act 0 want exit  then
    dup his-exit !  exit-in his-t1 !  his-t1 @ tri-center his-spot vec!
    0 his-t3 !  his-path path-clear  free-mode his-mode !  on-command on-wait-over or his-cmd !  ['] st-plan-pursuer behave ;

\ ---- in a door's way (Hewie_FirstDoor, by Progress_WhoIsWhere's bits for him at each exit):
\ standing on an exit's way out at a door he may pass (not locked against him; one at rest also
\ shut) - out by it (walk-out-act, Hewie_StateWalkOut); in its gap, or in front of it in its
\ swing - he squeezes out to 24 from where it stands, on his side (squeeze-out-act,
\ Hewie_StateSqueeze) ----
$20000 constant door-gap   \ (the floor flag of a door's gap)
create sq-at /vec allot   create door-at /vec allot
: on-way-out? ( e -- flag )   \ (bit 1: on the exit's own area)
    exit-area dup area-count < if  his-at vec@ area-in?  else  drop false  then ;
: may-pass? ( e -- flag )   \ (not locked against him, nor at all)
    room-id swap room-exit-door  dup 0< if  drop false exit  then
    dup against hewie-kind kind-bit and 0=  swap door-locked? 0= and ;
: at-door? ( e -- flag )  his-at vec@ door-near? ;                      \ (bit $20)
: on-its-sides? ( e -- flag )                                              \ (bit 8)
    dup at-door? 0= if  drop false exit  then
    dup 0 his-tri door-on-side?  swap 1 his-tri door-on-side? or ;
: in-its-gap? ( e -- flag )  dup at-door? swap 0 his-at vec@ door-in-area? and ;   \ (bit 4)
: in-front? ( e -- flag )  his-at vec@ door-side 1 = ;                     \ (bit $10)
: on-gap? ( -- flag )  his-tri dup 0< if  drop false exit  then  nav-flags door-gap and 0<> ;
: out-there ( e -- )   \ (Hewie_DoorPoint: 24 from where it stands, out on his side)
    {: e :}  e room-door-at 0= if  fdrop fdrop fdrop exit  then  door-at vec!
    e door-turn  e his-at vec@ door-side 0= if  pi f+  then  angle-wrap {: F: yaw :}
    yaw fsin 24e f* door-at v.x sf@ f+  door-at v.y sf@  yaw fcos 24e f* door-at v.z sf@ f+  sq-at vec! ;

: squeeze-from? ( e front? -- flag )   \ (near enough where it stands - then his point out)
    \ (the original measures in front of it from its far point: here, where it stands)
    {: e front :}
    e room-door-at 0= if  fdrop fdrop fdrop false exit  then  door-at vec!
    door-at v.y sf@ his-at v.y sf@ f- fabs 5e f< 0= if  false exit  then
    front 0= if   \ (in its gap: no more than 5 past it, along its turn at rest)
        e door-turn {: F: t :}
        t fsin door-at v.x sf@ his-at v.x sf@ f- f*  t fcos door-at v.z sf@ his-at v.z sf@ f- f* f+
        5e f< 0= if  false exit  then
        e his-exit !  e out-there  true exit
    then
    his-at door-at vec-dist-xz 16e f< 0= if  false exit  then
    e out-there  sq-at door-at vec-dist-xz  his-at door-at vec-dist-xz f> if  e his-exit !  true  else  false  then ;
: first-door ( -- 0 squeeze | 1 out | -1 )
    8 0 do
        i door-here? if
            i door-animating? if
                i on-way-out?  i may-pass? and if  i his-exit !  1 unloop exit  then
                i in-front? if
                    i on-its-sides?  i 3 his-at vec@ door-in-area? or if
                        i 1 squeeze-from? if  0  else  -1  then  unloop exit
                    then
                else  i in-its-gap?  i on-its-sides? or if
                    i 0 squeeze-from? if  0  else  -1  then  unloop exit
                then  then
            else
                i on-way-out?  i may-pass? and  room-id i room-exit-door door-open? 0= and if
                    i his-exit !  1 unloop exit
                then
                i on-its-sides?  on-gap? and if
                    i  i in-front? 1 and  squeeze-from? if  0  else  -1  then  unloop exit
                then
            then
        then
    loop  -1 ;
: st-squeeze ( -- )
    walk-pose keep-pose  sq-at run-straight
    on-gap? if  through-mask body-mask!  then
    his-t1 @ if  -1 his-t1 +!  then
    on-gap? 0= if
        his-t1 @ 0=  room-id his-exit @ room-exit-leads drop room-id = or if  to-default exit  then
    then
    his-exit @ at-door? 0= if  to-default  then ;
: squeeze ( -- )  free-mode his-mode !  90 his-t1 !  0 his-cmd !  ['] st-squeeze behave ;
\ (walk-out-act: already on the exit's own area, he is through it - the original walks the step there first)
: walk-out ( -- )  his-exit @ through-exit drop  to-default ;
