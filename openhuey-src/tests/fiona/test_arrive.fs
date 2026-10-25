\ Put somewhere - through a door into another room, or by the story - a character's body is at its
\ feet's height at once, not eased from where it was (Character_Place's FeetUp / FeetReplace: the
\ engine's actor-settle): coming out of a door into a room on another level, Fiona's model isn't
\ seen rising through the floor or coming down from the ceiling. (Her model's root as last drawn,
\ against its height over her place standing.) Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_arrive.fs
IN: test-arrive
USING: tester engine game-state actors messages room-names game debug fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: model ( -- id )  game:fiona enter  her-model @  leave-actor ;
: root-over ( F: -- d )   \ (her model's root above her place)
    model 0 bone-pos fdrop fswap fdrop  s" fiona" actor-named body-pos fdrop fswap fdrop  f- ;
fvariable standing
: at-rest? ( -- flag )  root-over standing f@ f- fabs 1e f< ;

free-play  0 game:fiona send costume  deliver  front-garden-2 room!  30 frames
root-over standing f!
testing standing in the front garden (high up): her root over her place
T{ standing f@ 5e f>  standing f@ 15e f< -> -1 -1 }T
testing into the castle's hall (far below): at her place as soon as she is drawn there
castle-1f-1 room!  2 frames
T{ at-rest? -> -1 }T
testing ... and back up
front-garden-2 room!  2 frames
T{ at-rest? -> -1 }T
test-summary
