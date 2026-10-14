\ subscreen/map.fs - the map page (src/game/subscreen.c Map_*): the maps she holds (a sketch
\ used for each: the castle, the mansion, the forest, the tower, the mystery), page by page with
\ the arrows; it opens on her room's page with an arrow where she stands. Each room as it is
\ entered is found on the maps (Map_FindRoom); the story can show it on another page (0xD1).
IN: subscreen.map
USING: engine actors facts story-names subscreen.state subscreen.art subscreen.input subscreen.tables ;

$28 constant map-group   \ (the page's picture in the texture cache)

\ a map's rooms: room, page | x scale, z scale, x offset, y offset (where the arrow goes)
2 cells 4 floats + constant /map-room
: room-of ( e -- room )  @ ;
: page-of ( e -- page )  cell+ @ ;
: scales ( e -- addr )  2 cells + ;
: next-room ( e -- e' )  /map-room + ;
: ended? ( e -- flag )  room-of 0< ;

: held-maps ( -- bits )   \ (Progress_MapsHeld)
    castle-map-held story-flag? 1 and   mansion-map-held story-flag? 2 and or
    forest-map-held story-flag? 4 and or  tower-map-held story-flag? 8 and or
    mystery-map-held story-flag? $10 and or ;
: held? ( map -- flag )  dup 0 #maps within 0= if  drop false exit  then  1 swap lshift held-maps and 0<> ;

\ ---- where she is ----
variable want-room  variable want-page
: entry-for ( map room page -- e | 0 )   \ that room's entry on that page of the map
    want-page !  want-room !  map-rooms
    begin  dup ended? 0= while
        dup room-of want-room @ =  over page-of want-page @ = and if  exit  then  next-room
    repeat  drop 0 ;
: find-room ( room -- )   \ (Map_FindRoom: the first map and page showing it)
    dup her-room !  -1 her-map !  -1 her-pg !
    #maps 0 do
        i map-rooms begin  dup ended? 0= while
            dup room-of 2 pick =  over page-of i map-pages < and if
                page-of her-pg !  i her-map !  drop unloop exit
            then  next-room
        repeat  drop
    loop  drop ;
: show-on-page ( page -- )   \ (Map_TurnTo: her room shown on that page of her map, if it is there)
    her-map @ 0< if  drop exit  then
    her-map @ her-room @ 2 pick entry-for if  her-pg !  else  drop  then ;
: back-to-her ( -- )  her-map @ map-at !  her-pg @ map-pg ! ;

\ ---- turning (Map_LeftRight: past the ends to the next / last map she holds, round) ----
: last-page ( -- )  map-at @ map-pages 1- map-pg ! ;
: other-map ( d -- )   \ the next map along that way she holds (or back to this one)
    map-at @ >r
    begin  map-at @ over + #maps wrap map-at !  map-at @ r@ =  map-at @ held? or  until
    r> 2drop ;
: first-held ( -- )  #maps 0 do  i held? if  i map-at !  unloop exit  then  loop ;
: map-left ( -- )
    map-at @ 0< if  first-held  last-page  exit  then
    map-at @ held?  map-pg @ 0> and if  -1 map-pg +!  exit  then
    -1 other-map  last-page ;
: map-right ( -- )
    map-at @ 0< if  first-held  0 map-pg !  exit  then
    map-at @ held?  map-pg @ 1+ map-at @ map-pages < and if  1 map-pg +!  exit  then
    1 other-map  0 map-pg ! ;

\ ---- drawing (Map_PageFrame, Map_HereArrow) ----
: load-page ( -- )   \ the page's picture
    map-at @ held? 0= if  exit  then
    map-at @ 16 * map-pg @ +  dup map-loaded @ = if  drop exit  then  map-loaded !
    map-at @ map-pg @ map-page drop  map-group load-bank ;
fvariable arrow-x  fvariable arrow-y
: here-arrow ( -- )   \ the arrow where she stands (not turned to her heading here)
    map-at @ her-map @ <>  map-pg @ her-pg @ <> or if  exit  then
    her-map @ her-room @ her-pg @ entry-for ?dup 0= if  exit  then  scales >r
    s" fiona" actor-named body-pos  fswap fdrop                         ( F: x z )
    r@ 1 floats + f@ f/  r@ 3 floats + f@ f+  128e f+  arrow-y f!     \ (z down the page, from its line 128)
    r@ f@ f/ 512e f* 640e f/  r> 2 floats + f@ f+  arrow-x f!          \ (x across, squeezed 512 / 640)
    arrow-x f@ f>s 16 -  arrow-y f@ f>s 16 -  32 32 picture
    base-group 0 -1  $1C0 $60 32 32  $80 from ;
: map-view ( -- )
    1 page-art  load-page
    map-at @ held? if
        0 $80 $200 $100 picture  map-group 0 -1  0 0 $200 $100  $80 from
        here-arrow
    then
    map-at @ held? if  map-at @ map-pg @ map-page nip nip  else  $85  then   \ (its title; none: "No Data")
    menu-line  $A6 $48 centred
    held-maps dup 0<>  swap 4 = map-at @ 2 = and 0= and if   \ (somewhere to turn to)
        $A $F0 $1A blink part  $1D6 $F0 $1B blink part
    then ;
: map-keys ( -- )
    held-maps if
        map-at @ map-pg @
        left? if  map-left  then
        right? if  map-right  then
        map-pg @ <> swap map-at @ <> or if  se-cursor  then
    then
    page-keys ;
