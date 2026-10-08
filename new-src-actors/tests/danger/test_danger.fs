\ danger: calm, followed, chased (docs/subsystems/danger.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/danger/test_danger.fs
IN: test-danger
USING: tester engine game-state actors messages room-names facts game fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: flag ( n on -- )   \ (a state flag set by hand: the story isn't built yet)
    over 5 rshift 4 * progress pr.state + >r  swap 31 and 1 swap lshift
    swap if  r@ l@ or  else  invert r@ l@ and  then  r> l! ;
: level ( -- l )  game:danger enter  danger:level @  leave-actor ;
: hers ( -- l )  fiona enter  her-danger @  leave-actor ;
: next-door ( -- room )  room-id 0 room-exit-leads drop ;
: stalker-in ( room alert chasing -- )  game:danger send stalker-here ;   \ (as the stalkers will, each frame)

3 frames
game:story send story-stop  deliver  progress pr.state 8 0 fill   \ (no room scripts: this test sets the flags by hand)
testing a new game: no stalker, calm; Fiona told
T{ level hers -> 0 0 }T

testing the story's flags force it: 0x1F chased, 7 tense, 0x1B calm
T{ $1F true flag  2 frames  level hers -> 2 2 }T
T{ $1F false flag  7 true flag  2 frames  level -> 1 }T
T{ 7 false flag  2 frames  level -> 0 }T              \ (no stalker: calm again at once)
T{ $1F true flag  $1B true flag  2 frames  level -> 0 }T   \ (0x1B first)
$1F false flag  $1B false flag

testing the stalker in her room: chased
: with-him ( room alert n -- )  0 ?do  2dup 0 stalker-in  game-tick  loop  2drop ;
T{ room-id 0 2 with-him  level hers -> 2 2 }T
testing next door, after her (alert 3): chased while the hold lasts (30), then followed
T{ next-door 3 20 with-him  level -> 2 }T
T{ next-door 3 15 with-him  level -> 1 }T
testing gone: calm once the hold is over
T{ 3 frames  level -> 1 }T
T{ 500 frames  level -> 0 }T

testing flag 0x22 makes calm tense (hold 450)
T{ $22 true flag  2 frames  level -> 1 }T
$22 false flag
T{ 460 frames  level -> 0 }T

testing struck while calm: hunted next frame (the quirk) - tense with no stalker? (no: no stalker wins)
T{ 1 game:danger send danger-signal  3 frames  level -> 0 }T

test-summary
