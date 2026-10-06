\ vectors.fs - 3D vectors on the float stack ( F: x y z ), and the camera's position and angles.

fvariable k
\ scale a vector
: v* ( F: x y z s -- x' y' z' )  k f!  k f@ f* frot  k f@ f* frot  k f@ f* frot ;

\ a 32-bit float field: read, write, add to
: field@ ( addr -- ) ( F: -- x )  sf@ ;
: field+! ( addr -- ) ( F: dx -- )  dup sf@ f+ sf! ;

: yaw   ( F: -- a ) camera cam.yaw sf@ ;
: pitch ( F: -- a ) camera cam.pitch sf@ ;
: world-up ( F: -- 1|-1 ) camera cam.up sf@ ;

\ where the camera looks, and its right hand (level)
: forward ( F: -- x y z )
    yaw fsin pitch fcos f*   pitch fsin world-up f*   yaw fcos pitch fcos f* fnegate ;
: right ( F: -- x y z )  yaw fcos  0e  yaw fsin ;

\ move the camera by a vector; put it somewhere
: cam-move ( F: dx dy dz -- )  camera cam.z field+!  camera cam.y field+!  camera cam.x field+! ;
: cam-at ( F: x y z -- )  camera cam.z sf!  camera cam.y sf!  camera cam.x sf! ;
: cam-pos ( F: -- x y z )  camera cam.x sf@  camera cam.y sf@  camera cam.z sf@ ;
: .cam  cam-pos frot f. fswap f. f.  ." yaw " yaw f. ." pitch " pitch f. cr ;
