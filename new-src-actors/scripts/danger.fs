\ danger.fs - calm, followed, chased (docs/subsystems/danger.md; the original's game mode,
\ SceneGame_Danger). Each frame it collects signals, and at the frame's end decides.
IN: danger
USING: engine actors messages facts flag-names ;

state: danger-state
  cell field level        \ calm, followed, chased
  cell field prev         \ the level before the last change
  cell field last         \ last frame's
  cell field hold         \ frames before it may change again
  cell field signals      \ this frame's (a bit each)
  cell field hunted-next  \ (struck while calm: hunted next frame - the quirk)
  cell field s-here  cell field s-room  cell field s-mode  cell field s-down   \ this frame's stalker
  cell field creature-here   \ a creature up in her room (told since the last decision: creature-doing)
end-state

\ the holds (frames)
30 constant short-hold   150 constant mid-hold   450 constant long-hold

: signal? ( signal -- flag )  1 swap lshift signals @ and 0<> ;
: signal ( signal -- )  1 swap lshift signals @ or signals ! ;
: become-level ( level hold -- )  hold !  level ! ;
: creature? ( -- flag )  creature-here @ ;   \ (any of the creatures up - IsUp - in her room)
: stalker-about? ( -- flag )  no-stalker signal? 0= ;
: hunted? ( -- flag )  hunted state-flag? ;
\ (as the original: any exit of her room not leading to his room counts as near)
: near? ( -- flag )
    s-room @ room-id = if  false exit  then
    8 0 do  room-id i room-exit-leads drop s-room @ <> if  true unloop exit  then  loop  false ;
\ he waits about or is held off next door: no longer after her
: lying-low? ( -- flag )  s-mode @ dup waiting-about = swap held-off = or ;
: from-calm ( -- )
    hunted? if  followed long-hold become-level exit  then
    hold @ 0= if
        stalker-about? creature? or being-hunted signal? or if  followed long-hold become-level  then
    then ;
: from-followed ( -- )
    being-hunted signal? hunted? or if  hold @ mid-hold + long-hold min hold !  then
    stalker-about? 0= creature? 0= and if
        hold @ 0= if  calm short-hold become-level  then
    else prev @ calm = if  long-hold hold !
    else prev @ chased = if  mid-hold hold !  then then then ;
: from-chased ( -- )
    stalker-about? 0= if  hunted? creature? or if  followed  else  calm  then  short-hold become-level exit  then
    lying-low? near? and if  hold @ 0= if  followed short-hold become-level  then
    else  mid-hold hold !  then ;
: by-rooms ( -- )
    s-here @ s-room @ room-id = and if  chased short-hold become-level exit  then
    level @ case
        calm of  from-calm  endof
        followed of  from-followed  endof
        chased of  from-chased  endof
    endcase
    hold @ if  -1 hold +!  then ;
: decide ( -- )
    hunted-next @ if  being-hunted signal  0 hunted-next !  then
    s-here @ 0= if  no-stalker signal  then
    s-here @ s-down @ and if  stalker-down signal  then
    level @ last !
    force-calm state-flag? if  calm 0 become-level
    else force-chased state-flag? if  chased 0 become-level
    else force-followed state-flag? if  followed 0 become-level
    else  by-rooms  then then then
    last @ level @ <> if  last @ prev !  level @ broadcast danger  then
    fiona-struck signal? level @ calm = and hunted-next !
    0 signals !  0 s-here !  0 creature-here ! ;

behaviour judging
  on spawned ( -- )  self subscribe frame-end  self subscribe entered-room  self subscribe creature-doing ;
  on creature-doing ( slot room mode up -- )  {: slot room mode up :}  room room-id = up and if  -1 creature-here !  then ;
  on danger-signal ( signal -- )  signal ;
  on stalker-here ( room mode down -- )  s-down !  s-mode !  s-room !  -1 s-here ! ;
  on entered-room ( room exit -- )  2drop  level @ broadcast danger ;
  on frame-end ( -- )  decide ;
end-behaviour

: danger-spawn ( -- id )  judging danger-state s" danger" spawn ;
