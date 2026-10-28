\ old-mansion-1f-16's place: the reservoir hall - the ripples, the marked door (locked, its marking;
\ the Mars Key), the water below, the Refined Magnesia. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_16.fs
IN: test-old-mansion-1f-16
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-16 ;
full-suite   \ (on: waits, scenes, walks - the full run's)
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
: room-afresh ( -- )  old-mansion-1f-16 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om3f-2-flag1 story-flag-set  found-file-234 story-flag-set   \ (past its first-time ways)
testing the marked door: locked, then its marking
room-afresh  marked-door s" doors" actor-named send lock  10 run-frames
marked-exit by-door
T{ said -> locked-msg }T
marked-exit by-door
T{ said -> marking-msg }T
testing ... the Mars Key: unlocked, spent
mars-key 1 items send give-item  10 run-frames  marked-exit by-door  0 2 -1 0 broadcast item-event  300 read-on
T{ marked-door doors:door-locked?  mars-key items:items-of -> 0 0 }T
testing the water below
T{ 166 -7e 100e 25e put  said -> water-msg }T   \ (on the walkway, a way off her spot)
testing the Refined Magnesia: taken
om1f-16-refined-magnesia story-flag-clear  room-afresh  refined-magnesia items:items-of before !
0 65.56e 94e 15.77e put  circle press  300 read-on
T{ om1f-16-refined-magnesia story-flag?  refined-magnesia items:items-of -> -1 before @ 1+ }T
test-summary
