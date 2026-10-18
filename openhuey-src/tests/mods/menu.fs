\ The mods menu (scripts/mods/menu.fs, the title's MODS): run with `--mods` naming a copy of the
\ sample mods (CMakeLists.txt). Its rows, the title's entry, a mod turned off and moved up, and
\ mods/order.txt written; menu-after.fs then starts hga again and finds that order in force.
IN: test-mods-menu
USING: tester engine game-state actors messages keys facts opening mods.menu ;

: tap ( key -- )  dup true key-hold  game-tick  false key-hold ;
: step ( key -- done? )  tap  mods.menu:keys  game-tick ;
game-tick game-tick

testing the folder's mods (the three samples), all on, by name; the title offers MODS
T{ mods-menu-count  0 mods-menu-mod nip nip  2 mods-menu-mod nip nip -> 3 -1 -1 }T
T{ 0 mods-menu-mod drop s" flower-scent" compare  2 mods-menu-mod drop s" room-tally" compare -> 0 0 }T
T{ entries!  #items 2 - entry -> e-mods }T
testing down to room-tally, off, moved up to the top: changed, and kept so in the menu
mods.menu:open
T{ key: Down step  key: Down step  key: X step  key: Left step  key: Left step  mods-menu-changed? -> 0 0 0 0 0 -1 }T
T{ 0 mods-menu-mod nip  swap drop  -> 0 }T
T{ 0 mods-menu-mod drop s" room-tally" compare -> 0 }T
testing back: mods/order.txt written, the menu done
T{ key: V step -> -1 }T
T{ s" room-tally" mod-on? -> -1 }T   \ (this run's as it started: the change applies next time)

test-summary
