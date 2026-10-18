\ Saving and loading (saves.fs): a quick save, things changed, the save loaded - the progress,
\ the room, Fiona's place and Hewie with her as they were. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/saves/test_saves.fs
IN: test-saves
USING: tester engine game-state actors messages room-names game facts story-names story.words fiona.state hewie.state stalker.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: in-saves ( xt -- )  game:saves enter  execute  leave-actor ;
: her-x ( -- n )  game:fiona body-pos fdrop fdrop fround f>s ;
: along? ( -- flag )  game:hewie body? ;
: trust ( -- n )  game:hewie enter  his-trust @  leave-actor ;
: skill ( k -- n )  game:hewie enter  cells his-skill + @  leave-actor ;
60 frames
front-garden-2 -1 game:rooms send go-to-room  5 frames
game:hewie send join-fiona  deliver  10 frames

testing a save written
plate-made story-flag-set
her-x constant saved-x
game:hewie enter  5 his-trust !  27 his-skill !  3 5 cells his-skill + !  leave-actor   \ (his skills at fetching)
game:hewie enter  2 his-mood !  400 his-mood-time !  -4 his-feel !  6 3 cells his-feel + !  leave-actor   \ (upset; his feelings)
' saves:quick-save in-saves  deliver  2 frames
T{ saves:save-path file-exists? -> -1 }T

testing things changed, then loaded back
plate-made story-flag-clear
game:hewie send part-from-fiona  deliver
: skills-reset ( -- )  game:hewie enter  6 0 do  16 i cells his-skill + !  loop  leave-actor ;  skills-reset
front-garden-3 -1 game:rooms send go-to-room  10 frames
T{ plate-made story-flag?  along?  room-id front-garden-2 = -> 0 0 0 }T
' saves:quick-load in-saves  deliver  20 frames
T{ room-id front-garden-2 = -> -1 }T
T{ plate-made story-flag? -> -1 }T
T{ her-x saved-x - abs 1 < -> -1 }T
T{ along?  trust -> -1 5 }T
T{ 0 skill  5 skill  1 skill -> 27 3 16 }T
T{ game:hewie enter  his-mood @  his-mood-time @ 300 >  his-feel @  3 cells his-feel + @  leave-actor -> 2 -1 -4 6 }T
testing the stalker kept: Debilitas in the game here, saved, let go, back where he was
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
[: 2 partner-load ;] game:story enter execute leave-actor  deliver  2 frames
front-garden-2 0 game:debilitas send stalker-in  deliver  [: 0 stalker:ordered ;] his  5 frames   \ (after her: kept in her room)
[: 7 hp !  12 hits !  300 temper ! ;] his
' saves:quick-save in-saves  deliver  2 frames
T{ saves:save-path file-exists? -> -1 }T
[: slot-unload ;] game:story enter execute leave-actor  deliver  5 frames
T{ [: in-game @ ;] his -> 0 }T
' saves:quick-load in-saves  deliver  20 frames
T{ [: in-game @ 0<>  hp @ ;] his  game:debilitas body-room -> -1 7 front-garden-2 }T
T{ [: hits @  temper @ ;] his -> 12 300 }T
test-summary
