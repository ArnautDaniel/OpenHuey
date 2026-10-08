\ game.fs - where the game starts (after prelude.fs): the actors that make up the game are
\ spawned here. (docs/subsystems.md: the map and the order.)
IN: game
USING: engine actors messages room-names acoustics rooms doors camera danger panic window screen music pause fiona hewie story story.words flag-names story-names debug ;

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
story-spawn constant story

\ a new game: the cage room, as the original's entry (no exit)
front-garden-3 -1 rooms send go-to-room

\ free play (`hga --eval free-play`, or at the console): the world as the opening leaves it -
\ a stand-in until the opening's scenes can play (cutscenes, the characters' scripted moves).
\ The opening done, the grate open, the world not held (no scene playing), Hewie hers to
\ command and with her.
: free-play ( -- )
    opening-done story-flag-set  grate-open story-flag-set  world-held state-flag-clear  hewie-commandable state-flag-set
    front-garden-3 -1 rooms send go-to-room
    hewie send join-fiona ;
