\ Hewie's pounce and rush (hewie/pounce.fs): the leap at his target ($1E01 up to 5 above it, down,
\ landing $1E03), the stalker bitten by his mouth on the way; the rush (0x6D) biting what it reaches.
\ Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/hewie/test_pounce.fs
IN: test-pounce
USING: tester engine game-state actors messages common facts room-names game debug flag-names hewie.state hewie.body hewie.model hewie.states hewie.fight hewie.pounce hewie.more fiona.state stalker.state stalker.senses stalker.moving stalker.search stalker.chase stalker.bitten story.words creatures ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his@ ( xt -- x )  game:hewie enter  execute  leave-actor ;
: his! ( xt -- )  game:hewie enter  execute  leave-actor ;
: deb@ ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: deb! ( xt -- )  game:debilitas enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
1 seed !

free-play  castle-1f-1 room!  20 frames
$B7 tri-center 0e tp-facing  5 frames  game:hewie send join-fiona  20 frames
stalkers-blind state-flag-set  s" fiona" actor-named body-tri debug:debilitas-tri  3 frames
[: 3 mode!  0 temper !  70 hp ! ;] deb!  3 frames
: calm ( -- )  [: 1000 add-trust  obeys  0 -1 set-mood  100 hp!  0 cond!  0 his-ready ! ;] his! ;
fvariable y0  fvariable top
: pounce-at ( -- )   \ (facing him: as when harrying, within 60 degrees)
    [: game:debilitas his-target !  game:debilitas body-at heading-to his-yaw!
       game:debilitas body-at his-at vec-dist  pounce ;] his! ;
: rising? ( -- flag )  [: his-at 4 + sf@  top f@ fmax top f! false ;] his@ ;

calm  5 frames
testing the pounce: the leap ($1E01), up above where he was, down and landing ($1E03), himself again
[: his-at 4 + sf@ fdup y0 f! top f! ;] his!
pounce-at
T{ [: anim@ ;] his@ -> $1E01 }T
T{ [: rising? drop [: anim@ $1E03 = ;] his@ ;] 120 soon -> -1 }T
T{ top f@ y0 f@ 1e f+ f> -> -1 }T
T{ [: [: his-act @ ['] st-landing <> ;] his@ ;] 120 soon -> -1 }T
testing ... his mouth reached the stalker: bitten - from in front 5, behind 8 (not hard, no boost)
T{ [: hp @ ;] deb@ dup 62 = swap 65 = or -> -1 }T

testing the rush (0x6D): at her (standing still), the bite once his mouth reaches her - done
debug:debilitas-out  5 frames
[: [: fiona-mode @ 0= ;] his@ ;] 600 soon drop   \ (her free again)
calm  [: her his-target !  close-in ;] his!
T{ [: [: his-act @ ['] st-rush = ;] his@ ;] 2 soon -> -1 }T
T{ [: [: his-act @ ['] st-1980 = ;] his@ ;] 200 soon -> -1 }T
T{ [: [: his-act @ ['] st-rush <> ;] his@ ;] 5 soon -> -1 }T

testing the flank (0x12): round to the stalker's side - settled there (within 40, behind him), then himself
game:hewie body-tri debug:debilitas-tri  3 frames  [: 3 mode!  0 temper ! ;] deb!   \ (waiting about: not after her)
[: 2 mode! ;] deb!  calm  [: $12 0 want ;] his!
T{ [: his-act @ ['] st-flank = ;] his@ -> -1 }T
T{ [: [: his-t2 @ 1 = his-act @ ['] st-flank = and ;] his@ ;] 600 soon -> -1 }T

testing a homunculus up and after her, the stalker gone: his target (Hewie_PickTarget); the rush strikes it, his temper with her easing
debug:debilitas-out  10 frames
: room-actor ( -- id )  room-id room-name actor-named ;
: h7 ( -- id )  game:creatures enter  7 slot@  leave-actor ;
: of7 ( xt -- x )  h7 enter  execute  leave-actor ;
s" fiona" actor-named body-tri constant her-tri
[: room-id 2 her-tri $80 0 -1 0 0e creature-place ;] room-actor enter execute leave-actor  deliver  5 frames
T{ [: creatures.homunculus:awake @ ;] of7 -> -1 }T
T{ [: pick-target ;] his@ h7 = -> -1 }T
h7 constant the-homunculus
calm  [: 2 his-hits !  the-homunculus his-target !  close-in ;] his!
T{ [: [: creatures.homunculus:move-mode @ 4 = ;] of7 ;] 200 soon -> -1 }T
T{ [: [: his-hits @ ;] his@ 1 = ;] 5 soon  [: creatures.homunculus:strength @ ;] of7 -> -1 0 }T   \ (5 less his bite's 5: it leaves)

test-summary
