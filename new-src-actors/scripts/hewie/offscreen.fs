\ hewie/offscreen.fs - Hewie in the rooms Fiona isn't in: when she leaves his room he follows
\ her the way she went (Hewie_Vt34), or later finds his own way through the house door by door
\ (Hewie_StateToDoor / State1920, Hewie_ThroughExit); when he or she comes into the other's
\ room he is put there (Hewie_Arrive). Off screen nothing moves him but time: each stretch to
\ the next door takes its length at his pace.
IN: hewie.offscreen
USING: engine actors common paths messages hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states ;

defer when-idle ( -- )   ' noop is when-idle     \ Hewie_WhenIdle (hewie.mind)
: avoid-clear ( -- )  his-avoid 13 cells 0 fill ;
: avoid-door ( door -- )   \ not that way again
    his-avoid over 5 rshift cells +  swap 31 and 1 swap lshift  over @ or swap ! ;

\ Character_Route: the doors to room `to` (> 0 a way, 0 there, -1 none), and door_from_path:
\ the first one and - the original's own measure - its triangle's number as the way to it
: route-to ( to -- n )  his-room swap hewie-kind -1 his-avoid route-avoiding ;
: door-tri ( door room -- tri )   \ (Rooms_DoorTri: its triangle on that room's side, -1 none)
    >r door-sides                                         ( r0 e0 t0 r1 e1 t1 )
    2 pick r@ = if  >r 2drop 2drop drop r> r> drop exit  then
    2drop drop  rot r> = if  nip  else  2drop -1  then ;
: door-from-path ( -- )  0 route-door dup his-route-door !  his-room door-tri s>f his-left f! ;
: door-usable? ( door -- flag )  door-open? ;   \ (open or a doorway, not locked)

\ Hewie_ThroughExit: into the room beyond exit `exit` of his (out of play there); -1 none
: through-exit ( exit -- 0 | -1 )
    his-room swap room-exit-leads  over 0< if  2drop -1 exit  then
    his-door !  body-room!  -1 body-tri!  0 his-at-door !  $17 his-sub !  1 away!  0 ;
\ arrived (in the room being played): seen, what he is alert to taken in
: arrived ( -- )  0 away!  his-alert @ his-alert-was ! ;
\ the animation he comes in with when none: down 0x1002, hurt 6, else standing
: arrive-anim ( -- )  his-cond @ case  2 of  $1002  endof  1 of  6  endof  >r 0 r>  endcase  play ;

\ ---- off screen: making for the door, then through it ----
: travel-speed ( F: -- s )
    his-action @ $39 = if  10e  else  his-cond @ 1 = if  0.38e  else  1.6e  then  then ;
create in-at 12 allot  create out-at 12 allot
\ come into the room being played: in the doorway of the door he made for, facing out of it
\ (as the original), walking
: come-in ( -- )
    his-door @ 1 exit-spot drop in-at vec!
    his-door @ 0 exit-spot drop out-at vec!
    in-at vec@ his-place  in-at out-at vec-heading his-yaw!
    $201 play
    his-action @ $39 = panic5-chance by-chance and if  $4F  else  $70  then  0 want
    arrived ;
: st-between ( -- )   \ Hewie_State1920: across the room he came into, toward the next door
    his-left f@ travel-speed f- fdup his-left f!  f0< 0= if  exit  then
    his-action @ case
        $33 of  his-room his-noise-room @ = if  when-idle  else  $33 start  then  endof
        $2C of  $2C start  endof  $2D of  $2D start  endof  $32 of  $32 start  endof
        $35 of  $35 start  endof  $39 of  $39 start  endof
    endcase ;
: lost-way ( -- )  0 his-at-door !  -1 body-tri!  his-action @ $35 = if  $35 0 want  else  when-idle  then ;
: st-to-door ( -- )
    his-left f@ travel-speed f- fdup his-left f!  f0< 0= if  exit  then  0e his-left f!
    his-route-door @ his-room door-exit-in dup his-door !
    0< if  lost-way exit  then
    his-route-door @ door-usable? 0= if  his-route-door @ avoid-door  lost-way exit  then   \ it let him down
    0 his-at-door !
    his-door @ through-exit drop
    his-room room-id = if  come-in exit  then
    his-route-door @ his-room door-tri s>f his-left f!  ['] st-between behave ;

\ ---- the off-screen actions (0x2C..0x39) ----
: guarded-idle ( -- )   \ (Hewie_WhenIdle may pick one that comes straight back here)
    his-idle-depth @ 6 > if  $2E 0 want exit  then  1 his-idle-depth +!  when-idle  -1 his-idle-depth +! ;
: to-door ( cmd -- )  6 his-mode !  his-cmd !  ['] st-to-door behave ;
: route-played ( -- )   \ 0x2C / 0x2D / 0x39: out of the room toward where she is
    his-at-door @ 0= if
        fiona-room @ route-to 0> 0= if  guarded-idle exit  then  door-from-path
    then  $CF to-door ;
: exit-random ( seen -- seen' e )   \ an exit not tried yet, from a random one
    8 roll begin  2dup 1 swap lshift and while  1+ 7 and  repeat  tuck 1 swap lshift or swap ;
variable or-seen
: off-random ( not-hers? -- )   \ 0x32 / 0x35: out by an open exit at random (0x35: not to her room)
    >r 0 or-seen !
    8 0 do
        or-seen @ exit-random swap or-seen !                           ( e )
        his-room over room-exit-door dup 0< 0= if
            door-usable? if
                his-room swap room-exit-leads drop                     ( room' )
                dup fiona-room @ = r@ and 0= if
                    route-to 0> if  0 his-at-door !  $17 his-sub !  door-from-path
                        r> drop  $DF to-door  unloop exit
                    then
                else  drop  then
            else  drop  then
        else  2drop  then
    loop  r> drop  guarded-idle ;
: to-noise ( -- )   \ 0x33: toward the room he heard something in
    his-noise-room @ route-to 0> 0= if  guarded-idle exit  then
    0 his-at-door !  door-from-path  $DF to-door ;

\ ---- Hewie_Vt34: Fiona leaves his room by exit `exit` - he follows her that way if he can get
\ to it, else by another, else he stays; by her or lying by her he first waits a while ----
create ex-at 12 allot
: exit-reachable? ( exit -- flag )   \ a way to its far point
    2 exit-spot dup 0< if  drop fdrop fdrop fdrop false exit  then  ex-at vec!  ex-at plan-to ;
: fiona-left ( exit -- )
    his-room room-id <> if  drop exit  then
    his-hp @ 0= if  1 hp!  1 cond!  then
    0 his-2b !  his-floor body-mask!
    avoid-clear  0 his-at-door !  1 his-hide !
    dup 0< 0= if  dup exit-reachable? if  0 his-hide !  1 his-at-door !  then  then
    his-hide @ if
        8 0 do  i over <> if  i exit-leads nip 0< 0= if
            i exit-reachable? if  0 his-hide !  leave  then
        then  then  loop
    then
    his-action @ $52 = his-hp @ 0= and if  drop exit  then
    his-action @ $76 = if  drop $77 0 want exit  then
    his-hide @ if  drop $36 0 want exit  then
    his-at-door @ if
        dup his-room swap room-exit-door his-route-door !
        rest his-left f!
    then  drop
    his-action @ 2 - 2 u< if
        his-wait @ 150 + his-wanted !  anim-group his-t2 !  $2F 0 want
    else
        avoid-clear  his-waiting @ 0= if  $2C  else  0  then  0 want
    then ;

\ ---- Hewie_Arrive: he is in the room being played after all (Fiona came into his room) ----
: arrive ( -- arrived? )
    his-room room-id <> if  false exit  then
    his-action @ dup 0= over $2C $3A within or over $52 = or swap $77 = or 0= if  false exit  then
    0 his-alert !  -1 his-yelp-anim !  false                              ( placed? )
    his-action @ case
        $2F of  -150 his-wait +!  his-wait @ 0> if
                    his-t2 @ 1 = if  his-cond @ 1 = if  7  else  1  then  play  then
                    his-t2 @ 2 = if  his-cond @ 1 = if  7  else  2  then  play  then
                else  0 his-wait !  then  endof
        $77 of  his-yelp-anim @ play  endof
    endcase
    \ where he was, if that is open floor; else by the door he came through, facing anywhere
    his-tri dup 0< 0= if
        nav-flags self body-mask and 0= if  his-at vec@ his-place  drop true  then
    else  drop  then
    0= if
        his-action @ dup $2F <> swap $77 <> and if  arrive-anim  then
        his-door @ 0 max 2 exit-spot dup 0< if
            drop fdrop fdrop fdrop  fiona-at vec@ his-place
        else  drop his-place  then
        rnd 2e f* 1e f- pi f* his-yaw!
    then
    his-cond @ 2 = if  $52 0 want
    else his-action @ $77 = if  $76 0 want
    else his-action @ $2F = his-t1 @ 0> and if  his-wait @ his-wanted !  3 0 want
    else  his-hit-t @ if  $24  else  0  then  0 want
    then then then
    arrived  true ;
