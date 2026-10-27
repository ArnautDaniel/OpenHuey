\ The House of Truth's room commands (S5c; src/game/rooms/room_*.c): house-of-truth-1f-9's debris,
\ traps and smoke. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_house_of_truth.fs
IN: test-house-of-truth
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.house-of-truth-1f-9 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
free-play
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
