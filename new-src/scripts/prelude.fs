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

: if     postpone 0branch here 0 , ; immediate
: then   here swap ! ; immediate
: else   postpone branch here 0 , swap postpone then ; immediate
: begin  here ; immediate
: again  postpone branch , ; immediate
: until  postpone 0branch , ; immediate
: while  postpone if swap ; immediate
: repeat postpone again postpone then ; immediate

\ do ... loop / +loop, ?do (skips when limit = start). (do) keeps the exit address, so
\ `leave` needs no patching: `do` leaves ( exit-slot loop-start ) for `loop` to finish.
: do     postpone (do) here 0 , here ; immediate
: ?do    postpone (?do) here 0 , here ; immediate
: loop   postpone (loop) , here swap ! ; immediate
: +loop  postpone (+loop) , here swap ! ; immediate

\ case ... of ... endof ... endcase   ( x -- )
: case    0 ; immediate
: of      postpone over postpone = postpone if postpone drop ; immediate
: endof   postpone else ; immediate
: endcase postpone drop begin ?dup while postpone then repeat ; immediate

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
\ deferred words: `defer name`, later `' word is name`
: noop ;
: defer  create ['] noop , does> @ execute ;
: is     ' >body state @ if postpone literal postpone ! else ! then ; immediate
\ a run of memory with a name: `16 cells buffer: name`
: buffer: create allot ;

\ ---- numbers ----
: within ( x lo hi -- flag ) over - >r - r> u< ;
: hex     16 base ! ;
: decimal 10 base ! ;
: ?  @ . ;
: clamp ( x lo hi -- x' ) rot min max ;

\ ---- floats ----
3.14159265358979 fconstant pi
: f2*  2e f* ;
: f2/  0.5e f* ;
: fsq  fdup f* ;
: fclamp ( F: x lo hi -- x' ) frot fmin fmax ;
: deg>rad  pi 180e f/ f* ;
: rad>deg  180e pi f/ f* ;
: f~ ( F: a b eps -- ) ( -- flag ) frot frot f- fabs f> ;

\ ---- strings ----
: s= ( a1 n1 a2 n2 -- flag ) compare 0= ;

\ ---- tasks ----
\ `wait` ( frames -- ) and `yield` give way to the rest of the frame; `seconds` turns seconds
\ into frames (60 a second).
: seconds  60 * ;
