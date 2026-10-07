\ lists, quotations and the higher-order words
IN: test-lists
USING: tester ;

testing quotations
T{ 3 [: 1+ ;] execute -> 4 }T
: twice ( xt -- n ) dup execute swap execute + ;
T{ [: 5 ;] dup drop twice -> 10 }T
: make-adder-test 10 [: 2 * ;] execute ;
T{ make-adder-test -> 20 }T
: nested [: [: 7 ;] execute 1+ ;] execute ;
T{ nested -> 8 }T

testing list literals
T{ { 1 2 3 } length -> 3 }T
T{ { 10 20 30 } 1 swap nth -> 20 }T
T{ { 10 20 30 } -1 swap nth -> 30 }T
: inside { 4 5 } ;
T{ inside length inside 0 swap nth -> 2 4 }T
T{ { } length -> 0 }T
T{ { { 1 } { 2 3 } } 1 swap nth length -> 2 }T

testing push pop nth!
list constant l
T{ 1 l push 2 l push l length -> 2 }T
T{ 9 0 l nth! 0 l nth -> 9 }T
T{ l pop l length -> 2 1 }T

testing each map filter reduce
variable total
T{ 0 total ! { 1 2 3 } [: total +! ;] each total @ -> 6 }T
T{ { 1 2 3 } [: dup * ;] map { 1 4 9 } [: + ;] 0 swap reduce swap [: + ;] 0 swap reduce -> 14 14 }T
T{ { 1 2 3 4 5 6 } [: 2 mod 0= ;] filter dup length swap 0 swap nth -> 3 2 }T
T{ { 3 4 5 } 0 [: + ;] reduce -> 12 }T

testing find any? all? count contains? range
T{ { 1 8 3 9 } [: 5 > ;] find -> 8 true }T
T{ { 1 2 } [: 5 > ;] find -> false }T
T{ { 1 2 7 } [: 5 > ;] any? { 1 2 } [: 5 > ;] any? -> true false }T
T{ { 6 7 } [: 5 > ;] all? { 6 2 } [: 5 > ;] all? -> true false }T
T{ { 1 6 7 2 9 } [: 5 > ;] count -> 3 }T
T{ 7 { 1 7 3 } contains? 4 { 1 7 3 } contains? -> true false }T
T{ 4 range 0 [: + ;] reduce -> 6 }T

test-summary
