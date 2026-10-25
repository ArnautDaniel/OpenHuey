\ vectors.fs - 3D vectors on the float stack ( F: x y z ), and the camera's position and angles.
IN: vectors
USING: engine ;

\ a point or a direction as the engine keeps them: three 32-bit floats (vec@ vec! vec-copy)
begin-structure /vec  sffield: v.x  sffield: v.y  sffield: v.z  end-structure

\ scale a vector
: v* ( F: x y z s -- x' y' z' )  {: F: x y z s :}  x s f*  y s f*  z s f* ;

\ a stick direction turned into the room by a camera heading (0 looks along -z): the original's
\ Mtx_TurnY of the heading applied to it, negated
: stick-turned ( F: cam-yaw x z -- dx dz )
    {: F: yaw x z :}  yaw fcos x f*  yaw fsin z f* f-   yaw fsin x f*  yaw fcos z f* f+ ;

\ a 32-bit float field: read, write, add to
: field@ ( addr -- ) ( F: -- x )  sf@ ;
: field+! ( addr -- ) ( F: dx -- )  dup sf@ f+ sf! ;

: cam-yaw   ( F: -- a ) camera cam.yaw sf@ ;
: cam-pitch ( F: -- a ) camera cam.pitch sf@ ;
: cam-up ( F: -- 1|-1 ) camera cam.up sf@ ;

\ where the camera looks, and its right hand (level)
: cam-forward ( F: -- x y z )
    cam-yaw fsin cam-pitch fcos f*   cam-pitch fsin cam-up f*   cam-yaw fcos cam-pitch fcos f* fnegate ;
: cam-right ( F: -- x y z )  cam-yaw fcos  0e  cam-yaw fsin ;

\ move the camera by a vector; put it somewhere
: cam-move ( F: dx dy dz -- )  camera cam.z field+!  camera cam.y field+!  camera cam.x field+! ;
: cam-at ( F: x y z -- )  camera cam.z sf!  camera cam.y sf!  camera cam.x sf! ;
: cam-pos ( F: -- x y z )  camera cam.x sf@  camera cam.y sf@  camera cam.z sf@ ;
: .cam  cam-pos frot f. fswap f. f.  ." yaw " cam-yaw f. ." pitch " cam-pitch f. cr ;
