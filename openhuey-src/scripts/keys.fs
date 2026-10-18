\ keys.fs - reading the keyboard: held now, or pressed this tick (scancodes from `key: Name`).
IN: keys
USING: engine ;

: held? ( scancode -- flag ) key-down? ;
: pressed? ( scancode -- flag ) key-pressed? ;

\ ---- the pad's buttons on the keyboard (the scripts' pad?: 0 circle, 1 square, 2 L1, 3 triangle,
\ 4 R1, 5 cross, 6 start; 7 select, the menu). Circle - the action button - is Space or Return. ----
enum: button  circle square l1 triangle r1 cross start-button select-button ;
: button-keys ( b -- key1 key2 )   \ (0: none)
    case
        circle of  key: C key: Backspace  endof   \ (as hg: native/platform/input.c)
        square of  key: Z 0  endof
        l1 of  key: Q 0  endof
        triangle of  key: V 0  endof
        r1 of  key: E 0  endof
        cross of  key: X key: Space  endof
        start-button of  key: Return 0  endof
        select-button of  key: Tab 0  endof   \ (as hg)
        0 0 rot   \ (none: the button stays on top for endcase)
    endcase ;
: key-so? ( key xt -- flag )  over if  execute  else  2drop false  then ;   \ (a key 0 is none)
: either ( k1 k2 xt -- flag )  {: k1 k2 xt :}  k1 xt key-so?  k2 xt key-so?  or ;   \ (either key so)
: button-down? ( b -- flag )  button-keys ['] key-down? either ;
: button-pressed? ( b -- flag )  button-keys ['] key-pressed? either ;

\ ---- the sticks ----
\ the left stick: W A S D (and a gamepad's left stick as far as it is pushed), x right, z down
: stick@ ( F: -- x z )
    0e 0e  {: F: x F: z :}
    key: D held? if  1e to x  then  key: A held? if  x 1e f- to x  then
    key: S held? if  1e to z  then  key: W held? if  z 1e f- to z  then
    pad-stick  fover f0= fdup f0= and if  fdrop fdrop x z exit  then ;
\ the right stick's gesture this frame (Gesture_Update): I up, K down, L right, J left, F as R3
\ (0 up, 1 down, 2 R3, 3 right, 4 left; -1 none - messages.fs stick-gesture); L held, right
\ again (the stick held over: the praise's repeats)
: gesture@ ( -- g )
    key: I pressed? if  0 exit  then  key: K pressed? if  1 exit  then
    key: F pressed? if  2 exit  then  key: L pressed? if  3 exit  then
    key: J pressed? if  4 exit  then
    key: L held? if  3 exit  then  -1 ;
\ the face buttons and the shoulders pressed this frame (circle .. cross: the pad's bits 10..15)
: buttons-pressed ( -- n )  0  start-button 0 do  i button-pressed? if  1+  then  loop ;
