\ stalker/travel.fs - a stalker out of sight, from room to room (src/game/pursuer.c): his route
\ through the doors to the room he heads for (Character_Route), each stretch to the next door
\ counted down at his pace (Pursuer_TravelOffscreen), the doors as he finds them (Npc_ExitKind:
\ held, not for him, a way through, shut), a knock at a shut door into Fiona's room
\ (Pursuer_KnockAtDoor), going through (Pursuer_ThroughDoor), his modes out there
\ (Pursuer_ModesSearching off screen, Pursuer_SearchRouteIn) and coming into the room being
\ played (Pursuer_EnterRoom; Pursuer_BackOnMesh when she comes into his).
IN: stalker.travel
USING: engine game-state actors messages common facts flag-names paths vectors doors stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.anims ;

: my-room ( -- room )  self body-room ;
: away? ( -- flag )  in-game @ if  my-room room-id <>  else  false  then ;
\ his own rooms (vtable +0x314: Debilitas $20 and $02 - his lair; Daniella none)
: own-room? ( -- flag )  my-room dup own-a @ =  swap own-b @ =  or  own-a @ 0< 0= and ;
: own-tri ( -- tri )  my-room own-a @ = if  own-a-tri  else  own-b-tri  then  @ ;
: her-room ( -- room )  fiona dup 0< if  drop -1 exit  then  dup body? if  body-room  else  drop -1  then ;

\ ---- the doors that let him down: not that way again until a fresh route ----
: avoid-clear ( -- )  avoid 13 cells 0 fill ;
: avoid-door ( door -- )   \ (a bit a door)
    dup 0< if  drop exit  then  {: door :}
    avoid door 5 rshift cells + {: a :}  a @  1 door 31 and lshift or  a ! ;

\ ---- his route to a room (> 0 doors, 0 there, -1 none); the next door, its exit here ----
: next-door ( -- door )  door-i @ doors-n @ < if  door-i @ cells doors + @  else  -1  then ;
: door-exit ( door -- exit )  dup 0< if  exit  then  my-room door-exit-in ;
\ Rooms_DoorTri: the door's triangle on a room's side - the original's own measure of the way
\ to it (its number); -1 none
: door-tri ( door room -- tri )
    {: door room :}  door 0< if  -1 exit  then
    door door-sides {: r0 e0 t0 r1 e1 t1 :}
    r1 room = if  t1  else  r0 room = if  t0  else  -1  then then ;
\ Npc_NodeDistance: from the door he came in by to the next one (each its triangle's number)
: came-door ( -- door )  came-by @ dup 0< if  exit  then  my-room swap room-exit-door ;
: stretch ( -- ) ( F: -- d )
    next-door my-room door-tri 0 max s>f
    came-door dup 0< 0= over next-door <> and if  my-room door-tri 0 max s>f f+  else  drop  then ;
\ the half of his room he is on: the one the exit he came in by opens onto, where the door marks
\ it (Character_Route with no side: Rooms_ExitSideFlag of his door, checked); -1 either
: my-way ( -- way )  came-by @ dup 0< if  exit  then  my-room swap 1 exit-way ;
\ to a half of the goal room (`want`; -1 either: Fiona's half when she is across his own room -
\ stalker/doorway.fs)
: route-way! ( room want -- n )   \ (planned: the first stretch set)
    {: room want :}  room goal-room !
    my-room my-way room want stalker-kind -1 avoid route-sides
    dup 0> if
        dup 16 min 0 do  i route-door  i cells doors + !  loop
        dup 16 min doors-n !  0 door-i !
        next-door door-exit making-for !  stretch to-door f!
    else  0 doors-n !  0 door-i !  -1 making-for !  then ;
: route! ( room -- n )  -1 route-way! ;
: route-over? ( -- flag )  door-i @ doors-n @ < 0= ;

\ ---- a door as he finds it (Npc_ExitKind): held, not for him (locked against him, locked,
\ stuck), a way through (a doorway, open), shut ----
enum: door-way  2 held-way barred-way open-way shut-way ;
: door-kind ( door -- way )
    dup 0< if  drop barred-way exit  then
    dup fixed? if  drop open-way exit  then
    dup door-held? if  drop held-way exit  then
    dup against stalker-kind kind-bit and if  drop barred-way exit  then
    dup door-locked? if  drop barred-way exit  then
    dup state@ stuck-bit and if  drop barred-way exit  then
    state@ opened-bit and if  open-way  else  shut-way  then ;
: usable-kind? ( way -- flag )  dup open-way =  swap shut-way = or ;

\ ---- his pace out of sight (a stretch's length a frame): after her 1.6..1.8, searching or
\ waiting 0.6, heading for her room or held off 1; while she hides 2 ----
: pace ( F: -- p )
    fiona-hidden state-flag? if  2e exit  then
    my-mode @ case
        after-her of  rnd 0.2e f* 1.6e f+  endof
        searching of  0.6e  endof  waiting-about of  0.6e  endof
        >r 1e r>
    endcase ;

\ ---- coming into the room being played by an exit: 12 inside it (Rooms_ExitTriThrough: the
\ exit's spots are 4 inside, 12 beyond, 12 inside), facing in - or, by the door he made for,
\ facing it (Pursuer_BackOnMesh); after her and not seeing her, or come to her room, he
\ searches it ----
create in-at /vec allot  create out-at /vec allot
: settle ( -- )  my-model @ dup 0< if  drop  else  actor-settle  then ;   \ (put somewhere: his hanging parts afresh - Pursuer_Place's +0x850)
: place-at-exit ( exit facing-in? -- ok? )
    {: e facing-in :}
    e 2 exit-spot dup 0< if  drop fdrop fdrop fdrop false exit  then  {: tri :}
    in-at vec!  e 0 exit-spot drop out-at vec!
    room-id tri in-at vec@ body-place  settle
    facing-in if  out-at in-at  else  in-at out-at  then  vec-heading body-turn  true ;
variable in-tri
\ through a door into her room he comes from its doorway - the exit's spot 12 beyond it - turned
\ in, and walks in (Pursuer_EnterRoom: Rooms_ExitTriIn, the exit's heading turned round)
: come-through ( exit -- ok? )
    {: e :}  e 1 exit-spot dup 0< if  drop fdrop fdrop fdrop false exit  then  {: tri :}
    out-at vec!  e 2 exit-spot in-tri !  in-at vec!
    room-id tri out-at vec@ body-place  settle
    out-at in-at vec-heading body-turn  true ;
: placed-anywhere ( -- )   \ (Actor_TeleportRandom: no exit to stand at)
    random-tri dup 0< if  drop fiona body-tri  then
    dup 0< if  drop exit  then  room-id swap dup tri-center body-place  settle ;
defer off-home ( -- )   ' noop is off-home   \ (held off: on out by his way home - stalker/struck.fs)
: come-into-play ( -- )   \ in the room being played now: his search there, or after her (in the run he waits 150 first);
    \ down (knocked out of health out of her sight): lying there, his time down running
    \ held off, on home - she coming into his room doesn't turn him back (Pursuer_StanceByFiona's held off)
    cond @ down-cond = if  -1 anim !  lie-anim @ play-now  exit  then
    0 hunting-dog !  0 away-search !  0 out-going !  -1 anim !  still-anim play-now  my-path path-clear
    hewie-played? my-mode @ dup after-her = swap heading-for-her = or and if  150 run-pause !  then
    my-mode @ held-off = own-room? and if   \ (held off in his own room: to its spot - Debilitas_PickDestination)
        own-tri dup tri-center goal-at vec!  goal-at plan 0> if  walk-slow play  ['] walking step!  else  stand  then  exit
    then
    my-mode @ held-off = if  off-home exit  then
    my-mode @ after-her = if  chase-start exit  then
    my-mode @ heading-for-her = if  searching mode!  then
    search-again ;

\ from the doorway on in to the spot 12 inside (Pursuer_SearchRouteIn), then as in-play has it
: walking-in ( -- )  walking  step-done @ if  0 step-done !  come-into-play  then ;
: walk-in ( -- )
    0 entering !
    in-tri @ 0< if  come-into-play exit  then
    goal-at in-at vec-copy  in-tri @ goal-at plan 0> 0= if   \ (no way in yet - the door still
        room-id in-tri @ in-at vec@ body-place  come-into-play exit   \ swinging open: inside at once)
    then
    mode-walk play  ['] walking-in step! ;

\ ---- through the next door (Pursuer_ThroughDoor) into the room beyond: into hers he comes
\ in; elsewhere on to the next door, or - the route over - his search there out of sight ----
: open-for-me ( exit -- )   \ (the shut door into her room taken - DoorHold_Take - and swung open: `door-held`)
    room-id swap stalker-kind doors-id send hold-door ;
: swing-open ( exit -- )  true self doors-id send swing-door ;
\ by a shut door into her room he waits in its doorway, beyond it, for the doors' answer: his to
\ open, it swings open and he walks in once it stands open; refused, he barges it open
\ (stalker/doorway.fs's barge-in) - never in past it shut (Pursuer_EnterRoom)
: in-doorway ( -- )
    barging @ if  exit  then
    room-id came-by @ room-exit-door door-open? if  walk-in  then ;
\ ---- the door he came into her room by, shut behind him once he is clear of it and it stands
\ open (Riccardo_DoorBreak, Daniella_DoorBreak, TintStalker_BreakDoor: DoorHold_Take then
\ DoorHold_Shut); Daniella in her rage leaves it open (Daniella_ExitArg). Debilitas, Lorenzo and
\ the rest leave doors as they are ----
0 constant leaves-doors   1 constant shuts-doors   2 constant shuts-doors-calm
: shuts? ( -- flag )   \ (in the mini-game's run they leave them open: Kind34_ExitArg, TintStalker_EndingDoor)
    hewie-played? if  false exit  then
    door-manner @ case
        shuts-doors of  true  endof
        shuts-doors-calm of  raging @ 0=  endof
        >r false r>
    endcase ;
: came-in-by ( exit -- )  shuts? if  to-shut !  else  drop  then ;
: clear-of? ( exit -- flag )  0 my-at vec@ door-in-area? 0= ;
: shut-behind ( -- )
    to-shut @ dup 0< if  drop exit  then
    my-room room-id <> if  drop -1 to-shut !  exit  then
    dup clear-of? 0= if  drop exit  then
    room-id over room-exit-door door-open? 0= if  drop exit  then
    self doors-id send pull-shut  -1 to-shut ! ;

: enter-her-room ( -- )   \ (into her room by the exit came-by: Pursuer_EnterRoom)
    came-by @ came-in-by
    came-by @ come-through 0= if  placed-anywhere  come-into-play exit  then
    room-id came-by @ room-exit-door door-kind open-way <> if   \ (a door not standing open: shut, held, locked)
        came-by @ open-for-me  -1 entering !  still-anim play-now  ['] in-doorway step!  exit
    then
    walk-in ;
\ what he was about in the room he leaves let go (Pursuer_ThroughDoor): whom he struck (+0x1760)
\ and who struck him (+0x1761), the wait before his next attack (+0x178C), his cry (+0x1764), his
\ blow's hold (+0x14D0), the growl after a stumble (+0x16F5), unshaken (+0x16F7), a stop's wait (+0x1798)
: door-passed ( -- )
    0 took !  0 by-her !  0 hold-off !  -1 cry !  0 growl-after !  0 unshaken !  0 stop-wait !
    freeze-t @ if  0 freeze-t !  unfrozen  then ;
: through ( exit -- )
    {: e :}  my-room e room-exit-leads {: room' exit' :}
    room' 0< if  my-room e room-exit-door avoid-door  goal-room @ route! drop  exit  then
    door-passed  exit' came-by !  room' body-room!  -1 body-tri!
    1 door-i +!  -1 making-for !  0 knock-t !
    my-room room-id = if  enter-her-room exit  then
    route-over? if
        my-mode @ heading-for-her = my-room goal-room @ = and if  searching mode!  then
        my-mode @ searching held-off within if  search-stops 150 * away-search !  then
        hewie-played? my-mode @ searching = and if  1 away-search !  then   \ (the run: Pursuer_Timer15s, 1)
        exit
    then
    next-door door-exit making-for !  stretch to-door f! ;

\ ---- a knock at a shut door into her room (Pursuer_KnockAtDoor: a sound by his kit's knocks -
\ Debilitas $28 or $23 -, bank 7, at the door on her side), then 60..120 frames' wait - longer
\ while she holds it ----
begin-structure /knock  field: kn.sound  ffield: kn.upto  end-structure
: knock-sound ( -- n )   \ (a roll against the knocks' running chances)
    rnd 100e f*  knocks  begin  dup kn.upto f@ fover f< while  /knock +  repeat  fdrop kn.sound @ ;
: knock ( exit -- )
    my-room swap room-exit-leads nip                                 ( exit' )
    dup 0< if  drop exit  then
    0 exit-spot drop  knock-sound 7 bank-sound-at
    rnd 60e f* 60e f+ f>s knock-t ! ;

\ ---- each frame out of sight on his route (Pursuer_TravelOffscreen) ----
: at-door ( -- )
    next-door dup door-exit {: door e :}
    e 0< if  goal-room @ route! drop  exit  then
    door door-kind case
        barred-way of  door avoid-door  goal-room @ route! drop  endof   \ (not for him: another way)
        held-way of  endof                                               \ (held shut: he waits)
        shut-way of
            my-room e room-exit-leads drop room-id =  my-mode @ held-off <> and if
                knock-t @ 0= if  e knock exit  then
                -1 knock-t +!  knock-t @ 1 > if  exit  then
            then  e through  endof
        >r e through r>
    endcase ;
\ ---- his footsteps heard through the walls (Pursuer_FootstepsThroughWalls): making for a door of
\ her room, within 80 of it, each 10 he closes in a step (his sounds 0 / 1 in turn, bank 7) out
\ beyond that door at his way left further off (100 more; a shut door 150: Character_RouteExitPoint) ----
create fs-a /vec allot  create fs-b /vec allot
: step-at ( exit -- )   \ (beyond its spot 1 along spot 2 -> 1, his way left + 100 / 150 out)
    {: e :}
    e 1 exit-spot drop fs-a vec!  e 2 exit-spot drop fs-b vec!
    room-id e room-exit-door door-open? if  100e  else  150e  then  to-door f@ f+  {: F: d :}
    fs-a v.x sf@ fs-b v.x sf@ f-  fs-a v.z sf@ fs-b v.z sf@ f-  {: F: dx dz :}
    dx dx f* dz dz f* f+ fsqrt {: F: len :}  len f0= if  exit  then
    d len f/ {: F: k :}
    world-held state-flag? hushed @ or if  exit  then
    step-foot @ 1 and 7 10 2  fs-a v.x sf@ dx k f* f+  fs-a v.y sf@  fs-a v.z sf@ dz k f* f+  sound-at
    1 step-foot +! ;   \ (always at its fullest: the original takes the step as 15)
: door-footsteps ( -- )
    next-door dup 0< if  drop exit  then  room-id door-exit-in              ( exit | -1 )
    dup 0< if  drop  -1e step-last f!  exit  then
    to-door f@ 80e f> if  drop  to-door f@ step-last f!  exit  then
    step-last f@ f0> if  step-last f@ to-door f@ f- step-acc f@ f+  else  0e  then  step-acc f!
    to-door f@ step-last f!
    step-acc f@ 10e f> 0= if  drop exit  then
    step-acc f@ 15e f- step-acc f!  step-at ;
: travel ( -- )
    route-over? if  exit  then
    to-door f@ f0> if  pace fnegate to-door f@ f+ to-door f!  door-footsteps  then
    to-door f@ f0> if  exit  then
    at-door ;

\ ---- his modes out of sight (Pursuer_ModesSearching, Pursuer_PlanToGoal): after her he
\ heads for her room, his route anew as she moves on; his clock out, heading there; at the
\ room he searches it (its stops x 150 frames), then waits about (900); his wait over, he
\ heads for her room unless she is in his or one next to it - then out by a door at random; held
\ off, home and away till the summoner takes him ----
: next-to-hers? ( -- flag )
    her-room dup 0< if  drop false exit  then
    dup my-room = if  drop true exit  then
    8 0 do  my-room i room-exit-leads drop over = if  drop true unloop exit  then  loop  drop false ;
: head-for-her ( -- )   \ (in the run, after Hewie: his room)
    hunting-dog @ if  room-id  else  her-room  then  dup 0< if  drop exit  then
    dup goal-room @ = route-over? 0= and if  drop exit  then
    avoid-clear  route! drop ;
: wander ( -- )   \ (Pursuer_Plan170: an exit at random he can go by)
    8 0 do
        8 s>f rnd f* f>s 7 min  my-room over room-exit-door              ( e door )
        dup 0< 0= if  door-kind usable-kind? if
            my-room swap room-exit-leads drop  route! drop  unloop exit
        then  else  drop  then  drop
    loop ;
\ held off: to the nearer of his own rooms (Debilitas_PickDestination: the shorter route); none
\ of his own (Daniella, Riccardo, ...: NPC_PickDestination's Npc_HeadRandomRoom - a room at
\ random, not hers), on by a random way out
: usable? ( exit -- flag )  my-room swap room-exit-door  dup 0< if  drop false exit  then  door-kind usable-kind? ;
: wander-off ( -- )   \ (a way out at random he can go by, not into hers)
    8 0 do
        8 s>f rnd f* f>s 7 min                                         ( e )
        dup usable?  my-room 2 pick room-exit-leads drop  room-id <> and if
            my-room swap room-exit-leads drop  route! drop  unloop exit
        then  drop
    loop ;
: head-home ( -- )
    own-a @ 0< if  wander-off exit  then
    own-a @ route! {: na :}
    own-b @ 0< if  na 0> if  own-a @ route! drop  then  exit  then
    own-b @ route! {: nb :}   \ (his route there now: the shorter kept)
    na 0> if  nb 0> 0=  nb na > or if  own-a @ route! drop  then  then ;
: modes-away ( -- )
    my-mode @ case
        after-her of  head-for-her  tick-down if  heading-for-her mode!  then  endof
        heading-for-her of  head-for-her  endof
        searching of  route-over? if  away-search @ if  -1 away-search +!
                  else  hewie-played? if  next-to-hers? 0= if  -1 hunting-dog !  heading-for-her mode!  head-for-her  then   \ (the run: nothing
                  else  waiting-about mode!  then  then  then  endof   \ near her - after Hewie instead)
        waiting-about of  route-over? if
                  tick-down if  next-to-hers? if  wander  else  heading-for-her mode!  head-for-her  then  900 mode-t !  then
              then  endof
        held-off of  own-room? 0= route-over? and if  head-home  then  endof   \ (held off he stays away: the summoner
    endcase ;                                                                    \ takes him out, and brings him in afresh)
: away ( -- )   \ (stunned a while once knocked out of her room by a door: stalker/doorway.fs; held
    \ off in his own room he stays there - Pursuer_TravelOffscreen)
    stun-t @ if  -1 stun-t +!  exit  then
    cond @ down-cond = if  exit  then   \ (down: where he fell, till she comes or the summoner takes him)
    modes-away
    my-mode @ held-off = own-room? and if  exit  then  travel ;

\ ---- the room being played changes (Fiona went through a door): his room no longer it - out
\ of sight, his walk stopped; she came into his - put on its floor (Pursuer_BackOnMesh: near
\ the door he made for - within 80 of it - there, facing it; else by the door he came in by,
\ facing in; else anywhere) ----
: gone-from-view ( -- )   \ (Pursuer_LeaveScreen: the room's doings forgotten - Pursuer_ResetForRoom)
    door-passed  -1 combo !  0 combo-step !
    0 out-going !  -1 body-tri!  my-path path-clear  ['] noop step !  no-search search-phase !  -1 anim !
    my-mode @ after-her = if  avoid-clear  her-room route! drop  then ;
: back-on-floor ( -- )
    making-for @ 0< 0=  to-door f@ 80e f<= and if
        making-for @ false place-at-exit if  come-into-play exit  then
    then
    came-by @ 0< 0= if  came-by @ true place-at-exit if  come-into-play exit  then  then
    placed-anywhere  come-into-play ;
: room-changed ( room -- )
    in-game @ 0= if  drop exit  then
    my-room = if  back-on-floor  else  gone-from-view  then ;
