\ fiona/react.fs - Fiona struck or seized (src/game/fiona.c Fiona_Reaction / Fiona_React and
\ the states after): what a request 4 from another makes of her (its kind: 1 / 2 a blow, 4 a
\ hard one, 3 seized, 5 a door, 6 caught, 0xA led away, 0xC held, 0xD a fall) by what she is
\ doing, and her falls and getting up again. Shaking off (Fiona_Shakes): the stick swung
\ about and the buttons.
IN: fiona.react
USING: engine game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain fiona.commands fiona.doors fiona.panic fiona.ladder ;

\ ---- Fiona_Shakes: how much she shakes this frame (the stick flicked over 120 degrees; a
\ button: Space / Return, F, Q, E) ----
fvariable sk-a  variable sk-fresh  1 sk-fresh !
: shakes-now ( -- n )
    hewie-control @ if  0 exit  then
    0  read-stick  sx f@ sz f@ vlen                                  ( n ) ( F: len )
    fdup 0.8e f<= if  0.2e f< if  1 sk-fresh !  then
    else  fdrop  sx f@ sz f@ fatan2
        sk-fresh @ if  1+  0 sk-fresh !  sk-a f!
        else  fdup sk-a f@ f- angle-wrap fabs 2.0943952e f> if  1+  sk-a f!  else  fdrop  then  then
    then
    key: Space key-pressed? key: Return key-pressed? or key: F key-pressed? or
    key: Q key-pressed? or key: E key-pressed? or if  1+  then ;
' shakes-now is shakes

\ ---- Fiona_StartInDoor: leaving what she was doing - off the ladder; at a door she held, it
\ swung back as it was ----
: start-in-door ( -- )
    f-mode 3 = if  fiona.ladder:lad-link @ fiona.ladder:off-ladder  exit  then
    f-mode 2 = if  0 f-2b !  not-yet" Fiona_StartInDoor at a door"  then ;

variable f-104   \ her +0x104[0]: the door of a door's blow; the way she's led; ...
: fl@ ( tri -- flags )  dup 0< if  drop 0  else  nav-flags  then ;
\ ---- Fiona_Reaction: the reaction to a request of `kind` (-1 none) ----
: on-trap? ( -- flag )   \ (her floor a step or a ladder's, in a doorway or on a ladder)
    f-tri fl@ $80003 and if  true exit  then
    8 0 do  i me flags-of $20 and if  true unloop exit  then  loop
    nav-links 0 ?do  i me fiona.ladder:link-bytes 8 and if  true unloop exit  then  loop  false ;
\ (1 / 2 / 4: knocked down - on a step, a ladder or in a doorway the short falls 0xC / 0xD)
: blow-reaction ( mode kind -- r )
    swap dup 4 = swap $A = or if  drop -1 exit  then
    f-mode 3 = f-sub 7 = and if  drop 8 exit  then
    on-trap? if  1 = if  $C  else  $D  then  exit  then
    case  1 of  $E  endof  2 of  $F  endof  4 of  $A  endof  >r -1 r>  endcase ;
: reaction ( kind -- r )
    8 state-flag? if  drop -1 exit  then
    me 2 relations:req-word-of dup $FF <> if
        dup c-active? 0= swap character char.disabled sl@ 0<> and if  drop -1 exit  then
    else  drop  then
    f-mode swap case
        $D of  dup 4 = over $A = or swap 3 = or if  -1  else  $13  then  endof
        1 of  1 blow-reaction  endof   2 of  2 blow-reaction  endof   4 of  4 blow-reaction  endof
        3 of  4 = f-sub $10 = and if  -1  else  $10  then  endof
        5 of  4 = f-sub $B = and  f-104 @ me 4 relations:req-word-of = and if  -1  else  $B  then  endof
        6 of  dup 4 = over $A = or  swap 0= f-act @ $F = and or if  -1
              else f-mode 3 = f-sub 7 = and if  8  else  $20  then then  endof
        $A of  dup 4 = over 3 = or swap $A = or if  -1  else  9  then  endof
        $C of  dup 4 = if  drop f-sub $12 = if  $12  else  -1  then
               else  dup 3 = swap $A = or if  -1  else  $12  then  then  endof
        >r drop -1 r>
    endcase ;

\ ---- her falls (Fiona_StateKnockedDown / StateThrown and after) ----
\ the side the blow came from: (who struck her, +0x100; $FF the place progress +0x1060 keeps)
variable f-who   \ +0x100
fvariable kd-a  fvariable kd-aa
: blow-side ( -- have? )   \ kd-a the side (signed), kd-aa its size
    f-who @ $FF = if  not-yet" Fiona_StateKnockedDown: the place of a blow from nobody"  false exit  then
    f-who @ dup c-active? over character char.disabled sl@ 0= and 0= if  drop false exit  then
    c-pos f-pos vec-heading  f-yaw f- angle-wrap  fdup kd-a f!  fabs kd-aa f!  true ;
: fall-by-side ( front -- )
    kd-aa f@ 1.0471976e f< if  -1 f-play-table exit  then
    kd-aa f@ 2.0943952e f> if  1+ -1 f-play-table exit  then
    kd-a f@ f0< if  3 +  else  2 +  then  -1 f-play-table ;
: stand-up ( -- )   \ at its end she stands (the stumble timer 30)
    f-end? if  30 f-stumble-t !  0 f-2d !  to-idle  then  f-root-move ;
: st-after-knockdown ( -- )  stand-up ;
: knock-noise ( -- )  $5F noise-here ;
: st-knocked-down ( -- )
    f-settled? if
        blow-side 0= if  f-yaw kd-a f!  0e kd-aa f!  then
        f-sub case
            $C of  $3E 5 0 0 voice  $100E -1 f-play-table  knock-noise  endof
            $E of  $3E 5 0 0 voice  $1000 fall-by-side  knock-noise  endof
            $D of  $3F 5 0 0 voice  $100F -1 f-play-table  knock-noise  endof
            $F of  $3F 5 0 0 voice  $1004 fall-by-side  knock-noise  endof
        endcase
        ['] st-after-knockdown behave
    then
    f-root-move ;
\ thrown: moved by the motion turned to +0x1AD6D0 (unless +0x1AD6C4 is -1), turning there at 20
\ degrees a frame (pull_back_move)
variable th-turn  fvariable th-heading  variable th-back
fvariable pn-hold-scr   \ (progress +0x7D8: the panic's screen hold - with the screen effects)
: pull-back-move ( -- )
    th-turn @ -1 = if  f-root-move exit  then
    root@ rm-x f@ rm-z f@ th-heading f@ rotate-by f-move
    th-turn @ 0= if  th-heading f@ 0.34906585e f-turn-toward f0= if  1 th-turn !  then  then ;
: st-get-up ( -- )   \ Fiona_StateGetUp: up at its end
    1 f-2a !  0 f-cam-on !
    f-end? if  0 f-2d !  to-idle  then  f-root-move ;
: floored ( next -- )
    0 f-2d !  $B01 -1 f-play-table  $A f-mode!  $B f-act !  behave ;
: st-after-throw ( -- )
    1 f-2a !  0 f-cam-on !
    f-end? if
        th-back @ 0= if
            f-anim@ $1008 = if
                f-fear-bits @ 2 and 0= if  1 f-2d !  $100A -1 f-play  ['] st-get-up behave
                else  ['] st-special-entry floored  then
            then
        else f-anim@ $B04 = if  ['] st-special-entry floored
        else f-anim@ $100B = if  1 f-2d !  $100D -1 f-play  ['] st-get-up behave  then then then
    then
    pull-back-move
    f-sub $A = if  8 0 do  i me flags-of 1 and if  unloop exit  then  loop  then
    along-wall ;
: st-thrown ( -- )
    1 f-2a !  0 f-cam-on !
    f-settled? if
        f-who @ $FF = if  -1 th-turn !  f-yaw th-heading f!
        else  0 th-turn !  f-who @ c-pos f-pos vec-heading th-heading f!  then
        th-heading f@ f-yaw f- angle-wrap fabs 1.5707964e f< if
            0 th-back !  $1008 -1 f-play-table
        else
            1 th-back !  th-heading f@ 3.1415927e f+ angle-wrap th-heading f!
            f-fear-bits @ 2 and if  -1 th-turn !  th-heading f@ f-yaw!  $B04 -1 f-play-table
            else  $100B -1 f-play-table  then
        then
        f-fear-bits @ 2 and if  1000e pn-hold-scr f!  $42 5 0 0 voice  $6F noise-here
        else  $40 5 0 0 voice  $5F noise-here  then
        ['] st-after-throw behave
    then
    pull-back-move ;

\ ---- Fiona_React: her reaction to a request 4 (the fright it carries; the charm 0x88 keeps
\ her from being caught) - she drops what she's doing and goes down / is seized ----
: st-knock ( -- )   \ (0xC..0xF: knocked down; a stumble resets her recovery)
    me 4 relations:req-word-of $8000 and if  reset-recovery  then
    0 f-2a !  ['] st-knocked-down behave ;
variable charm-item   \ (the sub screen's slot 1: the charm that keeps her from being caught)
: react-now ( -- done? )   \ (0: taken, the request used up)
    me 1 relations:req-word-of reaction  dup -1 = if  drop false exit  then
    me relations:req# 5 4 * + sf@ panic-fright
    dup $20 = charm-item @ $88 = and if  drop false exit  then
    0 f-target-t !  0 f-freeze-t !  1 f-2d !  start-in-door
    me 2 relations:req-word-of f-who !
    $A f-act !  4 f-mode!  dup f-sub!
    case
        $C of  st-knock  endof  $D of  st-knock  endof  $E of  st-knock  endof  $F of  st-knock  endof
        $A of  me 4 relations:req-word-of $8000 and if  reset-recovery  then
               1 f-2a !  ['] st-thrown behave  endof
        $20 of  me 4 relations:req-word-of 5 = if
                    1 f-2a !  $FF f-who !  ['] st-thrown behave
                else  not-yet" Fiona_React: caught (0x20)"  then  endof
        $13 of  not-yet" Fiona_React: the fall (0x13)"  endof
        $12 of  not-yet" Fiona_React: held (0x12)"  endof
        $B of  not-yet" Fiona_React: a door (0xB)"  endof
        9 of  not-yet" Fiona_React: led away (9)"  endof
        8 of  not-yet" Fiona_React: off the ladder (8)"  endof
        $10 of  not-yet" Fiona_React: seized (0x10)"  endof
    endcase
    true ;
' react-now is fiona.commands:react
