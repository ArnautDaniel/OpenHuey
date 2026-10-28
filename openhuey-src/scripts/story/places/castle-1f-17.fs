\ story/places/castle-1f-17.fs - the specimen room in Belli Castle's 1F ($1B): the pendulums
\ swinging, a rug that was an animal, the anatomical model, the insect models, the pool table, the
\ lavender, the door to unlock from this side. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/castle-1f-17.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.castle-1f-17
USING: engine game-state actors messages common facts flag-names story-names room-names
    items.names effects story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant rug-msg            \ "Is this real? Poor little thing. Being stepped and walked on all day..."
1 constant model-msg          \ "It's an anatomically correct model of the human body. It looks so real. Almost as if it..."
2 constant insects-msg        \ "There are miniature insect models here, too."
3 constant insects-again      \ "What type of a person would decorate a room with these things?"
4 constant pool-table-msg     \ "Looks like an ordinary pool table."
5 constant pool-table-again   \ "So maybe whoever lives here likes to play pool while looking at all of these gross..."
6 constant why-here-msg       \ "Why would someone put something like this in here?"

\ ---- its own state (a visit) ----
own: insects-remarked
own: pool-remarked           \ the pool table's first remark made (in turn with the second)
own: shelf-noticed           \ Hewie has sat by the shelf (once a visit)

\ ---- the room's things ----
2 constant near-exit-0-area   3 constant near-exit-1-area   6 constant rug-area   7 constant model-area
8 constant insects-area   $C constant pool-table-area
0 constant lavender-fx   $14 constant room-track   1 constant locked-exit

\ ---- the pendulums: each its own period (frames) and swing (degrees), a phase apart ----
: pendulum ( k -- ) ( F: -- period swing )
    case  0 of  180e 5e  endof  1 of  210e 5e  endof  3 of  180e 10e  endof  4 of  240e 15e  endof  >r 360e 15e r>  endcase ;
: pendulum-rest ( k -- )
    dup hanging {: k a n :}  a n placed-here? 0= if  exit  then
    0e a n swing-phase object!  60e k s>f f* a n swing-offset object! ;
: pendulum-step ( k -- )
    dup hanging {: k a n :}  a n placed-here? 0= if  exit  then
    k pendulum {: F: period swing :}
    360e period f/ a n swing-phase object+!
    a n swing-phase object@ a n swing-offset object@ f+ 360e f< 0= if  -360e a n swing-phase object+!  then
    a n swing-phase object@ a n swing-offset object@ f+ deg>rad fsin swing f* deg>rad
    fdup pi f> if  2e pi f* f-  then  a n turn-y object! ;
: pendulums ( xt -- )  >r  0 r@ execute  1 r@ execute  3 r@ execute  4 r> execute ;   \ (the room's strings 0, 1, 3, 4)
: swinging ( -- )  begin  ['] pendulum-step pendulums  yield  again ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-track 1e background
    c1f-17-lavender fact? 0= if  lavender-fx -6e 8.5e -5e flicker-at  then
    ['] pendulum-rest pendulums  ['] pendulum-step pendulums  ['] swinging start drop ;

\ ---- what she may do or say here ----
: rug ( -- )  fiona wait-move  fiona look-low-anim 10 start-blend  rug-msg say  fiona wait-anim ;
: anatomy ( -- )
    fiona wait-move  fiona 0 turn  1 40e 20e 0e 0e shot
    fiona look-at-anim 10 start-blend  model-msg say  fiona wait-anim  shot-off ;
: insects ( -- )
    fiona wait-move  insects-remarked @ if  insects-again  else  -1 insects-remarked !  insects-msg  then  say ;
: pool-table ( -- )
    fiona wait-move
    pool-remarked @ if  0 pool-remarked !  pool-table-again say
    else
        -1 pool-remarked !  pool-table-msg say  fiona rest-anim play  fiona -1 idle
        why-here-msg show  7 frames  wait-closed
    then ;
: reach-lavender ( -- )   \ (up on the shelf: she reaches for it)
    fiona wait-move  fiona -6e -5e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  lavender c1f-17-lavender lavender-fx takes-up  fiona reach-back-anim play ;
: unlocks-it ( -- )  locked-exit unlocks ;
: spots ( -- )
    0 before-locked?  1 before-locked? or if  ['] unlocks-it any-time offer  then   \ (either exit: the act unlocks exit 1's)
    fiona rug-area in-area? if  ['] rug unhurried offer  then
    fiona model-area in-area?  fiona 0 50 heading? and if  ['] anatomy unhurried offer  then
    fiona insects-area in-area?  fiona -90 50 heading? and if  ['] insects unhurried offer  then
    fiona insects-area 1+ in-area?  fiona 180 50 heading? and if  ['] insects unhurried offer  then
    fiona insects-area 2 + in-area?  fiona 180 50 heading? and if  ['] insects unhurried offer  then
    fiona insects-area 3 + in-area?  fiona 90 50 heading? and if  ['] insects unhurried offer  then
    c1f-17-lavender fact? 0=  fiona -6e 8.5e -5e 8e 4e touches? and if  ['] reach-lavender calm-only offer  then
    fiona pool-table-area 50 faces-area? if  ['] pool-table unhurried offer  then ;

\ ---- Hewie: what he looks up at; once a visit, now and then, he sits by the shelf ----
create looks
    20.5e   0e  49.99e  20e 17e 17e look,
   -22.87e  0e  38.05e  20e 11e 11e look,
    28.49e  0e -29.82e  20e 13e 13e look,
    1.39e   0e -30.06e  20e 13e 13e look,
   -29.89e  0e -30.85e  20e 11e 11e look,
   -29.03e  0e  -7.64e  15e 11e 11e look,
: sits-by-shelf ( -- )  20.64e 0e 11.01e hewie-sits-by ;
: by-the-shelf ( -- )
    shelf-noticed @ if  exit  then
    hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and 0= if  exit  then
    hewie 20.64e 0e 11.01e 9e 10e centred-in? 0= if  exit  then
    -1 shelf-noticed !  30 chance? if  ['] sits-by-shelf start drop  then ;

\ ---- every frame ----
create cameras  4 , 0 , -1 ,   5 , 1 , -1 ,   \ (area, set, path)
create ahead  near-exit-0-area , 0 ,   near-exit-1-area , 1 ,
create entries  0 , -1 ,   0 , -1 ,             \ (by exit 0, 1: the camera)
: ways ( -- )  2 exits-out  cameras 2 cameras-at  ahead 2 ways-out ;
: each-frame ( -- )  ways  looks 6 hewie-looks-at  by-the-shelf  spots ;
: person-in ( i -- )  fiona-p = if  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-17 place!
