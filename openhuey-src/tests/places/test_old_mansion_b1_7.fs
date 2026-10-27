\ old-mansion-b1-7's place: the aquarium - three fish swimming, the light shaft, her remark, the
\ Medallion Hewie sniffed out. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_7.fs
IN: test-old-mansion-b1-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-7 ;
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
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the three fish in, swimming; the light shaft
old-mansion-b1-7 room!  120 run-frames
: fish-x ( -- F: x )  [: 3 figure body-pos fdrop fdrop ;] in-place ;
T{ [: fish-in @  shaft-fx effect @ 0<> ;] in-place -> 3 -1 }T
T{ fish-x  60 run-frames  fish-x  f- fabs 0.01e f> -> -1 }T
testing the aquarium
T{ aquarium-area 0 at  said -> aquarium-msg }T
testing the Medallion Hewie sniffed out: taken
omb1-7-medallion-found story-flag-set  omb1-7-medallion story-flag-clear  old-mansion-b1-7 room!  60 run-frames
medallion items:items-of before !
0 8.95e 0e 7.89e put  circle press  300 read-on
T{ omb1-7-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
test-summary
