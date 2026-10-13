\ Lorenzo's other class (P4c; kind 10): his kit, his "rage" from the panic's first stage, his own
\ actions under the floor - sinking to come up by her, stalking her from below and his sweep,
\ his reach from afar (docs/subsystems/stalker.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/stalker/test_lorenzo2.fs
IN: test-lorenzo2
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.lorenzo2 fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: ly ( -- id )  s" lorenzo2" actor-named ;
: his ( xt -- x )  ly enter  execute  leave-actor ;
: fiona@ ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: shown? ( -- flag )  [: my-model @ actor act.visible l@ 0<> ;] his ;
fvariable dx  fvariable dz
: her-ahead ( deg -- ) ( F: d -- )   \ (he faces 340 degrees)
    ly body-pos dz f! fdrop dx f!
    fdup 340e deg>rad fsin f* dx f@ f+  ly body-pos fdrop fswap fdrop  frot 340e deg>rad fcos f* dz f@ f+
    s>f tp-facing ;
: d-ly ( -- n )  [: d-fiona f@ f>s ;] his ;
1 seed !

free-play  front-garden-2 room!  20 frames
testing his kit, his own actions, his model's moves under the floor
T{ [: kit @ lorenzo2-kit =  base-chase @  own-acts @ lorenzo2-acts =  hp @  my-kind @ ;] his -> -1 -1 -1 250 10 }T
T{ [: my-model @ dup $1303 has-motion?  over $1304 has-motion?  rot $E02 has-motion? ;] his -> -1 -1 -1 }T
testing his "rage" from the panic's first stage: his reach 18 (else from afar, 70)
T{ [: 1 her-stage !  0 reeling !  0 raging !  lorenzo2-frame  raging @  attack-range f@ f>s ;] his -> -1 18 }T
T{ [: 0 her-stage !  lorenzo2-frame  raging @  attack-range f@ f>s  reach-k f@ f>s ;] his -> 0 70 60 }T

room-id game:fiona body-tri ly send stalker-in  3 frames
[: 340e deg>rad body-turn ;] his  1 frames
testing $1001 stalking her from below: he sinks ($1304), out of sight under the floor, comes up by her in his sweep ($E02)
40e 160 her-ahead  3 frames
[: below-start ;] his
T{ [: anim @ ;] his -> $1304 }T
T{ [: shown? 0= ;] 120 soon  [: [: anim @ $E02 = ;] his ;] 60 soon  shown? -> -1 -1 -1 }T
testing ... up within 10 of her; its key strikes her
T{ [: d-ly 0 10 within ;] 10 soon  [: [: her-mode @ 4 = ;] fiona@ ;] 90 soon -> -1 -1 }T
[: [: her-mode @ 0= ;] fiona@ ;] 300 soon drop
testing $1000 sinking to come up by her: near her (under 30) his move 1 instead
2 frames
T{ [: sink-start  chase-act @ ;] his -> $13 }T
testing his reach from afar ($E01)
T{ [: grab-start  anim @ ;] his -> $E01 }T
testing the story's kind 10 is him
T{ 10 story.words:stalker-actor  ly = -> -1 }T
ly send stalker-out  3 frames

\ ---- kind 39: the same class with its own tables, no "rage" ----
: k39 ( -- id )  s" kind39" actor-named ;
testing kind 39: its kit, his own actions, his reach 18 whatever the panic
T{ k39 enter  kit @ kind39-kit =  own-acts @ lorenzo2-acts =  hp @  my-kind @  attack-range f@ f>s  leave-actor -> -1 -1 250 39 18 }T
T{ 39 story.words:stalker-actor  k39 = -> -1 }T
test-summary
