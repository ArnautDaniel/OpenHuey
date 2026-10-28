\ story/places/old-mansion-b1-7.fs - the aquarium in the old mansion's B1: three strange fish
\ swimming the pool (the first glinting), the light shaft (its haze from the far camera), her
\ remark; once all are in, Hewie's eyes on one fish then the next, 90 frames each; the Medallion
\ Hewie sniffs out; the map's pages. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-b1-7.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-b1-7
USING: engine game-state actors messages common vectors facts room-names flag-names story-names floor-flags items.names
    effects story.kit fiona.anims ;

own-reset

0 constant aquarium-msg   \ "An aquarium? Schools of strange fish glide through the water."
own: fish-in   \ how many of the fish are in
own: watching  own: watch-frames   \ the fish Hewie watches (0..2), frames to the next
0 constant medallion-fx   1 constant shaft-fx   2 constant hole-group   $A constant aquarium-area
$9000 constant fish-swim

\ ---- the fish: each in its slot, swimming its path (SwimPath) on and on ----
: swims ( slot path -- )
    {: slot k :}
    begin
        slot figure dup 0< 0= if
            dup body? if  dup body-pos  dup body-yaw  k swim-step  >r xyzw>cells r> send place  else  drop  then
        else  drop  then
        yield
    again ;
: fish-in! ( kind slot -- )   \ (loaded, swimming in place, by the pool)
    {: kind slot :}  kind slot figure-in  slot figure 0< if  yield  then
    slot figure-on  slot fish-swim 1 0 figure-play  slot 37.5e 12.5e 72.5e 0e figure-at  1 fish-in +! ;
: fish-1 ( -- )
    $14 3 fish-in!  0 2 3 7 480 25e 0e 40e swim-path  $14 cast-model dup 0< if  drop  else  effects:glint  then  3 0 swims ;
: fish-2 ( -- )  $15 4 fish-in!  1 2 3 5 240 30e 0e 50e swim-path  4 1 swims ;
: fish-3 ( -- )  $16 5 figure-in  10 frames  $16 5 fish-in!  2 2 3 5 280 30e 0e 50e swim-path  5 2 swims ;

\ ---- as she comes in ----
: medallion-shows ( -- )  omb1-7-medallion-found fact?  omb1-7-medallion fact? 0= and if  medallion-fx 8.95e 1e 11.89e flicker-at  then ;
: came-in ( exit -- )
    drop  ['] fish-1 start drop  ['] fish-2 start drop  ['] fish-3 start drop
    0 watching !  90 watch-frames !  medallion-shows  1 $2300 reverb
    effects:light-shaft shaft-fx effect!  hole-group hole-floor floor-on ;

\ ---- Hewie: his eyes on the fish in turn, calm, once all are in ----
: watches-fish ( -- )
    fish-in @ 3 <  danger-now @ calm <> or  hewie-in? 0= or if  exit  then
    -1 watch-frames +!  watch-frames @ 0= if  90 watch-frames !  watching @ 1+ 3 mod watching !  then
    watching @ 3 + figure dup 0< if  drop exit  then  dup body? 0= if  drop exit  then
    dup body-at  hewie body-at  vec-dist 100e f< if  body-pos hewie-eyes-on  else  drop  then ;

\ ---- what she may do here ----
: aquarium ( -- )  fiona wait-move  fiona rest-anim play  fiona -1 idle  aquarium-msg show  7 frames  wait-closed ;
: take-medallion ( -- )  medallion omb1-7-medallion medallion-fx 8.95e 11.89e picks-up ;
: medallion-sniffed ( -- )  omb1-7-medallion-found omb1-7-medallion medallion-fx  8.95e 1e 11.89e  hewie-finds ;

create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   6 , 1 , 1 ,   7 , 2 , 2 ,   8 , 1 , 1 ,   9 , 2 , 2 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    2 exits-out  cameras 6 cameras-at  ahead 2 ways-out
    fiona $B entered? if  1 map-page  then   fiona $B left? if  0 map-page  then
    watches-fish
    omb1-7-medallion-found fact? 0=  scrambling? and  hewie 8.95e 0e 11.89e 10e 5e touches? and if  ['] medallion-sniffed start-only  then
    fiona aquarium-area in-area? if  ['] aquarium unhurried offer  then
    omb1-7-medallion-found fact?  omb1-7-medallion fact? 0= and  fiona 8.95e 1e 11.89e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    cam-set@ 2 = if  1  else  2  then  shaft-fx tell-effect ;
: person-in ( i -- )
    fiona-p <> if  exit  then  entries 2 ways-in  came-by @ 0= if  1 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-b1-7 place!
