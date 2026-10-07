\ state.fs - what several parts of the game share: are we playing (or flying the free camera),
\ is a menu open, and what puts the player in a room (player.fs sets it).
IN: state

variable playing  0 playing !
variable menu-open  0 menu-open !
variable title  0 title !        \ the title screen is up (title.fs): the other keys keep quiet
defer place-player
\ the exit of this room the player came in by (-1: none - a jump, or the start)
variable came-in-by  -1 came-in-by !
\ about to leave the room (the event scripts' last phases run)
defer leaving-room  ' noop is leaving-room
\ exits: is one locked, and taking one (doors.fs goes straight through; the event scripts open
\ the door and step her into the doorway, and the room's scripts take her through)
defer exit-locked? ( exit -- flag )  :noname drop false ; is exit-locked?
defer use-exit ( exit -- )  ' drop is use-exit
