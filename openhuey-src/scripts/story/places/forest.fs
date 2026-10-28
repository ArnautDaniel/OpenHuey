\ story/places/forest.fs - what the Chaos Forest's rooms share: its maze dealt anew (as she comes
\ in to a room never dealt, and once a visit as her panic sets in); the bushes of chaos-forest-4 ..
\ -7 - she crouches in one (L1, or her examine), lying low till she gets up, panics or is found;
\ the stalker coming in while she hides searches by the odds; their three hanging leaves swing as
\ she pushes through; the wisp of -9, -11, -12 - it hovers, and at a noise (or someone near) it
\ cries out, rises and is gone. The rooms' own cells follow these: `own-reset  forest-owns owns !`.
IN: story.places.forest
USING: engine game-state actors messages common facts flag-names story-names room-names stalker-names
    floor-flags story.kit fiona.anims fiona.sounds keys ;

own-reset

\ ---- its own state (a visit) ----
own: maze-dealt            \ the maze dealt this visit (as her panic set in)
own: was-panicking         \ she was in a panic last frame
own: rustled               \ the first leaf last rustled with the second sound (they go in turn)
own: wisp-up               \ the wisp hovering, waiting to be startled
owns @ constant forest-owns

\ ---- the maze (the six rooms $100 ..: their doors dealt from a row - while under 4 deals) ----
$1F constant deals-var
: deal-maze ( -- )  story.words:deal-things ;   \ (0xCE: the rooms dealt, a stalker in them sent on, the exits rebuilt)
: maze-in ( -- )   \ (as she comes in: never dealt - dealt now)
    deals-var var@ 0= if  deal-maze  then  in-panic? was-panicking ! ;
: maze-frame ( -- )   \ (her panic setting in: dealt anew, once a visit)
    maze-dealt @ 0=  was-panicking @ 0= and  in-panic? and if  -1 maze-dealt !  deal-maze  then
    in-panic? was-panicking ! ;

\ ---- a bush's three hanging leaves (the room's strings 0..2): pushed by her steps, swinging ----
1 constant rustle-sound   2 constant rustle-2-sound
: leaf-still ( a n -- )
    {: a n :}  a n placed-here? 0= if  exit  then
    0e a n swing-phase object!  0e a n swing-left object!  0.9e a n swing-pushed object! ;
: leaf-push ( first? a n -- )   \ (her step, squared, adds up - past 1, 20 frames of swing; not while it swings hard)
    {: first a n :}  a n placed-here? 0= if  exit  then
    a n swing-left object@ 8e f> if  exit  then
    her-step fdup f* a n swing-pushed object+!
    a n swing-pushed object@ 1e f> 0= if  exit  then
    20e a n swing-left object!  0e a n swing-pushed object!
    first if  rustled @ 0= dup rustled !  if  rustle-2-sound  else  rustle-sound  then  room-bank fiona sound-by  then ;
: leaf-swing ( a n -- )   \ (a frame of its swing: on 36 degrees, tilted 1 + sin of it)
    {: a n :}  a n placed-here? 0= if  exit  then  a n swing-left object@ f0> 0= if  exit  then
    a n swing-left object@ 1e f- 0e fmax a n swing-left object!
    36e a n swing-phase object+!  a n swing-phase object@ 360e f< 0= if  -360e a n swing-phase object+!  then
    a n swing-phase object@ deg>rad fsin 1e f+ deg>rad  fdup pi f> if  2e pi f* f-  then  a n turn-x object! ;
: leaves-still ( -- )  3 0 do  i hanging leaf-still  loop ;
: leaves-pushed ( -- )  3 0 do  i 0=  i hanging leaf-push  loop ;
: leaves-swing ( -- )  3 0 do  i hanging leaf-swing  loop ;

\ ---- the bush to crouch in (rooms 4 .. 7): where it is, the exit the stalker comes by to search it,
\ the story variable counting his searches, his spot to find her from ----
0 constant bush-group   4 constant hauled-anim
fvariable bush-x   fvariable bush-z   variable bush-exit   variable bush-var
variable find-tri   variable find-deg   fvariable find-x   fvariable find-z
: bush ( exit var tri deg F: x z fx fz -- )
    find-z f!  find-x f!  bush-z f!  bush-x f!  find-deg !  find-tri !  bush-var !  bush-exit ! ;
: in-bush? ( -- flag )  fiona bush-x f@ bush-z f@ 5e centre-across? ;
: from-his-way? ( -- flag )  bush-exit @ stalker-came-by? ;   \ (he came by the bush's side)
: out-of-bush ( -- )
    stalkers-stay state!  fiona false noclip  stalkers-stay state-off  bush-group solid-floor floor-off ;
: hauled-from-bush ( -- )   \ (found, or panicking: up and out, her eyes on him)
    found-her @ if  2 avoid-prompt  then
    fiona-hidden state-off  fiona follow  stalkers-stay state!
    fiona stalker look-at  yield  fiona hauled-anim $14 play-blend  fiona -1 look-at  yield  out-of-bush ;
: in-the-bush ( -- )
    fiona hold  stalkers-stay state!  bush-group solid-floor floor-on  0 came-while-hidden !  fiona true noclip  0 found-her !
    fiona wait-move  fiona crouch-down-anim play  fiona crouching-anim start-anim
    fiona-hidden state!  stalkers-stay state-off  0 avoid-prompt
    begin
        circle button-pressed? if
            fiona-hidden state-off  fiona follow  stalker let-go
            chased? if  hauled-from-bush exit  then
            stalkers-stay state!  fiona crouch-up-anim play  out-of-bush exit
        then
        in-panic? found-her @ or if
            in-panic? 0= if  seized-sound common-bank fiona sound-by  then  hauled-from-bush exit
        then
        hidden-calm  yield
    again ;
: bush-found ( -- )  stalker hold  find-tri @ find-deg @ find-x f@ find-z f@ finds-her-at ;
: bush-search ( -- )   \ (the camera on him; whether he finds her - by the odds, a creature moving about - counted)
    stalker follow
    bush-var @ mansion-odds chance?  2 creature-moving? or if  ['] bush-found start drop  else  120 summoner-sooner  then
    bush-var @ var@ 1+ bush-var @ var! ;
: bush-searched ( -- )   \ (he came in by the bush's side as she hides)
    from-his-way? fiona-hidden state? and dup came-while-hidden ! if  bush-search  then ;
: bush-frame ( -- )   \ (each frame: L1 by it to crouch, her examine; the leaves)
    in-panic? 0=  fiona held? 0= and  l1 button-pressed? and  in-bush? and
    stalker-in? 0=  from-his-way? 0= or and if  ['] in-the-bush fiona start-holding drop  then
    in-bush? if
        leaves-pushed
        in-panic? 0= if
            stalker-in?  from-his-way? and if  no-time-message offer-remark-now  else  ['] in-the-bush calm-only offer  then
        then
    then
    leaves-swing ;

\ ---- the wisp (kind $E in slot 5): hovering, now one way now the other; startled, it cries out
\ (the room's sound 10), rises (dy a frame, 3 along) and is gone ----
5 constant wisp-slot   $E constant wisp-kind   10 constant wisp-cry
$9000 constant wisp-drift   $9001 constant wisp-drift-2   $9002 constant wisp-startled
fvariable wisp-x   fvariable wisp-y   fvariable wisp-z   fvariable wisp-rise
: wisp ( -- id )  wisp-slot figure ;
: hovers ( -- )
    wisp 0< if  yield  then  -1 wisp-up !  wisp-slot figure-on  wisp-slot wisp-drift 1 0 figure-play
    wisp-slot wisp-x f@ wisp-y f@ wisp-z f@ 0e figure-at
    begin
        wisp-slot 50 chance? if  wisp-drift  else  wisp-drift-2  then  1 0 figure-play
        wisp-slot figure-wait
    again ;
: wisp-in ( F: x y z dy -- )   \ (as she comes in, not yet startled)
    wisp-rise f!  wisp-z f!  wisp-y f!  wisp-x f!  wisp-kind wisp-slot figure-in  ['] hovers start drop ;
: flees ( -- )
    wisp-cry room-bank wisp sound-by  wisp-slot wisp-startled 1 0 figure-play  8 frames
    89 0 do  wisp-slot 0e wisp-rise f@ 3e figure-shift  yield  loop
    wisp-slot figure-out ;
: startled ( fact -- )   \ (it flies: its fact set)
    fact!  0 wisp-up !  ['] hovers stop-doing  ['] flees start drop ;

\ ---- a rabbit (kind $1A in slot 4) now and then: off at once into the trees ----
4 constant rabbit-slot   $1A constant rabbit-kind   $9000 constant rabbit-sits   2 constant rabbit-feet
: runs-off ( tri deg F: x z tx tz -- )
    {: tri deg F: x z tx tz :}  rabbit-slot figure 0< if  yield  then
    rabbit-slot figure-on  rabbit-slot figure dup 0< if  drop exit  then  tri deg x z place-at
    rabbit-slot rabbit-sits 1 0 figure-play  rabbit-slot rabbit-feet tx tz figure-goes  rabbit-slot figure-out ;
