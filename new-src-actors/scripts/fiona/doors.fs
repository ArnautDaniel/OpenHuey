\ fiona/doors.fs - Fiona at the doors (Fiona_StateDoorStart and on; her part of
\ Progress_WhoIsWhere / Progress_CharRequests / Progress_PlayerButtons): in a door's area she
\ may use it with the action button. She walks to her spot for the door's animation, takes hold
\ of the door, opens or shuts it by hand (her animation 0x600 + its kind, the door swinging with
\ her) and lets it go as she steps out. A locked door she tries, and gives up.
IN: fiona.doors
USING: engine keys actors messages common doors fiona.state fiona.model fiona.moving fiona.spots ;

: doors-id ( -- id )  s" doors" actor-named ;
defer to-idle   ' noop is to-idle      \ (fiona.fs)
: door-of-exit ( exit -- door | -1 )  room-id swap room-exit-door ;

\ ---- the exit she may use: the first whose door area she stands in (Progress_WhoIsWhere: the
\ area before its door), with a door nobody holds ----
: in-door-area? ( exit -- flag )  0 her-pos door-in-area? ;
: usable ( -- exit | -1 )
    8 0 do
        i door-here? if  i in-door-area? if
            i door-of-exit dup 0< 0= swap door-held? 0= and if  i unloop exit  then
        then  then
    loop  -1 ;
: quick? ( -- flag )  her-fear-bits @ 2 and 0=  her-danger @ 2 <> and ;   \ not panicking, not chased

\ ---- the door use (her states, run each frame through her-act) ----
: kind! ( kind anim -- )  her-door-anim !  her-door-kind ! ;
: play-own ( anim -- )  dup entry 8 or >r >r -1 r> r> start ;   \ (Motion_PlayOwnBlend, no variant)
: door-spot ( -- tri ) ( F: -- x y z yaw )  her-door-exit @ her-door-kind @ door-user-spot ;
: let-go ( -- )   \ the door left open (she opened it) or shut
    room-id her-door-exit @ fiona-noise
    her-door-opens @ if  doors-id send let-go-open  else  doors-id send let-go-shut  then ;

\ stepping out: free again at her animation's end, the door let go
: stepping-out ( -- )
    ended? if  let-go  -1 her-door-exit !  to-idle exit  then
    root-move ;
\ her animation fading in: then the door swings with her
: door-anim ( -- )
    settled? 0= if  exit  then
    her-door-exit @ her-door-kind @ fiona-noise doors-id send use-door
    ['] stepping-out her-act ! ;
\ waiting for the door's answer (door-held / door-refused: fiona.fs)
: holding ( -- ) ;
\ walking to her spot; there, she takes hold of it (if it is still as it was)
: to-the-door ( -- )
    walk-to-spot dup 0< if  drop to-idle exit  then  if  exit  then
    her-door-exit @ door-of-exit door-open?  her-door-opens @ = if  to-idle exit  then
    room-id her-door-exit @ fiona-kind doors-id send hold-door
    ['] holding her-act ! ;
: got-hold ( -- )   \ (the answer: hers) her animation, the door with it
    her-door-anim @ play-own  ['] door-anim her-act ! ;

\ a locked door: walking to the spot, then trying it (her rattle), and back to standing
: rattled ( -- )  1 her-rattle +!  her-rattle @ 7 >= if  to-idle  then ;
: rattling ( -- )  ended? if  -1 idle-anim  0 her-rattle !  ['] rattled her-act !  exit  then  root-move ;
: to-try ( -- )
    walk-to-spot dup 0< if  drop to-idle exit  then  if  exit  then
    6 her-still !  her-door-anim @ -1 play-table  ['] rattling her-act ! ;

\ Fiona_StateDoorStart: which animation (by the side she is on, quick or slow), then to the spot
: walk-or-stop ( next -- )
    >r door-spot dup 0< if  r> 2drop fdrop fdrop fdrop fdrop to-idle exit  then
    walk-spot  r> her-act ! ;
: door-start ( -- )
    her-door-opens @ 0= if   \ shutting it
        her-door-side @ 0= if  quick? if  1 $601  else  5 $605  then
        else  quick? if  3 $603  else  7 $607  then  then  kind!
        ['] to-the-door walk-or-stop exit
    then
    her-door-exit @ door-of-exit door-locked? 0= if   \ opening it
        her-door-side @ 0= if  quick? if  0 $600  else  4 $604  then
        else  quick? if  2 $602  else  6 $606  then  then  kind!
        ['] to-the-door walk-or-stop exit
    then
    \ locked: she tries it (the slow try - a seventh of the way a frame - with the panic: F2)
    quick? if  her-door-side @ 0= if  0 $609  else  2 $608  then
    else  her-door-side @ 0= if  4 $60B  else  6 $60A  then  then  kind!
    ['] to-try walk-or-stop ;

\ the action button at a door (Fiona_StateBlock 2: her request taken)
: use ( exit -- )
    dup her-door-exit !
    dup door-of-exit door-open? 0= her-door-opens !
    her-pos door-side 1 = 1 and her-door-side !
    3 her-doing !  2 her-mode !
    door-start ;
: action-button? ( -- flag )  key: Space key-pressed?  key: Return key-pressed? or ;
\ Progress_PlayerButtons (her doors): free, the button, an exit she may use
: try-doors ( -- )
    her-mode @ 0<>  her-doing @ 0<> or  her-sub @ 5 = or if  exit  then
    action-button? 0= if  exit  then
    usable dup 0< if  drop exit  then  use ;
