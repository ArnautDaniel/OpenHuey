\ The rooms' own commands (S5c; src/game/rooms/room_*.c Cmd*) - the castle's. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_room_cmds.fs
IN: test-room-cmds
USING: tester engine game-state actors messages common room-names game debug story.state facts story.words
  story.rooms.castle-2f-12 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play

castle-2f-12 room!  20 run-frames
testing castle-2f-12: the doors opened at once; the chairs set rocking and dying down
T{ [: s" left" turn-y object@  0 castle-2f-12.cmd00  s" left" turn-y object@ f- 2.25e ~= ;] in-room -> -1 }T
: rocks ( -- n )  [: 0 castle-2f-12.cmd01  2 castle-2f-12.cmd01  0  begin  1+  1 castle-2f-12.cmd01  castle-2f-12.cond00? 0=  over 5000 > or  until ;] in-room ;
T{ rocks 5000 < -> -1 }T

test-summary
