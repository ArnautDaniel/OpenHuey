\ the actor kernel: messages, behaviours, state, delivery order, safety
IN: test-actors
USING: tester actors ;

message ping ( n -- )
message pong ( n -- )
message hello ( -- )

state: counter-state
  cell field count
  cell field last
  1 floats field level
end-state

variable log  list log !
: note ( n -- )  log @ push ;

behaviour counting
  on ping ( n -- )  dup last !  1 count +!  note ;
  on hello ( -- )  100 note ;
end-behaviour

behaviour echoing  extends counting
  on ping ( n -- )  dup note  1+ sender send pong ;
end-behaviour

behaviour answering
  on pong ( n -- )  1000 + note ;
end-behaviour

testing spawning and sending
actors-reset
counting counter-state s" a" spawn constant a
T{ a alive? -> -1 }T
T{ 7 a send ping  deliver  log @ length -> 1 }T
T{ a enter  count @  last @  leave -> 1 7 }T

testing behaviours: inherited handlers, become
actors-reset  list log !
echoing counter-state s" e" spawn constant e
answering counter-state s" q" spawn constant q
: ask-e ( -- )  q enter  5 e send ping  leave ;
T{ ask-e deliver  log @ 0 swap nth  log @ 1 swap nth -> 5 1006 }T   \ e answered q's ping
T{ e send hello deliver  log @ 2 swap nth -> 100 }T               \ (from counting)
T{ e enter  counting become  leave  3 e send ping  deliver  e enter count @ leave -> 1 }T

testing delivery rounds and order
actors-reset  list log !
behaviour relay
  on ping ( n -- )  dup note  dup 3 < if  1+ self send ping  else  drop  then ;
end-behaviour
relay counter-state s" r" spawn constant r
T{ 0 r send ping  1 r send ping  deliver  log @ length -> 7 }T
T{ log @ 0 swap nth  log @ 1 swap nth  log @ 2 swap nth -> 0 1 1 }T   \ round by round

testing tick and frame-end subscriptions
actors-reset  list log !
behaviour ticker
  on tick ( -- )  1 note ;
  on frame-end ( -- )  2 note ;
end-behaviour
ticker counter-state s" t" spawn constant t
t subscribe tick  t subscribe frame-end
T{ actors-frame  log @ length  log @ 0 swap nth  log @ 1 swap nth -> 2 1 2 }T
t unsubscribe tick
T{ actors-frame  log @ length -> 3 }T

testing floats travel as cells
T{ 2.5e f>cell cell>f 2.5e f= -> -1 }T

testing safety: a field with no actor, a handler's errors don't spoil the stacks
actors-reset
T{ ' count catch -> -1 }T   \ (an error: no actor being run)
behaviour sloppy
  on ping ( n -- )  drop 1 2 3 ;            \ leaves three cells
  on hello ( -- )  5 12345 send ping ;      \ an error: no actor 12345
end-behaviour
sloppy counter-state s" s" spawn constant s
: depth-after ( -- n )  depth >r  4 s send ping  s send hello  deliver  depth r> - ;
T{ depth-after -> 0 }T
T{ s alive? -> -1 }T

test-summary
