\ The obstacles and Fiona pushing them (docs/subsystems/obstacles.md, fiona/push.fs): castle-b1-7's
\ (its script places obstacle 0 at squares $1ED / $245). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/obstacles/test_obstacles.fs
IN: test-obstacles
USING: tester engine game-state actors messages room-names game debug keys obstacles story.words fiona.state fiona.model ;

\ (the camera held looking along -z: W walks her toward -z)
: frames ( n -- )  0 ?do  0e engine:camera cam.yaw sf!  game-tick  loop ;
: hers ( xt -- x )  fiona enter  execute  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  0e engine:camera cam.yaw sf!  game-tick  loop  drop false ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;
: anim ( -- id )  ['] anim@ hers ;
: free? ( -- flag )  [: her-doing @ 0=  her-mode @ 0= and ;] hers ;
: obst-z ( -- n )  0 obstacle-at f>s fdrop fdrop ;
: obst-x ( -- n )  0 obstacle-at fdrop fdrop f>s ;
: her-z ( -- n )  [: her-at 8 + sf@ f>s ;] hers ;
: place ( -- )  0 0 0 $1ED $245 game:obstacles send place-obstacle  2 frames ;
: ring-tri ( -- tri )   \ (the first on its ring)
    nav-tris 0 do  i ring-mask 1 and if  i unloop exit  then  loop  -1 ;

2 frames
0 fiona send costume  1 frames
game:story send story-stop  deliver  progress pr.state 8 0 fill
castle-b1-7 room!  5 frames
place
testing placed: its square, its place, the floor under it blocked, the ring round it marked
T{ 0 $1ED on-square?  0 $245 on-square?  obst-x obst-z -> -1 -1 -32 -6 }T
T{ $1ED nav-flags $20400000 and  ring-tri dup 0< 0=  swap nav-flags $20800000 and -> $20400000 -1 $20800000 }T
testing which ways it can go: along -z here, not along +x (no floor for it)
T{ 0e -1e 0 can-push?  1e 0e 0 can-push?  0e -1e 1 can-push? -> -1 0 0 }T

testing Fiona walking into it from +z: she turns to it and leans on it ($1200), then pushes ($1201)
-32.5e  0 obstacle-at fdrop fswap fdrop  6e 180e tp-facing  5 frames
key: W hold
T{ [: anim $1200 = ;] 60 soon  [: anim $1201 = ;] 60 soon  [: her-push @ ;] hers -> -1 -1 0 }T
testing ... it goes two squares (5) over its move, she after it; again; then it can't - she strains ($1203)
T{ [: obst-z -11 <= ;] 60 soon  [: obst-z -16 <= ;] 120 soon  [: anim $1203 = ;] 120 soon -> -1 -1 -1 }T
T{ obst-z  her-z -7 <= -> -16 -1 }T
testing the stick let go: she lets go ($1202), free
key: W release
T{ [: anim $1202 = ;] 30 soon  [: free? ;] 60 soon -> -1 -1 }T
testing moved: its square the one it went to
T{ 0 $1ED on-square?  0e 1e 0 can-push? -> 0 -1 }T

testing stopped (the story's): it can't be pushed
0 game:obstacles send stop-obstacle  1 frames
T{ 0e 1e 0 can-push? -> 0 }T

testing its place kept: placed there again after leaving the room and coming back
0 game:obstacles send keep-obstacle  1 frames
castle-b1-7 room!  5 frames
T{ 0 $1ED on-square? -> 0 }T
0 0 0 game:obstacles send place-obstacle-kept  2 frames
T{ obst-z  0 $1ED on-square? -> -16 0 }T
testing the room left: gone
castle-b1-6 room!  5 frames
T{ 0e 1e 0 can-push? -> 0 }T

testing the story's words: castle-b1-7's script's placing, its question
castle-b1-7 room!  5 frames
0 0 0 $1ED $245 story.words:obstacle-place  2 frames
T{ 0 $1ED story.words:obstacle-on?  0 $245 story.words:obstacle-on? -> -1 -1 }T

test-summary
