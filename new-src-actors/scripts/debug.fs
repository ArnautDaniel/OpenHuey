\ debug.fs - words for the console and the tests: put Fiona anywhere, change rooms, press a
\ button, see where she is. They go through the actors' own messages (place, go-to-room, ...).
\   front-garden-2 room!            to a room (its start)          front-garden-2 1 room-by!  by its exit 1
\   10e 0e 20e tp                   Fiona there, her heading kept  10e 0e 20e -90e tp-facing  facing -90 degrees
\   $18 tp-area                     into an event area's middle    $18 -90 tp-area-facing
\   2 tp-exit                       into exit 2's area             $1F2 tp-tri                on a nav triangle's middle
\   10e 0e 20e walk                 walking there (over the nav mesh)
\   circle press                    a button pressed for a frame   .here                      where she is
\   0 act                           the room's action script 0, as Fiona's (a scene: try front-garden-3's)
IN: debug
USING: engine actors messages keys room-names flag-names story.words ;

: fiona-id ( -- id )  s" fiona" actor-named ;
: rooms-id ( -- id )  s" rooms" actor-named ;
: room! ( room -- )  -1 rooms-id send go-to-room ;
: room-by! ( room exit -- )  rooms-id send go-to-room ;
: tp-facing ( F: x y z deg -- )
    deg>rad  f>cell >r  f>cell >r  f>cell >r  f>cell  r> r> r>  fiona-id send place  deliver ;
: tp ( F: x y z -- )  fiona-id body-yaw  180e f* pi f/  tp-facing ;   \ (her heading kept)
create spot 12 allot
: tp-area-facing ( area deg -- )   \ (its middle)
    swap area-middle 0= if  drop fdrop fdrop fdrop ." no such area" cr exit  then  s>f tp-facing ;
: tp-area ( area -- )  area-middle 0= if  fdrop fdrop fdrop ." no such area" cr exit  then  tp ;
: tp-exit ( exit -- )  exit-area tp-area ;
: tp-tri ( tri -- )  tri-center tp ;
: walk ( F: x y z -- )   \ (over the nav mesh, then she stands)
    f>cell >r  f>cell >r  f>cell  r> r>  fiona-id send go-to ;
: press ( button -- )   \ (pressed for a frame: its key down, then up)
    button-keys drop  dup true key-hold  game-tick  false key-hold ;
: .here ( -- )
    ." room " room-id room-name type  ."  at " fiona-id body-pos  frot f. fswap f. f.
    ." tri " fiona-id body-tri .  ." areas:"
    area-count 0 ?do  fiona-id body-pos i area-in? if  space i .  then  loop  cr ;
: act ( n -- )   \ (the world let go first: the new game holds it until the opening has played)
    world-held state-flag-clear
    room-id room-name actor-named dup 0< if  2drop ." no room actor" cr exit  then
    enter  0 0 rot action  leave-actor ;
\ the stalker: Debilitas into Fiona's room on a triangle (her own: `0 debilitas-tri`), or out
: debilitas-id ( -- id )  s" debilitas" actor-named ;
: debilitas-tri ( tri -- )  room-id swap debilitas-id send stalker-in ;
: debilitas-out ( -- )  debilitas-id send stalker-out ;

\ the room's name and number top right, every frame (to say where something happened): `label-off`, `label-on`
8 constant label-layer
variable label?  -1 label? !
: label-on ( -- )  -1 label? ! ;
: label-off ( -- )  0 label? !  label-layer ui-clear ;
state: label-state  cell field label-unused  end-state
behaviour labelling
  on spawned ( -- )  self subscribe frame-end ;
  on frame-end ( -- )
      label-layer ui-clear
      label? @ 0= room-id 0< or if  exit  then
      2 pen-scale
      label-layer  room-id room-name  screen-size drop  room-id room-name nip 7 + char-size drop * -  12  $E8E0C8FF 2 ui-text
      label-layer  room-id h>s  screen-size drop 5 char-size drop * -  12  $A09880FF 2 ui-text ;
end-behaviour
: label-spawn ( -- id )  labelling label-state s" room-label" spawn ;
