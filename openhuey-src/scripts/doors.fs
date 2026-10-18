\ doors.fs - the doors: their locks, open or shut, held, closed off (docs/subsystems/doors.md).
\ Their state is a fact in C (the progress' door words: routes read it); only this actor
\ changes it. In the played room it also swings the door models and makes their noise.
IN: doors
USING: engine game-state actors messages facts ;

\ ---- a door's state word: held, opened, stuck, locked; the kinds it is locked against (bits 4..7) ----
1 constant held-bit   2 constant opened-bit   4 constant stuck-bit   8 constant locked-bit
: door-word ( door -- addr )  4 * progress pr.doors + ;
: state@ ( door -- w )  door-word l@ ;
: has? ( door bits -- flag )  swap state@ and 0<> ;
: set-bits ( door bits -- )  over door-word l@ or swap door-word l! ;
: clear-bits ( door bits -- )  invert over door-word l@ and swap door-word l! ;
: against ( door -- bits )  state@ 4 rshift $F and ;           \ the kinds it is locked against
: kind-bit ( kind -- bit )   \ (Fiona 1, Hewie 2, a stalker - or any other - 4)
    case  fiona-kind of  1  endof  hewie-kind of  2  endof  >r 4 r>  endcase ;
: locked-against? ( door kind -- flag )  kind-bit swap against and 0<> ;
: fixed? ( door -- flag )  door-flags 1 and 0<> ;                \ a doorway: always open
: door-of ( room exit -- door | -1 )  room-exit-door ;
\ (a fact for users: check it before trying a door - holding doesn't, as in the original)
: door-locked? ( door -- flag )  dup 0< if  drop false exit  then  locked-bit has? ;
: door-held? ( door -- flag )  dup 0< if  drop false exit  then  held-bit has? ;
: closed-off? ( door -- flag )  progress pr.closed-off bit? ;
: closed-off! ( door -- )  progress pr.closed-off bit-set ;
: reopened! ( door -- )  progress pr.closed-off bit-clear ;
: acoustics-id ( -- id )  s" acoustics" actor-named ;

\ ---- the rooms within two open doors of a room (SceneGame_RoomsNear): the room first, then
\ each room an open exit (not locked, nor locked against any of the three kinds) leads to, and
\ each an open exit of that one leads to - each once, `max` at most ----
: exit-open? ( room exit -- flag )
    room-exit-door dup 0< if  drop false exit  then
    dup door-locked? if  drop false exit  then  against 7 and 0= ;
: in-list? ( room list n -- flag )  {: r list n :}  false  n 0 ?do  list i cells + @ r = or  loop ;
: rooms-near ( room list max -- n )
    {: room list max | n :}
    room list !  1 to n
    8 0 do
        room i room-exit-leads drop {: r :}
        r 0< 0=  room i exit-open? and  r list n in-list? 0= and  n max < and if
            r list n cells + !  n 1+ to n
            8 0 do
                r i room-exit-leads drop {: t :}
                t 0< 0=  r i exit-open? and  t list n in-list? 0= and  n max < and if  t list n cells + !  n 1+ to n  then
            loop
        then
    loop  n ;

\ ---- the played room's doors on the floor (Doors_RoomIn / Doors_Refresh) ----
$1000000 constant hewie-barred   $4000000 constant stalker-barred   \ (the nav mesh's passage flags)
: locks ( door -- flags )   \ the passage flags its locks put on the floor
    dup locked-bit has? if  drop hewie-barred stalker-barred or exit  then
    0  over hewie-kind locked-against? if  hewie-barred or  then
    swap stalker-kind locked-against? if  stalker-barred or  then ;
: refresh ( exit -- )   \ its passage: open or shut, and its locks
    dup door-here? 0= if  drop exit  then
    room-id over door-of  {: ex door :}  door 0< if  exit  then
    ex  door door-open?  door locks  door-passage ;
: room-in ( -- )   \ each door stands open or shut as its state says
    8 0 do
        i door-here?  room-id i door-of 0< 0= and if
            room-id i door-of door-open? if  -90e  else  0e  then  i true door-swing
            i refresh
        then
    loop ;
: refresh-door ( door -- )   \ its passage, if it is in the played room
    8 0 do  room-id i door-of over = if  i refresh  then  loop  drop ;
: changed ( door -- )  dup refresh-door  broadcast door-changed ;
: lock-against ( door kind on -- )   \ locked against a kind of character, or not
    {: door kind on :}  door  kind kind-bit 4 lshift  on if  set-bits  else  clear-bits  then  door changed ;
: door-rooms ( door -- room0 room1 )  door-sides {: r0 e0 t0 r1 e1 t1 :}  r0 r1 ;
: grab ( exit -- )   \ (held by the slammer: not a doorway)
    room-id swap door-of dup 0< if  drop exit  then  dup fixed? if  drop exit  then  held-bit set-bits ;

\ ---- held doors (DoorHold_Take / DoorHold_Open / DoorHold_Shut) ----
: take ( room exit kind -- ok? )   \ refused: no door, a doorway, locked against the kind, held
    {: room ex kind :}  room ex door-of {: door :}
    door 0< if  false exit  then
    door fixed?  door kind locked-against? or  door held-bit has? or if  false exit  then
    door held-bit set-bits  true ;
$F constant quiet-door   $5F constant loud-door   \ (its noises' loudness)
: let-go ( room exit source open? -- )
    {: room ex source open :}  room ex door-of {: door :}
    door 0< if  exit  then
    door fixed?  door locked-bit has? or  door held-bit has? 0= or if  exit  then
    open if  door stuck-bit has? if  exit  then  door opened-bit set-bits
    else  door opened-bit clear-bits  then
    door held-bit clear-bits
    door changed
    \ let go off-screen, it is heard: at the door, from the user's source (door_heard)
    room room-id <> if  quiet-door room -1 door source acoustics-id send noise  then ;

\ ---- the played room's door models: who is using each (its noise source) ----
state: doors-state
  8 cells field users        \ by exit: the noise source of whoever is using its door (-1 none)
end-state
: user ( exit -- addr )  cells users + ;
\ (Door_PlaySound: quiet 1 / 2, loud 3 / 4; heard from the user's source)
: noise-of ( how -- loud )  case  1 of quiet-door endof  2 of quiet-door endof  3 of loud-door endof  4 of loud-door endof  >r 0 r>  endcase ;
: swing-events ( exit -- )   \ its sound's noise; at rest, let go open or shut by its user
    dup door-events  {: ex sound how settled :}
    how noise-of ?dup if  room-id -1  room-id ex door-of  ex user @  acoustics-id send noise  then
    settled 0< if  exit  then
    room-id ex  ex user @  settled let-go ;

behaviour keeping
  on spawned ( -- )  self subscribe tick  self subscribe entered-room  users 8 cells -1 fill ;
  on entered-room ( room exit -- )  2drop  room-in  users 8 cells -1 fill ;
  on redo-doors ( -- )  room-in ;   \ (Doors_RoomIn, for the story)
  on lock ( door -- )  dup locked-bit set-bits  changed ;
  on door-opened ( door on -- )  if  dup opened-bit set-bits  else  dup opened-bit clear-bits  then  changed ;
  on unlock ( door -- )  dup locked-bit clear-bits  changed ;
  on lock-for ( door kind on -- )  lock-against ;
  on lock-room-for ( room kind on -- )   \ every door into a room (SceneGame_SetRoomDoors)
      {: room kind on :}
      400 0 do
          i door-rooms  over 0< if  2drop leave  then        \ (the doors end at one with no sides)
          room =  swap room =  or if  i kind on lock-against  then
      loop ;
  on copy-door ( door from -- )   \ (the story's 0x4D) its open bit, lock and closed-off as from's
      {: door from :}
      door opened-bit clear-bits  door locked-bit clear-bits  door reopened!
      from opened-bit has? if  door opened-bit set-bits  then
      from locked-bit has? if  door locked-bit set-bits  then
      from closed-off? if  door closed-off!  then
      door changed ;
  on close-off ( door -- )  dup closed-off!  changed ;
  on reopen ( door -- )  dup reopened!  changed ;
  on rebuild-exits ( -- )  world-exits-rebuild ;
  on use-door ( exit anim source -- )
      {: ex anim source :}  source ex user !
      room-id ex door-of 0< if  exit  then  ex anim door-anim drop ;
  on swing-door ( exit open source -- )  {: ex open source :}  source ex user !  ex open false door-move ;
  \ slammed shut or burst open (Fiona_SlamDoor: taken hold of first - DoorHold_Take - then let go
  \ open or shut at rest by its user, as a swing along an animation is)
  on slam ( exit source -- )  over user !  dup grab  false true door-move ;
  on barge ( exit source -- )  over user !  dup grab  true true door-move ;
  \ pulled shut behind a stalker come in by it (his DoorBreak): held, swung shut, let go shut at rest
  on pull-shut ( exit source -- )  over user !  dup grab  false false door-move ;
  on hold-door ( room exit kind -- )
      {: room ex kind :}  room ex  room ex kind take  if  sender send door-held  else  sender send door-refused  then ;
  on let-go-open ( room exit source -- )  true let-go ;
  on open-from-afar ( room exit kind source -- )  {: room ex kind source :}  room ex kind take if  room ex source true let-go  then ;
  on let-go-shut ( room exit source -- )  false let-go ;
  on tick ( -- )  8 0 do  i door-here? if  i swing-events  then  loop ;
end-behaviour

: doors-spawn ( -- id )  keeping doors-state s" doors" spawn ;
