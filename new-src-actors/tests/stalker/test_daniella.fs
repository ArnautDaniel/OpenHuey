\ Daniella (P4b): her own tables and numbers (her kit), the Pursuer's chase (Pursuer_ChaseDecision /
\ BehaviourStalk), her rage, no giving up, her temper with Hewie (docs/subsystems/stalker.md).
\ Headless:  build/new-src-actors/hga --test new-src-actors/tests/stalker/test_daniella.fs
IN: test-daniella
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.struck stalker.bitten fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: dan ( -- id )  s" daniella" actor-named ;
: hers ( xt -- x )  dan enter  execute  leave-actor ;
: fiona@ ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
1 seed !

free-play  front-garden-2 room!  20 frames
testing her kit: her tables, turn (4 degrees), reach for Hewie (12), the Pursuer's chase, no giving up
T{ [: kit @ daniella-kit =  base-chase @  gives-up @  turn-k f@ 100e f* f>s  reach-k f@ f>s ;] hers -> -1 -1 0 6 12 }T
T{ [: throw-moves  moves /move 11 * + @  combos @ ;] hers -> 11 12 $1702 1 }T
testing in her room, Fiona 30 ahead: the Pursuer's chase - walking at her ($200) for her row's 300 frames
room-id game:fiona body-tri dan send stalker-in  3 frames
[: 340e deg>rad body-turn ;] hers  1 frames
fvariable dx  fvariable dz  dan body-pos dz f! fdrop dx f!
340e deg>rad fsin 30e f* dx f@ f+  dan body-pos fdrop fswap fdrop  340e deg>rad fcos 30e f* dz f@ f+  160e tp-facing  3 frames
T{ [: step @ ['] pursue-run =  chase-act @  anim @  hold-t @ 290 > ;] hers -> -1 5 $200 -1 }T
testing ... within reach: a combo from her tables; Fiona struck
T{ [: [: chase-act @ $13 = ;] hers ;] 120 soon  [: [: her-mode @ 4 = ;] fiona@ ;] 60 soon -> -1 -1 }T
testing in a rage (0x31): her slow walk $205, her stance 3, her rage's row (walking 150 frames)
T{ [: -1 raging !  walk-slow  stand-anim  0 pursue-row cell+ @ ;] hers -> $205 3 150 }T
[: 0 raging ! ;] hers
testing struck past 20 damage: she doesn't give up (the Pursuer's GivesUp)
T{ [: 25 hits !  give-up? ;] hers -> 0 }T
testing her temper with Hewie: her own table (growled at 2, while she hides 3; a bite head on 5)
T{ [: 4 temper#  5 temper#  10 temper# ;] hers -> 2 3 5 }T
dan send stalker-out  3 frames
test-summary
