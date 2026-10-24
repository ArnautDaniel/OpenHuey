\ stalker/ladder.fs - a stalker and the room's ladders (src/game/pursuer.c Pursuer_TargetOutOfReach,
\ Npc_RoundThroughDoor, Npc_FindDoor, Npc_OpenDoorFionaHides; his action $12: Pursuer_DoorApproach,
\ WalkToSpot, DoorArmOpen, Stairs, DoorAnim - for all their names, a ladder).
\ No way to her on foot while he is after her: round by a ladder - the first whose end on her level
\ has a way to her and whose end on his has a way from him; else, in a room of two ladders or
\ more, the first he has a way to. Fiona on one: to it (its foot, else its top). He walks to beside
\ that end (out from it by his family's offsets), turns to it and gets on, and climbs a rung at a
\ time (the legs by turns) by the moves' own motion; off at the foot within 20 of it, at the top
\ within 4, put out from that end. Whoever he touches on it is struck (kind 4, 5 damage). On his way to it - her off the
\ ladders with a way to her on foot - he gives it up and stands. (The original's turn back
\ half way up, when he is sent elsewhere meanwhile, isn't here: nothing sends him while he climbs.)
IN: stalker.ladder
USING: engine game-state actors messages common facts flag-names paths vectors stalker.names stalker.anims stalker.state stalker.senses stalker.moving stalker.search stalker.doorway stalker.head stalker.chase stalker.pursue ;

\ ---- out from a ladder's ends by family (vtable +0x9C: x, z in its frame) - beside its foot,
\ beside its top (where he gets on), off at its top, off at its foot ----
create pursuer-ladder    0e f, -6e f,       0.64e f, 10.4e f,   -0.4154e f, 10.0115e f,  0.64e f, -7.568e f,   \ (Pursuer_DoorOffset: Lorenzo, ...)
create debilitas-ladder  0e f, -6.2115e f,  0e f, 10.4e f,      0e f, 6.2885e f,         0e f, -7.4404e f,  \ (Debilitas_DoorOffset)
create daniella-ladder   0e f, -6.9651e f,  0e f, 6.9767e f,    0e f, 7.219e f,          0e f, -6.2558e f,  \ (Daniella_DoorOffset, her later kinds')
create riccardo-ladder   0e f, -5.3136e f,  0e f, 7.572e f,     0e f, 7.9372e f,         0e f, -6.8888e f,  \ (Riccardo's, kind 37's, the TintStalker's)
0 constant ladder-foot   1 constant ladder-top
: end-front ( link side -- tri ) ( F: -- x y z )  dup ladder-offset nav-link-front ;          \ (beside it)
: off-front ( link side -- tri ) ( F: -- x y z )  3 over - ladder-offset nav-link-front ;    \ (off it)
: beside ( link side -- ok? )  end-front aim-at ;   \ (Npc_PlanBesideDoor: his way planned there)

\ ---- the ladder action's end: his chase's standing; done - the Pursuer by his row
\ again (Pursuer_HoldOff); the way there gone - the Pursuer searches the room
\ (Pursuer_ChaseFionaHere) ----
: in-chase ( -- )   \ (back in his chase's step)
    -1 at-ladder !  own-move-mode my-move-mode !  0 apart !   \ (Pursuer_DoorAnim: in contact again)
    base-chase @ if  ['] pursue-run  else  own-run @ ?dup 0= if  ['] chase-run  then  then  step! ;
: to-stand ( -- )  in-chase  base-chase @ if  stand-to  else  stand-watch  then ;
: ladder-done ( -- )  in-chase  base-chase @ if  pick-pace  else  stand-watch  then ;
: no-way-there ( -- )  base-chase @ if  -1 at-ladder !  search-again  else  to-stand  then ;

\ ---- on it: moved by his move's root motion, up and down too, his triangle kept; whoever he
\ touches struck (Relation_Request: Pursuer_RaiseThreat's) ----
: climb-move ( -- )  root@ {: F: turn x y z :}  yaw turn f+ angle-wrap yaw!  x y z body-climb ;
: play-own ( anim -- )  dup anim !  my-model @ swap  2dup motion-entry nip 8 or  motion-play ;   \ (Motion_PlayOwnBlend)
: end-y ( side -- ) ( F: -- y )  at-ladder @ swap nav-link-spot  fdrop fswap fdrop ;
: my-y ( F: -- y )  my-at v.y sf@ ;
: tri-here! ( tri -- )  room-id swap my-at vec@ body-place ;
: put-off ( side -- )  at-ladder @ swap off-front  >r room-id r> body-place ;

\ off it (Pursuer_DoorAnim): at the move's end put out from that end; his chase again
: stepping-off ( -- )
    bump
    ended? 0= if  climb-move exit  then
    anim @ off-top-anim = if  ladder-top put-off  else  anim @ off-foot-anim = if  ladder-foot put-off  then  then
    ladder-done ;
: off-at ( anim from -- )   \ (straight on from that rung, else blended)
    anim @ = if  play-now  else  play-own  then  ['] stepping-off step! ;
\ the next rung, the legs by turns
: next-rung ( -- )
    anim @ case
        on-top-anim of  rung-down-anim play-now  endof   rung-down-2-anim of  rung-down-anim play-now  endof
        rung-down-anim of  rung-down-2-anim play-now  endof
        on-foot-anim of  rung-up-anim play-now  endof   rung-up-2-anim of  rung-up-anim play-now  endof
        rung-up-anim of  rung-up-2-anim play-now  endof
    endcase ;
\ Pursuer_Stairs: once his move has faded in; at each move's end down - off within 20 of the foot -
\ or up - off within 4 of the top (the first from the foot over, his triangle the top's)
: down-a-rung ( -- )
    0 end-y my-y f- 20e f< if  at-ladder @ ladder-foot off-front fdrop fdrop fdrop tri-here!  off-foot-anim rung-down-anim off-at exit  then
    next-rung ;
: up-a-rung ( -- )
    my-y 1 end-y f- 4e f< if  off-top-anim rung-up-2-anim off-at exit  then
    anim @ on-foot-anim = if  at-ladder @ ladder-top off-front fdrop fdrop fdrop tri-here!  then
    next-rung ;
: climbing ( -- )
    bump
    settled? 0= if  exit  then
    ended? 0= if  climb-move exit  then
    ladder-side @ if  down-a-rung  else  up-a-rung  then ;

\ ---- getting there (WalkToSpot, DoorArmOpen): to beside the end, his walk near it the one for
\ arriving; there (on it now) turned to it at his rate, his walk faded in - on ----
: given-up? ( -- flag )  her-ladder-on @ 0<  d-fiona f@ f0> and ;   \ (action $12's part 6: she free with a way)
: get-on ( -- )   \ (Pursuer_DoorArmOpen: out of contact while on it)
    -1 apart !  0 sent !  ladder-side @ if  on-top-anim  else  on-foot-anim  then  play-own  ['] climbing step! ;
: lining-up ( -- )
    given-up? if  to-stand exit  then
    at-ladder @ ladder-side @ nav-link-yaw  turn-rate body-turn-toward  f0=  settled? and if  get-on  then ;
: to-the-ladder ( -- )
    given-up? if  to-stand exit  then
    settled?  way-left fdup f0> 10e f< and  and if  walk-fast play  then
    walk-stride if  on-ladder-mode my-move-mode !  ['] lining-up step!  then ;
: climb-to ( link side -- )   \ (Pursuer_DoorApproach: his way there planned)
    ladder-side !  at-ladder !  ladder-act action-look!  no-search search-phase !
    way-left 10e f< if  walk-fast  else  mode-walk  then  play  ['] to-the-ladder step! ;
: on-ladder? ( -- flag )
    step @  dup ['] to-the-ladder =  over ['] lining-up = or  over ['] climbing = or  swap ['] stepping-off = or ;
' on-ladder? is at-a-ladder?
' ladder-done is round-done   ' no-way-there is round-failed   \ (the same for a door opened to get at her)

\ ---- no way to her on foot (Pursuer_TargetOutOfReach): round by a ladder ----
\ Npc_RoundThroughDoor: from the middle of her triangle a way to the ladder's end on her level (his
\ end the top when he is above it), and his way to his end
create her-mid /vec allot   create end-at /vec allot
: way-from-her? ( link side -- ok? )
    end-front dup 0< if  drop fdrop fdrop fdrop false exit  then  end-at vec!  >r
    fiona body-tri her-mid r> end-at self body-mask v-path 0> ;
: by-ladder ( -- link side true | false )
    fiona body-tri tri-center her-mid vec!
    my-y her-mid v.y sf@ f< 1 and                                       ( mine )
    nav-links 5 min 0 ?do
        i over 1 xor way-from-her? if  i over beside if  i swap true unloop exit  then  then
    loop  drop false ;
\ Npc_FindDoor: the first he has a way to - each one's ends in turn, the last tried first
: any-ladder ( -- link side true | false )
    0  nav-links 5 min 0 ?do                                            ( side )
        i over beside if  i swap true unloop exit  then  1 xor
        i over beside if  i swap true unloop exit  then
    loop  drop false ;
:noname ( -- went? )
    fiona body-tri 0< if  false exit  then
    by-ladder if  climb-to true exit  then
    door-to-her if  door-for-her-act action-look!  door-for-her true exit  then   \ (a door between them: stalker/doorway.fs)
    nav-links 2 < if  false exit  then
    any-ladder if  climb-to true exit  then  false ; is out-of-reach

\ ---- each frame: Fiona on one while he is after her and free (Npc_OpenDoorFionaHides) - to its
\ foot, else its top; no way to either: he stands ----
: ladder-watch ( -- )
    at-ladder @ 0< 0= if  on-ladder? 0= if  -1 at-ladder !  own-move-mode my-move-mode !  then  exit  then
    her-ladder-on @ 0<  my-mode @ after-her <> or  if  exit  then
    reeling @ bitten @ or  leading @ or  at-dog @ or  out-going @ or  fiona here? 0= or if  exit  then
    her-ladder-on @  dup ladder-foot beside if  ladder-foot climb-to exit  then
    dup ladder-top beside if  ladder-top climb-to exit  then
    drop  chasing? if  to-stand  then ;
