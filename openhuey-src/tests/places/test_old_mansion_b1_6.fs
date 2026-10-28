\ old-mansion-b1-6's place: the word's letters (the next L or R); a plate given back; the altar; the
\ golem statue; METH; the Medallion; the walls of flame (her fright); a gate's fire put out by the
\ golem; SALTATIO - the dance; the race begun; the replay. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_6.fs
IN: test-old-mansion-b1-6
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-6 ;
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
: room-afresh ( -- )  old-mansion-b1-6 room!  60 run-frames ;
: facing-middle ( area -- )  area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
: toward ( area F: px pz -- )
    {: a F: px pz :}  a area-middle drop {: F: x y z :}  px x f-  pz z f-  fatan2 rad>deg f>s  x y z put ;
: lrl ( -- n )  [char] L  [char] R 8 lshift or  [char] L 16 lshift or ;   \ ('L' 'R' 'L' 0)
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
omb1-6-flag1 story-flag-set  room-afresh
testing the word's letters: "LRL" - L (on), R wanted (on), R wanted: no (left at the L)
T{ [: lrl word-vars !  0 word-vars cell+ !  0 read-at !  false letter?  true letter?  true letter?  read-at @ ;] in-place -> -1 -1 0 2 }T
testing a plate given back: her word
0 plate-back -1 0 broadcast item-event
T{ shown-soon -> returned-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)
200 read-on
testing the altar, the golem statue, METH
T{ $15 facing-middle  said -> altar-msg }T
200 read-on
T{ $16 75e -40e toward  said -> golem-msg }T
T{ $1D 0 at  said -> meth-msg }T
testing the Medallion: taken
omb1-6-medallion story-flag-clear  room-afresh  medallion items:items-of before !
0 2.06e 0e -103.68e put  circle press  300 read-on
T{ omb1-6-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing SALTATIO: the dance, then her calmer
room-afresh  0 golem-dance -1 0 broadcast item-event  [: doing-busy? 0= ;] 2000 soon drop
T{ shown-soon -> calmer-msg }T
300 read-on
testing the race begun: the golem set going, the button to run
room-afresh  lrl 0 swap -1 swap broadcast item-var  0 1 0 broadcast item-var  0 golem-race -1 0 broadcast item-event
T{ shown-soon  [: golem-slot figure 0< 0= ;] in-place -> run-msg -1 }T
T{ [: ['] its-way running? ;] in-place -> -1 }T
testing the movie replayed (the extras): played, then back by the forced way
room-afresh  200 run-frames  events-held state-flag-set  old-mansion-b1-6 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
events-held state-flag-clear  200 run-frames
testing a gate's fire put out by the golem: the door beyond unlocked, the floor freed
omb1-6-flag4 story-flag-clear  $7B s" doors" actor-named send lock  room-afresh
T{ [: golem golem-slot figure-in ;] in-place  20 run-frames  [: golem-slot figure-on  golem-slot 0e 0e 0e 0e figure-at ;] in-place  10 run-frames  [: [: 2 omb1-6-flag4 $7B omb1-6-flag5 into-gate ;] start drop ;] in-place  900 run-frames
   omb1-6-flag4 story-flag?  $7B doors:door-locked? -> -1 0 }T
test-summary
