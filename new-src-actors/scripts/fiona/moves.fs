\ fiona/moves.fs - the story's moves for her (Fiona_Requests' +0xF4 and the Fiona_StateCmd*
\ states): walking or running to a spot or along a path, animations, turning on the spot,
\ looks; each sets her move done (+0xE1) as the original's do. She reports at each frame's end
\ (`moving`: done, her animation ended).
IN: fiona.moves
USING: engine common paths fiona.state fiona.model fiona.moving fiona.spots ;

defer to-idle   ' noop is to-idle      \ (fiona.fs)
: done ( -- )  -1 her-move-done ! ;
create there 12 allot

\ script_anim_keep: the move's animation (b; -1: her walk while `walk-code` is what she does,
\ else her run - none for code 0) started again whenever the last has run out
: anim-kept ( walk-code -- )
    settled? 0= if  drop exit  then
    her-move-b @ -1 = if
        ?dup if  her-doing @ = if  walk-look  else  run-look  then  then
    else  drop  anim@ her-move-b @ <> if  her-move-b @ -1 play-table  then  then ;

\ ---- along a planned path, turning toward it 10 degrees a frame and stepping by her root
\ motion - the less the sharper the turn (Fiona_StateCmdPath / CmdPathTurn) ----
: spot-tri ( -- tri )   \ (the spot's triangle: the one given, else under the point)
    her-move @ 17 = her-move-a @ 0< or if  her-move-at blocked-floor v-tri-in  else  her-move-a @  then ;
: plan ( -- ok? )  her-path her-tri @ her-at spot-tri her-move-at blocked-floor path-plan 0> ;
fvariable turn-left  fvariable stepping
: path-step ( -- arrived? )
    root@
    her-path her-at 3e path-ahead drop  her-at ahead vec-heading            ( F: toward )
    her-yaw f@ fswap 10e deg>rad turn-toward  turn-left f!  her-yaw f!
    rm-z f@ f0< if  0e  else  rm-z f@  pi turn-left f@ f-  pi f/ f*  then  stepping f!
    her-path her-at stepping f@ path-ahead                                   ( i )
    ahead blocked-floor v-tri-in dup 0< if  drop  else  her-tri !  then
    her-at ahead vec-copy  her-path path-i!
    her-path path-left? 0= ;
: anim-done ( -- )   \ Fiona_StateCmdAnimDone: once it has run out (the default, still moving: stopped first)
    settled? 0= if  exit  then
    her-move-b @ -1 = group 0<> and if  -1 idle-anim exit  then
    done ;
: cmd-turn ( -- )   \ Fiona_StateCmdTurn: to the heading 20 degrees a frame with the animation, then the wait
    0 anim-kept
    her-yaw f@ her-move-yaw f@ 20e deg>rad turn-toward f0= her-yaw f!
    if  ['] anim-done her-act !  then ;
: path-turn ( -- )   \ Fiona_StateCmdPathTurn (5 with a path, 10)
    path-step 0= if  $12 anim-kept exit  then
    her-yaw f@ her-move-yaw f@ 10e deg>rad turn-toward f0= her-yaw f!
    if  ['] anim-done  else  ['] cmd-turn  then  her-act ! ;
: path-to ( -- )   \ Fiona_StateCmdPath (6 / 11): at the end the rest of the step, done, idle
    $14 anim-kept
    path-step 0= if  exit  then
    done  to-idle ;

\ ---- 5 straight: to the spot facing the heading (Fiona_DoorFrame, as at a door) ----
: to-spot ( -- )  walk-to-spot dup 0< if  drop to-idle exit  then  0= if  done  then ;

\ ---- turning on the spot (14 / 15: Fiona_StateTurnOnSpot / TurnStanding, then her idle step) ----
: idle-step ( -- )   \ Fiona_StateIdleStep: standing played out - done; else her idle
    settled? if  group 0= if  done  else  -1 idle-anim  then  then  root-move ;
: turning ( -- )
    her-yaw f@ her-heading f@ 10e deg>rad turn-toward  turn-left f!  her-yaw f!
    settled? 0= if  exit  then
    group if  turn-left f@ pi f2/ f< if  -1 idle-anim  then  exit  then
    turn-left f@ f0= if  ['] idle-step her-act !  exit  then
    her-heading f@ her-yaw f@ f- angle-wrap f0< if  $400  else  $401  then
    dup anim@ <> if  -1 play-table  else  drop  then ;

\ ---- 17: the walk's animation along the way, arriving as it ends (Fiona_StateCmdWalkAnim) ----
: walk-anim ( -- )
    her-model @ 0 0 1 motion-events $20 and if
        her-move-at blocked-floor v-tri-in dup 0< if  drop  else  her-tri !  then
        her-at her-move-at vec-copy  her-move-yaw f@ her-yaw f!  done exit
    then
    her-yaw f@ her-move-yaw f@ her-move-turn f@ turn-toward fdrop her-yaw f!
    her-path her-at her-move-step f@ path-ahead  her-at ahead vec-copy  her-path path-i!
    ahead blocked-floor v-tri-in dup 0< if  drop  else  her-tri !  then ;

\ ---- Fiona_Requests: a move given ----
: path-start ( -- )   \ (a way to the spot; none: idle)
    plan if  ['] path-turn her-act !  else  to-idle  then ;
: start-move ( kind -- )
    0 her-move-done !  2 her-mode !
    case
        1 of  to-idle  done  endof
        2 of  ['] idle-step her-act !  endof
        5 of  $12 her-doing !
              her-move-b @ -1 = if
                  spot-tri her-move-at vec@ her-move-yaw f@ walk-spot  ['] to-spot her-act !
              else  path-start  then  endof
        10 of  $13 her-doing !  path-start  endof
        6 of  $14 her-doing !  plan if  ['] path-to her-act !  else  to-idle  then  endof
        11 of  $15 her-doing !  plan if  ['] path-to her-act !  else  to-idle  then  endof
        7 of  $11 her-doing !  her-move-a @ -1 play-table  done  ['] root-move her-act !  endof
        8 of  $11 her-doing !  her-move-a @ her-move-b @ -1 play-blend  done  ['] root-move her-act !  endof
        9 of  $11 her-doing !  her-move-a @ her-move-b @ -1 play-blend  done  ['] root-move her-act !  endof   \ (no time of its own: like 8 here)
        16 of  $11 her-doing !  her-move-b @ idle-anim  done  ['] root-move her-act !  endof
        12 of  her-move-a @ dup 0< if  drop 0 her-look-on !  else  her-look-who !  1 her-look-on !  then  done  endof
        13 of  her-move-at her-look-pt 12 move  -1 her-look-who !  1 her-look-on !  done  endof
        14 of  $16 her-doing !  her-move-yaw f@ her-heading f!  ['] turning her-act !  endof   \ (the heading to the character: the room's)
        15 of  $17 her-doing !  her-move-yaw f@ her-heading f!  ['] turning her-act !  endof
        17 of  plan if
                   her-move-a @ her-move-b @ -1 play-blend
                   her-path her-at path-rest  her-model @ motion-frames s>f  fdup f0= if  fdrop 1e  then
                   fover fover f/ 0.1e f+ her-move-step f!  fswap fdrop
                   her-move-yaw f@ her-yaw f@ f- angle-wrap fabs fswap f/ 0.1e deg>rad f+ her-move-turn f!
                   $18 her-doing !  ['] walk-anim her-act !
               else  done  then  endof
        >r done r>   \ (3 / 4 a door's use from the scripts: with the doors' story)
    endcase ;
