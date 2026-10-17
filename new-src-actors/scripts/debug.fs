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
\ a jump (`room!`) - nothing the original can do - leaves her without a camera setup where the
\ room's scripts give none by an exit: she gets the room's set nearest her (room-label, below)
variable jumped
: room! ( room -- )  fiona-id -1 -1 cam-char-setup  -1 rooms-id send go-to-room  -1 jumped ! ;   \ (hers from the room alone)
: room-by! ( room exit -- )  rooms-id send go-to-room ;
: nearest-set ( -- set )   \ (the one looking nearest her, along the floor)
    fiona-id body-pos fswap fdrop  {: F: x z | best F: best-d :}  -1 to best  1e30 to best-d
    room-cameras 0 ?do
        i room-camera  {: F: ex ey ez fov lx ly lz :}
        lx x f- fdup f*  lz z f- fdup f* f+  fdup best-d f< if  to best-d  i to best  else  fdrop  then
    loop  best ;
: jump-camera ( -- )
    jumped @ 0= if  exit  then  0 jumped !
    fiona-id cam-setup-of drop 0< 0= if  exit  then
    fiona-id nearest-set -1 cam-char-setup  fiona-id cam-on ;
: tp-facing ( F: x y z deg -- )
    deg>rad  xyzw>cells  fiona-id send place  deliver ;
: tp ( F: x y z -- )  fiona-id body-yaw  180e f* pi f/  tp-facing ;   \ (her heading kept)
: tp-area-facing ( area deg -- )   \ (its middle)
    swap area-middle 0= if  drop fdrop fdrop fdrop ." no such area" cr exit  then  s>f tp-facing ;
: tp-area ( area -- )  area-middle 0= if  fdrop fdrop fdrop ." no such area" cr exit  then  tp ;
: tp-exit ( exit -- )  exit-area tp-area ;
: tp-tri ( tri -- )  tri-center tp ;
: walk ( F: x y z -- )   \ (over the nav mesh, then she stands)
    xyz>cells  fiona-id send go-to ;
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
\ the stalker: Debilitas into Fiona's room on a triangle (her own: `fiona-id body-tri debilitas-tri`), or out
: debilitas-id ( -- id )  s" debilitas" actor-named ;
: debilitas-tri ( tri -- )  room-id swap debilitas-id send stalker-in ;
: debilitas-out ( -- )  debilitas-id send stalker-out ;
\ ... or out of sight in another room, heading for hers (`castle-1f-10 debilitas-hunt`)
: debilitas-hunt ( room -- )  0 debilitas-id send stalker-in  debilitas-id send stalker-hunt ;

\ the room's name and number top right, every frame (to say where something happened): `label-off`, `label-on`
8 constant label-layer
variable label?  -1 label? !
: label-on ( -- )  -1 label? ! ;
: label-off ( -- )  0 label? !  label-layer ui-clear ;
: room-label ( -- )
    2 pen-scale
    label-layer  room-id room-name  screen-size drop  room-id room-name nip 7 + char-size drop * -  12  $E8E0C8FF 2 ui-text
    label-layer  room-id h>s  screen-size drop 5 char-size drop * -  12  $A09880FF 2 ui-text ;

\ ---- the debug mode (F2, or `debug-on` / `debug-off` at the console), on the same layer:
\ Fiona's meters top left - her panic (0..100) and its stage, her fear (0..99) and, while she is
\ out of breath, the running she has left, and hga's version; down the right, the actors alive with their ids
\ (`<id> enter` at the console works inside one, `leave-actor` back) ----
variable debug?
: debug-on ( -- )  -1 debug? ! ;
: debug-off ( -- )  0 debug? ! ;
$E8E0C8FF constant text-colour   $A09880FF constant dim-colour
: say ( addr len x y colour -- )  >r >r >r  label-layer -rot r> r> r> 2 ui-text ;
360 constant most-run   \ (the longest out-of-breath run: stage 2's 240, and up to 4 x 30)
200 constant bar-width
\ a labelled bar at y: `filled` of bar-width in `colour`; after it the value (-1: none) and a note
: bar ( value filled colour y addr len -- )
    {: value filled colour y a n :}
    a n 12 y text-colour say
    label-layer 110 y bar-width 4 + 20 $000000A0 ui-rect
    label-layer 112 y 2 + filled 0 max bar-width min 16 colour ui-rect
    value 0< 0= if  value n>s 124 bar-width + y text-colour say  then ;
: note ( addr len y -- )  >r  190 bar-width + r> dim-colour say ;
: of-bar ( n most -- n filled )  over bar-width * swap / ;
: stage$ ( stage -- addr len )
    case  composed of s" calm" endof  panicking of s" PANIC" endof  calming-down of s" PANIC (calming)" endof
          >r s" uneasy" r>  endcase ;
\ the panic's (its broadcast) and hers (asked each frame, her answer by the next)
variable m-stage  variable m-level  variable m-fear  variable m-breathless  variable m-run
: meters ( -- )
    fiona-id dup 0< if  drop exit  then  dup body? 0= if  drop exit  then  send meters?
    m-level @ 100 of-bar  $D04040FF 40 s" panic" bar   m-stage @ stage$ 40 note
    m-fear @ 99 of-bar  $D0A040FF 66 s" fear" bar
    m-breathless @ if
        m-run @ 0 max most-run of-bar  $40A0D0FF 92 s" breath" bar   s" frames of running left" 92 note
    else  -1 bar-width $40A0D0FF 92 s" breath" bar   s" full" 92 note  then ;
\ the actors alive, two columns down the right from under the room's name
: actor-line ( id x y -- )
    {: id x y :}  id n>s x y dim-colour say  id actor-name x 44 + y text-colour say ;
256 constant most-actors   \ (the actors' table: hactor.c)
: actors-list ( -- )
    {: | n :}
    most-actors 0 do
        i alive? if
            i  screen-size drop 420 -  n 1 and 210 * +  n 2/ 22 * 44 +  actor-line  n 1+ to n
        then
    loop ;

\ which hga this is, under the meters
: version-line ( -- )  s" hga " 12 118 dim-colour say  hga-version 56 118 dim-colour say ;

state: label-state  cell field label-unused  end-state
behaviour labelling
  on spawned ( -- )  self subscribe frame-end  self subscribe panic  self subscribe entered-room ;
  on entered-room ( room exit -- )  2drop jump-camera ;
  on panic ( stage level -- )  cell>f f>s m-level !  m-stage ! ;   \ (its level a float, as a cell)
  on her-meters ( fear breathless run -- )  m-run !  m-breathless !  m-fear ! ;
  on frame-end ( -- )
      label-layer ui-clear
      key: F2 key-pressed? if  debug? @ 0= debug? !  then
      debug? @ if  meters  actors-list  version-line  then   \ (asked for: anywhere)
      room-id 0< in-play state-flag? 0= or if  exit  then   \ (the label: not over the title)
      label? @ if  room-label  then ;
end-behaviour
: label-spawn ( -- id )  labelling label-state s" room-label" spawn ;
