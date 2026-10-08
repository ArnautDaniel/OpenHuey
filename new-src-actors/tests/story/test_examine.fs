\ The story, S2: examining things (the action prepared, the action button, her action script),
\ the message window, items, zones (docs/subsystems/story.md, window.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_examine.fs
IN: test-examine
USING: tester engine game-state actors messages keys room-names game debug fiona.state story.state story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: shown ( -- msg )  game:window enter  window:shown @  leave-actor ;

free-play  10 frames
testing in front of the hole (area 0x18, facing -90): an action prepared and offered (scene 5, action 0x0A)
T{ $18 -90 tp-area-facing  3 frames  [: prep-scene @ prep-arg @ ;] in-room -> 5 $A }T
testing the action button: her action script; she is scripted and the window shows its message (10)
T{ circle press  5 frames  [: msg @ ;] in-room  shown  [: her-scripted @ her-reading @ ;] hers -> 10 10 -1 -1 }T
testing while it is up she doesn't move
create was 12 allot  game:fiona body-pos was vec!
T{ key: W true key-hold  20 frames  key: W false key-hold  was game:fiona body-at vec-dist 0.01e f< -> -1 }T
testing the button closes it; her script goes on to its end and lets her go
T{ circle press  5 frames  [: msg @ ;] in-room  shown  [: her-scripted @ her-reading @ ;] hers -> -1 -1 0 0 }T

testing a message with a choice: the answer chosen comes back (answer?)
: show ( msg -- )  [: story.words:message ;] in-room  deliver ;
T{ [: 42 answer ! ;] in-room  10 show  2 frames  circle press  3 frames  [: answer @ ;] in-room -> -1 }T   \ (no choice: -1)

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
