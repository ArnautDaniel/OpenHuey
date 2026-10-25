\ The House of Truth's and the Chaos Forest's room commands (S5c; src/game/rooms/room_8C.c,
\ room_8E.c, room_102.c): the dynamo shaken, the belt machine run and slowed, the shelf falling
\ over, Hewie's health kept and put back, the hanging things brushed. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_house_of_truth.fs
IN: test-house-of-truth
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.house-of-truth-1f-4 story.rooms.house-of-truth-1f-6 story.rooms.chaos-forest-4
  story.rooms.house-of-truth-1f-9 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play  house-of-truth-1f-4 room!  20 run-frames

testing house-of-truth-1f-4: the dynamo shaken, its size down 0.25 a frame
T{ [: s" dynamo" object-here?  s" roller" object-here?  s" belt" object-here? ;] in-room -> -1 -1 -1 }T
T{ [: 0 house-of-truth-1f-4.cmd01  s" dynamo" $3C object@ 1e ~= ;] in-room -> -1 }T

testing house-of-truth-1f-4: the belt set going at 0.4, a frame on: the belt 0.4 along, the wheel 0.05 round
T{ [: 0 house-of-truth-1f-4.cmd02  1 house-of-truth-1f-4.cmd02
      s" belt" place-x object@ 0.4e ~=  s" roller" turn-y object@ 0.05e ~= ;] in-room -> -1 -1 }T

testing house-of-truth-1f-4: Hewie's health kept in variable 2 and put back (0 there: 10)
T{ [: 2 0 var-set  1 house-of-truth-1f-4.cmd03 ;] in-room -> }T
5 run-frames
T{ [: hewie-hp @ ;] in-room -> 10 }T
T{ [: 0 house-of-truth-1f-4.cmd03  2 var @ ;] in-room -> 10 }T

house-of-truth-1f-6 room!  20 run-frames
testing house-of-truth-1f-6: the shelf set falling, then put down
T{ [: s" tana" object-here? ;] in-room -> -1 }T
T{ [: 0 house-of-truth-1f-6.cmd01  s" tana" $3C object@ 0.005e ~= ;] in-room -> -1 }T
T{ [: 2 house-of-truth-1f-6.cmd01  s" tana" turn-x object@ pi f2/ fsin ~= ;] in-room -> -1 }T

chaos-forest-4 room!  20 run-frames
testing chaos-forest-4: a hanging thing set swinging, a step on: 36 degrees round, 19 frames left
T{ [: 0 room-object object-here? ;] in-room -> -1 }T
T{ [: 0 swing-three  0 room-object $3C object@ 0.9e ~= ;] in-room -> -1 }T
T{ [: 20e 0 room-object $38 object!  2 swing-three
      0 room-object $30 object@ 36e ~=  0 room-object $38 object@ 19e ~= ;] in-room -> -1 -1 }T

house-of-truth-1f-9 room!  20 run-frames
: of-kind ( k -- n )  0  128 0 do  i thing-used? if  i thing-kind 2 pick = if  1+  then  then  loop  nip ;
testing house-of-truth-1f-9: debris dropped, each carrying a keri (shown); a trap set, smoke rising from it; all gone again
T{ [: 6 0 do  i keri$ 0 1 placed-op  loop  6 0 var-set  house-of-truth-1f-9.cmd02  6 var @ ;] in-room  3 run-frames  9 of-kind = -> -1 }T
T{ [: 0 keri$ placed-shown? ;] in-room -> -1 }T
T{ [: 1 0 var-set  house-of-truth-1f-9.cmd03  1 var @ ;] in-room -> 2 }T
3 run-frames
T{ 10 of-kind -> 1 }T
T{ s" smoke-trail" actor-named 0< -> 0 }T
T{ [: house-of-truth-1f-9.cmd0A ;] in-room  3 run-frames  9 of-kind 10 of-kind -> 0 0 }T
40 run-frames
T{ s" smoke-trail" actor-named 0< -> -1 }T
testing house-of-truth-1f-9: two fallen through - variable 6 down two, event bit 9
T{ [: 6 4 var-set  9 ebit-clear  2 keri-fallen !  house-of-truth-1f-9.cmd05  6 var @  9 ebit?  keri-fallen @ ;] in-room -> 2 -1 0 }T
testing house-of-truth-1f-9: stones falling from the ceiling, on and on (gone with the room)
T{ [: house-of-truth-1f-9.cmd01 ;] in-room  60 run-frames  s" stones" actor-named 0< 0= -> -1 }T
house-of-truth-1f-8 room!  5 run-frames
T{ s" stones" actor-named -> -1 }T
test-summary
