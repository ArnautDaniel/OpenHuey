\ story/places/old-mansion-1f-10.fs - a crossing of the old mansion's 1F ($4C): four ways out; Hewie
\ (once a visit, now and then) barking tense at one corner, wary at the other; his looks there. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-10.fs
\ in the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-10
USING: engine game-state actors messages common facts room-names flag-names story.kit ;

own-reset

own: corner-1-noticed  own: corner-2-noticed

: came-in ( exit -- )  drop  2 1e background ;
: barks-at-corner ( -- )  -24.64e 2.5e 27.03e hewie-barks-tense ;
: wary-of-corner ( -- )  25.74e 2.5e -26.39e hewie-stands-by ;
create looks  -24.64e 2.5e 27.03e  20e 6e 6e look,   25.74e 2.5e -26.39e  20e 6e 6e look,
create entries  0 , 0 ,   0 , 0 ,   0 , 0 ,   0 , 0 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   5 , 1 ,   6 , 2 ,   7 , 3 ,     \ (area, exit loaded ahead)
: each-frame ( -- )
    4 exits-out  ahead 4 ways-out
    corner-1-noticed 30 -24.64e 2.5e 27.03e 20e 6e notices? if  ['] barks-at-corner start drop  then
    corner-2-noticed 30 25.74e 2.5e -26.39e 20e 6e notices? if  ['] wary-of-corner start drop  then
    looks 2 hewie-looks-unchased ;
: person-in ( i -- )  fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-10 place!
