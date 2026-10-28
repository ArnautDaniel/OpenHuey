\ A place's grab of Hewie from behind (Pursuer_GrabHewieBehind: the kit's grabs-hewie?): asked of the stalker, his answer a
\ frame on - Hewie told where he is held. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_grab_behind.fs
IN: test-grab-behind
USING: tester engine game-state actors messages common room-names game debug flag-names facts hewie.state stalker.state stalker.senses stalker.moving stalker.search stalker.chase stalker.bitten story.kit story.base ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: deb! ( xt -- )  game:debilitas enter  execute  leave-actor ;
free-play  castle-1f-1 room!  20 run-frames
$B7 tri-center 0e tp-facing  5 run-frames  game:hewie send join-fiona  20 run-frames
stalkers-blind state-flag-set  game:hewie body-tri debug:debilitas-tri  3 run-frames  [: 3 mode! ;] deb!  3 run-frames
2 partner-load  deliver  2 run-frames
testing asked: no at once; a frame on, yes - and Hewie's spot by him
T{ [: grabs-hewie? ;] in-room -> 0 }T
T{ deliver 2 run-frames  [: grabs-hewie? ;] in-room  game:hewie enter his-to-tri @ leave-actor 0< 0= -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
