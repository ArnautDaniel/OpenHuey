\ facts.fs - the game's flags as facts (the progress, in C): read by anyone, changed by the
\ story (later its own actor) and the subsystems that own them.
IN: facts
USING: game-state ;

: bit? ( n addr -- flag )  over 5 rshift 4 * + l@  swap 31 and 1 swap lshift and 0<> ;
: state-flag? ( n -- flag )  progress pr.state bit? ;    \ the state flags (+0x8: control, panic...)
: story-flag? ( n -- flag )  progress pr.story bit? ;    \ the scenario flags (+0x1C)
$AF constant hard-mode-flag
: hard-mode? ( -- flag )  hard-mode-flag story-flag? ;   \ a game started in hard mode (the original's progress +0x30 bit 15: the title's hard new game)
: state-flag-clear ( n -- )   \ (its owner clears it: Hewie's trust clears 0x11 / 0x1D)
    dup 5 rshift 4 * progress pr.state +  swap 31 and 1 swap lshift invert  over l@ and swap l! ;
: state-flag-set ( n -- )   \ (its owner sets it: Fiona held - fiona-occupied; caught - the game's end)
    dup 5 rshift 4 * progress pr.state +  swap 31 and 1 swap lshift  over l@ or swap l! ;
: story-flag-set ( n -- )   \ (its owner sets it: the items set the maps' flags)
    dup 5 rshift 4 * progress pr.story +  swap 31 and 1 swap lshift  over l@ or swap l! ;
: resident-flag? ( n -- flag )  progress pr.resident bit? ;   \ the system data's (the unlocks: kept across games)
: resident-flag-set ( n -- )   \ (its owner sets it: the ending)
    dup 5 rshift 4 * progress pr.resident +  swap 31 and 1 swap lshift  over l@ or swap l! ;

\ ---- the play record (the results at the end): its owners count it ----
: tally ( addr -- )  dup l@ 1+ 9999 min swap l! ;   \ (one more, as the original: to 9999)
: panicked ( -- )  progress pr.panics tally ;         \ (the panic: she panics)
: down-for-good ( -- )  progress pr.knockouts tally ;   \ (a stalker: knocked out for good)
