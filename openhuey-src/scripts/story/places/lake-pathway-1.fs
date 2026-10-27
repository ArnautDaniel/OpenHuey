\ story/places/lake-pathway-1.fs - the lake pathway by the fallen body ($C8): the path collapsed
\ into the lake until the way across is open (the books' second event, c7-second-event, and the
\ bridge not yet gone); the body she remarks on, the collapsed path, the lavender by the water,
\ Hewie's look at the drop. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/lake-pathway-1.fs (converted from the game's, not loaded).
IN: story.places.lake-pathway-1
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant twisted-msg     \ "His limbs are twisted into unnatural positions. It looks so painful... But I gu..."
1 constant the-fall-msg    \ "No one could withstand a fall like that. No one."
2 constant leathery-msg    \ "His skin... It's so dry and leathery. This body... There is something that just doe..."
3 constant collapsed-msg   \ "The path has collapsed... So, all of this was on top of a lake."
4 constant the-light-msg   \ "There's a light coming from the other side of the lake. I've got to find a way to g..."

\ ---- its own state (a visit) ----
own: body-remarked         \ the first of the body's remarks made (the next: up close, then the first again)
own: path-remarked         \ the collapsed path's first remark made (in turn with the second)
own: drop-noticed          \ Hewie has looked at the drop

\ ---- the room's things ----
0 constant way-part   0 constant path-group   \ (the way across; the floor where the path fell in)
0 constant to-exit-0-area   1 constant to-exit-1-area   2 constant near-exit-0-area   3 constant near-exit-1-area
4 constant by-body-area   5 constant body-side-area   6 constant path-end-area
0 constant lavender-fx
$18 constant lake-track
: way-open? ( -- flag )  c7-second-event fact?  over-music-d fact? 0= and ;

\ ---- as she comes in ----
: water-light ( -- )  0 $40404040  400e 27.1e -125e  1000e 0.5e 0e  effects:caustic drop ;   \ (turning)
: came-in ( exit -- )
    drop  lake-track 1e background
    way-open? if  way-part show-part  else  path-group barred-floor hole-floor or floor-on  then
    lp-1-lavender fact? 0= if  lavender-fx 19.59e 39e -247.61e flicker-at  then
    water-light ;

\ ---- what she may do or say here ----
: the-body ( -- )   \ (by it, the first remark - or turned to the drop, the other; then up close)
    fiona wait-move
    body-remarked @ if
        fiona pick-up-anim play  leathery-msg say  fiona up-from-pick-anim play  0 body-remarked !
    else
        fiona by-body-area in-area? if  twisted-msg show  fiona look-low-anim play
        else  fiona 180 turn  the-fall-msg show  fiona look-at-anim play  then
        fiona -1 idle  7 frames  wait-closed  -1 body-remarked !
    then ;
: the-path ( -- )
    fiona wait-move
    path-remarked @ if
        0 path-remarked !  fiona look-close-anim start-anim  40 frames  the-light-msg say  fiona wait-anim
    else  -1 path-remarked !  collapsed-msg say  then ;
: pick-lavender ( -- )  lavender lp-1-lavender lavender-fx 19.59e -247.61e picks-up ;
: spots ( -- )   \ (by the books' second event the body is there to see; before it, the collapsed path)
    c7-second-event fact? if
        fiona by-body-area in-area?  fiona 0 50 heading? and if  ['] the-body unhurried offer  then
        fiona body-side-area in-area?  fiona 90 50 heading? and if  ['] the-body unhurried offer  then
    else
        fiona path-end-area in-area?  fiona 0 50 heading? and if  ['] the-path unhurried offer  then
    then
    lp-1-lavender fact? 0=  fiona 19.59e 39e -247.61e 5e 5e touches? and if  ['] pick-lavender calm-only offer  then ;

\ ---- Hewie: the drop where the path fell in - now and then he barks at it ----
: barks-at-drop ( -- )  -4.8e 38e -171.89e hewie-barks-at ;
: hewie-looks ( -- )
    c7-second-event fact?  drop-noticed @ or if  exit  then
    hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and 0= if  exit  then
    hewie -4.8e 38e -171.89e 53e 15e centred-in? 0= if  exit  then
    -1 drop-noticed !  30 chance? if  ['] barks-at-drop start drop  then ;

\ ---- every frame: its ways out ----
create ahead  near-exit-0-area , 0 ,   near-exit-1-area , 1 ,   \ (area, exit loaded ahead)
: ways ( -- )  to-exit-0-area 0 leaves-by  to-exit-1-area 1 leaves-by  ahead 2 ways-out ;
: each-frame ( -- )  ways  hewie-looks  spots ;
: player-came ( -- )   \ (by exit 0 the view along the path - the other while the way across is open)
    way-open? if  0 0 0 way-in  else  0 1 1 way-in  then  1 0 0 way-in ;
: person-in ( i -- )  fiona-p = if  player-came  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went lake-pathway-1 place!
