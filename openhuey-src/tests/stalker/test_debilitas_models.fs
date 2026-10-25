\ Debilitas's model by the kind the story loads him as (stalker.fs debilitas-dress; the original's
\ Pursuer_ModelFiles): kind 2 O_DB0, kind 6 (Debilitas2) O_DB1, kinds 7 (Debilitas3) and 27 O_DB2 -
\ his sounds (bank 7) theirs too - and back to O_DB0 as kind 2 again. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_debilitas_models.fs
IN: test-debilitas-models
USING: tester engine game-state actors messages common room-names game debug flag-names stalker.state facts story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: as ( kind -- )  partner-load  deliver  2 frames ;
: model? ( -- flag )  [: my-model @ ;] his  0< 0= ;
: bank? ( addr len -- flag )  7 sound-bank-file compare 0= ;
: model-file? ( addr len -- flag )  [: stalker:file ;] his  compare 0= ;

free-play  front-garden-2 room!  20 frames
stalkers-blind state-flag-set
testing kind 2: O_DB0
T{ 2 as  s" O_DB0/DB0_000" model-file?  s" O_DB0/DB0_000" bank?  model? -> -1 -1 -1 }T
testing kind 6 (Debilitas2): O_DB1
T{ 6 as  s" O_DB1/DB1_000" model-file?  s" O_DB1/DB1_000" bank?  model? -> -1 -1 -1 }T
testing kind 7 (Debilitas3): O_DB2
T{ 7 as  s" O_DB2/DB2_000" model-file?  s" O_DB2/DB2_000" bank?  model? -> -1 -1 -1 }T
testing kind 27: O_DB2
T{ 27 as  s" O_DB2/DB2_000" model-file?  model? -> -1 -1 }T
testing kind 2 again: O_DB0
T{ 2 as  s" O_DB0/DB0_000" model-file?  s" O_DB0/DB0_000" bank?  model? -> -1 -1 -1 }T
testing he comes into her room as kind 7, shown, blinking
7 as
s" fiona" actor-named body-tri debug:debilitas-tri  5 frames
T{ [: my-model @ ;] his actor dup act.visible l@  swap act.alive l@ -> 1 1 }T

test-summary
