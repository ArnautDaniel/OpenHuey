\ chars.fs - the characters' bodies in the room, as the original's Actor / Character
\ (src/game/actor.c): moving over the nav mesh within each one's blocked flags, turning,
\ walking-straight tests, the free distance ahead, planned paths and following them. A
\ character is an event-state slot (game-state); its actor is its body.
IN: chars
USING: engine game-state events.core ;

: c-ok? ( cs -- flag )  dup 0< 0= swap characters < and ;
: c-actor ( cs -- actor | -1 )  dup c-ok? 0= if  drop -1 exit  then  character char.actor sl@ ;
: c-pos ( cs -- v )  character char.pos ;
: c-tri ( cs -- tri )  character char.tri sl@ ;
: c-tri! ( tri cs -- )  character char.tri l! ;
: c-yaw ( cs -- ) ( F: -- a )  c-actor dup 0< if  drop 0e exit  then  actor act.yaw sf@ ;
: c-yaw! ( cs -- ) ( F: a -- )  c-actor dup 0< if  drop fdrop exit  then  actor act.yaw sf! ;

\ each one's blocked flags (navMask +0xC0: Fiona 0x28020018, Hewie 0x29020008)
create c-masks  characters cells allot  c-masks characters cells 0 fill
$28020018 0 cells c-masks + !  $29020008 1 cells c-masks + !
: c-mask ( cs -- mask )  cells c-masks + @ ;
: c-mask! ( mask cs -- )  cells c-masks + ! ;

\ the body follows the character's place
: c-sync ( cs -- )
    dup c-actor dup 0< if  2drop exit  then  actor >r
    c-pos vec@  r@ act.z sf!  r@ act.y sf!  r> act.x sf! ;
\ put it at a point (its triangle found under it)
: c-place! ( cs -- ) ( F: x y z -- )  dup c-pos vec!  dup c-pos v-tri over c-tri!  c-sync ;

\ Actor_Move: by (dx, dz) as far as its blocked flags allow, sliding along walls
: c-move ( cs -- ) ( F: dx dz -- )
    dup c-pos over c-mask v-nav-move  dup 0< if  2drop exit  then  over c-tri!  c-sync ;
\ Actor_MoveAny: the same through any triangle
: c-move-any ( cs -- ) ( F: dx dz -- )
    dup c-pos 0 v-nav-move  dup 0< if  2drop exit  then  over c-tri!  c-sync ;
\ a move by a step given in its own frame (x to its right, z ahead)
fvariable mx  fvariable mz  fvariable my
: c-move-local ( cs -- ) ( F: x z -- )
    mz f! mx f!
    dup c-yaw fdup fcos mx f@ f* fover fsin mz f@ f* f+       ( F: yaw dx )
    fswap fdup fcos mz f@ f* fswap fsin mx f@ f* f-          ( F: dx dz )
    c-move ;

\ Actor_TurnToward: toward heading `target` by at most `step`; how far it still is (0 there)
fvariable tt-target  fvariable tt-step
: c-turn-toward ( cs -- ) ( F: target step -- rest )
    tt-step f!  tt-target f!
    tt-target f@ dup c-yaw f- angle-wrap                         ( cs ) ( F: d )
    fdup fabs tt-step f@ f<= if  fdrop tt-target f@ c-yaw! 0e exit  then
    f0< if  dup c-yaw tt-step f@ f-  else  dup c-yaw tt-step f@ f+  then  angle-wrap
    dup c-yaw!  tt-target f@ c-yaw f- angle-wrap fabs ;

\ Actor_HeadingTo: from it to a point (its own heading when right above / below it)
: c-heading-to ( cs v -- ) ( F: -- yaw )
    over c-pos over vec-dist-xz f0= if  drop c-yaw exit  then
    >r c-pos r> vec-heading ;
\ Actor_Distance
: c-dist-to ( cs v -- ) ( F: -- d )  >r c-pos r> vec-dist ;

\ Actor_TriTo: walking straight from it to v (its blocked flags, mask -1; 0 none; or a mask):
\ v's triangle, or -1 when a wall comes first
: c-tri-to ( cs v mask -- tri )
    dup -1 = if  drop over c-mask  then  >r
    over c-tri rot c-pos rot r> v-walk ;
\ Actor_FreeDistance: free along heading `yaw` up to `d` from it (its own mask for -1)
: c-free ( cs mask -- ) ( F: yaw d -- free )
    dup -1 = if  drop dup c-mask  then  >r  dup c-tri swap c-pos r> v-free ;

\ ---- planned paths (Character_PlanPathKind, the waypoints +0x12C, followed by
\ Character_FollowWaypoints) ----
64 constant path-max
create c-paths  characters path-max * 12 * allot   \ each character's turning points
create c-path-n  characters cells allot  c-path-n characters cells 0 fill
create c-path-i  characters cells allot  c-path-i characters cells 0 fill
: path-point ( cs i -- v )  swap path-max * + 12 * c-paths + ;
: path-n ( cs -- n )  cells c-path-n + @ ;
: path-i ( cs -- i )  cells c-path-i + @ ;
: path-i! ( i cs -- )  cells c-path-i + ! ;
: path-left? ( cs -- flag )  dup path-i swap path-n < ;
\ plan a way to v on triangle `tri`, kept off `mask` (-1: its own): the number of points (0: none)
: c-plan ( cs tri v mask -- n )
    dup -1 = if  drop 3 pick c-mask  then  >r >r >r
    dup c-tri over c-pos r> r> r> v-path                        ( cs n )
    dup 0 ?do  over i path-point  i nav-path-point  vec!  loop
    over cells c-path-n + !  0 over path-i!  path-n ;
: c-path-clear ( cs -- )  0 over cells c-path-n + !  0 swap path-i! ;
\ Character_WaypointAhead: the point `d` along the rest of the way from it (without moving)
\ into pa-pos; the index of the next turning point then
create pa-pos 12 allot  fvariable pa-left
: path-ahead ( cs -- i ) ( F: d -- )
    pa-left f!  dup c-pos pa-pos swap vec-copy  dup path-i            ( cs i )
    begin  2dup swap path-n < while
        2dup path-point >r  pa-pos r@ vec-dist-xz                   ( F: seg )
        fdup pa-left f@ f<= if
            pa-left f@ fswap f- pa-left f!  pa-pos r> vec-copy  1+
        else
            fdrop  pa-pos r@ vec-heading  pa-left f@  pa-pos pa-pos vec-ahead  r> drop
            nip exit
        then
    repeat  nip ;
\ Character_FollowWaypoints: on along the way by `speed`, facing the way moved; true while
\ points are left
: c-follow ( cs -- more? ) ( F: speed -- )
    dup path-left? 0= if  fdrop drop false exit  then
    dup path-ahead                                                     ( cs i )
    over c-pos pa-pos vec-dist-xz f0= 0= if  over c-pos pa-pos vec-heading  over c-yaw!  then
    over path-i!  pa-pos vec@ dup c-place!  path-left? ;
