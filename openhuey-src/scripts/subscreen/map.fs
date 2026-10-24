\ subscreen/map.fs - the map page (src/game/subscreen.c Map_*): the maps she holds (a sketch
\ used for each: the castle, the mansion, the forest, the tower, the mystery), page by page with
\ the arrows; it opens on her room's page with an arrow where she stands, turned to her heading
\ (in the forest's maze, where the dealt row puts her room). Each room as it is
\ entered is found on the maps (Map_FindRoom); the story can show it on another page (0xD1).
IN: subscreen.map
USING: engine game-state actors facts story-names ps2-screen subscreen.state subscreen.art subscreen.input subscreen.tables ;

$28 constant map-group   \ (the page's picture in the texture cache)

\ a map's rooms: room, page | x scale, z scale, x offset, y offset (where the arrow goes)
begin-structure /map-room  field: room-of-e  field: page-of-e
    ffield: x-scale  ffield: z-scale  ffield: x-off  ffield: y-off  end-structure
: room-of ( e -- room )  room-of-e @ ;
: page-of ( e -- page )  page-of-e @ ;
: next-room ( e -- e' )  /map-room + ;
: ended? ( e -- flag )  room-of 0< ;

: held-maps ( -- bits )   \ (Progress_MapsHeld)
    castle-map-held story-flag? 1 and   mansion-map-held story-flag? 2 and or
    forest-map-held story-flag? 4 and or  tower-map-held story-flag? 8 and or
    mystery-map-held story-flag? $10 and or ;
: held? ( map -- flag )  dup 0 #maps within 0= if  drop false exit  then  1 swap lshift held-maps and 0<> ;

\ ---- where she is ----
: entry-for ( map room page -- e | 0 )   \ that room's entry on that page of the map
    {: map room page :}  map map-rooms
    begin  dup ended? 0= while
        dup room-of room =  over page-of page = and if  exit  then  next-room
    repeat  drop 0 ;
: shown-on ( room map -- e | 0 )   \ the room's first entry on a page the map has
    {: room map :}  map map-rooms
    begin  dup ended? 0= while
        dup room-of room =  over page-of map map-pages < and if  exit  then  next-room
    repeat  drop 0 ;
: find-room ( room -- )   \ (Map_FindRoom: the first map and page showing it)
    {: room :}  room her-room !  -1 her-map !  -1 her-pg !
    #maps 0 do
        room i shown-on ?dup if  page-of her-pg !  i her-map !  unloop exit  then
    loop ;
: show-on-page ( page -- )   \ (Map_TurnTo: her room shown on that page of her map, if it is there)
    {: page :}  her-map @ 0< if  exit  then
    her-map @ her-room @ page entry-for if  page her-pg !  then ;
: back-to-her ( -- )  her-map @ map-at !  her-pg @ map-pg ! ;

\ ---- turning (Map_LeftRight: past the ends to the next / last map she holds, round) ----
: last-page ( -- )  map-at @ map-pages 1- map-pg ! ;
: other-map ( d -- )   \ the next map along that way she holds (or back to this one)
    {: d :}  map-at @ {: was :}
    begin  map-at @ d + #maps wrap map-at !  map-at @ was =  map-at @ held? or  until ;
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
\ the forest's maze rooms ($100..$105): placed by where the dealt row puts the room (Events_ThingPlace:
\ its place 0..2 in the row, while rooms are dealt), on one of three spots of their own
$100 constant first-maze-room   $1F constant deals-var   $20 constant dealt-row-var
: var@ ( n -- v )  progress pr.vars + c@ ;
: maze-place ( room -- i | -1 )
    {: room :}  deals-var var@ 0= if  -1 exit  then
    s" deal-rows" exe-table {: addr n :}
    3 0 do  dealt-row-var var@ 3 * i +  dup n < if  addr + c@  else  drop -1  then
        first-maze-room + room = if  i unloop exit  then
    loop  -1 ;
: arrow-entry ( -- e | 0 )
    her-map @ her-room @ her-pg @ entry-for  dup 0= if  exit  then
    her-map @ 2 =  her-room @ first-maze-room dup 6 + within and if
        drop  her-room @ maze-place  dup 0< if  drop 0 exit  then  /map-room * maze-spots +
    then ;
\ a corner (cx, cy) of the 32 x 32 arrow turned by her heading (s, c its sine and cosine), x
\ squeezed 512 / 640, in the window (the texels' top left is (-16, 16), top right (16, 16),
\ bottom left (-16, -16), bottom right (16, -16))
: corner ( cx cy -- x y ) ( F: ay ax s c -- ay ax s c )
    {: cx cy F: ay ax s c :}
    ax  cx s>f c f*  cy s>f s f* f-  512e f* 640e f/  f+ f>s sx
    ay  cy s>f c f*  cx s>f s f* f+  f+ f>s sy
    ay ax s c ;
: here-arrow ( -- )   \ the arrow where she stands, turned to her heading (Map_HereArrow)
    map-at @ her-map @ <>  map-pg @ her-pg @ <> or if  exit  then
    arrow-entry ?dup 0= if  exit  then  {: e :}
    s" fiona" actor-named {: her :}  her body-pos  fswap fdrop  {: F: x z :}
    z e z-scale f@ f/  e y-off f@ f+  128e f+                 \ (z down the page, from its line 128)
    x e x-scale f@ f/ 512e f* 640e f/  e x-off f@ f+          \ (x across, squeezed 512 / 640)
    her body-yaw fnegate  fdup fsin fswap fcos                \ ( F: ay ax s c )
    art-layer  -16 16 corner  16 16 corner  -16 -16 corner  16 -16 corner
    fdrop fdrop fdrop fdrop
    base-group 0 6  $1C0 $60 32 32  $80 faded  ui-image4 ;
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
