\ story/places/water-tower-7f-1.fs - the Water Tower's 7F, the hall of the planets ($C5): eight
\ plates, one under each planet, lit in turn from Saturn's (the eighth, lit as she comes in) down to
\ the middle one - each lit while she or Hewie stands on the one before (left: dark again down to
\ the last one still stood on); each plate stepped onto rings; Hewie sent to the next one when she
\ points him at it, and he sits on any he reaches; all lit - the scene: the way on opened, the door
\ behind her closed for good, Hewie gone ahead; the clock; the inscription; hunted while she's here;
\ its four sounds in turn; the replay. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/water-tower-7f-1.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.water-tower-7f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    items.names story.kit fiona.anims scenes ;

0 constant inscription-msg   \ "All life on the surface of the planet stretches out below the stars' radiant light. All..."
own: her-plate   own: his-plate      \ the planet whose plate she, he stands on (1..8; 0 none) this frame
own: her-was     own: his-was        \ ... last frame
own: gear                            \ the planet to light next (8 down to 1: those above it lit)
own: lined-up                        \ all lit: the scene begun
own: hewie-sat                       \ the plates Hewie has sat on and not left (bit k: planet k)
5 constant first-ambient   1 constant glow-sound   18 constant ring-sounds   \ (planet k's ring: 18 - k)
$30 constant hall-track   $80 constant hewie-sit-act   $204 constant hewie-walk-anim
$D constant clock-area   $E constant inscription-area   $FF constant way-on-door   $101 constant behind-door
1 constant solved-movie   0 constant solved-cutscene   $33C constant glows-out-frame   $3E constant solved-seen

\ ---- the planets: each plate's middle; Hewie's zone on it (sitting, and left past a wider one); the
\ point she aims him by; his spot there (a triangle, facing) ----
create plates   0e f, 0e f,   -22e f, 0e f,   8e f, -30e f,   34.65e f, 20e f,
    -25e f, 43.31e f,   -40.37e f, -40.38e f,   48.58e f, -48.58e f,   57.25e f, 57.25e f,
create sit-radii  10 , 12 ,  4 , 6 ,  4 , 6 ,  5 , 7 ,  5 , 7 ,  4 , 6 ,  4 , 6 ,  5 , 7 ,
create aims  0 , 0 ,  -22 , 0 ,  8 , -30 ,  35 , 20 ,  -25 , 43 ,  -40 , -40 ,  49 , -49 ,  57 , 57 ,
create runs  $D8 , 89 ,  4 , 89 ,  $6A , -44 ,  $3F , -151 ,  $1B , 111 ,  $FA , 10 ,  $5C , -84 ,  $37 , -175 ,
: plate@ ( k -- ) ( F: -- x z )  1- 2* floats plates +  dup f@  float+ f@ ;
: pair@ ( k table -- a b )  swap 1- 2* cells +  dup @  swap cell+ @ ;
: planet ( k -- addr len )  1+ this-room @ story.strings:room-string ;   \ (its object: earth .. saturn)

\ ---- the glow under planet k: made the first time it's lit (effect k), its ring of light as it
\ comes on (not while a scene plays); off, fading ----
: glow ( on k -- )
    {: on k | a n :}  k planet to n to a  a n placed-here? 0= if  exit  then
    k live-effect nip 0= if
        on 0= if  exit  then
        8 k -  a n place-x object@ 0e a n place-z object@  effects:object-glow  k effect!
    then
    on  k effect @ send effect-command
    on  cutscene-active? 0= and if  glow-sound room-bank  a n place-x object@ a n place-y object@ a n place-z object@  sound-there  then ;
: glow-on ( k -- )  1 swap glow ;   : glow-off ( k -- )  0 swap glow ;
: all-glows ( on -- )  {: on :}  1 8 do  on i glow  -1 +loop ;   \ (Saturn's down to the middle's)

: came-in ( exit -- )
    drop  room-sounds  hunted state!  hall-track 1e background
    8 gear !  8 glow-on
    stalker let-go  1 summon-take
    wt7f-1-flag1 fact? 0= if  wt7f-1-flag1 fact!  1 s" creatures" actor-named send creatures-clear  then
    1 $2300 reverb ;

\ ---- the plates: who stands where; each one stepped onto rings ----
: on-plate ( id -- k )   \ (its centre on it: the middle's, overridden by the second's; else the first of the rest)
    {: id | k :}  0 to k
    id 1 plate@ 10e centre-across? if  1 to k  then
    id 2 plate@ 5e centre-across? if  2 to k  then
    k 0= if  9 3 do  id i plate@ 5e centre-across? if  i to k  leave  then  loop  then
    k ;
: stood-on? ( k -- flag )  dup her-plate @ =  swap his-plate @ = or ;
: rings ( -- )   \ (she stepped onto one from none; else he did)
    her-was @ 0=  her-plate @ and if  ring-sounds her-plate @ - room-bank fiona sound-by exit  then
    his-was @ 0=  his-plate @ and if  ring-sounds his-plate @ - room-bank hewie sound-by  then ;
: sat-off ( k -- )  1 swap lshift invert  hewie-sat @ and  hewie-sat ! ;
: sat? ( k -- flag )  1 swap lshift  hewie-sat @ and 0<> ;

\ ---- all lit: the scene - the glows out at its cue; after, the way on opened, the door behind her
\ closed off, Hewie gone ahead to the next room, on by the forced way ----
: glows-out ( -- )  begin  cutscene-frame glows-out-frame < while  yield  repeat  false all-glows ;
: solved-scene ( -- )
    solved-movie scenes:keyed-movie solved-cutscene load-scene drop ;
: solved ( -- )
    stalkers-stay state!  fiona wait-move  no-track 1e background
    15 screen:to-black-2 with-music or fade  solved-scene  wait-fade
    things-clear  fiona -1 idle  water-tower-7f-2 preload  $10 $FF movie-key
    ['] glows-out start drop  panic-eased  -1 run-scene
    50 trust+  world-held state!
    way-on-door reopened  behind-door closed-off  exits-redone
    water-tower-7f-2 -1 0 hewie-to  solved-seen resident-flag-set  forced-exit take-exit ;
: replayed ( -- )
    fiona hold  true all-glows  solved-scene  $10 $FF movie-key  ['] glows-out start drop  panic-eased  -1 run-scene
    hewie let-go  replay-over ;

\ ---- the next one lit while the one before is stood on; the last lit left: dark down to the
\ highest still stood on (the original clears Hewie's sitting there, and at the end Saturn's - and
\ Uranus's, unless only Uranus went dark) ----
: falls-back ( -- )
    gear @ {: g :}  g glow-off
    9 g 2 + ?do  i stood-on? if  i 1- gear !  i 1- sat-off  unloop exit  then  i 1- glow-off  loop
    g 7 < if  7 sat-off  then  8 sat-off  8 gear ! ;
: lights ( -- )
    gear @ {: g :}
    g 8 = if  8 stood-on? if  7 gear !  7 glow-on  then  exit  then
    g 1+ stood-on? 0= if  falls-back exit  then
    g stood-on? 0= if  exit  then
    g 1 = if  -1 lined-up !  panic-eased  ['] solved fiona start-holding drop  exit  then
    g 1- gear !  g 1- glow-on ;

\ ---- Hewie: sent to the next plate as she points him at it; sitting on any he reaches ----
: runs-to ( -- )   \ (an activity: a bark, over to it, settled)
    hewie hold  hewie wait-move  hewie-bark  hewie wait-move
    gear @ {: k :}  hewie  k runs pair@  hewie-walk-anim walk-move  k plate@  go-to
    hewie still-anim-move 0 10  0e 0e 0e 0e  move  10 frames ;
: sends-him ( -- )   \ (she told him to go there, facing the plate to light next - not standing on it)
    started-now @ go-there-cmd = 0= if  exit  then
    her-plate @ gear @ = if  exit  then
    gear @ aims pair@ s>f s>f fswap  fiona 50 faces-point?  hewie-breakable-now @ and if  ['] runs-to start drop  then ;
: sits ( -- )   \ (on a plate - not chased, at her command - he sits; past its wider zone he has left it)
    hewie-in?  hewie-cond-now @ well-cond = and 0= if  exit  then
    9 1 do
        i sat? 0= if
            chased? 0=  hewie-cmdable-now @ and  hewie i plate@ 0e fswap  i sit-radii pair@ drop s>f 5e  centred-in? and if
                1 i lshift hewie-sat @ or hewie-sat !  hewie-sit-act 0 hewie-do
            then
        else
            hewie i plate@ 0e fswap  i sit-radii pair@ nip s>f 5e  centred-in? 0= if  i sat-off  then
        then
    loop ;

\ ---- what she may do or say here ----
: the-inscription ( -- )
    stalkers-stay state!  fiona wait-move  fiona 0e -105e turn-to-xz  fiona look-close-anim start-anim  40 frames
    15 screen:to-dim fade wait-fade  inscription-msg say  15 screen:from-dim fade wait-fade  fiona wait-anim
    stalkers-stay state-off ;
: spots ( -- )
    fiona clock-area 50 faces-area? if  ['] save-clock calm-hewie-up offer  then
    fiona inscription-area 50 faces-area? if  ['] the-inscription unhurried offer  then ;

\ ---- every frame ----
create cameras  4 , 0 , -1 ,  5 , 1 , -1 ,  6 , 1 , -1 ,   \ (area, set, path)
: puzzle ( -- )
    her-plate @ her-was !  his-plate @ his-was !
    fiona on-plate her-plate !  hewie on-plate his-plate !
    rings  lights  sends-him  sits ;
: each-frame ( -- )
    1 1 leaves-by  cameras 3 cameras-at
    room-sounds-in? if  first-ambient -127e 1.7e 10e ambient-sounds  then
    lined-up @ 0= if  puzzle  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    1 0 -1 way-in ;
: went ( -- )  hunted state-off ;

' came-in ' person-in ' each-frame ' went water-tower-7f-1 place!
