\ water-tower-7f-2's place: coming in - hunted, the sparkle; after the planets' scene (the forced
\ way: she and Hewie at their spots, the world let go); back by the second forced way (Hewie by her,
\ she shown); the clock; the inscription; the Rubrum Medallion Hewie sniffs out, then taken; leaving -
\ no longer hunted. Headless:
\   build/openhuey --test tests/places/test_water_tower_7f_2.fs
IN: test-water-tower-7f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.water-tower-7f-2 ;
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
: by-exit ( exit -- )  water-tower-7f-2 swap rooms send go-to-room  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: answer-no ( -- )  60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
wt7f-2-rubrum-medallion-found story-flag-set  wt7f-2-rubrum-medallion story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  game:hewie send join-fiona
testing coming in after the planets (the forced way): she, Hewie at their spots; the world let go; hunted
world-held state-flag-set  water-tower-7f-2 -1 0 game:hewie send hewie-place  forced-exit by-exit
T{ [: her-tri @ ;] her  [: self body-tri ;] his  world-held state-flag?  hunted state-flag?  [: medallion-fx effect @ 0<> ;] in-place -> $1C2 $6D 0 -1 -1 }T
testing back by the second forced way: she at the door, Hewie by her, the world let go
world-held state-flag-set  back-way by-exit
T{ [: self body-tri ;] his  world-held state-flag?  [: doing-busy? ;] drop  0 -> $344 0 0 }T
testing the clock: saving (or Hewie not by her) - no
T{ clock-area 0 at  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
answer-no
T{ doing-busy? -> 0 }T
testing the inscription
T{ inscription-area 0 at  [: fiona inscription-area 50 faces-area? ;] in-place -> -1 }T
T{ said -> inscription-msg }T
testing the Rubrum Medallion: taken
rubrum-medallion items:items-of before !
T{ 0 -9e 141e -10.23e put  circle press  300 read-on  wt7f-2-rubrum-medallion story-flag?  rubrum-medallion items:items-of -> -1 before @ 1+ }T
testing ... Hewie sniffs it out first
wt7f-2-rubrum-medallion-found story-flag-clear  wt7f-2-rubrum-medallion story-flag-clear  0 by-exit  60 run-frames
T{ [: medallion-fx effect @ 0<> ;] in-place -> 0 }T
[: ['] medallion-sniffed start drop ;] in-place
T{ [: wt7f-2-rubrum-medallion-found story-flag? ;] 600 soon -> -1 }T
400 run-frames
testing leaving: no longer hunted
T{ water-tower-1f-1 room!  60 run-frames  hunted state-flag? -> 0 }T
test-summary
