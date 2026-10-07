\ state.fs - what several parts of the game share: are we playing (or flying the free camera),
\ is a menu open, and what puts the player in a room (player.fs sets it).
IN: state

variable playing  0 playing !
variable menu-open  0 menu-open !
variable paused  0 paused !       \ a menu stops the game (the inventory): nothing moves
variable title  0 title !        \ the title screen is up (title.fs): the other keys keep quiet
defer place-player
\ the exit of this room the player came in by (-1: none - a jump, or the start)
variable came-in-by  -1 came-in-by !
\ about to leave the room (the event scripts' last phases run)
defer leaving-room  ' noop is leaving-room
\ she leaves the room by an exit: the others in it decide what to do (characters' vtable +0x34)
defer char-leaves ( exit -- )  ' drop is char-leaves
\ exits: is one locked, and taking one (doors.fs goes straight through; the event scripts open
\ the door and step her into the doorway, and the room's scripts take her through)
defer exit-locked? ( exit -- flag )  :noname drop false ; is exit-locked?
defer use-exit ( exit -- )  ' drop is use-exit
\ is character slot n (0 Fiona, 1 Hewie) being moved by a script (then its own control waits)
defer scripted? ( n -- flag )  :noname drop false ; is scripted?
\ Hewie is in the scene with her (a script can send him off, and bring him back)
variable hewie-along  -1 hewie-along !
\ a new game from the title (events/play.fs: the game's own start)
defer new-game  ' noop is new-game
\ Fiona's model (her costume: the game's progress variable $26 picks it - events/play.fs)
defer fiona-model ( -- addr len )  :noname s" O_FIN/FIN_000" ; is fiona-model
