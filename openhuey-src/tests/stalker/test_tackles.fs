\ Hewie's tackles (stalker/struck.fs `tackled`; Pursuer_AddBiteDamage): a hit from Hewie that isn't
\ a bite or a hold raises the stalker's temper by where it caught him - his temper table's in front,
\ in front low, behind, behind low. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_tackles.fs
IN: test-tackles
USING: tester engine game-state actors messages facts room-names game debug flag-names stalker stalker.state stalker.tables stalker.moving stalker.travel stalker.struck ;

: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: tackle ( kind dir -- temper )  0 temper !  tackled  temper @ ;

free-play  front-garden-2 room!  10 run-frames
testing Debilitas tackled by Hewie: in front 5, in front low 8, behind 10, behind low 16
T{ [: hewie striker !  1 0 tackle  1 2 tackle  1 1 tackle  2 3 tackle ;] his -> 5 8 10 16 }T
testing ... not a bite ($B) or a hold ($A); not from Fiona
T{ [: hewie striker !  $B 1 tackle  $A 1 tackle  fiona striker !  1 1 tackle ;] his -> 0 0 0 }T
testing ... at most 1000
T{ [: hewie striker !  998 temper !  0 0 tackled  temper @ ;] his -> 1000 }T

testing Hewie let go of his leg (Pursuer_StateKnockedDown $1709): down on his front ($1709, lying as $1806), up again ($1807)
stalkers-blind state-flag-set  s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
T{ [: 70 hp !  leg-down  anim @  lie-anim @ ;] his -> $1709 $1806 }T
T{ [: [: anim @ $1807 = ;] his ;] 400 soon -> -1 }T
testing his animations' sounds (Pursuer_AnimSounds): by his kit for the motion at its sound keys - his blow $E00: $15 at the first, none at the second
T{ [: $E00 play-now  0 key-sound  1 key-sound  $200 play-now  0 key-sound ;] his -> $15 -1 -1 }T
testing his knocks at a shut door (Pursuer_KnockAtDoor): from his kit's (Debilitas: $28 or $23); none of her own (kind 35) the Pursuer's $21
: knock? ( n -- flag )  dup $28 = swap $23 = or ;
T{ [: 5 0 do  knock-sound knock?  loop  and and and and ;] his -> -1 }T
T{ s" kind35" actor-named enter  knock-sound  leave-actor -> $21 }T
testing his hanging parts (DebilitasModel_Springs): as drawn, the first from bone 10 and the other three all from its end (together)
: y-of ( b -- ) ( F: -- y )  [: my-model @ ;] his swap bone-pos fdrop fswap fdrop ;
T{ [: 11 y-of 13 y-of f- fabs 0.001e f<  12 y-of 13 y-of f- fabs 0.001e f< and ;] 10 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
test-summary
