\ The story's stalker words (P4a: the scripts' conditions 0x10 0x1E 0x30 0x3C 0x41 0x45 0x52 0x54
\ 0x55 0x65 and commands 0x15 0x2A 0x2F 0x30 0x31 0x3D 0x47 0x48 0xB8; docs/subsystems/stalker.md).
\ Headless:  build/openhuey-src/openhuey --test openhuey-src/tests/story/test_stalker_words.fs
IN: test-stalker-words
USING: tester engine game-state actors messages common room-names game debug flag-names stalker.state story.state facts story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
1 seed !

free-play  front-garden-2 room!  20 frames
stalkers-blind state-flag-set
testing loaded (0x11) but not in the game: his kind (0x54), not active (0x55), no alert ($FF)
2 partner-load  2 frames
T{ [: 2 stalker-kind?  $22 stalker-kind?  stalker-active?  $FF stalker-alert? ;] in-room -> -1 0 0 -1 }T
testing Daniella as a scene's figure (char-load into slot 3) leaves his sounds: bank 7 stays his
3 3 $FF char-load  deliver  2 frames
T{ 7 sound-bank-file s" O_DB0/DB0_000" compare  game:daniella body?  -> 0 0 }T
[: stalker.state:my-slot @ ;] game:daniella enter execute leave-actor  constant her-slot
T{ her-slot -> 3 }T
testing in her room: active, his kind here (0x3C), waiting about (alert 3), free (0x65), well (0x41)
s" fiona" actor-named body-tri constant her-tri
-500e 124e -500e 0e tp-facing  3 frames   \ (her well out of his sight)
her-tri debug:debilitas-tri  3 frames
T{ [: stalker-active?  2 stalker-kind-here?  3 stalker-alert?  stalker-free?  $FE 0 char-cond? ;] in-room -> -1 -1 -1 -1 -1 }T
testing the exit he came in by (0x30)
[: 2 stalker.state:came-by ! ;] his  2 frames
T{ [: $FE 2 char-heading-for?  $FE 1 char-heading-for? ;] in-room -> -1 0 }T
testing ordered after her (0x2F 0): his alert 0
[: $FE 0 stalker-command ;] in-room  3 frames
T{ [: 0 stalker-alert? ;] in-room -> -1 }T
testing ... to search (0x2F 3): alert 2; his clock (0x2A)
[: $FE 3 stalker-command  $FE 300 stalker-search-delay ;] in-room  1 frames
T{ [: 2 stalker-alert? ;] in-room  [: mode-t @ 300 <= mode-t @ 290 > and ;] his -> -1 -1 }T
testing in a rage (0x31): his stance; silenced (0x3D)
[: $FE 1 stalker-rage  $FE 1 char-silent ;] in-room  2 frames
T{ [: raging @  hushed @ ;] his -> -1 -1 }T
testing his condition set (0x47), health (0x48, 0xB8, 0x52)
[: $FE 1 char-cond! ;] in-room  2 frames
T{ [: $FE 1 char-cond? ;] in-room  [: cond @ ;] his -> -1 1 }T
[: 30 hp ! ;] his  [: $FE 5 char-heal ;] in-room  2 frames
T{ [: hp @ ;] his  [: $FE char-full-health? ;] in-room -> 35 0 }T
[: $FE char-full-health ;] in-room  2 frames
T{ [: $FE char-full-health? ;] in-room -> -1 }T
testing knocked down (0x30): out of health, down - dead (0x1E), not free
[: $FE 0 char-cond!  $FE stalker-knock-down ;] in-room  3 frames
T{ [: $FE char-dead?  stalker-free?  $FE 2 char-cond? ;] in-room -> -1 0 -1 }T
testing started again (0x15): waiting about, no rage, his health back
[: $FE char-activate ;] in-room  3 frames
T{ [: raging @  my-mode @  hp @ hp-max @ = ;] his -> 0 3 -1 }T
140.64e 124e -217.33e 0e tp-facing  5 frames
testing out of sight (0x45): her on the camera, and her place
T{ [: 0 char-unseen?  140 124 -217 point-on-camera? ;] in-room -> 0 -1 }T
debug:debilitas-out  3 frames
T{ [: $FE char-unseen?  stalker-active? ;] in-room -> -1 0 }T
stalkers-blind state-flag-clear
test-summary
