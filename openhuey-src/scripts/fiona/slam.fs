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
USING: engine vectors game-state actors messages common doors rumble stalker-names fiona.state fiona.anims fiona.sounds fiona.model ;

: doors-id ( -- id )  s" doors" actor-named ;
\ the stalker in her room (the one in the game: stalker-names), Hewie in her room
: stalker-in-room ( -- id | -1 )
    stalker-in-game dup 0< if  exit  then  dup body-room room-id <> if  drop -1  then ;
: dog-in-room ( -- id | -1 )
    s" hewie" actor-named dup 0< 0= if  dup body? if  dup body-room room-id = if  exit  then  then  then  drop -1 ;

$40 constant motion-frozen   \ (her motion's flag: held still)
\ ---- held still a moment by a blow struck home or a door slammed (Motion_Freeze, 5 frames) ----
: recoil ( -- )  5 her-recoil !  her act.mflags dup l@ motion-frozen or swap l! ;
: recoil-down ( -- )
    her-recoil @ 0= if  exit  then
    -1 her-recoil +!  her-recoil @ 0= if  her act.mflags dup l@ motion-frozen invert and swap l!  then ;

\ ---- where someone stands by a door (Progress_WhoIsWhere's bits): at its front (its area 0),
\ on its far side (Doors_Side 1), in its way (on its passage: its sides' triangles) ----
: at-front? ( id exit -- flag )  swap body-at vec@ 0 door-in-area? ;
: far-side? ( id exit -- flag )  swap body-at vec@ door-side 1 = ;
: in-way? ( id exit -- flag )  {: id ex :}  ex 0 id body-tri door-on-side?  ex 1 id body-tri door-on-side? or ;
: shut-out? ( id exit burst? -- flag )   \ (burst open: at its front, far side; slammed: and far side or in its way)
    {: id ex burst? :}  id 0< if  false exit  then
    id ex at-front? 0= if  false exit  then
    id ex far-side?  burst? if  exit  then  id ex in-way? or ;
: strike-with-door ( id exit -- )  {: id ex :}  door-blow 3 ex id send blow ;   \ (its how: the exit)
: strike-out ( exit burst? -- hit? )   \ (the stalker burst in on: only chasing her, as the original)
    {: ex burst? :}  false
    dog-in-room ex burst? shut-out? if  dog-in-room ex strike-with-door  drop true  then
    burst?  her-danger @ chased <> and if  exit  then
    stalker-in-room dup 0< 0= if  ex burst? shut-out? if  stalker-in-room ex strike-with-door  drop true  then  exit  then  drop
    burst? 0= if  exit  then
    stalker-in-game dup 0< if  drop exit  then  ex strike-with-door ;   \ (off screen: he decides)

\ ---- the door she faces on her way to `to` ----
: door-of-exit ( exit -- door | -1 )  room-id swap room-exit-door ;
$20000 constant door-passage   \ (a floor's nav flag: a door's passage)
: facing? ( ahead? -- flag ) ( F: turn -- )   \ (within 45 degrees: her way, or looking back)
    her-yaw f@ 0= if  pi f+ angle-wrap  then  f- angle-wrap fabs 45e deg>rad f< ;
: on-passage? ( to -- flag )  v-tri dup 0< if  drop false exit  then  nav-flags door-passage and 0<> ;
: hers-to-take? ( door -- flag )   \ (DoorHold_Take for her: not held, not locked against her)
    dup state@ held-bit and if  drop false exit  then  against fiona-kind kind-bit and 0= ;
: barge? ( exit ahead? -- flag )   \ shut, her in its area 4, neither locked nor stuck, hers, faced
    {: ex ahead? :}  ex door-of-exit {: door :}
    ex 4 her-pos door-in-area? 0=  door 0< or if  false exit  then
    door door-open?  door state@ locked-bit stuck-bit or and 0<> or if  false exit  then
    door hers-to-take? 0= if  false exit  then
    ex door-turn ahead? facing? ;
: slam? ( exit ahead? -- flag )   \ open, her in its area 1, hers, faced side on
    {: ex ahead? :}  ex door-of-exit {: door :}
    ex 1 her-pos door-in-area? 0=  door 0< or if  false exit  then
    door door-open? 0= if  false exit  then
    door hers-to-take? 0= if  false exit  then
    ex door-turn pi f2/ f+ angle-wrap ahead? facing? ;
: slammed ( exit burst? -- hit? )
    {: ex burst? :}  recoil  $90 8 fiona-jolt
    burst? if  ex fiona-noise doors-id send barge
    else  slam-sound 0 her-sound  ex fiona-noise doors-id send slam  then
    ex burst? strike-out ;
: slam-door ( to ahead? -- hit? )   \ Fiona_SlamDoor
    {: ahead? :}  on-passage? 0= if  false exit  then
    8 0 do
        i door-here?  i door-animating? 0= and if
            i ahead? barge? if  i true slammed  unloop exit  then
            i ahead? slam? if  i false slammed  unloop exit  then
        then
    loop  false ;
