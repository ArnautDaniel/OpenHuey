\ The faces' textures (Progress_LoadSpeech / Speak): a .TEX file's first texture shown in place of
\ the character's own, and its own again. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_speech.fs
IN: test-speech
USING: tester engine game-state actors messages common room-names game debug story.state story.words ;
: frames ( n -- )  0 ?do  game-tick  loop ;
free-play  front-garden-2 room!  10 frames
testing Fiona's face from O_FIO/FIO_201.TEX, then her own; Hewie's from HEW_201
T{ 0 cast-model 0 s" O_FIO/FIO_201.TEX" actor-texture-swap -> -1 }T
0 cast-model actor-texture-restore  2 frames
game:hewie send join-fiona  10 frames
T{ 1 cast-model 1 s" O_HEW/HEW_201.TEX" actor-texture-swap -> -1 }T
T{ 1 cast-model 1 s" O_DNL/DNL_201.TEX" actor-texture-swap -> -1 }T
1 cast-model actor-texture-restore  2 frames
test-summary
