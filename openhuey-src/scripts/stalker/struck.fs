\ stalker/struck.fs - a stalker struck (src/game/pursuer.c Pursuer_Hit, Pursuer_StateFlinch,
\ Pursuer_StateKnockedDown / StateHurt, Pursuer_FrameUpdate; Debilitas_GivesUp): a `blow` (her
\ shove or kick) takes his health and adds to the damage he's had; he flinches by the side and
\ height it came from - in a move that leaves him unshaken a shove only gets a grunt - or, out
\ of health, he falls and stays down a while (his setup's frames: Debilitas 1800, hard 1350); a
\ stumble hurts him a while (900 frames: his hurt walk). At his flinch's key he may strike back (his
\ tables for in front, behind). After it, enough damage (Debilitas 20, kind 6 80) and he gives up:
\ his cry, held off - he goes out of the room for a while.
IN: stalker.struck
USING: engine game-state actors messages common facts flag-names paths vectors stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.doorway stalker.tables stalker.attack stalker.chase stalker.names stalker.anims stalker.sounds ;

\ ---- in a doorway (PursuerGroup_Find 0x20: in an exit's door area): the short flinches; not
\ downed there ----
: at-door? ( -- flag )
    8 0 do  i door-here? if  i 0 my-at vec@ door-in-area? if  true unloop exit  then  then  loop  false ;

\ ---- each step he finishes (Pursuer_FrameUpdate) and after a blow's reaction: gives up, or on ----
: leave-by ( exit -- )   \ (out by the door he made for: stalker/doorway.fs)
    out-by 0= if  stand  then ;
: way-home ( -- found? )   \ (Debilitas_PickDestination: in one of his rooms, there; else the nearer of
    \ them he has a route to - its first door his way out, making-for; none: -1)
    own-a @ 0< if  false exit  then
    own-room? if  -1 making-for !  true exit  then
    head-home  route-over? 0= ;
: give-up? ( -- flag )   \ (Debilitas_GivesUp: his damage to give up at, not held off already, not in a
    gives-up @ 0= if  false exit  then   \ doorway, and a way home - else on after her; the Pursuer's: never)
    hits @ give-up-hits @ <  my-mode @ held-off = or  at-door? or if  false exit  then
    way-home ;
: give-up ( -- )   \ (out by the first door of his way home; in one of his rooms, to its spot)
    0 hits !  give-up-cry 7 sound  held-off mode!  no-search search-phase !
    making-for @ 0< if  in-play  else  making-for @ leave-by  then ;
:noname ( -- flag )  give-up? dup if  give-up  then ; is gave-up?
: recovered ( -- )
    0 reeling !
    hp @ 0> 0= if  hp-max @ hp !  then
    give-up? if  give-up exit  then
    growl-after @ my-mode @ held-off <> and if  stumble-growl 7 sound  then  0 growl-after !
    my-mode @ held-off = if   \ (held off already: on home)
        way-home 0= if  stand exit  then
        making-for @ 0< if  in-play  else  making-for @ leave-by  then  exit
    then
    sees-fiona @ if  chase-begin  else  my-mode @ after-her = if  chase-start  else  search-again  then  then ;

\ ---- flinching (Pursuer_StateFlinch: by the side and height - in front, behind, a low one; in
\ a doorway the short ones); her blow: at its key he may strike back ----
0 constant from-front   1 constant from-behind   2 constant low-blow   \ (a blow's direction: bits)
: flinching ( -- )
    root-move
    key?  by-her @ and  my-mode @ held-off <> and  her-plight @ unreachable-plight <> and if
        0 by-her !  0 reeling !
        ['] chase-run step!  0 chase-t !  -1 combo !
        fiona body-at heading-to yaw f- angle-wrap fabs pi f2/ f< if  strike-back-front  else  strike-back-behind  then  choose  exit
    then
    ended? if  recovered  then ;
: flinch ( dir -- )
    at-door? if  low-blow and if  flinch-door-low-anim  else  flinch-door-anim  then
    else  case
            from-front of  flinch-front-anim  endof
            from-behind of  flinch-behind-anim  endof
            low-blow of  flinch-low-anim  endof
            >r flinch-low-behind-anim r>
        endcase  then
    play-now  -1 reeling !  ['] flinching step! ;

\ ---- powdered (a powder trodden on - Pursuer_StateSpecialAnim): his hands to his face for the
\ powder's loops (4 / 6 / 8, a loop a key $400), its cloud dripping from him; struck by one
\ again meanwhile, its loops and a new cloud; done, on as after a blow ----
variable powder-loops  variable powder-how   \ (its loops as it struck)
defer powder-cloud ( loops -- )   ' drop is powder-cloud   \ (the cloud: stalker/powder.fs)
$400 constant loop-key   \ (the motion's key flag: at its last frame)
$7FFF constant loops-mask
: powdering ( -- )
    root-move
    model act.mflags l@ loop-key and 0= if  exit  then
    -1 powder-loops +!  powder-loops @ 0> if  exit  then
    recovered ;
: powdered? ( -- flag )  step @ ['] powdering = ;
: powdered ( loops -- )
    dup powder-loops !  powder-cloud
    powdered-anim play-now  -1 reeling !  ['] powdering step! ;

\ ---- knocked down (Pursuer_StateKnockedDown, StateHurt; out of health, down his setup's frames:
\ Pursuer_NoGiveUp): forward with floor ahead, else back;
\ Hewie letting go of his leg, on his front; lying while he's down for good, a blow then a
\ twitch (the lying's one before); up (the one after) ----
enum: down-stage#  falling lying twitching getting-up ;
create ahead-at /vec allot
: floor-ahead? ( -- flag )
    yaw self body-dims fswap fdrop ahead-at my-at vec-ahead
    self body-tri my-at ahead-at self body-mask v-walk 0< 0= ;
: downed ( -- )
    root-move
    down-stage @ case
        falling of  ended? if  lie-anim @ play-now  lying down-stage !  then  endof
        lying of
            twitch @ if  0 twitch !  lie-anim @ 1- play-now  twitching down-stage !  exit  then
            down-t @ if  -1 down-t +!  else  lie-anim @ 1+ play-now  getting-up down-stage !  then
        endof
        twitching of  ended? if  lie-anim @ play-now  lying down-stage !  then  endof
        getting-up of  ended? if  well-cond cond !  still-anim play-now  recovered  then  endof
    endcase ;
: fall ( anim lie -- )
    lie-anim !  play-now  falling down-stage !
    hp @ 0> 0= if
        down-frames @ down-t !  down-cond cond !  -1 beaten !  1 knockouts +!  down-for-good
    else  0 down-t !  then
    let-her-go  0 sees-fiona !  0 sees-hewie !  -1 reeling !  ['] downed step! ;
: knock-down ( -- )  floor-ahead? if  fall-front-anim lying-front-anim  else  fall-back-anim lying-back-anim  then  fall ;
: leg-down ( -- )  leg-fall-anim lying-front-anim fall ;

\ ---- a stumble by how worn he is (Fiona_ChaseRoll: his health over its most - over 0.8 none,
\ 0.6 2 in 100, 0.4 5, else 10) ----
: worn-roll ( -- flag )
    hp @ s>f hp-max @ 1 max s>f f/
    fdup 0.8e f> if  fdrop 0  else  fdup 0.6e f> if  fdrop 2  else  0.4e f> if  5  else  10  then  then  then
    rnd 100e f* f>s swap < ;

\ ---- Hewie's tackles (Pursuer_AddBiteDamage): his temper up by where it caught him - its table's
\ first four: in front, in front low, behind, behind low ----
create tackle-cells  0 , 2 , 1 , 3 ,     \ (by dir)
: tackled ( kind dir -- )
    striker @ hewie <>  rot dup hold-blow = swap bite-blow = or  or if  drop exit  then
    cells tackle-cells + @ cells temper-table + @  temper @ + 1000 min temper ! ;

\ ---- Pursuer_Hit ----
: blow-dir ( kind -- dir )   \ (where it came from, how low)
    striker @ dup -2 = if  drop strike-pt heading-to
    else dup 0< 0= if  dup body? if  body-at heading-to  else  drop yaw  then  else  drop yaw  then then
    yaw f- angle-wrap fabs pi f2/ f< if  from-front  else  from-behind  then
    swap dup kick-blow = swap hard-blow = or if  low-blow or  then ;
: on-ladder? ( -- flag )  anim @ $FF00 and on-top-anim = ;
: struck ( kind damage how -- )
    {: kind damage how :}
    kind powder-blow = if  how loops-mask and powder-how !  then
    cond @ down-cond = if  kind door-blow <> if  -1 twitch !  then  exit  then   \ (down: a twitch)
    hp @ damage abs - 0 max hp !  hits @ damage + 999 min hits !     \ (his health, the damage he's had)
    on-ladder? if  hp @ 1 max hp !  exit  then                     \ (on a ladder: hurt, but no fall)
    kind killing-blow = if  0 hp !  then
    kind blow-dir {: dir :}
    kind dir tackled
    unshaken @  kind shove-blow = and  hp @ 0> and if  grunt-sound 7 sound exit  then
    striker @ fiona = if  -1 by-her !  then
    kind door-blow = if  let-her-go combo-over 0 hold-off !  how door-struck exit  then   \ (a door slammed on him: how is its exit)
    hp @ 0> 0= if  at-door? if  1 hp !  else  knock-down exit  then  then
    kind powder-blow = powdered? and if  how loops-mask and dup powder-loops !  powder-cloud exit  then   \ (powdered again)
    reeling @ if  exit  then                                       \ (already reeling: the damage only)
    kind killing-blow = if   \ (a killing blow kept from felling him - in a doorway: the low flinch, nothing else)
        let-her-go  combo-over  0 hold-off !  low-blow flinch exit
    then
    how stumbles and  how 1 and if  worn-roll or  then  if
        hurt-cond cond !  900 hurt-t !  -1 growl-after !
    then
    let-her-go  combo-over  0 hold-off !
    kind powder-blow = if  powder-how @ powdered exit  then
    kind dup shove-blow = over kick-blow = or swap hard-blow = or if  dir flinch  then ;
\ each frame: his hurt worn off
: hurt-down ( -- )
    hurt-t @ 0= if  exit  then  -1 hurt-t +!  hurt-t @ 0= cond @ hurt-cond = and if  well-cond cond !  then ;
' recovered is door-recovered   ' recovered is door-done
