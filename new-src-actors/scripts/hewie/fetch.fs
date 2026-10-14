\ hewie/fetch.fs - fetching (action 0x78: Hewie_StateFetch, StateFollowMover and the states
\ after; Hewie_RollReaction / RollReactions; Hewie_NearestThing). Fiona throws the ball: his to
\ fetch (`fetch-this`, things.fs) - or she sends him for the nearest (her command 0x23). By his
\ skills (his-skill, out of 32: rolls against them, `try-skill`) he goes after it (eager or not),
\ else watches it go; at it, he picks it up and brings it back, or comes to tell her, or leaves
\ it; lost (gone, or lying where he can't get), he comes back to her or doesn't. Her praise or
\ scolding after (praise-scold) moves the skills he used up or down.
IN: hewie.fetch
USING: engine actors messages common paths hewie.state hewie.body hewie.model hewie.moving hewie.states ;

\ ---- his skills: a roll (of 32) against one, kept for her praise (Hewie_Roll) ----
: try-skill ( kind -- hit? )
    dup cells his-skill + @  32 roll >                               ( kind hit? )
    over cells his-rolled + over swap !
    swap  his-roll-n @ cells his-rolls + !  1 his-roll-n +! ;
0 constant going   1 constant watching   2 constant eager
3 constant picking-up   4 constant telling   5 constant coming-back

\ ---- the ball he fetches (his-follows) ----
: ball ( -- i )  his-follows @ ;
: ball-room ( -- room )  ball thing-info 2drop drop nip ;
: ball-there? ( -- flag )   \ (lying about still, in his room)
    ball 0< if  false exit  then  ball thing-used? 0= if  false exit  then  ball-room self body-room = ;
: ball-at ( -- v )  ball thing-at ;
: ball-tri ( -- tri )  ball thing-info 2drop nip nip ;
: ball-speed ( F: -- s )  ball thing-speed ;
: on-ball-line? ( mask -- flag )   \ (Actor_TriTo: straight from him onto its triangle)
    >r  his-tri his-at ball-at r> v-walk  ball-tri = ;
: forget-ball ( -- )  -1 his-follows ! ;
: eyes-on ( v -- )  look-at his-look-yaw f! his-look-pitch f!  8 look! ;   \ (Motion_LookAt: his head on it)

\ ---- after: sitting by her a while, then his doing counted as praised (Hewie_State1A60) ----
: st-1a60 ( -- )
    -1 his-t3 +!  his-t3 @ 0= if
        forget-ball  his-action @ his-did-was !  1 1 praise-scold drop  to-default exit
    then  3 keep-pose ;
: sit-by ( -- )  90 his-t3 !  ['] st-1a60 behave ;
\ by her, sat up (pose 1), looking at her (Hewie_State1B00); or ears down first ($1C04, State1AF0)
: st-1b00 ( -- )  1 step-to-pose if  exit  then  1 keep-pose  her his-target !  0 look!  sit-by ;
: st-1b40 ( -- )  anim-done? if  sit-by  then ;
: st-1af0 ( -- )  1 step-to-pose if  exit  then  $1C04 play  ['] st-1b40 behave ;
: got-back ( -- )  his-t2 @ if  ['] st-1b00  else  ['] st-1af0  then  behave ;
\ telling her: a bark, then sat up by her (Hewie_State1AA0, State1B60)
: st-1b60 ( -- )  anim-done? if  1 keep-pose  ['] st-1b00 behave  then ;
: st-1aa0 ( -- )  3 step-to-pose 0= if  bark  ['] st-1b60 behave  then ;

\ ---- back to her place (Hewie_StateSlideToCommand, StateFollowCommand) ----
: st-follow-back ( -- )  54e 44e follow-step 1 = if  got-back  then ;
: st-back ( -- )
    3 step-to-pose if  exit  then
    3 keep-pose  slide-root
    his-action @ command-place cp-pos plan-and-go if  to-default exit  then
    rest  fdup 12e f< if  fdrop path-stop  got-back exit  then
    his-cond @ 1 = fdup 20e f< or if  fdrop 0  else  44e f< if  1  else  2  then  then  his-t1 !
    ['] st-follow-back behave ;
: back-to-her ( came? -- )  his-t2 !  ['] st-back behave ;   \ (came: brought it, or tried)
: lost-it ( -- )   \ gone, or lying where he can't get: back to her, or not (his skill)
    coming-back try-skill if  false back-to-her  else  sit-by  then ;

\ ---- after it (Hewie_StateFetch) ----
: at-it ( -- )   \ (it lies still by him, him nearly stopped: pick it up, tell her, or leave it)
    1 his-t2 !
    picking-up try-skill if  ball s" things" actor-named send thing-taken  true back-to-her exit  then
    telling try-skill if  0 look!  her his-target !  ['] st-1aa0 behave exit  then
    sit-by ;
: gait ( F: rest -- )
    eager cells his-rolled + @ 0= if  fdrop 7 keep-pose exit  then
    fdup 20e f< ball-speed 1e f< and if  fdrop 7  else  30e f< if  8  else  9  then  then  keep-pose ;
: st-fetch ( -- )
    ball-there? 0= if  lost-it exit  then
    ball-speed 0.5e f<  if  ball-tri ball-at plan-and-go if  lost-it exit  then  then
    rest                                                             ( F: rest )
    ball-speed 0.5e f<  stride-len fabs 0.8e f< and  fdup 6e f< and if  fdrop at-it exit  then
    gait  stride drop
    self body-mask on-ball-line? if  ball-at eyes-on  then ;

\ ---- watching it go (Hewie_StateFollowMover): his eyes on it while it moves where he could go ----
: st-watch ( -- )
    ball-there? 0= if  sit-by exit  then
    0 on-ball-line? 0= ball-speed 0.5e f< or if  sit-by exit  then
    ball-at eyes-on ;

\ ---- 0x78 ----
: fetch ( -- )
    0 his-praise-b !  0 his-praise-a !  $78 his-did !  0 his-roll-n !
    going try-skill if  eager try-skill drop  else  watching try-skill drop  then
    0 his-mode !  $68E his-cmd !
    going cells his-rolled + @ if  ['] st-fetch behave exit  then
    watching cells his-rolled + @ if  ['] st-watch behave exit  then
    sit-by ;

\ ---- which ball: the nearest in front of her (within 90 degrees) he can get straight to
\ (Hewie_NearestThing) ----
variable best  fvariable best-d
: ahead-of-her? ( v -- flag )  fiona-at swap vec-heading  her body-yaw f- angle-wrap fabs pi f2/ f< ;
: gettable? ( i -- flag )
    dup thing-info 2drop nip nip >r                                  ( i ) ( R: tri )
    his-tri his-at rot thing-at self body-mask v-walk  r> = ;
: nearest-ball ( -- i | -1 )
    -1 best !
    128 0 do
        i thing-used? if  i thing-info 2drop drop  room-id =  swap 0= and if   \ (kind room: a ball, here)
            i thing-at fiona-at vec-dist
            best @ 0<  fdup best-d f@ f< or  i thing-at ahead-of-her? and  i gettable? and if
                best-d f!  i best !
            else  fdrop  then
        then  then
    loop  best @ ;
