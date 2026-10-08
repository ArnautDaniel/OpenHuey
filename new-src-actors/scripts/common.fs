\ common.fs - small helpers every subsystem uses: random numbers, headings, steps.
IN: common
USING: engine ;

variable seed  12345 seed !

: random ( n -- 0..n-1 )   \ (a plain linear generator; the original's gRandom: later, if the odds must match)
    seed @ 1103515245 * 12345 + $7FFFFFFF and dup seed !  16 rshift swap mod ;
: rnd ( F: -- r )  32768 random s>f 32768e f/ ;   \ 0 <= r < 1
: vlen ( F: x z -- l )  fsq fswap fsq f+ fsqrt ;
\ a heading turned toward `target` by at most `step`: the new heading, and how far it still is
fvariable tt-target  fvariable tt-step
: turn-toward ( F: yaw target step -- yaw' left )
    tt-step f!  tt-target f!
    tt-target f@ fover f- angle-wrap                         ( F: yaw d )
    fdup fabs tt-step f@ f<= if  fdrop fdrop tt-target f@ 0e exit  then
    f0< if  tt-step f@ f-  else  tt-step f@ f+  then  angle-wrap
    fdup tt-target f@ fswap f- angle-wrap fabs ;
\ a step (x, z) in something's own frame turned into the room by its heading
fvariable ry-x  fvariable ry-z  fvariable ry-a
: rotate-by ( F: x z yaw -- x' z' )
    ry-a f!  ry-z f!  ry-x f!
    ry-x f@ ry-a f@ fcos f*  ry-z f@ ry-a f@ fsin f* f+
    ry-z f@ ry-a f@ fcos f*  ry-x f@ ry-a f@ fsin f* f- ;
