\ hewie/pounce.fs - his pounce and his rush (Hewie_StateCloseOnTarget's leap, Hewie_StateTackle,
\ StateLeapBite, StateLanding; Hewie_StateCloseIn, StateRush - action 0x6D): close on his target
\ on open floor and facing it, he leaps at it - 2.8 a frame, up to 5 above it and down again - and
\ whoever his mouth (bone $1F) reaches on the way takes it: the stalker a blow (5 from in front,
\ 8 from behind; charged 10 / 16; doubled on hard, half again with Sylvesta Esca's boost - hard by
\ chance, the more hurt he is the likelier), else Fiona; the homunculi it reaches are bitten.
\ On a step or by a ladder (floor flag 1) he can't leap: he runs at it and bites (0x6D).
IN: hewie.pounce
USING: engine game-state actors messages common facts paths flag-names hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states hewie.fight ;

\ ---- whom his mouth reaches (Progress_CharNear: bone $1F within 3 of them) ----
create mouth 12 allot
: mouth! ( -- )  model $1F bone-pos mouth vec! ;
: reaches? ( id -- flag )
    dup 0< if  drop false exit  then  dup active? 0= if  drop false exit  then
    mouth 4 + sf@  dup body-at 4 + sf@ 3e f-  f> 0= if  drop false exit  then
    mouth 4 + sf@  dup body-at 4 + sf@  dup body-dims fswap fdrop f+ 3e f+  f< 0= if  drop false exit  then
    mouth over body-at vec-dist-xz  body-dims fdrop 3e f+  f< ;
: behind-of? ( id -- flag )   \ (his heading within 90 degrees of theirs: at their back)
    body-yaw his-yaw f- angle-wrap fabs pi f2/ f< ;
\ the damage: from in front / behind, charged (his-ready); hard mode doubles, the boost half again
: tackle-damage ( behind? -- n )
    his-ready @ if  if 16 else 10 then  else  if 8 else 5 then  then
    black-shepherd? if  2*  then
    progress pr.boosts 6 8 * + 4 + sl@ 0> if  dup 2/ +  then ;
variable hewie-blow-n   -2000 hewie-blow-n !
: new-bite ( -- )  -1 hewie-blow-n +! ;   \ (a rush's or a leap's: each homunculus bitten once in it)
: bite-creatures ( damage -- )   \ (Hewie_Bite: the homunculi his mouth reaches - as a blow there)
    >r hewie-blow-n @ r>  mouth sf@ f>cell  mouth 4 + sf@ f>cell  mouth 8 + sf@ f>cell  10e f>cell   \ (within 10 of its body)
    broadcast fiona-blow ;
\ the tackle's bite, once a leap: the stalker, else Fiona (her `hit` - knocked down)
: tackle-bite ( -- )
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  exit  then
    mouth!
    his-t3 @ 4 and 0=  pursuer reaches? and if
        pursuer behind-of?  dup tackle-damage                       ( behind? dmg )
        swap if  his-ready @ if 9 else 7 then  else  his-ready @ if 8 else 6 then  then
        hard-bite? if  $8000  else  0  then                         ( dmg how )
        his-ready @ if  2  else  1  then  -rot  pursuer send blow
        his-t3 @ 4 or his-t3 !  $6C make-sound
    else
        his-t3 @ 1 and 0=  her reaches? and if
            her behind-of? tackle-damage drop
            his-ready @ if  2  else  1  then  0 20e f>cell her send hit
            his-t3 @ 1 or his-t3 !
        then
    then
    his-ready @ if  10  else  5  then  bite-creatures ;

\ ---- the leap (Hewie_StateTackle, LeapBite, Landing) ----
fvariable lp-speed  fvariable lp-y0  fvariable lp-top  fvariable lp-vy  fvariable lp-yaw
: forward ( -- )  0e lp-speed f@ body-move-local  1 his-no-root ! ;
: floor-y ( F: -- y )  his-at sf@  lp-top f@  his-at 8 + sf@  nav-floor drop ;   \ (the floor under him, from the leap's top)
: st-landing ( -- )
    anim@ $1E03 <> if  settled? if  0 his-2d !  $1E03 play  then  forward exit  then
    anim-done? if  0 his-2d !  to-default exit  then
    his-t2 @ if  -1 his-t2 +!  forward  else  root-move  then ;   \ (its feet down: its own motion)
: land ( -- )
    floor-y his-y!  settled? if  $1E03 play  then
    his-cmd @ $7FFFFFFF and his-cmd !  4 his-t2 !  ['] st-landing behave ;
: st-leap-bite ( -- )   \ (past its top: falling, 0.5 faster a frame)
    tackle-bite  forward
    lp-y0 f@ lp-vy f@ f+ fdup lp-y0 f!  his-y!  lp-vy f@ 0.5e f- lp-vy f!
    his-at 4 + sf@ floor-y f< if  land  then ;
: st-tackle ( -- )   \ (rising to its top, eased: a quarter sine over the frames to it)
    tackle-bite
    his-yaw lp-yaw f@ 10e deg>rad turn-toward fdrop his-yaw!  forward
    his-t1 @ his-t2 @ >= if
        lp-top f@ his-y!  lp-top f@ lp-y0 f!  0e lp-vy f!  ['] st-leap-bite behave exit
    then
    lp-top f@ lp-y0 f@ f-  his-t1 @ s>f his-t2 @ s>f f/ pi f* 0.5e f* fsin f*  lp-y0 f@ f+  his-y!
    1 his-t1 +! ;
: pounce ( F: rest -- )   \ (at his target, `rest` along his way to it)
    his-spot his-target @ body-at vec-copy
    his-target @ body-at 4 + sf@ 5e f+ lp-top f!
    1 his-t1 !  2.8e f/ f>s 1+ his-t2 !  0 his-t3 !
    2.8e lp-speed f!  his-at 4 + sf@ lp-y0 f!
    his-target @ body-at heading-to fdup lp-yaw f!  his-yaw fswap 30e deg>rad turn-toward fdrop his-yaw!
    4 look!  $1E01 play  his-cmd @ $80000000 or his-cmd !  1 his-2d !  $68 make-sound  new-bite
    ['] st-tackle behave ;

: on-its-tri? ( -- flag )  his-target @ body-at -1 body-tri-to  his-target @ body-tri = ;

\ ---- 0x6D: close in and rush it, biting what his mouth reaches (Hewie_StateCloseIn, StateRush) ----
: st-1980 ( -- )  to-default ;   \ (the bite struck: back to himself)
: st-rush ( -- )
    his-target @ with? 0= if  to-default exit  then
    -1 his-t1 +!  his-t1 @ 0= if  to-default exit  then
    on-its-tri? 0= path-done? and if  his-target @ body-tri his-target @ body-at plan-to 0= if  to-default exit  then  then
    5 step-to-pose 0= if  $202 play-if-not  then
    on-its-tri? if  his-target @ body-at run-straight  else  stride 0= if  8 keep-pose  then  then   \ (there: straight at it)
    hewie-no-attack state-flag? fiona-occupied state-flag? or if  exit  then
    mouth!
    pursuer reaches?  her reaches? or if
        $68 make-sound
        pursuer reaches? if  1 5 black-shepherd? if 2* then  10 hard-bite? if $8000 else 0 then  pursuer send blow  then
        her reaches? if  1 0 20e f>cell her send hit  then
        ['] st-1980 behave exit
    then
    5 bite-creatures ;
: close-in ( -- )   \ (0x6D)
    his-target @ dup with? 0= if  drop to-default exit  then
    mode-of dup 3 = swap 4 = or if  to-default exit  then
    $5A his-t1 !  8 his-mode !  8 his-cmd !  his-path path-clear  new-bite  ['] st-rush behave ;

\ ---- in harrying (st-close-on): within 30 - on a step, close in (0x6D); on open floor, facing it
\ (60 degrees) and on its triangle, the pounce ----
: step-floor? ( tri -- flag )  dup 0< if  drop false exit  then  nav-flags 1 and 0<> ;
: facing-it? ( -- flag )  his-target @ body-at heading-to his-yaw f- angle-wrap fabs 60e deg>rad f< ;
:noname ( F: rest -- done? )
    his-tri step-floor?  his-target @ body-tri step-floor? or if  fdrop  $6D 0 want  true exit  then
    facing-it? on-its-tri? and if  pounce  true exit  then
    fdrop false ; is close-by
