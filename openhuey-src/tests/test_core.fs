\ the core language: stacks, arithmetic, control flow, defining words, floats, strings, tasks
IN: test-core
USING: tester ;

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

testing 2! 2@
create pair 2 cells allot
T{ 1 2 pair 2! pair 2@ -> 1 2 }T

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

testing held tasks, resume, restart
0 steps !  ' stepper spawn-held constant held
3 tick-tasks
T{ steps @ -> 0 }T
T{ held resume steps @ -> -1 1 }T
T{ held resume held resume held resume steps @ -> -1 -1 0 3 }T
T{ held resume -> 0 }T
variable path
: second  2 path !  yield  3 path ! ;
: first  1 path !  ['] second restart  99 path ! ;
' first spawn-held constant hop
T{ hop resume path @ -> -1 2 }T
T{ hop resume path @ -> 0 3 }T
: deep  1 2 3  ['] second restart ;
: deeper  deep 4 5 ;
' deeper spawn-held constant hop2
T{ hop2 resume hop2 resume path @ -> -1 0 3 }T

testing errors recover
: bad s" 1 2 nosuchword" evaluate ;
T{ ' bad catch -> -1 }T
T{ error-message s" unknown word nosuchword" s= -> true }T
: good 5 ;
T{ ' good catch -> 5 0 }T

testing locals: {: a b -- :} take from the stack (the last name the top), | starts one at 0, to stores
: l-sub ( a b -- a-b )  {: a b -- difference :}  a b - ;
T{ 10 3 l-sub -> 7 }T
: l-swap3 ( a b c -- c b a )  {: a b c :}  c b a ;
T{ 1 2 3 l-swap3 -> 3 2 1 }T
: l-acc ( n -- sum )  {: n | sum :}  n 0 ?do  sum i + to sum  loop  sum ;
T{ 5 l-acc -> 10 }T
: l-early ( n -- x )  {: n :}  n 0< if  0 exit  then  n 2 * ;
T{ -4 l-early  6 l-early -> 0 12 }T
: l-nested ( a -- b )  {: a :}  a 1+  a l-sub ;   \ (a word with locals calling another)
T{ 5 l-nested -> 1 }T
: l-quote ( a -- b )  {: a :}  [: {: x :} x 100 * ;] a swap execute  a + ;
T{ 3 l-quote -> 303 }T
: l-r ( a -- a )  {: a :}  a >r  r>  a + ;   \ (the return stack still free to use)
T{ 4 l-r -> 8 }T
testing float locals: F: makes the names after it floats (taken from the float stack), | starts them at 0
: fl-sub ( F: x y -- x-y )  {: F: x y :}  x y f- ;
T{ 10e 3e fl-sub f>s -> 7 }T
: fl-mixed ( n F: x -- n*x )  {: n F: x :}  n s>f x f* ;
T{ 4 2.5e fl-mixed f>s -> 10 }T
: fl-acc ( n -- )  {: n | F: sum :}  n 0 ?do  sum 0.5e f+ to sum  loop  sum ;
T{ 6 fl-acc f>s -> 3 }T
: fl-order ( a F: x b -- )  {: a F: x | c :}  a  x f>s  c ;   \ (the cells and the floats each in their own order)
T{ 1 9e fl-order -> 1 9 0 }T

testing several {: :} in one definition: each takes its values where it stands
: l-two ( a -- )  {: a :}  a 2 *  {: b :}  a b + ;
T{ 3 l-two -> 9 }T
: l-two-early ( a -- )  dup 0< if  exit  then  {: a :}  a 1+  {: b F: half :}  b half f>s ;
T{ -1 l-two-early  4 1.5e l-two-early -> -1 5 1 }T

: l-in-loop ( n -- sum )  {: n | sum :}  n 0 ?do  i 2 * {: twice :}  sum twice + to sum  loop  sum ;
T{ 4 l-in-loop -> 12 }T

testing combinators: dip keep bi tri bi@ when unless ?
T{ 1 2 [: 10 + ;] dip -> 11 2 }T
T{ 1 5 2 3 [: + ;] 2dip -> 6 2 3 }T
T{ 5 [: dup * ;] keep -> 25 5 }T
T{ 2 3 [: + ;] 2keep -> 5 2 3 }T
T{ 4 [: 1+ ;] [: 1- ;] bi -> 5 3 }T
T{ 4 [: 1+ ;] [: 1- ;] [: 2* ;] tri -> 5 3 8 }T
T{ 2 3 [: 10 * ;] bi@ -> 20 30 }T
T{ 7 true [: 1+ ;] when  7 false [: 1+ ;] when -> 8 7 }T
T{ 7 true [: 1+ ;] unless  7 false [: 1+ ;] unless -> 7 8 }T
T{ true 1 2 ?  false 1 2 ? -> 1 2 }T

testing enum: constants from 0, a number sets the next, name>name names a value
enum: colour  red green blue ;
enum: act  $36 hide  $52 lie-down rise ;
T{ red green blue -> 0 1 2 }T
T{ hide lie-down rise -> $36 $52 $53 }T
T{ blue colour>name s" blue" s= -> -1 }T
T{ 99 act>name s" ?" s= -> -1 }T
enum: way  ahead       \ the first
    ( none here ) back \ the second
    10 aside ;
T{ ahead back aside -> 0 1 10 }T

testing strings: a list of words as strings, a number sets the next index, "" outside it
strings: files  intro.sfd  DIR\menu.sfd   \ the first two
    ( a gap ) 4 end.sfd ;
T{ 0 files s" intro.sfd" s= -> -1 }T
T{ 1 files s" DIR\menu.sfd" s= -> -1 }T
T{ 2 files nip  3 files nip  9 files nip  -1 files nip -> 0 0 0 0 }T
T{ 4 files s" end.sfd" s= -> -1 }T
test-summary
