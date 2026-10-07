\ title.fs - where the game starts: Hewie lying in the dark, close up and warmly lit, by the menu.
\ New Game: he gets up, barks, and the game begins.
\   Up / Down  pick      Enter  take it
IN: title
USING: engine state vectors rooms views player hewie ;

\ the scene: no room, just him in the dark, warm lights on him; the camera close, at an angle
\ round him
fvariable cam-angle  0.9e cam-angle f!    \ radians round him from his front
fvariable cam-dist   20e cam-dist f!
fvariable cam-high   7e cam-high f!
fvariable cam-fov    0.55e cam-fov f!
fvariable cam-off    -0.25e cam-off f!    \ (him a little right of centre: the menu is on the left)
fvariable look-high  3e look-high f!
fvariable cx  fvariable cy  fvariable cz

: warm-lights ( -- )
    \ the key: a warm lamp-light, high in front of him to the side; behind, a low orange glow
    -10e 22e 26e   170e 115e 65e   160e  0 stage-light
     24e 18e -20e   60e 35e 18e    120e  1 stage-light
    2 stage-lights
    10e 8e 6e stage-ambient ;

$002 constant m-lie          \ lying, settled (Hewie_StepToPose's pose 2)
$103 constant m-get-up       \ lying to standing (sPoseInto[0][2])
$1B00 constant m-bark        \ a bark, standing (Hewie_Bark)
$65 constant bark-sound      \ the common bank's loud bark (Hewie_MakeSound)

variable stage   \ 0 the menu, 1 getting up, 2 barking, 3 over
variable picked
: items ( -- n )  2 ;
: item ( i -- addr len )  0= if  s" NEW GAME"  else  s" QUIT"  then ;

: dog ( -- addr )  hewie @ actor ;
: play ( motion loop? -- )  dog act.loop l!  hewie @ swap motion! ;

: frame-him ( -- )   \ the camera round him at cam-angle, looking at his chest
    dog act.yaw sf@ cam-angle f@ f+ fdup fsin cam-dist f@ f* cx f!  fcos cam-dist f@ f* cz f!
    cx f@ dog act.x sf@ f+  dog act.y sf@ cam-high f@ f+  cz f@ dog act.z sf@ f+  cam-at
    dog act.x sf@  dog act.y sf@ look-high f@ f+  dog act.z sf@  look-at
    camera cam.yaw sf@ cam-off f@ f+ camera cam.yaw sf!
    cam-fov f@ camera cam.fov sf! ;

: show-title ( -- )
    -1 title !  0 playing !  0 stage !  0 picked !
    room-clear  -1 view !  0e 0e 0e clear-color  warm-lights
    hewie @ 0< if  s" O_HEW/HEW_000" actor-load hewie !  then
    0e dog act.x sf!  0e dog act.y sf!  0e dog act.z sf!  0e dog act.yaw sf!
    0e dog act.shadow sf!
    m-lie -1 play  frame-him ;

: begin-game ( -- )
    0 title !  3 stage !  0 stage-lights  0.06e 0.06e 0.08e clear-color
    7e dog act.shadow sf!   \ (his contact shadow back: the actor default)
    first-room  start-playing ;

: title-tick ( -- )
    title @ 0= if  exit  then
    frame-him
    stage @ case
        0 of
            key: Up key-pressed? if  picked @ 1- 0 max picked !  then
            key: Down key-pressed? if  picked @ 1+ items 1- min picked !  then
            key: Return key-pressed? if
                picked @ 0= if  m-get-up 0 play  1 stage !  else  bye  then
            then
        endof
        1 of  hewie @ motion-done? if  m-bark 0 play  bark-sound common-sound  2 stage !  then  endof
        2 of  hewie @ motion-done? if  begin-game  then  endof
    endcase ;
' title-tick on-tick

\ the menu: the game's name above, the items low on the left
: title-draw ( -- )
    title @ 0= stage @ 0<> or if  exit  then
    $D8D0C8FF pen-color  4 pen-scale
    s" HAUNTING GROUND" screen-size drop 2 / 15 char-size drop * 2 / -  screen-size nip 8 /  draw-text
    3 pen-scale
    items 0 do
        i picked @ = if  $FFFFFFFF  else  $8C8880FF  then  pen-color
        i item  screen-size drop 10 /  screen-size nip 7 * 10 / i char-size nip 3 2 */ * +  draw-text
    loop
    s" >"  screen-size drop 10 / char-size drop 2 * -  screen-size nip 7 * 10 / picked @ char-size nip 3 2 */ * +
    $FFFFFFFF pen-color draw-text ;
' title-draw on-draw
