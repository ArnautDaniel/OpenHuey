\ partner/joint.fs - Hewie in the joint actions the game queues between him and Fiona
\ (src/game/hewie.c Hewie_JointAction, Hewie_FindSpot; progress' commands: relations.fs).
\ Her "stay", "praise" and "scold" by his side (types 4 / 2 / 0): he finds where she is to
\ stand and turns to her (his request 12 with the type); once she stands there and he sits,
\ the second part (types 5 / 3 / 1).
IN: partner.joint
USING: engine game-state events.core chars relations partner.core partner.tables partner.moves ;

\ ---- Hewie_FindSpot: a meeting point (kHewieMeetOffsets[kind]) round him, turning 0, -+10 ..
\ 180 degrees: open floor (not flags 0x80001) that a walk from her reaches ----
create fs-at 12 allot  fvariable fs-ox  fvariable fs-oz  fvariable fs-yaw
: spot-at ( F: yaw -- tri )
    fs-yaw f!
    fs-ox f@ fs-yaw f@ fcos f*  fs-oz f@ fs-yaw f@ fsin f* f+  h-pos sf@ f+  fs-at sf!   \ (Mtx_AtHeading)
    h-pos 4 + sf@ fs-at 4 + sf!
    fs-oz f@ fs-yaw f@ fcos f*  fs-ox f@ fs-yaw f@ fsin f* f-  h-pos 8 + sf@ f+  fs-at 8 + sf!
    fs-at v-tri dup 0< if  exit  then
    dup nav-flags $80001 and if  drop -1 exit  then
    dup  her c-tri her c-pos fs-at $280A0019 v-walk <> if  drop -1  then ;
: find-spot ( kind -- tri | -1 )
    2* floats meet-offsets + dup f@ fs-ox f!  1 floats + f@ fs-oz f!
    19 0 do
        h-yaw i 10 * s>f deg>rad f- angle-wrap spot-at dup 0< 0= if  unloop exit  then  drop
        i 0<> i 18 <> and if
            h-yaw i 10 * s>f deg>rad f+ angle-wrap spot-at dup 0< 0= if  unloop exit  then  drop
        then
    loop  -1 ;

\ post_state2: the request kept for when the action starts (12, the type)
: post2 ( type -- )  him req2# >r  $C r@ l!  r@ 4 + l!  r> 8 + 24 0 fill ;
: her-distance ( F: -- d )  her c-pos h-pos vec-dist ;
: hewie-joint ( -- )   \ Hewie_JointAction
    him cmd? 0= if  exit  then
    h-busy? if  him cmd-cancel exit  then
    him cmd-kind 2 = if
        him cmd-arg case
            0 of  0 endof  2 of  0 endof  4 of  0 endof
            1 of  1 endof  3 of  1 endof  5 of  1 endof
            >r -1 r>
        endcase
        case
            0 of   \ she starts it: he calm, standing (or turning to her), listening, on open
                   \ floor, her near in his room and straight reachable: her place by him
                game-mode @ 0=  h-mode dup 0= swap $C = or and  h-cmd @ $80000008 and 8 = and
                h-tri nav-flags $80001 and 0= and  her with? and  her-distance 30e f< and
                him her c-pos $60088 c-tri-to her c-tri = and if
                    him cmd-arg dup 4 = if  3  else dup 0= if  1  else  2  then then  find-spot
                    dup 0< 0= if
                        her cells move-a + !  fs-yaw f@ her character char.face sf!
                        her character char.target fs-at vec-copy
                        fs-yaw f@ h-to-yaw f!
                        him cmd-start  him cmd-arg post2  exit
                    then  drop
                then
            endof
            1 of   \ the second part: sitting (group 1), settled, facing the agreed way
                h-mode $C = settled? and anim-group 1 = and h-to-yaw f@ h-yaw f= and
                her c-room h-room = and if
                    him cmd-start  him cmd-arg post2  exit
                then
            endof
        endcase
    then
    him cmd-cancel ;
