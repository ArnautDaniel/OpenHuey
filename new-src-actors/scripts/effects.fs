\ effects.fs - the rooms' effects (docs/subsystems/effects.md; src/game/effects.c): flames,
\ flickering sprites, specks, splashes, dust, shards, butterflies. The engine's particles
\ (game/particles.c) move and draw them; an effect here is an actor that says what and where,
\ passes on the story's commands, and ends with its effect or the room. The sparks a flame
\ throws and the motes butterflies shed are the engine's own: they go by themselves.
\
\ Two kinds of actor: a room effect sits in one of the room's 32 slots until the story removes
\ it (a flame, a flickering sprite, butterflies); a scene effect ends by itself, or when the
\ next room loads (the original's EffectMgr: specks, a splash, dust, shards).
IN: effects
USING: engine actors messages ;

state: fx-state
  cell field particles    \ the engine's particle system (-1 none)
end-state

behaviour room-effect
  on spawned ( -- )  -1 particles ! ;
  on particles-made ( p -- )  particles ! ;
  on effect-command ( v -- )  particles @ swap particles-command ;   \ (the scripts' 0x87)
  on effect-place ( x y z -- )  cells>xyz  particles @ particles-place ;   \ (moved by a room's command)
  on effect-rest ( n -- )  particles @ swap particles-rest! ;
  on killed ( -- )  particles @ particles-free ;
end-behaviour

behaviour scene-effect
  extends room-effect
  on spawned ( -- )  -1 particles !  self subscribe tick  self subscribe arrived ;
  on tick ( -- )  particles @ particles-alive? 0= if  self kill  then ;
  on arrived ( room exit -- )  2drop  self kill ;
end-behaviour

: effect-actor ( p beh name len -- id | -1 )   \ an actor keeping particle system p
    3 pick 0< if  2drop 2drop -1 exit  then
    fx-state -rot spawn  tuck send particles-made ;

\ ---- a reflecting quad (Reflection: the characters drawn again mirrored in its plane) - its
\ engine slot (0..3: `reflect!`) taken while it lives, let go when it is killed (if still its) ----
create refl-owners  4 cells allot  refl-owners 4 cells -1 fill
state: refl-state
  cell field refl-slot
end-state
behaviour reflecting
  on reflect-in ( slot -- )  dup refl-slot !  self swap cells refl-owners + ! ;
  on killed ( -- )  refl-slot @ cells refl-owners + @ self = if  refl-slot @ reflect-off  then ;
end-behaviour
: reflection ( slot -- id )  reflecting refl-state s" reflection" spawn  tuck send reflect-in ;

\ ---- room effects: their actor ids (the story keeps them in its slots) ----
: flicker ( slow -- id ) ( F: x y z -- )  flicker-new  room-effect s" flicker" effect-actor ;   \ 0x7F
: flame ( kind -- id ) ( F: x y z -- )  flame-new  room-effect s" flame" effect-actor ;          \ 0x86
: butterflies ( count -- id ) ( F: x y z -- )  butterflies-new  room-effect s" butterflies" effect-actor ;   \ 0x35

: tv-screen ( which -- id ) ( F: x y z -- )  fdrop fdrop fdrop  tv-new  room-effect s" tv" effect-actor ;   \ (TvScreen) effect-command on / off
\ ---- scene effects ----
: specks ( count spread rgba -- ) ( F: x y z -- )  swarm-new  scene-effect s" specks" effect-actor drop ;   \ 0x9F
: splash ( count spread rgba -- ) ( F: x y z from-x from-y from-z -- )   \ 0xA0
    splash-new  scene-effect s" splash" effect-actor drop ;
: burst ( kind r g b top -- ) ( F: x y z -- )  burst-new  scene-effect s" dust" effect-actor drop ;   \ 0xC8
: shards ( gs kind tex -- ) ( F: x y z u v du dv floor -- )   \ 0x8C
    shards-new  scene-effect s" shards" effect-actor drop ;
: stones ( gs tex -- ) ( F: height u v du dv -- )   \ (Effect79B00) stones falling from the height, on and on
    stones-new  scene-effect s" stones" effect-actor drop ;
: debris ( kind -- )  debris-new  scene-effect s" debris" effect-actor drop ;   \ (Debris) falling down the water tower
: rising-smoke ( -- )  rising-smoke-new  scene-effect s" smoke" effect-actor drop ;   \ (RisingSmoke) castle 2F's kiln
: dust-shaft ( -- )  dust-shaft-new  scene-effect s" dust-shaft" effect-actor drop ;   \ (DustShaft) the old mansion's
: spiral-smoke ( -- id )  spiral-smoke-new  scene-effect s" spiral-smoke" effect-actor ;   \ (SpiralSmoke) the old mansion's fire
: drips ( -- )  drips-new  scene-effect s" drips" effect-actor drop ;   \ (Drips) the old mansion's flooded room
: one-drip ( -- )  one-drip-new  scene-effect s" drip" effect-actor drop ;   \ (OneDrip) the Chaos Forest's
: rising-motes ( -- )  rising-motes-new  scene-effect s" motes" effect-actor drop ;   \ (RisingMotes) castle 1F's hall
: embers ( ending -- id )  embers-new  scene-effect s" embers" effect-actor ;   \ (OrangeSparks) castle 1F's fireplace
: flecks ( F: x y z half-w depth tx ty tz -- )  flecks-new  scene-effect s" flecks" effect-actor drop ;   \ (DriftingFlecks)
: smoke-puffs ( -- id )  smoke-puffs-new  scene-effect s" smoke-puffs" effect-actor ;   \ (SmokePuffs) castle B1's
: wall-shadow ( -- id )  wall-shadow-new  scene-effect s" wall-shadow" effect-actor ;   \ (WallShadow) the old mansion's
: fire ( spot big -- id )  fire-new  scene-effect s" fire" effect-actor ;   \ (Fire) the old mansion B1's; effect-command -1 puts it out
: steam ( spot -- id )  steam-new  scene-effect s" steam" effect-actor ;   \ (Room66Effect) effect-command -1 dies it down
: floor-glow ( -- id )  floor-glow-new  scene-effect s" floor-glow" effect-actor ;   \ (FloorGlow) effect-command its level
: big-fire ( -- )  big-fire-new  scene-effect s" big-fire" effect-actor drop ;   \ (BigFire) the old mansion 1F's
: light-shaft ( -- id )  light-shaft-new  scene-effect s" light-shaft" effect-actor ;   \ (LightShaft) effect-command 1 / 2 its haze
: glint ( model -- )  glint-new  scene-effect s" glint" effect-actor drop ;   \ (Glint) on its bone 6
: marker ( model other second? -- id ) ( F: size -- )  marker-new  scene-effect s" marker" effect-actor ;   \ (Marker) effect-command -1 done, -2 a pause, else size x 1000
: window-flash ( F: x y z turn -- )  window-flash-new  scene-effect s" window-flash" effect-actor drop ;   \ (WindowFlash) five frames, then gone
: mag-burst ( prima? -- ) ( F: x y z nx ny nz -- )  mag-burst-new  scene-effect s" mag-burst" effect-actor drop ;   \ (BurstA / BurstB) a magnesia going off
: smoke-trail ( thing -- )  smoke-trail-new  scene-effect s" smoke-trail" effect-actor drop ;   \ (SmokeTrail) till the thing is gone
: haze ( -- id )  haze-new  scene-effect s" haze" effect-actor ;   \ (Room49Effect) the old mansion 1F's
: caustic ( tex rgba -- id ) ( F: x y z size step-deg turn-deg -- )   \ (Caustic_Draw) light rippling on a surface
    deg>rad fswap deg>rad fswap  caustic-new  scene-effect s" caustic" effect-actor ;
: glow ( rgba -- id ) ( F: x y z size -- )  glow-new  scene-effect s" glow" effect-actor ;   \ (Effect737D0) standing still
: wisp-column ( -- id )  wisp-column-new  scene-effect s" wisp-column" effect-actor ;   \ (WispColumn) effect-command 0 fades it
: spark-spray ( side -- id )  spark-spray-new  scene-effect s" spark-spray" effect-actor ;   \ (SparkSpray) effect-command 0 off
: heat-haze ( -- id )  heat-haze-new  scene-effect s" heat-haze" effect-actor ;   \ (Room92Effect) effect-command its wave's mark
: pit-embers ( flames? -- id )  pit-embers-new  scene-effect s" pit-embers" effect-actor ;   \ (Embers) effect-command 0 off
: leaves ( -- )  leaves-new  scene-effect s" leaves" effect-actor drop ;   \ (Room2AWisps) the front garden's
: loop-sprite ( -- id ) ( F: x y z -- )  loop-sprite-new  scene-effect s" loop-sprite" effect-actor ;   \ (LoopingSprite) effect-place moves it
: object-glow ( idx -- id ) ( F: x y z -- )  object-glow-new  scene-effect s" object-glow" effect-actor ;   \ (ObjectGlow) effect-command 1 / 0 / $FF
: rain ( -- )  rain-new  scene-effect s" rain" effect-actor drop ;   \ (FallingDrops)
: ash ( -- id )  ash-new  scene-effect s" ash" effect-actor ;   \ (AshFlakes) the Chaos Forest's
: wander ( -- id ) ( F: x y z rx rz tx ty tz -- )   \ 0xA9: a flat thing wandering about its place
    wander-new  scene-effect s" wander" effect-actor ;
