\ The rooms' own commands (S5c; src/game/rooms/room_*.c Cmd*) - the castle's. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_room_cmds.fs
IN: test-room-cmds
USING: tester engine game-state actors messages common room-names game debug story.state facts story.words
  story.rooms.castle-1f-2 story.rooms.castle-1f-7 story.rooms.castle-1f-11 story.rooms.castle-1f-12
  story.rooms.castle-2f-2 story.rooms.castle-2f-12 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play

castle-1f-2 room!  20 run-frames
testing castle-1f-2: the cage set low (11), raised 0.25 a step; the drum can struck and wobbling back to rest
T{ [: s" ori" object-here?  0 castle-1f-2.cmd00  s" ori" place-y object@ 11e ~=  1 castle-1f-2.cmd00  s" ori" place-y object@ 11.25e ~= ;] in-room -> -1 -1 -1 }T
: wobbles ( -- n )  [: 0 castle-1f-2.cmd03  2 castle-1f-2.cmd03  0  begin  1+  1 castle-1f-2.cmd03  s" doramukan" $34 object@ f0=  over 50 > or  until ;] in-room ;
T{ wobbles  [: s" doramukan" place-y object@ s" doramukan" $38 object@ ~= ;] in-room -> 6 -1 }T   \ (1 less 0.2 five times: just above 0 in single floats)

castle-1f-7 room!  20 run-frames
testing castle-1f-7: the lid up, then falling shut and settling at -25.3 (story flag $310 cleared on landing)
$310 story-flag-set
: lid-settles ( -- n )  [: 0 castle-1f-7.cmd00  0  begin  1+  lid-falls  over 300 > or  until ;] in-room ;
T{ lid-settles 300 <  [: lid-y -25.3e ~= ;] in-room  $310 story-flag? -> -1 -1 0 }T

castle-1f-11 room!  20 run-frames
testing castle-1f-11: the lever pulled to -10 degrees, back 2 at a time, never past 0
T{ [: 1 castle-1f-11.cmd01  s" kanagu" turn-z object@ rad>deg -10e ~=  0 castle-1f-11.cmd01  s" kanagu" turn-z object@ rad>deg -8e ~= ;] in-room -> -1 -1 }T

castle-1f-12 room!  20 run-frames
testing castle-1f-12: a dial set from its variable (3 notches: 0 degrees), then turning a degree a frame to 5 notches
0 3 [: pvar-set ;] in-room
T{ [: 0 0 castle-1f-12.cmd00  s" syuukouki2" turn-y object@ rad>deg 0e ~= ;] in-room -> -1 }T
0 5 [: pvar-set ;] in-room
: turns ( -- n )  [: 1 ebit-clear  0  begin  1+  0 2 castle-1f-12.cmd00  1 ebit?  over 200 > or  until ;] in-room ;
T{ turns 60 - abs 2 <  [: s" syuukouki2" turn-y object@ rad>deg 60e f- fabs 1.5e f< ;] in-room -> -1 -1 }T

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
