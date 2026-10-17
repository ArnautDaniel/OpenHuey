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

\ can it be pushed along (dx, dz) (Obstacles_CanPush): the other characters in the room - Hewie,
\ the stalker, the story's figures - not on the ring round it one or two squares along
create others 5 cells allot
: other-tri ( n id -- n' )   \ (its triangle added, if it is in the room)
    {: n id :}
    id 0< 0= if  id body? if  id body-room room-id = if
        id body-tri  others n cells + !  n 1+ exit
    then  then  then  n ;
: can-push? ( i -- flag ) ( F: dx dz -- )
    0  s" hewie" actor-named other-tri  stalker-in-game other-tri
    s" cast3" actor-named other-tri  s" cast4" actor-named other-tri  s" cast5" actor-named other-tri
    others swap obstacle-can-push? ;

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
: keep ( a b i -- )  kept tuck cell+ !  ! ;
: kept@ ( i -- a b )  kept dup @  swap cell+ @ ;
: spot ( i -- v )  /vec * spots + ;
: follow ( i -- )  dup model @ model$  rot obstacle-at  placed-at ;   \ (Obstacles_ModelsFollow)
: place ( i n kind a b -- )   \ (obstacle_make: its model must be in the room)
    {: i n kind a b :}  n model? 0= if  exit  then
    i kind a b obstacle-make if  n i model !  i follow  then ;
: scrape ( i -- )   \ (Obstacle_MoveFrame's sixth frame: its sound, its noise)
    dup obstacle-middle 0 6 bank-sound-at
    obstacle-square drop {: a :}  scrape-noise room-id a -1 fiona-noise  s" acoustics" actor-named send noise ;
: move-frame ( i -- )  dup obstacle-step if  dup scrape  then  follow ;

behaviour pushables
  on spawned ( -- )  self subscribe tick  self subscribe leaving-room  obstacles-reset ;
  on leaving-room ( room exit -- )  2drop  obstacles-reset ;
  on tick ( -- )  #obstacles 0 do  i obstacle-placed? if  i move-frame  then  loop ;
  on place-obstacle ( i n kind a b -- )  4 pick valid? if  place  else  2drop 2drop drop  then ;
  on place-obstacle-kept ( i n kind -- )
      {: i n kind :}  i valid? if  i n kind  i kept@  place  then ;
  on stop-obstacle ( i -- )  obstacle-stop ;
  on keep-obstacle ( i -- )  dup valid? if  dup obstacle-square rot keep  else  drop  then ;
  on keep-obstacle-at ( i a b -- )  {: i a b :}  i valid? if  a b i keep  then ;
  on keep-obstacle-spot ( i -- )  dup valid? if  dup obstacle-at spot vec!  else  drop  then ;
  on obstacle-model-to-spot ( i n -- )   \ (Obstacles_ModelBack: that model at its kept spot, its model)
      {: i n :}  i valid? n model? and if  n i model !  n model$  i spot vec@  placed-at  then ;
  on push-obstacle ( i dx dz -- )  {: i dx dz :}  i obstacle-placed? if  dx cell>f dz cell>f i obstacle-push  then ;
  on obstacle-moved ( i -- )  engine:obstacle-moved ;   \ (the message has the C word's name)
end-behaviour

PRIVATE>
: obstacles-spawn ( -- id )  pushables obstacles-state s" obstacles" spawn ;
