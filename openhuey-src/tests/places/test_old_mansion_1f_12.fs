\ old-mansion-1f-12's place: the dragon's remarks; a plate in a low head (right: its jaw shut; wrong:
\ nothing); the highest out of reach; with Hewie, right - the fire frozen; the ice pillar; the statue;
\ the locked door and its symbol; the Torn Letter; the ivy; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_12.fs
IN: test-old-mansion-1f-12
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-12 ;
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
: room-afresh ( -- )  old-mansion-1f-12 room!  60 run-frames ;
: facing-middle ( area -- )  area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
: plate ( right? ev -- )  swap 0 swap broadcast item-event-bit  0 swap -1 0 broadcast item-event ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om1f-12-flag1 story-flag-clear  om1f-12-flag2 story-flag-clear  om1f-12-flag3 story-flag-clear  om1f-12-flag4 story-flag-clear
got-torn-letter story-flag-set  room-afresh
testing the dragon: its open mouths
T{ 9 90 at  said -> dragon-msg }T
testing a wrong plate in the right head: nothing happened
false right-plate plate  600 play-on
T{ om1f-12-flag2 story-flag? -> 0 }T
testing the right plates: the jaws shut; then one more
true right-plate plate  600 play-on
true left-plate plate  600 play-on
T{ om1f-12-flag2 story-flag?  om1f-12-flag3 story-flag? -> -1 -1 }T
T{ 9 90 at  said -> one-more-msg }T
testing the highest out of her reach
T{ 0 out-of-reach -1 0 broadcast item-event  shown-soon -> too-short-msg }T
300 read-on
testing with Hewie by her, the right plate: the fire frozen
room-afresh  true high-plate plate
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
[: movie-skipped state-flag-set  doing-busy? 0= ;] 3000 soon drop  300 play-on
T{ om1f-12-flag1 story-flag? -> -1 }T
T{ 9 90 at  said -> solved-msg }T
testing the ice pillar: climbed down
T{ 7 0 at  said -> climb-down-msg }T
300 play-on
testing the statue
T{ 5 90 at  said -> statue-msg }T
testing the locked door: locked, then its symbol
marked-door s" doors" actor-named send lock  room-afresh
0 by-door
T{ said -> locked-msg }T
0 by-door
T{ said -> symbol-msg }T
testing the ivy
T{ $F -45 at  said -> ivy-msg }T
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-1f-12 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
