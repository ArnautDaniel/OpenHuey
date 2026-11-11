\ The last pieces of the nearly-done subsystems: an event's shot holding the camera (the summoner's
\ wait), the doll and the stalker, the pad's rumble, a model part hidden, the doorway fade. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_finishing.fs
IN: test-finishing
USING: tester engine game-state actors messages common room-names game debug story.base story.kit ;

free-play  castle-1f-1 room!  20 run-frames
testing an event's shot has the camera, and gives it back
T{ cam-event? -> 0 }T
T{ -1 20000 30000 0 0 s" camera" actor-named send event-shot -> }T
2 run-frames
T{ cam-event? -> -1 }T
T{ 0 0 0 0 0 s" camera" actor-named send event-shot -> }T
2 run-frames
T{ cam-event? -> 0 }T

testing a handler's error in the game: its actor lives on, the errors actor told (game.fs's `failures`, debug.fs)
message hello-fail ( -- )
behaviour failing  on hello-fail ( -- )  12345 send hello-fail ;  end-behaviour   \ (an error: no actor 12345)
state: failing-state  cell field failing-unused  end-state
: failures@ ( -- n )  game:failures enter  debug:failures @  leave-actor ;
failures@ constant failed-before
failing failing-state s" failing" spawn constant failer
T{ failer send hello-fail  deliver  failer alive?  failures@ failed-before - -> -1 1 }T
failer kill  deliver
testing the doll away from Fiona: Debilitas coming upon it - she his lead (+0x16C9 5) he leaves it, else he takes it
testing ... (Thing03_StalkerCheck: the things ask him, `drawn-to`, he answers `thing-taken`)
: doll-in ( room -- i )  3 swap 0  0e 0e 0e 0e 0e 0e  thing-new ;
flag-names:stalkers-blind facts:state-flag-set  game:fiona body-tri debug:debilitas-tri  5 run-frames   \ (blind: no jolts from him)
: lead! ( n -- )  game:debilitas enter  stalker.state:lead !  leave-actor ;
castle-1f-2 doll-in constant doll-a
castle-1f-3 doll-in constant doll-b
T{ 5 lead!  0 doll-a true game:debilitas send drawn-to  deliver  1 run-frames  doll-a thing-used? -> -1 }T
T{ 4 lead!  0 doll-b true game:debilitas send drawn-to  deliver  1 run-frames  doll-b thing-used? -> 0 }T
0 lead!  debug:debilitas-out  5 run-frames  doll-a things-id send thing-taken  deliver
flag-names:stalkers-blind facts:state-flag-clear  30 run-frames

testing the pad's rumble: the strongest ask is felt, each for its frames; the story's (0x71); a sequence step by step
T{ 1 $80 8 rumble-pad  1 run-frames  rumble-now -> 0 $80 }T
T{ 0 0 6 rumble-pad  1 run-frames  rumble-now -> -1 $80 }T
T{ 8 run-frames  rumble-now -> 0 0 }T
T{ rumble-sequence-clear  0 0 96 3 rumble-step  0 255 255 3 rumble-step  3 run-frames  rumble-now -> 0 96 }T
T{ 3 run-frames  rumble-now -> 0 255 }T
T{ 3 run-frames  rumble-now -> 0 0 }T

testing a model part hidden (the original's part flag 2): Fiona's part 10 (as castle-2f-3's place does)
: part-shown? ( model part -- flag )  1+ dup 5 rshift 4 *  rot actor act.parts-shown + l@  swap 31 and rshift 1 and 0<> ;
T{ 0 cast-model 10 part-shown? -> -1 }T
T{ 0 cast-model 10 false actor-part-show  0 cast-model 10 part-shown?  0 cast-model 9 part-shown? -> 0 -1 }T
T{ 0 cast-model 10 true actor-part-show  0 cast-model 10 part-shown? -> -1 }T

testing going out through a doorway (Character_RegionFade): none at the exit area's middle, half way to its outer spot about half
fvariable mx  fvariable my  fvariable mz  fvariable ox  fvariable oz  fvariable tx  fvariable tz
: her ( -- id )  s" fiona" actor-named ;
: put ( F: x z -- )  tz f!  tx f!  tx f@ f>cell  my f@ f>cell  tz f@ f>cell  0e f>cell  her send place ;
free-play  castle-1f-1 room!  20 run-frames
0 exit-area area-middle drop  mz f! my f! mx f!   0 0 exit-spot drop  oz f! fdrop ox f!
T{ 0 cast-model doorway-faded f0= -> -1 }T   \ (Fiona herself never fades in doorways - the others do: as one of them)
her true doorway-fades
T{ mx f@ mz f@ put  3 run-frames  0 cast-model doorway-faded  0.05e f< -> -1 }T
T{ mx f@ ox f@ f+ f2/  mz f@ oz f@ f+ f2/ put  3 run-frames  0 cast-model doorway-faded  fdup 0.2e f>  0.95e f< -> -1 -1 }T
T{ her false doorway-fades  0 cast-model doorway-faded f0= -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
