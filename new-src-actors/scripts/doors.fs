\ doors.fs - the doors: their locks, open or shut, held, closed off (docs/subsystems/doors.md).
\ Their state is a fact in C (the progress' door words: routes read it); only this actor
\ changes it. In the played room it also swings the door models and makes their noise.
IN: doors
USING: engine game-state actors messages ;

\ ---- a door's state word ----
1 constant held-bit   2 constant opened-bit   4 constant stuck-bit   8 constant locked-bit
: door-word ( door -- addr )  4 * progress pr.doors + ;
: state@ ( door -- w )  door-word l@ ;
: set-bits ( door bits -- )  over door-word l@ or swap door-word l! ;
: clear-bits ( door bits -- )  invert over door-word l@ and swap door-word l! ;
: against ( door -- bits )  state@ 4 rshift $F and ;           \ the kinds it is locked-bit against
: kind-bit ( kind -- bit )  case  0 of 1 endof  1 of 2 endof  >r 4 r>  endcase ;
: fixed? ( door -- flag )  door-flags 1 and 0<> ;                \ a doorway: always open
: door-of ( room exit -- door | -1 )  room-exit-door ;
\ (a fact for users: check it before trying a door - holding doesn't, as in the original)
: door-locked? ( door -- flag )  dup 0< if  drop false exit  then  state@ locked-bit and 0<> ;
: door-held? ( door -- flag )  dup 0< if  drop false exit  then  state@ held-bit and 0<> ;
: acoustics-id ( -- id )  s" acoustics" actor-named ;

\ ---- the played room's doors on the floor (Doors_RoomIn / Doors_Refresh) ----
: locks ( door -- flags )   \ the passage flags its locks put on the floor
    dup state@ locked-bit and if  drop $5000000 exit  then
    against  dup 2 and if  $1000000  else  0  then  swap 4 and if  $4000000 or  then ;
: refresh ( exit -- )   \ its passage: open or shut, and its locks
    dup door-here? 0= if  drop exit  then
    room-id over door-of dup 0< if  2drop exit  then          ( exit door )
    dup door-open? swap locks door-passage ;
: room-in ( -- )   \ each door stands open or shut as its state says
    8 0 do
        i door-here?  room-id i door-of 0< 0= and if
            room-id i door-of door-open? if  -90e  else  0e  then  i true door-swing
            i refresh
        then
    loop ;
: refresh-door ( door -- )   \ its passage, if it is in the played room
    8 0 do  room-id i door-of over = if  i refresh  then  loop  drop ;
: closed-off! ( door -- )
    dup 32 / 4 * progress pr.closed-off +  swap 32 mod 1 swap lshift  over l@ or  swap l! ;
: changed ( door -- )  dup refresh-door  broadcast door-changed ;

\ ---- held doors (DoorHold_Take / DoorHold_Open / DoorHold_Shut) ----
variable the-room   variable the-door   variable the-source
\ a held-bit door let go off-screen is heard: 0xF at the door, from the user's source (door_heard)
: heard-off-screen ( -- )
    the-room @ room-id = if  exit  then
    $F  the-room @  -1  the-door @  the-source @  acoustics-id send noise ;
: take ( room exit kind -- ok? )
    >r door-of dup 0< if  r> 2drop false exit  then              ( door ) ( r: kind )
    dup fixed? if  r> 2drop false exit  then
    r> kind-bit over against and if  drop false exit  then
    dup state@ held-bit and if  drop false exit  then
    held-bit set-bits  true ;
: let-go ( room exit source open? -- )
    >r  the-source !  over the-room !  door-of the-door !  r>       ( open? )
    the-door @ 0< if  drop exit  then
    the-door @ fixed?  the-door @ state@ locked-bit and or  the-door @ state@ held-bit and 0= or if  drop exit  then
    if  the-door @ state@ stuck-bit and if  exit  then  the-door @ opened-bit set-bits
    else  the-door @ opened-bit clear-bits  then
    the-door @ held-bit clear-bits
    the-door @ changed  heard-off-screen ;

\ ---- the played room's door models: who is using each (its noise source) ----
create users 8 cells allot
: user ( exit -- addr )  cells users + ;
\ (Door_PlaySound: quiet 1 / 2 -> 0xF, loud 3 / 4 -> 0x5F; heard from the user's source)
: noise-of ( how -- loud )  case  1 of $F endof  2 of $F endof  3 of $5F endof  4 of $5F endof  >r 0 r>  endcase ;
variable ev-exit
: swing-events ( exit -- )   \ its sound's noise; at rest, let go open or shut by its user
    dup ev-exit !  door-events                                     ( sound how settled )
    >r  noise-of nip                                               ( loud ) ( r: settled )
    ?dup if  room-id -1  room-id ev-exit @ door-of  ev-exit @ user @  acoustics-id send noise  then
    r> dup 0< if  drop exit  then
    >r  room-id ev-exit @  ev-exit @ user @  r> let-go ;

behaviour keeping
  on spawned ( -- )  self subscribe tick  self subscribe entered-room  users 8 cells -1 fill ;
  on entered-room ( room exit -- )  2drop  room-in  users 8 cells -1 fill ;
  on lock ( door -- )  dup locked-bit set-bits  changed ;
  on unlock ( door -- )  dup locked-bit clear-bits  changed ;
  on lock-for ( door kind on -- )
      >r kind-bit 4 lshift  over swap  r> if  set-bits  else  clear-bits  then  changed ;
  on close-off ( door -- )  dup closed-off!  changed ;
  on use-door ( exit anim source -- )
      2 pick user !  over room-id swap door-of 0< if  2drop exit  then
      door-anim drop ;
  on swing-door ( exit open source -- )  2 pick user !  false door-move ;
  on slam ( exit source -- )  over user !  false true door-move ;
  on hold-door ( room exit kind -- )
      >r 2dup r> take  if  sender send door-held  else  sender send door-refused  then ;
  on let-go-open ( room exit source -- )  true let-go ;
  on let-go-shut ( room exit source -- )  false let-go ;
  on tick ( -- )  8 0 do  i door-here? if  i swing-events  then  loop ;
end-behaviour

: doors-spawn ( -- id )  keeping -1 s" doors" spawn ;
