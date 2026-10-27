\ story/places/chaos-forest-9.fs - the Chaos Forest by the lake ($107): the crate on the rock (broken by
\ a blow: the Refined Antimony in it); her remarks at the lake (the tower in it, then no way on); the
\ wisp in the trees (story/places/forest.fs) startled by a loud noise; the light rippling far off; the
\ maze dealt. A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/
\ chaos-forest-9.fs (converted from the game's, not loaded).
IN: story.places.chaos-forest-9
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names effects story.kit story.places.forest fiona.anims ;

own-reset  forest-owns owns !

0 constant lake-msg        \ "It's... a lake, I think. Looks like there some kind of a... tower erected in the ce..."
1 constant no-way-msg      \ "I'm not going to be able to go any further this way. Better go back and find a way..."
own: lake-remarked         \ its first remark made
$19 constant forest-track   0 constant antimony-fx   1 constant lake-area
0 constant whole-part   1 constant broken-part   0 constant crate-group
cf-9-breakable-broken whole-part broken-part  -298.4e 71.6e -365.4e 5e 8e  breakable: the-crate
the-crate shard-grey 0 $B shard-look  the-crate 0.812e 0.687e 0.187e 0.312e shard-uv  the-crate crate-group frees-floor  the-crate $BA noise-from

: far-light ( -- )  $C $80808080  -225e -63.62e -1125e  1400e 0.5e 0.1e  effects:caustic drop ;   \ (the tower's light far off, turning)
: antimony-shows ( -- )  antimony-fx -298.4e 72.6e -365.4e flicker-at ;
: came-in ( exit -- )
    drop  room-sounds  forest-track 1e background  maze-in
    the-crate as-it-stands
    the-crate broken?  cf-9-refined-antimony fact? 0= and if  antimony-shows  then
    cf-9-flag1 fact? 0= if  -339.86e 125.95e -391.22e 2e wisp-in  then
    far-light ;

: pick-antimony ( -- )  refined-antimony cf-9-refined-antimony antimony-fx -298.4e -365.4e picks-up ;
: the-lake ( -- )
    fiona wait-move
    lake-remarked @ if  fiona hands-clasped-anim 5 play-blend  no-way-msg say exit  then
    fiona look-close-anim 5 play-blend  lake-msg say  -1 lake-remarked ! ;
: spots ( -- )
    the-crate broken? if
        cf-9-refined-antimony fact? 0=  fiona -298.4e 72.6e -365.4e 5e 5e touches? and if  ['] pick-antimony calm-only offer  then
    else  the-crate breaks? if  antimony-shows  then  then
    fiona lake-area in-area?  fiona 90 45 heading? and if  ['] the-lake unhurried offer  then ;
: each-frame ( -- )
    0 0 leaves-by  maze-frame
    wisp-up @  cf-9-flag1 fact? 0= and  loud-here? and if  cf-9-flag1 startled  then
    spots ;
: person-in ( i -- )  fiona-p = if  0 0 0 way-in  then ;
: went ( -- )  wisp-slot figure-out ;

' came-in ' person-in ' each-frame ' went chaos-forest-9 place!
