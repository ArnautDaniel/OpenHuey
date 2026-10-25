\ doors: locks, holding, letting go, noise (docs/subsystems/doors.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/doors/test_doors.fs
IN: test-doors
USING: tester engine game-state actors messages room-names game debug ;

\ ---- a user of doors, and a listener ----
state: user-state  cell field dummy  end-state
variable answer   \ 1 held, 0 refused
variable got  variable got-loud
behaviour using
  on door-held ( room exit -- )  2drop 1 answer ! ;
  on door-refused ( room exit -- )  2drop 0 answer ! ;
  on heard ( loud room tri door source -- )  got !  2drop drop got-loud ! ;
  on heard-nothing ( -- ) ;
end-behaviour
using user-state s" user" spawn constant user
front-garden-2 -1 rooms send go-to-room  2 run-frames
: as-user ( -- )  user enter ;

\ front-garden-2's exit 0 is a real door (to its neighbour), exit 2 a doorway
: door0 ( -- d )  front-garden-2 0 room-exit-door ;
: next0 ( -- room )  front-garden-2 0 room-exit-leads drop ;
: back0 ( -- exit )  front-garden-2 0 room-exit-leads nip ;     \ (the same door from the other side)
: state@ ( d -- w )  4 * progress pr.doors + l@ ;
: clean ( d -- )  4 * progress pr.doors + 0 swap l! ;
door0 clean
: hold ( room exit kind -- answer )  -1 answer !  as-user doors send hold-door leave-actor  deliver  answer @ ;

testing locking
T{ door0 doors send lock deliver  door0 state@ 8 and 0<>  door0 door-open? -> -1 0 }T
T{ front-garden-2 0 fiona-kind hold -> 1 }T        \ (quirk: a locked door can be held...)
T{ as-user front-garden-2 0 fiona-noise doors send let-go-open leave-actor deliver  door0 door-open?  door0 state@ 1 and -> 0 1 }T
                                                    \ (...but not opened, and it stays held)
T{ door0 doors send unlock deliver  door0 state@ 8 and -> 0 }T
door0 clean

testing holding: once, by one
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ front-garden-2 0 hewie-kind hold -> 0 }T                       \ already held
testing letting go open, then shut
T{ as-user front-garden-2 0 fiona-noise doors send let-go-open leave-actor deliver  door0 door-open?  door0 state@ 1 and -> -1 0 }T
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ as-user front-garden-2 0 fiona-noise doors send let-go-shut leave-actor deliver  door0 door-open? -> 0 }T
testing letting go of a door nobody holds changes nothing
T{ as-user front-garden-2 0 fiona-noise doors send let-go-open leave-actor deliver  door0 door-open? -> 0 }T

testing locked against one kind
T{ door0 stalker-kind true doors send lock-for deliver  front-garden-2 0 stalker-kind hold -> 0 }T
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ as-user front-garden-2 0 fiona-noise doors send let-go-shut leave-actor deliver -> }T
T{ door0 stalker-kind false doors send lock-for deliver  front-garden-2 0 stalker-kind hold -> 1 }T
T{ as-user front-garden-2 0 stalker-noise doors send let-go-shut leave-actor deliver -> }T

testing a doorway can't be held (always open)
T{ front-garden-2 2 fiona-kind hold -> 0 }T

testing a door used off-screen is heard (0xF at the door, from the user's source)
\ (the listener in the neighbour room, the door used from that side)
: listen-in ( room -- )   \ the user listens, standing on that room's first triangle (not played: no distance)
    as-user  0 0e 0e 0e body-place  0 stalker-noise s" acoustics" actor-named send listen  leave-actor  deliver ;
next0 listen-in
T{ next0 back0 hewie-kind hold -> 1 }T
T{ -1 got !  as-user next0 back0 hewie-noise doors send let-go-open leave-actor  actors-frame  got @ got-loud @ -> hewie-noise $F }T
T{ next0 back0 hewie-kind hold -> 1 }T
T{ as-user next0 back0 hewie-noise doors send let-go-shut leave-actor  actors-frame -> }T

testing in the played room a held door swung open settles open (and makes its creak's noise)
door0 clean
: listen-here ( -- )   \ the user listens at the door, in the played room
    as-user  0 exit-stand drop  room-id 0 body-place  leave-actor ;
listen-here
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ as-user 0 true fiona-noise doors send swing-door leave-actor  30 run-frames  door0 door-open? -> -1 }T
testing slammed shut: the latch is loud (0x5F)
T{ front-garden-2 0 fiona-kind hold -> 1 }T
: slammed ( -- source loud )
    -1 got !  0 got-loud !  as-user 0 fiona-noise doors send slam leave-actor
    12 0 do  game-tick  got @ 0< 0= if  leave  then  loop  got @ got-loud @ ;
T{ slammed -> fiona-noise $5F }T
T{ door0 door-open? -> 0 }T

testing a room's script sets it swinging open (Doors_SetOpened): 15 degrees a frame, its passage over
testing at once, a loud creak heard from Fiona's source, the door's state not opened
: side-tri ( side -- tri )   \ a nav triangle of only that side of the door
    {: side :}  nav-tris 0 ?do  0 side i door-on-side?  0 1 side - i door-on-side? 0= and if  i unloop exit  then  loop  -1 ;
: passage? ( side -- flag )  side-tri nav-flags $60000 and 0<> ;
T{ 0 passage?  1 passage? -> -1 0 }T
T{ -1 got !  0 got-loud !  as-user 0 true doors send set-door-opened leave-actor deliver -> }T
T{ 0 passage?  1 passage? -> 0 -1 }T
: heard ( n -- source loud )   \ (within n frames)
    0 ?do  game-tick  got @ 0< 0= if  leave  then  loop  got @ got-loud @ ;
T{ game-tick  0 door-angle f>s -> -15 }T
T{ 4 heard -> fiona-noise $5F }T
T{ 6 run-frames  0 door-angle f>s  door0 door-open? -> -90 0 }T
testing set swinging shut: the latch, loud, as it shuts; the passage back at once
T{ -1 got !  0 got-loud !  as-user 0 false doors send set-door-opened leave-actor deliver  0 passage?  1 passage? -> -1 0 }T
T{ 9 heard  0 door-angle f>s -> fiona-noise $5F 0 }T

testing a door with no nav triangle is never shown nor swung (Doors_TakeRoom +0x70: water-tower-7f-1's exit 0)
water-tower-7f-1 room
T{ 0 door-here? -> -1 }T
T{ 0 true false door-move  6 run-frames  0 door-angle f>s -> 0 }T
front-garden-2 room

testing routes between the halves of a room (room $40: two halves joined through other rooms)
create no-avoid 13 cells allot  no-avoid 13 cells 0 fill
: halves ( from-way want -- n )  >r >r $40 r> $40 r> stalker-kind -1 no-avoid route-sides ;
: first-way ( -- way )  0 route-door $40 door-exit-in  $40 swap 1 exit-way ;
T{ $40 0 1 exit-way  $40 2 1 exit-way -> 1 0 }T                  \ (its exits open onto either half)
T{ 0 0 halves  -1 1 halves  0 -1 halves -> 0 0 0 }T                \ the same half, or either: there
T{ 0 1 halves 0>  first-way -> -1 0 }T                             \ round to the other: out by its own half
T{ 1 0 halves 0>  first-way -> -1 1 }T

testing an open door blocks the lights past 78.75 degrees (Door_Draw's door_portal: its leaf's two
testing faces); behind it, away from a light, that light's shadows don't fall (Shadow_BlockerVolumes)
castle-1f-1 0 rooms send go-to-room  10 run-frames
0 -1 0e door-swing  2 run-frames
T{ light-blockers -> 0 }T
0 -1 -90e door-swing  2 run-frames
T{ light-blockers -> 2 }T
T{ 4 133.9e 4.5e -44.1e light-blocked? -> -1 }T      \ (light 4, through the leaf's middle and on)
T{ 4 110e 4.5e -44.1e light-blocked? -> 0 }T         \ (beside it)
T{ 4 126.5e 11e -47e light-blocked? -> 0 }T          \ (on the light's side of it)
0 -1 -70e door-swing  2 run-frames
T{ light-blockers -> 0 }T
testing a room script's blocker (0x66) lasts its frame
130e 30e -40e 120e 30e -40e 130e 10e -40e 120e 10e -40e light-blocker
T{ light-blockers -> 1 }T
T{ 1 run-frames  light-blockers -> 0 }T
0 -1 0e door-swing  2 run-frames
front-garden-2 -1 rooms send go-to-room  2 run-frames

test-summary
