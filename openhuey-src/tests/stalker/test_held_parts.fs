\ What the stalkers' models hold (docs/subsystems/stalker.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_held_parts.fs
IN: test-held-parts
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.struck stalker.bitten fiona.state ;

free-play  front-garden-2 room!  20 run-frames
testing what the models hold (Pursuer_Setup / FilesLoaded, Character_Enable): Daniella's kinds one of parts 17 and 31, her right hand's shape with it; Kind23Model's no gun; Riccardo his
: part-on? ( model part -- flag )  swap actor act.parts-shown swap 1+ dup 5 rshift 4 * rot + l@ swap 31 and rshift 1 and 0<> ;
: in-room ( name len -- id )  actor-named {: id :}  s" fiona" actor-named body-tri room-id swap id send stalker-in  3 run-frames  id ;
: model-of ( id -- m )  enter  my-model @  leave-actor ;
T{ s" daniella" in-room model-of  dup 17 part-on?  swap 31 part-on? -> -1 0 }T
T{ s" kind35" in-room model-of  dup 17 part-on?  swap 31 part-on? -> -1 0 }T
T{ s" kind34" in-room model-of  dup 31 part-on?  swap 17 part-on? -> -1 0 }T
T{ s" kind36" in-room model-of  dup 31 part-on?  swap 17 part-on? -> -1 0 }T
T{ s" tintstalker" in-room model-of  15 part-on? -> 0 }T
T{ s" kind37" in-room model-of  15 part-on? -> 0 }T
T{ s" riccardo" in-room model-of  17 part-on? -> -1 }T
: out ( name len -- )  actor-named send stalker-out  2 run-frames ;
s" daniella" out  s" kind35" out  s" kind34" out  s" kind36" out  s" tintstalker" out  s" kind37" out  s" riccardo" out

full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
