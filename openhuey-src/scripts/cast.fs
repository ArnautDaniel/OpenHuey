\ cast.fs - the event characters (docs/subsystems/cast.md; src/game/char_load.c CharLoad_EventChar,
\ story_chars.c): the story's figures in character slots 3..5 - the crows, Fiona's doubles, the
\ figures of its scenes. A figure an actor (`cast3`..`cast5`): loaded as character `id` (its kind's
\ model: the kinds' table below; cast as `id` for the cutscenes), placed by the story (`place`,
\ at an exit `to-exit`), shown or hidden, its moves the story's (`scripted-move`: an animation,
\ walking to a spot, turning; `hold-anim`), reporting each frame's end (`moving`). Given another's
\ model (`cast-dress`: the scripts' hand-over, Pursuer_GiveMotionBanks) it wears that.
IN: cast
USING: engine game-state actors messages common vectors facts floor-flags ;

\ ---- the kinds' figures (their ModelFiles: the first file, its name less the extension) ----
: kind-file ( kind -- addr len | 0 0 )
    case
        8 of  s" O_GLM/GLM_000"  endof   9 of  s" O_LRM/LRM_000"  endof  13 of  s" O_FIN/FIN_200"  endof
        14 of  s" O_CRW/CRW_000"  endof  15 of  s" O_CRW/CRW_000"  endof  16 of  s" O_WHC/WHC_000"  endof
        17 of  s" O_SGM/SGM_000"  endof  18 of  s" O_FIW/FIW_200"  endof  19 of  s" O_FS0/FS0_000"  endof
        20 of  s" O_FS1/FS1_000"  endof  21 of  s" O_FS2/FS2_000"  endof  22 of  s" O_FS2/FS2_000"  endof
        24 of  s" O_SHT/SHT_000"  endof  25 of  s" O_WIR/WIR_000"  endof  26 of  s" O_RBT/RBT_000"  endof
        28 of  s" O_HMA/HMA_000"  endof  29 of  s" O_DNT/DNT_000"  endof  30 of  s" O_LRC/LRC_000"  endof
        31 of  s" O_LRH/LRH_000"  endof  32 of  s" O_FIM/FIM_200"  endof  33 of  s" O_HMB/HMB_000"  endof
        38 of  s" O_HND/HND_000"  endof
        >r 0 0 r>
    endcase ;
9 constant hanging-lorenzo   13 constant fiona-double   14 constant crow   15 constant crow-2
18 constant fiona-double-2   26 constant rabbit   33 constant kind-33
\ the hard setting's textures (Kind09_ModelFiles: Lorenzo's hanging figure, LRM_000H)
: kind-textures ( kind -- addr len | 0 0 )
    hanging-lorenzo = hard-mode? and if  s" O_LRM/LRM_000H"  else  0 0  then ;
\ the kinds' motion tables - their fades (each model class's SecondaryMotion; the plain model's
\ kinds have none): Fiona's doubles hers, the crows', Lorenzo's hanging figure's, kind 33's
\ (the executable's tables by name: exe-table, motion-table)
: fiona-motions ( -- addr len )  s" motions-fiona" ;     : crow-motions ( -- addr len )  s" motions-crow" ;
: hanging-lorenzo-motions ( -- addr len )  s" motions-hanging-lorenzo" ;
: kind33-motions ( -- addr len )  s" motions-homunculus" ;
: kind-motions ( kind -- addr len | 0 0 )
    case
        fiona-double of  fiona-motions  endof   fiona-double-2 of  fiona-motions  endof
        crow of  crow-motions  endof           crow-2 of  crow-motions  endof
        hanging-lorenzo of  hanging-lorenzo-motions  endof   kind-33 of  kind33-motions  endof
        >r 0 0 r>
    endcase ;
: crow? ( kind -- flag )  dup crow = swap crow-2 = or ;

<PRIVATE
state: cast-state
  cell field who               \ the character's script id (-1: none)
  cell field model             \ its model (-1: none)
  cell field shown             \ the story's show / hide
  cell field in-scene          \ a cutscene has it (its director moves the model)
  cell field anim
  cell field act               \ what it does each frame (an xt)
  cell field mv-done  cell field mv-kind  cell field mv-a  cell field mv-b
  /vec field mv-at  1 floats field mv-yaw
  \ an animal's ways (the rabbits: Kind26_Behaviour) - what it is doing and whether that just
  \ began, its count, its footstep sound, its feet down, its home and where it runs off to, whom
  \ it watches, the one nearest
  cell field an-doing  cell field an-new  cell field an-t  cell field an-sound
  cell field an-left  cell field an-right
  1 floats field an-home-x  1 floats field an-home-z  1 floats field an-run-x  1 floats field an-run-z
  /vec field an-watch  /vec field an-near
  cell field st-actor  cell field st-room-is   \ (the stalker, as he tells: stalker-doing)
  cell field held               \ a script of the room has it (the story's scripted: +0xE0)
  cell field noclipped          \ through blocked floor (the story's 0x20: +0x2B)
  \ its head (Pursuer_Think): a scripted look on (+0x16F9), at whom the moves last said (+0x100:
  \ 0 Fiona, 1 Hewie, else the point), the point (+0x1700); the head's turn (its motion's +0x858)
  cell field look-on  cell field look-who  /vec field look-pt  1 floats field head-yaw
end-state
: him ( -- a )  model @ actor ;
: placed ( -- )  model @ dup 0< if  drop  else  actor-settle  then ;   \ (Pursuer_Place: its feet afresh, not eased from where it was)
: shown! ( on -- )  0<> dup shown !  broadcast figure-shown ;   \ (told: the obstacles leave a hidden one out)
: my-at ( -- v )  self body-at ;
: yaw ( F: -- a )  self body-yaw ;
: play-now ( anim -- )   \ (Motion_PlayTable; one it hasn't: as it is)
    model @ over has-motion? 0= if  drop exit  then
    dup anim !  model @ swap  2dup motion-entry nip  motion-play ;
: play-blend ( anim blend more-flags -- )   \ (Motion_PlayBlend / _PlayBlend8: its table's flags, and these, over
    {: a bl more :}                           \ blend frames)
    model @ a has-motion? 0= if  exit  then
    a anim !  model @ a bl  model @ a motion-entry nip nip more or  motion-play ;
: ended? ( -- flag )  him act.mflags l@ $20 and 0<> ;
: faded-in? ( -- flag )  him act.fade sf@ 0e f<= ;
: play ( anim -- )   \ (Pursuer_PlayAnim: not again while it plays)
    dup anim @ = ended? 0= and if  drop exit  then  play-now ;
: done ( -- )  -1 mv-done ! ;
: still ( -- ) ;
: settle ( -- )  done  ['] still act ! ;
\ its root motion this frame: the turn, and the step across and ahead
: root@ ( F: -- turn x z )  model @ root-delta  fswap fdrop ;   \ (no rise)
: root-move ( -- )  root@ {: F: turn x z :}  yaw turn f+ angle-wrap body-turn  x z body-move-local ;
\ ---- the story's moves (Pursuer_EventCommand) ----
$200 constant walk-anim
$201 constant other-walk-anim   \ (Pursuer_SlowWalkAnim: $201 with its +0x16C8 set - Pursuer_Reset leaves it 2,
                                \ and nothing of these kinds clears it)
0 constant stand-anim           \ (Pursuer_WalkAnim's, these kinds standing)
: turned? ( -- flag )  mv-yaw f@ 4e deg>rad body-turn-toward f0= ;   \ (Npc_TurnToward at Pursuer_TurnRate: 4 degrees a frame)
: arrive-turning ( -- )   \ (Pursuer_StateArrive) on its spot, turning to the move's heading; then standing
    turned? if  stand-anim play-now  settle  then ;
: turning ( -- )   \ (Pursuer_StateTurnThenWait) to the heading in its walk; then standing, done once that has faded in
    turned? 0= if  exit  then
    model @ stand-anim has-motion? 0= if  settle exit  then
    anim @ 4 u< if  faded-in? if  settle  then  else  stand-anim play  then ;
: to-a-triangle? ( -- flag )  mv-kind @ dup run-move = swap run-path-move = or ;   \ (6 / 11: Pursuer_StateRunToTri)
: walking ( -- )   \ toward the spot, by its walk's root motion (a slow step if it has none)
    root@ fnip fnip  fdup fabs 0.01e f< if  fdrop 0.6e  then  {: F: step :}
    to-a-triangle? self body-tri mv-a @ = and if  done  then   \ (to a triangle: done once on it)
    self mv-at body-heading-to 10e deg>rad body-turn-toward fdrop
    my-at mv-at vec-dist-xz step f< if
        mv-at vec@ body-place-at
        mv-kind @ walk-anim-move = if  mv-yaw f@ body-turn  stand-anim play-now  settle exit  then
        to-a-triangle? if  stand-anim play-now  settle exit  then
        ['] arrive-turning act !  exit
    then
    0e step body-move-local ;
\ ---- its head (Pursuer_Think, Pursuer_LookAt; Motion_LookAt, Motion_EaseTilt, Model_EaseTilt):
\ a scripted look on, turned toward whom the moves last named - Fiona, Hewie, else the look's
\ point (the original's +0x100: the looks, the turns to a character, the doors' and the walk
\ with an animation's argument) - small turns none; else eased back. 8 degrees a frame
\ (Pursuer_TurnRateFast). The tilt stays level (the look eases it to 0 by nothing, the easing
\ back to 0). The head bones take it as they are posed (HumanModel_AdjustBone: head-turn) ----
: rate ( F: -- r )  8e deg>rad ;
: ease-yaw ( F: target -- )
    {: F: goal :}  head-yaw f@ goal f- fabs rate f<= if  goal head-yaw f! exit  then
    head-yaw f@ goal f> if  head-yaw f@ rate f-  else  head-yaw f@ rate f+  then  head-yaw f! ;
-2 constant at-point
: look-char ( -- id | at-point )
    look-who @ case  0 of  s" fiona" actor-named  endof   1 of  s" hewie" actor-named  endof
                     >r at-point r>  endcase ;
: look-target ( -- v | 0 )   \ (one gone: none)
    look-char dup at-point = if  drop look-pt exit  then
    dup 0< if  drop 0 exit  then  dup body? 0= if  drop 0 exit  then  body-at ;
: look ( -- )
    look-on @ if
        look-target ?dup if
            self swap body-heading-to yaw f- angle-wrap  fdup fabs 0.05e f< if  fdrop 0e  then  ease-yaw
        then
    else  0e ease-yaw  then
    0e head-yaw f@ model @ head-turn ;
: who-was ( n -- )  look-who ! ;   \ (a move's argument as the original's +0x100)
: walk-anim-for ( -- anim )   \ (Pursuer_ScriptMove: 5 / 6 its b if one of the $2xx walks, else $200; 10 / 11 $201)
    mv-kind @ dup walk-move = swap run-move = or if
        mv-b @ $FF00 and walk-anim = if  mv-b @  else  walk-anim  then
    else  other-walk-anim  then ;
\ ---- an animal (Kind26_Behaviour, animal_step): grazing (turned to whom it watches, 1..2 s),
\ looking up, turning home and back to grazing; someone within 30 - alert (turning away; the
\ original takes the point's y for its z, kept); within 20 - off it runs to its spot, there its
\ sound and done ----
enum: animal-doing  grazing looking-up alert running-off turning-home back-to-grazing ;
$9000 constant graze-anim   $9001 constant look-up-anim   $9002 constant turn-anim   $9003 constant run-anim
1e8 fconstant nobody-near
: near-sq ( id -- ) ( F: -- d2 )   \ (one in its room: the squared distance, its place in an-near)
    dup 0< if  drop nobody-near exit  then  dup body? 0= if  drop nobody-near exit  then
    dup body-room self body-room <> if  drop nobody-near exit  then
    body-at  dup my-at vec-dist fdup f*  an-near swap vec-copy ;
create who-at /vec allot
: nearer ( F: best d2 -- best' )   \ (the nearer kept, and its place in who-at)
    fover fover f> if  fnip  who-at an-near vec-copy  else  fdrop  then ;
: nearest-sq ( -- ) ( F: -- d2 )   \ (Kind26_NearestDistSq: Fiona, Hewie, the stalker; its place in who-at)
    s" fiona" actor-named near-sq  who-at an-near vec-copy
    s" hewie" actor-named near-sq nearer
    st-actor @ near-sq nearer ;
: an-go ( doing -- )  an-doing !  1 an-new ! ;
: an-face ( F: x z -- )   \ (turned 0.3 of the way to it)
    {: F: x z :}  x my-at v.x sf@ f-  z my-at v.z sf@ f-  fatan2 angle-wrap   \ (its heading: atan2 of across over ahead)
    yaw f- angle-wrap 0.3e f*  yaw f+ angle-wrap body-turn ;
: an-anim ( anim -- )  an-new @ if  play-now  0 an-new !  else  drop  then ;
: graze ( F: d2 -- )
    an-new @ if  graze-anim play-now  rnd 1e f+ 60e f* f>s an-t !  0 an-new !  then
    an-watch v.x sf@ an-watch v.z sf@ an-face
    -1 an-t +!  an-t @ 0< if
        2500e f<= rnd 0.8e f< 0= or if  looking-up an-doing !  else  turning-home an-doing !  then  1 an-new !  0 an-t !
    else  fdrop  then ;
: be-alert ( -- )   \ (turned away from it: the y for z, the original's)
    an-new @ if  turn-anim play-now  0 an-new !  an-watch who-at vec-copy  then
    my-at v.x sf@ an-watch v.x sf@ f- 100e f* my-at v.x sf@ f+
    my-at v.y sf@ an-watch v.y sf@ f- 100e f* my-at v.y sf@ f+  an-face
    ended? if  grazing an-go  then ;
: run-off ( -- ran-off? )
    run-anim an-anim  an-run-x f@ an-run-z f@ an-face
    an-run-z f@ my-at v.z sf@ f- fdup f*  an-run-x f@ my-at v.x sf@ f- fdup f* f+ 1e f< if
        an-sound @ 1+ 6 0 0 my-at vec@ sound-at  true exit
    then  false ;
: animal-ways ( -- ran-off? )
    nearest-sq  {: F: d2 :}
    an-doing @ running-off <>  d2 400e f< and if  running-off an-go  then
    an-doing @ dup running-off <> swap alert <> and  d2 900e f< and if  alert an-go  then
    an-doing @ case
        grazing of  d2 graze  endof
        looking-up of  look-up-anim an-anim  ended? rnd 0.5e f< and if  grazing an-go  then  endof
        turning-home of  an-new @ if  5 an-t !  0 an-new !  then
              an-home-x f@ an-home-z f@ an-face  -1 an-t +!  an-t @ 0< if  back-to-grazing an-go  then  endof
        back-to-grazing of  turn-anim an-anim  ended? if  grazing an-go  then  endof
        alert of  be-alert  endof
        running-off of  run-off exit  endof
    endcase  false ;
: free-move ( -- )   \ (its root motion added as it is: no floor holds an animal)
    root@ {: F: turn x z :}  yaw turn f+ angle-wrap body-turn
    x z yaw rotate-by  {: F: dx dz :}                                 \ (the step, turned)
    my-at v.x sf@ dx f+  my-at v.y sf@  my-at v.z sf@ dz f+  body-place-at ;
: animal ( -- )   \ (animal_step: its ways, moved by its animation, a footstep as a foot comes down)
    animal-ways  free-move
    model @ 1 foot-down?  model @ 0 foot-down?  {: left right :}
    left an-left @ 0= and  right an-right @ 0= and or  an-sound @ -1 <> and if
        an-sound @ 6 0 0 my-at vec@ sound-at
    then  right an-right !  left an-left !
    if  settle  then ;
: animal-go ( sound F: x z -- )   \ (Kind26_MoveToB: grazing here, off to (x, z) when one comes near)
    an-run-z f!  an-run-x f!  an-sound !  grazing an-go  60 an-t !
    my-at v.x sf@ an-home-x f!  my-at v.z sf@ an-home-z f!  1 an-left !  1 an-right !
    0 mv-done !  ['] animal act ! ;
: start-move ( kind -- )
    0 mv-done !  dup mv-kind !
    case
        anim-move of  mv-a @ play-now  done  ['] root-move act !  endof
        blend-move of  mv-a @ mv-b @ 0 play-blend  done  ['] root-move act !  endof
        blend8-move of  mv-a @ mv-b @ 8 play-blend  done  ['] root-move act !  endof
        idle-anim-move of  stand-anim mv-b @ 0 play-blend  done  ['] root-move act !  endof   \ (its stand, over b frames)
        turn-to-who-move of  mv-a @ who-was  walk-anim play  ['] turning act !  endof
        turn-to-heading-move of  walk-anim play  ['] turning act !  endof
        walk-move of  walk-anim-for play  ['] walking act !  endof
        run-move of  walk-anim-for play  ['] walking act !  endof
        walk-path-move of  walk-anim-for play  ['] walking act !  endof
        run-path-move of  walk-anim-for play  ['] walking act !  endof
        walk-anim-move of  mv-a @ who-was  mv-a @ play-now  ['] walking act !  endof
        stand-move of  0 play-now  settle  endof
        look-move of  mv-b @ dup who-was  $FF = if  0 look-on !  else  mv-a @ 0< 0= if  1 look-on !  then  then  done  endof
        look-point-move of  look-pt mv-at vec-copy  1 look-on !  done  endof
        open-door-move of  mv-a @ who-was  done  endof   shut-door-move of  mv-a @ who-was  done  endof
        >r done r>
    endcase ;

\ ---- loaded, dressed, gone ----
: unload ( -- )
    who @ 0< 0= if  who @ -1 cast-as  then
    model @ 0< 0= if  model @ actor-free  then
    -1 model !  -1 who !  0 in-scene !  ['] still act !  body-off ;
: model-load ( file len id -- model )   \ (its kind's textures when it has its own)
    kind-textures dup if  actor-load-textured  else  2drop actor-load  then ;
\ its legs (each human model class's Loaded: +0x890..+0x8AC) by its kind - Lorenzo's hanging figure
\ (Kind09Model) his left from $12, Fiona's doubles (EventHumanModel, Kind18Model) 2..5 and 6..9; the
\ others' models are plain (no feet planted); their heads (+0x8B0 / +0x8B4) the same way
: kind-legs ( kind -- right left )
    case  hanging-lorenzo of  2 $12  endof  fiona-double of  2 6  endof  fiona-double-2 of  2 6  endof
          >r -1 -1 r>  endcase ;
: kind-head ( kind -- head neck )
    case  hanging-lorenzo of  $2B $22  endof  fiona-double of  $21 $17  endof  fiona-double-2 of  $21 $17  endof
          >r -1 -1 r>  endcase ;
: load ( file len id -- )
    {: id :}  unload  id model-load model !  id who !  id model @ cast-as
    id kind-motions dup if  model @ -rot motion-table  else  2drop  then
    model @ id kind-legs human-legs  model @ id kind-head head-bones
    0 look-on !  -1 look-who !  0e 0e 0e look-pt vec!  0e head-yaw f!   \ (Pursuer_Reset; +0x100 -1 as made)
    0 him act.visible l!  true shown!  -1 mv-done !  ['] still act !  0 play-now ;
: dress ( model -- )   \ (another's model worn: its files loaded for this one)
    who @ 0< if  drop exit  then
    model-file  who @ load ;
\ a crow's parts by its animation (Kind14Model_Draw): in $9000 / $9001 part 5, else parts 3 and 4
\ (the original's draw form flags with them, not kept)
: crow-9000s? ( -- flag )  anim @ $9000 - 2 u< ;
: crow-parts ( -- )
    who @ crow? 0= if  exit  then
    crow-9000s? {: on :}
    model @ 5 on actor-part-show  model @ 3 on 0= actor-part-show  model @ 4 on 0= actor-part-show ;
\ its model where its body is, seen in the room being played
: pose ( -- )
    model @ 0< if  exit  then
    crow-parts
    in-scene @ if  exit  then   \ (a cutscene's: shown by its shots, body or none - the golem has none)
    self body? 0= if  0 him act.visible l!  exit  then
    my-at v.x sf@ him act.x sf!  my-at v.y sf@ him act.y sf!  my-at v.z sf@ him act.z sf!
    yaw him act.yaw sf!
    self body-room room-id =  shown @ and  0<> 1 and  him act.visible l! ;
\ its feet on the floor (Pursuer_ModelUpdate: the model's +0x40 by its stance's points - its kinds
\ set none, 0 0: at its own height - and +0x4C): in the room played, on its triangle, planted but
\ while a script walks it through blocked floor; a scene's (its director's motion) not
: floor-mask ( -- mask )  noclipped @ if  0  else  pursuer-floor  then ;
: fit-legs ( -- )
    model @ 0< in-scene @ or if  exit  then
    self body? 0= if  exit  then
    self body-room room-id <> if  exit  then
    held @ noclipped @ and 0= 1 and {: on :}
    0e 0e  model @ self body-tri floor-mask on legs-fit ;
PRIVATE>

behaviour figure
  on spawned ( -- )
      -1 who !  -1 model !  ['] still act !  -1 mv-done !
      2e 15e body-size  pursuer-floor body-mask!  body-off
      -1 st-actor !  self subscribe tick  self subscribe frame-end  self subscribe scene  self subscribe stalker-doing ;
  on cast-load ( id -- )
      dup kind-file dup 0= if  2drop drop exit  then  rot load ;
  on cast-unload ( -- )  unload ;
  on cast-dress ( model -- )  dress ;
  on cast-out ( -- )  body-off ;
  on tick ( -- )   \ (a scene having it - Character_Disable - its frame waits: the scene moves its model)
      model @ 0< 0= self body? and  model @ scene-has? 0= and if  act @ execute  look  then  pose  fit-legs ;
  on frame-end ( -- )
      model @ 0< 0= self body? and if  mv-done @  ended?  model @ 0 0 1 motion-events  broadcast moving  then ;
  on scene ( on -- )  in-scene ! ;
  \ the story's
  on place ( x y z yaw -- )
      cell>f mv-yaw f!  cells>xyz mv-at vec!
      room-id  mv-at pursuer-floor v-tri-in  mv-at vec@ body-place  mv-yaw f@ body-turn  placed ;
  on to-exit ( exit -- )
      0 exit-spot dup 0< if  drop fdrop fdrop fdrop exit  then      ( tri ) ( F: x y z )
      room-id swap body-place  placed ;
  on show ( on -- )  shown! ;
  on scripted ( on -- )  0<> held ! ;
  on activate ( -- )   \ (the story's 0x15: Character_Activate - a script's hold and its way through blocked floor gone)
      0 held !  0 noclipped !  pursuer-floor body-mask! ;
  on scripted-move ( kind a b x y z yaw -- )
      cell>f mv-yaw f!  cells>xyz mv-at vec!
      mv-b !  mv-a !  start-move ;
  on hold-anim ( anim flags blend -- )   \ (as asked: a take-off at once, not over its motion table's fade)
      {: a fl bl :}  model @ a has-motion? if  a anim !  model @ a bl fl motion-play  then
      done  ['] root-move act ! ;
  on animal-go ( sound x z -- )  {: sound x z :}  sound x cell>f z cell>f animal-go ;   \ (the rooms' commands: the rabbits)
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      2drop 2drop 2drop 2drop  st-room-is !  sender st-actor ! ;   \ (who he is, and his room)
  on noclip ( on -- )  0<> dup noclipped !  if  0  else  pursuer-floor  then  body-mask! ;   \ (the story's 0x20: through blocked floor)
end-behaviour

: cast-spawn ( slot -- id )
    {: slot :}  figure cast-state  slot case  3 of  s" cast3"  endof  4 of  s" cast4"  endof  >r s" cast5" r>  endcase  spawn ;
