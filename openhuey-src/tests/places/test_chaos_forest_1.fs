\ chaos-forest-1's place: coming in - the figure hopping (the kit's hopper), the Forest Map on the
\ stump; the figure struck: it hops back, counted; the map taken; Hewie wary of the figure once a
\ visit; the way on before Hewie is found - out to chaos-forest-8 (its scene); its ways once he is.
\ Headless:
\   build/openhuey --test tests/places/test_chaos_forest_1.fs
IN: test-chaos-forest-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.chaos-forest-1 ;
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
: room-afresh ( -- )  chaos-forest-1 room!  60 run-frames ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
found-file-247 story-flag-set  got-forest-map story-flag-clear  room-afresh
testing coming in: the figure hopping, the map on the stump, hunted
T{ [: hopper-slot figure 0< 0= ;] in-place  map-part [: part? ;] in-place  hunted state-flag? -> -1 -1 -1 }T
testing the figure struck: it hops back (once counted)
: hit ( -- )  the-figure hops-back ;
T{ 22e -10e 145e blow-at  [: the-figure hopper-struck? dup if  ['] hit start drop  then ;] in-place -> -1 }T
T{ 5 run-frames  [: the-figure hp.hops @ ;] in-place -> 1 }T
[: body-point-off ;] her  60 run-frames
testing the Forest Map taken: its part gone
forest-map items:items-of before !
T{ map-area 90 at  circle press  300 read-on  got-forest-map story-flag?  map-part [: part? ;] in-place  forest-map items:items-of -> -1 0 before @ 1+ }T
testing Hewie by the figure: wary once a visit
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
chaos-forest-1 22.39e -16e 144.46e nav-tri 0 game:hewie send hewie-place  30 run-frames  22.39e -16e 144.46e hewie-spot
T{ [: [: wary-noticed @ ;] in-place ;] 120 soon -> -1 }T
300 run-frames
testing its ways: out by exit 0's area; in by each
T{ 0 0 at  [: room-id chaos-forest-1 <> ;] 300 soon -> -1 }T
: in-by ( exit -- n )  chaos-forest-1 swap rooms send go-to-room  60 run-frames  [: came-by @ ;] in-place ;
T{ 0 in-by  1 in-by -> 0 1 }T
testing the way on before Hewie is found: the scene's figures, out by exit 1 - to chaos-forest-8
found-file-247 story-flag-clear  room-afresh
T{ way-on-area 0 at  [: room-id chaos-forest-8 = ;] 600 soon -> -1 }T
600 play-on
test-summary
