\ story/places/castle-b1-5.fs - the dark passage in Belli Castle's B1 ($B), its pit: stepping into
\ it she falls (the game over); Hewie, the first time he comes in by the near door, tense at the
\ dark, barking; sent on ahead (her "go there" toward one of the three ways, a way off from it), he
\ barks and runs there, turns to her and waits. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/castle-b1-5.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.castle-b1-5
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    story.kit hewie.anims ;

own-reset

\ ---- its own state (a visit) ----
own: fallen      \ she has stepped into the pit
own: guide-tri   \ where Hewie was sent on to

\ ---- the room's things ----
2 constant to-exit-0-area   3 constant to-exit-1-area
0 constant pit-group   0 constant her-motions   0 constant wait-sound   1 constant caught-sound
$1D constant waits-act   $29 constant waits-sub   \ (his wait there, after: the room's $1D)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  pit-group keeps-hewie-off floor-on  pit-group hole-floor floor-on
    fiona her-motions motions-for  1 $2300 reverb ;

\ ---- into the pit: she falls - the game over ----
: falls ( -- )
    capture-no-end state!  fiona hold  fiona use-motions
    [: begin  room-sounds-in? 0= while  yield  repeat  wait-sound room-bank fiona sound-by ;] start drop
    fiona room-motion play  60 frames
    begin  room-sounds-in? 0= while  yield  repeat
    caught-sound room-bank fiona sound-by  16 frames  0 game-over ;

\ ---- Hewie: tense at the dark the first time in by the near door; sent on ahead ----
: tense ( -- )
    hewie hold  hewie wait-move  hewie $A4 90 -1 walk-path-move -55e 0e go-to
    hewie stance-anim start-anim  60 frames
    6 0 do  hewie bark-tense-anim play  loop
    hewie fiona look-at  yield  cb1-5-flag1 fact!
    hewie stance-anim start-anim  40 frames  hewie -1 look-at  yield ;
: hewie-arrives ( -- )   \ (by the near door, the first time: tense - his way in told a moment on)
    2 frames  cb1-5-flag1 fact? 0=  0 hewie-came-by? and if  tense  then ;
: leads-on ( -- )   \ (a bark, there at a run, turned to her - then his wait)
    hewie hold  hewie wait-move  hewie-bark  hewie wait-move
    hewie run-move guide-tri @ -1  guide-tri @ tri-center 0e  move  hewie wait-move
    hewie fiona body-pos fswap fdrop turn-to-xz  hewie let-go  waits-act waits-sub hewie-do ;
create ways-on  -80 , $A7 ,   10 , $AB ,   110 , $9B ,   \ (the point x - z 0 - she faces, his triangle there)
: off-by ( F: x bx bz -- d )  {: F: x bx bz :}  bx x f- fdup f*  bz fdup f* f+ fsqrt ;   \ ((bx, bz) from (x, 0))
: way-off? ( x -- flag )  s>f  fiona body-pos fswap fdrop  off-by  fdup 20e f< 0=  120e f<= and ;   \ (she 20 to 120 from (x, 0))
: toward? ( x -- flag )  {: x :}  fiona 45 x s>f 0e faces-point?  x way-off? and ;   \ (facing the way, a way off from it)
: sent-on ( -- )   \ (her "go there" toward a way: the last of them that fits)
    hewie-in? 0=  started-now @ go-there-cmd <> or  hewie-cond-now @ down-cond = or if  exit  then
    -1  3 0 do
        ways-on i 2* cells + @ toward?  hewie-breakable-now @ and if  drop ways-on i 2* 1+ cells + @  then
    loop
    dup 0< if  drop exit  then  guide-tri !  ['] leads-on start drop ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
create ahead  to-exit-0-area , 0 ,   to-exit-1-area , 1 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    2 exits-out  cameras 2 cameras-at  ahead 2 ways-out
    sent-on
    fallen @ 0=  fiona pit-group on-group? and if  -1 fallen !  ['] falls start drop  then ;
: person-in ( i -- )
    case
        fiona-p of  entries 2 ways-in  endof
        hewie-p of  ['] hewie-arrives start drop  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-b1-5 place!
