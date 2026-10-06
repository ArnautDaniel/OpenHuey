\ the core language: stacks, arithmetic, control flow, defining words, floats, strings, tasks

testing stack words
T{ 1 2 swap -> 2 1 }T
T{ 1 2 over -> 1 2 1 }T
T{ 1 2 3 rot -> 2 3 1 }T
T{ 1 2 3 -rot -> 3 1 2 }T
T{ 1 2 nip -> 2 }T
T{ 1 2 tuck -> 2 1 2 }T
T{ 1 2 2dup -> 1 2 1 2 }T
T{ 0 ?dup -> 0 }T
T{ 5 ?dup -> 5 5 }T
T{ 1 2 3 2 pick -> 1 2 3 1 }T
T{ 1 >r r@ r> -> 1 1 }T

testing arithmetic
T{ 7 3 / -> 2 }T
T{ -7 3 / -> -3 }T
T{ -7 3 mod -> 2 }T
T{ 7 3 /mod -> 1 2 }T
T{ 1000000 3000 2000 */ -> 1500000 }T
T{ 3 4 min 3 4 max -> 3 4 }T
T{ -5 abs 5 negate -> 5 -5 }T
T{ 1 3 lshift 16 2 rshift -> 8 4 }T
T{ 3 0 5 within 5 0 5 within -> true false }T
T{ 12 0 10 clamp -> 10 }T
T{ $ff #10 %101 -> 255 10 5 }T
T{ 'A' -> 65 }T

testing comparisons
T{ 1 2 < 2 1 < -> true false }T
T{ 0 0= 1 0= -> true false }T
T{ -1 1 u< -> false }T

testing if else then
: sign ( n -- s ) dup 0< if drop -1 else 0> if 1 else 0 then then ;
T{ -5 sign 0 sign 9 sign -> -1 0 1 }T

testing begin loops
: count-down ( n -- 0 ) begin 1- dup 0= until ;
T{ 10 count-down -> 0 }T
: sum-to ( n -- sum ) 0 swap begin dup while tuck + swap 1- repeat drop ;
T{ 4 sum-to -> 10 }T

testing do loops
: sum-i ( n -- sum ) 0 swap 0 do i + loop ;
T{ 5 sum-i -> 10 }T
: qsum ( n -- sum ) 0 swap 0 ?do i + loop ;
T{ 0 qsum 3 qsum -> 0 3 }T
: down ( -- ... ) 0 3 do i -1 +loop ;
T{ down -> 3 2 1 0 }T
: by2 ( -- ... ) 7 0 do i 2 +loop ;
T{ by2 -> 0 2 4 6 }T
: first-over ( limit -- i ) 100 0 do i over > if drop i leave then loop ;
T{ 5 first-over -> 6 }T
: grid ( -- n ) 0 3 0 do 4 0 do i j * + loop loop ;
T{ grid -> 18 }T

testing case
: name-of ( n -- c ) case 1 of 'a' endof 2 of 'b' endof drop 'z' 0 endcase ;
T{ 1 name-of 2 name-of 9 name-of -> 'a' 'b' 'z' }T

testing variables constants values
variable v  42 v !
T{ v @ -> 42 }T
T{ 3 v +! v @ -> 45 }T
7 constant seven
T{ seven -> 7 }T
10 value ten
T{ ten -> 10 }T
20 to ten
T{ ten -> 20 }T
: set-ten 30 to ten ;
T{ set-ten ten -> 30 }T

testing defer is
defer greet
: hello 1 ;  : bye-word 2 ;
' hello is greet
T{ greet -> 1 }T
' bye-word is greet
T{ greet -> 2 }T

testing create does>
: array ( n "name" -- ) create cells allot does> ( i -- addr ) swap cells + ;
4 array arr
T{ 11 0 arr !  22 3 arr !  0 arr @ 3 arr @ -> 11 22 }T
: counter create 0 , does> dup @ 1+ dup rot ! ;
counter ticks
T{ ticks ticks ticks -> 1 2 3 }T

testing recursion
: fact ( n -- n! ) dup 1 > if dup 1- recurse * then ;
T{ 5 fact -> 120 }T
: fib ( n -- f ) dup 2 < if exit then dup 1- recurse swap 2 - recurse + ;
T{ 10 fib -> 55 }T

testing :noname execute
:noname 6 7 * ; constant times
T{ times execute -> 42 }T
T{ ' seven execute -> 7 }T

testing floats
T{ 1.5e 2e f* 3e 1e-9 f~ -> true }T
T{ 2e fsqrt fsq 2e 1e-9 f~ -> true }T
T{ 7.9e f>s -7.9e f>s -> 7 -7 }T
T{ 3 s>f 2e f/ 1.5e f= -> true }T
T{ 1e 2e f< 2e 1e f< -> true false }T
T{ pi 180e f/ 90e f* fsin 1e 1e-9 f~ -> true }T
fvariable fv  2.5e fv f!
T{ fv f@ 2.5e f= -> true }T
1.25e fconstant fc
T{ fc 1.25e f= -> true }T
create f32 4 allot  0.75e f32 sf!
T{ f32 sf@ 0.75e f= -> true }T
T{ fdepth -> 0 }T

testing strings
: hi s" hello" ;
T{ hi nip -> 5 }T
T{ hi s" hello" s= hi s" help!" s= -> true false }T
T{ hi drop c@ -> 'h' }T

testing memory
create buf 8 allot
T{ buf 8 0 fill  $1234 buf w!  buf w@ buf c@ -> $1234 $34 }T
T{ -2 buf l!  buf sl@ buf l@ -> -2 $fffffffe }T

testing tasks
variable steps
: stepper 3 0 do 1 steps +! yield loop ;
0 steps !  ' stepper spawn drop
T{ steps @ -> 0 }T
1 tick-tasks
T{ steps @ -> 1 }T
5 tick-tasks
T{ steps @ -> 3 }T
variable woke
: sleeper 10 wait -1 woke ! ;
0 woke !  ' sleeper spawn drop
5 tick-tasks
T{ woke @ -> 0 }T
10 tick-tasks
T{ woke @ -> -1 }T
: forever begin yield again ;
' forever spawn constant tid
2 tick-tasks  tid kill  1 tick-tasks
T{ 0 -> 0 }T

testing errors recover
: bad s" 1 2 nosuchword" evaluate ;
T{ ' bad catch -> -1 }T
T{ error-message s" unknown word nosuchword" s= -> true }T
: good 5 ;
T{ ' good catch -> 5 0 }T

test-summary
