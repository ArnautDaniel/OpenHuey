\ old-mansion-2f-6's place: the lockers' shelves (their remarks); the water (calmed; then
\ refreshed); the device, the cabinet, the canister, the containers, the table; the Lavender;
\ hiding in the near locker from Daniella; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_6.fs
IN: test-old-mansion-2f-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-6 ;
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
: room-afresh ( -- )  old-mansion-2f-6 room!  60 run-frames ;
: facing-middle ( area -- )  area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
: toward ( area F: px pz -- )
    {: a F: px pz :}  a area-middle drop {: F: x y z :}  px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  5 run-frames
om2f-6-flag1 story-flag-clear  om2f-6-flag2 story-flag-clear  room-afresh
testing the lockers' shelves: the medicine, then its label; the chemicals, then theirs
T{ 7 45 at  said -> medicine-msg }T
T{ 7 45 at  said -> acetic-msg }T
T{ 8 -45 at  said -> chemicals-msg }T
T{ 8 -45 at  said -> sulfuric-msg }T
testing the water: calmed; then refreshed
T{ 9 0 at  said -> water-msg }T
300 play-on
T{ 9 0 at  said -> refreshed-msg }T
testing the device, the cabinet, the canister, the containers, the table
T{ $A -15 at  said -> device-msg }T
T{ $B -16e -17e toward  said -> modern-msg }T
T{ $C -35 at  said -> cabinet-msg }T
T{ $C -35 at  said -> locked-msg }T
T{ $E 0 at  said -> canister-msg }T
T{ $D 10 at  said -> containers-msg }T
om2f-6-lavender story-flag-set  room-afresh   \ (the Lavender by it offered first, as the original has it)
T{ $F 0 at  said -> table-msg }T
testing the Lavender: taken
om2f-6-lavender story-flag-clear  room-afresh  lavender items:items-of before !
0 4.58e 0e 10.22e put  circle press  300 read-on
T{ om2f-6-lavender story-flag?  lavender items:items-of -> -1 before @ 1+ }T
testing hiding in the near locker from Daniella: her scene, the locker gone
daniella-kind story.base:partner-load  10 run-frames  room-afresh
T{ [: ['] near-locker-hides start drop ;] in-place  [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  doing-busy? 0= ;] 1500 soon  om2f-6-flag2 story-flag? -> -1 -1 }T
partner send stalker-out  10 run-frames
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-2f-6 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
