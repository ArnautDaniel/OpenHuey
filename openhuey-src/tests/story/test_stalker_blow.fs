\ castle-b1-6's cmd01 (room $C): a script variable down by the stalker's blow under way
\ (stalker-blow; the original's Pursuer_PathNodeSound), none when he has none. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_stalker_blow.fs
IN: test-stalker-blow
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.castle-b1-6 ;
: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
free-play  castle-b1-6 room!  20 frames
testing castle-b1-6: no stalker, no blow; his blow of 7 takes variable 3 from 20 to 13, then to 6
T{ [: 3 20 var-set  3 0 castle-b1-6.cmd01  3 var @ ;] in-room -> 20 }T
T{ $C 0 0 0 100 100 0 0 -1 room-actor send stalker-doing  7 room-actor send stalker-blow -> }T
1 frames
T{ [: 3 0 castle-b1-6.cmd01  3 var @  3 0 castle-b1-6.cmd01  3 var @ ;] in-room -> 13 6 }T
T{ $C 0 0 0 100 100 0 0 -1 room-actor send stalker-doing  -1 room-actor send stalker-blow -> }T
1 frames
T{ [: 3 0 castle-b1-6.cmd01  3 var @ ;] in-room -> 6 }T
test-summary
