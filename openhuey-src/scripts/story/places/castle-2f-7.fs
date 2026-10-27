\ story/places/castle-2f-7.fs - the plate press room in Belli Castle's 2F ($23): the plate pressing
\ machine (the first time a scene - "It's not a computer" - and the press; a word plate made, its
\ scene and the file; again while fewer than ten are made, else none left; a plate given back);
\ once the golem is done, someone trying the door as she stands at the machine (a fright); brought
\ in by the forced way, Daniella's scene ("Where is the old man?" - her face); the birdcage in
\ turn, the ruin, the thing wandering, what Hewie looks up at. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-2f-7.fs (converted from the game's,
\ not loaded).
IN: story.places.castle-2f-7
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims fiona.sounds scenes ;

own-reset

\ ---- what is said here ----
0 constant old-man-msg     \ "Where is he?! Where is the old man? Where's he hiding? I won't let him touch he..."
6 constant computer-msg    \ "It's not a computer, that's for sure... It has alphabetic keys, though."
$C constant ruined-msg     \ "It is completely ruined. What could have happened here?"
$D constant birdcage-msg   \ "Is this a birdcage? It looks like the remaining husk of a bird in the cage."
$E constant animals-msg    \ "The terrible things that people do to animals these days. It's just so upsetting."
$F constant no-plates-msg  \ "There's a plate pressing machine here. There are no remaining plates currently so..."
$10 constant returned-msg  \ "Returned a plate to the plate pressing machine."
$11 constant press-msg     \ "It's a plate pressing machine."
$12 constant none-left-msg \ "There are no plates remaining. I'll have to get rid of some plate-keys before I c..."

\ ---- its own state (a visit) ----
own: press-remarked  own: cage-looked

\ ---- the room's things ----
0 constant wander-fx   6 constant cube-object   0 constant after-part   1 constant before-part
3 constant press-area   4 constant cage-area   5 constant ruin-area   $11 constant door-0
2 constant press-movie   0 constant press-cutscene   3 constant plate-movie   1 constant plate-cutscene
5 constant old-man-movie   4 constant old-man-cutscene   2 constant press-screen
1 constant cue-sound   $225 constant plate-file   $25 constant old-man-seen
: wander-about ( -- )  wander-fx -39e 6e 31e 3e 3e 0e 0e -60e wanderer-at ;
: tinted ( -- )  c2f-3-flag1 fact? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;

\ ---- as she comes in ----
: came-in ( exit -- )  drop  room-sounds  tinted  wander-about  cube-object object-hide ;

\ ---- the press: the plate made (its scene - a sound at its cue - the plate shown her, the file) ----
: plate-cue ( -- )  begin  cutscene-frame $69 < while  yield  repeat  cue-sound 35e 10e 25e room-sound ;
: plate-scene ( -- )
    plate-movie scenes:unseen-movie plate-cutscene load-scene  ['] plate-cue start drop
    panic-eased  if  -1 run-scene  then ;
: plate-shown ( -- )  word-plate-key item-named  word-plate-key show-item-tab  fanfare  obtained-2-msg say ;
: plate-done ( -- )   \ (back from the scene: by the machine, the plate, its file once the cue is past)
    fiona $7A 33 31.61e 18.93e place-at  fiona 0 idle  camera-restart  15 screen:from-black fade  wait-fade
    plate-shown  begin  ['] plate-cue running? while  yield  repeat  plate-file give-file ;
: presses ( -- )   \ (the press's screen; a plate made: its scene)
    press-screen open-subscreen  yield
    plate-made fact? if
        15 screen:to-black-2 fade  wait-fade  plate-scene  cube-object object-hide  plate-done
    then ;
: first-press ( -- )
    stalkers-stay state!  15 screen:to-black-2 fade  fiona wait-move
    press-movie scenes:unseen-movie press-cutscene load-scene  wait-fade
    panic-eased  if  computer-msg run-scene  then  world-held state!
    c2f-7-flag1 fact!  -1 press-remarked !
    press-screen open-subscreen  yield
    plate-made fact? if  plate-scene  world-held state!  cube-object object-hide  then
    fiona $7A 33 31.61e 18.93e place-at  fiona 0 idle  camera-restart  15 screen:from-black fade  wait-fade
    plate-made fact? if  plate-shown  begin  ['] plate-cue running? while  yield  repeat  plate-file give-file  then
    stalkers-stay state-off ;
: nobody-by-the-door? ( -- flag )   \ (nobody in exit 0's area; Debilitas loaded, not in the game)
    fiona 0 exit-area in-area?  hewie 0 exit-area in-area? or  if  false exit  then
    partner 0< 0=  partner body? 0= and  partner-kind dup debilitas-kind = over debilitas-6-kind = or swap debilitas-7-kind = or and ;
: someone-at-door ( -- )   \ (once the golem is done: the door tried from beyond - a fright)
    door-0 false open-mark  doors-redone  0 door-tried  5 frames
    seized-sound common-bank fiona sound-by  fiona uneasy-2-anim 5 start-blend  1 $FF 8 rumble-pad  16 frames
    c2f-7-flag2 fact!  90 threat  fiona wait-anim ;
: press-again ( -- )   \ (a plate made before: the screen; else - the press and its scene)
    press-remarked @ 0= if  press-msg say  -1 press-remarked !  then
    plate-made fact? if
        plate-made fact-off  press-screen open-subscreen  yield
        plate-made fact? if  plate-shown  then  plate-made fact!
    else  presses  then ;
: the-press ( -- )
    stalkers-stay state!  fiona wait-move
    golem-done fact?  c2f-7-flag2 fact? 0= and  nobody-by-the-door? and  0 exit-open? and if  someone-at-door
    else  plate-pressable? if  press-again
    else  press-remarked @ if  none-left-msg  else  -1 press-remarked !  no-plates-msg  then  say  then  then
    stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )   \ (a plate given back: her word)
    2drop  swap 0= 0= if  drop false exit  then
    5 = if  [: fiona hold  fiona wait-move  returned-msg say ;] start drop  true  else  false  then ;

\ ---- brought in by the forced way: Daniella's scene - her face as it turns to her ----
: her-face ( -- )   \ (from the scene's third shot)
    begin  cutscene-frame cutscene-shot-at 3 <> while  yield  repeat
    daniella-kind 0 s" O_DNL\DNL_201.TEX" face-on ;
: old-man-scene ( -- )
    old-man-movie scenes:unseen-movie old-man-cutscene load-scene  ['] her-face start drop
    panic-eased  if  old-man-msg run-scene  then  world-held state! ;
: daniella-comes ( -- )
    fiona hold  fiona $7A 33 31.61e 18.93e place-at  fiona false shown
    old-man-scene  fiona true shown  3 figure-out  4 figure-out  old-man-seen resident-flag-set  forced-exit take-exit ;

\ ---- what she may do or say here ----
: birdcage ( -- )
    fiona wait-move
    cage-looked @ if  animals-msg say exit  then
    fiona -36.2e 33.3e turn-to-xz  fiona look-at-anim start-anim  50 frames  birdcage-msg say  -1 cage-looked ! ;
: the-ruin ( -- )  fiona wait-move  ruined-msg say ;
: spots ( -- )
    fiona press-area 45 faces-area? if
        c2f-7-flag1 fact? if  ['] the-press unhurried offer  else  ['] first-press unhurried offer  then
    then
    fiona cage-area 45 faces-area? if  ['] birdcage unhurried offer  then
    fiona ruin-area 45 faces-area? if  ['] the-ruin unhurried offer  then ;

\ ---- the movies replayed (the extras): the press and the plate; Daniella's scene ----
: replay-press ( -- )
    fiona hold  room-sounds  begin  room-sounds-in? 0= while  yield  repeat
    cube-object object-hide  tinted  wander-about
    press-movie scenes:unseen-movie press-cutscene load-scene  panic-eased  if  computer-msg run-scene  then
    plate-scene  replay-over ;
: replay-old-man ( -- )
    fiona hold  wander-about  cube-object object-hide  fiona false shown  after-part show-part  before-part hide-part
    daniella-kind 3 figure-in  riccardo-kind 4 figure-in  yield
    old-man-scene  3 figure-out  4 figure-out  fiona true shown  replay-over ;

\ ---- every frame ----
create looks
    -35.84e 0e 32.47e  18e 16e 16e look,
    36.32e 0e 24.37e  15e 12e 12e look,
create cameras  1 , 0 , -1 ,   2 , 1 , -1 ,   \ (area, set, path)
: each-frame ( -- )
    events-held state? if  exit  then
    0 exit-out  cameras 2 cameras-at  looks 2 hewie-looks-at  spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act 8 = if  ['] replay-press  else  ['] replay-old-man  then  start drop exit
    then
    0 0 -1 way-in
    came-by @ forced-exit = if  after-part show-part  before-part hide-part  ['] daniella-comes start drop
    else  after-part hide-part  before-part show-part  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-7 place!
' items-used castle-2f-7 items-here
