\ story/places/off-map-5a.fs - room $5A (on no map page). The room has no data on the disc (no
\ ST_05A.PAC): it is never entered, its messages' texts aren't known, and its dials' own steps (the
\ original's Room5A_Cmd00) are a stub here - so its scripts are kept as the executable has them, for
\ completeness: its four ways (the rooms beyond loaded ahead as she nears them); brought in by the
\ forced way - at exit 1, faded in, a sound; the dials (her question; at them the world frozen, the
\ close camera, the board up while she turns them; after, a look up at a sound); before exit 1's
\ shut door once the scene is over, a remark; the wall's remarks in turn; leaving with the stalker
\ down out of sight, he is let go and taken. (Its act 0 - an item used, another given - and act 4
\ are never started by its scripts.) A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-5a.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.off-map-5a
USING: engine game-state actors messages common flag-names facts room-names story.kit fiona.anims ;

own-reset
own: scene-done      \ its scene played (brought in)
own: first-remarked  \ the wall's first remark made (the second next)
own: board-up        \ the dials' board up (their steps turning)
own: looked-up       \ ? she looks up after it (a sound)

0 constant door-msg   1 constant dials-msg   2 constant board-msg   3 constant wall-msg   4 constant wall-2-msg
$16 constant dials-area   $17 constant wall-area   $28 constant came-sound   $72 constant look-up-sound
: came-in ( exit -- )  drop ;

\ ---- brought in by the forced way ----
: brought-in ( -- )
    -1 scene-done !  15 screen:from-black with-music or fade  came-sound common-bank fiona sound-by  fiona wait-move ;

\ ---- the dials (their steps not known: the board down at once) ----
: dial-step ( -- )  0 board-up ! ;   \ (the original's Room5A_Cmd00 1 / 2 - a stub)
: the-dials ( -- )
    stalkers-stay state!  fiona wait-move
    dials-msg yes? if
        fiona $D2 180 293.136e -95e place-at  world-frozen state!  1 5e 0e 0e 2e shot
        board-msg show  -1 board-up !
        begin  dial-step  board-up @ while  yield  repeat
        text-gone  world-frozen state-off  shot-off
        looked-up @ if
            16 frames  look-up-sound common-bank fiona sound-by  fiona look-point-move 0 0  318e 10e 80e 0e  move  yield
            60 frames  fiona -1 look-at  yield
        then
    then
    stalkers-stay state-off ;
: the-wall ( -- )
    fiona wait-move  fiona -115e -75e turn-to-xz
    first-remarked @ if  wall-2-msg  0  else  wall-msg  -1  then  first-remarked !  say ;

\ ---- every frame ----
create entries  6 , -1 ,  3 , 2 ,  3 , 2 ,  2 , 1 ,   \ (by exit 0 .. 3: the camera)
create cameras  $A , 5 , 4 ,  $B , 6 , -1 ,  $C , 0 , 0 ,  $D , 5 , 4 ,  $E , 0 , 0 ,  $F , 1 , -1 ,  $10 , 1 , -1 ,
    $11 , 4 , 3 ,  $12 , 4 , 3 ,  $13 , 3 , 2 ,  $14 , 2 , 1 ,  $15 , 3 , 2 ,   \ (area, set, path)
create ahead  4 , 0 ,  5 , 1 ,  6 , 1 ,  7 , 2 ,  8 , 2 ,  9 , 3 ,   \ (area, exit loaded ahead)
: spots ( -- )
    fiona dials-area 50 faces-area? if  ['] the-dials unhurried offer  then
    1 exit-open? 0=  scene-done @ and  fiona 1 before-door? and if  door-msg any-time offer-remark  then
    fiona wall-area in-area?  fiona -90 50 heading? and if  ['] the-wall unhurried offer  then ;
: each-frame ( -- )  3 exits-out  3 3 leaves-by  cameras 12 cameras-at  ahead 6 ways-out  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 4 ways-in
    came-by @ forced-exit = if  fiona 1 at-exit  3 2 player-camera  ['] brought-in fiona start-holding drop  then ;
: went ( -- )   \ (the stalker down out of sight once the scene is over: let go, taken)
    scene-done @  stalker-in? 0= and  st-cond-now @ down-cond = and if  stalker let-go  2 summon-take  then ;

' came-in ' person-in ' each-frame ' went off-map-5a place!
