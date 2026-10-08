\ rooms.fs - the room being played, and going from room to room (docs/subsystems/rooms.md).
\ The change runs over delivery rounds, so each step's messages are handled before the next
\ step: leaving (the old room still in), loading, arriving, entering.
IN: rooms
USING: engine actors messages room-names ;

message rooms-load ( room exit -- )    \ (the rooms actor's own steps)
message rooms-enter ( room exit -- )

state: rooms-state
  cell field played        \ the room (-1: none yet)
  cell field came-by       \ the exit it was entered by (-1: none)
end-state

: leaving ( exit -- )   \ leaving-room, unless no room is played yet
    played @ 0< if  drop exit  then  played @ swap broadcast leaving-room ;

behaviour rooming
  on spawned ( -- )  -1 played !  -1 came-by ! ;
  on go-through ( exit -- )   \ the room behind it, arriving by the exit on its side
      played @ over room-exit-leads                         ( exit room' exit' )
      over 0< if  2drop drop exit  then
      rot leaving  self send rooms-load ;
  on go-to-room ( room exit -- )  -1 leaving  self send rooms-load ;
  on rooms-load ( room exit -- )
      over room-exists? 0= if  ." go-to-room: no room " swap . drop cr exit  then
      over room  over played !  dup came-by !
      2dup broadcast arrived  self send rooms-enter ;
  on rooms-enter ( room exit -- )  broadcast entered-room ;
end-behaviour

: rooms-spawn ( -- id )  rooming rooms-state s" rooms" spawn ;
