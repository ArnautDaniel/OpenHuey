\ rooms.fs - the room being played, and going from room to room (docs/subsystems/rooms.md).
\ The change runs over delivery rounds, so each step's messages are handled before the next
\ step: leaving (the old room still in), loading, arriving, entering.
IN: rooms
USING: engine actors messages room-names ;

message rooms-load ( room exit -- )    \ (the rooms actor's own steps)
message rooms-enter ( room exit -- )
\ a forced exit ($80 and up: the scripts' cmd_exit with bit 7) goes to the room loaded ahead
\ into the spare slot (SceneGame_LeaveRoom), arriving by that same number
$80 constant forced-exit
: forced? ( exit -- flag )  forced-exit and 0<> ;

state: rooms-state
  cell field played        \ the room (-1: none yet)
  cell field came-by       \ the exit it was entered by (-1: none)
  cell field spare         \ the room loaded ahead (room-preload, exit-prepare; -1 none): a forced exit's
end-state

: leaving ( exit -- )   \ leaving-room, unless no room is played yet
    played @ 0< if  drop exit  then  played @ swap broadcast leaving-room ;

behaviour rooming
  on spawned ( -- )  -1 played !  -1 came-by !  -1 spare ! ;
  \ (the spare slot: SceneGame_LoadSpareRoom, the scripts' 0x3A; SceneGame_PrepareExit, the room
  \ behind an exit. Not kept: the room left - the original's slots swap - so a forced exit with
  \ nothing loaded ahead in the room still takes exit 0's door)
  on room-preload ( room -- )  spare ! ;
  on exit-prepare ( exit -- )  played @ swap room-exit-leads drop  dup 0< if  drop exit  then  spare ! ;
  on go-through ( exit -- )   \ the room behind it, arriving by the exit on its side
      dup forced? spare @ 0< 0= and if  dup leaving  spare @ swap  self send rooms-load exit  then
      dup forced? if  $7F and  then
      played @ over room-exit-leads                         ( exit room' exit' )
      over 0< if  2drop drop exit  then
      rot leaving  self send rooms-load ;
  on go-to-room ( room exit -- )  -1 leaving  self send rooms-load ;
  on rooms-load ( room exit -- )
      over room-exists? 0= if  ." go-to-room: no room " swap . drop cr exit  then
      over room  over played !  dup came-by !  -1 spare !
      2dup broadcast arrived  self send rooms-enter ;
  on rooms-enter ( room exit -- )  broadcast entered-room ;
end-behaviour

: rooms-spawn ( -- id )  rooming rooms-state s" rooms" spawn ;
