\ chaos-forest-10's place: its fourteen areas counted (the engine's area-count); once Hewie is found,
\ him lying here hurt, whining now and then; she comes up to him (crossing toward him): the scene -
\ its fact, files, the stalker sent in, his commands off; then her word to him; the crate broken by
\ a blow, the Medallion behind it, taken; the replay. Headless:
\   build/openhuey --test tests/places/test_chaos_forest_10.fs
IN: test-chaos-forest-10
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.forest story.places.chaos-forest-10 ;
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
: room-afresh ( -- )  chaos-forest-10 room!  60 run-frames ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
cf-10-breakable-broken story-flag-clear  cf-10-medallion story-flag-clear  found-file-247 story-flag-clear
room-afresh
testing the room's areas: fourteen (its table padded after them)
T{ area-count -> 14 }T
testing the crate broken by a blow, the Medallion behind it, taken
T{ 76.5e -25e 134.5e blow-at  [: the-crate breaks? dup if  medallion-shows  then ;] in-place -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
[: body-point-off ;] her  medallion items:items-of before !
T{ 0 76.5e -25.5e 134.5e put  circle press  300 read-on  cf-10-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing Hewie found (and the heart's scene not yet): lying here hurt, whining now and then
hewie-freed story-flag-set  found-file-247 story-flag-set  cf-13-flag1 story-flag-clear  cf-10-flag1 story-flag-clear
room-afresh
T{ [: hewie-lying @ ;] in-place  [: hewie here? ;] in-place -> -1 -1 }T
T{ [: [: whine-frames @ 0> ;] in-place ;] 200 soon -> -1 }T
testing she comes up to him: the scene - its fact, its files; the stalker sent in, his commands off
hewie-commandable state-flag-set  plain-commands state-flag-set
0 40e -40e 54e put
T{ 0 20e -43e 54e put  cf-10-flag1 story-flag? -> -1 }T
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  [: ['] bandaged running? 0= ;] in-place ;] 2000 soon -> -1 }T
T{ shot-seen resident-flag?  shot-file filed? -> -1 -1 }T
T{ hewie-commandable state-flag?  plain-commands state-flag? -> 0 0 }T
T{ [: hewie-lying @ ;] in-place -> -1 }T
300 play-on
testing then her word to him as she faces him close by
fvariable hx  fvariable hy  fvariable hz
: by-him ( -- )  [: hewie body-pos ;] in-place  hz f!  hy f!  hx f!  -90 hx f@ 6e f+  hy f@  hz f@  put ;
by-him
T{ said -> stay-still-msg }T
testing the movie replayed (the extras): played, then back by the forced way
events-held state-flag-set  chaos-forest-10 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 600 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon -> -1 }T
test-summary
