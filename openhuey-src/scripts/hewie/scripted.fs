\ hewie/scripted.fs - the story's moves for Hewie (Hewie_Requests turns a move into one of the
\ story-..-act actions, carried out by these behaviours; his-done is the original's +0xE1).
IN: hewie.scripted
USING: engine actors common paths vectors messages hewie-names hewie.state hewie.body hewie.anims hewie.model hewie.moving hewie.states ;

: done! ( -- )  -1 his-done ! ;

\ Hewie_State1D88: idle under a script - standing, then done (down: lying)
: st-scripted-idle ( -- )
    his-cond @ down-cond = if  down-pose step-to-pose 0= if  done!  then  exit  then
    settled? 0= if  exit  then
    anim-group dup in-walking = over in-trotting = or over in-running = or if  drop -1 stand-still exit  then
    in-standing = if  done!  else  stand-pose keep-pose  then ;
\ Hewie_StateRootMotion (story-anim-act .. story-stand-act, story-anim-still-act): the
\ animation's root motion moves him (not at all while still; through blocked floor - his-2b -
\ its height too)
: root-rise ( -- )   \ (the step, and the height, over anything)
    his-at v.y sf@  root@ {: F: y turn x up z :}  x z body-move-any  y up f+ his-y! ;
: st-root-motion ( -- )
    his-action @ story-anim-still-act = if  1 his-no-root !  exit  then
    his-2b @ if  root-rise  1 his-no-root !  0 his-root-ok !  then ;
\ Hewie_StateWalkPath (story-path-act / story-path-run-act): along the way to his-to by his gait
\ or his-to-anim; at the end the rest of the step, done
: st-walk-path ( -- )
    settled? if
        his-to-anim @ -1 <> if  his-to-anim @ play-if-not
        else  his-action @ story-path-act = if  walk-pose  else  run-pose  then  keep-pose  then
    then
    rest                                                              ( F: rest )
    his-to-tri @  his-to -1 body-tri-to <> if
        his-action @ story-walk-act = if  10e  else  20e  then  ahead@ drop  ahead heading-to
    else  his-to heading-to  then                                     ( F: rest a )
    fdup head-toward-soon  run-turn body-turn-toward                  ( F: rest left )
    stride-len fdup f0< if  fdrop fdrop 0e  else  fswap pi fswap f- pi f/ f*  then   ( F: rest mv )
    ahead@ his-path path-i!  ahead vec@ his-place  1 his-no-root !
    path-done? if
        stride-len fswap fover fover f> if  f- 0e fswap body-move-local  else  fdrop fdrop  then
        done!  to-default exit
    then  fdrop ;
\ Hewie_State2138: his way to the triangle his-to-tri's middle, then walk it
: st-path-to-tri ( -- )
    slide-root
    his-to-tri @ dup 0< if  drop to-default exit  then  tri-center his-to vec!
    his-to-tri @ his-to plan-to if  ahead-look look!  ['] st-walk-path behave  else  to-default  then ;
\ Hewie_StateTurnStart / Turning / State1C98 (story-turn-2-act): once stopped, turning on the spot
\ (eased) to his-to-yaw; then standing again, done
: st-turned ( -- )   \ (Hewie_State1C98)
    settled? 0= if  exit  then
    anim@ turn-on-spot-anim = if  his-danger @ calm = if  stand-anim  else  alert-anim  then  5 play-blend  1e rate!  exit  then
    done!  to-default ;
: st-turning ( -- )
    1 his-no-root !  0e his-look-pitch f!  his-to-yaw f@ head-toward
    his-to-yaw f@ his-yaw f- angle-wrap fabs pi f* his-heading f@ f/ fsin            ( F: s )
    fdup his-heading f@ f* f2* pi f/ 0.6e fmin rate!
    his-heading f@ f* 0.05e f* 0.5e deg>rad fmax                                    ( F: step )
    his-to-yaw f@ fswap body-turn-toward f0= if  ['] st-turned behave  then ;
: st-turn-start ( -- )
    1 his-no-root !
    settled? 0= if  exit  then
    his-to-yaw f@ his-yaw f- angle-wrap fabs
    fdup 1e deg>rad f< if  fdrop his-to-yaw f@ his-yaw!  ['] st-turned behave exit  then
    turn-on-spot-anim play  0e rate!
    1.01e f* his-heading f!  ['] st-turning behave ;
\ Hewie_StateHeadForSpot / Wander (story-spot-act): to the spot his-to (walking, head on it);
\ there, milling about it
defer st-wander
: st-head-for-spot ( -- )
    his-at his-to vec-dist-xz 10e f< if
        0 his-t1 !  0 his-t2 !  his-yaw his-heading f!  ['] st-wander behave exit
    then
    his-to heading-to  run-turn turn-to                              ( F: left )
    his-to look-at his-look-yaw f! his-look-pitch f!  held-look look-now!
    60e deg>rad f<= if  stand-pose step-to-pose 0= if  done!  then  else  walk-pose keep-pose  then ;
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
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop  walk-pose keep-pose ;
is st-wander
\ the scripts' bark (story-bark-act: Hewie_State2168 / 1CC8 / 1C48): low when calm, then done
: st-barked-low ( -- )  resting-pose step-to-pose 0= if  done!  resting-pose keep-pose  to-default  then ;
: st-barked ( -- )  stand-pose step-to-pose 0= if  done!  stand-pose keep-pose  to-default  then ;
: st-story-bark ( -- )
    his-danger @ calm = his-cond @ hurt-cond <> and if
        resting-pose step-to-pose 0= if  bark  ['] st-barked-low behave  then
    else  stand-pose step-to-pose 0= if  bark  ['] st-barked behave  then  then ;

\ ---- the leap (story-leap-act: Hewie_StateRunForSpot / Leap / TurnTo) ----
: along-leap ( F: d -- )  {: F: d :}  his-jump-dir v.x sf@ d f*  his-jump-dir v.z sf@ d f*  body-move-any ;   \ (along the leap's way)
: root-up ( F: -- up )  root@ {: F: turn x y z :}  y ;
: st-turn-to ( -- )   \ (landed: round to his-to-yaw, carried on by the landing's step)
    his-to-yaw f@ his-yaw f- angle-wrap  fdup fabs 30e deg>rad f<  his-head-yaw f@ fover f* f0< 0= or if
        fdrop his-to-yaw f@ 10e deg>rad body-turn-toward fdrop
    else  f0< if  his-yaw 10e deg>rad f+  else  his-yaw 10e deg>rad f-  then  angle-wrap his-yaw!  then
    anim-done? if
        his-t2 @ 0= if  0 his-2d !  then
        -1 stand-still  ['] st-barked behave
    then
    stride-len along-leap  1 his-no-root ! ;
: st-leap ( -- )
    his-to-yaw f@ 10e deg>rad turn-to fdrop
    anim-done? anim@ leap-fall-anim <> and if  leap-fall-anim play-cut  1.2889e his-jump-fall f!  then
    anim@ leap-up-anim = if   \ (rising by the animation)
        stride-len along-leap
        his-jump-y f@ root-up his-jump-rise f@ f* f+                       ( F: y )
    else   \ (falling: 3 a frame on, faster down)
        3e along-leap
        his-jump-fall f@ 0.5e f+ 3e fmin his-jump-fall f!
        his-jump-y f@ his-jump-fall f@ f-
    then
    1 his-no-root !
    his-at v.y sf@ fover f> if   \ (below the ground: landed)
        fdrop leap-land-anim play-cut  ['] st-turn-to behave exit
    then
    fdup his-jump-y f!  his-y! ;
: aim-leap ( -- )   \ the leap's way: level, toward his-to
    his-to v.x sf@ his-at v.x sf@ f-  his-to v.z sf@ his-at v.z sf@ f-  {: F: dx dz :}
    dx fsq dz fsq f+ fsqrt  fdup f0= if  fdrop 1e  then  {: F: d :}
    dx d f/  0e  dz d f/  his-jump-dir vec! ;
: st-run-for-spot ( -- )
    his-to-tri @ his-to plan-and-go if  to-default exit  then
    settled? rest his-to-anim @ s>f f< and if   \ (within his-to-anim of it: the leap)
        aim-leap  his-at v.y sf@ his-jump-y f!
        his-to heading-to fdup his-to-yaw f!  20e deg>rad turn-to fdrop
        leap-up-anim play  1 his-2d !  1 his-no-root !
        ['] st-leap behave exit
    then
    as-is-pose step-to-pose 0= if  run-anim play-if-not  then
    stride 0= if  trot-pose keep-pose  then ;

\ ---- Hewie_Requests: a move given, as the action that carries it out ----
: move-action ( kind -- act | -1 )
    case
        stand-move of  choose-act  endof  idle-step-move of  choose-act  endof
        open-door-move of  choose-act  endof  shut-door-move of  choose-act  endof
        walk-move of  story-walk-act  endof  run-move of  story-path-act  endof
        anim-move of  story-anim-act  endof  blend-move of  story-blend-act  endof  blend8-move of  story-blend8-act  endof
        walk-path-move of  story-run-act  endof  run-path-move of  story-path-run-act  endof
        turn-to-who-move of  story-turn-act  endof  turn-to-heading-move of  story-turn-2-act  endof
        idle-anim-move of  story-stand-act  endof
        bark-move of  story-bark-act  endof  leap-move of  story-leap-act  endof
        spot-move of  story-spot-act  endof  still-anim-move of  story-anim-still-act  endof
        >r -1 r>
    endcase ;
