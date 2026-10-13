\ stalker/pursue.fs - the Pursuer's own chase (src/game/pursuer.c Pursuer_ChaseDecision,
\ Pursuer_BehaviourStalk, Pursuer_HoldOff; Pursuer_StateWalkThen404), the one stalkers without
\ their own use (Daniella; Debilitas has his: stalker/chase.fs). Seeing her: close and in front
\ of him, not panicking past stage 3 - he stands to her; else by his chase table's row for her
\ panic's stage (`pursue-row`: the chance to walk at her, frames walking, frames stalking) he
\ walks at her ($200) or stalks her ($201) a while. Walking, his time out or her past 100: he
\ stalks. Stalking, his time out within 100: his table 10. Touching her: a step back facing her
\ ($404), and again by the row once she is 5 further off. Standing: turning to her; out of the
\ ground he gains, or not in front of him (20 degrees): by the row again. A combo, a taunt or
\ his way round her over: by the row again. Within reach in front of him on her floor: his
\ attack (his table for how she stands).
IN: stalker.pursue
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase ;

: touch ( F: -- d )  self body-dims fdrop  fiona body-dims fdrop f+ ;   \ (their radii)
: in-front? ( -- flag )  fiona body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f< ;
\ Pursuer_HoldOff: walking at her or stalking her, for the row's frames
: pick-pace ( -- )
    her-stage @ pursue-row
    rnd 100e f*  dup f@ f<= if  5 swap cell+ @  else  6 swap 2 cells + @  then  keep ;
\ $1D (Pursuer_StateWalkThen404): a step back facing her, to its end
: backing ( -- )
    fiona body-at turn-rate turn-to fdrop  root-move
    ended? if  -1 step-done !  then ;
: step-back ( -- )  $404 play-now  ['] backing $1D act! ;
\ standing to her (Pursuer_Move17C): turned to her
: facing-her ( -- )  fiona body-at turn-rate turn-to fdrop  freeze-t @ 0= if  root-move  then ;
: stand-to ( -- )  stand-anim play  ['] facing-her 1 act! ;

: pursue-run ( -- )   \ (Pursuer_BehaviourStalk, each frame)
    1 chase-t +!
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    chase-act @ case
        $1D of  d-her touch 5e f+ f< 0=  d-her f0< or if  pick-pace  then  endof
        6 of  hold-t @ 0> 0=  d-her 100e f< and if  10 choose exit  then
              d-her touch f<=  d-her f0< 0= and if  step-back  then  endof
        5 of  hold-t @ 0> 0=  d-her 100e f> or if  6  her-stage @ pursue-row 2 cells + @  keep
              else  d-her touch f<=  d-her f0< 0= and if  step-back  then  then  endof
        1 of  d-her ground-gained f>  in-front? 0= or if  pick-pace  then  endof
        $1A of  step-done @ if  5  her-stage @ pursue-row cell+ @  keep  then  endof
        $1B of  step-done @ if  5  her-stage @ pursue-row cell+ @  keep  then  endof
        >r  step-done @ if  pick-pace  then  r>
    endcase
    hold-t @ 0> if  -1 hold-t +!  then
    chase-act @ dup 1 = over 5 = or swap 6 = or if  in-reach? if  attack!  0 hold-t !  then  then ;

\ Pursuer_ChaseDecision: close and in front of him on her floor, not past the panic's stage 3 -
\ standing to her; else by the row
: pursue-now ( -- )
    0 search-phase !  ['] pursue-run step!  0 chase-t !  0 hold-off !  -1 combo !
    d-her ground-gained f<  d-her f0< 0= and  same-floor? and  in-front? and  her-stage @ 4 < and if
        stand-to  0 hold-t !  exit
    then
    pick-pace ;

\ the chase begins by his own: Debilitas's, or the Pursuer's
:noname ( -- )   \ (his own: Riccardo's)
    own-chase @ ?dup if  execute exit  then  base-chase @ if  pursue-now  else  chase-now  then ; is chase-begin
:noname ( -- flag )  step @ dup ['] chase-run =  over ['] pursue-run = or  swap own-run @ = own-run @ 0<> and or ; is chasing?
