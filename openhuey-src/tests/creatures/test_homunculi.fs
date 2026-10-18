\ The homunculi (C3: creatures/homunculus.fs): placed by the story in slots 7..9, resting until
\ they notice Fiona, coming after her, closing in to grab her, struck by her blows, leaving with
\ no strength left, dropped from a height, kept in the progress. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/creatures/test_homunculi.fs
IN: test-homunculi
USING: tester engine game-state actors messages room-names flag-names facts paths saves game debug fiona.state story.state story.words creatures creatures.homunculus ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- )  room-actor enter  execute  leave-actor ;
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
    [: room-id 2 p-tri @ $80 p-sight @ -1 0 p-height f@ creature-place ;] in-room  deliver  1 frames ;
: soon ( xt n -- flag )  0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;
60 frames
front-garden-2 -1 game:rooms send go-to-room  5 frames
0 game:fiona send costume  deliver  2 frames   \ (her clothes: the slip has no falls)
40 tri-away constant far-tri

testing placed: a homunculus in slot 7, its model shown, resting until it notices her
0e far-tri 1 place-one   \ (sight 10)
T{ h7 alive?  [: awake @  model @ actor act.visible l@  strength @ ;] of7 -> -1 0 1 5 }T
30 frames
T{ [: awake @ ;] of7 -> 0 }T
T{ progress pr.creatures 7 /kept-creature * + kc.kind l@ -> $80 }T

testing her call while followed (danger 1): one 3..9 ahead of her - "go for it" ($2D); turned from it, "go there" ($23)
: code ( -- n )  game:fiona enter  1 fiona.state:her-danger !  0 fiona.commands:command-code  leave-actor ;
fvariable turned-by
: face ( deg -- )   \ (her where she is, facing the homunculus, turned by deg)
    s>f turned-by f!  game:fiona body-at vec@  game:fiona body-at h7 body-at vec-heading 180e f* pi f/  turned-by f@ f+
    debug:tp-facing  2 frames ;
2 game:creatures send creatures-clear  deliver  1 frames
0e 6 tri-away 1 place-one
T{ 0 face  code -> $2D }T
T{ 180 face  code -> $23 }T
2 game:creatures send creatures-clear  deliver  1 frames
0e far-tri 0 place-one

testing sight 0: it wakes, turns to her and comes; close, it closes in to grab her (move 8)
5 frames
T{ [: awake @ ;] of7 -> -1 }T
d-her constant d0
T{ [: [: running @ 3 = ;] of7 ;] 900 soon -> -1 }T
T{ d-her d0 < -> -1 }T
T{ [: [: move-mode @ 8 = ;] of7 ;] 300 soon  [: 8 creature-action? ;] in-room -> -1 -1 }T
T{ [: game:fiona h7 near-it? ;] 300 soon -> -1 }T   \ (the story's 0x58: touching it)

: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
testing it holds her down (reaction $12); its grab holds on, chattering, until the hold runs out
T{ [: [: fiona.state:her-mode @ 4 =  fiona.state:her-sub @ $12 = and ;] her ;] 400 soon -> -1 }T
T{ [: [: running @ 3 =  step @ 2 = and ;] of7 ;] 200 soon -> -1 }T
T{ [: [: fiona.state:her-mode @ 0= ;] her ;] 400 soon -> -1 }T
testing let go, it lets go and leaves
T{ [: h7 alive? 0= ;] 600 soon -> -1 }T
0e far-tri 0 place-one  5 frames

testing a kick that reaches it: it reels; with no strength left it leaves, and its slot is free
: kick-it ( -- )   \ (Fiona's blow 99, damage 5, at it)
    99 5  h7 body-at vec@ f>cell >r f>cell >r f>cell r> r>  2e f>cell  broadcast fiona-blow  deliver ;
kick-it  2 frames
T{ [: running @  strength @ ;] of7 -> 2 0 }T
T{ [: h7 alive? 0= ;] 600 soon -> -1 }T
T{ progress pr.creatures 7 /kept-creature * + kc.used l@ -> 0 }T

testing dropped from 20 up: it falls, lands, and is on its way
20e far-tri 0 place-one
T{ [: running @ ;] of7 -> $B }T
T{ [: [: running @ $B <> ;] of7 ;] 300 soon -> -1 }T

testing kept: saved, cleared, back from a loaded game
' saves:quick-save game:saves enter execute leave-actor  deliver  2 frames
2 game:creatures send creatures-clear  deliver  1 frames
T{ h7 alive? -> 0 }T
' saves:quick-load game:saves enter execute leave-actor  deliver  20 frames
T{ h7 alive?  [: kind @ awake @ ;] of7 -> -1 $80 -1 }T
test-summary
