\ stalker/chase.fs - a stalker after Fiona in her room, seeing her (src/game/debilitas.c
\ Debilitas_Behaviour / Debilitas_Chase, and the Pursuer's moves under them): he walks at her
\ ($200) or stalks her ($201) for a while by his chase table and whether she faces him, stops
\ to watch her when she is near and facing him, turns on the spot to keep her in front, now and
\ then taunts her (by the chance for how near she is), steps round her; within reach, facing
\ her on her floor, he attacks - his table for how she stands (`fiona-plight`): a combo
\ (stalker/attack.fs), a taunt, a step round her. Losing sight of her he goes to where she is.
IN: stalker.chase
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack ;

: close-reach ( F: -- d )  hard? if  40e  else  50e  then ;   \ (+0x17E4)
: frames# ( off -- n )  2 rshift cells chase-frames + @ ;     \ (his chase table by its offset)
: chance# ( i -- ) ( F: -- c )  floats chase-chances + f@ ;
: d-her ( F: -- d )  d-fiona f@ ;
\ she faces him (Debilitas_Seen: Eye_ActorSees - him within his sight's range of her, 90
\ degrees either side of her heading)
: faces-me? ( -- flag )
    fiona body-at my-at vec-dist view-range f@ f> if  false exit  then
    fiona body-at my-at vec-heading fiona body-yaw f- angle-wrap fabs pi f2/ f<= ;

\ ---- what he does (the actions) ----
: act! ( xt id -- )  chase-act !  act-xt !  0 step-done !  0 step-t ! ;
: watching ( -- )  freeze-t @ 0= if  root-move  then ;      \ (Pursuer_Move180)
: stand-watch ( -- )  0 play  ['] watching 1 act! ;           \ 1: standing (Pursuer_Move17C)
' stand-watch is to-stand
\ walking or stalking at her (Pursuer_CloseIn + Debilitas_ChaseTarget): his way to her re-planned
\ every 15 frames; straight at her once nothing is in the way
: at-her ( -- )
    -1 step-t +!  step-t @ 0> 0= if
        15 step-t !  fiona body-tri dup 0< if  drop  else  fiona body-at plan drop  then
    then
    fiona self body-mask straight? if  fiona body-at step-toward exit  then
    walk-stride drop ;
: walk-at ( id -- )   \ 5: walking ($200), 6: stalking ($201)
    dup 5 = if  $200  else  $201  then  play  ['] at-her swap act! ;
: keep ( id frames -- )  hold-t !  walk-at ;
\ 3: turned on the spot toward her
: turning-to ( -- )  root-move  ended? if  -1 step-done !  then ;
: turn-to-her ( way -- )  $400 + play-now  ['] turning-to 3 act! ;
\ a gesture (a taunt: his gesture table) played to its end
: gesturing-at ( -- )  freeze-t @ 0= if  root-move  then  ended? if  -1 step-done !  then ;
: gesture! ( n -- )
    2* cells gestures + @  dup 1 = if  drop stand-watch exit  then
    play-now  ['] gesturing-at $17 act! ;
\ $1A: round her (Pursuer_StateSidestepRoom / StateSidestep): at his distance, 5 degrees a step
\ the way she isn't facing; the other way when it's blocked; done after 30 of it or 120 frames
create rd-at 12 allot
: round-point ( -- )
    my-at fiona body-at vec-dist-xz
    fiona body-at my-at vec-heading  round-dir @ s>f 0.08726647e f* f+  fswap
    rd-at fiona body-at vec-ahead ;
: round-ok? ( -- flag )
    rd-at self body-mask v-tri-in dup 0< if  drop false exit  then
    rd-at fiona body-at 0 v-walk fiona body-tri = ;
create rd-was 12 allot
: rounding ( -- )
    1 step-t +!  step-t @ 120 > if  -1 step-done !  exit  then
    round-point  round-ok? 0= if  round-dir @ negate round-dir !  round-point  round-ok? 0= if  -1 step-done !  exit  then  then
    rd-was my-at vec-copy  rd-at step-toward
    rd-was my-at vec-dist-xz round-gone f@ f+ fdup round-gone f!  30e f> if  -1 step-done !  then ;
: round-her ( -- )
    fiona body-at my-at vec-heading fiona body-yaw f- angle-wrap f0> if  1  else  -1  then  round-dir !
    0e round-gone f!  $200 play  ['] rounding $1A act! ;

\ ---- his tables (Pursuer_PickFromTable): a roll against the rows' running percentages ----
defer pick-attack ( -- )
: row ( table -- addr )   \ (the row the roll falls in)
    rnd 100e f*  begin  dup 2 cells + f@ fover f< while  /row +  repeat  fdrop ;
: do-row ( kind arg -- )
    swap case
        1 of  drop  endof                                    \ (nothing new)
        $13 of  combo-start  endof
        $17 of  gesture!  endof   $18 of  gesture!  endof   $19 of  gesture!  endof   2 of  gesture!  endof
        $1A of  drop round-her  endof   $1B of  drop round-her  endof
        \ $14 / $15: his hand on her (led away) when her floor is free for him, else his attack
        \ tables 6 / 7 (Pursuer_PickAttack) - the hand with the grabs (P2b), the tables meanwhile
        $14 of  drop pick-attack  endof   $15 of  drop pick-attack  endof
        \ $1003 his grab, $1002 his lunge: with the grabs (P2b)
        >r drop stand-watch r>
    endcase ;
: pick ( situation -- )  hard? table row  dup @ swap cell+ @ do-row ;
\ Pursuer_PickAttack: his tables 6 / 7 (the panic's stage 4 on)
:noname ( -- )  her-stage @ 4 >= if  7  else  6  then  pick ; is pick-attack

\ ---- his chase's choices (Debilitas_BackOrHold, Debilitas_StrikeOrWait) ----
: back-or-hold ( i -- ) ( F: roll -- )   \ walk at her or stalk her, for his table's frames
    dup chance# f<= if  5 swap 8 * frames# keep  else  6 swap 8 * 4 + frames# keep  then ;
: lunge-chance ( -- n )   \ (Pursuer_ThresholdEntry: by how near she is)
    3 0 do  d-her  i 2* cells lunge-chances + f@ f<= if  i 2* 1+ cells lunge-chances + @ unloop exit  then  loop  0 ;
: strike-or-wait ( -- )
    rnd 100e f*  lunge-chance s>f f< if  10 pick exit  then
    rnd 100e f*  faces-me? 0= if  d-her close-reach f<= 1 and  else  2  then  back-or-hold ;

\ ---- the chase's step each frame (Debilitas_Chase) by what he is doing ----
: standing-choice ( -- )   \ 1
    chase-t @ 90 mod 0= chase-t @ 0> and if
        rnd 100e f* lunge-chance s>f f<= if  10 pick exit  then
    then
    settled? if  fiona body-at 1.0471976e 2.6179939e way-past dup $FF <> if  turn-to-her exit  then  drop  then
    rnd 100e f*
    faces-me? 0= if  1 back-or-hold exit  then
    d-her close-reach 20e f+ f> if  2 back-or-hold exit  then
    fdrop  watch-t @ if  -1 watch-t +!  else  5 $18 frames# keep  then ;
: near-and-facing? ( -- flag )  d-her close-reach f<= faces-me? and ;
: stalking-choice ( -- )   \ 6
    near-and-facing? if
        watch-t @ if  stand-watch  $20 frames# watch-t !  0 hold-t !  else  5 $18 frames# keep  then  exit
    then
    hold-t @ 0>  d-her 100e f< 0= or if  -1 hold-t +!  exit  then
    strike-or-wait ;
: walking-choice ( -- )   \ 5
    near-and-facing? watch-t @ 0> and if  stand-watch  $20 frames# watch-t !  0 hold-t !  exit  then
    hold-t @ 0> if
        d-her 100e f<= if  -1 hold-t +!  else  6 $1C frames# keep  then  exit
    then
    strike-or-wait ;
\ within reach, facing her on her floor: his attack (his table for how she stands)
: same-floor? ( -- flag )
    fiona body-at 4 + sf@  my-at 4 + sf@  fover fover 15e f+ f<=  10e f- f>= and ;
: in-reach? ( -- flag )
    hold-off @ if  false exit  then
    d-her fdup 24e f< 0=  fdup f0< or if  fdrop false exit  then
    fdup ground-gained f<  10e f< or 0= if  false exit  then
    fiona body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f<  same-floor? and ;
: attack! ( -- )
    her-plight @ dup 3 = if  drop exit  then   \ (out of reach: struck already, hidden)
    dup 0 6 within 0= if  drop 0  then  pick ;
: chase-run ( -- )
    1 chase-t +!
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    chase-act @ case
        1 of  standing-choice  endof
        5 of  walking-choice  endof
        6 of  stalking-choice  endof
        $13 of  step-done @ if  took @ if  12  else  13  then  pick  0 took !  exit  then  endof
        >r  step-done @ if  stand-watch  then  r>
    endcase
    chase-act @ dup 1 = over 5 = or swap 6 = or if  in-reach? if  attack!  then  then ;

\ ---- the chase begins (Debilitas_Behaviour): she facing him or not, near or far ----
: chase-now ( -- )
    0 search-phase !  ['] chase-run step!  0 chase-t !  0 hold-off !  -1 combo !
    $20 frames# watch-t !
    rnd 100e f*
    faces-me? 0= if
        d-her close-reach f> if  0 chance# f<= if  5 0 frames# keep  else  6 4 frames# keep  then
        else  8 frames# s>f f<= if  5 8 frames# keep  else  6 $C frames# keep  then  then
    else
        d-her close-reach f> if  2 chance# f<= if  5 $10 frames# keep  else  6 $14 frames# keep  then
        else  fdrop stand-watch  0 hold-t !  then
    then ;
' chase-now is chase-begin
:noname ( -- flag )  step @ ['] chase-run = ; is chasing?
:noname ( -- )   \ (lost sight of her - not in the middle of a combo)
    chase-act @ $13 = combo @ 0< 0= and if  exit  then  chase-start ; is lost-her
