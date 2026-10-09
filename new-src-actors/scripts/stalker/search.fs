\ stalker/search.fs - a stalker searching the room being played (src/game/pursuer.c): his route
\ of stops (Npc_RouteAdd, Pursuer_AddSearchStops: the room's search spots or triangles at
\ random), walking to each (Pursuer_SearchRoute, Pursuer_StepBack, Pursuer_CloseInGoal), what he
\ does there (Pursuer_PickFromTable over Debilitas_AttackTable's "searched": walk on, or look
\ about), and his mode with its clock (Pursuer_ModesSearching).
IN: stalker.search
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.spots ;

: here-played? ( -- flag )  in-game @ if  self body-room room-id =  else  false  then ;

\ ---- his route (8 stops: a triangle, and whether it was picked at random) ----
: stop ( k -- addr )  2* cells stops + ;
: route-clear ( -- )   \ Pursuer_ClearRoute
    8 0 do  -1 i stop !  0 i stop cell+ !  loop  0 route-next !  0 route-end ! ;
: stops-left ( -- n )  route-end @ route-next @ - ;
: route-add ( tri random? -- )   \ Npc_RouteAdd (full, or no triangle: not added)
    over 0<  route-end @ 8 < 0= or if  2drop exit  then
    route-end @ stop  tuck cell+ !  !  1 route-end +! ;
\ Npc_RandomTri: a triangle of the room he may stand on (his floor; not both $100000 and $200000)
: random-tri ( -- tri )
    here-played? 0= nav-tris 0= or if  -1 exit  then
    1000 0 do
        rnd nav-tris s>f f* f>s  nav-tris 1- min
        dup nav-flags self body-mask and 0=  over nav-flags $300000 and $300000 <> and if  unloop exit  then
        drop
    loop  -1 ;
\ Pursuer_AddSearchStops: n stops - each 60 in 100 the room's next search spot while it has
\ more, else a triangle at random
variable spots  variable spots-n  variable spots-used
: add-stops ( n -- )
    room-id search-spots spots-n !  spots !  0 spots-used !
    0 ?do
        spot-next @ spots-n @ < 0= if  0 spot-next !  then
        spots-used @ spots-n @ <  rnd 100e f* 60e f<= and if
            spot-next @ cells spots @ + @ 0 route-add  1 spot-next +!  1 spots-used +!
        else  random-tri -1 route-add  then
    loop ;
: search-stops ( -- n )  rnd 4e f* f>s 2 + ;   \ (searching: 2..5 stops)

\ ---- his mode's clock (vtable +0x2BC .. +0x2C4: Debilitas after her 300 frames (hard 540),
\ searching 600, waiting 900) ----
: hard? ( -- flag )  progress pr.vars $27 + c@ 1 = ;
: after-t ( -- )  hard? if  540  else  300  then  mode-t ! ;
: mode! ( m -- )
    dup my-mode !  case
        0 of  after-t  endof
        2 of  600 mode-t !  endof
        3 of  900 mode-t !  endof
    endcase ;

\ ---- the steps (one runs each frame: `step`) ----
: step! ( xt -- )  step !  0 step-done !  0 step-t ! ;
: stand ( -- )   \ (vtable +0x320: Debilitas stands with 0)
    0 play  ['] noop step! ;
\ his walk for his mode (Debilitas_StandAnim, vtable +0x128): after her, close (80 on foot) and
\ she isn't hiding, $200, else $206; waiting $200; searching or heading for her room $201
: walk-anim ( -- anim )
    my-mode @ case
        0 of  d-fiona f@ 80e f<= d-fiona f@ f0< 0= and  fiona-hidden state-flag? 0= and if  $200  else  $206  then  endof
        3 of  $200  endof
        >r $201 r>
    endcase ;

\ Pursuer_CloseInGoal: along his way; slowed to a stand within 10 of the goal; over once the
\ stand has faded in (a stop picked at random: at most 150 frames' walk)
: walking ( -- )
    walk-stride drop
    way-left 10e f<  anim @ $201 = and if  0 play  then
    anim @ 0= settled? and  my-path path-left? 0= or if  -1 step-done !  then ;
\ the turn on the spot: turned by it until it ends or he faces the way, then his walk
create to-at 12 allot
: turning ( -- )
    root-move
    settled? 0= if  exit  then
    ended?  to-at heading-to yaw f- angle-wrap fabs 0.1e f< or if
        walk-anim play  ['] walking step!
    then ;
\ Pursuer_StepBack: setting off - turned on the spot toward the way's next point, or walking
: set-off ( -- )
    0.1e my-path my-at path-ahead drop  to-at ahead vec-copy
    to-at way-to dup $FF = if  drop walk-anim play  ['] walking step!  exit  then
    $400 + play-now  ['] turning step! ;

\ the look about at a stop (Pursuer_StateWalkGesture: Debilitas_AttackTable's gestures $1302,
\ $1303, $1305 from his gesture table) to its end; or standing a moment (Pursuer_WalkAside)
: gesturing ( -- )   root-move  ended? if  -1 step-done !  then ;
: standing ( -- )   1 step-t +!  settled? step-t @ 30 > and if  -1 step-done !  then ;
: searched ( -- )   \ (situation $10: 50 walk on, 20 $1302, 20 $1303, 10 $1305)
    rnd 100e f*
    fdup 50e f< if  fdrop -1 gesture !  0 play  ['] standing step!  exit  then
    fdup 70e f< if  fdrop $1302  else  90e f< if  $1303  else  $1305  then  then
    dup gesture !  play-now  ['] gesturing step! ;

\ ---- Pursuer_SearchRoute: aimed at the route's next stop (none: one at random added); one
\ he can't reach is skipped; past 8 the route is given up ----
: aim ( -- ok? )
    begin
        route-next @ route-end @ < 0= if  random-tri -1 route-add  then
        route-next @ route-end @ < 0= if  false exit  then
        route-next @ stop @ dup goal-tri !  tri-center goal-at vec!
        goal-tri @ goal-at plan 0> if
            route-next @ stop cell+ @ if  150  else  0  then  stop-wait !  true exit
        then
        1 route-next +!  route-next @ 8 < 0=
    until  route-clear  -1 route-done !  false ;

\ ---- Pursuer_BehaviourSearch (searching): each stop walked to and searched, then the next;
\ the route over, he stands (his mode moves on: stalker/modes) ----
: to-next-stop ( -- )
    aim if  set-off  1 search-phase !  else  stand  0 search-phase !  then ;
: at-stop ( -- )
    1 route-next +!  0 stop-wait !
    route-next @ route-end @ < 0= if  -1 route-done !  then
    2 search-phase !  searched ;
: search ( -- )   \ (each frame, after his step)
    search-phase @ case
        1 of
            stop-wait @ if  -1 stop-wait +!  stop-wait @ 0= if  -1 step-done !  then  then
            step-done @ if  at-stop  then
        endof
        2 of
            step-done @ if  route-done @ if  stand 0 search-phase !  else  to-next-stop  then  then
        endof
    endcase ;
: search-start ( -- )  0 route-done !  to-next-stop ;

\ ---- to where she is (Pursuer_ChaseFiona, action 4: after her but not seeing her): toward
\ her on foot, his way re-planned every 30 frames; standing facing her within 15. Seeing her,
\ his chase (stalker/chase.fs: `chase-begin`) ----
: after-her ( -- )
    -1 step-t +!  step-t @ 0> if  exit  then  30 step-t !
    fiona body-tri dup 0< if  drop exit  then  fiona body-at plan 0> 0= if  exit  then
    d-fiona f@ 15e f< d-fiona f@ f0< 0= and if  0 play  my-path path-clear  else  walk-anim play  then ;
: chase-step ( -- )   \ (the step while after her)
    my-path path-left? if  walk-stride drop  else  fiona body-at turn-rate turn-to fdrop  then
    after-her ;
: chase-start ( -- )  0 search-phase !  ['] chase-step step!  after-her ;
defer chase-begin ( -- )   ' chase-start is chase-begin   \ (seeing her: stalker.chase)
defer chasing? ( -- flag )   ' false is chasing?
defer lost-her ( -- )   ' chase-start is lost-her

\ ---- Pursuer_ModesSearching, in the room being played (elsewhere: with his travel, P1d):
\ seeing her he's after her (mode 0) and his clock (300 frames) starts again; out of sight it
\ runs down, then he searches the room (mode 2: 2..5 stops); his route over he waits about
\ (mode 3, 900 frames), then searches again; heading for her room (1) he searches it once in;
\ held off (4) he's after her again when it runs out ----
: tick-down ( -- out? )  mode-t @ 0> if  -1 mode-t +!  then  mode-t @ 0= ;
: search-again ( -- )  2 mode!  route-clear  search-stops add-stops  search-start ;
: modes ( -- )
    here-played? 0= if  exit  then
    sees-fiona @ if
        my-mode @ if  0 mode!  route-clear  then
        chasing? 0= if  chase-begin  then  after-t exit
    then
    chasing? if  lost-her  then   \ (lost sight of her: to where she is)
    my-mode @ case
        0 of  tick-down if  search-again  then  endof
        1 of  search-again  endof
        2 of  route-done @ if  3 mode!  then  endof
        3 of  tick-down if  search-again  then  endof
        4 of  tick-down if  0 mode!  chase-start  then  endof
    endcase ;
