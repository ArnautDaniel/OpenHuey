\ hewie/states.fs - Hewie's behaviours (the original's Hewie_State*): what he does each frame
\ while an action lasts. One hands over to the next with `behave`. Named for what he does; the
\ original's state is in each one's comment.
\
\ Here: his own (idle, roaming, sniffing, tricks, keeping near, coming to her, stepping away,
\ dashing, the stance, barking, being down); praise and her commands. The stalkers and the
\ story's moves are in the later files.
IN: hewie.states
USING: engine game-state actors messages common facts paths vectors doors hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.sounds hewie.model hewie.moving flag-names floor-flags ;

\ ---- how he is treated: the progress's counters (for the dog's level at the end) ----
: counter+ ( n addr -- )  tuck l@ + 0 10000 clamp swap l! ;
: mistreated+ ( n -- )  progress pr.hewie-mistreated counter+ ;
: pleased+ ( n -- )  progress pr.hewie-pleased counter+ ;
\ obey_time: how long he obeys, by his trust; obeys: he heard her - obeying for a while
: obey-time! ( -- )  black-shepherd? if  obey-time-shepherd  else  obey-time  then  trust-of his-obey ! ;
: obeys ( -- )  0 his-waiting !  obey-time!  0 his-praise-due ! ;

\ Hewie_SetMode: his mood for a time (-1: the mood's own - pleased and upset 1800 frames, angry
\ 450). Down only normal; hurt only normal or angry. Pleased, he obeys again for his trust's time
: set-mood ( mood time -- )
    {: mood time :}  mood
    his-hp @ 0= his-cond @ down-cond = and if  drop normal-mood  then
    his-cond @ hurt-cond = mood angry-mood <> and if  drop normal-mood  then
    {: m :}
    time -1 <> if  time his-mood-time !
    else  m case
            normal-mood of  0 his-mood-time !  endof
            pleased-mood of  his-mood @ pleased-mood <> if  1 pleased+  -1 mistreated+  then  1800 his-mood-time !  endof
            upset-mood of  1800 his-mood-time !  endof
            angry-mood of  his-mood @ angry-mood <> if  20 mistreated+  then  450 his-mood-time !  endof
        endcase
    then  m his-mood !
    mood pleased-mood <> if  exit  then
    0 his-waiting !  obey-time!
    on-obeying breaks-in? if  his-away @ if  loud-noise-act start  else  1 his-pending !  then  then
    0 his-praise-due ! ;
: calm-down ( -- )  normal-mood -1 set-mood  0 his-hits !  0 his-hit-t ! ;

\ Hewie_AddTrust: his trust in her (0..10000) and its level 0..7 (from 2 the story's state flag
\ 0x11 is cleared, from 3 flag 0x1D)
create trust-bounds  100 , 280 , 450 , 600 , 750 , 900 , 1000 ,   \ (the points for levels 1..7)
: add-trust ( n -- )
    his-trust-points @ + 0 10000 clamp dup his-trust-points !  {: points :}
    0 begin  dup 7 <  if  points over cells trust-bounds + @ >=  else  false  then  while  1+  repeat
    his-trust !
    hewie-timid state-flag? his-trust @ 2 >= and if  hewie-timid state-flag-clear  then
    hewie-no-dodge state-flag? his-trust @ 3 >= and if  hewie-no-dodge state-flag-clear  then ;

\ his gait (his-t3 while roaming, keeping away, going out)
enum: gait  walk-gait trot-gait run-gait ;
\ Hewie_RandomLevel: a gait at random (brisker as the black shepherd, or when not calm)
: random-level ( -- gait )
    16 roll {: r :}
    black-shepherd? if  his-danger @ calm = if  8  else  5  then  13
    else  his-danger @ calm = if  10  else  5  then  15  then  {: walk-under trot-under :}
    r walk-under < if  walk-gait  else  r trot-under < if  trot-gait  else  run-gait  then then ;
: turn-speed ( -- )   \ his turn rate (degrees) by his gait
    his-t3 @ case  walk-gait of  20e his-turn f!  endof  trot-gait of  25e his-turn f!  endof  run-gait of  30e his-turn f!  endof  endcase ;
\ a pose by how far there is to go: walking under 10 (or his gait), trotting under 20, else running
: gait-for ( F: room -- pose )
    his-t3 @ walk-gait = fdup 10e f< or if  fdrop walk-pose
    else  his-t3 @ trot-gait = 20e f< or if  trot-pose  else  run-pose  then  then ;

\ Hewie_TargetTri: his target in his room, its place into tt-at; none, the exit he is alert to
\ (an exit into the stalker's room: its spot through it); -1 none
create tt-at /vec allot
: alert-exit-tri ( -- tri | -1 )
    his-alert @ exit-alert <> if  -1 exit  then
    his-alert-what @ 2 exit-spot  dup 0< if  fdrop fdrop fdrop exit  then  tt-at vec! ;
: target-tri ( -- tri | -1 )
    his-target @ dup 0< if  drop alert-exit-tri exit  then
    dup with? 0= if  drop -1 exit  then
    tt-at over body-at vec-copy  body-tri ;
: close-to ( v -- flag )  his-at swap vec-dist 10e f< ;

\ ---- the plain ones ----
: st-nothing ( -- ) ;                                         \ off screen
: st-lie-still ( -- )  lie-pose keep-pose ;
: st-hold-low ( -- )  resting-or-low-pose keep-pose ;
: st-loud-noise ( -- )  far-noise -1 noise-make ;              \ Hewie_StateLoudNoise
: st-play-anim ( -- )  his-yelp-anim @ play-if-not ;
: st-to-default ( -- )  to-default ;
: st-anim-over ( -- )  anim-done? if  to-default  then ;
: st-anim-next ( -- )  anim-done? if  his-next @ start  then ;
: st-ears-down ( -- )  sit-pose step-to-pose 0= if  ears-down-anim play  ['] st-anim-over behave  then ;   \ (Hewie_State1770)

\ ---- running to her (action 0x6F): a happy whine, lying low by her, then on ----
: st-low-by-her-on ( -- )   \ (Hewie_State19A0)
    -1 his-t1 +!  his-t1 @ 0= if  his-next @ start  then  resting-pose keep-pose ;
: st-low-by-her ( -- )   \ (Hewie_State1990)
    -1 his-t1 +!  his-t1 @ 0= if  ahead-look look!  15 his-t1 !  ['] st-low-by-her-on behave  then  resting-pose keep-pose ;
: st-run-to-fiona ( -- )
    her-with? 0= if  to-default exit  then
    resting-pose step-to-pose if  exit  then
    whine-sound 5 0 0 his-at vec@ his-sound
    target-look look!  her his-target !  resting-pose keep-pose  30 his-t1 !  ['] st-low-by-her behave ;

\ ---- idle ----
\ Hewie_State1D98: idle - into a low pose (stopping at a wall if walking), then choose
: st-idle ( -- )
    here? 0= if  1 his-pending !  exit  then
    anim-group dup in-walking = over in-trotting = or swap in-running = or if
        stride-len fdup his-yaw fswap free-ahead f> settled? and if  -1 stand-still  then
    then
    resting-or-low-pose step-to-pose 0= if  resting-or-low-pose keep-pose  1 his-pending !  then ;
: st-near-her ( -- )  her-with? if  resting-pose keep-pose  else  to-default  then ;   \ by her, low (Hewie_State1DC8)
\ Hewie_State1E48 (prick-ears-act): stops and pricks his ears, then sniffs or faces what alerted him
: st-prick-ears ( -- )
    settled? 0= if  exit  then
    anim-done? anim@ ears-up-anim = and if  his-alert @ no-alert = if  sniff-act  else  face-target-act  then  start  exit  then
    stand-pose step-to-pose 0= if  ahead-look look!  ears-up-anim play  then ;
\ Hewie_State1F28 (heard-her-act): a bark that he heard her (lying low when calm), then the next
: st-heard-her ( -- )
    his-danger @ calm = his-cond @ hurt-cond <> and if  resting-pose  else  stand-pose  then
    step-to-pose 0= if  bark  ['] st-anim-next behave  then ;

\ ---- after showing off (stand-for-her-act .. lie-then-stay-act): into the pose, then staying
\ (the "then stay" ones) or lying by her (waiting: settling) ----
: after-showing ( pose act -- )
    {: pose act :}  pose step-to-pose if  exit  then  pose keep-pose
    his-action @ act = if  stay-act start  else  his-waiting @ 0= if  lie-by-her-act  else  settle-act  then  start  then ;
: st-showing-lying ( -- )  lie-pose lie-then-stay-act after-showing ;
: st-showing-sitting ( -- )  sit-pose sit-then-stay-act after-showing ;
: st-showing-standing ( -- )  stand-pose stand-then-stay-act after-showing ;

\ ---- keeping his distance (near-target-act .. close-by-spot-act, near-target-2-act): near his
\ target or the spot, now and then a fresh heading away from it ----
create kn-at /vec allot
: by-spot? ( -- flag )  his-action @ dup near-spot-act = swap close-by-spot-act = or ;
: close-by? ( -- flag )  his-action @ dup close-by-spot-act = swap close-by-her-act = or ;
: st-keep-near ( -- )
    his-wanted @ if  -1 his-wanted +!  then
    by-spot? if  kn-at his-spot vec-copy
    else   \ his target, while he can get to it
        his-target @ dup with? 0= if  drop to-default exit  then
        dup body-tri over body-at plan-to 0= if  drop to-default exit  then
        kn-at swap body-at vec-copy
    then
    his-at kn-at vec-dist his-turn f@ f> if  his-next @ 0 want exit  then
    his-heading f@ his-turn f@ free-ahead                            ( F: room )
    his-t2 @ if  -1 his-t2 +!
    else
        his-t1 @ if  -1 his-t1 +!  then
        his-t1 @ 0= fdup his-turn f@ f< or if
            close-by? if  3 roll 30 * 30 + his-t1 !  30 his-t2 !  else  0 his-t1 !  then
            kn-at heading-to pi f+ angle-wrap  his-turn f@ 10e f+  30 150 30 best-heading
            his-heading f!
        then
    then
    his-heading f@ head-toward-soon
    his-heading f@ run-turn turn-to fdrop
    close-by? if  fdrop walk-pose keep-pose exit  then
    as-is-pose step-to-pose 0= if
        fdup 10e f< if  fdrop walk-anim  else  20e f< if  trot-anim  else  run-anim  then  then  play-if-not
    else  fdrop  then ;

\ ---- by her ----
: turned-to ( v -- far? )   \ turning to it as he runs: more than 60 degrees still to go
    heading-to fdup run-turn turn-to  fswap fdrop  60e deg>rad f> ;
\ Hewie_StateWaitForFiona (wait-for-her-act): turned to her, in the stance; close: close by her
: st-wait-for-fiona ( -- )
    her-with? 0= if  to-default exit  then
    fiona-at close-to if  close-by-her-act wait-for-her-act want exit  then
    fiona-at turned-to if  walk-pose keep-pose exit  then
    stance ;
\ Hewie_StateBarkAtFiona (bark-at-her-act): turned to her, barks his-t1 times; close: close by her
: st-bark-at-fiona ( -- )
    her-with? 0= if  to-default exit  then
    fiona-at close-to if  close-by-her-act bark-at-her-act want exit  then
    fiona-at turned-to if  walk-pose keep-pose exit  then
    stand-pose step-to-pose 0= if  bark  -1 his-t1 +!  his-t1 @ 0= if  to-default  then  then ;
\ Hewie_State1740: by her a while (his-t2), lying low looking at her; she moved off: again
: st-by-her-a-while ( -- )
    -1 his-t2 +!  his-t2 @ 0= if  to-default exit  then
    her-dist 20e f> if  his-action @ start  then
    her his-target !  target-look look!  resting-pose keep-pose ;
: stay-by-her ( -- )  his-next @ 0= if  60 his-t2 !  ['] st-by-her-a-while behave  else  his-next @ 0 want  then ;
\ Hewie_StateComeToFiona: to her side (follow-step); there, the next action (or by her 60 frames)
: st-come-to-fiona ( -- )
    her-with? 0= if  to-default exit  then
    40e 30e follow-step 1 = if  stay-by-her  then ;
\ Hewie_StateSlideToFiona (come-to-her-act .. sulk-to-her-act): up from lying low, the way to her
\ side planned; close: there, else come at the gait for the distance
: st-slide-to-fiona ( -- )
    her-with? 0= if  to-default exit  then
    resting-pose step-to-pose if  exit  then
    resting-pose keep-pose  slide-root
    his-action @ command-place cp-pos plan-and-go if  to-default exit  then
    rest  fdup 12e f< if  fdrop path-stop  stay-by-her exit  then
    his-cond @ hurt-cond = fdup 20e f< or if  fdrop 0  else  30e f< if  1  else  2  then  then  his-t1 !
    ['] st-come-to-fiona behave ;
\ Hewie_StateComeToCommand (come-to-place-act): to his place by her - along the way, straight
\ once there; the gait by the distance left
: st-come-to-command ( -- )
    her-with? 0= if  to-default exit  then
    his-action @ command-place                                       ( tri )
    dup cp-pos -1 body-tri-to <> if
        cp-pos plan-and-go if  to-default exit  then
        rest  stride 0= if  trot-pose keep-pose  then
    else
        drop  cp-pos aim-run
        cp-try head-at  cp-try -1 body-tri-to 0< if  to-default exit  then
        his-at cp-pos vec-dist
    then
    fdup 20e f< if  fdrop walk-pose  else  44e f< if  trot-pose  else  run-pose  then  then  keep-pose ;
\ Hewie_StateStepAwayFiona (step-off-her-act): too close to her - steps off
: back-off ( id -- )
    walk-pose keep-pose
    body-at heading-to pi f+ angle-wrap                               ( F: away )
    his-t1 @ if  fdrop -1 his-t1 +!
    else  30 his-t1 !  20e 30 150 30 best-heading his-heading f!  then
    his-heading f@ head-toward-soon
    his-heading f@ run-turn turn-to fdrop ;
: st-step-away-fiona ( -- )
    her-with? 0= if  to-default exit  then
    her-dist 6e f> if  to-default exit  then
    her back-off ;

\ ---- on his own ----
\ Hewie_StateWalkHeading: walks along his-heading for his-t1 frames
: st-walk-heading ( -- )
    -1 his-t1 +!  his-t1 @ 0= if  to-default  then
    walk-pose keep-pose  his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop ;
\ Hewie_StateOffMesh (off-edge-act): his nose over an edge - walks the freest way until his head
\ and the step ahead are back over the floor
: st-off-mesh ( -- )
    walk-pose keep-pose
    his-t1 @ if  -1 his-t1 +!
    else  30 his-t1 !  his-yaw his-head-yaw f@ f+ angle-wrap 20e 30 150 30 best-heading his-heading f!  then
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop
    cp-try head-at  cp-try -1 body-tri-to 0< if  exit  then
    his-yaw 5e cp-try his-at vec-ahead  cp-try -1 body-tri-to 0< if  exit  then
    16 his-t1 !  ['] st-walk-heading behave ;
\ Hewie_StateRun (dash-act): a dash - running till time or room runs out, then slowing; then to
\ her (calm: come, else hurry)
: st-run ( -- )
    his-t2 @ 0= if
        -1 his-t1 +!
        his-t1 @ 0=  his-yaw 20e free-ahead 20e f< or if  1 his-t2 !  then
        trot-pose keep-pose exit
    then
    walk-pose keep-pose
    settled? 0= if  exit  then
    his-waiting @ if  to-default
    else  his-danger @ calm = if  come-to-her-act  else  hurry-to-her-act  then  start  then ;
\ a fresh heading now and then (every his-t1 frames, or when the way is short): roaming, keeping away
: new-heading? ( F: room -- room ) ( -- flag )
    his-t2 @ if  -1 his-t2 +!  false exit  then
    his-t1 @ if  -1 his-t1 +!  then
    his-t1 @ 0= fdup his-turn f@ f< or ;
\ Hewie_StateRoam (roam-act): roaming - a new heading every 90..240 frames or when the way is
\ short; looking about at a walk
: st-roam ( -- )
    his-heading f@ his-turn f@ free-ahead                             ( F: room )
    new-heading? if
        6 roll 30 * 90 + his-t1 !  30 his-t2 !
        his-yaw his-head-yaw f@ f+ angle-wrap  his-turn f@ 10e f+  30 150 30 best-heading his-heading f!
    then
    his-heading f@ run-turn turn-to                                   ( F: room left )
    5e deg>rad f< his-t3 @ walk-gait = and if  glance-look look!
    else  his-heading f@ head-toward-soon  then
    gait-for keep-pose ;
\ Hewie_StateScramble (scramble-act / scramble-there-act): darting about for his-wait frames,
\ then sitting, his ears down
: st-scramble ( -- )
    his-wait @ 0= if  ahead-look look!  ['] st-ears-down behave exit  then
    his-t2 @ 0= if
        his-t1 @ if  -1 his-t1 +!  then
        his-t1 @ 0=  his-heading f@ his-turn f@ free-ahead his-turn f@ f< or if
            3 roll 10 * 10 + his-t1 !  30 his-t2 !  15e his-turn f!
            his-yaw 25e 150 30 30 best-heading his-heading f!
        then
    else  -1 his-t2 +!  then
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop
    as-is-pose step-to-pose 0= if  dart-anim play-if-not  then ;
\ Hewie_StateKeepAway (keep-off-her-act / keep-off-target-act): keeping off his target at a gait
: st-keep-away ( -- )
    his-target @ dup with? 0= if  drop to-default exit  then
    dup body-tri swap body-at plan-to 0= if  to-default exit  then
    his-heading f@ his-turn f@ free-ahead                             ( F: room )
    new-heading? if
        5 roll 30 * 30 + his-t1 !  30 his-t2 !
        his-target @ body-at heading-to pi f+ angle-wrap  his-turn f@ 10e f+  30 150 30 best-heading
        fdup his-heading f!  his-yaw f- angle-wrap fabs 170e deg>rad f> if  fdrop to-default exit  then
    then
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop
    gait-for keep-pose ;
\ Hewie_StateTricks (sit-trick-act .. lie-down-trick-act): his tricks, once in the pose they need
: trick ( pose anim -- )  swap step-to-pose 0= if  play  ['] st-anim-over behave  else  drop  then ;
: st-tricks ( -- )
    his-action @ case
        sit-trick-act of  his-t1 @ 0= if  sit-pose sit-trick-anim  else  lie-pose sit-trick-lying-anim  then  trick  endof
        sit-trick-2-act of  sit-pose sit-trick-2-anim trick  endof
        lie-down-trick-act of
            anim@ case
                get-low-anim of  anim-done? if  lie-low-anim play  then  endof
                lie-low-anim of  his-wait @ 0= anim-done? and if  to-default  then  endof
                sit-pose step-to-pose 0= if  get-low-anim play  then
            endcase  endof
        stand-trick-act of  stand-pose stand-trick-anim trick  endof
        stand-trick-2-act of  stand-pose stand-trick-2-anim trick  endof
    endcase ;

\ ---- sniffing, facing, barking at something ----
\ Hewie_StateFaceScent (sniff-act): sniffing toward his target (or a scent), alert
: st-face-scent ( -- )
    his-target @ 0< his-alert @ no-alert = and if  -1  else  target-tri  then
    dup 0< his-smells @ and if  drop 0  tt-at his-scent vec-copy  then
    0< if  ahead-look look!
    else
        tt-at close-to if  his-spot tt-at vec-copy  close-by-spot-act sniff-act want exit  then
        held-look look!  tt-at look-at  his-smells @ 0= if  fswap fdrop 0e fswap  then
        his-look-yaw f!  his-look-pitch f!
        his-yaw his-look-yaw f@ f+ angle-wrap  fdup run-turn turn-to
        fswap fdrop 60e deg>rad f> if  walk-pose keep-pose exit  then
    then
    stand-pose step-to-pose 0= if  alert-anim play-if-not  then ;
\ Hewie_StateFaceTarget (face-target-act): turned to his target, in the stance; close: by it
: st-face-target ( -- )
    target-tri 0< if  ahead-look look!  stance exit  then
    tt-at close-to if  his-spot tt-at vec-copy  close-by-spot-act face-target-act want exit  then
    tt-at heading-to  fdup head-toward-soon  fdup run-turn turn-to
    fswap fdrop 60e deg>rad f> if  walk-pose keep-pose exit  then
    stance ;
\ Hewie_StateBarkAtTarget (bark-at-target-act): turned to his target, barks his-t1 times
: st-bark-at-target ( -- )
    his-target @ 0< his-alert @ and if  sniff-act 0 want  stand-pose keep-pose exit  then
    target-tri 0< if  face-target-act 0 want  stand-pose keep-pose exit  then
    tt-at close-to if  his-spot tt-at vec-copy  close-by-spot-act bark-at-target-act want exit  then
    tt-at heading-to  fdup head-toward-soon  fdup run-turn turn-to
    fswap fdrop 60e deg>rad f> if  walk-pose keep-pose exit  then
    stand-pose step-to-pose 0= if
        snap-at  bark  -1 his-t1 +!  his-t1 @ 0= if  to-default exit  then
    then  stand-pose keep-pose ;
\ Hewie_StateSteer (watch-act): standing alert, head on his target
: st-steer ( -- )
    his-alert-was @ no-alert = his-alert @ no-alert <> and if  to-default  then
    target-tri 0< his-smells @ and if  tt-at his-scent vec-copy  0  else  target-tri  then
    0< if  ahead-look look!
    else  held-look look!  tt-at look-at  his-look-yaw f!  his-smells @ if  his-look-pitch f!  else  fdrop 0e his-look-pitch f!  then  then
    stand-pose step-to-pose 0= if  alert-anim play-if-not  then ;
\ Hewie_StateWhine (whine-act / whine-praise-act): standing, looking at her, a whine; then his-next
: st-whine ( -- )
    her-with? 0=  his-t1 @ 0<> his-wait @ 0= and  or if  his-next @ 0 want exit  then
    stand-pose step-to-pose if  exit  then
    his-t1 @ 0= if
        150 his-wait !  1 his-t1 !  target-look look!  her his-target !  whine-sound make-sound  whine-act his-action !
    then  stand-pose keep-pose ;
\ Hewie_StateLookLow / Bark (bark-act): a bark (low when calm), then the default action
: look-target ( -- )  his-target @ 0< if  ahead-look  else  target-look  then  look! ;
\ down low, he tells her (her within 100 in his room) of each shut exit he is at the front of - to
\ open; told of none, where he is (Hewie_StateLookLow: her state block's 0xD - the run's companion)
: at-front? ( exit -- flag )  dup door-here? 0= if  drop false exit  then
    dup his-at vec@ door-near?  swap 0 his-at vec@ door-in-area? and ;
: tell-her ( -- )
    false  her-with?  her-dist 100e f< and if
        8 0 do  i at-front?  room-id i exit-open? 0= and if  0 i her send hewie-told  drop true  then  loop
    then
    0= if  1 his-tri her send hewie-told  then ;
: st-look-low ( -- )  look-target  resting-pose step-to-pose if  exit  then  resting-pose keep-pose  tell-her  to-default ;
: st-bark ( -- )
    look-target
    his-danger @ calm = his-cond @ hurt-cond <> and if  resting-pose  else  stand-pose  then
    step-to-pose 0= if  snap-at  bark  ['] st-look-low behave  then ;

\ ---- hurt, down ----
: st-back-up ( -- )  stand-pose step-to-pose 0= if  0 his-2d !  stand-pose keep-pose  to-default  then ;   \ (Hewie_State1D18)
\ Hewie_StateKnockedDown (knocked-down-act): lying down until he has health again; in a hard
\ game, once he is down, the game is over (game over kind 3); in the run, too (kind 1)
: dies-in-hard-mode ( -- )   \ (a hard game: his end is hers - unless a scene has them, or the story keeps it from ending)
    caught state-flag? if  exit  then
    cutscene-active?  capture-no-end state-flag? or if  1 hp!  hurt-cond cond!  exit  then
    hewie-played? if  1  else  3  then  s" gameover" actor-named send game-over-kind  caught state-flag-set ;
: st-knocked-down ( -- )
    his-wait @ 0= if  1 hp!  hurt-cond cond!  then
    ahead-look look!
    down-pose step-to-pose 0= if
        hard-mode? hewie-played? or if  dies-in-hard-mode  then
        anim@ down-anim = if  0 his-2d !  else  down-pose keep-pose  then
    then
    his-hp @ if  1 his-2d !  ['] st-back-up behave  then ;
\ Hewie_StateSlope (off-slope-act): on a slope - off it the freest way, then the default action
: st-slope ( -- )
    his-tri nav-flags slope-floor and 0= if  to-default exit  then
    his-t1 @ 0< if
        30 his-t1 !  his-yaw pi f+ angle-wrap 20e 30 150 30 best-heading his-heading f!
    then
    trot-pose keep-pose  his-heading f@ head-toward-soon  his-heading f@ run-turn 1.5e deg>rad fmax turn-to fdrop ;

\ ==== Fiona's side - praise, scolding, petting, her commands ====

\ Hewie_PraiseScold: praised or scolded for what he just did, if it is still what he did: his
\ skills at it (after fetching) move by `by`. True if it counted
: skill+ ( n k -- )  cells his-skill +  tuck @ + 0 31 clamp  swap ! ;   \ (0..31)
\ one skill he used for it: praise moves it up when it went well, down when it didn't; scolding
\ the other way
: rate-skill ( praise? by k -- )
    {: praise by k :}
    k cells his-rolled + @ 0<>  praise 1 = =  if  by  else  by negate  then  k skill+ ;
: praise-scold ( praise? by -- flag )
    {: praise by :}
    his-did @ 0=  his-did @ his-did-was @ <> or if  false exit  then
    his-did-was @ fetch-act = if   \ (a fetch: the skills it used)
        his-roll-n @ 0 ?do  praise by  i cells his-rolls + @  rate-skill  loop
    then
    -1 his-follows !  0 his-did-2 !  0 his-did !  true ;

\ ---- scolded from afar (scolded-afar-act): sitting, the ears down; three times within 600
\ frames upsets him ----
: scolded ( -- upset? )   \ (three times within 600 frames, not already in a mood)
    his-praise-b @ 0> 0= if  1 his-praise-a !  600 his-praise-b !  false exit  then
    1 his-praise-a +!
    his-praise-a @ 3 <  his-mood @ normal-mood <> or if  600 his-praise-b !  false exit  then
    20 mistreated+  upset-mood -1 set-mood  0 his-praise-a !  true ;
: st-scolded-over ( -- )  anim-done? 0= if  exit  then  scolded drop  to-default ;   \ (Hewie_State19B0)
: st-scolded-afar ( -- )  sit-pose step-to-pose 0= if  ears-down-anim play  ['] st-scolded-over behave  then ;   \ (Hewie_State22B8)
\ Hewie_State1F88 (wont-act): won't - sits and turns his head away
: st-wont ( -- )
    anim-done? anim@ refuse-anim = and if  to-default exit  then
    sit-pose step-to-pose 0= if  refuse-anim play  then ;

\ ---- by her side: turning with her, then praised, scolded or petted ----
\ Hewie_StateTurnWithFiona (turn-with-her-act): turns with her (to his-to-yaw) while she
\ gestures, then sits
: st-turn-with-fiona ( -- )
    her-with? 0= if  to-default exit  then
    fiona-mode @ command-mode <> if  to-default
    else
        his-to-yaw f@ 6e deg>rad body-turn-toward fdrop
        -1 his-t1 +!
        his-t2 @ 0<> his-t1 @ 16 < and if  sit-pose keep-pose
        else  1 his-t2 !  turn-on-spot-anim play-if-not  then
    then  1 his-no-root ! ;
\ Hewie_StatePraised: at the end of each animation of a scolding close up (three times within 600
\ frames upsets him), petting (strokes heal him; she holds the praise for more), a pat
: petting ( -- )
    anim@ case
        petted-start-anim of  3 his-t2 !  3 his-pet !  petted-anim play-cut  endof
        petted-anim of
            -1 his-t2 +!  -1 his-pet +!
            his-pet @ 0= if  4 his-pet !  his-hp @ 5 + 100 min hp!  then
            his-t2 @ 0= if  petted-end-anim  else  petted-anim  then  play-cut  endof
        petted-end-anim of
            his-hp @ 100 = if  -8 his-t1 !  his-t3 @ 0= if  -1 mistreated+  then  then
            his-t1 @ -8 = if  pleased-mood -1 set-mood  heard-her-act 0 want  else  to-default  then  endof
    endcase ;
: st-praised ( -- )
    his-danger @ calm = his-action @ petted-act = and fiona-cmd @ praise-gesture = and anim@ petted-anim = and if  2 his-t2 !  then
    anim-done? 0= if  exit  then
    his-action @ case
        scolded-close-act of
            his-praise-b @ 0> if
                1 his-praise-a +!
                his-praise-a @ 3 >= his-mood @ normal-mood = and if
                    20 mistreated+  upset-mood -1 set-mood  0 his-praise-a !  true
                else  600 his-praise-b !  false  then
            else  1 his-praise-a !  600 his-praise-b !  false  then
            0= his-t1 @ -6 = and if  calm-down  then
            to-default
        endof
        petted-act of  petting  endof
        patted-act of  300 his-pet-time !  his-waiting @ 0= if  stay-act  else  choose-act  then  0 want  endof
    endcase ;
: st-praise-start ( -- )   \ (Hewie_State2198)
    his-action @ case
        patted-act of  patted-anim play  endof
        petted-act of  petted-start-anim play  his-hp @ 100 = if  1  else  0  then  his-t3 !  endof
        scolded-close-act of  ears-down-anim play  endof
    endcase  ['] st-praised behave ;

\ Hewie_State2388 (ready-act): crouched low, readying to spring
: st-readying ( -- )
    ahead-look look!
    anim@ crouch-anim = if
        hewie-played? steering? and if  to-default exit  then   \ (the run: the pad breaks it off)
        his-t1 @ 0= if  1 his-ready !  else  -1 his-t1 +!  then
    else  stand-pose step-to-pose 0= if  crouch-anim play  then  then ;

\ ---- walking to the spot she showed (go-there-act, then scramble-there-act about it) ----
\ Hewie_StateWalkPath2: along the planned way to his-to (straight once on its triangle), stepping
\ less the sharper he turns; near the end turning on the spot to his-to-yaw
: walk-path-step ( -- ) ( F: -- left )   \ (the turn and the step this frame; how far the turn is from done)
    his-to-tri @  his-to -1 body-tri-to = {: on | F: left :}
    stride-len
    on if  fdrop his-to heading-to
    else  0e fmax 12e f* ahead@ drop  ahead heading-to  then        ( F: a )
    anim@ turn-on-spot-anim = if
        fdrop  his-to-yaw f@ head-toward
        his-to-yaw f@ his-yaw f- angle-wrap fabs 1.7453293e f* his-turn f@ f/ fsin   ( F: t )
        fdup his-turn f@ f* 0.05e f* 1e deg>rad fmax 6e deg>rad fmin                 ( F: t step )
        his-to-yaw f@ fswap body-turn-toward to left  fdrop
        left f0= settled? and if
            his-danger @ calm = if  stand-anim  else  alert-anim  then  5 play-blend
        then
        0.25e
    else
        fdup head-toward  run-turn body-turn-toward  fdup to left
        stride-len fdup f0< if  fdrop fdrop 0e  else  fswap pi fswap f- pi f/ f*  then
        0.05e fmax
    then                                                             ( F: step )
    ahead@ his-path path-i!  ahead vec@ his-place  1 his-no-root !  left ;
: st-walk-path2 ( -- )
    walk-path-step {: F: left :}
    settled? 0= if  exit  then
    path-done? his-t1 @ 0<> and left f0= and
    his-to-anim @ -1 <> if  anim@ his-to-anim @ =  else  anim-group in-standing =  then  and if
        his-action @ go-there-act = if  scramble-there-act scramble-act want  else  -1 his-done !  to-default  then  exit
    then
    his-to-anim @ -1 <> if
        anim@ his-to-anim @ = if  rest 3e f< if  1 his-t1 !  then
        else  his-to-anim @ play  then  exit
    then
    his-t1 @ 0= if
        rest fdup 3e f< 0= if
            his-t2 @ 2 = fdup 10e f< 0= and if  34e f< if  trot-pose  else  run-pose  then  keep-pose
            else  fdrop walk-pose keep-pose  then
        else
            fdrop  his-to-yaw f@ his-yaw f- angle-wrap fabs 1e deg>rad fmax his-turn f!
            1 his-t1 !  turn-on-spot-anim play
        then
    then ;
\ Hewie_StateSetOff: once standing, the way planned (no way: the default action)
: st-set-off ( -- )
    slide-root
    his-to-anim @ -1 = if
        stand-pose step-to-pose if  exit  then
        his-to-tri @ his-to plan-to if
            his-t2 @ 2 = if  rest fdup 10e f< if  fdrop walk-pose  else  34e f< if  trot-pose  else  run-pose  then  then
            else  walk-pose  then  keep-pose
            ['] st-walk-path2 behave exit
        then
    else
        his-to-tri @ his-to plan-to if  ['] st-walk-path2 behave exit  then
    then
    to-default ;
