\ water-tower-2f-1's place: coming in - the walkway as its side has it, the sparkles, the summoner off
\ (on again as she leaves); the walkway half way on its turn (eased: 45 degrees); the lever pulled - the
\ walkway across, then back; the crate broken - a creature let out; the Medallion and the Refined
\ Antimony taken; the Medallion Hewie sniffs out; his bark at the walkway's far end; Riccardo heard
\ prowling, then come for her by her way up, and back 1800 frames after he left; his side of the room.
\ Headless:
\   build/openhuey --test tests/places/test_water_tower_2f_1.fs
IN: test-water-tower-2f-1
USING: tester hewie.state engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.places.walkway story.places.water-tower-2f-1 ;
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
: room-afresh ( -- )  water-tower-2f-1 room!  60 run-frames ;
: by-exit ( exit -- )  water-tower-2f-1 swap rooms send go-to-room  60 run-frames ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: walkway-angle ( F: -- rad )  [: walkway turn-y object@ ;] in-place ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
fvariable ax  fvariable ay  fvariable az  fvariable bx  fvariable bz   \ (a gate's ends)
: gate! ( area -- )  dup 0 area-corner drop  az f! ay f! ax f!  1 area-corner drop  bz f! fdrop bx f! ;
: g-dx ( F: -- v )  bx f@ ax f@ f- ;   : g-dz ( F: -- v )  bz f@ az f@ f- ;
: g-len ( F: -- l )  g-dx fdup f*  g-dz fdup f* f+ fsqrt ;
: gate-side ( F: d -- x y z )  {: F: d :}  ax f@ bx f@ f+ 2e f/  g-dz g-len f/ d f* f-  ay f@  az f@ bz f@ f+ 2e f/  g-dx g-len f/ d f* f+ ;
: across ( area F: d -- )   \ (her put d off the gate's middle, then as far the other side: across it)
    {: a F: d :}  a gate!  0 d gate-side put  0 d fnegate gate-side put ;
: hewie-to-side ( F: d -- )   \ (Hewie put on her triangle d off the gate's middle - she put there first)
    0 gate-side put  [: her-tri @ ;] her  [: hewie swap 0 place-on-tri ;] in-place  10 run-frames ;
: hewie-across ( area F: d -- )  {: a F: d :}  a gate!  d hewie-to-side  d fnegate hewie-to-side ;
: lever-done ( -- flag )  [: turning @ 0= ;] in-place  doing-busy? 0= and ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-set
wt2f-1-flag2 story-flag-clear  wt2f-1-flag4 story-flag-clear  wt2f-1-breakable-broken story-flag-clear
wt2f-1-medallion-found story-flag-set  wt2f-1-medallion story-flag-clear  wt2f-1-refined-antimony story-flag-clear
0 by-exit
testing coming in by exit 0: the walkway home, its parts and floors so; the sparkles; the summoner off
T{ wt2f-1-flag3 story-flag?  walkway-angle 0e ~=  [: home-part part?  across-part part? ;] in-place -> 0 -1 -1 0 }T
T{ [: medallion-fx effect @ 0<>  antimony-fx effect @ 0<>  summoner-was-on @ ;] in-place  summoner-on state-flag? -> -1 -1 -1 0 }T
testing the walkway half way on its turn: 45 degrees
T{ [: 60 turn-step !  eased ;] in-place  45e ~= -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the lever pulled: the walkway across (its parts, the flags)
T{ lever-area -90 at  circle press  [: lever-done ;] 900 soon -> -1 }T
T{ wt2f-1-flag3 story-flag?  wt2f-1-flag4 story-flag?  walkway-angle pi f2/ ~=  [: home-part part?  across-part part? ;] in-place -> -1 -1 -1 0 -1 }T
testing ... and back
T{ lever-area -90 at  circle press  [: lever-done ;] 900 soon -> -1 }T
T{ wt2f-1-flag3 story-flag?  walkway-angle 0e ~=  [: home-part part? ;] in-place -> 0 -1 -1 }T
testing ... not facing it: nothing offered
T{ lever-area 90 at  [: fiona -90 50 heading? ;] in-place -> 0 }T
testing coming in by exit 1: the walkway across
1 by-exit
T{ wt2f-1-flag3 story-flag?  walkway-angle pi f2/ ~= -> -1 -1 }T
testing leaving: the summoner on again
0 0 at  [: room-id water-tower-2f-1 <> ;] 300 soon drop  60 run-frames
T{ summoner-on state-flag? -> -1 }T
summoner-on state-flag-clear  0 by-exit
testing the crate broken: a creature let out
T{ -16.5e 153e -47.5e blow-at  [: crate breaks? ;] in-place  wt2f-1-breakable-broken story-flag? -> -1 -1 }T
T{ [: crate-broken-part part?  crate-part part? ;] in-place -> -1 0 }T
testing the Medallion, the Refined Antimony: taken
medallion items:items-of before !
T{ 0 4.12e 105e -18.51e put  circle press  300 read-on  wt2f-1-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T
refined-antimony items:items-of before !
T{ 0 -19.9e 153e 74.61e put  circle press  300 read-on  wt2f-1-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
testing ... Hewie sniffs the Medallion out
wt2f-1-medallion-found story-flag-clear  wt2f-1-medallion story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  0 by-exit  game:hewie send join-fiona  60 run-frames
[: ['] medallion-sniffed start drop ;] in-place
T{ [: wt2f-1-medallion-found story-flag? ;] 600 soon -> -1 }T
400 run-frames
testing Hewie at the walkway's far end: his bark (once a visit)
T{ [: ['] barks-at-end start drop ;] in-place  [: doing-busy? 0= ;] 600 soon -> -1 }T
testing the lever pulled with Hewie on the walkway: he walks to his spot on it first, then it turns
wt2f-1-flag3 story-flag-clear  0 by-exit  60 run-frames  [: hewie 200 0 place-on-tri ;] in-place  10 run-frames
T{ lever-area -90 at  circle press  [: lever-done ;] 900 soon  wt2f-1-flag3 story-flag?  [: self body-tri ;] his -> -1 -1 $3B6 }T
testing leaving with Hewie on the walkway's floor: he's left by the landing it reaches
lever-area -90 at  circle press  [: lever-done ;] 900 soon drop  [: hewie 200 0 place-on-tri ;] in-place  10 run-frames
T{ 0 0 at  [: room-id water-tower-2f-1 <> ;] 300 soon  [: self body-tri ;] his -> -1 874 }T
game:hewie send part-from-fiona  60 run-frames
testing Riccardo: heard prowling once she leaves the landing; stopped at the top; come for her by her way up
wt2f-1-flag2 story-flag-set  wt2f-1-flag1 story-flag-clear  c7-second-event story-flag-clear
4 story.base:partner-load  5 run-frames  0 by-exit
landing-area 5e across
T{ [: ['] prowls running?  prowl-begun @ ;] in-place -> -1 -1 }T
T{ top-area 5e across  [: ['] prowls running? ;] in-place -> 0 }T
T{ way-up-area 5e across  [: [: stalker-in? ;] in-place ;] 120 soon  wt2f-1-flag1 story-flag? -> -1 -1 }T
testing ... gone, back 1800 frames after (by the landing the walkway reaches)
[: stalker let-go  1 summon-take ;] in-place  60 run-frames
T{ [: return-due @  return-wait @ 1700 1800 within ;] in-place -> -1 -1 }T
[: 1 return-wait ! ;] in-place
T{ [: [: stalker-in? ;] in-place ;] 120 soon  [: stalker body-tri ;] in-place -> -1 473 }T
testing Hewie's side of the room: 1 in area , 0 out of it
[: stalker let-go  1 summon-take ;] in-place  30 run-frames  wt2f-1-flag2 story-flag-clear  0 by-exit  game:hewie send join-fiona  60 run-frames
T{ hewie-far-area 5e hewie-across  [: his-way @ ;] his -> 1 }T
T{ hewie-far-area -5e hewie-across  [: his-way @ ;] his -> 0 }T
test-summary
