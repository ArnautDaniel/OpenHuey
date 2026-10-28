\ story/places/off-map-d6.fs - the clock run's forest ($D6, a bush's ground as chaos-forest-7's):
\ four ways, each by its own area; the rooms ahead loaded as Hewie nears them; the bush - Fiona
\ crouches in it at his command as he stands by it (she came by exit 1, nobody chasing; no stalker,
\ or not one come her way), lying low till his next command, panicking or found; the stalker coming
\ in by her side searches it by the odds; its three hanging leaves swing as she, Hewie or the
\ stalker pushes through; left behind in it, she's out at its front; the clock. A place
\ (story/kit.fs, story/places/forest.fs, story/places/clock-run.fs). The original's scripts, for
\ checking: story/rooms-ref/off-map-d6.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.off-map-d6
USING: engine game-state actors messages common flag-names facts room-names floor-flags story.kit story.clock
    story.places.forest story.places.clock-run ;

own-reset  forest-owns owns !
own: her-held   \ Fiona in the bush this frame (held by it: kept for the room's end)

$2F constant search-var   1 constant bush-way
: came-in ( exit -- )
    drop  room-sounds  leaves-still
    bush-way search-var $E7 -90 70.5e -5.5e 90.5e -4.5e bush  floor-flag-40000 bush-floor ! ;

\ ---- the bush: in at Hewie's command ----
: hides ( -- )  stalkers-stay state!  fiona hold  fiona $19A 90 70.5e -5.5e walk-to  in-the-bush ;
: cue? ( -- flag )   \ (he stands by it, gives a command; she came by its side, free, nobody after her)
    hewie 70.5e -5.5e 20e centre-across?  fiona here? and  came-by @ bush-way = and
    fiona held? 0= and  in-panic? 0= and  fiona-idle? and  played-gesture -1 <> and
    stalker-in? 0=  bush-way stalker-came-by? 0= or and ;
: searched ( -- )   \ (he came in by her side as she hides: whether he finds her - by the odds, a
    from-his-way? fiona-hidden state? and dup came-while-hidden ! 0= if  exit  then   \ creature moving about - counted)
    search-var mansion-odds chance?  2 creature-moving? or if  ['] bush-found start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;
: pushed-by ( -- id | -1 )   \ (whoever stands in its middle: Fiona first, then Hewie, then the stalker)
    fiona 70.5e -5.5e 5e centre-across? if  fiona exit  then
    hewie 70.5e -5.5e 5e centre-across? if  hewie exit  then
    stalker 70.5e -5.5e 5e centre-across? if  stalker exit  then  -1 ;

\ ---- every frame ----
create entries  0 , 0 ,  1 , 1 ,  0 , 0 ,  0 , 0 ,   \ (by exit 0 .. 3: the camera)
create ahead  4 , 0 ,  6 , 2 ,  7 , 3 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    4 by-areas  ahead 3 his-ways
    cue? if  ['] hides fiona start-holding drop  then
    pushed-by dup 0< if  drop  else  leaves-pushed-by  then  leaves-swing
    fiona held? her-held !  clock-shown ;
: person-in ( i -- )
    dup stalker-p = if  drop searched exit  then
    player person-of = if  entries 4 ways-in  then ;
: went ( -- )  her-held @ if  $E7 90 90.99e -3.17e left-hidden  then  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d6 place!
