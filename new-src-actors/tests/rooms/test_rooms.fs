\ rooms: going from room to room (docs/subsystems/rooms.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/rooms/test_rooms.fs
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
: frames ( n -- )  0 ?do  actors-frame  loop ;
: seen@ ( i -- x )  seen @ nth ;
: forget  seen @ list-clear ;
2 frames

testing a new game starts in the cage room
T{ room-id -> front-garden-3 }T
T{ walker body-room -> front-garden-3 }T

testing going out by an exit: leaving, arrived, entered - in that order, with both exits
: out-by ( exit -- )  walker enter  rooms send go-through  leave  3 frames ;
front-garden-3 0 room-exit-leads constant arrive-exit constant next-room
forget  0 out-by
T{ room-id -> next-room }T
T{ seen @ length -> 9 }T
T{ 0 seen@ 1 seen@ 2 seen@ -> 1 front-garden-3 0 }T
T{ 3 seen@ 4 seen@ 5 seen@ -> 2 next-room arrive-exit }T
T{ 6 seen@ 7 seen@ 8 seen@ -> 3 next-room arrive-exit }T
testing the walker arrives by that exit, on the room's nav mesh
T{ walker body-room -> next-room }T
T{ walker body-tri 0< -> 0 }T

testing and back the way it came
forget  arrive-exit out-by
T{ room-id -> front-garden-3 }T
T{ 0 seen@ 1 seen@ 2 seen@ -> 1 next-room arrive-exit }T

testing an exit leading nowhere changes nothing
forget  7 out-by
T{ room-id  seen @ length -> front-garden-3 0 }T

testing a jump (no exit): no exit to arrive by
forget  front-garden-2 -1 rooms send go-to-room  3 frames
T{ room-id -> front-garden-2 }T
T{ 2 seen@ 5 seen@ -> -1 -1 }T
T{ walker body-room -> front-garden-2 }T

test-summary
