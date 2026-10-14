\ The three-dial lock (old-mansion-2f-12, Room69): set to 1 0 2 it opens (event flag 4); otherwise
\ not; a dial turns 4 degrees a frame to its setting. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_dial_lock.fs
IN: test-dial-lock
USING: tester engine game-state actors messages common room-names game debug story.state story.words story.rooms.old-mansion-2f-12 ;
: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
free-play  old-mansion-2f-12 room!  20 frames
: set ( a b c -- )  [: $1A swap progress-var!  $19 swap progress-var!  $18 swap progress-var! ;] in-room ;
testing the dials there
T{ [: 0 dial object-here?  3 dial object-here? ;] in-room -> -1 -1 }T
testing 1 0 2 opens it; 0 0 2 doesn't
1 0 2 set
T{ [: 0 old-mansion-2f-12.cmd00  0 0 var-set  4 ebit-clear  2 old-mansion-2f-12.cmd00  4 ebit? ;] in-room -> -1 }T
0 0 2 set
T{ [: 0 old-mansion-2f-12.cmd00  4 ebit-clear  3 ebit-set  2 old-mansion-2f-12.cmd00  4 ebit?  3 ebit? ;] in-room -> 0 0 }T
test-summary
