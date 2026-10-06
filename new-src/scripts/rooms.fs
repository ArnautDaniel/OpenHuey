\ rooms.fs - going from room to room (PageUp / PageDown), the camera put in each one's middle.
IN: rooms
USING: engine vectors views state ;

fvariable lx  fvariable ly  fvariable lz  fvariable hx  fvariable hy  fvariable hz
: mid ( F: a b -- m )  f+ f2/ ;

\ the camera in the middle of the room's box, level, looking along -z
: frame-room
    room-bounds  hz f! hy f! hx f! lz f! ly f! lx f!
    lx f@ hx f@ mid  ly f@ hy f@ mid  lz f@ hz f@ mid  cam-at
    0e camera cam.yaw sf!  0e camera cam.pitch sf! ;

: go ( id -- )
    room .room frame-room  1.0e camera cam.fov sf!  -1 view !
    playing @ if  place-player  then ;

\ the next room that exists from id, stepping by step (-1 if none)
: next-room ( id step -- id' )
    >r begin
        r@ +
        dup 0< over $fff > or if  rdrop drop -1 exit  then
        dup room-exists?
    until rdrop ;

: step-room ( step -- )
    room-id swap next-room
    dup 0< if  drop ." no more rooms that way" cr  else  go  then ;

: first-room  -1 1 next-room go ;

: room-keys
    key: PageUp key-pressed? if  1 step-room  then
    key: PageDown key-pressed? if  -1 step-room  then ;
' room-keys on-tick
