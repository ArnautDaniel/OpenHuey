\ hewie.fs - Hewie in the game: his model, put by Fiona's side as a room is entered, and his
\ mind each frame (partner.brain - the original's, src/game/hewie.c).
\
\ Fiona's commands to him are keys here until her own port (phase 2) gives them her gestures
\ and her side of meeting him:
\   1  call - go where she points (0x23; 0x2D / 0x2E with a stalker about)
\   2  come back (0x2C)
\   3  stay / come (0x24 by him, 0x25 near, 0x27 far)
\   4  praise (0x28 by him: she pets him; 0x29 from afar)   - hold it to keep petting
\   5  scold (0x2B by him; 0x2F from afar)
IN: hewie
USING: engine state game-state events.core chars partner.core partner.tables partner.moves partner.states partner.actions partner.brain ;

variable hewie        -1 hewie !
variable hewie-room   -1 hewie-room !
variable hewie-ready  \ his motion table set

\ his animations the scripts' moves use (events/play.fs)
$201 constant h-walk
$202 constant h-run
$0 constant h-stand

\ a character's place and triangle from its model (Fiona's moves are still player.fs's own)
: from-actor ( cs -- )
    dup c-actor dup 0< if  2drop exit  then  actor >r
    r@ act.x sf@ r@ act.y sf@ r> act.z sf@  dup c-pos vec!
    dup c-find-tri swap c-tri! ;

\ put him at heel: behind her and a little to her side (or where she is when that isn't floor)
create heel-at 12 allot
: heel ( -- )
    her c-pos vec@                                                ( F: x y z )
    her c-yaw fcos -14e f*  her c-yaw fsin -8e f* f+ f+  frot
    her c-yaw fsin -14e f*  her c-yaw fcos 8e f* f+ f+  frot frot heel-at vec!
    heel-at him c-mask v-tri-in 0< if  heel-at her c-pos vec-copy  then
    heel-at vec@ him c-place!  her c-yaw h-yaw!
    played-room hewie-room ! ;

: spawn-hewie ( -- )
    s" O_HEW/HEW_000" actor-load dup hewie !
    dup 0< if  drop exit  then
    dup $3D5F90 motion-table   \ (his motion table: fades and flags)
    1 character char.actor l! ;

\ ---- Fiona's commands (the keys standing in for her controls) ----
\ Fiona_CallAction: point and send (0x23); with the chase on, at the stalker (0x2D, 0x2E when
\ he is already at it)
: call-action ( -- code )
    game-mode @ 2 = if  hewie-along @ h-mode 8 = and if  $2E  else  $2D  then  exit  then
    $23 ;
: his-distance ( F: -- d )  her c-pos h-pos vec-dist ;
\ Fiona_HewieCommandAction
: command-code ( cmd -- code )
    case
        0 of  call-action  endof
        1 of  $2C  endof
        2 of  game-mode @ 0<> hewie-along @ 0= or if  $27
              else his-distance 30e f< 0= if  $27
              else his-distance 12e f< 0= if  $25
              else h-action @ $4B = him c-sub 3 = h-action @ 2 = and or if  $24  else  $25  then
              then then then  endof
        3 of  hewie-along @ 0= his-distance 15e f< 0= or if  $29
              else h-cond 2 = if  $2A
              else game-mode @ 0<> h-mood @ 3 = or if  $29  else  $28  then
              then then  endof
        4 of  game-mode @ 0<> hewie-along @ 0= or his-distance 15e f< 0= or h-mood @ 3 = or
              if  $2F  else  $2B  then  endof
        >r -1 r>
    endcase ;
: hreq! ( kind arg -- )   \ a request into his state block (unless it holds 7)
    him character char.req sl@ 7 = if  2drop exit  then
    him character char.req-arg l!  him character char.req l! ;
\ Fiona_CommandHewie: her command; for 0x23 the spot - up to 10 ahead of her where she faces
create cmd-at 12 allot
: command-hewie ( code -- )
    req@ if  drop exit  then
    dup $23 = if
        her c-tri h-cmd-tri !  h-cmd-pos her c-pos vec-copy  her c-yaw h-cmd-yaw f!
        her c-yaw 15e her -1 c-free  fdup 5e f> if
            5e f-  her c-yaw fswap  cmd-at her c-pos vec-ahead
            her cmd-at $29020008 c-tri-to dup 0< if  drop  else  h-cmd-tri !  h-cmd-pos cmd-at vec-copy  then
        else  fdrop  then
    then
    13 swap hreq! ;

\ ---- meeting him (0x24 / 0x28 / 0x2B): Hewie_JointAction finds where she stands by him; she
\ walks there, he turns to her, she praises, pets or calms him (her side is phase 2's) ----
variable meet  variable meet-type  variable meet-t   \ 0 none, 1 she goes over, 2 his part
create meet-at 12 allot  fvariable fs-ox  fvariable fs-oz  fvariable fs-yaw
\ Hewie_FindSpot: a heading round him (0, +-10 .. 180 degrees) whose meeting point is open floor
\ she can walk to
: spot-at ( F: yaw -- tri )
    fdup fs-yaw f!
    h-pos vec@  fs-yaw f@ fcos fs-oz f@ f* fs-yaw f@ fsin fs-ox f@ f* f- f+  frot
    fs-yaw f@ fsin fs-oz f@ f* fs-yaw f@ fcos fs-ox f@ f* f+ f+  frot frot  meet-at vec!
    meet-at v-tri dup 0< if  exit  then
    dup nav-flags $80001 and if  drop -1 exit  then
    dup  her c-tri her c-pos meet-at $280A0019 v-walk <> if  drop -1  then ;
: find-spot ( kind -- tri | -1 )
    2* floats meet-offsets + dup f@ fs-ox f!  1 floats + f@ fs-oz f!
    19 0 do
        h-yaw i 10 * s>f deg>rad f- angle-wrap spot-at dup 0< 0= if  unloop exit  then  drop
        i 0<> i 18 <> and if
            h-yaw i 10 * s>f deg>rad f+ angle-wrap spot-at dup 0< 0= if  unloop exit  then  drop
        then
    loop  -1 ;
: can-meet? ( -- flag )   \ the conditions of Hewie_JointAction (types 0 / 2 / 4)
    game-mode @ 0=  h-mode dup 0= swap $C = or and  h-cmd @ $80000008 and 8 = and
    h-tri nav-flags $80001 and 0= and  her with? and  his-distance 30e f< and
    him her c-pos $60088 c-tri-to her c-tri = and ;
: meet-start ( type -- ok? )
    can-meet? 0= if  drop false exit  then
    dup 4 = if  3  else dup 0= if  1  else  2  then then  find-spot 0< if  drop false exit  then
    meet-type !  fs-yaw f@ h-to-yaw f!
    $D her character char.mode l!  1 her character char.scripted l!
    12 meet-type @ hreq!  1 meet !  0 meet-t !  true ;
: meet-end ( -- )  0 meet !  0 her character char.mode l!  0 her character char.scripted l! ;
: her-anim ( anim -- )   \ (her own animations come with phase 2)
    0 c-actor dup 0< if  2drop exit  then  swap                ( id anim )
    2dup has-motion? 0= if  2drop exit  then
    over motion@ over = if  2drop exit  then  motion! ;
: meet-tick ( -- )
    meet @ 0= if  exit  then
    1 meet-t +!  meet-t @ 600 > if  meet-end exit  then
    meet @ 1 = if   \ she walks to the spot, then faces him
        her c-pos meet-at vec-dist-xz 1e f> if
            her meet-at c-heading-to her c-yaw!  0e 0.47e her c-move-local
            $200 her-anim
            meet-t @ 240 < if  exit  then
        then
        her h-pos c-heading-to her c-yaw!  0 her-anim
        h-action @ $48 = settled? and anim-group 1 = and  h-mode $C = and  meet-t @ 300 > or if
            12 meet-type @ 1+ hreq!  2 meet !
        then  exit
    then
    \ his part: until he is through with it
    h-action @ dup $49 <> over $4A <> and swap $4B <> and req@ 0= and if  meet-end  then ;

\ the controls: keys 1 .. 5 (her +0x1AD6B8, -1 none) - held for petting
: key-cmd ( -- cmd )
    key: 1 key-down? if  0 exit  then  key: 2 key-down? if  1 exit  then
    key: 3 key-down? if  2 exit  then  key: 4 key-down? if  3 exit  then
    key: 5 key-down? if  4 exit  then  -1 ;
: key-pressed ( -- cmd )
    key: 1 key-pressed? if  0 exit  then  key: 2 key-pressed? if  1 exit  then
    key: 3 key-pressed? if  2 exit  then  key: 4 key-pressed? if  3 exit  then
    key: 5 key-pressed? if  4 exit  then  -1 ;
: fiona-commands ( -- )
    key-cmd fiona-cmd !
    meet @ if  exit  then
    key-pressed dup 0< if  drop exit  then
    hewie-along @ 0= if  drop exit  then
    command-code dup 0< if  drop exit  then
    dup $24 = if  drop 4 meet-start drop exit  then
    dup $28 = if  drop 2 meet-start 0= if  $29 command-hewie  then  exit  then
    dup $2B = if  drop 0 meet-start 0= if  $2F command-hewie  then  exit  then
    command-hewie ;

\ ---- each frame ----
:noname ( -- flag )  can-command? ; is event-hewie-can-command?
: hewie-tick ( -- )
    playing @ hewie-along @ and paused @ 0= and 0= if  exit  then
    hewie @ 0< if  spawn-hewie  hewie @ 0< if  exit  then  then
    hewie-ready @ 0= if  hewie @ dup $3D5F90 motion-table  1 character char.actor l!  -1 hewie-ready !  then
    played-room dup her character char.room l!  him character char.room l!
    her from-actor  h-busy? if  him from-actor  then
    played-room hewie-room @ <> if  heel  hewie-start  then
    fiona-commands  meet-tick
    hewie-frame ;
' hewie-tick on-tick
