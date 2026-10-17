\ hewie/more.fs - more of Hewie's behaviours: the story's sit-then-on and keeping behind her;
\ going limp when struck down; the end of a scene's pose; back onto the floor; waiting after an
\ attack; stepping away from his target; pushed back or out by a door; at one of the cast;
\ keeping near his target; round to its flank.
IN: hewie.more
USING: engine game-state actors messages common facts paths vectors flag-names hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.sounds hewie.model hewie.moving hewie.states hewie.fight ;

\ ---- sit-then-act (the story's): once he has stopped, sitting a while (30 frames), then up and
\ on to the action it gave (Hewie_State23D8 / State1C58) ----
: st-sat-a-while ( -- )   \ (Hewie_State1C58)
    1 his-no-root !
    anim-group in-standing <> if  -1 his-t2 +!  his-t2 @ 0= if  -1 stand-still  then  exit  then
    settled? if  his-t1 @ start  then ;
: st-sit-a-while ( -- )   \ (Hewie_State23D8)
    1 his-no-root !
    settled? 0= if  exit  then
    anim-group in-standing <> if  -1 stand-still exit  then
    sit-a-while-anim play  30 his-t2 !  ['] st-sat-a-while behave ;
: sit-then ( act -- )
    his-t1 !  free-mode his-mode !  on-command his-cmd !  ['] st-sit-a-while behave ;

\ ---- keep-behind-act (the story's 0x63): to the place behind her (the meeting's first offset,
\ by the heading the story gave; her own spot when that is off the floor) and stays there facing
\ away from her - till the game takes him (Hewie_StateKeepBehind, StateBackToNormal) ----
create behind-at /vec allot
: fiona-busy? ( -- flag )  fiona-mode @ led-mode = ;   \ (+0xE0: a script moving her)
: behind-spot ( -- tri )   \ (kHewieMeetOffsets[0] in her frame by his-heading: Mtx_AtHeading)
    meet-offsets f@  meet-offsets float+ f@  {: F: ox oz :}
    his-heading f@ fcos  his-heading f@ fsin  {: F: c s :}
    ox c f*  oz s f* f+  fiona-at v.x sf@ f+  behind-at v.x sf!
    oz c f*  ox s f* f-  fiona-at v.z sf@ f+  behind-at v.z sf!
    fiona-at v.y sf@ behind-at v.y sf!
    behind-at his-floor v-tri-in dup 0< if  drop  behind-at fiona-at vec-copy  her body-tri  then ;
: facing-away? ( -- flag )  his-heading f@ pi f+ his-yaw f- angle-wrap fabs pi f2/ f< ;
: st-keep-behind ( -- )
    her-with? 0= fiona-busy? or if  to-default exit  then
    behind-spot                                                      ( tri )
    his-at behind-at vec-dist 10e f< facing-away? and if  drop  ahead-look look!  exit  then
    path-done? if  behind-at plan-and-go if  exit  then  else  drop  then
    stride if  run-anim  else  trot-anim  then  play-if-not ;
: keep-behind ( -- )
    her-with? 0= if  to-default exit  then
    her his-target !  free-mode his-mode !  0 his-cmd !  his-path path-clear  ['] st-keep-behind behave ;

\ ---- go-limp-act: struck down - limp, his yelp, then down (Hewie_State22E8) ----
: st-going-limp ( -- )  1 his-no-root !  anim-done? if  knocked-down-act start  then ;
: go-limp ( -- )  free-mode his-mode !  0 his-cmd !  flung-anim play  yelp-sound make-sound  ['] st-going-limp behave ;

\ ---- scene-pose-act: a scene's pose played out, left alone meanwhile (Hewie_State2358) ----
: st-scene-pose ( -- )  anim-done? if  0 his-2d !  to-default  then ;
: scene-pose ( -- )  1 his-2d !  posed-mode his-mode !  scene-pose-anim play  0 his-cmd !  ahead-look look!  ['] st-scene-pose behave ;

\ ---- back-on-floor-act: off the floor he may stand on - back toward his spot 0.4 a frame,
\ facing it; on it again, himself (Hewie_StateBackOnMesh) ----
: on-his-floor? ( -- flag )  his-tri dup 0< if  drop false exit  then  nav-flags his-floor and 0= ;
: st-back-on-mesh ( -- )
    on-his-floor? if  0 his-2d !  0 his-2b !  to-default exit  then
    1 his-no-root !
    his-spot heading-to fdup head-toward-soon                        ( F: a )
    fdup fsin 0.4e f*  fswap fcos 0.4e f*  body-move  walk-pose keep-pose ;
: back-on-mesh ( -- )  free-mode his-mode !  1 his-2b !  1 his-2d !  0 his-cmd !  ['] st-back-on-mesh behave ;

\ ---- after-attack-act: waiting off to the side for his trust's time - longer when he came at a
\ flank (Hewie_State20C8: nothing, the wait runs out) ----
: st-waiting-out ( -- ) ;
: after-attack ( -- )
    away-mode his-mode !  on-wait-over his-cmd !
    his-trust @ 1+ 15 *  his-grip @ dup 1 = swap 2 = or if  31  else  61  then  +  his-wait !
    ['] st-waiting-out behave ;

\ ---- step-off-target-act: stepping away from his target while it stands within 6
\ (Hewie_StateStepAwayTarget) ----
: st-step-away-target ( -- )
    his-target @ dup with? 0= if  drop to-default exit  then
    dup body-at his-at vec-dist-xz 6e f> if  drop to-default exit  then
    back-off ;
: step-away-target ( -- )
    his-target @ with? 0= if  to-default exit  then
    free-mode his-mode !  target-look look!  0 his-t1 !  on-command his-cmd !  ['] st-step-away-target behave ;

\ ---- pushed-back-act: pushed back 4, less 0.5 a frame (as his yelp); pushed-out-act: by a door,
\ 5 less 1, and out by the exit it leads to (Hewie_StateMoveAlong) ----
: pushed ( push less -- )   \ (the push along his-push-yaw, away from whoever struck him)
    struck-mode his-mode !  0 his-cmd !  s>f his-push-less f!  s>f his-push f!
    yelp-anim play  yelp-sound make-sound  ['] st-move-along behave ;
: pushed-back ( -- )
    his-target @ dup 0< 0= if  body-at heading-to pi f+ angle-wrap  else  drop his-yaw  then  his-push-yaw f!
    4 0 pushed  0.5e his-push-less f! ;
: pushed-out ( -- )   \ (through his door - his-door)
    1 his-2d !  1 his-2b !  ahead-look look!  5 1 pushed ;

\ ---- at-cast-3-act .. at-cast-5-long-act: at one of the cast (slots 3, 4, 5; the long ones
\ three times): near them, then at them ----
: cast-of ( slot -- id )   \ (the cast's actors: slots 3, 4, 5)
    case  3 of  s" cast3"  endof  4 of  s" cast4"  endof  >r s" cast5" r>  endcase  actor-named ;
: at-cast ( act -- )
    dup at-cast-3-act - 2/ 3 +  cast-of                              ( act id )
    swap dup at-cast-3-long-act =  over at-cast-4-long-act = or  swap at-cast-5-long-act = or if  3 his-wanted !  then
    dup with? if  his-target !  near-target-act close-on-act want  else  drop to-default  then ;

\ ---- flank-act: round to his target's flank (Hewie_StateFlank). Behind it (his bearing within
\ 45 degrees of its heading) and facing much its way (within 135): along its heading as he runs;
\ within 40 the side is settled, and lined up (30) himself again. Else, once on its triangle, the
\ side picked by his bearing, and on for the point 45 degrees round from straight out from it, 20
\ out (up to 40 the more squarely behind), as far as is free; off its triangle, along his way to
\ it. Gait by how far: walk under 20, trot under 44, else run ----
create fl-at /vec allot
: its-heading ( -- ) ( F: -- a )  his-target @ body-yaw ;
: heading-gap ( -- ) ( F: -- gap )  its-heading his-yaw f- angle-wrap fabs ;
: run-along ( F: a -- )  fdup head-toward-soon  run-turn turn-to fdrop ;
: behind-it? ( F: rel -- flag )  fabs pi 4e f/ f<  heading-gap 3e pi f* 4e f/ f< and ;
: side-spot ( F: rel -- reach )   \ (fl-at: 45 degrees round from straight out, as far as is free up to the reach)
    {: F: rel :}
    his-target @ body-at his-at vec-heading                          ( F: out: from it to him )
    his-t1 @ if  pi 4e f/ f+  else  pi 4e f/ f-  then  angle-wrap  ( F: side )
    rel fabs pi f2/ f< if  20e 1e rel fabs fsin f- 20e f* f+  else  20e  then  {: F: reach :}
    fdup reach  his-target @ dup body-tri swap body-at -1 v-free     ( F: side free )
    fl-at his-target @ body-at vec-ahead
    fl-at -1 body-tri-to 0< if  fl-at his-target @ body-at vec-copy  then  reach ;
: st-flank ( -- )
    his-target @ with? 0= if  to-default exit  then
    his-target @ body-at his-at vec-dist-xz                          ( F: dd )
    its-heading his-target @ body-at heading-to f- angle-wrap {: F: rel :}
    rel behind-it? if
        its-heading run-along
        fdup 40e f< if  -1 his-t1 !  1 his-t2 !  heading-gap pi 6e f/ f< if  to-default  then  then
        40e f-
    else
        his-t2 @ if  fdrop  to-default exit  then
        his-t1 @ -1 = if
            his-target @ body-at -1 body-tri-to his-target @ body-tri = if  rel f0< 0= 1 and his-t1 !  then
        then
        his-t1 @ -1 <> if
            fdrop  rel side-spot  fl-at heading-to run-along  40e f+
        else
            fdrop  his-target @ dup body-tri swap body-at plan-and-go if  to-default exit  then
            stride 0= if  trot-pose keep-pose  then  rest
        then
    then
    fdup 20e f< if  fdrop walk-pose  else  44e f< if  trot-pose  else  run-pose  then  then  keep-pose ;
: flank ( -- )
    pick-target dup with? 0= if  drop to-default exit  then
    his-target !  free-mode his-mode !  target-look look!  -1 his-t1 !  0 his-t2 !
    breaks-any on-footing on-100 or on-alert or but his-cmd !  ['] st-flank behave ;

\ ---- near-target-2-act: near his target (Hewie_PickTarget), keeping within 30 (st-keep-near) ----
: near-target ( -- )
    pick-target dup with? 0= if  drop sniff-act 0 want exit  then
    his-target !  30e his-turn f!  free-mode his-mode !  0 his-t1 !  0 his-t2 !  his-yaw his-heading f!
    on-command on-wait-over or his-cmd !  ['] st-keep-near behave ;
