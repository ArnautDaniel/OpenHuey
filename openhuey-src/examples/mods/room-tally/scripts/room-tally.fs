\ room-tally: the rooms Fiona has been in this game, counted, and the count shown for a moment
\ each time she enters one she hadn't been in.
\
\ The sample mod: an actor of its own. It hears the game's broadcasts (entered-room, frame-end),
\ keeps its own state and draws on the mods' layer - it never touches another actor's state.
\ It answers a question too (rooms-seen?), as the game's actors do: by a message back.
IN: room-tally
USING: engine actors messages mods ;

message rooms-seen? ( -- )     \ asked: it answers the sender with rooms-seen
message rooms-seen ( n -- )

90 constant shown-for          \ frames the count stays up (3 seconds)
$1000 constant room-ids        \ (rooms are $000..$FFF)

state: tally-state
  cell field count             \ the rooms she has been in
  cell field showing           \ frames left to show it
  room-ids field seen          \ a byte a room: been there
end-state

: first-time? ( room -- flag )  $FFF and seen +  dup c@ 0=  1 rot c! ;
: show ( -- )
    mod-layer  s" rooms seen"  24 24  $E8E0C8C0  2 ui-text
    mod-layer  count @ n>s     24 11 char-size drop * +  24  $F0C060FF  2 ui-text ;

behaviour tallying
  on spawned ( -- )  self subscribe entered-room  self subscribe frame-end ;
  on entered-room ( room exit -- )
      drop first-time? if  1 count +!  shown-for showing !  then ;
  on frame-end ( -- )
      mod-layer ui-clear
      showing @ 0> if  -1 showing +!  show  then ;
  on rooms-seen? ( -- )  count @  sender send rooms-seen ;
end-behaviour

tallying tally-state s" room-tally" spawn constant room-tally
