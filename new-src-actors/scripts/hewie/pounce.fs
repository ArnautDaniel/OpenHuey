\ hewie/pounce.fs - his pounce and his rush (Hewie_StateCloseOnTarget's leap, Hewie_StateTackle,
\ StateLeapBite, StateLanding; Hewie_StateCloseIn, StateRush - rush-act): close on his target
\ on open floor and facing it, he leaps at it - 2.8 a frame, up to 5 above it and down again - and
\ whoever his mouth reaches on the way takes it: the stalker a blow (5 from in front, 8 from
\ behind; charged 10 / 16; bite-damage: black shepherd, Sylvesta Esca - hard by chance, the more
\ hurt he is the likelier), else Fiona; the homunculi it reaches are bitten. On a step or by a
\ ladder (a sloped floor) he can't leap: he runs at it and bites.
IN: hewie.pounce
USING: engine game-state actors messages common facts paths vectors flag-names hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.sounds hewie.model hewie.moving hewie.states hewie.fight ;

\ ---- whom his mouth reaches (Progress_CharNear: his head's bone within 3 of them) ----
$1F constant his-head-bone
create mouth /vec allot
: mouth! ( -- )  model his-head-bone bone-pos mouth vec! ;
: reaches? ( id -- flag )
    {: id :}  id 0< if  false exit  then  id active? 0= if  false exit  then
    mouth v.y sf@  id body-at v.y sf@ 3e f-  f> 0= if  false exit  then                               \ (above their feet)
    mouth v.y sf@  id body-at v.y sf@  id body-dims fswap fdrop f+ 3e f+  f< 0= if  false exit  then   \ (below their top)
    mouth id body-at vec-dist-xz  id body-dims fdrop 3e f+  f< ;                                      \ (within their reach)
: behind-of? ( id -- flag )   \ (his heading within 90 degrees of theirs: at their back)
    body-yaw his-yaw f- angle-wrap fabs pi f2/ f< ;
\ the damage: from in front / behind, charged (his-ready); bite-damage
: tackle-damage ( behind? -- n )
    his-ready @ if  if 16 else 10 then  else  if 8 else 5 then  then
    bite-damage ;
: tackle-row ( behind? -- row )
    his-ready @ if  if  tackle-behind-charged-bite  else  tackle-front-charged-bite  then
    else  if  tackle-behind-bite  else  tackle-front-bite  then  then ;
: blow-kind-of ( -- kind )  his-ready @ if  kick-blow  else  shove-blow  then ;   \ (charged: the harder)
variable hewie-blow-n   -2000 hewie-blow-n !   \ (his blows' numbers for the homunculi: down from -2000)
: new-bite ( -- )  -1 hewie-blow-n +! ;   \ (a rush's or a leap's: each homunculus bitten once in it)
: bite-creatures ( damage -- )   \ (Hewie_Bite: the homunculi his mouth reaches - as a blow there, within 10 of its body)
    {: damage :}  hewie-blow-n @ damage  mouth v.x sf@ f>cell  mouth v.y sf@ f>cell  mouth v.z sf@ f>cell  10e f>cell
    broadcast fiona-blow ;
\ the tackle's bite, once a leap (his-t3: 1 her, 4 the stalker bitten): the stalker, else Fiona
\ (her `hit` - knocked down)
1 constant bit-her   4 constant bit-him
: tackle-bite ( -- )
    {: | behind :}
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  exit  then
    mouth!
    his-t3 @ bit-him and 0=  pursuer reaches? and if
        pursuer behind-of? to behind
        blow-kind-of  behind tackle-damage  behind tackle-row hard-bite? if  $8000  else  0  then   ( kind damage how )
        pursuer send blow
        his-t3 @ bit-him or his-t3 !  snap-sound make-sound
    else
        his-t3 @ bit-her and 0=  her reaches? and if
            his-ready @ if  heavy-blow-request  else  blow-request  then  0 20e f>cell her send hit
            his-t3 @ bit-her or his-t3 !
        then
    then
    his-ready @ if  10  else  5  then  bite-creatures ;

\ ---- the leap (Hewie_StateTackle, LeapBite, Landing) ----
2.8e fconstant pounce-speed
: forward ( -- )  0e pounce-speed body-move-local  1 his-no-root ! ;
: floor-y ( F: -- y )  his-at v.x sf@  his-pounce-top f@  his-at v.z sf@  nav-floor drop ;   \ (the floor under him, from the leap's top)
: st-landing ( -- )
    anim@ land-anim <> if  settled? if  0 his-2d !  land-anim play  then  forward exit  then
    anim-done? if  0 his-2d !  to-default exit  then
    his-t2 @ if  -1 his-t2 +!  forward  else  root-move  then ;   \ (its feet down: its own motion)
: land ( -- )
    floor-y his-y!  settled? if  land-anim play  then
    let-in  4 his-t2 !  ['] st-landing behave ;
: st-leap-bite ( -- )   \ (past its top: falling, 0.5 faster a frame)
    tackle-bite  forward
    his-pounce-y f@ his-pounce-vy f@ f+ fdup his-pounce-y f!  his-y!  his-pounce-vy f@ 0.5e f- his-pounce-vy f!
    his-at v.y sf@ floor-y f< if  land  then ;
: st-tackle ( -- )   \ (rising to its top, eased: a quarter sine over the frames to it)
    tackle-bite
    his-yaw his-pounce-yaw f@ 10e deg>rad turn-toward fdrop his-yaw!  forward
    his-t1 @ his-t2 @ >= if
        his-pounce-top f@ his-y!  his-pounce-top f@ his-pounce-y f!  0e his-pounce-vy f!  ['] st-leap-bite behave exit
    then
    his-pounce-top f@ his-pounce-y f@ f-  his-t1 @ s>f his-t2 @ s>f f/ pi f* 0.5e f* fsin f*  his-pounce-y f@ f+  his-y!
    1 his-t1 +! ;
: pounce ( F: rest -- )   \ (at his target, `rest` along his way to it)
    his-spot his-target @ body-at vec-copy
    his-target @ body-at v.y sf@ 5e f+ his-pounce-top f!
    1 his-t1 !  pounce-speed f/ f>s 1+ his-t2 !  0 his-t3 !
    his-at v.y sf@ his-pounce-y f!
    his-target @ body-at heading-to fdup his-pounce-yaw f!  his-yaw fswap 30e deg>rad turn-toward fdrop his-yaw!
    ahead-look look!  tackle-anim play  shut-out  1 his-2d !  leap-snarl make-sound  new-bite
    ['] st-tackle behave ;

: on-its-tri? ( -- flag )  his-target @ body-at -1 body-tri-to  his-target @ body-tri = ;

\ ---- rush-act: close in and rush it, biting what his mouth reaches (Hewie_StateCloseIn, StateRush) ----
: st-bitten ( -- )  to-default ;   \ (the bite struck: back to himself - Hewie_State1980)
: st-rush ( -- )
    his-target @ with? 0= if  to-default exit  then
    -1 his-t1 +!  his-t1 @ 0= if  to-default exit  then
    on-its-tri? 0= path-done? and if  his-target @ body-tri his-target @ body-at plan-to 0= if  to-default exit  then  then
    as-is-pose step-to-pose 0= if  run-anim play-if-not  then
    on-its-tri? if  his-target @ body-at run-straight  else  stride 0= if  trot-pose keep-pose  then  then   \ (there: straight at it)
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  exit  then
    mouth!
    pursuer reaches?  her reaches? or if
        leap-snarl make-sound
        pursuer reaches? if  shove-blow 5 bite-damage  rush-bite hard-bite? if $8000 else 0 then  pursuer send blow  then
        her reaches? if  blow-request 0 20e f>cell her send hit  then
        ['] st-bitten behave exit
    then
    5 bite-creatures ;
: close-in ( -- )
    his-target @ dup with? 0= if  drop to-default exit  then
    mode-of dup unreachable-mode = swap struck-mode = or if  to-default exit  then
    90 his-t1 !  engaged-mode his-mode !  on-command his-cmd !  his-path path-clear  new-bite  ['] st-rush behave ;

\ ---- in harrying (st-close-on): within 30 - on a step, close in (rush-act); on open floor,
\ facing it (60 degrees) and on its triangle, the pounce ----
: step-floor? ( tri -- flag )  dup 0< if  drop false exit  then  nav-flags slope-floor and 0<> ;
: facing-it? ( -- flag )  his-target @ body-at heading-to his-yaw f- angle-wrap fabs 60e deg>rad f< ;
:noname ( F: rest -- done? )
    his-tri step-floor?  his-target @ body-tri step-floor? or if  fdrop  rush-act 0 want  true exit  then
    facing-it? on-its-tri? and if  pounce  true exit  then
    fdrop false ; is close-by
