\ hewie/actions.fs - what Hewie does (Hewie_SetAction, Hewie_AdjustAction): each action sets
\ his move mode, where he looks, what may break in (his-cmd), the timers and the behaviour that
\ carries it out. The actions by name: hewie-names.fs.
IN: hewie.actions
USING: engine game-state actors common facts paths vectors messages hewie-names hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states hewie.offscreen hewie.scripted hewie.fight hewie.more hewie.pounce hewie.fetch hewie.exits hewie.steered flag-names ;

: mode! ( n -- )  his-mode ! ;
: trust-wait ( -- n )  his-trust @ 60 * 300 + ;
\ (Hewie_PickTarget: hewie/fight.fs)
\ Hewie_IdleAction: a trick at random (not the last one), on plain floor
create idle-tricks  sit-trick-act , sit-trick-2-act , lie-down-trick-act , stand-trick-act , stand-trick-2-act ,
: plain-floor? ( -- flag )  his-tri nav-flags 3 and 0= ;   \ (not sloped, nor a step)
: idle-action ( -- )
    plain-floor? 0= if  to-default exit  then
    begin  5 roll cells idle-tricks + @  dup his-last-idle @ = while  drop  repeat
    dup his-last-idle !  start ;
\ Hewie_RandomIdle: lying by her (half the time), lying low a while or settling
: random-idle ( -- )
    100 roll dup 50 < if  drop lie-by-her-act  else  65 < if  hold-low-act  else  settle-act  then  then  start ;
\ go_for (after a call or coming out): whom - none (0), the stalker (2, left to the caller); she
\ held, by chance at whoever holds her (1)
: go-for ( -- n )
    pick-target dup his-target2 !  dup with? 0= if  drop 0 exit  then
    his-held @ 0= swap pursuer = and if  2 exit  then
    harry-stalker-act 0 want  1 ;
: after-call ( -- )   \ (answer-call-act / -2: at the stalker - after the first at a flank)
    go-for case
        0 of  sniff-act 0 want  endof
        2 of  his-action @ answer-call-act = if  at-flank-act  else  at-stalker-act  then  0 want  endof
    endcase ;
: after-coming-out ( -- )   \ Hewie_AfterComingOut (her "go"; at the stalker: his leg when charged
    \ and coming at him head on, else a flank)
    go-for case
        0 of  hewie-played? if  run-up-jump-act  else  sniff-act  then  0 want  endof   \ (in the run: a leap)
        2 of  his-ready @ attack-side 0= and if  at-leg-act  else  at-flank-act  then  0 want  endof
    endcase ;
\ Hewie_After4D (show-off-act): by his pose - standing: sitting for her, hurt a roll over; sitting:
\ lying, or a roll over; lying: a roll over, hurt lying
: after-4d ( -- )
    anim-group rnd 0.5e f<                                           ( g coin )
    swap case
        in-standing of  his-cond @ hurt-cond <> and if  sit-for-her-act  else  lie-for-her-act  then  endof
        in-standing-up of  his-cond @ hurt-cond <> and if  sit-for-her-act  else  lie-for-her-act  then  endof
        in-sitting of  if  stand-for-her-act  else  lie-for-her-act  then  endof
        in-sitting-down of  if  stand-for-her-act  else  lie-for-her-act  then  endof
        in-lying of  his-cond @ hurt-cond = or if  stand-for-her-act  else  lie-for-her-act  then  endof
        in-lying-down of  his-cond @ hurt-cond = or if  stand-for-her-act  else  lie-for-her-act  then  endof
        >r drop stand-for-her-act r>
    endcase  0 want ;

\ a pose to hold, looking about, what may break in, the behaviour
: holding ( look cmd xt -- )  free-mode mode!  behave  his-cmd !  look! ;
: low-floor? ( -- flag )  his-tri nav-flags $80001 and 0<> ;   \ (a step or a doorway: no rolling over there)
: target-her ( -- )  target-look look!  her his-target ! ;
\ keeping near: the turn, the timers, what may break in
: anim-setup ( look cmd -- )  free-mode mode!  his-cmd !  look! ;   \ (the scripted animations' set-up)
: keep-near ( F: turn -- ) ( cmd -- )
    his-turn f!  0 his-t1 !  0 his-t2 !  his-yaw his-heading f!  his-cmd !  ['] st-keep-near behave ;
: by-her ( cmd xt -- ok? )   \ (on her own: by her - else the default)
    her-with? 0= if  2drop to-default false exit  then
    free-mode mode!  behave  his-cmd !  true ;
: come-to-her ( cmd -- )  ['] st-slide-to-fiona by-her if  2 his-t1 !  then ;
: whine ( -- )  on-obeying on-danger or on-command or on-footing or on-100 or on-alert or  ['] st-whine by-her if  0 his-t1 !  then ;
: low-by-her ( wait cmd -- )   \ (lying low by her - a while: wait -1 his own; not with her: holding low)
    her-with? 0= if  2drop hold-low-act 0 want exit  then
    free-mode mode!  target-her  his-cmd !  dup 0< if  drop  else  his-wait !  then  ['] st-near-her behave ;

: (set-action) ( act -- )
    case
    choose-act of
        -1 his-target2 !  free-mode mode!  0 his-cmd !
        his-busy @ if  ahead-look look!  ['] st-scripted-idle behave   \ (a script has him)
        else  0 his-2d !  1 his-pending !  ahead-look look!  ['] st-idle behave  then  endof
    lie-by-her-act of  -1  breaks-any on-obeying on-100 or but  low-by-her  endof
    stay-act of  trust-wait  on-command on-wait-over or on-alert or on-praise-due or  low-by-her  endof
    stay-a-while-act of  his-wanted @  on-command on-wait-over or on-alert or on-praise-due or  low-by-her  endof
    hold-low-act of  ahead-look breaks-any on-waiting but ['] st-hold-low holding  endof
    settle-act of
        45 roll dup 20 < if  drop slow-glance-look  else  dup 25 < if  drop aside-right-look  else  30 < if  low-look  else  aside-left-look  then  then  then
        breaks-any on-waiting but ['] st-hold-low holding  endof
    sniff-act of  free-mode mode!  90 his-wait !  pick-target his-target !  breaks-any on-100 but his-cmd !  ['] st-face-scent behave  endof
    watch-act of  free-mode mode!  trust-wait his-wait !  pick-target his-target !
        breaks-any on-obeying on-waiting or on-footing or on-alert or but his-cmd !  ['] st-steer behave  endof
    face-target-act of
        pick-target his-target !
        his-alert @ unused-alert =  his-target @ with? or if  150 his-wait !  free-mode mode!  breaks-any his-cmd !  ['] st-face-target behave
        else  sniff-act 0 want  then  endof
    wait-for-her-act of
        her-with? if  free-mode mode!  target-her  300 his-wait !  on-danger on-wait-over or on-100 or on-alert or his-cmd !  ['] st-wait-for-fiona behave
        else  sniff-act 0 want  then  endof
    bark-at-target-act of
        his-target2 @ 0< if  pick-target his-target2 !  then
        his-target2 @ with? if  free-mode mode!  2 roll 1+ his-t1 !  his-target2 @ his-target !  breaks-any his-cmd !  ['] st-bark-at-target behave
        else  sniff-act 0 want  then  endof
    bark-at-her-act of
        her-with? if  free-mode mode!  3 roll 3 + his-t1 !  target-her  on-danger on-wait-over or on-100 or on-alert or his-cmd !  ['] st-bark-at-fiona behave
        else  sniff-act 0 want  then  endof
    come-to-her-act of  breaks-any on-footing on-100 or but come-to-her  endof
    come-back-act of  breaks-any on-footing on-100 or but come-to-her  endof
    hurry-to-her-act of  breaks-any on-footing on-100 or but come-to-her  endof
    sulk-to-her-act of  breaks-any on-footing on-alert or but come-to-her  endof
    keep-off-her-act of
        her-with? if
            free-mode mode!  0 his-t1 !  0 his-t2 !  random-level his-t3 !  his-yaw his-heading f!  turn-speed
            her his-target !  3 roll 30 * 30 + his-wait !  breaks-any on-waiting on-footing or but his-cmd !  ['] st-keep-away behave
        else  to-default  then  endof
    keep-off-target-act of
        pick-target his-target !
        his-target @ with? if
            free-mode mode!  0 his-t1 !  0 his-t2 !  run-gait his-t3 !  his-yaw his-heading f!  30e his-turn f!
            30 his-wait !  breaks-any on-waiting on-footing or but his-cmd !  ['] st-keep-away behave
        else  to-default  then  endof
    scramble-act of  free-mode mode!  90 his-t1 !  0 his-t2 !  his-yaw his-heading f!  5e his-turn f!
        breaks-any on-footing on-wait-over or on-100 or but his-cmd !  ['] st-scramble behave  endof
    scramble-there-act of  free-mode mode!  90 his-t1 !  0 his-t2 !  his-yaw his-heading f!  5e his-turn f!
        breaks-any on-footing on-wait-over or on-100 or but his-cmd !  ['] st-scramble behave  endof
    prick-ears-act of  free-mode mode!  0 his-cmd !  ['] st-prick-ears behave  endof
    roam-act of
        free-mode mode!  30 his-t1 !  0 his-t2 !  random-level his-t3 !  his-yaw his-head-yaw f@ f+ angle-wrap his-heading f!
        turn-speed  3 roll 30 * 30 + his-wait !  breaks-any on-waiting on-footing or but his-cmd !  ['] st-roam behave  endof
    come-to-place-act of  on-waiting on-danger or on-command or on-wait-over or on-alert or on-praise-due or  ['] st-come-to-command by-her drop  endof
    trick-act of  idle-action  endof
    sit-trick-act of  plain-floor? if  2 roll his-t1 !  ahead-look breaks-any on-waiting on-wait-over or on-100 or but ['] st-tricks holding  else  to-default  then  endof
    sit-trick-2-act of  plain-floor? if  ahead-look breaks-any on-waiting on-wait-over or on-100 or but ['] st-tricks holding  else  to-default  then  endof
    stand-trick-act of  plain-floor? if  ahead-look breaks-any on-waiting on-wait-over or on-100 or but ['] st-tricks holding  else  to-default  then  endof
    stand-trick-2-act of  plain-floor? if  ahead-look breaks-any on-waiting on-wait-over or on-100 or but ['] st-tricks holding  else  to-default  then  endof
    lie-down-trick-act of  plain-floor? if  900 his-wait !  ahead-look breaks-any on-waiting on-wait-over or on-100 or but ['] st-tricks holding  else  to-default  then  endof
    heard-her-act of  free-mode mode!  target-her  0 his-cmd !  ['] st-heard-her behave  endof
    wont-act of  free-mode mode!  target-her  0 his-cmd !  ['] st-wont behave  endof
    \ her commands and meetings
    turn-with-her-act of
        his-to-yaw f@ his-yaw f- angle-wrap fabs 6e deg>rad f/ f>s  dup his-t1 !  6 < if  1  else  0  then  his-t2 !
        meeting-mode mode!  target-her  on-command his-cmd !  ['] st-turn-with-fiona behave  endof
    scolded-close-act of  meeting-mode mode!  ahead-look look!  on-command on-praise-due or his-cmd !  ['] st-praise-start behave  endof
    petted-act of  meeting-mode mode!  ahead-look look!  on-command on-praise-due or his-cmd !  ['] st-praise-start behave  endof
    patted-act of  meeting-mode mode!  ahead-look look!  on-command on-praise-due or his-cmd !  ['] st-praise-start behave  endof
    show-off-act of  after-4d  endof
    after-coming-out-act of  0 his-hold-call !  after-coming-out  endof
    go-there-act of
        her-with? if  free-mode mode!  breaks-any on-footing on-wait-over or on-100 or but his-cmd !  0 his-t1 !  2 his-t2 !  -1 his-to-anim !  ['] st-set-off behave
        else  to-default  then  endof
    scolded-afar-act of  free-mode mode!  ahead-look look!  on-praise-due his-cmd !  ['] st-scolded-afar behave  endof
    ready-act of  free-mode mode!  trust-wait his-wait !  60 his-t1 !  ahead-look look-now!  on-command on-wait-over or his-cmd !  ['] st-readying behave  endof
    \ at the stalker (hewie/fight.fs)
    at-flank-act of  at-pursuer  endof  at-stalker-act of  at-pursuer  endof
    at-leg-act of  at-pursuer  endof  at-leg-2-act of  at-pursuer  endof
    close-on-act of  close-on  endof
    \ out of the room away from her (Hewie_FleeExit - open doors he may pass)
    away-from-her-act of  away-from-her  endof
    away-act of  away  endof
    stand-for-her-act of  ahead-look breaks-any ['] st-showing-standing holding  endof
    stand-then-stay-act of  ahead-look breaks-any ['] st-showing-standing holding  endof
    sit-for-her-act of  low-floor? if  to-default  else  ahead-look breaks-any ['] st-showing-sitting holding  then  endof
    sit-then-stay-act of  low-floor? if  to-default  else  ahead-look breaks-any ['] st-showing-sitting holding  then  endof
    lie-for-her-act of  low-floor? if  to-default  else  ahead-look breaks-any ['] st-showing-lying holding  then  endof
    lie-then-stay-act of  low-floor? if  to-default  else  ahead-look breaks-any ['] st-showing-lying holding  then  endof
    \ off screen (out of the room being played): toward her, waiting, out at random, to a noise
    to-her-room-act of  route-played  endof
    back-to-her-act of  route-played  endof
    to-her-cry-act of  route-played  endof
    squeeze-out-act of  squeeze  endof
    walk-out-act of  walk-out  endof
    wait-off-act of  away-mode mode!  breaks-away his-cmd !  idle-wait trust-of his-wait !  ['] st-nothing behave  endof
    wait-a-while-act of  away-mode mode!  on-command on-wait-over or his-cmd !  his-wanted @ his-wait !  ['] st-nothing behave  endof
    wait-scuffle-act of  away-mode mode!  on-wait-over his-cmd !  his-wanted @ his-wait !  ['] st-nothing behave  endof
    wait-after-flight-act of  away-mode mode!  breaks-away his-cmd !  his-wanted @ his-wait !  ['] st-nothing behave  endof
    out-random-act of  false off-random  endof
    to-noise-act of  to-noise  endof
    out-random-away-act of  true off-random  endof
    hide-act of  away-mode mode!  0 his-cmd !  ['] st-nothing behave  endof
    loud-noise-act of  away-mode mode!  on-wait-over his-cmd !  60 his-wait !  ['] st-loud-noise behave  endof
    stop-off-act of  away-mode mode!  0 his-cmd !  his-path path-clear  ['] st-to-default behave  endof
    sniff-about-act of  roam-act 0 want  endof
    answer-call-act of  his-waiting @ 1 = if  900 his-hold-call !  then  after-call  endof
    answer-call-2-act of  his-waiting @ 1 = if  900 his-hold-call !  then  after-call  endof
    random-idle-act of  random-idle  endof
    knocked-down-act of
        normal-mood -1 set-mood  free-mode mode!  ahead-look look-now!  1 his-2d !  $1518 his-wait !  0 his-cmd !
        ['] st-knocked-down behave  endof
    near-her-act of
        her his-target !
        her-with? fiona-mode @ dup thrown-mode <> swap ladder-mode <> and and if
            engaged-mode mode!  60 his-wait !  30e  his-mood @ angry-mood <> if  on-command on-wait-over or  else  on-wait-over  then  keep-near
        else  give-up  then  endof
    close-by-her-act of  her-with? if  her his-target !  free-mode mode!  20e on-danger on-wait-over or on-100 or on-alert or keep-near  else  to-default  then  endof
    near-spot-act of  free-mode mode!  60 his-wait !  30e breaks-any keep-near  endof
    close-by-spot-act of  free-mode mode!  20e breaks-any keep-near  endof
    lie-low-act of  ahead-look breaks-any on-footing on-100 or but ['] st-lie-still holding  endof
    \ harrying the stalker or a creature (pick-target)
    harry-act of  harry  endof
    harry-stalker-act of  3 his-wanted !  harry  endof
    near-target-act of
        his-target @ dup with? if
            mode-of dup unreachable-mode <> swap struck-mode <> and if
                engaged-mode mode!  60 his-wait !  30e  his-mood @ angry-mood <> if  on-command on-wait-over or  else  on-wait-over  then  keep-near
            else  give-up  then
        else  drop give-up  then  endof
    harry-her-act of  her-with? if  near-her-act close-on-act want  else  give-up  then  endof
    harry-her-3-act of  3 his-wanted !  her-with? if  near-her-act close-on-act want  else  give-up  then  endof
    step-off-her-act of
        her-with? if  free-mode mode!  target-her  0 his-t1 !  breaks-any on-waiting on-footing or but his-cmd !  ['] st-step-away-fiona behave
        else  to-default  then  endof
    off-edge-act of  free-mode mode!  0 his-t1 !  on-command his-cmd !  ['] st-off-mesh behave  endof
    off-slope-act of  -1 his-t1 !  0 his-t2 !  free-mode mode!  on-command his-cmd !  ['] st-slope behave  endof
    run-to-her-act of  0 his-cmd !  ['] st-run-to-fiona behave  endof
    dash-act of  30 his-t1 !  0 his-t2 !  free-mode mode!  ahead-look look!  0 his-cmd !  ['] st-run behave  endof
    anim-then-act of  free-mode mode!  0 his-cmd !  ['] st-anim-over behave  endof
    yelp-anim-act of  free-mode mode!  0 his-cmd !  ['] st-play-anim behave  endof
    hidden-act of  away-mode mode!  on-command his-cmd !  ['] st-nothing behave  endof
    whine-act of  whine  endof
    whine-praise-act of  whine  endof
    smell-act of  his-scent look-at his-look-yaw f!  his-look-pitch f!  held-look breaks-any on-waiting but ['] st-hold-low holding  endof
    bark-act of   \ (at her within 150, else the stalker within 150)
        free-mode mode!  -1 his-target !
        her-with? her-dist 150e f< and if  her his-target !  then
        his-target @ 0<  pursuer with? and if  his-at pursuer body-at vec-dist 150e f< if  pursuer his-target !  then  then
        0 his-cmd !  ['] st-bark behave  endof
    \ the run (hewie/steered.fs)
    steered-act of  free-mode mode!  ahead-look look!  on-command his-cmd !  ['] st-steered behave  endof
    steered-2-act of  lie-flat  endof
    run-up-jump-act of  run-up-jump  endof
    \ the story's moves (Hewie_Requests): animations with their root motion, walks, turns, a bark, the leap
    story-anim-act of  ahead-look 0 anim-setup  his-to-tri @ play  done!  ['] st-root-motion behave  endof
    story-blend-act of  ahead-look 0 anim-setup  his-to-tri @ his-to-anim @ play-blend  done!  ['] st-root-motion behave  endof
    story-anim-still-act of  ahead-look 0 anim-setup  his-to-tri @ his-to-anim @ play-blend  done!  ['] st-root-motion behave  endof
    story-blend8-act of  ahead-look 0 anim-setup  his-to-tri @ his-to-anim @ play-blend8  done!  ['] st-root-motion behave  endof
    story-stand-act of  ahead-look 0 anim-setup  his-to-anim @ stand-still  done!  ['] st-root-motion behave  endof
    story-walk-act of  free-mode mode!  0 his-cmd !  0 his-t1 !  0 his-t2 !  ['] st-set-off behave  endof
    story-run-act of  free-mode mode!  0 his-cmd !  0 his-t1 !  2 his-t2 !  ['] st-set-off behave  endof
    story-path-act of  free-mode mode!  0 his-cmd !  ['] st-path-to-tri behave  endof
    story-path-run-act of  free-mode mode!  0 his-cmd !  ['] st-path-to-tri behave  endof
    story-turn-2-act of  free-mode mode!  0 his-cmd !  ['] st-turn-start behave  endof
    story-bark-act of  free-mode mode!  0 his-cmd !  ['] st-story-bark behave  endof
    story-spot-act of  free-mode mode!  0 his-cmd !  ['] st-head-for-spot behave  endof
    story-leap-act of
        his-to v.y sf@ his-jump-rise f!  his-2d @ 1 = if  1  else  0  then  his-t2 !
        free-mode mode!  0 his-cmd !  ['] st-run-for-spot behave  endof
    \ (hewie/more.fs)
    sit-then-act of  his-next @ sit-then  endof
    keep-behind-act of  keep-behind  endof
    go-limp-act of  go-limp  endof
    scene-pose-act of  scene-pose  endof
    back-on-floor-act of  back-on-mesh  endof
    after-attack-act of  after-attack  endof
    step-off-target-act of  step-away-target  endof
    pushed-back-act of  pushed-back  endof
    pushed-out-act of  pushed-out  endof
    at-cast-3-act of  at-cast-3-act at-cast  endof  at-cast-3-long-act of  at-cast-3-long-act at-cast  endof
    at-cast-4-act of  at-cast-4-act at-cast  endof  at-cast-4-long-act of  at-cast-4-long-act at-cast  endof
    at-cast-5-act of  at-cast-5-act at-cast  endof  at-cast-5-long-act of  at-cast-5-long-act at-cast  endof
    near-target-2-act of  near-target  endof
    flank-act of  flank  endof
    fetch-act of  fetch  endof
    draw-off-act of  draw-off  endof
    rush-act of  close-in  endof
    tackle-act of  at-pursuer  endof   \ (the story's tackle: Hewie_StateTargetPursuer - at him, as at a flank)
    \ the rest: the default
    >r  r@ if  to-default  then  r>
    endcase ;
: set-action-now ( act arg -- )   \ Hewie_SetAction
    his-next !  dup his-action !  0 his-pending !
    dup yelp-anim-act <>  over hidden-act <> and  over 0<> and if  -1 his-yelp !  then
    10 roll 30 * 90 + his-wait !
    (set-action) ;
' set-action-now is set-action

\ ---- Hewie_AdjustAction: the action his situation makes of `act` ----
: one-of ( act a b -- flag )  rot tuck = -rot = or ;
: attack-act? ( act -- flag )
    dup at-flank-act away-from-her-act within  over after-coming-out-act random-idle-act within or
    over harry-act = or  over harry-stalker-act = or  swap tackle-act = or ;
: adjust ( act -- act' )
    hewie-played? if  exit  then   \ (the run: as asked)
    his-busy @ 0=  over choose-act = and  his-hp @ 0= and  his-cond @ down-cond = and if  drop knocked-down-act  then
    dup knocked-down-act = if
        here?  his-tri nav-flags self body-mask and 0<> and if   \ (down where he can't lie: up again)
            drop choose-act  1 hp!  hurt-cond cond!
        else
            his-action @ dup knocked-down-act <> swap go-limp-act <> and  anim-group in-down <> and if  10 mistreated+  then
        then
    then
    cutscene-active? if
        dup away-from-her-act away-act one-of if  drop settle-act
        else dup harry-her-act harry-her-3-act one-of if  drop bark-at-her-act
        else dup attack-act? if  drop bark-at-target-act  then then then
    then
    hewie-no-attack state-flag? fiona-occupied state-flag? or if
        dup attack-act?  over harry-her-act harry-her-3-act one-of or if  drop bark-at-target-act  then
    then
    his-hold-call @ if  dup answer-call-act answer-call-2-act one-of if  drop bark-at-target-act  then  then
    his-cond @ hurt-cond = if
        dup scramble-act scramble-there-act one-of if  his-waiting @ 1 = if  drop keep-off-her-act  then
        else  dup come-to-place-act =  over sit-trick-act heard-her-act within or
              over stand-for-her-act = or  over sit-for-her-act = or  over lie-for-her-act = or if  drop lie-low-act
        then then
    then
    his-mood @ upset-mood = if
        dup scramble-act scramble-there-act one-of if  his-waiting @ 1 = if  drop keep-off-her-act  then
        else dup hurry-to-her-act = if  his-danger @ chased = if  drop sulk-to-her-act  then
        else  dup come-to-her-act =  over come-to-place-act = or  over sit-trick-act heard-her-act within or if  drop lie-for-her-act
        then then then
    then
    his-mood @ pleased-mood = over come-to-her-act = and if  drop come-to-place-act  then
    his-smells @  over dup settle-act = over hold-low-act = or swap lie-by-her-act = or and  his-danger @ calm = and if  drop smell-act  then
    his-stay @ if  drop hold-low-act  then ;
' adjust is adjust-action
