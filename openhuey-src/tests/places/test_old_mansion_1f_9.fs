\ old-mansion-1f-9's place: a plate given back; the window (the blinds, the dark, the first time
\ Daniella's scene); the water (calmed, the document; then refreshed); the tub; the title; the
\ Refined Antimony, the Refined Magnesia; the replay. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_9.fs
IN: test-old-mansion-1f-9
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-9 ;
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
: room-afresh ( -- )  old-mansion-1f-9 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
om1f-13-flag1 story-flag-set  om1f-9-flag1 story-flag-clear  room-afresh
testing a plate given back: her word
0 1 -1 0 broadcast item-event
T{ shown-soon -> returned-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)
200 read-on
testing the window: the blinds opened; once seen, the dark
T{ 1 -45 at  said -> blinds-msg }T
om1f-9-flag1 story-flag-set
T{ 1 -45 at  said -> pitch-black-msg }T
testing the water: calmed, its document the first time; then refreshed
got-doc-recovery story-flag-clear
T{ 3 15 at  said  [: rested @ ;] in-place -> water-msg -1 }T
200 play-on
T{ got-doc-recovery story-flag?  3 15 at  said -> -1 refreshed-msg }T
testing the tub, the title
T{ 4 -160 at  said -> tub-msg }T
T{ 5 0 at  said -> title-msg }T
200 read-on
testing the Refined Antimony: taken
om1f-9-refined-antimony story-flag-clear  room-afresh  refined-antimony items:items-of before !
0 -7.97e 0e 17.3e put  circle press  300 read-on
T{ om1f-9-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
testing the Refined Magnesia Hewie sniffed out: taken
om1f-9-refined-magnesia-found story-flag-set  om1f-9-refined-magnesia story-flag-clear  room-afresh
refined-magnesia items:items-of before !
0 7.72e 0e 11.08e put  circle press  300 read-on
T{ om1f-9-refined-magnesia story-flag?  refined-magnesia items:items-of -> -1 before @ 1+ }T
testing the window the first time: Daniella's scene, the windows opened, the file
om1f-13-flag1 story-flag-clear  om1f-9-flag1 story-flag-clear  room-afresh
1 -45 at  circle press  shown-soon drop  [: cross press  20 run-frames  window-shown 0< ;] 20 soon drop   \ (yes)
T{ [: cutscene-active? movie-status 1 = or ;] 300 soon -> -1 }T
T{ [: movie-skipped state-flag-set  doing-busy? 0= ;] 1500 soon  om1f-13-flag1 story-flag?  cage-file filed? -> -1 -1 -1 }T
testing the movie replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-1f-9 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
