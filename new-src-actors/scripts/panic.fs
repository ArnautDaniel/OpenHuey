\ panic.fs - the panic meter (docs/subsystems/panic.md; the original's Panic_*): a level made of
\ a lasting and a passing part, its stage, Fiona's breath and scream. Frights and fear come in
\ as messages; each frame's end it updates and tells everyone its stage and level.
IN: panic
USING: engine actors messages facts flag-names items items.names ;

state: panic-state
  cell field stage           \ (enum: panic-stage, messages.fs)
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
  cell field jolting         \ the camera shaken by her panic (Panic_Stage: 0.2 past its first stretch)
  1 floats field scale       \ the fear's scale (1)
  cell field the-danger      \ (told)
  cell field her-mode  cell field her-sub  cell field her-cond   \ (told by Fiona each frame)
end-state

: fiona-id ( -- id )  s" fiona" actor-named ;
$29 constant breath-sound   $42 constant scream-sound   \ (hers: the common bank)
: her-sound ( id vol -- )   \ a sound of hers (the common bank), at her
    fiona-id {: id vol her :}  her body? 0= if  exit  then
    id 5 vol 0  her body-pos sound-at ;
: inputs-clear ( -- )
    0 weighing !  0e passing f!  0e fright-a f!  0e fright-a2 f!  0e recovering f!
    0e fear-d f!  0e fear-d2 f!  0e calm-b f!  0e calm-b2 f! ;
450 constant panic-frames   \ (how long a panic lasts)
: stage-at ( level -- stage )
    dup 90 >= if  drop terrified exit  then
    dup 75 >= if  drop scared exit  then
    60 >= if  shaken  else  composed  then ;
\ Panic_SetLevel: the stage by the level (at 100 its length)
: set-level ( n -- )
    dup s>f level f!  dup s>f lasting f!
    dup 100 >= if  drop panic-frames length !  else  stage-at stage !  then
    inputs-clear ;
create stage-levels  0 , 60 , 75 , 90 , 100 , 100 ,   \ (a stage set: its level)
: set-stage ( stage -- )  dup calming-down 1+ u< if  cells stage-levels + @ set-level  else  drop  then ;
\ a fright: under 10 to this frame's fear; more, half of it to the passing part too, held 30 frames
: add-fright ( F: amount -- )
    fdup f0< if  fdrop exit  then
    fdup 10e f< if  fear-d f@ f+ fear-d f!  exit  then
    0.5e f*  30 hold !  fdup fear-d f@ f+ fear-d f!  fright-a f@ f+ fright-a f! ;

\ ---- her breath (Panic_Breath, softer at the lower stages) and the pad's beat with it: a swell
\ over 3 frames, harder by the stage; panicking then full a moment, calming down the small motor
\ with the swell ----
begin-structure /breath  field: breath-volume  field: breath-strength  end-structure
create breaths   \ (shaken .. calming down)
    -56 ,  96 ,   -40 , 144 ,   -16 , 192 ,   0 , 224 ,   0 , 224 ,
: breath-of ( stage -- addr )  1- /breath * breaths + ;
: breath-rumble ( stage -- )
    rumble-sequence-clear
    dup calming-down =  0  rot breath-of breath-strength @  3 rumble-step
    stage @ panicking = if  0 255 255 3 rumble-step  then ;
: breath ( -- )   \ (not while the screen's effect is off: with effects)
    stage @ shaken calming-down 1+ within 0= if  exit  then
    stage @ breath-rumble
    breath-sound  stage @ breath-of breath-volume @  her-sound ;

\ ---- Panic_Stage ----
: on-her-feet? ( -- flag )  her-mode @ dup 0= swap 3 = or ;
: by-level ( -- )
    level f@ 100e f< 0= if   \ she panics (on her feet: her scream)
        100e level f!  100e lasting f!  0e passing f!
        on-her-feet? if  scream-sound $40 her-sound  panicking stage !  -30 length !  0 beat !  panicked  then
        exit
    then
    level f@ f>s stage-at stage ! ;
: shake-toward ( n -- )   \ the shake a step nearer n
    dup shake @ > if  drop 1 shake +!  else  shake @ < if  -1 shake +!  then  then ;
: uneasy ( -- )   \ not yet panicking: her breath by the stage, the shake by the level
    stage @ composed = if  shake @ 8 - 0 max shake !  then
    by-level
    stage @ panicking = if  exit  then
    stage @ composed <> beat @ 0= and if  panicking stage @ - 30 * beat !  breath  then
    level f@ 60e f< if  exit  then
    level f@ 60e f- 1.5e f* f>s shake-toward ;
: panic-length ( -- frames )   \ longer hurt or down
    panic-frames  her-cond @ case  1 of  300 +  endof  3 of  150 +  endof  endcase ;
: staging ( -- )
    0 jolting !
    beat @ if  -1 beat +!  then
    stage @ panicking < if  uneasy exit  then
    stage @ panicking = if
        length @ 0< if   \ its start: then its length
            1 length +!  length @ 0= if  panic-length length !  then  exit
        then
        fear-d f@ 500e f< 0= if  length @ 150 + panic-frames min length !  calming-down stage !  then
    then
    -1 length +!
    length @ 0< if  composed stage !  50e level f!  50e lasting f!  0e passing f!  exit  then
    stage @ panicking <> if  $90  -1 jolting !  else  $50  then  shake !
    beat @ if  exit  then
    6 stage @ - 7 * beat !  breath ;

\ ---- Panic_FearInputs (calm with time; the stalker's part comes as fear-in) and Panic_Update ----
: time-inputs ( -- )
    the-danger @ calm = if  calm-b2 f@ 0.0999998e f+ calm-b2 f!
    else her-mode @ 0= her-sub @ 0= and if  calm-b2 f@ 0.0333333e f+ calm-b2 f!  then then
    hold @ 0= if  recovering f@ 0.1666653e f+ recovering f!  then ;
\ the fear's scale: Torva's doubling while it lasts (the progress' +0x9E8); the calming by her
\ earrings (Panic_Update: the Inquisitor's and the Philosopher's 1.25, the Alchemist's 2)
: fear-scale ( F: -- k )  fear-boost boost-on? if  fear-boost boost-value  else  scale f@  then ;
: earrings ( F: -- k )
    1 equipped case
        inquisitor-earrings of  1.25e  endof   philosopher-earrings of  1.25e  endof
        alchemist-earrings of  2e  endof   >r 1e r>
    endcase ;
: charmed ( F: -- k )   \ (Panic_Fright: the Philosopher's earrings three quarters, the Alchemist's half)
    1 equipped case  philosopher-earrings of  0.75e  endof  alchemist-earrings of  0.5e  endof  >r 1e r>  endcase ;
\ how this frame's told inputs (the story's, time's) weigh against her own: by `weighing`
begin-structure /weights  ffield: w-fright  ffield: w-fear  ffield: w-calm  end-structure
create weights   \ (fright, fear, calming)
    1e f,    1e f,    1e f,
    0.6e f,  0.6e f,  2e f,
    0.8e f,  0.8e f,  1.6e f,
    0.9e f,  0.9e f,  1.5e f,
: weight ( field-xt -- ) ( F: -- k )  weighing @ /weights * weights + swap execute f@ ;
: frights ( F: -- x )  fright-a f@  fright-a2 f@ ['] w-fright weight f* f+  fear-scale f* ;
: fears ( F: -- x )    fear-d f@    fear-d2 f@ ['] w-fear weight f* f+  fear-scale f* ;
: calming ( F: -- x )  calm-b f@    calm-b2 f@ ['] w-calm weight f* f+  earrings f* ;
: level-from-parts ( -- )  lasting f@ passing f@ f+ level f! ;
: cap-at-99 ( -- )   \ (it reaches 100 only under flag 0x15 - as the original: never)
    level f@ 100e f< 0= if  99e level f!  99e lasting f!  then ;
: weigh-inputs ( -- )
    frights fears calming  {: F: fright fear calmed :}
    passing f@ recovering f@ f- 0e fmax passing f!
    lasting f@ calmed f- 0e fmax fear f+ lasting f!
    fright f0> if   \ a fright: the passing part folds into the lasting, the fright is the new one
        lasting f@ passing f@ f+ lasting f!  fright passing f!  level-from-parts
    else
        level-from-parts  cap-at-99
    then
    panic-can-max state-flag? if  level f@ 100e f< 0= if  cap-at-99  0e passing f!  then  then ;
: update ( -- )
    staging
    hold @ if  -1 hold +!  then
    level f@ 100e f< if  time-inputs  weigh-inputs  then
    inputs-clear ;

: updating? ( -- flag )  cam-scene? 0=  panic-held state-flag? 0= and ;

behaviour measuring
  on spawned ( -- )  1e scale f!  self subscribe tick  self subscribe frame-end  self subscribe danger  self subscribe fiona-doing ;
  on danger ( level -- )  the-danger ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  2drop her-cond !  her-sub !  her-mode ! ;
  \ the story's (the scripts' 0x43, 0xC0, 0x93, 0xC9): the threat (Threat_Raise: as a fright, into
  \ the weighted parts), its accumulator, calming (bit $80: 128 at once; else v / 30), the level
  \ set to one it hasn't passed
  on threat ( amount -- )
      cell>f fdup f0< if  fdrop exit  then
      fdup 10e f< if  fear-d2 f@ f+ fear-d2 f!  exit  then
      0.5e f*  30 hold !  fdup fear-d2 f@ f+ fear-d2 f!  fright-a2 f@ f+ fright-a2 f! ;
  on threat-acc ( amount -- )  cell>f 0e fmax 100e fmin fear-d2 f@ f+ fear-d2 f! ;
  on calm-in ( v -- )
      dup $80 and if  drop 128e calm-b f@ f+ calm-b f!  else  s>f 30e f/ calm-b2 f@ f+ calm-b2 f!  then ;
  on panic-reach ( level -- )  dup s>f level f@ f< if  drop  else  set-level  then ;
  on fright ( amount -- )  cell>f charmed f*  add-fright ;   \ Panic_Fright
  on fright-raw ( amount -- )  cell>f add-fright ;            \ Panic_FrightRaw
  on panic-shorten ( frames -- )  negate length +! ;           \ (+0x7BA: her shaking as she gets up)
  on fear-set ( amount -- )  cell>f fear-d f! ;               \ (+0x7D8 = 1000: running into a wall, flung down panicking)
  on fear-in ( amount -- )   \ (a negative one is a fright: held 30, 50 lasting and passing)
      cell>f fdup f0< if  fdrop 30 hold !  fear-d f@ 50e f+ fear-d f!  fright-a f@ 50e f+ fright-a f!  exit  then
      0.0333333e f* fear-d2 f@ f+ fear-d2 f! ;
  on panic-stage! ( stage -- )  set-stage ;
  \ the items' remedies (b5_add_7E0, the progress' +0x7D8): calming at once - past the lasting
  \ part it eases the recovering too - and fear
  on soothe ( calm fear -- )
      s>f fear-d f@ f+ fear-d f!
      s>f fdup calm-b f@ f+ calm-b f!  lasting f@ f-  fdup f0< if  fdrop  else  recovering f@ f+ recovering f!  then ;
  on panic-level! ( level -- )  set-level ;
  \ (SceneGame_PlayStart: not while a cutscene has the camera nor under flag $F; first in the frame,
  \ so the camera is still unless her panic - or the story, after it - shakes it)
  on tick ( -- )  updating? if  jolting @ if  0.2e  else  0e  then  cam-shake!  then ;
  on frame-end ( -- )  updating? if  update  then  stage @ level f@ f>cell broadcast panic ;
end-behaviour

: panic-spawn ( -- id )  measuring panic-state s" panic" spawn ;
