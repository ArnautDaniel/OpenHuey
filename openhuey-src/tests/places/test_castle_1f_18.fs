\ castle-1f-18's place (scripts/story/places/castle-1f-18.fs): the moth room - the moths at the
\ lamp (their remark, the file, Hewie wary of them), the torn picture, the clock, the empty vase,
\ the Fresh Flowers laid (the scene: the moths drawn to them, the door unlocked), the replay.
\ Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_1f_18.fs
IN: test-castle-1f-18
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-1f-18 items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: room-afresh ( -- )  castle-1f-18 room!  60 run-frames ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: moths ( -- n )  [: 0  #swarms 0 do  i effect @ alive? if  1+  then  loop ;] in-place ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
: hewie-at ( F: x y z -- )  on-floor {: F: x y z :}  x f>cell y f>cell z f>cell 0e f>cell  game:hewie send place ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing the moths at the lamp (the door shut): eleven swarms
flowers-laid story-flag-clear  [: lamp-door lock ;] in-place  room-afresh
T{ moths  lamp-door doors:door-locked?  flowers-part part? -> #swarms -1 0 }T
: swarm-at ( k -- ok ) ( F: -- x y z )  [: effect @ ;] in-place enter  effects:particles @  leave-actor  butterflies-at ;
: near? ( F: x y z x' y' z' -- flag )  {: F: x y z a b c :}  x a f- fabs 6e f<  y b f- fabs 6e f<  and  z c f- fabs 6e f<  and ;   \ (they wander about it)
[: 3 0 -30e 12e -41e moths-to ;] in-place  400 run-frames   \ (sent off: 0.4 a frame - there by now)
T{ 3 swarm-at  -30e 12e -41e near? -> -1 -1 }T

testing the remarks: the torn picture (in turn), the empty vase
T{ 90 55e 4e -6e put  said -> torn-picture-msg }T
T{ 90 55e 4e -6e put  said -> torn-again-msg }T
T{ -90 -22.35e 0e -42.76e put  said -> empty-vase-msg }T

testing stepping near the moths: turned to them, the remark, the file
T{ 180 43e 4e -30e put  shown-soon  50 play-on  moths-file filed? -> moths-msg -1 }T

testing Hewie near them, at her command: once a visit he goes over, wary
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
game:hewie send join-fiona  60 run-frames
40e 4e -30e hewie-at  20 run-frames
T{ [: hewie-wary @ ;] in-place -> -1 }T
200 run-frames

testing the Fresh Flowers laid in the vase: the scene - the door unlocked, the flowers spent, the moths over by them
room-afresh  fresh-flowers 1 items send give-item  10 run-frames
-90 -22.35e 0e -42.76e put  0 use-on-place  900 play-on
T{ flowers-laid story-flag?  lamp-door doors:door-locked?  flowers-part part?  fresh-flowers items:items-of  moths -> -1 0 -1 0 #swarms }T
T{ -90 -25e 0e -44e put  shown-soon  50 play-on -> moths-gone-msg }T   \ (stepping near them, by the vase now)

testing the clock: saving (or Hewie not by her)
room-afresh
T{ 180 0e 4e -40e put  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames   \ (the text out; no)
T{ doing-busy?  window-shown 0< -> 0 -1 }T

testing the movie replayed (the extras): played, out
0 $2A progress pr.vars + c!  events-held state-flag-set  room-afresh  400 play-on
T{ room-id  events-held state-flag? -> castle-1f-18 0 room-exit-leads drop 0 }T
test-summary
