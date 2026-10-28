\ chaos-forest-13's place: whether Hewie was kept well (bandaged, or his mistreatment under 100) and
\ the way on it gives (the water tower, or the old mansion's 2F once the cage's garden is seen; else
\ the other); the first time in once Hewie has been found, the scene - its fact, then dressed anew
\ (costume 3, her bare feet's sounds) and out by the forced way to the water tower; its way out; the
\ replay. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_13.fs
IN: test-chaos-forest-13
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-13 ;
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
: room-afresh ( -- )  chaos-forest-13 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
cf-10-flag1 story-flag-clear  fg-3-flag3 story-flag-clear  0 progress pr.hewie-mistreated l!
found-file-247 story-flag-set  cf-13-flag1 story-flag-set  room-afresh
testing Hewie kept well: never mistreated - the tower; mistreated (100) - the other way; bandaged - the tower again
T{ [: kept-well?  way-on ;] in-place -> -1 tower-way }T
100 progress pr.hewie-mistreated l!
T{ [: kept-well?  way-on ;] in-place -> 0 other-way }T
cf-10-flag1 story-flag-set
T{ [: kept-well?  way-on ;] in-place -> -1 tower-way }T
testing ... the cage's garden seen: the old mansion's 2F
fg-3-flag3 story-flag-set
T{ [: way-on ;] in-place -> mansion-way }T
fg-3-flag3 story-flag-clear  0 progress pr.hewie-mistreated l!
testing its way out
T{ 0 0 at  [: room-id chaos-forest-13 <> ;] 300 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the first time in, Hewie found: the scene - its fact; dressed anew, out by the forced way to the tower
cf-13-flag1 story-flag-clear  room-afresh
0 25e 30e -20e put
T{ 0 5e 30e -20e put  cf-13-flag1 story-flag? -> -1 }T
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  room-id tower-way = ;] 2000 soon -> -1 }T
T{ $26 progress pr.vars + c@  new-game-sounds state-flag? -> 3 -1 }T
600 play-on  free-play  10 run-frames  0 game:fiona send costume
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  chaos-forest-13 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
