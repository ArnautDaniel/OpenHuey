\ old-mansion-1f-4's place: the hopping figure; the transmuter (its plaque and file; the White
\ Godstone changed - an incubator bursting, two homunculi; anything else nothing); the incubators;
\ the Chicken Jerky. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_4.fs
IN: test-old-mansion-1f-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-4 ;
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
: room-afresh ( -- )  old-mansion-1f-4 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om1f-4-flag1 story-flag-clear  room-afresh
testing coming in: the figure hopping, the room hunted
T{ [: hopper-slot figure 0< 0= ;] in-place  hunted state-flag? -> -1 -1 }T
testing the transmuter: its plaque, then her wonder; its file
T{ 5 20 at  said  transmuter-file filed? -> plaque-msg -1 }T
T{ 5 20 at  said -> transmuter-msg }T
testing the incubators: something inside, then meat
T{ 8 90 at  said -> incubator-msg }T
T{ 9 90 at  said -> meat-msg }T
testing the White Godstone put in: the Red Godstone out; an incubator burst
white-godstone 1 items send give-item  10 run-frames  red-godstone items:items-of before !
0 changes-it -1 0 broadcast item-event  900 play-on
T{ white-godstone items:items-of  red-godstone items:items-of  om1f-4-flag1 story-flag?  1 part? -> 0 before @ 1+ -1 -1 }T
testing ... the burst ones empty
T{ $B 0 at  said -> empty-msg }T
testing anything else put in: nothing comes of it
silver-ore 1 items send give-item  10 run-frames  silver-ore items:items-of before !
0 nothing-from-it 1 silver-ore broadcast item-event  600 play-on
T{ silver-ore items:items-of -> before @ }T
testing the Chicken Jerky: taken
om1f-4-chicken-jerky story-flag-clear  room-afresh  chicken-jerky items:items-of before !
0 -51.07e 0e 15.59e put  circle press  300 read-on
T{ om1f-4-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
test-summary
