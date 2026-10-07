\ partner/states.fs - Hewie's behaviours (src/game/hewie.c Hewie_State*): what he does each
\ frame while an action lasts. Each is the original's state of the same name (the address-named
\ ones keep their address: st-1d98 is Hewie_State1D98); one hands over to the next with
\ `behave`, as the original sets the pointer to member +0xF35D0.
\
\ Not here yet: going for the stalkers (biting, leaping, tackles - phase 3), leaving and coming
\ into rooms off screen (doors, exits, hiding - phase 4), fetching things (action 0x78).
IN: partner.states
USING: engine game-state events.core events.words chars partner.core partner.tables partner.moves ;

\ ---- small pieces the behaviours share ----
\ Hewie_Start: the action as his situation makes it, with no argument
: start ( act -- )  adjust-action 0 set-action ;
: hard? ( -- flag )  progress pr.vars $27 + c@ 1 = ;
: trust-of ( table -- n )  h-trust @ cells + @ ;
\ obey_time: how long he obeys, by his trust
: obey-time! ( -- )  hard? if  obey-time-hard  else  obey-time  then  trust-of h-obey ! ;
\ obeys: he heard her - no more waiting, obeying for a while
: obeys ( -- )  0 h-waiting !  obey-time!  0 h-praise-due ! ;

\ the progress's counters of how he has been treated, for the dog level at the end (+0xFB6:
\ mistreated - Fiona's kicks, his being knocked down, upset; +0xFB8: pleased; 0..10000)
variable mistreated  variable pleased-count
: counter+ ( n addr -- )  tuck @ + 0 10000 clamp swap ! ;

\ Hewie_SetMode: his mood (0 normal, 1 pleased - praised from afar, 2 upset - scolded too often,
\ 3 angry - hit by Fiona) for a time (-1: the mood's own: 1 and 2 1800 frames, 3 450). Down
\ only 0; hurt only 0 or 3. Pleased, he obeys again for his trust's time
: set-mode ( mode time -- )
    swap dup >r                                                   ( time m )
    h-hp 0= h-cond 2 = and if  drop 0  then
    h-cond 1 = r@ 3 <> and if  drop 0  then
    swap dup -1 <> if  h-mood-time !  else  drop
        dup case
            0 of  0 h-mood-time !  endof
            1 of  h-mood @ 1 <> if  1 pleased-count counter+  -1 mistreated counter+  then  1800 h-mood-time !  endof
            2 of  1800 h-mood-time !  endof
            3 of  h-mood @ 3 <> if  20 mistreated counter+  then  450 h-mood-time !  endof
        endcase
    then  h-mood !
    r> 1 <> if  exit  then
    0 h-waiting !  obey-time!
    h-cmd @ $80000001 and 1 = if
        h-disabled? if  $34 start  else  1 h-pending !  then
    then
    0 h-praise-due ! ;

\ Hewie_AddTrust: his trust in her (0..10000) and its level 0..7 (from 2 the story's state flag
\ 0x11 is cleared, from 3 flag 0x1D)
create trust-bounds  100 , 280 , 450 , 600 , 750 , 900 , 1000 ,
: add-trust ( n -- )
    h-trust-points @ + 0 10000 clamp dup h-trust-points !
    0 begin  dup 7 < if  2dup cells trust-bounds + @ >=  else  false  then  while  1+  repeat
    nip h-trust !
    $11 state-flag? h-trust @ 2 >= and if  $11 state-flag-clear  then
    $1D state-flag? h-trust @ 3 >= and if  $1D state-flag-clear  then ;

\ his skills at what he does after action 0x78 (+0xF3690.., out of 32), the rolls made on them
\ (+0xF3696.., +0xF36A2 of them) and how they came out (+0xF369C..)
create h-skill 3 cells allot  16 h-skill !  16 h-skill cell+ !  16 h-skill 2 cells + !
create h-rolls 8 cells allot  create h-rolled 3 cells allot  h-rolled 3 cells 0 fill
\ Hewie_PraiseScold: praised (1) or scolded (0) for what he just did, if it is still what he
\ did: his skills at it move by `by`. True if it counted
: praise-scold ( praise? by -- flag )
    h-did @ 0=  h-did @ h-did-was @ <> or if  2drop false exit  then
    h-did-was @ $78 = if
        h-1d @ 0 ?do
            i cells h-rolls + @ >r
            r@ cells h-rolled + @ 0<>  2 pick 1 <> if  0=  then
            if  dup  else  dup negate  then  r@ cells h-skill + +!
            r> cells h-skill + dup @ 0 31 clamp swap !
        loop
    then
    2drop  0 h-follows !  0 h-did-2 !  0 h-did !  true ;

\ Hewie_RandomLevel: a gait at random (0 walk, 1 trot, 2 run; brisker on hard or when not calm)
: random-level ( -- n )
    16 roll
    hard? if
        game-mode @ 0= if  dup 8 < if  drop 0  else  13 < if  1  else  2  then  then
        else  dup 5 < if  drop 0  else  13 < if  1  else  2  then  then  then
    else
        game-mode @ 0= if  dup 10 < if  drop 0  else  15 < if  1  else  2  then  then
        else  dup 5 < if  drop 0  else  15 < if  1  else  2  then  then  then
    then ;
\ his turn rate by his gait (h-turn: degrees)
: turn-speed ( -- )
    h-t3 @ case  0 of  20e h-turn f!  endof  1 of  25e h-turn f!  endof  2 of  30e h-turn f!  endof  endcase ;

\ the stance: once standing, animation 5 when mistreated, else 4
: stance ( -- )
    0 step-to-pose 0= if  h-mood @ 2 = if  5  else  4  then  play-if-not  then ;

\ a point he is going at (his target in his room, its place; -1: none) - Hewie_TargetTri. (Going
\ for an exit the stalker went by is the stalkers' phase.)
create tt-at 12 allot
: target-tri ( -- tri | -1 )
    h-target @ dup 0< if  drop -1 exit  then
    dup with? 0= if  drop -1 exit  then
    tt-at over c-pos vec-copy  c-tri ;

\ ---- the plain ones ----
: st-nothing ( -- ) ;                          \ Hewie_State2318 / 20C8 / 20A8 / 2018: off screen
: st-pose2 ( -- )  2 keep-pose ;               \ Hewie_StatePose2
: st-pose4 ( -- )  4 keep-pose ;               \ Hewie_StatePose4
: st-loud-noise ( -- )  $80 h-room -1 noise-make ;  \ Hewie_StateLoudNoise
: st-play-anim ( -- )  h-yelp-anim @ play-if-not ;     \ Hewie_StatePlayAnim
: st-to-default ( -- )  to-default ;           \ Hewie_StateToDefault
: st-anim-over ( -- )  anim-done? if  to-default  then ;   \ Hewie_StateAnimOver (2, 3)
: st-anim-next ( -- )  anim-done? if  h-next @ start  then ;   \ Hewie_StateAnimNext
: st-2358 ( -- )  anim-done? if  0 h-2d !  to-default  then ;

\ ---- after Fiona answered him: lying down near her, then praised ----
\ Hewie_State1A60: lying low a while (h-t3), then praised for it and the default action
: st-1a60 ( -- )
    -1 h-t3 +!  h-t3 @ 0= if
        0 h-follows !  h-action @ h-did-was !  1 1 praise-scold drop  to-default
    then  3 keep-pose ;
: st-1b00 ( -- )   \ Hewie_State1B00: sat down, looking at her 90 frames
    1 step-to-pose 0= if  1 keep-pose  90 h-t3 !  her h-target !  0 look!  ['] st-1a60 behave  then ;
: st-1b60 ( -- )  anim-done? if  1 keep-pose  ['] st-1b00 behave  then ;
: st-1aa0 ( -- )  3 step-to-pose 0= if  bark  ['] st-1b60 behave  then ;
: st-1b40 ( -- )  anim-done? if  90 h-t3 !  ['] st-1a60 behave  then ;
: st-1af0 ( -- )  1 step-to-pose 0= if  $1C04 play  ['] st-1b40 behave  then ;

\ ---- running to her (action 0x6F) ----
: st-19a0 ( -- )  -1 h-t1 +!  h-t1 @ 0= if  h-next @ start  then  3 keep-pose ;
: st-1990 ( -- )
    -1 h-t1 +!  h-t1 @ 0= if  4 look!  15 h-t1 !  ['] st-19a0 behave  then  3 keep-pose ;
: st-run-to-fiona ( -- )   \ Hewie_StateRunToFiona: a happy whine, lying low by her, then on
    her with? 0= if  to-default exit  then
    3 step-to-pose if  exit  then
    $60 5 h-pos vec@ event-sound-at
    0 look!  her h-target !  3 keep-pose  30 h-t1 !  ['] st-1990 behave ;

\ ---- scolded from afar (action 0x71): sitting, the ears down (0x1C04); three times within 600
\ frames upsets him ----
: st-19b0 ( -- )
    anim-done? 0= if  exit  then
    h-praise-b @ 0> 0= if  1 h-praise-a !  600 h-praise-b !
    else
        1 h-praise-a +!
        h-praise-a @ 3 <  h-mood @ 0<> or if  600 h-praise-b !
        else  20 mistreated counter+  2 -1 set-mode  0 h-praise-a !  then
    then  to-default ;
: st-22b8 ( -- )  1 step-to-pose 0= if  $1C04 play  ['] st-19b0 behave  then ;
: st-1770 ( -- )  1 step-to-pose 0= if  $1C04 play  ['] st-anim-over behave  then ;

\ ---- idle ----
\ Hewie_State1D88: idle under a script - standing, then his move is done (down: lying)
: st-1d88 ( -- )
    h-cond 2 = if  10 step-to-pose 0= if  h-done  then  exit  then
    settled? 0= if  exit  then
    anim-group dup 10 = over 9 = or over 8 = or if  drop -1 stand-anim exit  then
    0= if  h-done  else  0 keep-pose  then ;
\ Hewie_State1D98: idle - into a low pose (stopping at a wall if walking), then choose
: st-1d98 ( -- )
    h-room played-room <> if  1 h-pending !  exit  then
    anim-group dup 10 = over 9 = or swap 8 = or if
        stride-len fdup h-yaw fswap free-ahead f> settled? and if  -1 stand-anim  then
    then
    4 step-to-pose 0= if  4 keep-pose  1 h-pending !  then ;
\ Hewie_State1DC8: by her (low) while she is here
: st-1dc8 ( -- )  her with? if  3 keep-pose  else  to-default  then ;

\ Hewie_State1E48 (action 0x14): stops and pricks his ears (0x1C02), then sniffs (6) or faces
\ what alerted him (8)
: st-1e48 ( -- )
    settled? 0= if  exit  then
    anim-done? anim@ $1C02 = and if  h-alert @ 0= if  6  else  8  then  start  exit  then
    0 step-to-pose 0= if  4 look!  $1C02 play  then ;

\ Hewie_State1F28 (action 0x1D): a bark that he heard her (lying low when calm), then the action
\ asked for (h-next)
: st-1f28 ( -- )
    game-mode @ 0= h-cond 1 <> and if  3  else  0  then
    step-to-pose 0= if  bark  ['] st-anim-next behave  then ;
\ Hewie_State1F88 (action 0x1E): won't - sits and turns his head away (0x1C01)
: st-1f88 ( -- )
    anim-done? anim@ $1C01 = and if  to-default exit  then
    1 step-to-pose 0= if  $1C01 play  then ;
\ Hewie_State2168 / 1CC8 / 1C48 (action 0x45, a script's bark)
: st-1cc8 ( -- )  3 step-to-pose 0= if  h-done  3 keep-pose  to-default  then ;
: st-1c48 ( -- )  0 step-to-pose 0= if  h-done  0 keep-pose  to-default  then ;
: st-2168 ( -- )
    game-mode @ 0= h-cond 1 <> and if
        3 step-to-pose 0= if  bark  ['] st-1cc8 behave  then
    else  0 step-to-pose 0= if  bark  ['] st-1c48 behave  then  then ;

\ ---- the commands' helpers (actions 0x26..0x2B): into the pose, then follow (2) or stay by
\ her (1, or 5 while waiting) ----
: after-helper ( kind act -- )
    swap dup step-to-pose if  2drop exit  then  keep-pose
    h-action @ = if  2 start  else  h-waiting @ 0= if  1  else  5  then  start  then ;
: st-after-2b ( -- )  2 $2B after-helper ;
: st-after-29 ( -- )  1 $29 after-helper ;
: st-after-27 ( -- )  0 $27 after-helper ;

\ ---- praised, petted ----
\ Hewie_StatePraised: at the end of each animation of 0x49 (scolded close up: three times within
\ 600 frames upsets him), 0x4A (petted: strokes heal him), 0x4B (patted)
: st-praised ( -- )
    game-mode @ 0= h-action @ $4A = and fiona-cmd @ 3 = and anim@ $1D01 = and if  2 h-t2 !  then
    anim-done? 0= if  exit  then
    h-action @ case
        $49 of
            false  h-praise-b @ 0> if
                1 h-praise-a +!
                h-praise-a @ 3 >= h-mood @ 0= and if
                    20 mistreated counter+  2 -1 set-mode  drop true  0 h-praise-a !
                else  600 h-praise-b !  then
            else  1 h-praise-a !  600 h-praise-b !  then
            0= h-t1 @ -6 = and if  0 -1 set-mode  0 h-hits !  0 h-hit-t !  then
            0 0 want
        endof
        $4A of
            anim@ case
                $1D00 of  3 h-t2 !  3 h-pet !  $1D01 play-cut  endof
                $1D01 of  -1 h-t2 +!  -1 h-pet +!
                          h-pet @ 0= if  4 h-pet !  h-hp 5 + 100 min him character char.hp l!  then
                          h-t2 @ 0= if  $1D02  else  $1D01  then  play-cut  endof
                $1D02 of  h-hp 100 = if  -8 h-t1 !  h-t3 @ 0= if  -1 mistreated counter+  then  then
                          h-t1 @ -8 = if  1 -1 set-mode  $1D 0 want  else  0 0 want  then  endof
            endcase
        endof
        $4B of  300 h-pet-time !  h-waiting @ 0= if  2  else  0  then  0 want  endof
    endcase ;
: st-2198 ( -- )
    h-action @ case
        $4B of  $1C05 play  endof
        $4A of  $1D00 play  h-hp 100 = if  1  else  0  then  h-t3 !  endof
        $49 of  $1C04 play  endof
    endcase  ['] st-praised behave ;

\ ---- keeping his distance (actions 0x53..0x57, 0x7C) ----
create kn-at 12 allot
: st-keep-near ( -- )
    h-wanted @ if  -1 h-wanted +!  then
    h-action @ case
        $56 of  kn-at h-spot vec-copy  endof
        $57 of  kn-at h-spot vec-copy  endof
        \ 0x53 / 0x54 / 0x55 / 0x7C: his target, while he can get to it
        h-target @ dup with? 0= if  2drop 0 0 want exit  then
        dup c-tri over c-pos plan-to 0= if  2drop 0 0 want exit  then
        c-pos kn-at swap vec-copy
    endcase
    h-pos kn-at vec-dist h-turn f@ f> if  h-next @ 0 want exit  then
    h-heading f@ h-turn f@ free-ahead                               ( F: room )
    h-t2 @ if  -1 h-t2 +!
    else
        h-t1 @ if  -1 h-t1 +!  then
        h-t1 @ 0= fdup h-turn f@ f< or if
            h-action @ dup $57 = swap $55 = or if
                3 roll 30 * 30 + h-t1 !  30 h-t2 !
            else  0 h-t1 !  then
            him kn-at c-heading-to pi f+ angle-wrap  h-turn f@ 10e f+  30 150 30 best-heading
            h-heading f!
        then
    then
    h-heading f@ head-toward-soon
    h-heading f@ run-turn turn-to fdrop
    h-action @ dup $57 = swap $55 = or if  fdrop 7 keep-pose exit  then
    5 step-to-pose 0= if
        fdup 10e f< if  fdrop $200  else  20e f< if  $201  else  $202  then  then  play-if-not
    else  fdrop  then ;

\ ---- by her ----
\ Hewie_StateWaitForFiona (action 9): turned to her, standing by in the stance; close: 0x55
: st-wait-for-fiona ( -- )
    her with? 0= if  0 0 want exit  then
    h-pos her c-pos vec-dist 10e f< if  $55 9 want exit  then
    him her c-pos c-heading-to fdup run-turn turn-to
    fswap fdrop  60e deg>rad f> if  7 keep-pose exit  then
    stance ;
\ Hewie_StateBarkAtFiona (action 0xB): turned to her, barks h-t1 times; close: 0x55
: st-bark-at-fiona ( -- )
    her with? 0= if  0 0 want exit  then
    h-pos her c-pos vec-dist 10e f< if  $55 $B want exit  then
    him her c-pos c-heading-to fdup run-turn turn-to
    fswap fdrop  60e deg>rad f> if  7 keep-pose exit  then
    0 step-to-pose 0= if  bark  -1 h-t1 +!  h-t1 @ 0= if  0 0 want  then  then ;

\ Hewie_State1740: by her a while (h-t2), lying low looking at her; she moved off: again
: st-1740 ( -- )
    -1 h-t2 +!  h-t2 @ 0= if  to-default exit  then
    h-pos her c-pos vec-dist 20e f> if  h-action @ start  then
    her h-target !  0 look!  3 keep-pose ;
\ Hewie_StateComeToFiona: to her side (follow-step); there, the next action (or by her 60 frames)
: st-come-to-fiona ( -- )
    her with? 0= if  0 0 want exit  then
    40e 30e follow-step 1 = if
        h-next @ 0= if  60 h-t2 !  ['] st-1740 behave  else  h-next @ 0 want  then
    then ;
\ Hewie_StateSlideToFiona (actions 0xC..0xF): up from lying low, the way to her side planned;
\ close: there, else come at the gait for the distance
: st-slide-to-fiona ( -- )
    her with? 0= if  0 0 want exit  then
    3 step-to-pose if  exit  then
    3 keep-pose  slide-root
    h-action @ command-place cp-pos plan-and-go if  0 0 want exit  then
    rest  fdup 12e f< if  fdrop path-end
        h-next @ 0= if  60 h-t2 !  ['] st-1740 behave  else  h-next @ 0 want  then  exit
    then
    h-cond 1 = fdup 20e f< or if  fdrop 0  else  30e f< if  1  else  2  then  then  h-t1 !
    ['] st-come-to-fiona behave ;

\ Hewie_StateComeToCommand (action 0x16): to his place by her - along the way, straight once
\ there; the gait by the distance left
: st-come-to-command ( -- )
    her with? 0= if  0 0 want exit  then
    h-action @ command-place                                        ( tri )
    dup him cp-pos -1 c-tri-to <> if
        cp-pos plan-and-go if  0 0 want exit  then
        rest  stride 0= if  8 keep-pose  then
    else
        drop  cp-pos aim-run
        cp-try head-at  him cp-try -1 c-tri-to 0< if  0 0 want exit  then
        h-pos cp-pos vec-dist
    then
    fdup 20e f< if  fdrop 7  else  44e f< if  8  else  9  then  then  keep-pose ;

\ Hewie_StateStepAwayFiona (action 0x67): too close to her - steps off (pose 7)
: back-off ( cs -- )
    7 keep-pose
    c-pos  him swap c-heading-to pi f+ angle-wrap                   ( F: away )
    h-t1 @ if  fdrop -1 h-t1 +!
    else  30 h-t1 !  20e 30 150 30 best-heading h-heading f!  then
    h-heading f@ head-toward-soon
    h-heading f@ run-turn turn-to fdrop ;
: st-step-away-fiona ( -- )
    her with? 0= if  0 0 want exit  then
    h-pos her c-pos vec-dist 6e f> if  0 0 want exit  then
    her back-off ;

\ Hewie_StateTurnWithFiona (action 0x48): turns with her (to h-to-yaw) while she gestures
: st-turn-with-fiona ( -- )
    her with? 0= if  to-default exit  then
    her c-mode $D <> if  to-default
    else
        him h-to-yaw f@ 6e deg>rad c-turn-toward fdrop
        -1 h-t1 +!
        h-t2 @ 0<> h-t1 @ 16 < and if  1 keep-pose
        else  1 h-t2 !  $1300 play-if-not  then
    then  1 h-no-root ! ;

\ ---- on his own ----
\ Hewie_StateWalkHeading: walks along h-heading for h-t1 frames
: st-walk-heading ( -- )
    -1 h-t1 +!  h-t1 @ 0= if  0 0 want  then
    7 keep-pose  h-heading f@ head-toward-soon  h-heading f@ run-turn turn-to fdrop ;
\ Hewie_StateOffMesh (action 0x6A): his nose over an edge - walks the freest way until his head
\ and the step ahead are back over the floor
: st-off-mesh ( -- )
    7 keep-pose
    h-t1 @ if  -1 h-t1 +!
    else  30 h-t1 !  h-yaw h-head-yaw f@ f+ angle-wrap 20e 30 150 30 best-heading h-heading f!  then
    h-heading f@ head-toward-soon  h-heading f@ run-turn turn-to fdrop
    cp-try head-at  him cp-try -1 c-tri-to 0< if  exit  then
    cp-try h-pos h-yaw 5e vec-ahead  him cp-try -1 c-tri-to 0< if  exit  then
    16 h-t1 !  ['] st-walk-heading behave ;

\ Hewie_StateRun (action 0x70): a dash - running till time or room runs out, then slowing; then
\ to her (0xC calm, 0xE)
: st-run ( -- )
    h-t2 @ 0= if
        -1 h-t1 +!
        h-t1 @ 0=  h-yaw 20e free-ahead 20e f< or if  1 h-t2 !  then
        8 keep-pose exit
    then
    7 keep-pose
    settled? 0= if  exit  then
    h-waiting @ if  to-default
    else  game-mode @ 0= if  $C  else  $E  then  start  then ;

\ Hewie_StateRoam (action 0x15): roaming - a new heading every 90..240 frames or when the way
\ is short; looking about at a walk
: st-roam ( -- )
    h-heading f@ h-turn f@ free-ahead                                ( F: room )
    h-t2 @ if  -1 h-t2 +!
    else
        h-t1 @ if  -1 h-t1 +!  then
        h-t1 @ 0= fdup h-turn f@ f< or if
            6 roll 30 * 90 + h-t1 !  30 h-t2 !
            h-yaw h-head-yaw f@ f+ angle-wrap  h-turn f@ 10e f+  30 150 30 best-heading h-heading f!
        then
    then
    h-heading f@ run-turn turn-to                                    ( F: room left )
    5e deg>rad f< h-t3 @ 0= and if  2 look!
    else  h-heading f@ head-toward-soon  then
    h-t3 @ 0= fdup 10e f< or if  fdrop 7
    else  h-t3 @ 1 = 20e f< or if  8  else  9  then  then  keep-pose ;

\ Hewie_StateScramble (actions 0x13 / 0x64): darting about (0x204) for h-wait frames, then
\ sitting (0x1C04)
: st-scramble ( -- )
    h-wait @ 0= if  4 look!  ['] st-1770 behave exit  then
    h-t2 @ 0= if
        h-t1 @ if  -1 h-t1 +!  then
        h-t1 @ 0=  h-heading f@ h-turn f@ free-ahead h-turn f@ f< or if
            3 roll 10 * 10 + h-t1 !  30 h-t2 !  15e h-turn f!
            h-yaw 25e 150 30 30 best-heading h-heading f!
        then
    else  -1 h-t2 +!  then
    h-heading f@ head-toward-soon  h-heading f@ run-turn turn-to fdrop
    5 step-to-pose 0= if  $204 play-if-not  then ;

\ Hewie_StateKeepAway (actions 0x10 / 0x11): keeping off his target at a gait
: st-keep-away ( -- )
    h-target @ dup with? 0= if  drop 0 0 want exit  then
    dup c-tri swap c-pos plan-to 0= if  0 0 want exit  then
    h-heading f@ h-turn f@ free-ahead                                ( F: room )
    h-t2 @ if  -1 h-t2 +!
    else
        h-t1 @ if  -1 h-t1 +!  then
        h-t1 @ 0= fdup h-turn f@ f< or if
            5 roll 30 * 30 + h-t1 !  30 h-t2 !
            him h-target @ c-pos c-heading-to pi f+ angle-wrap  h-turn f@ 10e f+  30 150 30 best-heading
            fdup h-heading f!  h-yaw f- angle-wrap fabs 170e deg>rad f> if  fdrop 0 0 want exit  then
        then
    then
    h-heading f@ head-toward-soon  h-heading f@ run-turn turn-to fdrop
    h-t3 @ 0= fdup 10e f< or if  fdrop 7
    else  h-t3 @ 1 = 20e f< or if  8  else  9  then  then  keep-pose ;

\ Hewie_StateTricks (actions 0x18..0x1C): his tricks, once in the pose they need
: st-tricks ( -- )
    h-action @ case
        $18 of  h-t1 @ 0= if
                    1 step-to-pose 0= if  $1C06 play  ['] st-anim-over behave  then
                else  2 step-to-pose 0= if  $1C07 play  ['] st-anim-over behave  then  then  endof
        $19 of  1 step-to-pose 0= if  $1C00 play  ['] st-anim-over behave  then  endof
        $1C of  anim@ case
                    $106 of  anim-done? if  8 play  then  endof
                    8 of  h-wait @ 0= anim-done? and if  0 0 want  then  endof
                    1 step-to-pose 0= if  $106 play  then
                endcase  endof
        $1A of  0 step-to-pose 0= if  $1C08 play  ['] st-anim-over behave  then  endof
        $1B of  0 step-to-pose 0= if  $1C09 play  ['] st-anim-over behave  then  endof
    endcase ;

\ Hewie_StateFaceScent (action 6): sniffing toward his target (or a scent), alert (animation 3)
: st-face-scent ( -- )
    h-target @ 0< h-alert @ 0= and if  -1  else  target-tri  then
    dup 0< h-look @ and if  drop 0  tt-at h-scent vec-copy  then
    0< if  4 look!
    else
        h-pos tt-at vec-dist 10e f< if  h-spot tt-at vec-copy  $57 6 want exit  then
        8 look!  tt-at look-at  h-look @ 0= if  fswap fdrop 0e fswap  then
        h-look-yaw f!  h-look-pitch f!
        h-yaw h-look-yaw f@ f+ angle-wrap  fdup run-turn turn-to
        fswap fdrop 60e deg>rad f> if  7 keep-pose exit  then
    then
    0 step-to-pose 0= if  3 play-if-not  then ;
\ Hewie_StateFaceTarget (action 8): turned to his target, in the stance; close: 0x57
: st-face-target ( -- )
    target-tri 0< if  4 look!  stance exit  then
    h-pos tt-at vec-dist 10e f< if  h-spot tt-at vec-copy  $57 8 want exit  then
    him tt-at c-heading-to  fdup head-toward-soon  fdup run-turn turn-to
    fswap fdrop 60e deg>rad f> if  7 keep-pose exit  then
    stance ;
\ Hewie_StateBarkAtTarget (action 0xA): turned to his target, barks h-t1 times
: st-bark-at-target ( -- )
    h-target @ 0< h-alert @ and if  6 0 want  0 keep-pose exit  then
    target-tri 0< if  8 0 want  0 keep-pose exit  then
    h-pos tt-at vec-dist 10e f< if  h-spot tt-at vec-copy  $57 $A want exit  then
    him tt-at c-heading-to  fdup head-toward-soon  fdup run-turn turn-to
    fswap fdrop 60e deg>rad f> if  7 keep-pose exit  then
    0 step-to-pose 0= if
        bark  -1 h-t1 +!  h-t1 @ 0= if  0 0 want exit  then
    then  0 keep-pose ;

\ Hewie_StateSteer (action 7): standing alert (3), head on his target
: st-steer ( -- )
    h-alert-was @ 0= h-alert @ 0<> and if  to-default  then
    target-tri 0< h-look @ and if  tt-at h-scent vec-copy  0  else  target-tri  then
    0< if  4 look!
    else  8 look!  tt-at look-at  h-look-yaw f!  h-look @ if  h-look-pitch f!  else  fdrop 0e h-look-pitch f!  then  then
    0 step-to-pose 0= if  3 play-if-not  then ;
\ Hewie_State2388 (action 0x7A): waiting low (animation 9), readying
: st-2388 ( -- )
    4 look!
    anim@ 9 = if  h-t1 @ 0= if  1 h-ready !  else  -1 h-t1 +!  then
    else  0 step-to-pose 0= if  9 play  then  then ;

\ Hewie_StateWhine (actions 0x7D / 0x7E): standing, looking at her, a whine; then h-next
: st-whine ( -- )
    her with? 0=  h-t1 @ 0<> h-wait @ 0= and  or if  h-next @ 0 want exit  then
    0 step-to-pose if  exit  then
    h-t1 @ 0= if
        150 h-wait !  1 h-t1 !  0 look!  her h-target !  $60 make-sound  $7D h-action !
    then  0 keep-pose ;

\ Hewie_StateLookLow / Bark (action 0x85): a bark (low when calm), then the default action.
\ (Telling Fiona of the doors he may use waits for the doors' phase.)
: st-look-low ( -- )
    h-target @ 0< if  4 look!  else  0 look!  then
    3 step-to-pose if  exit  then  3 keep-pose  0 0 want ;
: st-bark ( -- )
    h-target @ 0< if  4 look!  else  0 look!  then
    game-mode @ 0= h-cond 1 <> and if
        3 step-to-pose 0= if  bark  ['] st-look-low behave  then
    else  0 step-to-pose 0= if  bark  ['] st-look-low behave  then  then ;

\ ---- walking to a spot (actions 0x3F / 0x40 / 0x63) ----
\ Hewie_StateWalkPath2: along the planned way to h-to (straight once on its triangle), stepping
\ less the sharper he turns; near the end turning on the spot (0x1300) to h-to-yaw. 0x63 then
\ 0x64 (scrambling about the spot)
fvariable wp-left  variable wp-on
: st-walk-path2 ( -- )
    h-to-tri @  him h-to -1 c-tri-to = wp-on !
    stride-len
    wp-on @ if  fdrop him h-to c-heading-to
    else  0e fmax 12e f* him path-ahead drop  him pa-pos c-heading-to  then   ( F: a )
    anim@ $1300 = if
        fdrop  h-to-yaw f@ head-toward
        h-to-yaw f@ h-yaw f- angle-wrap fabs 1.7453293e f* h-turn f@ f/ fsin   ( F: t )
        fdup h-turn f@ f* 0.05e f* 1e deg>rad fmax 6e deg>rad fmin            ( F: t step )
        him h-to-yaw f@ frot c-turn-toward wp-left f!  fdrop
        wp-left f@ f0= settled? and if
            game-mode @ 0= if  0  else  3  then  5 play-blend
        then
        0.25e
    else
        fdup head-toward  run-turn him c-turn-toward  fdup wp-left f!
        stride-len fdup f0< if  fdrop fdrop 0e  else  fswap pi fswap f- pi f/ f*  then
        0.05e fmax
    then
    him path-ahead him path-i!  pa-pos vec@ him c-place!  1 h-no-root !
    settled? 0= if  exit  then
    path-done? h-t1 @ 0<> and wp-left f@ f0= and
    h-to-anim @ -1 <> if  anim@ h-to-anim @ =  else  anim-group 0=  then  and if
        h-action @ $63 = if  $64 $13 want  else  h-done  0 0 want  then  exit
    then
    h-to-anim @ -1 <> if
        anim@ h-to-anim @ = if  rest 3e f< if  1 h-t1 !  then
        else  h-to-anim @ play  then  exit
    then
    h-t1 @ 0= if
        rest fdup 3e f< 0= if
            h-t2 @ 2 = fdup 10e f< 0= and if  34e f< if  8  else  9  then  keep-pose
            else  fdrop 7 keep-pose  then
        else
            fdrop  h-to-yaw f@ h-yaw f- angle-wrap fabs 1e deg>rad fmax h-turn f!
            1 h-t1 !  $1300 play
        then
    then ;
\ Hewie_StateSetOff: once standing, the way planned (no way: the default action)
: st-set-off ( -- )
    slide-root
    h-to-anim @ -1 = if
        0 step-to-pose if  exit  then
        h-to-tri @ h-to plan-to if
            h-t2 @ 2 = if  rest fdup 10e f< if  fdrop 7  else  34e f< if  8  else  9  then  then
            else  7  then  keep-pose
            ['] st-walk-path2 behave exit
        then
    else
        h-to-tri @ h-to plan-to if  ['] st-walk-path2 behave exit  then
    then
    0 0 want ;

\ ---- hurt, down ----
\ Hewie_State1D18: back up (standing), then the default action
: st-1d18 ( -- )
    0 step-to-pose 0= if  0 h-2d !  0 keep-pose  to-default  then ;
\ Hewie_StateKnockedDown (action 0x52): lying down (pose 10) until he has health again. (The
\ game over his being down brings waits for the stalkers' phase.)
: st-knocked-down ( -- )
    h-wait @ 0= if  1 him character char.hp l!  1 him character char.cond l!  then
    4 look!
    10 step-to-pose 0= if
        anim@ $1002 = if  0 h-2d !  else  10 keep-pose  then
    then
    h-hp if  1 h-2d !  ['] st-1d18 behave  then ;

\ Hewie_StateSlope (action 0x6E): on a slope - off it the freest way, then the default action.
\ (Through a door he may use there: the doors' phase.)
: st-slope ( -- )
    h-tri nav-flags 1 and 0= if  0 0 want exit  then
    h-t1 @ 0< if
        30 h-t1 !  h-yaw pi f+ angle-wrap 20e 30 150 30 best-heading h-heading f!
    then
    8 keep-pose  h-heading f@ head-toward-soon  h-heading f@ run-turn 1.5e deg>rad fmax turn-to fdrop ;

\ ---- under a script (the moves Hewie_Requests turns into actions 0x3B..0x47, 0x7F) ----
\ the playback rate of his motion (the motion's +0x6A4 +0x1C)
: rate! ( F: r -- )  h-actor dup 0< if  drop fdrop exit  then  actor act.rate sf! ;
\ Hewie_StateRootMotion (0x3B..0x3E, 0x47): the animation's root motion moves him (0x47: not
\ at all; through blocked floor, +0x2B, its height too)
: st-root-motion ( -- )
    h-action @ $47 = if  1 h-no-root !  exit  then
    h-2b @ if
        h-pos 4 + sf@  root@  him rm-x f@ rm-z f@ c-move-local  rm-y f@ f+ h-pos 4 + sf!  him c-sync
        1 h-no-root !  0 h-root-ok !
    then ;
\ Hewie_StateWalkPath (0x41 / 0x42): along the planned way to h-to, by his gait (7 / 9) or
\ animation h-to-anim; at the end the rest of the step, done
create wk-at 12 allot
: st-walk-path ( -- )
    settled? if
        h-to-anim @ -1 <> if  h-to-anim @ play-if-not
        else  h-action @ $41 = if  7  else  9  then  keep-pose  then
    then
    rest                                                              ( F: rest )
    h-to-tri @  him h-to -1 c-tri-to <> if
        h-action @ $3F = if  10e  else  20e  then  him path-ahead drop  him pa-pos c-heading-to
    else  him h-to c-heading-to  then                                ( F: rest a )
    fdup head-toward-soon  run-turn him c-turn-toward                ( F: rest left )
    stride-len fdup f0< if  fdrop fdrop 0e  else  fswap pi fswap f- pi f/ f*  then   ( F: rest mv )
    him path-ahead him path-i!  pa-pos vec@ him c-place!  1 h-no-root !
    path-done? if
        stride-len fswap fover fover f> if  f- 0e fswap him c-move-local  else  fdrop fdrop  then
        h-done  0 0 want exit
    then  fdrop ;
\ Hewie_State2138: his way to the triangle h-to-tri's middle, then walk it
: st-2138 ( -- )
    slide-root
    h-to-tri @ dup 0< if  drop to-default exit  then  tri-center h-to vec!
    h-to-tri @ h-to plan-to if  4 look!  ['] st-walk-path behave  else  to-default  then ;

\ Hewie_StateTurnStart / Turning / State1C98 (0x43 / 0x44): once stopped, turning on the spot
\ (0x1300, eased) to h-to-yaw; then standing again
: st-1c98 ( -- )
    settled? 0= if  exit  then
    anim@ $1300 = if  game-mode @ 0= if  0  else  3  then  5 play-blend  1e rate!  exit  then
    h-done  to-default ;
: st-turning ( -- )
    1 h-no-root !  0e h-look-pitch f!  h-to-yaw f@ head-toward
    h-to-yaw f@ h-yaw f- angle-wrap fabs pi f* h-heading f@ f/ fsin             ( F: s )
    fdup h-heading f@ f* f2* pi f/ 0.6e fmin rate!
    h-heading f@ f* 0.05e f* 0.5e deg>rad fmax                                    ( F: step )
    him h-to-yaw f@ fswap c-turn-toward f0= if  ['] st-1c98 behave  then ;
: st-turn-start ( -- )
    1 h-no-root !
    settled? 0= if  exit  then
    h-to-yaw f@ h-yaw f- angle-wrap fabs
    fdup 1e deg>rad f< if  fdrop h-to-yaw f@ h-yaw!  ['] st-1c98 behave exit  then
    $1300 play  0e rate!
    1.01e f* h-heading f!  ['] st-turning behave ;

\ Hewie_StateHeadForSpot / Wander (0x46): to the spot h-to (gait 7, head on it); there, milling
\ about it
defer st-wander
: st-head-for-spot ( -- )
    h-pos h-to vec-dist-xz 10e f< if
        0 h-t1 !  0 h-t2 !  h-yaw h-heading f!  ['] st-wander behave exit
    then
    him h-to c-heading-to  run-turn turn-to                          ( F: left )
    h-to look-at h-look-yaw f! h-look-pitch f!  8 look-now!
    1.0471976e f<= if  0 step-to-pose 0= if  h-done  then  else  7 keep-pose  then ;
:noname ( -- )   \ Hewie_StateWander
    h-pos h-to vec-dist-xz 15e f> if  ['] st-head-for-spot behave exit  then
    h-t2 @ if  -1 h-t2 +!
    else
        h-t1 @ if  -1 h-t1 +!  then
        h-heading f@ 20e free-ahead 20e f<  h-t1 @ 0= or if
            3 roll 30 * 30 + h-t1 !  30 h-t2 !
            him h-to c-heading-to pi f+ angle-wrap  30e 30 150 30 best-heading h-heading f!
        then
    then
    h-heading f@ head-toward-soon  h-heading f@ run-turn turn-to fdrop  7 keep-pose ;
is st-wander

\ ---- the leap (0x7F): Hewie_StateRunForSpot / Leap / TurnTo / 1C48 ----
fvariable lp-y  fvariable lp-fall  fvariable lp-rise  fvariable lx  fvariable lz  fvariable ll
create lp-dir 12 allot
: st-turn-to ( -- )
    h-to-yaw f@ h-yaw f- angle-wrap  fdup fabs 30e deg>rad f<  h-head-yaw f@ fover f* f0< 0= or if
        fdrop him h-to-yaw f@ 10e deg>rad c-turn-toward fdrop
    else  f0< if  h-yaw 10e deg>rad f+  else  h-yaw 10e deg>rad f-  then  angle-wrap h-yaw!  then
    anim-done? if
        h-t2 @ 0= if  0 h-2d !  then
        -1 stand-anim  ['] st-1c48 behave
    then
    root@  lp-dir vec@ rm-z f@ f* frot rm-z f@ f* frot frot  fswap fdrop  him c-move-any
    1 h-no-root ! ;
: st-leap ( -- )
    h-to-yaw f@ 10e deg>rad turn-to fdrop
    anim-done? anim@ $1E05 <> and if  $1E05 play-cut  1.2889e lp-fall f!  then
    anim@ $1E04 = if
        root@  lp-dir vec@ rm-z f@ f* frot rm-z f@ f* frot frot fswap fdrop  him c-move-any
        lp-y f@ rm-y f@ lp-rise f@ f* f+                                   ( F: y )
    else
        lp-dir vec@ 3e f* frot 3e f* frot frot fswap fdrop  him c-move-any
        lp-fall f@ 0.5e f+ 3e fmin lp-fall f!
        lp-y f@ lp-fall f@ f-
    then
    1 h-no-root !
    h-pos 4 + sf@ fover f> if   \ below the ground: landed
        fdrop $1E06 play-cut  ['] st-turn-to behave exit
    then
    fdup lp-y f!  h-pos 4 + sf!  him c-sync ;
: st-run-for-spot ( -- )
    h-to-tri @ h-to plan-and-go if  0 0 want exit  then
    settled? rest h-to-anim @ s>f f< and if
        h-to sf@ h-pos sf@ f- lx f!  h-to 8 + sf@ h-pos 8 + sf@ f- lz f!
        lx f@ fsq lz f@ fsq f+ fsqrt  fdup f0= if  fdrop 1e  then  ll f!
        lx f@ ll f@ f/  0e  lz f@ ll f@ f/  lp-dir vec!
        h-pos 4 + sf@ lp-y f!
        him h-to c-heading-to fdup h-to-yaw f!  20e deg>rad turn-to fdrop
        $1E04 play  1 h-2d !  1 h-no-root !
        ['] st-leap behave exit
    then
    5 step-to-pose 0= if  $202 play-if-not  then
    stride 0= if  8 keep-pose  then ;

\ ---- behind Fiona (action 0x72, the scripts' hewie-face): to the place behind her facing her way
\ (h-to-yaw), then settled there exactly (Hewie_StateKeepBehind / BackToNormal / BehindFiona /
\ ByFiona / PlayToEnd). (The original plays the event motion 0x8000 while he settles; without
\ one he takes 10 frames.) ----
create kb-at 12 allot  variable kb-tri  fvariable kb-step  fvariable kb-turn
: her-free? ( -- flag )  her with?  her character char.scripted sl@ 0= and ;
\ the place behind her: meet-offsets kind 0 in her frame by h-to-yaw (her own place when that
\ isn't open floor)
: behind-spot ( -- )
    meet-offsets f@  meet-offsets 1 floats + f@                       ( F: ox oz )
    fover h-to-yaw f@ fcos f*  fover h-to-yaw f@ fsin f* f+  her c-pos sf@ f+  kb-at sf!
    fswap h-to-yaw f@ fsin f* fnegate  fswap h-to-yaw f@ fcos f* f+  her c-pos 8 + sf@ f+  kb-at 8 + sf!
    her c-pos 4 + sf@ kb-at 4 + sf!
    kb-at $29020008 v-tri-in dup 0< if  drop her c-tri  kb-at her c-pos vec-copy  then  kb-tri ! ;
: st-play-to-end ( -- )
    her-free? 0= if  to-default exit  then
    anim-done? if  to-default  then  1 h-no-root ! ;
: st-by-fiona ( -- )
    her-free? 0= if  0 0 want exit  then
    -1 h-t1 +!  h-t1 @ 0<= if
        kb-at vec@ him c-place!  kb-turn f@ h-yaw!  ['] st-play-to-end behave
    else
        him kb-turn f@ h-f36cc f@ c-turn-toward fdrop
        kb-step f@ him path-ahead him path-i!  pa-pos vec@ him c-place!
    then  1 h-no-root ! ;
: req@-7? ( -- flag )   \ his state block holds 7 (then cleared)
    him character char.req sl@ 7 = dup if  0 him character char.req l!  then ;
: st-behind-fiona ( -- )
    req@-7? if  0 0 want exit  then
    her-free? 0= if  0 0 want exit  then
    behind-spot
    kb-tri @ kb-at plan-and-go if  0 0 want exit  then
    rest 0.1e f* kb-step f!
    h-to-yaw f@ pi f+ angle-wrap fdup kb-turn f!  h-yaw f- angle-wrap fabs 0.1e f* h-f36cc f!
    10 h-t1 !  1 h-no-root !  ['] st-by-fiona behave ;
: st-back-to-normal ( -- )
    her-free? if  ['] st-behind-fiona behave  else  to-default  then ;
: st-keep-behind ( -- )
    her-free? 0= if  0 0 want exit  then
    behind-spot
    h-pos kb-at vec-dist 10e f< if
        h-to-yaw f@ pi f+ h-yaw f- angle-wrap fabs pi f2/ f< if
            4 look-now!  ['] st-back-to-normal behave  exit
        then
    then
    path-done? if  kb-tri @ kb-at plan-and-go if  exit  then  then
    stride if  $202  else  $201  then  play-if-not ;
