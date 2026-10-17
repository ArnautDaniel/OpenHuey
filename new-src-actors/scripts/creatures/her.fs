\ creatures/her.fs - what the creatures know of Fiona: where she is, whether she's hidden, held
\ by a script, out of reach; what she wears; and of the floor: its triangles' flags they mind,
\ the divider of a split room.
IN: creatures.her
USING: engine game-state actors facts flag-names ;

: her ( -- id )  s" fiona" actor-named ;
: her-at ( -- v )  her body-at ;
: her-tri ( -- tri )  her body-tri ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: half-hidden? ( -- flag )  fiona-half-hidden state-flag? ;
: wearing? ( item slot -- flag )  4 * progress pr.equipped + l@ = ;   \ (the equipment's slots 0..3)
: charmed? ( -- flag )  $8D 3 wearing? ;   \ (keeps the spirits away)

\ ---- the floor's flags: a triangle never stood on; one kept off (walking, and put anywhere);
\ paths kept off; one a spirit can't cross; the halves of a split room ----
8 constant never-floor   $04020028 constant no-go-floor   $40080 constant path-off-floor
$20000 constant uncrossable   $100000 constant one-half   $200000 constant other-half
one-half other-half or constant both-halves
\ a and b on either side of the divider
: half ( tri -- h )  dup 0< if  drop 0 exit  then  nav-flags both-halves and ;
: across-divider? ( a b -- flag )
    half swap half  2dup <>  rot 0<> and  swap 0<> and ;
