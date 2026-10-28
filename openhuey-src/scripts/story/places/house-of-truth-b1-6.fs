\ story/places/house-of-truth-b1-6.fs - the House of Truth's B1 ($94): one of the colour rooms
\ (story/places/house-of-truth.fs): its inscription; its two doors into the dark - the room ahead by
\ the colours done. A place (story/kit.fs). The original's scripts, for checking: story/rooms-
\ ref/house-of-truth-b1-6.fs (converted from the game's, not loaded).
IN: story.places.house-of-truth-b1-6
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

2 constant inscription-msg   \ "One should care not of order that ties the colors of red,blue,yellow,green together"
: came-in ( exit -- )  drop  htb1-6-flag1 2 colour-in ;
: each-frame ( -- )  inscription-msg colour-frame  2 htb1-8-flag1 house-of-truth-b1-8 not-yet  3 htb1-7-flag1 house-of-truth-b1-7 not-yet ;
: person-in ( i -- )  fiona-p = if  came-by @ forced-exit = if  arrived  then  then ;
: went ( -- )  colour-went ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-6 place!
