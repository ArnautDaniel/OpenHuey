\ The rooms' own commands (S5c; src/game/rooms/room_*.c Cmd*) - the castle's. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_room_cmds.fs
IN: test-room-cmds
USING: tester engine game-state actors messages common room-names game debug story.state facts story.words
  story.rooms.castle-2f-2 story.rooms.castle-2f-12 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play

castle-2f-2 room!  20 run-frames
testing castle-2f-2: the doll put back, knocked (a push up), falling and landing on the floor, settled
: knock ( -- )   \ (as her blow knocks it: a push along her heading, its triangle)
    [: dool-at vec@ fdrop fdrop fdrop  0e dool $30 object!  1e dool $34 object!  1e dool $38 object!  dool-at v-tri dool $3C object-n! ;] in-room ;
T{ [: castle-2f-2.cmd01  dool object-here?  dool place-y object@ 0.51e ~= ;] in-room -> -1 -1 }T
knock
: falls ( -- n )  [: 0  begin  1+  castle-2f-2.cmd03  dool $3C object-n@ $80000000 and  over 300 > or  until ;] in-room ;
T{ falls 300 <  [: dool $34 object@ f0=  1 ebit? ;] in-room -> -1 -1 -1 }T

castle-2f-12 room!  20 run-frames
testing castle-2f-12: the doors opened at once; the chairs set rocking and dying down
T{ [: s" left" turn-y object@  0 castle-2f-12.cmd00  s" left" turn-y object@ f- 2.25e ~= ;] in-room -> -1 }T
: rocks ( -- n )  [: 0 castle-2f-12.cmd01  2 castle-2f-12.cmd01  0  begin  1+  1 castle-2f-12.cmd01  castle-2f-12.cond00? 0=  over 5000 > or  until ;] in-room ;
T{ rocks 5000 < -> -1 }T

test-summary
