\ story/places/off-map-e9.fs - the clock run's goal in the castle ($E9, front-garden-1's ground again):
\ the day's light in the run's second kind - else the night's look (the blend, the lamps lit with
\ their glows, the specks under them); in the run's fourth kind the lamps flicker as Fiona passes
\ under them, and now and then by themselves; the near depth range on its camera 2; the walls'
\ shade; out by exit 0; its cameras; Fiona reaching the goal - the time kept and shown, the results,
\ out to the extras room (story/places/clock-run.fs); the clock. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/off-map-e9.fs (converted from the game's, not
\ loaded).
IN: story.places.off-map-e9
USING: engine game-state actors messages common flag-names facts room-names effects story.kit
    story.clock story.places.clock-run ;

own-reset
own: reached   \ Fiona has reached the goal (her and Hewie's reaction made): the time shown as kept
own: lamp-0  own: lamp-1  own: lamp-2  own: lamp-3   \ each lamp's flicker (its activity; 0 none)

$2E constant run-kind-var   3 constant first-lamp-part   3 constant first-glow   $10 constant goal-area
: lamp-spot ( g -- ) ( F: -- x z )   \ (the four glows' spots, 100.8 up)
    case  0 of  -332e -12e  endof  1 of  -368e 55e  endof  2 of  -368e 165e  endof  >r -332e 165e r>  endcase ;
: lamp ( g on -- )   \ (its part, its glow)
    {: g on :}  g first-lamp-part + on part
    on if  first-glow g +  $80808018  g lamp-spot {: F: x z :}  x 100.8e z 30e glow-at
    else  first-glow g + effect-off  then ;
: blinks ( g n -- )  {: g n :}  n 0 do  g true lamp  yield  g false lamp  yield  loop ;   \ (on and off, a frame each)
: flickers ( g -- )   \ (off, a blink, longer blinks by chance, on again)
    {: g :}  g false lamp  yield  g true lamp  yield  g false lamp  4 frames
    50 chance? if  g true lamp  2 frames  g false lamp  yield  g true lamp  yield  g false lamp  8 frames  then
    25 chance? if  g true lamp  2 frames  g false lamp  yield  g true lamp  yield  g false lamp  16 frames  then
    5 chance? if  g 2 blinks  32 frames  then
    g true lamp  yield  g false lamp  yield  g true lamp ;
: flicker-0 ( -- )  0 flickers ;   : flicker-1 ( -- )  1 flickers ;
: flicker-2 ( -- )  2 flickers ;   : flicker-3 ( -- )  3 flickers ;
: lamp-act ( g -- addr )  cells lamp-0 + ;   \ (own cells in a row)
: flicker-lamp ( g -- )   \ (afresh: one already flickering starts over)
    dup lamp-act @ ?dup if  stop-act  then
    dup case  0 of  ['] flicker-0  endof  1 of  ['] flicker-1  endof  2 of  ['] flicker-2  endof  >r ['] flicker-3 r>  endcase
    start swap lamp-act ! ;
: by-chance ( g -- )  30 chance? if  flicker-lamp  else  drop  then ;
: lamps-flicker ( -- )   \ (in the run's fourth kind: as she passes under them; and, rarely, by itself)
    run-kind-var var@ 3 <> if  exit  then
    fiona $C entered?  fiona $C left? or if  0 by-chance  then
    fiona $D entered?  fiona $D left? or if  1 by-chance  then
    fiona $E entered? if  2 by-chance  then
    fiona $E left? if  3 by-chance  then
    20 chance? 0= if  exit  then
    1 chance? if  0 flicker-lamp  else  1 chance? if  1 flicker-lamp  else  1 chance? if  2 flicker-lamp  else  1 chance? if  3 flicker-lamp  then  then  then  then ;
: night-look ( -- )   \ (the night's blend, the lamps lit, the specks under them)
    $80C0F2AA 0 screen-blend
    6 0 do  first-lamp-part i + show-part  loop  4 0 do  i true lamp  loop
    $10 $A $80 $80 $80 $40 rgba -368e 100e 55e specks      $A $E $80 $80 $80 $40 rgba -332e 100.8e -12e specks
    $10 $E $80 $80 $80 $40 rgba -332e 100.8e 165e specks   $10 $C $80 $80 $80 $40 rgba -368e 100.8e 165e specks
    $A $E $80 $80 $80 $50 rgba -327.5e 82e 236.3e specks   $A $E $80 $80 $80 $50 rgba -372.5e 82e 236.3e specks ;
: came-in ( exit -- )
    drop  run-kind-var var@ 1 = if  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  else  night-look  then
    0 hide-part ;

\ ---- every frame ----
create cameras  6 , 1 , 1 ,  7 , 0 , -1 ,  8 , 2 , 0 ,  9 , 0 , -1 ,   \ (area, set, path)
: near-range ( -- )   \ (on its camera 2 the near depth range; else none - as the setup changes)
    cam-changed? 0= if  exit  then
    cam-set@ 2 = if  1e 1e 100e 200e depth-range  else  depth-range-off  then ;
: walls-shade ( -- )
    -370e 76e 178e  -370e 76e 109e  -370e 20e 178e  -370e 20e 109e  light-doorway
    cam-set@ 2 = if  -380e 74e 250e  -368e 74e 175e  -380e 22e 250e  -368e 22e 175e  light-doorway  then
    -370e 76e 110e  -370e 76e 41e  -370e 20e 110e  -370e 20e 41e  light-doorway
    -370e 76e 38e  -370e 76e -8e  -370e 20e 39e  -370e 20e -8e  light-doorway ;
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at  lamps-flicker
    reached @ 0=  fiona goal-area in-area? and if  -1 reached !  false at-goal  then
    near-range  walls-shade  reached @ clock-draw-saved ;
: person-in ( i -- )  player person-of = if  0 0 -1 way-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e9 place!
