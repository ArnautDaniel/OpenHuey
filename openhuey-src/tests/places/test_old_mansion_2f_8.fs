\ old-mansion-2f-8's place: the curtain (in, the curtain drawn; out by the button); the lamp; the
\ hole (no); the painting (insects, then the mother); the bones; the book and its recipes; the door
\ behind her; the Camomile; the wall giving way. Headless:
\   build/openhuey --test tests/places/test_old_mansion_2f_8.fs
IN: test-old-mansion-2f-8
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-2f-8 ;
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
: room-afresh ( -- )  old-mansion-2f-8 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: facing-middle ( area -- )  area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
: no-it ( -- )   \ (the choice's second; the question on its last page)
    [: key: Right tap  picked 1 = dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press ;
: curtain ( -- ) ( F: -- t )  s" Curtain" $7C object@ ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
found-file-234 story-flag-set  om2f-8-flag1 story-flag-clear  room-afresh
testing the curtain: in, drawn round her; out by the button
$18 180 at  circle press  [: hidden? ;] 900 soon drop
T{ hidden?  [: curtain 1e f- fabs 0.01e f< ;] in-place -> -1 -1 }T
circle press  [: doing-busy? 0= ;] 900 soon drop
T{ hidden?  free? -> 0 -1 }T
testing the lamp: unlit
T{ $16 facing-middle  said -> unlit-msg }T
testing the hole: no
$17 180 at  circle press  shown-soon drop  no-it  200 read-on
T{ room-id -> old-mansion-2f-8 }T
testing the painting: insects, then the mother; the bones; the book
T{ $1A 90 at  said -> insects-msg }T
T{ $1A 90 at  said -> mother-msg }T
T{ $1B -180 at  said -> bones-msg }T
T{ $1C 0 at  said -> written-msg }T
300 read-on
T{ $43 items:item-bit?  $45 items:item-bit? -> -1 -1 }T
testing the door behind her: water pouring in
0 exit-door s" doors" actor-named send lock  10 run-frames
0 by-door
T{ said -> water-msg }T
testing the Camomile: taken
om2f-8-camomile story-flag-clear  room-afresh  camomile items:items-of before !
0 48e 0e -57e put  circle press  300 read-on
T{ om2f-8-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
testing the wall giving way: out by the forced way
T{ [: ['] wall-gives start drop ;] in-place  [: room-id old-mansion-2f-8 <> ;] 900 soon -> -1 }T
test-summary
