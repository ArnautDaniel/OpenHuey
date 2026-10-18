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
$AF constant hard-mode-flag
: hard-mode? ( -- flag )  hard-mode-flag story-flag? ;   \ a game started in hard mode (the original's progress +0x30 bit 15: the title's hard new game)
: state-flag-clear ( n -- )  progress pr.state bit-clear ;   \ (its owner clears it: Hewie's trust clears 0x11 / 0x1D)
: state-flag-set ( n -- )  progress pr.state bit-set ;       \ (its owner sets it: Fiona held - fiona-occupied; caught - the game's end)
: story-flag-set ( n -- )  progress pr.story bit-set ;       \ (its owner sets it: the items set the maps' flags)
: resident-flag? ( n -- flag )  progress pr.resident bit? ;   \ the system data's (the unlocks: kept across games)
: resident-flag-set ( n -- )  progress pr.resident bit-set ;   \ (its owner sets it: the ending)

\ ---- the play record (the results at the end): its owners count it ----
: tally ( addr -- )  dup l@ 1+ 9999 min swap l! ;   \ (one more, as the original: to 9999)
: panicked ( -- )  progress pr.panics tally ;         \ (the panic: she panics)
: down-for-good ( -- )  progress pr.knockouts tally ;   \ (a stalker: knocked out for good)
