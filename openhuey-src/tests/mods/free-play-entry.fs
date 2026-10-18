\ The title's hook (scripts/opening.fs title-menu-hook) and its sample mod
\ (examples/mods/free-play-entry): FREE PLAY among the title's entries, after the game's, before
\ QUIT; what picking it runs. Run with the mod on (CMakeLists.txt).
IN: test-free-play-entry
USING: tester engine game-state actors messages opening ;

testing the mod on, its entry added by the hook, before QUIT
T{ s" free-play-entry" mod-on? -> -1 }T
T{ entries!  #items  #items 1- entry  #items 2 - entry -> 5 e-quit e-mod }T
T{ #items 2 - item s" FREE PLAY" compare -> 0 }T
testing picking it runs the mod's word; the game's entries as before
T{ mod-picks @  ' free-play-entry:roam = -> -1 }T
T{ 0 entry 1 entry 2 entry -> e-new e-load e-options }T

test-summary
