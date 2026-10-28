\ old-mansion-3f-1's place: the boxes (placed; one pushed home - stopped, the light up a step);
\ the first time in, the scene (Daniella after her, the door shut); all set, her on the middle -
\ her death (the files); after, her body, the Mars Key; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_3f_1.fs
IN: test-old-mansion-3f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-3f-1 ;
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
: room-afresh ( -- )  old-mansion-3f-1 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  5 run-frames
om3f-1-flag1 story-flag-clear  om3f-1-flag2 story-flag-clear  room-afresh
testing the boxes: a box on its square - stopped, the light up a step
T{ 0 part?  1 part?  8 part? -> -1 0 -1 }T
0 0 0 $20A $4D8 obstacle-in  20 run-frames
T{ [: stopped-mask @  set-boxes @ ;] in-place  3 part? -> 1 1 -1 }T
testing the first time in: the scene, Daniella after her, the door shut behind
daniella-kind story.words:partner-load  10 run-frames  room-afresh
T{ [: ['] stepped-in-first start drop ;] in-place  [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
[: movie-skipped state-flag-set  doing-busy? 0= ;] 2000 soon drop  100 run-frames
T{ om3f-1-flag1 story-flag?  hall-door doors:door-locked?  partner-kind -> -1 -1 35 }T
testing all four set, Daniella on the middle: her death - the files, the key
T{ [: $F stopped-mask !  om3f-1-flag2 fact!  ['] glass-gives start drop ;] in-place  [: cutscene-active? movie-status 1 = or ;] 1500 soon -> -1 }T
[: movie-skipped state-flag-set  doing-busy? 0= ;] 3000 soon drop  200 play-on
T{ $23E filed?  hall-door doors:door-locked?  1 part? -> -1 0 -1 }T
testing after: her body, its remarks; the Mars Key
T{ 5 0 at  said -> pinned-msg }T
T{ 5 0 at  said -> cold-msg }T
got-mars-key story-flag-clear  room-afresh  mars-key items:items-of before !
0 1.29e 0e -21.02e put  circle press  300 read-on
T{ got-mars-key story-flag?  mars-key items:items-of -> -1 before @ 1+ }T
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-3f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
test-summary
