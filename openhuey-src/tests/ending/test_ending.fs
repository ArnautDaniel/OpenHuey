\ The ending (ending.fs): the play record kept, the dog level and the TYPE it earns, what it
\ unlocks, and the story's end playing through the staff roll to the results. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/ending/test_ending.fs
IN: test-ending
USING: tester engine game-state actors messages room-names game debug keys facts flag-names story-names ending ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: in-ending ( xt -- )  game:ending enter  execute  leave-actor ;
: stage? ( -- n )  game:ending enter  ending:stage @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: record! ( trust mistreated pleased downs -- )
    progress pr.hewie-downs l!  progress pr.hewie-pleased l!  progress pr.hewie-mistreated l!
    [: ending:trust ! ;] in-ending ;
: level ( -- n )  0 [: drop ending:dog-level ;] in-ending ;
: earned ( -- n )  0 [: drop ending:dog-level ending:dog !  ending:type-earned ;] in-ending ;

free-play  10 frames  front-garden-2 room!  10 frames
testing the time played goes on in play, not while paused
T{ progress pr.play-frames l@  10 frames  progress pr.play-frames l@ swap - -> 10 }T
T{ 1 pause!  progress pr.play-frames l@  10 frames  progress pr.play-frames l@ swap -  0 pause! -> 0 }T

testing the dog level: his trust, less mistreated in 20s, more pleased in 10s (10 or more A, under 7 E)
T{ 9 0 0 0 record!  level -> 0 }T            \ (9 + 1 = 10: A)
T{ 8 0 0 0 record!  level -> 1 }T            \ (B)
T{ 6 0 0 0 record!  level -> 3 }T            \ (D)
T{ 5 0 0 0 record!  level -> 4 }T            \ (E)
T{ 9 40 0 0 record!  level -> 2 }T           \ (10 - 2: C)
T{ 6 0 50 0 record!  level -> 0 }T           \ (6 + 1 + 3 at most: A)

testing the TYPE: the first that applies
$2D progress pr.vars + 0 swap c!
T{ 9 100 0 0 record!  earned -> ending:dog-beater }T
T{ 9 0 0 0 record!  earned -> ending:top-breeder }T                   \ (A, no injuries)
T{ 9 0 0 1 record!  progress pr.play-frames l@  0 progress pr.play-frames l!  earned  swap progress pr.play-frames l! -> ending:track-star }T
T{ 9 0 0 1 record!  4 3600 * 30 * progress pr.play-frames l!  earned -> ending:dog-lover }T
T{ 5 0 0 1 record!  40 progress pr.panics l!  earned -> ending:scream-queen }T
T{ 5 0 0 1 record!  0 progress pr.panics l!  30 progress pr.knockouts l!  earned -> ending:warmonger }T
T{ 5 0 0 1 record!  0 progress pr.knockouts l!  earned -> ending:speed-walker }T
T{ 5 0 0 1 record!  25 3600 * 30 * progress pr.play-frames l!  earned -> ending:late-bloomer }T
$2D progress pr.vars + 2 swap c!
T{ 9 0 0 0 record!  earned -> ending:fugitive }T
testing the items: the story flags it counts
T{ 0 [: drop ending:#bonus-flags ending:items-found ;] in-ending -> 145 0 }T

testing the first clear unlocks the secret room (news 1), ending C its own; once only
T{ 0 [: drop 0 ending:kind !  ending:unlocks!  ending:news @ ;] in-ending  0 resident-flag?  3 resident-flag? -> 3 -1 -1 }T
T{ 0 [: drop ending:unlocks!  ending:news @ ;] in-ending -> 0 }T
T{ 5 resident-flag? -> -1 }T   \ (the TYPE's own: 5 + 0)

testing the story's end: the staff roll, Start skips it, its fade, then the results
$2D progress pr.vars + 0 swap c!  2 progress pr.hewie-trust l!
T{ stage? -> ending:playing }T
2 game:gameover send game-over-kind  2 frames  caught state-flag-set  3 frames
T{ stage? ending:staff-roll ending:staff-fade 1+ within  world-held state-flag? -> -1 -1 }T
: shows? ( -- flag )  stage? ending:staff-roll <> ;
: start-on ( -- flag )  start-button press  shows? ;
T{ ' start-on 600 soon -> -1 }T
: results? ( -- flag )  stage? ending:results = ;
T{ ' results? 60 soon -> -1 }T
testing the record shows; a button and the picture goes; then back to the title
: text-up? ( -- flag )  0 [: drop ending:text-a @ $80 = ;] in-ending ;
T{ ' text-up? 600 soon -> -1 }T
: on-and-on ( -- flag )  cross press  stage? ending:done = ;
T{ ' on-and-on 1200 soon -> -1 }T

test-summary
