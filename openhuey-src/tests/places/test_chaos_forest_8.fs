\ chaos-forest-8's place: the first time in, the scene - Hewie found (the file, fact, his trust); the
\ crates broken by a blow (a blow elsewhere missing), the Medallion and the Chicken Jerky behind two,
\ taken; the sundial's remark, then the save; the figures loaded ahead by the way on and let go
\ again; the replay. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_8.fs
IN: test-chaos-forest-8
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.chaos-forest-8 ;
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
: room-afresh ( -- )  chaos-forest-8 room!  60 run-frames ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
: whole? ( i -- flag )  [: crate@ broken? 0= ;] in-place ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
cf-8-breakable1-broken story-flag-clear  cf-8-breakable3-broken story-flag-clear  cf-8-breakable5-broken story-flag-clear
cf-8-medallion story-flag-clear  cf-8-chicken-jerky story-flag-clear  sundial-examined story-flag-clear
testing the first time in: the scene - Hewie found; its file, the fact; plain commands after
found-file-247 story-flag-clear  plain-commands state-flag-clear
chaos-forest-8 0 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  found-file-247 story-flag? ;] 2000 soon  [: doing-busy? 0= ;] 900 soon -> -1 -1 }T
T{ found-file filed?  plain-commands state-flag? -> -1 -1 }T
300 play-on
testing then the crates whole as she comes in; a blow elsewhere misses; on the third breaks it
room-afresh
T{ 0 whole?  2 whole?  4 whole? -> -1 -1 -1 }T
T{ -330e 2e -110e blow-at  [: crate-3 breaks? ;] in-place -> 0 }T
T{ -381e 2e -106.4e blow-at  [: crate-3 breaks? ;] in-place  cf-8-breakable3-broken story-flag? -> -1 -1 }T
[: body-point-off ;] her
testing the first: the Medallion behind it, taken
T{ -367e 2e -118.7e blow-at  [: crate-1 breaks? dup if  medallion-shows  then ;] in-place -> -1 }T
[: body-point-off ;] her  medallion items:items-of before !
T{ 0 -367e 1e -118.7e put  circle press  300 read-on  cf-8-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing the fifth: the Chicken Jerky behind it, taken
T{ -275.5e 2e -96e blow-at  [: crate-5 breaks? dup if  jerky-shows  then ;] in-place -> -1 }T
[: body-point-off ;] her  chicken-jerky items:items-of before !
T{ 0 -275.5e 1e -96e put  circle press  300 read-on  cf-8-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
testing coming in again: broken as they were, the pickups gone
room-afresh
T{ 0 whole?  4 whole?  2 whole?  1 whole?  [: medallion-fx effect @  jerky-fx effect @ ;] in-place -> 0 0 0 -1 0 0 }T
testing the sundial: its remark; then the save
T{ sundial-area 0 at  [: fiona sundial-area 50 faces-area? ;] in-place -> -1 }T
T{ said  sundial-examined story-flag? -> sundial-msg -1 }T
T{ circle press  shown-soon -> clock-msg }T
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
[: key: Right tap  picked 1 = dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press  300 read-on   \ (no save)
testing the way on (crossing into area 3, a gate): what is there loaded ahead; back, let go
0 -44e 10e 0e put
T{ 0 -44e 10e 20e put  [: scene-slot-2 figure 0< 0=  ahead-slot-2 figure 0< 0= ;] in-place -> -1 -1 }T
T{ 0 -44e 10e 0e put  [: scene-slot-2 figure 0<  ahead-slot-2 figure 0< ;] in-place -> -1 -1 }T
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  chaos-forest-8 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
