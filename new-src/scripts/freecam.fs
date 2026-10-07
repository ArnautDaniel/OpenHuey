\ freecam.fs - a free-flying camera.
\   W A S D  move     Q E  down / up     Shift  faster
\   arrows or the right mouse button  look around
IN: freecam
USING: engine keys vectors state ;

fvariable speed      600e speed f!      \ units a second
fvariable turn-rate  1.6e turn-rate f!  \ radians a second (keys)
0.004e fconstant mouse-turn             \ radians a pixel

: fly-step ( F: -- distance )  speed f@ dt f*  key: Left_Shift held? if 4e f* then ;
: turn ( F: -- angle )  turn-rate f@ dt f* ;

: turn-yaw ( F: a -- )  camera cam.yaw field+! ;
: turn-pitch ( F: a -- )  camera cam.pitch dup sf@ f+ -1.5e 1.5e fclamp sf! ;

: fly
    key: W held? if  forward fly-step v* cam-move  then
    key: S held? if  forward fly-step fnegate v* cam-move  then
    key: D held? if  right fly-step v* cam-move  then
    key: A held? if  right fly-step fnegate v* cam-move  then
    key: E held? if  0e fly-step world-up f* 0e cam-move  then
    key: Q held? if  0e fly-step fnegate world-up f* 0e cam-move  then ;

: look
    key: Right held? if  turn turn-yaw  then
    key: Left held?  if  turn fnegate turn-yaw  then
    key: Up held?    if  turn turn-pitch  then
    key: Down held?  if  turn fnegate turn-pitch  then
    3 mouse-down? if
        mouse-dx mouse-turn f* turn-yaw
        mouse-dy mouse-turn f* fnegate turn-pitch
    then ;

: freecam  title @ if  exit  then  playing @ 0= if  fly  menu-open @ 0= if  look  then  then ;
' freecam on-tick
