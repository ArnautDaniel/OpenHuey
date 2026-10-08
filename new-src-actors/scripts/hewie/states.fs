\ hewie/states.fs - Hewie's behaviours (the original's Hewie_State*): what he does each frame
\ while an action lasts. One hands over to the next with `behave`. The address-named ones keep
\ their address (st-1d98 is Hewie_State1D98).
\
\ Here in H1: his own (idle, roaming, sniffing, tricks, keeping near, coming to her, stepping
\ away, dashing, the stance, barking, being down). Later: praise and commands (H2), the stalkers
\ (H3), the story's moves (H4).
IN: hewie.states
USING: engine game-state actors common facts paths hewie.state hewie.body hewie.tables hewie.model hewie.moving ;

\ ---- how he is treated: the progress's counters (for the dog's level at the end) ----
: counter+ ( n addr -- )  tuck l@ + 0 10000 clamp swap l! ;
: mistreated+ ( n -- )  progress pr.hewie-mistreated counter+ ;
: pleased+ ( n -- )  progress pr.hewie-pleased counter+ ;
\ obey_time: how long he obeys, by his trust; obeys: he heard her - obeying for a while
: obey-time! ( -- )  hard? if  obey-time-hard  else  obey-time  then  trust-of his-obey ! ;
: obeys ( -- )  0 his-waiting !  obey-time!  0 his-praise-due ! ;

\ Hewie_SetMode: his mood (0 normal, 1 pleased - praised from afar, 2 upset - scolded too often,
\ 3 angry - hit by Fiona) for a time (-1: the mood's own: 1 and 2 1800 frames, 3 450). Down
\ only 0; hurt only 0 or 3. Pleased, he obeys again for his trust's time
: set-mood ( mood time -- )
    swap dup >r                                                   ( time m )
    his-hp @ 0= his-cond @ 2 = and if  drop 0  then
    his-cond @ 1 = r@ 3 <> and if  drop 0  then
    swap dup -1 <> if  his-mood-time !  else  drop
        dup case
            0 of  0 his-mood-time !  endof
            1 of  his-mood @ 1 <> if  1 pleased+  -1 mistreated+  then  1800 his-mood-time !  endof
            2 of  1800 his-mood-time !  endof
            3 of  his-mood @ 3 <> if  20 mistreated+  then  450 his-mood-time !  endof
        endcase
    then  his-mood !
    r> 1 <> if  exit  then
    0 his-waiting !  obey-time!
    his-cmd @ $80000001 and 1 = if
        his-away @ if  $34 start  else  1 his-pending !  then
    then
    0 his-praise-due ! ;
: calm-down ( -- )  0 -1 set-mood  0 his-hits !  0 his-hit-t ! ;

\ Hewie_AddTrust: his trust in her (0..10000) and its level 0..7 (from 2 the story's state flag
\ 0x11 is cleared, from 3 flag 0x1D)
create trust-bounds  100 , 280 , 450 , 600 , 750 , 900 , 1000 ,
: add-trust ( n -- )
    his-trust-points @ + 0 10000 clamp dup his-trust-points !
    0 begin  dup 7 < if  2dup cells trust-bounds + @ >=  else  false  then  while  1+  repeat
    nip his-trust !
    $11 state-flag? his-trust @ 2 >= and if  $11 state-flag-clear  then
    $1D state-flag? his-trust @ 3 >= and if  $1D state-flag-clear  then ;

\ Hewie_RandomLevel: a gait at random (0 walk, 1 trot, 2 run; brisker on hard or when not calm)
: random-level ( -- n )
    16 roll
    hard? if
        his-danger @ 0= if  dup 8 < if  drop 0  else  13 < if  1  else  2  then  then
        else  dup 5 < if  drop 0  else  13 < if  1  else  2  then  then  then
    else
        his-danger @ 0= if  dup 10 < if  drop 0  else  15 < if  1  else  2  then  then
        else  dup 5 < if  drop 0  else  15 < if  1  else  2  then  then  then
    then ;
: turn-speed ( -- )   \ his turn rate (degrees) by his gait
    his-t3 @ case  0 of  20e his-turn f!  endof  1 of  25e his-turn f!  endof  2 of  30e his-turn f!  endof  endcase ;
\ a gait by how far there is to go: 7 walk under 10 (or gait 0), 8 trot under 20 (or gait 1), else 9 run
: gait-for ( F: room -- kind )
    his-t3 @ 0= fdup 10e f< or if  fdrop 7
    else  his-t3 @ 1 = 20e f< or if  8  else  9  then  then ;

\ Hewie_TargetTri: his target in his room, its place into tt-at; -1 none
create tt-at 12 allot
: target-tri ( -- tri | -1 )
    his-target @ dup 0< if  exit  then
    dup with? 0= if  drop -1 exit  then
    tt-at over body-at vec-copy  body-tri ;
: close-to ( v -- flag )  his-at swap vec-dist 10e f< ;

\ ---- the plain ones ----
: st-nothing ( -- ) ;                                         \ off screen
: st-pose2 ( -- )  2 keep-pose ;
: st-pose4 ( -- )  4 keep-pose ;
: st-loud-noise ( -- )  $80 -1 noise-make ;                    \ Hewie_StateLoudNoise
: st-play-anim ( -- )  his-yelp-anim @ play-if-not ;
: st-to-default ( -- )  to-default ;
: st-anim-over ( -- )  anim-done? if  to-default  then ;
: st-anim-next ( -- )  anim-done? if  his-next @ start  then ;
: st-1770 ( -- )  1 step-to-pose 0= if  $1C04 play  ['] st-anim-over behave  then ;   \ sitting, ears down

\ ---- running to her (action 0x6F): a happy whine, lying low by her, then on ----
: st-19a0 ( -- )  -1 his-t1 +!  his-t1 @ 0= if  his-next @ start  then  3 keep-pose ;
: st-1990 ( -- )  -1 his-t1 +!  his-t1 @ 0= if  4 look!  15 his-t1 !  ['] st-19a0 behave  then  3 keep-pose ;
: st-run-to-fiona ( -- )
    her-with? 0= if  to-default exit  then
    3 step-to-pose if  exit  then
    $60 5 0 0 his-at vec@ sound-at
    0 look!  her his-target !  3 keep-pose  30 his-t1 !  ['] st-1990 behave ;

\ ---- idle ----
\ Hewie_State1D98: idle - into a low pose (stopping at a wall if walking), then choose
: st-1d98 ( -- )
    here? 0= if  1 his-pending !  exit  then
    anim-group dup 10 = over 9 = or swap 8 = or if
        stride-len fdup his-yaw fswap free-ahead f> settled? and if  -1 stand-anim  then
    then
    4 step-to-pose 0= if  4 keep-pose  1 his-pending !  then ;
: st-1dc8 ( -- )  her-with? if  3 keep-pose  else  to-default  then ;   \ by her, low
\ Hewie_State1E48 (action 0x14): stops and pricks his ears (0x1C02), then sniffs (6) or faces
\ what alerted him (8)
: st-1e48 ( -- )
    settled? 0= if  exit  then
    anim-done? anim@ $1C02 = and if  his-alert @ 0= if  6  else  8  then  start  exit  then
    0 step-to-pose 0= if  4 look!  $1C02 play  then ;
\ Hewie_State1F28 (action 0x1D): a bark that he heard her (lying low when calm), then the next
: st-1f28 ( -- )
    his-danger @ 0= his-cond @ 1 <> and if  3  else  0  then
    step-to-pose 0= if  bark  ['] st-anim-next behave  then ;

\ ---- after a trick (0x26..0x2B): into the pose, then follow (2) or stay by her (1; waiting 5) ----
: after-helper ( kind act -- )
    swap dup step-to-pose if  2drop exit  then  keep-pose
    his-action @ = if  2 start  else  his-waiting @ 0= if  1  else  5  then  start  then ;
: st-after-2b ( -- )  2 $2B after-helper ;
: st-after-29 ( -- )  1 $29 after-helper ;
: st-after-27 ( -- )  0 $27 after-helper ;

\ ---- keeping his distance (actions 0x53..0x57, 0x7C): near his target or the spot, now and
\ then a fresh heading away from it ----
create kn-at 12 allot
: st-keep-near ( -- )
    his-wanted @ if  -1 his-wanted +!  then
    his-action @ dup $56 = swap $57 = or if  kn-at his-spot vec-copy
    else   \ 0x53 / 0x54 / 0x55 / 0x7C: his target, while he can get to it
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
            his-action @ dup $57 = swap $55 = or if
                3 roll 30 * 30 + his-t1 !  30 his-t2 !
            else  0 his-t1 !  then
            kn-at heading-to pi f+ angle-wrap  his-turn f@ 10e f+  30 150 30 best-heading
            his-heading f!
        then
    then
    his-heading f@ head-toward-soon
    his-heading f@ run-turn turn-to fdrop
    his-action @ dup $57 = swap $55 = or if  fdrop 7 keep-pose exit  then
    5 step-to-pose 0= if
        fdup 10e f< if  fdrop $200  else  20e f< if  $201  else  $202  then  then  play-if-not
    else  fdrop  then ;

\ ---- by her ----
: turned-to ( v -- far? )   \ turning to it as he runs: more than 60 degrees still to go
    heading-to fdup run-turn turn-to  fswap fdrop  60e deg>rad f> ;
\ Hewie_StateWaitForFiona (action 9): turned to her, in the stance; close: 0x55
: st-wait-for-fiona ( -- )
    her-with? 0= if  to-default exit  then
    fiona-at close-to if  $55 9 want exit  then
    fiona-at turned-to if  7 keep-pose exit  then
    stance ;
\ Hewie_StateBarkAtFiona (action 0xB): turned to her, barks his-t1 times; close: 0x55
: st-bark-at-fiona ( -- )
    her-with? 0= if  to-default exit  then
    fiona-at close-to if  $55 $B want exit  then
    fiona-at turned-to if  7 keep-pose exit  then
    0 step-to-pose 0= if  bark  -1 his-t1 +!  his-t1 @ 0= if  to-default  then  then ;
\ Hewie_State1740: by her a while (his-t2), lying low looking at her; she moved off: again
: st-1740 ( -- )
    -1 his-t2 +!  his-t2 @ 0= if  to-default exit  then
    her-dist 20e f> if  his-action @ start  then
    her his-target !  0 look!  3 keep-pose ;
: by-her ( -- )  his-next @ 0= if  60 his-t2 !  ['] st-1740 behave  else  his-next @ 0 want  then ;
\ Hewie_StateComeToFiona: to her side (follow-step); there, the next action (or by her 60 frames)
: st-come-to-fiona ( -- )
    her-with? 0= if  to-default exit  then
    40e 30e follow-step 1 = if  by-her  then ;
\ Hewie_StateSlideToFiona (actions 0xC..0xF): up from lying low, the way to her side planned;
\ close: there, else come at the gait for the distance
: st-slide-to-fiona ( -- )
    her-with? 0= if  to-default exit  then
    3 step-to-pose if  exit  then
    3 keep-pose  slide-root
    his-action @ command-place cp-pos plan-and-go if  to-default exit  then
    rest  fdup 12e f< if  fdrop path-stop  by-her exit  then
    his-cond @ 1 = fdup 20e f< or if  fdrop 0  else  30e f< if  1  else  2  then  then  his-t1 !
    ['] st-come-to-fiona behave ;
\ Hewie_StateComeToCommand (action 0x16): to his place by her - along the way, straight once
\ there; the gait by the distance left
: st-come-to-command ( -- )
    her-with? 0= if  to-default exit  then
    his-action @ command-place                                       ( tri )
    dup cp-pos -1 body-tri-to <> if
        cp-pos plan-and-go if  to-default exit  then
        rest  stride 0= if  8 keep-pose  then
    else
        drop  cp-pos aim-run
        cp-try head-at  cp-try -1 body-tri-to 0< if  to-default exit  then
        his-at cp-pos vec-dist
    then
    fdup 20e f< if  fdrop 7  else  44e f< if  8  else  9  then  then  keep-pose ;
\ Hewie_StateStepAwayFiona (action 0x67): too close to her - steps off (pose 7)
: back-off ( id -- )
    7 keep-pose
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
    7 keep-pose  his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop ;
\ Hewie_StateOffMesh (action 0x6A): his nose over an edge - walks the freest way until his head
\ and the step ahead are back over the floor
: st-off-mesh ( -- )
    7 keep-pose
    his-t1 @ if  -1 his-t1 +!
    else  30 his-t1 !  his-yaw his-head-yaw f@ f+ angle-wrap 20e 30 150 30 best-heading his-heading f!  then
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop
    cp-try head-at  cp-try -1 body-tri-to 0< if  exit  then
    his-yaw 5e cp-try his-at vec-ahead  cp-try -1 body-tri-to 0< if  exit  then
    16 his-t1 !  ['] st-walk-heading behave ;
\ Hewie_StateRun (action 0x70): a dash - running till time or room runs out, then slowing; then
\ to her (0xC calm, 0xE)
: st-run ( -- )
    his-t2 @ 0= if
        -1 his-t1 +!
        his-t1 @ 0=  his-yaw 20e free-ahead 20e f< or if  1 his-t2 !  then
        8 keep-pose exit
    then
    7 keep-pose
    settled? 0= if  exit  then
    his-waiting @ if  to-default
    else  his-danger @ 0= if  $C  else  $E  then  start  then ;
\ a fresh heading now and then (every his-t1 frames, or when the way is short): roaming, keeping away
: new-heading? ( F: room -- room ) ( -- flag )
    his-t2 @ if  -1 his-t2 +!  false exit  then
    his-t1 @ if  -1 his-t1 +!  then
    his-t1 @ 0= fdup his-turn f@ f< or ;
\ Hewie_StateRoam (action 0x15): roaming - a new heading every 90..240 frames or when the way
\ is short; looking about at a walk
: st-roam ( -- )
    his-heading f@ his-turn f@ free-ahead                             ( F: room )
    new-heading? if
        6 roll 30 * 90 + his-t1 !  30 his-t2 !
        his-yaw his-head-yaw f@ f+ angle-wrap  his-turn f@ 10e f+  30 150 30 best-heading his-heading f!
    then
    his-heading f@ run-turn turn-to                                   ( F: room left )
    5e deg>rad f< his-t3 @ 0= and if  2 look!
    else  his-heading f@ head-toward-soon  then
    gait-for keep-pose ;
\ Hewie_StateScramble (actions 0x13 / 0x64): darting about (0x204) for his-wait frames, then
\ sitting (0x1C04)
: st-scramble ( -- )
    his-wait @ 0= if  4 look!  ['] st-1770 behave exit  then
    his-t2 @ 0= if
        his-t1 @ if  -1 his-t1 +!  then
        his-t1 @ 0=  his-heading f@ his-turn f@ free-ahead his-turn f@ f< or if
            3 roll 10 * 10 + his-t1 !  30 his-t2 !  15e his-turn f!
            his-yaw 25e 150 30 30 best-heading his-heading f!
        then
    else  -1 his-t2 +!  then
    his-heading f@ head-toward-soon  his-heading f@ run-turn turn-to fdrop
    5 step-to-pose 0= if  $204 play-if-not  then ;
\ Hewie_StateKeepAway (actions 0x10 / 0x11): keeping off his target at a gait
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
\ Hewie_StateTricks (actions 0x18..0x1C): his tricks, once in the pose they need
: trick ( pose anim -- )  swap step-to-pose 0= if  play  ['] st-anim-over behave  else  drop  then ;
: st-tricks ( -- )
    his-action @ case
        $18 of  his-t1 @ 0= if  1 $1C06  else  2 $1C07  then  trick  endof
        $19 of  1 $1C00 trick  endof
        $1C of  anim@ case
                    $106 of  anim-done? if  8 play  then  endof
                    8 of  his-wait @ 0= anim-done? and if  to-default  then  endof
                    1 step-to-pose 0= if  $106 play  then
                endcase  endof
        $1A of  0 $1C08 trick  endof
        $1B of  0 $1C09 trick  endof
    endcase ;

\ ---- sniffing, facing, barking at something ----
\ Hewie_StateFaceScent (action 6): sniffing toward his target (or a scent), alert (animation 3)
: st-face-scent ( -- )
    his-target @ 0< his-alert @ 0= and if  -1  else  target-tri  then
    dup 0< his-smells @ and if  drop 0  tt-at his-scent vec-copy  then
    0< if  4 look!
    else
        tt-at close-to if  his-spot tt-at vec-copy  $57 6 want exit  then
        8 look!  tt-at look-at  his-smells @ 0= if  fswap fdrop 0e fswap  then
        his-look-yaw f!  his-look-pitch f!
        his-yaw his-look-yaw f@ f+ angle-wrap  fdup run-turn turn-to
        fswap fdrop 60e deg>rad f> if  7 keep-pose exit  then
    then
    0 step-to-pose 0= if  3 play-if-not  then ;
\ Hewie_StateFaceTarget (action 8): turned to his target, in the stance; close: 0x57
: st-face-target ( -- )
    target-tri 0< if  4 look!  stance exit  then
    tt-at close-to if  his-spot tt-at vec-copy  $57 8 want exit  then
    tt-at heading-to  fdup head-toward-soon  fdup run-turn turn-to
    fswap fdrop 60e deg>rad f> if  7 keep-pose exit  then
    stance ;
\ Hewie_StateBarkAtTarget (action 0xA): turned to his target, barks his-t1 times
: st-bark-at-target ( -- )
    his-target @ 0< his-alert @ and if  6 0 want  0 keep-pose exit  then
    target-tri 0< if  8 0 want  0 keep-pose exit  then
    tt-at close-to if  his-spot tt-at vec-copy  $57 $A want exit  then
    tt-at heading-to  fdup head-toward-soon  fdup run-turn turn-to
    fswap fdrop 60e deg>rad f> if  7 keep-pose exit  then
    0 step-to-pose 0= if
        bark  -1 his-t1 +!  his-t1 @ 0= if  to-default exit  then
    then  0 keep-pose ;
\ Hewie_StateSteer (action 7): standing alert (3), head on his target
: st-steer ( -- )
    his-alert-was @ 0= his-alert @ 0<> and if  to-default  then
    target-tri 0< his-smells @ and if  tt-at his-scent vec-copy  0  else  target-tri  then
    0< if  4 look!
    else  8 look!  tt-at look-at  his-look-yaw f!  his-smells @ if  his-look-pitch f!  else  fdrop 0e his-look-pitch f!  then  then
    0 step-to-pose 0= if  3 play-if-not  then ;
\ Hewie_StateWhine (actions 0x7D / 0x7E): standing, looking at her, a whine; then his-next
: st-whine ( -- )
    her-with? 0=  his-t1 @ 0<> his-wait @ 0= and  or if  his-next @ 0 want exit  then
    0 step-to-pose if  exit  then
    his-t1 @ 0= if
        150 his-wait !  1 his-t1 !  0 look!  her his-target !  $60 make-sound  $7D his-action !
    then  0 keep-pose ;
\ Hewie_StateLookLow / Bark (action 0x85): a bark (low when calm), then the default action
: look-target ( -- )  his-target @ 0< if  4  else  0  then  look! ;
: st-look-low ( -- )  look-target  3 step-to-pose if  exit  then  3 keep-pose  to-default ;
: st-bark ( -- )
    look-target
    his-danger @ 0= his-cond @ 1 <> and if  3  else  0  then
    step-to-pose 0= if  bark  ['] st-look-low behave  then ;

\ ---- hurt, down ----
: st-1d18 ( -- )  0 step-to-pose 0= if  0 his-2d !  0 keep-pose  to-default  then ;   \ back up
\ Hewie_StateKnockedDown (action 0x52): lying down (pose 10) until he has health again
: st-knocked-down ( -- )
    his-wait @ 0= if  1 hp!  1 cond!  then
    4 look!
    10 step-to-pose 0= if
        anim@ $1002 = if  0 his-2d !  else  10 keep-pose  then
    then
    his-hp @ if  1 his-2d !  ['] st-1d18 behave  then ;
\ Hewie_StateSlope (action 0x6E): on a slope - off it the freest way, then the default action
: st-slope ( -- )
    his-tri nav-flags 1 and 0= if  to-default exit  then
    his-t1 @ 0< if
        30 his-t1 !  his-yaw pi f+ angle-wrap 20e 30 150 30 best-heading his-heading f!
    then
    8 keep-pose  his-heading f@ head-toward-soon  his-heading f@ run-turn 1.5e deg>rad fmax turn-to fdrop ;

\ ==== H2: Fiona's side - praise, scolding, petting, her commands ====

\ Hewie_PraiseScold: praised (1) or scolded (0) for what he just did, if it is still what he
\ did: his skills at it (after fetching) move by `by`. True if it counted
: praise-scold ( praise? by -- flag )
    his-did @ 0=  his-did @ his-did-was @ <> or if  2drop false exit  then
    his-did-was @ $78 = if
        his-roll-n @ 0 ?do
            i cells his-rolls + @ >r
            r@ cells his-rolled + @ 0<>  2 pick 1 <> if  0=  then
            if  dup  else  dup negate  then  r@ cells his-skill + +!
            r> cells his-skill + dup @ 0 31 clamp swap !
        loop
    then
    2drop  0 his-follows !  0 his-did-2 !  0 his-did !  true ;

\ ---- scolded from afar (action 0x71): sitting, the ears down (0x1C04); three times within 600
\ frames upsets him ----
: st-19b0 ( -- )
    anim-done? 0= if  exit  then
    his-praise-b @ 0> 0= if  1 his-praise-a !  600 his-praise-b !
    else
        1 his-praise-a +!
        his-praise-a @ 3 <  his-mood @ 0<> or if  600 his-praise-b !
        else  20 mistreated+  2 -1 set-mood  0 his-praise-a !  then
    then  to-default ;
: st-22b8 ( -- )  1 step-to-pose 0= if  $1C04 play  ['] st-19b0 behave  then ;
\ Hewie_State1F88 (action 0x1E): won't - sits and turns his head away (0x1C01)
: st-1f88 ( -- )
    anim-done? anim@ $1C01 = and if  to-default exit  then
    1 step-to-pose 0= if  $1C01 play  then ;

\ ---- by her side: turning with her, then praised, scolded or petted ----
\ Hewie_StateTurnWithFiona (action 0x48): turns with her (to his-to-yaw) while she gestures,
\ then sits
: st-turn-with-fiona ( -- )
    her-with? 0= if  to-default exit  then
    fiona-mode @ $D <> if  to-default
    else
        his-to-yaw f@ 6e deg>rad body-turn-toward fdrop
        -1 his-t1 +!
        his-t2 @ 0<> his-t1 @ 16 < and if  1 keep-pose
        else  1 his-t2 !  $1300 play-if-not  then
    then  1 his-no-root ! ;
\ Hewie_StatePraised: at the end of each animation of 0x49 (scolded close up: three times within
\ 600 frames upsets him), 0x4A (petted: strokes heal him; she holds the praise for more),
\ 0x4B (patted)
: st-praised ( -- )
    his-danger @ 0= his-action @ $4A = and fiona-cmd @ 3 = and anim@ $1D01 = and if  2 his-t2 !  then
    anim-done? 0= if  exit  then
    his-action @ case
        $49 of
            false  his-praise-b @ 0> if
                1 his-praise-a +!
                his-praise-a @ 3 >= his-mood @ 0= and if
                    20 mistreated+  2 -1 set-mood  drop true  0 his-praise-a !
                else  600 his-praise-b !  then
            else  1 his-praise-a !  600 his-praise-b !  then
            0= his-t1 @ -6 = and if  calm-down  then
            to-default
        endof
        $4A of
            anim@ case
                $1D00 of  3 his-t2 !  3 his-pet !  $1D01 play-cut  endof
                $1D01 of  -1 his-t2 +!  -1 his-pet +!
                          his-pet @ 0= if  4 his-pet !  his-hp @ 5 + 100 min hp!  then
                          his-t2 @ 0= if  $1D02  else  $1D01  then  play-cut  endof
                $1D02 of  his-hp @ 100 = if  -8 his-t1 !  his-t3 @ 0= if  -1 mistreated+  then  then
                          his-t1 @ -8 = if  1 -1 set-mood  $1D 0 want  else  to-default  then  endof
            endcase
        endof
        $4B of  300 his-pet-time !  his-waiting @ 0= if  2  else  0  then  0 want  endof
    endcase ;
: st-2198 ( -- )
    his-action @ case
        $4B of  $1C05 play  endof
        $4A of  $1D00 play  his-hp @ 100 = if  1  else  0  then  his-t3 !  endof
        $49 of  $1C04 play  endof
    endcase  ['] st-praised behave ;

\ Hewie_State2388 (action 0x7A): waiting low (animation 9), readying
: st-2388 ( -- )
    4 look!
    anim@ 9 = if  his-t1 @ 0= if  1 his-ready !  else  -1 his-t1 +!  then
    else  0 step-to-pose 0= if  9 play  then  then ;

\ ---- walking to the spot she showed (0x63, then 0x64: scrambling about it) ----
\ Hewie_StateWalkPath2: along the planned way to his-to (straight once on its triangle), stepping
\ less the sharper he turns; near the end turning on the spot (0x1300) to his-to-yaw
fvariable wp-left  variable wp-on
: st-walk-path2 ( -- )
    his-to-tri @  his-to -1 body-tri-to = wp-on !
    stride-len
    wp-on @ if  fdrop his-to heading-to
    else  0e fmax 12e f* ahead@ drop  ahead heading-to  then        ( F: a )
    anim@ $1300 = if
        fdrop  his-to-yaw f@ head-toward
        his-to-yaw f@ his-yaw f- angle-wrap fabs 1.7453293e f* his-turn f@ f/ fsin   ( F: t )
        fdup his-turn f@ f* 0.05e f* 1e deg>rad fmax 6e deg>rad fmin                 ( F: t step )
        his-to-yaw f@ fswap body-turn-toward wp-left f!  fdrop
        wp-left f@ f0= settled? and if
            his-danger @ 0= if  0  else  3  then  5 play-blend
        then
        0.25e
    else
        fdup head-toward  run-turn body-turn-toward  fdup wp-left f!
        stride-len fdup f0< if  fdrop fdrop 0e  else  fswap pi fswap f- pi f/ f*  then
        0.05e fmax
    then
    ahead@ his-path path-i!  ahead vec@ his-place  1 his-no-root !
    settled? 0= if  exit  then
    path-done? his-t1 @ 0<> and wp-left f@ f0= and
    his-to-anim @ -1 <> if  anim@ his-to-anim @ =  else  anim-group 0=  then  and if
        his-action @ $63 = if  $64 $13 want  else  1 his-done !  to-default  then  exit
    then
    his-to-anim @ -1 <> if
        anim@ his-to-anim @ = if  rest 3e f< if  1 his-t1 !  then
        else  his-to-anim @ play  then  exit
    then
    his-t1 @ 0= if
        rest fdup 3e f< 0= if
            his-t2 @ 2 = fdup 10e f< 0= and if  34e f< if  8  else  9  then  keep-pose
            else  fdrop 7 keep-pose  then
        else
            fdrop  his-to-yaw f@ his-yaw f- angle-wrap fabs 1e deg>rad fmax his-turn f!
            1 his-t1 !  $1300 play
        then
    then ;
\ Hewie_StateSetOff: once standing, the way planned (no way: the default action)
: st-set-off ( -- )
    slide-root
    his-to-anim @ -1 = if
        0 step-to-pose if  exit  then
        his-to-tri @ his-to plan-to if
            his-t2 @ 2 = if  rest fdup 10e f< if  fdrop 7  else  34e f< if  8  else  9  then  then
            else  7  then  keep-pose
            ['] st-walk-path2 behave exit
        then
    else
        his-to-tri @ his-to plan-to if  ['] st-walk-path2 behave exit  then
    then
    to-default ;
