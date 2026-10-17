\ creatures/her.fs - what the creatures know of Fiona: where she is, whether she's hidden, held
\ by a script, out of reach; what she wears; and of the floor, which side of a split room's
\ divider (its flags by name: floor-flags.fs).
IN: creatures.her
USING: engine game-state actors facts flag-names floor-flags ;

: her ( -- id )  s" fiona" actor-named ;
: her-at ( -- v )  her body-at ;
: her-tri ( -- tri )  her body-tri ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: half-hidden? ( -- flag )  fiona-half-hidden state-flag? ;
: wearing? ( item slot -- flag )  4 * progress pr.equipped + l@ = ;   \ (the equipment's slots 0..3)
: charmed? ( -- flag )  $8D 3 wearing? ;   \ (keeps the spirits away)

\ ---- a split room: a and b on either side of the divider ----
: half ( tri -- h )  dup 0< if  drop 0 exit  then  nav-flags both-halves and ;
: across-divider? ( a b -- flag )
    half swap half  2dup <>  rot 0<> and  swap 0<> and ;
