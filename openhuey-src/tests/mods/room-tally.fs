\ The sample mod (examples/mods/room-tally): found, its scripts loaded in their own vocabulary,
\ its actor hearing the game's broadcasts and answering by message. Not a test_*.fs: it runs
\ with the mod turned on (CMakeLists.txt):
\   build/openhuey-src/openhuey --mod openhuey-src/examples/mods/room-tally --test openhuey-src/tests/mods/room-tally.fs
IN: test-room-tally
USING: tester engine actors messages room-names game room-tally ;

\ ---- an asker: sends rooms-seen? and keeps the answer ----
state: asker-state  cell field answer  end-state
variable answered  -1 answered !
behaviour asking
  on rooms-seen ( n -- )  answered ! ;
end-behaviour
asking asker-state s" asker" spawn constant asker
: frames ( n -- )  0 ?do  actors-frame  loop ;
: tally ( -- n )  -1 answered !  asker enter  room-tally send rooms-seen?  leave-actor  2 frames  answered @ ;
2 frames

testing the mod is on and its actor is there
T{ s" room-tally" mod-on? -> -1 }T
T{ s" room-tally" actor-named 0< -> 0 }T
T{ s" no-such-mod" mod-on? -> 0 }T

testing the new game's room counted
T{ tally -> 1 }T

testing a room she hadn't been in counts; one she had doesn't
: out-by ( exit -- )  fiona enter  rooms send go-through  leave-actor  3 frames ;
front-garden-3 0 room-exit-leads constant arrive-exit drop
0 out-by
T{ tally -> 2 }T
arrive-exit out-by
T{ room-id tally -> front-garden-3 2 }T

test-summary
