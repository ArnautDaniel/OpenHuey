\ old-mansion-1f-2's place: the scales (the coin); an ore laid on them - the Handful of Ash; a
\ Godstone - the door unlocked, taken back; the great door (inscription and file; won't open); the
\ Camomile; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_2.fs
IN: test-old-mansion-1f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-2 ;
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
: room-afresh ( -- )  old-mansion-1f-2 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
: yes-it ( -- )   \ (its choice may start on no; the question on its last page)
    [: key: Left tap  picked 0= dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
medallion-40-done story-flag-clear  room-afresh
testing the scales: the remark, then the coin
T{ $11 180 at  said -> scales-msg }T
T{ $11 180 at  said -> coin-msg }T
testing the great door: its inscription and file; then it won't open
T{ $12 0 at  said -> door-msg }T
300 read-on
T{ inscription-file filed? -> -1 }T
T{ $12 0 at  said -> wont-open-msg }T
testing an ore laid on the scales: gone, the Handful of Ash
silver-ore 1 items send give-item  10 run-frames  silver-ore items:items-of before !
0 ore-laid 0 silver-ore broadcast item-event  1500 play-on
T{ silver-ore items:items-of  got-handful-of-ash story-flag? -> before @ 1- -1 }T
testing a Godstone laid on the scales: level, the great door unlocked; taken back
great-door s" doors" actor-named send lock  white-godstone 1 items send give-item  10 run-frames
0 godstone-laid 0 white-godstone broadcast item-event  1500 play-on
T{ medallion-40-done story-flag?  great-door doors:door-locked?  white-godstone items:items-of -> -1 0 0 }T
$11 180 at  circle press  shown-soon drop  yes-it  300 play-on
T{ white-godstone items:items-of  [: placed @ ;] in-place -> 1 0 }T
testing the Camomile: taken
om1f-2-camomile story-flag-clear  room-afresh  camomile items:items-of before !
0 -72.34e 50e -38.25e put  circle press  300 read-on
T{ om1f-2-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
testing the movies replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-1f-2 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
