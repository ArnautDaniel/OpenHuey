\ story/places/house-of-truth-b1-11.fs - a corridor of the House of Truth's B1 ($99): Lorenzo, met
\ in B1-10, leaves her be as she goes on (gone at once if he isn't here, else after a glance); once
\ she has met him again in B1-12, as she comes back this way he is here ahead of her (after his
\ glance if he was here); his slam's shake. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/house-of-truth-b1-11.fs in the history (converted from the game's; gone
\ with the old machinery in 0.2.20).
IN: story.places.house-of-truth-b1-11
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

own: sent                  \ he is to leave her be (the first time on): once he's done
own: sent-2                \ he is to be here ahead of her (coming back): once he's done
$1304 constant glance-anim   \ (one of his looks about)
: came-in ( exit -- )  drop  1 solid-floor floor-on ;
: let-be ( -- )  htb1-11-flag1 fact!  stalker let-go  1 summon-take ;
: glances ( -- )  stalker hold  stalker wait-move  stalker glance-anim play ;
: leaves-her ( -- )  glances  0 sent !  let-be ;   \ (an activity)
: there ( -- )  $136 0 appears  0 sent-2 !  htb1-11-flag2 fact! ;
: back-there ( -- )  glances  stalker false shown  there ;
: each-frame ( -- )
    2 2 leaves-by  3 3 leaves-by
    fiona 6 entered? if  2 prepare-exit  then
    fiona 7 entered? if  3 prepare-exit  then
    slam-shake
    htb1-10-flag1 fact?  htb1-11-flag1 fact? 0= and  fiona $B entered? and if
        stalker-in? if  -1 sent !  else  let-be  then
    then
    sent @  stalker held? 0= and  ['] leaves-her running? 0= and if  ['] leaves-her start drop  then
    htb1-12-flag1 fact?  htb1-11-flag2 fact? 0= and  fiona $D left? and if
        stalker-in? if  -1 sent-2 !
        else  htb1-11-flag2 fact!  this-room @ 310 sent-in  ['] there start drop  then
    then
    sent-2 @  stalker held? 0= and  ['] back-there running? 0= and if  ['] back-there start drop  then ;
: person-in ( i -- )  fiona-p = if  2 1 1 way-in  3 1 1 way-in  then ;
: went ( -- )   \ (leaving before he was done: as if he had)
    sent @  htb1-11-flag1 fact? 0= and if  let-be  then
    sent-2 @  htb1-11-flag2 fact? 0= and if  htb1-11-flag2 fact!  this-room @ 310 sent-in  then ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-11 place!
