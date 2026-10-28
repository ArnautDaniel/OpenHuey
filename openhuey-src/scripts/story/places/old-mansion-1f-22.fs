\ story/places/old-mansion-1f-22.fs - a room of the old mansion's 1F with a cabinet to hide in (the
\ kit's: Hewie beside her; the stalker's grab, or his opening it). The scene's figure kept by its
\ near way; two candles; the Refined Antimony; the doorways' shade. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/old-mansion-1f-22.fs in the history (converted
\ from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-22
USING: engine game-state actors messages common vectors facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims ;

own-reset

0 constant antimony-fx   1 constant flame-fx   2 constant flame-2-fx
6 constant cabinet-area   $1C constant scene-figure   3 constant figure-slot
om1f-22-flag1 $10 2  $47 90  $47 90  $47 90  $4F 80
    144.89e 65.089e  140.97e 60.42e  144.89e 65.089e  134.56e 61.52e  cabinet: the-cabinet

: came-in ( exit -- )
    drop  room-sounds  the-cabinet cabinet-shown
    om1f-22-refined-antimony fact? 0= if  antimony-fx 147.17e 11e 86.89e flicker-at  then
    flame-fx 1 -36.7e 19.1e -34.8e flame-at  flame-2-fx 1 -36.7e 19.1e 34.8e flame-at
    1 $2300 reverb ;
: hides ( -- )  the-cabinet hides-in-cabinet ;
: stalker-came ( -- )  the-cabinet cabinet-stalker-came ;

\ ---- what she may take here ----
: take-antimony ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona 147.17e 86.89e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  refined-antimony om1f-22-refined-antimony antimony-fx takes-up  fiona reach-back-anim play ;

create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   \ (area, set, path)
create entries  0 , 0 ,   1 , 1 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 2 cameras-at
    fiona 2 entered? if  0 prepare-exit  scene-figure figure-slot 0 figure-in-by  then
    fiona 3 entered? if  1 prepare-exit  figure-slot figure-out  then
    fiona cabinet-area in-area?  fiona 90 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    om1f-22-refined-antimony fact? 0=  fiona 147.17e 11e 86.89e 5e 5e touches? and if  ['] take-antimony calm-only offer  then
    -4.5e 15e 110e  -4.5e 15e 79.8e  -4.5e -15e 110e  -4.5e -15e 79.8e  light-doorway
    -3e 3e 92.5e  -33.6e 3e 86.4e  -3e -20e 92.5e  -33.6e -20e 86.4e  light-doorway
    149.4e 25e 59.1e  159.4e 25e 59.1e  149.4e 0e 59.1e  159.4e 0e 59.1e  light-doorway ;
: person-in ( i -- )
    case
        fiona-p of  entries 2 ways-in  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-22 place!
