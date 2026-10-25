\ A morph object by event variable 0 (var0_anim): the curtain of old-mansion-b1-2 drawn open
\ with the variable, its frames and blend on the object. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_curtain.fs
IN: test-curtain
USING: tester engine game-state actors messages common room-names game debug story.state story.words story.rooms.old-mansion-b1-2 ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play  old-mansion-b1-2 room!  20 run-frames
testing the curtain on with the variable: 21 - half way (9 of 18), frames 1 to 0; then back at rest
T{ [: s" Curtain" object-here? ;] in-room -> -1 }T
T{ [: 0 21 var-set  0 old-mansion-b1-2.cmd01  s" Curtain" $74 object-n@  s" Curtain" $78 object-n@  s" Curtain" $7C object@ 0.5e ~= ;] in-room -> 1 0 -1 }T
T{ 2 run-frames  [: 3 old-mansion-b1-2.cmd01  s" Curtain" $7C object@ 1e ~= ;] in-room -> -1 }T
test-summary
