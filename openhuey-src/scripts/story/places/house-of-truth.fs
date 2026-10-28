\ story/places/house-of-truth.fs - what the House of Truth's rooms share: the camera shaken as
\ Lorenzo's slam lands (in most of its rooms); the B1's voices - in a colour room long enough, three
\ in turn (shared $98); the colour rooms (B1-4 the white one, -5 .. -8): Lorenzo waiting once the
\ voices are over, out through either door into the dark (the room ahead decided by the colours
\ done), in again by the forced way, the inscription by the wall. The rooms' own cells follow
\ these: `own-reset  truth-owns owns !`.
IN: story.places.house-of-truth
USING: engine game-state actors messages common facts flag-names story-names room-names stalker-names
    floor-flags items.names story.kit stalker.anims fiona.anims fiona.sounds ;

own-reset

\ ---- its own state (a visit) ----
own: frames-here           \ frames in the room so far (the voices go by them)
own: through-reacted       \ out through a door already (once a visit)
own: all-four              \ the four colours done as she neared the way on: the music let go with her
owns @ constant truth-owns

\ ---- Lorenzo's slam (his other form, kind $A - in 1F-9 also kind $27): its impact this frame shakes the camera ----
$2301 constant slam-anim   $20 constant impact-key
: slam-shake-of ( kind -- )
    stalker-in?  partner-kind rot = and 0= if  exit  then
    stalker model-of dup 0< if  drop exit  then
    dup motion@ slam-anim <> if  drop exit  then
    0 0 1 motion-events impact-key and if  0.5e cam-shake!  then ;
: slam-shake ( -- )  lorenzo2-kind slam-shake-of ;

\ ---- the voices (the B1's colour rooms, shared $98): after 1800, 4500, 7200 frames in a room
\ (the room's sounds in), one each - a sound of bank $86 and its line (the rooms' 5, 6, 7); after
\ 7500 the last fact: Lorenzo waits in each colour room from then on ----
$86 constant voices-bank
: voice ( k -- )   \ (an activity)
    {: k :}  scene-locked state!  k voices-bank bank-sound  k 5 + say  scene-locked state-off ;
: voice-0 ( -- )  0 voice ;   : voice-1 ( -- )  1 voice ;   : voice-2 ( -- )  2 voice ;
: voice-on? ( -- flag )  ['] voice-0 running?  ['] voice-1 running? or  ['] voice-2 running? or ;
: voices ( -- )   \ (each frame)
    1 frames-here +!
    fading @  world-held state? or  voice-on? or if  exit  then
    fiona held? 0=  fiona-half-hidden state? or 0= if  exit  then
    shared-flag1 fact? 0= if
        frames-here @ 1800 >=  room-sounds-in? and if  shared-flag1 fact!  ['] voice-0 start drop  then  exit
    then
    shared-flag2 fact? 0= if  frames-here @ 4500 >= if  shared-flag2 fact!  ['] voice-1 start drop  then  exit  then
    shared-flag3 fact? 0= if  frames-here @ 7200 >= if  shared-flag3 fact!  ['] voice-2 start drop  then  exit  then
    shared-flag4 fact? 0= if  frames-here @ 7500 >= if  shared-flag4 fact!  then  then ;

\ ---- the colour rooms ----
$31 constant colour-track   0 constant colour-group   6 constant inscription-area
$129 constant arrival-tri   $EB constant his-arrival-tri
: colour-in ( fact tri -- )   \ (its colour done - fact 0: none; Lorenzo waiting at tri once the voices are over; no one else in)
    {: f tri :}  f if  f fact!  then
    shared-flag4 fact? if  this-room @ tri stalker-to  0 stalker-order  stalker 1 1 camera-set  then
    colour-track 1e background
    colour-group keeps-hewie-off keeps-stalkers-off or floor-on
    this-room @ stalker-slot 1 doors-into  this-room @ hewie-slot 1 doors-into ;
: comes-to ( -- )  fiona hold  fiona wait-move  15 screen:from-black fade  wait-fade ;   \ (an activity)
: arrived ( -- )   \ (by the forced way: at the room's spot, Hewie by her; the dark lifting)
    fiona arrival-tri -90 129e 0e place-at
    this-room @ 235 0 hewie-to  hewie his-arrival-tri -90 place-on-tri
    ['] comes-to start drop ;
: through ( -- )   \ (out through a door: into the dark, the room ahead by the forced way)
    fiona hold
    all-four @ if  no-track 1e background  keep-room-sounds state-off  then
    15 screen:to-black fade  fiona wait-move  wait-fade  world-held state!  forced-exit take-exit ;
: doors-through ( -- )   \ (into either door's area: out, once a visit)
    through-reacted @ if  exit  then
    fiona 0 in-area?  fiona 1 in-area? or 0= if  exit  then
    -1 through-reacted !  ['] through fiona start-holding drop ;
: loads-ahead ( area room -- )  swap fiona swap entered? if  preload  else  drop  then ;   \ (into the area: that room loaded ahead)
: colours-done? ( -- flag )  htb1-5-flag1 fact?  htb1-6-flag1 fact? and  htb1-7-flag1 fact? and  htb1-8-flag1 fact? and ;
: not-yet ( area fact room -- )   \ (that colour's room ahead while it isn't done; else back to the white)
    rot fiona swap entered? 0= if  2drop exit  then
    swap fact? if  drop house-of-truth-b1-4  then  preload ;
: way-on ( area -- )   \ (all four done: on to the 1F's room; else back to the white)
    fiona swap entered? 0= if  exit  then
    colours-done? dup all-four !  if  house-of-truth-1f-2  else  house-of-truth-b1-4  then  preload ;
variable said-msg
: inscription ( -- )   \ (an activity: turned to the wall, the screen dimmed, read)
    stalkers-stay state!  fiona hold  fiona wait-move  fiona -90 turn
    15 screen:to-dim fade wait-fade  said-msg @ say  15 screen:from-dim fade wait-fade  stalkers-stay state-off ;
: reads ( msg -- )   \ (by the wall, facing it: offered)
    fiona inscription-area in-area?  fiona -90 50 heading? and if  said-msg !  ['] inscription unhurried offer  else  drop  then ;
create colour-cameras  4 , 0 , 0 ,  5 , 1 , 1 ,
: colour-frame ( msg -- )  colour-cameras 2 cameras-at  doors-through  voices  slam-shake  reads ;
: colour-went ( -- )  scene-locked state-off ;

\ ---- Lorenzo coming in ahead of her (the corridors): there at once, looking about ----
: appears ( tri deg -- )   \ (an activity: out of sight till he stands there, then shown, looking about)
    {: tri deg :}  stalker-present  stalker hold  stalker false shown  stalker wait-move  stalker true shown
    stalker tri deg place-on-tri  stalker 1 1 camera-set  stalker look-about-3-anim 0 play-blend  0 stalker-order ;
: sent-in ( room tri -- )  stalker-to  0 stalker-order ;   \ (into that room on the triangle, about his own way)

\ ---- the tremors (shared $95, once the house has begun to come down - ht1f-9-flag2): with the
\ stalker still about it (her track $F, file 257 not yet found) the violent ones - now and then a
\ quake that may throw her down; else slight ones now and then ----
$31 constant quake-sound   $87 constant quake-bank
: shaken ( frames F: v -- )  {: n F: v :}  n 0 ?do  v cam-shake!  yield  loop ;
: slight-tremors ( -- )   \ (an activity: for good)
    begin  60 frames  25 chance? if  30 frames  then  25 chance? if  30 frames  then  30 0.1e shaken  again ;
: standing? ( -- )  fiona-doing-now @ dup 0= swap 5 = or  fiona held? 0= and ;   \ (on her feet, her own)
: thrown-down ( -- )  -1 true fiona send throw-down ;
: quake ( -- )   \ (one: the rumble growing, the thump - she is thrown down if standing - dying away)
    1 $80 30 rumble-pad  30 0.5e shaken  0 0 30 rumble-pad  1 $FF 30 rumble-pad
    quake-sound quake-bank bank-sound
    false 30 0 do  dup 0=  standing? and if  drop true  thrown-down  then  1e cam-shake!  yield  loop  drop
    1 $80 30 rumble-pad  30 0.5e shaken ;
: violent-tremors ( -- )  begin  90 0.1e shaken  quake  again ;
: tremors ( -- )   \ (as she comes in)
    ht1f-9-flag2 fact? 0= if  exit  then
    over-music-f fact?  found-file-257 fact? 0= and if  ['] violent-tremors  else  ['] slight-tremors  then  start drop ;

\ ---- the holes in the walls (1F-2, 1F-10): in at her yes, by the forced way ($FE: off-map-27's
\ crawl); out of one into the room (her slide, the dark lifting) ----
$FE constant hole-way
fvariable hole-x   fvariable hole-z   variable hole-msg'
: into-hole ( -- )   \ (an activity: turned to it, a close look, asked)
    stalkers-stay state!  fiona wait-move  fiona hole-x f@ hole-z f@ turn-to-xz  fiona look-close-anim 10 play-blend
    hole-msg' @ yes? if
        15 screen:to-black fade  wait-fade  slide-sound common-bank fiona sound-by
        world-held state!  hole-way take-exit
    else  stalkers-stay state-off  then ;
: hole-offer ( msg F: x z -- )  hole-z f!  hole-x f!  hole-msg' !  ['] into-hole unhurried offer ;
: out-of-hole ( -- )   \ (an activity)
    fiona hold  fiona wait-move  camera-restart  slide-sound common-bank fiona sound-by  1 map-page
    15 screen:from-black fade  wait-fade  stalkers-stay state-off ;

\ ---- the Sol Key turned in a door (exit 0's: 1F-4's, 2F-1's): spent but while the other door is
\ locked - its turning animation, the room's sound 3 ----
3 constant key-sound
: sol-turned ( anim other door -- )   \ (an activity)
    {: anim other door :}  fiona hold  fiona 0 to-door  fiona anim start-anim  20 frames  key-sound room-bank fiona sound-by
    sol-key item-named  other doors:door-locked? 0= if  sol-key spend  then  door unlock
    fiona 20 idle  20 frames  used-msg say  stalkers-stay state-off ;
