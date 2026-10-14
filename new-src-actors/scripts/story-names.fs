\ story-names.fs - the story flags (the scenario's, the progress' +0x1C; ~0x36B of them) by
\ what they mean, named as they are worked out from the room scripts. Use the names.
IN: story-names

$C   constant opening-done        \ the cage room's opening has played (its entering script plays it otherwise)
$33  constant grate-open          \ the cage room's grate is open (its way out unblocked, the grate's parts hidden)
$AF  constant items-40-as-70      \ ? the scripts' items / rooms 0x40 and 0x41 count as 0x70 (Events_ScriptRoom)
\ ---- the game over's music (GameOver's faded-out sequence picks its track by these: the
\ latest set wins - $A with none). What they mean in the story: not yet worked out ----
$41  constant over-music-b        \ ? the game over's track $B (+0x24 bit 2)
$82  constant over-music-c        \ ? its track $C (+0x2C bit 4)
$94  constant over-music-d        \ ? its track $D (+0x2C bit 0x100000)
$95  constant over-music-e        \ ? its track $E (+0x2C bit 0x200000)
$A4  constant over-music-f        \ ? its track $F (+0x30 bit 0x10)
\ ---- the items (src/game/items.c, story_chars.c: their uses test and set these) ----
$7   constant flowers-laid        \ ? the Fresh Flowers can't be used any more (their use's spot is done)
$37  constant garden-door-done    \ ? the Garden Key's door (room $25's door 1) no longer takes it
$42  constant saturnus-hint-on    \ ? the Saturnus Key used elsewhere adds its hint ($232)
$66  constant books-placed        \ ..$6E: book b (0..2: items $15..$17) on lectern p (0..2: room $54's
                                  \ spots $D, $11, $14) is flag books-placed + p * 3 + b
$20  constant plate-made          \ ? a word plate has been made (SubScreen_StateWordMake)
$85  constant medallion-40-done   \ ? room $40's medallion spot no longer takes one
$8E  constant c7-second-event     \ ? the books' spot in room $C7 starts event $10, not 3
\ the maps held (Castle / Mansion / Forest / Tower / Mystery sketch used: the sub screen's map
\ pages; Progress_MapsHeld) and the flowers withered (the Fresh Flowers' 9000 frames)
$356 constant castle-map-held
$357 constant mansion-map-held
$358 constant forest-map-held
$359 constant tower-map-held
$35A constant mystery-map-held
$35F constant flowers-withered
