IN: test-mirror
USING: tester engine game-state actors messages common room-names game debug story.state story.words
  story.rooms.castle-2f-5 ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
free-play  castle-2f-5 room!  20 run-frames
T{ [: s" a_fragment0" object-here? ;] in-room -> -1 }T
T{ [: 4 castle-2f-5.cmd04  $1A reflection? ;] in-room -> -1 }T
test-summary
