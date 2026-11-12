\ item-props: each item lying in the room shown as a model where it lies, turning slowly, instead
\ of its sparkle; gone when it is taken (or the room is left). She takes it as ever - her action
\ button by it - or a mod may take it for her (take-item-at, as a VR hand would).
\
\ The models: library/data/PROPS/item-<id>.gltf for the item of that id (decimal, items.names:
\ medallion 39 ...), else library/data/PROPS/item.gltf - any glTF or .glb, Y up, one unit a game
\ unit (Fiona stands about 15 tall). This sample has only the one: a small gold cube.
IN: item-props
USING: engine actors messages mods ;

32 constant #spots   \ (a place's effects: one item at each at most)
0.02e fconstant turn-rate   \ (radians a tick)

state: props-state
  #spots cells field shown       \ the prop shown at each of the place's effects (-1 none)
  1 floats field turned          \ how far they have turned
end-state

: spot ( k -- addr )  cells shown + ;
create path 64 allot  variable path-len
: add ( addr len -- )  path path-len @ + swap  dup path-len +!  move ;
: digits ( n -- )  dup 10 >= if  dup 10 / recurse  then  10 mod [char] 0 +  path path-len @ + c!  1 path-len +! ;
: model ( item -- addr len )   \ its own model if the mod has one, else the one for all
    0 path-len !  s" PROPS/item-" add  digits  s" .gltf" add
    path path-len @  2dup data-file? if  exit  then  2drop s" PROPS/item.gltf" ;
: gone ( k -- )  spot dup @ dup 0< if  2drop exit  then  prop-free  -1 swap ! ;
: placed ( room k item F: x y z -- )   \ a model where it lies, a little above the floor
    {: room k item F: x y z :}  k gone
    item model prop-load dup 0< if  drop exit  then  {: id :}
    id room  x  y 1e f+  z  prop-place  id k spot ! ;

behaviour showing-items
  on spawned ( -- )
      #spots 0 do  -1 i spot !  loop
      false item-sparkles!   \ (the game's sparkles not drawn: the models instead)
      self subscribe item-placed  self subscribe item-cleared  self subscribe tick ;
  on item-placed ( room k item x y z -- )  cell>f cell>f cell>f frot frot fswap placed ;
  on item-cleared ( room k -- )  nip gone ;
  on take-answer ( k started? -- )  2drop ;   \ (taken for her: refused, the item stays where it lies - a VR hand puts its model back)
  on tick ( -- )
      turned f@ turn-rate f+ turned f!
      #spots 0 do  i spot @ dup 0< if  drop  else  0e turned f@ 0e prop-turn  then  loop ;
end-behaviour

showing-items props-state s" item-props" spawn constant item-props
