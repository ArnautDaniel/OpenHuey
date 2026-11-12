\ creatures/trip.fs - a creature off screen (src/game/creature.c CreatureA_FionaLeft / _Travel /
\ _StateRoute, the same in CreatureB): its path walked to the exit she took, its body moving in the
\ room it was left in (`walk-on`), then door by door toward her room, each leg counted down at its
\ pace (a leg's length the original's own measure: the door's triangle number on that side); a shut
\ door ends the trip, and isn't tried again until no way is left. Each creature keeps a trip record
\ in its state (`/trip field`).
IN: creatures.trip
USING: engine game-state actors facts vectors paths ;

\ ---- time: the game's 30 frames a second ----
: minutes ( n -- frames )  1800 * ;

\ ---- the record ----
16 constant most-doors
begin-structure /trip
  field: t.room                          \ the room it is in
  field: t.tri                           \ its triangle there (-1: none known)
  sffield: t.leg                         \ what is left of this leg
  field: t.n  field: t.i                 \ its route's doors, and the next
  most-doors cells +field t.doors
  13 cells +field t.closed               \ the doors found shut: a bit each
  field: t.door                          \ the exit whose door it waits by (-1: none)
  field: t.at-door                       \ off-door, by-door (waiting while it swings), through-door
  field: t.walking                       \ (+0x61) walking its path off screen to the exit's far spot
  field: t.path                          \ that path (the creature's own record)
  field: t.last                          \ (+0x14C0) the door it last made for (-1: none)
  field: t.locks                         \ the doors' locks against creatures kept to (a homunculus: Progress_ExitPassable kind 2)
end-structure
enum: at-door#  off-door by-door through-door ;

: trip-reset ( t -- )
    0 over t.n !  0 over t.i !  -1 over t.door !  off-door over t.at-door !
    0 over t.walking !  -1 swap t.last ! ;
: trip-here? ( t -- flag )  t.room @ room-id = ;
: route-over? ( t -- flag )  dup t.i @ swap t.n @ < 0= ;
: next ( t -- door )  dup t.i @ cells swap t.doors + @ ;

: door-measure ( door room -- ) ( F: -- n )
    swap door-sides  {: room r0 e0 t0 r1 e1 t1 :}
    r0 room = if  t0  else  r1 room = if  t1  else  -1  then  then  0 max s>f ;
: leg-down ( t -- done? ) ( F: by -- )  t.leg dup sf@ fswap f-  0e fmax fdup sf!  f0= ;
: closed! ( t door -- )  swap t.closed  over 5 rshift cells +  swap 31 and 1 swap lshift  over @ or  swap ! ;
: plan-trip ( t -- )   \ its route to her room: its doors, the first leg
    {: t :}
    t t.room @ room-id 0 -1 t t.closed route-avoiding
    dup 0> 0= if  drop  t t.closed 13 cells 0 fill  0 t t.n !  exit  then
    most-doors min dup t t.n !  0 do  i route-door  i cells t t.doors + !  loop  0 t t.i !
    t next  dup t t.last !  t t.room @ door-measure  t t.leg sf! ;
\ into the room beyond `exit`: in the room being played, on the exit's spot, facing in (true);
\ elsewhere its next leg - a body it has there now in that room, where it was (CreatureB_ThroughExit
\ leaves its place as it was: no triangle)
: through ( t exit -- came-in? )
    {: t ex :}
    t t.room @ ex room-exit-leads                                  ( room' exit' )
    over 0< if  2drop  0 t t.n !  false exit  then
    swap t t.room !  -1 t t.tri !                                   ( exit' )
    self body? if  t t.room @ body-room!  -1 body-tri!  then
    t trip-here? if
        1 exit-spot dup 0< if  drop fdrop fdrop fdrop  true exit  then   \ (no spot: placed by its owner)
        dup t t.tri !  room-id swap body-place  true exit
    then  drop
    t route-over? 0= if  t next  t t.room @ door-measure  t t.leg sf!  then  false ;
\ the door at the end of a leg or of its walk (CreatureA_Travel, CreatureB_Travel): held by someone,
\ it waits there; shut - or, kept to them, locked against creatures - marked so and the trip over
\ (a spirit finds it shut before it waits, a homunculus waits first)
enum: pass#  pass-on pass-wait pass-shut ;
: shut-to? ( t door -- flag )  dup door-open? 0=  swap 2 doors:locked-against?  rot t.locks @ and  or ;
: door-ahead ( t door -- how )
    {: t door :}
    t t.locks @ if  door doors:door-held? if  pass-wait exit  then  then
    t door shut-to? if  pass-shut exit  then
    door doors:door-held? if  pass-wait exit  then
    pass-on ;
: next-door ( t -- came-in? )
    {: t | door :}  t next to door  door t t.last !
    t door door-ahead case
        pass-wait of  false  endof
        pass-shut of  t door closed!  t t.i @ t t.n !  false  endof   \ (the trip ends)
        >r  1 t t.i +!  t  door t t.room @ door-exit-in  through  r>
    endcase ;
\ walking its path off screen (Character_FollowWaypoints): its body moved `d` along it, turned
\ the way it goes; true at its end
create was /vec allot
: walk-on ( t -- done? ) ( F: d -- )
    t.path @ {: p :}
    p path-left? 0= if  fdrop true exit  then
    was self body-at vec-copy
    p was path-ahead p path-i!
    was ahead vec-dist-xz f0= 0= if  was ahead vec-heading body-turn  then
    ahead vec@  self body-room self body-tri body-place
    p path-left? 0= ;
\ its walk done: through the door it made for, on this side of it - none here, its trip from here;
\ held, it waits; shut, marked so and the trip over
: arrive ( t -- came-in? )
    {: t :}
    t t.last @ dup 0< if  drop -1  else  t t.room @ door-exit-in  then  {: ex :}
    ex 0< if  0 t t.walking !  -1 t t.tri !  false exit  then
    t  t t.last @ door-ahead case
        pass-wait of  false  endof
        pass-shut of  0 t t.walking !  t  t t.last @ closed!  -1 t t.tri !  0 t t.n !  false  endof
        >r  0 t t.walking !  t ex through  r>
    endcase ;
\ a frame of it: its path walked at `walk`, else its legs at `pace`; true when it has come into
\ the room being played
: trip-step ( t -- came-in? ) ( F: walk pace -- )
    {: t :}
    t t.walking @ if  fdrop  t walk-on if  t arrive  else  false  then  exit  then
    fswap fdrop
    t route-over? if  fdrop  t plan-trip  false exit  then
    t leg-down if  t next-door  else  false  then ;

\ ---- she leaves by `exit` (CreatureA_FionaLeft, CreatureB_FionaLeft): the exit's far spot (12 inside:
\ Rooms_ExitPointThrough) it walks to, its door spot (12 beyond: Rooms_ExitPointIn); nearer that than
\ she is (Actor_NearerRoom), it keeps the door it last made for; already as near the door as the far
\ spot and straight to it, its walk over at once ----
create far /vec allot   create door-at /vec allot
: far-spot ( exit -- tri )  2 exit-spot far vec! ;
: door-spot ( exit -- )  1 exit-spot drop door-at vec! ;
: nearer-than? ( exit v -- flag )  {: v :}  door-spot  self body-at door-at vec-dist  v door-at vec-dist f< ;
: trip-out ( t -- )   \ (afresh: no route, no door found shut)
    dup t.closed 13 cells 0 fill  0 over t.n !  0 over t.i !  0 swap t.walking ! ;
: straight-out ( t exit mask -- )   \ (its door made for: the walk over if straight on to it)
    {: t ex m :}
    t t.room @ ex room-exit-door t t.last !
    ex far-spot drop  ex door-spot
    self body-at door-at vec-dist  far door-at vec-dist f<= 0= if  exit  then
    self body-tri self body-at door-at m v-walk 0< 0= if  t t.path @ path-end  then ;

\ ---- a door it stands by, swinging (CreatureA_FirstDoor / _Think's +0x84, CreatureB_HeadForDoor):
\ standing in an exit's area as its door swings, it waits there; the door still again and
\ standing open, it carries on; left shut, it goes through after whoever used it ----
: by-exit? ( exit v -- flag )  >r exit-area dup area-count < if  r> vec@ area-in?  else  r> 2drop false  then ;
\ a triangle off every exit's area (its middle): somewhere to be put at random (CreatureA_PlaceRandom,
\ Actor_TeleportRandom)
create tri-mid /vec allot
: off-the-doors? ( tri -- flag )
    tri-center tri-mid vec!  8 0 do  i tri-mid by-exit? if  false unloop exit  then  loop  true ;
: swinging-by ( v -- exit | -1 )   \ (the first)
    8 0 do  i door-here? if  i door-animating? if  i over by-exit? if  drop i unloop exit  then  then  then  loop
    drop -1 ;
: door-way-open? ( t -- flag )  dup t.room @ swap t.door @ room-exit-door door-open? ;
\ this frame in the room being played, at `v`: true while it waits by the door; through it, it
\ has gone (its body off - the caller's)
: door-watch ( t v -- waiting? )
    {: t v :}
    v swinging-by dup 0< 0= if  t t.door !  by-door t t.at-door !  else  drop  then
    t t.at-door @ by-door <> if  false exit  then
    t door-way-open? 0= if  through-door t t.at-door !  t  t t.door @  through drop  true exit  then
    t t.door @ door-animating? if  true exit  then
    off-door t t.at-door !  -1 t t.door !  false ;
: gone-through? ( t -- flag )  t.at-door @ through-door = ;

