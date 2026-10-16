\ stalker.fs - the stalkers (docs/subsystems/stalker.md; src/game/pursuer.c, debilitas.c): one
\ actor per stalker in the game. The family's behaviour (`stalking`) and each one's over it
\ (Debilitas: `debilitas`). P1: in the house - placed, standing, growling, telling the danger
\ he is about; searching the room (stalker/search.fs: his modes, his route of stops, walking).
IN: stalker
USING: engine game-state actors messages common facts flag-names room-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.doorway stalker.tables stalker.attack stalker.chase stalker.leads stalker.pursue stalker.grab stalker.struck stalker.bitten stalker.riccardo stalker.lorenzo2 stalker.ladder stalker.head ;

: me ( -- id )  self ;
: him ( -- a )  my-model @ actor ;   \ (his model's fields)
: played? ( -- flag )  in-game @ if  my-room room-id =  else  false  then ;   \ (Npc_InPlayedRoom)
: danger-id ( -- id )  s" danger" actor-named ;

\ his model where his body is, shown in the room being played (Daniella on her back: her hair
\ and back by her animation - the engine's, DaniellaModel_Vt3C)
: pose ( -- )
    my-at sf@ him act.x sf!  my-at 4 + sf@ him act.y sf!  my-at 8 + sf@ him act.z sf!
    self body-yaw him act.yaw sf!
    played? under-t @ 0= and unseen @ 0= and 0<> 1 and him act.visible l! ;   \ (under the floor: Lorenzo's other class)

\ ---- in the game and out (Pursuer_Activate: waiting about, his first animation) ----
\ in the story's character slot 2 (CharLoad_Partner: loaded and started, not yet anywhere) and
\ out of it; while loaded the cutscenes find its model by its kind (their cast)
: file! ( addr len -- )  31 min dup my-file c!  my-file 1+ swap move ;
: file ( -- addr len )  my-file 1+ my-file c@ ;
: load ( slot -- )   \ (bank 7 holds slot 2's sounds - the one in play; slots 3..5 have no bank:
    \ Daniella as a scene's figure mid-Debilitas leaves his)
    dup my-slot !  -1 loaded !  my-kind @ my-model @ cast-as  2 = if  7 file sound-bank drop  then ;
: come-in ( room tri -- )
    loaded @ 0= if  2 load  then  forget-leads  0 beaten !
    over room-id = if  dup tri-center body-place
    else  >r -1 0e 0e 0e body-place r> drop  then                   \ (out of sight: in that room, nowhere yet)
    -1 in-game !  0 growl-t !  0 my-move-mode !  -1 anim !
    hp @ 0> 0= if  hp-max @ hp !  then
    0 cond !  0 reeling !  0 hits !
    route-clear  my-path path-clear  3 mode!  0 play-now  stand  pose
    -1 came-by !  -1 to-shut !  -1 at-ladder !  -1 making-for !  0 doors-n !  0 door-i !  0 away-search !  0 knock-t !  avoid-clear
    out-over  -1 held-door !  -1 push-exit !  0 stun-t !  0 raging !  0 bitten !  0 bite-how !  0 dog-t !  0 at-dog !
    away? 0= if  search-start  then ;
: go-out ( -- )  let-her-go  let-door-go  out-over  forget-leads  0 in-game !  body-off  0 him act.visible l! ;

\ ---- the summoner (summoner.fs): told where he is each frame while in slot 2; brought in by it
\ out of sight (Pursuer_IntoRoomByEvent): by a noise after her, else waiting about - with a plan
\ heading for her, without one off by an exit at random ----
: status ( -- )
    in-game @ if  my-room  else  -1  then  my-mode @  in-game @ 0<>  in-game @ if  own-room?  else  false  then
    away-base @  cool-secs @  beaten @  s" summoner" actor-named send stalker-status ;
: summoned ( room exit found plan -- )
    >r >r  over -1 come-in  came-by !  drop  r> r>                    ( found plan )
    swap if  drop
        her-room my-room = if  search-again  else  1 mode!  avoid-clear  head-for-her  then
    else
        3 mode!  if  avoid-clear  head-for-her  else  wander  then
    then ;

\ ---- his frame (Debilitas_Update: so far the growl, his model) and his part in the danger ----
: growl ( -- )   \ every 90 frames, standing on his own: sound $2A (bank 7)
    1 growl-t +!  growl-t @ 90 < if  exit  then
    0 growl-t !
    my-move-mode @ 0= anim @ $1300 <> and anim @ $1600 <> and if  $2A 7 sound  then ;
: frame ( -- )
    away? if  away  pose  my-room 0 my-mode @ 0= 1 and danger-id send stalker-here  exit  then
    thaw  hurt-down  senses  modes  dog-temper  shut-behind  ladder-watch
    step @ ?dup if  execute  then  search  looking   \ (Pursuer_Think: the head after the behaviour)
    played? growls @ and if  growl  then
    played? if  stalker.moving:footsteps  anim-sounds-play  then
    pose
    my-room  sees-fiona @ 0<> 1 and  my-mode @ 0= 1 and  danger-id send stalker-here ;
\ what he is doing, for Hewie (the original's character fields he reads: room, move mode and
\ part, condition, health)
: tell-doing ( -- )
    my-room
    away? if  3  else  reeling @ cond @ 2 = or bitten @ or if  4  else  leading @ if  8  else  0  then  then  then
    bitten @ if  9  else  cond @ 2 = if  $A  else  leading @ if  $18  else  0  then  then  then
    cond @  hp @  hp-max @  my-kind @  my-mode @  came-by @  broadcast stalker-doing
    away? 0=  combo @ 0< 0= and if  mv-damage  else  -1  then  broadcast stalker-blow ;
\ the family's setup (Pursuer_Setup): his sight 150 ahead, 60 degrees either side; Fiona and
\ Hewie known; listening for noises above his threshold (not his own)
: family-setup ( -- )
    150e view-range f!  1.0471976e view-half f!  -1e d-fiona f!  -1e d-hewie f!
    s" fiona" actor-named fiona-id !  s" hewie" actor-named hewie-id !
    hear-threshold @ stalker-noise s" acoustics" actor-named send listen
    pursuer-ladder ladder-offsets !  -1 her-ladder-on !  -1 at-ladder !
    self subscribe zone-target  self subscribe zone-targets-off  self subscribe fiona-unseen
    my-kind @ head-bones!  4 look-mode ! ;

\ a door slammed off screen, the one he makes for: right behind it, its damage (not below 1) and
\ he is after her
: door-slammed-away ( damage exit -- )
    behind-door? 0= if  drop exit  then
    hp @ swap - 1 max hp !  0 mode! ;
\ Hewie's scuffle with him off screen (Pursuer_Hit kind 8): its damage (not below 1), held those
\ frames more; hurt, his cry ($1D) from the door he would come into her room by
: cry-through-door ( -- )
    world-held state-flag? hushed @ or if  exit  then
    next-door dup 0< if  drop exit  then  room-id door-exit-in dup 0< if  drop exit  then
    1 exit-spot drop  $1D 7 bank-sound-at ;
: scuffled ( damage frames -- )
    stun-t +!  dup hp @ swap - 1 max hp !  0> if  cry-through-door  then ;
: blow-away ( kind damage how -- )
    rot case
        5 of  door-slammed-away  endof
        8 of  scuffled  endof
        >r 2drop r>
    endcase ;
\ ---- the story's orders (the scripts' 0x2F, 0x30, 0x15: Pursuer_GoForFionaStance0 /
\ ChaseFionaHere / StartSearch, Pursuer_KnockedDown, Pursuer_Activate) - not while struck,
\ bitten or leading her: then his mode only ----
: up-again ( -- )  cond @ 2 = if  0 cond !  0 down-t !  then ;
: busy? ( -- flag )  reeling @ bitten @ or leading @ or ;
: ordered ( mode -- )   \ (his wandering off given up: the order is his behaviour now)
    wandering-out @ if  let-door-go  out-over  then
    case
        0 of  up-again  0 mode!  avoid-clear  busy? if  exit  then
              played? if  sees-fiona @ if  chase-begin  else  chase-start  then  else  head-for-her  then  endof
        2 of  up-again  played? if  busy? if  2 mode!  else  search-again  then
              else  1 mode!  avoid-clear  head-for-her  then  endof
        3 of  up-again  played? if  busy? if  2 mode!  else  search-again  then
              else  3 mode!  route-clear  then  endof
    endcase ;
: knocked-out ( -- )  played? 0= if  exit  then  let-door-go  out-over  0 hp !  knock-down ;
: activated ( -- )
    forget-leads  0 beaten !  -1 making-for !  0 down-t !  3 mode!  900 mode-t !  0 raging !
    hp @ 0> 0= if  hp-max @ hp !  then ;
: unload ( -- )  0 loaded !  in-game @ if  go-out  then  my-kind @ -1 cast-as ;
create place-at 12 allot  fvariable place-yaw
behaviour stalking
  on stalker-load ( slot -- )  load ;
  on stalker-unload ( -- )  unload ;
  on stalker-in ( room tri -- )  come-in ;
  on place ( x y z yaw -- )   \ (put there, facing so: where a cutscene left him)
      in-game @ 0= if  2drop 2drop exit  then
      cell>f place-yaw f!  cells>xyz place-at vec!
      room-id  place-at my-floor @ v-tri-in  place-at vec@ body-place  place-yaw f@ body-turn ;
  on stalker-out ( -- )  go-out ;
  on write-save ( -- )   \ (saves.fs) loaded: his kind, in the game or not, his place, health and mode
      loaded @ 0=  my-slot @ 2 <> or if  exit  then   \ (a scene's figure isn't the stalker in play)
      my-kind @ .  in-game @ 0<> .  self body? if  self body-room .  self body-tri .  else  -1 . -1 .  then
      hp @ .  my-mode @ .  ." stalker-was" cr
      \ (Pursuer_SaveState: his condition, most health, rage, temper with Hewie, the damage he's had,
      \ his mode's clock, his time down, his heading)
      cond @ .  hp-max @ .  raging @ .  temper @ .  hits @ .  mode-t @ .  down-t @ .
      self body? if  self body-yaw  else  0e  then  f>cell .  ." stalker-more" cr ;
  on stalker-restore-more ( cond hp-max rage temper hits mode-t down-t yaw -- )   \ (Pursuer_LoadState)
      cell>f  self body? if  body-turn  else  fdrop  then
      down-t !  mode-t !  hits !  temper !  raging !  hp-max !  cond !
      cond @ 2 =  in-game @ and if   \ (down: lying as he was, his time down to run - $1806)
          lying-front lie-anim !  lie-anim @ play-now  1 down-stage !  -1 reeling !  ['] downed step!
      then ;
  on stalker-restore ( hp mode -- )  mode!  hp ! ;
  on noclip ( on -- )  dup 0<> noclipped !  if  0  else  my-floor @  then  body-mask! ;   \ (the story's 0x20: through blocked floor - at doors)
  on drawn-to ( tri -- )  in-game @ if  go-for-doll  else  drop  then ;
  on stalker-hunt ( -- )  in-game @ if  1 mode!  away? 0= if  search-again  then  then ;
  on entered-room ( room exit -- )  her-came !  room-changed  in-game @ if  pose  then ;
  on tick ( -- )  in-game @ if  frame  own-frame @ ?dup if  execute  then  tell-doing  then  loaded @ if  status  then ;
  \ Hewie (P3c): his attack asked, his letting go, his bites, his growls
  on dog-attack ( kind -- )  dog-attacked ;
  on shaken-off ( -- )  bitten @ bite-how @ 0= and if  2 bite-how !  then ;
  on bite ( damage hard -- )  bitten @ if  bitten-by  else  2drop  then ;
  on snap ( -- )  in-game @ if  snapped  then ;
  on grab-hewie ( -- )  in-game @ if  grab-hewie-behind  else  false  then  sender send grab-answer ;
  \ the story's orders (P4)
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
      sender striker !  2 pick 5 = >r  struck  r> 0= if  sender send blow-taken  then ;
  on seize-taken ( -- )   \ (her answers to his taking her; one he isn't waiting for: let go)
      leading @ if  1 answer !  else  fiona send unhand  then ;
  on seize-refused ( -- )  2 answer ! ;
  on broke-free ( -- )  3 answer ! ;              \ (his blow struck her: his cry)
  on fiona-plight ( state ladder -- )  her-ladder-on !  her-plight ! ;
  on zone-targets-off ( -- )  target-on #targets cells 0 fill ;
  on zone-target ( zone x y z -- )   \ (its centre: cells of floats)
      3 pick #targets u< 0= if  2drop 2drop exit  then
      >r >r >r  dup 3 * floats target-at +  r> cell>f dup sf!  r> cell>f dup 4 + sf!  r> cell>f 8 + sf!
      cells target-on +  -1 swap ! ;
  on panic ( stage level -- )  drop her-stage ! ;     \ (the door he came in by: swung open)
  on fiona-unseen ( on -- )  her-unseen ! ;
  on door-refused ( room exit -- )  nip refused-answer ;
end-behaviour

\ ---- Debilitas (debilitas.c Debilitas_Setup, DebilitasModel): 70 health (hard: 110), 5 across,
\ 20 tall; his model O_DB0/DB0_000, its motion table, his sounds (bank 7) ----
$2C020068 constant debilitas-floor   \ (Debilitas_BlockFlags: the triangles he can't stand on)
behaviour debilitas
  extends stalking
  on spawned ( -- )
      -1 her-came !
      debilitas-kit kit !  0 base-chase !  -1 gives-up !  0.052359879e turn-k f!  16e reach-k f!   \ (3 degrees)
      s" O_DB0/DB0_000" 2dup file!  actor-load dup my-model !  $3D89A0 motion-table
      0 him act.visible l!
      hard-mode? if  110  else  70  then  dup hp-max !  hp !  2 my-kind !  -1 growls !  0 hear-threshold !  family-setup
      4500 away-base !  90 cool-secs !  $20 own-a !  $02 own-b !   \ (Debilitas_Setup; his rooms D_0041BE40,
      796 own-a-tri !  794 own-b-tri !                             \ and their spots)
      20 tolerance !  10e throw-k f!  0 temper !   \ (his temper with Hewie)
      1800 dog-limit !  leaves-doors door-manner !  debilitas-ladder ladder-offsets !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      5e 20e body-size  debilitas-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour

: debilitas-spawn ( -- id )  debilitas stalker-state s" debilitas" spawn ;

\ ---- Daniella (daniella.c Daniella_Setup, DaniellaModel; kind 3): 120 health (hard: 100), 3
\ across, 17 tall; her model O_DNL/DNL_000, its textures DNL_000 (hard: DNL_000H), its motion
\ table $419E60, her sounds (bank 7); the Pursuer's floor (NPC_BlockFlags); her frame without
\ his growl ----
$2C020028 constant pursuer-floor
$419E60 constant daniella-motions
\ her blow's mark (Daniella_BlowEffect), as the one struck cries out or at her blow's effect key:
\ out along her hand (bone $2D: 1.5, 3.5 for $E03 / $E05), big for $E04 / $E05; her grabs ($1904,
\ $1A01) on Fiona, big
$2D constant hand-bone
: hand-mark ( heavy -- ) ( F: reach -- )
    >r  0e 0e frot  my-model @ hand-bone bone-point  self body-tri r> spray ;
: her-mark ( -- )
    anim @ case
        $1904 of  fiona -1 0.75e mark-on  endof   $1A01 of  fiona -1 0.75e mark-on  endof
        $E05 of  3.5e -1 hand-mark  endof         $E04 of  1.5e -1 hand-mark  endof
        $E03 of  3.5e 0 hand-mark  endof
        $E00 of  1.5e 0 hand-mark  endof   $E02 of  1.5e 0 hand-mark  endof
        $E06 of  1.5e 0 hand-mark  endof   $E07 of  1.5e 0 hand-mark  endof
    endcase ;
: her-frame ( -- )  effect-key? if  her-mark  then ;
: her-cry-mark ( who -- )  drop her-mark ;
: daniella-model ( addr len -- id )   \ (her model and its sounds' file; her textures by the hard setting)
    2dup file!  hard-mode? if  s" O_DNL/DNL_000H"  else  s" O_DNL/DNL_000"  then  actor-load-textured ;
behaviour daniella
  extends stalking
  on spawned ( -- )
      -1 her-came !
      daniella-kit kit !  -1 base-chase !  0 gives-up !  0.06981317e turn-k f!  12e reach-k f!   \ (4 degrees)
      ['] her-frame own-frame !  ['] her-cry-mark own-mark !
      s" O_DNL/DNL_000" daniella-model dup my-model !  daniella-motions motion-table
      0 him act.visible l!
      hard-mode? if  100  else  120  then  dup hp-max !  hp !  3 my-kind !  0 growls !  12 hear-threshold !  family-setup
      hard-mode? if  6000 100  else  3600 140  then  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Daniella_Setup's first)
      hard-mode? if  80 25e  else  200 10e  then  throw-k f!  tolerance !  0 temper !   \ (her temper with Hewie)
      6000 dog-limit !  shuts-doors-calm door-manner !  daniella-ladder ladder-offsets !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: daniella-spawn ( -- id )  daniella stalker-state s" daniella" spawn ;

\ ---- Daniella's later kinds (daniella.c Kind34_* / Kind35_* / Kind36_*): her chase, her look
\ (DaniellaModel; 34 and 36 the model DNL_001, 35 hers), each with its own tables (her kit's
\ shape: stalker/tables.fs) and numbers. 34 has her rage and pulls the doors she comes in by
\ shut while calm (left open on the hard setting); 35 and 36 have no rage and leave doors be ----
behaviour kind34
  extends stalking
  on spawned ( -- )
      -1 her-came !
      kind34-kit kit !  -1 base-chase !  0 gives-up !  0.06981317e turn-k f!  12e reach-k f!
      ['] her-frame own-frame !  ['] her-cry-mark own-mark !
      s" O_DNL/DNL_001" daniella-model dup my-model !  daniella-motions motion-table
      0 him act.visible l!
      hard-mode? if  200  else  120  then  dup hp-max !  hp !  34 my-kind !  0 growls !  12 hear-threshold !  family-setup
      hard-mode? if  6000 160  else  4500 180  then  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Kind34_Setup)
      hard-mode? if  65 50e  else  200 25e  then  throw-k f!  tolerance !  0 temper !   \ (her temper with Hewie)
      6000 dog-limit !  hard-mode? if  leaves-doors  else  shuts-doors-calm  then  door-manner !  daniella-ladder ladder-offsets !
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: kind34-spawn ( -- id )  kind34 stalker-state s" kind34" spawn ;

behaviour kind35
  extends stalking
  on spawned ( -- )
      -1 her-came !
      kind35-kit kit !  -1 base-chase !  0 gives-up !  0.06981317e turn-k f!  12e reach-k f!
      ['] her-frame own-frame !  ['] her-cry-mark own-mark !
      s" O_DNL/DNL_000" daniella-model dup my-model !  daniella-motions motion-table
      0 him act.visible l!
      hard-mode? if  80  else  65  then  dup hp-max !  hp !  35 my-kind !  0 growls !  12 hear-threshold !  family-setup
      hard-mode? if  6000 100  else  3600 140  then  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Kind35_Setup)
      hard-mode? if  75 10e  else  120 15e  then  throw-k f!  tolerance !  0 temper !   \ (her temper with Hewie)
      hard-mode? if  600  else  3000  then  dog-limit !  leaves-doors door-manner !  daniella-ladder ladder-offsets !
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: kind35-spawn ( -- id )  kind35 stalker-state s" kind35" spawn ;

behaviour kind36
  extends stalking
  on spawned ( -- )
      -1 her-came !
      kind36-kit kit !  -1 base-chase !  0 gives-up !  0.06981317e turn-k f!  12e reach-k f!
      ['] her-frame own-frame !  ['] her-cry-mark own-mark !
      s" O_DNL/DNL_001" daniella-model dup my-model !  daniella-motions motion-table
      0 him act.visible l!
      hard-mode? if  80  else  65  then  dup hp-max !  hp !  36 my-kind !  0 growls !  12 hear-threshold !  family-setup
      hard-mode? if  6000 160  else  4500 180  then  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Kind36_Setup)
      hard-mode? if  65 10e  else  120 25e  then  throw-k f!  tolerance !  0 temper !   \ (her temper with Hewie)
      hard-mode? if  600  else  3000  then  dog-limit !  leaves-doors door-manner !  daniella-ladder ladder-offsets !
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: kind36-spawn ( -- id )  kind36 stalker-state s" kind36" spawn ;

\ ---- Riccardo (riccardo.c Riccardo_Setup, RiccardoModel; kind 4): 100 health (hard: 125), 3
\ across, 17 tall; his model O_RCG/RCG_000, its motion table $41A2C0, his sounds (bank 7); the
\ Pursuer's floor; his own chase, flurry and lunge (stalker/riccardo.fs), his reach 20 ----
behaviour riccardo
  extends stalking
  on spawned ( -- )
      -1 her-came !
      riccardo-kit kit !  0 base-chase !  -1 gives-up !  0.06981317e turn-k f!  12e reach-k f!   \ (4 degrees)
      ['] cry-hit own-mark !
      ['] ric-begin own-chase !  ['] ric-run own-run !  riccardo-acts own-acts !  20e attack-range f!
      ['] riccardo-frame own-frame !
      s" O_RCG/RCG_000" 2dup file!  actor-load dup my-model !  $41A2C0 motion-table
      0 him act.visible l!
      hard-mode? if  125  else  100  then  dup hp-max !  hp !  4 my-kind !  0 growls !  12 hear-threshold !  family-setup
      9000 90  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Riccardo_Setup)
      hard-mode? if  10e  else  15e  then  throw-k f!  50 tolerance !  0 temper !   \ (his temper with Hewie)
      1800 dog-limit !  shuts-doors door-manner !  riccardo-ladder ladder-offsets !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: riccardo-spawn ( -- id )  riccardo stalker-state s" riccardo" spawn ;

\ ---- Lorenzo (lorenzo.c Lorenzo_Setup, LorenzoModel; kind 11): 50 health (hard: 75), 5 across,
\ 20 tall; his model O_LRO/LRO_000, its motion table $4246C0; the Pursuer's chase with his
\ tables, turning 2 degrees a frame, his reach 18; no temper with Hewie in his tables ----
behaviour lorenzo
  extends stalking
  on spawned ( -- )
      -1 her-came !
      lorenzo-kit kit !  -1 base-chase !  -1 gives-up !  0.03490659e turn-k f!  12e reach-k f!  18e attack-range f!
      s" O_LRO/LRO_000" 2dup file!  actor-load dup my-model !  $4246C0 motion-table
      0 him act.visible l!
      hard-mode? if  75  else  50  then  dup hp-max !  hp !  11 my-kind !  0 growls !  12 hear-threshold !  family-setup
      3600 100  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Lorenzo_Setup)
      0e throw-k f!  100 tolerance !  0 temper !
      0 dog-limit !  leaves-doors door-manner !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      5e 20e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: lorenzo-spawn ( -- id )  lorenzo stalker-state s" lorenzo" spawn ;

\ ---- Lorenzo's other class (lorenzo.c Lorenzo2_Setup; kind 10): 250 health (hard: 400), 4 across,
\ 18 tall; his model O_LRY/LRY_000 (motion table $424490); the Pursuer's chase with his kit (his
\ moves by the hard setting), his own actions under the floor (stalker/lorenzo2.fs) ----
behaviour lorenzo2
  extends stalking
  on spawned ( -- )
      -1 her-came !
      hard-mode? if  lorenzo2h-kit  else  lorenzo2-kit  then  kit !  -1 base-chase !  -1 gives-up !  0.06981317e turn-k f!
      lorenzo2-acts own-acts !  ['] lorenzo2-frame own-frame !  70e attack-range f!  60e reach-k f!
      s" O_LRY/LRY_000" 2dup file!  actor-load dup my-model !  $424490 motion-table
      0 him act.visible l!
      hard-mode? if  400  else  250  then  dup hp-max !  hp !  10 my-kind !  0 growls !  12 hear-threshold !  family-setup
      9000 150  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Lorenzo2_Setup)
      hard-mode? if  25e  else  50e  then  throw-k f!  100 tolerance !  0 temper !
      600 dog-limit !  leaves-doors door-manner !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      4e 18e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: lorenzo2-spawn ( -- id )  lorenzo2 stalker-state s" lorenzo2" spawn ;

\ ---- kind 39 (lorenzo.c Kind39_Setup): Lorenzo's other class again - its own tables, 250 health
\ (hard: 300), no "rage" (its reach 18, for Hewie 12), its sweep at whoever it sees ----
behaviour kind39
  extends stalking
  on spawned ( -- )
      -1 her-came !
      hard-mode? if  kind39h-kit  else  kind39-kit  then  kit !  -1 base-chase !  -1 gives-up !  0.06981317e turn-k f!
      lorenzo2-acts own-acts !  18e attack-range f!  12e reach-k f!
      s" O_LRY/LRY_000" 2dup file!  actor-load dup my-model !  $424490 motion-table
      0 him act.visible l!
      hard-mode? if  300  else  250  then  dup hp-max !  hp !  39 my-kind !  0 growls !  12 hear-threshold !  family-setup
      9000 150  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Kind39_Setup)
      hard-mode? if  65e 75  else  30e 40  then  tolerance !  throw-k f!  0 temper !
      hard-mode? if  360  else  300  then  dog-limit !  leaves-doors door-manner !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      4e 18e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: kind39-spawn ( -- id )  kind39 stalker-state s" kind39" spawn ;

\ ---- Riccardo's other class (riccardo.c Kind37_Setup; kind 37): his chase with its own tables,
\ 100 health (hard: 125), 3 across, 17 tall, its model O_RCT/RCT_000 (motion table $42AED0);
\ near him at 30; struck enough, alerted (stalker/riccardo.fs `kind37-frame`) ----
behaviour kind37
  extends stalking
  on spawned ( -- )
      -1 her-came !
      kind37-kit kit !  0 base-chase !  -1 gives-up !  0.06981317e turn-k f!  12e reach-k f!
      ['] ric-begin own-chase !  ['] ric-run own-run !  20e attack-range f!  ['] kind37-frame own-frame !
      s" O_RCT/RCT_000" 2dup file!  actor-load dup my-model !  $42AED0 motion-table
      0 him act.visible l!
      hard-mode? if  125  else  100  then  dup hp-max !  hp !  37 my-kind !  0 growls !  12 hear-threshold !  family-setup
      5400 45  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Kind37_Setup)
      10e throw-k f!  50 tolerance !  0 temper !
      900 dog-limit !  leaves-doors door-manner !  riccardo-ladder ladder-offsets !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: kind37-spawn ( -- id )  kind37 stalker-state s" kind37" spawn ;

\ ---- the TintStalker (tintstalker.c TintStalker_Setup; kind 23): the Pursuer's chase with its
\ tables, Riccardo's look (O_RCT/RCT_000, motion table $42AED0), 100 health (hard: 125); seen
\ only a while after each turn (stalker/riccardo.fs `tint-frame`) ----
behaviour tintstalker
  extends stalking
  on spawned ( -- )
      -1 her-came !
      tintstalker-kit kit !  -1 base-chase !  -1 gives-up !  0.06981317e turn-k f!  12e reach-k f!
      ['] tint-frame own-frame !  -1 tint-was !  -1 unseen !
      s" O_RCT/RCT_000" 2dup file!  actor-load dup my-model !  $42AED0 motion-table
      0 him act.visible l!
      hard-mode? if  125  else  100  then  dup hp-max !  hp !  23 my-kind !  0 growls !  12 hear-threshold !  family-setup
      5400 45  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (TintStalker_Setup)
      hard-mode? if  10e  else  15e  then  throw-k f!  50 tolerance !  0 temper !
      0 dog-limit !  shuts-doors door-manner !  riccardo-ladder ladder-offsets !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      3e 17e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: tintstalker-spawn ( -- id )  tintstalker stalker-state s" tintstalker" spawn ;

\ ---- kind 12 (lorenzo.c Kind12_Setup): the Pursuer's chase with its tables, 9999 health and never
\ hurt, 3.5 across, 18 tall, its model O_LRF/LRF_000 (motion table $443FB0), reach 18, for Hewie
\ 14; in her room it seizes her (stalker/riccardo.fs `kind12-frame`) ----
behaviour kind12
  extends stalking
  on spawned ( -- )
      -1 her-came !
      kind12-kit kit !  -1 base-chase !  0 gives-up !  0.06981317e turn-k f!  14e reach-k f!  18e attack-range f!
      ['] kind12-frame own-frame !  -1 mark-id !
      s" O_LRF/LRF_000" 2dup file!  actor-load dup my-model !  $443FB0 motion-table
      0 him act.visible l!
      9999 dup hp-max !  hp !  12 my-kind !  0 growls !  12 hear-threshold !  family-setup
      3000 999  cool-secs !  away-base !  -1 own-a !  -1 own-b !   \ (Kind12_Setup)
      0e throw-k f!  100 tolerance !  0 temper !
      0 dog-limit !  leaves-doors door-manner !   \ (after Hewie: his time for it; the door he comes into her room by; out from a ladder)
      3.5e 18e body-size  pursuer-floor dup my-floor ! body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room  self subscribe write-save  self subscribe fiona-plight  self subscribe panic  self subscribe frame-end ;
end-behaviour
: kind12-spawn ( -- id )  kind12 stalker-state s" kind12" spawn ;
