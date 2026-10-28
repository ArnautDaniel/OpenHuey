\ old-mansion-1f-24's place: Daniella scrubbing - spoken to: Hebel's Perfume, then "cleaning time";
\ the covered statue; the vase's Beef Jerky; the Rubrum Medallion. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_24.fs
IN: test-old-mansion-1f-24
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-24 ;
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
: room-afresh ( -- )  old-mansion-1f-24 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  daniella-kind story.base:partner-load  5 run-frames
testing Daniella scrubbing: in, held at her task
white-mandragora-used story-flag-clear  room-afresh
T{ [: scrubbing @ ;] in-place  partner body-room -> -1 old-mansion-1f-24 }T
testing ... spoken to: Hebel's Perfume the first time, then cleaning time
0 -76e 0e -48e put  180 -76e 0e -48e put   \ (just before her, facing her)
T{ said  hebels-perfume items:items-of 0> -> princess-msg -1 }T
200 read-on
T{ said -> cleaning-msg }T
200 read-on
partner send stalker-out
testing the covered statue
T{ $A -180 at  said -> statue-msg }T
testing the Rubrum Medallion: taken
om1f-24-rubrum-medallion story-flag-clear  white-mandragora-used story-flag-set  room-afresh  rubrum-medallion items:items-of before !
0 -98.51e 0e -16.98e put  circle press  300 read-on
T{ om1f-24-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> -1 before @ 1+ }T
testing the vase broken: its Beef Jerky
om1f-24-breakable-broken story-flag-set  om1f-24-beef-jerky story-flag-clear  room-afresh  beef-jerky items:items-of before !
0 -97.5e 0e -3e put  circle press  300 read-on
T{ om1f-24-beef-jerky story-flag?  beef-jerky items:items-of -> -1 before @ 1+ }T
test-summary
