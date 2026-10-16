\ things.fs - the things lying about (docs/subsystems/things.md; src/game/placed.c, the original's
\ placed things): what Fiona throws, drops or sets down from her equipment's slot 2, and what
\ becomes of it. Their motion and drawing are the engine's (game/things.c); here, what they do:
\ - the ball (kind 0): bounces and rolls, kicked by her blows; gone after 3 minutes - back in her
\   items if she has none;
\ - the powders (1 antimony, 4 refined, 6 the tube): burst on what they hit (a splash); a stalker
\   treading on one is hurt (5 / 10 / 20);
\ - the magnesias (2, 5 refined, 7 prima): set down; armed after a while (30 s, 1 min, 2 min) they
\   go off - a hard blow (15 / 30 / 45, startling him maybe) to the stalker within reach; the
\   stalker or a homunculus treading on one sets it off at once; homunculi within reach are struck;
\ - the doll (3): lies there 5 minutes; Debilitas is drawn to it (its noise), and takes it;
\ - the stone (8): after 15 s a noise that draws the stalker, and gone.
\ The kinds 0, 2, 3, 5, 7, 8 are kept in the save; ten of a kind at most (the oldest goes).
IN: things
USING: engine game-state actors messages common facts flag-names items effects fiona.slam stalker-names ;

128 constant #things

state: things-state
  #things cells field kicked   \ each one kicked this touch (her blow's point near it)
  #things cells field met-in   \ (the doll) the room the stalker last came upon it in (-1 none)
  3 cells field homunculi      \ the homunculi's actors (slots 7..9, as they tell: creature-doing)
  cell field st-kind  cell field st-alert   \ (the stalker as he tells: his kind, his mode)
  cell field st-room  cell field st-told     \ (his room; frames since he told: 0 this one)
  #things cells field keris   \ (kind 9) the room object it carries: "keri0".."keri5" (-1 none)
end-state

\ ---- the one looked at ----
variable it
: kind ( -- n )  it @ thing-info 2drop 2drop ;
: its-room ( -- room )  it @ thing-info 2drop drop nip ;
: its-tri ( -- tri )  it @ thing-info 2drop nip nip ;
: age ( -- n )  it @ thing-info >r 2drop 2drop r> ;
: events ( -- bits )  it @ thing-events ;
: at ( -- v )  it @ thing-at ;
: here? ( -- flag )  its-room room-id = ;
\ the ball gone (its time, a hole): hers again, if she has none and no other lies about (Ball_dtor)
: balls ( -- n )  0  #things 0 do  i thing-used? if  i thing-info 2drop 2drop 0= if  1+  then  then  loop ;
: ball-back ( -- )
    balls 0=  $90 items-of 0= and if  $90 1 s" items" actor-named send give-item  then ;
: gone ( -- )  kind  it @ thing-free  0= if  ball-back  then ;
: thing-sound ( id -- )  5 0 0 at vec@ sound-at ;
: running? ( -- flag )   \ (Thing_Frame: the game going - no scene has the camera, the world not held)
    cam-scene? 0=  world-held state-flag? 0= and ;

\ ---- by kind ----
: kept? ( k -- flag )  dup 0= over 2 = or over 3 = or over 5 = or over 7 = or swap 8 = or ;
: powder? ( k -- flag )  dup 1 = over 4 = or swap 6 = or ;
: magnesia? ( k -- flag )  dup 2 = over 5 = or swap 7 = or ;
: lasts ( k -- frames )  case  0 of 5400 endof  3 of 9000 endof  10 of 450 endof  >r 0 r>  endcase ;   \ (0: no end)
: powder-damage ( k -- n )  case  1 of 5 endof  4 of 10 endof  >r 20 r>  endcase ;
: powder-stun ( k -- n )  case  1 of 4 endof  4 of 6 endof  >r 8 r>  endcase ;   \ (how long he's held: the stun's loops - thing_stomped's b)
: powder-colour ( k -- rgba )  case  1 of $402080FF endof  4 of $806400FF endof  >r $808080FF r>  endcase ;
\ the magnesias' settings (thing_setup): startle %, damage, armed at, reach across
: startle ( k -- % )  case  2 of 20 endof  5 of 50 endof  >r 100 r>  endcase ;
: damage ( k -- n )  case  2 of 15 endof  5 of 30 endof  >r 45 r>  endcase ;
: armed-at ( k -- frames )  case  2 of 900 endof  5 of 1800 endof  7 of 3600 endof  >r 450 r>  endcase ;
: reach ( k -- n )  case  2 of 10 endof  5 of 15 endof  >r 20 r>  endcase ;

\ ---- whom it reaches ----
: stalker ( -- id | -1 )  stalker-in-room ;   \ (in the room being played)
: treads? ( body margin -- flag )   \ standing on it: within its radius (+ margin), 1 high
    over dup 0< swap body? 0= or if  2drop false exit  then
    over body-dims fdrop s>f f+  it @  1e thing-reaches? ;
: in-reach? ( body -- flag )   \ (a magnesia's: across, 10 high)
    dup 0< if  drop false exit  then  it @  kind reach s>f  10e thing-reaches? ;
: homunculus ( slot -- id )  7 - cells homunculi + @ ;
: homunculi-tread? ( -- flag )
    false  10 7 do  i homunculus 0 treads? or  loop ;

\ ---- their effects ----
: prima? ( -- flag )  kind 7 = ;
: go-burst ( -- )   \ (a magnesia going off: the plain one the shove's puffs, the refined and prima their own bursts)
    kind 2 = if  at vec@ puffs exit  then
    prima?  at vec@  its-tri tri-normal  effects:mag-burst ;
: powder-splash ( -- )   \ (DropSplash: the powders' colour, and its sound)
    8 2 kind powder-colour  at vec@  at vec@  splash  $8D thing-sound ;
: dust ( -- )  2 $50 $50 $50 16  at vec@  burst ;   \ (the doll glancing off a wall: a grey puff)
: heard ( loud -- )   \ (a noise at it the stalker hears: Noise_Make)
    its-room its-tri -1 world-noise  s" acoustics" actor-named send noise ;
: blow-him ( id kind damage startle% -- )   \ (Progress_Noise to him: his request 4 from where it lies - in whatever room)
    {: id k d pct :}  id 0< if  exit  then
    k d  rnd 100e f* f>s pct < if  $8000  else  0  then  at vec@ xyz>cells  id send blow-from ;
: blow-stalker ( kind damage startle% -- )  >r >r >r  stalker r> r> r> blow-him ;   \ (the one in her room)
variable blow-n   -1000 blow-n !   \ (its blows to the homunculi: their own numbers)
: blow-homunculi ( damage reach -- )   \ (ThingShared_ReachStalkers: as one of her blows there)
    {: d r :}  blow-n @  d  at sf@ f>cell  at 4 + sf@ f>cell  at 8 + sf@ f>cell  r s>f f>cell
    -1 blow-n +!  broadcast fiona-blow ;

\ ---- her kick (thing_kick_up: her blow's point within 5 kicks it her way - 2 ahead, up - once
\ a touch) ----
: up ( -- ) ( F: -- up )  kind case  9 of 0.6e endof  10 of 0.4e endof  >r 2e r>  endcase ;
: kicked? ( -- addr )  it @ cells kicked + ;
: kick ( -- )
    s" fiona" actor-named dup 0< if  drop exit  then
    dup body-point 0= if  drop  0 kicked? ! exit  then          ( her v )
    kicked? @ if  2drop exit  then
    at vec-dist 5e f> if  drop exit  then
    -1 kicked? !
    0e 2e body-yaw rotate-by  up fswap  it @  thing-kick ;

\ ---- the story's own (house-of-truth-1f-9): kind 9 carries a room object "keri<k>" with it
\ (Thing09_Draw) - free again (hidden) once it has fallen through, and the story told; kind 10
\ is unseen, a trap: the stalker treading on it is knocked down (Thing10_Frame: a blow of kind
\ 2, $28; its sound 4) and it is gone ----
: keri$ ( k -- addr len )
    case  0 of  s" keri0"  endof  1 of  s" keri1"  endof  2 of  s" keri2"  endof
          3 of  s" keri3"  endof  4 of  s" keri4"  endof  >r s" keri5" r>  endcase ;
: keri ( -- addr )  it @ cells keris + ;
: carry-keri ( -- )
    keri @ dup 0< if  drop exit  then  keri$ {: a n :}
    at sf@ a n $20 placed-f!  at 4 + sf@ a n $24 placed-f!  at 8 + sf@ a n $28 placed-f! ;
: keri-gone ( -- )
    keri @ dup 0< if  drop exit  then  dup keri$ 0 1 placed-op  -1 keri !  broadcast keri-fell ;
: trap-frame ( -- )
    kick
    stalker 0 treads? if  4 6 0 0 at vec@ sound-at  2 $28 0 blow-stalker  gone  then ;

\ ---- each kind's frame ----
: quiet? ( -- flag )  $20 state-flag? ;   \ (progress flag $20: nothing set off)
: powder-frame ( -- )   \ (thing_stomped: the stalker treading on it - kind 7, its damage and his stun's loops; under flag $20 nothing, no kick)
    quiet? if  exit  then
    stalker 1 treads? if
        7 kind powder-damage kind powder-stun at vec@ xyz>cells  stalker send blow-from
        powder-splash  gone exit
    then  kick ;
: go-off ( -- )  $8E thing-sound  go-burst  gone ;
\ armed, it goes off wherever it lies (ThingShared_Armed): the stalker in its room - in her room
\ within its reach, elsewhere always (+0x122: 100) - has its blow (not under flag $20)
: him-in-its-room ( -- id | -1 )  stalker-in-game dup 0< if  exit  then  dup body-room its-room <> if  drop -1  then ;
: sets-him-off? ( -- flag )
    him-in-its-room dup 0< if  drop false exit  then
    here? if  in-reach?  else  drop true  then ;
: magnesia-frame ( -- )   \ (thing_watch)
    age kind armed-at >= if
        here? if  kind damage kind reach blow-homunculi  then   \ (ThingShared_ReachStalkers)
        sets-him-off? quiet? 0= and if  him-in-its-room 2 kind damage kind startle blow-him  then
        here? if  go-off  else  gone  then  exit
    then
    here? 0= if  exit  then
    quiet? if  exit  then
    homunculi-tread?  stalker 0 treads?  2dup or 0= if  2drop kick exit  then
    if  2 kind damage kind startle blow-stalker  then
    if  kind damage 0 blow-homunculi  then   \ (those standing on it struck: ThingShared_StompStalkers)
    go-off ;
: stone-frame ( -- )   \ (Thing08_Frame: armed, a noise the stalker hears - $4F at it - and gone)
    age 450 < if  kick exit  then
    here? if  0 thing-sound  3 $10 $10 $10 $80  at vec@  burst  then
    $4F heard  gone ;
\ the doll and Debilitas (Thing03_StalkerCheck): only him - kinds 2 6 7 $1B - and in its room
$1F constant doll-noise
: drawn-kind? ( -- flag )  st-kind @ dup 2 = over 6 = or over 7 = or swap $1B = or ;
: he-is-with-it? ( -- flag )  st-told @ 2 <  drawn-kind? and  st-room @ its-room = and ;
: met ( -- addr )  it @ cells met-in + ;
: after-her? ( -- flag )  st-alert @ 0= ;
\ away from Fiona: he comes upon it once a visit - hears it, and takes it unless he is after her
: doll-away ( -- )
    met @ st-room @ = if  exit  then  st-room @ met !
    doll-noise heard  after-her? 0= if  gone  then ;
\ in her room, while he searches (modes 1..3): it draws him (its noise, once; he goes for it if
\ he sees it and can get there); within 10 of him it is taken
: doll-near ( -- )
    st-alert @ 1 4 within 0= if  -1 met !  exit  then
    stalker dup 0< if  drop exit  then
    dup at swap body-at vec-dist 10e f< if  drop gone exit  then
    met @ its-room <> if  its-room met !  doll-noise heard  then
    its-tri swap send drawn-to ;
: doll-frame ( -- )
    he-is-with-it? if
        here? if  doll-near  else  doll-away exit  then
    then
    it @ thing-used? here? and if  kick  then ;
: one-frame ( -- )
    events 1 and if  powder-splash exit  then   \ (burst: freed next step)
    events 4 and if  kind 0= if  ball-back  then  kind 9 = if  keri-gone  then  exit  then   \ (through a hole: freed next step)
    events 2 and if  $7C thing-sound  then
    events 8 and if  dust  then
    kind lasts ?dup if  age <= if  gone exit  then  then
    kind 3 = if  doll-frame exit  then   \ (in any room: he may come upon it away from her)
    kind magnesia? if  magnesia-frame exit  then   \ (in any room: armed, it goes off there)
    here? 0= if  exit  then
    kind powder? if  powder-frame exit  then
    kind magnesia? if  magnesia-frame exit  then
    kind case
        8 of  stone-frame  endof
        9 of  carry-keri kick  endof
        10 of  trap-frame  endof
        >r kick r>
    endcase ;

\ ---- each frame: their motion (in the room being played), then what each does ----
: frame ( -- )
    room-id running? things-step
    running? 0= if  exit  then
    #things 0 do  i thing-used? if  i it !  one-frame  then  loop ;
: age@ ( i -- age )  thing-info >r 2drop 2drop r> ;

\ ---- a new one (her let go): ten of a kind at most (the kind 10: five) - the oldest goes
\ (PlacedThings_AnotherCame) ----
variable oldest  variable n-of
: one-of ( k i -- )   \ (counted, and the oldest kept)
    dup thing-used? 0= if  2drop exit  then
    dup thing-info 2drop 2drop rot <> if  drop exit  then
    1 n-of +!  oldest @ 0< if  oldest ! exit  then
    dup age@ oldest @ age@ > if  oldest !  else  drop  then ;
: make-room ( k -- )
    dup kept? over 10 = or 0= if  drop exit  then
    -1 oldest !  0 n-of !
    #things 0 do  dup i one-of  loop
    10 = if  5  else  10  then  n-of @ <= if  oldest @ thing-free  then ;

\ ---- the save: those kept, a line each (`lying`) ----
: .f ( F: r -- )  6 f>s$ type space ;
: write-thing ( i -- )
    {: i | k r t a :}  i thing-info to a drop to t to r to k
    k kept? 0= if  exit  then
    k . r . t . a .  i thing-at dup sf@ .f  dup 4 + sf@ .f  8 + sf@ .f  ." things:lying" cr ;
: write-things ( -- )
    ." things:clear-all" cr
    #things 0 do  i thing-used? if  i write-thing  then  loop ;
: clear-all ( -- )  things-clear ;
: lying ( kind room tri age -- ) ( F: x y z -- )   \ (a save read back: lying there)
    {: k r t a :}  0e 0e 0e  k r t thing-new  dup 0< if  drop exit  then  a swap thing-age! ;

behaviour keeping-things
  on spawned ( -- )  kicked #things cells 0 fill  homunculi 3 cells -1 fill  keris #things cells -1 fill
      met-in #things cells -1 fill  100 st-told !
      self subscribe tick  self subscribe write-save  self subscribe creature-doing  self subscribe stalker-doing ;
  on tick ( -- )  frame  st-told @ 100 < if  1 st-told +!  then ;
  on thing-made ( kind tri x y z vx vy vz -- )   \ (her let go: at her hand, its way)
      {: k tri x y z vx vy vz :}
      k make-room
      k room-id tri  x cell>f y cell>f z cell>f  vx cell>f vy cell>f vz cell>f  thing-new
      dup 0< 0= k 0= and  vx vz or 0<> and if  s" hewie" actor-named send fetch-this  else  drop  then ;   \ (thrown: his to fetch)
  on keri-drop ( k tri x y z -- )   \ (the story's: a kind-9 thing carrying "keri<k>", falling from there)
      {: k tri x y z :}
      9 room-id tri  x cell>f y cell>f z cell>f  0e 0e 0e  thing-new  dup 0< if  drop exit  then
      cells keris + k swap !  k keri$ 0 0 placed-op ;
  on trap-place ( tri x y z -- )   \ (the story's: a kind-10 trap set down there - five at most - smoke rising from it)
      {: tri x y z :}  10 make-room  10 room-id tri  x cell>f y cell>f z cell>f  0e 0e 0e  thing-new  dup 0< if  drop  else  smoke-trail  then ;
  on story-things-off ( -- )   \ (the story's kinds 9 and 10 all gone - their objects left where they are)
      #things 0 do  i thing-used? if  i it !  kind 9 = kind 10 = or if  -1 keri !  i thing-free  then  then  loop ;
  on thing-taken ( i -- )  dup thing-used? if  it !  gone  else  drop  then ;   \ (Hewie picked it up)
  on write-save ( -- )  write-things ;
  on creature-doing ( slot room mode up -- )  2drop drop  dup 7 10 within if  7 - cells homunculi +  sender swap !  else  drop  then ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      drop st-alert ! st-kind !  2drop 2drop drop  st-room !  0 st-told ! ;
end-behaviour

: things-spawn ( -- id )  keeping-things things-state s" things" spawn ;
