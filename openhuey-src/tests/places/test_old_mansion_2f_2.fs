\ old-mansion-2f-2's place: the corridor by the furnace room - the blocked door (won't budge, then the
\ window); Hewie inside, barking ("Hey! Hewie is inside!"); her call: his scene, the way opened, the
\ door unlocked, his file. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_2.fs
IN: test-old-mansion-2f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors hewie-names story.places.old-mansion-2f-2 ;
full-suite   \ (on: waits, scenes, walks - the full run's)
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
: room-afresh ( -- )  old-mansion-2f-2 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om2f-2-flag1 story-flag-clear
testing the blocked door: it won't budge; then the tiny window
room-afresh  0 by-door
T{ said -> budge-msg }T
200 read-on  0 by-door
T{ said -> window-msg }T
200 read-on
testing Hewie hidden in old-mansion-2f-3's hole: come through, barking inside; her call - his scene, the way opened
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona
old-mansion-2f-3 0 0 game:hewie send hewie-place  hidden-act 0 game:hewie send do-action  10 run-frames
room-afresh
T{ [: hewie-inside @ ;] in-place -> -1 }T
0 by-door
T{ said -> inside-msg }T
200 read-on
[: ['] called start drop ;] in-place  900 play-on   \ (her call: the gesture's frame not driven here)
T{ om2f-2-flag1 story-flag?  furnace-door doors:door-locked? -> -1 0 }T
test-summary
