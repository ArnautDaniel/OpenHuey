\ story/places/house-of-truth-b1-9.fs - the House of Truth's B1 ($97) where the dark takes her: one
\ step in and she is in the white room again (Lorenzo let go, the B1's track); its door kept from
\ Hewie and the stalker; his slam's shake. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/house-of-truth-b1-9.fs in the history (converted from the game's; gone
\ with the old machinery in 0.2.20).
IN: story.places.house-of-truth-b1-9
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    story.kit story.places.house-of-truth ;

own-reset  truth-owns owns !

own: taken                 \ the dark has taken her (once a visit)
$F3 constant dark-door
create cameras  8 , 0 , 0 ,  9 , 2 , 2 ,   \ (area, set, path)
: came-in ( exit -- )
    drop  0 solid-floor floor-on  2 keeps-hewie-off keeps-stalkers-off or floor-on
    dark-door stalker-slot true door-for  dark-door hewie-slot true door-for ;
: into-the-dark ( -- )   \ (to the white room by the forced way)
    fiona hold  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    world-held state!  stalker let-go  1 summon-take  colour-track 1e background  forced-exit take-exit ;
: each-frame ( -- )
    taken @ 0=  fiona 0 in-area? and if  -1 taken !  ['] into-the-dark fiona start-holding drop  then
    1 1 leaves-by  cameras 2 cameras-at
    fiona 4 entered? if  house-of-truth-b1-4 preload  then
    fiona 5 entered? if  1 prepare-exit  then
    slam-shake ;
: person-in ( i -- )  fiona-p = if  1 2 2 way-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-9 place!
