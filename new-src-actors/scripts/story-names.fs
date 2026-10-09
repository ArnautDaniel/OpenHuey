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
