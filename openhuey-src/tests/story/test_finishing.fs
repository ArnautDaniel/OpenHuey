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

testing the doll away from Fiona: Debilitas coming upon it - after her he leaves it, searching he takes it
: doll-in ( room -- i )  3 swap 0  0e 0e 0e 0e 0e 0e  thing-new ;
: he-is-in ( room his-mode -- )   \ (Debilitas, kind 2, telling the things where he is: stalker-doing)
    {: room his-mode :}  room 0 0 0 100 100 2 his-mode -1  things-id send stalker-doing ;
castle-1f-2 doll-in constant doll-a
castle-1f-3 doll-in constant doll-b
T{ castle-1f-2 0 he-is-in  2 run-frames  doll-a thing-used? -> -1 }T
T{ castle-1f-3 2 he-is-in  2 run-frames  doll-b thing-used? -> 0 }T

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
test-summary
