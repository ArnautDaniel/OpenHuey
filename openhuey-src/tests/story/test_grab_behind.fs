\ A room's grab of Hewie from behind (Pursuer_GrabHewieBehind): asked of the stalker, his answer a
\ frame on - Hewie told where he is held. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_grab_behind.fs
IN: test-grab-behind
USING: tester engine game-state actors messages common room-names game debug flag-names story.state story.words hewie.state stalker.state stalker.senses stalker.moving stalker.search stalker.chase stalker.bitten ;
: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: deb! ( xt -- )  game:debilitas enter  execute  leave-actor ;
free-play  castle-1f-1 room!  20 frames
$B7 tri-center 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
stalkers-blind state-flag-set  game:hewie body-tri debug:debilitas-tri  3 frames  [: 3 mode! ;] deb!  3 frames
[: 2 partner-load ;] in-room  deliver  2 frames
testing asked: no at once; a frame on, yes - and Hewie's spot by him
T{ [: grab-hewie-behind? ;] in-room -> 0 }T
T{ deliver 2 frames  [: grab-hewie-behind? ;] in-room  game:hewie enter his-to-tri @ leave-actor 0< 0= -> -1 -1 }T
test-summary
