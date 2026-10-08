\ story-names.fs - the story flags (the scenario's, the progress' +0x1C; ~0x36B of them) by
\ what they mean, named as they are worked out from the room scripts. Use the names.
IN: story-names

$C   constant opening-done        \ the cage room's opening has played (its entering script plays it otherwise)
$33  constant grate-open          \ the cage room's grate is open (its way out unblocked, the grate's parts hidden)
$AF  constant items-40-as-70      \ ? the scripts' items / rooms 0x40 and 0x41 count as 0x70 (Events_ScriptRoom)
