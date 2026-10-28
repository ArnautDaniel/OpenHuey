\ story/places/house-of-truth-b1-12.fs - a corridor of the House of Truth's B1 ($9A): once he has
\ left her be in B1-11, the first time she comes through, Lorenzo ahead of her, looking about; his
\ slam's shake. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/house-of-truth-b1-12.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.house-of-truth-b1-12
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

: came-in ( exit -- )  drop  1 solid-floor floor-on ;
: there ( -- )  $429 170 appears ;
: each-frame ( -- )
    2 2 leaves-by  3 3 leaves-by
    fiona 6 entered? if  2 prepare-exit  then
    fiona 7 entered? if  3 prepare-exit  then
    slam-shake
    htb1-11-flag1 fact?  htb1-12-flag1 fact? 0= and  fiona $C entered? and if
        htb1-12-flag1 fact!  this-room @ 1065 sent-in  ['] there start drop
    then ;
: person-in ( i -- )  fiona-p = if  2 1 1 way-in  3 1 1 way-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-12 place!
