\ story/places/old-mansion-2f-9.fs - the old mansion's 2F room with the stuffed chimera ($5D): the
\ Sylvesta Esca on the shelf; the chimera's remark; Hewie wary there (his mood up as he comes in),
\ barking tense at it now and then, his looks. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/old-mansion-2f-9.fs in the history (converted from the game's; gone with
\ the old machinery in 0.2.20).
IN: story.places.old-mansion-2f-9
USING: engine game-state actors messages common facts room-names flag-names story-names items.names story.kit fiona.anims ;

own-reset

0 constant chimera-msg   \ "A stuffed chimera is showing off its fierce gaze. Why would anyone ever decorate a room..."
own: chimera-noticed
0 constant esca-fx   7 constant chimera-area

: esca-shows ( -- )  got-sylvesta-esca fact? 0= if  esca-fx 42e 63.5e 6.6e flicker-at  then ;
: came-in ( exit -- )  drop  esca-shows ;
: take-esca ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 42e 6.6e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  sylvesta-esca got-sylvesta-esca esca-fx takes-up  fiona reach-back-anim play ;
: the-chimera ( -- )
    fiona wait-move  fiona 50e 9e turn-to-xz  fiona look-at-anim start-anim  40 frames  chimera-msg say  fiona wait-anim ;
: barks-at-chimera ( -- )  43.86e 50e 6.98e hewie-barks-tense ;
create near-looks  43.86e 50e 6.98e  21e 16e 16e look,
create far-looks  -5.14e 50e 13.21e  68e 29e 29.43e look,
create cameras  5 , 1 , 1 ,   6 , 2 , 2 ,   \ (area, set, path)
create entries  1 , 1 ,   2 , 2 ,   0 , 0 ,   \ (by exit 0..2: the camera)
create ahead  3 , 0 ,   4 , 1 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    3 exits-out  cameras 2 cameras-at  ahead 2 ways-out
    chimera-noticed 30 43.86e 50e 6.98e 21e 16e notices? if  ['] barks-at-chimera start drop  then
    near-looks 1 hewie-looks-unchased  far-looks 1 hewie-looks-at
    got-sylvesta-esca fact? 0=  fiona 42e 63.5e 6.6e 5e 5e touches? and if  ['] take-esca calm-only offer  then
    fiona chimera-area 60 faces-area? if  ['] the-chimera unhurried offer  then ;
: person-in ( i -- )
    case  fiona-p of  entries 3 ways-in  esca-shows  endof  hewie-p of  2 hewie send mood!  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-9 place!
