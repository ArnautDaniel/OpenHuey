\ stalker.fs - the stalkers (docs/subsystems/stalker.md; src/game/pursuer.c, debilitas.c): one
\ actor per stalker in the game. The family's behaviour (`stalking`) and each one's over it: his
\ tables, his numbers, his model; in the house - placed, standing, growling, telling the danger
\ he is about; searching the room (stalker/search.fs: his modes, his route of stops, walking).
IN: stalker
USING: engine game-state actors messages common facts flag-names room-names paths vectors hewie-names stalker-names stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.doorway stalker.tables stalker.attack stalker.chase stalker.leads stalker.pursue stalker.grab stalker.struck stalker.bitten stalker.riccardo stalker.lorenzo2 stalker.ladder stalker.head stalker.names stalker.anims stalker.sounds floor-flags ;

: me ( -- id )  self ;
: him ( -- a )  my-model @ actor ;   \ (his model's fields)
: played? ( -- flag )  in-game @ if  my-room room-id =  else  false  then ;   \ (Npc_InPlayedRoom)
: danger-id ( -- id )  s" danger" actor-named ;

\ his model where his body is, shown in the room being played (Daniella on her back: her hair
\ and back by her animation - the engine's, DaniellaModel_Vt3C)
: pose ( -- )
    my-at v.x sf@ him act.x sf!  my-at v.y sf@ him act.y sf!  my-at v.z sf@ him act.z sf!
    self body-yaw him act.yaw sf!
    played? under-t @ 0= and unseen @ 0= and 0<> 1 and him act.visible l! ;   \ (under the floor: Lorenzo's other class)

\ ---- in the game and out (Pursuer_Activate: waiting about, his first animation) ----
\ in the story's character slot 2 (CharLoad_Partner: loaded and started, not yet anywhere) and
\ out of it; while loaded the cutscenes find its model by its kind (their cast)
2 constant play-slot   \ (the stalker in play: his sounds in bank 7, saved)
: file! ( addr len -- )  31 min dup my-file c!  my-file 1+ swap move ;
: file ( -- addr len )  my-file 1+ my-file c@ ;
: load ( slot -- )   \ (slots 3..5 have no bank: Daniella as a scene's figure mid-Debilitas leaves his)
    dup my-slot !  -1 loaded !  my-kind @ my-model @ cast-as  play-slot = if  7 file sound-bank drop  then ;
: come-in ( room tri -- )
    loaded @ 0= if  play-slot load  then  forget-leads  0 beaten !
    {: room tri :}
    room room-id = if  room tri dup tri-center body-place
    else  room -1 0e 0e 0e body-place  then                         \ (out of sight: in that room, nowhere yet)
    -1 in-game !  0 growl-t !  own-move-mode my-move-mode !  -1 anim !
    hp @ 0> 0= if  hp-max @ hp !  then
    well-cond cond !  0 reeling !  0 hits !
    route-clear  my-path path-clear  waiting-about mode!  stand-anim play-now  stand  pose
    -1 came-by !  -1 to-shut !  -1 at-ladder !  -1 making-for !  0 doors-n !  0 door-i !  0 away-search !  0 knock-t !  avoid-clear
    out-over  -1 held-door !  -1 push-exit !  0 hunting-dog !  0 stun-t !  0 raging !  0 bitten !  held-by-dog bite-how !  0 dog-t !  0 at-dog !
    away? 0= if  search-start  then ;
: go-out ( -- )  let-her-go  let-door-go  out-over  forget-leads  0 in-game !  body-off  0 him act.visible l! ;

\ ---- the summoner (summoner.fs): told where he is each frame while in slot 2; brought in by it
\ out of sight (Pursuer_IntoRoomByEvent): by a noise after her, else waiting about - with a plan
\ heading for her, without one off by an exit at random ----
: status ( -- )
    in-game @ if  my-room  else  -1  then  my-mode @  in-game @ 0<>  in-game @ if  own-room?  else  false  then
    away-base @  cool-secs @  beaten @  s" summoner" actor-named send stalker-status ;
: summoned ( room exit found plan -- )
    {: room e found plan :}
    room -1 come-in  e came-by !
    found if
        her-room my-room = if  search-again  else  heading-for-her mode!  avoid-clear  head-for-her  then
    else
        waiting-about mode!  plan if  avoid-clear  head-for-her  else  wander  then
    then ;

\ ---- his frame (Debilitas_Update: so far the growl, his model) and his part in the danger ----
$1300 constant gesture-anims
: growl ( -- )   \ every 90 frames, standing on his own
    1 growl-t +!  growl-t @ 90 < if  exit  then
    0 growl-t !
    my-move-mode @ own-move-mode = anim @ gesture-anims <> and anim @ stand-about-anims <> and if  growl-sound 7 sound  then ;
: tell-danger ( -- )  my-room my-mode @ cond @ down-cond = danger-id send stalker-here ;   \ (SceneGame_Danger reads his mode, and whether he's down)
: frame ( -- )
    away? if  away  pose  tell-danger  exit  then
    thaw  hurt-down  senses  modes  dog-temper  shut-behind  ladder-watch
    step @ ?dup if  execute  then  search  looking   \ (Pursuer_Think: the head after the behaviour)
    played? growls @ and if  growl  then
    played? if  stalker.moving:footsteps  anim-sounds-play  then
    pose  tell-danger ;
\ what he is doing, for Hewie (the original's character fields he reads: room, move mode and
\ part, condition, health)
: move-mode-told ( -- mode )
    away? if  unreachable-mode exit  then
    reeling @ cond @ down-cond = or bitten @ or if  struck-mode exit  then
    leading @ if  engaged-mode  else  free-mode  then ;
: sub-told ( -- sub )
    bitten @ if  bitten-sub exit  then  cond @ down-cond = if  down-sub exit  then
    leading @ if  leading-sub  else  0  then ;
: tell-doing ( -- )
    my-room  move-mode-told  sub-told
    cond @  hp @  hp-max @  my-kind @  my-mode @  came-by @  broadcast stalker-doing
    d-fiona f@ f>cell broadcast stalker-reach
    away? 0=  combo @ 0< 0= and if  mv-damage  else  -1  then  broadcast stalker-blow ;
\ the family's setup (Pursuer_Setup): his sight 150 ahead, 60 degrees either side; Fiona and
\ Hewie known; listening for noises above his threshold (not his own)
: family-setup ( -- )
    -1 her-came !  0 temper !
    150e view-range f!  60e deg>rad view-half f!  -1e d-fiona f!  -1e d-hewie f!
    s" fiona" actor-named fiona-id !  s" hewie" actor-named hewie-id !
    hear-threshold @ stalker-noise s" acoustics" actor-named send listen
    pursuer-ladder ladder-offsets !  -1 her-ladder-on !  -1 at-ladder !
    self subscribe zone-target  self subscribe zone-targets-off  self subscribe fiona-unseen
    my-kind @ head-bones!  ahead-look look-mode ! ;

\ a door slammed off screen, the one he makes for: right behind it, its damage (not below 1) and
\ he is after her
: door-slammed-away ( damage exit -- )
    behind-door? 0= if  drop exit  then
    hp @ swap - 1 max hp !  after-her mode! ;
\ Hewie's scuffle with him off screen (Pursuer_Hit kind 8): its damage (not below 1), held those
\ frames more; hurt, his cry from the door he would come into her room by
: cry-through-door ( -- )
    world-held state-flag? hushed @ or if  exit  then
    next-door dup 0< if  drop exit  then  room-id door-exit-in dup 0< if  drop exit  then
    1 exit-spot drop  hurt-cry 7 bank-sound-at ;
: scuffled ( damage frames -- )
    stun-t +!  dup hp @ swap - 1 max hp !  0> if  cry-through-door  then ;
: blow-away ( kind damage how -- )
    rot case
        door-blow of  door-slammed-away  endof
        scuffle-blow of  scuffled  endof
        >r 2drop r>
    endcase ;
\ ---- the story's orders (the scripts' 0x2F, 0x30, 0x15: Pursuer_GoForFionaStance0 /
\ ChaseFionaHere / StartSearch, Pursuer_KnockedDown, Pursuer_Activate) - not while struck,
\ bitten or leading her: then his mode only ----
: up-again ( -- )  cond @ down-cond = if  well-cond cond !  0 down-t !  then ;
: busy? ( -- flag )  reeling @ bitten @ or leading @ or ;
: ordered ( mode -- )   \ (his wandering off given up: the order is his behaviour now)
    wandering-out @ if  let-door-go  out-over  then
    case
        after-her of  up-again  after-her mode!  avoid-clear  busy? if  exit  then
              played? if  sees-fiona @ if  chase-begin  else  chase-start  then  else  head-for-her  then  endof
        searching of  up-again  played? if  busy? if  searching mode!  else  search-again  then
              else  heading-for-her mode!  avoid-clear  head-for-her  then  endof
        waiting-about of  up-again  played? if  busy? if  searching mode!  else  search-again  then
              else  waiting-about mode!  route-clear  then  endof
    endcase ;
: knocked-out ( -- )  played? 0= if  exit  then  let-door-go  out-over  0 hp !  knock-down ;
: run-temper ( -- )   \ (the run: how much of Hewie he stands - Kind34_Activate 80, TintStalker_Reset 20,
    my-kind @ case                     \ Debilitas3_Activate 16)
        kind34-kind of  80 tolerance !  endof  tintstalker-kind of  20 tolerance !  endof
        debilitas-7-kind of  16 tolerance !  endof
    endcase ;
: activated ( -- )
    hewie-played? if  run-temper  then
    forget-leads  0 beaten !  -1 making-for !  0 down-t !  waiting-about mode!  900 mode-t !  0 raging !
    hp @ 0> 0= if  hp-max @ hp !  then ;
: unload ( -- )  0 loaded !  in-game @ if  go-out  then  my-kind @ -1 cast-as ;
create place-at /vec allot
behaviour stalking
  on stalker-load ( slot -- )  load ;
  \ the kind he is loaded as (Debilitas's: kind 7 is the Pursuer with his model, its chase the
  \ Pursuer's - Debilitas3)
  on stalker-as ( kind -- )
      dup debilitas-7-kind = if  my-kind !  -1 base-chase ! exit  then  drop
      my-kind @ debilitas-7-kind = if  debilitas-kind my-kind !  0 base-chase !  then ;   \ (himself again)
  on stalker-unload ( -- )  unload ;
  on stalker-in ( room tri -- )  come-in ;
  on place ( x y z yaw -- )   \ (put there, facing so: where a cutscene left him)
      in-game @ 0= if  2drop 2drop exit  then
      {: yaw :}  cells>xyz place-at vec!
      room-id  place-at my-floor @ v-tri-in  place-at vec@ body-place  yaw cell>f body-turn ;
  on stalker-out ( -- )  go-out ;
  on write-save ( -- )   \ (saves.fs) loaded: his kind, in the game or not, his place, health and mode
      loaded @ 0=  my-slot @ play-slot <> or if  exit  then   \ (a scene's figure isn't the stalker in play)
      my-kind @ .  in-game @ 0<> .  self body? if  self body-room .  self body-tri .  else  -1 . -1 .  then
      hp @ .  my-mode @ .  ." stalker-was" cr
      \ (Pursuer_SaveState: his condition, most health, rage, temper with Hewie, the damage he's had,
      \ his mode's clock, his time down, his heading)
      cond @ .  hp-max @ .  raging @ .  temper @ .  hits @ .  mode-t @ .  down-t @ .
      self body? if  self body-yaw  else  0e  then  f>cell .  ." stalker-more" cr ;
  on stalker-restore-more ( cond hp-max rage temper hits mode-t down-t yaw -- )   \ (Pursuer_LoadState)
      cell>f  self body? if  body-turn  else  fdrop  then
      down-t !  mode-t !  hits !  temper !  raging !  hp-max !  cond !
      cond @ down-cond =  in-game @ and if   \ (down: lying as he was, his time down to run)
          lying-front-anim lie-anim !  lie-anim @ play-now  lying down-stage !  -1 reeling !  ['] downed step!
      then ;
  on stalker-restore ( hp mode -- )  mode!  hp ! ;
  on noclip ( on -- )  dup 0<> noclipped !  if  0  else  my-floor @  then  body-mask! ;   \ (the story's 0x20: through blocked floor - at doors)
  on drawn-to ( tri -- )  in-game @ if  go-for-doll  else  drop  then ;
  on stalker-hunt ( -- )  in-game @ if  heading-for-her mode!  away? 0= if  search-again  then  then ;
  on entered-room ( room exit -- )  her-came !  room-changed  in-game @ if  pose  then ;
  on tick ( -- )  in-game @ if  frame  own-frame @ ?dup if  execute  then  tell-doing  then  loaded @ if  status  then ;
  \ Hewie: his attack asked, his letting go, his bites, his growls
  on dog-attack ( kind -- )  dog-attacked ;
  on shaken-off ( -- )  bitten @ bite-how @ held-by-dog = and if  dog-let-go bite-how !  then ;
  on bite ( damage hard -- )  bitten @ if  bitten-by  else  2drop  then ;
  on snap ( -- )  in-game @ if  snapped  then ;
  on grab-hewie ( -- )  in-game @ if  grab-hewie-behind  else  false  then  sender send grab-answer ;
  \ the story's orders
  on stalker-order ( mode -- )  in-game @ if  ordered  else  drop  then ;
  on stalker-clock ( frames -- )  mode-t ! ;
  on stalker-knock-out ( -- )  in-game @ if  knocked-out  then ;
  on rage ( on -- )  0<> raging ! ;
  on activate ( -- )  in-game @ if  activated  then ;
  on set-cond ( v -- )  cond ! ;
  on full-health ( -- )  hp-max @ hp ! ;
  on heal ( n -- )  abs hp @ + hp-max @ min hp ! ;
  on silent ( on -- )  0<> hushed ! ;
  \ his move and animation for the story's waits (the scripts' 0x7B, 0x9E): as Fiona's and Hewie's
  on frame-end ( -- )  in-game @ if  -1 ended? my-model @ 0 0 1 motion-events broadcast moving  then ;
  on stalker-summoned ( room exit found plan -- )  summoned ;
  on heard ( loud room tri door source -- )  nip hear ;
  on heard-nothing ( -- )  heard-none ;
  on door-held ( room exit -- )  nip held-answer ;   \ (his to open, or the door he came in by)
  on hit-taken ( kind -- )  struck-home ;
  on blow-from ( kind damage how x y z -- )   \ (from a point: Progress_Noise)
      cells>xyz strike-pt vec!  away? if  blow-away exit  then  -2 striker !  struck ;
  on blow ( kind damage how -- )   \ (her shove or kick; a door she slammed: no answer)
      away? if  blow-away exit  then
      sender striker !  2 pick door-blow = {: door? :}  struck  door? 0= if  sender send blow-taken  then ;
  on seize-taken ( -- )   \ (her answers to his taking her; one he isn't waiting for: let go)
      leading @ if  she-came answer !  else  fiona send unhand  then ;
  on seize-refused ( -- )  she-refused answer ! ;
  on broke-free ( -- )  she-broke-free answer ! ;              \ (his blow struck her: his cry)
  on fiona-plight ( state ladder -- )  her-ladder-on !  her-plight ! ;
  on zone-targets-off ( -- )  target-on #targets cells 0 fill ;
  on zone-target ( zone x y z -- )   \ (its centre: cells of floats)
      {: zone x y z :}  zone #targets u< 0= if  exit  then
      x cell>f y cell>f z cell>f  zone 3 * floats target-at + vec!
      -1 zone cells target-on + ! ;
  on panic ( stage level -- )  drop her-stage ! ;     \ (the door he came in by: swung open)
  on fiona-unseen ( on -- )  her-unseen ! ;
  on door-refused ( room exit -- )  nip refused-answer ;
end-behaviour

\ ---- each one's own: his tables and ways, his model, his numbers (by the hard setting), his
\ body ----
: by-hardness ( normal hard -- n )  hard-mode? if  nip  else  drop  then ;
: fby-hardness ( F: normal hard -- f )  hard-mode? if  fnip  else  fdrop  then ;
: ways ( kit pursuers? gives-up? -- ) ( F: turn-deg reach -- )   \ (his tables; the Pursuer's chase or his own; giving up)
    gives-up !  base-chase !  kit !  reach-k f!  deg>rad turn-k f! ;
: dressed ( id addr len -- )   \ (his model loaded, its motion table by name: hidden till he comes in)
    {: id a n :}  id my-model !  id a n motion-table  0 him act.visible l! ;
: model-file ( addr len -- id )  2dup file!  actor-load ;
: health! ( n -- )  dup hp-max !  hp ! ;
: hewie-temper ( tolerance dog-limit -- ) ( F: throw -- )  dog-limit !  tolerance !  throw-k f! ;
: no-own-rooms ( -- )  -1 own-a !  -1 own-b ! ;
: bodied ( floor -- ) ( F: width height -- )
    body-size  dup my-floor !  body-mask!  body-off  0 in-game !
    self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;

\ ---- Debilitas (debilitas.c Debilitas_Setup, DebilitasModel): 70 health (hard: 110), 5 across,
\ 20 tall; his model O_DB0/DB0_000, its motion table, his sounds (bank 7); his own rooms (his
\ lair, D_0041BE40, and their spots) ----
behaviour debilitas
  extends stalking
  on spawned ( -- )
      3e 16e  debilitas-kit false true ways
      s" O_DB0/DB0_000" model-file s" motions-debilitas" dressed
      70 110 by-hardness health!  debilitas-kind my-kind !  -1 growls !  0 hear-threshold !  family-setup
      4500 away-base !  90 cool-secs !  $20 own-a !  $02 own-b !  796 own-a-tri !  794 own-b-tri !
      10e  20 1800 hewie-temper  leaves-doors door-manner !  debilitas-ladder ladder-offsets !
      5e 20e debilitas-floor bodied ;
end-behaviour
: debilitas-spawn ( -- id )  debilitas stalker-state s" debilitas" spawn ;

\ ---- Daniella (daniella.c Daniella_Setup, DaniellaModel; kind 3): 120 health (hard: 100), 3
\ across, 17 tall; her model O_DNL/DNL_000, its textures DNL_000 (hard: DNL_000H), its motion
\ table $419E60, her sounds (bank 7); the Pursuer's floor (NPC_BlockFlags); her frame without
\ his growl ----
: daniella-motions ( -- addr len )  s" motions-daniella" ;
\ her blow's mark (Daniella_BlowEffect), as the one struck cries out or at her blow's effect key:
\ out along her hand (1.5, 3.5 for her third and fifth blows), big for her fourth and fifth; her
\ grabs on Fiona, big
$2D constant hand-bone
: blow# ( n -- anim )  $E00 + ;   \ (her blows: their animations)
: hand-mark ( heavy -- ) ( F: reach -- )
    {: heavy F: reach :}  0e 0e reach  my-model @ hand-bone bone-point  self body-tri heavy spray ;
: her-mark ( -- )
    anim @ case
        hold-on-anim of  fiona -1 0.75e mark-on  endof   carry-anim of  fiona -1 0.75e mark-on  endof
        5 blow# of  3.5e -1 hand-mark  endof         4 blow# of  1.5e -1 hand-mark  endof
        3 blow# of  3.5e 0 hand-mark  endof
        0 blow# of  1.5e 0 hand-mark  endof   2 blow# of  1.5e 0 hand-mark  endof
        6 blow# of  1.5e 0 hand-mark  endof   7 blow# of  1.5e 0 hand-mark  endof
    endcase ;
: her-frame ( -- )  effect-key? if  her-mark  then ;
: her-cry-mark ( who -- )  drop her-mark ;
: daniella-model ( addr len -- id )   \ (her model and its sounds' file; her textures by the hard setting)
    2dup file!  hard-mode? if  s" O_DNL/DNL_000H"  else  s" O_DNL/DNL_000"  then  actor-load-textured ;
: daniella-marks ( -- )  ['] her-frame own-frame !  ['] her-cry-mark own-mark ! ;
behaviour daniella
  extends stalking
  on spawned ( -- )
      4e 12e  daniella-kit true false ways  daniella-marks
      s" O_DNL/DNL_000" daniella-model daniella-motions dressed
      120 100 by-hardness health!  daniella-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      140 100 by-hardness cool-secs !  3600 6000 by-hardness away-base !  no-own-rooms   \ (Daniella_Setup's first)
      10e 25e fby-hardness  200 80 by-hardness 6000 hewie-temper
      shuts-doors-calm door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: daniella-spawn ( -- id )  daniella stalker-state s" daniella" spawn ;

\ ---- Daniella's later kinds (daniella.c Kind34_* / Kind35_* / Kind36_*): her chase, her look
\ (DaniellaModel; 34 and 36 the model DNL_001, 35 hers), each with its own tables (her kit's
\ shape: stalker/tables.fs) and numbers. 34 has her rage and pulls the doors she comes in by
\ shut while calm (left open on the hard setting); 35 and 36 have no rage and leave doors be ----
behaviour kind34
  extends stalking
  on spawned ( -- )
      4e 12e  kind34-kit true false ways  daniella-marks
      s" O_DNL/DNL_001" daniella-model daniella-motions dressed
      120 200 by-hardness health!  kind34-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      180 160 by-hardness cool-secs !  4500 6000 by-hardness away-base !  no-own-rooms   \ (Kind34_Setup)
      25e 50e fby-hardness  200 65 by-hardness 6000 hewie-temper
      shuts-doors-calm leaves-doors by-hardness door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind34-spawn ( -- id )  kind34 stalker-state s" kind34" spawn ;

behaviour kind35
  extends stalking
  on spawned ( -- )
      4e 12e  kind35-kit true false ways  daniella-marks
      s" O_DNL/DNL_000" daniella-model daniella-motions dressed
      65 80 by-hardness health!  kind35-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      140 100 by-hardness cool-secs !  3600 6000 by-hardness away-base !  no-own-rooms   \ (Kind35_Setup)
      15e 10e fby-hardness  120 75 by-hardness 3000 600 by-hardness hewie-temper
      leaves-doors door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind35-spawn ( -- id )  kind35 stalker-state s" kind35" spawn ;

behaviour kind36
  extends stalking
  on spawned ( -- )
      4e 12e  kind36-kit true false ways  daniella-marks
      s" O_DNL/DNL_001" daniella-model daniella-motions dressed
      65 80 by-hardness health!  kind36-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      180 160 by-hardness cool-secs !  4500 6000 by-hardness away-base !  no-own-rooms   \ (Kind36_Setup)
      25e 10e fby-hardness  120 65 by-hardness 3000 600 by-hardness hewie-temper
      leaves-doors door-manner !  daniella-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind36-spawn ( -- id )  kind36 stalker-state s" kind36" spawn ;

\ ---- Riccardo (riccardo.c Riccardo_Setup, RiccardoModel; kind 4): 100 health (hard: 125), 3
\ across, 17 tall; his model O_RCG/RCG_000, its motion table $41A2C0, his sounds (bank 7); the
\ Pursuer's floor; his own chase, flurry and lunge (stalker/riccardo.fs), his reach 20 ----
: riccardo-chase ( -- )  ['] ric-begin own-chase !  ['] ric-run own-run !  20e attack-range f! ;
behaviour riccardo
  extends stalking
  on spawned ( -- )
      4e 12e  riccardo-kit false true ways
      ['] cry-hit own-mark !  riccardo-chase  riccardo-acts own-acts !  ['] riccardo-frame own-frame !
      s" O_RCG/RCG_000" model-file s" motions-riccardo" dressed
      100 125 by-hardness health!  riccardo-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      90 cool-secs !  9000 away-base !  no-own-rooms   \ (Riccardo_Setup)
      15e 10e fby-hardness  50 1800 hewie-temper  shuts-doors door-manner !  riccardo-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: riccardo-spawn ( -- id )  riccardo stalker-state s" riccardo" spawn ;

\ ---- Lorenzo (lorenzo.c Lorenzo_Setup, LorenzoModel; kind 11): 50 health (hard: 75), 5 across,
\ 20 tall; his model O_LRO/LRO_000, its motion table $4246C0; the Pursuer's chase with his
\ tables, turning 2 degrees a frame, his reach 18; no temper with Hewie in his tables ----
behaviour lorenzo
  extends stalking
  on spawned ( -- )
      2e 12e  lorenzo-kit true true ways  18e attack-range f!
      s" O_LRO/LRO_000" model-file s" motions-lorenzo" dressed
      50 75 by-hardness health!  lorenzo-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      100 cool-secs !  3600 away-base !  no-own-rooms   \ (Lorenzo_Setup)
      0e  100 0 hewie-temper  leaves-doors door-manner !
      5e 20e pursuer-floor bodied ;
end-behaviour
: lorenzo-spawn ( -- id )  lorenzo stalker-state s" lorenzo" spawn ;

\ ---- Lorenzo's other class (lorenzo.c Lorenzo2_Setup; kind 10): 250 health (hard: 400), 4 across,
\ 18 tall; his model O_LRY/LRY_000 (motion table $424490); the Pursuer's chase with his kit (his
\ moves by the hard setting), his own actions under the floor (stalker/lorenzo2.fs) ----
behaviour lorenzo2
  extends stalking
  on spawned ( -- )
      4e 60e  lorenzo2-kit lorenzo2h-kit by-hardness true true ways
      lorenzo2-acts own-acts !  ['] lorenzo2-frame own-frame !  70e attack-range f!
      s" O_LRY/LRY_000" model-file s" motions-lorenzo2" dressed
      250 400 by-hardness health!  lorenzo2-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      150 cool-secs !  9000 away-base !  no-own-rooms   \ (Lorenzo2_Setup)
      50e 25e fby-hardness  100 600 hewie-temper  leaves-doors door-manner !
      4e 18e pursuer-floor bodied ;
end-behaviour
: lorenzo2-spawn ( -- id )  lorenzo2 stalker-state s" lorenzo2" spawn ;

\ ---- kind 39 (lorenzo.c Kind39_Setup): Lorenzo's other class again - its own tables, 250 health
\ (hard: 300), no "rage" (its reach 18, for Hewie 12), its sweep at whoever it sees ----
behaviour kind39
  extends stalking
  on spawned ( -- )
      4e 12e  kind39-kit kind39h-kit by-hardness true true ways
      lorenzo2-acts own-acts !  18e attack-range f!
      s" O_LRY/LRY_000" model-file s" motions-lorenzo2" dressed
      250 300 by-hardness health!  kind39-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      150 cool-secs !  9000 away-base !  no-own-rooms   \ (Kind39_Setup)
      30e 65e fby-hardness  40 75 by-hardness 300 360 by-hardness hewie-temper
      leaves-doors door-manner !
      4e 18e pursuer-floor bodied ;
end-behaviour
: kind39-spawn ( -- id )  kind39 stalker-state s" kind39" spawn ;

\ ---- Riccardo's other class (riccardo.c Kind37_Setup; kind 37): his chase with its own tables,
\ 100 health (hard: 125), 3 across, 17 tall, its model O_RCT/RCT_000 (motion table $42AED0);
\ near him at 30; struck enough, alerted (stalker/riccardo.fs `kind37-frame`) ----
behaviour kind37
  extends stalking
  on spawned ( -- )
      4e 12e  kind37-kit false true ways
      riccardo-chase  ['] kind37-frame own-frame !
      s" O_RCT/RCT_000" model-file s" motions-tintstalker" dressed
      100 125 by-hardness health!  kind37-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      45 cool-secs !  5400 away-base !  no-own-rooms   \ (Kind37_Setup)
      10e  50 900 hewie-temper  leaves-doors door-manner !  riccardo-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: kind37-spawn ( -- id )  kind37 stalker-state s" kind37" spawn ;

\ ---- the TintStalker (tintstalker.c TintStalker_Setup; kind 23): the Pursuer's chase with its
\ tables, Riccardo's look (O_RCT/RCT_000, motion table $42AED0), 100 health (hard: 125); seen
\ only a while after each turn (stalker/riccardo.fs `tint-frame`) ----
behaviour tintstalker
  extends stalking
  on spawned ( -- )
      4e 12e  tintstalker-kit true true ways
      ['] tint-frame own-frame !  -1 tint-was !  -1 unseen !
      s" O_RCT/RCT_000" model-file s" motions-tintstalker" dressed
      100 125 by-hardness health!  tintstalker-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      45 cool-secs !  5400 away-base !  no-own-rooms   \ (TintStalker_Setup)
      15e 10e fby-hardness  50 0 hewie-temper  shuts-doors door-manner !  riccardo-ladder ladder-offsets !
      3e 17e pursuer-floor bodied ;
end-behaviour
: tintstalker-spawn ( -- id )  tintstalker stalker-state s" tintstalker" spawn ;

\ ---- kind 12 (lorenzo.c Kind12_Setup): the Pursuer's chase with its tables, 9999 health and never
\ hurt, 3.5 across, 18 tall, its model O_LRF/LRF_000 (motion table $443FB0), reach 18, for Hewie
\ 14; in her room it seizes her (stalker/riccardo.fs `kind12-frame`) ----
behaviour kind12
  extends stalking
  on spawned ( -- )
      4e 14e  kind12-kit true false ways  18e attack-range f!
      ['] kind12-frame own-frame !  -1 mark-id !
      s" O_LRF/LRF_000" model-file s" motions-kind12" dressed
      9999 health!  kind12-kind my-kind !  0 growls !  12 hear-threshold !  family-setup
      999 cool-secs !  3000 away-base !  no-own-rooms   \ (Kind12_Setup)
      0e  100 0 hewie-temper  leaves-doors door-manner !
      3.5e 18e pursuer-floor bodied ;
end-behaviour
: kind12-spawn ( -- id )  kind12 stalker-state s" kind12" spawn ;
