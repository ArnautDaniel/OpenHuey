\ story/places/house-of-truth-b1-7.fs - the House of Truth's B1 ($95): one of the colour rooms
\ (story/places/house-of-truth.fs): its inscription, the Beef Jerky; its two doors into the dark -
\ the room ahead by the colours done. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/house-of-truth-b1-7.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.house-of-truth-b1-7
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

3 constant inscription-msg   \ "The colors, like the unique memories of the mind, may only come once in a lifetime."
0 constant pickup-fx
: pick-up ( -- )  beef-jerky htb1-7-beef-jerky pickup-fx -119.22e -96.82e picks-up ;
: came-in ( exit -- )  drop  htb1-7-flag1 124 colour-in  htb1-7-beef-jerky fact? 0= if  pickup-fx -119.22e 1e -96.82e flicker-at  then ;
: each-frame ( -- )
    inscription-msg colour-frame  2 htb1-6-flag1 house-of-truth-b1-6 not-yet  3 htb1-8-flag1 house-of-truth-b1-8 not-yet
    htb1-7-beef-jerky fact? 0=  fiona -119.22e 1e -96.82e 5e 5e touches? and if  ['] pick-up calm-only offer  then ;
: person-in ( i -- )  fiona-p = if  came-by @ forced-exit = if  arrived  then  then ;
: went ( -- )  colour-went ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-7 place!
