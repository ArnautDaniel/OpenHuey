\ The stalkers' legs on the floor (src/game/humanlegs.c; Pursuer_ModelUpdate - the model's +0x40 by
\ his stance's points along his step, his Setup's +0x1694..+0x16A0 - and +0x4C): each kind's numbers
\ and leg bones, his body down at the lower of his feet on the castle's steps. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_legs.fs
IN: test-stalker-legs
USING: story.words tester engine game-state actors messages common room-names game debug facts flag-names stalker.state stalker ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: as ( kind -- )  partner-load  deliver  2 frames ;
: fits ( -- )   \ ( F: -- slow-back slow-front back front )
    feet-slow-back f@ feet-slow-front f@ feet-back f@ feet-front f@ ;
: fits? ( F: a b c d slow-back slow-front back front -- ) ( -- flag )   \ (the four alike)
    {: F: a b c d e g h k :}  a e f=  b g f= and  c h f= and  d k f= and ;

free-play  castle-1f-1 room!  20 frames
stalkers-blind state-flag-set
testing each kind's points along his step on a slope (+0x1694 / +0x169C slow, +0x1698 / +0x16A0 else)
T{ 2 as  [: fits ;] his  8e 1.5e 12e 1.5e fits? -> -1 }T
T{ 6 as  [: fits ;] his  8e 1.5e 12e 1.5e fits? -> -1 }T
T{ 7 as  [: fits ;] his  6e 1.5e 13e 0e fits? -> -1 }T
T{ 27 as  [: fits ;] his  6e 1.5e 13e 0e fits? -> -1 }T
testing his legs by his model: Lorenzo's other class's left from $12; Lorenzo's own model none
T{ s" O_DB0/DB0_000" leg-bones  s" O_LRY/LRY_000" leg-bones  s" O_LRO/LRO_000" leg-bones -> 2 6 2 $12 -1 -1 }T
testing on the steps, standing across them: his body below his place, at the lower of his feet
2 as  342 debilitas-tri  40 frames
: sunk ( F: -- d )  game:debilitas body-pos fdrop fswap fdrop  [: my-model @ ;] his legs-height f- ;
T{ sunk 0.2e f>  sunk 4e f< -> -1 -1 }T
testing on the level floor well back from their top, at his place
393 debilitas-tri  40 frames
T{ sunk fabs 0.05e f< -> -1 }T
test-summary
