\ story/places/castle-2f-15.fs - the suspended bridge's hall in Belli Castle's 2F ($30), its far way
\ open: climbing down to the great hall; Hewie's fetch of the Medallion across the gap, his calling
\ back, the stalker's grab on the plank, the statue (the hall's own: story/places/bridge.fs); the
\ Prima Magnesia Hewie sniffs out; Daniella's motions loaded ahead on the way out while she is the
\ maid; the map's pages. A place (story/kit.fs). The original's scripts, for checking: story/
\ rooms-ref/castle-2f-15.fs in the history (converted from the game's; gone in 0.2.20).
IN: story.places.castle-2f-15
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims story.places.bridge ;

own-reset

\ ---- the room's things ----
0 constant magnesia-fx   2 constant to-exit-0-area   3 constant to-exit-1-area   1 constant maid-motions
: maid? ( -- flag )  c1f-14-flag2 fact?  c3f-2-flag1 fact? 0= and ;   \ (the chapter's Daniella the maid)

\ ---- as she comes in ----
create holes  $13 , $124 , $6B , $69 , $54 ,   \ (things lying about fall through)
: came-in ( exit -- )
    drop  $2B $F1 $140 far-side!  room-sounds  hall-track 1e background  medallion-sparkle
    c2f-15-prima-magnesia-found fact?  c2f-15-prima-magnesia fact? 0= and if  magnesia-fx -105e 135e 23e flicker-at  then
    5 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the Prima Magnesia Hewie sniffs out ----
: magnesia-sniffed ( -- )  c2f-15-prima-magnesia-found c2f-15-prima-magnesia magnesia-fx  -105e 135e 23e hewie-finds ;
: hewie-sniffs ( -- )
    c2f-15-prima-magnesia-found fact?  scrambling? 0= or if  exit  then
    hewie -105e 134e 23e 10e 5e touches? 0= if  exit  then
    ['] magnesia-sniffed start-only ;

\ ---- what she may do or say here ----
: pick-magnesia ( -- )  prima-magnesia c2f-15-prima-magnesia magnesia-fx -105e 23e picks-up ;
: spots ( -- )
    climb-spot
    fiona statue-area in-area?  fiona 45 50 heading? and if  ['] the-statue unhurried offer  then
    c2f-15-prima-magnesia-found fact?  c2f-15-prima-magnesia fact? 0= and  fiona -105e 135e 23e 5e 5e touches? and if
        ['] pick-magnesia calm-only offer  then ;

\ ---- every frame ----
create cameras  2 , 2 , 2 ,   3 , 1 , 1 ,   $B , 0 , 0 ,   $C , 2 , 2 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
: ways ( -- )
    2 exits-out  cameras 4 cameras-at
    fiona to-exit-0-area entered? if  0 prepare-exit  3 map-page  then
    fiona to-exit-1-area entered? if  maid? if  stalker maid-motions motions-for  then  1 prepare-exit  4 map-page  then ;
: each-frame ( -- )
    ways  hewie-way-now @ 0= if  hewie-by-the-gap  then  hewie-sniffs  medallion-notice  medallion-look 1 hewie-looks-at  spots ;
: person-in ( i -- )
    case
        fiona-p of  entries 2 ways-in  came-by @ 1 = if  4 map-page  then  endof
        hewie-p of  hewie-comes-in  endof
    endcase ;
: went ( -- )  hewie-as-she-goes ;

' came-in ' person-in ' each-frame ' went castle-2f-15 place!
