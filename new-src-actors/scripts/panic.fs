\ panic.fs - the panic meter (docs/subsystems/panic.md; the original's Panic_*): a level made of
\ a lasting and a passing part, its stage, Fiona's breath and scream. Frights and fear come in
\ as messages; each frame's end it updates and tells everyone its stage and level.
IN: panic
USING: engine actors messages facts ;

state: panic-state
  cell field stage           \ 0 calm, 1..3 at 60 / 75 / 90, 4 panicking, 5 calming down
  cell field weighing        \ how the inputs weigh (0..3)
  cell field length          \ frames of the panic left (negative: its start)
  1 floats field level       \ 0..100
  cell field hold            \ frames a fright holds off recovering
  1 floats field passing     \ the passing part
  1 floats field lasting     \ the lasting part
  \ this frame's inputs: frights (and from time), fear, calming, recovering
  1 floats field fright-a   1 floats field fright-a2
  1 floats field fear-d     1 floats field fear-d2
  1 floats field calm-b     1 floats field calm-b2
  1 floats field recovering
  cell field beat            \ frames to her next breath
  cell field shake
  1 floats field scale       \ the fear's scale (1)
  cell field the-danger      \ (told: 0 calm...)
  cell field her-mode  cell field her-sub  cell field her-cond   \ (told by Fiona each frame)
end-state

: fiona-id ( -- id )  s" fiona" actor-named ;
: her-sound ( id vol -- )   \ a sound of hers, at her
    fiona-id dup body? 0= if  drop 2drop exit  then  >r  5 swap 0  r> body-pos sound-at ;
: inputs-clear ( -- )
    0 weighing !  0e passing f!  0e fright-a f!  0e fright-a2 f!  0e recovering f!
    0e fear-d f!  0e fear-d2 f!  0e calm-b f!  0e calm-b2 f! ;
\ Panic_SetLevel: the stage by the level (at 100 its length 450)
: set-level ( n -- )
    dup s>f level f!  dup s>f lasting f!
    dup 100 >= if  450 length !
    else dup 90 >= if  3 stage !  else dup 75 >= if  2 stage !  else dup 60 >= if  1 stage !  else  0 stage !
    then then then then  drop  inputs-clear ;
create stage-levels  0 , 60 , 75 , 90 , 100 , 100 ,
: set-stage ( stage -- )  dup 6 u< if  cells stage-levels + @ set-level  else  drop  then ;
\ a fright: under 10 to this frame's fear; more, half of it to the passing part too, held 30 frames
: add-fright ( F: amount -- )
    fdup f0< if  fdrop exit  then
    fdup 10e f< if  fear-d f@ f+ fear-d f!  exit  then
    0.5e f*  30 hold !  fdup fear-d f@ f+ fear-d f!  fright-a f@ f+ fright-a f! ;

\ ---- her breath (Panic_Breath: 0x29, softer at the lower stages) ----
create breath-vol  -56 , -40 , -16 , 0 , 0 ,
: breath ( -- )   \ (not while the screen's effect is off: with effects)
    stage @ dup 1 6 within 0= if  drop exit  then
    1- cells breath-vol + @  $29 swap her-sound ;

\ ---- Panic_Stage ----
: by-level ( -- )
    level f@ 100e f< 0= if   \ she panics (on her feet: her scream)
        100e level f!  100e lasting f!  0e passing f!
        her-mode @ dup 0= swap 3 = or if  $42 $40 her-sound  4 stage !  -30 length !  0 beat !  then
        exit
    then
    level f@ 90e f< 0= if  3  else level f@ 75e f< 0= if  2  else level f@ 60e f< 0= if  1  else  0  then then then
    stage ! ;
: staging ( -- )
    beat @ if  -1 beat +!  then
    stage @ 4 < if
        stage @ 0= if  shake @ 8 - 0 max shake !  then
        by-level
        stage @ 4 = if  exit  then
        stage @ 0> beat @ 0= and if  4 stage @ - 30 * beat !  breath  then
        level f@ 60e f< if  exit  then
        level f@ 60e f- 1.5e f* f>s  dup shake @ > if  drop 1 shake +!  else  shake @ < if  -1 shake +!  then  then
        exit
    then
    stage @ 4 = if
        length @ 0< if   \ its start: then its length, longer hurt or down
            1 length +!  length @ 0= if
                450 length !  her-cond @ dup 1 = if  300 length +!  then  3 = if  150 length +!  then
            then  exit
        then
        fear-d f@ 500e f< 0= if  150 length +!  length @ 450 > if  450 length !  then  5 stage !  then
    then
    -1 length +!
    length @ 0< if  0 stage !  50e level f!  50e lasting f!  0e passing f!  exit  then
    stage @ 4 <> if  $90  else  $50  then  shake !
    beat @ if  exit  then
    6 stage @ - 7 * beat !  breath ;

\ ---- Panic_FearInputs (calm with time; the stalker's part comes as fear-in) and Panic_Update ----
: time-inputs ( -- )
    the-danger @ 0= if  calm-b2 f@ 0.0999998e f+ calm-b2 f!
    else her-mode @ 0= her-sub @ 0= and if  calm-b2 f@ 0.0333333e f+ calm-b2 f!  then then
    hold @ 0= if  recovering f@ 0.1666653e f+ recovering f!  then ;
fvariable u-a  fvariable u-b  fvariable u-c  fvariable u-d
: weigh ( F: ka kd kb -- )   \ this frame's frights, fear and calming from time, weighed
    calm-b2 f@ f* u-b f@ f+ u-b f!  fear-d2 f@ f* u-d f@ f+ u-d f!  fright-a2 f@ f* u-a f@ f+ u-a f! ;
: update ( -- )
    staging
    hold @ if  -1 hold +!  then
    level f@ 100e f< if
        time-inputs
        fright-a f@ u-a f!  calm-b f@ u-b f!  recovering f@ u-c f!  fear-d f@ u-d f!
        weighing @ case
            0 of  1e 1e 1e weigh  endof
            1 of  0.6e 0.6e 2e weigh  endof
            2 of  0.8e 0.8e 1.6e weigh  endof
            3 of  0.9e 0.9e 1.5e weigh  endof
        endcase
        u-d f@ scale f@ f* u-d f!  u-a f@ scale f@ f* u-a f!
        passing f@ u-c f@ f- 0e fmax passing f!
        lasting f@ u-b f@ f- 0e fmax u-d f@ f+ lasting f!
        u-a f@ f0> 0= if
            lasting f@ passing f@ f+ level f!
            level f@ 100e f< 0= if  99e level f!  99e lasting f!  then
        else
            lasting f@ passing f@ f+ lasting f!  u-a f@ passing f!  lasting f@ passing f@ f+ level f!
        then
        $15 state-flag? if   \ (it reaches 100 only under flag 0x15 - as the original: never)
            level f@ 100e f< 0= if  99e level f!  99e lasting f!  0e passing f!  then
        then
    then
    inputs-clear ;

behaviour panicking
  on spawned ( -- )  1e scale f!  self subscribe frame-end  self subscribe danger  self subscribe fiona-doing ;
  on danger ( level -- )  the-danger ! ;
  on fiona-doing ( mode sub cond cmd -- )  drop her-cond !  her-sub !  her-mode ! ;
  on fright ( amount -- )  cell>f add-fright ;
  on fear-in ( amount -- )   \ (a negative one is a fright: held 30, 50 lasting and passing)
      cell>f fdup f0< if  fdrop 30 hold !  fear-d f@ 50e f+ fear-d f!  fright-a f@ 50e f+ fright-a f!  exit  then
      0.0333333e f* fear-d2 f@ f+ fear-d2 f! ;
  on panic-stage! ( stage -- )  set-stage ;
  on panic-level! ( level -- )  set-level ;
  on frame-end ( -- )  update  stage @ level f@ f>cell broadcast panic ;
end-behaviour

: panic-spawn ( -- id )  panicking panic-state s" panic" spawn ;
