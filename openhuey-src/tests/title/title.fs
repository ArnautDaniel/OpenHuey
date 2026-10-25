\ The title as a player meets it (scripts/opening.fs): run with --title (CMakeLists.txt), so it
\ comes up as with a window - no new game behind it, Fiona not there - and every actor's frame
\ goes on round it. Its frames raise no errors (each actor works with nobody else about: the
\ panic's measuring once read Fiona's room each frame, an error 30 times a second on every
\ title); its entries all on the screen; the menu, the load slots and back.
\   build/openhuey-src/openhuey --title --test openhuey-src/tests/title/title.fs
IN: test-title
USING: tester engine game-state actors keys game opening debug ;

: of-title ( xt -- x )  game:opening enter  execute  leave-actor ;
: on-title? ( -- flag )  game:opening alive? ;
: tap ( key -- )  dup true key-hold  1 run-frames  false key-hold  1 run-frames ;

testing the title up, nobody in the game
T{ on-title?  [: stage @ ;] of-title -> -1 on-menu }T
T{ s" fiona" actor-named body? -> 0 }T
testing its frames raise no errors
error-count constant before
120 run-frames
T{ error-count before - -> 0 }T
testing its entries: the game's first, QUIT last, each inside the screen
T{ 0 entry 1 entry 2 entry  #items 1- entry -> e-new e-load e-options e-quit }T
: lowest ( -- y )  4 pen-scale  3 pen-scale  #items 1- entry-y ;
T{ lowest  screen-size nip < -> -1 }T
testing the load slots and back, without errors
T{ key: Down tap  [: picked @ ;] of-title -> 1 }T
T{ key: X tap  [: stage @ ;] of-title -> on-slots }T
T{ key: V tap  [: stage @ ;] of-title -> on-menu }T
30 run-frames
T{ error-count before - -> 0 }T

test-summary
