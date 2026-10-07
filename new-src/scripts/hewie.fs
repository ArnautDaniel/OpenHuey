\ hewie.fs - Hewie keeps up with Fiona: he trots when she gets a little away, runs when she is
\ far, and stands watching her when she's close. On a new room he arrives at her side.
IN: hewie
USING: engine vectors state player ;

variable hewie        -1 hewie !
variable hewie-room   -1 hewie-room !

$201 constant h-walk
$202 constant h-run
$1B03 constant h-stand

fvariable h-walk-speed  0.7e h-walk-speed f!    \ (a tick: 30 a second)
fvariable h-run-speed   2.0e h-run-speed f!
fvariable h-turn        0.3e h-turn f!
22e fconstant close-enough      \ stop this near her
45e fconstant far-away          \ run beyond this
300e fconstant lost             \ further than this: he catches up at once

: him ( -- addr ) hewie @ actor ;
: his-pos ( F: -- x y z )  him act.x sf@  him act.y sf@  him act.z sf@ ;

\ from him to her, on the ground
: to-her ( F: -- dx dz )  her act.x sf@ him act.x sf@ f-  her act.z sf@ him act.z sf@ f- ;
: her-distance ( F: -- d )  to-her fsq fswap fsq f+ fsqrt ;

: turn-to-her
    to-her fatan2  him act.yaw sf@ f- fwrap
    h-turn f@ fnegate h-turn f@ fclamp
    him act.yaw sf@ f+ fwrap him act.yaw sf! ;

\ a step the way he faces, on the nav mesh (anywhere, in a room without one)
: trot ( F: distance -- )
    fdup him act.yaw sf@ fsin f* px f!  him act.yaw sf@ fcos f* pz f!
    nav-tris if
        $29020008 nav-block!                     \ (the triangles that stop him: the game's mask)
        his-pos px f@ pz f@ step-up 2.5e nav-move  0 nav-block!
        him act.z sf!  him act.y sf!  him act.x sf!
    else
        him act.x sf@ px f@ f+ him act.x sf!
        him act.z sf@ pz f@ f+ him act.z sf!
    then ;

\ put him at heel: behind her and a little to her side (or right where she is, if that spot
\ isn't walkable)
fvariable hx  fvariable hz
: heel
    her act.yaw sf@ fsin -14e f*  her act.yaw sf@ fcos 8e f* f+  her act.x sf@ f+  hx f!
    her act.yaw sf@ fcos -14e f*  her act.yaw sf@ fsin -8e f* f+  her act.z sf@ f+  hz f!
    hx f@ her act.y sf@ hz f@ step-up nav-at if
        him act.y sf!  hx f@ him act.x sf!  hz f@ him act.z sf!
    else
        her-pos  him act.z sf!  him act.y sf!  him act.x sf!
    then
    her act.yaw sf@ him act.yaw sf!
    room-id hewie-room ! ;

: spawn-hewie
    hewie @ 0< if  s" O_HEW/HEW_000" actor-load hewie !  then
    heel  hewie @ h-stand motion! ;

: keep-up
    fiona @ 0< if  exit  then
    hewie @ 0< if  spawn-hewie exit  then
    room-id hewie-room @ <>  her-distance lost f> or if  heel exit  then
    her-distance
    fdup far-away f> if  fdrop turn-to-her h-run-speed f@ trot  hewie @ h-run motion!  exit  then
    close-enough f> if  turn-to-her h-walk-speed f@ trot  hewie @ h-walk motion!  exit  then
    hewie @ h-stand motion!  turn-to-her ;

: hewie-tick  playing @ hewie-along @ and paused @ 0= and if  1 scripted? 0= if  keep-up  then  then ;
' hewie-tick on-tick
