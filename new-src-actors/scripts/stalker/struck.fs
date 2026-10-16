\ stalker/struck.fs - a stalker struck (src/game/pursuer.c Pursuer_Hit, Pursuer_StateFlinch,
\ Pursuer_StateKnockedDown / StateHurt, Pursuer_FrameUpdate; Debilitas_GivesUp): a `blow` (her
\ shove or kick) takes his health and adds to the damage he's had; he flinches by the side and
\ height it came from - in a move that leaves him unshaken a shove only gets a grunt - or, out
\ of health, he falls and stays down a while (1800 frames; hard 1350); a stumble hurts him a while
\ (900 frames: his hurt walk). At his flinch's key he may strike back (his tables 8 in front, 9
\ behind). After it, 20 damage or more and he gives up: his cry, held off (mode 4) - he goes
\ out of the room for a while.
IN: stalker.struck
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.doorway stalker.tables stalker.attack stalker.chase ;

\ ---- in a doorway (PursuerGroup_Find 0x20: in an exit's door area): the short flinches; not
\ downed there ----
: at-door? ( -- flag )
    8 0 do  i door-here? if  i 0 my-at vec@ door-in-area? if  true unloop exit  then  then  loop  false ;

\ ---- after a blow's reaction (Pursuer_FrameUpdate's end of step): gives up, or on ----
: way-out ( -- exit | -1 )   \ (an exit he can go by: a usable door with a way to it)
    8 0 do
        my-room i room-exit-door dup 0< 0= if
            door-kind dup 4 = swap 5 = or if
                i 0 exit-spot dup 0< 0= if  goal-at vec!  goal-at plan 0> if  i unloop exit  then  else  drop fdrop fdrop fdrop  then
            then
        else  drop  then
    loop  -1 ;
: leave-by ( exit -- )   \ (out by the door he made for: stalker/doorway.fs)
    out-by 0= if  stand  then ;
: give-up? ( -- flag )   \ (Debilitas_GivesUp: 20 damage, not held off already, not in a doorway, a way out;
    gives-up @ 0= if  false exit  then   \ the Pursuer's: never)
    hits @ 20 <  my-mode @ 4 = or  at-door? or if  false exit  then
    way-out dup 0< if  drop false exit  then  making-for !  true ;
: give-up ( -- )
    0 hits !  $20 7 sound  4 mode!  0 search-phase !  making-for @ leave-by ;
: recovered ( -- )
    0 reeling !
    hp @ 0> 0= if  hp-max @ hp !  then
    give-up? if  give-up exit  then
    growl-after @ my-mode @ 4 <> and if  $1F 7 sound  then  0 growl-after !
    my-mode @ 4 = if   \ (held off already: on out)
        way-out dup 0< if  drop stand exit  then  leave-by exit
    then
    sees-fiona @ if  chase-begin  else  my-mode @ 0= if  chase-start  else  search-again  then  then ;

\ ---- flinching (Pursuer_StateFlinch: by the side and height - 0 in front, 1 behind, +2 a low
\ one; in a doorway the short ones); her blow: at its key he may strike back ----
: flinching ( -- )
    root-move
    my-model @ 0 0 0 motion-events 2 and  by-her @ and  my-mode @ 4 <> and  her-plight @ 3 <> and if
        0 by-her !  0 reeling !
        ['] chase-run step!  0 chase-t !  -1 combo !
        fiona body-at heading-to yaw f- angle-wrap fabs pi f2/ f< if  8  else  9  then  choose  exit
    then
    ended? if  recovered  then ;
: flinch ( dir -- )
    at-door? if  2 and if  $1005  else  $1002  then
    else  case  0 of $1001 endof  1 of $1000 endof  2 of $1004 endof  >r $1003 r>  endcase  then
    play-now  -1 reeling !  ['] flinching step! ;

\ ---- powdered (a powder trodden on - Pursuer_Hit's kind 7 - action $24, Pursuer_StateSpecialAnim):
\ his hands to his face ($1006) for the powder's loops (4 / 6 / 8, a loop a key $400), its cloud
\ dripping from him; struck by one again meanwhile, its loops and a new cloud; done, on as after
\ a blow ----
$1006 constant powder-anim
variable powder-loops  variable powder-how   \ (its loops as it struck)
defer powder-cloud ( loops -- )   ' drop is powder-cloud   \ (the cloud: stalker/powder.fs)
: powdering ( -- )
    root-move
    my-model @ actor act.mflags l@ $400 and 0= if  exit  then   \ (the motion's key flag $400: at its last frame)
    -1 powder-loops +!  powder-loops @ 0> if  exit  then
    recovered ;
: powdered? ( -- flag )  step @ ['] powdering = ;
: powdered ( loops -- )
    dup powder-loops !  powder-cloud
    powder-anim play-now  -1 reeling !  ['] powdering step! ;

\ ---- knocked down (Pursuer_StateKnockedDown, StateHurt): forward ($1804) with floor ahead,
\ else back ($1800); Hewie letting go of his leg, on his front ($1709); lying ($1806 / $1802) while he's down for good, a blow then a twitch (one
\ before); up ($1807 / $1803) ----
create ahead-at 12 allot
: floor-ahead? ( -- flag )
    yaw self body-dims fswap fdrop ahead-at my-at vec-ahead
    self body-tri my-at ahead-at self body-mask v-walk 0< 0= ;
: downed ( -- )
    root-move
    down-stage @ case
        0 of  ended? if  lie-anim @ play-now  1 down-stage !  then  endof
        1 of
            twitch @ if  0 twitch !  lie-anim @ 1- play-now  2 down-stage !  exit  then
            down-t @ if  -1 down-t +!  else  lie-anim @ 1+ play-now  3 down-stage !  then
        endof
        2 of  ended? if  lie-anim @ play-now  1 down-stage !  then  endof
        3 of  ended? if  0 cond !  stand-anim play-now  recovered  then  endof
    endcase ;
$1709 constant leg-fall      \ (Hewie let go of his leg: down on his front, as from $1804)
$1806 constant lying-front
: fall ( anim lie -- )
    lie-anim !  play-now  0 down-stage !
    hp @ 0> 0= if
        hard-mode? if  1350  else  1800  then  down-t !  2 cond !  -1 beaten !  1 knockouts +!  down-for-good
    else  0 down-t !  then
    let-her-go  0 sees-fiona !  0 sees-hewie !  -1 reeling !  ['] downed step! ;
: knock-down ( -- )  floor-ahead? if  $1804  else  $1800  then  dup 2 +  fall ;
: leg-down ( -- )  leg-fall lying-front fall ;

\ ---- a stumble by how worn he is (Fiona_ChaseRoll: his health over its most - over 0.8 none,
\ 0.6 2 in 100, 0.4 5, else 10) ----
: worn-roll ( -- flag )
    hp @ s>f hp-max @ 1 max s>f f/
    fdup 0.8e f> if  fdrop 0  else  fdup 0.6e f> if  fdrop 2  else  0.4e f> if  5  else  10  then  then  then
    rnd 100e f* f>s swap < ;

\ ---- Hewie's tackles (Pursuer_AddBiteDamage): his temper up by where it caught him - its table's
\ first four: in front, in front low, behind, behind low ----
create tackle-cells  0 , 2 , 1 , 3 ,     \ (by dir: bit 0 behind, bit 1 low)
: tackled ( kind how dir -- kind how dir )
    striker @ hewie <>  3 pick dup $A = swap $B = or  or if  exit  then
    dup cells tackle-cells + @ cells temper-table + @  temper @ + 1000 min temper ! ;

\ ---- Pursuer_Hit ----
: struck ( kind damage how -- )
    2 pick 7 = if  dup $7FFF and powder-how !  then
    cond @ 2 = if  drop drop  5 <> if  -1 twitch !  then  exit  then   \ (down: a twitch)
    over hp @ swap abs - 0 max hp !  over hits @ + 999 min hits !     \ (his health, the damage he's had)
    anim @ $FF00 and $700 = if  drop 2drop  hp @ 1 max hp ! exit  then   \ (on a ladder: hurt, but no fall)
    >r drop r>                                                    ( kind how )
    over 3 = if  0 hp !  then
    striker @ dup -2 = if  drop strike-pt heading-to
    else dup 0< 0= if  dup body? if  body-at heading-to  else  drop yaw  then  else  drop yaw  then then
    yaw f- angle-wrap fabs pi f2/ f< if  0  else  1  then
    2 pick dup 2 = swap 4 = or if  2 or  then                      ( kind how dir )
    tackled
    unshaken @  3 pick 1 = and  hp @ 0> and if  drop 2drop  $1C 7 sound exit  then
    striker @ fiona = if  -1 by-her !  then
    2 pick 5 = if  drop nip  let-her-go combo-over 0 hold-off !  door-struck exit  then   \ (a door slammed on him: how is its exit)
    hp @ 0> 0= if  at-door? if  1 hp !  else  drop 2drop knock-down exit  then  then
    2 pick 7 = powdered? and if  drop nip  $7FFF and dup powder-loops !  powder-cloud exit  then   \ (powdered again)
    reeling @ if  drop 2drop exit  then                            \ (already reeling: the damage only)
    2 pick 3 = if   \ (a killing blow kept from felling him - in a doorway: the low flinch, nothing else)
        drop 2drop  let-her-go  combo-over  0 hold-off !  2 flinch exit
    then
    swap dup $8000 and  swap 1 and if  worn-roll or  then  if
        1 cond !  900 hurt-t !  -1 growl-after !
    then                                                          ( kind dir )
    let-her-go  combo-over  0 hold-off !
    over 7 = if  2drop  powder-how @ powdered exit  then
    over dup 1 = over 2 = or swap 4 = or if  nip flinch  else  2drop  then ;
\ each frame: his hurt worn off
: hurt-down ( -- )
    hurt-t @ 0= if  exit  then  -1 hurt-t +!  hurt-t @ 0= cond @ 1 = and if  0 cond !  then ;
' recovered is door-recovered   ' recovered is door-done
