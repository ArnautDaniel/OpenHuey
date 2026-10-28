\ old-mansion-1f-17's place: the picture (its two remarks), the mummy (the fright, then the
\ question), the Chicken Jerky, the Medallion, the vase broken and its Magnesia; brought in by the
\ way behind the wall, the movie; the replay. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_17.fs
IN: test-old-mansion-1f-17
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-17 ;
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
: room-afresh ( -- )  old-mansion-1f-17 room!  60 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the picture: its title, then the lady with the lamp
om2f-8-flag1 story-flag-clear  room-afresh
T{ $E -45 at  said -> picture-msg }T
T{ $E -45 at  said -> lamp-msg }T
testing the mummy: the fright the first time, then the question
mummy-examined story-flag-clear
T{ $10 45 at  said  mummy-examined story-flag? -> mummy-msg -1 }T
T{ $10 45 at  said -> mummy-2-msg }T
testing the Chicken Jerky: taken
om1f-17-chicken-jerky story-flag-clear  room-afresh  chicken-jerky items:items-of before !
0 -137.37e -50e -18.57e put  circle press  300 read-on
T{ om1f-17-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
testing the Medallion Hewie sniffed out: taken
om1f-17-medallion-found story-flag-set  om1f-17-medallion story-flag-clear  room-afresh  medallion items:items-of before !
0 68.99e 0e 15.99e put  circle press  300 read-on
T{ om1f-17-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing the vase broken: its Magnesia
om1f-17-breakable1-broken story-flag-set  om1f-17-magnesia story-flag-clear  room-afresh  magnesia items:items-of before !
T{ 1 part?  0 part? -> -1 0 }T
0 43e 0e -44e put  circle press  300 read-on
T{ om1f-17-magnesia story-flag?  magnesia items:items-of -> -1 before @ 1+ }T
testing brought in by the way behind the wall: the movie, out by the forced way
off-map-0d room!  30 run-frames   \ (from another room: the forced way out goes back to the room left)
om2f-8-flag1 story-flag-set  old-mansion-1f-17 forced-exit rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  room-id old-mansion-1f-17 <> ;] 1500 soon -> -1 }T
testing the movie replayed (the extras): played, then back by the forced way
200 run-frames  events-held state-flag-set  old-mansion-1f-17 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
