\ fiona/core.fs - Fiona (the original's gCharPlayer, src/game/fiona.c): her own state, her
\ animations and their looks, her fear. Words keep the original's names in their comments
\ (Fiona_*) so the two read side by side; the offsets there are her fields in the original.
\
\ She is character slot 0; Hewie slot 1; the pursuer the slot in `pursuer` (partner.core's).
IN: fiona.core
USING: engine game-state events.core events.words chars ;

0 constant me
1 constant dog
variable pursuer-slot  -1 pursuer-slot !   \ gCharPursuer's slot (-1 none; the stalkers set it)

: vector ( "name" -- )  create 12 allot ;
\ ---- chance (gRandom +0x1C: 0 <= r < 1) ----
: rnd ( F: -- r )  32768 random s>f 32768e f/ ;
: f-wrap-abs ( F: a -- |a| )  angle-wrap fabs ;

\ ---- her fields ----
variable f-act        \ +0x1AD580: what she is busy with (0 free, 1 pushing, 2 a ladder, 3 a door,
                      \ 5 a shove, 6 a kick, 7 letting go, 8 caught, 9 a scripted door, $A a panic
                      \ stumble, $C a command, $D fleeing, $E a panic attack, $F out of breath,
                      \ $10 turning for a request, $11.. scripted moves)
variable f-fear-bits  \ +0x1AD584: bit 0 frightened (the panic's stage 2+), 1 panicking (stage 4)
variable f-turn-mode  \ +0x1AD588: 0 free, 1 the old camera steers, 2 held, 3 reset (a cut)
variable f-still      \ +0x1AD58C: frames without the stick (up to 6; 0 held)
variable f-lock       \ +0x1AD58A: frames the old camera still steers
vector f-dir          \ +0x1AD550: where to move (world, unit or 0)
vector f-stick        \ +0x1AD590: last frame's raw input
vector f-last         \ +0x1AD5A0: last frame's input, normalized
fvariable f-camyaw    \ +0x1AD5B0: the camera heading the controls go by
fvariable f-turn-rate \ +0x1AD5B4: the accelerating turn's step
fvariable f-yaw-was   \ +0x1AD5B8: her heading the frame before
variable f-go         \ +0x1AD5D8: the run button
fvariable f-heading   \ +0x1AD5E0 (savedYaw): the heading she turns to
fvariable f-turn-to   \ +0x1AD5E4: the accelerating turn's goal
variable f-rest       \ +0x1AD5C0: frames standing (the rest animation at 90)
variable f-run-t      \ +0x1AD5C4: frames running
variable f-busy-t     \ +0x1AD5C8: frames the pad isn't read (after a call or a command)
fvariable f-tired-w   \ +0x1AD5CC: the out-of-breath idle's weight
fvariable f-fear      \ +0x1AD5F4: her fear, 0..99 (100: a panic attack)
variable f-recovery   \ +0x1AD5F8: frames still shaken (of 1800)
variable f-panic-t    \ +0x1AD5E8: frames of the panic run left
variable f-stumble-t  \ +0x1AD5EC
variable f-door-t     \ +0x1AD5F0
variable f-freeze-t   \ +0x14D0: frames held still by a blow (Motion_Freeze)
fvariable f-stick-k   \ +0x1AD624: the root motion's share by the stick (eases off when released)
fvariable f-blend-w   \ +0x1AD628: the walk's blend last set
variable f-look-on    \ +0x1AD5FC: looking at a character / point
variable f-look-who   \ +0x1AD600: whom (a slot; -1 a point)
vector f-look-pt      \ +0x1AD610: the point (its head)
variable f-look-hold  \ +0x1AD620: frames to keep looking
variable f-look-cmd   \ +0x1AD664: the one a command looked at (-1 none)
variable f-cmd        \ +0x1AD6B8: the command her controls give this frame (-1 none)
variable f-cmd-code   \ +0x1AD6BC
variable f-dog-here  variable f-dog-ok      \ +0x1AD5D4 / D5: Hewie in the game, and not hidden
variable f-pu-here   variable f-pu-ok       \ +0x1AD5D6 / D7: the pursuer
variable f-step-l  variable f-step-r  variable f-steps   \ +0x1AD5D0 / D1 / DC: her feet
variable f-cam-on                            \ +0x1AD5BC: her feet fitted to the floor
variable f-2a  variable f-2b  variable f-2d  \ the actor's +0x2A / +0x2B / +0x2D
variable f-e1                                \ +0xE1: a scripted move done
variable f-target   -1 f-target !            \ +0x1AD600 target: the one she deals with
variable f-target-t                          \ +0x1AD620 targetParam
variable f-state                             \ a.state: her behaviour, run each frame (an xt)
vector f-prev  variable f-prev-tri           \ +0x40 / +0x38: where she was as the frame began

: f-reset-fields ( -- )
    0 f-act !  0 f-fear-bits !  0 f-turn-mode !  6 f-still !  0 f-lock !
    f-dir 12 0 fill  f-stick 12 0 fill  f-last 12 0 fill
    0 f-rest !  0 f-run-t !  0 f-busy-t !  1e f-tired-w f!  0e f-fear f!  0 f-recovery !
    0 f-panic-t !  0 f-stumble-t !  0 f-door-t !  0 f-freeze-t !  1e f-stick-k f!  0e f-blend-w f!
    0 f-look-on !  -1 f-look-who !  0 f-look-hold !  -1 f-look-cmd !  -1 f-cmd !  -1 f-cmd-code !
    1 f-step-l !  1 f-step-r !  0 f-steps !  0 f-2a !  0 f-2b !  0 f-2d !  0 f-e1 !
    -1 f-target !  0 f-target-t ! ;
f-reset-fields
: behave ( xt -- )  f-state ! ;   \ Actor_SetState

\ ---- her body (chars) ----
: f-actor ( -- a )  me c-actor ;
: f-pos ( -- v )  me c-pos ;
: f-yaw ( F: -- a )  me c-yaw ;
: f-yaw! ( F: a -- )  me c-yaw! ;
: f-tri ( -- tri )  me c-tri ;
: f-room ( -- room )  me character char.room sl@ ;
: f-mode ( -- n )  me character char.mode sl@ ;       \ +0xF8 moveMode
: f-mode! ( n -- )  me character char.mode l! ;
: f-sub ( -- n )  me character char.sub sl@ ;        \ +0xFC moveSub
: f-sub! ( n -- )  me character char.sub l! ;
: f-busy? ( -- flag )  me character char.scripted sl@ 0<> ;   \ +0xE0
: f-disabled? ( -- flag )  me character char.disabled sl@ 0<> ;
: f-turn-toward ( F: a step -- left )  me c-turn-toward ;   \ Actor_TurnToward

\ ---- her animations (her motion: Motion_Play*, the variant and its weight +0x6A4 +0x1C) ----
variable f-var  -1 f-var !            \ the variant playing (+0x560), -1 none
: f-anim@ ( -- id )  f-actor dup 0< if  exit  then  motion@ ;
: f-rate-1 ( a -- )  actor 1e act.rate sf! ;
\ Motion_Start with a variant: the motion, blended over `blend`, with `flags`
variable fs-a  variable fs-v  variable fs-b  variable fs-f
: f-start ( anim variant blend flags -- )
    fs-f !  fs-b !  fs-v !  fs-a !
    f-actor dup 0< if  drop exit  then
    dup fs-a @ has-motion? 0= if  drop exit  then   \ (one her model lacks: nothing)
    dup f-rate-1  dup fs-a @ fs-b @ fs-f @ motion-play
    fs-v @ dup f-var !  motion-variant ;
\ the table's fade and flags for an animation (0s: none)
: f-entry ( anim -- blend flags )  f-actor dup 0< if  2drop 0 0 exit  then  swap motion-entry nip ;
\ Motion_PlayTable: with the table's fade and flags (an animation not in it cuts in)
: f-play-table ( anim variant -- )  over f-entry f-start ;
\ Motion_Play: the table's flags, cut in
: f-play ( anim variant -- )  over f-entry nip 0 swap f-start ;
\ Motion_PlayBlend: over `n` frames
: f-play-blend ( anim n variant -- )  2 pick f-entry nip >r swap r> f-start ;
\ Motion_PlayBlend8: blended over `n` frames, with no time of its own
: f-play-blend8 ( anim n -- )  over f-entry nip 8 or >r -1 swap r> f-start ;
\ the weight of the motion against its variant (+0x6A4 +0x1C)
: f-weight! ( F: w -- )  f-actor dup 0< if  drop fdrop exit  then  actor act.vweight sf! ;
\ the blend into it is over (+0x550 <= 0)
: f-settled? ( -- flag )  f-actor dup 0< if  drop true exit  then  actor act.fade sf@ 0e f<= ;
\ it came to its end this frame (its key flag 0x20)
: f-end? ( -- flag )  f-actor dup 0< if  exit  then  actor act.mflags l@ $20 and 0<> ;
\ Motion_EventFlags(m, 0, 0, 1): the event keys at this frame
: f-events ( -- bits )  f-actor dup 0< if  drop 0 exit  then  0 0 1 motion-events ;
: f-events-at ( dt -- bits )  f-actor dup 0< if  2drop 0 exit  then  0 rot 1 motion-events ;

\ Fiona_AnimGroup / fiona_motion_kind: the group of an animation (0 standing, 1 walking, 2
\ running, 3 out of breath, 4 0xB01, 5 resting, 6 0x12xx, 7 a ladder, 8 its top, 9 0x403, 10
\ 0xE01, 11 anything else)
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
: f-group ( -- g )  f-anim@ anim-group ;

\ ---- her fear (Fiona_ChangeFear: the charm she wears - the sub screen's slot 3 - scales it;
\ 0x8A less gain, 0x8B less gain and more loss, 0x8C no gain and double loss, 0x8D none) ----
variable charm   \ (the worn accessory, the sub screen's slot 3: with the sub screen)
variable worn    \ (the sub screen's slot 0: 0x80 her soft shoes)
: pvar@ ( n -- v )  progress pr.vars + c@ ;   \ Progress_GetVar
: change-fear ( F: d -- )
    charm @ case
        $8C of  fdup f0> if  fdrop 0e  else  2e f*  then  endof
        $8B of  fdup f0> if  0.75e f*  else  1.5e f*  then  endof
        $8A of  fdup f0> if  0.75e f*  then  endof
    endcase
    f-fear f@ f+ 0e fmax 99e fmin f-fear f! ;
\ Fiona_CalmDown: by n/30
: calm-down ( n -- )  s>f 0.0333333e f* fnegate change-fear ;
\ Fiona_LowerRecovery: 60 / n frames off, not below 0
: lower-recovery ( n -- )  60 swap / negate f-recovery +!  f-recovery @ 0 max f-recovery ! ;

\ ---- her looks: standing, walking, running by how she feels ----
: idle-base ( -- anim )  game-mode @ 2 = if  5  else  0  then ;
\ whether a weight moved more than 0.1 from the last one set (+0x1AD628)
: weight-moved? ( F: w -- flag )  f-blend-w f@ f- fabs 0.1e f> ;
fvariable fl-a  fvariable fl-b
: feel ( -- )   \ a: (100 - fear) / 60, b: (1800 - recovery) / 1800
    100e f-fear f@ f- 60e f/ fl-a f!  1800 f-recovery @ - s>f 1800e f/ fl-b f! ;
\ fiona_idle: animation `anim` with variant `var`, blended over `blend` (-1 at once)
: idle-play ( anim blend var -- )
    over -1 = if  nip f-play-table  else  f-play-blend  then ;
\ Fiona_IdleAnim: out of breath (bit 0: variant 4, its weight eased toward the panic's, the
\ progress +0x7BC), shaken (variant 2 by her fear) or frightened (variant 3 by her recovery)
fvariable panic-level   \ (progress +0x7BC: the panic's level, 0..100)
: idle-anim ( blend -- )
    f-mode 0= if  0 f-sub!  then
    f-fear-bits @ 1 and if
        f-anim@ idle-base = f-var @ 4 = and 0= if  idle-base over 4 idle-play  then  drop
        90e panic-level f@ f- 15e f/ 0e fmax                      ( F: t )
        f-tired-w f@ fover f< if  f-tired-w f@ 0.05e f+ fmin  else  f-tired-w f@ 0.05e f- fmax  then
        fdup f-tired-w f!  f-weight! exit
    then
    feel
    fl-b f@ 0.5e f< 0=  fl-b f@ fl-a f@ 0.25e f+ f>= and if
        fl-a f@ 1e f< if
            f-anim@ idle-base = f-var @ 2 = and 0=  fl-a f@ weight-moved? or if  idle-base over 2 idle-play  then
            drop fl-a f@ f-weight!  fl-a f@ f-blend-w f!
        else
            f-anim@ idle-base = f-var @ -1 = and 0= if  idle-base over -1 idle-play  then
            drop 1e f-weight!
        then  exit
    then
    f-anim@ idle-base = f-var @ 3 = and 0=  fl-b f@ weight-moved? or if  idle-base over 3 idle-play  then
    drop fl-b f@ f-weight!  fl-b f@ f-blend-w f! ;

\ Fiona_WalkLook / Fiona_RunLook: the walk (0x200, the chase 0x208) or run (0x202) alone, or
\ blended with its frightened (0x204 / 0x205) or shaken (0x201 / 0x203) variant by those
\ amounts; a blend set again only when it moves more than 0.1. Panicking: the panic run 0x206.
variable lk-base  variable lk-scared  variable lk-shaken
: look-blend ( F: w -- ) ( var -- )
    f-anim@ lk-base @ = f-var @ 2 pick = and if  fdup weight-moved? 0= if  drop  else  lk-base @ swap f-play-table  then
    else  lk-base @ swap f-play-table  then
    fdup f-weight!  f-blend-w f! ;
: move-look ( -- )
    feel
    fl-b f@ 0.5e f<  fl-b f@ fl-a f@ 0.25e f+ f< or if  fl-b f@ lk-scared @ look-blend exit  then
    fl-a f@ 1e f< if  fl-a f@ lk-shaken @ look-blend exit  then
    f-anim@ lk-base @ = f-var @ -1 = and 0= if  lk-base @ -1 f-play-table  then  1e f-weight! ;
: walk-look ( -- )
    f-mode 0= if  1 f-sub!  then
    game-mode @ 2 = if  $208  else  $200  then  lk-base !  $204 lk-scared !  $201 lk-shaken !  move-look ;
: run-look ( -- )
    f-mode 0= if  2 f-sub!  then
    f-fear-bits @ 2 and if  f-anim@ $206 <> if  $206 -1 f-play-table  1e f-weight!  then  exit  then
    $202 lk-base !  $205 lk-scared !  $203 lk-shaken !  move-look ;

\ Fiona_Stand
: stand ( -- )  0 f-rest !  -1 idle-anim ;
\ Fiona_Exhausted: out of breath from the panic run (0x207), a new run length
: exhausted ( -- )  3e rnd f* f>s 30 * 120 + f-panic-t !  $F f-act !  $207 -1 f-play-table ;

\ ---- the states to go back to (fiona.states fills them in) ----
defer idle-move-state   ' noop is idle-move-state   \ Fiona_StateIdleMove
defer idle-step-state   ' noop is idle-step-state   \ Fiona_StateIdleStep
\ Fiona_ToIdle: back to the idle state
: to-idle ( -- )
    0 f-act !  0 f-mode!  f-yaw f-heading f!  0 f-rest !  0 f-turn-mode !
    f-busy? 0= if  0 f-2d !  -1 idle-anim  ['] idle-move-state behave
    else  ['] idle-step-state behave  then
    $2B state-flag-clear ;
