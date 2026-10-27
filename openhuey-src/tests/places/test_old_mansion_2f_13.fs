\ old-mansion-2f-13's place: the curtained mirror - drawn back (its glass lit), across again;
\ smashed, its remark; the Rubrum Medallion. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_13.fs
IN: test-old-mansion-2f-13
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-13 ;
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
: room-afresh ( -- )  old-mansion-2f-13 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the curtain: drawn back, the glass lit; across again
om2f-13-breakable-broken story-flag-clear  om2f-13-flag1 story-flag-clear  room-afresh
90 111e 0e 47e put  circle press  [: doing-busy? 0= ;] 600 soon drop   \ (a way off her spot by it)
T{ om2f-13-flag1 story-flag?  [: glass-fx effect @ 0<> ;] in-place -> -1 -1 }T
90 111e 0e 47e put  circle press  [: doing-busy? 0= ;] 600 soon drop   \ (a way off her spot by it)
T{ om2f-13-flag1 story-flag?  [: glass-fx effect @ 0<> ;] in-place -> 0 0 }T
testing the mirror smashed: the floor flooded, its remark
om2f-13-breakable-broken story-flag-set  room-afresh
T{ 1 part?  mirror-area 90 at  said -> -1 shattered-msg }T
testing the Rubrum Medallion Hewie sniffed out: taken
om2f-13-rubrum-medallion-found story-flag-set  om2f-13-rubrum-medallion story-flag-clear  room-afresh
rubrum-medallion items:items-of before !
0 88.5e 0e 67.56e put  circle press  300 read-on
T{ om2f-13-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> -1 before @ 1+ }T
test-summary
