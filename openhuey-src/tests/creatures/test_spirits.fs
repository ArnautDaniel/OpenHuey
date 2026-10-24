\ The spirits (C1, C2: creatures.fs, creatures/spirit.fs): placed by the story, waiting for Fiona
\ to come within reach, coming after her, vanishing when they touch her or their time runs out,
\ hanging about her while she hides, following her out, kept in the progress. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/creatures/test_spirits.fs
IN: test-spirits
USING: tester engine game-state actors messages room-names flag-names facts paths saves game fiona.state story.state story.words creatures creatures.trip creatures.state creatures.spirit ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- )  room-actor enter  execute  leave-actor ;
: spirit0 ( -- id )  game:creatures enter  0 slot@  leave-actor ;
: of0 ( xt -- x )  spirit0 enter  execute  leave-actor ;
: d-her ( -- n )  spirit0 body-at  game:fiona body-at vec-dist-xz fround f>s ;
\ a triangle about `d` from her, reached straight from hers
create centre 12 allot
: tri-away ( d -- tri )   \ a triangle about d from her
    nav-tris 0 do
        i tri-center centre vec!  centre game:fiona body-at vec-dist-xz  dup s>f f- fabs 3e f<
        if  drop i unloop exit  then
    loop  drop -1 ;
variable p-tri  variable p-kind  variable p-reach
: place-spirit ( tri kind reach -- )   \ in her room, level 2, a free slot
    p-reach !  p-kind !  p-tri !
    [: room-id 2 p-tri @ p-kind @ p-reach @ -1 0 0e creature-place ;] in-room  deliver  1 frames ;
60 frames
front-garden-2 -1 game:rooms send go-to-room  5 frames
0 game:fiona send costume  deliver  2 frames   \ (her clothes: the slip has no falls)

testing placed: a spirit in slot 0, on its triangle, waiting for her within its reach
30 tri-away constant far-tri
far-tri 4 1 place-spirit   \ (kind 4: blue, 2 minutes, 0.25 a frame; reach 10)
T{ spirit0 alive?  [: come @ ;] of0 -> -1 0 }T
T{ d-her 25 > -> -1 }T
30 frames
T{ [: come @ time @ ;] of0  d-her 25 > -> 0 0 -1 }T
T{ progress pr.creatures kc.used l@  progress pr.creatures kc.kind l@ -> 1 4 }T

testing its kind: speed, time, colour, straight-only
T{ [: speed fround f>s  lifetime  red?  straight-only? ;] of0 -> 0 3600 0 0 }T
T{ [: $13 kind !  lifetime  red?  $24 kind !  lifetime  odd-one?  4 kind ! ;] of0 -> 1800 -1 1800 -1 }T

testing reach 0: it comes at once, straight at her, and touching her it vanishes
1 game:creatures send creatures-clear  deliver  1 frames
far-tri 4 0 place-spirit
d-her constant d0
20 frames
T{ [: come @ ;] of0  d-her d0 < -> -1 -1 }T
: gone-in? ( n -- flag )  0 ?do  game-tick  spirit0 alive? 0= if  true unloop exit  then  loop  false ;
: touched? ( n -- flag )  0 ?do  game-tick  [: vanishing @ ;] of0 if  true unloop exit  then  loop  false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
testing touched, she falls (reaction $13)
T{ 600 touched? -> -1 }T
1 frames
T{ [: fiona.state:her-mode @ fiona.state:her-sub @ ;] her -> 4 $13 }T
T{ 100 gone-in? -> -1 }T
T{ progress pr.creatures kc.used l@ -> 0 }T
testing she gets up again
: up? ( n -- flag )  0 ?do  game-tick  [: fiona.state:her-mode @ 0= ;] her if  true unloop exit  then  loop  false ;
T{ 400 up? -> -1 }T

testing its time out: it fades away where it is
far-tri 4 0 place-spirit
[: lifetime 2 - time ! ;] of0
T{ 5 frames  [: vanishing @ seen @ ;] of0 -> -1 0 }T
T{ 60 gone-in? -> -1 }T

testing while she hides it hangs about her (the story asks: 0x53 2)
far-tri 4 0 place-spirit
fiona-hidden state-flag-set  5 frames
T{ [: move-mode @ ;] of0  [: 2 creature-action? ;] in-room -> 2 -1 }T
fiona-hidden state-flag-clear  5 frames
T{ [: 2 creature-action? ;] in-room -> 0 }T

testing cleared by the story
T{ 1 game:creatures send creatures-clear  deliver  1 frames  spirit0 alive? -> 0 }T
T{ progress pr.creatures kc.used l@ -> 0 }T

testing left behind, it comes after her room by room, and comes in by a door
far-tri 4 0 place-spirit  10 frames
front-garden-2 constant was-room
front-garden-3 -1 game:rooms send go-to-room  10 frames
T{ [: my-room @ in-play? ;] of0  was-room swap -> was-room was-room 0 }T
: arrives? ( n -- flag )  0 ?do  game-tick  [: in-play? ;] of0 if  true unloop exit  then  loop  false ;
T{ 3000 arrives? -> -1 }T
T{ spirit0 body?  spirit0 body-room room-id = -> -1 -1 }T

testing put at random: off the doors' spots (CreatureA_PlaceRandom)
T{ [: true  50 0 do  anywhere dup 0< if  drop  else  off-the-doors? and  then  loop ;] of0 -> -1 }T
testing kept: saved, cleared, and back from a loaded game
' saves:quick-save game:saves enter execute leave-actor  deliver  2 frames
1 game:creatures send creatures-clear  deliver  1 frames
T{ spirit0 alive? -> 0 }T
' saves:quick-load game:saves enter execute leave-actor  deliver  20 frames
T{ spirit0 alive?  [: kind @ come @ ;] of0 -> -1 4 -1 }T
testing by a door played open (the first door of her room): standing open, it waits there till the door is still, then carries on; shut, it goes through after her
fvariable fx  fvariable fy  fvariable fz
: soon ( xt n -- flag )  0 ?do  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;
front-garden-2 -1 game:rooms send go-to-room  5 frames
: first-door ( -- exit )  8 0 do  i door-here? if  i unloop exit  then  loop  -1 ;
first-door constant ex
: in-its-area ( -- tri )   \ (a triangle whose centre is in the exit's area)
    nav-tris 0 do  i tri-center centre vec!  centre vec@ ex exit-area area-in? if  i unloop exit  then  loop  -1 ;
: door0 ( -- d )  room-id ex room-exit-door ;
: play-door ( -- )  [: ex 0 0 game:doors send use-door ;] in-room  deliver ;
: at-door ( -- n )  [: trip creatures.trip:t.at-door @ ;] of0 ;
door0 true game:doors send door-opened  deliver  1 frames
1 game:creatures send creatures-clear  deliver  1 frames
in-its-area 4 0 place-spirit
play-door  2 frames
T{ ex door-animating?  at-door -> -1 1 }T
spirit0 body-at vec@ fz f! fy f! fx f!
10 frames
T{ spirit0 body-at vec@ fz f@ f= fdrop fx f@ f= -> -1 -1 }T   \ (still)
T{ [: at-door 0= ;] 300 soon  ex door-animating? -> -1 0 }T
door0 false game:doors send door-opened  deliver  1 frames
1 game:creatures send creatures-clear  deliver  1 frames
in-its-area 4 0 place-spirit
play-door  2 frames
T{ at-door  [: my-room @ ;] of0 room-id <> -> 2 -1 }T

testing the story's count (0x99): five spirits scattered as a room comes in - each in a room next to hers, out and resting
: spirit ( i -- id )  game:creatures enter  slot@  leave-actor ;
: next-to-hers? ( room -- flag )  false  8 0 do  over room-id i room-exit-leads drop = or  loop  nip ;
0 game:creatures send creatures-clear  deliver  1 frames
5 0 creature-count  deliver
front-garden-2 -1 game:rooms send go-to-room  5 frames
: scattered? ( i -- flag )
    spirit dup alive? 0= if  drop false exit  then
    enter  my-room @ dup room-id <> swap next-to-hers? and  out @ and  come @ 0= and  rest @ 0> and  leave-actor ;
T{ 0 scattered?  1 scattered?  2 scattered?  3 scattered?  4 scattered?  5 spirit alive? -> -1 -1 -1 -1 -1 0 }T
testing one gone, the next frame of play another takes its slot (SceneGame_RoomCreatures each frame)
T{ 0 game:creatures send creature-clear  deliver  0 spirit alive? -> 0 }T
2 frames
T{ 0 scattered? -> -1 }T
testing off screen with company: 2/3 its pace for the first leg only; held while the stalkers stay
: s1 ( xt -- )  1 spirit enter  execute  leave-actor ;
: leg ( -- n )  trip t.leg sf@ 100e f* fround f>s ;
T{ [: 0 rest !  true come !  true company !  trip trip-reset  travel  trip t.n @ 0> ;] s1 -> -1 }T
T{ [: 10e trip t.leg sf!  travel  10e trip t.leg sf@ f-  speed 1.5e f/ f- fabs 0.0001e f<  company @ ;] s1 -> -1 -1 }T
T{ [: 0.01e trip t.leg sf!  travel  company @ ;] s1 -> 0 }T
T{ [: 10e trip t.leg sf!  stalkers-stay state-flag-set  travel  stalkers-stay state-flag-clear  leg ;] s1 -> 1000 }T
0 game:creatures send creatures-clear  deliver  creatures-on state-flag-clear  1 frames

test-summary
