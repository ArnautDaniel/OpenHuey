\ The story's conditions about characters and places, and small settings (S5d; src/game/event.c):
\ close to each other, facing, within a distance, on a nav group or flags, before a door; the
\ doors' open bits; Fiona's fear; story flags by variable. Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_conditions.fs
IN: test-conditions
USING: tester engine game-state actors messages room-names game fiona.state hewie.state story.state story.words doors ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
fvariable off-x  fvariable off-z
: put-hewie ( F: dx dz -- )   \ Hewie that far from Fiona, in her room
    off-z f!  off-x f!
    fiona body-pos  off-z f@ f+  frot off-x f@ f+  frot frot     ( F: x y z )
    hewie enter  room-id  fiona body-tri  body-place  leave-actor ;
60 frames
front-garden-2 -1 rooms send go-to-room  5 frames
hewie send join-fiona  deliver  10 frames   \ (Hewie in the game, by her)

testing close: Hewie beside her touches her; 40 away he doesn't
3e 0e put-hewie  2 frames
T{ [: fiona hewie 0 0 touching? ;] in-room -> -1 }T
40e 0e put-hewie  2 frames
T{ [: fiona hewie 0 0 touching? ;] in-room -> 0 }T
testing within a distance: 50 yes, 30 no (40 apart)
T{ [: 0 1 50 chars-within?  0 1 30 chars-within? ;] in-room -> -1 0 }T

testing standing on her triangle's own flags
: her-flags ( -- flags )  fiona body-tri nav-flags ;
T{ [: 0 her-flags char-on-nav-flags? ;] in-room  her-flags 0<> = -> -1 }T
testing a door's open bit (the doors'): set, then cleared
: door0 ( -- d )  room-id 0 room-exit-door ;
: open-bit? ( -- flag )  door0 4 * progress pr.doors + l@ opened-bit and 0<> ;
[: door0 door-open-set ;] in-room  deliver
T{ open-bit? -> -1 }T
[: door0 door-open-clear ;] in-room  deliver
T{ open-bit? -> 0 }T
testing her fear set, then reset with her recovery's count
[: 60 fiona-fear ;] in-room  deliver
T{ fiona enter her-fear f@ f>s leave-actor -> 60 }T
[: fiona-calm-reset ;] in-room  deliver
T{ fiona enter her-fear f@ f>s her-recovery @ leave-actor -> 0 0 }T
testing a story flag by script variable; the room's frames
T{ [: $200 3 cells vars + !  3 story-flag-set-var  3 story-flag-var? ;] in-room  $200 story-flag? -> -1 -1 }T
T{ [: 500 room-frames-set  400 room-frames-reached?  600 room-frames-reached? ;] in-room -> -1 0 }T
testing her shadows off (by her script id), and back
: shadow-off? ( -- n )  fiona enter her-model @ leave-actor actor act.shadow-off l@ ;
[: 0 -1 char-no-shadow ;] in-room
T{ shadow-off? -> 1 }T
[: 0 0 char-no-shadow ;] in-room
T{ shadow-off? -> 0 }T
testing the camera's setup now
T{ [: cam-set@ camera-mode?  cam-set@ 1+ camera-mode? ;] in-room -> -1 0 }T
testing the story's shot: 20 behind her, level, at her middle; held while she moves
: her-x ( F: -- x )  fiona body-pos fdrop fdrop ;
: her-y ( F: -- y )  fiona body-pos fdrop fswap fdrop ;
: her-z ( F: -- z )  fiona body-pos fswap fdrop fswap fdrop ;
: cam-off ( -- along up )   \ how far the camera is from her along the floor, and above her, rounded
    engine:camera cam.x sf@ her-x f- fsq  engine:camera cam.z sf@ her-z f- fsq  f+ fsqrt fround f>s
    engine:camera cam.y sf@ her-y f- fround f>s ;
[: 1 20e 0e 0e 0e event-camera ;] in-room  deliver  2 frames
T{ cam-off -> 20 10 }T
: behind? ( -- flag )   \ the camera on the far side from where she faces
    engine:camera cam.x sf@ her-x f-  fiona body-yaw fsin f*
    engine:camera cam.z sf@ her-z f-  fiona body-yaw fcos f*  f+  f0< ;
T{ behind? -> -1 }T
her-x f>cell  her-y f>cell  her-z 10e f+ f>cell  fiona body-yaw f>cell  fiona send place  deliver  2 frames
T{ cam-off drop 20 = -> 0 }T
testing the shot given back
[: 0 0e 0e 0e 0e event-camera ;] in-room  deliver  2 frames
T{ cam-off drop 20 = -> 0 }T
testing the shake: a script's lasts the frame it is set in - the panic stills the camera first each frame (Panic_Stage)
: cam-still? ( -- flag )  engine:camera cam.x sf@  1 frames  engine:camera cam.x sf@  f= ;
[: 2e camera-shake ;] in-room  deliver
T{ cam-still? -> -1 }T
testing her panic past its first stretch shakes it by 0.2, a frame at a time
: jolt ( flag -- )  game:panic enter  panic:jolting !  leave-actor ;
-1 jolt  T{ cam-still? -> 0 }T
1 frames  T{ cam-still? -> -1 }T   \ (its frame over: still again)
testing the avoid prompt: its art in, a row shown for 30 frames, then gone
: prompt ( -- row )  game:avoid enter  avoid:row @  leave-actor ;
T{ game:avoid enter  avoid:loaded @ 0<>  leave-actor -> -1 }T
[: 2 avoid-prompt ;] in-room  deliver
T{ prompt -> 2 }T
29 frames  T{ prompt -> 2 }T
2 frames   T{ prompt -> -1 }T
testing a room's motions for her: loaded, not hers till used; then she plays them
: her-model ( -- n )  fiona enter fiona.state:her-model @ leave-actor ;
T{ s" O_FIN_M\FIN_101.MTN" her-model motions-load -> -1 }T
T{ her-model $8004 has-motion? -> 0 }T
[: 0 char-file-use ;] in-room
T{ her-model $8004 has-motion?  her-model $200 has-motion? -> -1 -1 }T
T{ her-model $8004 0 0 motion-play  her-model motion@ -> $8004 }T
1 frames   \ (and posed from it)
T{ s" O_FIN_M\NOPE.MTN" her-model motions-load -> 0 }T
testing every door into her room locked against Hewie, and not; one door against the stalker
: kept-out ( -- bits )   \ the kinds the room's first door keeps out
    8 0 do  room-id i room-exit-door dup 0< 0= if  doors:against unloop exit  then  drop  loop  -1 ;
: all-against? ( bit -- flag )   \ every door into this room keeps that kind out
    true  8 0 do  room-id i room-exit-door dup 0< 0= if  doors:against 2 pick and 0<> and  else  drop  then  loop  nip ;
[: 1 room-id 1 room-doors-state ;] in-room  deliver
T{ 2 all-against? -> -1 }T
[: 1 room-id 0 room-doors-state ;] in-room  deliver
T{ kept-out -> 0 }T
[: $FE  room-id 0 room-exit-door  1 door-lock-for ;] in-room  deliver
T{ kept-out -> 4 }T
[: $FE  room-id 0 room-exit-door  0 door-lock-for ;] in-room  deliver
testing Hewie's coat: kind 1's model, still shown and moving; back to his own
: his-model ( -- n )  hewie enter hewie.state:his-model @ leave-actor ;
[: 1 hewie-model ;] in-room  deliver  5 frames
T{ his-model model-file s" O_HEW/HEW_001" compare -> 0 }T
T{ his-model actor act.visible l@  his-model motion@ 0< 0= -> 1 -1 }T
[: 0 hewie-model ;] in-room  deliver  5 frames
T{ his-model model-file s" O_HEW/HEW_000" compare -> 0 }T
testing the doors set up again
T{ [: doors-room-in ;] in-room  deliver -> }T
testing a splash and a burst of dust by her: there, then gone by themselves
: there? ( addr len -- flag )  actor-named 0< 0= ;
[: 0 6 4 $80 $80 $80 $40  her-x her-y 2e f+ her-z splash ;] in-room  deliver
[: 0 0 0 0 0  her-x her-y her-z dust ;] in-room  deliver  1 frames
T{ s" splash" there?  s" dust" there? -> -1 -1 }T
400 frames
T{ s" splash" there?  s" dust" there? -> 0 0 }T
testing the story's Fiona words: thrown down, her look held on Hewie, not unseen
0 fiona send costume  deliver  2 frames
: hers ( xt -- x )  fiona enter  execute  leave-actor ;
[: 1 0 fiona-thrown ;] in-room  deliver  1 frames
T{ [: fiona.state:her-mode @ fiona.state:her-sub @ ;] hers -> 4 $A }T
: risen? ( n -- flag )  0 ?do  game-tick  [: fiona.state:her-mode @ 0= ;] hers if  true unloop exit  then  loop  false ;
T{ 400 risen? -> -1 }T
[: 1 0 fiona-target ;] in-room  deliver  1 frames
T{ [: her-look-who @ ;] hers -> hewie }T
T{ [: fiona-unseen? ;] in-room -> 0 }T
T{ [: fiona-hewie-react ;] in-room  deliver -> }T
testing the story's Hewie animations: hidden away's, played, his root-motion move, held
: his ( xt -- x )  hewie enter  execute  leave-actor ;
[: 300 $D00 hewie-anim ;] in-room  deliver
T{ [: his-yelp @ his-yelp-anim @ ;] his -> 300 $D00 }T
[: 4 hewie-anim-set ;] in-room  deliver  1 frames
T{ his-model motion@ -> 4 }T
[: $201 0 hewie-anim-root ;] in-room  deliver  2 frames
T{ [: his-action @ ;] his -> $47 }T
[: 1 hewie-pause ;] in-room  deliver
T{ [: his-stay @ ;] his -> -1 }T
[: 0 hewie-pause ;] in-room  deliver
testing through an exit's door: she sets off to its far side (move 5); a knock heard
0 2 exit-spot drop debug:tp  5 frames   \ (on the floor 12 inside it: the tests above left her off it)
[: ctx-who @ >r  0 ctx-who !  0 self-through-exit  r> ctx-who ! ;] in-room  deliver  2 frames   \ (as her script)
T{ [: fiona.state:her-doing @ ;] hers -> $12 }T
T{ [: 0 1 self-door-knock ;] in-room -> }T
testing the maze dealt: its count up, each of its rooms' doors open for the rows naming it, the rest closed off
: closed? ( door -- flag )  dup 32 / 4 * progress pr.closed-off + l@  swap 32 mod rshift 1 and 0<> ;
: open-count ( -- n )  0  72 0 do  $95 i + closed? 0= if  1+  then  loop ;
[: deal-things ;] in-room  deliver
T{ $1F progress pr.vars + c@  open-count -> 1 12 }T
testing a door copying another's lock; the camera's clips; the noise setting; the frame blend
$95 door-lock   deliver
[: $96 $95 door-copy ;] in-room  deliver
T{ $96 doors:door-locked? -> -1 }T
$95 doors-id send unlock  $96 doors-id send unlock  deliver
[: 1 0.9e camera-value  0 20000e camera-value ;] in-room
T{ engine:camera cam.near sf@ 0.9e f-  fabs 0.001e f<  engine:camera cam.far sf@ 20000e f= -> -1 -1 }T
[: 2 noise-level  $2D sprites-additive ;] in-room  deliver  1 frames
T{ s" acoustics" actor-named enter  acoustics:setting @  leave-actor -> 2 }T
testing her drawn as a silhouette, and back
: tint ( -- rgba )  fiona enter fiona.state:her-model @ leave-actor actor act.tint l@ ;
[: 0 $C8000000 1 char-tint ;] in-room
T{ tint -> $000000C8 }T
[: 0 0 0 char-tint ;] in-room
T{ tint -> $80808080 }T
test-summary
