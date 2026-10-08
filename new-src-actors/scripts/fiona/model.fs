\ fiona/model.fs - Fiona's model: playing her animations, and which ones she plays to stand,
\ walk and run as she feels (Fiona_IdleAnim, Fiona_WalkLook, Fiona_RunLook). Her place on
\ the model and her body follow from her state.
IN: fiona.model
USING: engine actors fiona.state ;

: her ( -- a )  her-model @ actor ;
: her-pos ( F: -- x y z )  her-at dup sf@ dup 4 + sf@ 8 + sf@ ;
\ the model and the body where she is
: show-her ( -- )
    her-at sf@ her act.x sf!  her-at 4 + sf@ her act.y sf!  her-at 8 + sf@ her act.z sf!
    her-yaw f@ her act.yaw sf!
    room-id her-tri @ her-pos body-place  her-yaw f@ body-turn ;

\ ---- her animations ----
: anim@ ( -- id )  her-model @ motion@ ;
\ Motion_Start: the animation (with a variant: -1 none), blended over `blend` frames, with flags
\ (1 loops, 8 no time of its own); one her model lacks: nothing
variable ps-a  variable ps-v  variable ps-b  variable ps-f
: start ( anim variant blend flags -- )
    ps-f !  ps-b !  ps-v !  ps-a !
    her-model @ ps-a @ has-motion? 0= if  exit  then
    1e her act.rate sf!
    her-model @ ps-a @ ps-b @ ps-f @ motion-play
    ps-v @ dup her-variant !  her-model @ swap motion-variant ;
: entry ( anim -- blend flags )  her-model @ swap motion-entry nip ;   \ its table's fade and flags
: play-table ( anim variant -- )  over entry start ;                 \ Motion_PlayTable
: play-blend ( anim n variant -- )  2 pick entry nip >r swap r> start ;   \ blended over n frames
: weight! ( F: w -- )  her act.vweight sf! ;      \ the animation against its variant
: settled? ( -- flag )  her act.fade sf@ 0e f<= ;  \ faded in
: ended? ( -- flag )  her act.mflags l@ $20 and 0<> ;   \ came to its end this frame

\ the group an animation belongs to (Fiona_AnimGroup): 0 standing, 1 walking, 2 running, 3 out
\ of breath, 4 0xB01, 5 resting, 6 0x12xx, 7 a ladder, 8 its top, 9 0x403, 10 0xE01, 11 others
: anim-group ( anim -- g )
    case
        0 of 0 endof  2 of 0 endof  3 of 0 endof  4 of 0 endof  5 of 0 endof
        1 of 5 endof
        $200 of 1 endof  $201 of 1 endof  $204 of 1 endof  $208 of 1 endof
        $400 of 1 endof  $401 of 1 endof  $402 of 1 endof
        $202 of 2 endof  $203 of 2 endof  $205 of 2 endof  $206 of 2 endof
        $207 of 3 endof  $B01 of 4 endof
        $1200 of 6 endof  $1201 of 6 endof  $1202 of 6 endof  $1203 of 6 endof
        $700 of 7 endof  $701 of 7 endof  $702 of 7 endof  $703 of 7 endof
        $704 of 7 endof  $705 of 7 endof  $706 of 7 endof  $707 of 7 endof
        $708 of 8 endof  $709 of 8 endof  $403 of 9 endof  $E01 of 10 endof
        >r 11 r>
    endcase ;
: group ( -- g )  anim@ anim-group ;

\ ---- her looks: how she stands, walks and runs as she feels ----
: idle-base ( -- anim )  her-danger @ 2 = if  5  else  0  then ;
: weight-moved? ( F: w -- flag )  her-blend-w f@ f- fabs 0.1e f> ;
fvariable calm-a  fvariable calm-b
: feel ( -- )   \ a: (100 - fear) / 60, b: (1800 - recovery) / 1800
    100e her-fear f@ f- 60e f/ calm-a f!  1800 her-recovery @ - s>f 1800e f/ calm-b f! ;
: idle-play ( anim blend variant -- )  over -1 = if  nip play-table  else  play-blend  then ;
: playing? ( anim variant -- flag )  her-variant @ = swap anim@ = and ;
\ Fiona_IdleAnim: out of breath (variant 4, its weight eased toward the panic), shaken (2, by
\ her fear) or frightened (3, by her recovery); weight 1 is her plain idle
: idle-anim ( blend -- )
    her-mode @ 0= if  0 her-sub !  then
    her-fear-bits @ 1 and if
        idle-base 4 playing? 0= if  idle-base over 4 idle-play  then  drop
        90e her-panic-level f@ f- 15e f/ 0e fmax                      ( F: t )
        her-tired-w f@ fover f< if  her-tired-w f@ 0.05e f+ fmin  else  her-tired-w f@ 0.05e f- fmax  then
        fdup her-tired-w f!  weight! exit
    then
    feel
    calm-b f@ 0.5e f< 0=  calm-b f@ calm-a f@ 0.25e f+ f>= and if
        calm-a f@ 1e f< if
            idle-base 2 playing? 0=  calm-a f@ weight-moved? or if  idle-base over 2 idle-play  then
            drop calm-a f@ weight!  calm-a f@ her-blend-w f!
        else
            idle-base -1 playing? 0= if  idle-base over -1 idle-play  then
            drop 1e weight!
        then  exit
    then
    idle-base 3 playing? 0=  calm-b f@ weight-moved? or if  idle-base over 3 idle-play  then
    drop calm-b f@ weight!  calm-b f@ her-blend-w f! ;

\ Fiona_WalkLook / Fiona_RunLook: the walk (0x200; chased 0x208) or run (0x202) alone, or blended
\ with its frightened (0x204 / 0x205) or shaken (0x201 / 0x203) variant; panicking, the panic
\ run 0x206
variable look-base  variable look-scared  variable look-shaken
: look-blend ( var -- ) ( F: w -- )
    look-base @ over playing? if  fdup weight-moved? 0= if  drop  else  look-base @ swap play-table  then
    else  look-base @ swap play-table  then
    fdup weight!  her-blend-w f! ;
: move-look ( -- )
    feel
    calm-b f@ 0.5e f<  calm-b f@ calm-a f@ 0.25e f+ f< or if  calm-b f@ look-scared @ look-blend exit  then
    calm-a f@ 1e f< if  calm-a f@ look-shaken @ look-blend exit  then
    look-base @ -1 playing? 0= if  look-base @ -1 play-table  then  1e weight! ;
: walk-look ( -- )
    her-mode @ 0= if  1 her-sub !  then
    her-danger @ 2 = if  $208  else  $200  then  look-base !  $204 look-scared !  $201 look-shaken !  move-look ;
: run-look ( -- )
    her-mode @ 0= if  2 her-sub !  then
    her-fear-bits @ 2 and if  anim@ $206 <> if  $206 -1 play-table  1e weight!  then  exit  then
    $202 look-base !  $205 look-scared !  $203 look-shaken !  move-look ;
: stand ( -- )  0 her-rest !  -1 idle-anim ;   \ Fiona_Stand
