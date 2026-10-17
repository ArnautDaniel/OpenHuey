\ stalker/chase.fs - a stalker after Fiona in her room, seeing her (src/game/debilitas.c
\ Debilitas_Behaviour / Debilitas_Chase, and the Pursuer's moves under them): he walks at her
\ or stalks her for a while by his chase table and whether she faces him, stops to watch her
\ when she is near and facing him, turns on the spot to keep her in front, now and then taunts
\ her (by the chance for how near she is), steps round her; within reach, facing her on her
\ floor, he attacks - his table for how she stands (`fiona-plight`): a combo (stalker/attack.fs),
\ a taunt, a step round her. Losing sight of her he goes to where she is.
IN: stalker.chase
USING: engine game-state actors messages common facts flag-names paths vectors stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.head stalker.names stalker.anims ;

: close-reach ( F: -- d )  hard-mode? if  40e  else  50e  then ;   \ (+0x17E4)
\ his chase table: frames walking at her and stalking her by how he finds her - not facing him
\ far, near; facing him far; standing near - then frames watching her; and the chances to walk
\ (rather than stalk) for the first three
0 constant unseen-far   1 constant unseen-near   2 constant seen-far   3 constant seen-near
: walk-frames ( i -- n )  2* cells chase-frames + @ ;
: stalk-frames ( i -- n )  2* 1+ cells chase-frames + @ ;
: watch-frames ( -- n )  8 cells chase-frames + @ ;
: chance# ( i -- ) ( F: -- c )  floats chase-chances + f@ ;
: d-her ( F: -- d )  d-fiona f@ ;
\ she faces him (Debilitas_Seen: Eye_ActorSees - him within his sight's range of her, 90
\ degrees either side of her heading)
: faces-me? ( -- flag )
    fiona body-at my-at vec-dist view-range f@ f> if  false exit  then
    fiona body-at my-at vec-heading fiona body-yaw f- angle-wrap fabs pi f2/ f<= ;

\ ---- what he does (the actions) ----
: let-her-go ( -- )  leading @ if  0 leading !  fiona send unhand  then ;
: act! ( xt act -- )  let-her-go  dup action-look!  chase-act !  act-xt !  0 step-done !  0 step-t ! ;
: watching ( -- )  freeze-t @ 0= if  root-move  then ;      \ (Pursuer_Move180)
: stand-watch ( -- )  still-anim play  ['] watching watch-act act! ;   \ (Pursuer_Move17C)
' stand-watch is to-stand
\ walking or stalking at her (Pursuer_CloseIn + Debilitas_ChaseTarget): his way to her re-planned
\ every 15 frames; straight at her once nothing is in the way
: replan ( -- )  15 step-t !  fiona body-tri dup 0< if  drop  else  fiona body-at plan 0> 0= no-way !  then ;
: at-her ( -- )
    -1 step-t +!  step-t @ 0< {: set-off :}  step-t @ 0> 0= if  replan  then
    no-way @  set-off base-chase @ or  and if  out-of-reach if  exit  then  then   \ (no way to her: round by a ladder -
    \ Pursuer_CloseIn as he sets off, the Pursuer's ChaseTarget all the way)
    fiona self body-mask straight? if  fiona body-at step-toward exit  then
    walk-stride drop ;
: walk-at ( act -- )   \ walking at her (fast) or stalking her (slow)
    0 no-way !  dup walk-at-act = if  walk-fast  else  walk-slow  then  play  ['] at-her swap act! ;
: keep ( act frames -- )  hold-t !  walk-at ;
\ turned on the spot toward her
: turning-to ( -- )  root-move  ended? if  -1 step-done !  then ;
: turn-to-her ( way -- )  turn-base + play-now  ['] turning-to turn-act act! ;
\ a gesture (a taunt: his gesture table) played to its end
: gesturing-at ( -- )  freeze-t @ 0= if  root-move  then  ended? if  -1 step-done !  then ;
: gesture! ( n -- )
    2* cells gestures + @  dup 1 = if  drop stand-watch exit  then
    play-now  ['] gesturing-at taunt-act act! ;
\ round her (Pursuer_StateSidestepRoom / StateSidestep): at his distance, 5 degrees a step the
\ way she isn't facing; the other way when it's blocked; done after 30 of it or 120 frames
create rd-at /vec allot
: round-point ( -- )
    my-at fiona body-at vec-dist-xz
    fiona body-at my-at vec-heading  round-dir @ s>f 5e deg>rad f* f+  fswap
    rd-at fiona body-at vec-ahead ;
: round-ok? ( -- flag )
    rd-at self body-mask v-tri-in dup 0< if  drop false exit  then
    rd-at fiona body-at 0 v-walk fiona body-tri = ;
create rd-was /vec allot
: rounding ( -- )
    1 step-t +!  step-t @ 120 > if  -1 step-done !  exit  then
    round-point  round-ok? 0= if  round-dir @ negate round-dir !  round-point  round-ok? 0= if  -1 step-done !  exit  then  then
    rd-was my-at vec-copy  rd-at step-toward
    rd-was my-at vec-dist-xz round-gone f@ f+ fdup round-gone f!  30e f> if  -1 step-done !  then ;
: round-her ( -- )
    fiona body-at my-at vec-heading fiona body-yaw f- angle-wrap f0> if  1  else  -1  then  round-dir !
    0e round-gone f!  walk-fast play  ['] rounding round-act act! ;

\ ---- his tables (Pursuer_PickFromTable): a roll against the rows' running percentages ----
defer pick-attack ( -- )
defer take-her ( type -- )   defer lunge ( -- )   defer grab-dog ( -- )   \ (stalker.grab)
: row ( table -- addr )   \ (the row the roll falls in)
    rnd 100e f*  begin  dup r.upto f@ fover f< while  /row +  repeat  fdrop ;
\ ---- Debilitas's own: his wander and his turn to her ----
\ touching whom (Npc_WhoSeen: within 1 of his body)
1 constant touched-her   2 constant touched-him
: touching-bits ( -- bits )
    0  fiona here? if  self fiona 1e 0e bodies-touching? touched-her and or  then
       hewie here? if  self hewie 1e 0e bodies-touching? touched-him and or  then ;
\ Debilitas_StartWander / StateLookWalk: a triangle at random on his own floor's level (not
\ blocked to him; not both sides of a divider) at least 20 away on foot, walked to in his slow
\ walk for at most 60 frames; whom he bumps into on the way a light blow (Pursuer_RaiseThreat:
\ kind 4, damage 5, threat 10). None to go to: nothing new
create wd-at /vec allot
: my-level ( -- bits )  self body-tri nav-flags divider-sides and ;
: wander-tri ( -- tri | -1 )
    {: | t :}
    nav-tris 0 ?do
        rnd nav-tris s>f f* f>s  nav-tris 1- min  to t
        t nav-flags  dup self body-mask and 0=  over divider-sides and my-level = and
        swap divider-sides and divider-sides <> and if
            t tri-center wd-at vec!  t wd-at plan 0> if
                my-path my-at path-rest 20e f< 0= if  t unloop exit  then
            then
        then
    loop  -1 ;
: bump ( -- )   \ (Pursuer_RaiseThreat)
    touching-bits sent @ invert and
    dup touched-her and if  throw-request 0 10e f>cell fiona send hit  then
    touched-him and if  hard-blow 5 0 hewie send blow  then  touching-bits sent @ or sent ! ;
: wandering ( -- )
    bump  -1 step-t +!
    walk-stride  step-t @ 0> 0= or if  -1 step-done !  then ;
: wander ( -- )
    wander-tri dup 0< if  drop stand-watch exit  then
    goal-tri !  0 sent !  walk-slow play  ['] wandering own-act-1 act!  60 step-t ! ;
\ Debilitas_StartTurnToFiona / StateTurnToFiona / StateBlow / StateStun: unshaken, turned to her
\ at his rate; at its end his charge - his move 4's bone: into a wall, a stun; whom it reaches
\ (move 4's reach) or touches struck by move 4 (a stumble by its chance); then the stun to its end
$1304 constant turn-in-anim   $205 constant charge-anim   $1004 constant stun-anim   \ (Debilitas's)
: move4 ( -- addr )  4 move-of ;
: stunned ( -- )   \ (Debilitas_StateStun)
    freeze-t @ 0= if  root-move  then  ended? if  0 unshaken !  -1 step-done !  then ;
: stun ( -- )  stun-anim play-now  ['] stunned act-xt ! ;
: charge-hits ( -- bits )   \ (whom it reaches or touches, not struck yet)
    0  blow-at move4 m.reach f@ reaches? touched-her and or  blow-at move4 m.reach f@ reaches-dog? touched-him and or
    touching-bits or  sent @ invert and ;
: charging ( -- )
    my-model @ move4 m.bone @ bone-pos blow-at vec!
    blow-at walled? if  stun exit  then
    charge-hits ?dup 0= if  root-move exit  then
    move4 m.stumble f@ stumble-roll {: bits how :}
    bits touched-her and if  move4 m.kind @ how move4 m.threat f@ f>cell fiona send hit  then
    bits touched-him and if  move4 m.kind @ move4 m.damage @ how hewie send blow  then
    -1 sent !  stun ;
: turning-in ( -- )
    root-move  fiona body-at turn-rate turn-to fdrop
    ended? if  charge-anim play-now  ['] charging act-xt !  then ;
: turn-at-her ( -- )  turn-in-anim play-now  -1 unshaken !  0 sent !  ['] turning-in own-act-2 act! ;
\ his own actions (own-act-1 .. of his tables): his `own-acts` table, else the family's
\ (Debilitas's: his wander, his turn to her, his lunge, his grab at Hewie)
create family-acts  ' wander , ' turn-at-her , ' lunge , ' grab-dog ,
: own-act ( arg act -- )
    nip own-act-1 -  own-acts @ ?dup 0= if  family-acts  then  swap cells + @ execute ;
: own? ( act -- flag )  own-act-1 own-act-1 $10 + within ;
: do-row ( act arg -- )
    over own? if  swap own-act exit  then
    swap case
        watch-act of  drop  endof                                    \ (nothing new)
        combo-act of  combo-start  endof
        taunt-act of  gesture!  endof   taunt-2-act of  gesture!  endof   taunt-3-act of  gesture!  endof   gesture-act of  gesture!  endof
        round-act of  drop round-her  endof   round-2-act of  drop round-her  endof
        take-by-hand-act of  drop by-the-hand take-her  endof   take-walking-act of  drop walked-off take-her  endof
        >r drop stand-watch r>
    endcase ;
: choose ( situation -- )  hard-mode? table row  dup r.act @ swap r.arg @ do-row ;   \ (Pursuer_PickFromTable)
\ Pursuer_PickAttack: by the panic's stage
:noname ( -- )  her-stage @ panicking >= if  attack-panic  else  attack-calm  then  choose ; is pick-attack

\ ---- his chase's choices (Debilitas_BackOrHold, Debilitas_StrikeOrWait) ----
: back-or-hold ( i -- ) ( F: roll -- )   \ walk at her or stalk her, for his table's frames
    dup chance# f<= if  walk-at-act swap walk-frames keep  else  stalk-act swap stalk-frames keep  then ;
: lunge-chance ( -- n )   \ (Pursuer_ThresholdEntry: by how near she is)
    3 0 do  d-her  i 2* cells lunge-chances + f@ f<= if  i 2* 1+ cells lunge-chances + @ unloop exit  then  loop  0 ;
: strike-or-wait ( -- )
    rnd 100e f*  lunge-chance s>f f< if  lunge-situation choose exit  then
    rnd 100e f*  faces-me? 0= if  d-her close-reach f<= if  unseen-near  else  unseen-far  then  else  seen-far  then  back-or-hold ;

\ ---- the chase's step each frame (Debilitas_Chase) by what he is doing ----
: standing-choice ( -- )
    chase-t @ 90 mod 0= chase-t @ 0> and if
        rnd 100e f* lunge-chance s>f f<= if  lunge-situation choose exit  then
    then
    settled? if  fiona body-at 60e deg>rad 150e deg>rad way-past dup no-turn <> if  turn-to-her  else  drop  then  then
    rnd 100e f*
    d-her f0< if  fdrop  out-of-reach 0= if  search-again  then  exit  then   \ (no way to her: Pursuer_ChaseFionaHere)
    faces-me? 0= if  unseen-near back-or-hold exit  then
    d-her close-reach 20e f+ f> if  seen-far back-or-hold exit  then
    fdrop  watch-t @ if  -1 watch-t +!  else  walk-at-act seen-near walk-frames keep  then ;
: near-and-facing? ( -- flag )  d-her close-reach f<= faces-me? and ;
: watch-her ( -- )  stand-watch  watch-frames watch-t !  0 hold-t ! ;
: stalking-choice ( -- )
    near-and-facing? if
        watch-t @ if  watch-her  else  walk-at-act seen-near walk-frames keep  then  exit
    then
    hold-t @ 0>  d-her 100e f< 0= or if  -1 hold-t +!  exit  then
    strike-or-wait ;
: walking-choice ( -- )
    near-and-facing? watch-t @ 0> and if  watch-her exit  then
    hold-t @ 0> if
        d-her 100e f<= if  -1 hold-t +!  else  stalk-act seen-near stalk-frames keep  then  exit
    then
    strike-or-wait ;
\ within reach, facing her on her floor: his attack (his table for how she stands)
: same-floor? ( -- flag )
    fiona body-at v.y sf@  my-at v.y sf@  {: F: hy my :}  hy my 15e f+ f<=  hy my 10e f- f>= and ;
: range ( F: -- r )  attack-range f@ fdup f0= if  fdrop 24e  then ;   \ (vtable +0x2F4)
: in-reach? ( -- flag )
    hold-off @ if  false exit  then
    d-her fdup range f< 0=  fdup f0< or if  fdrop false exit  then
    fdup ground-gained f<  10e f< or 0= if  false exit  then
    fiona body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f<  same-floor? and ;
: attack! ( -- )
    her-plight @ dup unreachable-plight = if  drop exit  then   \ (out of reach: struck already, hidden)
    dup free-plight calming-plight 1+ within 0= if  drop free-plight  then  choose ;
: moving-act? ( -- flag )  chase-act @ dup watch-act =  over walk-at-act = or  swap stalk-act = or ;
: chase-run ( -- )
    1 chase-t +!
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    chase-act @ case
        watch-act of  standing-choice  endof
        walk-at-act of  walking-choice  endof
        stalk-act of  stalking-choice  endof
        combo-act of  step-done @ if  took @ if  struck-her  else  missed-her  then  choose  0 took !  exit  then  endof
        >r  step-done @ if  stand-watch  then  r>
    endcase
    moving-act? if  in-reach? if  attack!  then  then ;

\ ---- the chase begins (Debilitas_Behaviour): she facing him or not, near or far ----
: chase-now ( -- )
    no-search search-phase !  ['] chase-run step!  0 chase-t !  0 hold-off !  -1 combo !
    watch-frames watch-t !
    rnd 100e f*
    faces-me? 0= if
        d-her close-reach f> if  unseen-far chance# f<= if  walk-at-act unseen-far walk-frames keep  else  stalk-act unseen-far stalk-frames keep  then
        else   \ (the original takes the walking frames here for the chance)
            unseen-near walk-frames s>f f<= if  walk-at-act unseen-near walk-frames keep  else  stalk-act unseen-near stalk-frames keep  then
        then
    else
        d-her close-reach f> if  seen-far chance# f<= if  walk-at-act seen-far walk-frames keep  else  stalk-act seen-far stalk-frames keep  then
        else  fdrop stand-watch  0 hold-t !  then
    then ;
' chase-now is chase-begin
:noname ( -- flag )  step @ ['] chase-run = ; is chasing?
:noname ( -- )   \ (lost sight of her - not in the middle of a combo)
    chase-act @ combo-act = combo @ 0< 0= and if  exit  then  chase-start ; is lost-her
