\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. (docs/subsystems.md: the map and the order.)
IN: game
USING: engine actors messages room-names acoustics rooms doors camera danger panic fiona ;

acoustics-spawn constant acoustics
rooms-spawn constant rooms
doors-spawn constant doors
camera-spawn constant camera
danger-spawn constant danger
panic-spawn constant panic
fiona-spawn constant fiona

\ a new game: the cage room, as the original's entry (no exit)
front-garden-3 -1 rooms send go-to-room
