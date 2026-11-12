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
T{ shown -> $800A }T   \ (the prompt's marker: the empty one - an offer of kind 0)
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

\ ---- the action prompt's markers (SceneGame_ActionPrompt): system messages in the window ----
: marker ( what kind -- )   \ (one frame's offer, shown if it changed)
    [: story.kit:show-marker ;] in-room  deliver deliver ;
: closed ( -- )  $FFFF game:window send close-text  deliver deliver ;   \ (as a button closes it: no frames - the room's own prompt would run)
: fresh ( -- )  [: story.kit:forget-marker  story.kit:markers-off  calm story.kit:danger-now !  calm story.kit:danger-was ! ;] in-room  deliver deliver ;
: up? ( -- msg )  [: story.kit:marker-up @ ;] in-room ;
testing an examining offered: "Huh? What's this?" ($800C); a doing of no kind of its own, the empty one ($800A); her own way out, $800B
fresh  T{ 1 story.kit:calm-only marker  shown up? -> $800C $800C }T
T{ 2 story.kit:unhurried marker  shown up? -> $800A $800A }T
T{ 3 story.kit:any-time marker  shown -> $800A }T
T{ 2 story.kit:way-kind marker  shown -> $800B }T
testing the same offer again isn't shown again (its marker closed by a button); another kind of it is
T{ closed  shown up? -> -1 0 }T
T{ 2 story.kit:way-kind marker  shown -> -1 }T
T{ 2 story.kit:calm-only marker  shown -> $800C }T
testing the extras room's corners: none; a hiding place: the empty one, calm - "Wait a second! What's that?!" chased, and again as the chase starts or ends
fresh  T{ 4 story.kit:quiet-any-time marker  shown -> -1 }T
T{ 5 story.kit:hiding-spot marker  shown -> $800A }T
T{ [: chased story.kit:danger-now ! ;] in-room  5 story.kit:hiding-spot marker  shown -> $800D }T
T{ [: chased story.kit:danger-was ! ;] in-room  5 story.kit:hiding-spot marker  shown -> $800D }T   \ (still chased: the same)
T{ closed  5 story.kit:hiding-spot marker  shown -> -1 }T
T{ [: calm story.kit:danger-now ! ;] in-room  5 story.kit:hiding-spot marker  shown -> $800A }T
testing hidden: only ours closed; the place's message taking the window closes ours, ours closes the place's
fresh  T{ 6 story.kit:calm-only marker  [: story.kit:markers-off ;] in-room  deliver deliver  shown up? -> -1 0 }T
T{ 7 story.kit:calm-only marker  $8001 broadcast fiona-message  deliver deliver  shown up? [: story.kit:shown-msg @ ;] in-room -> $8001 0 $8001 }T
T{ 8 story.kit:calm-only marker  shown up? [: story.kit:shown-msg @ ;] in-room -> $800C $800C -1 }T
fresh
testing each frame: nothing offered forgets the last one shown; while she is held the markers close and it's kept
T{ [: 0 story.kit:offer-xt !  0 story.kit:offer-msg !  -1 story.kit:could-now !  story.kit:prompt-frame  story.kit:marker-what @ ;] in-room -> -1 }T
T{ 9 story.kit:calm-only marker  [: story.kit:fiona-p story.kit:holder-of @  0 story.kit:fiona-p story.kit:holder-of !  story.kit:prompt-frame  story.kit:fiona-p story.kit:holder-of !  story.kit:marker-what @ ;] in-room  deliver deliver  shown -> 9 -1 }T
testing her own way: an exit's door area (fiona-could), straining shows none
T{ [: 0 story.kit:offer-xt !  0 story.kit:offer-msg !  3 story.kit:could-now !  story.kit:offered ;] in-room -> 3 story.kit:way-kind -1 }T
T{ [: 0 story.kit:offer-xt !  0 story.kit:offer-msg !  strain-could story.kit:could-now !  story.kit:offered ;] in-room -> 0 }T
T{ [: 0 story.kit:offer-xt !  $8016 story.kit:offer-msg !  3 story.kit:could-now !  story.kit:offered ;] in-room -> 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

test-summary
