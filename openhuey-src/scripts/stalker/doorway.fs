\ stalker/doorway.fs - a stalker at the doors of the room being played (P3b; src/game/pursuer.c
\ Pursuer_BehaviourDoors and its steps - Pursuer_NextPathExit, Pursuer_Door1C8 / DoorLineUp /
\ DoorGoThrough / DoorFace / DoorOpen / DoorThrough / DoorPush / Door1C4, Pursuer_ExitGoTo,
\ Debilitas_ExitDone, Pursuer_DoorWait).
\ Out of her room on foot by an exit: walked to its inside, then by its door as he finds it -
\ open, or a doorway: on through it; shut: lined up at his spot for it by the side he is on,
\ turned to it, its handle taken (`hold-door`), his animation (from behind it, in front) with the
\ door swinging along it (`use-door`), let go open, on through; held by someone: he waits by it; not
\ for him (locked): another way.
\ Round to her (Debilitas_HeadForFiona, Npc_SameRoomOtherSide): she across his own room - the
\ halves the exits they came in by open onto differ - he heads for her half by the doors.
\ A door slammed on him (Pursuer_Hit kind 5; Pursuer_StateHitAtDoor / StateBargedThrough): in
\ its way - knocked on through it, out of the room where it leads out (stunned there 150
\ frames); else staggered back from it.
IN: stalker.doorway
USING: engine game-state actors messages common facts flag-names paths vectors doors stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.anims rumble floor-flags ;

: doors-actor ( -- id )  s" doors" actor-named ;
defer door-done ( -- )   ' stand is door-done          \ given up on the way out (struck.fs: on again)
defer round-done ( -- )   ' stand is round-done        \ a door opened to get at her: his chase again (stalker/ladder.fs)
defer round-failed ( -- )   ' stand is round-failed    \ ... given up (stalker/ladder.fs)
defer door-recovered ( -- )   ' stand is door-recovered   \ a door's blow over (struck.fs)

\ ---- a door as he finds it at an exit of his room (travel's door-kind) ----
: kind-at ( exit -- k )  my-room swap room-exit-door door-kind ;
create spot-at /vec allot
: aim-at ( tri -- ok? ) ( F: x y z -- )   \ (his way planned there)
    spot-at vec!  dup 0< if  drop false exit  then  spot-at plan 0> ;

\ ---- the door he holds let go open (Npc_DoorRelease) - shut, his animation one that shuts it
\ (Npc_DoorShutOther: the story's door use, 1 / 3); his way out over ----
: shutting? ( -- flag )  door-use @ 0< 0=  door-use @ 1 and 0<> and ;
: let-door-go ( -- )
    held-door @ dup 0< if  drop exit  then  room-id swap stalker-noise
    shutting? if  doors-actor send let-go-shut  else  doors-actor send let-go-open  then  -1 held-door ! ;
: out-over ( -- )  0 apart !  0 out-going !  0 wandering-out !  -1 door-use !  -1 asked !  0 for-her !  0 door-script ! ;
\ the story's door use (stalker/scripted.fs): over - or given up, his step left trying it (the
\ original's: never done)
defer script-door-over ( -- )   ' noop is script-door-over
: script-door-failed ( -- )  let-door-go  out-over  ['] noop step ! ;
: give-up-door ( -- )
    door-script @ if  script-door-failed exit  then
    let-door-go  for-her @  out-over  if  round-failed  else  door-done  then ;
: no-way-to-door ( -- )   \ (no way to his spot by it: not that way again - Pursuer_OnToNextExit)
    my-room making-for @ room-exit-door avoid-door  give-up-door ;
: out ( -- )  let-door-go  out-over  making-for @ through ;   \ (Pursuer_ThroughDoor)

\ ---- on through an open door or a doorway (Pursuer_ExitGoTo: to its spot 12 beyond - its
\ spots: 0 4 inside, 1 12 beyond, 2 12 inside -, or by it; then Debilitas_ExitDone: through) ----
: going-through ( -- )  walk-stride if  out  then ;
: on-through ( -- )   \ (the door only opened to get at her: done - his chase again)
    for-her @ if  out-over  round-done exit  then
    making-for @ 1 exit-spot aim-at 0= if
        making-for @ 0 exit-spot aim-at 0= if  out exit  then   \ (no way on: through from here)
    then
    mode-walk play  ['] going-through step! ;

\ ---- his door animation (by how he uses it): once it has faded in the door swings along it
\ (Pursuer_DoorThrough); he is moved by it until the door is at rest (Pursuer_DoorPush), then
\ lets it go open (Pursuer_Door1C4) and goes on through ----
: door-pushing ( -- )   \ (out of contact while on barred floor: Pursuer_DoorPush's +0x2A)
    self body-tri dup 0< if  drop 0  else  nav-flags barred-floor and  then  0<> apart !
    root-move
    1 step-t +!  step-t @ 3 < if  exit  then               \ (the doors take it up next frame)
    making-for @ door-animating? if  exit  then
    door-script @ if  let-door-go  out-over  script-door-over exit  then   \ (the story's: done with it)
    let-door-go  0 apart !  on-through ;   \ (Pursuer_Door1C4: in contact again)
: door-swinging ( -- )
    root-move  settled? 0= if  exit  then
    making-for @ door-use @ stalker-noise doors-actor send use-door
    ['] door-pushing step! ;
: got-door ( -- )   \ (Pursuer_DoorBarge: out of contact till the door is let go)
    -1 apart !  making-for @ held-door !  -1 asked !  door-anims door-use @ + play-now  ['] door-swinging step! ;

\ ---- lined up at his spot for its animation (Pursuer_DoorLineUp, DoorGoThrough: from behind
\ it, in front; one more - the animation that shuts it - while it is open), turned to its heading
\ (Pursuer_DoorFace / DoorOpen), its handle asked for ----
0 constant from-behind-use   2 constant in-front-use
: asking ( -- ) ;                                          \ (the doors' answer: `door-held` / `door-refused`)
: facing-door ( -- )
    spot-yaw f@ turn-rate body-turn-toward f0= 0= if  exit  then
    making-for @ asked !  room-id making-for @ stalker-kind doors-actor send hold-door
    ['] asking step! ;
: lining-up ( -- )  walk-stride if  still-anim play  ['] facing-door step!  then ;
: line-up ( -- )
    my-at vec@ making-for @ door-side  dup 0< if  drop no-way-to-door exit  then
    if  in-front-use  else  from-behind-use  then  making-for @ kind-at open-way = 1 and +  door-use !
    making-for @ door-use @ door-user-spot  spot-yaw f!      ( tri ) ( F: x y z )
    aim-at 0= if  no-way-to-door exit  then
    mode-walk play  ['] lining-up step! ;

\ ---- by a door held shut (Pursuer_DoorWait): he waits, looking again every half second; 10
\ seconds and he gives up on it ----
defer at-exit ( -- )
\ while he waits, the door swinging toward him (between 10 and 80 open) and her behind it (its
\ area 2) or holding it: it is pushed into her (her request 5: thrown by the door)
: door-on-her? ( -- flag )
    making-for @ door-angle fdup -80e f> fswap -10e f< and 0= if  false exit  then
    making-for @ 2 fiona body-at vec@ door-in-area?
    my-room making-for @ room-exit-door door-held? or ;
: push-door ( -- )  door-on-her? if  door-request making-for @ 20e f>cell fiona send hit  then ;
: waiting-door ( -- )
    my-room room-id = if  push-door  then
    1 door-wait +!
    door-wait @ 300 > if  my-room making-for @ room-exit-door avoid-door  give-up-door exit  then
    door-wait @ 15 mod 0= if  at-exit  then ;

\ ---- at the exit (Pursuer_BehaviourDoors' 0x27: Npc_ExitKind) ----
:noname ( -- )
    making-for @ kind-at case
        open-way of  0 door-wait !  on-through  endof
        shut-way of  0 door-wait !  line-up  endof
        held-way of  still-anim play  ['] waiting-door step!  endof
        >r  my-room making-for @ room-exit-door avoid-door  give-up-door  r>
    endcase ; is at-exit
: to-exit ( -- )   \ (Pursuer_NextPathExit / FollowPathExit; going round, seeing her: after her)
    my-mode @ after-her = sees-fiona @ and if  out-over  chase-begin exit  then
    walk-stride if  at-exit  then ;
: out-by ( exit -- ok? )   \ out of the room being played by that exit, on foot
    dup making-for !  -1 out-going !  0 door-wait !  -1 asked !  -1 door-use !  no-search search-phase !
    dup 2 exit-spot aim-at if  drop                        \ (12 inside it; else 4 inside; else at
    else  0 exit-spot aim-at 0= if  at-exit  out-going @ 0<> exit  then  then   \ its door at once)
    mode-walk play  ['] to-exit step!  true ;

\ ---- out by an exit at random (Npc_RandomExit, Pursuer_PlanWhere's mode 3): one of the usable
\ exits on his side of the room - the side of the exit he came in by - at random; the one he
\ came in by drawn first, drawn once more ----
create re-list 8 cells allot
: my-side ( exit -- way )  my-room swap 1 exit-way ;
: random-exit ( -- exit | -1 )
    0 {: n :}
    8 0 do
        came-by @ 0< if  true  else  i my-side came-by @ my-side =  then
        i usable? and if  i n cells re-list + !  n 1+ to n  then
    loop
    n 0= if  -1 exit  then
    n s>f rnd f* f>s n 1- min cells re-list + @ ;
\ off stage, waiting about (Pursuer_IntoRoomByEvent, not after her nor planned: Pursuer_PlanWhere's
\ mode 3): his route to the room past such an exit; none, to the room past the one he came by
: route-at-random ( -- )
    random-exit dup came-by @ = if  drop random-exit  then
    dup 0< if  drop exit  then
    my-room swap room-exit-leads drop  route! 0> if  exit  then
    came-by @ dup 0< if  drop exit  then  my-room swap room-exit-leads drop  route! drop ;
:noname ( -- )
    random-exit dup came-by @ = if  drop random-exit  then
    dup 0< if  drop stand exit  then
    out-by if  -1 wandering-out !  else  stand  then ; is leave-at-random
:noname ( -- )  let-door-go  out-over ; is out-cancelled

\ ---- the run (Hewie played): after her while she is in another room, out by the first door of
\ his way to it (Pursuer_BehaviourFollow's 0x27 / 0xE: to the exit, through it) ----
:noname ( -- went? )
    out-going @ if  false exit  then
    her-room dup 0<  over my-room = or if  drop false exit  then
    my-mode @ dup after-her = swap heading-for-her = or 0= if  drop false exit  then
    avoid-clear  route! 0> 0= if  false exit  then
    next-door door-exit dup 0< if  drop false exit  then
    out-by ; is after-her-elsewhere

\ ---- the doors' answers to his asking ----
: held-answer ( exit -- )   \ (`door-held`: his to open; else the door he came in by - swung open)
    dup asked @ = if  drop got-door exit  then  swing-open ;
: refused-answer ( exit -- )   \ (`door-refused`: as he finds it now - shaken, it won't give)
    asked @ <> if  exit  then  -1 asked !  $80 15 stalker-jolt
    door-script @ if  script-door-failed exit  then  at-exit ;

\ ---- the story's door use (its moves 3 / 4: Pursuer_Door1C8, his action $A) - the door at exit
\ `exit` opened if shut, shut if open, from his side; then (stalker/scripted.fs) he stands ----
: script-door ( exit -- )
    out-over  making-for !  -1 door-script !  0 door-wait !  -1 asked !
    line-up ;

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

\ ---- a door between him and her in his room (Npc_ExitsToTri, his action $B: Pursuer_DoorWalkTo,
\ DoorWalking, DoorFacing, DoorGoThrough ...): of his room's exits with a door (not a doorway),
\ neither open and free nor locked, the first whose spot her side (0, else 2) has a way to the
\ middle of her triangle and his side a way from him, on another triangle; else one with a way
\ from him that leads back into his room - the last of those, or the first of the others while
\ there is none. He lines up at it and opens it (as on his way out), lets it go open and is
\ after her again; held, he waits by it ----
create dh-at /vec allot   create ds-at /vec allot
: door-spot ( exit tri v -- spot-tri | -1 )   \ (Npc_PlanToDoor: its spot on side 0, else 2, with a way there)
    {: e t v :}
    2 0 do
        e i 2* door-user-spot fdrop ds-at vec!
        dup 0< 0= if  dup t v rot ds-at self body-mask v-path 0> if  unloop exit  then  then  drop
    loop  -1 ;
: shut-between? ( exit -- flag )   \ (a door, neither open and free nor locked)
    my-room swap room-exit-door  dup 0< if  drop false exit  then
    dup fixed? if  drop false exit  then
    dup state@ dup opened-bit and 0<>  swap held-bit and 0=  and if  drop false exit  then
    door-locked? 0= ;
: loops? ( exit -- flag )  my-room over room-exit-leads drop my-room =  nip ;
: door-to-her ( -- exit true | false )
    fiona body-tri dup 0< if  drop false exit  then  tri-center dh-at vec!  -1 {: fallback :}
    8 0 do
        i shut-between? if
            i fiona body-tri dh-at door-spot dup 0< if
                drop  i loops? fallback 0< or if
                    i self body-tri my-at door-spot 0< 0= if  i to fallback  then
                then
            else
                i self body-tri my-at door-spot  dup 0< 0= rot rot <> and if  i true unloop exit  then
            then
        then
    loop
    fallback dup 0< 0= if  dup loops? if  true exit  then  then  drop false ;
: to-her-door ( -- )  walk-stride if  line-up  then ;   \ (Pursuer_DoorWalking: there, lined up for it)
: door-for-her ( exit -- )   \ (Pursuer_DoorWalkTo: to its spot on his side first)
    dup making-for !  -1 out-going !  0 wandering-out !  0 door-wait !  -1 asked !  -1 door-use !  -1 for-her !
    no-search search-phase !
    self body-tri my-at door-spot  ds-at vec@ aim-at 0= if  give-up-door exit  then
    mode-walk play  ['] to-her-door step! ;

\ ---- a door slammed on him ----
\ away from the side Fiona is on (Npc_DoorFacingFromFiona)
: door-heading ( exit -- ) ( F: -- a )  dup door-turn  fiona body-at vec@ door-side 1 = if  pi f+ angle-wrap  then ;
create dr-at /vec allot
: in-its-way? ( exit -- flag )   \ within 9 of it, or on its passage
    dup room-door-at drop  dr-at vec!  my-at dr-at vec-dist 9e f< if  drop true exit  then
    dup 0 self body-tri door-on-side?  swap 1 self body-tri door-on-side? or ;
: pushed ( -- )   \ 3 along the heading (Actor_Move: off his floor, his triangle kept)
    push-way f@ 3e dr-at my-at vec-ahead
    dr-at put-at ;
: knocked-through ( -- )   \ (on through the doorway until past it; where it leads out, out of the room)
    root-move  pushed
    my-at vec@ push-exit @ door-near? if  exit  then
    0 reeling !
    my-room push-exit @ room-exit-leads drop my-room = if   \ (between his room's halves)
        hp @ 0> 0= if  1 hp !  then  -1 push-exit !  door-recovered exit
    then
    hp @ 0> 0= if  hp-max @ hp !  held-off mode!  then    \ (beaten: as he gives up - stalker.md P3a)
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
    let-door-go  out-over  after-her mode!
    dup push-exit !  dup door-heading push-way f!
    dup in-its-way?  over kind-at barred-way <> and if
        drop  flinch-low-anim play-now  -1 reeling !  ['] knocked-through step!  exit
    then
    drop  hp @ 0> 0= if  1 hp !  then
    push-way f@ pi f+ angle-wrap push-way f!  -1 push-exit !
    flinch-door-low-anim play-now  -1 reeling !  ['] staggered-back step! ;
\ off screen she slams the door he makes for: right behind it (within 15 of it) he takes the
\ damage; else nothing (Fiona_SlamDoor's chased case)
: behind-door? ( exit -- flag )
    room-id over room-exit-leads drop my-room <> if  drop false exit  then
    room-id swap room-exit-door next-door <> if  false exit  then
    to-door f@ 15e f< ;
