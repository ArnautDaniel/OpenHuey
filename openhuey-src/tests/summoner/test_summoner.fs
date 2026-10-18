\ The summoner (docs/subsystems/summoner.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/summoner/test_summoner.fs
IN: test-summoner
USING: tester engine game-state actors messages room-names game facts flag-names doors stalker.state summoner debug ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: its ( xt -- x )  game:summoner enter  execute  leave-actor ;
: in-game? ( -- flag )  [: in-game @ ;] his ;
: his-room ( -- room )  game:debilitas body-room ;

free-play  front-garden-2 room!  10 frames

testing a noise's level: by loudness; one less away from doors with every door shut; at most 3
: lvl ( loud door -- lvl )  room-id summoner:level ;
T{ $10 -1 lvl  $20 0 lvl  $40 0 lvl  $60 0 lvl  $FF 0 lvl -> 0 1 2 3 3 }T

testing the rooms near hers: hers first, then those within two open doors
T{ [: room-id summoner:rooms-near  summoner:near-list @  summoner:near-n @ 1 > ;] its -> room-id -1 }T
T{ [: room-id 0 room-exit-leads drop summoner:listed? ;] its -> -1 }T

testing off stage: loaded, the summoner on - his time away rolled, then counted down
2 story.words:partner-load  summoner-on state-flag-set  2 frames
T{ in-game?  summoner-timing state-flag?  [: away @ 4500 >= ;] its -> 0 -1 -1 }T
T{ [: away @ ;] its  5 frames  [: away @ ;] its - -> 5 }T

testing his time away over: in, out of sight, near her room (not hers), waiting about
[: 1 away ! ;] its  3 frames
T{ in-game?  his-room room-id <>  [: room-id summoner:rooms-near  his-room summoner:listed? ;] its  [: my-mode @ ;] his -> -1 -1 -1 3 }T
T{ summoner-waiting state-flag?  [: cooldown @ ;] its 2600 > -> 0 -1 }T

testing the story takes him (0x84): out, his room noted, his time away rolled afresh
variable was  his-room was !  0 story.words:summon-take  2 frames
T{ in-game?  [: took-room @ ;] its  was @ =  [: away @ 4500 >= ;] its -> 0 -1 -1 }T

testing the cooldown (0x5A, 0x5B): set, less, his own
T{ 10 0 story.words:summoner-wait 1 frames [: cooldown @ ;] its -> 300 }T
T{ 4 1 story.words:summoner-wait 1 frames [: cooldown @ ;] its -> 180 }T
T{ story.words:summoner-own-cooldown 1 frames [: cooldown @ ;] its -> 2700 }T

testing a loud noise calls him in (3 s waited, his last taking's wait over): after her
[: 900 waited ! 10000 away ! ;] its
$90 room-id -1 broadcast noise-made  4 frames
T{ in-game?  [: my-mode @ ;] his -> -1 1 }T

testing in play: held off (mode 4) away from her for 5 s, he is taken
0 story.words:summon-take  2 frames
\ (in again, then held off two rooms off: not next to hers)
[: 1 away ! ;] its  3 frames
: far? ( -- flag )  his-room  [: summoner:next-to-hers? ;] its 0=  his-room room-id <> and ;
T{ in-game? -> -1 }T
\ (two rooms off he is taken; next door, held off out of his own rooms, he isn't)
variable expected  far? 0= expected !
[: 4 my-mode ! ;] his  155 frames
T{ in-game? -> expected @ }T

testing in play: beaten (knocked out of health: +0xC4 2) away from her for 5 s, taken as beaten (kind 2)
0 story.words:summon-take  2 frames
[: 1 away ! ;] its  3 frames
far? 0= expected !
[: 3 my-mode !  -1 beaten ! ;] his  155 frames
T{ in-game?  expected @ = -> -1 }T
: taken-as ( -- kind )  expected @ if  2  else  [: took-kind @ ;] its  then ;   \ (still in: nothing to check)
T{ taken-as -> 2 }T   \ (taken: as beaten)

test-summary
