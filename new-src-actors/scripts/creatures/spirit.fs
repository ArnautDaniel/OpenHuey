\ creatures/spirit.fs - a spirit (src/game/creature.c CreatureA_*; docs/subsystems/creatures.md):
\ a glow that waits until Fiona comes within its reach, then comes after her - straight at her
\ when it can, along a path when not, through the doors off screen when she leaves. Touching her
\ it vanishes in a burst of light and she falls; its time out first, it fades away. Its look -
\ bobbing, the trail, the burst - is the engine's (`spirit-new`: game/particles.c).
IN: creatures.spirit
USING: engine game-state actors messages common facts flag-names paths creatures.trip creatures.state creatures.her ;

: held? ( -- flag )  her-doing @ 0<> ;                          \ (a script has her)
: out-of-reach? ( -- flag )  hidden? her-mode @ 3 = or ;

\ ---- where it is ----
: in-play? ( -- flag )  my-room @ room-id = ;
: my-at ( -- v )  self body-at ;
: x-z ( v -- ) ( F: -- x z )  dup sf@  8 + sf@ ;
: measure-her ( -- )  in-play? if  my-at her-at vec-dist-xz her-dist f!  then ;
: path-mask ( -- mask )  $40080 ;   \ (its paths keep off these; it may stand and float anywhere)
: level-mask ( -- mask )   \ put somewhere at random: on its floor of a split room
    level @ case  0 of  $100000  endof  1 of  $200000  endof  >r 0 r>  endcase ;
: may-stand? ( tri -- flag )  dup 0< if  drop false exit  then  nav-flags  level-mask 8 or  and 0= ;
: anywhere ( -- tri )   \ a random triangle it may stand on (CreatureA_PlaceRandom)
    nav-tris 0= if  -1 exit  then
    1000 0 do  nav-tris random  dup may-stand? if  unloop exit  then  drop  loop  -1 ;
: onto ( tri -- )  dup my-tri !  dup 0< if  drop exit  then  room-id swap dup tri-center body-place ;
: on-the-floor ( -- )   \ in the room being played: on its triangle, or (none) somewhere at random
    0 body-mask!  my-tri @ dup 0< if  drop anywhere  then  onto ;

\ ---- kept in the progress, as the original keeps each slot (CreatureA_Save) ----
: record ( -- addr )  slot @ /kept-creature *  progress pr.creatures + ;
: keep ( -- )
    in-play? if  self body-tri my-tri !  then
    1 record kc.used l!  kind @ record kc.kind l!  reach @ record kc.reach l!  level @ record kc.level l!
    my-room @ record kc.room l!  my-tri @ record kc.tri l!  time @ record kc.time l!
    out @ record kc.out l!  come @ record kc.come l! ;
: unkeep ( -- )  record /kept-creature 0 fill ;
: from-record ( -- )
    record kc.kind l@ kind !  record kc.reach l@ reach !  record kc.level l@ level !
    record kc.room l@ my-room !  record kc.tri l@ my-tri !  record kc.time l@ time !
    record kc.out l@ 0<> out !  record kc.come l@ 0<> come !
    out @ if  4 random 1+ 1800 * rest !  then ;   \ (out, a rest again: CreatureA_SetupKind)

\ ---- coming for her (CreatureA_FionaDistance) ----
: reaches-her? ( -- flag )   \ straight from it onto her triangle
    in-play? 0= if  false exit  then  self body-tri my-at her-at 0 v-walk  her-tri = ;
: comes? ( -- flag )
    out @ if   \ (sent after her off screen: its rest first)
        rest @ if  -1 rest +!  false exit  then  in-play? 0=  exit
    then
    reach @ 0= if  true exit  then
    odd-one? if  1 waited +!  waited @ 61 >=  exit  then
    reaches-her?  her-dist f@ reach @ s>f f<= and ;

\ ---- after her (_StateAfterFiona, _StraightAtFiona) ----
: advance ( v -- there? )  x-z speed body-advance ;
: follow-path ( -- )   \ a step on to the next point; there, the one after
    my-path path-left? 0= if  exit  then
    my-path dup path-i path-point advance if  my-path dup path-i 1+ swap path-i!  then ;
: path-to ( tri v -- )   \ a new path, if the old one is done
    my-path path-left? if  2drop exit  then
    >r >r  my-path self body-tri my-at r> r> path-mask path-plan drop ;
: another-level? ( -- flag )  my-at 4 + sf@  her-at 4 + sf@ f= 0= ;
: giving-up? ( -- flag )   \ straight at her on another level: too many tries while close
    straight-only? another-level? 0= or if  false exit  then
    her-dist f@ 2e f< if  1 tries +!  then
    tries @ most-tries >=  self body-tri her-tri <> and dup if  0 tries !  true gave-up !  then ;
: straight-at-her ( -- went? )
    reaches-her? 0= if  false exit  then
    my-path path-clear
    giving-up? if  false exit  then
    her-dist f@ 1e f<= her-mode @ 4 = and 0= if  her-at advance drop  then  true ;
: after-her ( -- )
    her-mode @ 3 = if  exit  then
    gave-up @ 0= if
        straight-at-her if  exit  then
        self body-tri nav-flags $20000 and if  lifetime time !  then   \ (a triangle it can't cross: it gives up)
    then
    her-tri her-at path-to  follow-path ;

\ ---- hanging about her while she can't be reached (_StateHangAbout): a spot 10..25 before or
\ behind her, turned at random, on a triangle not its own nor blocked; there, another ----
create spot 12 allot   fvariable turn   fvariable off
: turn-of ( k -- ) ( F: -- a )  dup 8 > if  8 - negate  then  s>f 0.3925e f* ;   \ (D_004167A0)
: spot-at ( F: dx dz -- )   \ her place moved by (dx, dz)
    her-at 8 + sf@ f+  spot 8 + sf!   her-at sf@ f+  spot sf!   her-at 4 + sf@ spot 4 + sf! ;
: spot-try ( i -- found? )
    2 mod if  -1e  else  1e  then  16 random 10 + s>f f*  off f!    \ 10..25 before her, or behind
    0e off f@ turn f@ rotate-by spot-at                             \ turned by the try before's turn
    16 random turn-of turn f!
    spot v-tri  dup hang-tri !
    dup 0< over self body-tri = or if  drop false exit  then
    nav-flags $04020028 and 0= ;
: find-spot ( -- )  0e turn f!  -1 hang-tri !  17 0 do  i spot-try if  spot hang-at vec-copy  unloop exit  then  loop  -1 hang-tri ! ;
: hang-about ( -- )
    hang-again @ if  0 hang-again !  my-path path-clear  find-spot  then
    hang-tri @ 0< if  -1 hang-again !  exit  then
    hang-tri @ hang-at path-to
    my-path path-left? 0= if  -1 hang-again !  exit  then
    follow-path ;

\ ---- off screen (creatures/trip.fs): to her exit at half its speed, its legs at its speed
\ (2/3 with company) ----
: travel ( -- )
    speed f2/  speed company @ if  1.5e f/  then  trip trip-step 0= if  exit  then
    0 body-mask!  my-tri @ 0< if  on-the-floor  then ;   \ (in by a door: on its spot)

\ ---- its end: touching her, or its time out ----
: sound ( id -- )  in-play? 0= if  drop exit  then  5 0 0  my-at vec@ sound-at ;
: touched ( -- )   \ (_Seen) she falls; it bursts and sinks away
    $D my-at vec@ xyz>cells 0  her send fiona-request
    glow @ true spirit-vanish  cutscene-active? 0= if  $8B sound  then
    true vanishing !  true seen ! ;
: fade-away ( -- )  $8C sound  glow @ false spirit-vanish  true vanishing ! ;
: gone ( -- )   \ (_StateVanish's end) a red one shakes her; a blue one is heard in its room
    seen @ if
        red? if  3 my-at vec@ xyz>cells 0  her send fiona-request
        else  $80 my-room @ my-tri @ -1 world-noise  s" acoustics" actor-named send noise  then
    then
    unkeep  slot @ s" creatures" actor-named send creature-gone  self kill ;
: put-away ( -- )   \ (a room's: put away at its spot - Room4F / Room5C: the vanishing above it, blue
    \ or red, its sound $8B; out of play, unseen)
    red? if  $80 $30 $30  else  $30 $30 $80  then  $60
    my-at vec@ fswap  odd-one? if  -9e  else  0e  then  f+ 12e f+  fswap  vanish-new drop
    $8B sound  unkeep  slot @ s" creatures" actor-named send creature-gone  self kill ;
: touching-her? ( -- flag )  in-play? 0= if  false exit  then  self her 0e 0e bodies-touching? ;

\ ---- a frame (CreatureA_Frame, _Think) ----
: wait ( -- )   \ idle: on its triangle's centre the first time
    0 move-mode !  centred @ 0= in-play? and if  -1 centred !  self body-tri dup 0< if  drop exit  then  onto  then ;
: in-her-room ( -- )
    come @ 0= if  wait exit  then
    1 time +!
    another-level? 0= if  0 gave-up !  then
    out-of-reach? charmed? 0= and if
        move-mode @ 2 <> if  2 move-mode !  -1 hang-again !  then  hang-about exit
    then
    0 move-mode !
    hidden? held? and if  exit  then
    trip my-at door-watch if   \ (by a swinging door: waiting; left shut, through it)
        trip gone-through? if  my-path path-clear  body-off  then  exit
    then
    charmed? her-dist f@ 40e f> and if  my-path path-clear exit  then
    after-her ;
: elsewhere ( -- )
    0 move-mode !  0 gave-up !
    come @ 0= if  exit  then
    1 time +!  time @ lifetime >= if  gone exit  then
    travel ;
: shown? ( -- flag )  in-play? 0= if  false exit  then  out @ if  come @  else  rest @ 0=  then ;
: show ( -- )
    glow @  shown?  in-play? if  my-at vec@  else  0e 0e 0e  then  spirit-at
    slot @ my-room @ move-mode @ come @ 0<> broadcast creature-doing ;
: frame ( -- )
    vanishing @ if  show  glow @ particles-alive? 0= if  gone  then  exit  then
    measure-her
    come @ 0= if  comes? if  -1 come !  then  then
    in-play? if  in-her-room  else  elsewhere  then
    vanishing @ if  exit  then
    come @ touching-her? and hidden? 0= and held? 0= and her-plight @ 3 <> and if  touched  show exit  then   \ (left be while out of reach)
    in-play? come @ and time @ lifetime >= and if  fade-away  then
    show keep ;

: start ( -- )   \ its look, and on the floor if in the room being played
    in-play? if  on-the-floor  else  body-off  then
    red? 0<>  my-room @ room-id = if  my-at vec@  else  0e 0e 0e  then  odd-one? if  -9e  else  0e  then  spirit-new  glow !
    trip trip-reset  my-path path-clear  keep ;

behaviour spirit
  on spawned ( -- )
      -1 glow !  trip trip-reset  1.5e 16e body-size  body-off
      self subscribe tick  self subscribe arrived  self subscribe leaving-room  self subscribe fiona-doing
      self subscribe fiona-plight ;
  on spirit-setup ( slot room level tri kind reach -- )
      10 * reach !  kind !  my-tri !  level !  my-room !  slot !  start ;
  on spirit-kept ( slot -- )  slot !  from-record  start ;
  on spirit-sent ( slot room company -- )   \ (CreatureA_Setup, _ComeAfterFiona) any kind, on level 2, out and resting
      0<> company !  my-room !  slot !  -1 my-tri !  16 random kind !  0 reach !  2 level !
      true out !  false come !  4 random 1+ 1800 * rest !  start ;
  on tick ( -- )  glow @ 0< if  exit  then  frame ;
  on fiona-doing ( mode sub cond cmd doing -- )  her-doing !  2drop drop  her-mode ! ;
  on fiona-plight ( state ladder -- )  drop her-plight ! ;
  on leaving-room ( room exit -- )   \ she leaves: in her room and after her, it follows
      over my-room @ = come @ and vanishing @ 0= and if  trip swap follow-out drop exit  then  2drop ;
  on arrived ( room exit -- )   \ a room in: back on its floor if it's there, else out of play
      2drop  in-play? if  on-the-floor  else  body-off  then  0 centred ! ;
  on killed ( -- )  glow @ particles-free ;
  on put-away ( -- )  in-play? if  put-away  then ;
  on set-level ( level -- )  level ! ;   \ (its floor of a split room: a room's command - Room48)
end-behaviour

: spirit-spawn ( -- id )  spirit spirit-state s" spirit" spawn ;
