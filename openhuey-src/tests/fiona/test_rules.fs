\ Fiona's rules as the original's (docs/subsystems/fiona.md): the buttons refused while she is
\ busy or panicking, her turn mode back to the camera's, getting up and falls cut in, the push
\ out of others. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_rules.fs
IN: test-fiona-rules
USING: tester engine game-state actors messages room-names game keys items items.names facts flag-names vectors common fiona.state fiona.model fiona.anims fiona.moving fiona.controls fiona.slam fiona.fear fiona.hurt fiona.defend fiona.letgo fiona debug ;

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
: pushed ( -- tri d )   \ (from a point just beside herself: out of her own body)
    pt her-at vec-copy  pt v.x sf@ 0.1e f+ pt v.x sf!
    pt her-tri @ self blocked-floor push-out  pt self body-at vec-dist-xz ;
: two-radii ( F: -- d )  self body-dims fdrop 2e f* 0.01e f+ ;
T{ [: pushed fdup two-radii f- fabs 0.001e f<  swap 0< 0= ;] her@ -> -1 -1 }T

testing a cut (Motion_Play: no blend) against the table's blend (Motion_PlayTable)
T{ [: up-from-back-anim play-cut  settled? ;] her@ -> -1 }T
free!

testing struck by somebody gone: the fall as from behind (Fiona_StateKnockedDown, have 0)
T{ [: 9999 her-hit-who !  1e side f!  blow-side  side f@ f0= ;] her@ -> -1 }T
[: -1 her-hit-who ! ;] her@

testing thrown and after: nav flag 1 blocks her; by a door the door's floor ($8000018)
T{ [: thrown-r her-sub !  thrown-floor 1 and  door-thrown-r her-sub !  thrown-floor  0 her-sub ! ;] her@ -> 1 $8000019 }T

testing down after a hard fall: her shakes start over (Fiona_StateAfterThrow's floored)
T{ [: 0 her-shake-rest !  1e her-shake-a f!  5 her-attack-t !  lie-down  her-shake-rest @  her-shake-a f@ f0=  her-attack-t @  her-mode @ ;] her@ -> -1 -1 0 $A }T
: up? ( n -- flag )  0 ?do  game-tick  mode 0= if  true unloop exit  then  loop  false ;
T{ 600 up? -> -1 }T

testing a shove into a wall (Fiona_StateShove): blended into standing once it has faded in, then she stands
[: shove-doing her-doing !  defend-mode her-mode !  $1A her-sub !  shove-anim -1 play-table  ['] shove-end her-act ! ;] her@
: anim? ( a n -- flag )  0 ?do  game-tick  dup [: anim@ ;] her@ = if  drop true unloop exit  then  loop  drop false ;
T{ stand-anim 60 anim?  60 up? -> -1 -1 }T

test-summary
