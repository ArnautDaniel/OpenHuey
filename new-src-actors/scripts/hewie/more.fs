\ hewie/more.fs - more of Hewie's behaviours (the original's Hewie_SetAction cases not built in
\ H1..H3): the story's sit-then-on (0x80) and keeping behind her (0x72); going limp when struck
\ down (0x74); the end of a scene's pose (0x79); back onto the floor (0x86); waiting after an
\ attack (0x38); stepping away from his target (0x82); pushed back (0x68) or out by a door (0x6C);
\ at one of the cast (0x5B..0x60); keeping near his target (0x7C). The address-named ones keep
\ their address (st-23d8 is Hewie_State23D8).
IN: hewie.more
USING: engine game-state actors messages common facts paths flag-names hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states hewie.fight ;

\ ---- 0x80 (the story's): once he has stopped, sitting a while ($1C03, 30 frames), then up and
\ on to the action it gave (Hewie_State23D8 / State1C58) ----
: st-1c58 ( -- )
    1 his-no-root !
    anim-group if  -1 his-t2 +!  his-t2 @ 0= if  -1 stand-anim  then  exit  then
    settled? if  his-t1 @ start  then ;
: st-23d8 ( -- )
    1 his-no-root !
    settled? 0= if  exit  then
    anim-group if  -1 stand-anim exit  then
    $1C03 play  30 his-t2 !  ['] st-1c58 behave ;
: sit-then ( act -- )   \ (0x80)
    his-t1 !  0 his-mode !  8 his-cmd !  ['] st-23d8 behave ;

\ ---- 0x72 (the story's 0x63): to the place behind her (the meeting's first offset, by the
\ heading the story gave; her own spot when that is off the floor) and stays there facing away
\ from her - till the game takes him (Hewie_StateKeepBehind, StateBackToNormal) ----
create behind-at 12 allot
: fiona-busy? ( -- flag )  fiona-mode @ 2 = ;   \ (+0xE0: a script moving her)
: behind-spot ( -- tri )   \ (kHewieMeetOffsets[0] in her frame by his-heading: Mtx_AtHeading)
    meet-offsets f@ his-heading f@ fcos f*  meet-offsets float+ f@ his-heading f@ fsin f* f+  fiona-at sf@ f+  behind-at sf!
    meet-offsets float+ f@ his-heading f@ fcos f*  meet-offsets f@ his-heading f@ fsin f* f-  fiona-at 8 + sf@ f+  behind-at 8 + sf!
    fiona-at 4 + sf@ behind-at 4 + sf!
    behind-at $29020008 v-tri-in dup 0< if  drop  behind-at fiona-at vec-copy  her body-tri  then ;
: facing-away? ( -- flag )  his-heading f@ pi f+ his-yaw f- angle-wrap fabs pi f2/ f< ;
: st-keep-behind ( -- )
    her-with? 0= fiona-busy? or if  to-default exit  then
    behind-spot                                                      ( tri )
    his-at behind-at vec-dist 10e f< facing-away? and if  drop  4 look!  exit  then
    path-done? if  behind-at plan-and-go if  exit  then  else  drop  then
    stride if  $202  else  $201  then  play-if-not ;
: keep-behind ( -- )   \ (0x72)
    her-with? 0= if  to-default exit  then
    her his-target !  0 his-mode !  0 his-cmd !  his-path path-clear  ['] st-keep-behind behave ;

\ ---- 0x74: struck down - limp ($2213, his yelp $66), then down (0x52) (Hewie_State22E8) ----
: st-22e8 ( -- )  1 his-no-root !  anim-done? if  $52 start  then ;
: go-limp ( -- )  0 his-mode !  0 his-cmd !  $2213 play  $66 make-sound  ['] st-22e8 behave ;

\ ---- 0x79: a scene's pose ($1301) played out, left alone meanwhile (Hewie_State2358) ----
: st-2358 ( -- )  anim-done? if  0 his-2d !  to-default  then ;
: scene-pose ( -- )  1 his-2d !  $B his-mode !  $1301 play  0 his-cmd !  4 look!  ['] st-2358 behave ;

\ ---- 0x86: off the floor he may stand on - back toward his spot 0.4 a frame, facing it; on it
\ again, himself (Hewie_StateBackOnMesh) ----
: on-his-floor? ( -- flag )  his-tri dup 0< if  drop false exit  then  nav-flags $29020008 and 0= ;
: st-back-on-mesh ( -- )
    on-his-floor? if  0 his-2d !  0 his-2b !  to-default exit  then
    1 his-no-root !
    his-spot heading-to fdup head-toward-soon                        ( F: a )
    fdup fsin 0.4e f*  fswap fcos 0.4e f*  body-move  7 keep-pose ;
: back-on-mesh ( -- )  0 his-mode !  1 his-2b !  1 his-2d !  0 his-cmd !  ['] st-back-on-mesh behave ;

\ ---- 0x38: after an attack - waiting off to the side for his trust's time (Hewie_State20C8:
\ nothing, the wait runs out) ----
: st-20c8 ( -- ) ;
: after-attack ( -- )
    6 his-mode !  $80 his-cmd !
    his-trust @ 1+ 15 *  his-grip @ dup 1 = swap 2 = or if  $1F  else  $3D  then  +  his-wait !
    ['] st-20c8 behave ;

\ ---- 0x82: stepping away from his target while it stands within 6 (Hewie_StateStepAwayTarget) ----
: st-step-away-target ( -- )
    his-target @ dup with? 0= if  drop to-default exit  then
    dup body-at his-at vec-dist-xz 6e f> if  drop to-default exit  then
    back-off ;
: step-away-target ( -- )
    his-target @ with? 0= if  to-default exit  then
    0 his-mode !  0 look!  0 his-t1 !  8 his-cmd !  ['] st-step-away-target behave ;

\ ---- 0x68: pushed back 4, less 0.5 a frame (as his yelp, 0x69); 0x6C: by a door, 5 less 1, and
\ out by the exit it leads to (Hewie_StateMoveAlong) ----
: pushed ( push less -- ) ( F: -- )   \ (the push from his-push-yaw, away from whoever struck him)
    4 his-mode !  0 his-cmd !  s>f his-push-less f!  s>f his-push f!
    $1000 play  $66 make-sound  ['] st-move-along behave ;
: pushed-back ( -- )   \ (0x68)
    his-target @ dup 0< 0= if  body-at heading-to pi f+ angle-wrap  else  drop his-yaw  then  his-push-yaw f!
    4 0 pushed  0.5e his-push-less f! ;
: pushed-out ( -- )   \ (0x6C: through his door - his-door)
    1 his-2d !  1 his-2b !  4 look!  5 1 pushed ;

\ ---- 0x5B..0x60: at one of the cast (slots 3, 4, 5; the odd ones a while longer): near them,
\ then at them (0x53 then 0x23) ----
: cast-of ( slot -- id )   \ (the cast's actors: slots 3, 4, 5)
    case  3 of  s" cast3"  endof  4 of  s" cast4"  endof  >r s" cast5" r>  endcase  actor-named ;
: at-cast ( act -- )
    dup $5B - 2/ 3 +  cast-of                                        ( act id )
    swap dup $5C = over $5E = or swap $60 = or if  3 his-wanted !  then
    dup with? if  his-target !  $53 $23 want  else  drop to-default  then ;

\ ---- 0x12: round to his target's flank (Hewie_StateFlank). Behind it (his bearing within 45
\ degrees of its heading) and facing much its way (within 135): along its heading as he runs;
\ within 40 the side is settled, and lined up (30) himself again. Else, once on its triangle, the
\ side picked by his bearing, and on for the point 45 degrees round from straight out from it, 20
\ out (up to 40 the more squarely behind), as far as is free; off its triangle, along his way to
\ it. Gait by how far: walk under 20, trot under 44, else run ----
fvariable fl-rel  fvariable fl-d  create fl-at 12 allot
: its-heading ( -- ) ( F: -- a )  his-target @ body-yaw ;
: heading-gap ( -- ) ( F: -- gap )  its-heading his-yaw f- angle-wrap fabs ;
: run-along ( F: a -- )  fdup head-toward-soon  run-turn turn-to fdrop ;
: behind-it? ( -- flag )  fl-rel f@ fabs pi 4e f/ f<  heading-gap 3e pi f* 4e f/ f< and ;
: side-spot ( -- )   \ (fl-at: 45 degrees round from straight out, as far as is free up to the reach)
    his-target @ body-at his-at vec-heading                          ( F: out: from it to him )
    his-t1 @ if  pi 4e f/ f+  else  pi 4e f/ f-  then  angle-wrap  ( F: side )
    fl-rel f@ fabs pi f2/ f< if  20e 1e fl-rel f@ fabs fsin f- 20e f* f+  else  20e  then  fdup fl-d f!
    fover fover  his-target @ dup body-tri swap body-at -1 v-free                 ( F: side reach free )
    fswap fdrop  fl-at his-target @ body-at vec-ahead
    fl-at -1 body-tri-to 0< if  fl-at his-target @ body-at vec-copy  then ;
: st-flank ( -- )
    his-target @ with? 0= if  0 0 want exit  then
    his-target @ body-at his-at vec-dist-xz                          ( F: dd )
    its-heading his-target @ body-at heading-to f- angle-wrap fl-rel f!
    behind-it? if
        its-heading run-along
        fdup 40e f< if  -1 his-t1 !  1 his-t2 !  heading-gap pi 6e f/ f< if  0 0 want  then  then
        40e f-
    else
        his-t2 @ if  fdrop  0 0 want exit  then
        his-t1 @ -1 = if
            his-target @ body-at -1 body-tri-to his-target @ body-tri = if  fl-rel f@ f0< 0= 1 and his-t1 !  then
        then
        his-t1 @ -1 <> if
            fdrop  side-spot  fl-at heading-to run-along  fl-d f@ 40e f+
        else
            fdrop  his-target @ dup body-tri swap body-at plan-and-go if  0 0 want exit  then
            stride 0= if  8 keep-pose  then  rest
        then
    then
    fdup 20e f< if  fdrop 7  else  44e f< if  8  else  9  then  then  keep-pose ;
: flank ( -- )   \ (0x12)
    pick-target dup with? 0= if  drop 0 0 want exit  then
    his-target !  0 his-mode !  0 look!  -1 his-t1 !  0 his-t2 !  $48F his-cmd !  ['] st-flank behave ;

\ ---- 0x7C: near his target (Hewie_PickTarget), keeping within 30 (st-keep-near) ----
: near-target ( -- )
    pick-target dup with? 0= if  drop 6 0 want exit  then
    his-target !  30e his-turn f!  0 his-mode !  0 his-t1 !  0 his-t2 !  his-yaw his-heading f!
    $88 his-cmd !  ['] st-keep-near behave ;
