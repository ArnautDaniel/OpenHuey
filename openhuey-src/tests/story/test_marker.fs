IN: test-marker
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.house-of-truth-1f-5 ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
free-play  house-of-truth-1f-5 room!  20 run-frames
T{ [: 0 house-of-truth-1f-5.cmd01  0 var @ alive? ;] in-room -> -1 }T
5 run-frames
T{ [: 1 40 var-set  5 house-of-truth-1f-5.cmd01  1 var @  0 var @ alive? ;] in-room -> 39 -1 }T
T{ [: 2 house-of-truth-1f-5.cmd01 ;] in-room  3 run-frames  [: 0 var @ alive? ;] in-room -> 0 }T
test-summary
