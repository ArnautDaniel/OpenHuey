\ The story's moves for a stalker (stalker/scripted.fs; Pursuer_EventState's 5, Pursuer_Think,
\ Pursuer_EventCommand): taken by a script in the room being played (not elsewhere), his own
\ behaviour stopped; standing, animations, turning, walking to a spot and to a triangle, looking,
\ a door used both ways; blows not taken; let go, on as before; posed by the scripts' 0x9D till
\ started afresh; a stalker loaded as a scene's figure placed, posed and shown. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_scripted.fs
IN: test-stalker-scripted
USING: stalker-names facts story.words tester engine game-state actors messages common room-names game debug paths doors vectors flag-names stalker stalker.state stalker.anims stalker.search stalker.moving ;

: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: deb ( -- id )  game:debilitas ;
: move ( kind a b F: x y z yaw -- )   \ (floats f>cell, as the story sends them)
    f>cell >r f>cell >r f>cell >r f>cell r> r> r>  deb send scripted-move  deliver ;
: done? ( -- flag )  [: sm-done @ ;] his ;
: held? ( -- flag )  [: script-held @ ;] his ;
: anim? ( -- a )  [: anim @ ;] his ;
: facing? ( F: yaw -- flag )  deb body-yaw f- angle-wrap fabs 0.02e f< ;

free-play  front-garden-3 room!  10 run-frames  front-garden-2 room!  10 run-frames
stalkers-blind state-flag-set
68e 124e -174e 180e tp-facing  3 run-frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames

testing not in the room being played, a script's action for him is dropped
front-garden-3 room!  5 run-frames  -1 deb send scripted  deliver  2 run-frames
T{ held? -> 0 }T
0 deb send scripted  deliver  front-garden-2 room!  10 run-frames
68e 124e -174e 180e tp-facing  3 run-frames
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames

testing in her room taken at his next frame: held, standing, done once his stance has faded in
-1 deb send scripted  deliver
T{ held? -> 0 }T
1 run-frames
T{ held? -> -1 }T
T{ ' done? 60 soon  anim? 4 u< -> -1 -1 }T
testing ... his own behaviour stopped (his mode's clock stands); blows not taken
[: 30 mode-t !  70 hp ! ;] his  40 run-frames
T{ [: mode-t @ ;] his -> 30 }T
0 25 0 deb send blow  deliver  3 run-frames
T{ [: hp @ reeling @ ;] his -> 70 0 }T

testing an animation (7): done at once; blended over b frames (8); his stance over b (16)
anim-move look-about-anim -1  no-place move
T{ done?  anim? -> -1 look-about-anim }T
blend-move look-about-2-anim 10  no-place move
T{ done?  anim? -> -1 look-about-2-anim }T
idle-anim-move 0 10  no-place move
T{ done?  anim? -> -1 0 }T
20 run-frames

testing a turn to a heading (15): in his walk, turned at his rate, then standing - done once that has faded in
turn-to-heading-move 0 -1  0e 0e 0e 90e deg>rad move
T{ done?  anim? -> 0 walk-anim }T
T{ ' done? 120 soon  90e deg>rad facing?  anim? 4 u< -> -1 -1 -1 }T

\ a triangle 20..40 away he has a way to
variable far-tri  create far-at /vec allot
: far ( -- )
    nav-tris 0 do
        i tri-center far-at vec!  far-at deb body-at vec-dist-xz fdup 20e f>  40e f< and if
            i far-tri !  [: far-tri @ far-at plan 0> ;] his  i nav-flags [: self body-mask ;] his and 0= and if  unloop exit  then
        then
    loop  -1 far-tri ! ;
far
T{ far-tri @ 0< -> 0 }T
testing walking to the spot (5): in his walk on the way, then on its triangle turned to the move's heading, done once standing
walk-move far-tri @ -1  far-at vec@ 45e deg>rad move
T{ done? -> 0 }T
10 run-frames
T{ anim? $FF00 and dup walk-anim = swap turn-anims = or  done? -> -1 0 }T
T{ ' done? 600 soon  deb body-tri far-tri @ =  45e deg>rad facing? -> -1 -1 -1 }T
T{ deb body-at far-at vec-dist-xz 0.5e f< -> -1 }T
testing a look at a point (13), standing (his action 0: no look of his own): his head turned toward it - past his own looks' one step
create pt-at /vec allot
: aside ( -- )   \ (30 to his side, at his eye)
    deb body-at vec@ deb body-yaw 90e deg>rad f+ {: F: x y z a :}
    x a fsin 30e f* f+  y 16e f+  z a fcos 30e f* f+  pt-at vec! ;
aside
look-point-move 0 -1  pt-at vec@ 0e move  20 run-frames
T{ done?  [: head-yaw f@ fabs 0.5e f> ;] his -> -1 -1 }T
testing ... and off (12 at nobody): eased back
look-move -1 $FF  no-place move  20 run-frames
T{ [: head-yaw f@ fabs 0.01e f< ;] his -> -1 }T

testing walking back to a triangle slowly (11): his slow walk; done as soon as he is on it
s" fiona" actor-named body-tri constant her-tri
run-path-move her-tri -1  no-place move
10 run-frames
T{ anim? $FF00 and dup walk-anim = swap turn-anims = or  done? -> -1 0 }T
T{ ' done? 600 soon  deb body-tri her-tri = -> -1 -1 }T

testing a door used (3): the door at exit 0 opened from his side - lined up, his animation, let go open; then standing, done
: door0 ( -- door )  room-id 0 room-exit-door ;
T{ door0 door-open? -> 0 }T
open-door-move 0 -1  no-place move
T{ ' done? 900 soon  door0 door-open?  door0 door-held? -> -1 -1 0 }T
testing ... and again (4): open now, shut by it - his animation one more
shut-door-move 0 -1  no-place move
10 run-frames
T{ [: door-use @ 1 and ;] his -> 1 }T
T{ ' done? 900 soon  door0 door-open?  door0 door-held? -> -1 0 0 }T

testing let go: held no more, on as before (sensing her: after her; else searching the room again)
0 deb send scripted  deliver  5 run-frames
T{ held?  [: sees-fiona @ if  my-mode @ after-her =  else  my-mode @ searching =  search-phase @ 0<> and  then ;] his -> 0 -1 }T

testing posed (the scripts' 0x9D): held in its animation, shown; a script letting go doesn't free him, starting afresh does
look-about-anim 1 0 deb send hold-anim  deliver  3 run-frames
T{ held?  [: posed @  anim @  my-model @ actor act.visible l@ ;] his -> -1 -1 look-about-anim 1 }T
0 deb send scripted  deliver  3 run-frames
T{ held? -> -1 }T
deb send activate  deliver  3 run-frames
T{ held?  [: posed @ ;] his -> 0 0 }T
debug:debilitas-out  5 run-frames

testing a stalker as a scene's figure (Debilitas into slot 3): placed by the story, posed, shown there; started afresh, posed no more
2 3 $FF char-load  3 run-frames
T{ 2 char-slot  [: in-game @ my-slot @ ;] his -> 3 0 3 }T
2 her-tri 90 char-to-tri-facing  3 run-frames
2 look-about-anim 1 0 char-anim-hold  3 run-frames
T{ deb body-tri her-tri =  [: posed @  anim @ ;] his  [: my-model @ actor act.visible l@ ;] his -> -1 -1 look-about-anim 1 }T
2 char-activate  3 run-frames
T{ [: posed @ ;] his -> 0 }T
3 0 char-remove  3 run-frames
T{ [: my-model @ actor act.visible l@ ;] his -> 0 }T
test-summary
