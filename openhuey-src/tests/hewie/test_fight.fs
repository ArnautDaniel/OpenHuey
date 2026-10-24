\ Hewie and the stalker, H3 (and the stalkers' P3c): his attack as a joint action, his bites, the
\ stalker's reaction and temper, Hewie struck (docs/subsystems/hewie.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_fight.fs
IN: test-fight
USING: tester engine game-state actors messages common facts room-names game debug flag-names hewie.state hewie.body hewie.model hewie.states hewie.fight fiona.state stalker.state stalker.senses stalker.moving stalker.search stalker.anims stalker.chase stalker.attack stalker.bitten ;

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
T{ [: [: his-act @ ['] st-clamped <> his-action @ $1F <> and ;] his@ ;] 300 soon -> -1 }T
debug:debilitas-out  5 frames

testing the stalker's footsteps (Pursuer_Footsteps): walking ($200), a step as each foot comes down, heard
0 deb-in
: walking ( -- )  [: ['] noop step !  $200 play ;] deb! ;
: steps ( -- n )  [: step-foot @ ;] deb@ ;
: walked ( n -- steps )  steps swap  0 do  walking  game-tick  loop  steps swap - ;
T{ 60 walked 2 > -> -1 }T
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
testing ... struck out of health: down ($52), counted for the dog's level (progress +0xFBA - not in a hard game); not a game over
progress pr.hewie-downs l@ constant downs0
2 90 0 game:hewie [: send blow ;] deb!  3 frames
T{ [: [: his-action @ $52 = ;] his@ ;] 120 soon  [: his-hp @ ;] his@ -> -1 0 }T
T{ progress pr.hewie-downs l@ downs0 -  caught state-flag?  30 frames caught state-flag? -> 1 0 0 }T
[: 100 hp!  0 cond!  0 0 set-action ;] his!  10 frames

testing barked at, his temper up (Pursuer_Hit kind 6: 1 each, searching or waiting about)
[: 0 temper !  3 mode! ;] deb!  3 game:debilitas send snap  game:debilitas send snap  2 frames
T{ [: temper @ ;] deb@ -> 2 }T
testing past what he stands (20): he goes for Hewie - an attack from his table for Hewie - each over, the chance roll (Pursuer_ChanceRoll): won, his temper back to 0 and on
false blind  [: 30 temper ! ;] deb!
T{ [: [: at-dog @ ;] deb@ ;] 10 soon -> -1 }T
T{ [: [: at-dog @ 0= ;] deb@ ;] 2000 soon  [: temper @ ;] deb@ -> -1 0 }T
testing ... his time for it up, his temper still past it: his taunt ($1602), his temper back to 0
[: 30 temper ! ;] deb!
T{ [: [: at-dog @ ;] deb@ ;] 10 soon -> -1 }T
[: 1 dog-t !  closing-in dog-phase ! ;] deb!
T{ [: [: anim @ taunt-anim = ;] deb@ ;] 30 soon  [: temper @ ;] deb@ -> -1 0 }T
T{ [: [: at-dog @ 0= ;] deb@ ;] 300 soon -> -1 }T
testing the chance roll's rows (Debilitas: Hewie down 100, his blow struck Hewie 100, else 25 rising 12, struck 75 rising 25)
: rolled ( cond struck-dog struck? -- won? )  [: {: c d s :}  c dog-cond !  d struck-dog !  0e roll-build f!  50 temper !  s chance-roll ;] deb@ ;
T{ 2 0 0 rolled  0 -1 0 rolled  [: temper @ ;] deb@ -> -1 -1 0 }T
T{ [: 0 dog-cond !  0 struck-dog !  -1e8 roll-build f!  false chance-roll drop  roll-build f@ -1e8 f- f>s  true chance-roll drop  roll-build f@ -1e8 f- f>s  0e roll-build f! ;] deb@ -> 12 37 }T
testing ... Hewie gone from the room meanwhile: out after him by the exit to his, his temper back to 0
variable by  variable to
: an-exit ( -- )  8 0 do  room-id i room-exit-leads drop dup 0< 0= if  to !  i by !  unloop exit  then  drop  loop ;
an-exit  [: 30 temper ! ;] deb!
T{ [: [: at-dog @ ;] deb@ ;] 10 soon -> -1 }T
[: to @ 0 dup tri-center body-place ;] his!
T{ [: [: out-going @ ;] deb@ ;] 10 soon  [: making-for @  temper @  at-dog @  my-mode @ ;] deb@ -> -1 by @ 0 0 1 }T
true blind  debug:debilitas-out  5 frames
[: room-id s" fiona" actor-named body-tri dup tri-center body-place ;] his!  game:hewie send join-fiona  20 frames

testing her kick on him: struck home (her answer), he yelps; hit by her often - angry by chance
[: 100 hp!  0 cond!  0 0 set-action  0 his-hits ! ;] his!  5 frames
2 5 0 game:hewie [: send blow ;] game:fiona enter execute leave-actor  3 frames
T{ [: his-hp @  his-action @  his-hits @ ;] his@ -> 90 $69 1 }T

testing a door into him (kind 5): only while it swings - its damage, back from it or aside ($6B), or out by it ($6C)
[: 100 hp!  0 cond!  0 0 set-action ;] his!  5 frames
5 3 0 game:hewie [: send blow ;] game:fiona enter execute leave-actor  3 frames
T{ [: his-hp @  his-action @ dup $6B = swap $6C = or ;] his@ -> 100 0 }T
T{ 0 0 door-anim  1 frames  0 door-animating? -> -1 -1 }T
5 3 0 game:hewie [: send blow ;] game:fiona enter execute leave-actor  3 frames
T{ [: his-hp @  his-action @ dup $6B = swap $6C = or ;] his@ -> 97 -1 }T
[: 100 hp!  0 cond!  0 his-2b !  0 his-2d !  0 0 set-action ;] his!  60 frames
[: room-id s" fiona" actor-named body-tri dup tri-center body-place ;] his!  5 frames

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

testing off screen, the stalker searching his room (Hewie_Scuffle): they scuffle - he waits it out ($30), the stalker held; once a meeting
2 seed !   \ (a seed it comes to a scuffle with: a quarter of the time nothing does)
front-garden-2 room!  10 frames  140.64e 124e -217.33e 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
front-garden-3 room!  2 frames                     \ (she goes on: he is left off screen in front-garden-2)
true blind  front-garden-2 0 game:debilitas send stalker-in  2 frames  [: 2 mode! ;] deb!
T{ [: [: his-action @ $30 = ;] his@  [: stun-t @ 0> ;] deb@ or ;] 30 soon  [: his-scuffled @ ;] his@ -> -1 -1 }T
testing his feeling about the stalker (Hewie_FeelPursuer): disliking him (-10), calm, the stalker about - upset (mood 2), 14 in 16; liking him, never
T{ [: 0 his-mood !  -10 feel !  4 0 do  feel-pursuer  loop  his-mood @ ;] his@ -> 2 }T
T{ [: 0 his-mood !  5 feel !  20 0 do  feel-pursuer  loop  his-mood @ ;] his@ -> 0 }T
[: 0 feel ! ;] his!
debug:debilitas-out  5 frames
false blind
testing his bites' damage: as it is; Sylvesta Esca's half again (+0xA10); the black shepherd twice, then half again
: sylvesta ( frames -- )  progress pr.boosts 6 8 * + 4 + l! ;
: shepherd ( n -- )  progress pr.vars $27 + c! ;
T{ [: 10 bite-damage ;] his@ -> 10 }T
T{ 450 sylvesta  [: 10 bite-damage  5 bite-damage ;] his@ -> 15 7 }T
T{ 1 shepherd  [: 10 bite-damage ;] his@  0 sylvesta  [: 10 bite-damage ;] his@  0 shepherd -> 30 20 }T
testing his damage (Hewie_TakeDamage): down to 0; in his third costume ($27 2) never below 1
T{ [: 5 hp!  50 hurt-by  his-hp @ ;] his@ -> 0 }T
T{ 2 shepherd  [: 5 hp!  50 hurt-by  his-hp @ ;] his@  0 shepherd -> 1 }T
[: 100 hp! ;] his!
testing struck by her: counted (Fiona's hits, eased 300 frames on) - one counter, read when he cowers after
T{ [: 0 his-hits !  0 his-hit-t !  1 0 0 her take-blow drop  his-hits @ his-hit-t @ ;] his@ -> 1 300 }T
T{ [: 0 -1 set-mood  0 his-hits !  0 his-hit-t ! ;] his! -> }T
testing down in a hard game (last: the game ends): the game is over (game over kind 3)
$AF story-flag-set  [: dies-in-hard-mode ;] his!   \ (as his down state does once he lies there)
: over ( -- step kind )  game:gameover enter  gameover:step @  gameover:kind @  leave-actor ;
T{ [: over drop 0< 0= ;] 120 soon  over nip -> -1 3 }T
test-summary
