\ stalker/moving.fs - how a stalker moves (src/game/pursuer.c): by his animations' root motion,
\ along his way a stride at a time (Npc_WalkPathStride: turned toward the point a stride on
\ and stepped onto it), turning on the spot when the way goes off to one side, his walk for his
\ mode, standing.
IN: stalker.moving
USING: engine game-state actors messages common facts flag-names paths vectors stalker-names stalker.state stalker.senses stalker.tables stalker.anims floor-flags ;

: model ( -- a )  my-model @ actor ;
: sound ( id bank -- )   \ Pursuer_Sound: at him (not while the world is held, nor silenced)
    world-held state-flag? hushed @ or if  2drop exit  then  {: id bank :}  my-at vec@  id bank  bank-sound-at ;
: yaw ( F: -- a )  self body-yaw ;
: yaw! ( F: a -- )  body-turn ;
0 constant own-move-mode   3 constant on-ladder-mode   \ (his move mode: his own; on a ladder - stalker/ladder.fs)

\ ---- his animations' sounds (Pursuer_AnimSounds): at its sound keys (event $1 the first, $10
\ the second) the sound his kit gives that motion for it (bank 7) - a blow's swish, a fall's
\ thud; a fall onto water - his triangle's water bits - a splash instead (bank 6) ----
$1 constant first-key   $10 constant second-key
$1D constant splash-sound    \ (bank 6)
$8000 constant soundless-anims   \ (animations with the top bit: none of their sounds)
: key-sound ( key -- n | -1 )   \ (0 the first, 1 the second)
    my-model @ anim @ motion-index                                ( key i )
    dup 0<  over anim-sounds @ < 0= or if  2drop -1 exit  then
    2* +  1+ cells anim-sounds + @ ;
: falling? ( -- flag )  anim @ dup leg-fall-anim = over fall-back-anim = or  swap fall-front-anim = or ;
: on-water? ( -- flag )
    self body-tri dup 0< if  drop false exit  then  nav-flags water-floor and water-floor = ;
: thud? ( n -- flag )  $11 - 2 u< ;   \ (his falls' sounds)
: key-sound-play ( n -- )
    dup 0< if  drop exit  then
    dup thud? falling? and on-water? and if  drop splash-sound 6 sound exit  then
    7 sound ;
: anim-sounds-play ( -- )   \ (in the room being played)
    anim @ soundless-anims and if  exit  then
    my-model @ 0 0 1 motion-events
    dup first-key and if  0 key-sound key-sound-play  then
    second-key and if  1 key-sound key-sound-play  then ;

\ ---- his animations (Pursuer_PlayAnim: blended as its table says, not restarted while it
\ plays) ----
: play-now ( anim -- )   \ (Motion_PlayTable)
    dup anim !  {: a :}  my-model @ a  my-model @ a motion-entry nip  motion-play ;
: play ( anim -- )  dup anim @ = if  drop exit  then  play-now ;
: riccardo? ( -- flag )  my-kind @ riccardo-kind = ;
: riccardo-like? ( -- flag )   \ (and his other class, the TintStalker)
    my-kind @ dup riccardo-kind =  over kind37-kind = or  swap tintstalker-kind = or ;
: near-seen-walk ( -- a )  sees-fiona @ d-fiona f@ 100e f< and d-fiona f@ f0> and if  walk-after-anim  else  walk-slow-anim  then ;
: still-anim ( -- a )   \ (vtable +0x320 Pursuer_WalkAnim: hurt, in a rage; Riccardo his own)
    cond @ hurt-cond = if  stand-hurt-anim  else  riccardo? if  stand-alt-anim  else  raging @ if  stand-rage-anim  else  stand-anim  then  then  then ;
\ his walks (vtable +0x324 / +0x328): the fast one - hurt, in a rage, else walking; the slow one
: walk-fast ( -- a )
    cond @ hurt-cond = if  walk-hurt-anim  else  raging @ riccardo? 0= and if  walk-rage-anim  else  walk-anim  then  then ;
: walk-slow ( -- a )   \ (Pursuer_SlowWalkAnim)
    my-mode @ held-off = base-chase @ 0= and riccardo-like? 0= and  cond @ hurt-cond = or if  walk-slow-hurt-anim exit  then
    my-kind @ lorenzo2-kind =  hard-mode? and if  walk-slow-rage-anim exit  then   \ (Lorenzo2_SlowWalkAnim: hard)
    riccardo? if  near-seen-walk exit  then   \ (Riccardo_SlowWalkAnim)
    raging @ if  walk-slow-rage-anim exit  then
    my-kind @ dup kind37-kind = swap tintstalker-kind = or if  near-seen-walk exit  then   \ (Kind37 / TintStalker_SlowWalkAnim)
    my-mode @ after-her <> if  walk-slow-anim  else  walk-after-anim  then ;
: turn-base ( -- a )  cond @ hurt-cond = if  turn-hurt-anims  else  turn-anims  then ;   \ (Pursuer_TurnOnSpot)
: ended? ( -- flag )  model act.mflags l@ $20 and 0<> ;   \ came to its end this frame
: settled? ( -- flag )  model act.fade sf@ 0e f<= ;       \ faded in

\ ---- his root motion this frame (Motion_RootMovement): turned and moved by it, along walls ----
: root@ ( F: -- turn x y z )  my-model @ root-delta ;
: root-move ( -- )  root@ {: F: turn x y z :}  yaw turn f+ angle-wrap yaw!  x z body-move-local ;
: stride ( F: -- d )  root@ {: F: turn x y z :}  z ;

\ ---- his footsteps (Pursuer_Footsteps; Lorenzo's only walking or turning): as a foot comes
\ down (his feet a frame back), the sound of the floor under it (bank 7, the feet by turns:
\ wood 2, stone 4, metal 6, grass 8, water $14 in bank 6), higher the faster he goes (0.6 .. 1.4
\ a frame), and a noise the others hear (louder striding out). Standing still he is on both
\ feet and his count starts over. In rooms 7 / $106 the water ripples ----
$1C constant step-noise   $1F constant stride-noise
create left-at /vec allot  create right-at /vec allot
: feet ( -- l r )   \ (Motion_FootPos, a frame back)
    my-model @ 1 -1e foot-at left-at vec!  1 and
    my-model @ 0 -1e foot-at right-at vec!  1 and ;
: still-feet ( -- l r )   \ standing: down - while the motion before fades out, as it had them
    settled? if  1 1 exit  then
    my-model @ -4 -1 -1e motion-track-by 0= if  fdrop fdrop fdrop 0 0 exit  then
    fdrop  f0> 1 and  f0> 1 and ;
: standing? ( -- flag )  anim @ dup 4 u<  swap stand-about-anims dup 3 + within or ;   \ (motion groups 0 / $1600)
: new-step ( l r -- step )   \ 1 the left foot, -1 the right, 0 none
    {: l r :}
    l 1 = foot-l-was @ 0= and if  1  else  r 1 = foot-r-was @ 0= and if  -1  else  0  then  then
    l foot-l-was !  r foot-r-was ! ;
: floor-sound ( flags -- id bank )   \ (the feet by turns)
    dup grass-floor and if  wood-floor and if  $14 6  else  8 7  then
    else  metal-floor and case  metal-floor of  6  endof  stone-floor of  4  endof  wood-floor of  2  endof  >r 0 r>  endcase  7  then
    swap step-foot @ 1 and +  swap  1 step-foot +! ;
: step-pace ( F: -- 0..1 )  stride 0.6e f- 0.8e f/  0e fmax 1e fmin ;
: step-floor ( v -- flags )  v-tri dup 0< if  drop self body-tri  then  nav-flags ;
\ on a ladder the rungs' sound (10) - but getting on or off at its foot with the foot over the
\ floor out from it, the floor's (off there: under the foot from the foot's spot); off at its
\ top the floor's from the top's spot. -1: the floor under the foot as anywhere
-2 constant rungs
: ladder-offset ( i -- ) ( F: -- ox oz )  2* floats ladder-offsets @ +  dup f@  float+ f@ ;   \ (vtable +0x9C)
create lf-at /vec allot
: from-end ( v side -- flags | -1 )   \ (Actor_TriFrom: walked straight to the foot from that end's spot)
    {: v side :}
    at-ladder @ side nav-link-tri  at-ladder @ side nav-link-spot lf-at vec!     ( tri )
    lf-at v 0 v-walk  dup 0< if  exit  then  nav-flags ;
: over-foot-floor? ( v -- flag )   \ (out from its foot by his offset 3: Actor_DoorFront)
    {: v :}
    at-ladder @ 0  3 ladder-offset nav-link-front  dup 0< if  drop fdrop fdrop fdrop false exit  then
    lf-at vec!  lf-at v 0 v-walk 0< 0= ;
: ladder-floor ( v -- flags | -1 | rungs )
    anim @ case
        on-top-anim of  drop settled? if  rungs  else  -1  then  endof
        rung-down-anim of  drop rungs  endof   rung-down-2-anim of  drop rungs  endof
        rung-up-anim of  drop rungs  endof   rung-up-2-anim of  drop rungs  endof
        off-foot-anim of  dup over-foot-floor? if  0 from-end  else  drop rungs  then  endof
        on-foot-anim of  over-foot-floor? if  -1  else  rungs  then  endof
        off-top-anim of  1 from-end  endof
        >r drop -1 r>
    endcase ;
: step-sound ( v -- id bank )
    my-move-mode @ on-ladder-mode = if  dup ladder-floor  else  -1  then
    dup rungs = if  2drop 10 7 exit  then
    dup 0< if  drop step-floor  else  nip  then  floor-sound ;
: lorenzo-steps? ( -- flag )   \ (Lorenzo's: walking or turning)
    anim @ $FF00 and dup walk-anim = swap turn-anims = or ;
: footsteps ( -- )
    self body-tri 0< if  exit  then
    my-kind @ lorenzo-kind = if  lorenzo-steps? 0= if  exit  then  then
    feet  standing? if  2drop still-feet  then  new-step
    ?dup 0= if  settled? standing? and if  0 step-foot !  then  exit  then
    1 = if  left-at  else  right-at  then
    flooded? if  1 over water-step  then
    step-sound
    world-held state-flag? hushed @ or if  2drop  else  0  step-pace 2e f* f>s $7F and  my-at vec@ sound-at  then
    step-pace 0.5e f< if  step-noise  else  stride-noise  then  room-id self body-tri -1 stalker-noise s" acoustics" actor-named send noise ;

\ ---- turning (Npc_TurnTowardPos: toward a point by at most `step`; how far is left) ----
: heading-to ( v -- ) ( F: -- a )  self swap body-heading-to ;
: turn-to ( v -- ) ( F: step -- left )  heading-to fswap body-turn-toward ;
\ the turn rate (vtable +0xA0; Debilitas 3 degrees a frame)
: turn-rate ( F: -- r )  turn-k f@ ;
\ Pursuer_TurnOnSpot via Npc_TurnWayTo: past 80 degrees off he turns on the spot - to the left,
\ to the right, +2 a half turn (past 160); $FF none
$FF constant no-turn
: way-past ( v -- way ) ( F: a b -- )   \ (Npc_TurnWayTo: past a off, b for a half turn)
    {: v F: a b :}  v heading-to yaw f- angle-wrap {: F: d :}
    d fabs a f<= if  no-turn exit  then
    d f0< if  0  else  1  then
    d fabs b f> if  2 or  then ;
: way-to ( v -- way )  80e deg>rad 160e deg>rad way-past ;

\ ---- along his way: planned to (tri, v) over his floor; how far is left ----
: plan ( tri v -- n )  {: tri v :}  my-path self body-tri my-at tri v self body-mask path-plan ;
: way-left ( F: -- d )  my-path path-left? if  my-path my-at path-rest  else  0e  then ;
\ Npc_WalkPathStride: the point a stride on; well off his heading (45 degrees, with 4 or more
\ left) he only turns, twice as fast; else turned to it and put there. True at the end
create sp-at /vec allot
: put-at ( v -- )   \ (off the floor: his triangle kept)
    {: v :}  self body-tri {: tri :}
    v vec@ body-place-at  self body-tri 0< if  room-id tri v vec@ body-place  then ;
: walk-stride ( -- end? )
    my-path path-left? 0= if  true exit  then
    stride fdup f0< if  fdrop false exit  then  0.01e fmax
    my-path my-at path-ahead  sp-at ahead vec-copy                  ( i )
    sp-at heading-to yaw f- angle-wrap fabs 45e deg>rad f< 0=
    way-left 4e f< 0= and if
        drop  sp-at turn-rate 2e f* turn-to fdrop  false exit
    then
    sp-at turn-rate turn-to fdrop
    sp-at put-at
    my-path path-i!
    my-path path-left? 0= ;

\ Npc_StepToward: turned toward a point (past 45 degrees only turning, twice as fast); a stride
\ at it - onto it when it is within the stride
: step-toward ( v -- )
    stride fdup f0< if  fdrop drop exit  then                        ( v ) ( F: s )
    dup heading-to yaw f- angle-wrap fabs 45e deg>rad f< 0= if
        fdrop  turn-rate 2e f* turn-to fdrop exit
    then
    dup turn-rate turn-to fdrop
    my-at over vec-dist-xz fover f<= if  fdrop vec@ body-place-at exit  then
    drop  0e fswap body-move-local ;
