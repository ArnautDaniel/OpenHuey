\ hewie/model.fs - Hewie's model: playing his animations, their groups, his poses (getting into
\ one, holding it), how he stands and walks for the danger and his mood, his voice and his bark.
IN: hewie.model
USING: engine actors common hewie.state hewie.body ;

: model ( -- n )  his-model @ ;
: him-model ( -- a )  model actor ;
: anim@ ( -- id )  model motion@ ;
\ Motion_PlayTable: with the fade and flags his motion table gives (anew: at its own pace)
: rate-1 ( -- )  1e him-model act.rate sf! ;
: play ( anim -- )  rate-1  model swap  2dup motion-entry >r drop r>  motion-play ;
: play-cut ( anim -- )  rate-1  model swap  2dup motion-entry nip nip  0 swap motion-play ;   \ Motion_Play
: play-blend ( anim n -- )   \ Motion_PlayBlend: over n frames
    rate-1  >r model swap  2dup motion-entry nip nip  r> swap motion-play ;
: play-blend8 ( anim n -- )   \ Motion_PlayBlend8: and no time of its own
    rate-1  >r model swap  2dup motion-entry nip nip 8 or  r> swap motion-play ;
: play-if-not ( anim -- )  dup anim@ <> if  play  else  drop  then ;   \ Hewie_PlayIfNot
: play-again ( anim -- )  dup anim@ <> if  play  else  play-cut  then ;
: anim-done? ( -- flag )  him-model act.mflags l@ $20 and 0<> ;   \ came to its end this frame
: settled? ( -- flag )  him-model act.fade sf@ 0e f<= ;            \ faded in
: rate! ( F: r -- )  him-model act.rate sf! ;

\ Hewie_AnimGroup: 0 standing .. 14, 15 others
: anim-group ( -- g )
    anim@ case
        $0 of 0 endof  $3 of 0 endof  $4 of 0 endof  $5 of 0 endof  $6 of 0 endof  $9 of 0 endof
        $1 of 1 endof  $2 of 2 endof  $7 of 2 endof  $8 of 3 endof
        $101 of 4 endof  $103 of 4 endof  $1000 of 4 endof  $1003 of 4 endof  $1301 of 4 endof
        $1B00 of 4 endof  $1B03 of 4 endof  $1B04 of 4 endof  $1B05 of 4 endof  $1C02 of 4 endof
        $2212 of 4 endof
        $100 of 5 endof  $105 of 5 endof  $107 of 5 endof  $1B01 of 5 endof  $1C00 of 5 endof
        $1C01 of 5 endof  $1C04 of 5 endof  $1C05 of 5 endof  $1C06 of 5 endof  $1D00 of 5 endof
        $1D01 of 5 endof  $1D02 of 5 endof
        $102 of 6 endof  $104 of 6 endof  $301 of 6 endof  $1B02 of 6 endof  $1C07 of 6 endof
        $106 of 7 endof  $202 of 8 endof  $201 of 9 endof
        $200 of $A endof  $204 of $A endof  $205 of $A endof  $206 of $A endof
        $203 of $B endof  $300 of $C endof  $1002 of $D endof  $1001 of $E endof  $2213 of $E endof
        >r $F r>
    endcase ;
\ where he is to look (Hewie_TurnHead's modes): taken up 10 frames later when it changes, or now
: look! ( mode -- )  dup his-look-to @ <> if  his-look-to !  10 his-look-delay !  else  drop  then ;
: look-now! ( mode -- )  his-look-to !  0 his-look-delay ! ;

\ Hewie_StandAnim: his standing animation for the danger and his mood (blend -1: the table's)
: stand-anim ( blend -- )
    his-danger @ 2 = his-mood @ 3 = and if  4
    else his-cond @ 1 = if  6
    else his-danger @ 2 = his-mood @ 2 = and if  5
    else his-danger @ 0<> his-alert @ 1 = and if  4   \ (the danger's bit 4: his alert this frame)
    else his-danger @ 0<> if  3
    else his-action @ 8 = his-action @ $A = or if  4
    else  0  then then then then then then                ( blend a )
    dup anim@ = if  2drop exit  then
    swap dup -1 = if  drop play  else  play-blend  then ;
\ Hewie_WalkAnim: 0x200; limping 0x206; 0x205 while followed
: walk-anim ( -- )
    his-mood @ 3 = if  $200
    else his-cond @ 1 = if  $206
    else his-mood @ 2 = if  $200
    else his-danger @ 1 = if  $205  else  $200  then then then then  play-if-not ;

\ ---- poses: getting into one (Hewie_StepToPose) and holding it (Hewie_KeepPose) ----
\ kinds: 0 stand, 1 sit, 2 lie, 3 / 4 the low groups (0..3) do, 5 most anything still, 6 lying
\ on his side (group 11), 7..9 walking (as 5), 10 down (group 13)
: pose-leave ( g -- )   \ the way out of group g most poses share
    case
        3 of  $107 play  endof
        7 of  anim-done? if  $107 play  then  endof
        8 of  his-cond @ 1 = if  $206 play-if-not  else  $201 play-if-not  then  endof
        9 of  walk-anim  endof
        10 of  -1 stand-anim  endof
        15 of  -1 stand-anim  endof
        11 of  $301 play  endof
        12 of  anim-done? if  $301 play  then  endof
        13 of  $1003 play  endof
        14 of  anim-done? if  $1003 play  then  endof
    endcase ;
\ from a basic group (0..2, or its entering 4..6), the animation into the pose
: pose-basic ( g from0 from1 from2 -- 0 | -1 )
    3 pick >r
    r@ dup 0= swap 4 = or if  2drop nip  else
    r@ dup 1 = swap 5 = or if  drop nip nip  else  nip nip nip  then then   ( anim )
    r> 4 >= anim-done? 0= and if  drop -1 exit  then
    dup 0= if  exit  then  play -1 ;
: basic? ( g -- flag )  dup 3 < swap 4 7 within or ;
: step-to-pose ( kind -- 0 | -1 )   \ 0: in it
    settled? 0= if  drop -1 exit  then
    dup 11 >= if  drop -1 exit  then
    dup 7 10 within if  drop 5 recurse 0= if  0  else  -1  then  exit  then
    anim-group                                                     ( kind g )
    dup 16 >= if  2drop -1 exit  then
    swap case
        0 of  dup basic? if  0 $101 $103 pose-basic exit  then  endof
        1 of  dup basic? if  $100 0 $105 pose-basic exit  then  endof
        2 of  dup basic? if  $102 $104 0 pose-basic exit  then  endof
        3 of  dup 3 < if  drop 0 exit  then
              dup 4 7 within if  drop anim-done? if 0 else -1 then exit  then  endof
        4 of  dup 4 < if  drop 0 exit  then
              dup 4 7 within if  drop anim-done? if 0 else -1 then exit  then
              dup 7 = if  drop anim-done? if  8 play  then  -1 exit  then  endof
        5 of  dup 3 < over 8 12 within or if  drop 0 exit  then
              dup 4 7 within if  drop anim-done? if 0 else -1 then exit  then  endof
        6 of  dup 11 = if  drop 0 exit  then
              dup 12 = if  drop anim-done? if 0 else -1 then exit  then
              drop 2 recurse 0= if  $300 play  then  -1 exit  endof
        10 of  dup 13 = if  drop 0 exit  then
               dup 14 = if  drop anim-done? if 0 else -1 then exit  then
               dup basic? if  $1001 $101 $103 pose-basic exit  then  endof
    endcase
    pose-leave -1 ;
: pose-walk ( anim -- )  his-cond @ 1 = if  drop $206  then  play-if-not ;
: pose-settle ( base -- )
    case
        0 of  -1 stand-anim  endof
        1 of  1 play-if-not  endof
        2 of  his-cond @ 1 = if  7  else  2  then  play-if-not  endof
    endcase ;
\ sPoseInto[to][from]: the animations between the basic poses (3: standing up from down)
create pose-into  0 , $101 , $103 ,  $100 , 0 , $105 ,  $102 , $104 , 0 ,  $1001 , $101 , $103 ,
: keep-pose ( kind -- )
    settled? 0= if  drop exit  then
    anim-group swap                                                ( g kind )
    case
        7 of  dup 10 = if  true  else  5 step-to-pose 0=  then
              if  walk-anim  then  drop exit  endof
        8 of  dup 10 = over 9 = or if  true  else  5 step-to-pose 0=  then
              if  $201 pose-walk  then  drop exit  endof
        9 of  dup 10 = over 8 = or if  true  else  5 step-to-pose 0=  then
              if  $202 pose-walk  then  drop exit  endof
        dup
    endcase                                                        ( g kind )
    dup 11 >= 2 pick 16 >= or if  2drop exit  then
    dup 6 = if
        drop dup 12 = if  drop anim-done? if  $203 play  then  exit  then
        11 <> if  2 step-to-pose 0= if  $300 play  then  then  exit
    then
    over basic? if
        over 4 >= anim-done? 0= and if  2drop exit  then
        swap dup 4 >= if  4 -  then  swap                         ( base kind )
        dup 3 6 within if  drop pose-settle exit  then
        dup 10 = if  drop 3  then                                  ( base to )
        3 * over + cells pose-into + @                             ( base anim )
        dup 0= if  drop pose-settle  else  nip play  then  exit
    then
    dup 4 = 2 pick 3 = and if  2drop exit  then
    dup 4 = 2 pick 7 = and if  2drop anim-done? if  8 play  then  exit  then
    dup 5 = 2 pick 8 11 within and if
        drop dup 10 = if  drop walk-anim  else  8 = if  $202  else  $201  then  pose-walk  then  exit
    then
    dup 10 = 2 pick 13 = and if  2drop exit  then
    dup 10 = 2 pick 14 = and if  2drop anim-done? if  $1002 play  then  exit  then
    drop pose-leave ;
\ the stance: once standing, animation 5 when mistreated, else 4
: stance ( -- )  0 step-to-pose 0= if  his-mood @ 2 = if  5  else  4  then  play-if-not  then ;

\ ---- his voice (Hewie_MakeSound): not within 10 frames of the last; some only after 40..60;
\ his loud barks (0x65 / 0x66) and growls (0x5D / 0x5E) heard by the others ----
: soon-after? ( snd t -- flag )  his-snd @ rot = his-snd-t @ rot < and ;
: too-soon? ( snd -- flag )
    case
        $59 of  $59 40 soon-after?  endof
        $58 of  $58 50 soon-after?  $70 60 soon-after? or  $6F 40 soon-after? or  endof
        $6F of  $58 50 soon-after?  $70 60 soon-after? or  $6F 40 soon-after? or  endof
        $70 of  $58 50 soon-after?  $70 60 soon-after? or  $6F 40 soon-after? or  endof
        >r false r>
    endcase ;
: make-sound ( snd -- )
    his-snd @ dup $59 <> over $58 <> and over $70 <> and swap $6F <> and his-snd-t @ 10 < and if
        drop exit
    then
    dup $65 = over $66 = or if  $80 his-tri noise-make  then
    dup $5D = over $5E = or if  $1B his-tri noise-make  then
    dup too-soon? if  drop exit  then
    dup 5 0 0 his-at vec@ sound-at  his-snd !  0 his-snd-t ! ;

\ Hewie_Bark: by his pose when calm (standing 0x1B00, sitting 0x1B01, lying 0x1B02, limping
\ 0x1B05), else by his mood (2: 0x1B04, else 0x1B03); restarted when already barking
: bark ( -- )
    his-danger @ 0= his-action @ $A <> and his-action @ $B <> and if
        his-cond @ 1 = if  $1B05 play-again exit  then
        anim-group dup 6 = over 2 = or if  drop $1B02
        else dup 5 = swap 1 = or if  $1B01  else  $1B00  then then  play-again
    else
        his-mood @ 2 = if  $1B04  else  $1B03  then  play-again
    then ;
