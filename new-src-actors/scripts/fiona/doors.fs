\ fiona/doors.fs - Fiona at the doors (Fiona_StateDoorStart and on; her part of
\ Progress_WhoIsWhere / Progress_CharRequests / Progress_PlayerButtons): in a door's area she
\ may use it with the action button. She walks to her spot for the door's animation, takes hold
\ of the door, opens or shuts it by hand (her animation 0x600 + its kind, the door swinging with
\ her) and lets it go as she steps out. A locked door she tries, and gives up.
IN: fiona.doors
USING: engine vectors keys actors messages common paths doors fiona.state fiona.anims fiona.model fiona.moving fiona.spots ;

: doors-id ( -- id )  s" doors" actor-named ;
defer to-idle   ' noop is to-idle      \ (fiona.fs)
: door-over ( -- )  -1 her-move-done !  to-idle ;   \ (a use from the scripts: their move done)
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
: quick? ( -- flag )  her-fear-bits @ 2 and 0=  her-danger @ chased <> and ;   \ not panicking, not chased

\ ---- the door use (her states, run each frame through her-act) ----
: kind! ( kind anim -- )  her-door-anim !  her-door-kind ! ;
: door-spot ( -- tri ) ( F: -- x y z yaw )  her-door-exit @ her-door-kind @ door-user-spot ;
: let-go ( -- )   \ the door left open (she opened it) or shut
    room-id her-door-exit @ fiona-noise
    her-door-opens @ if  doors-id send let-go-open  else  doors-id send let-go-shut  then ;

\ stepping out: free again at her animation's end, the door let go
: stepping-out ( -- )
    ended? if  let-go  -1 her-door-exit !  door-over exit  then
    root-move ;
\ her animation fading in: then the door swings with her
: swinging-in ( -- )
    settled? 0= if  exit  then
    her-door-exit @ her-door-kind @ fiona-noise doors-id send use-door
    ['] stepping-out her-act ! ;
\ waiting for the door's answer (door-held / door-refused: fiona.fs)
: holding ( -- ) ;
\ walking to her spot; there, she takes hold of it (if it is still as it was)
: to-the-door ( -- )
    walk-to-spot dup 0< if  drop door-over exit  then  if  exit  then
    her-door-exit @ door-of-exit door-open?  her-door-opens @ = if  door-over exit  then
    room-id her-door-exit @ fiona-kind doors-id send hold-door
    ['] holding her-act ! ;
: got-hold ( -- )   \ (the answer: hers) her animation, the door with it
    her-door-anim @ play-own  ['] swinging-in her-act ! ;

\ a locked door. Quick: at her spot she tries it, and as the built-in script has it (D_003B2450:
\ the try, 7 frames, then - not while Hewie is the one played - "locked from the other side",
\ system message $8001) she stands again once the message is read and the try played out.
\ Slow (chased): from the spot, no message; worn out (panicking: her fear's bit 2) from where she
\ stands - sliding to the spot a seventh of the way a frame as the try fades in
\ (Fiona_StateLockedSlow / LockedTry / LockedSpot / LockedQuick).
: try-over ( -- )  ended? if  door-over exit  then  root-move ;   \ (Fiona_StateIdleAtEvent2)
: quick-trying ( -- )
    1 her-rattle +!
    her-rattle @ 7 = if  $8001 broadcast fiona-message  then
    ended? if  -1 idle-anim  then
    her-rattle @ 7 > her-reading @ 0= and  anim@ her-door-anim @ <> and if  0 her-rattle !  door-over exit  then
    anim@ her-door-anim @ = if  root-move  then ;
: to-try ( -- )   \ (quick)
    walk-to-spot dup 0< if  drop door-over exit  then  if  exit  then
    6 her-still !  0 her-rattle !  her-door-anim @ play-own  ['] quick-trying her-act ! ;
: to-try-slow ( -- )
    walk-to-spot dup 0< if  drop door-over exit  then  if  exit  then
    her-door-anim @ play-own  ['] try-over her-act ! ;
: sliding ( -- )   \ (as the try fades in: along the way and round to the door; then on the spot)
    settled? if  arrive-spot drop  ['] try-over her-act !  exit  then
    her-yaw f@ her-heading f@ her-try-turn f@ turn-toward fdrop her-yaw f!
    her-path her-at her-try-step f@ path-ahead drop  her-at ahead vec-copy
    her-at blocked-floor v-tri-in dup 0< if  drop  else  her-tri !  then ;
: slide-to-try ( -- )   \ (once what she was doing has faded in: the way to the spot, none - she gives up)
    settled? 0= if  exit  then
    plan 0= if  door-over exit  then
    rest-of-way 7e f/ her-try-step f!
    her-heading f@ her-yaw f@ f- angle-wrap fabs 7e f/ her-try-turn f!
    her-door-anim @ play-own  ['] sliding her-act ! ;

\ Fiona_StateDoorStart: which animation (by the side she is on, quick or slow), then to the spot
: walk-or-stop ( next -- )
    {: next :}  door-spot dup 0< if  drop fdrop fdrop fdrop fdrop door-over exit  then
    walk-spot  next her-act ! ;
: door-kind ( -- kind )   \ (FIN_D000.MTN's: the side she is on, shutting it, slow - chased or panicking)
    her-door-side @ 2*  her-door-opens @ 0= 1 and +  quick? 0= 4 and + ;
: door-start ( -- )
    her-door-opens @ 0=  her-door-exit @ door-of-exit door-locked? 0= or if   \ shutting it, or opening it
        door-kind dup door-anim + kind!  ['] to-the-door walk-or-stop exit
    then
    \ locked: she tries it
    door-kind
    quick? if
        her-door-side @ if  rattle-front-anim  else  rattle-behind-anim  then  kind!  ['] to-try walk-or-stop exit
    then
    her-door-side @ if  rattle-front-slow-anim  else  rattle-behind-slow-anim  then  kind!
    her-fear-bits @ 2 and if  ['] slide-to-try  else  ['] to-try-slow  then  walk-or-stop ;

\ struck while using a door (Fiona_StartInDoor): a door she holds is let go as she meant it -
\ open, or shut
: drop-door ( -- )
    her-door-exit @ dup 0< if  drop exit  then
    door-of-exit door-held? 0= if  -1 her-door-exit !  exit  then
    let-go  -1 her-door-exit ! ;

\ the story's door use (its move 3 / 4: Fiona_Requests): through the door at `exit`, opening it or
\ shutting it, whatever its state
: scripted-door ( exit opens? -- )
    her-door-opens !  dup her-door-exit !  her-pos door-side 1 = 1 and her-door-side !
    door-doing her-doing !  led-mode her-mode !  door-start ;

\ the action button at a door (Fiona_StateBlock 2: her request taken)
: use ( exit -- )
    dup her-door-exit !
    dup door-of-exit door-open? 0= her-door-opens !
    her-pos door-side 1 = 1 and her-door-side !
    door-doing her-doing !  led-mode her-mode !
    door-start ;
: action-button? ( -- flag )  circle button-pressed? ;   \ (Progress_PlayerButtons: circle)
\ Progress_PlayerButtons (her doors): free, the button, an exit she may use
: try-doors ( -- )
    her-mode @ own-mode <>  her-doing @ free-doing <> or  her-sub @ pushing-sub = or if  exit  then   \ (not while pushing)
    action-button? 0= if  exit  then
    usable dup 0< if  drop exit  then  use ;
