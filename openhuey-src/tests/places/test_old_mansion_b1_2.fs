\ old-mansion-b1-2's place: the curtain (drawn back - its glass lit, the curtain open; across
\ again); the mirror smashed (flooded, its remark); the plaque; the symbol doors (locked, the symbol;
\ the Jupiter Key - spent once both are open); the book (twice: the file); the dolls; the vases and
\ their Medallions; the Refined Magnesia. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_2.fs
IN: test-old-mansion-b1-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-2 ;
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
: room-afresh ( -- )  old-mansion-b1-2 room!  60 run-frames ;
: ~= ( F: a b -- flag )  f- fabs 0.01e f< ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om1f-12-flag4 story-flag-set
testing the curtain: drawn back, the glass lit; across again
omb1-2-breakable3-broken story-flag-clear  omb1-2-flag1 story-flag-clear  room-afresh
mirror-area 0 at  circle press  [: doing-busy? 0= ;] 600 soon drop
T{ omb1-2-flag1 story-flag?  [: glass-fx effect @ 0<>  s" Curtain" $7C object@ 0e ~= ;] in-place -> -1 -1 -1 }T
mirror-area 0 at  circle press  [: doing-busy? 0= ;] 600 soon drop
T{ omb1-2-flag1 story-flag?  [: glass-fx effect @ 0<>  s" Curtain" $7C object@ 1e ~= ;] in-place -> 0 0 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the mirror smashed: the floor flooded, its remark
omb1-2-breakable3-broken story-flag-set  room-afresh
T{ 4 part?  mirror-area 0 at  said -> -1 shattered-msg }T
testing the plaque
T{ $19 45 at  said -> paths-msg }T
200 read-on
testing the symbol doors: locked, then the symbol; the key in both - spent once both are open
left-door s" doors" actor-named send lock  right-door s" doors" actor-named send lock  room-afresh
2 by-door
T{ said -> locked-msg }T
2 by-door
T{ said -> symbol-msg }T
jupiter-key 1 items send give-item  10 run-frames
2 by-door  0 left-key -1 0 broadcast item-event  400 play-on
T{ left-door doors:door-locked?  jupiter-key items:items-of -> 0 1 }T
3 by-door  0 right-key -1 0 broadcast item-event  400 play-on
T{ right-door doors:door-locked?  jupiter-key items:items-of -> 0 0 }T
testing the book: read twice, the file
T{ $1D -45 at  said -> title-msg }T
200 read-on
T{ $1D -45 at  said -> title-msg }T
200 read-on
T{ $236 filed? -> -1 }T
testing a doll
T{ 0 -29.58e 0e 72e put  said -> doll-msg }T
testing the vases broken: their Medallions
omb1-2-breakable1-broken story-flag-set  omb1-2-viola-medallion story-flag-clear  room-afresh  viola-medallion items:items-of before !
0 13.5e -10e -79e put  circle press  300 read-on
T{ omb1-2-viola-medallion story-flag?  viola-medallion items:items-of -> -1 before @ 1+ }T
testing the Refined Magnesia: taken
omb1-2-refined-magnesia story-flag-clear  room-afresh  refined-magnesia items:items-of before !
0 -109.83e 0e 68.62e put  circle press  300 read-on
T{ omb1-2-refined-magnesia story-flag?  refined-magnesia items:items-of -> -1 before @ 1+ }T
test-summary
