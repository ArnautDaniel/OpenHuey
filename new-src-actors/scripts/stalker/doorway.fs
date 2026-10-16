\ stalker/doorway.fs - a stalker at the doors of the room being played (P3b; src/game/pursuer.c
\ Pursuer_BehaviourDoors and its steps - Pursuer_NextPathExit, Pursuer_Door1C8 / DoorLineUp /
\ DoorGoThrough / DoorFace / DoorOpen / DoorThrough / DoorPush / Door1C4, Pursuer_ExitGoTo,
\ Debilitas_ExitDone, Pursuer_DoorWait).
\ Out of her room on foot by an exit: walked to its inside, then by its door as he finds it -
\ open, or a doorway: on through it; shut: lined up at his spot for it by the side he is on,
\ turned to it, its handle taken (`hold-door`), his animation ($600 / $602) with the door
\ swinging along it (`use-door`), let go open, on through; held by someone: he waits by it; not
\ for him (locked): another way.
\ Round to her (Debilitas_HeadForFiona, Npc_SameRoomOtherSide): she across his own room - the
\ halves the exits they came in by open onto differ - he heads for her half by the doors.
\ A door slammed on him (Pursuer_Hit kind 5; Pursuer_StateHitAtDoor / StateBargedThrough): in
\ its way - knocked on through it ($1004), out of the room where it leads out (stunned there 150
\ frames); else staggered back from it ($1005).
IN: stalker.doorway
USING: engine game-state actors messages common facts flag-names paths doors stalker.state stalker.senses stalker.moving stalker.search stalker.travel rumble ;

: doors-actor ( -- id )  s" doors" actor-named ;
defer door-done ( -- )   ' stand is door-done          \ given up on the way out (struck.fs: on again)
defer door-recovered ( -- )   ' stand is door-recovered   \ a door's blow over (struck.fs)

\ ---- a door as he finds it at an exit of his room (travel's door-kind: 2 held, 3 not for him,
\ 4 a way through, 5 shut) ----
: kind-at ( exit -- k )  my-room swap room-exit-door door-kind ;
create spot-at 12 allot
: aim-at ( tri -- ok? ) ( F: x y z -- )   \ (his way planned there)
    spot-at vec!  dup 0< if  drop false exit  then  spot-at plan 0> ;

\ ---- the door he holds let go open (Npc_DoorRelease); his way out over ----
: let-door-go ( -- )
    held-door @ dup 0< if  drop exit  then  room-id swap stalker-noise doors-actor send let-go-open  -1 held-door ! ;
: out-over ( -- )  0 out-going !  0 wandering-out !  -1 door-use !  -1 asked ! ;
: give-up-door ( -- )  let-door-go  out-over  door-done ;
: out ( -- )  let-door-go  out-over  making-for @ through ;   \ (Pursuer_ThroughDoor)

\ ---- on through an open door or a doorway (Pursuer_ExitGoTo: to its spot 12 beyond - its
\ spots: 0 4 inside, 1 12 beyond, 2 12 inside -, or by it; then Debilitas_ExitDone: through) ----
: going-through ( -- )  walk-stride if  out  then ;
: on-through ( -- )
    making-for @ 1 exit-spot aim-at 0= if
        making-for @ 0 exit-spot aim-at 0= if  out exit  then   \ (no way on: through from here)
    then
    walk-anim play  ['] going-through step! ;

\ ---- his door animation ($600 + its kind): once it has faded in the door swings along it
\ (Pursuer_DoorThrough); he is moved by it until the door is at rest (Pursuer_DoorPush), then
\ lets it go open (Pursuer_Door1C4) and goes on through ----
: door-pushing ( -- )
    root-move
    1 step-t +!  step-t @ 3 < if  exit  then               \ (the doors take it up next frame)
    making-for @ door-animating? if  exit  then
    let-door-go  on-through ;
: door-swinging ( -- )
    root-move  settled? 0= if  exit  then
    making-for @ door-use @ stalker-noise doors-actor send use-door
    ['] door-pushing step! ;
: got-door ( -- )  making-for @ held-door !  -1 asked !  $600 door-use @ + play-now  ['] door-swinging step! ;

\ ---- lined up at his spot for its animation (Pursuer_DoorLineUp, DoorGoThrough: 0 from behind
\ it, 2 in front), turned to its heading (Pursuer_DoorFace / DoorOpen), its handle asked for ----
fvariable spot-yaw
: asking ( -- ) ;                                          \ (the doors' answer: `door-held` / `door-refused`)
: facing-door ( -- )
    spot-yaw f@ turn-rate body-turn-toward f0= 0= if  exit  then
    making-for @ asked !  room-id making-for @ stalker-kind doors-actor send hold-door
    ['] asking step! ;
: lining-up ( -- )  walk-stride if  stand-anim play  ['] facing-door step!  then ;
: line-up ( -- )
    my-at vec@ making-for @ door-side  dup 0< if  drop give-up-door exit  then
    if  2  else  0  then  door-use !
    making-for @ door-use @ door-user-spot  spot-yaw f!      ( tri ) ( F: x y z )
    aim-at 0= if  give-up-door exit  then
    walk-anim play  ['] lining-up step! ;

\ ---- by a door held shut (Pursuer_DoorWait): he waits, looking again every half second; 10
\ seconds and he gives up on it ----
defer at-exit ( -- )
\ while he waits, the door swinging toward him (between 10 and 80 open) and her behind it (its
\ area 2) or holding it: it is pushed into her (her request 5: thrown by the door)
: door-on-her? ( -- flag )
    making-for @ door-angle fdup -80e f> fswap -10e f< and 0= if  false exit  then
    making-for @ 2 fiona body-at vec@ door-in-area?
    my-room making-for @ room-exit-door door-held? or ;
: push-door ( -- )  door-on-her? if  5 making-for @ 20e f>cell fiona send hit  then ;
: waiting-door ( -- )
    my-room room-id = if  push-door  then
    1 door-wait +!
    door-wait @ 300 > if  my-room making-for @ room-exit-door avoid-door  give-up-door exit  then
    door-wait @ 15 mod 0= if  at-exit  then ;

\ ---- at the exit (Pursuer_BehaviourDoors' 0x27: Npc_ExitKind) ----
:noname ( -- )
    making-for @ kind-at case
        4 of  0 door-wait !  on-through  endof
        5 of  0 door-wait !  line-up  endof
        2 of  stand-anim play  ['] waiting-door step!  endof
        >r  my-room making-for @ room-exit-door avoid-door  give-up-door  r>
    endcase ; is at-exit
: to-exit ( -- )   \ (Pursuer_NextPathExit / FollowPathExit; going round, seeing her: after her)
    my-mode @ 0= sees-fiona @ and if  out-over  chase-begin exit  then
    walk-stride if  at-exit  then ;
: out-by ( exit -- ok? )   \ out of the room being played by that exit, on foot
    dup making-for !  -1 out-going !  0 door-wait !  -1 asked !  -1 door-use !  0 search-phase !
    dup 2 exit-spot aim-at if  drop                        \ (12 inside it; else 4 inside; else at
    else  0 exit-spot aim-at 0= if  at-exit  out-going @ 0<> exit  then  then   \ its door at once)
    walk-anim play  ['] to-exit step!  true ;

\ ---- out by an exit at random (Npc_RandomExit, Pursuer_PlanWhere's mode 3): one of the usable
\ exits on his side of the room - the side of the exit he came in by - at random; the one he
\ came in by drawn first, drawn once more ----
create re-list 8 cells allot  variable re-n
: my-side ( exit -- way )  my-room swap 1 exit-way ;
: random-exit ( -- exit | -1 )
    0 re-n !
    8 0 do
        came-by @ 0< if  true  else  i my-side came-by @ my-side =  then
        i usable? and if  i re-n @ cells re-list + !  1 re-n +!  then
    loop
    re-n @ 0= if  -1 exit  then
    re-n @ s>f rnd f* f>s re-n @ 1- min cells re-list + @ ;
:noname ( -- )
    random-exit dup came-by @ = if  drop random-exit  then
    dup 0< if  drop stand exit  then
    out-by if  -1 wandering-out !  else  stand  then ; is leave-at-random
:noname ( -- )  let-door-go  out-over ; is out-cancelled

\ ---- the doors' answers to his asking ----
: held-answer ( exit -- )   \ (`door-held`: his to open; else the door he came in by - swung open)
    dup asked @ = if  drop got-door exit  then  swing-open ;
: refused-answer ( exit -- )   \ (`door-refused`: as he finds it now - shaken, it won't give)
    asked @ <> if  exit  then  -1 asked !  $80 15 stalker-jolt  at-exit ;

\ ---- round to her across his own room ----
: her-way ( -- way )  her-came @ dup 0< if  exit  then  room-id swap 1 exit-way ;
: across? ( -- flag )
    my-room room-id <> if  false exit  then
    my-way her-way  over 0< over 0< or if  2drop false exit  then  <> ;
: go-round ( -- went? )
    across? 0= if  false exit  then
    avoid-clear  my-room her-way route-way! 0> 0= if  false exit  then
    next-door door-exit dup 0< if  drop false exit  then  out-by ;
' go-round is round-to-her

\ ---- a door slammed on him ----
\ away from the side Fiona is on (Npc_DoorFacingFromFiona)
: door-heading ( exit -- ) ( F: -- a )  dup door-turn  fiona body-at vec@ door-side 1 = if  pi f+ angle-wrap  then ;
create dr-at 12 allot
: in-its-way? ( exit -- flag )   \ within 9 of it, or on its passage
    dup room-door-at drop  dr-at vec!  my-at dr-at vec-dist 9e f< if  drop true exit  then
    dup 0 self body-tri door-on-side?  swap 1 self body-tri door-on-side? or ;
: pushed ( -- )   \ 3 along the heading (Actor_Move: off his floor, his triangle kept)
    push-way f@ 3e dr-at my-at vec-ahead
    self body-tri >r  dr-at vec@ body-place-at
    self body-tri 0< if  room-id r@ dr-at vec@ body-place  then  r> drop ;
: knocked-through ( -- )   \ (on through the doorway until past it; where it leads out, out of the room)
    root-move  pushed
    my-at vec@ push-exit @ door-near? if  exit  then
    0 reeling !
    my-room push-exit @ room-exit-leads drop my-room = if   \ (between his room's halves)
        hp @ 0> 0= if  1 hp !  then  -1 push-exit !  door-recovered exit
    then
    hp @ 0> 0= if  hp-max @ hp !  4 mode!  then           \ (beaten: as he gives up - stalker.md P3a)
    150 stun-t !  push-exit @ making-for !  -1 push-exit !  out ;
: staggered-back ( -- )   \ (back from it to its end; then off any floor not his)
    root-move
    ended? 0= if  exit  then
    20 0 do
        self body-tri dup 0< if  drop leave  then
        nav-flags self body-mask and 0= if  leave  then  pushed
    loop
    0 reeling !  door-recovered ;
: door-struck ( exit -- )
    let-door-go  out-over  0 mode!
    dup push-exit !  dup door-heading push-way f!
    dup in-its-way?  over kind-at 3 <> and if
        drop  $1004 play-now  -1 reeling !  ['] knocked-through step!  exit
    then
    drop  hp @ 0> 0= if  1 hp !  then
    push-way f@ pi f+ angle-wrap push-way f!  -1 push-exit !
    $1005 play-now  -1 reeling !  ['] staggered-back step! ;
\ off screen she slams the door he makes for: right behind it (within 15 of it) he takes the
\ damage; else nothing (Fiona_SlamDoor's chased case)
: behind-door? ( exit -- flag )
    room-id over room-exit-leads drop my-room <> if  drop false exit  then
    room-id swap room-exit-door next-door <> if  false exit  then
    to-door f@ 15e f< ;
