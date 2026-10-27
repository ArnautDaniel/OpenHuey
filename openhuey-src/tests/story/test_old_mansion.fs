\ The old mansion's room commands (S5c; src/game/rooms/room_4C.c, room_5C.c): the four floor
\ plates pressed in by their event bits (old-mansion-b1-3), her shakes counted (the same room).
\ (old-mansion-2f-8's curtain: tests/places/test_old_mansion_2f_8.fs.) Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_old_mansion.fs
IN: test-old-mansion
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.old-mansion-b1-3 story.rooms.old-mansion-1f-15 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: plate-y ( -- ) ( F: -- y )  s" fumi0" place-y object@ ;
free-play  old-mansion-b1-3 room!  20 run-frames

testing old-mansion-b1-3: plate 0 pressed while its bit is set, to -0.7; back up 0.2 a step
T{ [: s" fumi0" object-here? ;] in-room -> -1 }T
T{ [: 0 old-mansion-b1-3.cmd00  0 ebit-set  1 old-mansion-b1-3.cmd00  plate-y -0.2e ~= ;] in-room -> -1 }T
T{ [: 5 0 do  1 old-mansion-b1-3.cmd00  loop  plate-y -0.7e ~= ;] in-room -> -1 }T
T{ [: 0 ebit-clear  1 old-mansion-b1-3.cmd00  plate-y -0.5e ~= ;] in-room -> -1 }T
T{ [: s" fumi1" place-y object@ 0e ~= ;] in-room -> -1 }T

testing old-mansion-b1-3: her shakes counted from 0; at 35, event bit $13
T{ [: 99 fiona-shook !  0 old-mansion-b1-3.cmd01 ;] in-room -> }T
3 run-frames
T{ [: fiona-shook @ ;] in-room -> 0 }T
T{ [: 35 fiona-shook !  1 old-mansion-b1-3.cmd01  $13 ebit? ;] in-room -> -1 }T

old-mansion-1f-15 room!  20 run-frames
testing old-mansion-1f-15: the yellow shelves moved back 0.25 in x a step, held at their stop (39)
T{ [: s" hondana_b0" object-here? ;] in-room -> -1 }T
T{ [: 8 4 do  40e i shelf place-x object!  loop  0 ebit-set  1 1 old-mansion-1f-15.cmd00  s" hondana_b0" place-x object@ 39.75e ~=  0 ebit? ;] in-room -> -1 -1 }T
T{ [: 5 0 do  1 1 old-mansion-1f-15.cmd00  loop  s" hondana_b0" place-x object@ 39e ~=  0 ebit? ;] in-room -> -1 0 }T

testing swim paths: a fish's way stays in its box (2 x 3 x 5 cells from (30, 0, 50)), turning 3.6 degrees a frame at most
: within? ( F: v lo hi -- ) ( -- flag )  frot fswap fover f>= >r f<= r> and ;
: in-box? ( F: x y z -- flag )  50e 100e within?  0e 30e within? and  30e 50e within? and ;
T{ 1 2 3 5 240  30e 0e 50e swim-path  1 0e 0e 0e 0e swim-step  fabs 0.0629e f<  in-box? -> -1 -1 }T
T{ [: 0e 0e 0e 0e  100 0 do  1 swim-step  loop  fdrop  in-box? ;] execute -> -1 }T
test-summary
