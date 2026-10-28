\ Saving and loading (saves.fs): a quick save, things changed, the save loaded - the progress,
\ the room, Fiona's place and Hewie with her as they were. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/saves/test_saves.fs
IN: test-saves
USING: tester engine game-state actors messages room-names game facts flag-names story-names fiona.state hewie.state stalker.state debug story.base ;

: in-saves ( xt -- )  game:saves enter  execute  leave-actor ;
: her-x ( -- n )  game:fiona body-pos fdrop fdrop fround f>s ;
: along? ( -- flag )  game:hewie body? ;
: trust ( -- n )  game:hewie enter  his-trust @  leave-actor ;
: skill ( k -- n )  game:hewie enter  cells his-skill + @  leave-actor ;
60 run-frames
front-garden-2 -1 game:rooms send go-to-room  5 run-frames
game:hewie send join-fiona  deliver  10 run-frames

testing a save written
plate-made story-flag-set
her-x constant saved-x
game:hewie enter  5 his-trust !  27 his-skill !  3 5 cells his-skill + !  leave-actor   \ (his skills at fetching)
game:hewie enter  2 his-mood !  400 his-mood-time !  -4 his-feel !  6 3 cells his-feel + !  leave-actor   \ (upset; his feelings)
game:hewie enter  12 his-met !  77 3 cells his-met + !  leave-actor   \ (how often he leapt at Debilitas, Riccardo)
' saves:quick-save in-saves  deliver  2 run-frames
T{ saves:save-path file-exists? -> -1 }T

testing things changed, then loaded back
plate-made story-flag-clear
game:hewie send part-from-fiona  deliver
game:hewie enter  his-met 4 cells 0 fill  leave-actor
: skills-reset ( -- )  game:hewie enter  6 0 do  16 i cells his-skill + !  loop  leave-actor ;  skills-reset
front-garden-3 -1 game:rooms send go-to-room  10 run-frames
T{ plate-made story-flag?  along?  room-id front-garden-2 = -> 0 0 0 }T
' saves:quick-load in-saves  deliver  20 run-frames
T{ room-id front-garden-2 = -> -1 }T
T{ plate-made story-flag? -> -1 }T
T{ her-x saved-x - abs 1 < -> -1 }T
T{ along?  trust -> -1 5 }T
T{ 0 skill  5 skill  1 skill -> 27 3 16 }T
T{ game:hewie enter  his-mood @  his-mood-time @ 300 >  his-feel @  3 cells his-feel + @  leave-actor -> 2 -1 -4 6 }T
T{ game:hewie enter  his-met @  3 cells his-met + @  leave-actor -> 12 77 }T
testing the stalker kept: Debilitas in the game here, saved, let go, back where he was
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
[: 2 partner-load ;] game:story enter execute leave-actor  deliver  2 run-frames
front-garden-2 0 game:debilitas send stalker-in  deliver  [: 0 stalker:ordered ;] his  5 run-frames   \ (after her: kept in her room)
[: 7 hp !  12 hits !  300 temper ! ;] his
' saves:quick-save in-saves  deliver  2 run-frames
T{ saves:save-path file-exists? -> -1 }T
[: slot-unload ;] game:story enter execute leave-actor  deliver  5 run-frames
T{ [: in-game @ ;] his -> 0 }T
' saves:quick-load in-saves  deliver  20 run-frames
T{ [: in-game @ 0<>  hp @ ;] his  game:debilitas body-room -> -1 7 front-garden-2 }T
T{ [: hits @  temper @ ;] his -> 12 300 }T

testing the summoner's record kept (its progress +0x764), not zeroed by the stalker loaded again
: summ ( xt -- x )  game:summoner enter  execute  leave-actor ;
[: 1234 summoner:away !  900 summoner:cooldown !  7 summoner:waited !  2 summoner:took-kind ! ;] summ
' saves:quick-save in-saves  deliver  2 run-frames
game:summoner send summoner-reset  deliver
' saves:quick-load in-saves  deliver  20 run-frames
T{ [: summoner:away @  summoner:took-kind @  summoner:cooldown @ 850 > ;] summ -> 1234 2 -1 }T

testing a save made with him held off in another room: the summoner takes him on loading
front-garden-3 0 game:debilitas send stalker-in  deliver  2 run-frames
[: held-off my-mode ! ;] his
' saves:quick-save in-saves  deliver  2 run-frames
' saves:quick-load in-saves  deliver  25 run-frames
T{ [: in-game @ ;] his  [: summoner:took-kind @ ;] summ -> 0 0 }T

testing the doors' held bits left out of a save, and cleared from one loaded
: door5 ( -- addr )  5 4 * progress pr.doors + ;
T{ $B 5 saves:door-words drop + saves:unheld  $B 4 saves:unheld -> $A $B }T
door5 l@ 1 or door5 l!
' saves:quick-save in-saves  deliver  2 run-frames
door5 l@ 1 or door5 l!
' saves:quick-load in-saves  deliver  20 run-frames
T{ door5 l@ 1 and -> 0 }T

testing the stage music the story has come to, made again on loading (SceneGame_LoadedStart)
: stage ( -- s )  game:music enter  music:sm-stage @  leave-actor ;
opening-done story-flag-set  over-music-b story-flag-set
' saves:quick-save in-saves  deliver  2 run-frames
-1 game:music send music-stage  deliver
' saves:quick-load in-saves  deliver  20 run-frames
T{ stage -> 1 }T
over-music-d story-flag-set  game:music send game-loaded  deliver   \ (the latest stage's flag wins)
T{ stage -> 3 }T

testing the options kept in the system data: told as they change, written, read back, handed back
: sub ( xt -- x )  game:subscreen enter  execute  leave-actor ;
[: 64 subscreen.state:volume !  0 subscreen.state:vibration !  subscreen.options:kept ;] sub  deliver
T{ saves:options 2 cells + @  saves:options @  saves:options? @ -> 64 0 -1 }T
T{ saves:unlocks-changed? -> -1 }T
s" hga-test-system.fs" saves:in-user-dir to-file  saves:system-lines  end-file
saves:options 6 cells 0 fill  0 saves:options? !
s" hga-test-system.fs" saves:in-user-dir included
T{ saves:options 2 cells + @  saves:options? @ -> 64 -1 }T
[: 128 subscreen.state:volume !  1 subscreen.state:vibration ! ;] sub
saves:options-out  deliver
T{ [: subscreen.state:volume @  subscreen.state:vibration @ ;] sub -> 64 0 }T
[: subscreen.options:defaults ;] sub
test-summary
