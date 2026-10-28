\ house-of-truth-b1-3's place: the first time in, Lorenzo leaves her be (its fact, the summoner
\ off); not again; its ways. Headless:
\   build/openhuey --test tests/places/test_house_of_truth_b1_3.fs
IN: test-house-of-truth-b1-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.house-of-truth story.places.house-of-truth-b1-3 ;
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
: room-afresh ( -- )  house-of-truth-b1-3 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
: lorenzo-loaded ( -- )  lorenzo2-kind story.words:partner-load  5 run-frames ;
: lorenzo-out ( -- )  game:lorenzo2 send stalker-out  10 run-frames ;
: lorenzo-here? ( -- flag )  [: stalker-in? ;] in-place ;
: on-tri? ( tri -- flag )  [: stalker body-tri = ;] in-place ;
htb1-3-flag1 story-flag-clear  summoner-on state-flag-set  room-afresh
testing the first time in: its fact, the summoner off
T{ htb1-3-flag1 story-flag?  summoner-on state-flag? -> -1 0 }T
testing not again
summoner-on state-flag-set  room-afresh
T{ summoner-on state-flag? -> -1 }T
summoner-on state-flag-clear
testing its ways: out by exit 1's area; in by each
T{ 1 0 at  [: room-id house-of-truth-b1-3 <> ;] 300 soon -> -1 }T
: in-by ( exit -- n )  house-of-truth-b1-3 swap rooms send go-to-room  60 run-frames  [: came-by @ ;] in-place ;
T{ 0 in-by  1 in-by -> 0 1 }T
test-summary
