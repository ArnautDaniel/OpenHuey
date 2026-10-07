\ the converted event scripts: they all load, and each one, run as a task with the stub
\ conditions answering at random, leaves its stacks as it found them
IN: test-events
USING: tester game-state events.core events.words events.builtin events.map0 events.map1 events.map2 events.map3
    events.map4 events.other ;

variable seed  12345 seed !
: rnd ( -- n )  seed @ 1103515245 * 12345 + $7FFFFFFF and dup seed ! ;
: coin ( -- flag )  rnd 16 rshift 1 and 0<> ;
:noname 2drop coin ; is stub-flag  :noname 2drop yield ; is stub-step

variable script   variable finished   variable unbalanced   variable runs
\ each script runs as if in action slot 0 for Fiona (its waits wait - nobody carries out her moves
\ here - and its frame count counts)
reset-characters  0 0 character char.id l!  -1 0 character char.present l!
: checked ( -- )
    0 event-state ev.slot l!  0 0 script-slot slot.frames l!  0 event-state ev.self-char l!
    0 0 character char.move l!
    script @ execute
    depth fdepth or if 1 unbalanced +! ." unbalanced: " script @ . cr then
    true finished ! ;
\ run `xt` as a task for up to 300 frames (a script waiting on a stub forever is stopped)
: try ( xt -- )
    ?dup 0= if exit then
    script !  false finished !  1 runs +!
    ['] checked spawn
    300 0 do  finished @ if leave then  1 0 script-slot slot.frames +l!  1 tick-tasks  loop
    finished @ if drop else kill then ;
: try-all ( -- )
    rooms 0 do
        7 0 do j i room-script @ try loop
        $80 0 do j i action-script @ try loop
    loop
    $100 $80 do i builtin-script @ try loop ;

testing every script, three times
0 unbalanced !  0 runs !
try-all try-all try-all
T{ unbalanced @ -> 0 }T
T{ runs @ 6000 > -> true }T
