\ water-tower-4f-1's place: coming in - the walkway as its side has it, its doors so, the drum, the
\ sparkle, the summoner off (on again as she leaves); the lever pulled - the walkway across (its
\ doors open, creatures let out), then back (shut); the box broken - the Camomile, taken; the
\ Medallion taken; Hewie's bark at the walkway's far end; the drum falling as she first goes on -
\ her fright, Riccardo come for her; back 1800 frames after he left. Headless:
\   build/openhuey --test tests/places/test_water_tower_4f_1.fs
IN: test-water-tower-4f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.walkway story.places.water-tower-4f-1 ;
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
: room-afresh ( -- )  water-tower-4f-1 room!  60 run-frames ;
: by-exit ( exit -- )  water-tower-4f-1 swap rooms send go-to-room  60 run-frames ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: walkway-angle ( F: -- rad )  [: walkway turn-y object@ ;] in-place ;
: locked? ( -- a b )  near-door doors:door-locked?  far-door doors:door-locked? ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
fvariable ax  fvariable ay  fvariable az  fvariable bx  fvariable bz   \ (a gate's ends)
: gate! ( area -- )  dup 0 area-corner drop  az f! ay f! ax f!  1 area-corner drop  bz f! fdrop bx f! ;
: g-dx ( F: -- v )  bx f@ ax f@ f- ;   : g-dz ( F: -- v )  bz f@ az f@ f- ;
: g-len ( F: -- l )  g-dx fdup f*  g-dz fdup f* f+ fsqrt ;
: gate-side ( F: d -- x y z )  {: F: d :}  ax f@ bx f@ f+ 2e f/  g-dz g-len f/ d f* f-  ay f@  az f@ bz f@ f+ 2e f/  g-dx g-len f/ d f* f+ ;
: across ( area F: d -- )   \ (her put d off the gate's middle, then as far the other side: across it)
    {: a F: d :}  a gate!  0 d gate-side put  0 d fnegate gate-side put ;
: lever-done ( -- flag )  [: turning @ 0= ;] in-place  doing-busy? 0= and ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-set
wt2f-1-flag2 story-flag-clear  wt4f-1-flag2 story-flag-clear  wt4f-1-flag3 story-flag-clear  wt4f-1-flag4 story-flag-clear
wt4f-1-breakable-broken story-flag-clear  wt4f-1-medallion story-flag-clear  wt4f-1-camomile story-flag-clear
0 by-exit
testing coming in by exit 0: the walkway home, its doors locked; the drum; the sparkle; the summoner off
T{ wt4f-1-flag3 story-flag?  walkway-angle 0e ~=  [: home-part part?  across-part part? ;] in-place  locked? -> 0 -1 -1 0 -1 -1 }T
T{ [: medallion-fx effect @ 0<>  summoner-was-on @ ;] in-place  summoner-on state-flag? -> -1 -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the lever pulled: the walkway across (-90 degrees), its doors open, creatures let out
T{ lever-area 180 at  circle press  [: lever-done ;] 900 soon -> -1 }T
T{ wt4f-1-flag3 story-flag?  wt4f-1-flag4 story-flag?  walkway-angle pi f2/ fnegate ~=  locked? -> -1 -1 -1 0 0 }T
testing ... and back: its doors shut
T{ lever-area 180 at  circle press  [: lever-done ;] 900 soon -> -1 }T
T{ wt4f-1-flag3 story-flag?  walkway-angle 0e ~=  locked? -> 0 -1 -1 -1 }T
testing ... not facing it: nothing offered
T{ lever-area 0 at  [: fiona 180 50 heading? ;] in-place -> 0 }T
testing coming in by exit 1: the walkway across, its doors open
1 by-exit
T{ wt4f-1-flag3 story-flag?  walkway-angle pi f2/ fnegate ~=  locked? -> -1 -1 0 0 }T
testing leaving: the summoner on again
water-tower-1f-1 room!  60 run-frames
T{ summoner-on state-flag? -> -1 }T
summoner-on state-flag-clear  0 by-exit
testing the box broken: the Camomile shows; taken
T{ -86.45e 47e -8e blow-at  [: box breaks? dup if  camomile-shows  then ;] in-place  wt4f-1-breakable-broken story-flag?  [: camomile-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T
camomile items:items-of before !
T{ 0 -86.45e 46e -8e put  circle press  300 read-on  wt4f-1-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T
testing the Medallion: taken
medallion items:items-of before !
T{ 0 -71.41e 142e -7.67e put  circle press  300 read-on  wt4f-1-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
testing Hewie at the walkway's far end: his bark
hewie-freed story-flag-set  hewie-commandable state-flag-set  0 by-exit  game:hewie send join-fiona  60 run-frames
T{ [: ['] barks-at-end start drop ;] in-place  [: doing-busy? 0= ;] 600 soon -> -1 }T
game:hewie send part-from-fiona  60 run-frames
testing the drum falling as she first goes on: her fright; Riccardo come for her
wt2f-1-flag2 story-flag-set  wt4f-1-flag1 story-flag-clear  c7-second-event story-flag-clear
4 story.base:partner-load  5 run-frames  0 by-exit
T{ on-area 5e across  [: ['] drum-falls running? ;] in-place  wt4f-1-flag2 story-flag? -> -1 -1 }T
T{ [: [: stalker-in? ;] in-place ;] 200 soon  wt4f-1-flag1 story-flag? -> -1 -1 }T
testing ... gone, back 1800 frames after (by the walkway's side: across, its far end)
[: stalker let-go  1 summon-take ;] in-place  60 run-frames
T{ [: return-due @  return-wait @ 1700 1800 within ;] in-place -> -1 -1 }T
[: 1 return-wait ! ;] in-place
T{ [: [: stalker-in? ;] in-place ;] 120 soon  [: stalker body-tri ;] in-place -> -1 944 }T
testing coming in after: the drum gone
[: stalker let-go  1 summon-take ;] in-place  30 run-frames  wt2f-1-flag2 story-flag-clear  0 by-exit
T{ [: drum placed-here? ;] in-place -> -1 }T
test-summary
