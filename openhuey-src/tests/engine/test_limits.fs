\ The engine's words at their edges (the C sweep, 2026-11): an angle or a motion's time that taking a
\ turn off at a time can't bring in - not finite, or many turns out - is brought in all the same,
\ never a hang (core/mathx.h angle_near, time_near). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/engine/test_limits.fs
IN: test-limits
USING: tester engine actors game debug ;

: inf ( F: -- x )  1e 0e f/ ;
: within-turn? ( F: a -- flag )  fabs 3.1416e f<= ;

testing angle-wrap: many turns out, or not finite - within a turn; a turn out as before
T{ 1e30 angle-wrap within-turn?  -1e30 angle-wrap within-turn?  inf angle-wrap within-turn? -> -1 -1 -1 }T
T{ 7e angle-wrap  7e 6.28318531e f-  f- fabs 1e-6 f< -> -1 }T

full-suite   \ (on: waits, scenes, walks - the full run's)
free-play  10 run-frames
s" O_FIN/FIN_000" actor-load constant model
model $0200 0 1 motion-play   \ (looping)
: frame ( F: -- t )  model actor act.frame sf@ ;
: in-motion? ( F: t -- flag )  fdup f0< 0=  model motion-frames s>f f< and ;
testing a looping motion at a rate past all reckoning, or an endless one: its time still in the motion
T{ 1e30 model actor act.rate sf!  3 run-frames  frame in-motion? -> -1 }T
T{ inf model actor act.rate sf!  3 run-frames  frame in-motion? -> -1 }T
1e model actor act.rate sf!  model actor-free

test-summary
