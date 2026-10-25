\ rooms: going from room to room (docs/subsystems/rooms.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/rooms/test_rooms.fs
IN: test-rooms
USING: tester engine actors messages room-names game ;

\ ---- a witness: writes down the rooms' broadcasts in order ----
state: witness-state  cell field dummy  end-state
variable seen  list seen !
: saw ( kind room exit -- )  rot seen @ push  swap seen @ push  seen @ push ;
behaviour witnessing
  on spawned ( -- )  self subscribe leaving-room  self subscribe arrived  self subscribe entered-room ;
  on leaving-room ( room exit -- )  1 -rot saw ;
  on arrived ( room exit -- )  2 -rot saw ;
  on entered-room ( room exit -- )  3 -rot saw ;
end-behaviour
witnessing witness-state s" witness" spawn constant witness
: actor-frames ( n -- )  0 ?do  actors-frame  loop ;
: seen@ ( i -- x )  seen @ nth ;
: forget  seen @ list-clear ;
2 actor-frames

testing a new game starts in the cage room
T{ room-id -> front-garden-3 }T
T{ fiona body-room -> front-garden-3 }T

testing going out by an exit: leaving, arrived, entered - in that order, with both exits
: out-by ( exit -- )  fiona enter  rooms send go-through  leave-actor  3 actor-frames ;
front-garden-3 0 room-exit-leads constant arrive-exit constant next-room
forget  0 out-by
T{ room-id -> next-room }T
T{ seen @ length -> 9 }T
T{ 0 seen@ 1 seen@ 2 seen@ -> 1 front-garden-3 0 }T
T{ 3 seen@ 4 seen@ 5 seen@ -> 2 next-room arrive-exit }T
T{ 6 seen@ 7 seen@ 8 seen@ -> 3 next-room arrive-exit }T
testing Fiona arrives by that exit, on the room's nav mesh
T{ fiona body-room -> next-room }T
T{ fiona body-tri 0< -> 0 }T

testing and back the way it came
forget  arrive-exit out-by
T{ room-id -> front-garden-3 }T
T{ 0 seen@ 1 seen@ 2 seen@ -> 1 next-room arrive-exit }T

testing an exit leading nowhere changes nothing
forget  7 out-by
T{ room-id  seen @ length -> front-garden-3 0 }T

testing a jump (no exit): no exit to arrive by
forget  front-garden-2 -1 rooms send go-to-room  3 actor-frames
T{ room-id -> front-garden-2 }T
T{ 2 seen@ 5 seen@ -> -1 -1 }T
T{ fiona body-room -> front-garden-2 }T

testing the fog drawn early: by its parameters' +16 (Fog_SetParams reads 26 bytes - on past the
testing section, as the room in memory goes on, when its fog's 16 come last)
create fog-bytes 32 allot
: early? ( room -- flag )  room  room-look look.fog-early l@ 0<> ;
T{ $18 early?  $3 early?  $4 early? -> 0 -1 -1 }T
T{ $1D fog-bytes look@  fog-bytes 16 + l@ -> 26 2 }T      \ ($4's fog: the next section's first word)
T{ $1D fog-bytes 16 look-set  room-look look.fog-early l@ -> 0 }T   \ (16 given: the rest 0)

testing no lights section: no lights, the ambient 128 (Lights_Reset); off the nav mesh, light 0 alone
T{ $D room  room-ambient f>s f>s f>s  0e 0e 0e lights-at -> 128 128 128 -1 -1 -1 }T
T{ front-garden-3 room  0 light@ fdrop f0> -> -1 }T
T{ 0e 100000e 0e lights-at -> 0 -1 -1 }T

test-summary
