\ stalker/bitten.fs - a stalker and Hewie (P3c; src/game/pursuer.c Pursuer_EventConcerns kind 9,
\ Pursuer_GrabOrder / Pursuer_GrabHewieSpot, Pursuer_StateHewieBites / StateTurnAnim /
\ StateHewieHolds / StateBackOnFeet, Pursuer_Hit's kinds 6 and $B, Pursuer_BehaviourHewie).
\ Hewie's attack is asked (`dog-attack`): in her room, not already bitten or down or leading
\ her, he takes it - the spot by him Hewie leaps to (`dog-grip`) - or refuses. Bitten he plays
\ his reaction for where (by the arm $1700, the other $1703, head on $1706, the leg $1708),
\ turned with it; each time round it his temper rises, and near its end he may throw Hewie off
\ (the likelier the longer Hewie holds on and the hotter his temper): his throw (+2) strikes
\ Hewie. Hewie letting go (`shaken-off`), he recovers (+1); down if out of health (the leg: down
\ at once). Hewie's bites (`bite`) take his health; a hard one makes him stumble. Growled and
\ barked at (`snap`) his temper rises too; past what he stands he goes for Hewie a while.
IN: stalker.bitten
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.travel stalker.tables stalker.attack stalker.chase stalker.struck stalker.doorway ;

: temper+ ( n -- )  temper @ + 1000 min temper ! ;
: temper# ( cell -- n )  cells temper-table + @ ;
: hot? ( -- flag )  temper @ tolerance @ > ;

\ ---- the spot by him for Hewie's attack (Pursuer_GrabHewieSpot; his ActionOffsets: his kit): the
\ offset in his frame by kind, round him from Hewie's side (head on and the leg: from the other
\ side) 10 degrees at a time either way until one is on the floor and Hewie can run straight
\ there ----
create gs-at 12 allot  fvariable gs-h  fvariable gs-r  fvariable gs-x  fvariable gs-z
: try-heading ( F: r -- ) ( -- tri | -1 )   \ the offset turned by r from where he stands
    gs-r f!
    my-at sf@  gs-x f@ gs-r f@ fcos f* f+  gs-z f@ gs-r f@ fsin f* f+
    my-at 4 + sf@
    my-at 8 + sf@  gs-x f@ gs-r f@ fsin f* f-  gs-z f@ gs-r f@ fcos f* f+  gs-at vec!
    gs-at v-tri dup 0< if  exit  then
    hewie body-tri hewie body-at gs-at $29020008 v-walk over <> if  drop -1  then ;
: grip-spot ( kind -- tri | -1 ) ( F: -- yaw | )
    dup 10 16 within 0= if  drop -1 exit  then
    my-at hewie body-at vec-heading  dup 14 >= if  pi f+ angle-wrap  then  gs-h f!
    10 - 2* floats grip-offsets +  dup f@ gs-x f!  1 floats + f@ gs-z f!
    19 0 do
        gs-h f@ i 10 * s>f deg>rad f+ angle-wrap try-heading dup 0< 0= if  gs-r f@  unloop exit  then  drop
        i if  gs-h f@ i 10 * s>f deg>rad f- angle-wrap try-heading dup 0< 0= if  gs-r f@  unloop exit  then  drop  then
    loop  -1 ;

\ ---- bitten (Pursuer_StateHewieBites): his reaction for where, his temper up, turned with it to
\ the grip's heading (in 8 frames' worth, the leg 12); then held (Pursuer_StateHewieHolds) ----
create bite-anims  $1700 , $1700 , $1703 , $1703 , $1706 , $1708 ,
create bite-cells  8 , 9 , 6 , 7 , 10 , 11 ,      \ (temper-table: on the bite)
create hold-cells  14 , 15 , 12 , 13 , 16 , 17 ,  \ (... each hold on)
: k# ( table -- n )  bitten @ 10 - cells + @ ;
defer bitten-over ( -- )
: throw-move ( -- n )  throw-moves  bite-anim @ $1700 = if  drop  else  nip  then ;   \ (his AttackAnimA / B)
: throw-damage ( -- n )  throw-move /move * moves + 5 cells + @ ;
: holding-on ( -- )   \ (Pursuer_StateHewieHolds)
    ended? if
        bite-how @ case
            1 of   \ (throwing him off: the throw, which strikes him)
                anim @ bite-anim @ 2 + = if  bitten-over exit  then
                bite-anim @ 2 + play-now  $B throw-damage 0 hewie send blow
            endof
            2 of   \ (he let go: back on his feet; the leg - down)
                bitten @ 15 = if  0 bitten !  leg-down exit  then
                anim @ bite-anim @ 1+ = if  bitten-over exit  then
                hp @ 0> 0= if  0 bitten !  knock-down exit  then
                bite-anim @ 1+ play-now
            endof
            >r  hold-cells k# temper# temper+  r>
        endcase
        holds f@ 1e f+ holds f!
    else
        model act.mflags l@ $400 and  bite-how @ 0= and  bitten @ 14 < and  stalkers-blind state-flag? 0= and if
            holds f@ throw-k f@ f*  temper @ s>f tolerance @ 1 max s>f f/ 25e f* f+
            rnd 100e f* f>= if  1 bite-how !  hewie send shaken-off  then
        then
    then
    bite-how @ 1 = if  hp @ 0> 0= if  1 hp !  then  then
    root-move ;
: bite-turning ( -- )   \ (Pursuer_StateTurnAnim)
    settled? if  bite-yaw f@ yaw!  1e holds f!  ['] holding-on step!  exit  then
    bite-yaw f@ bite-turn f@ body-turn-toward fdrop ;
: bitten-at ( kind -- )   \ (bite-yaw: the grip's heading)
    bitten !  0 bite-how !
    let-her-go  combo-over  0 search-phase !  -1 reeling !
    bite-anims k# dup bite-anim !  play-now
    bite-cells k# temper# temper+
    bite-yaw f@ yaw f- angle-wrap fabs  bitten @ 15 = if  12e  else  8e  then  f/ bite-turn f!
    ['] bite-turning step! ;
\ Pursuer_StateBackOnFeet: up again, at least 1 health; on as after a blow
:noname ( -- )
    0 bitten !  0 bite-how !  0 reeling !  hp @ 0> 0= if  1 hp !  then
    recovered ; is bitten-over

\ ---- Hewie's attack asked (Pursuer_EventConcerns kind 9, Pursuer_GrabOrder): in her room, not
\ bitten, down, reeling or leading her ----
: grip? ( kind -- tri | -1 ) ( F: -- yaw | )
    away?  bitten @ or  cond @ 2 = or  leading @ or  in-game @ 0= or  hewie here? 0= or if  drop -1 exit  then
    grip-spot ;
: dog-attacked ( kind -- )   \ (gs-at: the spot found)
    dup grip? dup 0< if  2drop  hewie send dog-refused exit  then   ( kind tri ) ( F: yaw )
    fdup bite-yaw f!  swap bitten-at
    gs-at sf@ f>cell  gs-at 4 + sf@ f>cell  gs-at 8 + sf@ f>cell  f>cell
    hewie send dog-grip ;

\ ---- a room's grab of Hewie from behind (Pursuer_GrabHewieBehind: kind $F): the spot found and
\ straight to him from where he stands - Hewie told where he is held (his to-spot); the room's
\ scene takes them both on ----
: grab-hewie-behind ( -- ok? )
    $F grip? dup 0< if  drop false exit  then                          ( tri ) ( F: yaw )
    self body-tri my-at gs-at $28020028 v-walk over <> if  drop fdrop false exit  then
    gs-at sf@ f>cell  gs-at 4 + sf@ f>cell  gs-at 8 + sf@ f>cell  f>cell
    hewie send hold-spot  true ;

\ ---- his bites (Pursuer_Hit kind $B: bitten - the damage, a hard one a stumble, his cry) ----
: bitten-by ( damage hard -- )
    swap  dup hits @ + 999 min hits !  hp @ swap - 0 max hp !
    if  1 cond !  900 hurt-t !  -1 growl-after !  then
    $1D 7 sound ;

\ ---- growled and barked at (Pursuer_Hit kind 6): within 100 of Hewie on foot, not struck,
\ searching or waiting about - his temper up ----
: snapped ( -- )
    d-hewie f@ fdup 100e f< 0=  f0< or if  exit  then
    reeling @  bitten @ or  my-mode @ 0= or  my-mode @ 4 = or if  exit  then
    fiona-hidden state-flag? fiona-half-hidden state-flag? or if  5  else  4  then  temper# temper+ ;

\ ---- after Hewie, his temper past what he stands (Pursuer_GoForHewie, Pursuer_BehaviourHewie):
\ toward him; within reach facing him, an attack from his table for Hewie (a blow, or a hold of
\ him); that over - or his time for it (`dog-limit`) up - his taunt ($1602), his temper back to 0,
\ and on as before. Hewie gone from the room: out after him by the exit to his (heading for her
\ room after it - Pursuer_ThroughDoor); none open to it, on as before ----
0 constant closing-in   1 constant attacking   2 constant taunting
$B constant at-hewie        \ (his tables' situation)
$1602 constant taunt-anim
: dog-over ( -- )  0 temper !  0 dog-t !  0 at-dog !  closing-in dog-phase !  combo-over  recovered ;
: dog-reach? ( -- flag )
    hold-off @ if  false exit  then
    d-hewie f@ fdup 24e f< 0=  fdup f0< or if  fdrop false exit  then
    fdup ground-gained f<  10e f< or 0= if  false exit  then
    hewie body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f< ;
: taunt ( -- )  0 temper !  taunting dog-phase !  taunt-anim play-now  ['] gesturing-at $17 act! ;
: attack-dog ( -- )  attacking dog-phase !  at-hewie choose  chase-act @ 1 = if  -1 step-done !  then ;
: close-in-on-dog ( -- )
    dog-t @ 15 mod 1 = if  hewie body-tri dup 0< if  drop  else  hewie body-at plan drop  then  then
    walk-fast play  walk-stride drop ;
: exit-to ( room -- exit | -1 )
    8 0 do  my-room i room-exit-leads drop over = if  drop i unloop exit  then  loop  drop -1 ;
: follow-dog ( -- )
    hewie dup body? if  body-room exit-to  else  drop -1  then
    dup 0< if  drop dog-over exit  then
    0 temper !  0 at-dog !  closing-in dog-phase !  combo-over  1 mode!
    out-by 0= if  recovered  then ;
: after-dog ( -- )
    dog-t @ 0> if  -1 dog-t +!  then
    hewie here? 0= if  follow-dog exit  then
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    dog-phase @ closing-in <> if
        step-done @ 0= if  exit  then
        dog-phase @ taunting = if  dog-over exit  then
        0 dog-t !  closing-in dog-phase !  ['] watching 1 act!   \ (his attack over: the taunt)
    then
    dog-t @ 0> 0= if  temper @ if  taunt  else  dog-over  then  exit  then
    close-in-on-dog
    dog-reach? if  attack-dog  then ;
: go-for-dog ( -- )
    0 search-phase !  dog-limit @ dog-t !  0 hold-off !  -1 combo !  -1 at-dog !  closing-in dog-phase !
    ['] watching 1 act!  ['] after-dog step! ;
\ his temper past what he stands, free to go (not bitten, reeling, leading her)
: dog-temper ( -- )
    hot? 0= bitten @ or reeling @ or leading @ or  stalkers-blind state-flag? or if  exit  then
    hewie here? 0= if  0 temper !  exit  then
    step @ ['] after-dog = if  exit  then  go-for-dog ;
