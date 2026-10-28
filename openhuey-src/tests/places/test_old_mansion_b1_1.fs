\ old-mansion-b1-1's place: the hopping figure (in; struck where it stands); the vase broken and its
\ Chicken Jerky; Hewie fetching the Album Medallion; brought in by the forced way: dressed afresh,
\ the file. Headless:
\   build/openhuey --test tests/places/test_old_mansion_b1_1.fs
IN: test-old-mansion-b1-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-b1-1 ;
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
: room-afresh ( -- )  old-mansion-b1-1 room!  60 run-frames ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the figure hopping, the hall hunted; struck where it stands
room-afresh
T{ [: hopper-slot figure 0< 0= ;] in-place  hunted state-flag? -> -1 -1 }T
T{ 39.3e 5e 50.4e blow-at  [: the-figure hopper-struck? ;] in-place  [: body-point-off ;] as-her -> -1 }T
testing the vase broken: its Chicken Jerky taken
omb1-1-breakable-broken story-flag-set  omb1-1-chicken-jerky story-flag-clear  room-afresh  chicken-jerky items:items-of before !
T{ 1 part?  0 part? -> -1 0 }T
0 60.5e 0e -37.4e put  circle press  300 read-on
T{ omb1-1-chicken-jerky story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T
testing Hewie fetching the Album Medallion off the ledge
omb1-1-album-medallion story-flag-clear  room-afresh  album-medallion items:items-of before !
[: ['] fetches start drop ;] in-place  600 read-on
T{ omb1-1-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T
testing brought in by the forced way: dressed afresh, the file
old-mansion-b1-1 forced-exit rooms send go-to-room  600 play-on
T{ free?  hall-file filed? -> -1 -1 }T
test-summary
