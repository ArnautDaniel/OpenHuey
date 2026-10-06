\ views.fs - the room's own camera setups (the views the game cuts between): [ and ] step
\ through them. The free camera keeps working from wherever a view puts it.
IN: views
USING: engine vectors ;

variable view  -1 view !

\ aim the camera from where it is at a point
fvariable tx  fvariable ty  fvariable tz
: look-at ( F: x y z -- )
    tz f! ty f! tx f!
    tx f@ camera cam.x sf@ f-              ( F: dx )
    tz f@ camera cam.z sf@ f- fnegate      ( F: dx -dz )
    fover fover fatan2 camera cam.yaw sf!
    fsq fswap fsq f+ fsqrt                 ( F: horizontal )
    ty f@ camera cam.y sf@ f- fswap fatan2 camera cam.pitch sf! ;

: show-view ( n -- )
    dup view !
    room-camera                            ( F: ex ey ez fov tx ty tz )
    tz f! ty f! tx f!  camera cam.fov sf!  cam-at
    tx f@ ty f@ tz f@ look-at
    ." view " view @ . ." of " room-cameras . cr ;

: step-view ( step -- )
    room-cameras 0= if  drop ." this room has no camera setups" cr exit  then
    view @ + room-cameras + room-cameras mod show-view ;

: view-keys
    key: ] key-pressed? if  1 step-view  then
    key: [ key-pressed? if  -1 step-view  then ;
' view-keys on-tick
