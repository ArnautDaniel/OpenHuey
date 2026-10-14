\ creatures/trip.fs - a creature off screen (src/game/creature.c CreatureA_FionaLeft / _Travel /
\ _StateRoute, the same in CreatureB): where it is, the exit she took that it walks to, then door
\ by door toward her room, each leg counted down at its pace (a leg's length the original's own
\ measure: the door's triangle number on that side); a shut door ends the trip, and isn't tried
\ again until no way is left. Each creature keeps a trip record in its state (`/trip field`).
IN: creatures.trip
USING: engine game-state actors facts ;

\ ---- the record ----
: t.room ( t -- a )  ;                   \ the room it is in
: t.tri ( t -- a )  cell+ ;              \ its triangle there (-1: none known)
: t.exit ( t -- a )  2 cells + ;         \ the exit it walks to (-1: none)
: t.leg ( t -- a )  3 cells + ;          \ what is left of this leg (a single float)
: t.n ( t -- a )  4 cells + ;            \ its route's doors, and the next
: t.i ( t -- a )  5 cells + ;
: t.doors ( t -- a )  6 cells + ;        \ 16 of them
: t.closed ( t -- a )  22 cells + ;      \ the doors found shut: a bit each (13 cells)
: t.door ( t -- a )  35 cells + ;        \ the exit whose door it waits by (-1: none)
: t.at-door ( t -- a )  36 cells + ;     \ 0 not, 1 waiting by it while it swings, 2 through it
37 cells constant /trip

: trip-reset ( t -- )  -1 over t.exit !  0 over t.n !  0 over t.i !  -1 over t.door !  0 swap t.at-door ! ;
: in-play? ( t -- flag )  t.room @ room-id = ;
: route-over? ( t -- flag )  dup t.i @ swap t.n @ < 0= ;
: next ( t -- door )  dup t.i @ cells swap t.doors + @ ;

: door-measure ( door room -- ) ( F: -- n )
    swap door-sides  {: room r0 e0 t0 r1 e1 t1 :}
    r0 room = if  t0  else  r1 room = if  t1  else  -1  then  then  0 max s>f ;
: leg-down ( t -- done? ) ( F: by -- )  t.leg dup sf@ fswap f-  0e fmax fdup sf!  f0= ;
: closed! ( t door -- )  swap t.closed  over 5 rshift cells +  swap 31 and 1 swap lshift  over @ or  swap ! ;
: plan ( t -- )   \ its route to her room: its doors, the first leg
    {: t :}
    t t.room @ room-id 0 -1 t t.closed route-avoiding
    dup 0> 0= if  drop  t t.closed 13 cells 0 fill  0 t t.n !  exit  then
    16 min dup t t.n !  0 do  i route-door  i cells t t.doors + !  loop  0 t t.i !
    t next  t t.room @ door-measure  t t.leg sf! ;
\ into the room beyond `exit`: in the room being played, on the exit's spot, facing in (true);
\ elsewhere its next leg
: through ( t exit -- came-in? )
    {: t ex :}
    t t.room @ ex room-exit-leads                                  ( room' exit' )
    over 0< if  2drop  0 t t.n !  false exit  then
    swap t t.room !  -1 t t.tri !  -1 t t.exit !                     ( exit' )
    t in-play? if
        1 exit-spot dup 0< if  drop fdrop fdrop fdrop  true exit  then   \ (no spot: placed by its owner)
        dup t t.tri !  room-id swap body-place  true exit
    then  drop
    t route-over? 0= if  t next  t t.room @ door-measure  t t.leg sf!  then  false ;
: next-door ( t -- came-in? )
    {: t | door :}  t next to door
    door door-open? 0= if  t door closed!  t t.i @ t t.n !  false exit  then   \ (shut: the trip ends)
    1 t t.i +!  t  door t t.room @ door-exit-in  through ;
\ a frame of it: to her exit at `walk`, its legs at `pace`; true when it has come into the room
\ being played
: trip-step ( t -- came-in? ) ( F: walk pace -- )
    {: t :}
    t t.exit @ 0< 0= if  fdrop  t leg-down if  t  t t.exit @  through  else  false  then  exit  then
    fswap fdrop
    t route-over? if  fdrop  t plan  false exit  then
    t leg-down if  t next-door  else  false  then ;
\ she left by `exit`: to its spot first (the leg: how far it stands from it)
create spot 12 allot
: follow-out ( t exit -- )
    {: t ex :}  ex t t.exit !  0 t t.n !  0 t t.i !
    ex 0 exit-spot drop spot vec!  self body-at spot vec-dist-xz  t t.leg sf! ;

\ ---- a door it stands by, swinging (CreatureA_FirstDoor / _Think's +0x84, CreatureB_HeadForDoor):
\ standing in an exit's area as its door swings, it waits there; the door still again and
\ standing open, it carries on; left shut, it goes through after whoever used it ----
: by-exit? ( exit v -- flag )  >r exit-area dup area-count < if  r> vec@ area-in?  else  r> 2drop false  then ;
: swinging-by ( v -- exit | -1 )   \ (the first)
    8 0 do  i door-here? if  i door-animating? if  i over by-exit? if  drop i unloop exit  then  then  then  loop
    drop -1 ;
: door-way-open? ( t -- flag )  dup t.room @ swap t.door @ room-exit-door door-open? ;
\ this frame in the room being played, at `v`: true while it waits by the door; through it, it
\ has gone (its body off - the caller's)
: door-watch ( t v -- waiting? )
    {: t v :}
    v swinging-by dup 0< 0= if  t t.door !  1 t t.at-door !  else  drop  then
    t t.at-door @ 1 <> if  false exit  then
    t door-way-open? 0= if  2 t t.at-door !  t  t t.door @  through drop  true exit  then
    t t.door @ door-animating? if  true exit  then
    0 t t.at-door !  -1 t t.door !  false ;
: gone-through? ( t -- flag )  t.at-door @ 2 = ;

