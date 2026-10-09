\ keys.fs - reading the keyboard: held now, or pressed this tick (scancodes from `key: Name`).
IN: keys
USING: engine ;

: held? ( scancode -- flag ) key-down? ;
: pressed? ( scancode -- flag ) key-pressed? ;

\ ---- the pad's buttons on the keyboard (the scripts' pad?: 0 circle, 1 square, 2 L1, 3 triangle,
\ 4 R1, 5 cross, 6 start). Circle - the action button - is Space or Return. ----
0 constant circle   1 constant square   2 constant l1   3 constant triangle
4 constant r1       5 constant cross    6 constant start-button
: button-keys ( b -- key1 key2 )   \ (0: none)
    case
        circle of  key: C key: Backspace  endof   \ (as hg: native/platform/input.c)
        square of  key: Z 0  endof
        l1 of  key: Q 0  endof
        triangle of  key: V 0  endof
        r1 of  key: E 0  endof
        cross of  key: X key: Space  endof
        start-button of  key: Return 0  endof
        >r 0 0 r>
    endcase ;
: either ( k1 k2 xt -- flag )   \ (either key so)
    >r  ?dup if  r@ execute  else  false  then  swap ?dup if  r@ execute  else  false  then  or  r> drop ;
: button-down? ( b -- flag )  button-keys ['] key-down? either ;
: button-pressed? ( b -- flag )  button-keys ['] key-pressed? either ;
