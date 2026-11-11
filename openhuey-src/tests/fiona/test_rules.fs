\ Fiona's rules as the original's (docs/subsystems/fiona.md): the buttons refused while she is
\ busy or panicking, her turn mode back to the camera's, getting up and falls cut in, the push
\ out of others. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_rules.fs
IN: test-fiona-rules
USING: floor-flags vectors tester engine game-state actors messages room-names game keys items items.names facts flag-names vectors common fiona.state fiona.model fiona.anims fiona.moving fiona.controls fiona.slam fiona.fear fiona.hurt fiona.defend fiona.letgo fiona debug ;

: her@ ( xt -- x )  fiona enter  execute  leave-actor ;
: mode ( -- n )  [: her-mode @ ;] her@ ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;
: free! ( -- )  [: idle ;] her@  10 run-frames ;

2 run-frames
0 fiona send costume  1 run-frames
game:story send story-stop  deliver  progress pr.state 8 0 fill
30 run-frames

testing Fiona_ToIdle: the stick by the camera again (+0x1AD588 0); arriving afresh keeps a door's held stick
T{ [: steered her-turn-mode !  idle  her-turn-mode @ ;] her@ -> 0 }T
T{ [: door-steered her-turn-mode !  fresh  her-turn-mode @ ;] her@ -> 3 }T
free!

testing the square: not while out of breath (Fiona_IsIdle), taken free (Progress_PlayerButtons)
[: breathless-doing her-doing ! ;] her@
key: Z hold  1 run-frames
T{ mode -> 0 }T
T{ [: free-doing her-doing !  try-defend  her-mode @ ;] her@ -> 8 }T
key: Z release  free!

testing the triangle: no throw while panicking (the threat at 4), else thrown
1 run-frames
key: V hold  1 run-frames   \ (pressed, nothing in her hand yet)
ball throw-slot 4 * progress pr.equipped + l!
T{ [: panicking her-panic-stage !  try-let-go  her-mode @ ;] her@ -> 0 }T
T{ [: 0 her-panic-stage !  try-let-go  her-mode @  her-sub @ ;] her@ -> $E $31 }T
key: V release  -1 throw-slot 4 * progress pr.equipped + l!  free!

testing nobody near: not touching the stalker (no door refused), touching nobody
T{ [: stalker-touching?  touching ;] her@ -> 0 0 }T

testing push-out (Actor_PushOut): out of a body to their radii + 0.01 apart, on her floor
create pt 12 allot
: pushed ( -- tri ) ( F: -- d )   \ (from a point just beside herself: out of her own body)
    pt her-at vec-copy  pt v.x sf@ 0.1e f+ pt v.x sf!
    pt her-tri @ self blocked-floor push-out  pt self body-at vec-dist-xz ;
: two-radii ( F: -- d )  self body-dims fdrop 2e f* 0.01e f+ ;
T{ [: pushed fdup two-radii f- fabs 0.001e f<  swap 0< 0= ;] her@ -> -1 -1 }T

testing a cut (Motion_Play: no blend) against the table's blend (Motion_PlayTable)
T{ [: up-from-back-anim play-cut  settled? ;] her@ -> -1 }T
free!

testing after a cut, the stick held: she faces the way the old camera steers, as she would walk (Fiona_MoveInput: atan2 x z)
T{ [: own-mode her-mode !  1e way-x f!  0.6e way-z f!  after-cut  her-heading f@  cam-yaw dir-by stick-heading  f- angle-wrap fabs 0.001e f<
      0e way-x f!  0e way-z f!  camera-relative her-turn-mode ! ;] her@ -> -1 }T
testing Fiona_ToIdle: flag $2B cleared; under a script her idle step (its end the move's done), free her idle move
T{ [: fiona-occupied state-flag-set  idle  fiona-occupied state-flag? ;] her@ -> 0 }T
T{ [: -1 her-scripted !  idle  her-act @ ['] fiona.moves:idle-step =  0 her-scripted !  idle  her-act @ ['] idle-move = ;] her@ -> -1 -1 }T
testing seized (Fiona_JointAction kind 1): only moving freely - not mid kick
T{ [: defend-mode her-mode !  0 6 fiona.seized:seized  own-mode her-mode ! ;] her@ -> 0 }T
testing the panic calming (stage 5): no full panic again (Fiona_Panic: at 4 only)
T{ [: her-panic-stage @  calming-down her-panic-stage !  1 her-fear-bits !  feel-the-panic  her-fear-bits @ 2 and  0 her-fear-bits !  swap her-panic-stage ! ;] her@ -> 0 }T
testing her reach for the creatures: her +0x2D 4, a script's hold 8
T{ [: -1 her-out-of-reach !  reach 4 and 0<>  0 her-out-of-reach !  -1 her-scripted !  reach 8 and 0<>  0 her-scripted !  reach 12 and ;] her@ -> -1 -1 0 }T
testing out of reach (+0x2D) while falling, not once she's down
T{ [: -1 her-out-of-reach !  fiona.fear:lie-down  her-out-of-reach @  reach 1 and ;] her@ -> 0 0 }T
free!

testing struck by somebody gone: the fall as from behind (Fiona_StateKnockedDown, have 0)
T{ [: 9999 her-hit-who !  1e side f!  blow-side  side f@ f0= ;] her@ -> -1 }T
[: -1 her-hit-who ! ;] her@

testing thrown and after: nav flag 1 blocks her; by a door the door's floor ($8000018)
T{ [: thrown-r her-sub !  thrown-floor 1 and  door-thrown-r her-sub !  thrown-floor  0 her-sub ! ;] her@ -> 1 $8000019 }T

testing down after a hard fall: her shakes start over (Fiona_StateAfterThrow's floored)
T{ [: 0 her-shake-rest !  1e her-shake-a f!  5 her-attack-t !  lie-down  her-shake-rest @  her-shake-a f@ f0=  her-attack-t @  her-mode @ ;] her@ -> -1 -1 0 $A }T
testing R1 and the square (Progress_PlayerButtons, Fiona_IsIdle, Fiona_StateBlock): not out of breath, in a panic attack, at a push or at 100; free again in her step back past its mark
T{ [: own-mode her-mode !  0 her-sub !  free-doing her-doing !  0e her-fear f!  free-to-act? ;] her@ -> -1 }T
T{ [: backstep-doing her-doing !  free-to-act? ;] her@ -> -1 }T
T{ [: breathless-doing her-doing !  free-to-act? ;] her@ -> 0 }T
T{ [: attack-doing her-doing !  free-to-act? ;] her@ -> 0 }T
T{ [: free-doing her-doing !  pushing-sub her-sub !  free-to-act? ;] her@ -> 0 }T
T{ [: 0 her-sub !  100e her-fear f!  free-to-act? ;] her@ -> 0 }T
[: 0e her-fear f! ;] her@
testing stepping back: out of the stalkers' reach till its mark (+0x2D), then not
T{ [: -1 her-out-of-reach !  react-mode her-mode !  plight ;] her@ -> unreachable-plight }T
T{ [: 0 her-out-of-reach !  plight ;] her@ -> fleeing-plight }T
T{ [: own-mode her-mode !  plight ;] her@ -> free-plight }T
free!
testing the shove starts by Motion_PlayOwnBlend: at its first frame, no root step, while it fades in; then its own
[: shove-doing her-doing !  defend-mode her-mode !  $1B her-sub !  ['] shove-start her-act ! ;] her@  25 run-frames
T{ [: anim@ shove-anim =  her act.fade sf@ f0> or ;] her@ -> -1 }T
free!  [: shove-doing her-doing !  defend-mode her-mode !  $1B her-sub !  ['] shove-start her-act ! ;] her@
: shove-on? ( -- flag )  [: anim@ shove-anim = ;] her@ ;
: till-shove ( -- )  60 0 do  shove-on? if  leave  then  game-tick  loop ;
till-shove
T{ [: her act.frame sf@ f0= ;] her@  3 run-frames  [: her act.frame sf@ f0= ;] her@ -> -1 -1 }T
free!
testing the kick: her feet where they are - no step of hers, all through it
fvariable kx  fvariable kz
: spot! ( -- )  s" fiona" actor-named body-pos kz f! fdrop kx f! ;
: stayed? ( -- flag )  s" fiona" actor-named body-pos kz f@ f- fabs 0.001e f<  fdrop kx f@ f- fabs 0.001e f< and ;
spot!  square debug:press  45 run-frames
T{ stayed? -> -1 }T
free!

testing costumes 6 and 7: the weapon hidden until the swing starts (part 25, part 35: Costume6Model / Costume7Model +0x2C, +0x30)
: part-on? ( part -- flag )  [: her-model @ ;] her@ actor act.parts-shown swap 1+ dup 5 rshift 4 * rot + l@ swap 31 and rshift 1 and 0<> ;
: costume! ( n -- )  fiona send costume  2 run-frames  free! ;
: swung ( -- )  [: ['] kicking her-act ! ;] her@  1 run-frames ;
7 costume!
T{ 35 part-on? -> 0 }T
swung
T{ 35 part-on? -> -1 }T
[: kick-anim -1 play-table  ['] kick-end her-act ! ;] her@  1 run-frames
T{ 35 part-on? -> -1 }T
free!
T{ 35 part-on? -> 0 }T
6 costume!
T{ 25 part-on? -> 0 }T
swung
T{ 25 part-on? -> -1 }T
free!
T{ 25 part-on? -> 0 }T
testing ... costume 7's revolver fires once a swing, not costume 6's weapon
7 costume!
T{ [: 0 fired !  fire  fired @ 0<>  ;] her@ -> -1 }T
6 costume!
T{ [: 0 fired !  fire  fired @ ;] her@ -> 0 }T
0 costume!  free!

testing her step at a wall (Actor_Move: NavMesh_Walk): along it, not through it - pulled back over the edge, not by x or z alone
front-garden-3 room!  3 run-frames
create pv /vec allot
: from-wall ( F: dx dz -- tri )   \ (by a wall in the garden: blocked floor to the south of z -233)
    209.445e 0e -232.937e pv vec!  pv 76 fiona-floor v-slide ;
: across ( F: -- d )  pv v.x sf@ 209.445e f- fabs ;
T{ -0.354e -0.354e from-wall  across 0.3e f>  pv v.z sf@ -233e f> -> 76 -1 -1 }T
testing ... away from it, the whole step
T{ 0e 0.5e from-wall drop  pv v.z sf@ -232.437e f- fabs 0.001e f< -> -1 }T
testing ... a triangle she has left behind found afresh: she still moves
T{ 0e 0.5e  209.445e 0e -232.937e pv vec!  pv 0 fiona-floor v-slide 0< 0=  pv v.z sf@ -232.5e f> and -> -1 }T
free!

testing placed on the triangle she is told: at an exit its own, by the story the one it names (Character_Place)
T{ [: 0 fiona:arrive-at  her-tri @ ;] her@ -> 0 0 exit-spot fdrop fdrop fdrop }T
T{ 5 fiona send on-triangle  deliver  [: her-tri @  her-was-tri @ ;] her@ -> 5 5 }T
testing her move through anything (+0x2B) keeps her height, plus the step's rise; x and z by the walk (Actor_MoveAny)
: her-y ( -- n )  [: her-at vec@ fdrop fswap fdrop f>s ;] her@ ;
free!  [: her-at vec@ {: F: x y z :}  x 40e z her-at vec! ;] her@
T{ [: 0e 0.1e 5e fiona.moving:move-any ;] her@  her-y -> 45 }T
[: 0 fiona:arrive-at ;] her@  free!

full-suite   \ (on: waits, scenes, walks - the full run's)
: up? ( n -- flag )  0 ?do  game-tick  mode 0= if  true unloop exit  then  loop  false ;
T{ 600 up? -> -1 }T

testing a kick into a wall (Fiona_StateShove): blended into standing once it has faded in, then she stands
[: kick-doing her-doing !  defend-mode her-mode !  $1A her-sub !  kick-anim -1 play-table  ['] kick-end her-act ! ;] her@
: anim? ( a n -- flag )  0 ?do  game-tick  dup [: anim@ ;] her@ = if  drop true unloop exit  then  loop  drop false ;
T{ stand-anim 60 anim?  60 up? -> -1 -1 }T

test-summary
