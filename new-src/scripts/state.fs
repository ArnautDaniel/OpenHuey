\ state.fs - what several parts of the game share: are we playing (or flying the free camera),
\ is a menu open, and what puts the player in a room (player.fs sets it).
IN: state

variable playing  0 playing !
variable menu-open  0 menu-open !
defer place-player
\ the exit of this room the player came in by (-1: none - a jump, or the start)
variable came-in-by  -1 came-in-by !
\ about to leave the room (the event scripts' last phases run)
defer leaving-room  ' noop is leaving-room
