\ Riccardo (P4c): his kit, his own chase (Riccardo_StateBehaviour / Riccardo_Chase), his flurry of
\ hammer blows and his lunge, his attack by how near, his rage at the panic's stage 5
\ (docs/subsystems/stalker.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/stalker/test_riccardo.fs
IN: test-riccardo
USING: tester engine game-state actors messages common room-names game debug flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.tables stalker.attack stalker.chase stalker.pursue stalker.riccardo fiona.state ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: ric ( -- id )  s" riccardo" actor-named ;
: his ( xt -- x )  ric enter  execute  leave-actor ;
: fiona@ ( xt -- x )  game:fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
fvariable dx  fvariable dz
\ Fiona `d` ahead of him (he faces 340 degrees), facing `deg`
: her-ahead ( deg -- ) ( F: d -- )
    ric body-pos dz f! fdrop dx f!
    fdup 340e deg>rad fsin f* dx f@ f+  ric body-pos fdrop fswap fdrop  frot 340e deg>rad fcos f* dz f@ f+
    s>f tp-facing ;
1 seed !

free-play  front-garden-2 room!  20 frames
testing his kit: his tables, his own chase and actions, his reach (20), his model and motions
T{ [: kit @ riccardo-kit =  own-chase @ ['] ric-begin =  own-acts @ riccardo-acts =  attack-range f@ f>s ;] his -> -1 -1 -1 20 }T
T{ [: hp @  my-kind @  my-model @ $1300 has-motion?  my-model @ $E00 has-motion? ;] his -> 100 4 -1 -1 }T
T{ [: throw-moves  0 wait#  4 wait#  waits 5 cells + f@ f>s ;] his -> 16 17 60 60 20 }T

testing his attack by how near: within 10 his grab (3); past 150 none
T{ [: 5e attack-for  200e attack-for ;] his -> 3 0 }T

testing in her room, she turned from him 30 off: he walks at her (60) or stalks her (150) by a roll
room-id game:fiona body-tri ric send stalker-in  3 frames
[: 340e deg>rad body-turn ;] his  1 frames
30e 340 her-ahead  3 frames
T{ [: [: sees-fiona @ ;] his ;] 60 soon -> -1 }T
[: ric-begin ;] his
T{ [: step @ ['] ric-run =  chase-act @ dup 5 = swap 6 = or  hold-t @ 59 > ;] his -> -1 -1 -1 }T
testing ... 60 off, facing him: he stands to her ($1, the comings-on from his waits)
60e 160 her-ahead  2 frames
[: ric-begin ;] his
T{ [: chase-act @  comings @ ;] his -> 1 60 }T

testing his flurry: 1..7 blows; on her floor a swing ($E00 / $E04)
10e 160 her-ahead  2 frames
[: flurry-start ;] his
T{ [: chase-act @  blows @ 1 8 within  anim @ dup $E00 = swap $E04 = or ;] his -> $1000 -1 -1 }T
testing ... at its key it strikes her: within 10 his grab (3) - she is seized ($10)
T{ [: [: her-mode @ 4 = ;] fiona@ ;] 200 soon  [: her-sub @ ;] fiona@ -> -1 $10 }T

testing his lunge ($1300), and his tables' own actions ($1000 the flurry, $1001 the lunge)
[: lunge-start ;] his 
T{ [: anim @  chase-act @ ;] his -> $1300 $1001 }T
T{ [: $1001 0 do-row  chase-act @ ;] his -> $1001 }T

testing his rage at the panic's stage 5, and out of it below
[: 5 her-stage !  0 reeling !  0 raging !  riccardo-frame ;] his
T{ [: raging @  4 her-stage !  riccardo-frame  raging @ ;] his -> -1 0 }T

testing the story's kind 4 is him
T{ 4 story.words:stalker-actor  ric = -> -1 }T
ric send stalker-out  3 frames
test-summary
