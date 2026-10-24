\ hewie/steered.fs - Hewie played: the mini-game's run (hewie-played?, the original's progress
\ +0x1FBEC1). The stick runs him, camera-relative as it walks her (Hewie_LeftStick): after a
\ camera cut the old camera keeps steering while the stick is held - three frames for sure, then
\ until it moves - and then he turns on the spot toward the old camera's heading while the stick
\ holds within 15 degrees; cross held, he runs flat out (Hewie_StateSteered). The right stick's
\ gestures (keys.fs gesture@) are his to bark with, and the story reads them (the scripts' 0x21).
\ His other moves in the run: lying flat until the pad is touched (steered-2-act); a run-up and a
\ leap where he would have gone for the stalker and couldn't (run-up-jump-act).
IN: hewie.steered
USING: engine game-state actors messages common facts keys vectors hewie-names hewie.state hewie.body hewie.anims hewie.sounds hewie.model hewie.moving hewie.pounce hewie.exits floor-flags ;

\ ---- the stick (Hewie_LeftStick) ----
\ how the stick steers him: the camera now; the old camera after a cut; turning round to the old
\ camera's heading; afresh (he went through a door: Hewie_Vt34)
enum: steering  by-camera-now cut-steered turning-round steer-afresh ;
\ the way taken this frame: by the camera now, his own facing, none, the steering camera
enum: way-from  from-camera from-facing from-none from-steering ;
: small? ( F: x z -- flag )  fabs 0.5e f<= fabs 0.5e f<= and ;
: unit ( F: x z -- x' z' )  {: F: x F: z :}  x z vlen f0> if  x x z vlen f/  z x z vlen f/  else  x z  then ;
: dir! ( F: x z -- )  his-dir-z f!  his-dir-x f! ;
: steered-way ( F: yaw x z -- )  stick-turned dir! ;   \ (the way turned into the room by a camera heading)
: read-steering ( -- )
    0 his-sprint !
    stick@  {: F: ex F: ez :}  ex ez unit  {: F: nx F: nz :}
    his-steer @ steer-afresh =  his-cam-cut @ or  0 his-cam-cut !  if   \ (a cut: the old camera steers while the stick is held)
        by-camera-now his-steer !  0 his-still !
        nx fabs 0.5e f>  nz fabs 0.5e f> or if  3 his-lock !  cut-steered his-steer !  then
    then
    ex ez small? dup if  his-still @ 1+ 6 min his-still !  else  0 his-still !  then  0= {: moving :}
    his-steer @ case
        by-camera-now of  moving if  from-camera  else  settled? if  from-none  else  from-facing  then  then  endof
        cut-steered of
            moving 0= if  his-still @ 6 = if  by-camera-now his-steer !  then  from-none
            else
                his-lock @ if  -1 his-lock +!  his-steer-yaw f@ his-rot-yaw f!
                else
                    ex his-stick-x f@ f-  ez his-stick-z f@ f-  vlen 0.01e f< if   \ (the stick held still: turn round)
                        turning-round his-steer !  0.5e deg>rad his-spin-rate f!  yaw his-rot-yaw f!
                    then
                then
                his-rot-yaw f@ nx nz stick-turned fswap fatan2 his-spin-to f!  from-steering
            then  endof
        turning-round of
            moving if
                nx nz fatan2  his-last-x f@ his-last-z f@ fatan2 f- angle-wrap fabs 15e deg>rad f> if  by-camera-now his-steer !  then
                from-camera
            else  his-still @ 6 = if  by-camera-now his-steer !  then  from-none  then  endof
        >r from-camera r>
    endcase
    case
        from-steering of  his-steer-yaw f@ nx nz steered-way  endof
        from-none of  0e 0e dir!  endof
        from-facing of  his-yaw fdup fsin fswap fcos dir!  endof
        from-camera of  yaw nx nz steered-way  endof
    endcase
    ex his-stick-x f!  ez his-stick-z f!
    his-steer @ by-camera-now = if  yaw his-steer-yaw f!  then
    his-steer @ turning-round <> if  nx his-last-x f!  nz his-last-z f!  then
    cross button-down? if  1 his-sprint !  then ;

\ ---- run by the stick (steered-act: Hewie_StateSteered) ----
: st-steered ( -- )
    read-steering
    his-still @ if  ahead-look look!  resting-or-low-pose keep-pose exit  then
    his-steer @ turning-round = if
        his-spin-to f@ head-toward-soon
        his-spin-rate f@ 0.075e deg>rad f+ 3e deg>rad fmin his-spin-rate f!
        his-spin-to f@ his-spin-rate f@ turn-to his-spin-rate f@ f< if  by-camera-now his-steer !  then
    else
        his-dir-x f@ his-dir-z f@ fatan2  fdup head-toward-soon  run-turn turn-to fdrop
    then
    his-sprint @ if  run-pose  else  trot-pose  then  keep-pose ;
\ played again (Hewie_StartScene in the run): the stick afresh, standing, steered
: steer-afresh! ( -- )
    by-camera-now his-steer !  6 his-still !  0e his-last-x f!  0e his-last-z f!  0 his-lock !
    0e his-dir-x f!  0e his-dir-z f! ;

\ ---- the moves of the run ----
\ moved by his animation's root motion through the floor that would stop him (Hewie_ForcedMove)
: forced-move ( -- )  hewie-floor $80001 invert and body-mask!  slide-root  hewie-floor body-mask! ;
\ up again (Hewie_State1D38): standing, then the default action
: st-getting-up ( -- )  stand-pose step-to-pose 0= if  stand-pose keep-pose  to-default exit  then  forced-move ;
\ steered-2-act (Hewie_State2418): lying flat, down, until the pad is touched
: st-lying-flat ( -- )
    down-pose step-to-pose 0= if
        down-cond cond!  down-pose keep-pose
        steering? if  0 his-2d !  ['] st-getting-up behave  then
    then  forced-move ;
: lie-flat ( -- )  1 his-2d !  free-mode his-mode !  ahead-look look-now!  0 his-cmd !  ['] st-lying-flat behave ;
\ run-up-jump-act (Hewie_StateRunUpJump): running up toward the way his head was turned, head
\ level; once into his run on floor that allows it, the leap - 20 ahead, 5 up, 2.8 a frame over 8
\ frames (hewie/pounce.fs's tackle)
: st-run-up-jump ( -- )
    as-is-pose step-to-pose 0= anim@ run-anim <> and if  run-anim play  then
    his-spin-to f@ head-toward  his-spin-to f@ run-turn body-turn-toward fdrop
    settled? 0= anim@ run-anim <> or if  exit  then
    his-tri nav-flags slope-floor and if  to-default exit  then
    ahead-look look!
    his-at v.y sf@ 5e f+ his-pounce-top f!  his-at v.y sf@ his-pounce-y f!  his-spin-to f@ his-pounce-yaw f!
    1 his-t1 !  8 his-t2 !  0 his-t3 !
    his-spin-to f@ 30e deg>rad turn-to fdrop
    tackle-anim play  shut-out  1 his-2d !  leap-snarl make-sound  new-bite
    ['] st-tackle behave ;
: run-up-jump ( -- )
    engaged-mode his-mode !  his-yaw his-head-yaw f@ f+ angle-wrap his-spin-to f!  0 his-cmd !
    ['] st-run-up-jump behave ;

\ ---- each frame in the run (Hewie_Update, Hewie_Think): the gesture read; any one, with what he
\ does open to her commands and nothing scripting him, a bark (Hewie_StateBlock's end) ----
: gesture! ( -- )  hewie-played? if  gesture@  else  -1  then  his-gesture ! ;
: gesture-bark ( -- )
    his-gesture @ 0< his-busy @ or if  exit  then
    his-mode @ free-mode <> if  exit  then
    his-cmd @ dup 0= swap nothing-breaks-in and or if  exit  then
    bark-act 0 set-action ;

\ ---- his own choices in the run (Hewie_Update, in place of Hewie_OwnDecisions): down while
\ steered, knocked down; in the sweep of a door swinging, off it to the exit's far point ----
: run-decisions ( -- )
    his-cond @ down-cond =  his-hp @ 0= and  his-action @ steered-act = and if  knocked-down-act 0 set-action  then
    8 0 do
        i door-here?  i door-animating? and  i on-its-sides? and if
            i 2 exit-spot drop his-spot vec!  back-on-floor-act 0 set-action
        then
    loop ;
\ his mode's behaviour in the run (Hewie_StatePending, in place of calm / wary / tense): asked
\ to choose, steered again
: st-pending ( -- )  his-pending @  his-action @ steered-act <> and if  steered-act 0 set-action  then ;
\ played afresh (Hewie_StartScene in the run: the clock's start, Hewie_Restart): the stick afresh,
\ standing, steered
: played-afresh ( -- )
    steer-afresh!  ['] st-pending his-mood-act !  stand-anim play-cut  steered-act 0 set-action ;
\ out of the room by `exit`, played (Hewie_Vt34 in the run): free of the floor's holds, the stick
\ afresh in the next room
\ into the next room, played (the rooms' change carries him): at the exit he came in by
: played-arrive ( exit -- )
    dup his-door !  dup 0< if  drop exit  then
    0 exit-spot dup 0< if  drop fdrop fdrop fdrop exit  then  room-id swap body-place ;
: played-through ( -- )
    0 his-2b !  his-action @ steered-2-act <> if  0 his-2d !  then  steer-afresh his-steer ! ;
