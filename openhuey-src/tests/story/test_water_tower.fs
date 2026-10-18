\ The water tower's room commands (S5c; src/game/rooms/room_C2.c, room_C7.c): the hatch turned by
\ event variable 0, the aiming cursor moved by the arrows and kept on the screen, Fiona's hit
\ taking a variable down. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_water_tower.fs
IN: test-water-tower
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.water-tower-2f-1 story.rooms.water-tower-8f-1 ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play  water-tower-2f-1 room!  20 frames

testing water-tower-2f-1: the hatch open, then half way (60 of 120 frames) on its way open: 45 degrees
T{ [: s" move00" object-here? ;] in-room -> -1 }T
T{ [: 1 water-tower-2f-1.cmd00  s" move00" turn-y object@ pi f2/ ~= ;] in-room -> -1 }T
T{ [: 0 60 var-set  2 water-tower-2f-1.cmd00  s" move00" turn-y object@ 45e deg>rad ~= ;] in-room -> -1 }T

water-tower-8f-1 room!  20 frames
testing water-tower-8f-1: the cursor from (246, 242): right 4, up 4; at the edge it stays
T{ [: 0 water-tower-8f-1.cmd02  7 var @  8 var @ ;] in-room -> 246 242 }T
T{ [: 1 cells arrows-held + 1 swap !  0 cells arrows-held + 1 swap !  cursor-step  7 var @  8 var @ ;] in-room -> 250 238 }T
T{ [: 7 490 var-set  cursor-step  7 var @ ;] in-room -> 492 }T
T{ [: 4 0 do  0 i cells arrows-held + !  loop  2 water-tower-8f-1.cmd02 ;] in-room -> }T

testing water-tower-8f-1: a variable down by her hit (5, not below 0)
T{ [: 3 7 var-set  3 water-tower-8f-1.cmd01  3 var @ ;] in-room -> 2 }T
T{ [: 3 water-tower-8f-1.cmd01  3 var @ ;] in-room -> 0 }T
test-summary
