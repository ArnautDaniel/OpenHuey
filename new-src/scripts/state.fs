\ state.fs - what several parts of the game share: are we playing (or flying the free camera),
\ is a menu open, and what puts the player in a room (player.fs sets it).
IN: state

variable playing  0 playing !
variable menu-open  0 menu-open !
defer place-player
