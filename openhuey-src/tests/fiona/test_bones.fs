\ A mod's hold on a model's bones (docs/modding.md "A model's bones"; data/model.h ModelSet):
\ found by name, their rest read, their own turns set over the motions' - and freed again. On
\ Fiona's model (the game's bones; model-check reads one with a bone a mod added). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/fiona/test_bones.fs
IN: test-bones
USING: tester engine actors room-names game debug ;

: model ( -- m )  0 cast-model ;   \ (hers, as a mod finds it)
: leaf-parent ( -- bone )   \ (the bone above her last: one with a child to move)
    model  model bone-count 1- bone-parent ;
: arm ( F: -- dx dy dz )   \ the last bone from the one above it, in the room
    model model bone-count 1- bone-pos  model leaf-parent bone-pos {: F: cx cy cz px py pz :}
    cx px f-  cy py f-  cz pz f- ;
create by-motion 3 floats allot   create at-rest 3 floats allot   \ (arms kept)
: keep ( addr -- )  >r arm r@ 2 floats + f!  r@ float+ f!  r> f! ;
: turned ( addr -- ) ( F: -- cos )   \ the arm now against one kept: its turn's cosine
    {: a :}  arm {: F: x y z :}  a f@ {: F: kx :}  a float+ f@ {: F: ky :}  a 2 floats + f@ {: F: kz :}
    x kx f*  y ky f* f+  z kz f* f+
    x x f* y y f* f+ z z f* f+ fsqrt  kx kx f* ky ky f* f+ kz kz f* f+ fsqrt f*  f/ ;
fvariable rx  fvariable ry  fvariable rz
: rest-set ( -- )  model leaf-parent rx f@ ry f@ rz f@ bone-set ;
: quarter ( axis -- )   \ (its rest turned a quarter about axis 0 / 1)
    {: k :}  model leaf-parent  rx f@ ry f@ rz f@  k 0= if  frot 1.5708e f+ frot frot  then
    k 1 = if  fswap 1.5708e f+ fswap  then  bone-set ;

free-play  castle-1f-1 room!  20 run-frames  0 game:fiona send costume  20 run-frames
testing finding them: her model as a mod has it; by the game's names ("bone <i>"), none of another; the first a root
T{ model  game:fiona enter fiona.state:her-model @ leave-actor = -> -1 }T
T{ model bone-count 0>  model s" bone 3" bone-named  model s" no such bone" bone-named  model 0 bone-parent
   -> -1 3 -1 -1 }T
T{ model model bone-count bone-parent  model -1 bone-parent -> -1 -1 }T   \ (no such bone)
model leaf-parent bone-rest rz f! ry f! rx f!
full-suite   \ (on: waits, scenes, walks - the full run's)
by-motion keep
testing a bone's own turn set over the motion's: at rest, then turned a quarter about x, about y
rest-set  2 run-frames  at-rest keep
0 quarter  2 run-frames  at-rest turned  1 quarter  2 run-frames  at-rest turned  fmin
T{ 0.75e f< -> -1 }T   \ (its child swung round with it)
testing set again: the same bone's turn replaced, not added
rest-set  2 run-frames
T{ at-rest turned 0.99e f> -> -1 }T
testing freed: the motion's again; a bone that isn't there left alone
0 quarter  model leaf-parent bone-free  2 run-frames
T{ by-motion turned 0.99e f> -> -1 }T
model 999 1e 1e 1e bone-set  model -1 1e 1e 1e bone-set  2 run-frames
T{ by-motion turned 0.99e f> -> -1 }T
testing every bone freed at once
0 quarter  model bones-free  2 run-frames
T{ by-motion turned 0.99e f> -> -1 }T

test-summary
