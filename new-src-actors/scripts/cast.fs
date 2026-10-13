\ cast.fs - the event characters (docs/subsystems/cast.md; src/game/char_load.c CharLoad_EventChar,
\ story_chars.c): the story's figures in character slots 3..5 - the crows, Fiona's doubles, the
\ figures of its scenes. A figure an actor (`cast3`..`cast5`): loaded as character `id` (its kind's
\ model: the kinds' table below; cast as `id` for the cutscenes), placed by the story (`place`,
\ at an exit `to-exit`), shown or hidden, its moves the story's (`scripted-move`: an animation,
\ walking to a spot, turning; `hold-anim`), reporting each frame's end (`moving`). Given another's
\ model (`cast-dress`: the scripts' hand-over, Pursuer_GiveMotionBanks) it wears that.
IN: cast
USING: engine game-state actors messages common ;

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
: load ( file len id -- )
    >r  unload  actor-load model !  r@ who !  r> model @ cast-as
    0 him act.visible l!  -1 shown !  -1 mv-done !  ['] still act !  0 play-now ;
: dress ( model -- )   \ (another's model worn: its files loaded for this one)
    who @ 0< if  drop exit  then
    model-file  who @ load ;
\ its model where its body is, seen in the room being played
: pose ( -- )
    model @ 0< if  exit  then
    self body? 0= if  0 him act.visible l!  exit  then
    in-scene @ if  exit  then
    my-at sf@ him act.x sf!  my-at 4 + sf@ him act.y sf!  my-at 8 + sf@ him act.z sf!
    yaw him act.yaw sf!
    self body-room room-id =  shown @ and  0<> 1 and  him act.visible l! ;
PRIVATE>

behaviour figure
  on spawned ( -- )
      -1 who !  -1 model !  ['] still act !  -1 mv-done !
      2e 15e body-size  cast-floor body-mask!  body-off
      self subscribe tick  self subscribe frame-end  self subscribe scene ;
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
  on hold-anim ( anim blend -- )  drop play-now  done  ['] root-move act ! ;
end-behaviour

: cast-spawn ( slot -- id )
    figure cast-state  rot case  3 of  s" cast3"  endof  4 of  s" cast4"  endof  >r s" cast5" r>  endcase  spawn ;
