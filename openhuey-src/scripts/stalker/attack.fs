\ stalker/attack.fs - a stalker's attacks (src/game/pursuer.c): a combo of his moves (his combo
\ table: up to 4 moves, -1 ending it) from his move table (stalker/tables.fs `/move`: animation,
\ the bones that strike, their reach, the blow's kind, the fright it puts on her, a stumble's
\ chance, turning with it, his cry) - Pursuer_AttackNextStep, Pursuer_StateAttackStep (a blow:
\ at its key, whoever its bones reach in front of him is sent a `hit`), Pursuer_StateAttackActive
\ (a hold: whoever is within its reach at its key is held) - and the threat of it on her
\ (Pursuer_Threat).
IN: stalker.attack
USING: engine game-state actors messages common facts flag-names paths vectors stalker.state stalker.senses stalker.moving stalker.tables stalker.head stalker.names stalker.sounds ;


\ ---- the combo's move and its fields ----
: move-at ( k -- n )   \ (the combo's k-th move number, -1 none)
    dup 4 < 0= if  drop -1 exit  then  combo @ 4 * + cells combos + @ ;
: move-no ( -- n )  combo-step @ move-at ;
: mv ( -- addr )  move-no move-of ;
: mv-anim ( -- a )  mv m.anim @ ;          : mv-bone ( -- b )  mv m.bone @ ;
: mv-bone2 ( -- b )  mv m.bone2 @ ;        : mv-reach ( F: -- r )  mv m.reach f@ ;
: mv-kind ( -- k )  mv m.kind @ ;          : mv-threat ( F: -- t )  mv m.threat f@ ;
: mv-stumble ( F: -- c )  mv m.stumble f@ ;
: mv-turns? ( -- flag )  mv m.turns @ 0<> ;   : mv-cry ( -- id )  mv m.cry @ ;
: mv-unshaken? ( -- flag )  mv m.unshaken @ 0<> ;   : mv-damage ( -- n )  mv m.damage @ ;
: mv-hold? ( -- flag )  mv-kind hold-request = ;
$8000 constant stumbles   \ (a blow's how: she stumbles)
: stumble-roll ( F: chance -- how )  rnd 100e f* f>= if  stumbles  else  0  then ;   \ (by its chance)

\ ---- the threat on her (Pursuer_Threat): the fright, less with each 10 further off (x 0.75),
\ none beyond 40 ----
: threat ( F: amount -- )
    d-fiona f@ fdup f0< if  fdrop fdrop exit  then                   ( F: a d )
    fdup 10e f< if  fdrop  else
    fswap 0.75e f* fswap  fdup 20e f< if  fdrop  else
    fswap 0.75e f* fswap  fdup 30e f< if  fdrop  else
    fswap 0.75e f* fswap  40e f< 0= if  fdrop exit  then  0.75e f*
    then then then
    f>cell panic-id send threat ;   \ (Threat_Raise)

\ ---- how much nearer he'll be in 5 frames (Pursuer_GroundGained: his strides, less hers
\ while she isn't facing him - running from him; his reach for Hewie, Debilitas 16; her panic
\ calming down, 10 more) ----
: her-back-to-me? ( -- flag )   \ (Eye_ActorSees from her: not within 150 and 90 degrees of her heading)
    fiona here? 0= if  false exit  then
    fiona body-at my-at vec-dist 150e f> if  true exit  then
    fiona body-at my-at vec-heading fiona body-yaw f- angle-wrap fabs pi f2/ f> ;
: her-stride ( F: -- d )
    0 cast-model dup 0< if  drop 0e exit  then  root-delta {: F: turn x y z :}  z ;
: ground-gained ( F: -- d )
    stride 5e f*  her-back-to-me? if  her-stride 5e f* f-  then
    her-stage @ calming-down = if  10e  else  reach-k f@  then  f+ ;

\ ---- whom a blow at a point reaches (Npc_WhoReachable, Progress_CharNear): their body within
\ its height and radius, widened by the reach, and in front of him (90 degrees) ----
create blow-at /vec allot
: reaches-whom? ( v who -- flag ) ( F: margin -- )
    {: v who F: m :}  who here? 0= if  false exit  then
    v v.y sf@  who body-at v.y sf@ m f-  f> 0= if  false exit  then                                \ (above their feet)
    v v.y sf@  who body-at v.y sf@  who body-dims fswap fdrop f+ m f+  f< 0= if  false exit  then   \ (below their top)
    v who body-at vec-dist-xz  who body-dims fdrop m f+  f< 0= if  false exit  then               \ (within their radius)
    who body-at heading-to yaw f- angle-wrap fabs pi f2/ f< ;                                     \ (in front of him)
: reaches? ( v -- flag ) ( F: margin -- )  fiona reaches-whom? ;
\ ... and Hewie's (Npc_WhoReachable takes him too: struck, he is sent the blow - its damage)
: reaches-dog? ( v -- flag ) ( F: margin -- )  hewie reaches-whom? ;
: strike-dog ( -- )   \ (the move's bones at him: kind, damage, a stumble by its chance)
    dog-sent @ if  exit  then
    my-model @ mv-bone bone-pos blow-at vec!  blow-at mv-reach reaches-dog?
    mv-bone2 0< 0= if  my-model @ mv-bone2 bone-pos blow-at vec!  blow-at mv-reach reaches-dog? or  then
    0= if  exit  then
    mv-kind mv-damage  mv-stumble stumble-roll  hewie send blow  -1 dog-sent !  -1 struck-dog ! ;
\ a wall between him and the blow (Npc_CanWalkStraight): it lands on nothing
: walled? ( v -- flag )
    {: v :}  v v-tri dup 0< if  drop false exit  then  {: t :}
    self body-tri my-at v self body-mask v-walk t <> ;

\ ---- the combo: at her, or at Hewie while he is after him (stalker/bitten.fs) ----
: quarry ( -- id )  at-dog @ if  hewie  else  fiona  then ;
: d-quarry ( F: -- d )  at-dog @ if  d-hewie  else  d-fiona  then  f@ ;
defer to-stand ( -- )
: combo-over ( -- )   \ (its move no longer his act: none left to run - Pursuer_BehaviourEnded's state reset)
    -1 combo !  0 combo-step !  -1 step-done !  0 unshaken !  ['] noop act-xt ! ;
\ after it: held off a while (Pursuer_StateAttackStep: struck her, his rests' first - Debilitas 90;
\ else his rests' second at least - 30)
: held-off-after ( -- )  took @ if  rest-struck  else  hold-off @ rest-missed max  then  hold-off ! ;
defer next-move ( -- )
\ his blow's swing (swung): not yet, swung, after it
0 constant unswung   1 constant swinging   2 constant swung-past
\ Pursuer_StateAttackStep: the move to its end, turning to her with it; at its key the blow
: strike ( -- )
    world-held state-flag?  stalkers-blind state-flag? or if  exit  then
    my-model @ mv-bone bone-pos blow-at vec!
    anim @ 0<>  blow-at walled? and if   \ (into a wall: his stand, a thud)
        still-anim play-now  thud-sound 7 sound  combo-over exit
    then
    swung @ swinging <> if  swish-sound 7 sound  swinging swung !  then
    strike-dog
    sent @ if  exit  then
    blow-at mv-reach reaches?
    mv-bone2 0< 0= if  my-model @ mv-bone2 bone-pos blow-at vec!  blow-at mv-reach reaches? or  then
    took @ 1 and 0= and if
        mv-kind  mv-stumble stumble-roll
        mv-threat f>cell  fiona send hit  -1 sent !  mv-cry cry !
    then ;
: key? ( -- flag )  my-model @ 0 0 0 motion-events 2 and 0<> ;   \ (his move's key: the blow)
: striking ( -- )
    ended? if  held-off-after  combo-over exit  then
    mv-turns? if  quarry body-at turn-rate turn-to fdrop  then
    freeze-t @ 0= if  root-move  then
    key? if  strike exit  then
    swung @ swinging <> if  exit  then
    took @ 1 and 0=  at-dog @ 0= and if  mv-threat 2e f/ threat  then   \ (missed her: half its fright)
    combo-step @ 1+ move-at -1 <>  d-quarry 50e f< and  d-quarry f0< 0= and if
        1 combo-step +!  next-move exit
    then
    swung-past swung !  0 unshaken ! ;
\ Pursuer_StateAttackActive: a hold - at its key whoever is within its reach of him is held
\ (the grip: the move's bone number); at its end held off by his rests' third (Debilitas 30) if it
\ held her, and the next move
: holding ( -- )
    key?  sent @ 0= and  world-held state-flag? 0= and if
        fiona here? if  d-fiona f@ mv-reach f<  d-fiona f@ f0< 0= and if
            hold-request  mv-bone  mv-threat f>cell  fiona send hit  -1 sent !
        then  then
    then
    ended? if
        took @ if  rest-held hold-off !  then
        1 combo-step +!  next-move exit
    then
    mv-turns? if  fiona body-at turn-rate turn-to fdrop  then
    root-move ;
\ Pursuer_AttackNextStep: the combo's next move while she is in his room and near (50, or the
\ ground he gains; a hold at any distance); else the combo is over
:noname ( -- )
    combo-step @ 4 < move-no -1 <> and  quarry here? and if
        mv-hold? if  true  else  d-quarry 50e ground-gained fmax f<=  d-quarry f0< 0= and  then
        if
            0 sent !  0 dog-sent !  unswung swung !  mv-anim play-now  mv-unshaken? unshaken !
            mv-hold? if  ['] holding  else  ['] striking  then  act-xt !  exit
        then
    then
    combo-over ; is next-move
: combo-start ( n -- )  0 struck-dog !  combo !  0 combo-step !  0 took !  0 step-done !  combo-act chase-act !  combo-act action-look!  next-move ;

\ ---- a blow's mark (HitEffect_Spawn: `spray`) on someone, that much of their height up them;
\ his animation's effect key ($20: where his blows land) ----
: mark-on ( who heavy -- ) ( F: height -- )
    {: who heavy F: h :}
    who body-tri heavy  who body-at vec@ {: F: x y z :}  x  y who body-dims fswap fdrop h f* f+  z  spray ;
: effect-key? ( -- flag )  my-model @ 0 -1 1 motion-events $20 and 0<> ;

\ ---- his blow struck home (`hit-taken`, Pursuer_CryHeard): held off his rests' first, his cry, held still 5 frames; his blow's mark on the
\ one struck, as they cry out (his own: Riccardo_CryHit, Daniella_BlowEffect) ----
: struck-home ( kind -- )
    drop  1 took !  rest-struck hold-off !
    own-mark @ ?dup if  sender swap execute  then
    cry @ dup 0< 0= if  7 sound  else  drop  then  -1 cry !
    5 freeze-t !  model act.mflags dup l@ motion-frozen or swap l! ;
: thaw ( -- )   \ (each frame: the hold counted down)
    freeze-t @ 0= if  exit  then
    -1 freeze-t +!  freeze-t @ 0= if  unfrozen  then ;

\ ---- his chance roll (Pursuer_ChanceRoll): his kit's table (+0x1740) - its row {chance, rise}: Hewie
\ down, his blow struck Hewie in this attack, struck by someone (struck?), else. Won: his temper with
\ Hewie gone (and what built up); lost, what built up rises - the next likelier ----
: roll-row ( struck? -- i )
    dog-cond @ down-cond = if  drop 0 exit  then  struck-dog @ if  drop 1 exit  then  if  3  else  2  then ;
: chance-roll ( struck? -- won? )
    roll-row 2* floats rolls + {: row :}
    rnd 100e f*  row f@ roll-build f@ f+  f<= if  0 temper !  0e roll-build f!  true exit  then
    row float+ f@ roll-build f@ f+ roll-build f!  false ;
