\ cast.fs - the event characters (docs/subsystems/cast.md; src/game/char_load.c CharLoad_EventChar,
\ story_chars.c): the story's figures in character slots 3..5 - the crows, Fiona's doubles, the
\ figures of its scenes. A figure an actor (`cast3`..`cast5`): loaded as character `id` (its kind's
\ model: the kinds' table below; cast as `id` for the cutscenes), placed by the story (`place`,
\ at an exit `to-exit`), shown or hidden, its moves the story's (`scripted-move`: an animation,
\ walking to a spot, turning; `hold-anim`), reporting each frame's end (`moving`). Given another's
\ model (`cast-dress`: the scripts' hand-over, Pursuer_GiveMotionBanks) it wears that.
IN: cast
USING: engine game-state actors messages common facts ;

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
$2C020028 constant cast-floor   \ (the Pursuer's: NPC_BlockFlags)
\ the hard setting's textures (Kind09_ModelFiles: Lorenzo's hanging figure, LRM_000H)
: kind-textures ( kind -- addr len | 0 0 )
    9 = hard-mode? and if  s" O_LRM/LRM_000H"  else  0 0  then ;
\ the kinds' motion tables - their fades (each model class's SecondaryMotion; the plain model's
\ kinds have none): Fiona's doubles hers, the crows', Lorenzo's hanging figure's, kind 33's
$3D5CC0 constant fiona-motions   $429C10 constant crow-motions
$424250 constant hanging-lorenzo-motions   $42C730 constant kind33-motions
: kind-motions ( kind -- vaddr | 0 )
    case
        13 of  fiona-motions  endof   18 of  fiona-motions  endof
        14 of  crow-motions  endof    15 of  crow-motions  endof
        9 of  hanging-lorenzo-motions  endof   33 of  kind33-motions  endof
        >r 0 r>
    endcase ;
: crow? ( kind -- flag )  14 - 2 u< ;

<PRIVATE
state: cast-state
  cell field who               \ the character's script id (-1: none)
  cell field model             \ its model (-1: none)
  cell field shown             \ the story's show / hide
  cell field in-scene          \ a cutscene has it (its director moves the model)
  cell field anim
  cell field act               \ what it does each frame (an xt)
  cell field mv-done  cell field mv-kind  cell field mv-a  cell field mv-b
  12 field mv-at  1 floats field mv-yaw
  \ an animal's ways (the rabbits, kind 26: Kind26_Behaviour) - what it is doing (+0x1624) and
  \ whether that just began (+0x1628), its count (+0x162C), its home (+0x1634) and where it runs
  \ off to (+0x163C), its footstep sound (+0x1630), its feet down (+0x16A8), whom it watches
  cell field an-doing  cell field an-new  cell field an-t  cell field an-sound
  cell field an-left  cell field an-right
  1 floats field an-home-x  1 floats field an-home-z  1 floats field an-run-x  1 floats field an-run-z
  12 field an-watch  12 field an-near
  cell field st-actor  cell field st-room-is   \ (the stalker, as he tells: stalker-doing)
end-state
: him ( -- a )  model @ actor ;
: my-at ( -- v )  self body-at ;
: yaw ( F: -- a )  self body-yaw ;
: play-now ( anim -- )   \ (Motion_PlayTable; one it hasn't: as it is)
    model @ over has-motion? 0= if  drop exit  then
    dup anim !  model @ swap  2dup motion-entry nip  motion-play ;
: ended? ( -- flag )  him act.mflags l@ $20 and 0<> ;
: done ( -- )  -1 mv-done ! ;
\ its root motion this frame, turned and moved by it
fvariable rm-turn  fvariable rm-x  fvariable rm-z
: root@ ( -- )  model @ root-delta  rm-z f!  fdrop rm-x f!  rm-turn f! ;
: root-move ( -- )  root@  yaw rm-turn f@ f+ angle-wrap body-turn  rm-x f@ rm-z f@ body-move-local ;
: still ( -- ) ;
\ ---- the story's moves ----
: turning ( -- )   \ to its heading, 10 degrees a frame
    mv-yaw f@ 10e deg>rad body-turn-toward f0= if  done  ['] still act !  then ;
: walking ( -- )   \ toward the spot, by its walk's root motion (a slow step if it has none)
    root@  rm-z f@ fabs 0.01e f< if  0.6e  else  rm-z f@  then          ( F: step )
    self mv-at body-heading-to 10e deg>rad body-turn-toward fdrop
    my-at mv-at vec-dist-xz fover f< if
        fdrop  mv-at vec@ body-place-at  mv-yaw f@ body-turn  0 play-now  done  ['] still act ! exit
    then
    0e fswap body-move-local ;
\ ---- an animal (Kind26_Behaviour, animal_step): grazing (0, turned to whom it watches, 1..2 s),
\ looking up (1), turning home (4) and back to grazing (5); someone within 30 - alert (2, turning
\ away; the original takes the point's y for its z, kept); within 20 - off it runs (3) to its
\ spot, there its sound and done ----
: near-sq ( id -- ) ( F: -- d2 )   \ (one in its room: the squared distance, its place in an-near)
    dup 0< if  drop 1e8 exit  then  dup body? 0= if  drop 1e8 exit  then
    dup body-room self body-room <> if  drop 1e8 exit  then
    body-at  dup my-at vec-dist fdup f*  an-near swap vec-copy ;
create who-at 12 allot
: nearest-sq ( -- ) ( F: -- d2 )   \ (Kind26_NearestDistSq: Fiona, Hewie, the stalker; its place in who-at)
    s" fiona" actor-named near-sq  who-at an-near vec-copy
    s" hewie" actor-named near-sq  fover fover f> if  fswap fdrop  who-at an-near vec-copy  else  fdrop  then
    st-actor @ near-sq  fover fover f> if  fswap fdrop  who-at an-near vec-copy  else  fdrop  then ;
: an-go ( doing -- )  an-doing !  1 an-new ! ;
: an-face ( F: x z -- )   \ (turned 0.3 of the way to it)
    my-at 8 + sf@ f-  fswap my-at sf@ f-  fswap fatan2 angle-wrap        ( F: a )
    yaw f- angle-wrap 0.3e f*  yaw f+ angle-wrap body-turn ;
: an-anim ( anim -- )  an-new @ if  play-now  0 an-new !  else  drop  then ;
: animal-ways ( -- ran-off? )
    nearest-sq                                                        ( F: d2 )
    an-doing @ 3 <> fdup 400e f< and if  3 an-go  then
    an-doing @ dup 3 <> swap 2 <> and fdup 900e f< and if  2 an-go  then
    an-doing @ case
        0 of  an-new @ if  $9000 play-now  rnd 1e f+ 60e f* f>s an-t !  0 an-new !  then
              an-watch sf@ an-watch 8 + sf@ an-face
              -1 an-t +!  an-t @ 0< if
                  fdup 2500e f<= rnd 0.8e f< 0= or if  1 an-doing !  else  4 an-doing !  then  1 an-new !  0 an-t !
              then  endof
        1 of  $9001 an-anim  ended? rnd 0.5e f< and if  0 an-go  then  endof
        4 of  an-new @ if  5 an-t !  0 an-new !  then
              an-home-x f@ an-home-z f@ an-face  -1 an-t +!  an-t @ 0< if  5 an-go  then  endof
        5 of  $9002 an-anim  ended? if  0 an-go  then  endof
        2 of  an-new @ if  $9002 play-now  0 an-new !  an-watch who-at vec-copy  then
              my-at sf@ an-watch sf@ f- 100e f* my-at sf@ f+
              my-at 4 + sf@ an-watch 4 + sf@ f- 100e f* my-at 4 + sf@ f+  an-face   \ (y for z: the original's)
              ended? if  0 an-go  then  endof
        3 of  $9003 an-anim  an-run-x f@ an-run-z f@ an-face
              an-run-z f@ my-at 8 + sf@ f- fdup f*  an-run-x f@ my-at sf@ f- fdup f* f+ 1e f< if
                  fdrop  an-sound @ 1+ 6 0 0 my-at vec@ sound-at  true exit
              then  endof
    endcase  fdrop false ;
: free-move ( -- )   \ (its root motion added as it is: no floor holds an animal)
    root@  yaw rm-turn f@ f+ angle-wrap body-turn
    rm-x f@ rm-z f@ yaw rotate-by  rm-z f!  rm-x f!                  \ (the step, turned)
    my-at sf@ rm-x f@ f+  my-at 4 + sf@  my-at 8 + sf@ rm-z f@ f+  body-place-at ;
: animal ( -- )   \ (animal_step: its ways, moved by its animation, a footstep as a foot comes down)
    animal-ways  free-move
    model @ 1 foot-down?  model @ 0 foot-down?                       ( l r )
    over an-left @ 0= and  over an-right @ 0= and or  an-sound @ -1 <> and if
        an-sound @ 6 0 0 my-at vec@ sound-at
    then  an-right !  an-left !
    if  done  ['] still act !  then ;
: animal-go ( sound F: x z -- )   \ (Kind26_MoveToB: grazing here, off to (x, z) when one comes near)
    an-run-z f!  an-run-x f!  an-sound !  0 an-go  60 an-t !
    my-at sf@ an-home-x f!  my-at 8 + sf@ an-home-z f!  1 an-left !  1 an-right !
    0 mv-done !  ['] animal act ! ;
: start-move ( kind -- )
    0 mv-done !  dup mv-kind !
    case
        7 of  mv-a @ play-now  done  ['] root-move act !  endof
        8 of  mv-a @ play-now  done  ['] root-move act !  endof
        9 of  mv-a @ play-now  done  ['] root-move act !  endof
        16 of  mv-b @ play-now  done  ['] root-move act !  endof
        14 of  ['] turning act !  endof   15 of  ['] turning act !  endof
        5 of  $200 play-now  ['] walking act !  endof   6 of  $202 play-now  ['] walking act !  endof
        10 of  $200 play-now  ['] walking act !  endof   11 of  $202 play-now  ['] walking act !  endof
        17 of  mv-a @ play-now  ['] walking act !  endof
        1 of  0 play-now  done  ['] still act !  endof
        >r done r>   \ (the looks, the doors' uses: nothing for it)
    endcase ;

\ ---- loaded, dressed, gone ----
: unload ( -- )
    who @ 0< 0= if  who @ -1 cast-as  then
    model @ 0< 0= if  model @ actor-free  then
    -1 model !  -1 who !  0 in-scene !  ['] still act !  body-off ;
: model-load ( file len id -- model )   \ (its kind's textures when it has its own)
    kind-textures dup if  actor-load-textured  else  2drop actor-load  then ;
: load ( file len id -- )
    >r  unload  r@ model-load model !  r@ who !  r@ model @ cast-as
    r> kind-motions ?dup if  model @ swap motion-table  then
    0 him act.visible l!  -1 shown !  -1 mv-done !  ['] still act !  0 play-now ;
: dress ( model -- )   \ (another's model worn: its files loaded for this one)
    who @ 0< if  drop exit  then
    model-file  who @ load ;
\ a crow's parts by its animation (Kind14Model_Draw): in $9000 / $9001 part 5, else parts 3 and 4
\ (the original's draw form flags with them, not kept)
: crow-9000s? ( -- flag )  anim @ $9000 - 2 u< ;
: crow-parts ( -- )
    who @ crow? 0= if  exit  then
    crow-9000s?  model @ 5 2 pick actor-part-show
    0=  model @ 3 2 pick actor-part-show  model @ 4 rot actor-part-show ;
\ its model where its body is, seen in the room being played
: pose ( -- )
    model @ 0< if  exit  then
    crow-parts
    in-scene @ if  exit  then   \ (a cutscene's: shown by its shots, body or none - the golem has none)
    self body? 0= if  0 him act.visible l!  exit  then
    my-at sf@ him act.x sf!  my-at 4 + sf@ him act.y sf!  my-at 8 + sf@ him act.z sf!
    yaw him act.yaw sf!
    self body-room room-id =  shown @ and  0<> 1 and  him act.visible l! ;
PRIVATE>

behaviour figure
  on spawned ( -- )
      -1 who !  -1 model !  ['] still act !  -1 mv-done !
      2e 15e body-size  cast-floor body-mask!  body-off
      -1 st-actor !  self subscribe tick  self subscribe frame-end  self subscribe scene  self subscribe stalker-doing ;
  on cast-load ( id -- )
      dup kind-file dup 0= if  2drop drop exit  then  rot load ;
  on cast-unload ( -- )  unload ;
  on cast-dress ( model -- )  dress ;
  on cast-out ( -- )  body-off ;
  on tick ( -- )  model @ 0< 0= self body? and if  act @ execute  then  pose ;
  on frame-end ( -- )
      model @ 0< 0= self body? and if  mv-done @  ended?  model @ 0 0 1 motion-events  broadcast moving  then ;
  on scene ( on -- )  in-scene ! ;
  \ the story's
  on place ( x y z yaw -- )
      cell>f mv-yaw f!  >r >r cell>f r> cell>f r> cell>f mv-at vec!
      room-id  mv-at cast-floor v-tri-in  mv-at vec@ body-place  mv-yaw f@ body-turn ;
  on to-exit ( exit -- )
      0 exit-spot dup 0< if  drop fdrop fdrop fdrop exit  then      ( tri ) ( F: x y z )
      room-id swap body-place ;
  on show ( on -- )  0<> shown ! ;
  on scripted ( on -- )  drop ;
  on scripted-move ( kind a b x y z yaw -- )
      cell>f mv-yaw f!  >r >r cell>f r> cell>f r> cell>f mv-at vec!
      mv-b !  mv-a !  start-move ;
  on hold-anim ( anim flags blend -- )   \ (as asked: a take-off at once, not over its motion table's fade)
      {: a fl bl :}  model @ a has-motion? if  a anim !  model @ a bl fl motion-play  then
      done  ['] root-move act ! ;
  on animal-go ( sound x z -- )  >r cell>f r> cell>f  animal-go ;   \ (the rooms' commands: the rabbits)
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      2drop 2drop 2drop 2drop  st-room-is !  sender st-actor ! ;
  on noclip ( on -- )  if  0  else  cast-floor  then  body-mask! ;   \ (the story's 0x20: through blocked floor)
end-behaviour

: cast-spawn ( slot -- id )
    figure cast-state  rot case  3 of  s" cast3"  endof  4 of  s" cast4"  endof  >r s" cast5" r>  endcase  spawn ;
