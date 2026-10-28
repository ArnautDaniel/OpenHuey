\ The story, S2: examining things (the action prepared, the action button, her action script),
\ the message window (docs/subsystems/story.md, window.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_examine.fs
IN: test-examine
USING: tester engine game-state actors messages keys room-names game debug flag-names fiona.state fiona.model facts ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: within ( n xt -- flag )   \ (within n frames xt says so)
    swap 0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;

free-play  10 run-frames
testing in front of the hole (area 0x18, facing -90): the place offers its remark (front-garden-3's place)
T{ $18 -90 tp-area-facing  3 run-frames  [: story.kit:offer-xt @ ;] in-room -> ' story.places.front-garden-3:hole }T
testing the action button: her action script - she walks to the spot (0x200), kneels (0x800), the window shows its message (10)
T{ circle press  [: [: her-doing @ ;] hers $12 = ;] 5 swap within -> -1 }T
T{ 120 [: [: story.kit:shown-msg @ ;] in-room 0< 0= ;] within -> -1 }T
T{ [: story.kit:shown-msg @ ;] in-room  shown  [: her-scripted @ her-reading @ fiona.model:anim@ ;] hers -> 10 10 -1 -1 $801 }T
T{ [: her-at sf@ her-at 8 + sf@ ;] hers  136.933e f- fabs 0.01e f<  -12.99e f- fabs 0.01e f< -> -1 -1 }T   \ (on the script's spot)
testing while it is up she doesn't move
create was 12 allot  game:fiona body-pos was vec!
T{ key: W true key-hold  20 run-frames  key: W false key-hold  was game:fiona body-at vec-dist 0.01e f< -> -1 }T
testing the button closes it; she gets up (0x802), her script goes on to its end and lets her go
T{ cross press  5 run-frames  [: story.kit:shown-msg @ ;] in-room  shown  [: fiona.model:anim@ ;] hers -> -1 -1 $802 }T
T{ 120 [: [: her-scripted @ ;] hers 0= ;] within  [: her-reading @ ;] hers -> -1 0 }T

\ ---- what every place does with her messages (the kit's) ----
front-garden-2 -1 rooms send go-to-room  30 run-frames
testing a locked door's script: the place shows "locked from the other side" for her
T{ $8001 broadcast fiona-message  deliver  2 run-frames  shown  [: story.kit:shown-msg @ ;] in-room -> $8001 $8001 }T
T{ cross press  5 run-frames  [: story.kit:shown-msg @ ;] in-room  shown -> -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

test-summary
