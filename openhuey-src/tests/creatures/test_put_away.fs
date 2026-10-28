\ A spirit put away by a room's command (Room4F / Room5C: the kit's spirit-put-away-near?): the first of the
\ creatures within reach of the spot vanishes there and is out of play. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/creatures/test_put_away.fs
IN: test-put-away
USING: tester engine game-state actors messages room-names flag-names facts paths saves game fiona.state creatures creatures.state creatures.spirit debug story.kit ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room-actor ( xt -- x )  room-actor enter  execute  leave-actor ;
: spirit0 ( -- id )  game:creatures enter  0 slot@  leave-actor ;
create centre 12 allot
: tri-away ( d -- tri )
    nav-tris 0 do
        i tri-center centre vec!  centre game:fiona body-at vec-dist-xz  dup s>f f- fabs 3e f<
        if  drop i unloop exit  then
    loop  drop -1 ;
60 run-frames
front-garden-2 -1 game:rooms send go-to-room  5 run-frames
30 tri-away constant far-tri
room-id 2 far-tri 4 1 -1 0 0e f>cell game:creatures send creature-place  deliver  5 run-frames
testing a spot elsewhere: none put away; at the spirit: it is, gone from play
T{ spirit0 alive?  [: 0e 0e 3e spirit-put-away-near? ;] in-room-actor -> -1 0 }T
fvariable sx  fvariable sz  spirit0 constant s0
s0 body-at dup sf@ sx f!  8 + sf@ sz f!
T{ [: sx f@ sz f@ 3e spirit-put-away-near? ;] in-room-actor  deliver 2 run-frames  s0 alive? -> -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
