\ Hewie's coat (Hewie_LoadFiles): the model of the set the story's 0xB7 gives (O_HEW / O_HEG /
\ O_HED, DogModel / DogModelA / DogModelB) and in it the n-th by how far the story has come - the
\ progress' flags the game over's music goes by too ($41 / $82 / $94 / $95: the latest set wins); a
\ game loaded: DogModel's set again. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_coat.fs
IN: test-coat
USING: tester engine game-state actors messages room-names game debug facts story-names hewie.state ;

: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: file ( -- addr len )  [: his-model @ ;] his model-file ;
: is? ( addr len -- flag )  file s= ;
: stage-off ( -- )  over-music-b story-flag-clear  over-music-c story-flag-clear  over-music-d story-flag-clear  over-music-e story-flag-clear ;

free-play  castle-1f-1 room!  20 run-frames  stage-off
testing at the start: HEW_000
T{ 0 game:hewie send hewie-costume  deliver  s" O_HEW/HEW_000" is? -> -1 }T
testing the story further on: the latest of its flags picks the coat
over-music-b story-flag-set  over-music-c story-flag-set
T{ 0 game:hewie send hewie-costume  deliver  s" O_HEW/HEW_002" is? -> -1 }T
testing the 0xB7's sets, at that coat
T{ 1 game:hewie send hewie-costume  deliver  s" O_HEG/HEG_002" is? -> -1 }T
T{ 2 game:hewie send hewie-costume  deliver  s" O_HED/HED_002" is? -> -1 }T
testing a game loaded: DogModel's set, the coat by the story
over-music-e story-flag-set
T{ game:hewie send game-loaded  deliver  s" O_HEW/HEW_004" is? -> -1 }T
stage-off  0 game:hewie send hewie-costume  deliver

test-summary
