\ stalker/travel.fs - a stalker out of sight, from room to room (src/game/pursuer.c): his route
\ through the doors to the room he heads for (Character_Route), each stretch to the next door
\ counted down at his pace (Pursuer_TravelOffscreen), the doors as he finds them (Npc_ExitKind:
\ held, not for him, a way through, shut), a knock at a shut door into Fiona's room
\ (Pursuer_KnockAtDoor), going through (Pursuer_ThroughDoor), his modes out there
\ (Pursuer_ModesSearching off screen, Pursuer_SearchRouteIn) and coming into the room being
\ played (Pursuer_EnterRoom; Pursuer_BackOnMesh when she comes into his).
IN: stalker.travel
USING: engine game-state actors messages common facts flag-names paths doors stalker.state stalker.senses stalker.moving stalker.search ;

: my-room ( -- room )  self body-room ;
: away? ( -- flag )  in-game @ if  my-room room-id <>  else  false  then ;
: her-room ( -- room )  fiona dup 0< if  drop -1 exit  then  dup body? if  body-room  else  drop -1  then ;

\ ---- the doors that let him down: not that way again until a fresh route ----
: avoid-clear ( -- )  avoid 13 cells 0 fill ;
: avoid-door ( door -- )
    dup 0< if  drop exit  then
    avoid over 5 rshift cells +  swap 31 and 1 swap lshift  over @ or swap ! ;

\ ---- his route to a room (> 0 doors, 0 there, -1 none); the next door, its exit here ----
: next-door ( -- door )  door-i @ doors-n @ < if  door-i @ cells doors + @  else  -1  then ;
: door-exit ( door -- exit )  dup 0< if  exit  then  my-room door-exit-in ;
\ Rooms_DoorTri: the door's triangle on a room's side - the original's own measure of the way
\ to it (its number); -1 none
: door-tri ( door room -- tri )
    >r dup 0< if  r> 2drop -1 exit  then  door-sides               ( r0 e0 t0 r1 e1 t1 )
    2 pick r@ = if  >r 2drop 2drop drop r> r> drop exit  then
    2drop drop  rot r> = if  nip  else  2drop -1  then ;
\ Npc_NodeDistance: from the door he came in by to the next one (each its triangle's number)
: came-door ( -- door )  came-by @ dup 0< if  exit  then  my-room swap room-exit-door ;
: stretch ( -- ) ( F: -- d )
    next-door my-room door-tri 0 max s>f
    came-door dup 0< 0= over next-door <> and if  my-room door-tri 0 max s>f f+  else  drop  then ;
: route! ( room -- n )   \ (planned: the first stretch set)
    dup goal-room !  my-room swap stalker-kind -1 avoid route-avoiding
    dup 0> if
        dup 16 min 0 do  i route-door  i cells doors + !  loop
        dup 16 min doors-n !  0 door-i !
        next-door door-exit making-for !  stretch to-door f!
    else  0 doors-n !  0 door-i !  -1 making-for !  then ;
: route-over? ( -- flag )  door-i @ doors-n @ < 0= ;

\ ---- a door as he finds it (Npc_ExitKind): 2 held, 3 not for him (locked against him, locked,
\ stuck), 4 a way through (a doorway, open), 5 shut ----
: door-kind ( door -- k )
    dup 0< if  drop 3 exit  then
    dup fixed? if  drop 4 exit  then
    dup door-held? if  drop 2 exit  then
    dup against stalker-kind kind-bit and if  drop 3 exit  then
    dup door-locked? if  drop 3 exit  then
    dup state@ stuck-bit and if  drop 3 exit  then
    state@ opened-bit and if  4  else  5  then ;

\ ---- his pace out of sight (a stretch's length a frame): after her 1.6..1.8, searching or
\ waiting 0.6, heading for her room or held off 1; while she hides 2 ----
: pace ( F: -- p )
    fiona-hidden state-flag? if  2e exit  then
    my-mode @ case
        0 of  rnd 0.2e f* 1.6e f+  endof
        2 of  0.6e  endof  3 of  0.6e  endof
        >r 1e r>
    endcase ;

\ ---- coming into the room being played by an exit: just inside it, facing in, walking in
\ (Pursuer_EnterRoom); after her and not seeing her, or come to her room, he searches it ----
create in-at 12 allot  create out-at 12 allot
: place-at-exit ( exit facing-in? -- ok? )
    >r  dup 1 exit-spot dup 0< if  r> drop 2drop fdrop fdrop fdrop false exit  then
    >r in-at vec!  0 exit-spot drop out-at vec!
    room-id r> in-at vec@ body-place
    r> if  out-at in-at  else  in-at out-at  then  vec-heading body-turn  true ;
: placed-anywhere ( -- )   \ (Actor_TeleportRandom: no exit to stand at)
    random-tri dup 0< if  drop fiona body-tri  then
    dup 0< if  drop exit  then  room-id swap dup tri-center body-place ;
: in-play ( -- )   \ in the room being played now: his search there, or after her
    0 away-search !  -1 anim !  0 play-now  my-path path-clear
    my-mode @ 0= if  chase-start exit  then
    my-mode @ 1 = if  2 mode!  then
    search-again ;

\ ---- through the next door (Pursuer_ThroughDoor) into the room beyond: into hers he comes
\ in; elsewhere on to the next door, or - the route over - his search there out of sight ----
: doors-id ( -- id )  s" doors" actor-named ;
: open-for-me ( exit -- )   \ (the shut door into her room taken - DoorHold_Take - and swung open: `door-held`)
    room-id swap stalker-kind doors-id send hold-door ;
: swing-open ( exit -- )  true self doors-id send swing-door ;
: through ( exit -- )
    my-room over room-exit-leads                                     ( exit room' exit' )
    over 0< if  2drop  my-room swap room-exit-door avoid-door  goal-room @ route! drop  exit  then
    rot drop  came-by !  body-room!  -1 body-tri!
    1 door-i +!  -1 making-for !  0 knock-t !
    my-room room-id = if
        room-id came-by @ room-exit-door door-kind 5 = if  came-by @ open-for-me  then
        came-by @ true place-at-exit 0= if  placed-anywhere  then
        in-play exit
    then
    route-over? if
        my-mode @ 1 = my-room goal-room @ = and if  2 mode!  then
        my-mode @ 2 4 within if  search-stops 150 * away-search !  then  exit
    then
    next-door door-exit making-for !  stretch to-door f! ;

\ ---- a knock at a shut door into her room (Pursuer_KnockAtDoor: his sounds $28 or $23, bank
\ 7, at the door on her side), then 60..120 frames' wait - longer while she holds it ----
: knock ( exit -- )
    my-room swap room-exit-leads nip                                 ( exit' )
    dup 0< if  drop exit  then
    0 exit-spot drop  rnd 0.5e f< if  $28  else  $23  then  7 bank-sound-at
    rnd 60e f* 60e f+ f>s knock-t ! ;

\ ---- each frame out of sight on his route (Pursuer_TravelOffscreen) ----
: at-door ( -- )
    next-door dup door-exit                                          ( door exit )
    dup 0< if  2drop  goal-room @ route! drop  exit  then
    over door-kind case
        3 of  drop avoid-door  goal-room @ route! drop  endof         \ (not for him: another way)
        2 of  2drop  endof                                            \ (held shut: he waits)
        5 of  nip
              my-room over room-exit-leads drop room-id =  my-mode @ 4 <> and if
                  knock-t @ 0= if  knock exit  then
                  -1 knock-t +!  knock-t @ 1 > if  drop exit  then
              then  through  endof
        >r nip through r>
    endcase ;
: travel ( -- )
    route-over? if  exit  then
    to-door f@ f0> if  pace fnegate to-door f@ f+ to-door f!  then
    to-door f@ f0> if  exit  then
    at-door ;

\ ---- his modes out of sight (Pursuer_ModesSearching, Pursuer_PlanToGoal): after her he
\ heads for her room, his route anew as she moves on; his clock out, heading there; at the
\ room he searches it (its stops x 150 frames), then waits about (900); his wait over, he
\ heads for her room unless she is in his or one next to it - then out by a door at random ----
: next-to-hers? ( -- flag )
    her-room dup 0< if  drop false exit  then
    dup my-room = if  drop true exit  then
    8 0 do  my-room i room-exit-leads drop over = if  drop true unloop exit  then  loop  drop false ;
: head-for-her ( -- )
    her-room dup 0< if  drop exit  then
    dup goal-room @ = route-over? 0= and if  drop exit  then
    avoid-clear  route! drop ;
create seen-exits 8 cells allot
: wander ( -- )   \ (Pursuer_Plan170: an exit at random he can go by)
    8 0 do
        8 s>f rnd f* f>s 7 min  my-room over room-exit-door              ( e door )
        dup 0< 0= if  door-kind dup 4 = swap 5 = or if
            my-room swap room-exit-leads drop  route! drop  unloop exit
        then  else  drop  then  drop
    loop ;
: modes-away ( -- )
    my-mode @ case
        0 of  head-for-her  tick-down if  1 mode!  then  endof
        1 of  head-for-her  endof
        2 of  route-over? if  away-search @ if  -1 away-search +!  else  3 mode!  then  then  endof
        3 of  route-over? if
                  tick-down if  next-to-hers? if  wander  else  1 mode!  head-for-her  then  900 mode-t !  then
              then  endof
        4 of  tick-down if  0 mode!  then  endof
    endcase ;
: away ( -- )  modes-away  travel ;

\ ---- the room being played changes (Fiona went through a door): his room no longer it - out
\ of sight, his walk stopped; she came into his - put on its floor (Pursuer_BackOnMesh: near
\ the door he made for - within 80 of it - there, facing it; else by the door he came in by,
\ facing in; else anywhere) ----
: gone-from-view ( -- )
    -1 body-tri!  my-path path-clear  ['] noop step !  0 search-phase !  -1 anim !
    my-mode @ 0= if  avoid-clear  her-room route! drop  then ;
: back-on-floor ( -- )
    making-for @ 0< 0=  to-door f@ 80e f<= and if
        making-for @ false place-at-exit if  in-play exit  then
    then
    came-by @ 0< 0= if  came-by @ true place-at-exit if  in-play exit  then  then
    placed-anywhere  in-play ;
: room-changed ( room -- )
    in-game @ 0= if  drop exit  then
    my-room = if  back-on-floor  else  gone-from-view  then ;
