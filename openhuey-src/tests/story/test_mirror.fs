IN: test-mirror
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.castle-1f-12 story.rooms.castle-2f-5 story.rooms.castle-2f-1 ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
free-play  castle-1f-12 room!  20 run-frames
T{ [: s" lens" object-here?  s" syuukouki1" object-here? ;] in-room -> -1 -1 }T
T{ [: 0 castle-1f-12.cmd02  $1B reflection? ;] in-room -> -1 }T
T{ [: 1 castle-1f-12.cmd02 ;] in-room  2 run-frames  [: $1B reflection? ;] in-room -> 0 }T
T{ [: 1 castle-1f-12.cmd03 ;] in-room  2 run-frames  s" window-flash" actor-named 0< -> 0 }T
6 run-frames
T{ s" window-flash" actor-named 0< -> -1 }T
castle-2f-5 room!  20 run-frames
T{ [: s" a_fragment0" object-here? ;] in-room -> -1 }T
T{ [: 4 castle-2f-5.cmd04  $1A reflection? ;] in-room -> -1 }T
castle-2f-1 room!  20 run-frames
T{ [: castle-2f-1.cmd01 ;] in-room  2 run-frames  s" window-flash" actor-named 0< -> 0 }T
test-summary
