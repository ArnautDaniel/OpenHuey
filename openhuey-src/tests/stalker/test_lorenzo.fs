\ Lorenzo (P4c): his kit and the Pursuer's chase with it, his slow turn, his reach (18)
\ (docs/subsystems/stalker.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_lorenzo.fs
IN: test-lorenzo
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.riccardo fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: lor ( -- id )  s" lorenzo" actor-named ;
: his ( xt -- x )  lor enter  execute  leave-actor ;
: fiona@ ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
1 seed !

free-play  front-garden-2 room!  20 frames
testing his kit: his tables, the Pursuer's chase, his turn (2 degrees), reach for Hewie (12), his reach (18)
T{ [: kit @ lorenzo-kit =  base-chase @  turn-k f@ 1000e f* f>s  reach-k f@ f>s  attack-range f@ f>s ;] his -> -1 -1 34 12 18 }T
T{ [: hp @  my-kind @  my-model @ $E00 has-motion?  moves @ ;] his -> 50 11 -1 $E00 }T
testing no temper with Hewie, no throws of him
T{ [: 4 cells temper-table + @  throw-moves ;] his -> 0 -1 -1 }T
testing in her room, Fiona 30 ahead: the Pursuer's chase - walking at her ($200) or stalking her
room-id game:fiona body-tri lor send stalker-in  3 frames
[: 340e deg>rad body-turn ;] his  1 frames
fvariable dx  fvariable dz  lor body-pos dz f! fdrop dx f!
340e deg>rad fsin 30e f* dx f@ f+  lor body-pos fdrop fswap fdrop  340e deg>rad fcos 30e f* dz f@ f+  160e tp-facing  3 frames
T{ [: step @ ['] pursue-run = ;] his -> -1 }T
testing ... within reach: his attack; Fiona struck
T{ [: [: her-mode @ 4 = ;] fiona@ ;] 600 soon -> -1 }T
testing the story's kind 11 is him
T{ 11 story.words:stalker-actor  lor = -> -1 }T
lor send stalker-out  3 frames

\ ---- kind 12: unkillable; in her room it seizes her ----
: k12 ( -- id )  s" kind12" actor-named ;
testing kind 12: its kit, the Pursuer's chase, 9999 health, its model
T{ k12 enter  kit @ kind12-kit =  base-chase @  hp-max @  my-model @ $E04 has-motion?  leave-actor -> -1 -1 9999 -1 }T
[: [: her-mode @ 0= ;] fiona@ ;] 300 soon drop
room-id game:fiona body-tri k12 send stalker-in  3 frames
testing ... in her room: it seizes her ($10), and is never hurt
T{ [: [: her-mode @ 4 =  her-sub @ $10 = and ;] fiona@ ;] 30 soon -> -1 }T
T{ k12 enter  100 hp !  kind12-frame  hp @  leave-actor -> 9999 }T
T{ 12 story.words:stalker-actor  k12 = -> -1 }T
testing ... its marker (Kind12_ShowUp: the Marker on it) while in her room; gone as it leaves
T{ k12 enter  mark-id @ dup 0< 0= swap alive? and  leave-actor -> -1 }T
k12 send stalker-out  3 frames
T{ k12 enter  kind12-marker  mark-id @  leave-actor -> -1 }T
test-summary
