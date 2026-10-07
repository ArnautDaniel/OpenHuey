\ fiona/doors.fs - Fiona at the doors (src/game/fiona.c Fiona_StateDoorStart and on; the
\ progress' Progress_WhoIsWhere / Progress_CharRequests / Progress_PlayerButtons): who is at
\ each exit, the request her place there makes (open the door, shut it behind her, try a locked
\ one), her buttons taking it, and the door animations: she walks to her spot by the door and
\ opens or shuts it by hand, the door swinging with her (the door's animation from
\ O_FIN/FIN_D000.MTN). Through it, the room's scripts take the exit.
\
\ Her buttons on the keyboard: Space (or Return) circle - the action; F square - a kick or a
\ shove; Q R1 - turning to flee.
IN: fiona.doors
USING: engine keys game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain fiona.commands ;

\ ---- Progress_WhoIsWhere: at each exit, the characters (a bit a slot) 0 in the area before its
\ door, 3 on its sides' floor lists, 4 in front of it, 5 at it ----
create exit-who 8 6 * cells allot
: who# ( exit field -- addr )  swap 6 * + cells exit-who + ;
: mark ( exit field slot -- )  >r who# 1 r> lshift over @ or swap ! ;
: occupant? ( cs -- flag )
    dup c-ok? 0= if  drop false exit  then
    dup character char.present sl@ 0= if  drop false exit  then
    dup character char.room sl@ room-id =  swap character char.disabled sl@ 0= and ;
: who-is-where ( -- )
    exit-who 48 cells 0 fill
    8 0 do
        characters 6 min 0 do
            i occupant? if
                j i c-pos vec@ door-near? if
                    j 5 i mark
                    j 0 i c-pos vec@ door-in-area? if  j 0 i mark  then
                    j 1 i c-tri door-on-side?  j 0 i c-tri door-on-side? or if  j 3 i mark  then
                then
                j i c-pos vec@ door-side 1 = if  j 4 i mark  then
            then
        loop
    loop ;
\ exit_flags: slot's state at the exit (2 in its area, 8 on its sides, 0x10 in front, 0x20 at it)
: flags-of ( exit slot -- f )
    1 swap lshift >r  0
    over 0 who# @ r@ and if  4 or  then  over 3 who# @ r@ and if  8 or  then
    over 4 who# @ r@ and if  $10 or  then  swap 5 who# @ r> and if  $20 or  then ;

\ ---- the doors' state (the progress: bit 0 held, 1 open, 2 barred, 3 locked; Progress_ExitOpen,
\ DoorHold_*) ----
variable door-exit   \ unk100: the exit she deals with
: the-door ( -- d )  door-exit @ event-exit-door ;
: fixed? ( d -- flag )  dup 0< if  drop true exit  then  event-door-flags 1 and 0<> ;
: dw ( d -- w )  door-word dup if  l@  then ;
: dw! ( w d -- )  door-word dup if  l!  else  2drop  then ;
: exit-open? ( exit -- flag )   \ Progress_ExitOpen
    event-exit-door dup fixed? if  drop true exit  then  dw dup 8 and if  drop false exit  then  2 and 0<> ;
: hold-take ( -- fail? )   \ DoorHold_Take for her (side 0: the lock's side bits)
    the-door dup fixed? if  drop true exit  then
    dup dw 4 rshift 1 and if  drop true exit  then
    dup dw 1 and if  drop true exit  then
    dup dw 1 or swap dw!  false ;
: hold-open ( -- )   \ DoorHold_Open
    the-door dup fixed? if  drop exit  then  dup dw
    dup 8 and over 4 and or over 1 and 0= or if  2drop exit  then
    2 or 1 invert and swap dw! ;
: hold-shut ( -- )   \ DoorHold_Shut
    the-door dup fixed? if  drop exit  then  dup dw
    dup 8 and over 1 and 0= or if  2drop exit  then
    2 invert and 1 invert and swap dw! ;
: barred? ( -- flag )   \ DoorHold_Usable: it can't be used (locked, or barred)
    the-door dup fixed? if  drop false exit  then  dw dup 8 and if  drop true exit  then  4 and 0<> ;

\ ---- Progress_CharRequests (her part: the exit she is before) ----
variable req-exit
: others-in-way? ( exit -- flag )   \ nobody but Hewie may be in the doorway
    6 2 do
        i occupant? if  dup i flags-of dup 8 and swap $14 and $14 = or if  drop true unloop exit  then  then
    loop  drop false ;
: exit-request ( -- )
    -1 req-exit !
    8 0 do  req-exit @ 0< i 0 flags-of 4 and 0<> and if  i req-exit !  then  loop
    req-exit @ 0< if  exit  then
    req-exit @ event-exit-door dup 0< if  drop exit  then           ( d )
    dup dw 1 and if  drop exit  then
    req-exit @ others-in-way? if  drop exit  then
    dup fixed? if  1  else  dup dw dup 8 and if  drop 0  else  2 and 0<> 1 and  then  then   ( d open )
    swap dw 8 and >r                                                 ( open ) ( R: locked )
    if  r> if  $80000002  else  2  then  0
    else r> if  $80000002 1  else  2 1  then then
    request-arg !  request !  req-exit @ request-b ! ;

\ ---- Progress_PlayerButtons: her buttons, while she is free ----
: take ( state a b -- )   \ player_take_action
    me relations:req-of 7 = if  2drop drop exit  then
    0e  0 0 me relations:req-set ;
: circle? ( -- flag )  key: Space key-pressed?  key: Return key-pressed? or ;
defer start-action-script ( arg -- )  ' drop is start-action-script
: player-buttons ( -- )
    me in-game? 0= f-mode 0<> or f-sub 5 = or if  exit  then
    f-act @ dup $E = over 1 = or swap $F = or if  exit  then
    me relations:held? if  exit  then
    f-act @ $D = 0= circle? and if
        request @ case
            $80000000 of  2 request-arg @ request-b @ take  endof
            $80000001 of  2 request-arg @ request-b @ take  endof
            $80000002 of  2 request-arg @ request-b @ take  endof
            5 of  request-arg @ start-action-script  5 request-arg @ request-b @ take  endof
            $80000005 of  endof  $80000004 of  endof  $80000003 of  endof  0 of  endof
            >r  request @ request-arg @ request-b @ take  r>
        endcase
    then
    panic @ 4 >= if  exit  then
    key: F key-pressed? f-mode 0= and if  8 $1A 0 take  then
    key: Q key-pressed? f-mode 0= and if  $B $22 0 take  then ;

\ ---- the door animations: the kind (0..7 of FIN_D000.MTN) and her animation 0x600 + kind; in
\ 0x14 (she opens it), out 0x15 (she shuts it); the side she is on; quick unless panicking or
\ in the chase ----
variable door-side#  variable door-used  variable door-kind  variable door-anim#
: kind! ( kind anim -- )  door-anim# !  door-kind ! ;
: quick? ( -- flag )  f-fear-bits @ 2 and 0=  game-mode @ 2 <> and ;
\ Motion_PlayOwnBlend(anim, -1): the table's fade, held while it fades in
: play-own ( anim -- )  dup f-entry 8 or >r >r -1 r> r> f-start ;
: door-spot ( -- tri ) ( F: -- x y z yaw )  door-exit @ door-kind @ door-user-spot ;
defer st-door-walk  ' noop is st-door-walk
defer st-locked-slow  ' noop is st-locked-slow
defer st-locked-spot  ' noop is st-locked-spot
defer st-locked-quick  ' noop is st-locked-quick
: give-up-shut ( -- )   \ no spot: idle, the door as it is
    to-idle
    f-sub $14 = if
        door-exit @ 0 $60000 false door-side-flags  door-exit @ 1 $60000 true door-side-flags  hold-open
    else
        door-exit @ 1 $60000 false door-side-flags  door-exit @ 0 $60000 true door-side-flags  hold-shut
    then ;
: walk-or-give-up ( next -- )
    door-spot dup 0< if  2drop fdrop fdrop fdrop fdrop give-up-shut exit  then
    swap walk-spot ;
: st-door-start ( -- )   \ Fiona_StateDoorStart
    f-sub $14 <> if
        door-side# @ 0= if  quick? if  1 $601  else  5 $605  then
        else  quick? if  3 $603  else  7 $607  then  then  kind!
        ['] st-door-walk walk-or-give-up exit
    then
    barred? 0= if
        door-side# @ 0= if  quick? if  0 $600  else  4 $604  then
        else  quick? if  2 $602  else  6 $606  then  then  kind!
        ['] st-door-walk walk-or-give-up exit
    then
    quick? 0= if
        door-side# @ 0= if  4 $60B  else  6 $60A  then  kind!
        f-fear-bits @ 2 and if
            door-spot spot-tri !  f-heading f!  spot vec!  ['] st-locked-slow behave
        else  door-spot dup 0< if  2drop fdrop fdrop fdrop fdrop to-idle exit  then  ['] st-locked-spot swap walk-spot  then
    else
        door-side# @ 0= if  0 $609  else  2 $608  then  kind!
        door-spot dup 0< if  2drop fdrop fdrop fdrop fdrop to-idle exit  then  ['] st-locked-quick swap walk-spot
    then ;

\ ---- through it ----
: st-door-step-out ( -- )   \ Fiona_StateDoorStepOut: free again at her animation's end
    1 f-2a !
    f-end? if
        0 f-2a !  0 f-2b !
        f-sub $14 = if
            door-exit @ 0 $60000 false door-side-flags  door-exit @ 1 $60000 true door-side-flags  hold-open
        else
            door-exit @ 1 $60000 false door-side-flags  door-exit @ 0 $60000 true door-side-flags  hold-shut
        then
        -1 door-in-use !  to-idle
    then
    f-root-move
    door-exit @ f-pos vec@ door-side door-side# @ <> if  1 f-2d ! exit  then
    f-tri dup 0< 0= if  nav-flags $20000 and 0<> 1 and  else  drop 0  then  f-2d ! ;
: st-door-anim ( -- )   \ Fiona_StateDoorAnim: faded in, the door swings with her
    1 f-2a !
    f-settled? 0= if  exit  then
    door-exit @ door-kind @ door-anim drop
    ['] st-door-step-out behave ;
:noname ( -- )   \ Fiona_StateDoorWalk: at the spot, the door used (unless it can't be)
    walk-to-spot dup 0< if  drop to-idle exit  then  if  exit  then
    door-exit @ 3 who# @ 4 and if  to-idle exit  then             \ (the stalker on its sides)
    door-exit @ exit-open?  f-sub $14 = if  if  to-idle exit  then  else  0= if  to-idle exit  then  then
    hold-take if  to-idle exit  then
    1 door-used !
    door-exit @  f-sub $14 = if  1  else  0  then  $20000 true door-side-flags
    door-exit @ door-in-use !
    door-anim# @ play-own
    1 f-2b !  1 f-2a !
    ['] st-door-anim behave ; is st-door-walk

\ ---- a locked door ----
: st-idle-at-event2 ( -- )  f-end? if  to-idle  then  f-root-move ;   \ Fiona_StateIdleAtEvent2
:noname ( -- )   \ Fiona_StateLockedSpot: there, the try (her animation)
    walk-to-spot dup 0< if  drop to-idle exit  then  if  exit  then
    door-anim# @ play-own  ['] st-idle-at-event2 behave ; is st-locked-spot
fvariable lt-step  fvariable lt-turn
: st-locked-try ( -- )   \ Fiona_StateLockedTry: turning and stepping to the spot while it plays
    f-settled? if
        spot-tri @ me c-tri!  me c-pos spot vec-copy  me c-sync  f-heading f@ f-yaw!
        ['] st-idle-at-event2 behave exit
    then
    f-heading f@ lt-turn f@ f-turn-toward fdrop
    lt-step f@ me path-ahead me path-i!  me c-pos pa-pos vec-copy  me c-sync ;
:noname ( -- )   \ Fiona_StateLockedSlow: a way to the spot, a seventh of it a frame
    f-settled? 0= if  exit  then
    me spot-tri @ spot -1 c-plan 0= if  to-idle exit  then
    me path-rest 7e f/ lt-step f!  f-heading f@ f-yaw f- angle-wrap fabs 7e f/ lt-turn f!
    door-anim# @ play-own  ['] st-locked-try behave ; is st-locked-slow
\ the rattle (D_003B2450, run for her: the try played out, the idle, 7 frames on)
variable rattle-t
: st-rattle-wait ( -- )  1 rattle-t +!  rattle-t @ 7 >= if  to-idle  then ;
: st-rattle ( -- )  f-end? if  -1 idle-anim  0 rattle-t !  ['] st-rattle-wait behave  then  f-root-move ;
:noname ( -- )   \ Fiona_StateLockedQuick: there, the try
    walk-to-spot dup 0< if  drop to-idle exit  then  if  exit  then
    1 f-2a !  6 f-still !
    door-anim# @ -1 f-play-table  ['] st-rattle behave ; is st-locked-quick

\ ---- her request for a door (Fiona_StateBlock 2) ----
: door-request-2 ( -- )
    me pursuer-slot @ 0e 0e c-touching? if  exit  then
    me 2 relations:req-word-of door-exit !
    me 1 relations:req-word-of 0= if  $15  else  $14  then  f-sub!
    door-exit @ dup exit-leads drop 0< 0= if  2 exit-spot drop  else  drop f-pos vec@  then
    door-exit @ door-side door-side# !
    0 door-used !  0 f-target-t !
    3 f-act !  2 f-mode!
    ['] st-door-start behave ;
:noname ( kind -- )   \ (2 a door; 9 a scripted door and 3 a ladder: with them)
    2 = if  door-request-2  then ; is door-request
' who-is-where is event-who-is-where   ' exit-request is event-exit-request
' player-buttons is event-player-buttons
