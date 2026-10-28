\ story/places/castle-2f-14.fs - the suspended bridge's hall in Belli Castle's 2F ($2F), the great
\ hall's balcony fallen: the first time in, the screen back (the crows' file); climbing down to the
\ great hall; the bridge, Hewie's fetch of the Medallion across the gap, his calling back, the
\ stalker's grab on the plank, the statue (the hall's own: story/places/bridge.fs); the map's pages.
\ A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-2f-14.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-2f-14
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit story.places.bridge ;

own-reset

\ ---- as she comes in ----
create holes  $13 , $124 , $6B , $69 , $54 ,   \ (things lying about fall through)
: came-in ( exit -- )
    drop  $2B $1DF $140 far-side!  room-sounds  hall-track 1e background  medallion-sparkle
    5 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- the first time in: the screen back, the crows' file ----
: first-in ( -- )
    yield  camera-restart  world-held state-off  15 screen:from-black with-music or fade
    found-file-227 fact!  0 prepare-exit  wait-fade  $227 give-file  stalkers-stay state-off ;

\ ---- every frame ----
create cameras  2 , 2 , 2 ,   3 , 1 , 1 ,   $B , 0 , 0 ,   $C , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at
    fiona 2 entered? if  3 map-page  then   fiona 3 entered? if  4 map-page  then
    hewie-by-the-gap  medallion-notice  medallion-look 1 hewie-looks-at  climb-spot  bridge-spots ;
: person-in ( i -- )
    case
        fiona-p of
            0 0 0 way-in
            found-file-227 fact? 0= if
                fiona $4D -45 place-on-tri  hewie activate  hewie-in? if  hewie $120 -45 place-on-tri  then
                0 0 player-camera  camera-restart  ['] first-in start drop
            then  endof
        hewie-p of  hewie-comes-in  endof
    endcase ;
: went ( -- )  hewie-as-she-goes ;

' came-in ' person-in ' each-frame ' went castle-2f-14 place!
