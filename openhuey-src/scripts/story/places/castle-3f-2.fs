\ story/places/castle-3f-2.fs - the 3F corridor of Belli Castle ($2C): once she has rested (the
\ Saturnus hint), the first time she steps on from the door, the door tried behind her (a knock
\ first if it's shut) and Daniella standing ahead of her - the door locked behind, Hewie brought by
\ her; Daniella kept from her alert after, until the old mansion's first file. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-3f-2.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-3f-2
USING: engine game-state actors messages common facts flag-names story-names room-names stalker-names
    story.kit fiona.anims ;

own-reset

\ ---- its own state (a visit) ----
own: arrive-step   \ Daniella's coming: she stands (1), the shot over (2)

\ ---- the room's things ----
2 constant to-exit-0-area   3 constant to-exit-1-area   4 constant by-the-door-area
$34 constant the-door   0 constant her-motions

: came-in ( exit -- )  drop ;

\ ---- the first time on from the door, once she has rested: Daniella ahead ----
: daniella-due? ( -- flag )  c1f-14-flag2 fact?  c3f-2-flag1 fact? 0= and ;
: stands ( -- )   \ (Daniella, there: standing, till the shot is over)
    stalker hold  stalker wait-move  stalker stalker.anims:stand-anim start-anim  1 arrive-step !
    begin  arrive-step @ 2 < while  yield  repeat ;
: daniella-ahead ( -- )
    c3f-2-flag1 fact!  stalkers-stay state!  fiona hold
    0 exit-open? 0= if  0 knock  16 frames  then
    0 door-tried  fiona wait-move  0 arrive-step !
    partner activate  castle-3f-2 46 stalker-to  his-own-time
    stalker $2E -180 0e 192e place-at  ['] stands start drop
    fiona uneasy-2-anim play  fiona -1 idle  7 frames
    begin  arrive-step @ 1 < while  yield  repeat
    fiona false shown  stalker 0 0 camera-set  stalker follow  1 20e 10e 180e 0e shot
    0 exit-open? if  the-door false open-mark  then  doors-redone  the-door lock  60 frames
    hewie-in? 0=  hewie 0 in-area? or if  castle-3f-2 43 0 hewie-to  hewie $2B 0 9.584e 178.014e place-at  then
    shot-off  fiona true shown  fiona follow  2 arrive-step !  stalkers-stay state-off ;

\ ---- every frame ----
create entries  0 , 0 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    2 exits-out
    fiona to-exit-0-area entered? if  daniella-due? if  partner her-motions motions-for  then  0 prepare-exit  then
    fiona to-exit-1-area entered? if  1 prepare-exit  then
    daniella-due?  fiona by-the-door-area left? and if  panic-eased  ['] daniella-ahead start drop  then
    c3f-2-flag1 fact?  found-file-233 fact? 0= and if  0 stalker-alert? 0= if  0 stalker-order  then  then ;
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-3f-2 place!
