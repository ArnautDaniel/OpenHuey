\ common.fs - small helpers every subsystem uses: random numbers, headings, steps.
IN: common
USING: engine room-names ;

variable seed  12345 seed !

: random ( n -- r )   \ (0 .. n-1) (a plain linear generator; the original's gRandom: later, if the odds must match)
    seed @ 1103515245 * 12345 + $7FFFFFFF and dup seed !  16 rshift swap mod ;
: rnd ( F: -- r )  32768 random s>f 32768e f/ ;   \ 0 <= r < 1
: vlen ( F: x z -- l )  fsq fswap fsq f+ fsqrt ;
\ a heading turned toward `target` by at most `step`: the new heading, and how far it still is
: turn-toward ( F: yaw target step -- yaw' left )
    {: F: yaw target step | F: d :}
    target yaw f- angle-wrap to d
    d fabs step f<= if  target 0e exit  then
    yaw  d f0< if  step f-  else  step f+  then  angle-wrap
    fdup target fswap f- angle-wrap fabs ;
\ the rooms whose water the characters' steps ripple (Character_WaterStep's callers: rooms 7 and
\ $106; Fiona's and Hewie's steps, and a stalker's fall, also in $D1)
: flooded? ( -- flag )  room-id dup castle-b1-4 = swap chaos-forest-8 = or ;
: wading? ( -- flag )  flooded? room-id off-map-d1 = or ;
\ a step (x, z) in something's own frame turned into the room by its heading
: rotate-by ( F: x z yaw -- x' z' )
    {: F: x z yaw :}
    x yaw fcos f*  z yaw fsin f* f+
    z yaw fcos f*  x yaw fsin f* f- ;
\ the value at `addr` moved toward a target by at most `step` (Motion_EaseTilt)
: ease ( addr -- ) ( F: target step -- )
    fabs {: a F: goal step :}
    a f@ goal f- fabs step f<= if  goal a f! exit  then
    a f@ goal f> if  a f@ step f-  else  a f@ step f+  then  a f! ;
\ a door's triangle on a room's side (Rooms_DoorTri: the original's own measure of a link), -1 none
: door-tri ( door room -- tri )
    {: door room :}  door 0< if  -1 exit  then
    door door-sides {: r0 e0 t0 r1 e1 t1 :}
    r1 room = if  t1  else  r0 room = if  t0  else  -1  then then ;
\ a number 0..99 as two digits at `at`
: two-digits ( n at -- )  >r  10 /mod [char] 0 + r@ c!  [char] 0 + r> 1+ c! ;
