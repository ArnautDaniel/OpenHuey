\ partner/core.fs - Hewie (the original's gCharPartner, src/game/hewie.c): his own state, how he
\ sees the others, his animations and poses. Words keep the original's names in their comments
\ (Hewie_*) so the two read side by side; the offsets there are his fields in the original.
\
\ He is character slot 1, Fiona slot 0; the pursuer (gCharPursuer) is the slot in `pursuer`.
IN: partner.core
USING: engine game-state events.core events.words chars ;

1 constant him
0 constant her
variable pursuer  -1 pursuer !        \ the stalker's slot, -1 none
variable fiona-cmd  -1 fiona-cmd !    \ the command her controls give this frame (her +0x1AD6B8), -1 none

: vector ( "name" -- )  create 12 allot ;

\ ---- chance (gRandom +0x1C: 0 <= r < 1) ----
: rnd01 ( F: -- r )  32768 random s>f 32768e f/ ;
: roll ( n -- i )  s>f rnd01 f* f>s ;   \ (s32)(n x r)

\ ---- the characters (chars has their bodies) ----
: c-sub ( cs -- n )  character char.sub sl@ ;
: played-room ( -- room )  room-id ;   \ (the room loaded: the one being played)
\ |a| of a wrapped angle (hwrap_abs)
: wrap-abs ( F: a -- |a| )  angle-wrap fabs ;
: deg>rad ( F: d -- r )  3.14159265e f* 180e f/ ;

\ ---- Hewie's fields ----
\ what he is doing
variable h-action      variable h-last-action     \ +0xF3564, last frame's +0xF3568
variable h-next                                   \ +0xF3570: the argument / the action after
variable h-state                                  \ +0xF35D0: his behaviour, run each frame (an xt)
variable h-mood-state                             \ a.state: his mode's behaviour (calm / wary / tense)
variable h-wait                                   \ +0xF355C: frames before he moves on
variable h-target   variable h-target2            \ +0xF3544 / +0xF3548: whom (a slot, -1 none)
variable h-cmd                                    \ +0xF356C: what may break in (bit 31: nothing)
variable h-pending                                \ +0xF3559: choose what to do next
variable h-look-to  variable h-look-delay       \ +0xF3604 / +0xF3608: where he is to look, after a delay
variable h-yelp   variable h-yelp-anim            \ +0xF35B4 / +0xF35B8
\ how he feels and obeys
variable h-mood   variable h-mood-time            \ +0xF35C0 (0 normal, 1 pleased, 2 upset, 3 angry), +0xF35BE
variable h-waiting                                \ +0xF3598: waiting (1) or obeying (0)
variable h-obey                                   \ +0xF359C: frames left of that
variable h-obey-marked                            \ +0xF3587
variable h-nudge                                  \ +0xF35A0
variable h-praise-due                             \ +0xF3586
variable h-trust                                  \ +0xF35CC: 0..7
variable h-trust-points                           \ (Hewie_AddTrust: 0..10000)
variable h-broke                                  \ +0xF358C: an activity broken off once
variable h-cooldown                               \ +0xF35DC
variable h-stay                                   \ +0xF3588: keeps him in pose 4
variable h-pet-time                               \ +0xF3688
variable h-praise-a  variable h-praise-b          \ +0xF3684 / +0xF3686
variable h-did  variable h-did-was                \ +0xF36A4 / +0xF36A8: what he did, for praise
variable h-did-2                                  \ +0xF36AC
variable h-follows                                \ +0xF368C: a thing / creature he follows (0 none)
create h-feel 4 cells allot  h-feel 4 cells 0 fill   \ +0xF3674..: his feeling about each stalker
create h-dare 4 cells allot  h-dare 4 cells 0 fill   \ +0xF367C..
\ what he notices
variable h-alert  variable h-alert-was  variable h-alert-what   \ +0xF366D, +0xF366C, +0xF3670
variable h-held                                   \ +0xF3580: Fiona was held
variable h-panic-seen                             \ +0xF3589
variable h-scene-req                              \ +0xF3584
variable h-call   $FF h-call !                    \ +0xF36B0: who called him (0 Fiona), $FF none
variable h-hits  variable h-hit-t                  \ +0xF35C4 / +0xF35C8: how often Fiona hit him lately (one forgiven every 300 frames)
variable h-hold-call                              \ +0xF35B0
variable h-ready                                  \ +0xF3585
variable h-no-root  variable h-root-ok            \ +0xF3558 / +0xF3582
variable h-look                                   \ +0xF3620: look at something
variable h-snd   variable h-snd-t                 \ +0xF35A4 / +0xF35A8
variable h-2b   variable h-2d                     \ through blocked floor / left alone (+0x2B, +0x2D)
\ his behaviours' working values
variable h-t1  variable h-t2  variable h-t3       \ +0xF36B4 / +0xF36B8 / +0xF36BC
fvariable h-heading  fvariable h-turn  fvariable h-f36cc   \ +0xF36C4 / +0xF36C8 / +0xF36CC
vector h-spot                                     \ +0xF36E0
variable h-wanted                                 \ +0xF3560: how many times (attacks), or a wait
\ where he looks (Hewie_TurnHead)
variable h-look-now  variable h-look-t            \ +0xF3600 the look mode taken up, +0xF360C its timer
fvariable h-look-pitch  fvariable h-look-yaw      \ +0xF3614 / +0xF3618: a look held (mode 8, glances)
fvariable h-head-pitch  fvariable h-head-yaw      \ the motion's +0x854 / +0x858: his head turned
fvariable h-yaw-was                               \ +0xF354C: his heading the frame before
variable h-look-char  $FF h-look-char !           \ +0xF3610: a character he looks at ($FF none)
variable h-look-pt?  vector h-look-pt             \ +0xF35E0 / +0xF35F0: a point he looks at
vector h-scent                                    \ +0xF3630: what he smells (h-look: +0xF3620 set)
\ where Fiona's commands put him
fvariable h-side  variable h-side-dir             \ +0xF3550 / +0xF3554: his place beside her
variable h-last-idle                              \ +0xF357C: the last idle trick
variable h-cmd-was  variable h-cmd-act            \ +0xF3578 / +0xF3574: her command, its action
variable h-cmd-tri  vector h-cmd-pos  fvariable h-cmd-yaw   \ the spot her 0x23 shows (state [2..5])
\ a spot to go to (the character's +0x104 triangle, +0x108 animation, +0x10C heading, +0x110)
variable h-to-tri  variable h-to-anim  fvariable h-to-yaw  vector h-to
variable h-pet  variable h-1d                     \ +0xF36C0 strokes, +0xF36A2 rolls
variable h-hurt-t                                 \ +0xF35AC: frames to his next health point
variable h-by  $FF h-by !  variable h-how          \ +0x100 / +0x104: who struck him last ($FF: a door), and how

: h-reset-fields ( -- )
    0 h-action !  0 h-last-action !  0 h-next !  0 h-wait !  -1 h-target !  -1 h-target2 !
    0 h-cmd !  0 h-pending !  0 h-look-to !  0 h-look-delay !  -1 h-yelp !
    0 h-mood !  0 h-mood-time !  0 h-waiting !  0 h-obey !  0 h-obey-marked !  0 h-nudge !
    0 h-praise-due !  0 h-broke !  0 h-cooldown !  0 h-stay !  0 h-pet-time !
    0 h-did !  0 h-did-was !  0 h-follows !  0 h-alert !  0 h-alert-was !  0 h-held !
    0 h-panic-seen !  0 h-scene-req !  $FF h-call !  0 h-hits !  0 h-hit-t !
    0 h-hold-call !  0 h-look !  0 h-snd !  0 h-snd-t !  0 h-2b !  0 h-2d !
    0 h-look-now !  0 h-look-t !  $FF h-look-char !  0 h-look-pt? !  0 h-last-idle !
    0e h-head-pitch f!  0e h-head-yaw f!  0e h-side f!  0 h-side-dir !  300 h-hurt-t ! ;
h-reset-fields

\ ---- what to call next (the actions are in partner.actions) ----
defer set-action ( act arg -- )     ' 2drop is set-action      \ Hewie_SetAction
defer adjust-action ( act -- act' ) ' noop is adjust-action    \ Hewie_AdjustAction
\ hewie_want: the action his situation makes of `act` (with `arg` only if it stays itself)
: want ( act arg -- )  over adjust-action rot over <> if  nip 0  else  swap  then  set-action ;
\ Hewie_ToDefault
: to-default ( -- )  0 0 want ;
\ instead: the action picked instead of `act` (as want)
: instead ( act arg -- )  want ;
\ his behaviour (the pointer to member +0xF35D0)
: behave ( xt -- )  h-state ! ;

\ ---- his body ----
: h-actor ( -- a )  him c-actor ;
: h-pos ( -- v )  him c-pos ;
: h-yaw ( F: -- a )  him c-yaw ;
: h-yaw! ( F: a -- )  him c-yaw! ;
: h-tri ( -- tri )  him c-tri ;
: h-room ( -- room )  him c-room ;
: h-cond ( -- n )  him c-cond ;
: h-mode ( -- n )  him c-mode ;                   \ +0xF8 (MODE)
: h-mode! ( n -- )  him character char.mode l! ;
: h-busy? ( -- flag )  him character char.scripted sl@ 0<> ;   \ +0xE0
: h-disabled? ( -- flag )  him character char.disabled sl@ 0<> ;
: h-done ( -- )  1 him character char.move-done l! ;   \ +0xE1: his scripted move is over
: h-hp ( -- n )  him character char.hp sl@ ;

\ ---- his animations (his motion: Motion_Play*, MOTION_ANIM, the end and fade flags) ----
: anim@ ( -- id )  h-actor dup 0< if  exit  then  motion@ ;
\ Motion_PlayTable: with the fade and flags his motion table gives
: rate-1 ( a -- )  actor 1e act.rate sf! ;   \ (playing anew: its own pace)
: play ( anim -- )
    h-actor dup 0< if  2drop exit  then  dup rate-1  swap
    2dup motion-entry >r drop r>                  ( a anim blend flags )
    motion-play ;
\ Motion_Play: the table's flags, cut in
: play-cut ( anim -- )
    h-actor dup 0< if  2drop exit  then  dup rate-1  swap
    2dup motion-entry nip nip 0 swap motion-play ;
\ Motion_PlayBlend: over `n` frames
: play-blend ( anim n -- )
    h-actor dup 0< if  drop 2drop exit  then  dup rate-1  -rot
    over h-actor swap motion-entry nip nip motion-play ;
\ Motion_PlayBlend8: blended over `n` frames, with no time of its own (flag 8)
: play-blend8 ( anim n -- )
    h-actor dup 0< if  drop 2drop exit  then  dup rate-1  -rot
    over h-actor swap motion-entry nip nip 8 or motion-play ;
\ the animation came to its end this frame (its key flag 0x20); it isn't fading in any more
: anim-done? ( -- flag )  h-actor dup 0< if  exit  then  actor act.mflags l@ $20 and 0<> ;
: settled? ( -- flag )  h-actor dup 0< if  drop true exit  then  actor act.fade sf@ 0e f<= ;

\ Hewie_AnimGroup: the group of his animation (0 standing .. 14, 15 other)
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
\ anim(): where he is to look (Hewie_TurnHead's modes; taken up 10 frames later when it changes)
: look! ( mode -- )  dup h-look-to @ <> if  h-look-to !  10 h-look-delay !  else  drop  then ;
\ the same at once
: look-now! ( mode -- )  h-look-to !  0 h-look-delay ! ;
: play-if-not ( anim -- )  dup anim@ <> if  play  else  drop  then ;   \ Hewie_PlayIfNot

\ Hewie_StandAnim: his standing animation for the game's state (-1: the table's fade)
: stand-anim ( blend -- )
    game-mode @ 2 = h-mood @ 3 = and if  4
    else h-cond 1 = if  6
    else game-mode @ 2 = h-mood @ 2 = and if  5
    else game-mode @ 0<> 4 cond-bit? and if  4
    else game-mode @ 0<> if  3
    else h-action @ 8 = h-action @ $A = or if  4
    else  0  then then then then then then                ( blend a )
    dup anim@ = if  2drop exit  then
    swap dup -1 = if  drop play  else  play-blend  then ;

\ Hewie_WalkAnim: 0x200; limping 0x206; 0x205 while followed
: walk-anim ( -- )
    h-mood @ 3 = if  $200
    else h-cond 1 = if  $206
    else h-mood @ 2 = if  $200
    else game-mode @ 1 = if  $205  else  $200  then then then then  play-if-not ;

\ ---- poses: getting into one (Hewie_StepToPose) and holding it (Hewie_KeepPose) ----
\ kinds: 0 stand, 1 sit, 2 lie, 3 / 4 the low groups (0..3) do, 5 most anything still, 6 lying
\ on his side (group 11), 7..9 walking (as 5), 10 down (group 13)
\ pose_leave: the way out of group g shared by most poses
: pose-leave ( g -- )
    case
        3 of  $107 play  endof
        7 of  anim-done? if  $107 play  then  endof
        8 of  h-cond 1 = if  $206 play-if-not  else  $201 play-if-not  then  endof
        9 of  walk-anim  endof
        10 of  -1 stand-anim  endof
        15 of  -1 stand-anim  endof
        11 of  $301 play  endof
        12 of  anim-done? if  $301 play  then  endof
        13 of  $1003 play  endof
        14 of  anim-done? if  $1003 play  then  endof
    endcase ;
\ pose_basic: from a basic group (0..2, or its entering 4..6), the animation into the pose
: pose-basic ( g from0 from1 from2 -- 0 | -1 )
    3 pick >r
    r@ dup 0= swap 4 = or if  2drop nip  else
    r@ dup 1 = swap 5 = or if  drop nip nip  else  nip nip nip  then then   ( anim )
    r> 4 >= anim-done? 0= and if  drop -1 exit  then
    dup 0= if  exit  then  play -1 ;
: basic? ( g -- flag )  dup 3 < swap 4 7 within or ;
: step-to-pose ( kind -- 0 | -1 )   \ Hewie_StepToPose
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

\ pose_walk / pose_settle
: pose-walk ( anim -- )  h-cond 1 = if  drop $206  then  play-if-not ;
: pose-settle ( base -- )
    case
        0 of  -1 stand-anim  endof
        1 of  1 play-if-not  endof
        2 of  h-cond 1 = if  7  else  2  then  play-if-not  endof
    endcase ;
\ sPoseInto[to][from]: the animations between the basic poses (3: standing up from down)
create pose-into  0 , $101 , $103 ,  $100 , 0 , $105 ,  $102 , $104 , 0 ,  $1001 , $101 , $103 ,
: keep-pose ( kind -- )   \ Hewie_KeepPose
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

\ ---- his voice (Hewie_MakeSound): not within 10 frames of the last; some only after 40..60;
\ the loud barks (0x65 / 0x66) and growls (0x5D / 0x5E) heard by the others ----
defer noise-make ( loudness room tri -- )   :noname $FFFF 1 noise-make-in ; is noise-make   \ Noise_Make (his slot 1)
: soon-after? ( snd t -- flag )  h-snd @ rot = h-snd-t @ rot < and ;
: too-soon? ( snd -- flag )
    case
        $59 of  $59 40 soon-after?  endof
        $58 of  $58 50 soon-after?  $70 60 soon-after? or  $6F 40 soon-after? or  endof
        $6F of  $58 50 soon-after?  $70 60 soon-after? or  $6F 40 soon-after? or  endof
        $70 of  $58 50 soon-after?  $70 60 soon-after? or  $6F 40 soon-after? or  endof
        >r false r>
    endcase ;
: make-sound ( snd -- )
    h-snd @ dup $59 <> over $58 <> and over $70 <> and swap $6F <> and h-snd-t @ 10 < and if
        drop exit
    then
    dup $65 = over $66 = or if  $80 h-room h-tri noise-make  then
    dup $5D = over $5E = or if  $1B h-room h-tri noise-make  then
    dup too-soon? if  drop exit  then
    him over 5 0 0 h-pos vec@ actor-sound  h-snd !  0 h-snd-t ! ;

\ Hewie_Bark: by his pose when calm (standing 0x1B00, sitting 0x1B01, lying 0x1B02, limping
\ 0x1B05), else by his mood (2: 0x1B04, else 0x1B03); restarted when already barking
: play-again ( anim -- )  dup anim@ <> if  play  else  play-cut  then ;
: bark ( -- )
    game-mode @ 0= h-action @ $A <> and h-action @ $B <> and if
        h-cond 1 = if  $1B05 play-again exit  then
        anim-group dup 6 = over 2 = or if  drop $1B02
        else dup 5 = swap 1 = or if  $1B01  else  $1B00  then then  play-again
    else
        h-mood @ 2 = if  $1B04  else  $1B03  then  play-again
    then ;

\ ---- whom he is with (in_his_room / Hewie_WithChar) ----
\ active, not down, in his room, and in the room being played on the mesh
: with? ( cs -- flag )
    dup c-active? 0= if  drop false exit  then
    dup c-cond 2 = if  drop false exit  then
    dup c-room h-room <> if  drop false exit  then
    h-room played-room <> if  drop true exit  then
    c-tri 0< 0= ;
