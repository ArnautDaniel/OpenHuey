\ old-mansion-b1-4's place: the things to look at; the cabinet (in, out); Daniella at the kiln -
\ the cabinet no use, her scene: she turns, the next of her after Fiona; the replay. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_4.fs
IN: test-old-mansion-b1-4
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-4 ;
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
: room-afresh ( -- )  old-mansion-b1-4 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
game:debilitas send stalker-out  5 run-frames
x-37-flag1 story-flag-set  room-afresh
testing what she may look at
T{ 5 90 at  said -> wax-msg }T
T{ 6 45 at  said -> chess-msg }T
T{ 7 -45 at  said -> hourglass-msg }T
T{ 8 -45 at  said -> models-msg }T
T{ 9 -45 at  said -> flask-msg }T
T{ $A -45 at  said -> kiln-msg }T
T{ $B 90 at  said -> still-msg }T
testing the cabinet: in, hidden; out by the button
cabinet-area 0 at  circle press  [: hidden? ;] 900 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 900 soon drop
T{ hidden?  free? -> 0 -1 }T
testing Daniella at the kiln: the cabinet no use; her scene - the next of her after Fiona
x-37-flag1 story-flag-clear  om1f-13-flag2 story-flag-clear  daniella-kind story.base:partner-load  10 run-frames  room-afresh
T{ [: waiting @ ;] in-place  partner body-room -> -1 old-mansion-b1-4 }T
T{ cabinet-area 0 at  said -> right-there-msg }T
T{ [: ['] she-turns start drop ;] in-place  [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
[: movie-skipped state-flag-set  doing-busy? 0= ;] 1500 soon drop  200 run-frames
T{ x-37-flag1 story-flag?  partner-kind -> -1 kind34-kind }T
partner send stalker-out  10 run-frames
testing the movie replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-b1-4 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
