\ title.fs - where the game starts: Hewie lying in the dark of a room, close up, under the menu.
\ New Game: he gets up, barks, and the game begins.
\   Up / Down  pick      Enter  take it
IN: title
USING: engine state vectors rooms views player hewie ;

$0E constant title-room
\ where he lies and how we see him
20e fconstant hx0   0e fconstant hy0   60e fconstant hz0   2.2e fconstant hyaw
fvariable cx  fvariable cy  fvariable cz

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

fvariable cam-dist   30e cam-dist f!     \ how far, how high, the view's angle, how far off centre
fvariable cam-high   12e cam-high f!
fvariable cam-fov    0.8e cam-fov f!
fvariable cam-off    -0.35e cam-off f!
fvariable his-yaw    3.0e his-yaw f!     \ which way he lies
: frame-him ( -- )   \ the camera low at his side, looking at his middle, him right of centre
    dog act.x sf@ cam-dist f@ hyaw fcos f* f+ cx f!
    dog act.y sf@ cam-high f@ f+ cy f!
    dog act.z sf@ cam-dist f@ hyaw fsin f* f- cz f!
    cx f@ cy f@ cz f@ cam-at
    dog act.x sf@  dog act.y sf@ 4e f+  dog act.z sf@  look-at
    camera cam.yaw sf@ cam-off f@ f+ camera cam.yaw sf!
    cam-fov f@ camera cam.fov sf! ;

: show-title ( -- )
    -1 title !  0 playing !  0 stage !  0 picked !
    title-room room  -1 view !
    hewie @ 0< if  s" O_HEW/HEW_000" actor-load hewie !  then
    hx0 dog act.x sf!  hy0 dog act.y sf!  hz0 dog act.z sf!  his-yaw f@ dog act.yaw sf!
    hx0 hy0 8e f+ hz0 floor-below if  dog act.y sf!  then
    m-lie -1 play  frame-him ;

: begin-game ( -- )
    0 title !  3 stage !
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
