\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. (docs/subsystems.md: the map and the order.)
IN: game
USING: engine actors messages room-names acoustics rooms doors camera danger panic window screen music pause fiona hewie stalker opening story story.words flag-names story-names debug ;

acoustics-spawn constant acoustics
rooms-spawn constant rooms
doors-spawn constant doors
camera-spawn constant camera
danger-spawn constant danger
panic-spawn constant panic
window-spawn constant window
screen-spawn constant screen
music-spawn constant music
pause-spawn constant pause
fiona-spawn constant fiona
hewie-spawn constant hewie
debilitas-spawn constant debilitas
daniella-spawn constant daniella
story-spawn constant story
label-spawn constant room-label   \ (debug: the room's name top right)

\ the title, the opening movie, then a new game (opening.fs; without a window, the new game at once)
opening-spawn constant opening
: start-headless ( -- )  hidden? if  opening:new-game  then ;   \ (no window - tests, screenshots: the
start-headless                                                 \ new game at once, ahead of the tests' messages)

\ free play (`hga --eval free-play`, or at the console): the world as the opening leaves it -
\ a stand-in until the opening's scenes can play (cutscenes, the characters' scripted moves).
\ The opening done, the grate open, the world not held (no scene playing), Hewie hers to
\ command and with her.
: free-play ( -- )   \ (the title and opening left out)
    s" opening" actor-named dup 0< if  drop  else  kill  then   \ (by name: its id may be another's by now)
    opening-done story-flag-set  grate-open story-flag-set  world-held state-flag-clear  hewie-commandable state-flag-set
    front-garden-3 -1 rooms send go-to-room
    hewie send join-fiona ;
