\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. (docs/subsystems.md: the map and the order.)
IN: game
USING: engine actors messages room-names acoustics rooms doors obstacles items subscreen avoid saves creatures cast camera danger panic window screen scenes music pause fiona hewie stalker summoner gameover ending things opening story story.words flag-names story-names debug graphics party ;

acoustics-spawn constant acoustics
rooms-spawn constant rooms
doors-spawn constant doors
obstacles-spawn constant obstacles
items-spawn constant items
subscreen-spawn constant subscreen
avoid-spawn constant avoid
saves-spawn constant saves
creatures-spawn constant creatures
camera-spawn constant camera
danger-spawn constant danger
panic-spawn constant panic
window-spawn constant window
screen-spawn constant screen
scenes-spawn constant scenes
music-spawn constant music
pause-spawn constant pause
fiona-spawn constant fiona
hewie-spawn constant hewie
debilitas-spawn constant debilitas
daniella-spawn constant daniella
kind34-spawn constant kind34
kind35-spawn constant kind35
kind36-spawn constant kind36
riccardo-spawn constant riccardo
lorenzo-spawn constant lorenzo
lorenzo2-spawn constant lorenzo2
kind39-spawn constant kind39
kind37-spawn constant kind37
tintstalker-spawn constant tintstalker
kind12-spawn constant kind12
3 cast-spawn constant cast3   4 cast-spawn constant cast4   5 cast-spawn constant cast5
summoner-spawn constant summoner
story-spawn constant story
gameover-spawn constant gameover
things-spawn constant things
ending-spawn constant ending
label-spawn constant room-label   \ (debug: the room's name top right)

\ the title, the opening movie, then a new game (opening.fs; without a window, the new game at once)
opening-spawn constant opening
: start-headless ( -- )  hidden? if  opening:new-game  then ;   \ (no window - tests, screenshots: the
start-headless                                                 \ new game at once, ahead of the tests' messages)

\ free play (`hga --eval free-play`, or at the console): a shortcut past the game's start, for the
\ tests and debugging - the opening done, the grate open, the world not held, Hewie hers to command
\ and with her (where the first rooms' story leaves her). The opening itself plays from a new game
\ (the title, its movie, the cage room's script: tests/story/test_opening.fs).
: free-play ( -- )   \ (the title and opening left out)
    s" opening" actor-named dup 0< if  drop  else  kill  then   \ (by name: its id may be another's by now)
    opening-done story-flag-set  grate-open story-flag-set  world-held state-flag-clear  hewie-commandable state-flag-set
    front-garden-3 room!   \ (a jump: debug.fs gives her a camera)
    hewie send join-fiona ;

\ the dance party (party.fs: golems dancing in a row, Fiona and Hewie in front; for fun) - F3 frees
\ the camera to film it
: dance-party ( -- )  party-spawn ;
: party-over ( -- )  party:party-over ;
