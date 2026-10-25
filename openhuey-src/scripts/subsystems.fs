\ subsystems.fs - every subsystem, loaded: game.fs spawns their actors by name (`stalker:riccardo-spawn`)
\ without using their words, so its handles (`game:fiona`, `game:riccardo`) hide none of theirs.
IN: subsystems
USING: acoustics rooms doors obstacles items subscreen avoid saves creatures cast camera danger panic window screen scenes music pause fiona hewie stalker summoner gameover ending things opening story story.words debug graphics party ;
