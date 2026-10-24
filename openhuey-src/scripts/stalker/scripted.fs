\ stalker/scripted.fs - the story's moves for a stalker (src/game/pursuer.c Pursuer_EventCommand
\ and the steps it sets - Pursuer_ScriptMove, StateArrive, StateRunToTri, StateRootStep,
\ StateTurnThenWait, StateRunThenNext - with his actions 0 (Pursuer_StateStand), 1 (Move17C /
\ Move180), 4 (WalkToGoal, StepBack, CloseInGoal) and $A (Door1C8 and the door's steps)). While
\ a script holds him (stalker.fs `held-frame`: Pursuer_Think) each move the story gives him
\ becomes his step: an animation, a walk to a spot or a triangle, a door used, a turn, a look;
\ he says when it is done (+0xE1: `moving`).
IN: stalker.scripted
USING: engine game-state actors messages common facts paths vectors stalker.state stalker.senses stalker.moving stalker.search stalker.head stalker.doorway stalker.anims ;

: done ( -- )  -1 sm-done ! ;   \ (+0xE1)

\ ---- standing: his action 0 (Pursuer_StateStand / StateStopAtEnd) - his stance, done once it
\ has faded in; his action 1 (Pursuer_Move17C / Move180) - the same, moved by its root motion as
\ it fades in ----
: stood ( -- )  settled? if  done  then ;
: stand-done ( -- )  0 action-look!  still-anim play  ['] stood step! ;
: settling ( -- )  settled? if  done  else  root-move  then ;
: stand-to ( -- )  1 action-look!  still-anim play  ['] settling step! ;
' stand-done is script-door-over   \ (a door used: Pursuer_StateRunThenNext's action 0)

\ ---- an animation (moves 7, 8, 9, 16: Motion_PlayTable / PlayBlend over b frames / PlayBlend8 -
\ no time of its own / his stance over b frames): done at once, moved by its root motion
\ (Pursuer_StateRootStep) ----
: play-blend ( anim blend more-flags -- )
    {: a bl more :}  a anim !  my-model @ a bl  my-model @ a motion-entry nip nip more or  motion-play ;
: rooted ( -- )  done  ['] root-move step! ;

\ ---- walking to the spot or to a triangle (moves 5 / 10, 6 / 11: Pursuer_ScriptMove; his action
\ 4): his way planned to the goal (WalkToGoal - none: on by his motion, never done); set off in
\ the move's walk, turned on the spot first toward the way's first point past 80 degrees off
\ (StepBack); stride by stride, turned on the spot again where the way turns past 80 degrees
\ once the walk has faded in - by the turn till it ends or he faces the way, then on afresh - and
\ his way planned again once the goal lies straight ahead (CloseInGoal). The move's walk: its b
\ if one of the group's ($200 walking, $201 slowly), else his fast walk, his slow one ----
: walk-for ( group -- anim )
    sm-b @ $FF00 and over = if  drop sm-b @  else  $200 = if  walk-fast  else  walk-slow  then  then ;
defer arrived ( -- )   \ (the step after the way: StateArrive, or none - StateRunToTri)
defer walk-start ( -- )
create sm-to /vec allot
: turn-way ( -- way )   \ (Npc_TurnWayTo, toward the way's point 0.1 on)
    0.1e my-path my-at path-ahead drop  sm-to ahead vec-copy  sm-to way-to ;
: turning-on ( -- )
    root-move  settled? 0= if  exit  then
    ended?  sm-to heading-to yaw f- angle-wrap fabs 0.1e f< or if  sm-walk @ play  walk-start  then ;
: turn-on-spot ( way -- )  turn-base + play-now  ['] turning-on step! ;
: straight? ( -- flag )  self body-tri my-at goal-at self body-mask v-walk goal-tri @ = ;
: walking-on ( -- )
    way-left 4e f< 0=  settled? and if  turn-way dup no-turn <> if  turn-on-spot exit  then  drop  then
    step-t @ 0= straight? and if  goal-tri @ goal-at plan drop  1 step-t !  then   \ (+0x1628)
    walk-stride if  arrived  then ;
: no-way ( -- )  root-move ;
:noname ( -- )
    4 action-look!
    goal-tri @ goal-at plan 0> 0= if  ['] no-way step! exit  then
    way-left 4e f< 0= if  turn-way  else  no-turn  then
    dup no-turn = if  drop  sm-walk @ play  ['] walking-on step!  else  turn-on-spot  then ;
is walk-start
\ to the spot (moves 5 / 10: the goal its triangle and the point on it - NPC_GoTo: off it, the
\ triangle's middle); there (Pursuer_StateArrive): on another triangle a way under 0.1 long left
\ - put on the goal -, longer - on afresh; on it, turned to the move's heading at his turn rate,
\ then standing (done once his stance has faded in)
: turned-there ( -- )  sm-yaw f@ turn-rate body-turn-toward f0= if  stand-done  then ;
: at-goal ( -- )
    self body-tri goal-tri @ = if  ['] turned-there step!  turned-there exit  then
    goal-tri @ goal-at plan 0> if  way-left  else  -1e  then
    fdup f0< 0=  fswap 0.1e f< and if  room-id goal-tri @ goal-at vec@ body-place exit  then
    walk-start ;
: goal! ( tri -- )   \ (NPC_GoTo)
    dup goal-tri !  dup sm-at sm-at 0 v-walk over = if  drop goal-at sm-at vec-copy  else  tri-center goal-at vec!  then ;
: to-spot ( group -- )
    walk-for sm-walk !  sm-a @ goal!  ['] at-goal is arrived  walk-start ;
\ to a triangle (moves 6 / 11: its middle): done as soon as he is on it (Pursuer_StateRunToTri -
\ `held-frame`); his way over, he walks on the spot till the next move
: to-tri ( group -- )
    walk-for sm-walk !  sm-a @ dup goal-tri !  tri-center goal-at vec!
    ['] noop is arrived  walk-start ;
: to-tri? ( -- flag )  sm-kind @ dup run-move = swap run-path-move = or ;
: on-the-tri ( -- )  to-tri? if  self body-tri sm-a @ = if  done  then  then ;

\ ---- turning to a character / to the move's heading (moves 14 / 15: in his walk $200,
\ Pursuer_StateTurnThenWait) at his turn rate; there, his stance - done once it has faded in ----
: turning-to ( -- )
    sm-yaw f@ turn-rate body-turn-toward f0= 0= if  exit  then
    anim @ 4 u< if  settled? if  done  then  else  still-anim play  then ;
: turn-to-move ( -- )  walk-anim play  ['] turning-to step! ;

\ ---- the moves ----
defer released ( -- )   \ (move 1: stalker.fs)
: who! ( n -- )  sm-who ! ;   \ (+0x100: a move's argument, as the original keeps it)
: start-move ( kind -- )
    0 sm-done !  dup sm-kind !
    case
        stand-move of  released  endof
        idle-step-move of  stand-to  endof
        open-door-move of  sm-a @ dup who!  $A action-look!  script-door  endof
        shut-door-move of  sm-a @ dup who!  $A action-look!  script-door  endof
        walk-move of  $200 to-spot  endof        walk-path-move of  $201 to-spot  endof
        run-move of  $200 to-tri  endof          run-path-move of  $201 to-tri  endof
        anim-move of  sm-a @ play-now  rooted  endof
        blend-move of  sm-a @ sm-b @ 0 play-blend  rooted  endof
        blend8-move of  sm-a @ sm-b @ 8 play-blend  rooted  endof
        idle-anim-move of  0 sm-b @ 0 play-blend  rooted  endof
        look-move of   \ (b its slot, $FF none; a its actor: on only if someone is there)
            sm-b @ dup who!  $FF = if  0 look-on !  else  sm-a @ 0< 0= if  -1 look-on !  then  then  done  endof
        look-point-move of  look-pt sm-at vec-copy  -1 look-on !  done  endof
        turn-to-who-move of  sm-a @ who!  sm-a @ 0< if  done  else  turn-to-move  then  endof
        turn-to-heading-move of  turn-to-move  endof
        walk-anim-move of  sm-a @ who!  endof   \ (not the stalkers': nothing - never done)
    endcase ;
