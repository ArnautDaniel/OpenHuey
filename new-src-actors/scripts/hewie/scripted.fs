\ hewie/scripted.fs - the story's moves for Hewie (H4: Hewie_Requests turns a move into actions
\ 0x3B..0x47, 0x7F, carried out by these behaviours; his-done is the original's +0xE1).
IN: hewie.scripted
USING: engine actors common paths hewie.state hewie.body hewie.model hewie.moving hewie.states ;

: done! ( -- )  -1 his-done ! ;

\ Hewie_State1D88: idle under a script - standing, then done (down: lying)
: st-1d88 ( -- )
    his-cond @ 2 = if  10 step-to-pose 0= if  done!  then  exit  then
    settled? 0= if  exit  then
    anim-group dup 10 = over 9 = or over 8 = or if  drop -1 stand-anim exit  then
    0= if  done!  else  0 keep-pose  then ;
\ Hewie_StateRootMotion (0x3B..0x3E, 0x47): the animation's root motion moves him (0x47: not at
\ all; through blocked floor - his-2b - its height too)
: st-root-motion ( -- )
    his-action @ $47 = if  1 his-no-root !  exit  then
    his-2b @ if
        his-at 4 + sf@  root@  rm-x f@ rm-z f@ body-move-any  rm-y f@ f+ his-y!
        1 his-no-root !  0 his-root-ok !
    then ;
\ Hewie_StateWalkPath (0x41 / 0x42): along the way to his-to by his gait (7 / 9) or his-to-anim;
\ at the end the rest of the step, done
: st-walk-path ( -- )
    settled? if
        his-to-anim @ -1 <> if  his-to-anim @ play-if-not
        else  his-action @ $41 = if  7  else  9  then  keep-pose  then
    then
    rest                                                              ( F: rest )
    his-to-tri @  his-to -1 body-tri-to <> if
        his-action @ $3F = if  10e  else  20e  then  ahead@ drop  ahead heading-to
    else  his-to heading-to  then                                     ( F: rest a )
    fdup head-toward-soon  run-turn body-turn-toward                  ( F: rest left )
    stride-len fdup f0< if  fdrop fdrop 0e  else  fswap pi fswap f- pi f/ f*  then   ( F: rest mv )
    ahead@ his-path path-i!  ahead vec@ his-place  1 his-no-root !
    path-done? if
        stride-len fswap fover fover f> if  f- 0e fswap body-move-local  else  fdrop fdrop  then
        done!  to-default exit
    then  fdrop ;
\ Hewie_State2138: his way to the triangle his-to-tri's middle, then walk it
: st-2138 ( -- )
    slide-root
    his-to-tri @ dup 0< if  drop to-default exit  then  tri-center his-to vec!
    his-to-tri @ his-to plan-to if  4 look!  ['] st-walk-path behave  else  to-default  then ;
\ Hewie_StateTurnStart / Turning / State1C98 (0x43 / 0x44): once stopped, turning on the spot
\ (0x1300, eased) to his-to-yaw; then standing again, done
: st-1c98 ( -- )
    settled? 0= if  exit  then
    anim@ $1300 = if  his-danger @ 0= if  0  else  3  then  5 play-blend  1e rate!  exit  then
    done!  to-default ;
: st-turning ( -- )
    1 his-no-root !  0e his-look-pitch f!  his-to-yaw f@ head-toward
    his-to-yaw f@ his-yaw f- angle-wrap fabs pi f* his-heading f@ f/ fsin            ( F: s )
    fdup his-heading f@ f* f2* pi f/ 0.6e fmin rate!
    his-heading f@ f* 0.05e f* 0.5e deg>rad fmax                                    ( F: step )
    his-to-yaw f@ fswap body-turn-toward f0= if  ['] st-1c98 behave  then ;
: st-turn-start ( -- )
    1 his-no-root !
    settled? 0= if  exit  then
    his-to-yaw f@ his-yaw f- angle-wrap fabs
    fdup 1e deg>rad f< if  fdrop his-to-yaw f@ his-yaw!  ['] st-1c98 behave exit  then
    $1300 play  0e rate!
    1.01e f* his-heading f!  ['] st-turning behave ;
\ Hewie_StateHeadForSpot / Wander (0x46): to the spot his-to (gait 7, head on it); there, milling about it
defer st-wander
: st-head-for-spot ( -- )
    his-at his-to vec-dist-xz 10e f< if
        0 his-t1 !  0 his-t2 !  his-yaw his-heading f!  ['] st-wander behave exit
    then
    his-to heading-to  run-turn turn-to                              ( F: left )
    his-to look-at his-look-yaw f! his-look-pitch f!  8 look-now!
    1.0471976e f<= if  0 step-to-pose 0= if  done!  then  else  7 keep-pose  then ;
:noname ( -- )   \ Hewie_StateWander
    his-at his-to vec-dist-xz 15e f> if  ['] st-head-for-spot behave exit  then
    his-t2 @ if  -1 his-t2 +!
    else
        his-t1 @ if  -1 his-t1 +!  then
        his-heading f@ 20e free-ahead 20e f<  his-t1 @ 0= or if
            3 roll 30 * 30 + his-t1 !  30 his-t2 !
            his-to heading-to pi f+ angle-wrap  30e 30 150 30 best-heading his-heading f!
        then
    then
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop  7 keep-pose ;
is st-wander
\ the scripts' bark (0x45: Hewie_State2168 / 1CC8 / 1C48)
: st-1cc8 ( -- )  3 step-to-pose 0= if  done!  3 keep-pose  to-default  then ;
: st-1c48 ( -- )  0 step-to-pose 0= if  done!  0 keep-pose  to-default  then ;
: st-2168 ( -- )
    his-danger @ 0= his-cond @ 1 <> and if
        3 step-to-pose 0= if  bark  ['] st-1cc8 behave  then
    else  0 step-to-pose 0= if  bark  ['] st-1c48 behave  then  then ;

\ ---- the leap (0x7F: Hewie_StateRunForSpot / Leap / TurnTo) ----
fvariable lx  fvariable lz  fvariable ll
create lp-dir 12 allot  fvariable lp-y  fvariable lp-fall  fvariable lp-rise
: st-turn-to ( -- )
    his-to-yaw f@ his-yaw f- angle-wrap  fdup fabs 30e deg>rad f<  his-head-yaw f@ fover f* f0< 0= or if
        fdrop his-to-yaw f@ 10e deg>rad body-turn-toward fdrop
    else  f0< if  his-yaw 10e deg>rad f+  else  his-yaw 10e deg>rad f-  then  angle-wrap his-yaw!  then
    anim-done? if
        his-t2 @ 0= if  0 his-2d !  then
        -1 stand-anim  ['] st-1c48 behave
    then
    root@  lp-dir vec@ rm-z f@ f* frot rm-z f@ f* frot frot  fswap fdrop  body-move-any
    1 his-no-root ! ;
: st-leap ( -- )
    his-to-yaw f@ 10e deg>rad turn-to fdrop
    anim-done? anim@ $1E05 <> and if  $1E05 play-cut  1.2889e lp-fall f!  then
    anim@ $1E04 = if
        root@  lp-dir vec@ rm-z f@ f* frot rm-z f@ f* frot frot fswap fdrop  body-move-any
        lp-y f@ rm-y f@ lp-rise f@ f* f+                                   ( F: y )
    else
        lp-dir vec@ 3e f* frot 3e f* frot frot fswap fdrop  body-move-any
        lp-fall f@ 0.5e f+ 3e fmin lp-fall f!
        lp-y f@ lp-fall f@ f-
    then
    1 his-no-root !
    his-at 4 + sf@ fover f> if   \ (below the ground: landed)
        fdrop $1E06 play-cut  ['] st-turn-to behave exit
    then
    fdup lp-y f!  his-y! ;
: st-run-for-spot ( -- )
    his-to-tri @ his-to plan-and-go if  to-default exit  then
    settled? rest his-to-anim @ s>f f< and if
        his-to sf@ his-at sf@ f- lx f!  his-to 8 + sf@ his-at 8 + sf@ f- lz f!
        lx f@ fsq lz f@ fsq f+ fsqrt  fdup f0= if  fdrop 1e  then  ll f!
        lx f@ ll f@ f/  0e  lz f@ ll f@ f/  lp-dir vec!
        his-at 4 + sf@ lp-y f!
        his-to heading-to fdup his-to-yaw f!  20e deg>rad turn-to fdrop
        $1E04 play  1 his-2d !  1 his-no-root !
        ['] st-leap behave exit
    then
    5 step-to-pose 0= if  $202 play-if-not  then
    stride 0= if  8 keep-pose  then ;

\ ---- Hewie_Requests: a move given, as the action that carries it out ----
: move-action ( kind -- act | -1 )
    case
        1 of  0  endof  2 of  0  endof  3 of  0  endof  4 of  0  endof
        5 of  $3F  endof  6 of  $41  endof  7 of  $3B  endof  8 of  $3C  endof  9 of  $3D  endof
        10 of  $40  endof  11 of  $42  endof  14 of  $44  endof  15 of  $44  endof  16 of  $3E  endof
        18 of  $45  endof  19 of  $7F  endof  20 of  $46  endof  21 of  $47  endof
        >r -1 r>
    endcase ;
