\ old-mansion-2f-12's place: the synthesizer (its plaque and file; the Silver Ore changed, anything
\ else nothing); the dials on the pipe (turned to 1, 0, 2: the doors unlocked; drained, its
\ remark); the locked door and the water under it; the books; the Antimony Powder. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_12.fs
IN: test-old-mansion-2f-12
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-12 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  old-mansion-2f-12 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om2f-12-flag1 story-flag-clear  0 $18 var!  0 $19 var!  0 $1A var!  room-afresh
testing the synthesizer: its plaque, then her wonder; its file
T{ $B -45 at  said  synthesizer-file filed? -> plaque-msg -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
T{ $B -45 at  said -> synthesizer-msg }T
testing the Silver Ore put in: the White Godstone out
silver-ore 1 items send give-item  10 run-frames  white-godstone items:items-of before !
0 changes-it -1 0 broadcast item-event  600 play-on
T{ silver-ore items:items-of  white-godstone items:items-of -> 0 before @ 1+ }T
testing ... anything else: nothing comes of it
sulfuric-ore 1 items send give-item  10 run-frames  sulfuric-ore items:items-of before !
0 nothing-from-it 1 sulfuric-ore broadcast item-event  600 play-on
T{ sulfuric-ore items:items-of -> before @ }T
testing the books
T{ $C 22 at  said -> books-msg }T
testing the locked door: locked, then the water under it
far-door s" doors" actor-named send lock  10 run-frames
0 by-door
T{ said -> locked-msg }T
0 by-door
T{ said -> water-msg }T
testing the dials: turned to 1, 0, 2 - the doors unlocked
$A 0 at  circle press  shown-soon drop  [: cross press  20 run-frames  window-shown 0< ;] 20 soon drop   \ (yes)
30 run-frames
T{ [: picked @ ;] in-place -> 0 }T
key: Right tap  60 run-frames   \ (dial 0 to 1)
key: Down tap  key: Down tap  key: Right tap  60 run-frames  key: Right tap  60 run-frames   \ (dial 2 to 2)
T{ $18 var@  $19 var@  $1A var@ -> 1 0 2 }T
[: doing-busy? 0= ;] 600 soon drop
T{ om2f-12-flag1 story-flag?  far-door doors:door-locked? -> -1 0 }T
T{ $A 0 at  said -> drain-msg }T
test-summary
