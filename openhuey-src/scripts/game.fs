\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. (docs/subsystems.md: the map and the order.)
IN: game
USING: actors messages room-names facts flag-names story-names subsystems ;

acoustics:acoustics-spawn constant acoustics
rooms:rooms-spawn constant rooms
doors:doors-spawn constant doors
obstacles:obstacles-spawn constant obstacles
items:items-spawn constant items
subscreen:subscreen-spawn constant subscreen
avoid:avoid-spawn constant avoid
saves:saves-spawn constant saves
creatures:creatures-spawn constant creatures
camera:camera-spawn constant camera
danger:danger-spawn constant danger
panic:panic-spawn constant panic
window:window-spawn constant window
screen:screen-spawn constant screen
scenes:scenes-spawn constant scenes
music:music-spawn constant music
pause:pause-spawn constant pause
fiona:fiona-spawn constant fiona
hewie:hewie-spawn constant hewie
stalker:debilitas-spawn constant debilitas
stalker:daniella-spawn constant daniella
stalker:kind34-spawn constant kind34
stalker:kind35-spawn constant kind35
stalker:kind36-spawn constant kind36
stalker:riccardo-spawn constant riccardo
stalker:lorenzo-spawn constant lorenzo
stalker:lorenzo2-spawn constant lorenzo2
stalker:kind39-spawn constant kind39
stalker:kind37-spawn constant kind37
stalker:tintstalker-spawn constant tintstalker
stalker:kind12-spawn constant kind12
3 cast:cast-spawn constant cast3   4 cast:cast-spawn constant cast4   5 cast:cast-spawn constant cast5
summoner:summoner-spawn constant summoner
story:story-spawn constant story
gameover:gameover-spawn constant gameover
things:things-spawn constant things
ending:ending-spawn constant ending
debug:label-spawn constant room-label   \ (debug: the room's name top right)

\ the title, the opening movie, then a new game (opening.fs; without a window, the new game at once)
opening:opening-spawn constant opening
: start-headless ( -- )  hidden? if  opening:new-game  then ;   \ (no window - tests, screenshots: the
start-headless                                                 \ new game at once, ahead of the tests' messages)

\ free play (`hga --eval free-play`, or at the console): a shortcut past the game's start, for the
\ tests and debugging - the opening done, the grate open, the world not held, Hewie hers to command
\ and with her (where the first rooms' story leaves her). The opening itself plays from a new game
\ (the title, its movie, the cage room's script: tests/story/test_opening.fs).
: free-play ( -- )   \ (the title and opening left out)
    s" opening" actor-named dup 0< if  drop  else  kill  then   \ (by name: its id may be another's by now)
    opening-done story-flag-set  grate-open story-flag-set  world-held state-flag-clear  hewie-commandable state-flag-set
    front-garden-3 debug:room!   \ (a jump: debug.fs gives her a camera)
    hewie send join-fiona ;

\ the dance party (party.fs: golems dancing in a row, Fiona and Hewie in front; for fun) - F3 frees
\ the camera to film it
: dance-party ( -- )  party:party-spawn ;
: party-over ( -- )  party:party-over ;
