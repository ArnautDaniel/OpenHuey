\ hewie/fetch.fs - fetching (action 0x78: Hewie_StateFetch, StateFollowMover and the states
\ after; Hewie_RollReaction / RollReactions; Hewie_NearestThing). Fiona throws the ball: his to
\ fetch (`fetch-this`, things.fs) - or she sends him for the nearest (her command 0x23). By his
\ skills (his-skill, out of 32: rolls against them, `try-skill`) he goes after it (eager or not),
\ else watches it go; at it, he picks it up and brings it back, or comes to tell her, or leaves
\ it; lost (gone, or lying where he can't get), he comes back to her or doesn't. Her praise or
\ scolding after (praise-scold) moves the skills he used up or down.
IN: hewie.fetch
USING: things engine actors messages common paths vectors hewie-names hewie.state hewie.body hewie.anims hewie.model hewie.moving hewie.states ;

\ ---- his skills: a roll (of 32) against one, kept for her praise (Hewie_Roll) ----
\ (going after it, following it with his eyes, eager, picking it up, bringing word of it, coming
\ back when it's lost)
enum: skill  going-skill watching-skill eager-skill picking-up-skill telling-skill coming-back-skill ;
: try-skill ( skill -- hit? )
    {: k :}  k cells his-skill + @  32 roll >  {: hit :}
    hit k cells his-rolled + !
    k his-roll-n @ cells his-rolls + !  1 his-roll-n +!  hit ;
: rolled? ( skill -- hit? )  cells his-rolled + @ 0<> ;

\ ---- the ball he fetches (his-follows) ----
: ball ( -- i )  his-follows @ ;
: ball-room ( -- room )  ball thing-room ;
: ball-there? ( -- flag )   \ (lying about still, in his room)
    ball 0< if  false exit  then  ball thing-used? 0= if  false exit  then  ball-room self body-room = ;
: ball-at ( -- v )  ball thing-at ;
: ball-tri ( -- tri )  ball thing-tri ;
: ball-speed ( F: -- s )  ball thing-speed ;
: on-ball-line? ( mask -- flag )   \ (Actor_TriTo: straight from him onto its triangle)
    {: mask :}  his-tri his-at ball-at mask v-walk  ball-tri = ;
: forget-ball ( -- )  -1 his-follows ! ;
: eyes-on ( v -- )  look-at his-look-yaw f! his-look-pitch f!  held-look look! ;   \ (Motion_LookAt: his head on it)

\ ---- after: lying low by her a while, then his doing counted as praised (Hewie_State1A60) ----
: st-sat-by-her ( -- )
    -1 his-t3 +!  his-t3 @ 0= if
        forget-ball  his-action @ his-did-was !  1 1 praise-scold drop  to-default exit
    then  resting-pose keep-pose ;
: sit-by ( -- )  90 his-t3 !  ['] st-sat-by-her behave ;
\ by her, sat up, looking at her (Hewie_State1B00); or ears down first (State1AF0 / 1B40)
: st-sat-up ( -- )  sit-pose step-to-pose if  exit  then  sit-pose keep-pose  her his-target !  target-look look!  sit-by ;
: st-ears-down-done ( -- )  anim-done? if  sit-by  then ;
: st-ears-down-first ( -- )  sit-pose step-to-pose if  exit  then  ears-down-anim play  ['] st-ears-down-done behave ;
: got-back ( -- )  his-t2 @ if  ['] st-sat-up  else  ['] st-ears-down-first  then  behave ;
\ telling her: a bark, then sat up by her (Hewie_State1AA0, State1B60)
: st-told ( -- )  anim-done? if  sit-pose keep-pose  ['] st-sat-up behave  then ;
: st-telling ( -- )  resting-pose step-to-pose 0= if  bark  ['] st-told behave  then ;

\ ---- back to her place (Hewie_StateSlideToCommand, StateFollowCommand) ----
: st-follow-back ( -- )  54e 44e follow-step 1 = if  got-back  then ;
: st-back ( -- )
    resting-pose step-to-pose if  exit  then
    resting-pose keep-pose  slide-root
    his-action @ command-place cp-pos plan-and-go if  to-default exit  then
    rest  fdup 12e f< if  fdrop path-stop  got-back exit  then
    his-cond @ hurt-cond = fdup 20e f< or if  fdrop 0  else  44e f< if  1  else  2  then  then  his-t1 !
    ['] st-follow-back behave ;
: back-to-her ( came? -- )  his-t2 !  ['] st-back behave ;   \ (came: brought it, or tried)
: lost-it ( -- )   \ gone, or lying where he can't get: back to her, or not (his skill)
    coming-back-skill try-skill if  false back-to-her  else  sit-by  then ;

\ ---- after it (Hewie_StateFetch) ----
: at-it ( -- )   \ (it lies still by him, him nearly stopped: pick it up, tell her, or leave it)
    1 his-t2 !
    picking-up-skill try-skill if  ball s" things" actor-named send thing-taken  true back-to-her exit  then
    telling-skill try-skill if  target-look look!  her his-target !  ['] st-telling behave exit  then
    sit-by ;
: fetch-gait ( F: rest -- )
    eager-skill rolled? 0= if  fdrop walk-pose keep-pose exit  then
    fdup 20e f< ball-speed 1e f< and if  fdrop walk-pose  else  30e f< if  trot-pose  else  run-pose  then  then  keep-pose ;
: st-fetch ( -- )
    ball-there? 0= if  lost-it exit  then
    ball-speed 0.5e f<  if  ball-tri ball-at plan-and-go if  lost-it exit  then  then
    rest                                                             ( F: rest )
    ball-speed 0.5e f<  stride-len fabs 0.8e f< and  fdup 6e f< and if  fdrop at-it exit  then
    fetch-gait  stride drop
    self body-mask on-ball-line? if  ball-at eyes-on  then ;

\ ---- watching it go (Hewie_StateFollowMover): his eyes on it while it moves where he could go ----
: st-watch ( -- )
    ball-there? 0= if  sit-by exit  then
    0 on-ball-line? 0= ball-speed 0.5e f< or if  sit-by exit  then
    ball-at eyes-on ;

\ ---- fetch-act ----
: fetch ( -- )
    0 his-praise-b !  0 his-praise-a !  fetch-act his-did !  0 his-roll-n !
    going-skill try-skill if  eager-skill try-skill drop  else  watching-skill try-skill drop  then
    free-mode his-mode !  on-waiting on-danger or on-command or on-wait-over or on-alert or on-praise-due or his-cmd !
    going-skill rolled? if  ['] st-fetch behave exit  then
    watching-skill rolled? if  ['] st-watch behave exit  then
    sit-by ;

\ ---- which ball: the nearest in front of her (within 90 degrees) he can get straight to
\ (Hewie_NearestThing) ----
: ahead-of-her? ( v -- flag )  fiona-at swap vec-heading  her body-yaw f- angle-wrap fabs pi f2/ f< ;
: gettable? ( i -- flag )
    {: i :}  his-tri his-at i thing-at self body-mask v-walk  i thing-tri = ;
: nearest-ball ( -- i | -1 )
    -1 0e {: best F: best-d :}
    #things 0 do
        i thing-used? if  i thing-kind ball-thing =  i thing-room room-id = and if   \ (a ball, here)
            i thing-at fiona-at vec-dist
            best 0<  fdup best-d f< or  i thing-at ahead-of-her? and  i gettable? and if
                to best-d  i to best
            else  fdrop  then
        then  then
    loop  best ;
