\ facts.fs - the game's flags as facts (the progress, in C): read by anyone, changed by the
\ story (later its own actor) and the subsystems that own them.
IN: facts
USING: game-state ;

: bit? ( n addr -- flag )  over 5 rshift 4 * + l@  swap 31 and 1 swap lshift and 0<> ;
: state-flag? ( n -- flag )  progress pr.state bit? ;    \ the state flags (+0x8: control, panic...)
: story-flag? ( n -- flag )  progress pr.story bit? ;    \ the scenario flags (+0x1C)
: state-flag-clear ( n -- )   \ (its owner clears it: Hewie's trust clears 0x11 / 0x1D)
    dup 5 rshift 4 * progress pr.state +  swap 31 and 1 swap lshift invert  over l@ and swap l! ;
: state-flag-set ( n -- )   \ (its owner sets it: Fiona held - fiona-occupied; caught - the game's end)
    dup 5 rshift 4 * progress pr.state +  swap 31 and 1 swap lshift  over l@ or swap l! ;
