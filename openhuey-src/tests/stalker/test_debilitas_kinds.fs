\ Debilitas as each kind the story loads him as (stalker.fs debilitas-setup: the original's
\ Debilitas_Setup, Debilitas2_Setup, Debilitas3_Setup, Kind27_Setup): his kit, his chase, giving
\ up, health, Hewie's numbers, the summoner's, his clocks, his rooms - by the hard setting; his
\ kind the cutscenes' cast finds him by. And the other stalkers' clocks (their Setups' +0x16D4 /
\ +0x16D0), kind 37's reach, the TintStalker's time after Hewie, the Pursuer's way round her
\ (Pursuer_BehaviourStalk $19 / $1A). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_debilitas_kinds.fs
IN: test-debilitas-kinds
USING: story.words tester engine game-state actors messages common room-names game debug flag-names facts stalker.state stalker.tables stalker.search stalker.struck stalker.chase stalker.pursue stalker.names ;

: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: as ( kind -- )  partner-load  deliver  2 run-frames ;
: whose ( xt name len -- x )  actor-named enter  execute  leave-actor ;
: hard-mode! ( on -- )  hard-mode-flag progress pr.story rot if  bit-set  else  bit-clear  then ;
\ his numbers: health, Hewie's tolerance, the throw's chance, his time after Hewie, the summoner's
\ time away and cooldown, his clocks after her and down
: numbers ( -- hp tol throw dog away cool after down )
    hp-max @  tolerance @  throw-k f@ f>s  dog-limit @  away-base @  cool-secs @  after-frames @  down-frames @ ;
\ his ways: the Pursuer's chase, giving up and at what, his rooms, his kind
: ways? ( -- chase gives hits a b kind )  base-chase @  gives-up @  give-up-hits @  own-a @  own-b @  my-kind @ ;

free-play  front-garden-2 room!  20 run-frames
stalkers-blind state-flag-set
testing kind 2 (Debilitas_Setup): his own chase, 20 damage to give up, his one room $20
T{ 2 as  [: kit @ debilitas-kit =  numbers ;] his -> -1 70 20 10 1800 4500 90 300 1800 }T
T{ [: ways? ;] his -> 0 -1 20 $20 -1 2 }T
testing kind 6 (Debilitas2_Setup): the Pursuer's chase, 80 damage to give up, his rooms $20 and $02
T{ 6 as  [: kit @ debilitas6-kit =  numbers ;] his -> -1 90 30 10 1800 10800 150 300 1800 }T
T{ [: ways? ;] his -> -1 -1 80 $20 $02 6 }T
testing kind 7 (Debilitas3_Setup): the Pursuer's chase, never giving up, no rooms of his own
T{ 7 as  [: kit @ debilitas7-kit =  numbers ;] his -> -1 110 40 12 1800 9000 150 300 1350 }T
T{ [: ways? ;] his -> -1 0 0 -1 -1 7 }T
testing kind 27 (Kind27_Setup): as kind 7, its own tables, Hewie's tolerance 30
T{ 27 as  [: kit @ kind27-kit =  numbers ;] his -> -1 110 30 12 1800 9000 150 300 1350 }T
T{ [: ways? ;] his -> -1 0 0 -1 -1 27 }T
testing the hard setting: 2 110, after her 540, down 1350; 6 135 throw 15; 7 165 throw 18
-1 hard-mode!
T{ 2 as  [: hp-max @  after-frames @  down-frames @ ;] his -> 110 540 1350 }T
T{ 6 as  [: hp-max @  throw-k f@ f>s  after-frames @ ;] his -> 135 15 300 }T
T{ 7 as  [: hp-max @  throw-k f@ f>s  hp @ ;] his -> 165 18 165 }T
0 hard-mode!
testing his attack tables are his kind's (situation 0, normal and hard)
T{ 6 as  [: 0 false table  0 true table ;] his -> db6-tables @  db6-tables 34 cells + @ }T

testing the cutscenes find him by his kind (char-model, speak, reflections: slot 2's kind)
T{ 6 as  6 cast-model  [: my-model @ ;] his =  stalker-who char-model  [: my-model @ ;] his = -> -1 -1 }T
T{ 27 as  27 cast-model  [: my-model @ ;] his =  2 cast-model 0< -> -1 -1 }T
T{ 2 as  2 cast-model  [: my-model @ ;] his =  6 cast-model 0< -> -1 -1 }T

testing in the game: kind 6 gives up at 80 damage, not 25; kind 7 never
castle-1f-2 room!  10 run-frames
6 as  castle-1f-10 0 game:debilitas send stalker-in  2 run-frames
T{ [: 25 hits !  give-up?  85 hits !  give-up? ;] his -> 0 -1 }T
debug:debilitas-out  2 run-frames
7 as  castle-1f-10 0 game:debilitas send stalker-in  2 run-frames
T{ [: 200 hits !  give-up?  after-her mode!  mode-t @ ;] his -> 0 300 }T
debug:debilitas-out  2 run-frames
testing ... knocked out, down his setup's frames (kind 7 1350)
front-garden-2 room!  10 run-frames
game:fiona body-tri debug:debilitas-tri  3 run-frames
T{ [: 0 hp !  knock-down  down-t @  cond @ ;] his -> 1350 down-cond }T
debug:debilitas-out  2 run-frames  2 as

testing the others' clocks (after her, down): Daniella 450 750, Riccardo 300 1200, Lorenzo 600
testing ... 510, Lorenzo2 900 0, kind 12 29970 0, kinds 34..37 and the TintStalker
: clocks? ( -- after down )  after-frames @  down-frames @ ;
T{ ' clocks? s" daniella" whose  ' clocks? s" riccardo" whose  ' clocks? s" lorenzo" whose -> 450 750 300 1200 600 510 }T
T{ ' clocks? s" lorenzo2" whose  ' clocks? s" kind39" whose  ' clocks? s" kind12" whose -> 900 0 900 0 29970 0 }T
T{ ' clocks? s" kind34" whose  ' clocks? s" kind35" whose  ' clocks? s" kind36" whose -> 300 900 600 450 300 900 }T
T{ ' clocks? s" kind37" whose  ' clocks? s" tintstalker" whose -> 300 150 300 1200 }T
testing ... none of them gives up (Pursuer_GivesUp); kind 37's reach the Pursuer's 24; the TintStalker after Hewie 1800
: gives? ( -- f )  gives-up @ ;
T{ ' gives? s" riccardo" whose  ' gives? s" lorenzo" whose  ' gives? s" lorenzo2" whose  ' gives? s" kind39" whose -> 0 0 0 0 }T
T{ ' gives? s" kind37" whose  ' gives? s" tintstalker" whose -> 0 0 }T
T{ [: attack-range f@ f>s ;] s" kind37" whose  [: dog-limit @ ;] s" tintstalker" whose -> 24 1800 }T

testing the Pursuer's way round her ($1A): far off (past 100) stalking for the row's frames; near, done, walking at her
: dan ( xt -- x )  s" daniella" whose ;
T{ [: 0 her-stage !  150e d-fiona f!  0 step-done !  true round-or-taunt  chase-act @  hold-t @ ;] dan -> stalk-act [: 0 pursue-row 2 cells + @ ;] dan }T
T{ [: 50e d-fiona f!  -1 step-done !  true round-or-taunt  chase-act @  hold-t @ ;] dan -> walk-at-act [: 0 pursue-row cell+ @ ;] dan }T
testing ... his third taunt ($19) far off: stalking with its gesture flagged (Daniella's first), else walking at her
T{ [: 150e d-fiona f!  -1 step-done !  0 taunt-gesture !  taunt-gesture @ gesture-flag? round-or-taunt  chase-act @ ;] dan -> stalk-act }T
T{ [: 150e d-fiona f!  -1 step-done !  false round-or-taunt  chase-act @ ;] dan -> walk-at-act }T
testing ... the flag his gesture table's (Debilitas: $1302 flagged, $1306 not)
T{ [: 1 gesture-flag?  2 gesture-flag? ;] his -> -1 0 }T

test-summary
