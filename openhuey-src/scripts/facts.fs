\ facts.fs - the game's flags as facts (the progress, in C): read by anyone, changed by the
\ story (later its own actor) and the subsystems that own them.
IN: facts
USING: game-state ;

: bit ( n base -- addr mask )   \ flag n of a run of 32-bit words
    {: n base :}  n 5 rshift 4 * base +   1 n 31 and lshift ;
: bit? ( n base -- flag )  bit swap l@ and 0<> ;
: bit-set ( n base -- )  bit over l@ or swap l! ;
: bit-clear ( n base -- )  bit invert over l@ and swap l! ;
: state-flag? ( n -- flag )  progress pr.state bit? ;    \ the state flags (control, panic, ...)
: story-flag? ( n -- flag )  progress pr.story bit? ;    \ the scenario flags
\ the one played: Fiona, or Hewie in the mini-game's run (the original's progress +0x1FBEC1 - not
\ the saved state: set as the run's clock starts, clear as it stops and with each new game)
variable hewie-played
: hewie-played? ( -- flag )  hewie-played @ 0<> ;
\ ... and its best times (the system data's +0x3C: each challenge A..D its three, hours minutes
\ seconds; 99:59:59 none), kept across games; the time a run took (clock_save: its frames)
create run-records 36 allot
: record ( challenge i -- addr )  swap 3 * + 3 * run-records + ;
: records-clear ( -- )  12 0 do  99 run-records i 3 * + c!  59 run-records i 3 * + 1+ c!  59 run-records i 3 * + 2 + c!  loop ;
records-clear
variable run-took
variable mini-game-wanted   \ (the title's MINI-GAME: the extras room starts it at once - off-map-37's entry)
$AF constant hard-mode-flag
: hard-mode? ( -- flag )  hard-mode-flag story-flag? ;   \ a game started in hard mode (the original's progress +0x30 bit 15: the title's hard new game)
: state-flag-clear ( n -- )  progress pr.state bit-clear ;   \ (its owner clears it: Hewie's trust clears 0x11 / 0x1D)
: state-flag-set ( n -- )  progress pr.state bit-set ;       \ (its owner sets it: Fiona held - fiona-occupied; caught - the game's end)
: story-flag-set ( n -- )  progress pr.story bit-set ;       \ (its owner sets it: the items set the maps' flags)
: story-flag-clear ( n -- )  progress pr.story bit-clear ;

\ ---- the story's facts by name: story-names.fs defines each (`fact:` - a constant, its name
\ kept), the saves write the ones set by name and read them back by name ----
$400 constant #story-flags
create fact-names  #story-flags 2 * cells allot   fact-names #story-flags 2 * cells 0 fill
: fact-slot ( n -- addr )  2 * cells fact-names + ;
: fact-name ( n -- addr len )  fact-slot 2@ swap ;   \ ("" for one not named)
: name-fact ( addr len n -- )   \ (its first name kept - a copy)
    {: a len n | dst :}  n fact-name nip if  exit  then
    here to dst  a dst len move  len allot  dst n fact-slot !  len n fact-slot cell+ ! ;
: fact-named ( addr len -- n | -1 )
    {: a len :}  #story-flags 0 do  i fact-name dup if  a len compare 0= if  i unloop exit  then  else  2drop  then  loop  -1 ;
create fact-def 96 allot   variable fact-def-len
: fact-def+ ( addr len -- )  {: a len :}  a fact-def fact-def-len @ + len move  len fact-def-len +! ;
: fact: ( n "name" -- )   \ a story fact: `$3 fact: hewie-found-in-wire`
    parse-name {: n a len :}  a len n name-fact
    0 fact-def-len !  s" constant " fact-def+  a len fact-def+  n fact-def fact-def-len @ evaluate ;
: resident-flag? ( n -- flag )  progress pr.resident bit? ;   \ the system data's (the unlocks: kept across games)
: resident-flag-set ( n -- )  progress pr.resident bit-set ;   \ (its owner sets it: the ending)

\ ---- the play record (the results at the end): its owners count it ----
: tally ( addr -- )  dup l@ 1+ 9999 min swap l! ;   \ (one more, as the original: to 9999)
: panicked ( -- )  progress pr.panics tally ;         \ (the panic: she panics)
: down-for-good ( -- )  progress pr.knockouts tally ;   \ (a stalker: knocked out for good)
