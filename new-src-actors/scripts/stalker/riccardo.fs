\ stalker/riccardo.fs - Riccardo's own (P4c; src/game/riccardo.c): his behaviour and chase
\ (Riccardo_StateBehaviour / Riccardo_Chase), his flurry of hammer blows (Riccardo_StartFlurry,
\ FlurryBlow, StateBlow / StateBlowFiona / StateBlowHewie, StateAfterBlow), his lunge
\ (Riccardo_StartLunge), his attack by how near (Riccardo_AttackForDistance) and the impact of his
\ hammer (Riccardo_Impact). At the panic's stage 5 he chases as the Pursuer does (stalker/pursue.fs).
\ Seeing her: further than 100 he holds off; near (60) and seen by her he stands to her, coming on
\ a few times (his waits' count) before walking at her; else by a roll he walks at her or stalks
\ her for his waits' frames, and when they run out within 100 lunges (by his lunge table) or
\ starts again. Within his reach (20), in front of him, on her floor: his attack table. His
\ flurry: 1..7 blows by his chances; on her floor a swing ($E00 / $E04), not on it a slam down
\ at her ($2302 / $2304, one blow); at Hewie on his floor his swing, else he gives up. A blow at
\ its key: whoever it reaches - Hewie in the way of her, or her (by how near: 3 his grab within
\ 10, 6 caught - stage below 4 - or 4 a hard blow; crouched under it, it passes over) - and else
\ its debris where it lands. Its impact: a noise ($40) where he stands.
IN: stalker.riccardo
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue ;

: acoustics-id ( -- id )  s" acoustics" actor-named ;
: wait# ( i -- n )  cells waits + @ ;              \ (his waits: 0 walking, 1 stalking, 2 walking once
: wait-chance ( F: -- c )  5 cells waits + f@ ;   \ seen, 3 stalking when far, 4 the comings-on)
: come-reach ( F: -- r )  60e ;                     \ (+0x17C0)
: d-her ( F: -- d )  d-fiona f@ ;
: stage5? ( -- flag )  her-stage @ 5 = ;

\ ---- his chase (Riccardo_Chase) ----
defer ric-begin ( -- )
: back-or-hold ( F: roll -- )   \ (Riccardo_BackOrHold)
    wait-chance f<= if  5 0 wait# keep  else  6 1 wait# keep  then ;
: lunge? ( -- flag )   \ (Riccardo_Lunge: by his lunge table, his table 10)
    rnd 100e f*  lunge-chance s>f f< if  10 choose true  else  false  then ;
: strike-or-wait ( -- )  lunge? if  exit  then  rnd 100e f* back-or-hold ;
: stand-to ( -- )  stand-watch  4 wait# comings !  0 hold-t ! ;
: near-seen? ( -- flag )  d-her come-reach f<=  faces-me? and ;
: standing-step ( -- )   \ 1
    chase-t @ 60 mod 0= if  lunge? if  exit  then  then
    settled? if  fiona body-at 1.0471976e 2.6179939e way-past dup $FF <> if  turn-to-her exit  then  drop  then
    faces-me? 0=  d-her come-reach 20e f+ f> or if  rnd 100e f* back-or-hold exit  then
    comings @ 0> if  -1 comings +!  else  5 2 wait# keep  then ;
: stalking-step ( -- )   \ 6
    near-seen? if  comings @ 0> if  stand-to  else  5 2 wait# keep  then  exit  then
    hold-t @ 0>  d-her 100e f< 0= or if  -1 hold-t +!  exit  then
    strike-or-wait ;
: walking-step ( -- )   \ 5
    near-seen? comings @ 0> and if  stand-to exit  then
    hold-t @ 0> if  d-her 100e f<= if  -1 hold-t +!  else  6 3 wait# keep  then  exit  then
    strike-or-wait ;
: moving-act? ( -- flag )  chase-act @ dup 1 =  over 5 = or  swap 6 = or ;
: ric-run ( -- )
    1 chase-t +!
    hold-off @ if  -1 hold-off +!  then
    act-xt @ execute
    stage5? moving-act? and if  pursue-now exit  then   \ (the panic's stage 5: the Pursuer's chase)
    chase-act @ case
        1 of  standing-step  endof
        5 of  walking-step  endof
        6 of  stalking-step  endof
        $1A of  d-her 100e f> if  6 3 wait# keep  else  step-done @ if  stand-watch  then  then  endof
        >r  step-done @ if  ric-begin  then  r>
    endcase
    moving-act? if  in-reach? if  attack!  then  then ;
\ his behaviour (Riccardo_ChaseDecision / StateBehaviour): the Pursuer's at the panic's stage 5
:noname ( -- )
    stage5? if  pursue-now exit  then
    0 search-phase !  ['] ric-run step!  0 chase-t !  0 hold-off !  -1 combo !
    sees-fiona @ 0= if  chase-start exit  then
    d-her 100e f> if  6 3 wait# keep
    else  near-seen? if  stand-watch  0 hold-t !  else  rnd 100e f* back-or-hold  then  then
    4 wait# comings ! ; is ric-begin

\ ---- his attack by how near (Riccardo_AttackForDistance): a chance to attack at all by the
\ distance; at Hewie 4; at her 6 (below stage 4), 4 or 3 by the distance's chances. 0: none ----
fvariable c3  fvariable c4  fvariable c6
: early? ( -- flag )  her-stage @ 4 < ;
: attack-for ( F: d -- k )
    0e c3 f!  0e c4 f!  0e c6 f!
    fdup 10e f< if  fdrop 100e  100e c3 f!
    else fdup 30e f< if  fdrop 50e  early? if  50e c6 f!  then  100e c4 f!
    else fdup 60e f< if  fdrop 25e  early? if  75e c6 f!  then  100e c4 f!
    else fdup 90e f< if  fdrop 15e  early? if  75e c6 f!  then  100e c4 f!
    else 150e f< if  10e  early? if  100e c6 f!  else  100e c4 f!  then
    else  0e  then then then then then
    rnd 100e f* f> 0= if  0 exit  then
    rnd 100e f*  fdup c6 f@ f<= if  fdrop 6 exit  then
    fdup c4 f@ f<= if  fdrop 4 exit  then  c3 f@ f<= if  3  else  0  then ;

\ ---- his blows ----
: move# ( n -- addr )  /move * moves + ;
: blow-entry ( -- addr )  blow-move @ move# ;
: blow-key? ( -- flag )  my-model @ 0 -1 1 motion-events 2 and 0<> ;
fvariable lx  fvariable lz  fvariable px  fvariable pz  variable la
: line-dist ( a b p -- ) ( F: -- d )   \ (Vec_LineDistance: p off the line a - b, across the floor)
    dup sf@ px f!  8 + sf@ pz f!  swap la !
    dup sf@ la @ sf@ f- lx f!  8 + sf@ la @ 8 + sf@ f- lz f!
    px f@ la @ sf@ f- px f!  pz f@ la @ 8 + sf@ f- pz f!
    lx f@ pz f@ f*  lz f@ px f@ f* f- fabs
    lx f@ fdup f*  lz f@ fdup f* f+ fsqrt  fdup f0= if  fdrop  else  f/  then ;
: debris ( -- ) ;   \ (its debris where it lands: the effect isn't written yet)
: impact? ( -- flag )  my-model @ 0 -1 1 motion-events $20 and 0<> ;
\ the impact (Riccardo_Impact): a noise where he stands (and its debris)
: impact ( -- )
    impact? 0= if  exit  then
    $40 room-id self body-tri -1 stalker-noise acoustics-id send noise
    anim @ dup $2301 = over $2303 = or swap $1602 = or if  debris  then ;
\ Hewie struck: the move's kind and damage
: strike-hewie ( -- )  blow-entry 4 cells + @  blow-entry 5 cells + @  0  hewie send blow ;
\ her struck by k (6: the heavy move with its stun - from his swing $E00 the move 6, $E04 the
\ move 7, his swing at Hewie the move 8 - and its debris; 4: a stumble three times in four)
variable stun
: heavy ( n -- n' )  case  0 of 6 endof  3 of 7 endof  1 of 8 endof  dup  endcase ;
: strike-her ( k -- )
    dup 0= if  drop debris exit  then
    0 stun !
    dup 6 = if
        blow-move @ heavy blow-move !
        blow-entry cell+ @ $FFFF and  dup $8000 and if  $10000 -  then  stun !  debris
    then
    dup 4 = if  rnd 0.75e f< if  $8000 stun !  then  then
    stun @  blow-entry 6 cells + f@ f>cell  fiona send hit ;
: in-front? ( id -- flag ) ( F: a -- )   \ within a of his heading
    body-at heading-to yaw f- angle-wrap fabs f> ;
: dog-floor? ( -- flag )
    hewie body-at 4 + sf@  my-at 4 + sf@  fover fover 15e f+ f<=  10e f- f>= and ;
: d-dog ( F: -- d )  my-at hewie body-at vec-dist ;
\ she in the way of a blow at Hewie: within 4 of the line to him, and no further than he is
: her-in-way? ( -- flag )
    my-at hewie body-at fiona body-at line-dist 4e f<=  d-dog d-her f< 0= and ;
\ (Riccardo_OutOfLine) she's clear of a blow at Hewie: off its line, or further than he is
: her-clear? ( -- flag )  her-in-way? 0= ;

\ ---- the flurry ----
defer flurry-blow ( -- )
: blow-over ( -- )  0 blow-aim !  -1 step-done ! ;
: after-blow ( -- )   \ (Riccardo_StateAfterBlow: another while it's in front, 90 degrees)
    -1 blows +!
    blows @ 0> 0= if  blow-over exit  then
    at-dog @ if
        sees-hewie @ 0=  d-hewie f@ 30e f> or if  blow-over exit  then
        hewie  pi f2/ in-front? 0= if  blow-over exit  then
    else
        sees-fiona @ 0=  d-her 100e f> or  stage5? or if  blow-over exit  then
        fiona  pi f2/ in-front? 0= if  blow-over exit  then
    then
    flurry-blow ;
: grunt ( id -- )   \ (at the motion's key 1, half the time)
    my-model @ 0 0 1 motion-events 1 and  rnd 0.5e f< and if  7 sound  else  drop  then ;
\ a kind 6 blow (Riccardo_Blow): at its key, whoever is within its reach
: blowing ( -- )
    blow-key? if
        quarry body-at my-at vec-dist  blow-entry 3 cells + f@ f< if
            at-dog @ if  strike-hewie
            else  blow-entry 4 cells + @  blow-entry cell+ @ $FFFF and  blow-entry 6 cells + f@ f>cell  fiona send hit  then
        then
    then
    ended? if  after-blow exit  then  root-move ;
\ at Hewie (Riccardo_StateBlowHewie)
: blowing-dog ( -- )
    blow-key? if
        0 blow-aim !
        dog-floor?  sees-hewie @ and  d-hewie f@ 30e f< and  hewie 30e deg>rad in-front? and if
            her-in-way? if  d-her attack-for strike-her
            else  d-hewie f@ attack-for if  strike-hewie  else  debris  then  then
        then
    then
    blow-aim @ if  hewie body-at turn-rate turn-to fdrop  then
    ended? if  after-blow exit  then
    $15 grunt  root-move ;
\ at her (Riccardo_StateBlowFiona): Hewie in the way takes it; crouched under it, it passes over
: blowing-her ( -- )
    blow-key? if
        0 blow-aim !
        sees-fiona @ same-floor? and  fiona 30e deg>rad in-front? and if
            hewie here?  fiona body-at hewie body-at vec-dist 50e f< and  d-hewie f@ 20e f> and
            dog-floor? and  her-clear? and if
                d-hewie f@ attack-for if  strike-hewie  else  debris  then
            else
                d-her 20e f<  fiona body-at 4 + sf@ fiona body-dims fswap fdrop f+  my-at 4 + sf@ 5e f+ f< and if  debris
                else  d-her attack-for strike-her  then
            then
        then
    then
    blow-aim @ if  fiona body-at turn-rate turn-to fdrop  then
    ended? if  after-blow exit  then
    blow-move @ case  0 of $22 grunt endof  3 of $23 grunt endof  endcase  root-move ;
:noname ( -- )   \ (Riccardo_FlurryBlow)
    at-dog @ if
        dog-floor? 0= if  0 blows !  stand-watch exit  then  1 blow-move !
    else
        same-floor? if  rnd 0.5e f< if  0  else  3  then
        else  rnd 0.5e f< if  9  else  8  then  1 blows !  then  blow-move !
    then
    blow-entry @ play-now  -1 blow-aim !
    blow-entry 4 cells + @ 6 = if  ['] blowing
    else  at-dog @ if  ['] blowing-dog  else  ['] blowing-her  then  then  $1000 act! ; is flurry-blow
: more? ( n -- n flag ) ( F: roll -- roll )
    dup 6 < if  dup floats flurry-chances + f@ fover f<  else  false  then ;
: flurry-start ( -- )   \ (Riccardo_StartFlurry: 1..7 blows by his chances)
    world-held state-flag?  stalkers-blind state-flag? or if  stand-watch exit  then
    rnd 100e f*  0  begin  more?  while  1+  repeat  fdrop  1+ blows !  flurry-blow ;

\ ---- his lunge (Riccardo_StartLunge: $1300; at the panic's stage 5 into his move 5) ----
: lunging ( -- )
    root-move
    ended? 0= if  exit  then
    stage5? if  5 combo-start  else  -1 step-done !  then ;
: lunge-start ( -- )  $1300 play-now  ['] lunging $1001 act! ;
create riccardo-acts  ' flurry-start , ' lunge-start ,

\ ---- his frame's own (Riccardo_Update): his rage at the panic's stage 5 (a roar, $26), his
\ snort at $600's key, the impact of his hammer ----
: riccardo-frame ( -- )
    raging @ if  stage5? 0= if  0 raging !  then
    else  stage5? reeling @ 0= and  my-mode @ 0= and if  -1 raging !  $26 7 sound  then  then
    anim @ $600 =  my-model @ 0 0 1 motion-events 1 and and if  $24 5 sound  then
    impact ;
