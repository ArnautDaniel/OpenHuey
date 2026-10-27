\ old-mansion-1f-21's place: the four vases - the first broken holds the Album Medallion; the door
\ unlocked from this side. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_21.fs
IN: test-old-mansion-1f-21
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-21 ;
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
: room-afresh ( -- )  old-mansion-1f-21 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the vases whole; broken, in pieces
om1f-21-breakable1-broken story-flag-clear  om1f-21-breakable2-broken story-flag-set  room-afresh
T{ 0 part?  1 part?  2 part?  3 part? -> -1 0 0 -1 }T
testing the first broken: its Album Medallion taken
om1f-21-breakable1-broken story-flag-set  om1f-21-album-medallion story-flag-clear  room-afresh
album-medallion items:items-of before !
0 20e 0e -76.5e put  circle press  300 read-on
T{ om1f-21-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T
testing the door locked from this side: unlocked
variable the-door  [: 0 exit-door ;] in-place the-door !
the-door @ s" doors" actor-named send lock  10 run-frames  0 by-door  circle press  300 read-on
T{ the-door @ doors:door-locked? -> 0 }T
test-summary
