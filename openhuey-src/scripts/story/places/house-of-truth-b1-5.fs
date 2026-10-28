\ story/places/house-of-truth-b1-5.fs - the House of Truth's B1 ($93): one of the colour rooms
\ (story/places/house-of-truth.fs): its inscription, the Camomile; its two doors into the dark - the
\ room ahead by the colours done. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/house-of-truth-b1-5.fs (converted from the game's, not loaded).
IN: story.places.house-of-truth-b1-5
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

1 constant inscription-msg   \ "For the next step, be it red or blue, the answers will make themselves known one by ..."
0 constant pickup-fx
: pick-up ( -- )  camomile htb1-5-camomile pickup-fx -117.05e 101.81e picks-up ;
: came-in ( exit -- )  drop  htb1-5-flag1 124 colour-in  htb1-5-camomile fact? 0= if  pickup-fx -117.05e 1e 101.81e flicker-at  then ;
: each-frame ( -- )
    inscription-msg colour-frame  2 htb1-7-flag1 house-of-truth-b1-7 not-yet  3 way-on
    htb1-5-camomile fact? 0=  fiona -117.05e 1e 101.81e 5e 5e touches? and if  ['] pick-up calm-only offer  then ;
: person-in ( i -- )  fiona-p = if  came-by @ forced-exit = if  arrived  then  then ;
: went ( -- )  colour-went ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-5 place!
