\ The title's entries (opening.fs; src/game/scene_title.c SceneTitle_SeqMenu): a hard game's new
\ game (story flag $AF, what the stalkers' and Hewie's hard ways read), the extras room straight,
\ the options alone (the sub screen's mode 5: no turning to the other pages). Headless (the
\ title itself needs a window: its words are run here):
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_title.fs
IN: test-title
USING: tester engine game-state actors messages keys facts flag-names room-names game opening subscreen.state debug ;

: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: menu@ ( xt -- x )  game:subscreen enter  execute  leave-actor ;
20 run-frames

testing the entries: three, and quit; with the first clear three more (hard mode, extras, the mini-game)
T{ entries!  #items  3 entry  -> 4 e-quit }T
0 resident-flag-set
T{ entries!  #items  3 entry  4 entry  5 entry  6 entry -> 7 e-hard e-extras e-mini e-quit }T
testing no MODS entry without mods (the tests see none in mods/)
T{ mods-menu-count -> 0 }T

testing the options alone: the options page, no turning to the others; closed, the pages as before
5 game:subscreen send menu-mode  subscreen-wanted state-flag-set  deliver  20 run-frames
T{ [: page @  options-only @ ;] menu@ -> pg-options -1 }T
key: E tap
T{ [: page @ ;] menu@ -> pg-options }T
key: V tap  30 run-frames
T{ subscreen-wanted state-flag?  [: options-only @  page @ ;] menu@ -> 0 0 0 }T

testing a hard game's new game: hard mode set (the stalkers' hard ways), in the first room
-1 hard !  new-game  30 run-frames
T{ hard-mode?  progress pr.vars $26 + c@ -> -1 1 }T

testing the extras: straight into their room, in her clothes
extras  30 run-frames
T{ room-id off-map-37 =  progress pr.vars $26 + c@ -> -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
