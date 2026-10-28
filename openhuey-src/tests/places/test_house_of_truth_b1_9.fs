\ house-of-truth-b1-9's place: one step in and the dark takes her - the white room by the forced
\ way (the B1's track); its door kept from Hewie and the stalker. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_b1_9.fs
IN: test-house-of-truth-b1-9
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-b1-9 ;
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
: room-afresh ( -- )  house-of-truth-b1-9 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
: lorenzo-loaded ( -- )  lorenzo2-kind story.words:partner-load  5 run-frames ;
: lorenzo-out ( -- )  game:lorenzo2 send stalker-out  10 run-frames ;
: lorenzo-here? ( -- flag )  [: stalker-in? ;] in-place ;
: on-tri? ( tri -- flag )  [: stalker body-tri = ;] in-place ;
room-afresh
testing its door kept from Hewie and the stalker
T{ dark-door 2 doors:locked-against?  dark-door 1 doors:locked-against? -> -1 -1 }T
testing a step in: the dark takes her - the white room
[: house-of-truth-b1-4 preload ;] in-place
T{ 0 0 at  [: room-id house-of-truth-b1-4 = ;] 900 soon -> -1 }T
600 play-on
test-summary
