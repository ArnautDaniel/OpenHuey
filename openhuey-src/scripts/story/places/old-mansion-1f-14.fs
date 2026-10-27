\ story/places/old-mansion-1f-14.fs - the projection room of the old mansion's 1F: the projector and
\ its screen; the Film Reel used there - the film played (the start button stops it); Daniella the
\ stalker then: the screen slashed, her scene ("I was being filmed...") and her after her; the
\ mannequins, the shelves of old books, the mannequin parts; the Chicken Jerky; the replay. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-14.fs
\ (converted from the game's, not loaded).
IN: story.places.old-mansion-1f-14
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    items.names story.kit fiona.anims scenes ;

own-reset

0 constant filmed-msg       \ "What is this? I was being filmed. Who's there? Why are you after me? What d..."
1 constant projector-msg    \ "There's a projector set up here. Its looks rather old, but perhaps it still works."
2 constant screen-set-msg   \ "The screen is already set up. All I need now is a film to play."
3 constant screen-msg       \ "A screen... Has someone been watching films here recently?"
4 constant played-msg       \ "The film seems to have totally played out. I've seen everything there is to see."
5 constant slashed-msg      \ "A slash runs along the surface of the screen. I'd best forget it. It's definitely u..."
6 constant mannequin-msg    \ "Just a lifeless mannequin."
7 constant books-msg        \ "This shelf is packed full of old books, discolored from years of disuse. A majority of..."
8 constant parts-msg        \ "Scattered mannequin parts are strewn about. When assembled correctly, these parts will..."
own: projector-remarked
0 constant jerky-fx   0 constant whole-screen   1 constant slashed-screen   8 constant arm   9 constant chair
0 constant the-film   5 constant film-class   7 constant filmed-movie   6 constant filmed-cutscene
$75 constant her-door

: screen-shown ( -- )
    film-reel-used-om1f-14 fact? if  whole-screen hide-part  slashed-screen show-part
    else  whole-screen show-part  slashed-screen hide-part  then ;
: came-in ( exit -- )
    drop  room-sounds  screen-shown  arm object-show  chair object-show
    om1f-14-chicken-jerky fact? 0= if  jerky-fx 15.82e 1e 20.96e flicker-at  then ;

\ ---- the film: played; Daniella the stalker - the screen slashed, her scene, her after her ----
: daniella-now? ( -- flag )  partner-kind dup daniella-kind = swap kind34-kind = or ;
: blended ( -- )   \ (her scene's last part: each frame over the last)
    begin  cutscene-frame $8E8 < while  yield  repeat
    begin  cutscene-frame 0< 0= while  $2D frame-blend  yield  repeat ;
: filmed-scene ( -- )
    filmed-movie scenes:unseen-movie filmed-cutscene load-scene  4 cutscene-keep-model   \ (its actor 4 left as it is: Mark83)
    old-mansion-1f-14 43 stalker-to  stalker 0 0 camera-set  ['] blended start drop
    panic-eased  if  filmed-msg run-scene  then ;
: the-film-plays ( -- )
    fiona hold  daniella-now? if  stalker let-go  then  fiona wait-move  16 frames
    15 screen:to-black-2 with-music or fade  wait-fade  the-film film-class film
    film-reel-used-om1f-14-2 fact!  world-held state!  film-reel spend
    daniella-now? if
        whole-screen hide-part  slashed-screen show-part  film-reel-used-om1f-14 fact!  yield  yield
        filmed-scene  world-held state!
        old-mansion-1f-14 stalker-slot 0 doors-into  her-door stalker-slot false door-for
        old-mansion-1f-14 43 stalker-to  partner activate  stalker $2B -160 7.18e 33.88e place-at  his-own-time
        om1f-14-flag1 fact!  stalkers-stay state-off
        fiona $16 0 -1.36e 20.03e place-at  0 0 player-camera  yield  camera-restart
    else
        fiona $76 168 -6.79e 9.91e place-at  0 0 player-camera  yield  camera-restart  stalkers-stay state-off
    then
    arm object-show  chair object-show  15 fade-in  $33 resident-flag-set ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    0= if  ['] the-film-plays start drop  true  else  false  then ;

\ ---- what she may say here ----
: the-projector ( -- )
    fiona wait-move
    film-reel-used-om1f-14-2 fact? if  played-msg
    else  projector-remarked @ if  screen-set-msg  else  -1 projector-remarked !  projector-msg  then  then  say ;
: the-screen ( -- )  fiona wait-move  film-reel-used-om1f-14 fact? if  slashed-msg  else  screen-msg  then  say ;
: mannequin ( -- )  fiona wait-move  mannequin-msg say ;
: books ( deg -- )
    fiona wait-move  fiona swap turn  fiona look-close-anim start-anim  40 frames  books-msg say  fiona wait-anim ;
: books-left ( -- )  -90 books ;
: books-right ( -- )  90 books ;
: parts ( -- )  fiona wait-move  fiona 19e 33e turn-to-xz  fiona look-low-anim start-anim  40 frames  parts-msg say  fiona wait-anim ;
: take-jerky ( -- )  chicken-jerky om1f-14-chicken-jerky jerky-fx 15.82e 20.96e picks-up ;
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;
: spots ( -- )
    fiona 3 50 faces-area? if  ['] the-projector unhurried offer  then
    4 180 50 facing? if  ['] the-screen unhurried offer  then
    5 -90 60 facing?  6 -90 60 facing? or  7 90 60 facing? or if  ['] mannequin unhurried offer  then
    8 -90 60 facing?  9 -90 60 facing? or if  ['] books-left unhurried offer  then
    $A 90 60 facing? if  ['] books-right unhurried offer  then
    fiona $B 60 faces-area? if  ['] parts unhurried offer  then
    om1f-14-chicken-jerky fact? 0=  fiona 15.82e 1e 20.96e 5e -8e touches? and if  ['] take-jerky calm-only offer  then ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  daniella-kind stalker-ready  yield  whole-screen hide-part  slashed-screen show-part  yield  yield
    filmed-scene  stalker-unloaded  replay-over ;

create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   \ (by exit 0: the camera)
: each-frame ( -- )  events-held state? if  exit  then  0 exit-out  cameras 2 cameras-at  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    entries 1 ways-in ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-14 place!
' items-used old-mansion-1f-14 items-here
