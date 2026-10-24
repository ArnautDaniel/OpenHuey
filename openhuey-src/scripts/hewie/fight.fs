\ hewie/fight.fs - Hewie and the stalker (src/game/hewie.c): going for him (at-flank-act ..
\ at-leg-2-act: Hewie_StateRunAtPursuer, StateGoesForPursuer / GoForPursuer2 - the attack asked
\ of the stalker, who takes it with a spot to leap to or refuses), the leap (StateLeapGrip,
\ LeapLands), biting and holding on (StateBiteHold: bites by his trust, a shake roll each time
\ round, let go when she calls him off or the stalker is down), letting go (StateClamped) or
\ thrown off (StateHanging, StateJumpArc); harrying him (harry-act, close-on-act: keeping near,
\ closing in); struck (Hewie_TakeBlow: by the stalker, by her, by a door) - his yelp, pushed back
\ (StateMoveAlong), then down or on (Hewie_AfterYelp).
IN: hewie.fight
USING: engine keys game-state actors messages common facts paths vectors flag-names stalker-names hewie-names hewie.exits hewie.state hewie.body hewie.tables hewie.anims hewie.sounds hewie.model hewie.moving hewie.states floor-flags ;

\ his-biting: holding on - the first time round, still; letting go; thrown off. Thrown off
\ (hanging): he held on again, or flung
enum: bite-hold  first-hold still-holding letting-go thrown-off ;
0 constant held-on   1 constant flung
: at-leg? ( -- flag )  his-action @ dup at-leg-act =  over at-leg-2-act = or  swap tackle-act = or ;   \ (the story's tackle holds by the leg too)
: on-him? ( -- flag )  his-action @ dup at-flank-act = swap at-stalker-act = or ;   \ (at him: hanging on, flung)
: by-grip ( near far other -- anim )   \ (by his grip: the near flank, the far one, else)
    {: near far other :}  his-grip @ case  near-grip of  near  endof  far-grip of  far  endof  >r other r>  endcase ;

\ ---- his feelings about the stalker (Hewie_Feeling, Hewie_ChangeFeeling: -10..10) and how often
\ he has leapt at him (Hewie_CountEncounter: 0..100), kept for each of them by the stalker's
\ kind: Debilitas (and his figures), Daniella (and her later kinds), Lorenzo (his classes),
\ Riccardo (the TintStalker, his other class); none for any other kind ----
: kind-slot ( kind -- i | -1 )
    case
        debilitas-kind of  0  endof  debilitas-6-kind of  0  endof  debilitas-7-kind of  0  endof  debilitas-27-kind of  0  endof
        daniella-kind of  1  endof  kind34-kind of  1  endof  kind35-kind of  1  endof  kind36-kind of  1  endof
        lorenzo2-kind of  2  endof  lorenzo-kind of  2  endof  kind12-kind of  2  endof  kind39-kind of  2  endof
        riccardo-kind of  3  endof  tintstalker-kind of  3  endof  kind37-kind of  3  endof
        >r -1 r>
    endcase ;
: stalker-slot ( -- i | -1 )  st-kind @ kind-slot ;
: feel ( -- addr | 0 )  stalker-slot dup 0< if  drop 0  else  cells his-feel +  then ;
: feeling ( -- n )  feel dup if  @  then ;   \ (none: 0)
: met ( -- addr | 0 )  stalker-slot dup 0< if  drop 0  else  cells his-met +  then ;
: met+ ( -- )  met ?dup if  dup @ 1+ 100 min swap !  then ;   \ Hewie_CountEncounter
\ Hewie_FeelPursuer: his mood normal, a stalker about - the more he dislikes him, the likelier he
\ is upset: a 1-in-16 roll under 14 at -10, falling by one a step to 1 at 0, none above
create upset-chance  14 , 10 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 , 1 ,
: feel-pursuer ( -- )
    his-mood @ normal-mood <>  pursuer active? 0= or  feel 0= or if  exit  then
    feeling 0> if  exit  then
    feeling 10 + cells upset-chance + @  16 roll > if  upset-mood -1 set-mood  then ;
: feel+ ( n -- )  feel {: n a :}  a if  a @ n + -10 10 clamp a !  then  feel-pursuer ;
\ Hewie_Dares: from trust 2, a 1-in-16 roll under his level + 1 + how often he has leapt at the
\ stalker / 5 (at most 8)
: dares? ( -- flag )
    his-trust @ 2 <  pursuer active? 0= or if  false exit  then
    met dup if  @ 5 / 8 min  then  his-trust @ + 1+  16 roll > ;

\ ---- a bite (bite_pursuer): its damage (bite-damage), hard by chance - the more hurt the
\ stalker, the likelier (Hewie_ReactionRoll; the chances' rows by the bite) ----
enum: bite-row  far-bite far-charged-bite near-bite near-charged-bite head-on-bite leg-bite
    tackle-front-bite tackle-behind-bite tackle-front-charged-bite tackle-behind-charged-bite rush-bite ;
: band ( -- n )   \ (his health over its most: above 0.8 0, 0.6 1, 0.4 2, 0.2 3, 0.1 4, else 5)
    st-hp @ s>f st-hp-max @ 1 max s>f f/
    fdup 0.1e f< if  fdrop 5 exit  then  fdup 0.2e f< if  fdrop 4 exit  then
    fdup 0.4e f< if  fdrop 3 exit  then  fdup 0.6e f< if  fdrop 2 exit  then
    0.8e f< if  1  else  0  then ;
: hard-bite? ( row -- flag )  6 * band + cells hard-bite-chance + @  100 roll > ;
\ (Hewie_BiteEffect: a mark at his mouth - a big one with it when he came charging; at the first
\ bite and at each of the bite animations' $20 keys)
$25 constant his-mouth   \ (his mouth's bone)
create mouth-at /vec allot
: bite-mark ( -- )
    his-model @ his-mouth bone-pos mouth-at vec!
    his-ready @ if  his-tri -1 mouth-at vec@ spray  then
    his-tri 0 mouth-at vec@ spray ;
: event? ( bit -- flag )  model 0 0 1 motion-events and 0<> ;
: bite-him ( row base -- )  bite-damage  swap hard-bite? pursuer send bite ;

\ ---- which side he comes at him from (Hewie_AttackSide): from behind, facing the stalker's
\ back - head on (not while he is timid); else a flank (Daniella the other way) ----
: daniella? ( -- flag )  st-kind @ dup daniella-kind = swap kind34-kind kind37-kind within or ;
: attack-side ( -- grip )
    pursuer body-at heading-to                                     ( F: a )
    hewie-timid state-flag? 0=
    fdup pursuer body-yaw f- angle-wrap fabs pi f2/ f< and
    fdup his-yaw f- angle-wrap fabs pi f2/ f< and if  fdrop head-on-grip exit  then
    pi f+ angle-wrap pursuer body-yaw f- angle-wrap f0< if
        daniella? if  near-grip  else  far-grip  then
    else  daniella? st-kind @ lorenzo-kind = or if  far-grip  else  near-grip  then  then ;

\ ---- held at the stalker's height (his feet's plane: HumanModel +0x8C0) ----
: his-height ( F: -- y )  pursuer body-at v.y sf@ ;
: at-his-height ( -- )  his-height his-y! ;

\ ---- thrown off (StateHanging, StateJumpArc): hanging on by his teeth to the end of it; then
\ he holds on again if he dares, else flung (10 damage) - along his last root motion, falling;
\ landing, then on ----
\ (Hewie_State18B0: down - 300 frames before he's petted again; flung, he gets up and is as
\ after a yelp - down, or cowering from her)
defer after-yelp ( -- )   \ (yelped, below)
: st-landed ( -- )
    anim-done? 0= if  exit  then
    300 his-pet-time !
    his-biting @ held-on = if  to-default exit  then
    anim@ flung-anim = if  his-cond @ down-cond <> if  get-up-anim play  else  after-yelp  then  exit  then
    anim@ get-up-anim = if  after-yelp  then ;
\ his splash into water (the ripple in rooms 7 / $106)
: splash-here ( -- )
    his-tri dup 0< if  drop exit  then  nav-flags water-floor and water-floor <> if  exit  then
    splash-sound 6 0 0 his-at vec@ his-sound  flooded? if  1 his-at water-step  then ;
: land ( -- )  his-biting @ held-on = if  land-held-anim  else  flung-anim  then  play  ['] st-landed behave ;
: air-floor ( F: -- y )  his-at v.x sf@ his-fly-h f@ his-at v.z sf@ nav-floor drop ;
: fly-on ( -- landed? )   \ (a frame of it: across by his-fly-x / z, up by his-fly-y less 0.5 a frame)
    his-fly-x f@ his-fly-z f@ body-move-local  1 his-no-root !
    his-fly-h f@ his-fly-y f@ f+  fdup air-floor f< if  fdrop true exit  then
    fdup his-fly-h f!  his-y!  his-fly-y f@ 0.5e f- his-fly-y f!  false ;
: st-falling ( -- )   \ (Hewie_StateFalling: coming down - held on, the hanging pose; flung, a splash)
    his-biting @ held-on = anim-done? and on-him? and if
        his-grip @ case
            near-grip of  anim@ falling-held-near-anim <> if  falling-held-near-anim play-cut  then  endof
            far-grip of  anim@ top-flung-far-anim <> if  falling-held-far-anim play-cut  then  endof   \ (the original tests the flung top, starts this)
        endcase
    then
    fly-on if  land  his-biting @ flung = if  splash-here  then  then ;
: st-jump-arc ( -- )   \ (Hewie_StateJumpArc: going up - at the top, falling)
    fly-on if  land exit  then
    his-fly-y f@ f0> if  exit  then
    on-him? if
        his-biting @ flung = if  top-flung-near-anim top-flung-far-anim  else  top-held-near-anim top-held-far-anim  then
        0 by-grip ?dup if  play  then
    then  ['] st-falling behave ;
: st-hanging ( -- )
    root@ {: F: turn x up z :}
    x z body-move-local  his-fly-h f@ up f+ fdup his-fly-h f! his-y!  1 his-no-root !
    anim-done? 0= if  exit  then
    pursuer with? 0= if  to-default exit  then
    rnd 0.25e f< if  -1 feel+  then
    dares? if  held-on his-biting !  his-grip @ near-grip = if  hold-again-near-anim  else  hold-again-far-anim  then
    else  flung his-biting !  10 hurt-by  his-grip @ near-grip = if  flung-near-anim  else  flung-far-anim  then  then  play-cut
    x his-fly-x f!  up his-fly-y f!  z his-fly-z f!
    ['] st-jump-arc behave ;

\ ---- letting go (StateClamped): the let-go animation, its bite marks at its $20 keys, until its
\ end, then on (+0xF3688 300 frames: his-pet-time); held at the stalker's height till its key 2,
\ then down from it over 5 frames (his-fly-h 0..1, then 2: free) ----
: ease-off ( -- )
    his-fly-h f@ 0.2e f+  fdup 1e f< 0= if  fdrop 2e his-fly-h f! exit  then
    fdup his-fly-h f!  {: F: k :}
    1e k f- his-height f*  k his-at v.y sf@ f* f+  his-y! ;
: st-clamped ( -- )
    $20 event? if  bite-mark  then
    anim-done? if  300 his-pet-time !  to-default exit  then
    slide-root
    pursuer with? 0= if  exit  then
    his-biting @ first-hold = if  2 event? if  still-holding his-biting !  then  at-his-height exit  then
    his-fly-h f@ 1e f< if  ease-off  then ;

\ ---- biting and holding on (StateBiteHold): at each time round, a shake roll - his bites left,
\ his chance to hold on (+30 the first time, -10 each time held); failing it he lets go and
\ the stalker shakes. Thrown off meanwhile (the stalker's `shaken-off`): hanging. Let go when
\ she calls him off (her come / come back) or the stalker is down ----
: shake-off ( -- )  letting-go his-biting !  pursuer send shaken-off ;
: bite-anim ( -- a )  at-leg? if  bite-leg-anim exit  then  bite-near-anim bite-far-anim bite-head-on-anim by-grip ;
: let-go-anim ( -- a )  at-leg? if  let-go-leg-anim exit  then  let-go-near-anim let-go-far-anim let-go-head-on-anim by-grip ;
: bite-again ( -- )   \ (the bite by his action and grip: damage 10 the leg, 2 / 3 a flank, 3 head on)
    bite-anim play-if-not
    at-leg? if  leg-bite 10 bite-him exit  then
    his-grip @ case
        near-grip of  his-ready @ if  near-charged-bite 3  else  near-bite 2  then  endof
        far-grip of  his-ready @ if  far-charged-bite 3  else  far-bite 2  then  endof
        >r head-on-bite 3 r>
    endcase  bite-him ;
: shake-roll ( -- )
    his-bites @ 1- dup his-bites !
    if  his-hold @  his-biting @ first-hold = if  still-holding his-biting !  30 +  0 his-mash !  then  else  0  then
    100 roll > if  his-hold @ 10 - 0 max his-hold !  else  shake-off  then ;
: let-go ( -- )  first-hold his-biting !  0e his-fly-h f!  let-go-anim play-cut  ['] st-clamped behave ;
: hang-on ( -- )   \ (thrown off at a flank: hanging on; else he lets go)
    his-grip @ dup near-grip = swap far-grip = or  on-him? and 0= if  let-go exit  then
    his-grip @ near-grip = if  hanging-near-anim  else  hanging-far-anim  then  play-cut
    at-his-height  his-height his-fly-h f!  struck-mode his-mode !  holding-on-sub his-sub !
    ['] st-hanging behave ;
: st-bite-hold ( -- )
    pursuer with? 0= if  to-default exit  then
    $20 event? if  bite-mark  then
    anim-done? if
        his-biting @ letting-go < if  shake-roll  then
        his-biting @ case
            letting-go of  let-go exit  endof
            thrown-off of  hang-on exit  endof
        endcase
        bite-again
    else
        his-biting @ letting-go < hewie-played? 0= and if   \ (her command this frame: come / come back - let go; attack now - his grip anew)
            his-heard @ dup come-cmd = swap come-back-cmd = or  st-hp @ 0= or if  shake-off
            else  his-biting @ still-holding = his-heard @ attack-now-cmd = and if  first-hold his-biting !  then  then
        then
        his-biting @ letting-go < hewie-played? and if   \ (the run: cross held lets go; square five times, his grip anew)
            cross button-down?  st-hp @ 0= or if  shake-off
            else
                square button-pressed? if  1 his-mash +!  then
                his-biting @ still-holding = his-mash @ 5 >= and if  first-hold his-biting !  then
            then
        then
    then
    slide-root  at-his-height  1 his-on-him ! ;

\ ---- the leap (StateLeapGrip, StateLeapLands): his bites by his action, grip and trust; his way
\ to the spot and the turn covered in 8 frames (the leg 12), at the stalker's height; landed -
\ the first bite (the leg 5 / 30, a flank 2 or 3 / 5 or 10 charged, head on 4 / 10), his sound,
\ on the spot facing the grip's heading, holding on (chance (his feeling + 10) x 5) ----
: first-bite ( -- )
    at-leg? if  leg-bite 30 bite-him  bite-mark exit  then
    his-grip @ case
        near-grip of  his-ready @ if  near-charged-bite 10  else  near-bite 5  then  endof
        far-grip of  his-ready @ if  far-charged-bite 10  else  far-bite 5  then  endof
        >r head-on-bite 10 r>
    endcase  bite-him  bite-mark ;
: st-leap-lands ( -- )
    pursuer with? 0= if  to-default exit  then
    anim-done? if
        first-bite  bite-sound make-sound
        his-to vec@ his-place  his-leap-yaw f@ his-yaw!
        bite-anim play-cut  still-holding his-biting !
        feeling 10 + 5 * his-hold !  0 his-mash !
        ['] st-bite-hold behave
    else
        his-leap-yaw f@ his-leap-turn f@ body-turn-toward fdrop
        his-leap-k f@ ahead@ his-path path-i!  ahead vec@ his-place  1 his-no-root !
    then
    at-his-height  1 his-on-him ! ;
: bites-by-trust ( -- n )
    his-action @ case
        at-flank-act of
            his-grip @ head-on-grip <> if  his-ready @ if  bites-flank-charged  else  bites-flank  then
            else  bites-head-on  then  trust-of  endof
        at-leg-act of  bites-leg trust-of  endof
        >r 1 r>
    endcase ;
: turn-round ( -- )  his-leap-yaw f@ pi f+ angle-wrap his-leap-yaw f! ;
: leap ( tri F: x y z yaw -- )
    his-leap-yaw f!  his-to vec!  his-to-tri !
    his-to-tri @ his-to plan-to 0= if  to-default exit  then
    bites-by-trust his-bites !
    no-call his-call !   \ (the call answered)
    his-hits @ if  -1 his-hits +!  300 his-hit-t !  then
    leaping-sub his-sub !
    rnd 0.33333e f< if  1 feel+  then  met+
    1 his-on-him !
    at-leg? if  leap-leg-anim
    else  his-grip @ case
            near-grip of  st-kind @ lorenzo-kind <> if  turn-round  then  leap-near-anim  endof
            far-grip of  turn-round  leap-far-anim  endof
            >r leap-head-on-anim r>
          endcase
    then  play
    at-leg? if  12e  else  8e  then  {: F: frames :}
    rest frames f/ his-leap-k f!
    his-leap-yaw f@ his-yaw f- angle-wrap fabs frames f/ his-leap-turn f!
    leap-snarl make-sound
    ['] st-leap-lands behave ;

\ ---- the story's tackle (0x75: Hewie_SetAction, StateTargetPursuer / GoForPursuer - his slot
\ command 2 / 7 to the stalker, `dog-tackle`, answered with the spot by his leg - StateLeapLaunch,
\ LeapTakeOff, LeapAtPursuer): the leap there along his way, a third of its length in frames (19
\ at least), turning to the grip's heading and rising to land at its height under 0.3 a frame of
\ fall, never below the floor ($1E01, his snarl); the spring faded in, the leg's bite ($2218) faded
\ in over the frames left, held still meanwhile; 13 before he lands his attack at the leg asked
\ (`dog-attack`: the stalker bitten, the spot anew). Landed - on the spot, the bite of 30 (hard by
\ chance; the black shepherd twice), its big mark, holding on with one bite left: let go at its end
\ (the hold by his mood - angry 110, upset none, else (his feeling + 10) x 5 - is never rolled
\ against) or when she calls him off. Refused, or the stalker gone: his default ----
0.3e fconstant tackle-fall   19 constant tackle-frames   13 constant tackle-asks-at
: play-held ( anim n -- )   \ (Motion_PlayWith flags 9: looping, its time held while it fades in over n)
    rate-1  {: anim n :}  model anim n 9 motion-play ;
: tackle-fly ( -- )   \ (leap_fly)
    his-leap-yaw f@ his-leap-turn f@ body-turn-toward fdrop
    his-fly-x f@ ahead@ his-path path-i!  ahead vec@ his-place
    his-fly-y f@ tackle-fall f- fdup his-fly-y f!  his-fly-h f@ f+ fdup his-fly-h f!
    fdup his-at v.y sf@ f> if  his-y!  else  fdrop  then
    1 his-no-root ! ;
: tackle-hold ( -- n )   \ (by his mood: never rolled against - one bite left)
    his-mood @ case  angry-mood of  110  endof  upset-mood of  0  endof  >r feeling 10 + 5 * r>  endcase ;
: st-tackle-lands ( -- )   \ (Hewie_StateLeapAtPursuer)
    pursuer with? 0= if  to-default exit  then
    settled? 0= if
        -1 his-t1 +!
        his-t1 @ tackle-asks-at =  hewie-no-attack state-flag? fiona-occupied state-flag? or 0= and if
            leg-attack pursuer send dog-attack
        then
        tackle-fly
    else
        his-room his-to-tri @ his-to vec@ body-place
        tackle-hold his-hold !  still-holding his-biting !  1 his-bites !  1 his-ready !
        leg-bite 30 bite-him  bite-mark
        ['] st-bite-hold behave
    then
    1 his-on-him !  1 his-no-root ! ;
: st-tackle-take-off ( -- )   \ (Hewie_StateLeapTakeOff)
    pursuer with? 0= if  to-default exit  then
    settled? if  bite-leg-anim his-t1 @ play-held  ['] st-tackle-lands behave  then
    tackle-fly  -1 his-t1 +! ;
: tackle-launch ( -- )   \ (Hewie_StateLeapLaunch: to his-to on his-to-tri, the grip's heading his-leap-yaw)
    pursuer with? 0= if  to-default exit  then
    his-to-tri @ his-to plan-to 0= if  to-default exit  then   \ ("Dog New Hide Attack -> No Route")
    rest {: F: r :}  r 3e f/ f>s tackle-frames max {: n :}  n s>f {: F: k :}
    his-at v.y sf@ his-fly-h f!  n his-t1 !  r k f/ his-fly-x f!
    his-to v.y sf@ his-at v.y sf@ f- k f/  0.5e tackle-fall f* k f* f+  his-fly-y f!
    his-leap-yaw f@ his-yaw f- angle-wrap fabs k f/ his-leap-turn f!
    tackle-anim play  1 his-no-root !  leap-snarl make-sound
    ['] st-tackle-take-off behave ;
: st-await-tackle ( -- )  pursuer with? 0= if  to-default  then ;   \ (Hewie_StateGoForPursuer: his answer)
: st-target-pursuer ( -- )   \ (Hewie_StateTargetPursuer: here - the tackle asked of him)
    pursuer with? 0= if  to-default exit  then
    pursuer his-target !  pursuer send dog-tackle  ['] st-await-tackle behave ;
: tackle ( -- )   \ (Hewie_SetAction 0x75: seeing him, not off screen, not bitten or down)
    pursuer dup with? 0= if  drop to-default exit  then
    mode-of unreachable-mode =  st-sub @ dup bitten-sub = swap down-sub = or  or if  to-default exit  then
    engaged-mode his-mode !  pursuer his-target !  ahead-look look-now!  0 his-cmd !  1 his-2d !
    ['] st-target-pursuer behave ;
\ the stalker's answers while he tackles: the spot (then the leap), the spot anew as he reaches
\ him (where he lands), or refused
: tackling? ( -- flag )  his-act @ dup ['] st-await-tackle =  swap ['] st-tackle-lands = or ;
: tackle-answer ( tri x y z yaw -- )   \ (floats in cells)
    cell>f {: tri x y z F: yaw :}  x cell>f y cell>f z cell>f his-to vec!  tri his-to-tri !
    his-act @ ['] st-await-tackle = if  yaw his-leap-yaw f!  ['] tackle-launch behave  then ;

\ ---- going for him (StateRunAtPursuer, StateGoesForPursuer, GoForPursuer2): at him at a trot
\ or a run; within 23, the side he comes from picks the grip (and the action), the attack is
\ asked of the stalker (`dog-attack`), and he waits for the answer ----
: st-await-grip ( -- )  pursuer with? 0= if  to-default exit  then  stand-pose keep-pose ;
: attack-kind ( -- kind )
    at-leg? if  leg-attack exit  then
    his-grip @ case  near-grip of  his-ready @ if  near-charged-attack  else  near-flank-attack  then  endof
                    far-grip of  his-ready @ if  far-charged-attack  else  far-flank-attack  then  endof
                    >r head-on-attack r>  endcase ;
: goes-for ( -- )
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  to-default exit  then
    his-grip @ no-grip = if
        attack-side dup head-on-grip = if
            on-him? if  head-on-grip his-grip !  then  drop
        else
            his-action @ dup at-flank-act = swap at-leg-act = or if  at-flank-act  else  at-stalker-act  then  his-action !
            his-grip !
        then
    then
    attack-kind pursuer send dog-attack
    ['] st-await-grip behave ;
: st-run-at-pursuer ( -- )
    pursuer with? 0= if  to-default exit  then
    pursuer body-at his-at vec-dist 23e f< if   \ (nothing breaks in from here)
        ahead-look look-now!  shut-out  1 his-2d !  goes-for exit
    then
    path-done? if  pursuer body-tri pursuer body-at plan-to 0= if  to-default exit  then  then
    stride if  run-anim  else  trot-anim  then  play-if-not ;
\ at-flank-act .. at-leg-2-act set up (Hewie_SetAction): seeing him, not off screen, not bitten or down
: at-pursuer ( -- )
    pursuer dup with? 0= if  drop missed-pursuer exit  then
    mode-of unreachable-mode =  st-sub @ dup bitten-sub = swap down-sub = or  or if  missed-pursuer exit  then
    engaged-mode his-mode !  going-for-sub his-sub !  pursuer his-target !  no-grip his-grip !  first-hold his-biting !  on-command his-cmd !
    his-path path-clear  ['] st-run-at-pursuer behave ;

\ a homunculus bitten (bite_landed): his temper with her easing (one fewer of her hits held
\ against him, for 300 frames more), his sound
: bite-landed ( -- )
    his-hits @ if  -1 his-hits +!  300 his-hit-t !  then  no-call his-call !  snap-sound make-sound ;

\ Hewie_PickTarget: whom he goes for - the stalker when he has hold of her; a homunculus holding
\ her down; the stalker; else the nearest creature up and after her, not struck or going -
\ each in his room, one he can get to (the stalker not off screen)
: reachable? ( id -- flag )  dup with? 0= if  drop false exit  then  dup body-tri swap body-at plan-to ;
: chase? ( -- flag )  pursuer dup with? if  dup mode-of unreachable-mode <> swap reachable? and  else  drop false  then ;
: her-held? ( sub -- flag )  her with? fiona-mode @ thrown-mode = and  swap fiona-sub @ = and ;
: holding-her ( -- id | -1 )   \ (a homunculus, slots 7..9)
    #creatures first-homunculus do  i creature dup mode-of creature-holding = over reachable? and if  unloop exit  then  drop  loop  -1 ;
: nearest-creature ( -- id | -1 )   \ (up, not struck, one he can get to: the nearest)
    -1 0e {: best F: best-d :}
    10 0 do
        i creature  dup with?  i creature-up? and  over mode-of creature-struck <> and  over reachable? and if
            dup body-at his-at vec-dist  best 0< fdup best-d f< or if  to best-d  dup to best  else  fdrop  then
        then  drop
    loop  best ;
: pick-target ( -- id | -1 )
    chase?  led-away-r her-held? and if  pursuer exit  then
    held-down-r her-held? if  holding-her dup 0< 0= if  exit  then  drop  then
    chase? if  pursuer exit  then
    nearest-creature ;

\ ---- harrying (harry-act / harry-stalker-act; near-target-act then close-on-act:
\ Hewie_StateCloseOnTarget): near him a while, then at him and off again his-wanted times ----
defer close-by ( F: rest -- done? )   :noname fdrop false ; is close-by   \ (hewie/pounce.fs: the pounce, or closing in)
: harried ( -- )  his-target @ her = if  harry-her-act  else  harry-act  then  0 want ;
: st-close-on ( -- )
    his-target @ with? 0= if  to-default exit  then
    -1 his-t1 +!  his-t1 @ 0= if  to-default exit  then
    his-target @ body-tri his-target @ body-at plan-to 0= if  to-default exit  then
    as-is-pose step-to-pose 0= if  run-anim play-if-not  then
    settled? 0= if  exit  then
    rest 5e f< if
        his-wanted @ if  -1 his-wanted +!  then
        his-wanted @ 0= if  to-default exit  then
        harried exit
    then
    rest fdup 30e f< if  close-by if  exit  then  else  fdrop  then
    stride drop ;
: close-on ( -- )
    his-target @ dup with? 0= if  drop give-up exit  then
    mode-of dup unreachable-mode = swap struck-mode = or if  give-up exit  then
    his-target @ body-at his-at vec-dist 10e f<= if  harried exit  then
    engaged-mode his-mode !  90 his-t1 !  on-command his-cmd !  ['] st-close-on behave ;
: harry ( -- )   \ (whom he goes for - near him, then at him)
    his-target2 @ 0< if  pick-target his-target2 !  then
    his-target2 @ dup with? if  his-target !  near-target-act close-on-act want  else  drop sniff-act 0 want  then ;

\ ---- struck (Hewie_TakeBlow): `blow` by the stalker (his blows, the throw), by her (her shove
\ or kick: twice its damage, and he is scolded - hit often, angry), or a door slammed on him.
\ His yelp (loud for a shove), pushed back from whoever struck him (1.5 less 0.5 a frame; a
\ door 5 less 1), then at its event down (out of health) or on ----
: yelped ( -- )   \ (Hewie_AfterYelp)
    his-hp @ 0= if   \ (down: counted for the dog's level at the end - not in a hard game)
        his-by @ pursuer? if  -3 feel+  then
        hard-mode? 0= if  progress pr.hewie-downs dup l@ 1+ 9999 min swap l!  then
        knocked-down-act 0 want exit
    then
    0 his-2d !
    his-by @ her =  her-with? and  his-hits @ 0> and  hewie-played? 0= and if   \ (cowering from her - not in the run)
        his-hits @ 1 = if  wait-for-her-act  else  bark-at-her-act  then  0 want exit
    then
    to-default ;
' yelped is after-yelp
: st-on-event20 ( -- )  $20 event?  anim-done? or if  yelped  then ;
: st-move-along ( -- )
    0e his-push f@ his-push-yaw f@ his-yaw f- angle-wrap rotate-by body-move-local
    his-push f@ his-push-less f@ f- fdup his-push f!  1 his-no-root !
    f0> if  exit  then
    ['] st-on-event20 behave ;
: yelp ( kind from -- )   \ (yelp-act; from -2: a blow from a point, his-blow-pt)
    {: kind from :}
    struck-mode his-mode !  0 his-cmd !  1.5e his-push f!  0.5e his-push-less f!
    from -2 = if  his-blow-pt heading-to pi f+ angle-wrap
    else from 0< 0= if  from his-target !  from body-at heading-to pi f+ angle-wrap  else  his-yaw  then then  his-push-yaw f!
    yelp-anim play  kind shove-blow = if  shoved-yelp  else  yelp-sound  then  make-sound
    yelp-act his-action !  ['] st-move-along behave ;
: door-yelp ( F: yaw -- )   \ (back from the door's swing, along yaw)
    his-push-yaw f!
    struck-mode his-mode !  0 his-cmd !  5e his-push f!  1e his-push-less f!
    yelp-anim play  yelp-sound make-sound
    door-yelp-act his-action !  ['] st-move-along behave ;
\ a door swung into him (its exit): she not in front of it - back along its turn, through blocked
\ floor; she in front of it and he on its sides where he may pass - out by it, against its turn;
\ else, off its gap, aside (a quarter turn); in its gap nothing more
: door-hit ( exit -- )
    {: e :}  e door-turn {: F: yaw :}
    e her body-at vec@ door-side 1 <> if  1 his-2b !  yaw door-yelp exit  then
    e on-its-sides?  e may-pass? and if  yaw pi f+ angle-wrap his-push-yaw f!  pushed-out-act 0 want exit  then
    on-gap? 0= if  yaw pi f2/ f+ angle-wrap door-yelp  then ;
\ who called him, struck while angry: her, else whoever it was (none: nobody)
2 constant other-call
: caller ( from -- n )  dup 0< if  drop no-call exit  then  her = if  0  else  other-call  then ;
: take-blow ( kind damage how from -- taken? )
    {: kind damage how from :}  from his-by !
    his-cond @ down-cond =  kind ignored-blow <> and if   \ (down: he goes limp - not at a grab)
        his-action @ go-limp-act <> if  go-limp-act 0 want  then  true exit
    then
    kind throw-off-blow <>  his-mode @ struck-mode = and if  false exit  then
    kind throw-off-blow <>  his-mode @ engaged-mode = and  his-sub @ going-for-sub leaping-sub 1+ within and if  false exit  then
    kind door-blow = if
        how door-animating? 0= if  true exit  then   \ (a door hurts only while it swings)
        damage hurt-by
    else
        his-mood @ angry-mood = if  from caller his-call !  then
        damage  from her = if  2*  then  hurt-by   \ (her blows twice)
    then
    his-hurt-t @ 0> 0= if  300 his-hurt-t !  then
    0 his-2b !
    kind case
        door-blow of  how door-hit  true  endof                    \ (a door: how is its exit)
        ignored-blow of  false  endof  throw-off-blow of  false  endof
        from 0< 0= from her <> and if   \ (by another: a grudge now and then, her hits forgiven)
            from pursuer? if  rnd 0.25e f< if  -1 feel+  then  then  0 his-hits !
        else from her = if
            0 3 praise-scold drop
            his-hits @ 1+ 6 min his-hits !  300 his-hit-t !
            16 roll  his-hits @ cells angry-chance + @ < if  no-call his-call !  angry-mood -1 set-mood  then
        then then
        dup from yelp  true swap
    endcase ;

\ ---- his scuffles off screen (Hewie_Scuffle): the stalker hunting in his room (its alert 1..3), no
\ script of the room having him (stalker-ties),
\ once a meeting and not within 900 frames of the last, him neither hurt nor down. By chance:
\ 15 in 100 both hurt, 45 him only, 15 the stalker only, 25 nothing. Him hurt: a tenth of his
\ health (not below 1), a grudge one time in four. Heard next door: his yelp (another when he
\ was not hurt). The stalker held 30 .. 180 frames, hurt a tenth of its health (bite-damage)
\ unless it was him only or he may not attack - a liking one time in three. Then he waits it out ----
0 constant both-hurt   1 constant he-is-hurt   2 constant it-is-hurt   3 constant nothing-came
create scuffle-chances  both-hurt , 15 ,  he-is-hurt , 45 ,  it-is-hurt , 15 ,  nothing-came , 25 ,
: scuffle-outcome ( -- outcome )   \ (the first whose chances so far pass a roll of 100)
    100 roll  {: r | sum :}
    scuffle-chances  begin  dup cell+ @ sum + to sum  r sum < 0= while  2 cells +  repeat  @ ;
: next-door? ( -- flag )   \ (his room is through an exit of hers)
    8 0 do  room-id i room-exit-leads drop his-room = if  true unloop exit  then  loop  false ;
: scuffle-heard ( outcome -- )
    next-door? 0= if  drop exit  then
    it-is-hurt = if  scuffle-yelp  else  yelp-sound  then  5 0 0  his-at vec@ his-sound ;
: scuffle ( -- )
    pursuer active?  pursuer with? 0= and if  0 his-scuffled !  then   \ (met again once it has been elsewhere)
    his-cond @ hurt-cond down-cond 1+ within  his-hold-call @ or  his-scuffled @ or if  exit  then
    pursuer with?  st-alert @ 1 4 within and  st-scripted @ 0= and 0= if  exit  then   \ (not one a script has: +0xE0)
    -1 his-scuffled !  900 his-hold-call !
    scuffle-outcome  dup nothing-came = if  drop exit  then
    6 roll 30 * 30 +  his-wanted !
    dup it-is-hurt <> if
        10 hurt-by  his-hp @ 1 max hp!  rnd 0.25e f< if  -1 feel+  then
    then
    dup scuffle-heard
    he-is-hurt =  hewie-no-attack state-flag? or  fiona-occupied state-flag? or if  0
    else  st-hp-max @ 10 / bite-damage  rnd 0.33333e f< if  1 feel+  then  then
    scuffle-blow swap his-wanted @  pursuer send blow
    wait-scuffle-act 0 want ;
