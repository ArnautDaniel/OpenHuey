\ old-mansion-1f-11's place: the water in the way (then the far door), the collapsed floor, the
\ curtained mirror - drawn back (its glass lit), across again; smashed, its remark; the Album
\ Medallion. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_11.fs
IN: test-old-mansion-1f-11
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-11 ;
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
: room-afresh ( -- )  old-mansion-1f-11 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the water in the way; then the door beyond it
omb1-5-flag3 story-flag-clear  room-afresh
T{ $12 -45 at  said -> water-msg }T
T{ $12 -45 at  said -> far-door-msg }T
testing the collapsed floor, ahead and behind
omb1-5-flag3 story-flag-set  room-afresh
T{ $10 0 at  said -> collapsed-msg }T
T{ $11 90 at  said -> collapsed-msg }T
omb1-5-flag3 story-flag-clear
testing the curtain: drawn back, the glass lit; across again
om1f-11-breakable-broken story-flag-clear  om1f-11-flag1 story-flag-clear  room-afresh
mirror-area 0 at  circle press  [: doing-busy? 0= ;] 600 soon drop
T{ om1f-11-flag1 story-flag?  [: glass-fx effect @ 0<> ;] in-place -> -1 -1 }T
mirror-area 0 at  circle press  [: doing-busy? 0= ;] 600 soon drop
T{ om1f-11-flag1 story-flag?  [: glass-fx effect @ 0<> ;] in-place -> 0 0 }T
testing the mirror smashed: the floor flooded, its remark
om1f-11-breakable-broken story-flag-set  room-afresh
T{ 2 part?  mirror-area 0 at  said -> -1 shattered-msg }T
testing the Album Medallion Hewie sniffed out: taken
om1f-11-album-medallion-found story-flag-set  om1f-11-album-medallion story-flag-clear  room-afresh
album-medallion items:items-of before !
0 13.96e 50e -9.34e put  circle press  300 read-on
T{ om1f-11-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T
test-summary
