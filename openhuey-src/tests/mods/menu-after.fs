\ After tests/mods/menu.fs: hga started again with the same mods folder - room-tally first and off
\ (mods/order.txt as the menu wrote it), the others on.
IN: test-mods-menu-after
USING: tester engine ;

testing the menu's order.txt in force
T{ s" room-tally" mod-on?  s" flower-scent" mod-on?  s" free-play-entry" mod-on? -> 0 -1 -1 }T
T{ 0 mods-menu-mod nip  swap drop  -> 0 }T
T{ 0 mods-menu-mod drop s" room-tally" compare -> 0 }T

test-summary
