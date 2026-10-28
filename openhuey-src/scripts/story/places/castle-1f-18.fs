\ story/places/castle-1f-18.fs - the moth room in Belli Castle's 1F ($1C): moths swarm round the
\ lamp by the door (exit 1), shut till the Fresh Flowers are laid in the vase - the scene, the moths
\ drawn to the flowers instead; the torn picture, the clock, Hewie wary of the moths. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-18.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-18
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant empty-vase-msg      \ "An empty flower vase."
1 constant moths-msg           \ "Yuck! Look at all those moths! I'm not going there. Looks like they are attracted t..."
2 constant torn-picture-msg    \ "This picture has been ripped out and discarded. I'm no critic, but it doesn't seem..."
3 constant torn-again-msg      \ "Who would do such a thing?!"
4 constant moths-gone-msg      \ "Guess I'll have to chalk that up to another strange occurrence here. Moths that are..."
$229 constant moths-file       \ (filed as she first sees them)

\ ---- its own state (a visit) ----
own: torn-remarked          \ the torn picture's first remark made (in turn with the second)
own: hewie-wary             \ Hewie has gone to the moths (once a visit)
own: sound-started          \ the moths' sound begun (then kept going)

\ ---- the room's things ----
0 constant flowers-part   $26 constant lamp-door   0 constant lamp-floor   1 constant vase-floor
7 constant clock-area   8 constant picture-area   9 constant vase-area
0 constant moths-sound-id   1 constant moths-movie   0 constant moths-cutscene
$D2 constant moths-off-frame   $14A constant depth-frame
: laid? ( -- flag )  flowers-laid fact? ;
: frame-reached? ( frame -- flag )  cutscene-frame swap < 0= ;   \ (the scene's, the original's cue)
: nothing ( -- ) ;

\ ---- the moths: eleven swarms (the first a single moth, the rest 8 each), where they make for ----
11 constant #swarms
: swarm, ( F: x y z -- )  {: F: x y z :}  x f,  y f,  z f, ;
: swarm@ ( table k -- ) ( F: -- x y z )  3 * floats +  dup f@  dup float+ f@  2 floats + f@ ;
create at-lamp   \ round the lamp by the door (it shut)
    51e 20e -29e swarm,  51e 22e -30e swarm,  51e 18e -30e swarm,  51e 14e -30e swarm,
    51e 12e -30e swarm,  51e 8e -30e swarm,  51e 16e -26e swarm,  51e 24e -30e swarm,
    50e 20e -30e swarm,  48e 16e -30e swarm,  48e 12e -26e swarm,
create at-vase   \ round the flowers (laid)
    -30e 13e -40e swarm,  -30e 10e -40e swarm,  -31e 11e -39e swarm,  -30e 12e -41e swarm,
    -29e 14e -38e swarm,  -29e 13e -39e swarm,  -28e 12e -38e swarm,  -29e 13e -37e swarm,
    -27e 14e -40e swarm,  -29e 15e -40e swarm,  -27e 12e -41e swarm,
create at-scene   \ as the scene begins (the single one by the doorway)
    10e 20e -29e swarm,  52e 21e -32e swarm,  51e 18e -31e swarm,  52e 14e -32e swarm,
    53e 23e -28e swarm,  54e 19e -30e swarm,  53e 15e -31e swarm,  54e 16e -32e swarm,
    51e 13e -30e swarm,  52e 18e -31e swarm,  50e 13e -28e swarm,
: swarm-count ( k -- n )  0= if  1  else  0  then ;   \ (0: 8)
: moths-in ( table -- )  #swarms 0 do  dup i swarm@  i  i swarm-count  moths-at  loop  drop ;
: moths-off ( -- )   \ (the scene's: all for the flowers, the single one to the corner)
    #swarms 0 do  i  0  i if  at-vase i swarm@  else  -36e 11e -47e  then  moths-to  loop ;
: lamp-x ( F: -- x y z )  laid? if  -34e 0e -44e  else  55e 4e -30e  then ;   \ (what draws them: the lamp, the vase)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds
    lamp-door doors:door-locked? if
        at-lamp moths-in  lamp-floor keeps-people-off-2 floor-on  vase-floor keeps-hewie-off floor-on
    else
        flowers-part show-part  at-vase moths-in  vase-floor keeps-people-off-2 floor-on
    then ;

\ ---- the moths' sound: begun once the room's sounds are in, then kept going (with them, or at the vase) ----
: moths-heard ( -- )
    sound-started @ 0= if
        room-sounds-in? if  moths-sound-id lamp-x room-sound  -1 sound-started !  then  exit
    then
    laid? if  moths-sound-id -34e 0e -44e keep-sound  else  1 moths-sound-id moths-sound  then ;

\ ---- the flowers laid (Fresh Flowers used at the vase): the scene - the moths come over to them ----
: depth-opening ( -- )   \ (from frame $14A: near 1 .. 67.6, far 48.6 .. 230)
    cutscene-frame depth-frame - s>f {: F: t :}  t 1.4e f* 1e f+ 67.6e fmin {: F: near :}
    1e near near  t 4e f* 48.6e f+ 230e fmin  depth-range ;
: depth-follows ( xt -- )   \ (each frame xt; the depth range shut to 48.6 till frame $14A, then opening, while the scene runs)
    >r  begin  depth-frame frame-reached? 0= while  r@ execute  1e 1e 1e 48.6e depth-range  yield  repeat
    begin  0 frame-reached? while  r@ execute  depth-opening  yield  repeat  r> drop  wait-fade ;
: moths-to-vase ( -- )   \ (the scene's own: the moths as it begins, off for the flowers; their sound with them)
    at-scene moths-in  moths-off
    begin  moths-off-frame frame-reached? 0= while  yield  repeat
    moths-sound-id -34e 0e -44e keep-sound  ['] nothing depth-follows ;
: lays-flowers ( -- )
    fiona hold  stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move
    moths-movie scenes:unseen-movie moths-cutscene load-scene  wait-fade
    flowers-part show-part  ['] moths-to-vase start drop
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie hold  hewie false shown  then
    panic-eased  if  -1 run-scene  then
    fiona 0 idle  fiona $10B -177 -33.329e -29.383e place-at  camera-restart
    hewie true shown  hewie let-go
    fresh-flowers spend  withered-flowers spend  lamp-door unlock  flowers-laid fact!
    at-vase moths-in
    lamp-floor keeps-people-off-2 floor-off  vase-floor keeps-hewie-off floor-off  vase-floor keeps-people-off-2 floor-on
    15 fade-in  stalkers-stay state-off  100 trust+ ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    0= if  ['] lays-flowers start drop  true  else  false  then ;

\ ---- what she may do or say here ----
: sees-moths ( -- )   \ (stepping near them: turned to them, a shudder)
    fiona hold  fiona wait-move  fiona lamp-x fnip turn-to-xz
    fiona turn-round-anim play  fiona -1 idle
    laid? if  moths-gone-msg say  else  moths-msg say  moths-file give-file  then ;
: empty-vase ( -- )
    fiona $EC -130 -1 walk-move -32.523e -39.444e go-to  1 30e 10e 0e 0e shot
    fiona look-low-anim 10 start-blend  empty-vase-msg say  fiona wait-anim  16 frames  shot-off ;
: torn-picture ( -- )
    fiona wait-move
    torn-remarked @ if  0 torn-remarked !  torn-again-msg  else  -1 torn-remarked !  torn-picture-msg  then  say ;
: spots ( -- )
    laid? 0=  fiona vase-area 90 faces-area? and if  ['] empty-vase unhurried offer  then
    fiona-mode-now @ own-mode =  in-panic? 0= and  fiona lamp-x 10e 5e steps-into? and if  ['] sees-moths start drop  then
    fiona clock-area in-area?  fiona 90 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    fiona picture-area in-area?  fiona 45 50 heading? and if  ['] torn-picture unhurried offer  then ;

\ ---- Hewie wary of them: once a visit, near them, he goes over and stands tense (less likely once laid) ----
: stands-by-them ( -- )  lamp-x hewie-stands-by ;
: hewie-moths ( -- )
    hewie-wary @ if  exit  then
    hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and 0= if  exit  then
    hewie lamp-x 20e 50e centred-in? 0= if  exit  then
    -1 hewie-wary !  laid? if  30  else  100  then  chance? if  ['] stands-by-them start drop  then ;

\ ---- the movie replayed (the extras) ----
: moths-follow ( -- )   \ (their sound with the moths till they're off, then at the vase)
    moths-off-frame frame-reached? if  moths-sound-id -34e 0e -44e keep-sound  else  1 moths-sound-id moths-sound  then ;
: replay-track ( -- )  at-scene moths-in  moths-off  ['] moths-follow depth-follows ;
: replayed ( -- )
    fiona hold  room-sounds  at-lamp moths-in
    moths-movie scenes:unseen-movie moths-cutscene load-scene
    flowers-part show-part  ['] replay-track start drop
    begin  room-sounds-in? 0= while  yield  repeat
    moths-sound-id 55e 4e -30e room-sound  1 moths-sound-id moths-sound
    panic-eased  if  -1 run-scene  then  replay-over ;

\ ---- every frame ----
create cameras  4 , 0 , -1 ,   5 , 1 , 0 ,   6 , 1 , 0 ,   \ (area, set, path)
create ahead  2 , 0 ,   3 , 1 ,
create entries  1 , 0 ,   0 , -1 ,             \ (by exit 0, 1: the camera)
: ways ( -- )  2 exits-out  cameras 3 cameras-at  ahead 2 ways-out ;
: each-frame ( -- )  ways  hewie-moths  moths-heard  spots ;
: person-in ( i -- )
    fiona-p = if  events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then  entries 2 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-18 place!
' items-used castle-1f-18 items-here
