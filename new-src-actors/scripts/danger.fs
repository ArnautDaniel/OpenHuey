\ danger.fs - calm, followed, chased (docs/subsystems/danger.md; the original's game mode,
\ SceneGame_Danger). Each frame it collects signals, and at the frame's end decides.
IN: danger
USING: engine actors messages facts ;

state: danger-state
  cell field level        \ 0 calm, 1 followed, 2 chased
  cell field prev         \ the level before the last change
  cell field last         \ last frame's
  cell field hold         \ frames before it may change again
  cell field signals      \ this frame's (a bit each)
  cell field hunted-next  \ (struck while calm: hunted next frame - the quirk)
  cell field s-here  cell field s-room  cell field s-alert  cell field s-chasing   \ this frame's stalker
end-state

: signal? ( bit -- flag )  1 swap lshift signals @ and 0<> ;
: signal ( bit -- )  1 swap lshift signals @ or signals ! ;
: set-level ( level frames -- )  hold !  level ! ;
: creature? ( -- flag )  false ;   \ (creatures: later)
\ (as the original: any exit of her room not leading to his room counts as near)
: near? ( -- flag )
    s-room @ room-id = if  false exit  then
    8 0 do  room-id i room-exit-leads drop s-room @ <> if  true unloop exit  then  loop  false ;
: from-calm ( -- )
    $22 state-flag? if  1 450 set-level exit  then
    hold @ 0= if  2 signal? 0= creature? or 6 signal? or if  1 450 set-level  then  then ;
: from-tense ( -- )
    6 signal? $22 state-flag? or if  hold @ 150 + 450 min hold !  then
    2 signal? creature? 0= and if
        hold @ 0= if  0 30 set-level  then
    else prev @ 0= if  450 hold !
    else prev @ 2 = if  150 hold !  then then then ;
: from-chased ( -- )
    2 signal? if  $22 state-flag? creature? or 1 and 30 set-level exit  then
    s-alert @ 3 - 2 u< near? and if  hold @ 0= if  1 30 set-level  then
    else  150 hold !  then ;
: by-rooms ( -- )
    s-here @ s-room @ room-id = and if  2 30 set-level exit  then
    level @ case  0 of  from-calm  endof  1 of  from-tense  endof  2 of  from-chased  endof  endcase
    hold @ if  -1 hold +!  then ;
: decide ( -- )
    hunted-next @ if  6 signal  0 hunted-next !  then
    s-here @ 0= if  2 signal  then
    s-here @ s-chasing @ and if  3 signal  then
    level @ last !
    $1B state-flag? if  0 0 set-level
    else $1F state-flag? if  2 0 set-level
    else 7 state-flag? if  1 0 set-level
    else  by-rooms  then then then
    last @ level @ <> if  last @ prev !  level @ broadcast danger  then
    1 signal? level @ 0= and hunted-next !
    0 signals !  0 s-here ! ;

behaviour judging
  on spawned ( -- )  self subscribe frame-end  self subscribe entered-room ;
  on danger-signal ( bit -- )  signal ;
  on stalker-here ( room alert chasing -- )  s-chasing !  s-alert !  s-room !  -1 s-here ! ;
  on entered-room ( room exit -- )  2drop  level @ broadcast danger ;
  on frame-end ( -- )  decide ;
end-behaviour

: danger-spawn ( -- id )  judging danger-state s" danger" spawn ;
