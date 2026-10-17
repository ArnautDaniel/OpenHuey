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
