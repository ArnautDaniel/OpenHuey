\ events/runner.fs - running the rooms' event scripts as the original does (src/game/event.c:
\ Events_RunPhase, Events_CharEnter, Events_RunCharScripts; the order: src/game/scene_game.c).
\
\ Entering a room: phase 0 (the room's entering script, after resetting the event state), each
\ character's entering script, then phase 3. Every frame after: phase 1 (then the shared
\ after-phase-1 script), phase 2 (and after-phase-2), the action scripts in their 17 slots in
\ order, phase 3. Leaving: phase 4, then phase 5 once the next room is there.
\ Phase scripts run straight through; action scripts are held tasks given one turn a frame.
\ State flag $26 runs only phase 3 and a special script for characters entering.
IN: events.runner
USING: game-state events.core events.words events.builtin events.map0 events.map1 events.map2
    events.map3 events.map4 events.other ;

: ev-room ( -- n )  event-state ev.room sl@ ;
\ the context a phase script runs in: no slot, no character, id $FF, its own frame count 0
: phase-context ( -- )
    -1 event-state ev.slot l!  -1 event-state ev.self-char l!  $FF event-state ev.self-id l!
    0 event-state ev.self-frames l! ;
: run ( xt | 0 -- )  ?dup if  execute  then ;

\ phase 0: the room is entered (marked visited, the event state reset)
: entering ( -- )
    ev-room progress pr.visited bit-on
    event-state ev.vars script-vars 4 * 0 fill   0 event-state ev.bits l!
    free-slots  -1 event-state ev.message l!  -1 event-state ev.message-owner l!
    0 event-state ev.room-frames l!  0 event-state ev.leaving l! ;

: run-phase ( phase -- )
    phase-context
    dup 0 = if  entering  then
    dup 1 = if  1 event-state ev.room-frames +l!  then
    dup 5 = if  $22 state-flag-clear  then
    $26 state-flag? over 3 <> and 0= if  dup ev-room swap room-script @ run  then
    dup 1 = if  builtin.after-phase1  then
    2 = if  builtin.after-phase2  then ;

\ character slot `cs` enters this room: its entering script, run as itself
: char-enter ( cs -- )
    dup character char.room sl@ ev-room <> if  drop exit  then
    $26 state-flag? if  ['] builtin.char-enter-26  else  ev-room 6 room-script @  then
    ?dup 0= if  drop exit  then
    phase-context
    swap dup event-state ev.self-char l!  character char.id sl@ event-state ev.self-id l!
    execute ;

\ the action scripts, one turn each: a character's script is dropped once the character is no
\ longer doing its scripted action (and the message it opened goes with it)
: dropped? ( slot-addr -- flag )
    slot.who sl@ dup 0< if  drop false exit  then
    character char.scripted sl@ 0= ;
: drop-script ( k -- )
    dup script-slot slot.who sl@ event-state ev.message-owner sl@ = if
        -1 event-state ev.message l!  -1 event-state ev.message-owner l!
    then  free-slot ;
: turn ( k -- )
    dup script-slot >r
    r@ slot.task sl@ 0= if  rdrop drop exit  then
    r@ dropped? if  rdrop drop-script exit  then
    1 r@ slot.frames +l!
    dup event-state ev.slot l!
    r@ slot.who sl@ event-state ev.self-char l!  r@ slot.id sl@ event-state ev.self-id l!
    r@ slot.task sl@ resume 0= if
        r@ slot.task sl@ if  free-slot  else  drop  then   \ (it ended on its own)
    else  drop  then
    rdrop ;
: run-slots ( -- )  script-slots 0 do  i turn  loop  phase-context ;

\ ---- what the game calls ----------------------------------------------------------------------

\ the start of play (SceneGame_RoomIn): the camera director anew, following Fiona
: start-play ( -- )  director-new-room  0 camera-on  director-room-start ;
\ the characters in the scene go into this room with whoever took the exit (SceneGame_EnterRoom,
\ then the room's camera taken and the camera on whoever it follows)
: enter-room ( room exit -- )
    event-state ev.exit l!  event-state ev.room l!
    characters 0 do  i character char.present sl@ if  ev-room i character char.room l!  then  loop
    0 run-phase
    characters 0 do  i character char.present sl@ if  i char-enter  then  loop
    director-room-start  event-state ev.camera-char sl@ camera-on
    3 run-phase  director-update ;
: leave-room ( -- )  4 run-phase  5 run-phase ;
\ a frame (SceneGame's update): the camera eases, phases 1 and 2, the action scripts, the
\ camera takes the followed character's setup, phase 3, the camera placed
: run-frame ( -- )
    director-ease  1 run-phase  2 run-phase  run-slots
    camera-frame director-track  3 run-phase  director-update ;

\ for the console: what is running
: .slots ( -- )
    script-slots 0 do
        i script-slot slot.task sl@ if
            ." slot " i .  ." task " i script-slot slot.task sl@ .
            ." who " i script-slot slot.who sl@ .  ." id " i script-slot slot.id sl@ hex . decimal
            ." frames " i script-slot slot.frames sl@ . cr
        then
    loop ;
