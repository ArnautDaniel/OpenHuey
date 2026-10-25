\ Hewie: the rest of his actions (hewie/more.fs) - the story's sit-then-on (0x80) and keeping
\ behind her (0x72), going limp when struck down (0x74), a scene's pose (0x79), back onto his
\ floor (0x86), stepping away from his target (0x82). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/hewie/test_more.fs
IN: test-hewie-more
USING: tester engine game-state actors messages common room-names game hewie.state hewie.body hewie.model hewie.states hewie.more fiona.state debug ;

: his@ ( xt -- x )  hewie enter  execute  leave-actor ;
: action ( -- n )  [: his-action @ ;] his@ ;
: anim ( -- id )  [: anim@ ;] his@ ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: order ( act arg -- )  hewie send do-action  deliver ;
1 seed !

5 run-frames  hewie send join-fiona  30 run-frames
testing 0x80: he sits ($1C03) a while, then on to the action given ($29)
T{ $80 $29 order  [: anim $1C03 = ;] 120 soon -> -1 }T
T{ [: action $29 = ;] 200 soon -> -1 }T
testing 0x74: limp ($2213), then down (0x52)
T{ $74 0 order  1 run-frames  anim  [: action $52 = ;] 200 soon -> $2213 -1 }T
0 0 order  30 run-frames
testing 0x79: the scene's pose ($1301) played out, left alone meanwhile
T{ $79 0 order  1 run-frames  anim  [: his-2d @ ;] his@ -> $1301 1 }T
T{ [: [: his-2d @ ;] his@ 0= ;] 300 soon -> -1 }T
testing 0x72: behind her, 10 of the place behind her, facing away
fiona body-yaw  [: his-heading f! ;] his@
T{ $72 0 order  [: [: his-at behind-spot drop behind-at vec-dist 10e f< ;] his@ ;] 400 soon -> -1 }T
testing 0x82: no target near - himself again
hewie enter -1 his-target ! leave-actor
T{ $82 0 order  2 run-frames  action $82 = -> 0 }T

test-summary
