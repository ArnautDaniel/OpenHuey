\ The sample hook mod (examples/mods/flower-scent): the Fresh Flowers used where the game has no
\ use for them calm her and are kept; the game's own uses unchanged. Run with the mod on
\ (CMakeLists.txt):
\   build/openhuey-src/openhuey --mod openhuey-src/examples/mods/flower-scent --test openhuey-src/tests/mods/flower-scent.fs
IN: test-flower-scent
USING: tester engine game-state actors messages game items items.names fiona.state debug ;

: give ( id n -- )  game:items send give-item  deliver ;
variable used-result
state: menu-state  cell field dummy  end-state
behaviour stand-in-menu
  on item-used ( id result -- )  used-result !  drop ;
end-behaviour
stand-in-menu menu-state s" stand-in-menu" spawn constant menu
: use ( list i -- result )
    -9 used-result !  menu enter  game:items send use-item  leave-actor  deliver  used-result @ ;
: fear ( -- n )  game:fiona enter  her-fear f@ f>s  leave-actor ;
: fear! ( n -- )  game:fiona send fear-to  deliver ;
2 run-frames

testing the mod is on and its rule set
T{ s" flower-scent" mod-on? -> -1 }T
T{ action-of item-use-rule  ' flower-scent:scent = -> -1 }T

testing the Fresh Flowers away from their vase: her fear down 10, the flowers kept
fresh-flowers 1 give  50 fear!
T{ 0 0 item-at -> fresh-flowers }T
T{ 0 0 use  fear  0 0 item-at -> done-quietly 40 fresh-flowers }T

testing an item the mod leaves alone: the game's rule (a remedy still a remedy)
camomile 1 give
T{ 1 0 use  1 0 item-at -> used-up -1 }T

test-summary
