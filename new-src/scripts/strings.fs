\ strings.fs - text: building a string from pieces, and keeping one.
IN: strings

\ one string built at a time:  str-reset  s" a" +str  s" b" +str  str ( -- addr len )
create (str) 1024 allot
variable (str-len)
: str-reset  0 (str-len) ! ;
: +str ( addr len -- )  dup >r  (str) (str-len) @ +  swap move  r> (str-len) +! ;
: str ( -- addr len )  (str) (str-len) @ ;

\ a string copied into the dictionary, to last (s" text" at the prompt is a scratch buffer)
: keep ( addr len -- addr' len )  dup >r here dup >r swap move r> r> dup allot align ;
