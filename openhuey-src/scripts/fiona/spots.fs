\ fiona/spots.fs - walking to a spot and facing a heading there (Fiona_DoorFrame): far off
\ she follows a planned way, turning 8 degrees a frame; within 7 she settles how to come round
\ to the heading - a quarter turn (0x400 / 0x401) or a half turn (0x402) on the way, else a
\ straight last part - and ends with a stop. Doors and the story's moves use it.
IN: fiona.spots
USING: engine messages vectors common paths fiona.state fiona.anims fiona.model fiona.moving ;

\ the walk's flags (her-wk): turning left, a quarter turn, a half turn, the last part, there,
\ stopping, the last part started, the turn animation half weighted
1 constant leftward-wk   2 constant quarter-turn-wk   4 constant half-turn-wk   8 constant last-part-wk
$10 constant there-wk   $20 constant stopping-wk   $40 constant last-started-wk   $100 constant half-weighted-wk
quarter-turn-wk half-turn-wk or constant turning-on-way-wk   last-started-wk stopping-wk or constant ending-wk
: wk? ( bit -- flag )  her-wk @ and 0<> ;
: wk+ ( bit -- )  her-wk @ or her-wk ! ;
\ to a spot (its triangle, point, heading); then the walk is run by walk-to-spot
: walk-spot ( tri -- ) ( F: x y z yaw -- )
    her-heading f!  her-spot vec!  her-spot-tri !  her-path path-clear  0 her-wk ! ;
: turn-left-to ( F: -- |d| )  her-heading f@ her-yaw f@ f- angle-wrap fabs ;
: turn-anim ( anim -- ) ( F: k -- )   \ the turn blended with her standing pose by k
    her-mode @ 0= if  1 her-sub !  then
    dup entry 8 or >r >r  idle-base r> r> start          \ (Motion_PlayOwnBlend: idle as the variant)
    fdup 1e f< if
        fdup 0.5e f< if  fdrop 0.5e weight!  her-wk-speed f@ 2e f* her-wk-speed f!  half-weighted-wk wk+
        else  weight!  then
    else  fdrop 1e weight!  half-weighted-wk wk+  then ;
: arrive-spot ( -- 0 )
    her-spot-tri @ her-tri !  her-at her-spot vec-copy  her-heading f@ her-yaw f!  there-wk wk+  0 ;
: rest-of-way ( F: -- d )  her-path her-at path-rest ;
: plan-to-spot ( -- ok? )  her-path her-tri @ her-at her-spot-tri @ her-spot blocked-floor path-plan-curved 0> ;   \ (curved: Character_WaypointsCurve)
\ settle how to come round to the heading for the last part
: settle-last-part ( -- )
    last-part-wk wk+
    her-heading f@  her-spot v.x sf@ her-at v.x sf@ f-  her-spot v.z sf@ her-at v.z sf@ f-  fatan2  f- angle-wrap   ( F: d )
    fdup fabs 45e deg>rad f> if
        fdup fabs 135e deg>rad f> if  half-turn-wk wk+  else  quarter-turn-wk wk+  fdup f0< if  leftward-wk wk+  then  then
    then  fdrop ;
: start-last ( -- )   \ (the way's last 7: how to come round)
    rest-of-way fdup 0.5e f< if
        fdup 0.2e f* her-wk-step f!  fdrop  turn-left-to 0.2e f* her-wk-turn f!  stopping-wk wk+  -1 idle-anim
    else
        last-started-wk wk+
        half-turn-wk wk? if
            0e her-wk-step f!  fdup 3.15e f/ her-wk-speed f!  fdrop  turn-left-to 0.05e f* 2e f* her-wk-turn f!
            turn-round-anim her-wk-speed f@ turn-anim
        else quarter-turn-wk wk? 0= if
            10 her-wk-n !  fdup 0.072e f* her-wk-step f!  fdrop  turn-left-to 0.072e f* her-wk-turn f!  walk-look
        else
            leftward-wk wk? 0= if  fdup 4.56e f/ her-wk-speed f!  turn-left-anim her-wk-speed f@ turn-anim
            else  fdup 3.94e f/ her-wk-speed f!  turn-right-anim her-wk-speed f@ turn-anim  then
            fdrop  0e her-wk-step f!  turn-left-to 0.045e f* 2e f* her-wk-turn f!
        then then
    then ;
\ a frame's walk: 0 there, 1 on the way, -1 she can't (no way, a wall)
: walk-to-spot ( -- r )
    there-wk wk? if  0 exit  then
    last-part-wk wk? 0= if
        her-path path-left? 0= if  plan-to-spot 0= if  root-move -1 exit  then  then
        rest-of-way 7e f< if
            settled? 0= if  root-move  her-path path-end  1 exit  then
            settle-last-part
        then
    then
    last-part-wk wk? 0= if   \ on the way: the step by how well she faces its next point, turning 8 degrees
        settled? if  walk-look  then
        her-path her-at 3e path-ahead drop  her-at ahead vec-heading         ( F: yaw )
        root-step fnip 0e fmax                                                ( F: yaw fwd )
        fover her-yaw f@ f- fcos 1e f+ 2e f/ f* her-wk-step f!
        her-yaw f@ fswap 8e deg>rad turn-toward fdrop her-yaw f!
    else
        ending-wk wk? 0= if  start-last  then
        turning-on-way-wk wk? if   \ turning on the way: the step is the turn's root motion
            0e her-wk-step f!
            settled? if
                root-step  quarter-turn-wk wk? if  fdrop  else  fnip  then  fabs
                half-weighted-wk wk? if  her-wk-speed f@ f*  then  0.05e fmax her-wk-step f!
            then
        else  her-wk-step f@ 0.05e fmax her-wk-step f!  then
        her-wk-turn f@ 0.5e deg>rad fmax her-wk-turn f!
    then
    \ the step along the way
    last-part-wk wk? if  her-wk-turn f@ turn-to-heading f0=  else  false  then         ( turned? )
    her-path her-at her-wk-step f@ path-ahead                                ( turned? i )
    ahead blocked-floor v-tri-in dup 0< if  2drop drop -1 exit  then
    her-tri !  her-at ahead vec-copy  her-path path-i!                       ( turned? )
    last-part-wk wk? 0= if  drop 1 exit  then
    -1 her-wk-n +!
    settled? 0= if  drop 1 exit  then
    turning-on-way-wk wk? if
        her-path path-left? if  drop 1 exit  then
        if  stopping-wk wk? if  arrive-spot exit  then  stopping-wk wk+  then
        group if  idle-base 5 -1 play-blend  then
        1 exit
    then
    stopping-wk wk? if  if  arrive-spot  else  1  then  exit  then  drop
    her-wk-n @ 0> if  1 exit  then
    her-wk @ ending-wk and last-started-wk <> if  1 exit  then
    stopping-wk wk+  -1 idle-anim  1 ;
