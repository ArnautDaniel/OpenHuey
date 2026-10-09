\ The story, S2: examining things (the action prepared, the action button, her action script),
\ the message window, items, zones (docs/subsystems/story.md, window.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_examine.fs
IN: test-examine
USING: tester engine game-state actors messages keys room-names game debug fiona.state fiona.model story.state story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: within ( n xt -- flag )   \ (within n frames xt says so)
    swap 0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;

free-play  10 frames
testing in front of the hole (area 0x18, facing -90): an action prepared and offered (scene 5, action 0x0A)
T{ $18 -90 tp-area-facing  3 frames  [: prep-scene @ prep-arg @ ;] in-room -> 5 $A }T
testing the action button: her action script - she walks to the spot (0x200), kneels (0x800), the window shows its message (10)
T{ circle press  [: [: her-doing @ ;] hers $12 = ;] 5 swap within -> -1 }T
T{ 120 [: [: msg @ ;] in-room 0< 0= ;] within -> -1 }T
T{ [: msg @ ;] in-room  shown  [: her-scripted @ her-reading @ fiona.model:anim@ ;] hers -> 10 10 -1 -1 $801 }T
T{ [: her-at sf@ her-at 8 + sf@ ;] hers  136.933e f- fabs 0.01e f<  -12.99e f- fabs 0.01e f< -> -1 -1 }T   \ (on the script's spot)
testing while it is up she doesn't move
create was 12 allot  game:fiona body-pos was vec!
T{ key: W true key-hold  20 frames  key: W false key-hold  was game:fiona body-at vec-dist 0.01e f< -> -1 }T
testing the button closes it; she gets up (0x802), her script goes on to its end and lets her go
T{ cross press  5 frames  [: msg @ ;] in-room  shown  [: fiona.model:anim@ ;] hers -> -1 -1 $802 }T
T{ 120 [: [: her-scripted @ ;] hers 0= ;] within  [: her-reading @ ;] hers -> -1 0 }T

testing a message with a choice: the answer chosen comes back (answer?)
: show ( msg -- )  [: story.words:message ;] in-room  deliver ;
T{ [: 42 answer ! ;] in-room  10 show  2 frames  cross press  3 frames  [: answer @ ;] in-room -> -1 }T   \ (no choice: -1)

testing items: given with the pickup (a file), counted
T{ [: $205 item-give  $205 item-give ;] in-room  progress pr.files w@ -> $205 }T
T{ [: $30 2 item-give-count  $30 2 item-count?  $30 3 item-count? ;] in-room -> -1 0 }T

testing zones: a cylinder set this frame, her body against it
: zone-here ( -- )   \ zone 3 around her: radius 10, height 30
    [: 3 10 30 0  game:fiona body-pos  zone  0 3 3 char-zone-bits? ;] in-room ;
T{ zone-here -> -1 }T
T{ [: 0 3 3 char-zone-bits? ;] in-room -> -1 }T
T{ [: zones-off  0 3 3 char-zone-bits? ;] in-room -> 0 }T

test-summary
