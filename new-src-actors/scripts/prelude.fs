\ prelude.fs - the rest of the language, written in Forth on top of the C core (forth.c).
\ Loaded before anything else. Like jonesforth, control structures are ordinary immediate
\ words that lay down branches and patch them: `here` is the next cell of the definition
\ being compiled, and branch targets are absolute addresses.

IN: forth

: true  -1 ;
: false 0 ;
: cell+ cell + ;
: 2*  1 lshift ;
: 2/  1 arshift ;
: not 0= ;
: 2! ( x1 x2 addr -- ) swap over ! cell+ ! ;
: 2@ ( addr -- x1 x2 ) dup cell+ @ swap @ ;

\ ---- conditionals and loops ----
\ if ... else ... then           ( flag -- )
\ begin ... again                forever
\ begin ... until                ( -- flag ) at the end
\ begin ... while ... repeat     ( -- flag ) in the middle

: if     inside-definition postpone 0branch here 0 , ; immediate
: then   inside-definition here swap ! ; immediate
: else   inside-definition postpone branch here 0 , swap postpone then ; immediate
: begin  inside-definition here ; immediate
: again  inside-definition postpone branch , ; immediate
: until  inside-definition postpone 0branch , ; immediate
: while  inside-definition postpone if swap ; immediate
: repeat inside-definition postpone again postpone then ; immediate

\ do ... loop / +loop, ?do (skips when limit = start). (do) keeps the exit address, so
\ `leave` needs no patching: `do` leaves ( exit-slot loop-start ) for `loop` to finish.
: do     inside-definition postpone (do) here 0 , here ; immediate
: ?do    inside-definition postpone (?do) here 0 , here ; immediate
: loop   inside-definition postpone (loop) , here swap ! ; immediate
: +loop  inside-definition postpone (+loop) , here swap ! ; immediate

\ case ... of ... endof ... endcase   ( x -- )
: case    inside-definition 0 ; immediate
: of      inside-definition postpone over postpone = postpone if postpone drop ; immediate
: endof   inside-definition postpone else ; immediate
: endcase inside-definition postpone drop begin ?dup while postpone then repeat ; immediate

\ ---- defining words ----
: variable   create 0 , ;
: constant   create , does> @ ;
: fvariable  create 0e f, ;
: fconstant  create f, does> f@ ;
: value      create , does> @ ;
: fvalue     create f, does> f@ ;
\ `to name` stores into a value (from the stack, or the float stack for an fvalue)
: (to)  ( x addr -- ) ! ;
: to    ' >body state @ if postpone literal postpone (to) else (to) then ; immediate
: fto   ' >body state @ if postpone literal postpone f! else f! then ; immediate
\ deferred words: `defer name`, later `' word is name`; `action-of name` what it does now (a mod
\ setting a hook keeps it, to call for the rest)
: noop ;
: defer  create ['] noop , does> @ execute ;
: is     ' >body state @ if postpone literal postpone ! else ! then ; immediate
: action-of ( "name" -- xt )  ' >body @ ;
\ a run of memory with a name: `16 cells buffer: name`
: buffer: create allot ;
\ records (Forth 2012's structures): a name for each field's offset, and the size -
\   begin-structure /point  ffield: p.x  ffield: p.y  end-structure     ( addr p.y f@ )
: begin-structure ( "name" -- addr 0 )  create here 0 ,  0  does> @ ;
: +field ( n size "name" -- n' )  create over , +  does> @ + ;
: field: ( n "name" -- n' )  1 cells +field ;
: cfield: ( n "name" -- n' )  1 +field ;   \ (a byte: c@ c!)
: lfield: ( n "name" -- n' )  4 +field ;   \ (32 bits: l@ l!)
: ffield: ( n "name" -- n' )  1 floats +field ;
: sffield: ( n "name" -- n' )  4 +field ;   \ (a 32-bit float, as the engine's: sf@ sf!)
: end-structure ( addr n -- )  swap ! ;

\ ---- numbers ----
: within ( x lo hi -- flag ) over - >r - r> u< ;
: hex     16 base ! ;
: decimal 10 base ! ;
: clamp ( x lo hi -- x' ) rot min max ;
: 0<= ( n -- flag ) 0> 0= ;
: 0>= ( n -- flag ) 0< 0= ;

\ ---- floats ----
: fnip ( F: a b -- b )  fswap fdrop ;
3.14159265358979 fconstant pi
: f2*  2e f* ;
: f2/  0.5e f* ;
: fsq  fdup f* ;
: float+ ( addr -- addr' )  1 floats + ;
: f0> ( F: x -- ) ( -- flag )  0e f> ;
: fclamp ( F: x lo hi -- x' ) frot fmin fmax ;
: deg>rad  pi 180e f/ f* ;
: rad>deg  180e pi f/ f* ;
: f~ ( F: a b eps -- ) ( -- flag ) frot frot f- fabs f> ;

\ ---- strings ----
: s= ( a1 n1 a2 n2 -- flag ) compare 0= ;

\ ---- tasks ----
\ `wait` ( frames -- ) and `yield` give way to the rest of the frame; `seconds` turns seconds
\ into frames (30 a second: the game's rate).
: seconds  30 * ;

\ ---- combinators (Factor's): code run on values without shuffling them about ----
\   x [: ... ;] dip        run it under x, x back on top          ( x xt -- x )
\   x [: ... ;] keep       run it on x, x kept after its results  ( x xt -- .. x )
\   x [: p ;] [: q ;] bi   p and q each on x                     ( x p q -- )
\   x [: p ;] [: q ;] [: r ;] tri
\   x y [: q ;] bi@        q on x, then on y                     ( x y q -- )
\   flag [: ... ;] when / unless
\   flag a b ?             a if the flag, else b                 ( flag a b -- a|b )
: dip ( x xt -- x )  swap >r execute r> ;
: 2dip ( x y xt -- x y )  -rot >r >r execute r> r> ;
: keep ( x xt -- .. x )  over >r execute r> ;
: 2keep ( x y xt -- .. x y )  >r 2dup r> -rot >r >r execute r> r> ;
: bi ( x p q -- )  >r keep r> execute ;
: tri ( x p q r -- )  >r >r keep r> keep r> execute ;
: bi@ ( x y q -- )  tuck >r >r execute r> r> execute ;
: when ( flag xt -- )  swap if  execute  else  drop  then ;
: unless ( flag xt -- )  swap if  drop  else  execute  then ;
: ? ( flag a b -- a|b )  rot if  drop  else  nip  then ;

\ ---- lists and higher-order words ----
\ Lists hold cells: `{ 1 2 3 }` makes one (also inside definitions), `list` an empty one;
\ push pop nth nth! length list-clear list-free. The words below take a list and code -
\ usually a quotation, [: ... ;] - and run the code on each item.

: each ( l xt -- )  \ xt ( x -- )
    over length 0 ?do  over i swap nth  over execute  loop  2drop ;
: map ( l xt -- l' )  \ xt ( x -- y ), a new list
    list  2 pick length 0 ?do  2 pick i swap nth  2 pick execute  over push  loop  nip nip ;
: filter ( l xt -- l' )  \ xt ( x -- flag ), a new list of the items it keeps
    list  2 pick length 0 ?do
        2 pick i swap nth  dup 3 pick execute if  over push  else  drop  then
    loop  nip nip ;
: reduce ( l acc xt -- acc' )  \ xt ( acc x -- acc' )
    swap  2 pick length 0 ?do  2 pick i swap nth  2 pick execute  loop  nip nip ;
: find-word ( addr len -- xt | 0 )  find ;   \ (the dictionary's: `find` below is the lists')
: find ( l xt -- x true | false )  \ the first item xt ( x -- flag ) holds for
    over length 0 ?do
        over i swap nth  dup 2 pick execute if  nip nip true unloop exit  then  drop
    loop  2drop false ;
: any? ( l xt -- flag )  find dup if nip then ;
: count ( l xt -- n )  \ how many it holds for
    0 swap  2 pick length 0 ?do  2 pick i swap nth  over execute if  swap 1+ swap  then  loop
    drop nip ;
: all? ( l xt -- flag )  over length >r count r> = ;
: range ( n -- l )  \ 0 .. n-1
    list swap 0 ?do  i over push  loop ;
: contains? ( x l -- flag )
    dup length 0 ?do  2dup i swap nth = if  2drop true unloop exit  then  loop  2drop false ;
: .list ( l -- )  ." { " [: . ;] each ." }" ;
