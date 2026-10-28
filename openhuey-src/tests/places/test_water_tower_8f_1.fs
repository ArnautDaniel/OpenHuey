\ water-tower-8f-1's place: coming in - the models whole, the light on the water; the walls' remark;
\ the diorama's remark; a model worn through by her blow - its debris, the wall's other part; the
\ figure used at the diorama - the aiming cursor (moved by the arrows, kept on the screen); the
\ Bronze Figurine won't go; the Wooden Miniature in its place brings Lorenzo; the locked door; Hewie
\ hides in the hole she signs at, and comes out at her "come back"; the final scenes (the doors, the
\ files); the drop's remarks in turn; the hole's remark; the Azoth scene coming up; the replays.
\ Headless:
\   build/openhuey --test tests/places/test_water_tower_8f_1.fs
IN: test-water-tower-8f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.water-tower-8f-1 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  water-tower-8f-1 0 rooms send go-to-room  60 run-frames ;
fvariable ax  fvariable az  fvariable tx  fvariable tz
: aim ( area F: x z -- )   \ (her put at the area's middle, facing the point)
    tz f! tx f!  area-middle drop  az f! fdrop ax f!
    tx f@ ax f@ f-  tz f@ az f@ f-  fatan2 rad>deg f>s  ax f@ 620e az f@ put ;   \ (at the top: its floor at 620)
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] her ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
c7-second-event story-flag-clear  fg-3-flag3 story-flag-clear  wt8f-1-flag1 story-flag-clear  wt8f-1-flag2 story-flag-clear
wt8f-1-flag3 story-flag-clear  wt8f-1-flag4 story-flag-clear  room-afresh
testing coming in: the models whole (worn 3 each), the light on the water, the diorama's figures not yet there
T{ [: 2 part?  4 part?  6 part?  0 part?  stage-1 @  stage-3 @  water-fx live-effect nip ;] in-place -> -1 -1 -1 0 3 3 -1 }T
testing the walls' remark; the diorama's
wall-2-area -58e 54e aim
T{ said -> wall-msg }T
T{ diorama-area 180 at  said -> diorama-msg }T
testing a model worn through by her blow (a strong one): down - its debris, the wall's other part
wall-1-area 0 at
T{ 3e 621e 79e blow-at  [: models  ['] model-1 running? ;] in-place -> -1 }T
T{ [: [: ['] model-1 running? 0= ;] in-place ;] 300 soon  [: stage-1 @  2 part?  3 part? ;] in-place  wt8f-1-flag1 story-flag? -> -1 0 0 -1 -1 }T
testing the aiming cursor: from (246, 242) right 4, up 4; at the edge it stays
[: 246 cursor-x !  242 cursor-y ! ;] in-place  key: Right true key-hold  key: Up true key-hold
T{ [: cursor-step  cursor-x @  cursor-y @ ;] in-place -> 250 238 }T
[: 490 cursor-x ! ;] in-place
T{ [: cursor-step  cursor-x @ ;] in-place -> 492 }T
key: Right false key-hold  key: Up false key-hold  5 run-frames
testing the Bronze Figurine at the diorama: it won't go - put away (triangle)
diorama-area 180 at
T{ 3 use-on-place  [: [: miniature? @ 0=  ['] sets-figure running? and ;] in-place ;] 200 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
60 run-frames  triangle press
T{ [: [: ['] sets-figure running? 0= ;] in-place ;] 300 soon  [: model-set @ ;] in-place  world-frozen state-flag? -> -1 0 0 }T
testing the Wooden Miniature in its place: Lorenzo comes (the door shut behind, the scene's flag)
wooden-miniature [: 1 give-item ;] in-place  5 run-frames  wooden-miniature items:items-of before !
diorama-area 180 at
T{ 4 use-on-place  [: [: ['] sets-figure running? ;] in-place ;] 200 soon -> -1 }T
60 run-frames  [: 200 cursor-x !  200 cursor-y ! ;] in-place  cross press
T{ [: movie-skipped state-flag-set  $3F resident-flag? ;] 3000 soon  wooden-miniature items:items-of  scene-5-pending state-flag?  stairs-door doors:door-locked? -> -1 before @ 1- -1 -1 }T
300 play-on
testing the locked door then: "It's locked!"
T{ [: 0 offer-xt !  0 offer-msg !  2 request-now !  spots  offer-msg @ ;] in-place  locked-msg = -> -1 }T
testing Hewie hides in the hole she signs at (lying in wait); out at her "come back"
scene-5-pending state-flag-clear  hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames
5 -6e -12e aim
T{ [: 0 fiona-cmd-now !  sign-to-hole  ['] hides running?  hole @ ;] in-place -> -1 0 }T
T{ [: [: lying-down @ ;] in-place ;] 900 soon -> -1 }T
T{ [: come-back-cmd started-now !  lying-in-wait  called-out @ ;] in-place -> -1 }T
T{ [: [: ['] hides running? 0= ;] in-place ;] 900 soon  [: lying-down @ ;] in-place -> -1 0 }T
testing leaving with Hewie on the holes' floor: he's put out by the hole he went for
[: true hole-way  hewie $D0 5 -0.12e 8.95e place-at ;] in-place  10 run-frames
T{ [: hewie-in-hole @ ;] in-place  water-tower-1f-1 room!  5 run-frames  [: self body-tri ;] game:hewie enter execute leave-actor -> -1 $33 }T
room-afresh  game:hewie send join-fiona  60 run-frames
testing Hewie at the drop: his bark
T{ [: ['] barks-at-drop start drop ;] in-place  [: doing-busy? 0= ;] 600 soon -> -1 }T
testing the final scenes: Riccardo falls - the doors opened (the way down locked), the files
[: ['] final-scene fiona start-holding drop ;] in-place
T{ [: movie-skipped state-flag-set  $41 resident-flag? ;] 4000 soon  c7-second-event story-flag?  top-door doors:door-locked?  stairs-door doors:door-locked?  down-door doors:door-locked? -> -1 -1 0 0 -1 }T
300 play-on  wt8f-1-flag1 story-flag-set
testing after it: the drop's remarks in turn; the hole's; the diorama's (with the memo, without)
T{ wall-1-area 3e 79e aim  said  wall-1-area 3e 79e aim  said -> drop-msg path-msg }T
T{ $F -6e -12e aim  said -> hole-msg }T
folded-memo [: 1 give-item ;] in-place  5 run-frames
T{ diorama-area 180 at  said -> magisterium-msg }T
[: folded-memo spend ;] in-place  5 run-frames
T{ diorama-area 180 at  said -> switch-msg }T
testing the Bronze Figurine at it again: her remark
T{ $10 use-on-place  shown-soon -> again-msg }T
300 read-on
testing coming up after the garden's scene: Lorenzo takes Riccardo's Azoth - on by the forced way
fg-3-flag3 story-flag-set  wt8f-1-flag2 story-flag-clear  room-afresh  0 0e 620e -40e put  0 0e 620e -20e put
T{ [: movie-skipped state-flag-set  $42 resident-flag? ;] 3000 soon  wt8f-1-flag2 story-flag? -> -1 -1 }T
300 play-on
testing the movie replayed (the extras, 56): played, then back by the forced way
56 replay-var var!  events-held state-flag-set  water-tower-8f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 900 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
300 play-on
testing the movie replayed (the extras, 57): played, then back by the forced way
57 replay-var var!  events-held state-flag-set  water-tower-8f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 900 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
300 play-on
testing the movie replayed (the extras, 58): played, then back by the forced way
58 replay-var var!  events-held state-flag-set  water-tower-8f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 900 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
300 play-on
testing the movie replayed (the extras, 59): played, then back by the forced way
59 replay-var var!  events-held state-flag-set  water-tower-8f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 900 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 3000 soon -> -1 }T
300 play-on
test-summary
