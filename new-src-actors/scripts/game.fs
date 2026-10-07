\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. So far: sound in the house. (docs/subsystems.md: the map and the order.)
IN: game
USING: engine actors messages acoustics ;

acoustics-spawn constant acoustics
