\ The extras room's galleries (scripts/subscreen/galleries.fs; src/game/subscreen.c): the room's
\ script opens each with its mode - the movies (8), the models ($B), the music ($D), the art ($E),
\ the TYPE ranks ($F). Their lists, locked and unlocked; the movies' choice into the story's
\ $2A / $2B; a model shown alone and put away; a track played and stopped; a picture unpacked; a
\ rank's message. In the extras room ($37). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/items/test_galleries.fs
IN: test-galleries
USING: tester engine game-state actors messages keys facts flag-names game debug subscreen.state subscreen.galleries subscreen.gallery-tables ;

: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: menu@ ( xt -- x )  game:subscreen enter  execute  leave-actor ;
: open-mode ( mode -- )  game:subscreen send menu-mode  subscreen-wanted state-flag-set  30 run-frames ;
: stage@ ( -- n )  [: stage @ ;] menu@ ;
: mode@ ( -- n )  [: mode @ ;] menu@ ;
: kind@ ( -- n )  [: gal-kind @ ;] menu@ ;
: at@ ( -- n )  [: gal-at @ ;] menu@ ;
: story-var@ ( n -- v )  progress pr.vars + c@ ;
: shut? ( -- flag )  60 run-frames  stage@ shut = ;
: lock-all ( -- )  96 0 do  i progress pr.resident bit-clear  loop ;
: unlock-all ( -- )  96 0 do  i resident-flag-set  loop ;

game:free-play  $37 room!  60 run-frames

testing the tables from the executable
T{ #movies #tracks #pictures #types #models -> $6E 27 48 11 32 }T
T{ 0 movie-flags + c@  4 movie-flags + c@ -> 0 $16 }T

testing the movies: their cursor kept in $2B; locked, the cross buzzes; one seen chosen into $2A
lock-all  5 $2B progress pr.vars + c!
T{ 8 open-mode  stage@ kind@ at@ -> up gal-movies 5 }T
T{ key: Down tap  at@ -> 6 }T
T{ key: X tap  stage@ -> up }T   \ (not seen)
T{ key: Right tap  at@  key: Left tap  at@ -> 8 0 }T
T{ key: Up tap  at@ -> 7 }T
unlock-all
T{ key: X tap  $2A story-var@ $2B story-var@ -> 7 7 }T
T{ shut?  kind@ -> -1 0 }T
T{ 8 open-mode  at@  key: V tap  $2A story-var@ $2B story-var@ -> 7 $FF 7 }T
T{ shut? -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the models: a figure unlocked shown alone, its motions stepped; back to the list, the scene again
T{ $B open-mode  key: X tap  20 run-frames  [: gal-model @ ;] menu@ 0< 0= -> -1 }T
T{ [: gal-motion @ ;] menu@  key: E tap  [: gal-motion @ ;] menu@ -> 0 1 }T
T{ key: Q tap  key: Q tap  [: gal-motion @ ;] menu@ -> 12 }T   \ (round: its thirteen)
T{ key: V tap  40 run-frames  [: gal-model @ gal-step @ ;] menu@ -> -1 0 }T
T{ key: V tap  shut? -> -1 }T

testing the model list's unlocks (SubScreen_ExtraUnlocked)
lock-all
T{ 0 model-open?  3 model-open?  10 model-open? -> 0 0 0 }T
0 resident-flag-set  1 resident-flag-set
T{ 0 model-open?  3 model-open?  10 model-open?  15 model-open? -> -1 -1 0 0 }T
86 resident-flag-set  85 resident-flag-set
T{ 10 model-open?  15 model-open?  16 model-open?  8 model-open? -> -1 -1 0 0 }T
12 resident-flag-set
T{ 8 model-open? -> -1 }T
unlock-all

testing the music: a track played, the triangle stops it, then leaves
T{ $D open-mode  key: X tap  10 run-frames  music-playing?  [: gal-playing @ ;] menu@ -> -1 0 }T
T{ key: V tap  60 run-frames  music-playing?  [: gal-playing @ ;] menu@  stage@ -> 0 $FF up }T
T{ key: V tap  shut? -> -1 }T

testing the art: a picture unpacked (512 x 896), zoomed, back to the list
T{ 0 39 art-picture -> 896 -1 }T
T{ 30 39 art-picture -> 448 -1 }T
T{ $E open-mode  key: X tap  20 run-frames  [: gal-step @ art-h @ ;] menu@ -> 2 896 }T
T{ key: F tap  [: art-zoom @ ;] menu@ -> -1 }T
T{ key: V tap  40 run-frames  [: gal-step @ ;] menu@ -> 0 }T
T{ key: V tap  shut? -> -1 }T

testing the TYPE ranks: their message in the box, then the one after; the second group
T{ $F open-mode  key: X tap  mode@ -> on-box }T
T{ [: shown @ ;] menu@ -> 4 }T
T{ key: X tap  [: shown @ ;] menu@ -> $17 }T
T{ key: X tap  [: mode @ ;] menu@ -> on-gallery }T
T{ key: Right tap  at@  key: Up tap  at@ -> 8 10 }T
T{ key: V tap  shut? -> -1 }T

test-summary
