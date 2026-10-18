\ Riccardo (P4c): his kit, his own chase (Riccardo_StateBehaviour / Riccardo_Chase), his flurry of
\ blows with his gun and his lunge, his attack by how near, his rage at the panic's stage 5
\ (docs/subsystems/stalker.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_riccardo.fs
IN: test-riccardo
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.riccardo fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: ric ( -- id )  s" riccardo" actor-named ;
: his ( xt -- x )  ric enter  execute  leave-actor ;
: fiona@ ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
fvariable dx  fvariable dz
\ Fiona `d` ahead of him (he faces 340 degrees), facing `deg`
: her-ahead ( deg -- ) ( F: d -- )
    ric body-pos dz f! fdrop dx f!
    fdup 340e deg>rad fsin f* dx f@ f+  ric body-pos fdrop fswap fdrop  frot 340e deg>rad fcos f* dz f@ f+
    s>f tp-facing ;
1 seed !

free-play  front-garden-2 room!  20 frames
testing his kit: his tables, his own chase and actions, his reach (20), his model and motions
T{ [: kit @ riccardo-kit =  own-chase @ ['] ric-begin =  own-acts @ riccardo-acts =  attack-range f@ f>s ;] his -> -1 -1 -1 20 }T
T{ [: hp @  my-kind @  my-model @ $1300 has-motion?  my-model @ $E00 has-motion? ;] his -> 100 4 -1 -1 }T
T{ [: throw-moves  0 wait#  4 wait#  waits 5 cells + f@ f>s ;] his -> 16 17 60 60 20 }T

testing his attack by how near: within 10 his grab (3); past 150 none
T{ [: 5e attack-for  200e attack-for ;] his -> 3 0 }T

testing in her room, she turned from him 30 off: he walks at her (60) or stalks her (150) by a roll
room-id game:fiona body-tri ric send stalker-in  3 frames
[: 340e deg>rad body-turn ;] his  1 frames
30e 340 her-ahead  3 frames
T{ [: [: sees-fiona @ ;] his ;] 60 soon -> -1 }T
[: ric-begin ;] his
T{ [: step @ ['] ric-run =  chase-act @ dup 5 = swap 6 = or  hold-t @ 59 > ;] his -> -1 -1 -1 }T
testing ... 60 off, facing him: he stands to her ($1, the comings-on from his waits)
60e 160 her-ahead  2 frames
[: ric-begin ;] his
T{ [: chase-act @  comings @ ;] his -> 1 60 }T

testing his flurry: 1..7 blows; on her floor a swing ($E00 / $E04)
10e 160 her-ahead  2 frames
[: flurry-start ;] his
T{ [: chase-act @  blows @ 1 8 within  anim @ dup $E00 = swap $E04 = or ;] his -> $1000 -1 -1 }T
testing ... a barrier by her (the story's zone of a kind with bit 1: Events_NearestPoint) - his shot strikes it instead, and it breaks ($210)
ric story.words:slot2 !   \ (the story's slot 2 holds him, as partner-load does: its zones see his point)
T{ [: $210 story.words:story-flag? ;] 200 soon  [: [: her-mode @ 4 = ;] fiona@ ;] 1 soon -> -1 0 }T
testing ... the barrier gone, at its key it strikes her: within 10 his grab (3) - she is seized ($10)
[: 0 blows !  flurry-start ;] his
T{ [: [: her-mode @ 4 = ;] fiona@ ;] 200 soon  [: her-sub @ ;] fiona@ -> -1 $10 }T
-1 story.words:slot2 !

testing his lunge ($1300), and his tables' own actions ($1000 the flurry, $1001 the lunge)
[: lunge-start ;] his 
T{ [: anim @  chase-act @ ;] his -> $1300 $1001 }T
T{ [: $1001 0 do-row  chase-act @ ;] his -> $1001 }T

testing his rage at the panic's stage 5, and out of it below
[: 5 her-stage !  0 reeling !  0 raging !  riccardo-frame ;] his
T{ [: raging @  4 her-stage !  riccardo-frame  raging @ ;] his -> -1 0 }T

testing the story's kind 4 is him
T{ 4 story.words:stalker-actor  ric = -> -1 }T
ric send stalker-out  3 frames

\ ---- his other class (kind 37): his chase with its own tables; struck enough, alerted ----
: k37 ( -- id )  s" kind37" actor-named ;
testing kind 37: its kit, his chase, near him at 30, his model
T{ k37 enter  kit @ kind37-kit =  own-chase @ ' ric-begin =  come-reach f>s  hp @  my-model @ $E00 has-motion?  leave-actor -> -1 -1 30 100 -1 }T
testing ... struck to 100 alerted (hurt): its alert tables and waits; calmer below
T{ k37 enter  120 hits !  0 cond !  kind37-frame  cond @  alt-tables @  0 wait#  leave-actor -> 1 -1 150 }T
T{ k37 enter  0 hits !  kind37-frame  cond @  alt-tables @  0 wait#  leave-actor -> 0 0 60 }T
T{ 37 story.words:stalker-actor  k37 = -> -1 }T
testing ... its tint (Kind37_Update): whole while its hold runs (150 frames), then pulsing in and out
: k37-pulse ( -- flag )   \ (its hold over: within 400 frames below half)
    0 tint-t !  false  400 0 do  kind37-frame  my-model @ actor act.alpha sf@ 0.5e f< if  drop true leave  then  loop ;
T{ k37 enter  3 my-mode !  -1 tint-was !  kind37-frame kind37-frame  tint-t @ 0>  my-model @ actor act.alpha sf@ 1e f=  k37-pulse  leave-actor -> -1 -1 -1 }T

\ ---- the TintStalker (kind 23): the Pursuer's chase, seen only a while after each turn ----
: ts ( -- id )  s" tintstalker" actor-named ;
testing the TintStalker: its kit, the Pursuer's chase, unseen till it does something
T{ ts enter  kit @ tintstalker-kit =  base-chase @  hp @  unseen @  leave-actor -> -1 -1 100 -1 }T
room-id game:fiona body-tri ts send stalker-in  3 frames
testing ... in her room: seen a while (150 frames), then gone
T{ ts enter  unseen @  tint-t @ 0>  my-model @ actor act.visible l@  leave-actor -> 0 -1 1 }T
testing ... its tint (TintStalker_Fade): shown at 112 of 128; its while over, fading out by 4 a frame, then gone
T{ ts enter  tint-alpha f@ f>s  my-model @ actor act.alpha sf@ 0.875e f- fabs 0.01e f<  leave-actor -> 112 -1 }T
T{ [: ts enter  tint-fade @ tint-out =  leave-actor ;] 200 soon  ts enter tint-alpha f@ f>s leave-actor  1 frames  ts enter tint-alpha f@ f>s leave-actor - -> -1 4 }T
T{ [: ts enter  unseen @  leave-actor ;] 200 soon -> -1 }T
T{ 23 story.words:stalker-actor  ts = -> -1 }T
ts send stalker-out  3 frames

testing his hanging parts (RiccardoModel_Springs): as drawn, his coat's pairs at their length ($A to $B 1.1), those drawn along their bones too ($26 to $27 0.67)
castle-1f-3 room!  20 frames   \ (a room whose camera has him in view: posed as drawn)
3 seed !   \ (where he wanders is by chance: one that keeps him in view)
room-id game:fiona body-tri ric send stalker-in  [: 3 my-mode ! ;] his  40 frames
create b1-at 12 allot  create b2-at 12 allot
: apart ( b1 b2 -- ) ( F: -- d )
    [: my-model @ ;] his swap bone-pos b2-at vec!  [: my-model @ ;] his swap bone-pos b1-at vec!  b1-at b2-at vec-dist ;
: ~ ( F: a b -- flag )  f- fabs 0.05e f< ;
T{ [: $A $B apart 1.1e ~  $26 $27 apart 0.67e ~ and ;] 10 soon -> -1 }T
testing his shot's debris (Riccardo_BlowFloorPoint): aimed down ($2301) - 10 out along his gun, on the floor about his feet's height
T{ [: $2301 play-now ;] his  [: [: floor-hit  hit-at 4 + sf@ my-at 4 + sf@ f- fabs 3e f< and ;] his ;] 10 soon -> -1 }T
ric send stalker-out  3 frames
test-summary
