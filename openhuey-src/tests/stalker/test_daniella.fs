\ Daniella (P4b): her own tables and numbers (her kit), the Pursuer's chase (Pursuer_ChaseDecision /
\ BehaviourStalk), her rage, no giving up, her temper with Hewie (docs/subsystems/stalker.md).
\ Headless:  build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_daniella.fs
IN: test-daniella
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.struck stalker.bitten fiona.state ;

: dan ( -- id )  s" daniella" actor-named ;
: hers ( xt -- x )  dan enter  execute  leave-actor ;
: fiona@ ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
1 seed !

free-play  front-garden-2 room!  20 run-frames
testing her kit: her tables, turn (4 degrees), reach for Hewie (12), the Pursuer's chase, no giving up
T{ [: kit @ daniella-kit =  base-chase @  gives-up @  turn-k f@ 100e f* f>s  reach-k f@ f>s ;] hers -> -1 -1 0 6 12 }T
T{ [: throw-moves  moves /move 11 * + @  combos @ ;] hers -> 11 12 $1702 1 }T
testing in her room, Fiona 30 ahead: the Pursuer's chase - walking at her ($200) for her row's 300 frames
1 seed !  room-id game:fiona body-tri dan send stalker-in  3 run-frames   \ (the chase row by chance: walking - whatever Hewie rolled meanwhile)
[: 340e deg>rad body-turn ;] hers  1 run-frames
fvariable dx  fvariable dz  dan body-pos dz f! fdrop dx f!
340e deg>rad fsin 30e f* dx f@ f+  dan body-pos fdrop fswap fdrop  340e deg>rad fcos 30e f* dz f@ f+  160e tp-facing  3 run-frames
T{ [: step @ ['] pursue-run =  chase-act @  anim @  hold-t @ 290 > ;] hers -> -1 5 $200 -1 }T
testing ... within reach: a combo from her tables; Fiona struck
T{ [: [: chase-act @ $13 = ;] hers ;] 120 soon  [: [: her-mode @ 4 = ;] fiona@ ;] 60 soon -> -1 -1 }T
testing in a rage (0x31): her slow walk $205, her stance 3, her rage's row (walking 150 frames)
T{ [: -1 raging !  walk-slow  still-anim  0 pursue-row cell+ @ ;] hers -> $205 3 150 }T
[: 0 raging ! ;] hers
testing struck past 20 damage: she doesn't give up (the Pursuer's GivesUp)
T{ [: 25 hits !  give-up? ;] hers -> 0 }T
testing her temper with Hewie: her own table (growled at 2, while she hides 3; a bite head on 5)
T{ [: 4 temper#  5 temper#  10 temper# ;] hers -> 2 3 5 }T
dan send stalker-out  3 run-frames
testing her hanging parts (DaniellaModel_Springs): her hair down from her head ($B over $F), each point by the one beside it in the other strand; what hangs from her waist down ($22 over $24)
room-id game:fiona body-tri dan send stalker-in  [: 3 mode! ;] hers  40 run-frames
: y-of ( b -- ) ( F: -- y )  [: my-model @ ;] hers swap bone-pos fdrop fswap fdrop ;
create b-at 12 allot  create b2-at 12 allot
: bones-apart ( b1 b2 -- ) ( F: -- d )
    [: my-model @ ;] hers swap bone-pos b2-at vec!  [: my-model @ ;] hers swap bone-pos b-at vec!  b-at b2-at vec-dist ;
T{ $B y-of  $F y-of  f>  $22 y-of  $24 y-of  f>  $D $12 bones-apart  1.5e f< -> -1 -1 -1 }T
dan send stalker-out  3 run-frames
testing held off out of sight, no rooms of her own (Npc_HeadRandomRoom): on by a way out to another room, not hers
front-garden-3 0 dan send stalker-in  3 run-frames
T{ [: 4 mode!  front-garden-3 goal-room ! ;] hers  [: [: goal-room @ dup front-garden-2 <>  swap front-garden-3 <> and ;] hers ;] 120 soon -> -1 }T
dan send stalker-out  3 run-frames

testing her later kinds (34..36): each its own actor, kit and numbers; her look (34 and 36 the model DNL_001)
: later ( kind -- id )  case  34 of  s" kind34"  endof  35 of  s" kind35"  endof  >r s" kind36" r>  endcase  actor-named ;
: numbers ( -- kind hp tolerance limit )  my-kind @  hp-max @  tolerance @  dog-limit @ ;
T{ 34 later enter  kit @ kind34-kit =  numbers  my-model @ $E00 has-motion?  leave-actor -> -1 34 120 200 6000 -1 }T
T{ 35 later enter  kit @ kind35-kit =  numbers  leave-actor -> -1 35 65 120 3000 }T
T{ 36 later enter  kit @ kind36-kit =  numbers  leave-actor -> -1 36 65 120 3000 }T
T{ 34 later enter  stalker:file s" O_DNL/DNL_001" s=  leave-actor  35 later enter  stalker:file s" O_DNL/DNL_000" s=  leave-actor -> -1 -1 }T
testing ... the story's slot 2 holds her own for each (`stalker-actor`)
T{ 34 story.base:stalker-actor 34 later =  36 story.base:stalker-actor 36 later = -> -1 -1 }T
testing ... in her room (kind 34): the Pursuer's chase, as hers
room-id game:fiona body-tri 34 later send stalker-in  3 run-frames
T{ [: [: step @ ['] pursue-run = ;] 34 later enter execute leave-actor ;] 30 soon -> -1 }T
34 later send stalker-out  3 run-frames
test-summary
