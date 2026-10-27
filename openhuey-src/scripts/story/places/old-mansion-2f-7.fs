\ story/places/old-mansion-2f-7.fs - the old mansion's long 2F corridor ($5B): its five flames, its
\ track. A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/
\ old-mansion-2f-7.fs (converted from the game's, not loaded).
IN: story.places.old-mansion-2f-7
USING: engine game-state actors messages common facts room-names flag-names story.kit ;

own-reset

$17 constant corridor-track   5 constant #flames
create flames  -12.3e f, 17.6e f, 125e f,   -12.3e f, 27.6e f, 70e f,   -12.3e f, 27.6e f, 10e f,
               -12.3e f, 27.6e f, -50e f,   -12.3e f, 17.7e f, -106e f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup f@  dup float+ f@  2 floats + f@ ;

: came-in ( exit -- )  drop  corridor-track 1e background  #flames 0 do  i 1  i flame@  flame-at  loop ;
create entries  0 , 0 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,      \ (area, exit loaded ahead)
: each-frame ( -- )  2 exits-out  ahead 2 ways-out ;
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-7 place!
