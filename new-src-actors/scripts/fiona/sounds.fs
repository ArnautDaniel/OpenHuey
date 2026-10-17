\ fiona/sounds.fs - Fiona's sounds by name (her bank: named for where the original's states play
\ them, src/game/fiona.c). Played with `her-sound` (fiona/model.fs); her cries through `voice`
\ (muffled under water: fiona/voice.fs).
IN: fiona.sounds

$0F constant flee-turn-sound       \ turning to flee
$1C constant muffled-sound         \ a cry under water (bank 6)
$20 constant cowgirl-blow-sound    \ the special costumes' blows
$21 constant illegal-blow-sound
$22 constant illegal-hit-sound     \ ... landed
$3C constant effort-sound          \ a throw, a shove
$3D constant kick-sound
$3E constant knocked-sound         \ struck
$3F constant struck-hard-sound     \ struck hard, losing her hold
$40 constant fall-sound            \ a fall, a slip
$41 constant caught-sound          \ held for good
$42 constant panic-scream-sound    \ struck while panicking
$43 constant seized-sound          \ seized, flinching
$44 constant attack-sound          \ her panic attack
$45 constant push-sound
$71 constant rattle-sound          \ a locked door's handle
$7D constant pull-free-cry         \ pulling free, dragged, carried
$7E constant slide-sound           \ sliding down after a panic fall
$7F constant bump-sound            \ running blindly into a wall
$80 constant falling-cry
$81 constant long-fall-cry         \ off a ladder from high up
$8F constant shove-hit-sound       \ her shove landed
$90 constant kick-hit-sound        \ her kick landed
$91 constant slam-sound            \ a door slammed shut
