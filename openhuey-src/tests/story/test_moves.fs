\ The story, S3: the characters' moves (scripted-move), as the room's scripts give them
\ (docs/subsystems/story.md). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_moves.fs
IN: test-moves
USING: tester engine game-state actors messages keys room-names game debug fiona.state fiona.model hewie.state paths ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: hers ( xt -- x )  game:fiona enter  execute  leave-actor ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: within ( n xt -- flag )   \ (within n frames xt says so)
    swap 0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;
\ a move given: kind, a, b, the point (x y z), the heading in degrees
: give ( kind a b who F: x y z deg -- )
    >r  deg>rad  f>cell >r  f>cell >r  f>cell >r  f>cell  r> r> r> r>  send scripted-move ;
create p 12 allot  create s 12 allot
fvariable tol
: near ( who F: tol -- flag )   \ (on the level, within tol of the spot s)
    tol f!  body-at s vec-dist-xz tol f@ f< ;

free-play  10 frames  game:story send story-stop  deliver
front-garden-2 room!  10 frames  hewie send join-fiona  10 frames
-1 game:fiona send scripted  -1 game:hewie send scripted  deliver

testing Fiona: an animation (7) - playing at once, done at once
T{ 7 $800 -1 game:fiona  0e 0e 0e 0e give  3 frames  [: anim@ her-move-done @ ;] hers -> $800 -1 }T
testing a walk to a spot (5, straight): there, done
game:fiona body-pos p vec!
: spot ( F: -- x y z )  s vec@ ;
: spot! ( -- )  s p vec-copy  s sf@ 12e f+ s sf! ;   \ (12 along x from p)
: spot-tri ( -- tri )  s $29020008 v-tri-in ;
spot!
T{ 5 -1 -1 game:fiona  spot 90e give  2 frames  [: her-move-done @ ;] hers -> 0 }T
T{ 300 [: [: her-move-done @ ;] hers ;] within  game:fiona 0.5e near -> -1 -1 }T
testing a turn to a heading (15): facing it, done
T{ 15 0 -1 game:fiona  0e 0e 0e -90e give  200 [: [: her-move-done @ ;] hers ;] within -> -1 }T
T{ game:fiona body-yaw -90e deg>rad f- angle-wrap fabs 0.01e f< -> -1 }T

testing Hewie (scripted: his actions for the moves): an animation (7 -> 0x3B)
T{ 7 $1C02 -1 game:hewie  0e 0e 0e 0e give  3 frames  [: his-action @ hewie.model:anim@ his-done @ ;] his -> $3B $1C02 -1 }T
testing a turn to a heading (15 -> 0x44): facing it, done
T{ 15 0 -1 game:hewie  0e 0e 0e 45e give  300 [: [: his-done @ ;] his ;] within -> -1 }T
T{ game:hewie body-yaw 45e deg>rad f- angle-wrap fabs 0.02e f< -> -1 }T
testing a walk to a spot (5 -> 0x3F): there, done
\ (from 20 behind her along x, wherever his own wanderings left him)
game:fiona body-pos p vec!  p sf@ 20e f- p sf!  p vec@ xyz>cells 0 game:hewie send place  2 frames
game:hewie body-pos p vec!  spot!
T{ 5 spot-tri -1 game:hewie  spot 0e give  400 [: [: his-done @ ;] his ;] within -> -1 }T
T{ game:hewie 3e near -> -1 }T
testing let go, he's his own dog again
T{ 0 game:hewie send scripted  10 frames  [: his-busy @ ;] his -> 0 }T

testing a curved way (Character_WaypointsCurve): its corners rounded (8 steps a stretch), each step walked straight over the floor, ending at the spot
create way /path allot  create a0 12 allot  create b0 12 allot
: her-tri ( -- tri )  game:fiona body-tri ;
variable best  fvariable best-d
: farther? ( tri -- flag ) ( F: -- )   \ (a way to it with a turn, farther than the best yet)
    dup tri-center b0 vec!
    >r  way her-tri a0 r> b0 0 path-plan  dup 2 < swap 7 > or  if  false exit  then   \ (2..7: the curve fits)
    b0 a0 vec-dist-xz  fdup best-d f@ f> if  best-d f!  true  else  fdrop false  then ;
: far-tri ( -- tri )   \ (the farthest triangle a way of 2..7 turns reaches)
    a0 game:fiona body-at vec-copy  -1 best !  0e best-d f!
    nav-tris 0 do  i dup farther? if  best !  else  drop  then  loop  best @ ;
far-tri constant to-tri
to-tri tri-center b0 vec!  a0 game:fiona body-at vec-copy
way her-tri a0 to-tri b0 0 path-plan constant straight-n
way her-tri a0 to-tri b0 0 path-plan-curved constant curved-n
create from 12 allot  variable at-tri
: walked? ( -- flag )   \ (each step straight from the last over the floor, as one walks it)
    from a0 vec-copy  her-tri at-tri !
    curved-n 0 do
        at-tri @ from way i path-point 0 v-walk  dup 0< if  drop false unloop exit  then  at-tri !
        from way i path-point vec-copy
    loop  true ;
T{ straight-n 2 >=  curved-n straight-n >  to-tri tri-center b0 vec!  way curved-n 1- path-point b0 vec-dist-xz 0.01e f< -> -1 -1 -1 }T
T{ walked? -> -1 }T

test-summary
