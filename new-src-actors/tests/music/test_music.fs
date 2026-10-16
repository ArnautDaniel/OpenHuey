\ The stage music following the danger (scripts/music.fs; MusicDir_StateFrame): calm, the calm
\ parts; followed, they fade; chased, the chase; the panic's stage 4, the panic alone; after it,
\ back. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/music/test_music.fs
IN: test-music
USING: tester engine game-state actors messages facts flag-names room-names game debug music stalker.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: mus ( xt -- x )  game:music enter  execute  leave-actor ;
: vols ( -- p a b c )  [: 4 0 do  i cells sm-track + @  loop ;] mus ;
: state ( -- s )  [: sm-state @ ;] mus ;
: chase-vol ( -- v )  vols >r 2drop drop r> ;
: danger! ( flag -- )   \ (one of the danger's forcing flags; the others off)
    force-calm state-flag-clear  force-followed state-flag-clear  force-chased state-flag-clear
    state-flag-set ;
: stage! ( n -- )  s" panic" actor-named send panic-stage! ;

free-play  front-garden-2 room!  20 frames
0 game:music send music-stage  4 0 0 game:music send music-op  10 frames

testing calm: the calm parts full, the panic and the chase silent
force-calm danger!  20 frames
T{ vols -> 0 255 255 0 }T
testing followed: the calm parts fade out over 90 frames
force-followed danger!  5 frames
T{ state -> sm-fading }T
100 frames
T{ vols -> 0 0 0 0 }T
testing chased, no stalker in the game: the chase from its start, silent (MusicDir_ChaseStep: none)
force-chased danger!  5 frames
T{ state -> sm-chase-far }T
100 frames
T{ chase-vol -> 0 }T
testing ... the stalker by her and after her (he sees her): the chase loud (255); its step by how near he is (within 30: 210)
stalkers-blind state-flag-set  s" fiona" actor-named body-tri debug:debilitas-tri
120 frames
T{ state  chase-vol -> sm-chase-seen 255 }T
T{ [: chase-step cell+ @ ;] mus -> 210 }T
T{ [: seq3 f@ f>s  1 bend# f@ f>s ;] mus -> $88 127 }T      \ (stage 1's chase seen: its volume $88, channel 1 bent full)
testing a bend goes the whole range over its frames, up or down (MusicDir_BendTo: 127 / 90 a frame)
T{ [: 9 64 bend-now  9 90 127 bend-to  9 floats ch-bend-by + f@ 1000e f* f>s
      9 90 0 bend-to  9 floats ch-bend-by + f@ 1000e f* f>s ;] mus -> 1411 -1411 }T
testing the scripts' op 1 (the stage's channels by the story's flags) and op 3 (loaded at once)
1 0 0 game:music send music-op  3 0 0 game:music send music-op  2 frames
T{ state -> sm-chase-seen }T
debug:debilitas-out  stalkers-blind state-flag-clear  5 frames
testing the panic's stage 4: the panic alone, at once
4 stage!  5 frames
T{ state vols -> sm-panicking 255 0 0 0 }T
testing ... its stage 5: over; then calm again - the calm parts back over 90 frames
5 stage!  3 frames  2 stage!  force-calm danger!  100 frames
T{ state vols -> sm-quiet 0 255 255 0 }T
test-summary
