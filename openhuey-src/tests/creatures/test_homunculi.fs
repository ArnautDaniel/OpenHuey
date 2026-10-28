\ The homunculi (C3: creatures/homunculus.fs): placed by the story in slots 7..9, resting until
\ they notice Fiona, coming after her, closing in to grab her, struck by her blows, leaving with
\ no strength left, dropped from a height, kept in the progress. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/creatures/test_homunculi.fs
IN: test-homunculi
USING: tester engine game-state actors messages room-names flag-names facts paths saves game debug fiona.state creatures creatures.trip creatures.homunculus ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room-actor ( xt -- )  room-actor enter  execute  leave-actor ;
: h7 ( -- id )  game:creatures enter  7 slot@  leave-actor ;
: of7 ( xt -- x )  h7 enter  execute  leave-actor ;
: d-her ( -- n )  h7 body-at  game:fiona body-at vec-dist-xz fround f>s ;
create centre 12 allot
: tri-away ( d -- tri )   \ a triangle about d from her
    nav-tris 0 do
        i tri-center centre vec!  centre game:fiona body-at vec-dist-xz  dup s>f f- fabs 3e f<
        if  drop i unloop exit  then
    loop  drop -1 ;
variable p-tri  variable p-sight  fvariable p-height
: place-one ( tri sight -- ) ( F: height -- )   \ kind $80 in her room, facing 0
    p-sight !  p-tri !  p-height f!
    [: room-id 2 p-tri @ $80 p-sight @ -1 0 p-height f@ f>cell game:creatures send creature-place ;] in-room-actor  deliver  1 run-frames ;
: soon ( xt n -- flag )  0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;
60 run-frames
front-garden-2 -1 game:rooms send go-to-room  5 run-frames
0 game:fiona send costume  deliver  2 run-frames   \ (her clothes: the slip has no falls)
40 tri-away constant far-tri

testing placed: a homunculus in slot 7, its model shown, resting until it notices her
0e far-tri 1 place-one   \ (sight 10)
T{ h7 alive?  [: awake @  model @ actor act.visible l@  strength @ ;] of7 -> -1 0 1 5 }T
30 run-frames
T{ [: awake @ ;] of7 -> 0 }T
T{ progress pr.creatures 7 /kept-creature * + kc.kind l@ -> $80 }T

testing held off no more once settled (cr19_settle: +0x6B kept only once it has held her)
T{ [: true held-off !  settle  held-off @  true grabbed !  true held-off !  settle  held-off @  false grabbed !  false held-off ! ;] of7 -> 0 -1 }T
testing seeing her (Actor_CanWalkBetween: both ways); from her place to itself, plainly
T{ game:fiona body-tri game:fiona body-at  game:fiona body-tri game:fiona body-at  0 can-walk-between? -> -1 }T
testing its masks: $04020028; kinds $88 / $89 till they come to their spot none ($40080 its paths)
T{ [: to-spot-free @  path-mask  stand-mask ;] of7 -> 0 $04020028 $04020028 }T
testing with no triangle in the room being played: put somewhere at random on its level, off the doors' spots, watching (CreatureB_EnterRoom)
T{ [: 0 level !  -1 my-tri !  chasing want !  chasing running !  on-the-floor
      my-tri @ 0< 0=  my-tri @ off-the-doors?  my-tri @ nav-flags $04020028 and  running @ want @ ;] of7 -> -1 -1 0 0 0 }T
testing kept: leaving, not in use
T{ [: leaving want !  store-record  my-record kc.used l@  watching want !  store-record  my-record kc.used l@ ;] of7 -> 0 1 }T
2 game:creatures send creatures-clear  deliver  1 run-frames
testing kind $88 placed: blocking nothing, its paths $40080
2 game:creatures send creatures-clear  deliver  1 run-frames
: place-88 ( -- )  [: room-id 0 far-tri $88 1 -1 0 0e f>cell game:creatures send creature-place ;] in-room-actor  deliver  1 run-frames ;
place-88
T{ [: to-spot-free @  path-mask  stand-mask  k ;] of7 -> -1 $40080 0 8 }T
2 game:creatures send creatures-clear  deliver  1 run-frames
0e far-tri 1 place-one
testing her call while followed (danger 1): one 3..9 ahead of her - "go for it" ($2D); turned from it, "go there" ($23)
: code ( -- n )  game:fiona enter  1 fiona.state:her-danger !  0 fiona.commands:command-code  leave-actor ;
fvariable turned-by
: face ( deg -- )   \ (her where she is, facing the homunculus, turned by deg)
    s>f turned-by f!  game:fiona body-at vec@  game:fiona body-at h7 body-at vec-heading 180e f* pi f/  turned-by f@ f+
    debug:tp-facing  2 run-frames ;
2 game:creatures send creatures-clear  deliver  1 run-frames
0e 6 tri-away 1 place-one
T{ 0 face  code -> $2D }T
T{ 180 face  code -> $23 }T
2 game:creatures send creatures-clear  deliver  1 run-frames
0e far-tri 0 place-one

testing sight 0: it wakes, turns to her and comes; close, it closes in to grab her (move 8)
5 run-frames
T{ [: awake @ ;] of7 -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
d-her constant d0
T{ [: [: running @ 3 = ;] of7 ;] 900 soon -> -1 }T
T{ d-her d0 < -> -1 }T
T{ [: [: move-mode @ 8 = ;] of7 ;] 300 soon  [: 8 story.kit:creature-moving? ;] in-room-actor -> -1 -1 }T
: near-it? ( me it -- flag )   \ (touching: its own radius and height the margins - the rooms' cond 0x58)
    {: me it | r h :}  it body-dims fround f>s to h  fround f>s to r  me it r h story.base:touching? ;
T{ [: game:fiona h7 near-it? ;] 300 soon -> -1 }T   \ (the story's 0x58: touching it)

: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
testing it holds her down (reaction $12); its grab holds on, chattering, until the hold runs out
T{ [: [: fiona.state:her-mode @ 4 =  fiona.state:her-sub @ $12 = and ;] her ;] 400 soon -> -1 }T
T{ [: [: running @ 3 =  step @ 2 = and ;] of7 ;] 200 soon -> -1 }T
T{ [: [: fiona.state:her-mode @ 0= ;] her ;] 400 soon -> -1 }T
testing let go, it lets go and leaves
T{ [: h7 alive? 0= ;] 600 soon -> -1 }T
0e far-tri 0 place-one  5 run-frames

testing a kick that reaches it: it reels; with no strength left it leaves, and its slot is free
: kick-it ( -- )   \ (Fiona's blow 99, damage 5, at it)
    99 5  h7 body-at vec@ f>cell >r f>cell >r f>cell r> r>  2e f>cell  broadcast fiona-blow  deliver ;
kick-it  2 run-frames
T{ [: running @  strength @ ;] of7 -> 2 0 }T
T{ [: h7 alive? 0= ;] 600 soon -> -1 }T
T{ progress pr.creatures 7 /kept-creature * + kc.used l@ -> 0 }T

testing dropped from 20 up: it falls, lands, and is on its way
20e far-tri 0 place-one
T{ [: running @ ;] of7 -> $B }T
T{ [: [: running @ $B <> ;] of7 ;] 300 soon -> -1 }T

testing kept: saved, cleared, back from a loaded game
: yaw7 ( -- deg )  h7 body-yaw 180e f* pi f/ fround f>s ;
2 game:creatures send creatures-clear  deliver  1 run-frames
0e far-tri 1 place-one   \ (resting: not turning)
[: 3 strength !  1.0e body-turn ;] of7  1 run-frames
yaw7 constant saved-yaw
' saves:quick-save game:saves enter execute leave-actor  deliver  2 run-frames
2 game:creatures send creatures-clear  deliver  1 run-frames
T{ h7 alive? -> 0 }T
' saves:quick-load game:saves enter execute leave-actor  deliver  1 run-frames
T{ h7 alive?  [: kind @ awake @  strength @ ;] of7  yaw7 saved-yaw - abs 2 < -> -1 $80 0 3 -1 }T
testing she leaves: it makes for her exit's far spot, walking there off screen - its body moving in the room it was left in (CreatureB_FionaLeft, _Travel)
2 game:creatures send creatures-clear  deliver  1 run-frames
1 constant way-out   \ (front garden 2's exit 1: an open door, on her level)
room-id constant left-room
create far-at 12 allot  create in-at 12 allot  create was-at 12 allot
way-out 2 exit-spot drop far-at vec!  way-out 1 exit-spot drop in-at vec!
: inward ( F: -- x y z )   \ (12 further in from the exit's near spot: off its area)
    in-at sf@ fdup far-at sf@ f- 0.5e f* f+  in-at 4 + sf@  in-at 8 + sf@ fdup far-at 8 + sf@ f- 0.5e f* f+ ;
inward 0e debug:tp-facing  2 run-frames
0e far-tri 0 place-one  5 run-frames
: moved ( -- n )  h7 body-at was-at vec-dist-xz fround f>s ;
: walking? ( -- flag )  [: trip creatures.trip:t.walking @ ;] of7 ;
way-out game:rooms send go-through  deliver  3 run-frames
T{ room-id left-room <>  [: my-room @ ;] of7 left-room =  h7 body?  h7 body-room left-room =  walking? -> -1 -1 -1 -1 -1 }T
T{ [: trip creatures.trip:t.last @ ;] of7  left-room way-out room-exit-door = -> -1 }T
was-at h7 body-at vec-copy  30 run-frames
T{ moved 9 12 within -> -1 }T   \ (0.35 a frame)
testing trodden on there off screen, a magnesia goes off: it is struck, facing heading 0 (ThingShared_StompStalkers)
was-at h7 body-at vec-copy
2 left-room -1  was-at vec@  0e 0e 0e  thing-new constant trodden
2 run-frames
T{ trodden thing-used?  [: blow-pending @  blow-damage @  blow-yaw f@ f0= ;] of7 -> 0 -1 15 -1 }T
[: 0 blow-pending ! ;] of7
testing a magnesia going off in that room strikes it there, taken once it is in play: it reels (ThingShared_ReachStalkers)
was-at h7 body-at vec-copy
2 left-room -1  was-at vec@ fswap 5e f+ fswap  0e 0e 0e  thing-new constant mag
900 mag thing-age!  2 run-frames
T{ mag thing-used?  [: blow-pending @  blow-damage @  striker @ ;] of7 -> 0 -1 15 -1 }T
T{ [: [: my-room @ room-id = ;] of7 ;] 1500 soon -> -1 }T
1 run-frames
T{ h7 body-room room-id =  [: running @  blow-pending @ ;] of7 -> -1 2 0 }T   \ (it came in by her door, struck)
test-summary
