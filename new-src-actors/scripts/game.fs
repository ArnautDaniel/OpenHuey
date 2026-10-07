\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. (docs/subsystems.md: the map and the order.)
IN: game
USING: engine actors messages room-names acoustics rooms camera walker ;

acoustics-spawn constant acoustics
rooms-spawn constant rooms
camera-spawn constant camera
walker-spawn constant walker        \ (the debug stand-in for Fiona)

\ a new game: the cage room, as the original's entry (no exit)
front-garden-3 -1 rooms send go-to-room
