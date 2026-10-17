\ common.fs - small helpers every subsystem uses: random numbers, headings, steps.
IN: common
USING: engine room-names ;

variable seed  12345 seed !

: random ( n -- 0..n-1 )   \ (a plain linear generator; the original's gRandom: later, if the odds must match)
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
