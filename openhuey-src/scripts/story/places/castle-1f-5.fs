\ story/places/castle-1f-5.fs - the long gallery in Belli Castle's 1F ($10): Debilitas coming in
\ (the golem done, she back by exit 1 - he bursts through after her), his shove bringing the rubble
\ down; brought in after Daniella's scene (castle-1f-6): the chapter turns - the castle's doors set
\ anew, Daniella the stalker now, Hewie gone back to his room, she weak ("I don't feel so hot")
\ till rested; three flames stirring as someone passes, a vase (the Antimony Powder), the box, the
\ dim corner, the twin paintings, the map's pages, the walls' shade. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-1f-5.fs (converted from the game's, not
\ loaded).
IN: story.places.castle-1f-5
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects keys story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
2 constant empty-box-msg    \ "There is nothing inside."
3 constant dust-box-msg     \ "A second, more-thorough investigation of the box reveals that there is dust inside..."
4 constant dim-msg          \ "The lighting is dim making it hard to see, but... I can ever so faintly make out the sh..."
5 constant not-hot-msg      \ "I... I don't feel so hot. It's like I have no energy all of a sudden."
6 constant rest-msg         \ "I should try to get some rest."
7 constant painting-msg     \ "A painting... Several people appear to be clasping their hands in prayer."
8 constant painting-2-msg   \ "If I had the time, I'd like to spend more time taking in the painting's full beauty..."
9 constant twins-msg        \ "It seems like this and the painting hanging next to it make up a set of twin paintings."

\ ---- its own state (a visit) ----
own: his-entrance          \ Debilitas's entrance under way (followed; Hewie not to attack)
own: box-remarked  own: tired-remarked  own: painting-remarked
own: in-gallery            \ she is in the gallery's end (entered, not yet left)
own: his-scene             \ his shove under way (at the room's end he is put back)
own: him-sent              \ the stalker sent to shove (once a visit)

\ ---- the room's things ----
3 constant antimony-fx   3 constant to-exit-0-area   4 constant from-exit-2-area   5 constant to-exit-2-area
6 constant from-exit-1-area   $11 constant rubble-area   $12 constant gallery-area   $F constant dim-area
$10 constant box-area   $13 constant painting-area   $14 constant twin-area
0 constant tired-sound   11 constant thud-sound   0 constant his-motions   0 constant his-shove
$C constant chase-door
c1f-5-breakable-broken 0 1  41e 0e -19e 5e 8e  breakable: the-vase
the-vase $80101010 0 4 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv
the-vase 0 frees-floor  the-vase $232 noise-from
: night? ( -- flag )  c2f-3-flag1 fact? ;

\ ---- the flames (effects 0..2), each stirred as someone steps into its draught ----
create flames  6e 21.8e -3.2e f, f, f,   29e 41.8e 41.9e f, f, f,   -46.8e 93.8e 18.5e f, f, f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup 2 floats + f@  dup float+ f@  f@ ;
: flames-lit ( -- )  3 0 do  i 0  i flame@  flame-at  loop ;
: draughts ( -- )  3 0 do  i  i flame@  8e -4e  flame-stirred  loop ;

\ ---- Debilitas coming in after her (the golem done, she back by exit 1) ----
: run-to-tri ( id tri -- )  {: id tri :}  id run-path-move tri -1  tri tri-center 0e  move  id wait-move ;
: entrance-over ( -- )  hewie-no-attack state-off  force-followed state-off  0 his-entrance !  stalker let-go ;
: bursts-in ( -- )   \ (in by the far door, through the near one if shut, on along the gallery)
    yield  stalker -1 -1 camera-set  stalker full-health
    stalker hold  1 $FF 8 rumble-pad  stalker $A1 -90 place-on-tri
    0 exit-open? 0= if  stalker $1B4 run-to-tri  stalker 0 open-door  then
    stalker $3C run-to-tri  entrance-over ;
: he-comes? ( -- flag )   \ (Debilitas, loaded as the stalker but not yet in the game)
    partner dup 0< if  drop false exit  then  body? if  false exit  then
    partner-kind dup debilitas-kind = over debilitas-6-kind = or swap debilitas-7-kind = or ;
: his-entrance-starts ( -- )
    c1f-5-flag1 fact!  force-followed state!  chase-door false open-mark  doors-redone  -1 his-entrance !
    hewie-no-attack state!  partner activate  castle-1f-5 161 partner send stalker-in  ['] bursts-in start drop ;

\ ---- his shove: Debilitas by the rubble, she at the gallery's end - he shoves it down ----
: shoves ( -- )
    stalker hold  stalker his-motions motions-for  stalker $8A 0 31.949e -18.333e walk-to
    stalker use-motions  stalker true noclip  -1 his-scene !  stalker room-motion his-shove + 5 start-blend8
    0  begin
        stalker fiona 0 close-to? if  stalker throws-her  then
        dup 60 = if  29e 19e 25e dust  36e 19e 25e dust  thud-sound room-bank stalker sound-by  then
        1+  st-kind-now @  cam-set@ 0<> no-shadow
        stalker went-round? 0= while  yield
    repeat  drop
    stalker false noclip  st-kind-now @ false no-shadow  0 his-scene !  2 stalker-order ;
: shove? ( -- )
    him-sent @  4 stalker-alert? or  st-cond-now @ down-cond = or  debilitas? 0= or if  exit  then
    0 stalker-alert?  2 stalker-alert? or 0= if  exit  then
    stalker rubble-area in-area?  in-gallery @ and if  -1 him-sent !  ['] shoves start drop  then ;

\ ---- the chapter turns (brought in after Daniella's scene): the castle's doors anew, Daniella the
\ stalker, she weak; her costume and Hewie's coat put on afresh, the stage music anew ----
create stalker-locked-rooms   $E , $F , $25 , $60 , $63 , $4C , $66 , $61 , $53 , $54 , $5F ,
create closed-off-doors   $58 , $5A , $4D , $4E , $50 , $53 , $5C , $79 ,
create locked-doors   2 , $34 , $62 , $44 , $47 , $4F , $56 , $67 , $7D , $51 , $52 , $64 , $6C , $74 , $78 , $5E , $76 , $7B , $7C , $63 , $54 ,
: each-cell ( addr n xt -- )  {: a n xt :}  n 0 ?do  a i cells + @ xt execute  loop ;
: kept-from ( room -- )  stalker-slot 1 doors-into ;
: doors-anew ( -- )
    stalker-locked-rooms 11 ['] kept-from each-cell  $6C stalker-slot true door-for
    $53 hewie-slot 1 doors-into  $5F hewie-slot 1 doors-into
    1 reopened  $D reopened  closed-off-doors 8 ['] closed-off each-cell  exits-redone  camera-restart
    locked-doors 21 ['] lock each-cell
    1 false open-mark  1 lock  3 false open-mark  3 lock  chase-door false open-mark  chase-door lock
    $D unlock  $D true open-mark  doors-redone ;
: chapter-turns ( -- )
    hewie let-go  fiona hold  daniella-kind stalker-ready  yield
    doors-anew
    no-flee state!  no-running state!  hewie-commandable state!  scene-locked state-off  world-held state!
    over-music-b fact!  subscreen-locked state!
    costume-var var@ dress  yield  coat-var var@ coat  yield
    -1 music-part  1 music-part  2 0 0 stage-music  3 0 0 stage-music  4 0 0 stage-music
    hewie activate  castle-1f-5 155 0 hewie-to  hewie $9B 0 place-on-tri  in-play state-off
    15 screen:from-black fade  wait-fade ;

\ ---- she weak (the chapter turned): reaching to act, she flags ----
: flags ( -- )
    fiona hold  fiona wait-move  fiona knocked-anim start-anim  tired-sound room-bank fiona sound-by  20 frames
    tired-remarked @ if  0 tired-remarked !  rest-msg  else  -1 tired-remarked !  not-hot-msg  then  show
    fiona attack-anim 10 start-blend  wait-closed  fiona wait-anim ;

\ ---- what she may do or say here ----
: dim-corner ( -- )  fiona wait-move  fiona -177 turn  fiona look-close-anim play  dim-msg say ;
: the-box ( -- )
    fiona wait-move
    box-remarked @ if  dust-box-msg say exit  then
    fiona 40e -11e turn-to-xz  fiona reach-out-anim play  empty-box-msg say  -1 box-remarked ! ;
: the-painting ( -- )
    fiona $17B 90 -1 walk-move 94e -27.5e go-to  1 15e 0e 0e 8e shot  4 frames
    painting-remarked @ if  painting-2-msg  else  -1 painting-remarked !  painting-msg  then  say
    4 frames  shot-off ;
: the-twin ( -- )
    fiona $19A 90 -1 walk-move 94e 27.5e go-to  1 15e 0e 0e 8e shot  4 frames  twins-msg say  4 frames  shot-off ;
: pick-antimony ( -- )  antimony-powder c1f-5-antimony-powder antimony-fx 41e -19e picks-up ;
: spots ( -- )
    fiona dim-area in-area?  fiona 90 50 heading? and if  ['] dim-corner unhurried offer  then
    fiona box-area 45 faces-area? if  ['] the-box unhurried offer  then
    fiona painting-area in-area?  fiona 45 50 heading? and if  ['] the-painting unhurried offer  then
    fiona twin-area in-area?  fiona 45 50 heading? and if  ['] the-twin unhurried offer  then
    weak?  3 locked-request? and if  ['] flags any-time offer  then
    the-vase broken? if
        c1f-5-antimony-powder fact? 0=  fiona 41e 1e -19e 5e 5e touches? and if  ['] pick-antimony calm-only offer  then
    else  the-vase breaks? if  antimony-fx 41e 1e -19e flicker-at  then  then ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  0 his-scene !
    the-vase as-it-stands
    the-vase broken?  c1f-5-antimony-powder fact? 0= and if  antimony-fx 41e 1e -19e flicker-at  then
    night? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    flames-lit
    c1f-6-flag1 fact?  over-music-b fact? 0= and if  hewie let-go  then
    1 $2300 sound-reverb! ;

\ ---- the walls' shade ----
: walls-shade ( -- )
    -8e 82e -5e  -8e 82e 45e  -8e 30e -5e  -8e 30e 45e  light-doorway
    -5.3e 54e -4.1e  -5.3e 54e 47e  -58e 54e -4.1e  -58e 54e 47e  light-doorway
    15e 30e -3e  45e 30e -3e  15e 15e -3e  45e 15e -3e  light-doorway
    15e 15e -3e  45e 15e -3e  15e 0e -3e  45e 0e -3e  light-doorway
    -11e 30e -3e  15e 30e -3e  -11e 15e -3e  15e 15e -3e  light-doorway
    -11e 15e -3e  15e 15e -3e  -11e 0e -3e  15e 0e -3e  light-doorway ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    7 , 2 , 2 ,   8 , 1 , 1 ,   9 , 0 , 0 ,   $A , 1 , 1 ,   $D , 0 , 0 ,   $15 , 3 , -1 ,   $16 , 4 , 3 ,   $17 , 3 , -1 ,
create entries  3 , -1 ,   0 , 0 ,   4 , 3 ,     \ (by exit 0, 1, 2: the camera)
: ways ( -- )
    3 exits-out  cameras 8 cameras-at
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    fiona from-exit-2-area left?  fiona to-exit-2-area entered? or if  2 prepare-exit  then
    fiona from-exit-1-area left? if  1 prepare-exit  then
    fiona gallery-area entered? if  -1 in-gallery !  then   fiona gallery-area left? if  0 in-gallery !  then
    fiona to-exit-2-area entered? if  3 map-page  then   fiona to-exit-2-area left? if  2 map-page  then ;
: each-frame ( -- )
    ways  shove?  ['] flags weakness  draughts  spots  walls-shade
    room-sounds-in? if  tired-sound keep-playing or room-bank fiona sound-by  then ;
: person-in ( i -- )
    dup stalker-p = if  drop  0 him-sent ! exit  then
    fiona-p <> if  exit  then
    entries 3 ways-in
    came-by @ 1 = if  -1 in-gallery !  then
    c1f-6-flag1 fact?  over-music-b fact? 0= and if
        fiona 1 at-exit  0 0 player-camera  ['] chapter-turns start drop
    then
    came-by @ 0=  came-by @ 2 = or if  3 map-page  then
    golem-done fact?  c1f-5-flag1 fact? 0= and  came-by @ 1 = and  he-comes? and if  his-entrance-starts  then ;
: went ( -- )
    his-entrance @ if  entrance-over  then
    stalker held?  his-scene @ and if  stalker $196 0 place-on-tri  st-kind-now @ false no-shadow  0 his-scene !  then ;

' came-in ' person-in ' each-frame ' went castle-1f-5 place!
