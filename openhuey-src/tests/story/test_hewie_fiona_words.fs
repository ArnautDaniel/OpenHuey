\ The story's words about Hewie and Fiona (H4 / F5: the scripts' conds 0x0F 0x24 0x26 0x31 0x33 0x3D
\ 0x3F 0x4C 0x5A and commands 0x20 0x21 0x39 0x3F 0x43 0x78 0x85 0x93 0x94 0x95 0xBD 0xC4 0xC5 0xC9
\ 0xD0; docs/subsystems/hewie.md, fiona.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_hewie_fiona_words.fs
IN: test-hewie-fiona-words
USING: tester engine game-state actors messages common room-names game debug flag-names hewie.state hewie.body hewie.states fiona.state story.state story.words ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
1 seed !

-1 story:places-off !   \ (front-garden-2 as its converted room: the machinery's host)
free-play  front-garden-2 room!  20 run-frames  game:hewie send join-fiona  20 run-frames
testing his action told (0x24) and set (0x39): lying by her (1)
[: 1 0 hewie-do ;] in-room  2 run-frames
T{ [: his-action @ ;] his  [: 1 hewie-action?  $15 hewie-action? ;] in-room -> 1 -1 0 }T
testing ... she may command him then (0x4C), near enough (0x0F), he may be broken off (0x31), can reach her (0x5A)
T{ [: hewie-can-command?  hewie-near-command?  hewie-may-break-off?  hewie-reachable? ;] in-room -> -1 -1 -1 -1 }T
testing ... angry (0xC4: his mood 3): not
[: 3 hewie-mood ;] in-room  2 run-frames
T{ [: his-mood @ ;] his  [: hewie-can-command?  hewie-may-break-off? ;] in-room -> 3 0 0 }T
[: 0 hewie-mood ;] in-room  2 run-frames
testing his trust (0x85), his half of the room (0xD0, 0x33), his wait (0xBD)
variable was  [: his-trust-points @ ;] his was !  [: 300 hewie-trust  1 hewie-side  hewie-stay-put ;] in-room  2 run-frames
T{ [: his-trust-points @ ;] his was @ -  [: his-way @  his-pet-time @ 290 > ;] his  [: 1 hewie-side? ;] in-room -> 300 1 -1 -1 }T
testing a point to look to (0xC5): zone 3 (in variable 5), 2 higher - he has none
[: 3 2 10 1  140e 124e -200e zone  5 3 var-set  5 2e hewie-look-zone ;] in-room  [: 0 his-smells ! ;] his
[: 5 2e hewie-look-zone ;] in-room  2 run-frames
T{ [: his-scent sf@ f>s  his-scent 4 + sf@ f>s  his-scent 8 + sf@ f>s ;] his -> 140 126 -200 }T   \ (his flag: used up)
[: 0 his-smells ! ;] his
testing his bark (0x78: his move 0x12, action 0x45)
[: hewie-bark ;] in-room
T{ [: [: his-action @ $45 = ;] his ;] 10 soon -> -1 }T
[: 0 0 hewie-do ;] in-room  10 run-frames
testing through blocked floor and left alone, as the script's own (0x20, 0x21)
[: ctx-who @ >r  1 ctx-who !  1 self-noclip  1 self-scripted  r> ctx-who ! ;] in-room  2 run-frames
T{ [: his-2b @  his-2d @ ;] his -> 1 1 }T
[: ctx-who @ >r  1 ctx-who !  0 self-noclip  0 self-scripted  r> ctx-who ! ;] in-room  2 run-frames

testing Fiona free (0x3D), her action (0x3F), her command this frame (0x26: none)
T{ [: fiona-free?  0 fiona-action?  35 fiona-started? ;] in-room -> -1 -1 0 }T
testing calmer (0x94: n / 30 off her fear), her recovery shorter (0x95: 60 / n)
[: 50e her-fear f!  600 her-recovery ! ;] hers  [: 300 fiona-calm  2 fiona-recovery-lower ;] in-room  2 run-frames
T{ [: her-fear f@ 40.5e f< her-fear f@ 39e f> and  her-recovery @ 570 <= ;] hers -> -1 -1 }T
testing the threat (0x43) raises the panic; calming (0x93); set to a level it hasn't passed (0xC9)
[: 60 threat-raise ;] in-room  1 run-frames
T{ game:panic enter  panic:level f@ f>s  leave-actor -> 36 }T   \ (half lasting, half passing - each x0.6 with Hewie within 20 of her)
[: 80 panic-level ;] in-room  2 run-frames
T{ game:panic enter  panic:level f@ 79e f>  panic:stage @  leave-actor -> -1 2 }T   \ (80, easing)
[: 40 panic-level ;] in-room  2 run-frames
T{ game:panic enter  panic:level f@ 79e f>  leave-actor -> -1 }T
[: $80 panic-calm-in ;] in-room  2 run-frames
T{ game:panic enter  panic:level f@ 10e f<  leave-actor -> -1 }T
test-summary
