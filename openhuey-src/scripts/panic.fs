\ panic.fs - the panic meter (docs/subsystems/panic.md; the original's Panic_*): a level made of
\ a lasting and a passing part, its stage, Fiona's breath and scream. Frights and fear come in
\ as messages; each frame's end it updates and tells everyone its stage and level.
IN: panic
USING: engine vectors common actors messages facts flag-names stalker-names items items.names ;

state: panic-state
  cell field stage           \ (enum: panic-stage, messages.fs)
  cell field weighing        \ how the inputs weigh (0..3)
  cell field panic-left          \ frames of the panic left (negative: its start)
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
  cell field pulse           \ the heartbeat on the screen: 0 at each breath, back to $80 by 4 a frame (+0x38)
  cell field shake           \ the screen's grey (+0x40): with the level past 60; panicking $50, calming down $90
  cell field jolting         \ the camera shaken by her panic (Panic_Stage: 0.2 past its first stretch)
  1 floats field fade        \ the screen fade's level (+0x34): her breath only at 1 - 0 while the world is held
  cell field fade-held       \ the screen's fade stepped this frame (Events_ScenePlaying): its level not set back to 1
  1 floats field scale       \ the fear's scale (1)
  cell field the-danger      \ (told)
  cell field her-mode  cell field her-sub  cell field her-cond   \ (told by Fiona each frame)
  cell field st-room  cell field st-mode  cell field st-sub  cell field st-cond  cell field st-kind   \ (told by the stalker)
end-state

$29 constant breath-sound   $42 constant scream-sound   \ (hers: the common bank)
: her-sound ( id vol -- )   \ a sound of hers (the common bank), at her
    fiona-id {: id vol her :}  her body? 0= if  exit  then
    id 5 vol 0  her body-pos sound-at ;
: inputs-clear ( -- )   \ (Panic_Update's end: the passing part stays - only setting the level clears it)
    0 weighing !  0e fright-a f!  0e fright-a2 f!  0e recovering f!
    0e fear-d f!  0e fear-d2 f!  0e calm-b f!  0e calm-b2 f! ;
450 constant panic-frames   \ (how long a panic lasts)
: stage-at ( level -- stage )
    dup 90 >= if  drop terrified exit  then
    dup 75 >= if  drop scared exit  then
    60 >= if  shaken  else  composed  then ;
\ Panic_SetLevel: the stage by the level (at 100 its length)
: set-level ( n -- )
    dup s>f level f!  dup s>f lasting f!
    dup 100 >= if  drop panic-frames panic-left !  else  stage-at stage !  then
    0e passing f!  inputs-clear ;
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
: breath ( -- )   \ (only with the screen fade full: not while the world is held, nor in a fade)
    fade f@ 1e f= 0= if  exit  then
    stage @ shaken calming-down 1+ within 0= if  exit  then
    stage @ breath-rumble
    breath-sound  stage @ breath-of breath-volume @  her-sound ;

\ ---- Panic_Stage ----
\ her panic's start, from her scream for 30 frames (SceneGame +0xF6CD24 bit 3: the story's action
\ scripts held, SceneGame_Busy busy)
: onset ( on -- )  broadcast panic-onset ;
: on-her-feet? ( -- flag )  her-mode @ dup 0= swap 3 = or ;
: by-level ( -- )
    level f@ 100e f< 0= if   \ she panics (on her feet: her scream)
        100e level f!  100e lasting f!  0e passing f!
        on-her-feet? if  scream-sound $40 her-sound  panicking stage !  -30 panic-left !  0 beat !  panicked  -1 onset  then
        exit
    then
    level f@ f>s stage-at stage ! ;
: shake-toward ( n -- )   \ the shake a step nearer n
    dup shake @ > if  drop 1 shake +!  else  shake @ < if  -1 shake +!  then  then ;
: uneasy ( -- )   \ not yet panicking: her breath by the stage, the shake by the level (just
    \ panicking too: its first breath at once, the shake toward 60)
    stage @ composed = if  shake @ 8 - 0 max shake !  then
    by-level
    stage @ composed <> beat @ 0= and if  panicking stage @ - 30 * beat !  0 pulse !  breath  then
    level f@ 60e f< if  exit  then
    level f@ 60e f- 1.5e f* f>s shake-toward ;
: panic-length ( -- frames )   \ longer hurt or down
    panic-frames  her-cond @ case  1 of  300 +  endof  3 of  150 +  endof  endcase ;
: staging ( -- )
    0 jolting !
    pulse @ $80 < if  4 pulse +!  pulse @ $80 min pulse !  then
    beat @ if  -1 beat +!  then
    stage @ panicking < if  uneasy exit  then
    stage @ panicking = if
        panic-left @ 0< if   \ its start: then its length
            1 panic-left +!  panic-left @ 0= if  panic-length panic-left !  0 onset  then  $80 pulse !  exit
        then
        fear-d f@ 500e f< 0= if  panic-left @ 150 + panic-frames min panic-left !  calming-down stage !  then
    then
    -1 panic-left +!
    panic-left @ 0< if  composed stage !  50e level f!  50e lasting f!  0e passing f!  exit  then
    stage @ panicking <> if  $90  -1 jolting !  else  $50  then  shake !
    beat @ if  exit  then
    6 stage @ - 7 * beat !  0 pulse !  breath ;

\ ---- the screen (ScreenFade_Frame, its level full in play): at the panic's start the negative; each
\ heartbeat the frame held and blended in again as the pulse comes back (the first frames also
\ blurred bigger over itself); the grey by the shake - the dark black, the rest pale. With the
\ level under full (a fade's frames, the world held) none of it but the grey, by the level, and the
\ camera still: panicking or calming down, or the heartbeat cut short ----
$30 constant grey-limit
: full? ( -- flag )  fade f@ 1e f= ;
: camera-still ( -- )  0 jolting !  0e cam-shake! ;
: panicked? ( -- flag )  stage @ panicking calming-down 1+ within ;
: screens ( -- )
    panicked? if
        full? if  stage @ panicking =  panic-left @ 0< and if  panic-negative  then
        else  camera-still  then
    then
    pulse @ $80 <> if
        full? if  pulse @ frame-blend  pulse @ $10 <= if  zoom-blur  then
        else  camera-still  $80 pulse !  then
    then
    shake @ if  grey-limit shake @ s>f fade f@ f* f>s panic-tint  then ;   \ (the overlay tinted by its level)

\ ---- Panic_FearInputs (calm with time; the stalker's part comes as fear-in) and Panic_Update ----
: time-inputs ( -- )
    the-danger @ calm = if  calm-b2 f@ 0.0999998e f+ calm-b2 f!
    else her-mode @ 0= her-sub @ 0= and if  calm-b2 f@ 0.0333333e f+ calm-b2 f!  then then
    hold @ 0= if  recovering f@ 0.1666653e f+ recovering f!  then ;
\ the stalker's part (Panic_FearInputs): by his kind and how near he is - the level distance and
\ three times the rise between them (both on a ladder, by the rise alone); a negative amount is a
\ fright (held 30 frames, 50 lasting and passing). In the mini-game's run, she out of sight with
\ him, her fear grows; not while he is down, nor under flag $21
: fear-add ( F: amount -- )
    fdup f0< if  fdrop 30 hold !  fear-d f@ 50e f+ fear-d f!  fright-a f@ 50e f+ fright-a f!  exit  then
    0.0333333e f* fear-d2 f@ f+ fear-d2 f! ;
: fear-kind ( kind -- row | -1 )   \ (pursuer_kind)
    case
        5 of  3  endof  4 of  2  endof
        $24 of  1  endof  $23 of  1  endof  $22 of  1  endof  3 of  1  endof
        $1B of  0  endof  7 of  0  endof  6 of  0  endof  2 of  0  endof
        >r -1 r>
    endcase ;
create near-fears   4e f, 2e f, 1.5e f, 1e f, 0.5e f,   \ (D_0041A0A0: within 10, 20, 40, 60, 100; the
    2e f, 1.8e f, 1.5e f, 1e f, 0.75e f,   2e f, 1.8e f, 1.5e f, 1e f, 0.75e f,   2e f, 1.8e f, 1.5e f, 1e f, 0.75e f,   \ rows by kind)
create rise-fears   -1e f, 8e f, 6e f, 4e f, 2e f, 1e f,   \ (D_0041A0F0: both on a ladder - a rise within 20, 30, 40, 50, 60, 100; every kind's alike)
: band ( F: d -- ) ( limits n -- i | -1 )   \ (the first limit d is under; past them all -1)
    {: lims n F: d :}  n 0 do  d lims i floats + f@ f< if  i unloop exit  then  loop  -1 ;
create near-limits  10e f, 20e f, 40e f, 60e f, 100e f,
create rise-limits  20e f, 30e f, 40e f, 50e f, 60e f, 100e f,
: her-away-played? ( -- flag )   \ Hewie played, she out of his room (no body - the title: not)
    hewie-played? 0=  fiona-id body? 0= or if  false exit  then
    fiona-id body-room room-id <> ;
: stalker-fear ( -- )
    stalker-in-game {: s :}  s 0<  st-cond @ 2 = or  fiona-id body? 0= or if  exit  then
    fiona-id body-room room-id <>  fiona-id body-room s body-room = and if
        hewie-played? if  0.0666667e fear-d2 f@ f+ fear-d2 f!  then  exit
    then
    s body-room room-id <>  stalker-no-fear state-flag? or if  exit  then
    st-kind @ fear-kind {: row :}  row 0< if  exit  then
    s body-at fiona-id body-at  {: a b :}
    a v.x sf@ b v.x sf@ f-  a v.z sf@ b v.z sf@ f-  vlen  a v.y sf@ b v.y sf@ f- fabs {: F: level F: rise :}
    level rise 3e f* f+  near-limits 5 band  dup 0< 0= if  row 5 * + floats near-fears + f@ fear-add  else  drop  then
    her-mode @ 3 =  her-sub @ 7 = and  st-mode @ 3 = and  st-sub @ 7 = and 0= if  exit  then
    rise rise-limits 6 band  dup 0< 0= if  floats rise-fears + f@ fear-add  else  drop  then ;

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
    \ (under flag $15 - or in the run, she out of the room played - it stops at 99)
    panic-can-max state-flag?  her-away-played? or if
        level f@ 100e f< 0= if  cap-at-99  0e passing f!  then
    then ;
: update ( -- )
    world-held state-flag? if  0e fade f!  then
    staging
    hold @ if  -1 hold +!  then
    level f@ 100e f< if  time-inputs  stalker-fear  weigh-inputs  then
    inputs-clear ;

: updating? ( -- flag )  cam-scene? 0=  panic-held state-flag? 0= and ;

behaviour measuring
  on spawned ( -- )  1e scale f!  1e fade f!  2 st-cond !  $80 pulse !  self subscribe hewie-near  self subscribe tick  self subscribe frame-end  self subscribe danger  self subscribe fiona-doing  self subscribe stalker-doing  self subscribe write-save ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )  2drop st-kind !  2drop st-cond !  st-sub !  st-mode !  st-room ! ;
  on danger ( level -- )  the-danger ! ;
  on hewie-near ( near -- )  weighing ! ;   \ (report_fiona_near: Hewie with her - 1 within 20, 2 within 50, 3 further)
  on panic-overlay ( thousandths -- )  s>f 1000e f/ 0e fmax 1e fmin fade f! ;   \ (ScreenFade_Level: the game over's fade, the screen's steps)
  on overlay-held ( -- )  -1 fade-held ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  2drop her-cond !  her-sub !  her-mode ! ;
  \ the story's (the scripts' 0x43, 0xC0, 0x93, 0xC9): the threat (Threat_Raise: as a fright, into
  \ the weighted parts), its accumulator, calming (bit $80: 128 at once; else v / 30), the level
  \ set to one it hasn't passed
  on threat ( amount -- )
      cell>f fdup f0< if  fdrop exit  then
      fdup 10e f< if  fear-d2 f@ f+ fear-d2 f!  exit  then
      0.5e f*  30 hold !  fdup fear-d2 f@ f+ fear-d2 f!  fright-a2 f@ f+ fright-a2 f! ;
  on calm-add ( amount -- )  cell>f calm-b2 f@ f+ calm-b2 f! ;   \ (Lavender's, each frame: StatusTimers_Frame)
  on threat-acc ( amount -- )  cell>f 0e fmax 100e fmin fear-d2 f@ f+ fear-d2 f! ;
  on calm-in ( v -- )
      dup $80 and if  drop 128e calm-b f@ f+ calm-b f!  else  s>f 30e f/ calm-b2 f@ f+ calm-b2 f!  then ;
  on panic-reach ( level -- )  dup s>f level f@ f< if  drop  else  set-level  then ;
  on fright ( amount -- )  cell>f charmed f*  add-fright ;   \ Panic_Fright
  on fright-raw ( amount -- )  cell>f add-fright ;            \ Panic_FrightRaw
  on panic-shorten ( frames -- )  negate panic-left +! ;           \ (+0x7BA: her shaking as she gets up)
  on fear-set ( amount -- )  cell>f fear-d f! ;               \ (+0x7D8 = 1000: running into a wall, flung down panicking)
  on fear-in ( amount -- )  cell>f fear-add ;   \ (a negative one is a fright: held 30, 50 lasting and passing)
  on panic-stage! ( stage -- )  set-stage ;
  \ the items' remedies (b5_add_7E0, the progress' +0x7D8): calming at once - past the lasting
  \ part it eases the recovering too - and fear
  on soothe ( calm fear -- )
      s>f fear-d f@ f+ fear-d f!
      s>f fdup calm-b f@ f+ calm-b f!  lasting f@ f-  fdup f0< if  fdrop  else  recovering f@ f+ recovering f!  then ;
  on panic-level! ( level -- )  set-level ;
  \ (saves.fs) kept in a save as the original's progress +0x7B0 is (Progress_Sub7B0): her stage,
  \ her panic's time, the fright's hold, her next breath, the level and its parts; loaded, back
  on write-save ( -- )
      stage @ .  panic-left @ .  hold @ .  beat @ .  level f@ f>cell .  passing f@ f>cell .  lasting f@ f>cell .
      ." panic-was" cr ;
  on panic-restore ( stage left hold beat level passing lasting -- )
      cell>f lasting f!  cell>f passing f!  cell>f level f!  beat !  hold !  panic-left !  stage !  inputs-clear ;
  on panic-reset ( -- )   \ (Panic_Reset)
      composed stage !  0 panic-left !  0 hold !  0 beat !  0e level f!  0e passing f!  0e lasting f!  inputs-clear
      $80 pulse !  0 shake !  0 jolting !  1e fade f! ;
  \ (SceneGame_PlayStart: not while a cutscene has the camera nor under flag $F; first in the frame,
  \ so the camera is still unless her panic - or the story, after it - shakes it)
  on tick ( -- )  updating? if  jolting @ if  0.2e  else  0e  then  cam-shake!  then ;
  on frame-end ( -- )
      \ (in play the level back to full - not in a frame of the screen's fade; a scene's: as it is)
      updating? if  update  fade-held @ 0= if  1e fade f!  then  screens  else  $80 pulse !  then
      0 fade-held !
      stage @ level f@ f>cell broadcast panic ;
end-behaviour

: panic-spawn ( -- id )  measuring panic-state s" panic" spawn ;
