\ stalker/attack.fs - a stalker's attacks (src/game/pursuer.c): a combo of his moves (his combo
\ table: up to 4 moves, -1 ending it) from his move table (stalker/tables.fs: animation, the
\ bones that strike, their reach, the blow's kind, the fright it puts on her, a stumble's
\ chance, turning with it, his cry) - Pursuer_AttackNextStep, Pursuer_StateAttackStep (a blow:
\ at its key, whoever its bones reach in front of him is sent a `hit`), Pursuer_StateAttackActive
\ (a hold: whoever is within its reach at its key is held) - and the threat of it on her
\ (Pursuer_Threat).
IN: stalker.attack
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.tables ;

: panic-id ( -- id )  s" panic" actor-named ;

\ ---- the combo's move and its fields ----
: move-at ( k -- n )   \ (the combo's k-th move number, -1 none)
    dup 4 < 0= if  drop -1 exit  then  combo @ 4 * + cells combos + @ ;
: move-no ( -- n )  combo-step @ move-at ;
: mv ( -- addr )  move-no /move * moves + ;
: mv-anim ( -- a )  mv @ ;              : mv-bone ( -- b )  mv cell+ @ ;
: mv-bone2 ( -- b )  mv 2 cells + @ ;   : mv-reach ( F: -- r )  mv 3 cells + f@ ;
: mv-kind ( -- k )  mv 4 cells + @ ;    : mv-threat ( F: -- t )  mv 6 cells + f@ ;
: mv-stumble ( F: -- c )  mv 7 cells + f@ ;
: mv-turns? ( -- flag )  mv 8 cells + @ 0<> ;   : mv-cry ( -- id )  mv 9 cells + @ ;

\ ---- the threat on her (Pursuer_Threat): the fright, less with each 10 further off (x 0.75),
\ none beyond 40 ----
: threat ( F: amount -- )
    d-fiona f@ fdup f0< if  fdrop fdrop exit  then                   ( F: a d )
    fdup 10e f< if  fdrop  else
    fswap 0.75e f* fswap  fdup 20e f< if  fdrop  else
    fswap 0.75e f* fswap  fdup 30e f< if  fdrop  else
    fswap 0.75e f* fswap  40e f< 0= if  fdrop exit  then  0.75e f*
    then then then
    f>cell panic-id send fright ;

\ ---- how much nearer he'll be in 5 frames (Pursuer_GroundGained: his strides; his reach for
\ Hewie, Debilitas 16; in the panic's stage 5, 10 more) ----
: ground-gained ( F: -- d )
    stride 5e f*  her-stage @ 5 = if  10e  else  16e  then  f+ ;

\ ---- whom a blow at a point reaches (Npc_WhoReachable, Progress_CharNear): her body within its
\ height and radius, widened by the reach, and in front of him (90 degrees) ----
create blow-at 12 allot
fvariable wr-m
: reaches? ( v -- flag ) ( F: margin -- )
    wr-m f!  fiona here? 0= if  drop false exit  then
    dup 4 + sf@  fiona body-at 4 + sf@  wr-m f@ f- f> 0= if  drop false exit  then
    dup 4 + sf@  fiona body-at 4 + sf@  fiona body-dims fswap fdrop f+ wr-m f@ f+ f< 0= if  drop false exit  then
    fiona body-at vec-dist-xz  fiona body-dims fdrop wr-m f@ f+ f< 0= if  false exit  then
    fiona body-at heading-to yaw f- angle-wrap fabs pi f2/ f< ;
\ a wall between him and the blow (Npc_CanWalkStraight): it lands on nothing
: walled? ( v -- flag )
    dup v-tri dup 0< if  2drop false exit  then  >r
    self body-tri my-at rot self body-mask v-walk r> <> ;

\ ---- the combo ----
defer to-stand ( -- )
: combo-over ( -- )  -1 combo !  0 combo-step !  -1 step-done ! ;
\ after it: held off a while (struck her: his table's 90; else 30 at least)
: held-off-after ( -- )  took @ if  90  else  hold-off @ 30 max  then  hold-off ! ;
defer next-move ( -- )
\ Pursuer_StateAttackStep: the move to its end, turning to her with it; at its key the blow
: strike ( -- )
    world-held state-flag? if  exit  then
    my-model @ mv-bone bone-pos blow-at vec!
    anim @ 0<>  blow-at walled? and if   \ (into a wall: his stand, a thud)
        0 play-now  $2B 7 sound  combo-over exit
    then
    swung @ 1 <> if  $10 7 sound  1 swung !  then
    sent @ if  exit  then
    blow-at mv-reach reaches?
    mv-bone2 0< 0= if  my-model @ mv-bone2 bone-pos blow-at vec!  blow-at mv-reach reaches? or  then
    took @ 1 and 0= and if
        mv-kind  rnd 100e f* mv-stumble f<= if  $8000  else  0  then
        mv-threat f>cell  fiona send hit  -1 sent !  mv-cry cry !
    then ;
: striking ( -- )
    ended? if  held-off-after  combo-over exit  then
    mv-turns? if  fiona body-at turn-rate turn-to fdrop  then
    freeze-t @ 0= if  root-move  then
    my-model @ 0 0 0 motion-events 2 and if  strike exit  then
    swung @ 1 <> if  exit  then
    took @ 1 and 0= if  mv-threat 2e f/ threat  then          \ (missed her: half its fright)
    combo-step @ 1+ move-at -1 <>  d-fiona f@ 50e f< and  d-fiona f@ f0< 0= and if
        1 combo-step +!  next-move exit
    then
    2 swung ! ;
\ Pursuer_StateAttackActive: a hold - at its key whoever is within its reach of him is held
\ (the grip: the move's bone number); at its end held off 30 if it held her, and the next move
: holding ( -- )
    my-model @ 0 0 0 motion-events 2 and  sent @ 0= and  world-held state-flag? 0= and if
        fiona here? if  d-fiona f@ mv-reach f<  d-fiona f@ f0< 0= and if
            6  mv-bone  mv-threat f>cell  fiona send hit  -1 sent !
        then  then
    then
    ended? if
        took @ if  30 hold-off !  then
        1 combo-step +!  next-move exit
    then
    mv-turns? if  fiona body-at turn-rate turn-to fdrop  then
    root-move ;
\ Pursuer_AttackNextStep: the combo's next move while she is in his room and near (50, or the
\ ground he gains; a hold at any distance); else the combo is over
:noname ( -- )
    combo-step @ 4 < move-no -1 <> and  fiona here? and if
        mv-kind 6 = if  true  else  d-fiona f@ 50e ground-gained fmax f<=  d-fiona f@ f0< 0= and  then
        if
            0 sent !  0 swung !  mv-anim play-now
            mv-kind 6 = if  ['] holding  else  ['] striking  then  act-xt !  exit
        then
    then
    combo-over ; is next-move
: combo-start ( n -- )  combo !  0 combo-step !  0 took !  0 step-done !  $13 chase-act !  next-move ;

\ ---- his blow struck home (`hit-taken`): his cry, held still 5 frames ----
: struck-home ( kind -- )
    drop  1 took !  90 hold-off !
    cry @ dup 0< 0= if  7 sound  else  drop  then  -1 cry !
    5 freeze-t !  model act.mflags dup l@ $40 or swap l! ;
: thaw ( -- )   \ (each frame: the hold counted down)
    freeze-t @ 0= if  exit  then
    -1 freeze-t +!  freeze-t @ 0= if  model act.mflags dup l@ $40 invert and swap l!  then ;
