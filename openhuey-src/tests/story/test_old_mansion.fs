\ The fish's swim paths (the engine's swim-path / swim-step). The old mansion's room commands this
\ tested once - b1-3's plates and shakes, 2f-8's curtain, 1f-15's shelves - are their places' tests
\ now (tests/places/). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_old_mansion.fs
IN: test-old-mansion
USING: tester engine game-state actors messages common room-names game debug ;

free-play  20 run-frames

testing swim paths: a fish's way stays in its box (2 x 3 x 5 cells from (30, 0, 50)), turning 3.6 degrees a frame at most
: within? ( F: v lo hi -- ) ( -- flag )  frot fswap fover f>= >r f<= r> and ;
: in-box? ( F: x y z -- flag )  50e 100e within?  0e 30e within? and  30e 50e within? and ;
T{ 1 2 3 5 240  30e 0e 50e swim-path  1 0e 0e 0e 0e swim-step  fabs 0.0629e f<  in-box? -> -1 -1 }T
T{ [: 0e 0e 0e 0e  100 0 do  1 swim-step  loop  fdrop  in-box? ;] execute -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
