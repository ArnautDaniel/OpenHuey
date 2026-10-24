\ obstacles.fs - the things in a room that can be pushed about (docs/subsystems/obstacles.md;
\ src/game/room_map.c Obstacles_* / Obstacle_*, the room manager's +0x9380). Their geometry -
\ the squares of the pushable floor each covers, the floor blocked under it and marked round it,
\ its move - is the engine's (game/obstacles.c: obstacle-* words), a fact anyone may ask
\ about (Fiona's push, the story). This actor places them, pushes them (`push-obstacle`: two
\ squares along over the 30 frames of its move), plays their scrape and noise, keeps each
\ model - the room's object "oshi0n" - where it stands, and keeps the places and spots the
\ story saves.
IN: obstacles
USING: engine game-state actors messages vectors stalker-names ;

5 constant #obstacles
: valid? ( i -- flag )  0 #obstacles within ;

\ can it be pushed along (dx, dz) (Obstacles_CanPush, Obstacle_RingClear): the other characters in
\ the room - Hewie, the stalker, the story's figures not hidden (+0x29) - not on the ring round it
\ one or two squares along
create others 5 cells allot
create figures-hidden 3 cells allot   figures-hidden 3 cells 0 fill   \ (cast3..cast5, as they tell: figure-shown)
: figure-name ( k -- addr len )  case  0 of  s" cast3"  endof  1 of  s" cast4"  endof  >r s" cast5" r>  endcase ;
: hidden-figure? ( id -- flag )
    3 0 do  i figure-name actor-named over = if  drop i cells figures-hidden + @ unloop exit  then  loop  drop false ;
: other-tri ( n id -- n' )   \ (its triangle added, if it is in the room)
    {: n id :}
    id 0< 0= if  id body? if  id body-room room-id = id hidden-figure? 0= and if
        id body-tri  others n cells + !  n 1+ exit
    then  then  then  n ;
: can-push? ( i -- flag ) ( F: dx dz -- )
    0  s" hewie" actor-named other-tri  stalker-in-game other-tri
    3 0 do  i figure-name actor-named other-tri  loop
    others swap obstacle-can-push? ;
: figure-hidden! ( id hidden -- )   \ (the obstacles' own: kept as each figure tells)
    {: id h :}  3 0 do  i figure-name actor-named id = if  h i cells figures-hidden + !  then  loop ;

<PRIVATE
\ ---- its model: the room's object "oshi0n" ----
create model-name 8 allot
: model$ ( n -- addr len )  s" oshi00" model-name swap move  [char] 0 + model-name 5 + c!  model-name 6 ;
: model? ( n -- flag )
    model$  placed-count 0 ?do
        i placed-info drop fdrop fdrop fdrop  2over compare 0= if  2drop true unloop exit  then
    loop  2drop false ;
$1F constant scrape-noise   \ (heard by the stalker: at its square, as hers)

state: obstacles-state
  #obstacles cells field models          \ each one's model (n of "oshi0n")
  #obstacles 2 * cells field kept-squares   \ the places the story keeps (a, b)
  #obstacles /vec * field spots          \ the spots it keeps (where each stood)
end-state
: model ( i -- addr )  cells models + ;
: kept ( i -- addr )  2 * cells kept-squares + ;
: keep-square ( a b i -- )  kept tuck cell+ !  ! ;
: kept@ ( i -- a b )  kept dup @  swap cell+ @ ;
: spot ( i -- v )  /vec * spots + ;
: follow ( i -- )  dup model @ model$  rot obstacle-at  placed-at ;   \ (Obstacles_ModelsFollow)
: place ( i n kind a b -- )   \ (obstacle_make: its model must be in the room)
    {: i n kind a b :}  n model? 0= if  exit  then
    i kind a b obstacle-make if  n i model !  i follow  then ;
create was-at /vec allot
: scrape ( a -- )   \ (Obstacle_MoveFrame's sixth frame: its sound, its noise - where it was, before the frame's step)
    {: a :}  0 6 was-at vec@ bank-sound-at
    scrape-noise room-id a -1 fiona-noise  s" acoustics" actor-named send noise ;
: move-frame ( i -- )
    {: i :}  i obstacle-middle was-at vec!  i obstacle-square drop  {: a :}
    i obstacle-step if  a scrape  then  i follow ;

behaviour pushables
  on spawned ( -- )  self subscribe tick  self subscribe leaving-room  self subscribe figure-shown  obstacles-reset ;
  on figure-shown ( on -- )  sender swap 0= figure-hidden! ;
  on leaving-room ( room exit -- )  2drop  obstacles-reset ;
  on tick ( -- )  #obstacles 0 do  i obstacle-placed? if  i move-frame  then  loop ;
  on place-obstacle ( i n kind a b -- )  4 pick valid? if  place  else  2drop 2drop drop  then ;
  on place-obstacle-kept ( i n kind -- )
      {: i n kind :}  i valid? if  i n kind  i kept@  place  then ;
  on stop-obstacle ( i -- )  obstacle-stop ;
  on keep-obstacle ( i -- )  dup valid? if  dup obstacle-square rot keep-square  else  drop  then ;
  on keep-obstacle-at ( i a b -- )  {: i a b :}  i valid? if  a b i keep-square  then ;
  on keep-obstacle-spot ( i -- )  dup valid? if  dup obstacle-at spot vec!  else  drop  then ;
  on obstacle-model-to-spot ( i n -- )   \ (Obstacles_ModelBack: that model at its kept spot, its model)
      {: i n :}  i valid? n model? and if  n i model !  n model$  i spot vec@  placed-at  then ;
  on push-obstacle ( i dx dz -- )  {: i dx dz :}  i obstacle-placed? if  dx cell>f dz cell>f i obstacle-push  then ;
  on obstacle-moved ( i -- )  engine:obstacle-moved ;   \ (the message has the C word's name)
end-behaviour

PRIVATE>
: obstacles-spawn ( -- id )  pushables obstacles-state s" obstacles" spawn ;
