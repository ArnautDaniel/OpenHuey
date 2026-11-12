\ The Fairy Earrings ($89, the equipment's slot 1): out of sight after 60 frames still (+0x1AD630,
\ fiona-unseen). Still: free and standing (Fiona_Update's Fiona_Panic), or held by a script while
\ crouched in a hiding place (Fiona_Think's Fiona_AreaFade: state flag $A, fiona-half-hidden) -
\ held otherwise she is seen. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_camouflage.fs
IN: test-camouflage
USING: tester engine game-state actors messages room-names game debug facts flag-names fiona.state ;

: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: unseen? ( -- flag )  [: her-unseen @ ;] hers 0<> ;
: wear ( id -- )  progress pr.equipped 4 + l! ;
: held ( on -- )  game:fiona send scripted  deliver ;

free-play  castle-1f-1 room!  20 run-frames  0 game:fiona send costume
full-suite   \ (on: waits, scenes, walks - the full run's)
testing free and standing, earrings on: unseen after 60 frames; without them, seen
0 wear  70 run-frames
T{ unseen? -> 0 }T
$89 wear  70 run-frames
T{ unseen? -> -1 }T
testing held by a script, standing: seen (Fiona_AreaFade: not crouched)
-1 held  10 run-frames
T{ unseen? -> 0 }T
70 run-frames
T{ unseen? -> 0 }T
testing held, crouched in a hiding place: unseen after 60 frames
fiona-half-hidden state-flag-set  70 run-frames
T{ unseen? -> -1 }T
fiona-half-hidden state-flag-clear  2 run-frames
T{ unseen? -> 0 }T
0 held  0 wear

test-summary
