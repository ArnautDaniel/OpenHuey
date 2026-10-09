\ stalker/grab.fs - a stalker taking Fiona (src/game/pursuer.c, debilitas.c): closing on her and
\ asking her to his side (Pursuer_StateFaceFiona / StateWalkOn, Pursuer_CloseOnFiona: in reach
\ and facing her, `seize` - by the hand, or walking) - her floor not free for him, his attack
\ tables instead; she taken: by the hand his taunting hold ($1900, $1901 again and again - each
\ a fright to her - until she breaks free, $1903, or the sixth, $1904), walking his grab
\ ($1A01); refused, his attack tables. His lunge ($1306: in the panic's stage 5 on into his
\ seize) and his grab at Hewie ($E06: Debilitas_Actions $1002 / $1003).
IN: stalker.grab
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.attack stalker.chase ;

: fright ( F: amount -- )  f>cell panic-id send fright ;
\ her floor free for him to take her on (Actor_TriFreeFor: not where nothing may stand)
: her-floor-free? ( -- flag )
    fiona body-tri dup 0< if  drop false exit  then  nav-flags $80001 and 0= ;
\ she may be taken (Pursuer_MayGoForTarget: not out of reach - struck, seized -, not with the
\ stalkers blind)
: may-take? ( -- flag )  her-plight @ 3 <>  stalkers-blind state-flag? 0= and ;

\ ---- taken by the hand: his taunting hold, a fright each time (10 the first, then 5); she
\ breaks free - he lets go ($1903); the sixth - he holds on ($1904) while she's dragged off ----
: holding-on ( -- )  root-move ;
: let-go-end ( -- )   \ ($1903 to its end: held off her a while)
    root-move  ended? if  1 took !  90 hold-off !  let-her-go  -1 step-done !  then ;
: taunting ( -- )
    root-move
    ended? 0= if  exit  then
    5e fright  1 taunts +!
    answer @ 3 = if  $1903 play-now  ['] let-go-end act-xt ! exit  then
    taunts @ 6 >= if  $1904 play-now  ['] holding-on act-xt ! exit  then
    $1901 play-now ;
: taunt-start ( -- )
    root-move
    answer @ 3 = if  $1903 play-now  ['] let-go-end act-xt ! exit  then
    ended? if  $1901 play-now  10e fright  0 taunts !  ['] taunting act-xt !  then ;
\ ---- taken walking: his grab ($1A01), holding on ----
: carrying ( -- )  root-move ;

\ ---- her answer ----
: awaiting ( -- )
    root-move
    answer @ case
        1 of  -1 leading !
              lead-type @ 6 = if  $1900 play-now  ['] taunt-start  else  $1A01 play-now  ['] carrying  then
              act-xt !  endof
        2 of  0 leading !  pick-attack  endof
    endcase ;
\ ---- closing on her (Pursuer_CloseOnFiona): in reach (20) and facing her (20 degrees): taken
\ if she may be (else his table for being near her, 3); her on a ladder (mode 3) - his attack
\ tables; given up after 120 frames ----
: closing ( -- )
    1 step-t +!  step-t @ 120 > if  -1 step-done !  exit  then
    d-her 20e f<  d-her f0< 0= and
    fiona body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f< and if
        may-take? if
            0 answer !  1 leading !  lead-type @ fiona send seize  stand-anim play   \ (asked: let go if he moves on)
            ['] awaiting act-xt !  exit
        then
        3 choose exit
    then
    fiona body-at turn-rate turn-to fdrop
    fiona self body-mask straight? if  fiona body-at step-toward  else  walk-stride drop  then ;
:noname ( type -- )
    her-floor-free? 0= if  drop pick-attack exit  then
    dup lead-type !  6 = if  stand-anim  else  walk-fast  then  play
    fiona body-tri dup 0< 0= if  fiona body-at plan drop  else  drop  then
    ['] closing $14 act! ; is take-her

\ ---- his lunge ($1306): in the panic's stage 5 on into his seize (combo 8) ----
: lunging ( -- )
    root-move
    ended? if  her-stage @ 5 = if  8 combo-start  else  -1 step-done !  then  then ;
:noname ( -- )  $1306 play-now  ['] lunging $1002 act! ; is lunge

\ ---- his grab ($E06): at its key, Hewie by his hand (bone $1E, within 5) is held (a hold, from
\ behind); over at its end, losing her, or her 60 off ----
create hand-at 12 allot
: dog-at-hand? ( -- flag )
    hewie here? 0= if  false exit  then
    my-model @ $1E bone-pos hand-at vec!
    hand-at 4 + sf@  hewie body-at 4 + sf@ 5e f- f> 0= if  false exit  then
    hand-at 4 + sf@  hewie body-at 4 + sf@  hewie body-dims fswap fdrop f+ 5e f+ f< 0= if  false exit  then
    hand-at hewie body-at vec-dist-xz  hewie body-dims fdrop 5e f+ f< ;
: grabbing ( -- )
    root-move
    my-model @ 0 0 0 motion-events 2 and  sent @ 0= and if
        dog-at-hand? if  6 3 10e f>cell hewie send hit  -1 sent !  then
    then
    ended?  sees-fiona @ 0= or  d-her 60e f< 0= or  d-her f0< or if  -1 step-done !  then ;
:noname ( -- )  0 sent !  $E06 play-now  ['] grabbing $1003 act! ; is grab-dog
