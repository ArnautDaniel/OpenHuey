\ story/places/castle-2f-10.fs - the room of the four boxes in Belli Castle's 2F ($17): the boxes
\ pushed onto their squares (each fixed there; all four, the scene - the way opened, and out by the
\ forced way), where they stand kept as she leaves. A place (story/kit.fs). The original's scripts,
\ for checking: story/rooms-ref/castle-2f-10.fs in the history (converted from the game's; gone with
\ the old machinery in 0.2.20).
IN: story.places.castle-2f-10
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    effects story.kit fiona.anims scenes ;

own-reset

\ ---- the room's things ----
4 constant #boxes   1 constant to-exit-0-area   2 constant to-exit-1-area
1 constant way-movie   0 constant way-cutscene
: boxes-kept ( -- )  #boxes 0 do  i i 0 obstacle-kept  loop ;
: keep-boxes ( -- )  #boxes 0 do  i obstacle-keep  loop ;

\ ---- as she comes in ----
: came-in ( exit -- )  drop  room-sounds  boxes-kept  1 $2300 sound-reverb! ;

\ ---- the boxes on their squares: each fixed; all four - the scene, the way opened ----
: on-square? ( box -- flag )   \ (its own square - the first two either of theirs)
    case
        0 of  0 $455 obstacle-on-tri?  0 $459 obstacle-on-tri? or  endof
        1 of  1 $455 obstacle-on-tri?  1 $459 obstacle-on-tri? or  endof
        2 of  2 $473 obstacle-on-tri?  endof
        3 of  3 $4B8 obstacle-on-tri?  endof
        >r false r>
    endcase ;
: boxes-home ( -- n )  0  #boxes 0 do  i on-square? if  i obstacle-fixed  1+  then  loop ;
: way-scene ( -- )  way-movie scenes:keyed-movie way-cutscene load-scene  wait-fade ;
: way-opens ( -- )
    stalkers-stay state!  fiona hold  fiona let-go-push-anim 0 play-blend  fiona -1 idle  7 frames
    fiona 0e 0e turn-to-xz  fiona rest-anim start-anim  60 frames
    15 screen:to-black-2 with-music or fade  way-scene
    things-clear  fiona -1 idle  castle-2f-9 preload  $28 $B4 movie-key
    panic-eased  if  -1 run-scene  then
    world-held state!
    $1C reopened  $1D reopened  $38 closed-off  $39 closed-off  exits-redone
    castle-2f-9 stalker-slot false doors-into  castle-3f-1 stalker-slot false doors-into
    cam-set@ 0= if  forced-exit  else  forced-exit 1 or  then  take-exit ;

\ ---- the movie replayed (the extras): the boxes on their squares ----
: replayed ( -- )
    fiona hold  0 0 0 $455 $CA5 obstacle-in  1 1 0 $459 $CCD obstacle-in  2 2 0 $473 $CE7 obstacle-in  3 3 0 $4B8 $D2C obstacle-in
    fiona false shown  castle-2f-9 preload  way-scene  $28 $B4 movie-key  panic-eased  if  -1 run-scene  then
    replay-over ;

\ ---- every frame ----
create cameras  5 , 0 , 0 ,   6 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
: leaves-by-keeping ( area exit -- )   \ (out by it: where the boxes stand kept)
    {: area ex :}  fiona area in-area?  fiona held? 0= and if  keep-boxes  area ex leaves-by  then ;
: each-frame ( -- )
    events-held state? if  exit  then
    to-exit-0-area 0 leaves-by-keeping  to-exit-1-area 1 leaves-by-keeping  cameras 2 cameras-at
    boxes-home #boxes =  fiona held? 0= and  ['] way-opens running? 0= and if  ['] way-opens start drop  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 2 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-10 place!
