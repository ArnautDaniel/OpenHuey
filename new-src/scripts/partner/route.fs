\ partner/route.fs - from room to room off screen (src/game/doors.c RoutePlanner_*: a
\ breadth-first search through the doors), and the house's doors seen from any room (the
\ original's gRooms queries: Rooms_DoorExit / DoorLeadsTo / DoorTri, Progress_ExitOpen).
IN: partner.route
USING: engine game-state events.core ;

\ ---- a door from either side ----
variable r0  variable e0  variable t0  variable r1  variable e1  variable t1
: sides! ( d -- )  door-sides t1 ! e1 ! r1 ! t0 ! e0 ! r0 ! ;
\ Rooms_DoorExit: the exit of `room` door d is (-1: not there, or closed off)
: door-exit ( d room -- exit | -1 )
    over closed-off? if  2drop -1 exit  then
    swap sides!  dup r0 @ = if  drop e0 @ exit  then  r1 @ = if  e1 @  else  -1  then ;
\ Rooms_DoorLeadsTo: the room beyond door d from `room` (-1: none, or closed off)
: door-leads ( d room -- room' | -1 )
    over closed-off? if  2drop -1 exit  then
    swap sides!  dup r0 @ = if  drop r1 @ exit  then  r1 @ = if  r0 @  else  -1  then ;
\ Rooms_DoorTri: door d's triangle on `room`'s side (-1: none)
: door-tri ( d room -- tri | -1 )
    over closed-off? if  2drop -1 exit  then
    swap sides!  dup r0 @ = if  drop t0 @ exit  then  r1 @ = if  t1 @  else  -1  then ;
\ Progress_ExitOpen for any room's door: a doorway, or a door not locked with its open bit
: door-open? ( d -- flag )
    dup 0< if  drop false exit  then
    dup event-door-flags 1 and if  drop true exit  then
    dup door-locked if  drop false exit  then  2 door-bit? ;
\ Progress_ExitPassable (bits 4..7 of its state: the sides it is locked from; side 0 here)
: door-passable? ( d -- flag )  $10 door-bit? 0= ;

\ ---- RoutePlanner_FindRoute: the doors from room `from` to room `to`, avoiding the doors in
\ `avoid` (a bit a door, 13 words), unlocked and passable; up to 128 steps ----
128 constant steps
create st-door steps cells allot  create st-room steps cells allot
create st-from steps cells allot  create st-depth steps cells allot
create seen 13 cells allot
variable head  variable tail  variable avoid  variable rt-to
create route 64 cells allot  variable route-n     \ the route: its doors, first first
: bit@ ( n addr -- flag )  over 5 rshift cells + @  1 rot 31 and lshift and 0<> ;
: bit! ( n addr -- )  over 5 rshift cells +  swap 31 and 1 swap lshift  over @ or swap ! ;
\ RoutePlanner_QueueDoors: the doors out of `room` as steps from step `from`
: queue ( room from -- full? )
    8 0 do
        over i room-exit-door dup 0< if  drop  else
            dup seen bit@ if  drop  else
                dup avoid @ ?dup if  bit@  else  drop false  then  0=
                over door-locked 0= and  over door-passable? and if
                    tail @ steps >= if  drop 2drop true unloop exit  then
                    dup tail @ cells st-door + !
                    dup 3 pick door-leads tail @ cells st-room + !
                    over tail @ cells st-from + !
                    over 0< if  0  else  over cells st-depth + @ 1+  then  tail @ cells st-depth + !
                    1 tail +!
                then
                seen bit!
            then
        then
    loop  2drop false ;
\ the route of step k into `route`, first door first
: unwind ( k -- n )
    0 over begin  dup 0< 0= while  swap 1+ swap  cells st-from + @  repeat  drop   ( k n )
    dup route-n !
    swap over 1- swap                                                       ( n i k )
    begin  dup 0< 0= while
        dup cells st-door + @  2 pick cells route + !
        cells st-from + @  swap 1- swap
    repeat  2drop ;
: find-route ( from to avoid -- n | 0 here | -1 none )
    avoid !  rt-to !  0 route-n !
    dup rt-to @ = if  drop 0 exit  then
    seen 13 cells 0 fill  0 head !  0 tail !
    -1 queue if  -1 exit  then
    begin  head @ tail @ < while
        head @  1 head +!
        dup cells st-room + @ rt-to @ = if  unwind exit  then
        dup cells st-room + @ swap queue if  -1 exit  then
    repeat  -1 ;
