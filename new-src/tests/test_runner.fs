\ the event runner: entering a room runs its scripts as the original does, and frames run the
\ phases and the action scripts' slots
IN: test-runner
USING: tester game-state events.core events.words events.runner ;

\ the game's part, recorded: where characters were put
variable placed-cs  variable placed-exit
:noname ( cs exit -- )  placed-exit !  placed-cs ! ; is place-at-exit
:noname ( addr len -- )  2drop ; is stub-step
:noname ( addr len -- flag )  2drop false ; is stub-flag

\ Fiona (script id 0) and Hewie (1) in the scene
: in-scene ( slot id -- )  over character char.id l!  -1 swap character char.present l! ;
reset-characters  0 0 in-scene  1 1 in-scene

testing entering room 00 by its exit 0
-1 placed-cs !  -1 placed-exit !
$00 0 enter-room
T{ $00 progress pr.visited bit? -> true }T
T{ event-state ev.exit sl@ -> 0 }T
\ room 00's character-entering script puts Fiona, who came in by exit 0, at it (Hewie only
\ when he is the one controlled)
T{ placed-cs @ placed-exit @ -> 0 0 }T
T{ 0 character char.room sl@ -> 0 }T

testing frames
: frames ( n -- )  0 ?do  run-frame  loop ;
T{ 10 frames  event-state ev.room-frames sl@ -> 10 }T
T{ depth fdepth -> 0 0 }T

testing an action script in a scene slot runs a turn a frame and ends
variable ticks
: counting ( -- )  3 0 do  1 ticks +!  yield  loop ;
' counting $00 $7F action-script!
0 ticks !
0 $F1 $7F action
T{ 7 script-slot slot.task sl@ 0<> -> true }T
1 frames  T{ ticks @ -> 1 }T
5 frames  T{ ticks @ 7 script-slot slot.task sl@ -> 3 0 }T

testing self: the slot's id, its frame count, waits
variable seen
: watcher ( -- )  self-frames-reset  $F2 self-is? seen !  5 self-wait-frames  -1 ticks ! ;
' watcher $00 $7E action-script!
0 seen !  0 ticks !
0 $F2 $7E action
1 frames  T{ seen @ ticks @ -> true 0 }T
4 frames  T{ ticks @ -> 0 }T
1 frames  T{ ticks @ -> -1 }T

testing a character's action: it is scripted until its script ends
: releasing ( -- )  yield  self-idle-or-end ;
' releasing $00 $7D action-script!
0 1 $7D action
T{ 1 character char.scripted sl@ 0<> 2 script-slot slot.who sl@ -> true 1 }T
2 frames
T{ 1 character char.scripted sl@  2 script-slot slot.task sl@ -> 0 0 }T

testing goto: the slot goes on in another script, its frame count cleared
: landed ( -- )  self-frames sl@ seen !  -2 ticks ! ;
: jumper ( -- )  yield yield  ['] landed goto  99 ticks ! ;
' jumper $00 $7C action-script!
0 ticks !
0 $F3 $7C action
3 frames  T{ ticks @ seen @ -> -2 0 }T

testing flags and variables
T{ $123 story-flag?  $123 story-flag-set  $123 story-flag?  $123 story-flag-clear  $123 story-flag? -> false true false }T
T{ 5 7 var-set  5 var-inc  5 8 var? -> true }T
T{ 3 ebit-set  3 ebit?  4 ebit? -> true false }T
T{ $10 9 pvar-set  $10 9 pvar? -> true }T
T{ 100 chance?  0 chance? -> true false }T
