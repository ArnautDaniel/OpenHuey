\ hewie/fight.fs - Hewie and the stalker (H3; src/game/hewie.c): going for him (actions 0x1F..
\ 0x22: Hewie_StateRunAtPursuer, StateGoesForPursuer / GoForPursuer2 - the attack asked of the
\ stalker, who takes it with a spot to leap to or refuses), the leap (StateLeapGrip, LeapLands),
\ biting and holding on (StateBiteHold: bites by his trust, a shake roll each time round, let
\ go when she calls him off or the stalker is down), letting go (StateClamped) or thrown off
\ (StateHanging, StateJumpArc); harrying him (0x59 / 0x5A / 0x23: keeping near, closing in);
\ struck (Hewie_TakeBlow: by the stalker, by her, by a door) - his yelp, pushed back (0x69,
\ StateMoveAlong), then down or on (Hewie_AfterYelp).
IN: hewie.fight
USING: engine game-state actors messages common facts paths flag-names hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states ;

\ ---- his feelings about the stalker (Hewie_Feeling, Hewie_ChangeFeeling: -10..10) ----
: feel ( -- addr )   \ (by the stalker's kind: Debilitas 2, Daniella 3, Lorenzo 10..12, Riccardo 4)
    st-kind @ case
        3 of  1  endof  4 of  3  endof  10 of  2  endof  11 of  2  endof  12 of  2  endof
        >r 0 r>
    endcase  cells his-feel + ;
\ Hewie_FeelPursuer: his mood normal, a stalker about - the more he dislikes him, the likelier he
\ is upset (mood 2): a 1-in-16 roll under 14 at -10, falling by one a step to 1 at 0, none above
create upset-chance  14 , 10 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 , 1 ,
: feel-pursuer ( -- )
    his-mood @  pursuer active? 0= or  feel @ 0> or if  exit  then
    feel @ 10 + cells upset-chance + @  16 roll > if  2 -1 set-mood  then ;
: feel+ ( n -- )  feel @ + -10 10 clamp feel !  feel-pursuer ;
\ Hewie_Dares: from trust 2, a 1-in-16 roll under his level + 1 + his feeling / 5 (at most 8)
: dares? ( -- flag )
    his-trust @ 2 < if  false exit  then
    feel @ 5 / 8 min  his-trust @ + 1+  16 roll > ;

\ ---- a bite (bite_pursuer): its damage (bite-damage), hard by chance - the more hurt the
\ stalker, the likelier (Hewie_ReactionRoll) ----
: band ( -- n )   \ (his health over its most: above 0.8 0, 0.6 1, 0.4 2, 0.2 3, 0.1 4, else 5)
    st-hp @ s>f st-hp-max @ 1 max s>f f/
    fdup 0.1e f< if  fdrop 5 exit  then  fdup 0.2e f< if  fdrop 4 exit  then
    fdup 0.4e f< if  fdrop 3 exit  then  fdup 0.6e f< if  fdrop 2 exit  then
    0.8e f< if  1  else  0  then ;
: hard-bite? ( kind -- flag )  6 * band + cells hard-bite-chance + @  100 roll > ;
\ (Hewie_BiteEffect: a mark at his mouth, bone $25 - a big one with it while his grip holds)
$25 constant his-mouth
create mouth-at 12 allot
: bite-mark ( -- )
    his-model @ his-mouth bone-pos mouth-at vec!
    his-mode @ 8 = if  his-tri -1 mouth-at vec@ spray  then
    his-tri 0 mouth-at vec@ spray ;
: bite-him ( kind base -- )
    bite-damage  swap hard-bite? pursuer send bite  bite-mark ;

\ ---- which side he comes at him from (Hewie_AttackSide): from behind, facing the stalker's
\ back - 0 (not while he is timid); else a flank (1 near, 2 far: Daniella the other way) ----
: daniella? ( -- flag )  st-kind @ dup 3 = swap 34 37 within or ;
: attack-side ( -- side )
    pursuer body-at heading-to                                     ( F: a )
    hewie-timid state-flag? 0=
    fdup pursuer body-yaw f- angle-wrap fabs pi f2/ f< and
    fdup his-yaw f- angle-wrap fabs pi f2/ f< and if  fdrop 0 exit  then
    pi f+ angle-wrap pursuer body-yaw f- angle-wrap f0< if
        daniella? if  1  else  2  then
    else  daniella? st-kind @ 11 = or if  2  else  1  then  then ;

\ ---- held at the stalker's height (his feet's plane: HumanModel +0x8C0) ----
: at-his-height ( -- )  pursuer body-at 4 + sf@ his-y! ;

\ ---- thrown off (StateHanging, StateJumpArc): hanging on by his teeth to the end of it; then
\ he holds on again if he dares, else flung (10 damage) - along his last root motion, falling;
\ landing ($2212 held on, $2213 flung), then on ----
: st-landed ( -- )  anim-done? if  to-default  then ;
: st-jump-arc ( -- )
    his-fly-x f@ his-fly-z f@ body-move-local
    his-fly-h f@ his-fly-y f@ f+  fdup his-fly-h f!
    pursuer dup 0< if  drop his-at 4 + sf@  else  body-at 4 + sf@  then  f< if
        his-biting @ 0= if  $2212  else  $2213  then  play
        ['] st-landed behave exit
    then
    his-fly-h f@ his-y!  his-fly-y f@ 0.5e f- his-fly-y f!  1 his-no-root ! ;
: st-hanging ( -- )
    root@  rm-x f@ rm-z f@ body-move-local  his-fly-h f@ rm-y f@ f+ fdup his-fly-h f! his-y!  1 his-no-root !
    anim-done? 0= if  exit  then
    pursuer with? 0= if  to-default exit  then
    rnd 0.25e f< if  -1 feel+  then
    dares? if  0 his-biting !  his-grip @ 1 = if  $2204  else  $220D  then
    else  1 his-biting !  his-hp @ 10 - 0 max hp!  his-grip @ 1 = if  $2207  else  $2210  then  then  play-cut
    rm-x f@ his-fly-x f!  rm-y f@ his-fly-y f!  rm-z f@ his-fly-z f!
    ['] st-jump-arc behave ;

\ ---- letting go (StateClamped): the let-go animation at his height until its end, then on
\ (+0xF3688 300 frames: his-pet-time) ----
: st-clamped ( -- )
    slide-root
    anim-done? if  300 his-pet-time !  to-default exit  then
    pursuer with? if  at-his-height  then ;

\ ---- biting and holding on (StateBiteHold): at each time round, a shake roll - his bites left,
\ his chance to hold on (+30 the first time, -10 each time held); failing it he lets go and
\ the stalker shakes. Thrown off meanwhile (the stalker's `shaken-off`): hanging. Let go when
\ she calls him off (her commands 0x27 / 0x2C) or the stalker is down ----
: shake-off ( -- )  2 his-biting !  pursuer send shaken-off ;
: bite-anim ( -- a )
    his-action @ dup $21 = swap $22 = or if  $2218 exit  then
    his-grip @ case  1 of  $2201  endof  2 of  $220A  endof  >r $2215 r>  endcase ;
: let-go-anim ( -- a )
    his-action @ dup $21 = swap $22 = or if  $2219 exit  then
    his-grip @ case  1 of  $2202  endof  2 of  $220B  endof  >r $2216 r>  endcase ;
: bite-again ( -- )   \ (the bite by his action and grip: damage 10 the leg, 2 / 3 a flank, 3 head on)
    bite-anim play-if-not
    his-action @ dup $21 = swap $22 = or if  5 10 bite-him exit  then
    his-grip @ case
        1 of  his-ready @ if  3 3  else  2 2  then  endof
        2 of  his-ready @ if  1 3  else  0 2  then  endof
        >r 4 3 r>
    endcase  bite-him ;
: shake-roll ( -- )
    his-bites @ 1- dup his-bites !
    if  his-hold @  his-biting @ 0= if  1 his-biting !  30 +  then  else  0  then
    100 roll > if  his-hold @ 10 - 0 max his-hold !  else  shake-off  then ;
: st-bite-hold ( -- )
    pursuer with? 0= if  to-default exit  then
    anim-done? if
        his-biting @ 2 < if  shake-roll  then
        his-biting @ case
            2 of  0 his-biting !  let-go-anim play-cut  ['] st-clamped behave  exit  endof
            3 of  his-grip @ dup 1 = swap 2 = or his-action @ dup $1F = swap $20 = or and if
                      his-grip @ 1 = if  $2203  else  $220C  then  play-cut
                      at-his-height  pursuer body-at 4 + sf@ his-fly-h f!  4 his-mode !  $A his-sub !
                      ['] st-hanging behave  exit
                  then
                  0 his-biting !  let-go-anim play-cut  ['] st-clamped behave  exit  endof
        endcase
        bite-again
    else
        his-biting @ 2 < if
            fiona-cmd @ dup $27 = swap $2C = or  st-hp @ 0= or if  shake-off  then
        then
    then
    slide-root  at-his-height ;

\ ---- the leap (StateLeapGrip, StateLeapLands): his bites by his action, grip and trust; his way
\ to the spot and the turn covered in 8 frames (the leg 12), at the stalker's height; landed -
\ the first bite (the leg 5 / 30, a flank 2 or 3 / 5 or 10 charged, head on 4 / 10), his sound,
\ on the spot facing the grip's heading, holding on (chance (his feeling + 10) x 5) ----
: first-bite ( -- )
    his-action @ dup $21 = swap $22 = or if  5 30 bite-him exit  then
    his-grip @ case
        1 of  his-ready @ if  3 10  else  2 5  then  endof
        2 of  his-ready @ if  1 10  else  0 5  then  endof
        >r 4 10 r>
    endcase  bite-him ;
: st-leap-lands ( -- )
    pursuer with? 0= if  to-default exit  then
    anim-done? if
        first-bite  $6B make-sound
        his-to vec@ his-place  his-leap-yaw f@ his-yaw!
        bite-anim play-cut  1 his-biting !
        feel @ 10 + 5 * his-hold !
        ['] st-bite-hold behave
    else
        his-leap-yaw f@ his-leap-turn f@ body-turn-toward fdrop
        his-leap-k f@ ahead@ his-path path-i!  ahead vec@ his-place  1 his-no-root !
    then
    at-his-height ;
: bites-by-trust ( -- n )
    his-action @ case
        $1F of  his-grip @ if  his-ready @ if  bites-flank-charged  else  bites-flank  then
                else  bites-head-on  then  trust-of  endof
        $21 of  bites-leg trust-of  endof
        >r 1 r>
    endcase ;
: leap ( tri F: x y z yaw -- )
    his-leap-yaw f!  his-to vec!  his-to-tri !
    his-to-tri @ his-to plan-to 0= if  to-default exit  then
    bites-by-trust his-bites !
    his-hits @ if  -1 his-hits +!  300 his-hit-t !  then
    $19 his-sub !
    rnd 0.33333e f< if  1 feel+  then
    his-action @ dup $21 = swap $22 = or if  $2217
    else  his-grip @ case
            1 of  st-kind @ $B <> if  his-leap-yaw f@ pi f+ angle-wrap his-leap-yaw f!  then  $2200  endof
            2 of  his-leap-yaw f@ pi f+ angle-wrap his-leap-yaw f!  $2209  endof
            >r $2214 r>
          endcase
    then  play
    his-action @ dup $21 = swap $22 = or if  12e  else  8e  then
    rest fover f/ his-leap-k f!
    his-leap-yaw f@ his-yaw f- angle-wrap fabs fswap f/ his-leap-turn f!
    $68 make-sound
    ['] st-leap-lands behave ;

\ ---- going for him (StateRunAtPursuer, StateGoesForPursuer, GoForPursuer2): at him at a trot
\ or a run; within 23, the side he comes from picks the grip (and the action), the attack is
\ asked of the stalker (`dog-attack`: 15 the leg, a flank 10 / 11 charged or 12 / 13, head on
\ 14), and he waits for the answer ----
: st-await-grip ( -- )  pursuer with? 0= if  to-default exit  then  0 keep-pose ;
: attack-kind ( -- kind )
    his-action @ dup $21 = swap $22 = or if  15 exit  then
    his-grip @ case  1 of  his-ready @ if  11  else  10  then  endof
                    2 of  his-ready @ if  13  else  12  then  endof  >r 14 r>  endcase ;
: goes-for ( -- )
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  to-default exit  then
    his-grip @ 0< if
        attack-side dup 0= if
            his-action @ dup $1F = swap $20 = or if  0 his-grip !  then  drop
        else
            his-action @ dup $1F = swap $21 = or if  $1F  else  $20  then  his-action !
            his-grip !
        then
    then
    attack-kind pursuer send dog-attack
    ['] st-await-grip behave ;
: st-run-at-pursuer ( -- )
    pursuer with? 0= if  to-default exit  then
    pursuer body-at his-at vec-dist 23e f< if   \ (nothing breaks in from here: +0xF356C's top bit)
        4 look-now!  his-cmd @ $80000000 or his-cmd !  1 his-2d !  goes-for exit
    then
    path-done? if  pursuer body-tri pursuer body-at plan-to 0= if  to-default exit  then  then
    stride if  $202  else  $201  then  play-if-not ;
\ action 0x1F..0x22 set up (Hewie_SetAction): seeing him, not off screen, not bitten or down
: at-pursuer ( -- )
    pursuer dup with? 0= if  drop 6 0 want exit  then
    mode-of 3 =  st-sub @ dup 9 = swap $A = or or if  6 0 want exit  then
    8 his-mode !  $18 his-sub !  pursuer his-target !  -1 his-grip !  0 his-biting !  8 his-cmd !
    his-path path-clear  ['] st-run-at-pursuer behave ;

\ a homunculus bitten (bite_landed): his temper with her easing (one fewer of her hits held
\ against him, for 300 frames more), his sound
: bite-landed ( -- )
    his-hits @ if  -1 his-hits +!  300 his-hit-t !  then  $6C make-sound ;

\ Hewie_PickTarget: whom he goes for - the stalker when he has hold of her; a homunculus holding
\ her down; the stalker; else the nearest creature up and after her, not struck or going -
\ each in his room, one he can get to (the stalker not off screen)
: reachable? ( id -- flag )  dup with? 0= if  drop false exit  then  dup body-tri swap body-at plan-to ;
: chase? ( -- flag )  pursuer dup with? if  dup mode-of 3 <> swap reachable? and  else  drop false  then ;
: her-held? ( sub -- flag )  her with? fiona-mode @ 4 = and  swap fiona-sub @ = and ;
4 constant creature-struck   8 constant creature-holding
: holding-her ( -- id | -1 )   \ (a homunculus, slots 7..9)
    10 7 do  i creature dup mode-of creature-holding = over reachable? and if  unloop exit  then  drop  loop  -1 ;
variable nearest  fvariable nearest-d
: closer? ( id -- flag ) ( F: -- )   \ (nearer than the nearest yet)
    body-at his-at vec-dist  nearest @ 0< fdup nearest-d f@ f< or  if  nearest-d f!  true  else  fdrop false  then ;
: nearest-creature ( -- id | -1 )
    -1 nearest !
    10 0 do
        i creature  dup with?  i creature-up? and  over mode-of creature-struck <> and
        over reachable? and  if  dup closer? if  dup nearest !  then  then  drop
    loop  nearest @ ;
: pick-target ( -- id | -1 )
    chase?  9 her-held? and if  pursuer exit  then
    $12 her-held? if  holding-her dup 0< 0= if  exit  then  drop  then
    chase? if  pursuer exit  then
    nearest-creature ;

\ ---- harrying (0x59 / 0x5A, 0x53 then 0x23: Hewie_StateCloseOnTarget): near him a while, then
\ at him and off again his-wanted times (0x5A: 3) ----
defer close-by ( F: rest -- done? )   :noname fdrop false ; is close-by   \ (hewie/pounce.fs: the pounce, or closing in)
: st-close-on ( -- )
    his-target @ with? 0= if  to-default exit  then
    -1 his-t1 +!  his-t1 @ 0= if  to-default exit  then
    his-target @ body-tri his-target @ body-at plan-to 0= if  to-default exit  then
    5 step-to-pose 0= if  $202 play-if-not  then
    settled? 0= if  exit  then
    rest 5e f< if
        his-wanted @ if  -1 his-wanted +!  then
        his-wanted @ 0= if  to-default exit  then
        his-target @ her = if  $61  else  $59  then  0 want exit
    then
    rest fdup 30e f< if  close-by if  exit  then  else  fdrop  then
    stride drop ;
: close-on ( -- )   \ (0x23)
    his-target @ dup with? 0= if  drop 6 0 want exit  then
    mode-of dup 3 = swap 4 = or if  6 0 want exit  then
    his-target @ body-at his-at vec-dist 10e f<= if
        his-target @ her = if  $61  else  $59  then  0 want exit
    then
    8 his-mode !  $5A his-t1 !  8 his-cmd !  ['] st-close-on behave ;
: harry ( -- )   \ (0x59: whom he goes for - near him, then 0x23)
    his-target2 @ 0< if  pick-target his-target2 !  then
    his-target2 @ dup with? if  his-target !  $53 $23 want  else  drop 6 0 want  then ;

\ ---- struck (Hewie_TakeBlow): `blow` by the stalker (his blows, the throw), by her (her shove
\ or kick: twice its damage, and he is scolded - hit often, angry), or a door slammed on him.
\ His yelp ($1000; $65 a shove, else $66), pushed back from whoever struck him (0x69: 1.5 less
\ 0.5 a frame; a door 0x6B: 5 less 1), then at its event down (out of health) or on ----
variable tb-from  variable tb-how
: yelped ( -- )   \ (Hewie_AfterYelp)
    his-hp @ 0= if   \ (down: counted for the dog's level at the end - not in a hard game)
        his-by @ pursuer? if  -3 feel+  then
        hard-mode? 0= if  progress pr.hewie-downs dup l@ 1+ 9999 min swap l!  then
        $52 0 want exit
    then
    0 his-2d !
    his-by @ her =  her-with? and  his-hits @ 0> and if
        his-hits @ 1 = if  9  else  $B  then  0 want exit
    then
    to-default ;
: st-on-event20 ( -- )  model 0 0 1 motion-events $20 and  anim-done? or if  yelped  then ;
: st-move-along ( -- )
    0e his-push f@ his-push-yaw f@ his-yaw f- angle-wrap rotate-by body-move-local
    his-push f@ his-push-less f@ f- fdup his-push f!  1 his-no-root !
    f0> if  exit  then
    ['] st-on-event20 behave ;
: yelp ( kind -- )   \ (0x69)
    4 his-mode !  0 his-cmd !  1.5e his-push f!  0.5e his-push-less f!
    tb-from @ dup -2 = if  drop his-blow-pt heading-to pi f+ angle-wrap
    else dup 0< 0= if  dup his-target !  body-at heading-to pi f+ angle-wrap  else  drop his-yaw  then then  his-push-yaw f!
    $1000 play  1 = if  $65  else  $66  then  make-sound
    $69 his-action !  ['] st-move-along behave ;
: door-yelp ( exit -- )   \ (0x6B: back from the door's swing, away from her side of it)
    4 his-mode !  0 his-cmd !  5e his-push f!  1e his-push-less f!
    dup door-turn  her body-at vec@ door-side 1 = if  pi f+ angle-wrap  then  his-push-yaw f!
    $1000 play  $66 make-sound
    $6B his-action !  ['] st-move-along behave ;
: take-blow ( kind damage how from -- taken? )
    dup tb-from !  his-by !  tb-how !                                ( kind damage )
    his-cond @ 2 = if  2drop  his-action @ $74 <> if  $74 0 want  then  true exit  then
    over $B <>  his-mode @ 4 = and if  2drop false exit  then
    over $B <>  his-mode @ 8 = and  his-sub @ $18 $1A within and if  2drop false exit  then
    tb-from @ her =  2 pick 5 <> and if  2*  then  his-hp @ swap - 0 max hp!   \ (her blows twice: not a door)
    his-hurt-t @ 0> 0= if  300 his-hurt-t !  then
    case
        5 of  tb-how @ door-yelp  true  endof                     \ (a door: how is its exit)
        6 of  false  endof  $B of  false  endof
        tb-from @ pursuer? if  rnd 0.25e f< if  -1 feel+  then  0 his-hits !
        else tb-from @ her = if
            0 3 praise-scold drop
            his-hits @ 1+ 6 min his-hits !  300 his-hit-t !
            16 roll  his-hits @ cells angry-chance + @ < if  3 -1 set-mood  then
        then then
        dup yelp  true swap
    endcase ;

\ ---- his scuffles off screen (Hewie_Scuffle): the stalker hunting in his room (its mode 1..3),
\ once a meeting and not within 900 frames of the last, him neither hurt nor down. By chance:
\ 15 in 100 both hurt, 45 him only, 15 the stalker only, 25 nothing. Him hurt: a tenth of his
\ health (not below 1), a grudge one time in four. Heard next door: his yelp ($66; $69 when he
\ was not hurt). The stalker held 30 .. 180 frames, hurt a tenth of its health (bite-damage)
\ unless it was him only or he may not attack - a liking one time in three. Then he waits it
\ out ($30) ----
0 constant both-hurt   1 constant he-is-hurt   2 constant it-is-hurt   3 constant nothing-came
create scuffle-chances  both-hurt , 15 ,  he-is-hurt , 45 ,  it-is-hurt , 15 ,  nothing-came , 25 ,
: scuffle-outcome ( -- outcome )   \ (the first whose chances so far pass a roll of 100)
    100 roll  {: r | sum :}
    scuffle-chances  begin  dup cell+ @ sum + to sum  r sum < 0= while  2 cells +  repeat  @ ;
: next-door? ( -- flag )   \ (his room is through an exit of hers)
    8 0 do  room-id i room-exit-leads drop his-room = if  true unloop exit  then  loop  false ;
: scuffle-heard ( outcome -- )
    next-door? 0= if  drop exit  then
    it-is-hurt = if  $69  else  $66  then  5 0 0  his-at vec@ his-sound ;
: scuffle ( -- )
    pursuer active?  pursuer with? 0= and if  0 his-scuffled !  then   \ (met again once it has been elsewhere)
    his-cond @ 1 3 within  his-hold-call @ or  his-scuffled @ or if  exit  then
    pursuer with?  st-alert @ 1 4 within and 0= if  exit  then
    -1 his-scuffled !  900 his-hold-call !
    scuffle-outcome  dup nothing-came = if  drop exit  then
    6 roll 30 * 30 +  his-wanted !
    dup it-is-hurt <> if
        his-hp @ 10 - 1 max hp!  rnd 0.25e f< if  -1 feel+  then
    then
    dup scuffle-heard
    he-is-hurt =  hewie-no-attack state-flag? or  fiona-occupied state-flag? or if  0
    else  st-hp-max @ 10 / bite-damage  rnd 0.33333e f< if  1 feel+  then  then
    8 swap his-wanted @  pursuer send blow
    $30 0 want ;
