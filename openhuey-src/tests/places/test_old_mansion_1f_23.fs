\ old-mansion-1f-23's place: the hiding place (in, out), the save clock, the Mansion Sketch, the
\ Viride Medallion and the Medallion. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_23.fs
IN: test-old-mansion-1f-23
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-23 ;
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
: room-afresh ( -- )  old-mansion-1f-23 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the hiding place: in, hidden; out by the button
room-afresh  hide-area -45 at  circle press  [: hidden? ;] 600 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the clock: saving (or Hewie not by her)
room-afresh
T{ $B 0 at  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames   \ (the text out; no)
T{ doing-busy?  window-shown 0< -> 0 -1 }T
testing the Mansion Sketch: taken, gone from the table
got-mansion-sketch story-flag-clear  room-afresh  mansion-sketch items:items-of before !
$D 45 at  circle press  300 read-on
T{ got-mansion-sketch story-flag?  mansion-sketch items:items-of  0 part? -> -1 before @ 1+ 0 }T
testing the Viride Medallion Hewie sniffed out: taken
om1f-23-viride-medallion-found story-flag-set  om1f-23-viride-medallion story-flag-clear  room-afresh
viride-medallion items:items-of before !
0 0.03e 0e -89.54e put  circle press  300 read-on
T{ om1f-23-viride-medallion story-flag?  viride-medallion items:items-of -> -1 before @ 1+ }T
testing the Medallion Hewie sniffed out: taken
om1f-23-medallion-found story-flag-set  om1f-23-medallion story-flag-clear  room-afresh
medallion items:items-of before !
0 109.8e -10e -98.58e put  circle press  300 read-on
T{ om1f-23-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
test-summary
