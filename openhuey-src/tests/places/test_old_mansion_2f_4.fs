\ old-mansion-2f-4's place: the laboratory - the Purifier (its plaque and file; the Sulfuric Ore turned
\ to Silver Ore; another ore handed back), the inscription, the desk, the passage, the Viride Medallion. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_4.fs
IN: test-old-mansion-2f-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-4 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
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
: room-afresh ( -- )  old-mansion-2f-4 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the Purifier: its plaque, its file
room-afresh
T{ -30 -32e 0e 46e put  said  $243 filed? -> plaque-msg -1 }T   \ (a way off the Purifier: her walk to it reports done)
testing ... the Sulfuric Ore laid in: Silver Ore
sulfuric-ore 1 items send give-item  10 run-frames  silver-ore items:items-of before !
-30 -32e 0e 46e put  0 0 -1 0 broadcast item-event  600 read-on
T{ sulfuric-ore items:items-of  silver-ore items:items-of -> 0 before @ 1+ }T
testing ... another (Prima Materia): nothing happens
-30 -32e 0e 46e put  0 3 1 8 broadcast item-event
T{ [: window-shown no-change-msg =  window-shown 0< 0= if  cross press  then ;] 900 soon -> -1 }T
300 read-on
testing the inscription, the desk, the passage
T{ $11 180 at  said -> written-msg }T
200 read-on
T{ $F 40 at  said -> desk-msg }T
T{ $10 90 at  said -> passage-msg }T
testing the Viride Medallion: taken
om2f-4-viride-medallion story-flag-clear  room-afresh  viride-medallion items:items-of before !
0 5.64e 0e 1.58e put  circle press  300 read-on
T{ om2f-4-viride-medallion story-flag?  viride-medallion items:items-of -> -1 before @ 1+ }T
test-summary
