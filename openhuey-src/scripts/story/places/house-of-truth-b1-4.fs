\ story/places/house-of-truth-b1-4.fs - the House of Truth's white room in the B1 ($87), where the
\ colours begin (story/places/house-of-truth.fs): the four colours undone and their doors shut each
\ time she comes in; its inscription; its two doors into the dark (the red and the blue rooms
\ ahead); in by the forced way - the first time its fact, after that its file. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-b1-4.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-b1-4
USING: engine game-state actors messages common facts room-names flag-names story-names story.kit
    story.places.house-of-truth ;

own-reset  truth-owns owns !

0 constant inscription-msg   \ "All things begin pure of color."
$254 constant white-file
create colour-doors  $F1 , $F2 , $104 , $105 , $106 , $107 , $108 , $109 , $10A , $10B ,
: came-in ( exit -- )
    drop
    found-file-254 fact? 0= if  room-sounds  keep-room-sounds state!  then
    htb1-5-flag1 fact-off  htb1-6-flag1 fact-off  htb1-7-flag1 fact-off  htb1-8-flag1 fact-off
    10 0 do  colour-doors i cells + @ false open-mark  loop  doors-redone
    0 2 colour-in ;
: comes-to-white ( -- )   \ (the dark lifting; the first time its fact, after that its file)
    comes-to  found-file-254 fact? if  white-file give-file  else  found-file-254 fact!  then ;
: each-frame ( -- )
    inscription-msg colour-frame  2 house-of-truth-b1-5 loads-ahead  3 house-of-truth-b1-6 loads-ahead ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    came-by @ forced-exit = if
        fiona arrival-tri -90 129e 0e place-at  0 0 player-camera
        this-room @ 235 0 hewie-to  hewie his-arrival-tri -90 place-on-tri
        ['] comes-to-white start drop
    then ;
: went ( -- )  colour-went ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-4 place!
