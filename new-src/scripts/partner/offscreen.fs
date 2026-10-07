\ partner/offscreen.fs - Hewie in the rooms Fiona isn't in (src/game/hewie.c): when she leaves
\ his room he follows her the way she went (Hewie_Vt34), or later finds his own way through the
\ house door by door (Hewie_StateToDoor, Hewie_ThroughExit: the route planner), and when he or
\ she comes into the other's room he is put there (Hewie_Arrive). Off screen nothing moves him
\ but the time each stretch takes.
IN: partner.offscreen
USING: engine game-state events.core events.words chars partner.core partner.tables partner.moves partner.route partner.states ;

variable h-door  -1 h-door !         \ the character's +0x2C door: the exit he came in by
variable h-route-door                \ +0x14C0: the door he is making for
fvariable h-left                     \ +0x14C4: how far there is left to it
variable h-at-door                   \ +0xF3590: 1 his way to it planned in the room, 0 counting
variable h-noise-room  -1 h-noise-room !   \ +0xF3594: the room of a noise he heard
variable h-hide                      \ +0xF3583: he stays where he is
create h-avoid 13 cells allot        \ +0x148C: the doors that let him down (a bit a door)
: avoid-clear ( -- )  h-avoid 13 cells 0 fill ;
avoid-clear
defer idle-off ( -- )   ' noop is idle-off     \ Hewie_WhenIdle (partner.brain)

: disabled! ( flag -- )  0<> 1 and him character char.disabled l! ;
: room! ( room -- )  him character char.room l! ;

\ Character_Route: the doors to room `to` (> 0 a way, 0 there, -1 none), and door_from_path: the
\ first one and - the original's own measure - its triangle number as the way to it
: route-to ( to -- n )  h-room swap h-avoid find-route ;
: door-from-path ( -- )  route @ dup h-route-door !  h-room door-tri s>f h-left f! ;

\ Hewie_ThroughExit: into the room beyond exit `exit` of his (out of play there); -1 none
: through-exit ( exit -- 0 | -1 )
    h-room swap room-exit-leads  over 0< if  2drop -1 exit  then
    h-door !  room!  -1 him c-tri!  0 h-at-door !  $17 him character char.sub l!  -1 disabled!  0 ;

\ arrived (in the room being played): seen, what he is alert to taken in. (Handing him to the
\ room's objects and the sound of his steps: with the rooms' phase.)
: arrived ( -- )  0 disabled!  h-alert @ h-alert-was ! ;
\ the animation he comes in with when none: down 0x1002, hurt 6, else standing
: arrive-anim ( -- )  h-cond case  2 of  $1002  endof  1 of  6  endof  >r 0 r>  endcase  play ;

\ ---- off screen: making for the door, then through it (Hewie_StateToDoor / State1920) ----
: travel-speed ( F: -- s )
    h-action @ $39 = if  10e  else  h-cond 1 = if  0.38e  else  1.6e  then  then ;
create in-at 12 allot  create out-at 12 allot
: trust-chance? ( table -- flag )  trust-of 0 100 clamp  100 roll > ;   \ by_chance
\ come into the room being played: in the doorway of the door he made for, facing in, walking
: come-in ( -- )
    h-door @ 1 exit-spot drop in-at vec!
    h-door @ 0 exit-spot drop out-at vec!
    in-at vec@ him c-place!  in-at out-at vec-heading h-yaw!
    $201 play
    h-action @ $39 = panic5-chance trust-chance? and if  $4F  else  $70  then  0 want
    arrived ;
: st-between ( -- )   \ Hewie_State1920: across the room he came into, toward the next door
    h-left f@ travel-speed f- fdup h-left f!  f0< 0= if  exit  then
    h-action @ case
        $33 of  h-room h-noise-room @ = if  idle-off  else  $33 start  then  endof
        $2C of  $2C start  endof  $2D of  $2D start  endof  $32 of  $32 start  endof
        $35 of  $35 start  endof  $39 of  $39 start  endof
    endcase ;
: missed ( -- )   \ the door let him down: not that way again
    h-route-door @ h-avoid bit!  0 h-at-door !  -1 him c-tri!
    h-action @ $35 = if  $35 0 want  else  idle-off  then ;
: st-to-door ( -- )
    h-left f@ travel-speed f- fdup h-left f!  f0< 0= if  exit  then  0e h-left f!
    h-route-door @ h-room door-exit dup h-door !
    dup 0< if  drop 0 h-at-door !  -1 him c-tri!  h-action @ $35 = if  $35 0 want  else  idle-off  then  exit  then
    drop
    h-route-door @ dup door-open? over door-locked 0= and swap door-passable? and 0= if  missed exit  then
    0 h-at-door !
    h-door @ through-exit drop
    h-room played-room = if  come-in exit  then
    h-route-door @ h-room door-tri s>f h-left f!  ['] st-between behave ;

\ ---- the off-screen actions (Hewie_SetAction 0x2C..0x39) ----
variable idle-depth
: guarded-idle ( -- )   \ (Hewie_WhenIdle can pick one that comes straight back to it)
    idle-depth @ 6 > if  $2E 0 want exit  then  1 idle-depth +!  idle-off  -1 idle-depth +! ;
: to-door ( cmd -- )  6 him character char.mode l!  h-cmd !  ['] st-to-door behave ;
: route-played ( -- )   \ 0x2C / 0x2D / 0x39: out of the room toward where she is
    h-at-door @ 0= if
        played-room route-to 0> 0= if  guarded-idle exit  then  door-from-path
    then  $CF to-door ;
: exit-random ( seen -- seen' e )   \ an exit not tried yet, from a random one
    8 roll begin  2dup 1 swap lshift and while  1+ 7 and  repeat  tuck 1 swap lshift or swap ;
: off-random ( avoid-played? -- )   \ 0x32 / 0x35: out by an open exit at random
    >r 0  8 0 do
        exit-random                                                    ( seen e )
        h-room over room-exit-door dup 0< 0= if
            dup door-open? over door-locked 0= and swap door-passable? and if
                h-room swap room-exit-leads drop                       ( seen room' )
                dup played-room = r@ and 0= if
                    route-to 0> if  0 h-at-door !  $17 him character char.sub l!  door-from-path
                        drop r> drop  $DF to-door  unloop exit
                    then
                else  drop  then
            else  drop  then
        else  2drop  then
    loop  drop r> drop  guarded-idle ;
: to-noise ( -- )   \ 0x33: toward the room he heard something in
    h-noise-room @ route-to 0> 0= if  guarded-idle exit  then
    0 h-at-door !  door-from-path  $DF to-door ;

\ ---- Hewie_Vt34: Fiona leaves his room by exit `exit` - he follows her that way if he can get
\ to it, else by another, else he stays; following her or lying by her he first waits a while
\ ----
create ex-at 12 allot
: exit-reachable? ( exit -- flag )   \ a way to its far point
    2 exit-spot dup 0< if  drop fdrop fdrop fdrop false exit  then  ex-at vec!  ex-at plan-to ;
: fiona-left ( exit -- )
    h-room played-room <> if  drop exit  then
    h-hp 0= if  1 him character char.hp l!  1 him character char.cond l!  then
    0 h-2b !  $29020008 him c-mask!
    avoid-clear  0 h-at-door !  1 h-hide !
    dup 0< 0= if  dup exit-reachable? if  0 h-hide !  1 h-at-door !  then  then
    h-hide @ if
        8 0 do  i over <> if  i exit-leads nip 0< 0= if
            i exit-reachable? if  0 h-hide !  leave  then
        then  then  loop
    then
    h-action @ $52 = h-hp 0= and if  drop exit  then
    h-action @ $76 = if  drop $77 0 want exit  then
    h-hide @ if  drop $36 0 want exit  then
    h-at-door @ if
        dup h-room swap room-exit-door h-route-door !
        rest h-left f!
    then  drop
    h-action @ 2 - 2 u< if
        h-wait @ 150 + h-wanted !  anim-group h-t2 !  $2F 0 want
    else
        avoid-clear  h-waiting @ 0= if  $2C  else  0  then  0 want
    then ;

\ ---- Hewie_Arrive: he is in the room being played after all (Fiona came into his room) ----
: arrive ( -- arrived? )
    h-room played-room <> if  false exit  then
    h-action @ dup 0= over $2C $3A within or over $52 = or swap $77 = or 0= if  false exit  then
    0 h-alert !  -1 h-yelp-anim !  false                                ( placed? )
    h-action @ case
        $2F of  -150 h-wait +!  h-wait @ 0> if
                    h-t2 @ 1 = if  h-cond 1 = if  7  else  1  then  play  then
                    h-t2 @ 2 = if  h-cond 1 = if  7  else  2  then  play  then
                else  0 h-wait !  then  endof
        $77 of  h-yelp-anim @ play  endof
    endcase
    \ where he was, if that is open floor; else through the door he came by, facing anywhere
    h-tri dup 0< 0= if
        dup nav-flags him c-mask and 0= if  drop h-pos vec@ him c-place!  drop true  else  drop  then
    else  drop  then
    0= if
        h-action @ dup $2F <> swap $77 <> and if  arrive-anim  then
        h-door @ 0< if  0  else  h-door @  then  2 exit-spot dup 0< if
            drop fdrop fdrop fdrop  her c-pos vec@ him c-place!
        else  drop him c-place!  then
        rnd01 2e f* 1e f- pi f* h-yaw!
    then
    h-cond 2 = if  $52 0 want
    else h-action @ $77 = if  $76 0 want
    else h-action @ $2F = h-t1 @ 0> and if  h-wait @ h-wanted !  3 0 want
    else  h-hit-t @ if  $24  else  0  then  0 want
    then then then
    arrived  true ;
