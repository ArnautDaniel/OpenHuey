\ Hewie and the stalker, H3 (and the stalkers' P3c): his attack as a joint action, his bites, the
\ stalker's reaction and temper, Hewie struck (docs/subsystems/hewie.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/hewie/test_fight.fs
IN: test-fight
USING: tester engine game-state actors messages common facts room-names game debug flag-names hewie.state hewie.body hewie.model hewie.states hewie.fight fiona.state stalker.state stalker.senses stalker.moving stalker.search stalker.chase stalker.bitten ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  game:hewie enter  execute  leave-actor ;
: his! ( xt -- )  game:hewie enter  execute  leave-actor ;
: deb@ ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: deb! ( xt -- )  game:debilitas enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: blind ( on -- )  if  stalkers-blind state-flag-set  else  stalkers-blind state-flag-clear  then ;
1 seed !

free-play  front-garden-2 room!  20 frames
140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
\ (Debilitas on her triangle, not after her: a stalker at hand for Hewie)
variable arg
: deb-in ( temper -- )
    true blind  s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
    arg !  [: 3 mode!  arg @ temper !  70 hp ! ;] deb!  3 frames ;
: go ( act -- )  arg !  [: 1000 add-trust  obeys  0 -1 set-mood  100 hp!  0 cond!  arg @ 0 set-action ;] his! ;

testing he knows the stalker in his room: who, where (`stalker-doing`)
0 deb-in
T{ [: pursuer  st-room @ ;] his@ -> game:debilitas room-id }T
testing at him (0x1F): asked, taken head on from behind him - leap ($2214), the first bite (10), holding on
$1F go
T{ [: [: bitten @ ;] deb@ 14 = ;] 120 soon  [: his-grip @ ;] his@ -> -1 0 }T
T{ [: [: anim@ $2215 = ;] his@ ;] 60 soon  [: hp @ ;] deb@ -> -1 60 }T
T{ [: temper @ 10 >= ;] deb@ -> -1 }T
testing ... let go: both on again, the stalker after his reaction's end ($1707)
T{ [: [: bitten @ 0= ;] deb@ ;] 600 soon  [: reeling @ ;] deb@ -> -1 0 }T
T{ [: [: his-biting @ 0= his-action @ $1F <> and ;] his@ ;] 300 soon -> -1 }T
debug:debilitas-out  5 frames

testing refused when the stalker can't take it (not in the room): he goes again later (0x5A)
s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
[: $1F 0 set-action  -1 his-grip !  ['] st-await-grip his-act ! ;] his!
front-garden-3 room!  1 frames
12 game:debilitas send dog-attack  3 frames
T{ [: bitten @ ;] deb@ -> 0 }T
debug:debilitas-out  front-garden-2 room!  20 frames
140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames

testing struck by the stalker (his blow): its damage, his yelp ($69, $1000), then on
0 deb-in  [: 0 -1 set-mood  100 hp!  0 0 set-action ;] his!  2 frames
2 30 0 game:hewie [: send blow ;] deb!  3 frames
T{ [: his-hp @  his-action @  anim@ ;] his@ -> 70 $69 $1000 }T
T{ [: [: his-action @ $69 <> ;] his@ ;] 120 soon -> -1 }T
testing ... struck out of health: down ($52)
2 90 0 game:hewie [: send blow ;] deb!  3 frames
T{ [: [: his-action @ $52 = ;] his@ ;] 120 soon  [: his-hp @ ;] his@ -> -1 0 }T
[: 100 hp!  0 cond!  0 0 set-action ;] his!  10 frames

testing barked at, his temper up (Pursuer_Hit kind 6: 1 each, searching or waiting about)
[: 0 temper !  3 mode! ;] deb!  3 game:debilitas send snap  game:debilitas send snap  2 frames
T{ [: temper @ ;] deb@ -> 2 }T
testing past what he stands (20): he goes for Hewie a while - a blow ($E00 / $E01) - then his temper back to 0
false blind  [: 30 temper ! ;] deb!
T{ [: [: at-dog @ ;] deb@ ;] 10 soon -> -1 }T
T{ [: [: temper @ 0= ;] deb@ ;] 400 soon -> -1 }T
true blind  debug:debilitas-out  5 frames

testing her kick on him: struck home (her answer), he yelps; hit by her often - angry by chance
[: 100 hp!  0 cond!  0 0 set-action  0 his-angry-n ! ;] his!  5 frames
2 5 0 game:hewie [: send blow ;] game:fiona enter execute leave-actor  3 frames
T{ [: his-hp @  his-action @  his-angry-n @ ;] his@ -> 90 $69 1 }T

testing a door slammed on him (kind 5): its damage, pushed back from it ($6B)
[: 100 hp!  0 cond!  0 0 set-action ;] his!  5 frames
5 3 0 game:hewie [: send blow ;] game:fiona enter execute leave-actor  3 frames
T{ [: his-hp @  his-action @ ;] his@ -> 97 $6B }T
[: 100 hp!  0 cond!  0 0 set-action ;] his!  60 frames

testing at a flank, the stalker's temper high: thrown off ($1702) - hanging ($2203), then held on or flung, and on
200 deb-in  false blind
[: 1000 add-trust  obeys  100 hp!  0 cond!  $1F his-action !  1 his-grip !  0 his-biting !  8 his-mode !
   $80000008 his-cmd !  ['] st-await-grip his-act !  10 game:debilitas send dog-attack ;] his!   \ (as st-run-at-pursuer leaves him)
T{ [: [: bite-how @ 1 = ;] deb@ ;] 300 soon -> -1 }T
T{ [: [: anim@ $2203 = ;] his@ ;] 200 soon -> -1 }T
T{ [: [: anim@ dup $2204 = swap $2207 = or ;] his@ ;] 200 soon -> -1 }T
T{ [: [: his-action @ $1F <> ;] his@ ;] 600 soon  [: [: bitten @ 0= ;] deb@ ;] 600 soon -> -1 -1 }T
true blind  debug:debilitas-out  5 frames
false blind
test-summary
