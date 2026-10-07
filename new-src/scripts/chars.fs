\ chars.fs - the characters' bodies in the room, as the original's Actor / Character
\ (src/game/actor.c): moving over the nav mesh within each one's blocked flags, turning,
\ walking-straight tests, the free distance ahead, planned paths and following them. A
\ character is an event-state slot (game-state); its actor is its body.
IN: chars
USING: engine game-state events.core ;

: c-ok? ( cs -- flag )  dup 0< 0= swap characters < and ;
: c-actor ( cs -- actor | -1 )  dup c-ok? 0= if  drop -1 exit  then  character char.actor sl@ ;
: c-pos ( cs -- v )  character char.pos ;
: c-active? ( cs -- flag )  dup c-ok? 0= if  drop false exit  then  character char.present sl@ 0<> ;
: c-room ( cs -- room )  character char.room sl@ ;
: c-cond ( cs -- n )  character char.cond sl@ ;
: c-mode ( cs -- n )  character char.mode sl@ ;
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
\ the triangle under it it may stand on (else any)
: c-find-tri ( cs -- tri )  dup c-pos over c-mask v-tri-in  dup 0< if  drop c-pos v-tri  else  nip  then ;
: c-place! ( cs -- ) ( F: x y z -- )  dup c-pos vec!  dup c-find-tri over c-tri!  c-sync ;

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
    dup -1 = if  drop 2 pick c-mask  then  >r >r >r
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
\ the way's end made where it is now (the original's +0x124 = +0x128: nothing left to follow)
: c-path-end ( cs -- )  dup path-i swap cells c-path-n + ! ;
\ how far along the rest of the way it still has (the planner's +0x3C: from where it is through
\ the points left)
create pr-at 12 allot
: path-rest ( cs -- ) ( F: -- d )
    0e  dup c-pos pr-at swap vec-copy
    dup path-n over path-i ?do
        dup i path-point  pr-at over vec-dist-xz f+  pr-at swap vec-copy
    loop  drop ;

\ ---- bodies against each other (Actor_Touching / Actor_PushOut) ----
: c-radius ( cs -- ) ( F: -- r )  character char.radius sf@ ;
: c-height ( cs -- ) ( F: -- h )  character char.height sf@ ;
: c-size! ( cs -- ) ( F: radius height -- )  dup character char.height sf!  character char.radius sf! ;
: c-here? ( cs -- flag )   \ in the game and not hidden
    dup c-ok? 0= if  drop false exit  then
    dup character char.present sl@ 0= if  drop false exit  then  character char.disabled sl@ 0= ;
\ Actor_Touching: within each other's height (the lower one's, and `vmargin`), their radii
\ (and `margin`) apart
fvariable tc-m  fvariable tc-v  fvariable tc-ya  fvariable tc-yb
: c-touching? ( a b -- flag ) ( F: margin vmargin -- )
    tc-v f!  tc-m f!
    over c-here? over c-here? and 0= if  2drop false exit  then
    over c-pos 4 + sf@ tc-ya f!  dup c-pos 4 + sf@ tc-yb f!
    tc-ya f@ tc-yb f@ f< if
        tc-yb f@ tc-ya f@ f-  over c-height tc-v f@ f+ f> if  2drop false exit  then
    else
        tc-ya f@ tc-yb f@ f-  dup c-height tc-v f@ f+ f> if  2drop false exit  then
    then
    over c-pos over c-pos vec-dist-xz  c-radius c-radius f+ tc-m f@ f+ f<= ;
\ Actor_PushOut: `a` put just outside `b` - their radii apart, from b toward a; else the nearest
\ free place round b, 2 degrees at a time either way; 0, or -1 when there is none
create po-at 12 allot  fvariable po-x  fvariable po-z  fvariable po-ax  fvariable po-az
variable po-a  variable po-b
: po-turned ( F: ang -- )   \ po-at = b + the offset turned by ang
    fdup fcos po-x f@ f* fover fsin po-z f@ f* f+  po-b @ c-pos sf@ f+  po-at sf!
    fdup fcos po-z f@ f* fswap fsin po-x f@ f* f-  po-b @ c-pos 8 + sf@ f+  po-at 8 + sf! ;
: po-try ( -- placed? ) ( F: ang -- )
    po-turned  po-a @ po-at -1 c-tri-to dup 0< if  drop false exit  then
    po-a @ c-pos po-at vec-copy  po-a @ c-tri!  po-a @ c-sync  true ;
: c-push-out ( a b -- 0|-1 )
    po-b !  po-a !
    po-a @ c-pos sf@ po-b @ c-pos sf@ f- po-x f!
    po-a @ c-pos 8 + sf@ po-b @ c-pos 8 + sf@ f- po-z f!
    po-x f@ fsq po-z f@ fsq f+ fsqrt fdup f0= if  fdrop 0e po-x f!  1e po-z f!  else
        po-x f@ fover f/ po-x f!  po-z f@ fswap f/ po-z f!  then
    po-a @ c-radius po-b @ c-radius f+ 0.01e f+  fdup po-x f@ f* po-x f!  po-z f@ f* po-z f!
    po-b @ c-pos 4 + sf@ po-at 4 + sf!
    0e po-try if  0 exit  then
    91 1 do
        i 2* s>f deg>rad fnegate po-try if  0 unloop exit  then
        i 2* s>f deg>rad po-try if  0 unloop exit  then
    loop  -1 ;

\ Doors_HasExit (gDoors +0x40): the room has a door at that exit
: has-door? ( exit -- flag )  room-door-at >r fdrop fdrop fdrop r> ;

\ ---- what isn't ported yet (the stalkers', Fiona's): says so once on the console ----
create told 64 32 * allot  variable ntold   \ (the names told: 31 characters each)
: told# ( i -- addr )  32 * told + ;
: told$ ( i -- addr len )  told# dup 1+ swap c@ ;
: (not-yet) ( addr len -- )
    31 min
    ntold @ 0 ?do  i told$ 2over compare 0= if  2drop unloop exit  then  loop
    ntold @ 64 >= if  2drop exit  then
    dup ntold @ told# c!  ntold @ told# 1+ swap move  1 ntold +!
    ." not yet: " ntold @ 1- told$ type cr ;
: not-yet" ( "text" -- )  postpone s" postpone (not-yet) ; immediate
