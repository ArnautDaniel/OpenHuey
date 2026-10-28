\ story/places/house-of-truth-b1-8.fs - the House of Truth's B1 ($96): one of the colour rooms
\ (story/places/house-of-truth.fs): its inscription; its two doors into the dark - the room ahead by
\ the colours done. A place (story/kit.fs). The original's scripts, for checking: story/rooms-
\ ref/house-of-truth-b1-8.fs (converted from the game's, not loaded).
IN: story.places.house-of-truth-b1-8
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

4 constant inscription-msg   \ "All colors must, in the end, form black. This is mankind's inevitable fate."
: came-in ( exit -- )  drop  htb1-8-flag1 2 colour-in ;
: each-frame ( -- )  inscription-msg colour-frame  2 way-on  3 htb1-5-flag1 house-of-truth-b1-5 not-yet ;
: person-in ( i -- )  fiona-p = if  came-by @ forced-exit = if  arrived  then  then ;
: went ( -- )  colour-went ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-8 place!
