\ old-mansion-1f-3's place: the hopping figure (in, hopping), the cart, the Medallion, the Refined
\ Antimony, the door unlocked from this side, brought in by the forced way. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_3.fs
IN: test-old-mansion-1f-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-3 ;
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
: room-afresh ( -- )  old-mansion-1f-3 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the figure hopping by the far wall
room-afresh
T{ [: hopper-slot figure 0< 0= ;] in-place  hunted state-flag? -> -1 -1 }T
testing the cart
T{ cart-area -140 at  said -> cart-msg }T
testing the Medallion on the shelf: taken
om1f-3-medallion story-flag-clear  room-afresh  medallion items:items-of before !
0 -10.64e 0e -112.64e put  circle press  300 read-on
T{ om1f-3-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing the Refined Antimony Hewie sniffed out: taken
om1f-3-refined-antimony-found story-flag-set  om1f-3-refined-antimony story-flag-clear  room-afresh
refined-antimony items:items-of before !
0 -19.52e 0e -17.68e put  circle press  300 read-on
T{ om1f-3-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
testing brought in by the forced way: she gets up, free
old-mansion-1f-3 forced-exit rooms send go-to-room  600 play-on
T{ free? -> -1 }T
test-summary
