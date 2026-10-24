\ Fiona's face and hands (actor.c; the original's Motion_Eyes, Motion_Hands): out of scenes she
\ blinks - her eyelids' shapes (7 and 8) down and up again, by 150 frames at the latest - and her
\ hands have her models' own shape; her earrings and choker on her clothes are the ones she wears
\ (Fiona_ShowEquipment: EventHumanModel_ShowParts), her other pairs hidden. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_face.fs
IN: test-face
USING: tester engine game-state actors messages game fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: model ( -- id )  game:fiona enter  her-model @  leave-actor ;
: lid ( F: -- w )  model actor act.face 7 4 * + sf@ ;
: hand-shape ( hand -- shape )  model actor act.face 16 4 * + swap 12 * + l@ ;
variable shut
: watch-lids ( n -- )  0 shut !  0 ?do  game-tick  lid 0.5e f> if  1 shut +!  then  loop ;
: shown? ( part -- flag )   \ (its bit in the model's parts shown: the part's number + 1)
    1+ {: g :}  model actor act.parts-shown  g 5 rshift 4 * + l@  g 31 and rshift 1 and 0<> ;
: wear ( k -- )  game:fiona send costume  deliver  2 frames ;
: earrings! ( item -- )   \ (worn in the equipment's slot 1, as the sub screen says)
    progress pr.equipped 4 + l!  -1 -1 -1 -1 broadcast equipment  deliver  2 frames ;

2 frames
0 wear
testing she blinks: her eyelids shut for a few frames, more than once in 300
T{ model actor act.alive l@  300 watch-lids  shut @ 4 > -> 1 -1 }T
testing her hands' own shape (CharModel_Loaded: 1), from her motion's start
T{ 0 hand-shape  1 hand-shape -> 1 1 }T
testing no earrings worn: none of the four pairs shown; the bare neck's parts (3, 4) shown
T{ -1 earrings!  32 shown?  36 shown?  37 shown?  33 shown?  3 shown?  4 shown? -> 0 0 0 0 -1 -1 }T
testing the second pair ($87) worn: its part (36) alone
T{ $87 earrings!  32 shown?  36 shown?  37 shown?  33 shown? -> 0 -1 0 0 }T
testing the slip (no such parts): nothing hidden
-1 earrings!  1 wear
T{ 32 shown?  36 shown? -> -1 -1 }T

test-summary
