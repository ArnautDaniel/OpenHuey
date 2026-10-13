\ fiona/slam.fs - Fiona slamming a door on whoever is behind it (src/game/fiona.c Fiona_SlamDoor;
\ her flee turn, her kick and her shove try it). A door of the room at rest that she faces within
\ 45 degrees (her way, or looking back), on her way to a point on a door's passage (triangle
\ flag $20000):
\ - shut, her in its area 4, neither locked nor stuck: burst open (`barge`);
\ - open, her in its area 1: slammed shut (`slam`; her sound $91).
\ She takes hold of it first (not one someone holds, nor one locked against her). Shut out by
\ it: Hewie with her, and the stalker, at its front on its far side (slammed shut: or in its
\ way, on its passage) - each struck by it (`blow` kind 5, 3 damage, how: the exit); she is held
\ still 5 frames. Off screen, she being chased, the stalker making for that door decides for
\ himself (`blow` all the same: struck only when he is right behind it).
IN: fiona.slam
USING: engine game-state actors messages common doors fiona.state fiona.model ;

: doors-id ( -- id )  s" doors" actor-named ;
\ the stalker in the game (by name), the one in her room, Hewie in her room
: stalker-in-game ( -- id | -1 )
    s" debilitas" actor-named dup 0< 0= if  dup body? if  exit  then  then  drop
    s" daniella" actor-named dup 0< 0= if  dup body? if  exit  then  then  drop
    s" riccardo" actor-named dup 0< 0= if  dup body? if  exit  then  then  drop
    s" lorenzo" actor-named dup 0< 0= if  dup body? if  exit  then  then  drop
    -1 ;
: stalker-in-room ( -- id | -1 )
    stalker-in-game dup 0< if  exit  then  dup body-room room-id <> if  drop -1  then ;
: dog-in-room ( -- id | -1 )
    s" hewie" actor-named dup 0< 0= if  dup body? if  dup body-room room-id = if  exit  then  then  then  drop -1 ;

\ ---- held still a moment by a blow struck home or a door slammed (Motion_Freeze, 5 frames) ----
: recoil ( -- )  5 her-recoil !  her act.mflags dup l@ $40 or swap l! ;
: recoil-down ( -- )
    her-recoil @ 0= if  exit  then
    -1 her-recoil +!  her-recoil @ 0= if  her act.mflags dup l@ $40 invert and swap l!  then ;

\ ---- where someone stands by a door (Progress_WhoIsWhere's bits): at its front (its area 0),
\ on its far side (Doors_Side 1), in its way (on its passage: its sides' triangles) ----
variable sl-exit  variable sl-barge
: at-front? ( id -- flag )  sl-exit @ swap body-at vec@ 0 door-in-area? ;
: far-side? ( id -- flag )  sl-exit @ swap body-at vec@ door-side 1 = ;
: in-way? ( id -- flag )
    body-tri >r  sl-exit @ 0 r@ door-on-side?  sl-exit @ 1 r> door-on-side? or ;
: shut-out? ( id -- flag )   \ (burst open: at its front, far side; slammed: and far side or in its way)
    dup 0< if  drop false exit  then
    dup at-front? 0= if  drop false exit  then
    sl-barge @ if  far-side? exit  then
    dup far-side? swap in-way? or ;
: door-blow ( id -- )  >r 5 3 sl-exit @ r> send blow ;
: strike-out ( -- hit? )
    false
    dog-in-room shut-out? if  dog-in-room door-blow  drop true  then
    stalker-in-room dup 0< 0= if  shut-out? if  stalker-in-room door-blow  drop true  then  exit  then  drop
    sl-barge @ her-danger @ 2 = and  stalker-in-game dup 0< 0= and if  door-blow  else  drop  then ;

\ ---- the door she faces on her way to `to` ----
: door-of-exit ( exit -- door | -1 )  room-id swap room-exit-door ;
variable sl-ahead
: facing? ( F: turn -- flag )   \ (within 45 degrees: her way, or looking back)
    her-yaw f@ sl-ahead @ 0= if  pi f+ angle-wrap  then  f- angle-wrap fabs 0.7853982e f< ;
: on-passage? ( to -- flag )  v-tri dup 0< if  drop false exit  then  nav-flags $20000 and 0<> ;
: hers-to-take? ( door -- flag )   \ (DoorHold_Take for her: not held, not locked against her)
    dup state@ held-bit and if  drop false exit  then  against fiona-kind kind-bit and 0= ;
: barge? ( exit -- flag )   \ shut, her in its area 4, neither locked nor stuck, hers, faced
    dup 4 her-pos door-in-area? 0= if  drop false exit  then
    dup door-of-exit dup 0< if  2drop false exit  then
    dup door-open? if  2drop false exit  then
    dup state@ locked-bit stuck-bit or and if  2drop false exit  then
    hers-to-take? 0= if  drop false exit  then
    door-turn facing? ;
: slam? ( exit -- flag )   \ open, her in its area 1, hers, faced side on
    dup 1 her-pos door-in-area? 0= if  drop false exit  then
    dup door-of-exit dup 0< if  2drop false exit  then
    dup door-open? 0= if  2drop false exit  then
    hers-to-take? 0= if  drop false exit  then
    door-turn pi f2/ f+ angle-wrap facing? ;
: slammed ( exit barge? -- hit? )
    sl-barge !  sl-exit !  recoil
    sl-barge @ if  sl-exit @ fiona-noise doors-id send barge
    else  $91 5 0 0 her-pos sound-at  sl-exit @ fiona-noise doors-id send slam  then
    strike-out ;
: slam-door ( to ahead? -- hit? )   \ Fiona_SlamDoor
    sl-ahead !  on-passage? 0= if  false exit  then
    8 0 do
        i door-here?  i door-animating? 0= and if
            i barge? if  i true slammed  unloop exit  then
            i slam? if  i false slammed  unloop exit  then
        then
    loop  false ;
