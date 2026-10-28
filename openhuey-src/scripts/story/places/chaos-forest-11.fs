\ story/places/chaos-forest-11.fs - the Chaos Forest's crossing ($109): five ways; a rabbit now and
\ then, off into the trees; the wisp (story/places/forest.fs) startled as she or Hewie comes near
\ it; the maze dealt. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/chaos-forest-11.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.chaos-forest-11
USING: engine game-state actors messages common facts room-names flag-names story-names story.kit
    story.places.forest ;

own-reset  forest-owns owns !

$19 constant forest-track
create entries  0 , 0 ,  3 , 3 ,  1 , 1 ,  2 , 2 ,  2 , 2 ,   \ (by exit 0 .. 4: the camera)
create cameras  $A , 3 , 3 ,  $B , 1 , 1 ,  $C , 3 , 3 ,  $D , 2 , 2 ,  $E , 0 , 0 ,  $F , 3 , 3 ,   \ (area, set, path)
create ahead  5 , 0 ,  6 , 1 ,  7 , 2 ,  8 , 3 ,  9 , 4 ,   \ (area, exit loaded ahead)
: rabbit ( -- )  1 180 3e 235e -6e 257e runs-off ;
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background  maze-in
    30 chance? if  rabbit-kind rabbit-slot figure-in  ['] rabbit start drop  then
    cf-11-flag1 fact? 0= if  -21.17e 31.84e -137.07e 1e wisp-in  then ;
: near-wisp? ( id -- flag )  -24e -132e 25e reaches-across? ;
: each-frame ( -- )
    5 0 do  i i leaves-by  loop  cameras 6 cameras-at  ahead 5 ways-out  maze-frame
    wisp-up @  cf-11-flag1 fact? 0= and  fiona near-wisp?  hewie near-wisp? or and if  cf-11-flag1 startled  then ;
: person-in ( i -- )  fiona-p = if  entries 5 ways-in  then ;
: went ( -- )  rabbit-slot figure-out  wisp-slot figure-out ;

' came-in ' person-in ' each-frame ' went chaos-forest-11 place!
