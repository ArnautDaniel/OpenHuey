\ story/room.fs - a room as an actor (docs/subsystems/story.md): the played room's event
\ scripts run as it. Entering: phase 0 (the event state reset), each character's entering
\ script, phase 3. Each frame (after the characters): phase 1 and the shared after-phase-1
\ script, phase 2 and after-phase-2, then the action scripts in their slots in order - each a
\ coroutine given one turn; at the frame's end phase 3, and an exit asked for is taken. Leaving:
\ phase 4; once the next room is in, phase 5 (src/game/event.c Events_RunPhase,
\ Events_CharEnter, Events_RunCharScripts; the order: src/game/scene_game.c).
\ Under state flag 0x26 only phase 3 runs, and a shared script for characters entering.
IN: story.room
USING: engine game-state actors messages common room-names story.state facts story.words story.shared flag-names story.clock ;

: phase-context ( -- )  -1 ctx-slot !  -1 ctx-who !  $FF ctx-id !  0 phase-frames ! ;
: run ( xt | 0 -- )  ?dup if  execute  then ;

\ phase 0: the room is entered (marked visited, the event state anew)
: entering ( -- )
    this-room @ progress pr.visited bit-set
    vars #vars cells 0 fill  0 ebits !  0 counter !  0 room-frames !  0 result !
    free-slots  -1 msg !  -1 msg-owner !  0 leaving !  -1 going !
    #chars 0 do  0 i cells char-scripted + !  -1 i cells move-done-of + !  loop
    -1 prep-scene !  -1 prepared-msg !  -1 prepared-page !  zones-off
    0 movie-kind !  -1 cue !  -1 cue-prev !  0 fading !  effects-none
    0 panic-onset-on !   \ (SceneGame_SetFlag3 0)
    $FF 0 1e bgm ;   \ (no background track wanted: the room's scripts ask again, else it fades out)
: phase ( n -- )
    phase-context
    dup 0 = if  entering  then
    dup 1 = if  1 room-frames +!  zones-off  then
    dup 5 = if  hunted state-flag-clear  then
    events-held state-flag? over 3 <> and 0= if  dup this-room @ swap room-script @ run  then
    dup 1 = if  shared.after-phase1  then
    2 = if  shared.after-phase2  then ;

\ character slot `cs` enters this room: its entering script, run as itself (Events_CharEnter) -
\ the room's entering characters, and each one coming in while it is played; the context of the
\ script running then put back
: char-enter ( cs -- )
    dup char-here 0= if  drop exit  then
    events-held state-flag? if  ['] shared.char-enter-26  else  this-room @ 6 room-script @  then
    ?dup 0= if  drop exit  then
    ctx-slot @ ctx-who @ ctx-id @ phase-frames @ {: cs xt slot who id frames :}
    phase-context  cs ctx-who !  cs cs>id ctx-id !  xt execute
    slot ctx-slot !  who ctx-who !  id ctx-id !  frames phase-frames ! ;
\ who is in this room as they last told (char-in): one not in it before and in it now has come in.
\ The original's characters run Events_CharEnter as they come into the room played: the stalker
\ through a door (Pursuer_ThroughDoor - not put there by a script: came-by -1), Hewie arriving
\ (Hewie_Vt38, arrived, Hewie_PlaceInRoom), Fiona back in view (fiona_show)
: char-in-cell ( cs -- addr )  cells char-in-room + ;
: chars-in ( -- )  #chars 0 do  i char-here i char-in-cell !  loop ;
: arrival ( cs in? -- )
    {: cs in? :}  cs char-in-cell @ 0= in? and  in? cs char-in-cell !  if  cs char-enter  then ;
\ the stalker in slot 2 as he tells it: in by a door, his entering script; put here (came-by -1), in
: stalker-told-in ( -- )
    stalker-cs char-here {: in? :}
    in? st-came @ 0< and if  -1 stalker-cs char-in-cell !  exit  then
    stalker-cs in? arrival ;

\ the action scripts, a turn each: a character's goes once the character isn't in its scripted
\ action any more (and the message it opened with it)
: dropped? ( k -- flag )
    cells slot-who + @ dup 0< if  drop false exit  then  cells char-scripted + @ 0= ;
: drop-script ( k -- )
    dup cells slot-who + @ msg-owner @ = if  $FFFF message-close  -1 msg-owner !  then  free-slot ;
: turn ( k -- )
    dup slot-task @ 0= if  drop exit  then
    dup dropped? if  drop-script exit  then
    1 over cells slot-frames + +!
    dup ctx-slot !  dup cells slot-who + @ ctx-who !  dup cells slot-ids + @ ctx-id !
    dup slot-task @ resume 0= if  dup slot-task @ if  free-slot  else  drop  then   \ (it ended)
    else  drop  then ;
: run-slots ( -- )  #slots 0 do  i turn  loop  phase-context ;

: release-all ( -- )  #chars 0 do  i release  loop ;   \ (its scripts gone: the characters free)
\ the action prepared this frame, offered to her (Progress_PlayerButtons reads her button) and
\ shown as the action prompt - not while a message is open
: offer-it ( -- )
    prep-scene @ dup 5 <> swap read-message <> and  msg @ 0< 0= or if  exit  then
    0 char-actor dup 0< if  drop exit  then  >r  prep-scene @ prep-arg @ r> send offer
    1 window-id send prompt ;

\ the camera at the start of play in the room, between its first scripts and its phase 3
\ (CamDirector_RoomStart): from room to room the one it follows taken again (Progress_CameraOn)
\ and its setup taken - placed, the phase 3 scripts seeing it changed; a fresh start (a new
\ game, a load: SceneGame_RoomIn) cuts to it on the first frame
: from-a-room? ( -- flag )  came-by @ 0< 0= ;
: start-camera ( -- )  cam-room-start  from-a-room? if  cam-followed cam-on  then ;
: shot-camera ( -- )  from-a-room? if  cam-update  then ;
behaviour playing-room
  on spawned ( -- )  effects-none ;   \ (its effects none from the start: a room left before it was entered frees nothing)
  on room-enter ( room exit -- )   \ (queued as the room came in: if another came in since, not this one's)
      over room-id <> if  2drop exit  then  -1 entered !
      dup came-by !  exit-taken !  this-room !
      self subscribe tick  self subscribe frame-end
      self subscribe danger  self subscribe panic  self subscribe panic-onset  self subscribe fiona-doing  self subscribe hewie-doing  self subscribe hewie-gesture  self subscribe moving  self subscribe fiona-started  -1 fiona-started-now !
      self subscribe stalker-doing  self subscribe stalker-ties  self subscribe stalker-blow  self subscribe item-event  self subscribe item-var  self subscribe item-event-bit  100 st-age !  -1 st-blow !
      self subscribe creature-doing  self subscribe noise-was  self subscribe summoner-cooldown-is  self subscribe fiona-struck-by  self subscribe fiona-shakes  self subscribe fiona-shaking  self subscribe hewie-health  self subscribe keri-fell  self subscribe fiona-unseen  self subscribe fiona-message  self subscribe fiona-requested  #creatures 0 do  100 i cells cr-ages + !  loop
      0 phase
      #chars 0 do  i char-enter  loop
      chars-in  remember-chars
      start-camera  3 phase  shot-camera ;
  on tick ( -- )
      clock-off   \ (drawn again by the rooms that show it, each frame)
      1 phase  -1 prep-scene !   \ (Progress_CharRequests: the actions prepared afresh)
      0 fiona-request-kind !     \ (Progress_OwnRequests: her request taken)
      2 phase  offer-it  -1 fiona-started-now !   \ (her action's start: read by phases 1 and 2)
      panic-onset-on @ 0= if  run-slots  then ;   \ (not in her panic's start: SubPlay's frame flag bit 3)
  on fiona-message ( msg -- )   \ (her locked door's script: not while Hewie is the one played)
      leaving @ msg @ 0< 0= or  hewie-controlled? or if  drop exit  then
      show-message  -1 msg-owner ! ;
  on take-offer ( scene arg -- )   \ (she took it: scene 5, her action script `arg`; or a message
      leaving @ msg @ 0< 0= or if  2drop exit  then                \ to read, looking at character 2)
      swap case
          5 of  0 0 rot action  endof
          read-message of  2 fiona-look-at  show-message  -1 msg-owner !  endof
          nip
      endcase ;
  \ an item used here (items.fs; item_event): character slot `who`'s action script `ev`, a script
  \ variable given the item first (Events_SetVar; -1 none)
  on item-var ( var value -- )  swap cells vars + ! ;
  on item-event-bit ( n on -- )  if  ebit-set  else  ebit-clear  then ;
  on item-event ( who ev var value -- )
      leaving @ if  2drop 2drop exit  then
      over 0< if  2drop  else  swap cells vars + !  then
      0 -rot action ;
  on moving ( done ended events -- )   \ (a character's move: done, its animation come round, its event keys)
      sender cs>who dup 0< if  drop 2drop drop exit  then
      tuck cells events-of + !  tuck cells anim-ended-of + !  cells move-done-of + ! ;
  on fade-done ( -- )  0 fading ! ;
  on tab-done ( -- )  0 tab-waiting ! ;
  on text-closed ( msg answer -- )  answer !  msg @ = if  -1 msg !  then ;
  on frame-end ( -- )
      st-age @ 100 < if  1 st-age +!  then
      loud-here @ if  -1 loud-here +!  then  count-arrows  fill-frame
      #creatures 0 do  i cells cr-ages +  dup @ 100 < if  1 swap +!  else  drop  then  loop
      3 phase  remember-chars  draw-scene
      going @ 0< 0= if  going @  -1 going !  -1 leaving !  rooms-id send go-through  then ;
  on room-leave ( -- )  4 phase ;
  on room-left ( -- )  entered @ if  5 phase  then  clock-off  free-slots  release-all  scene-gone  effects-off  story-id send room-done ;
  on story-stop ( -- )  free-slots  release-all  scene-gone  effects-off  story-id send room-done ;
  on danger ( level -- )  the-danger ! ;
  on panic ( stage level -- )  cell>f f>s the-panic-level !  the-panic ! ;
  on panic-onset ( on -- )  panic-onset-on ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  fiona-act !  fiona-cmd !  fiona-cond !  fiona-sub !  fiona-mode !
      entered @ if  fiona-cs dup char-here arrival  then ;
  on fiona-requested ( kind -- )  fiona-request-kind ! ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      hewie-trust-lv !  hewie-way !  hewie-breakable !  hewie-reach !  hewie-near !  hewie-cmdable !
      2drop hewie-cond !  drop  hewie-move-mode !  hewie-act !  hewie-here !
      entered @ if  hewie-cs dup char-here arrival  then ;
  on hewie-gesture ( g -- )  hewie-gesture-now ! ;
  on fiona-started ( code -- )  fiona-started-now ! ;
  on fiona-unseen ( on -- )  fiona-unseen ! ;
  on summoner-cooldown-is ( secs -- )  summoner-secs ! ;
  on fiona-struck-by ( who -- )  fiona-hit-by ! ;
  on fiona-shakes ( n -- )  fiona-shook ! ;
  on fiona-shaking ( n -- )  fiona-shakes-now ! ;
  on hewie-health ( hp -- )  hewie-hp ! ;
  on keri-fell ( k -- )  drop  1 keri-fallen +! ;
  on grab-answer ( ok -- )  grab-ok !  0 grab-asked ! ;
  on noise-was ( loud room -- )  this-room @ =  swap $20 >= and if  2 loud-here !  then ;
  on creature-doing ( slot room mode up -- )
      drop rot cells  tuck cr-modes + !  tuck cr-rooms + !  sender over cr-ids + !  0 swap cr-ages + ! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      st-came !  st-alert !  drop  st-hp-max !  st-hp !  st-cond !  st-sub !  st-mode !  st-room !  0 st-age !
      sender slot2 @ =  entered @ and if  stalker-told-in  then ;
  on stalker-ties ( scripted apart linked -- )   \ (the one in slot 2's: stalker-free?'s Progress_IsLinked)
      sender slot2 @ = if  st-linked !  2drop  else  2drop drop  then ;
  on stalker-blow ( n -- )  st-blow ! ;
end-behaviour

\ a room's actor, named for the room
: room-spawn ( room -- id )  playing-room room-state rot room-name spawn ;
