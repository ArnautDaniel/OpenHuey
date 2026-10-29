\ stalker/pursue.fs - the Pursuer's own chase (src/game/pursuer.c Pursuer_ChaseDecision,
\ Pursuer_BehaviourStalk, Pursuer_HoldOff; Pursuer_StateWalkThen404), the one stalkers without
\ their own use (Daniella; Debilitas has his: stalker/chase.fs). Seeing her: close and in front
\ of him, not panicking past stage 3 - he stands to her; else by his chase table's row for her
\ panic's stage (`pursue-row`: the chance to walk at her, frames walking, frames stalking) he
\ walks at her or stalks her a while. Walking, his time out or her past 100: he
\ stalks. Stalking, his time out within 100: his lunge table. Touching her: a step back facing
\ her, and again by the row once she is 5 further off. Standing: turning to her; out of the
\ ground he gains, or not in front of him (20 degrees): by the row again. Round her, or his third
\ taunt (its gesture flagged), far off: stalking; over, walking at her. A combo or another taunt
\ over: by the row again. Within reach in front of him on her floor: his
\ attack (his table for how she stands).
IN: stalker.pursue
USING: engine game-state actors messages common facts flag-names paths stalker.anims stalker.names stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase ;

: touch ( F: -- d )  self body-dims fdrop  fiona body-dims fdrop f+ ;   \ (their radii)
: in-front? ( -- flag )  fiona body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f< ;
\ Pursuer_HoldOff: walking at her or stalking her, for the row's frames
\ (the row: the chance to walk at her, frames walking, frames stalking)
: walk-frames# ( row -- n )  cell+ @ ;   : stalk-frames# ( row -- n )  2 cells + @ ;
: pick-pace ( -- )
    her-stage @ pursue-row
    rnd 100e f*  dup f@ f<= if  walk-at-act swap walk-frames#  else  stalk-act swap stalk-frames#  then  hold-for ;
\ round her, or his third taunt with its gesture's flag (Pursuer_BehaviourStalk $1A / $19): more
\ than 100 from her, stalking her for the row's frames; else, over, walking at her for the row's
\ ($1B, his second way round, has no case there)
: far-off? ( -- flag )  d-her 100e f> ;
: round-or-taunt ( far-stalks? -- )
    far-off? and if  stalk-act her-stage @ pursue-row stalk-frames# hold-for exit  then
    step-done @ if  walk-at-act her-stage @ pursue-row walk-frames# hold-for  then ;
\ standing to her (Pursuer_Move17C): turned to her
: facing-her ( -- )  fiona body-at turn-rate turn-to fdrop  freeze-t @ 0= if  root-move  then ;
: stand-watching ( -- )  still-anim play  ['] facing-her watch-act act! ;

: pursue-run ( -- )   \ (Pursuer_BehaviourStalk, each frame)
    1 chase-t +!
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    in-attack @ if  attack-run exit  then   \ (what he picked from his tables: Pursuer_BehaviourAttack)
    chase-act @ case
        back-off-act of  d-her touch 5e f+ f< 0=  d-her f0< or if  pick-pace  then  endof
        stalk-act of  hold-t @ 0> 0=  d-her 100e f< and if  lunge-situation choose exit  then
              d-her touch f<=  d-her f0< 0= and if  step-back  then  endof
        walk-at-act of  hold-t @ 0> 0=  d-her 100e f> or if  stalk-act  her-stage @ pursue-row stalk-frames#  hold-for
              else  d-her touch f<=  d-her f0< 0= and if  step-back  then  then  endof
        watch-act of  d-her ground-gained f>  in-front? 0= or if  pick-pace
              else  d-her f0< if  out-of-reach 0= if  search-again exit  then  then  then  endof
        round-act of  true round-or-taunt  endof
        taunt-3-act of  taunt-gesture @ gesture-flag? round-or-taunt  endof
        >r  step-done @ if  pick-pace  then  r>
    endcase
    hold-t @ 0> if  -1 hold-t +!  then
    moving-act? if  in-reach? if  attack!  0 hold-t !  then  then ;

\ Pursuer_ChaseDecision: close and in front of him on her floor, not past the panic's stage 3 -
\ standing to her; else by the row
: pursue-now ( -- )
    no-search search-phase !  ['] pursue-run step!  0 chase-t !  0 hold-off !  -1 combo !  0 in-attack !
    d-her ground-gained f<  d-her f0< 0= and  same-floor? and  in-front? and  her-stage @ panicking < and if
        stand-watching  0 hold-t !  exit
    then
    pick-pace ;

\ the chase begins by his own: Debilitas's, or the Pursuer's - Debilitas's too while her panic is
\ calming down (Debilitas_ChaseDecision)
:noname ( -- )   \ (his own: Riccardo's)
    own-chase @ ?dup if  execute exit  then
    base-chase @  her-stage @ calming-down = or if  pursue-now  else  chase-now  then ; is chase-begin
:noname ( -- flag )  step @ dup ['] chase-run =  over ['] pursue-run = or  swap own-run @ = own-run @ 0<> and or ; is chasing?
