\ The graphics menu (scripts/graphics.fs): F1 opens it. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/graphics/test_graphics.fs
IN: test-graphics
USING: tester engine game graphics ;

testing F1 opens the menu
key: F1 true key-hold  game-tick  key: F1 false key-hold  game-tick
T{ menu-open @ 0<> -> -1 }T

testing the placed objects' layers by name (PlacedObject_Draw): "g_" $26 marking the bloom mask
testing only, "a_" $19 see-through, others 1 (castle-2f-2's chandelier glow, $21's lattice)
: named ( addr u -- i )   \ (the placed object of that name; -1 none)
    {: a u :}  placed-count 0 ?do  i placed-info drop fdrop fdrop fdrop  a u compare 0= if  i unloop exit  then  loop  -1 ;
$20 room
T{ s" g_chandelie" named placed-draws -> $26 -1 }T
$21 room
T{ s" a_nozoki" named placed-draws -> $19 0 }T
T{ 0 placed-draws nip -> 0 }T
test-summary
