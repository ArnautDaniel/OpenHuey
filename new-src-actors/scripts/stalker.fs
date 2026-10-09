\ stalker.fs - the stalkers (docs/subsystems/stalker.md; src/game/pursuer.c, debilitas.c): one
\ actor per stalker in the game. The family's behaviour (`stalking`) and each one's over it
\ (Debilitas: `debilitas`). P1: in the house - placed, standing, growling, telling the danger
\ he is about; searching the room (stalker/search.fs: his modes, his route of stops, walking).
IN: stalker
USING: engine game-state actors messages common facts flag-names room-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.tables stalker.attack stalker.chase stalker.grab ;

: me ( -- id )  self ;
: him ( -- a )  my-model @ actor ;   \ (his model's fields)
: played? ( -- flag )  in-game @ if  my-room room-id =  else  false  then ;   \ (Npc_InPlayedRoom)
: danger-id ( -- id )  s" danger" actor-named ;

\ his model where his body is, shown in the room being played
: pose ( -- )
    my-at sf@ him act.x sf!  my-at 4 + sf@ him act.y sf!  my-at 8 + sf@ him act.z sf!
    self body-yaw him act.yaw sf!
    played? 0<> 1 and him act.visible l! ;

\ ---- in the game and out (Pursuer_Activate: waiting about, his first animation) ----
\ in the story's character slot 2 (CharLoad_Partner: loaded and started, not yet anywhere) and
\ out of it; while loaded the cutscenes find its model by its kind (their cast)
: file! ( addr len -- )  31 min dup my-file c!  my-file 1+ swap move ;
: file ( -- addr len )  my-file 1+ my-file c@ ;
: load ( -- )  -1 loaded !  my-kind @ my-model @ cast-as  7 file sound-bank drop ;   \ (slot 2's sounds: bank 7)
: come-in ( room tri -- )
    loaded @ 0= if  load  then
    over room-id = if  dup tri-center body-place
    else  >r -1 0e 0e 0e body-place r> drop  then                   \ (out of sight: in that room, nowhere yet)
    -1 in-game !  0 growl-t !  0 my-move-mode !  -1 anim !
    hp @ 0> 0= if  hp-max @ hp !  then
    route-clear  my-path path-clear  3 mode!  0 play-now  stand  pose
    -1 came-by !  -1 making-for !  0 doors-n !  0 door-i !  0 away-search !  0 knock-t !  avoid-clear
    away? 0= if  search-start  then ;
: go-out ( -- )  let-her-go  0 in-game !  body-off  0 him act.visible l! ;

\ ---- his frame (Debilitas_Update: so far the growl, his model) and his part in the danger ----
: growl ( -- )   \ every 90 frames, standing on his own: sound $2A (bank 7)
    1 growl-t +!  growl-t @ 90 < if  exit  then
    0 growl-t !
    my-move-mode @ 0= anim @ $1300 <> and anim @ $1600 <> and if  $2A 7 sound  then ;
: frame ( -- )
    away? if  away  pose  my-room 0 my-mode @ 0= 1 and danger-id send stalker-here  exit  then
    thaw  senses  modes
    step @ ?dup if  execute  then  search
    played? growls @ and if  growl  then
    pose
    my-room  sees-fiona @ 0<> 1 and  my-mode @ 0= 1 and  danger-id send stalker-here ;
\ the family's setup (Pursuer_Setup): his sight 150 ahead, 60 degrees either side; Fiona and
\ Hewie known; listening for noises above his threshold (not his own)
: family-setup ( -- )
    150e view-range f!  1.0471976e view-half f!  -1e d-fiona f!  -1e d-hewie f!
    s" fiona" actor-named fiona-id !  s" hewie" actor-named hewie-id !
    hear-threshold @ stalker-noise s" acoustics" actor-named send listen ;

: unload ( -- )  0 loaded !  in-game @ if  go-out  then  my-kind @ -1 cast-as ;
behaviour stalking
  on stalker-load ( -- )  load ;
  on stalker-unload ( -- )  unload ;
  on stalker-in ( room tri -- )  come-in ;
  on stalker-out ( -- )  go-out ;
  on stalker-hunt ( -- )  in-game @ if  1 mode!  away? 0= if  search-again  then  then ;
  on entered-room ( room exit -- )  drop room-changed  in-game @ if  pose  then ;
  on tick ( -- )  in-game @ if  frame  then ;
  on heard ( loud room tri door source -- )  2drop hear ;
  on heard-nothing ( -- )  heard-none ;
  on door-held ( room exit -- )  nip swing-open ;
  on hit-taken ( kind -- )  struck-home ;
  on seize-taken ( -- )   \ (her answers to his taking her; one he isn't waiting for: let go)
      leading @ if  1 answer !  else  fiona send unhand  then ;
  on seize-refused ( -- )  2 answer ! ;
  on broke-free ( -- )  3 answer ! ;              \ (his blow struck her: his cry)
  on fiona-plight ( state -- )  her-plight ! ;
  on panic ( stage level -- )  drop her-stage ! ;     \ (the door he came in by: swung open)
  on door-refused ( room exit -- )  2drop ;            \ (it won't open for him: he gives up on it - with P3's doors)
end-behaviour

\ ---- Debilitas (debilitas.c Debilitas_Setup, DebilitasModel): 70 health (hard: 110), 5 across,
\ 20 tall; his model O_DB0/DB0_000, its motion table, his sounds (bank 7) ----
$2C020068 constant debilitas-floor   \ (Debilitas_BlockFlags: the triangles he can't stand on)
behaviour debilitas
  extends stalking
  on spawned ( -- )
      s" O_DB0/DB0_000" 2dup file!  actor-load dup my-model !  $3D89A0 motion-table
      0 him act.visible l!
      hard? if  110  else  70  then  dup hp-max !  hp !  2 my-kind !  -1 growls !  0 hear-threshold !  family-setup
      5e 20e body-size  debilitas-floor body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe fiona-plight  self subscribe panic ;
end-behaviour

: debilitas-spawn ( -- id )  debilitas stalker-state s" debilitas" spawn ;

\ ---- Daniella (daniella.c Daniella_Setup, DaniellaModel; kind 3): 120 health (hard: 100), 3
\ across, 17 tall; her model O_DNL/DNL_000 (hard: DNL_001), its motion table $419E60, her
\ sounds (bank 7); the Pursuer's floor (NPC_BlockFlags); her frame without his growl ----
$2C020028 constant pursuer-floor
behaviour daniella
  extends stalking
  on spawned ( -- )
      hard? if  s" O_DNL/DNL_001"  else  s" O_DNL/DNL_000"  then  2dup file!  actor-load dup my-model !  $419E60 motion-table
      0 him act.visible l!
      hard? if  100  else  120  then  dup hp-max !  hp !  3 my-kind !  0 growls !  12 hear-threshold !  family-setup
      3e 17e body-size  pursuer-floor body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe fiona-plight  self subscribe panic ;
end-behaviour
: daniella-spawn ( -- id )  daniella stalker-state s" daniella" spawn ;
