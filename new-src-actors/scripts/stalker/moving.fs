\ stalker/moving.fs - how a stalker moves (src/game/pursuer.c): by his animations' root motion,
\ along his way a stride at a time (Npc_WalkPathStride: turned toward the point a stride on
\ and stepped onto it), turning on the spot ($400..$403) when the way goes off to one side,
\ his walk for his mode, standing.
IN: stalker.moving
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.tables ;

: model ( -- a )  my-model @ actor ;
: sound ( id bank -- )   \ Pursuer_Sound: at him (not while the world is held, nor silenced)
    world-held state-flag? hushed @ or if  2drop exit  then  >r >r  my-at vec@  r> r>  bank-sound-at ;
: yaw ( F: -- a )  self body-yaw ;
: yaw! ( F: a -- )  body-turn ;

\ ---- his animations' sounds (Pursuer_AnimSounds): at its sound keys (event $1 the first, $10
\ the second) the sound his kit gives that motion for it (bank 7) - a blow's swish, a fall's
\ thud; a fall ($1709 / $1800 / $1804) onto water - his triangle's water bits - a splash instead
\ ($1D, bank 6) ----
$1 constant first-key   $10 constant second-key
$2008000 constant water-floor
: key-sound ( key -- n | -1 )   \ (0 the first, 1 the second)
    my-model @ anim @ motion-index                                ( key i )
    dup 0<  over anim-sounds @ < 0= or if  2drop -1 exit  then
    2* +  1+ cells anim-sounds + @ ;
: falling? ( -- flag )  anim @ dup $1709 = over $1800 = or  swap $1804 = or ;
: on-water? ( -- flag )
    self body-tri dup 0< if  drop false exit  then  nav-flags water-floor and water-floor = ;
: thud? ( n -- flag )  $11 - 2 u< ;   \ (his falls' sounds)
: key-sound-play ( n -- )
    dup 0< if  drop exit  then
    dup thud? falling? and on-water? and if  drop $1D 6 sound exit  then
    7 sound ;
: anim-sounds-play ( -- )   \ (in the room being played)
    anim @ $8000 and if  exit  then
    my-model @ 0 0 1 motion-events
    dup first-key and if  0 key-sound key-sound-play  then
    second-key and if  1 key-sound key-sound-play  then ;

\ ---- his animations (Pursuer_PlayAnim: blended as its table says, not restarted while it
\ plays) ----
: play-now ( anim -- )   \ (Motion_PlayTable)
    dup anim !  my-model @ swap  2dup motion-entry nip  motion-play ;
: play ( anim -- )  dup anim @ = if  drop exit  then  play-now ;
: riccardo? ( -- flag )  my-kind @ 4 = ;
: riccardo-like? ( -- flag )  my-kind @ dup 4 =  over 37 = or  swap 23 = or ;   \ (and his other class, the TintStalker)
: near-seen-walk ( -- a )  sees-fiona @ d-fiona f@ 100e f< and d-fiona f@ f0> and if  $206  else  $201  then ;
: stand-anim ( -- a )   \ (vtable +0x320 Pursuer_WalkAnim: hurt 2, in a rage 3; Riccardo 1 for 0 / 3)
    cond @ 1 = if  2  else  riccardo? if  1  else  raging @ if  3  else  0  then  then  then ;
\ his walks (vtable +0x324 / +0x328): hurt $202 / $203; else $200, and the slow one - held off
\ $203, searching or heading $201, after her $206
: walk-fast ( -- a )  cond @ 1 = if  $202  else  raging @ riccardo? 0= and if  $204  else  $200  then  then ;   \ (+0x324: in a rage $204)
: walk-slow ( -- a )   \ (Pursuer_SlowWalkAnim: in a rage $205; Debilitas's held off $203)
    my-mode @ 4 = base-chase @ 0= and riccardo-like? 0= and  cond @ 1 = or if  $203 exit  then
    my-kind @ 10 =  hard-mode? and if  $205 exit  then   \ (Lorenzo2_SlowWalkAnim: hard, $205)
    riccardo? if  near-seen-walk exit  then   \ (Riccardo_SlowWalkAnim)
    raging @ if  $205 exit  then
    my-kind @ dup 37 = swap 23 = or if  near-seen-walk exit  then   \ (Kind37 / TintStalker_SlowWalkAnim)
    my-mode @ if  $201  else  $206  then ;
: turn-base ( -- a )  cond @ 1 = if  $405  else  $400  then ;   \ (Pursuer_TurnOnSpot: hurt, $405..)
: ended? ( -- flag )  model act.mflags l@ $20 and 0<> ;   \ came to its end this frame
: settled? ( -- flag )  model act.fade sf@ 0e f<= ;       \ faded in

\ ---- his root motion this frame (Motion_RootMovement): turned and moved by it, along walls ----
fvariable rm-turn  fvariable rm-x  fvariable rm-y  fvariable rm-z
: root@ ( -- )  my-model @ root-delta  rm-z f!  rm-y f!  rm-x f!  rm-turn f! ;
: root-move ( -- )  root@  yaw rm-turn f@ f+ angle-wrap yaw!  rm-x f@ rm-z f@ body-move-local ;
: stride ( F: -- d )  root@ rm-z f@ ;

\ ---- his footsteps (Pursuer_Footsteps; Lorenzo's - kind 11 - only walking or turning): as a
\ foot comes down (his feet a frame back), the sound of the floor under it (bank 7, the feet by
\ turns: wood 2, stone 4, metal 6, grass 8, wet $14 in bank 6), higher the faster he goes
\ (0.6 .. 1.4 a frame), and a noise the others hear ($1C, $1F striding out). Standing still he
\ is on both feet and his count starts over. In rooms 7 / $106 the water ripples ----
create left-at 12 allot  create right-at 12 allot
: feet ( -- l r )   \ (Motion_FootPos, a frame back)
    my-model @ 1 -1e foot-at left-at vec!  1 and
    my-model @ 0 -1e foot-at right-at vec!  1 and ;
: still-feet ( -- l r )   \ standing: down - while the motion before fades out, as it had them
    settled? if  1 1 exit  then
    my-model @ -4 -1 -1e motion-track-by 0= if  fdrop fdrop fdrop 0 0 exit  then
    fdrop  f0> 1 and  f0> 1 and ;
: standing? ( -- flag )  anim @ dup 4 u<  swap $1600 $1603 within or ;   \ (motion groups 0 / $1600)
: new-step ( l r -- step )   \ 1 the left foot, -1 the right, 0 none
    over 1 = foot-l-was @ 0= and if  1  else  dup 1 = foot-r-was @ 0= and if  -1  else  0  then  then
    >r  foot-r-was !  foot-l-was !  r> ;
: floor-sound ( flags -- id bank )   \ (the feet by turns)
    dup $2000000 and if  $8000 and if  $14 6  else  8 7  then
    else  $18000 and case  $18000 of  6  endof  $10000 of  4  endof  $8000 of  2  endof  >r 0 r>  endcase  7  then
    swap step-foot @ 1 and +  swap  1 step-foot +! ;
: step-pace ( F: -- 0..1 )  stride 0.6e f- 0.8e f/  0e fmax 1e fmin ;
: step-floor ( v -- flags )  v-tri dup 0< if  drop self body-tri  then  nav-flags ;
: footsteps ( -- )
    self body-tri 0< if  exit  then
    my-kind @ 11 = if  anim @ $FF00 and dup $200 = swap $400 = or 0= if  exit  then  then
    feet  standing? if  2drop still-feet  then  new-step
    ?dup 0= if  settled? standing? and if  0 step-foot !  then  exit  then
    1 = if  left-at  else  right-at  then
    flooded? if  1 over water-step  then
    step-floor floor-sound
    world-held state-flag? hushed @ or if  2drop  else  0  step-pace 2e f* f>s $7F and  my-at vec@ sound-at  then
    step-pace 0.5e f< if  $1C  else  $1F  then  room-id self body-tri -1 stalker-noise s" acoustics" actor-named send noise ;

\ ---- turning (Npc_TurnTowardPos: toward a point by at most `step`; how far is left) ----
: heading-to ( v -- ) ( F: -- a )  self swap body-heading-to ;
: turn-to ( v -- ) ( F: step -- left )  heading-to fswap body-turn-toward ;
\ the turn rate (vtable +0xA0; Debilitas 3 degrees a frame)
: turn-rate ( F: -- r )  turn-k f@ ;
\ Pursuer_TurnOnSpot via Npc_TurnWayTo: past 80 degrees off he turns on the spot - $400 left,
\ $401 right, +2 a half turn (past 160); $FF none
fvariable wp-a  fvariable wp-b
: way-past ( v -- way ) ( F: a b -- )   \ (Npc_TurnWayTo: past a off, b for a half turn)
    wp-b f!  wp-a f!  heading-to yaw f- angle-wrap                   ( F: d )
    fdup fabs wp-a f@ f<= if  fdrop $FF exit  then
    fdup f0< if  0  else  1  then
    fabs wp-b f@ f> if  2 or  then ;
: way-to ( v -- way )  1.3962634e 2.7925268e way-past ;

\ ---- along his way: planned to (tri, v) over his floor; how far is left ----
: plan ( tri v -- n )  >r >r  my-path self body-tri my-at r> r> self body-mask path-plan ;
: way-left ( F: -- d )  my-path path-left? if  my-path my-at path-rest  else  0e  then ;
\ Npc_WalkPathStride: the point a stride on; well off his heading (45 degrees, with 4 or more
\ left) he only turns, twice as fast; else turned to it and put there. True at the end
create sp-at 12 allot
: walk-stride ( -- end? )
    my-path path-left? 0= if  true exit  then
    stride fdup f0< if  fdrop false exit  then  0.01e fmax
    my-path my-at path-ahead  sp-at ahead vec-copy                  ( i )
    sp-at heading-to yaw f- angle-wrap fabs 0.7853982e f< 0=
    way-left 4e f< 0= and if
        drop  sp-at turn-rate 2e f* turn-to fdrop  false exit
    then
    sp-at turn-rate turn-to fdrop
    self body-tri >r  sp-at vec@ body-place-at                       \ (just off the floor: his
    self body-tri 0< if  room-id r@ sp-at vec@ body-place  then  r> drop   \ triangle kept)
    my-path path-i!
    my-path path-left? 0= ;

\ Npc_StepToward: turned toward a point (past 45 degrees only turning, twice as fast); a stride
\ at it - onto it when it is within the stride
: step-toward ( v -- )
    stride fdup f0< if  fdrop drop exit  then                        ( v ) ( F: s )
    dup heading-to yaw f- angle-wrap fabs 0.7853982e f< 0= if
        fdrop  turn-rate 2e f* turn-to fdrop exit
    then
    dup turn-rate turn-to fdrop
    my-at over vec-dist-xz fover f<= if  fdrop vec@ body-place-at exit  then
    drop  0e fswap body-move-local ;
