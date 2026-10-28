\ stalker/search.fs - a stalker searching the room being played (src/game/pursuer.c): his route
\ of stops (Npc_RouteAdd, Pursuer_AddSearchStops: the room's search spots or triangles at
\ random), walking to each (Pursuer_SearchRoute, Pursuer_StepBack, Pursuer_CloseInGoal), what he
\ does there (Pursuer_PickFromTable over Debilitas_AttackTable's "searched": walk on, or look
\ about), and his mode with its clock (Pursuer_ModesSearching).
IN: stalker.search
USING: engine game-state actors messages common facts flag-names paths vectors stalker.state stalker.senses stalker.moving stalker.spots stalker.head stalker.names stalker.anims floor-flags ;

: here-played? ( -- flag )  in-game @ if  self body-room room-id =  else  false  then ;
\ his search's phase: none, walking to a stop, searching it
enum: search-phase#  no-search to-stop at-the-stop ;

\ ---- his route (8 stops: a triangle, and whether it was picked at random) ----
: stop ( k -- addr )  2* cells stops + ;
: route-clear ( -- )   \ Pursuer_ClearRoute
    8 0 do  -1 i stop !  0 i stop cell+ !  loop  0 route-next !  0 route-end ! ;
: stops-left ( -- n )  route-end @ route-next @ - ;
: route-add ( tri random? -- )   \ Npc_RouteAdd (full, or no triangle: not added)
    over 0<  route-end @ 8 < 0= or if  2drop exit  then
    route-end @ stop  tuck cell+ !  !  1 route-end +! ;
\ Npc_RandomTri: a triangle of the room he may stand on (his floor; not both sides of a divider)
: random-tri ( -- tri )
    here-played? 0= nav-tris 0= or if  -1 exit  then
    1000 0 do
        rnd nav-tris s>f f* f>s  nav-tris 1- min
        dup nav-flags self body-mask and 0=  over nav-flags both-halves and both-halves <> and if  unloop exit  then
        drop
    loop  -1 ;
\ Pursuer_AddSearchStops: n stops - each 60 in 100 the room's next search spot while it has
\ more, else a triangle at random
: add-stops ( n -- )
    room-id search-spots {: n spots count | used :}
    n 0 ?do
        spot-next @ count < 0= if  0 spot-next !  then
        used count <  rnd 100e f* 60e f<= and if
            spot-next @ cells spots + @ 0 route-add  1 spot-next +!  used 1+ to used
        else  random-tri -1 route-add  then
    loop ;
: search-stops ( -- n )  rnd 4e f* f>s 2 + ;   \ (searching: 2..5 stops)

\ ---- his mode's clock (vtable +0x2BC .. +0x2C4: after her his setup's frames - Pursuer_Caught's
\ +0x16D4: Debilitas 300, hard 540 -, searching 600, waiting 900) ----
: after-t ( -- )  after-frames @ mode-t ! ;
: mode! ( m -- )
    dup my-mode !  case
        after-her of  after-t  endof
        searching of  600 mode-t !  endof
        waiting-about of  900 mode-t !  endof
        held-off of  900 mode-t !  endof   \ (held off: Debilitas 900)
    endcase ;

\ ---- the steps (one runs each frame: `step`) ----
: step! ( xt -- )  step !  0 step-done !  0 step-t ! ;
: stand ( -- )   \ (vtable +0x320)
    still-anim play  ['] noop step!  after-her-act action-look! ;
\ his walk for his mode (Debilitas_StandAnim, vtable +0x128): after her, close (80 on foot) and
\ she isn't hiding, the fast one, else the slow; waiting the fast one; searching or heading for
\ her room the slow
: mode-walk ( -- anim )
    my-mode @ case
        after-her of  base-chase @ 0=  d-fiona f@ 80e f<= and d-fiona f@ f0< 0= and  fiona-hidden state-flag? 0= and if  walk-fast  else  walk-slow  then  endof
        waiting-about of  walk-fast  endof
        >r walk-slow r>
    endcase ;

\ Pursuer_CloseInGoal: along his way; slowed to a stand within 10 of the goal; over once the
\ stand has faded in (a stop picked at random: at most 150 frames' walk)
: walking ( -- )
    walk-stride drop
    way-left 10e f<  anim @ walk-slow = and if  still-anim play  then
    anim @ still-anim = settled? and  my-path path-left? 0= or if  -1 step-done !  then ;
\ the turn on the spot: turned by it until it ends or he faces the way, then his walk
create to-at /vec allot
: turning ( -- )
    root-move
    settled? 0= if  exit  then
    ended?  to-at heading-to yaw f- angle-wrap fabs 0.1e f< or if
        mode-walk play  ['] walking step!
    then ;
\ Pursuer_StepBack: setting off - turned on the spot toward the way's next point, or walking
: set-off ( -- )
    set-off-act action-look!
    0.1e my-path my-at path-ahead drop  to-at ahead vec-copy
    to-at way-to dup no-turn = if  drop mode-walk play  ['] walking step!  exit  then
    turn-base + play-now  ['] turning step! ;

\ the look about at a stop (Pursuer_StateWalkGesture: Debilitas_AttackTable's gestures) to its
\ end; or standing a moment (Pursuer_WalkAside)
: gesturing ( -- )   root-move  ended? if  -1 step-done !  then ;
: standing ( -- )   1 step-t +!  settled? step-t @ 30 > and if  -1 step-done !  then ;
: searched ( -- )   \ (50 walk on, 20, 20 and 10 a look about)
    rnd 100e f*
    fdup 50e f< if  fdrop -1 gesture !  still-anim play  ['] standing step!  exit  then
    fdup 70e f< if  fdrop look-about-anim  else  90e f< if  look-about-2-anim  else  look-about-3-anim  then  then
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
\ the route over, he stands (his mode moves on) ----
: to-next-stop ( -- )
    aim if  set-off  to-stop search-phase !  else  stand  no-search search-phase !  then ;
: at-stop ( -- )
    1 route-next +!  0 stop-wait !
    route-next @ route-end @ < 0= if  -1 route-done !  then
    at-the-stop search-phase !  searched ;
: search ( -- )   \ (each frame, after his step)
    search-phase @ case
        to-stop of
            stop-wait @ if  -1 stop-wait +!  stop-wait @ 0= if  -1 step-done !  then  then
            step-done @ if  at-stop  then
        endof
        at-the-stop of
            step-done @ if  route-done @ if  stand no-search search-phase !  else  to-next-stop  then  then
        endof
    endcase ;
: search-start ( -- )  0 route-done !  to-next-stop ;

\ ---- to where she is (Pursuer_ChaseFiona: after her but not seeing her): toward her on foot,
\ his way re-planned every 30 frames; standing facing her within 15. Seeing her, his chase
\ (stalker/chase.fs: `chase-begin`) ----
defer round-to-her ( -- went? )   ' false is round-to-her   \ (she across his room: stalker/doorway.fs)
defer out-of-reach ( -- went? )   ' false is out-of-reach   \ (no way to her on foot: round by a ladder - stalker/ladder.fs)
defer at-a-ladder? ( -- flag )   ' false is at-a-ladder?   \ (his ladder action - stalker/ladder.fs)
defer leave-at-random ( -- )   ' noop is leave-at-random   \ (out by an exit at random: stalker/doorway.fs)
defer out-cancelled ( -- )   ' noop is out-cancelled      \ (his way out given up: stalker/doorway.fs)
: toward-her ( -- )
    -1 step-t +!  step-t @ 0> if  exit  then  30 step-t !
    round-to-her if  exit  then
    fiona body-tri dup 0< if  drop exit  then  fiona body-at plan 0> 0= if  out-of-reach drop exit  then   \ (none: round by a ladder -
    \ Pursuer_WalkToGoal)
    d-fiona f@ 15e f< d-fiona f@ f0< 0= and if  still-anim play  my-path path-clear  else  mode-walk play  then
    her-unseen @  d-fiona f@ 10e f<  d-fiona f@ f0< 0= and and  my-mode @ after-her = and if  0 mode-t !  then ;   \ (there, and she's not: Pursuer_BehaviourRun)
: chase-step ( -- )   \ (the step while after her)
    my-path path-left? if  walk-stride drop  else  fiona body-at turn-rate turn-to fdrop  then
    toward-her ;
: chase-start ( -- )  after-her-act action-look!  no-search search-phase !  ['] chase-step step!  toward-her ;
defer chase-begin ( -- )   ' chase-start is chase-begin   \ (seeing her: stalker.chase)
defer chasing? ( -- flag )   ' false is chasing?
defer lost-her ( -- )   ' chase-start is lost-her

\ ---- Pursuer_ModesSearching, in the room being played (elsewhere: with his travel): seeing her
\ he's after her and his clock starts again; out of sight it runs down, then he searches the
\ room (2..5 stops); his route over he waits about - out of the room by an exit at random
\ (Pursuer_GoAfterFiona); heading for her room he searches it once in; held off he's after her
\ again when it runs out ----
: run-pause? ( -- paused? )   \ (the run: coming in, he waits a while first - Pursuer_ThroughDoorEnding)
    run-pause @ 0> 0= if  false exit  then  -1 run-pause +!  true ;
defer after-her-elsewhere ( -- went? )   ' false is after-her-elsewhere   \ (the run: out after her - stalker/doorway.fs)
: tick-down ( -- out? )  mode-t @ 0> if  -1 mode-t +!  then  mode-t @ 0= ;
defer lead-taken? ( -- flag )   ' false is lead-taken?   \ (a lead he heard: stalker/leads.fs)
defer starting? ( -- flag )   ' false is starting?      \ (starting at one)
defer leads-forgotten ( -- )   ' noop is leads-forgotten
: search-again ( -- )  leads-forgotten  -1 drawn-tri !  searching mode!  route-clear  search-stops add-stops  search-start ;
\ the doll (Thing03_StalkerCheck): searching the room being played, he sees it and has a way
\ there - it is his one stop, his search going on from it
create drawn-at /vec allot
: go-for-doll ( tri -- )
    here-played? 0=  my-mode @ heading-for-her held-off within 0= or  over drawn-tri @ = or if  drop exit  then
    dup tri-center drawn-at vec!
    drawn-at in-view? 0= if  drop exit  then
    dup drawn-at plan 0> 0= if  drop exit  then
    dup drawn-tri !  searching mode!  route-clear  0 route-add  search-start ;
\ down (+0xC4 2): waiting about, nothing else (Pursuer_ModesSearching - in the room or out of it)
: down-waits? ( -- flag )
    cond @ down-cond = dup if  my-mode @ waiting-about <> if  waiting-about mode!  then  then ;
: modes ( -- )
    here-played? 0= if  exit  then
    down-waits? if  exit  then
    at-a-ladder? if  exit  then   \ (to a ladder or on it: over first)
    reeling @ at-dog @ or if  exit  then   \ (struck: his reaction first; after Hewie)
    entering @ if  exit  then   \ (coming in by a shut door: in first - opened, or barged open: action $C)
    out-going @ if   \ (on his way out by a door: that first - unless he's only wandering off, when the
        wandering-out @ 0=  sees-fiona @ did-hear @ or 0=  or if  exit  then   \ sight of her or a noise
        out-cancelled  0 wandering-out !                                     \ turns him back: Pursuer_PickTarget)
    then
    start-t @ 0> if  starting? if  exit  then  0 start-t !  then   \ (starting at something he heard or saw: that
    \ first - unless something else has him now)
    sees-fiona @  my-mode @ held-off <> and if   \ (held off: going away, not after her)
        my-mode @ after-her <> if  after-her mode!  route-clear  then
        chasing? 0= if  chase-begin  then  after-t exit
    then
    chasing? if  lost-her  then   \ (lost sight of her: to where she is)
    hewie-played? if   \ (the run: she is elsewhere - his chase follows her trail out, Pursuer_Modes / BehaviourFollow)
        run-pause? if  exit  then
        after-her-elsewhere if  exit  then
    then
    lead-taken? if  exit  then
    my-mode @ case
        after-her of  tick-down if  search-again  then  endof
        heading-for-her of  search-again  endof
        searching of  route-done @ if  0 route-done !  waiting-about mode!  leave-at-random  then  endof   \ (his route over:
        \ Pursuer_GoAfterFiona - Pursuer_PlanWhere's mode 3 - out of the room by an exit at random)
        waiting-about of  route-done @ if  0 route-done !  leave-at-random  then  endof   \ (no clock here: a lead or
        \ the sight of her moves him on; his route over, out as above)
        held-off of  endof   \ (held off he keeps to it, on his way home - no clock: Pursuer_ModesSearching's
    endcase ;              \ mode 4 waits for his clock to be emptied, by Debilitas_CarryOn finding no way home)
