\ tester.fs - a small test harness in the style of John Hayes' tester:
\   T{ 1 2 + -> 3 }T
\ runs the code before `->`, then checks the stack against the values after it.
\ `testing name` labels what follows; `test-summary` prints the totals and sets the exit status.
IN: tester

variable #tests   variable #errors
variable t-depth  variable t-n  variable t-bad
create t-results 32 cells allot
create t-label 64 allot  variable t-label-len

: testing ( "rest of line" -- )  parse-line 64 min dup t-label-len !  t-label swap move ;
: t-clear  begin depth t-depth @ > while drop repeat ;
: t-fail ( addr n -- )
    1 #errors +!  ." FAIL [" t-label t-label-len @ type ." ] test " #tests @ . ." : " type cr ;

: T{  depth t-depth ! ;
: ->  depth t-depth @ - dup t-n !  0 ?do  t-results i cells + !  loop ;
: }T
    1 #tests +!
    depth t-depth @ - t-n @ <> if  t-clear s" wrong number of results" t-fail exit  then
    0 t-bad !
    t-n @ 0 ?do  t-results i cells + @ <> if  -1 t-bad !  then  loop
    t-bad @ if  s" wrong result" t-fail  then ;

: test-summary
    #tests @ . ." tests, " #errors @ . ." failed" cr
    #errors @ exit-status ;
