\ doors: locks, holding, letting go, noise (docs/subsystems/doors.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/doors/test_doors.fs
IN: test-doors
USING: tester engine game-state actors messages room-names game ;

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
: frames ( n -- )  0 ?do  game-tick  loop ;   \ (whole frames: the doors swing)
front-garden-2 -1 rooms send go-to-room  2 frames
: as-user ( -- )  user enter ;

\ front-garden-2's exit 0 is a real door (to its neighbour), exit 2 a doorway
: door0 ( -- d )  front-garden-2 0 room-exit-door ;
: next0 ( -- room )  front-garden-2 0 room-exit-leads drop ;
: back0 ( -- exit )  front-garden-2 0 room-exit-leads nip ;     \ (the same door from the other side)
: state@ ( d -- w )  4 * progress pr.doors + l@ ;
: clean ( d -- )  4 * progress pr.doors + 0 swap l! ;
door0 clean
: hold ( room exit kind -- answer )  -1 answer !  as-user doors send hold-door leave  deliver  answer @ ;

testing locking
T{ door0 doors send lock deliver  door0 state@ 8 and 0<>  door0 door-open? -> -1 0 }T
T{ front-garden-2 0 fiona-kind hold -> 1 }T        \ (quirk: a locked door can be held...)
T{ as-user front-garden-2 0 fiona-noise doors send let-go-open leave deliver  door0 door-open?  door0 state@ 1 and -> 0 1 }T
                                                    \ (...but not opened, and it stays held)
T{ door0 doors send unlock deliver  door0 state@ 8 and -> 0 }T
door0 clean

testing holding: once, by one
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ front-garden-2 0 hewie-kind hold -> 0 }T                       \ already held
testing letting go open, then shut
T{ as-user front-garden-2 0 fiona-noise doors send let-go-open leave deliver  door0 door-open?  door0 state@ 1 and -> -1 0 }T
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ as-user front-garden-2 0 fiona-noise doors send let-go-shut leave deliver  door0 door-open? -> 0 }T
testing letting go of a door nobody holds changes nothing
T{ as-user front-garden-2 0 fiona-noise doors send let-go-open leave deliver  door0 door-open? -> 0 }T

testing locked against one kind
T{ door0 stalker-kind true doors send lock-for deliver  front-garden-2 0 stalker-kind hold -> 0 }T
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ as-user front-garden-2 0 fiona-noise doors send let-go-shut leave deliver -> }T
T{ door0 stalker-kind false doors send lock-for deliver  front-garden-2 0 stalker-kind hold -> 1 }T
T{ as-user front-garden-2 0 stalker-noise doors send let-go-shut leave deliver -> }T

testing a doorway can't be held (always open)
T{ front-garden-2 2 fiona-kind hold -> 0 }T

testing a door used off-screen is heard (0xF at the door, from the user's source)
\ (the listener in the neighbour room, the door used from that side)
: listen-in ( room -- )   \ the user listens, standing on that room's first triangle (not played: no distance)
    as-user  0 0e 0e 0e body-place  0 stalker-noise s" acoustics" actor-named send listen  leave  deliver ;
next0 listen-in
T{ next0 back0 hewie-kind hold -> 1 }T
T{ -1 got !  as-user next0 back0 hewie-noise doors send let-go-open leave  actors-frame  got @ got-loud @ -> hewie-noise $F }T
T{ next0 back0 hewie-kind hold -> 1 }T
T{ as-user next0 back0 hewie-noise doors send let-go-shut leave  actors-frame -> }T

testing in the played room a held door swung open settles open (and makes its creak's noise)
door0 clean
: listen-here ( -- )   \ the user listens at the door, in the played room
    as-user  0 exit-stand drop  room-id 0 body-place  leave ;
listen-here
T{ front-garden-2 0 fiona-kind hold -> 1 }T
T{ as-user 0 true fiona-noise doors send swing-door leave  30 frames  door0 door-open? -> -1 }T
testing slammed shut: the latch is loud (0x5F)
T{ front-garden-2 0 fiona-kind hold -> 1 }T
: slammed ( -- source loud )
    -1 got !  0 got-loud !  as-user 0 fiona-noise doors send slam leave
    12 0 do  game-tick  got @ 0< 0= if  leave  then  loop  got @ got-loud @ ;
T{ slammed -> fiona-noise $5F }T
T{ door0 door-open? -> 0 }T

test-summary
