\ story/room.fs - a room as an actor (docs/subsystems/story.md): the played room's event
\ scripts run as it. Entering: phase 0 (the event state reset), each character's entering
\ script, phase 3. Each frame (after the characters): phase 1 and the shared after-phase-1
\ script, phase 2 and after-phase-2, then the action scripts in their slots in order - each a
\ coroutine given one turn; at the frame's end phase 3, and an exit asked for is taken. Leaving:
\ phase 4; once the next room is in, phase 5 (src/game/event.c Events_RunPhase,
\ Events_CharEnter, Events_RunCharScripts; the order: src/game/scene_game.c).
\ Under state flag 0x26 only phase 3 runs, and a shared script for characters entering.
IN: story.room
USING: engine game-state actors messages common room-names story.state story.words story.shared ;

: phase-context ( -- )  -1 ctx-slot !  -1 ctx-who !  $FF ctx-id !  0 phase-frames ! ;
: run ( xt | 0 -- )  ?dup if  execute  then ;

\ phase 0: the room is entered (marked visited, the event state anew)
: entering ( -- )
    this-room @ progress pr.visited bit-on
    vars #vars cells 0 fill  0 ebits !  0 counter !  0 room-frames !  0 result !
    free-slots  -1 msg !  -1 msg-owner !  0 leaving !  -1 going !
    #chars 0 do  0 i cells char-scripted + !  loop ;
: phase ( n -- )
    phase-context
    dup 0 = if  entering  then
    dup 1 = if  1 room-frames +!  then
    dup 5 = if  $22 state-flag-clear  then
    $26 state-flag? over 3 <> and 0= if  dup this-room @ swap room-script @ run  then
    dup 1 = if  shared.after-phase1  then
    2 = if  shared.after-phase2  then ;

\ character slot `cs` enters this room: its entering script, run as itself
: char-enter ( cs -- )
    dup char-here 0= if  drop exit  then
    $26 state-flag? if  ['] shared.char-enter-26  else  this-room @ 6 room-script @  then
    ?dup 0= if  drop exit  then
    phase-context  swap dup ctx-who !  ctx-id !  execute ;

\ the action scripts, a turn each: a character's goes once the character isn't in its scripted
\ action any more (and the message it opened with it)
: dropped? ( k -- flag )
    cells slot-who + @ dup 0< if  drop false exit  then  cells char-scripted + @ 0= ;
: drop-script ( k -- )
    dup cells slot-who + @ msg-owner @ = if  -1 msg !  -1 msg-owner !  then  free-slot ;
: turn ( k -- )
    dup slot-task @ 0= if  drop exit  then
    dup dropped? if  drop-script exit  then
    1 over cells slot-frames + +!
    dup ctx-slot !  dup cells slot-who + @ ctx-who !  dup cells slot-ids + @ ctx-id !
    dup slot-task @ resume 0= if  dup slot-task @ if  free-slot  else  drop  then   \ (it ended)
    else  drop  then ;
: run-slots ( -- )  #slots 0 do  i turn  loop  phase-context ;

: rooms-id ( -- id )  s" rooms" actor-named ;
: story-id ( -- id )  s" story" actor-named ;

behaviour playing-room
  on room-enter ( room exit -- )
      dup came-by !  exit-taken !  this-room !  $FF camera-char !
      self subscribe tick  self subscribe frame-end
      self subscribe danger  self subscribe panic  self subscribe fiona-doing  self subscribe hewie-doing
      0 phase
      #chars 0 do  i char-enter  loop
      remember-chars
      3 phase ;
  on tick ( -- )  1 phase  2 phase  run-slots ;
  on frame-end ( -- )
      3 phase  remember-chars
      going @ 0< 0= if  going @  -1 going !  -1 leaving !  rooms-id send go-through  then ;
  on room-leave ( -- )  4 phase ;
  on room-left ( -- )  5 phase  free-slots  story-id send room-done ;
  on story-stop ( -- )  free-slots  story-id send room-done ;
  on danger ( level -- )  the-danger ! ;
  on panic ( stage level -- )  drop the-panic ! ;
  on fiona-doing ( mode sub cond cmd -- )  2drop  fiona-sub !  fiona-mode ! ;
  on hewie-doing ( here action mode sub cond mood group -- )  2drop 2drop drop  hewie-act !  hewie-here ! ;
end-behaviour

\ a room's actor, named for the room
: room-spawn ( room -- id )  playing-room room-state rot room-name spawn ;
