\ hewie/offscreen.fs - Hewie in the rooms Fiona isn't in: when she leaves his room he follows
\ her the way she went (Hewie_Vt34), or later finds his own way through the house door by door
\ (Hewie_StateToDoor / State1920, Hewie_ThroughExit); when he or she comes into the other's
\ room he is put there (Hewie_Arrive). Off screen nothing moves him but time: each stretch to
\ the next door takes its length at his pace.
IN: hewie.offscreen
USING: engine actors common paths vectors messages hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.model hewie.moving hewie.states ;

defer when-idle ( -- )   ' noop is when-idle     \ Hewie_WhenIdle (hewie.mind)
: avoid-clear ( -- )  his-avoid 13 cells 0 fill ;
: avoid-door ( door -- )   \ not that way again (a bit a door)
    {: door :}  his-avoid door 5 rshift cells + {: a :}  a @  1 door 31 and lshift or  a ! ;

\ Character_Route: the doors to room `to` (> 0 a way, 0 there, -1 none) from the half of his
\ room he is on - his-way, or (-1) the half the exit he came in by opens onto, looked up only
\ where the door marks it (the walker's kind then counts) - and door_from_path: the first one
\ and - the original's own measure - its triangle's number as the way to it
: his-start ( -- way kind )
    his-way @ dup -1 <> if  0 exit  then  drop
    his-door @ dup 0< if  drop -1  else  his-room swap 1 exit-way  then  hewie-kind ;
: route-to ( to -- n )  {: dest :}  his-start {: way kind :}  his-room way dest -1 kind -1 his-avoid route-sides ;
: door-tri ( door room -- tri )   \ (Rooms_DoorTri: its triangle on that room's side, -1 none)
    {: room :}  door-sides {: r0 e0 t0 r1 e1 t1 :}
    r1 room = if  t1  else  r0 room = if  t0  else  -1  then then ;
: door-from-path ( -- )  0 route-door dup his-route-door !  his-room door-tri s>f his-left f! ;
: door-usable? ( door -- flag )  door-open? ;   \ (open or a doorway, not locked)

\ Hewie_ThroughExit: into the room beyond exit `exit` of his (out of play there); -1 none
: through-exit ( exit -- 0 | -1 )
    his-room swap room-exit-leads  over 0< if  2drop -1 exit  then
    2dup 0 exit-way his-way !   \ (the half its exit opens onto)
    his-door !  body-room!  -1 body-tri!  0 his-at-door !  travelling-sub his-sub !  1 away!  0 ;
\ arrived (in the room being played): seen, what he is alert to taken in
: arrived ( -- )  0 away!  his-alert @ his-alert-was ! ;
\ the animation he comes in with when none: down, hurt, else standing
: arrive-anim ( -- )  his-cond @ case  down-cond of  down-anim  endof  hurt-cond of  stand-hurt-anim  endof  >r stand-anim r>  endcase  play ;

\ ---- off screen: making for the door, then through it ----
: travel-speed ( F: -- s )
    his-action @ to-her-cry-act = if  10e  else  his-cond @ hurt-cond = if  0.38e  else  1.6e  then  then ;
create in-at /vec allot  create out-at /vec allot
\ come into the room being played: in the doorway of the door he made for, facing out of it
\ (as the original), walking
: come-in ( -- )
    his-door @ 1 exit-spot drop in-at vec!
    his-door @ 0 exit-spot drop out-at vec!
    in-at vec@ his-place  in-at out-at vec-heading his-yaw!
    trot-anim play
    his-action @ to-her-cry-act = panic5-chance by-chance and if  answer-call-act  else  dash-act  then  0 want
    arrived ;
: st-between ( -- )   \ Hewie_State1920: across the room he came into, toward the next door
    his-left f@ travel-speed f- fdup his-left f!  f0< 0= if  exit  then
    his-action @ case
        to-noise-act of  his-room his-noise-room @ = if  when-idle  else  to-noise-act start  then  endof
        to-her-room-act of  to-her-room-act start  endof  back-to-her-act of  back-to-her-act start  endof
        out-random-act of  out-random-act start  endof  out-random-away-act of  out-random-away-act start  endof
        to-her-cry-act of  to-her-cry-act start  endof
    endcase ;
: lost-way ( -- )
    0 his-at-door !  -1 body-tri!  his-action @ out-random-away-act = if  out-random-away-act 0 want  else  when-idle  then ;
: st-to-door ( -- )
    his-left f@ travel-speed f- fdup his-left f!  f0< 0= if  exit  then  0e his-left f!
    his-route-door @ his-room door-exit-in dup his-door !
    0< if  lost-way exit  then
    his-route-door @ door-usable? 0= if  his-route-door @ avoid-door  lost-way exit  then   \ it let him down
    0 his-at-door !
    his-door @ through-exit drop
    his-room room-id = if  come-in exit  then
    his-route-door @ his-room door-tri s>f his-left f!  ['] st-between behave ;

\ ---- the off-screen actions (to-her-room-act .. to-her-cry-act) ----
: guarded-idle ( -- )   \ (Hewie_WhenIdle may pick one that comes straight back here)
    his-idle-depth @ 6 > if  wait-off-act 0 want exit  then  1 his-idle-depth +!  when-idle  -1 his-idle-depth +! ;
: to-door ( cmd -- )  away-mode his-mode !  his-cmd !  ['] st-to-door behave ;
: route-played ( -- )   \ (to-her-room-act, back-to-her-act, to-her-cry-act: out of the room toward where she is)
    his-at-door @ 0= if
        fiona-room @ route-to 0> 0= if  guarded-idle exit  then  door-from-path
    then  breaks-away on-noise but to-door ;
: exit-random ( seen -- seen' e )   \ an exit not tried yet, from a random one
    8 roll begin  2dup 1 swap lshift and while  1+ 7 and  repeat  tuck 1 swap lshift or swap ;
: off-random ( not-hers? -- )   \ out by an open exit at random (out-random-away-act: not to her room)
    {: not-hers | seen :}
    8 0 do
        seen exit-random swap to seen                                    ( e )
        his-room over room-exit-door dup 0< 0= if
            door-usable? if
                his-room swap room-exit-leads drop                     ( room' )
                dup fiona-room @ = not-hers and 0= if
                    route-to 0> if  0 his-at-door !  travelling-sub his-sub !  door-from-path
                        breaks-away to-door  unloop exit
                    then
                else  drop  then
            else  drop  then
        else  2drop  then
    loop  guarded-idle ;
: to-noise ( -- )   \ (to-noise-act: toward the room he heard something in)
    his-noise-room @ route-to 0> 0= if  guarded-idle exit  then
    0 his-at-door !  door-from-path  breaks-away to-door ;

\ ---- Hewie_Vt34: Fiona leaves his room by exit `exit` - he follows her that way if he can get
\ to it, else by another, else he stays; by her or lying by her he first waits a while ----
create ex-at /vec allot
: exit-reachable? ( exit -- flag )   \ a way to its far point
    2 exit-spot dup 0< if  drop fdrop fdrop fdrop false exit  then  ex-at vec!  ex-at plan-to ;
: fiona-left ( exit -- )
    his-room room-id <> if  drop exit  then
    his-hp @ 0= if  1 hp!  hurt-cond cond!  then
    0 his-2b !  his-floor body-mask!
    avoid-clear  0 his-at-door !  1 his-hide !
    dup 0< 0= if  dup exit-reachable? if  0 his-hide !  1 his-at-door !  then  then
    his-hide @ if
        8 0 do  i over <> if  i exit-leads nip 0< 0= if
            i exit-reachable? if  0 his-hide !  leave  then
        then  then  loop
    then
    his-action @ knocked-down-act = his-hp @ 0= and if  drop exit  then
    his-action @ yelp-anim-act = if  drop hidden-act 0 want exit  then
    his-hide @ if  drop hide-act 0 want exit  then
    his-at-door @ if
        dup his-room swap room-exit-door his-route-door !
        rest his-left f!
    then  drop
    his-action @ dup stay-act = swap stay-a-while-act = or if   \ (by her: a while first)
        his-wait @ 150 + his-wanted !  anim-group his-t2 !  wait-a-while-act 0 want
    else
        avoid-clear  his-waiting @ 0= if  to-her-room-act  else  choose-act  then  0 want
    then ;

\ ---- Hewie_Arrive: he is in the room being played after all (Fiona came into his room) ----
: arrive ( -- arrived? )
    his-room room-id <> if  false exit  then
    his-action @ dup choose-act = over to-her-room-act story-anim-act within or  over knocked-down-act = or  swap hidden-act = or
    0= if  false exit  then
    0 his-alert !  -1 his-yelp-anim !
    his-room his-door @ 0 exit-way his-way !   \ (the half by the exit he came through)
    false                                                                ( placed? )
    his-action @ case
        wait-a-while-act of  -150 his-wait +!  his-wait @ 0> if   \ (as he was: sitting, lying)
                    his-t2 @ in-sitting = if  his-cond @ hurt-cond = if  lie-hurt-anim  else  sit-anim  then  play  then
                    his-t2 @ in-lying = if  his-cond @ hurt-cond = if  lie-hurt-anim  else  lie-anim  then  play  then
                else  0 his-wait !  then  endof
        hidden-act of  his-yelp-anim @ play  endof
    endcase
    \ where he was, if that is open floor; else by the door he came through, facing anywhere
    his-tri dup 0< 0= if
        nav-flags self body-mask and 0= if  his-at vec@ his-place  drop true  then
    else  drop  then
    0= if
        his-action @ dup wait-a-while-act <> swap hidden-act <> and if  arrive-anim  then
        his-door @ 0 max 2 exit-spot dup 0< if
            drop fdrop fdrop fdrop  fiona-at vec@ his-place
        else  drop his-place  then
        rnd 2e f* 1e f- pi f* his-yaw!
    then
    his-cond @ down-cond = if  knocked-down-act 0 want
    else his-action @ hidden-act = if  yelp-anim-act 0 want
    else his-action @ wait-a-while-act = his-t1 @ 0> and if  his-wait @ his-wanted !  stay-a-while-act 0 want
    else  his-hit-t @ if  away-from-her-act  else  choose-act  then  0 want
    then then then
    arrived  true ;
