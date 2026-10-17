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
USING: engine game-state actors messages common vectors facts flag-names items items.names effects fiona.slam stalker-names ;

\ the kinds (her items' throwables, in their order; then the story's own two)
enum: thing-kind  ball-thing antimony-thing magnesia-thing doll-thing refined-antimony-thing
    refined-magnesia-thing tube-thing prima-thing stone-thing keri-thing trap-thing ;
128 constant #things
\ what a thing did in its motion this tick (thing-events)
1 constant burst-event   2 constant bounced-event   4 constant fell-event   8 constant glanced-event

state: things-state
  cell field it                \ the one being looked at
  #things cells field kicked   \ each one kicked this touch (her blow's point near it)
  #things cells field met-in   \ (the doll) the room the stalker last came upon it in (-1 none)
  3 cells field homunculi      \ the homunculi's actors (slots 7..9, as they tell: creature-doing)
  cell field st-kind  cell field st-mode    \ (the stalker as he tells: his kind, his mode)
  cell field st-room  cell field st-told     \ (his room; frames since he told: 0 this one)
  #things cells field keris   \ (kind 9) the room object it carries: "keri0".."keri5" (-1 none)
  cell field blow-n           \ (its blows to the homunculi: their own numbers, down from -1000)
end-state

\ ---- the one looked at ----
: kind ( -- n )  it @ thing-kind ;
: its-room ( -- room )  it @ thing-room ;
: its-tri ( -- tri )  it @ thing-tri ;
: age ( -- n )  it @ thing-age ;
: event? ( bit -- flag )  it @ thing-events and 0<> ;
: at ( -- v )  it @ thing-at ;
: here? ( -- flag )  its-room room-id = ;
\ the ball gone (its time, a hole): hers again, if she has none and no other lies about (Ball_dtor)
: balls ( -- n )  0  #things 0 do  i thing-used? if  i thing-kind ball-thing = if  1+  then  then  loop ;
: ball-back ( -- )
    balls 0=  ball items-of 0= and if  ball 1 s" items" actor-named send give-item  then ;
: gone ( -- )  kind  it @ thing-free  ball-thing = if  ball-back  then ;
: thing-sound ( id -- )  5 0 0 at vec@ sound-at ;
: running? ( -- flag )   \ (Thing_Frame: the game going - no scene has the camera, the world not held)
    cam-scene? 0=  world-held state-flag? 0= and ;

\ ---- by kind ----
: kept? ( k -- flag )   \ (kept in the save)
    case  ball-thing of true endof  magnesia-thing of true endof  doll-thing of true endof
        refined-magnesia-thing of true endof  prima-thing of true endof  stone-thing of true endof
        >r false r>  endcase ;
: powder? ( k -- flag )  dup antimony-thing = over refined-antimony-thing = or swap tube-thing = or ;
: magnesia? ( k -- flag )  dup magnesia-thing = over refined-magnesia-thing = or swap prima-thing = or ;
: lasts ( k -- frames )   \ (0: no end)
    case  ball-thing of 5400 endof  doll-thing of 9000 endof  trap-thing of 450 endof  >r 0 r>  endcase ;
\ the powders: the stalker's damage, how long he's held (the stun's loops - thing_stomped's b), the colour
: powder-damage ( k -- n )  case  antimony-thing of 5 endof  refined-antimony-thing of 10 endof  >r 20 r>  endcase ;
: powder-stun ( k -- n )  case  antimony-thing of 4 endof  refined-antimony-thing of 6 endof  >r 8 r>  endcase ;
: powder-colour ( k -- rgba )  case  antimony-thing of $402080FF endof  refined-antimony-thing of $806400FF endof  >r $808080FF r>  endcase ;
\ the magnesias' settings (thing_setup): startle %, damage, armed at, reach across (the tube's and
\ prima's last)
: startle ( k -- % )  case  magnesia-thing of 20 endof  refined-magnesia-thing of 50 endof  >r 100 r>  endcase ;
: damage ( k -- n )  case  magnesia-thing of 15 endof  refined-magnesia-thing of 30 endof  >r 45 r>  endcase ;
: armed-at ( k -- frames )
    case  magnesia-thing of 900 endof  refined-magnesia-thing of 1800 endof  prima-thing of 3600 endof  >r 450 r>  endcase ;
: reach ( k -- n )  case  magnesia-thing of 10 endof  refined-magnesia-thing of 15 endof  >r 20 r>  endcase ;

\ ---- whom it reaches ----
: stalker ( -- id | -1 )  stalker-in-room ;   \ (in the room being played)
: treads? ( body margin -- flag )   \ standing on it: within its radius (+ margin), 1 high
    {: body margin :}  body 0<  body body? 0= or if  false exit  then
    body  body body-dims fdrop margin s>f f+  it @  1e thing-reaches? ;
: in-reach? ( body -- flag )   \ (a magnesia's: across, 10 high)
    dup 0< if  drop false exit  then  it @  kind reach s>f  10e thing-reaches? ;
7 constant first-homunculus   \ (the creatures' slots 7..9)
: homunculus ( slot -- id )  first-homunculus - cells homunculi + @ ;
: homunculi-tread? ( -- flag )
    false  first-homunculus 3 + first-homunculus do  i homunculus 0 treads? or  loop ;

\ ---- their effects ----
: go-burst ( -- )   \ (a magnesia going off: the plain one the shove's puffs, the refined and prima their own bursts)
    kind magnesia-thing = if  at vec@ puffs exit  then
    kind prima-thing =  at vec@  its-tri tri-normal  effects:mag-burst ;
$8D constant splash-sound   $8E constant go-off-sound   $7C constant bounce-sound
: powder-splash ( -- )   \ (DropSplash: the powders' colour, and its sound)
    8 2 kind powder-colour  at vec@  at vec@  splash  splash-sound thing-sound ;
: dust ( -- )  2 $50 $50 $50 16  at vec@  burst ;   \ (the doll glancing off a wall: a grey puff)
: heard ( loud -- )   \ (a noise at it the stalker hears: Noise_Make)
    its-room its-tri -1 world-noise  s" acoustics" actor-named send noise ;
$8000 constant startled   \ (a blow's flag: he flinches)
: blow-him ( id kind damage startle% -- )   \ (Progress_Noise to him: his request 4 from where it lies - in whatever room)
    {: id k d pct :}  id 0< if  exit  then
    k d  rnd 100e f* f>s pct < if  startled  else  0  then  at vec@ xyz>cells  id send blow-from ;
: blow-stalker ( kind damage startle% -- )  {: k d pct :}  stalker k d pct blow-him ;   \ (the one in her room)
: blow-homunculi ( damage reach -- )   \ (ThingShared_ReachStalkers: as one of her blows there)
    {: d r :}  blow-n @  d  at v.x sf@ f>cell  at v.y sf@ f>cell  at v.z sf@ f>cell  r s>f f>cell
    -1 blow-n +!  broadcast fiona-blow ;

\ ---- her kick (thing_kick_up: her blow's point within 5 kicks it her way - 2 ahead, up - once
\ a touch) ----
: up ( -- ) ( F: -- up )  kind case  keri-thing of 0.6e endof  trap-thing of 0.4e endof  >r 2e r>  endcase ;
: kicked? ( -- addr )  it @ cells kicked + ;
: kick ( -- )
    s" fiona" actor-named {: her :}  her 0< if  exit  then
    her body-point 0= if  0 kicked? ! exit  then          ( v )
    kicked? @ if  drop exit  then
    at vec-dist 5e f> if  exit  then
    -1 kicked? !
    0e 2e her body-yaw rotate-by  up fswap  it @  thing-kick ;

\ ---- the story's own (house-of-truth-1f-9): kind 9 carries a room object "keri<k>" with it
\ (Thing09_Draw) - free again (hidden) once it has fallen through, and the story told; kind 10
\ is unseen, a trap: the stalker treading on it is knocked down (Thing10_Frame: a blow of kind
\ 2, $28; its sound 4) and it is gone ----
: keri$ ( k -- addr len )
    case  0 of  s" keri0"  endof  1 of  s" keri1"  endof  2 of  s" keri2"  endof
          3 of  s" keri3"  endof  4 of  s" keri4"  endof  >r s" keri5" r>  endcase ;
: keri ( -- addr )  it @ cells keris + ;
$20 constant object-x   $24 constant object-y   $28 constant object-z   \ (a room object's place: placed-f!)
: carry-keri ( -- )
    keri @ dup 0< if  drop exit  then  keri$ {: a n :}
    at v.x sf@ a n object-x placed-f!  at v.y sf@ a n object-y placed-f!  at v.z sf@ a n object-z placed-f! ;
: keri-gone ( -- )
    keri @ dup 0< if  drop exit  then  dup keri$ 0 1 placed-op  -1 keri !  broadcast keri-fell ;
: trap-frame ( -- )
    kick
    stalker 0 treads? if  4 6 0 0 at vec@ sound-at  2 $28 0 blow-stalker  gone  then ;

\ ---- each kind's frame ----
: quiet? ( -- flag )  $20 state-flag? ;   \ (progress flag $20: nothing set off)
7 constant tread-blow   \ (a blow's kind: trod on a powder)
: powder-frame ( -- )   \ (thing_stomped: the stalker treading on it - its damage and his stun's loops; under flag $20 nothing, no kick)
    quiet? if  exit  then
    stalker 1 treads? if
        tread-blow kind powder-damage kind powder-stun at vec@ xyz>cells  stalker send blow-from
        powder-splash  gone exit
    then  kick ;
: go-off ( -- )  go-off-sound thing-sound  go-burst  gone ;
\ armed, it goes off wherever it lies (ThingShared_Armed): the stalker in its room - in her room
\ within its reach, elsewhere always (+0x122: 100) - has its blow (not under flag $20)
: him-in-its-room ( -- id | -1 )  stalker-in-game dup 0< if  exit  then  dup body-room its-room <> if  drop -1  then ;
: sets-him-off? ( -- flag )
    him-in-its-room dup 0< if  drop false exit  then
    here? if  in-reach?  else  drop true  then ;
2 constant hard-blow   \ (a blow's kind)
: magnesia-frame ( -- )   \ (thing_watch)
    age kind armed-at >= if
        here? if  kind damage kind reach blow-homunculi  then   \ (ThingShared_ReachStalkers)
        sets-him-off? quiet? 0= and if  him-in-its-room hard-blow kind damage kind startle blow-him  then
        here? if  go-off  else  gone  then  exit
    then
    here? 0= quiet? or if  exit  then
    homunculi-tread?  stalker 0 treads?  {: homunculi? him? :}
    homunculi? him? or 0= if  kick exit  then
    him? if  hard-blow kind damage kind startle blow-stalker  then
    homunculi? if  kind damage 0 blow-homunculi  then   \ (those standing on it struck: ThingShared_StompStalkers)
    go-off ;
: stone-frame ( -- )   \ (Thing08_Frame: armed, a noise the stalker hears - $4F at it - and gone)
    age 450 < if  kick exit  then
    here? if  0 thing-sound  3 $10 $10 $10 $80  at vec@  burst  then
    $4F heard  gone ;
\ the doll and Debilitas (Thing03_StalkerCheck): only him - kinds 2 6 7 $1B (his cast ids) - and in its room
$1F constant doll-noise
: debilitas-kind? ( -- flag )  st-kind @ dup 2 = over 6 = or over 7 = or swap $1B = or ;
: he-is-with-it? ( -- flag )  st-told @ 2 <  debilitas-kind? and  st-room @ its-room = and ;
: met ( -- addr )  it @ cells met-in + ;
: after-her? ( -- flag )  st-mode @ after-her = ;
: searching? ( -- flag )  st-mode @ heading-for-her held-off within ;   \ (heading for her room, searching it, waiting about)
\ away from Fiona: he comes upon it once a visit - hears it, and takes it unless he is after her
: doll-away ( -- )
    met @ st-room @ = if  exit  then  st-room @ met !
    doll-noise heard  after-her? 0= if  gone  then ;
\ in her room, while he searches: it draws him (its noise, once; he goes for it if he sees it and
\ can get there); within 10 of him it is taken
: doll-near ( -- )
    searching? 0= if  -1 met !  exit  then
    stalker {: him :}  him 0< if  exit  then
    at him body-at vec-dist 10e f< if  gone exit  then
    met @ its-room <> if  its-room met !  doll-noise heard  then
    its-tri him send drawn-to ;
: doll-frame ( -- )
    he-is-with-it? if
        here? if  doll-near  else  doll-away exit  then
    then
    it @ thing-used? here? and if  kick  then ;
: one-frame ( -- )
    burst-event event? if  powder-splash exit  then   \ (freed next step)
    fell-event event? if   \ (through a hole: freed next step)
        kind ball-thing = if  ball-back  then  kind keri-thing = if  keri-gone  then  exit
    then
    bounced-event event? if  bounce-sound thing-sound  then
    glanced-event event? if  dust  then
    kind lasts ?dup if  age <= if  gone exit  then  then
    kind doll-thing = if  doll-frame exit  then        \ (in any room: he may come upon it away from her)
    kind magnesia? if  magnesia-frame exit  then       \ (in any room: armed, it goes off there)
    here? 0= if  exit  then
    kind powder? if  powder-frame exit  then
    kind case
        stone-thing of  stone-frame  endof
        keri-thing of  carry-keri kick  endof
        trap-thing of  trap-frame  endof
        >r kick r>
    endcase ;

\ ---- each frame: their motion (in the room being played), then what each does ----
: frame ( -- )
    room-id running? things-step
    running? 0= if  exit  then
    #things 0 do  i thing-used? if  i it !  one-frame  then  loop ;

\ ---- a new one (her let go): ten of a kind at most (the trap: five) - the oldest goes
\ (PlacedThings_AnotherCame) ----
: of-kind? ( k i -- flag )  dup thing-used? if  thing-kind =  else  2drop false  then ;
: make-room ( k -- )
    {: k | n oldest :}  -1 to oldest
    k kept? k trap-thing = or 0= if  exit  then
    #things 0 do
        k i of-kind? if
            1 n + to n
            oldest 0<  i thing-age oldest thing-age >  or if  i to oldest  then
        then
    loop
    k trap-thing = if  5  else  10  then  n <= if  oldest thing-free  then ;

\ ---- the save: those kept, a line each (`lying`) ----
: .f ( F: r -- )  6 f>s$ type space ;
: write-thing ( i -- )
    {: i :}  i thing-kind kept? 0= if  exit  then
    i thing-kind .  i thing-room .  i thing-tri .  i thing-age .
    i thing-at {: v :}  v v.x sf@ .f  v v.y sf@ .f  v v.z sf@ .f  ." things:lying" cr ;
: write-things ( -- )
    ." things:clear-all" cr
    #things 0 do  i thing-used? if  i write-thing  then  loop ;
: clear-all ( -- )  things-clear ;
: lying ( kind room tri age -- ) ( F: x y z -- )   \ (a save read back: lying there)
    {: k r t a :}  0e 0e 0e  k r t thing-new  dup 0< if  drop exit  then  a swap thing-age! ;

behaviour keeping-things
  on spawned ( -- )  kicked #things cells 0 fill  homunculi 3 cells -1 fill  keris #things cells -1 fill
      met-in #things cells -1 fill  100 st-told !  -1000 blow-n !
      self subscribe tick  self subscribe write-save  self subscribe creature-doing  self subscribe stalker-doing ;
  on tick ( -- )  frame  st-told @ 100 < if  1 st-told +!  then ;
  on thing-made ( kind tri x y z vx vy vz -- )   \ (her let go: at her hand, its way)
      {: k tri x y z vx vy vz :}
      k make-room
      k room-id tri  x cell>f y cell>f z cell>f  vx cell>f vy cell>f vz cell>f  thing-new
      dup 0< 0= k ball-thing = and  vx vz or 0<> and if  s" hewie" actor-named send fetch-this  else  drop  then ;   \ (thrown: his to fetch)
  on keri-drop ( k tri x y z -- )   \ (the story's: a kind-9 thing carrying "keri<k>", falling from there)
      {: k tri x y z :}
      keri-thing room-id tri  x cell>f y cell>f z cell>f  0e 0e 0e  thing-new  dup 0< if  drop exit  then
      cells keris + k swap !  k keri$ 0 0 placed-op ;
  on trap-place ( tri x y z -- )   \ (the story's: a trap set down there - five at most - smoke rising from it)
      {: tri x y z :}  trap-thing make-room
      trap-thing room-id tri  x cell>f y cell>f z cell>f  0e 0e 0e  thing-new  dup 0< if  drop  else  smoke-trail  then ;
  on story-things-off ( -- )   \ (the story's kinds 9 and 10 all gone - their objects left where they are)
      #things 0 do  i thing-used? if  i it !  kind keri-thing = kind trap-thing = or if  -1 keri !  i thing-free  then  then  loop ;
  on thing-taken ( i -- )  dup thing-used? if  it !  gone  else  drop  then ;   \ (Hewie picked it up)
  on write-save ( -- )  write-things ;
  on creature-doing ( slot room mode up -- )
      {: slot room mode up :}  slot first-homunculus dup 3 + within if  sender slot first-homunculus - cells homunculi + !  then ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      {: room mode sub cond hp hp-max k alert came :}  alert st-mode !  k st-kind !  room st-room !  0 st-told ! ;
end-behaviour

: things-spawn ( -- id )  keeping-things things-state s" things" spawn ;
